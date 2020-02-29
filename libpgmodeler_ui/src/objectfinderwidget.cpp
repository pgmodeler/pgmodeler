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

#include "objectfinderwidget.h"
#include "pgmodeleruins.h"

const QStringList ObjectFinderWidget::search_attribs =
{ Attributes::Name, Attributes::Comment, Attributes::Signature,
	Attributes::Schema, Attributes::Owner, Attributes::Tablespace,
	Attributes::Type, Attributes::ReturnType };

const QStringList ObjectFinderWidget::search_attribs_i18n =
{ QT_TR_NOOP("Name"), QT_TR_NOOP("Comment"), QT_TR_NOOP("Signature"),
	QT_TR_NOOP("Schema"), QT_TR_NOOP("Owner"), QT_TR_NOOP("Tablespace"),
	QT_TR_NOOP("Data type"), QT_TR_NOOP("Return type")};

ObjectFinderWidget::ObjectFinderWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	filter_frm->setVisible(false);
	splitter->handle(1)->setEnabled(false);
	updateObjectTypeList(obj_types_lst);

	select_menu.addAction(tr("Listed"), this, SLOT(selectObjects()));
	select_menu.addAction(tr("Not listed"), this, SLOT(selectObjects()));
	select_btn->setMenu(&select_menu);

	fade_menu.addAction(tr("Listed"), this, SLOT(fadeObjects()));
	fade_menu.addAction(tr("Not listed"), this, SLOT(fadeObjects()));
	fade_btn->setMenu(&fade_menu);

	connect(filter_btn, SIGNAL(toggled(bool)), filter_frm, SLOT(setVisible(bool)));
	connect(filter_btn, &QToolButton::toggled, [&](){
		splitter->setSizes({0, 1000});
		splitter->handle(1)->setEnabled(filter_btn->isChecked());
	});

	connect(find_btn, SIGNAL(clicked(bool)), this, SLOT(findObjects()));
	connect(hide_tb, SIGNAL(clicked()), this, SLOT(hide()));
	connect(result_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(selectObject()));
	connect(result_tbw, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(editObject()));
	connect(result_tbw, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(showObjectMenu()));
	connect(clear_res_btn, SIGNAL(clicked()), this, SLOT(clearResult()));
	connect(select_all_btn, SIGNAL(clicked()), this, SLOT(setAllObjectsChecked()));
	connect(clear_all_btn, SIGNAL(clicked()), this, SLOT(setAllObjectsChecked()));

	for(auto &attr : search_attribs_i18n)
		search_attrs_cmb->addItem(attr);

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
		return true;
	}
	else
		return QWidget::eventFilter(object, event);
}

void ObjectFinderWidget::hide()
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void ObjectFinderWidget::showEvent(QShowEvent *)
{
	pattern_edt->setFocus();
}

void ObjectFinderWidget::resizeEvent(QResizeEvent *event)
{
	Qt::ToolButtonStyle style=Qt::ToolButtonTextBesideIcon;

	if(event->size().width() < this->baseSize().width())
		style=Qt::ToolButtonIconOnly;

	if(find_btn->toolButtonStyle()!=style)
	{
		filter_btn->setToolButtonStyle(style);
		find_btn->setToolButtonStyle(style);
		clear_res_btn->setToolButtonStyle(style);
		select_btn->setToolButtonStyle(style);
		fade_btn->setToolButtonStyle(style);
	}
}

void ObjectFinderWidget::fadeObjects()
{
	if(!model_wgt)
		return;

	vector<BaseObject *> objects, other_objs;
	bool fade_listed = false;

	for(auto obj_type : {ObjectType::Table, ObjectType::ForeignTable, ObjectType::View, ObjectType::Textbox,
											 ObjectType::Relationship, ObjectType::BaseRelationship, ObjectType::Schema})
	{
		objects.insert(objects.end(),
									 model_wgt->getDatabaseModel()->getObjectList(obj_type)->begin(),
									 model_wgt->getDatabaseModel()->getObjectList(obj_type)->end());
	}

	model_wgt->fadeObjects(objects, true);

	if(!fade_menu.actions().contains(qobject_cast<QAction *>(sender())))
		return;

	fade_listed = qobject_cast<QAction *>(sender()) == fade_menu.actions().at(0);

	std::sort(objects.begin(), objects.end());
	std::sort(found_objs.begin(), found_objs.end());
	std::set_difference(objects.begin(), objects.end(), found_objs.begin(), found_objs.end(),
											std::inserter(other_objs, other_objs.begin()));

	model_wgt->fadeObjects(found_objs, !fade_listed);
	model_wgt->fadeObjects(other_objs, fade_listed);
}

void ObjectFinderWidget::selectObjects()
{
	if(!model_wgt)
		return;

	vector<BaseObject *> objects, other_objs;
	BaseObjectView *obj_view = nullptr;
	BaseGraphicObject *graph_obj = nullptr;
	bool sel_listed = false;

	for(auto obj_type : {ObjectType::Table, ObjectType::View, ObjectType::Textbox, ObjectType::Relationship, ObjectType::BaseRelationship, ObjectType::Schema})
	{
		objects.insert(objects.end(),
									 model_wgt->getDatabaseModel()->getObjectList(obj_type)->begin(),
									 model_wgt->getDatabaseModel()->getObjectList(obj_type)->end());
	}

	model_wgt->scene->blockSignals(true);
	fadeObjects();
	model_wgt->scene->blockSignals(false);

	sel_listed = qobject_cast<QAction *>(sender()) == select_menu.actions().at(0);

	std::sort(objects.begin(), objects.end());
	std::sort(found_objs.begin(), found_objs.end());
	std::set_difference(objects.begin(), objects.end(), found_objs.begin(), found_objs.end(),
											std::inserter(other_objs, other_objs.begin()));

	objects.clear();

	if(sel_listed)
		objects.assign(found_objs.begin(), found_objs.end());
	else
		objects.assign(other_objs.begin(), other_objs.end());

	for(auto &obj : objects)
	{
		graph_obj = dynamic_cast<BaseGraphicObject *>(obj);

		if(graph_obj)
		{
			obj_view = dynamic_cast<BaseObjectView *>(graph_obj->getOverlyingObject());

			if(obj_view)
			{
				obj_view->blockSignals(true);
				obj_view->setSelected(true);
				obj_view->blockSignals(false);
			}
		}
	}

	model_wgt->configureObjectSelection();
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
}

void ObjectFinderWidget::clearResult()
{
	selected_obj=nullptr;
	found_objs.clear();
	selected_objs.clear();

	result_tbw->clearContents();
	result_tbw->setRowCount(0);

	found_lbl->setVisible(false);
	clear_res_btn->setEnabled(false);

	select_btn->setEnabled(false);
	fade_btn->setEnabled(false);
}

void ObjectFinderWidget::findObjects()
{
	if(model_wgt)
	{
		vector<ObjectType> types;
		QString search_attr = search_attribs.at(search_attrs_cmb->currentIndex());
		QTableWidgetItem *item = result_tbw->horizontalHeaderItem(result_tbw->columnCount() - 1);

		clearResult();

		//Getting the selected object types
		for(int i=0; i < obj_types_lst->count(); i++)
		{
			if(obj_types_lst->item(i)->checkState()==Qt::Checked)
				types.push_back(static_cast<ObjectType>(obj_types_lst->item(i)->data(Qt::UserRole).toUInt()));
		}

		//Search the objects on model
		found_objs=model_wgt->getDatabaseModel()->findObjects(pattern_edt->text(), types,
																													case_sensitive_chk->isChecked(), regexp_chk->isChecked(),
																													exact_match_chk->isChecked(),
																													search_attr);

		//Show the found objects on the result table
		updateObjectTable(result_tbw, found_objs, search_attr);

		//Rename the last column of the results grid wth the name of the field used to search objects
		if(search_attr != Attributes::Name &&
			 search_attr != Attributes::Schema &&
			 search_attr != Attributes::Comment)
			item->setText(search_attrs_cmb->currentText());
		else
			item->setText(tr("Comment"));

		found_lbl->setVisible(true);

		//Show a message indicating the number of found objects
		if(!found_objs.empty())
		{
			found_lbl->setText(tr("Found <strong>%1</strong> object(s).").arg(found_objs.size()));
			result_tbw->horizontalHeader()->setStretchLastSection(true);
			result_tbw->resizeColumnsToContents();
		}
		else
			found_lbl->setText(tr("No objects found."));

		clear_res_btn->setEnabled(!found_objs.empty());
		select_btn->setEnabled(!found_objs.empty());
		fade_btn->setEnabled(!found_objs.empty());
		fadeObjects();
	}
}

void ObjectFinderWidget::selectObject()
{
	BaseGraphicObject *graph_obj = nullptr;
	BaseObjectView *obj_view = nullptr;
	TableObject *tab_obj = nullptr;

	selected_objs.clear();
	model_wgt->scene->clearSelection();

	for(auto &item : result_tbw->selectedItems())
	{
		if(item->column() != 0)
			continue;

		selected_objs.push_back(reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>()));
	}

	for(auto &obj : selected_objs)
	{
		graph_obj = dynamic_cast<BaseGraphicObject *>(obj);
		tab_obj = dynamic_cast<TableObject *>(obj);

		if(tab_obj && !graph_obj)
			graph_obj = dynamic_cast<BaseGraphicObject *>(tab_obj->getParentTable());

		if(graph_obj)
		{
			obj_view=dynamic_cast<BaseObjectView *>(graph_obj->getOverlyingObject());

			if(obj_view)
			{
				model_wgt->viewport->centerOn(obj_view);
				obj_view->setSelected(true);
			}
		}
	}

	model_wgt->configurePopupMenu(selected_objs);
	model_wgt->emitSceneInteracted();
}

void ObjectFinderWidget::showObjectMenu()
{
	if(!selected_objs.empty() && QApplication::mouseButtons()==Qt::RightButton)
		model_wgt->showObjectMenu();
}

void ObjectFinderWidget::editObject()
{
	if(selected_obj)
	{
		if(selected_obj->getObjectType()==ObjectType::Permission)
			model_wgt->showObjectForm(ObjectType::Permission, dynamic_cast<Permission *>(selected_obj)->getObject());
		else
		{
			vector<BaseObject *> vect;
			vect.push_back(selected_obj);
			model_wgt->scene->clearSelection();
			model_wgt->configurePopupMenu(vect);
			model_wgt->editObject();
		}

		selected_obj=nullptr;
	}
}

void ObjectFinderWidget::setAllObjectsChecked()
{
	bool checked=(sender()==select_all_btn);

	for(int i=0; i < obj_types_lst->count(); i++)
		obj_types_lst->item(i)->setCheckState((checked ? Qt::Checked : Qt::Unchecked));
}

void ObjectFinderWidget::updateObjectTable(QTableWidget *tab_wgt, vector<BaseObject *> &objs, const QString &search_attr)
{
	if(tab_wgt && tab_wgt->columnCount()!=0)
	{
		unsigned lin_idx, i;
		QTableWidgetItem *tab_item=nullptr;
		BaseObject *parent_obj=nullptr;
		QFont fnt;
		QString str_aux;
		bool new_row = false;

		tab_wgt->setUpdatesEnabled(false);
		tab_wgt->setSortingEnabled(false);

		for(lin_idx=0, i=0; i < objs.size(); i++)
		{
			if(objs[i]->getObjectType()==ObjectType::BaseRelationship)
				str_aux=QString("tv");
			else
				str_aux.clear();

			new_row = false;

			if(static_cast<int>(lin_idx) >= tab_wgt->rowCount())
			{
			  tab_wgt->insertRow(lin_idx);
			  new_row = true;
			}

			//First column: Object id
			tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 0));
			tab_item->setText(QString::number(objs[i]->getObjectId()));
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(objs[i])));
			if(new_row) tab_wgt->setItem(lin_idx, 0, tab_item);

			//Second column: Object name
			if(tab_wgt->columnCount() > 1)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 1));
				tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(objs[i])));
				fnt=tab_item->font();

				tab_item->setText(objs[i]->getName());
				tab_item->setIcon(QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(objs[i]->getObjectType()) + str_aux)));
				if(new_row) tab_wgt->setItem(lin_idx, 1, tab_item);

				if(objs[i]->isProtected() || objs[i]->isSystemObject())
				{
					fnt.setItalic(true);
					tab_item->setForeground(BaseObjectView::getFontStyle(Attributes::ProtColumn).foreground());
				}
				else if(dynamic_cast<TableObject *>(objs[i]) &&
						dynamic_cast<TableObject *>(objs[i])->isAddedByRelationship())
				{
					fnt.setItalic(true);
					tab_item->setForeground(BaseObjectView::getFontStyle(Attributes::InhColumn).foreground());
				}
				else
				{
					fnt.setItalic(false);
					tab_item->setForeground(BaseObjectView::getFontStyle(Attributes::Column).foreground());
				}


				fnt.setStrikeOut(objs[i]->isSQLDisabled() && !objs[i]->isSystemObject());
				tab_item->setFont(fnt);
				fnt.setStrikeOut(false);
			}

			//Third column: Object type
			if(tab_wgt->columnCount() > 2)
			{
				fnt.setItalic(true);
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 2));
				tab_item->setFont(fnt);
				tab_item->setText(objs[i]->getTypeName());
				if(new_row) tab_wgt->setItem(lin_idx, 2, tab_item);
			}

			//Fourth column: Parent object name
			if(tab_wgt->columnCount() > 3)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 3));

				if(dynamic_cast<TableObject *>(objs[i]))
					parent_obj=dynamic_cast<TableObject *>(objs[i])->getParentTable();
				else if(objs[i]->getSchema())
					parent_obj=objs[i]->getSchema();
				else if(dynamic_cast<Permission *>(objs[i]))
					parent_obj=dynamic_cast<Permission *>(objs[i])->getObject();
				else
					parent_obj=objs[i]->getDatabase();

				tab_item->setText(parent_obj ? parent_obj->getName() : QString("-"));
				tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(parent_obj)));
				if(new_row) tab_wgt->setItem(lin_idx, 3, tab_item);

				if(parent_obj)
				{
					if(parent_obj->isProtected() || parent_obj->isSystemObject())
					{
						fnt.setItalic(true);
						tab_item->setForeground(BaseObjectView::getFontStyle(Attributes::ProtColumn).foreground());
					}
					else
					{
						fnt.setItalic(false);
						tab_item->setForeground(BaseObjectView::getFontStyle(Attributes::Column).foreground());
					}

					tab_item->setFont(fnt);
					tab_item->setIcon(QPixmap(PgModelerUiNs::getIconPath(parent_obj->getObjectType())));
				}
			}

			//Fifth column: Parent object type
			if(tab_wgt->columnCount() > 4)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 4));
				fnt.setItalic(true);
				tab_item->setFont(fnt);
				tab_item->setText(parent_obj ? parent_obj->getTypeName() : QString("-"));
				if(new_row) tab_wgt->setItem(lin_idx, 4, tab_item);
			}

			//Sixth column: object comment
			if(tab_wgt->columnCount() > 5)
			{				
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 5));
				fnt.setItalic(false);
				tab_item->setFont(fnt);

				if(search_attr != Attributes::Name &&
					 search_attr != Attributes::Schema &&
					 search_attr != Attributes::Comment)
					tab_item->setText(objs[i]->getSearchAttributes().at(search_attr));
				else
					tab_item->setText(objs[i]->getComment());

				if(new_row) tab_wgt->setItem(lin_idx, 5, tab_item);
			}

			lin_idx++;
		}

		if(static_cast<int>(objs.size()) != tab_wgt->rowCount())
		  tab_wgt->setRowCount(objs.size());

		tab_wgt->setUpdatesEnabled(true);
		tab_wgt->setSortingEnabled(true);
		tab_wgt->resizeColumnsToContents();
		tab_wgt->resizeRowsToContents();
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

			if(types[type_id]==ObjectType::BaseRelationship)
				str_aux=QString(BaseObject::getSchemaName(types[type_id])) + QString("tv");
			else
				str_aux=QString(BaseObject::getSchemaName(types[type_id]));

			icon=QPixmap(PgModelerUiNs::getIconPath(str_aux));

			item->setText(BaseObject::getTypeName(types[type_id]));
			item->setIcon(icon);

			//By default all object types are checked
			item->setCheckState(Qt::Checked);
			item->setData(Qt::UserRole, QVariant(enum_cast(types[type_id])));
			list_wgt->insertItem(type_id, item);
		}
	}
}
