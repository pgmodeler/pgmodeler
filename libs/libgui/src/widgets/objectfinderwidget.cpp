/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "guiutilsns.h"
#include "coreutilsns.h"
#include "objectstablewidget.h"

const QStringList ObjectFinderWidget::search_attribs =
{ Attributes::Name, Attributes::Comment, Attributes::Signature,
	Attributes::Schema, Attributes::Owner, Attributes::Tablespace,
	Attributes::Type, Attributes::ReturnType, Attributes::SrcTable,
	Attributes::DstTable, Attributes::RelatedForeignKey,
	Attributes::SrcColumns, Attributes::RefColumns };

const QStringList ObjectFinderWidget::search_attribs_i18n =
{ QT_TR_NOOP("Name"), QT_TR_NOOP("Comment"), QT_TR_NOOP("Signature"),
	QT_TR_NOOP("Schema"), QT_TR_NOOP("Owner"), QT_TR_NOOP("Tablespace"),
	QT_TR_NOOP("Data type"), QT_TR_NOOP("Return type"),
	QT_TR_NOOP("Source table"), QT_TR_NOOP("Destination table"),
	QT_TR_NOOP("Related foreign key"),	QT_TR_NOOP("Source column(s)"),
	QT_TR_NOOP("Referenced column(s)") };

ObjectFinderWidget::ObjectFinderWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	filter_wgt = new QWidget(this);
	obj_types_lst = new ObjectTypesListWidget(this);
	obj_types_lst->layout()->setContentsMargins(0,0,0,0);

	regexp_chk = new QCheckBox(this);
	regexp_chk->setText(tr("Regular expression"));

	exact_match_chk = new QCheckBox(this);
	exact_match_chk->setText(tr("Exact match"));

	case_sensitive_chk = new QCheckBox(this);
	case_sensitive_chk->setText(tr("Case sensitive"));

	QVBoxLayout *vbox = new QVBoxLayout(filter_wgt);
	vbox->addWidget(obj_types_lst);
	vbox->addWidget(regexp_chk);
	vbox->addWidget(exact_match_chk);
	vbox->addWidget(case_sensitive_chk);
	vbox->setContentsMargins(GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin);
	vbox->setSpacing(GuiUtilsNs::LtSpacing);
	filter_wgt->setLayout(vbox);

	QWidgetAction *wgt_act_filter = new QWidgetAction(this);
	wgt_act_filter->setDefaultWidget(filter_wgt);
	filter_menu.addAction(wgt_act_filter);
	filter_btn->setMenu(&filter_menu);

	select_menu.addAction(tr("Listed"), this, &ObjectFinderWidget::selectObjects);
	select_menu.addAction(tr("Not listed"), this, &ObjectFinderWidget::selectObjects);
	select_btn->setMenu(&select_menu);

	fade_menu.addAction(tr("Listed"), this, &ObjectFinderWidget::fadeObjects);
	fade_menu.addAction(tr("Not listed"), this, &ObjectFinderWidget::fadeObjects);
	fade_btn->setMenu(&fade_menu);

	connect(find_btn, &QToolButton::clicked, this, &ObjectFinderWidget::findObjects);
	connect(hide_tb, &QToolButton::clicked, this, &ObjectFinderWidget::hide);
	connect(result_tbw, &QTableWidget::itemSelectionChanged, this, &ObjectFinderWidget::selectObject);
	connect(result_tbw, &QTableWidget::itemDoubleClicked, this, &ObjectFinderWidget::editObject);
	connect(result_tbw, &QTableWidget::itemPressed, this, &ObjectFinderWidget::showObjectMenu);
	connect(clear_res_btn, &QToolButton::clicked, this, &ObjectFinderWidget::clearResult);

	connect(regexp_chk, &QCheckBox::toggled, this, [this](bool checked){
		exact_match_chk->setEnabled(checked);

		if(!checked)
			exact_match_chk->setChecked(false);
	});

	for(auto &attr : search_attribs_i18n)
		search_attrs_cmb->addItem(attr);

	this->setModel(nullptr);
	pattern_edt->installEventFilter(this);
}

bool ObjectFinderWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	//Executes the search when user press and releases enter/return on the pattern field
	if(event->type() == QEvent::KeyRelease &&
			(k_event->key()==Qt::Key_Return || k_event->key()==Qt::Key_Enter))
	{
		find_btn->click();
		return true;
	}

	return QWidget::eventFilter(object, event);
}

void ObjectFinderWidget::hide()
{
	QWidget::hide();
	emit s_hideRequested();
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

	std::vector<BaseObject *> objects, other_objs;
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

	std::vector<BaseObject *> objects, other_objs;
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
	//fadeObjects();
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
		std::vector<ObjectType> types;
		QString search_attr = search_attribs.at(search_attrs_cmb->currentIndex());
		QTableWidgetItem *item = result_tbw->horizontalHeaderItem(result_tbw->columnCount() - 1);

		clearResult();
		types = obj_types_lst->getTypesPerCheckState(Qt::Checked);

		//Search the objects on model
		found_objs=model_wgt->getDatabaseModel()->findObjects(pattern_edt->text(), types,
																													case_sensitive_chk->isChecked(),
																													regexp_chk->isChecked(),
																													exact_match_chk->isChecked(),
																													search_attr);

		//Show the found objects on the result table
		GuiUtilsNs::updateObjectTable(result_tbw, found_objs, search_attr);

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
		//fadeObjects();
	}
}

void ObjectFinderWidget::selectObject()
{
	BaseGraphicObject *graph_obj = nullptr;
	BaseObjectView *obj_view = nullptr;
	TableObject *tab_obj = nullptr;
	BaseObject *object = nullptr;

	selected_objs.clear();
	model_wgt->scene->clearSelection();

	if(result_tbw->selectedRanges().size() == 1 && result_tbw->currentItem()->column() == 0)
	{
		object = reinterpret_cast<BaseObject *>(result_tbw->currentItem()->data(Qt::UserRole).value<void *>());
		selected_obj = object;

		if(object->getObjectType() == ObjectType::Permission)
			return;
	}

	for(auto &item : result_tbw->selectedItems())
	{
		object = reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

		if(item->column() != 0 ||
			 (object && object->getObjectType() == ObjectType::Permission))
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
		if(selected_obj->getObjectType() == ObjectType::Permission)
			model_wgt->showObjectForm(ObjectType::Permission, dynamic_cast<Permission *>(selected_obj)->getObject());
		else
		{
			std::vector<BaseObject *> vect;
			vect.push_back(selected_obj);
			model_wgt->scene->clearSelection();
			model_wgt->configurePopupMenu(vect);
			model_wgt->editObject();
		}

		selected_obj=nullptr;
	}
}

/* void ObjectFinderWidget::updateObjectTable(QTableWidget *tab_wgt, std::vector<BaseObject *> &objs, const QString &search_attr, bool checkable_items)
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

			//First column: Object name
			tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 0));
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(objs[i])));
			fnt=tab_item->font();

			tab_item->setText(objs[i]->getName());
			tab_item->setIcon(QPixmap(GuiUtilsNs::getIconPath(BaseObject::getSchemaName(objs[i]->getObjectType()) + str_aux)));
			if(new_row) tab_wgt->setItem(lin_idx, 0, tab_item);
			if(checkable_items)	tab_item->setCheckState(Qt::Checked);

			if(objs[i]->isProtected() || objs[i]->isSystemObject())
			{
				fnt.setItalic(true);
				tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor));
			}
			else if(dynamic_cast<TableObject *>(objs[i]) &&
					dynamic_cast<TableObject *>(objs[i])->isAddedByRelationship())
			{
				fnt.setItalic(true);
				tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::RelAddedItemAltFgColor));
			}
			else
				fnt.setItalic(false);

			fnt.setStrikeOut(objs[i]->isSQLDisabled() && !objs[i]->isSystemObject());
			tab_item->setFont(fnt);
			fnt.setStrikeOut(false);

			//Second column: Object type
			if(tab_wgt->columnCount() > 1)
			{
				fnt.setItalic(true);
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 1));
				tab_item->setFont(fnt);
				tab_item->setText(objs[i]->getTypeName());
				if(new_row) tab_wgt->setItem(lin_idx, 1, tab_item);
			}

			//Third column: Object id
			if(tab_wgt->columnCount() > 2)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 2));
				tab_item->setText(QString::number(objs[i]->getObjectId()));
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
						tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor));
					}
					else
						fnt.setItalic(false);

					tab_item->setFont(fnt);
					tab_item->setIcon(QPixmap(GuiUtilsNs::getIconPath(parent_obj->getObjectType())));
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
				attribs_map search_attribs = objs[i]->getSearchAttributes();
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 5));
				fnt.setItalic(false);
				tab_item->setFont(fnt);

				if(search_attr != Attributes::Name &&
					 search_attr != Attributes::Schema &&
					 search_attr != Attributes::Comment)
					tab_item->setText(search_attribs[search_attr]);
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
} */
