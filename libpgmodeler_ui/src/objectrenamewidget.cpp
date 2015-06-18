/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "objectrenamewidget.h"

ObjectRenameWidget::ObjectRenameWidget(QWidget * parent) : QDialog(parent)
{
	object=nullptr;
	op_list=nullptr;
	model=nullptr;

	setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

	connect(new_name_edt, SIGNAL(returnPressed(void)), this, SLOT(applyRenaming(void)));
	connect(apply_tb, SIGNAL(clicked(void)), this, SLOT(applyRenaming(void)));
	connect(cancel_tb, SIGNAL(clicked(void)), this, SLOT(reject(void)));
}

void ObjectRenameWidget::setAttributes(BaseObject *object, DatabaseModel *model, OperationList *op_list)
{
	TableObject *tab_obj=dynamic_cast<TableObject *>(object);

	if(!object || !op_list)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(tab_obj && tab_obj->isAddedByRelationship())
		throw Exception(Exception::getErrorMessage(ERR_OPR_REL_INCL_OBJECT)
                    .arg(tab_obj->getName())
                    .arg(tab_obj->getTypeName())
										,ERR_OPR_REL_INCL_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->adjustSize();
	this->object=object;
	this->op_list=op_list;
	this->model=model;

	obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + object->getSchemaName() + QString(".png")));
	obj_icon_lbl->setToolTip(object->getTypeName());

  obj_name_lbl->setText(object->getName());
  new_name_edt->setText(object->getName());
}

int ObjectRenameWidget::exec(void)
{
	if(object && op_list)
		return(QDialog::exec());
	else
		return(QDialog::Rejected);
}

void ObjectRenameWidget::hideEvent(QHideEvent *)
{
	object=nullptr;
	op_list=nullptr;
	model=nullptr;
	new_name_edt->clear();
	obj_name_lbl->clear();
}

void ObjectRenameWidget::applyRenaming(void)
{
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		//Apply the new name only when its not empty and its differs from the original one
		if(!new_name_edt->text().isEmpty() &&
			 this->object->getName()!=new_name_edt->text())
		{
			BaseGraphicObject *obj_graph=dynamic_cast<BaseGraphicObject *>(object);
			TableObject *tab_obj=dynamic_cast<TableObject *>(object);
			BaseObject *aux_obj=nullptr, *parent_obj=nullptr;
			QString fmt_name;
			vector<BaseObject *> ref_objs;

			obj_type=object->getObjectType();

			if(obj_type!=OBJ_DATABASE)
			{
				//Register the object on operations list before the modification
				op_list->registerObject(object, Operation::OBJECT_MODIFIED, -1, (tab_obj ? tab_obj->getParentTable() : nullptr));

				//Format the object name to check if it will have a conflicting name
				fmt_name=BaseObject::formatName(new_name_edt->text().toUtf8(), obj_type==OBJ_OPERATOR);

				if(object->getSchema())
          fmt_name=object->getSchema()->getName(true) + QString(".") + fmt_name;

				//For table child object, check if there is another object with the same new name
				if(tab_obj)
				{
					parent_obj=tab_obj->getParentTable();
					aux_obj=dynamic_cast<Table *>(tab_obj->getParentTable())->getObject(fmt_name, obj_type);
				}
				//For database child object, check if there is another object with the same new name
				else
				{
					parent_obj=model;
					aux_obj=model->getObject(fmt_name, obj_type);
				}

				//Raises a error if another object is found
				if(aux_obj && aux_obj!=object)
				{
					throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                          .arg(fmt_name)
                          .arg(object->getTypeName())
                          .arg(parent_obj->getName(true))
                          .arg(parent_obj->getTypeName()),
													ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			object->setName(new_name_edt->text().toUtf8());

			//If the renamed object is a graphical one, set as modified to force its redraw
			if(obj_graph)
			{
				obj_graph->setModified(true);

				if(obj_graph->getObjectType()==OBJ_TABLE ||
					 obj_graph->getObjectType()==OBJ_VIEW)
				{
					dynamic_cast<Schema *>(obj_graph->getSchema())->setModified(true);
				}
			}
			else if(tab_obj)
			{
				Table *tab=dynamic_cast<Table *>(tab_obj->getParentTable());
				Column *col=dynamic_cast<Column *>(tab_obj);

				/* If the object is a column and some primary key on table is referencing it
			 the model relationship will be revalidated */
				if(col && tab->isConstraintRefColumn(col, ConstraintType::primary_key))
					model->validateRelationships();

				tab->setModified(true);
				tab->setCodeInvalidated(true);
				dynamic_cast<Schema *>(tab->getSchema())->setModified(true);
			}
			else if(object->getObjectType()==OBJ_SCHEMA)
			{
				model->validateSchemaRenaming(dynamic_cast<Schema *>(object), obj_name_lbl->text().toUtf8());
				dynamic_cast<Schema *>(object)->setModified(true);
			}

      Column *col=nullptr;
      model->getObjectReferences(object, ref_objs);

      for(auto &obj : ref_objs)
      {
        if(obj->getObjectType()==OBJ_COLUMN)
        {
          col=dynamic_cast<Column *>(obj);
          col->getParentTable()->setModified(true);
          col->setCodeInvalidated(true);
        }
        else
          obj->setCodeInvalidated(true);
      }


			accept();
		}
	}
	catch(Exception &e)
	{
		Messagebox msg_box;

		if(obj_type!=OBJ_DATABASE)
			op_list->removeLastOperation();

		msg_box.show(e);
	}
}
