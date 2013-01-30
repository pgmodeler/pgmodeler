#include "quickrenamewidget.h"
#include "messagebox.h"

extern MessageBox *caixa_msg;

QuickRenameWidget::QuickRenameWidget(QWidget * parent) : QDialog(parent)
{
	object=NULL;
	op_list=NULL;
	model=NULL;

	setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

	connect(new_name_edt, SIGNAL(returnPressed(void)), this, SLOT(applyRenaming(void)));
	connect(apply_tb, SIGNAL(clicked(void)), this, SLOT(applyRenaming(void)));
	connect(cancel_tb, SIGNAL(clicked(void)), this, SLOT(reject(void)));
}

void QuickRenameWidget::setAttributes(BaseObject *object, DatabaseModel *model, OperationList *op_list)
{
	if(!object || !op_list)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->adjustSize();
	this->object=object;
	this->op_list=op_list;
	this->model=model;

	obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + object->getSchemaName() + QString(".png")));
	obj_icon_lbl->setToolTip(object->getTypeName());

	obj_name_lbl->setText(QString::fromUtf8(object->getName()));
	new_name_edt->setText(QString::fromUtf8(object->getName()));
}

void QuickRenameWidget::exec(void)
{
	if(object && op_list)
		QDialog::exec();
}

void QuickRenameWidget::hideEvent(QHideEvent *)
{
	object=NULL;
	op_list=NULL;
	model=NULL;
	new_name_edt->clear();
	obj_name_lbl->clear();
}

void QuickRenameWidget::applyRenaming(void)
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
			BaseObject *aux_obj=NULL, *parent_obj=NULL;
			QString fmt_name;

			obj_type=object->getObjectType();

			if(obj_type!=OBJ_DATABASE)
			{
				//Register the object on operations list before the modification
				op_list->registerObject(object, Operation::OBJECT_MODIFIED, -1, (tab_obj ? tab_obj->getParentTable() : NULL));

				//Format the object name to check if it will have a conflicting name
				fmt_name=BaseObject::formatName(new_name_edt->text().toUtf8(), obj_type==OBJ_OPERATOR);

				if(object->getSchema())
					fmt_name=object->getSchema()->getName(true) + "." + fmt_name;

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
													.arg(QString::fromUtf8(fmt_name))
													.arg(QString::fromUtf8(object->getTypeName()))
													.arg(QString::fromUtf8(parent_obj->getName(true)))
													.arg(QString::fromUtf8(parent_obj->getTypeName())),
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
				else
					tab->setModified(true);

				dynamic_cast<Schema *>(tab->getSchema())->setModified(true);
			}
			else if(object->getObjectType()==OBJ_SCHEMA)
			{
				model->validateSchemaRenaming(dynamic_cast<Schema *>(object), obj_name_lbl->text().toUtf8());
				dynamic_cast<Schema *>(object)->setModified(true);
			}

			accept();
		}
	}
	catch(Exception &e)
	{
		if(obj_type!=OBJ_DATABASE)
			op_list->removeLastOperation();

		caixa_msg->show(e);
	}
}
