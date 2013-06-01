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
	parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
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

	/* As the list of dependencies include the this->object itself is necessary
	to remove only for semantics reasons */
	objs.erase(std::find(objs.begin(), objs.end(), this->object));
	ObjectFinderWidget::updateObjectTable(dependences_tbw, objs);

	model->getObjectReferences(object, objs);
	ObjectFinderWidget::updateObjectTable(references_tbw, objs);

	references_tbw->resizeColumnsToContents();
	dependences_tbw->resizeColumnsToContents();
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
