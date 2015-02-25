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

#include "objectdepsrefswidget.h"

ObjectDepsRefsWidget::ObjectDepsRefsWidget(QWidget *parent): BaseObjectWidget(parent)
{
	Ui_ObjectDepsRefsWidget::setupUi(this);
	configureFormLayout(objectdepsrefs_grid, BASE_OBJECT);

  model_wgt=nullptr;
  parent_form->setWindowTitle(trUtf8("Object's dependencies & references"));
	parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
  parent_form->setMinimumSize(580, 350);
  alert_frm->setVisible(false);

	connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
  connect(exc_ind_deps_chk,	SIGNAL(toggled(bool)), this, SLOT(updateObjectTables(void)));
  connect(dependences_tbw, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(handleItemSelection(QTableWidgetItem*)));
  connect(references_tbw, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(handleItemSelection(QTableWidgetItem*)));
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

  updateObjectTables();
}

void ObjectDepsRefsWidget::setAttributes(ModelWidget *model_wgt, BaseObject *object, BaseObject *parent_obj)
{
  if(!model_wgt)
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  this->model_wgt=model_wgt;
  setAttributes(model_wgt->getDatabaseModel(), object, parent_obj);
}

void ObjectDepsRefsWidget::clearTables(void)
{
  while(dependences_tbw->rowCount() > 0)
    dependences_tbw->removeRow(0);

  while(references_tbw->rowCount() > 0)
    references_tbw->removeRow(0);
}

void ObjectDepsRefsWidget::hideEvent(QHideEvent *event)
{
	tabWidget->setCurrentIndex(0);
  model_wgt=nullptr;
  references_tbw->setEnabled(true);
  dependences_tbw->setEnabled(true);
  exc_ind_deps_chk->setEnabled(true);
  alert_frm->setVisible(false);

  clearTables();
	BaseObjectWidget::hideEvent(event);
}

void ObjectDepsRefsWidget::updateObjectTables(void)
{
	vector<BaseObject *> objs;
  model->getObjectDependecies(object, objs, !exc_ind_deps_chk->isChecked());

	/* As the list of dependencies include the this->object itself is necessary
	to remove only for semantics reasons */
	objs.erase(std::find(objs.begin(), objs.end(), this->object));
  ObjectFinderWidget::updateObjectTable(dependences_tbw, objs);

  model->getObjectReferences(object, objs);
  ObjectFinderWidget::updateObjectTable(references_tbw, objs);

  references_tbw->resizeColumnsToContents();
  dependences_tbw->resizeColumnsToContents();
}

void ObjectDepsRefsWidget::handleItemSelection(QTableWidgetItem *item)
{
  BaseObject *sel_obj=nullptr, *parent=nullptr;
  Table *parent_tab=nullptr;
  View *parent_view=nullptr;

  sel_obj=reinterpret_cast<BaseObject*>(item->data(Qt::UserRole).value<void *>());

  if(sel_obj)
  {
    if(TableObject::isTableObject(sel_obj->getObjectType()))
      parent=dynamic_cast<TableObject *>(sel_obj)->getParentTable();

    model_wgt->showObjectForm(sel_obj->getObjectType(), sel_obj, parent);
    clearTables();

    if(TableObject::isTableObject(this->object->getObjectType()))
    {
      parent=dynamic_cast<TableObject *>(this->object)->getParentTable();

      if(parent->getObjectType()==OBJ_TABLE)
        parent_tab=dynamic_cast<Table *>(parent);
      else
        parent_view=dynamic_cast<View *>(parent);
    }

    if((parent_tab && parent_tab->getObjectIndex(this->object) >= 0) ||
       (parent_view && parent_view->getObjectIndex(this->object) >= 0) ||
       (model->getObjectIndex(this->object) >= 0))
    {
      updateObjectTables();
    }
    else
    {
      references_tbw->setEnabled(false);
      dependences_tbw->setEnabled(false);
      exc_ind_deps_chk->setEnabled(false);
      alert_frm->setVisible(true);
    }
  }
}
