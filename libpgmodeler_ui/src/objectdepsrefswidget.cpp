/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "objectdepsrefswidget.h"

ObjectDepsRefsWidget::ObjectDepsRefsWidget(QWidget *parent): BaseObjectWidget(parent)
{
	Ui_ObjectDepsRefsWidget::setupUi(this);
	configureFormLayout(objectdepsrefs_grid, BASE_OBJECT);

	parent_form->setWindowTitle(trUtf8("Dependences / References of Object"));
	parent_form->setButtonConfiguration(MessageBox::OK_BUTTON);
	parent_form->setMinimumSize(550, 300);

	connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
}

void ObjectDepsRefsWidget::setAttributes(DatabaseModel *model, BaseObject *object, BaseObject *parent_obj)
{
	vector<BaseObject *> objs;

	BaseObjectWidget::setAttributes(model, object, parent_obj);

	this->name_edt->setReadOnly(true);
	this->parent_form->apply_ok_btn->setEnabled(true);
	this->protected_obj_frm->setVisible(false);
	this->comment_edt->setVisible(false);
	this->comment_lbl->setVisible(false);

	obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
																	BaseObject::getSchemaName(object->getObjectType()) + QString(".png")));

	model->getObjectDependecies(object, objs);
	this->updateTableWidget(objs, dependences_tbw);

	model->getObjectReferences(object, objs);
	this->updateTableWidget(objs, references_tbw);
}

void ObjectDepsRefsWidget::hideEvent(QHideEvent *event)
{
	tabWidget->setCurrentIndex(0);

	while(dependences_tbw->rowCount() > 0)
		dependences_tbw->removeRow(0);

	while(references_tbw->rowCount() > 0)
		references_tbw->removeRow(0);

	BaseObjectWidget::hideEvent(event);
}

void ObjectDepsRefsWidget::updateTableWidget(vector<BaseObject *> &objs, QTableWidget *tab_wgt)
{
	int count, lin_idx, i;
	QTableWidgetItem *tab_item=NULL;
	BaseObject *parent_obj=NULL;

	count=objs.size();
	for(lin_idx=0, i=0; i < count; i++)
	{
		if(objs[i]!=this->object)
		{
			tab_wgt->insertRow(lin_idx);

			tab_item=new QTableWidgetItem;
			tab_item->setText(Utf8String::create(objs[i]->getName()));
			tab_item->setIcon(QPixmap(QString(":/icones/icones/") +
																BaseObject::getSchemaName(objs[i]->getObjectType())+ QString(".png")));
			tab_wgt->setItem(lin_idx, 0, tab_item);

			tab_item=new QTableWidgetItem;
			tab_item->setText(Utf8String::create(objs[i]->getTypeName()));
			tab_wgt->setItem(lin_idx, 1, tab_item);

			tab_item=new QTableWidgetItem;
			object=objs[i];
			if(dynamic_cast<TableObject *>(objs[i]))
				parent_obj=dynamic_cast<TableObject *>(objs[i])->getParentTable();
			else if(objs[i]->getSchema())
				parent_obj=objs[i]->getSchema();
			else
				parent_obj=this->model;

			tab_item->setText(Utf8String::create(parent_obj->getName()));
			tab_item->setIcon(QPixmap(QString(":/icones/icones/") +
																BaseObject::getSchemaName(parent_obj->getObjectType())+ QString(".png")));
			tab_wgt->setItem(lin_idx, 2, tab_item);

			tab_item=new QTableWidgetItem;
			tab_item->setText(Utf8String::create(parent_obj->getTypeName()));
			tab_wgt->setItem(lin_idx, 3, tab_item);

			lin_idx++;
		}
	}
}

