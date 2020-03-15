/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodeleruins.h"

ObjectRenameWidget::ObjectRenameWidget(QWidget * parent) : QDialog(parent)
{
	op_list = nullptr;
	model = nullptr;

	setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

	connect(new_name_edt, SIGNAL(returnPressed()), apply_tb, SLOT(click()));
	connect(apply_tb, SIGNAL(clicked()), this, SLOT(applyRenaming()));
	connect(cancel_tb, SIGNAL(clicked()), this, SLOT(reject()));

	connect(new_name_edt, &QLineEdit::textChanged, [&](){
		apply_tb->setEnabled(!new_name_edt->text().isEmpty());
	});
}

void ObjectRenameWidget::setAttributes(vector<BaseObject *> objs, DatabaseModel *model, OperationList *op_list)
{
	TableObject *tab_obj = nullptr;

	for(auto &obj : objs)
	{
		tab_obj = dynamic_cast<TableObject *>(obj);

		if(obj->isSystemObject())
				throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
												.arg(obj->getName()).arg(obj->getTypeName()),
												ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(tab_obj && tab_obj->isAddedByRelationship())
				throw Exception(Exception::getErrorMessage(ErrorCode::OprRelationshipAddedObject)
												.arg(tab_obj->getName())
												.arg(tab_obj->getTypeName()),
												ErrorCode::OprRelationshipAddedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	if(objs.size() == 1)
	{
		obj_icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(objs[0]->getSchemaName())));
		obj_icon_lbl->setToolTip(objs[0]->getTypeName());
		obj_name_lbl->setText(objs[0]->getName());
	}
	else
	{
		obj_icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("seltodos")));
		obj_icon_lbl->setToolTip("");
		rename_lbl->setText(tr("Rename <strong>%1</strong> object(s) to:").arg(objs.size()));
		to_lbl->setVisible(false);
		obj_name_lbl->setVisible(false);
	}

	adjustSize();
	objects = objs;
	this->op_list = op_list;
	this->model = model;
}

int ObjectRenameWidget::exec()
{
	if(!objects.empty() && op_list)
		return QDialog::exec();

	return QDialog::Rejected;
}

void ObjectRenameWidget::hideEvent(QHideEvent *)
{
	op_list = nullptr;
	model = nullptr;
	new_name_edt->clear();
	obj_name_lbl->clear();
}

void ObjectRenameWidget::applyRenaming()
{
	ObjectType obj_type = ObjectType::BaseObject;
	unsigned renamed_objs = 0;

	try
	{
		//Apply the new name only when its not empty and its differs from the original one
		if(!new_name_edt->text().isEmpty())
		{
			if(objects.size() > 1)
			{
				Messagebox msg_box;
				msg_box.show(tr("<strong>CAUTION:</strong> You're about to rename multiple objects at once! This operation may cause irreversible changes to other objects not necessarily selected. Do you really want to proceed?"),
										 Messagebox::AlertIcon, Messagebox::YesNoButtons);

				if(msg_box.result() == QDialog::Rejected)
					return;
			}

			map<unsigned, BaseObject *> sel_objs_map;
			map<unsigned, BaseObject *>::reverse_iterator itr;
			BaseGraphicObject *graph_obj = nullptr;
			TableObject *tab_obj = nullptr;
			QString fmt_name, new_name;
			vector<BaseObject *> ref_objs, obj_list;
			vector<TableObject *> tab_objs;
			map<ObjectType, vector<BaseObject *>> obj_map;
			BaseObject *object = nullptr;
			bool revalidate_rels = false;

			/* First we need to store object in a ordered map using the objects ids as key
			 * This is done because the renaming will be performed from the major id to the
			 * minor id. This will diminish the risk of permanent object invalidation of
			 * special objects (which reference relationship added fields)	*/
			for(auto &obj : objects)
				sel_objs_map[obj->getObjectId()] = obj;

			for(itr = sel_objs_map.rbegin(); itr != sel_objs_map.rend(); itr++)
			{
				object = itr->second;
				new_name = new_name_edt->text();
				obj_type = object->getObjectType();
				graph_obj = dynamic_cast<BaseGraphicObject *>(object);
				tab_obj = dynamic_cast<TableObject *>(object);

				if(obj_type != ObjectType::Database)
				{
					//Register the object on operations list before the modification
					op_list->registerObject(object, Operation::ObjectModified, -1, (tab_obj ? tab_obj->getParentTable() : nullptr));
					object->setName(new_name);

					//For table child object, generate an unique name among the other objects of the same type in the table
					if(tab_obj)
					{
						tab_objs = *dynamic_cast<PhysicalTable *>(tab_obj->getParentTable())->getObjectList(obj_type);
						new_name = PgModelerNs::generateUniqueName<TableObject>(object, tab_objs, false, "", false, true);
					}
					//For database child object, generate an unique name among the other objects of the same type in the database
					else
					{
						if(!BaseTable::isBaseTable(obj_type) && obj_map.count(obj_type) == 0)
							obj_map[obj_type] = *model->getObjectList(obj_type);
						else if(obj_map.count(ObjectType::BaseTable) == 0)
						{
								obj_map[ObjectType::BaseTable] = *model->getObjectList(ObjectType::Table);

								obj_map[ObjectType::BaseTable].insert(obj_map[ObjectType::BaseTable].end(),
																model->getObjectList(ObjectType::View)->begin(),
																model->getObjectList(ObjectType::View)->end());

								obj_map[ObjectType::BaseTable].insert(obj_map[ObjectType::BaseTable].end(),
																model->getObjectList(ObjectType::ForeignTable)->begin(),
																model->getObjectList(ObjectType::ForeignTable)->end());
						}

						new_name = PgModelerNs::generateUniqueName<BaseObject>(object,
																																	 BaseTable::isBaseTable(obj_type) ? obj_map[ObjectType::BaseTable] : obj_map[obj_type],
																																	 true, "", false, true);
					}
				}

				object->setName(new_name);

				//If the renamed object is a graphical one, set as modified to force its redraw
				if(object->getObjectType() == ObjectType::Schema)
				{
					model->validateSchemaRenaming(dynamic_cast<Schema *>(object), new_name);
					dynamic_cast<Schema *>(object)->setModified(true);
				}
				else if(graph_obj)
				{
					graph_obj->setModified(true);

					if(BaseTable::isBaseTable(graph_obj->getObjectType()))
						dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
				}
				else if(tab_obj)
				{
					BaseTable *base_tab = tab_obj->getParentTable();
					PhysicalTable *tab = dynamic_cast<PhysicalTable *>(base_tab);
					Column *col=dynamic_cast<Column *>(tab_obj);

					/* If the object is a column and some primary key on table is referencing it
					 * the relationships will be revalidated */
					if(col && tab)
						revalidate_rels = true;

					base_tab->setModified(true);
					dynamic_cast<Schema *>(base_tab->getSchema())->setModified(true);
				}

				Column *col = nullptr;
				model->getObjectReferences(object, ref_objs);

				for(auto &obj : ref_objs)
				{
					if(obj->getObjectType()==ObjectType::Column)
					{
						col=dynamic_cast<Column *>(obj);
						col->getParentTable()->setModified(true);
					}
				}

				renamed_objs++;
			}

			if(revalidate_rels)
				model->validateRelationships();

			model->setCodesInvalidated();
			accept();
		}
	}
	catch(Exception &e)
	{
		Messagebox msg_box;

		if(obj_type != ObjectType::Database)
			op_list->removeLastOperation();

		msg_box.show(e);

		if(renamed_objs > 0)
			accept();
		else
			reject();
	}
}
