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

#include "objectfinderwidget.h"

ObjectFinderWidget::ObjectFinderWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	filter_frm->setVisible(false);
	updateObjectTypeList(obj_types_lst);

	connect(filter_btn, SIGNAL(toggled(bool)), filter_frm, SLOT(setVisible(bool)));
	connect(find_btn, SIGNAL(clicked(bool)), this, SLOT(findObjects(void)));
	connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
	connect(result_tbw, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(selectObject(void)));
	connect(result_tbw, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(editObject(void)));
	connect(clear_res_btn, SIGNAL(clicked(void)), this, SLOT(clearResult(void)));
	connect(select_all_btn, SIGNAL(clicked(void)), this, SLOT(setAllObjectsChecked(void)));
	connect(clear_all_btn, SIGNAL(clicked(void)), this, SLOT(setAllObjectsChecked(void)));
	this->setModel(nullptr);
	pattern_edt->installEventFilter(this);
}

bool ObjectFinderWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	//Executes the search when user press enter/return on the pattern field
	if(event->type() == QEvent::KeyPress &&
		 (k_event->key()==Qt::Key_Return || k_event->key()==Qt::Key_Enter))
	{
		find_btn->click();
		return(true);
	}
	else
		return(QWidget::eventFilter(object, event));
}

void ObjectFinderWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void ObjectFinderWidget::showEvent(QShowEvent *)
{
	pattern_edt->setFocus();
}

void ObjectFinderWidget::setModel(ModelWidget *model_wgt)
{
	bool enable=model_wgt!=nullptr;

	clearResult();
	this->model_wgt=model_wgt;
	filter_btn->setEnabled(enable);
	pattern_edt->setEnabled(enable);
	filter_frm->setEnabled(enable);
	pattern_lbl->setEnabled(enable);
	find_btn->setEnabled(enable);
	result_tbw->setEnabled(enable);
	highlight_btn->setEnabled(enable);
}

void ObjectFinderWidget::clearResult(void)
{
	selected_obj=nullptr;

	while(result_tbw->rowCount() > 0)
		result_tbw->removeRow(0);

	found_lbl->setVisible(false);
	clear_res_btn->setEnabled(false);
}

void ObjectFinderWidget::findObjects(void)
{
	if(model_wgt)
	{
		vector<BaseObject *> objs;
		vector<ObjectType> types;

		clearResult();

		//Getting the selected object types
		for(int i=0; i < obj_types_lst->count(); i++)
		{
			if(obj_types_lst->item(i)->checkState()==Qt::Checked)
				types.push_back(static_cast<ObjectType>(obj_types_lst->item(i)->data(Qt::UserRole).toUInt()));
		}

		//Search the objects on model
		objs=model_wgt->getDatabaseModel()->findObjects(pattern_edt->text(), types, case_sensitive_chk->isChecked(),
																										regexp_chk->isChecked(), exact_match_chk->isChecked());

		//Show the found objects on the result table
		updateObjectTable(result_tbw, objs);
		found_lbl->setVisible(true);

		//Show a message indicating the number of found objects
		if(!objs.empty())
		{
			found_lbl->setText(trUtf8("Found <strong>%1</strong> object(s).").arg(objs.size()));
			result_tbw->resizeColumnsToContents();
			result_tbw->horizontalHeader()->setStretchLastSection(true);
		}
		else
			found_lbl->setText(trUtf8("No objects found."));

		clear_res_btn->setEnabled(!objs.empty());
	}
}

void ObjectFinderWidget::selectObject(void)
{
	QTableWidgetItem *tab_item=result_tbw->item(result_tbw->currentRow(), 0);

	if(tab_item)
	{
		selected_obj=reinterpret_cast<BaseObject *>(tab_item->data(Qt::UserRole).value<void *>());
		BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(selected_obj);

		//Highlight the graphical object when the 'highlight' button is checked
		if(graph_obj && highlight_btn->isChecked())
		{
			BaseObjectView *obj=dynamic_cast<BaseObjectView *>(graph_obj->getReceiverObject());
			model_wgt->scene->clearSelection();
			model_wgt->viewport->centerOn(obj);
			obj->setSelected(true);
		}
	}
}

void ObjectFinderWidget::editObject(void)
{
	if(selected_obj)
	{
		if(selected_obj->getObjectType()==OBJ_PERMISSION)
			model_wgt->showObjectForm(OBJ_PERMISSION, dynamic_cast<Permission *>(selected_obj)->getObject());
		else
		{
			vector<BaseObject *> vect;
			vect.push_back(selected_obj);
			model_wgt->scene->clearSelection();
			model_wgt->configurePopupMenu(vect);
			model_wgt->editObject();
		}
	}
}

void ObjectFinderWidget::setAllObjectsChecked(void)
{
	bool checked=(sender()==select_all_btn);

	for(int i=0; i < obj_types_lst->count(); i++)
		obj_types_lst->item(i)->setCheckState((checked ? Qt::Checked : Qt::Unchecked));
}

void ObjectFinderWidget::updateObjectTable(QTableWidget *tab_wgt, vector<BaseObject *> &objs)
{
	if(tab_wgt && tab_wgt->columnCount()!=0)
	{
		unsigned lin_idx, i;
		QTableWidgetItem *tab_item=nullptr;
		BaseObject *parent_obj=nullptr;
		QFont fnt;
		QString str_aux;

		tab_wgt->setSortingEnabled(false);

		for(lin_idx=0, i=0; i < objs.size(); i++)
		{
			if(objs[i]->getObjectType()==BASE_RELATIONSHIP)
				str_aux="tv";
			else
				str_aux.clear();

			//First column: Object name
			tab_wgt->insertRow(lin_idx);		
			tab_item=new QTableWidgetItem;
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(objs[i])));
			fnt=tab_item->font();

			tab_item->setText(Utf8String::create(objs[i]->getName()));
			tab_item->setIcon(QPixmap(QString(":/icones/icones/") +
																BaseObject::getSchemaName(objs[i]->getObjectType()) + str_aux + QString(".png")));
			tab_wgt->setItem(lin_idx, 0, tab_item);

			if(objs[i]->isProtected() || objs[i]->isSystemObject())
			{
				fnt.setItalic(true);
				tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
			}
			else if(dynamic_cast<TableObject *>(objs[i]) &&
							dynamic_cast<TableObject *>(objs[i])->isAddedByRelationship())
			{
				fnt.setItalic(true);
				tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::INH_COLUMN).foreground());
			}
			tab_item->setFont(fnt);

			//Second column: Object type
			if(tab_wgt->columnCount() > 1)
			{
				fnt.setItalic(true);
				tab_item=new QTableWidgetItem;
				tab_item->setFont(fnt);
				tab_item->setText(objs[i]->getTypeName());
				tab_wgt->setItem(lin_idx, 1, tab_item);
			}

			//Third column: Parent object name
			if(tab_wgt->columnCount() > 2)
			{
				tab_item=new QTableWidgetItem;

				if(dynamic_cast<TableObject *>(objs[i]))
					parent_obj=dynamic_cast<TableObject *>(objs[i])->getParentTable();
				else if(objs[i]->getSchema())
					parent_obj=objs[i]->getSchema();
				else if(dynamic_cast<Permission *>(objs[i]))
					parent_obj=dynamic_cast<Permission *>(objs[i])->getObject();
				else
					parent_obj=objs[i]->getDatabase();

				tab_item->setText(parent_obj ? Utf8String::create(parent_obj->getName()) : "-");
				tab_wgt->setItem(lin_idx, 2, tab_item);

				if(parent_obj)
				{
					if(parent_obj->isProtected() || parent_obj->isSystemObject())
					{
						fnt.setItalic(true);
						tab_item->setFont(fnt);
						tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
					}

					tab_item->setIcon(QPixmap(QString(":/icones/icones/") +
																		BaseObject::getSchemaName(parent_obj->getObjectType())+ QString(".png")));
				}
			}

			//Fourth column: Parent object type
			if(tab_wgt->columnCount() > 3)
			{
				tab_item=new QTableWidgetItem;
				fnt.setItalic(true);
				tab_item->setFont(fnt);
				tab_item->setText(parent_obj ? parent_obj->getTypeName() : "-");
				tab_wgt->setItem(lin_idx, 3, tab_item);
			}

			lin_idx++;
		}

		tab_wgt->setSortingEnabled(true);
	}
}

void ObjectFinderWidget::updateObjectTypeList(QListWidget *list_wgt)
{
	vector<ObjectType> types=BaseObject::getObjectTypes();
	QListWidgetItem *item=nullptr;
	QPixmap icon;
	QString str_aux;

	if(list_wgt)
	{
		list_wgt->clear();

		//Creating the visible object list
		for(unsigned type_id=0; type_id < types.size(); type_id++)
		{
			item=new QListWidgetItem;

			if(types[type_id]==BASE_RELATIONSHIP)
				str_aux=QString(BaseObject::getSchemaName(types[type_id])) + "tv";
			else
				str_aux=QString(BaseObject::getSchemaName(types[type_id]));

			icon=QPixmap(Utf8String::create(":/icones/icones/") + str_aux + QString(".png"));

			item->setText(BaseObject::getTypeName(types[type_id]));
			item->setIcon(icon);

			//By default all object types are checked
			item->setCheckState(Qt::Checked);
			item->setData(Qt::UserRole, QVariant(types[type_id]));
			list_wgt->insertItem(type_id, item);
		}
	}
}
