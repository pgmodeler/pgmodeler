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

#include "modelobjectswidget.h"
#include "databaseimportform.h"
#include "pgmodeleruins.h"

ModelObjectsWidget::ModelObjectsWidget(bool simplified_view, QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	model_wgt=nullptr;
	db_model=nullptr;
	setModel(db_model);

	title_wgt->setVisible(!simplified_view);
	this->simplified_view=simplified_view;
	this->save_tree_state=!simplified_view;
	enable_obj_creation=simplified_view;

	select_tb->setVisible(simplified_view);
	cancel_tb->setVisible(simplified_view);
	options_tb->setVisible(!simplified_view);
	visibleobjects_grp->setVisible(false);
	filter_wgt->setVisible(simplified_view);
	splitter->handle(1)->setEnabled(false);

	connect(objectstree_tw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(selectObject()));
	connect(objectstree_tw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(showObjectMenu()));
	connect(objectslist_tbw, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(selectObject()));
	connect(objectslist_tbw, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(showObjectMenu()));
	connect(objectstree_tw,SIGNAL(itemSelectionChanged()),this, SLOT(selectObject()));
	connect(objectslist_tbw,SIGNAL(itemSelectionChanged()),this, SLOT(selectObject()));
	connect(expand_all_tb, SIGNAL(clicked()), objectstree_tw, SLOT(expandAll()));
	connect(collapse_all_tb, SIGNAL(clicked()), this, SLOT(collapseAll()));

	if(!simplified_view)
	{
		widgets_conf.setValue(QString("splitterSize"), splitter->saveState());
		connect(options_tb,SIGNAL(clicked()),this,SLOT(changeObjectsView()));
		connect(visibleobjects_lst,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setObjectVisible(QListWidgetItem*)));
		connect(select_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(clear_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(editObject()));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(editObject()));
		connect(hide_tb, SIGNAL(clicked(bool)), this, SLOT(hide()));

		ObjectFinderWidget::updateObjectTypeList(visibleobjects_lst);
		setAllObjectsVisible(true);
		objectslist_tbw->installEventFilter(this);
		objectstree_tw->installEventFilter(this);
		objectslist_tbw->setSelectionMode(QAbstractItemView::ExtendedSelection);
		objectstree_tw->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	else
	{
		setMinimumSize(250, 300);
		setWindowModality(Qt::ApplicationModal);
		setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(close()));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(close()));
		connect(select_tb,SIGNAL(clicked()),this,SLOT(close()));
		connect(cancel_tb,SIGNAL(clicked()),this,SLOT(close()));
	}

	connect(tree_view_tb,SIGNAL(clicked()),this,SLOT(changeObjectsView()));
	connect(list_view_tb,SIGNAL(clicked()),this,SLOT(changeObjectsView()));
	connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects()));
	connect(by_id_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects()));
}

bool ModelObjectsWidget::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::FocusOut &&
		 (object==objectslist_tbw || object==objectstree_tw))
	{
		QFocusEvent *evnt=dynamic_cast<QFocusEvent *>(event);

		if(evnt->reason()==Qt::MouseFocusReason)
		{
			clearSelectedObject();

			if(model_wgt)
				model_wgt->configurePopupMenu(nullptr);

			return true;
		}
	}

	return QWidget::eventFilter(object, event);
}

void ModelObjectsWidget::hide()
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}


void ModelObjectsWidget::showObjectMenu()
{
	if(!selected_objs.empty() && QApplication::mouseButtons()==Qt::RightButton && model_wgt && !simplified_view)
	{
		model_wgt->showObjectMenu();
		clearSelectedObject();
	}
}

void ModelObjectsWidget::editObject()
{
	if(selected_objs.size() == 1 && model_wgt && !simplified_view)
	{
		//If the user double-clicked the item "Permission (n)" on tree view
		if(sender()==objectstree_tw && objectstree_tw->currentItem() &&
			 objectstree_tw->currentItem()->data(1, Qt::UserRole).toUInt() == enum_cast(ObjectType::Permission))
			model_wgt->showObjectForm(ObjectType::Permission, reinterpret_cast<BaseObject *>(objectstree_tw->currentItem()->data(0, Qt::UserRole).value<void *>()));
		//If the user double-clicked a permission on  list view
		else if(sender() == objectslist_tbw && objectslist_tbw->currentRow() >= 0)
		{
			BaseObject *obj=reinterpret_cast<Permission *>(objectslist_tbw->item(objectslist_tbw->currentRow(), 0)->data(Qt::UserRole).value<void *>());
			Permission *perm=dynamic_cast<Permission *>(obj);

			if(perm)
				model_wgt->showObjectForm(ObjectType::Permission, perm->getObject());
			else
			  model_wgt->editObject();
		}
		else
			model_wgt->editObject();

		clearSelectedObject();
	}
}

void ModelObjectsWidget::selectObject()
{
	BaseObject *selected_obj = nullptr;
	ObjectType obj_type = ObjectType::BaseObject;
	ModelWidget *model_wgt = nullptr;

	selected_objs.clear();

	if(!simplified_view && this->model_wgt)
		model_wgt = this->model_wgt;
	else if(simplified_view)
		model_wgt = db_model->getModelWidget();

	if(tree_view_tb->isChecked())
	{
		QTreeWidgetItem *tree_item = objectstree_tw->currentItem();

		if(tree_item)
		{
			obj_type = static_cast<ObjectType>(tree_item->data(1,Qt::UserRole).toUInt());
			selected_obj = reinterpret_cast<BaseObject *>(tree_item->data(0,Qt::UserRole).value<void *>());

			for(auto &item : objectstree_tw->selectedItems())
			{
				selected_obj = reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

				if(selected_obj)
					selected_objs.push_back(selected_obj);
			}
		}

		//If user select a group item popups a "New [OBJECT]" menu
		if((!simplified_view || (simplified_view && enable_obj_creation)) &&
				!selected_obj && QApplication::mouseButtons() == Qt::RightButton &&
				obj_type != ObjectType::Column && obj_type != ObjectType::Constraint && obj_type != ObjectType::Rule &&
				obj_type != ObjectType::Index && obj_type != ObjectType::Trigger && obj_type != ObjectType::Permission)
		{
			QAction act(QPixmap(PgModelerUiNs::getIconPath(obj_type)),
									tr("New") + QString(" ") + BaseObject::getTypeName(obj_type), nullptr);
			QMenu popup;

			//If not a relationship, connect the action to the addNewObject method of the model wiget
			if(obj_type != ObjectType::Relationship)
			{
				act.setData(QVariant(enum_cast(obj_type)));
				connect(&act, SIGNAL(triggered()), model_wgt, SLOT(addNewObject()));
			}
			//Case is a relationship, insert the relationship menu of the model wiget into the action
			else
				act.setMenu(model_wgt->rels_menu);

			if(simplified_view && enable_obj_creation)
				connect(model_wgt->getDatabaseModel(), SIGNAL(s_objectAdded(BaseObject*)), this, SLOT(selectCreatedObject(BaseObject *)), Qt::QueuedConnection);

			popup.addAction(&act);
			popup.exec(QCursor::pos());
			disconnect(&act, nullptr, model_wgt, nullptr);
			disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);
		}
	}
	else
	{
		QTableWidgetItem *tab_item=objectslist_tbw->item(objectslist_tbw->currentRow(), 0);

		if(tab_item)
		{
			selected_obj = reinterpret_cast<BaseObject *>(tab_item->data(Qt::UserRole).value<void *>());
			obj_type = selected_obj->getObjectType();

			for(auto &item : objectslist_tbw->selectedItems())
			{
				if(item->column() !=0 )
					continue;

				selected_obj = reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

				if(selected_obj)
					selected_objs.push_back(selected_obj);
			}
		}
	}

	if(obj_type != ObjectType::Permission && !selected_objs.empty() && !simplified_view)
	{
		model_wgt->scene->clearSelection();
		model_wgt->configurePopupMenu(selected_objs);
		model_wgt->emitSceneInteracted();
	}
}

QVariant ModelObjectsWidget::generateItemValue(BaseObject *object)
{
	return QVariant::fromValue(reinterpret_cast<void *>(object));
}

QTreeWidgetItem *ModelObjectsWidget::createItemForObject(BaseObject *object, QTreeWidgetItem *root, bool update_perms)
{
	QTreeWidgetItem *item=nullptr;
	QFont font;
	QString str_aux;
	unsigned rel_type=0;
	ConstraintType constr_type;
	ObjectType obj_type;
	TableObject *tab_obj=nullptr;
	QString obj_name;

	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	obj_type=object->getObjectType();
	tab_obj=dynamic_cast<TableObject *>(object);
	item=new QTreeWidgetItem(root);

	if(obj_type==ObjectType::Function)
	{
		Function *func=dynamic_cast<Function *>(object);
		func->createSignature(false);
		item->setText(0,func->getSignature());
		obj_name=func->getSignature();
		func->createSignature(true);
	}
	else if(obj_type==ObjectType::Operator)
	{
		Operator *oper=dynamic_cast<Operator *>(object);
		item->setText(0, oper->getSignature(false));
		obj_name=oper->getSignature(false);
	}
	else if(obj_type==ObjectType::OpClass || obj_type == ObjectType::OpFamily)
	{
		obj_name=object->getSignature(false);
		obj_name.replace(QRegExp("( )+(USING)( )+"), QString(" ["));
		obj_name+=QChar(']');
		item->setText(0,obj_name);
	}
	else
	{
		item->setText(0,object->getName());
		obj_name=object->getName();
	}

	item->setToolTip(0, QString("%1 (id: %2)").arg(obj_name).arg(object->getObjectId()));
	item->setData(0, Qt::UserRole, generateItemValue(object));
	item->setText(1, QString::number(object->getObjectId()));

	if(update_perms)
		updatePermissionTree(item, object);

	font=item->font(0);
	font.setStrikeOut(object->isSQLDisabled() && !object->isSystemObject());

	if(tab_obj && tab_obj->isAddedByRelationship())
	{
		font.setItalic(true);
		item->setForeground(0,BaseObjectView::getFontStyle(Attributes::InhColumn).foreground());
	}
	else if(object->isProtected() || object->isSystemObject())
	{
		font.setItalic(true);
		item->setForeground(0,BaseObjectView::getFontStyle(Attributes::ProtColumn).foreground());
	}

	item->setFont(0,font);

	if(obj_type==ObjectType::BaseRelationship || obj_type==ObjectType::Relationship)
	{
		rel_type=dynamic_cast<BaseRelationship *>(object)->getRelationshipType();

		if(obj_type==ObjectType::BaseRelationship)
		{
			if(rel_type==BaseRelationship::RelationshipFk)
				str_aux=QString("fk");
			else
				str_aux=QString("tv");
		}
		else if(rel_type==BaseRelationship::Relationship11)
			str_aux=QString("11");
		else if(rel_type==BaseRelationship::Relationship1n)
			str_aux=QString("1n");
		else if(rel_type==BaseRelationship::RelationshipNn)
			str_aux=QString("nn");
		else if(rel_type==BaseRelationship::RelationshipDep)
			str_aux=QString("dep");
		else if(rel_type==BaseRelationship::RelationshipGen)
			str_aux=QString("gen");
	}
	else if(obj_type==ObjectType::Constraint)
	{
		constr_type=dynamic_cast<Constraint *>(object)->getConstraintType();

		if(constr_type==ConstraintType::PrimaryKey)
			str_aux=QString("_%1").arg(TableObjectView::TextPrimaryKey);
		else if(constr_type==ConstraintType::ForeignKey)
			str_aux=QString("_%1").arg(TableObjectView::TextForeignKey);
		else if(constr_type==ConstraintType::Check)
			str_aux=QString("_%1").arg(TableObjectView::TextCheck);
		else if(constr_type==ConstraintType::Unique)
			str_aux=QString("_%1").arg(TableObjectView::TextUnique);
		else if(constr_type==ConstraintType::Exclude)
			str_aux=QString("_%1").arg(TableObjectView::TextExclude);
	}

	item->setIcon(0, QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(obj_type) + str_aux)));

	return item;
}

void ModelObjectsWidget::setObjectVisible(ObjectType obj_type, bool visible)
{
	if(obj_type!=ObjectType::BaseObject && obj_type!=ObjectType::BaseTable)
		visible_objs_map[obj_type]=visible;

	if(visible && simplified_view)
	{
		if(obj_type!=ObjectType::Database)
			visible_objs_map[ObjectType::Database]=true;

		if(TableObject::isTableObject(obj_type))
		{
			visible_objs_map[ObjectType::Table]=visible_objs_map[ObjectType::Schema]=true;

			if(obj_type == ObjectType::Column ||
				 obj_type == ObjectType::Constraint ||
				 obj_type == ObjectType::Trigger)
				visible_objs_map[ObjectType::ForeignTable]=true;
		}

		if(BaseObject::acceptsSchema(obj_type))
			visible_objs_map[ObjectType::Schema]=true;
	}
}

void ModelObjectsWidget::setObjectVisible(QListWidgetItem *item)
{
	ObjectType obj_type;

	//Get the object type related to the selected item
	obj_type=static_cast<ObjectType>(item->data(Qt::UserRole).toInt());

	//Set the visibility of the object type
	setObjectVisible(obj_type, item->checkState()==Qt::Checked);

	//Updates the entire objects view (list and tree) to reflect the modification
	updateObjectsView();
}

void ModelObjectsWidget::setAllObjectsVisible(bool value)
{
	ObjectType obj_type;
	QListWidgetItem *item=nullptr;
	bool checked;

	checked=(sender()==select_all_tb || value);
	for(int i=0; i < visibleobjects_lst->count(); i++)
	{
		item=visibleobjects_lst->item(i);
		obj_type=static_cast<ObjectType>(item->data(Qt::UserRole).toInt());
		visible_objs_map[obj_type]=checked;
		item->setCheckState((checked ? Qt::Checked : Qt::Unchecked));
	}

	updateObjectsView();
}

void ModelObjectsWidget::changeObjectsView()
{
	if(sender()==tree_view_tb || sender()==list_view_tb)
	{
		visaoobjetos_stw->setCurrentIndex(sender()==tree_view_tb ? 0 : 1);
		tree_view_tb->setChecked(sender()==tree_view_tb);
		list_view_tb->setChecked(sender()==list_view_tb);
		by_id_chk->setEnabled(sender()==tree_view_tb);
	}
	else if(sender()==options_tb)
	{
		filter_wgt->setVisible(options_tb->isChecked());
		visibleobjects_grp->setVisible(options_tb->isChecked());
		splitter->handle(1)->setEnabled(options_tb->isChecked());

		//Restore the splitter state if the options toolbutton is not toggled
		if(!options_tb->isChecked())
			splitter->restoreState(widgets_conf.value(QString("splitterSize")).toByteArray());
	}

	expand_all_tb->setEnabled(tree_view_tb->isChecked());
	collapse_all_tb->setEnabled(tree_view_tb->isChecked());
}

void ModelObjectsWidget::collapseAll()
{
	QTreeWidgetItem *root=objectstree_tw->topLevelItem(0);
	objectstree_tw->collapseAll();

	if(root)
		root->setExpanded(true);
}

void ModelObjectsWidget::filterObjects()
{
	if(tree_view_tb->isChecked())
	{
		DatabaseImportForm::filterObjects(objectstree_tw, filter_edt->text(), (by_id_chk->isChecked() ? 1 : 0), simplified_view);
	}
	else
	{
		QList<QTableWidgetItem*> items=objectslist_tbw->findItems(filter_edt->text(), Qt::MatchStartsWith | Qt::MatchRecursive);

		objectslist_tbw->blockSignals(true);
		for(int row=0; row < objectslist_tbw->rowCount(); row++)
			objectslist_tbw->setRowHidden(row, true);

		while(!items.isEmpty())
		{
			objectslist_tbw->setRowHidden(items.front()->row(), false);
			items.pop_front();
		}

		objectslist_tbw->blockSignals(false);
	}
}

void ModelObjectsWidget::updateObjectsView()
{
	selected_objs.clear();
	updateDatabaseTree();
	updateObjectsList();

	if(!filter_edt->text().isEmpty())
		filterObjects();
}

void ModelObjectsWidget::updateObjectsList()
{
	vector<BaseObject *> objects;

	if(db_model)
	{
		vector<ObjectType> visible_types;

		for(auto &tp : visible_objs_map)
		{
			if(tp.second)
				visible_types.push_back(tp.first);
		}

		objects = db_model->findObjects(QString(), visible_types, false, false, false);
	}

	ObjectFinderWidget::updateObjectTable(objectslist_tbw, objects);
	objectslist_tbw->clearSelection();
}

void ModelObjectsWidget::updateSchemaTree(QTreeWidgetItem *root)
{
	if(db_model && visible_objs_map[ObjectType::Schema])
	{
		BaseObject *schema=nullptr;
		vector<BaseObject *> obj_list;
		QFont font;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr, *item3=nullptr;
		vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::Schema);
		int count = 0, count2 = 0, i = 0;
		QPixmap group_icon=QPixmap(PgModelerUiNs::getIconPath(QString(BaseObject::getSchemaName(ObjectType::Schema)) + QString("_grp")));

		//Removing the ObjectType::Table and ObjectType::View types since they are handled separetedly
		types.erase(std::find(types.begin(), types.end(), ObjectType::Table));
		types.erase(std::find(types.begin(), types.end(), ObjectType::ForeignTable));
		types.erase(std::find(types.begin(), types.end(), ObjectType::View));

		//Get the current schema count on database
		count=(db_model->getObjectCount(ObjectType::Schema));
		item=new QTreeWidgetItem(root);
		item->setIcon(0,group_icon);
		item->setData(1, Qt::UserRole, QVariant(enum_cast(ObjectType::Schema)));

		//Create the schema group item
		item->setText(0, QString("%1 (%2)").arg(BaseObject::getTypeName(ObjectType::Schema)).arg(count));
		font=item->font(0);
		font.setItalic(true);
		item->setFont(0, font);

		try
		{
			for(i=0; i < count; i++)
			{
				//The first item is the public schema
				if(i==-1)
				{
					//The new sub item to be configured will be the public schema item
					item2=item1;
					schema=nullptr;
				}
				else
				{
					schema=db_model->getObject(i, ObjectType::Schema);
					item2=createItemForObject(schema, item);
				}

				//Updates the table subtree for the current schema
				updateTableTree(item2, schema, ObjectType::Table);

				//Updates the foreign table subtree for the current schema
				updateTableTree(item2, schema, ObjectType::ForeignTable);

				//Updates the view subtree for the current schema
				updateViewTree(item2, schema);

				//Creates the object group at schema level (function, domain, sequences, etc)
				for(auto type : types)
				{
					if(visible_objs_map[type])
					{
						item3=new QTreeWidgetItem(item2);
						item3->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(type) + QString("_grp"))));

						//Get the objects that belongs to the current schema
						obj_list=db_model->getObjects(type, schema);

						count2=obj_list.size();
						item3->setText(0, QString("%1 (%2)").arg(BaseObject::getTypeName(type)).arg(count2));
						item3->setData(1, Qt::UserRole, QVariant(enum_cast(type)));

						font=item3->font(0);
						font.setItalic(true);
						item3->setFont(0, font);

						for(auto obj : obj_list)
							createItemForObject(obj, item3);
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateTableTree(QTreeWidgetItem *root, BaseObject *schema, ObjectType table_type)
{
	if(db_model && PhysicalTable::isPhysicalTable(table_type) && visible_objs_map[table_type])
	{
		vector<BaseObject *> obj_list;
		PhysicalTable *table=nullptr;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr;
		QFont font;
		vector<ObjectType> types = BaseObject::getChildObjectTypes(table_type);
		QPixmap group_icon=QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(table_type) + QString("_grp")));

		try
		{
			//Get all tables that belongs to the specified schema
			obj_list=db_model->getObjects(table_type, schema);

			//Create a table group item
			item=new QTreeWidgetItem(root);
			item->setIcon(0,group_icon);
			item->setText(0,BaseObject::getTypeName(table_type) +
						  QString(" (%1)").arg(obj_list.size()));
			item->setData(1, Qt::UserRole, QVariant(enum_cast(table_type)));

			font=item->font(0);
			font.setItalic(true);
			item->setFont(0, font);

			for(auto obj : obj_list)
			{
				table=dynamic_cast<PhysicalTable *>(obj);
				item1=createItemForObject(table, item);

				//Creating the group for the child objects (column, rules, triggers, indexes and constraints)
				for(auto type : types)
				{
					if(visible_objs_map[type])
					{
						item2=new QTreeWidgetItem(item1);
						item2->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(type) + QString("_grp"))));
						font=item2->font(0);
						font.setItalic(true);
						item2->setFont(0, font);

						item2->setText(0, QString("%1 (%2)").arg(BaseObject::getTypeName(type)).arg(table->getObjectCount(type)));

						for(auto tab_obj : *table->getObjectList(type))
							createItemForObject(tab_obj, item2);
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateViewTree(QTreeWidgetItem *root, BaseObject *schema)
{
	if(db_model && visible_objs_map[ObjectType::View])
	{
		BaseObject *object=nullptr;
		vector<BaseObject *> obj_list;
		View *view=nullptr;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr;
		QFont font;
		vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::View);
		int count = 0, count1 = 0, i = 0, i2 = 0;
		QPixmap group_icon=QPixmap(PgModelerUiNs::getIconPath(QString(BaseObject::getSchemaName(ObjectType::View)) + QString("_grp")));

		try
		{
			//Get all views that belongs to the specified schema
			obj_list=db_model->getObjects(ObjectType::View, schema);

			//Create a table group item
			item=new QTreeWidgetItem(root);
			item->setIcon(0,group_icon);
			item->setText(0,BaseObject::getTypeName(ObjectType::View) + QString(" (%1)").arg(obj_list.size()));
			item->setData(1, Qt::UserRole, QVariant(enum_cast(ObjectType::View)));

			font=item->font(0);
			font.setItalic(true);
			item->setFont(0, font);

			count=obj_list.size();

			for(i=0; i < count; i++)
			{
				view=dynamic_cast<View *>(obj_list[i]);
				item1=createItemForObject(view, item);

				//Creating the group for the child objects (rules, triggers)
				for(auto &type : types)
				{
					if(visible_objs_map[type])
					{
						item2=new QTreeWidgetItem(item1);
						item2->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(type) + QString("_grp"))));
						font=item2->font(0);
						font.setItalic(true);
						item2->setFont(0, font);

						count1 = view->getObjectCount(type);
						item2->setText(0,BaseObject::getTypeName(type) + QString(" (%1)").arg(count1));

						for(i2=0; i2 < count1; i2++)
						{
							object=view->getObject(i2, type);
							createItemForObject(object, item2);
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updatePermissionTree(QTreeWidgetItem *root, BaseObject *object)
{
	try
	{
		if(db_model && visible_objs_map[ObjectType::Permission] &&
				Permission::acceptsPermission(object->getObjectType()))
		{
			vector<Permission *> perms;
			QTreeWidgetItem *item=new QTreeWidgetItem(root);
			QFont font=item->font(0);

			db_model->getPermissions(object, perms);
			item->setIcon(0,QPixmap(PgModelerUiNs::getIconPath("permission_grp")));

			font.setItalic(true);
			item->setFont(0, font);
			item->setText(0, QString("%1 (%2)")
							.arg(BaseObject::getTypeName(ObjectType::Permission))
						  .arg(perms.size()));

			item->setData(0, Qt::UserRole, generateItemValue(object));
			item->setData(1, Qt::UserRole, static_cast<unsigned>(ObjectType::Permission));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelObjectsWidget::updateDatabaseTree()
{
	if(!db_model)
		objectstree_tw->clear();
	else
	{
		QString str_aux;
		BaseObject *object=nullptr;
		QTreeWidgetItem *root=nullptr,*item1=nullptr, *item2=nullptr;
		QFont font;
		vector<BaseObject *> ref_list, tree_state, obj_list;
		vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::Database);
		unsigned count = 0, i = 0, i1 = 0;

		types.push_back(ObjectType::Tag);
		types.push_back(ObjectType::GenericSql);
		types.push_back(ObjectType::Textbox);
		types.push_back(ObjectType::Relationship);
		types.erase(std::find(types.begin(), types.end(), ObjectType::Schema));

		try
		{
			if(save_tree_state)
				saveTreeState(tree_state);

			objectstree_tw->setUpdatesEnabled(false);
			objectstree_tw->clear();

			if(visible_objs_map[ObjectType::Database])
			{
				root=createItemForObject(db_model);
				objectstree_tw->insertTopLevelItem(0,root);

				updateSchemaTree(root);

				for(auto &type : types)
				{
					if(visible_objs_map[type])
					{
						item1=new QTreeWidgetItem(root);
						str_aux=QString(BaseObject::getSchemaName(type));

						item1->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(str_aux + QString("_grp"))));
						item1->setData(1, Qt::UserRole, QVariant(enum_cast(type)));

						obj_list=(*db_model->getObjectList(type));

						//Special case for relationship, merging the base relationship list to the relationship list
						if(type==ObjectType::Relationship)
						{
							vector<BaseObject *> obj_list_aux;
							obj_list_aux=(*db_model->getObjectList(ObjectType::BaseRelationship));
							obj_list.insert(obj_list.end(), obj_list_aux.begin(), obj_list_aux.end());
						}

						count=obj_list.size();
						item1->setText(0,BaseObject::getTypeName(type) + QString(" (%1)").arg(count));
						font=item1->font(0);
						font.setItalic(true);
						item1->setFont(0, font);

						for(i1=0; i1 < count; i1++)
						{
							object=obj_list.at(i1);
							item2=createItemForObject(object, item1);

							if(types[i]==ObjectType::Tag)
							{
								db_model->getObjectReferences(object, ref_list);

								for(auto &ref : ref_list)
									createItemForObject(ref, item2, false);
							}
						}
					}
				}

				objectstree_tw->setUpdatesEnabled(true);
				objectstree_tw->expandItem(root);

				if(save_tree_state)
					restoreTreeState(tree_state);
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		objectstree_tw->sortByColumn(0, Qt::AscendingOrder);
	}
}

BaseObject *ModelObjectsWidget::getSelectedObject()
{
	if(!simplified_view || selected_objs.size() != 1)
		return nullptr;

	return selected_objs[0];
}

void ModelObjectsWidget::enableObjectCreation(bool value)
{
  enable_obj_creation=value;
}

void ModelObjectsWidget::close()
{
	QObject *obj_sender=sender();

	if(obj_sender == cancel_tb)
		selected_objs.clear();
	else
	{
		QVariant data;
		BaseObject *selected_obj = nullptr;

		if(tree_view_tb->isChecked() && objectstree_tw->currentItem())
			data = objectstree_tw->currentItem()->data(0,Qt::UserRole);
		else if(objectslist_tbw->currentItem())
			data = objectslist_tbw->currentItem()->data(Qt::UserRole);

		selected_obj = reinterpret_cast<BaseObject *>(data.value<void *>());

		if(selected_obj && std::find(selected_objs.begin(), selected_objs.end(), selected_obj) == selected_objs.end())
			selected_objs.push_back(selected_obj);
	}

	QWidget::close();
}

void ModelObjectsWidget::setModel(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;

	if(model_wgt)
		setModel(model_wgt->db_model);
	else
		setModel(static_cast<DatabaseModel *>(nullptr));
}

void ModelObjectsWidget::setModel(DatabaseModel *db_model)
{
	bool enable = (db_model!=nullptr);

	this->db_model=db_model;
	content_wgt->setEnabled(enable);
	updateObjectsView();
	visaoobjetos_stw->setEnabled(true);
	expand_all_tb->setEnabled(enable && tree_view_tb->isChecked());
	collapse_all_tb->setEnabled(enable && tree_view_tb->isChecked());
	tree_view_tb->setEnabled(enable);
	list_view_tb->setEnabled(enable);
	options_tb->setEnabled(enable);
	filter_lbl->setEnabled(enable);
	filter_edt->setEnabled(enable);
	by_id_chk->setEnabled(enable);
}

void ModelObjectsWidget::showEvent(QShowEvent *)
{
	if(simplified_view)
	{
		QWidget *wgt=QApplication::activeWindow();

		filter_edt->setFocus();
		filter_edt->blockSignals(true);
		by_id_chk->blockSignals(true);
		filter_edt->clear();
		by_id_chk->setChecked(false);
		filter_edt->blockSignals(false);
		by_id_chk->blockSignals(false);

		if(wgt)
		{
			int x, y;
			x = wgt->pos().x() + abs((wgt->width() - this->width()) / 2);
			y = wgt->pos().y() + abs((wgt->height() - this->height()) / 2);
			this->setGeometry(QRect(QPoint(x,y), this->minimumSize()));
		}
	}
}

void ModelObjectsWidget::closeEvent(QCloseEvent *)
{
	if(simplified_view)
	{
		map<ObjectType, bool>::iterator itr, itr_end;

		itr=visible_objs_map.begin();
		itr_end=visible_objs_map.end();

		while(itr!=itr_end)
		{
			itr->second=false;
			itr++;
		}

		this->resize(this->minimumSize());
	}

	emit s_visibilityChanged(getSelectedObject(), !this->isVisible());
}

void ModelObjectsWidget::mouseMoveEvent(QMouseEvent *)
{
	static QPoint pos=QCursor::pos(), pos1=QCursor::pos();

	pos=pos1;
	pos1=QCursor::pos();

	if(simplified_view && QApplication::mouseButtons()==Qt::LeftButton)
	{
		QPoint pos_dif;
		QDesktopWidget desktop;
		QRect ret=qApp->screens().at(desktop.screenNumber(this))->geometry();
		int px, py;

		pos_dif=pos1-pos;
		px=this->pos().x() + pos_dif.x();
		py=this->pos().y() + pos_dif.y();

		if(px<0)
			px=0;
		else if((px + this->width()) > ret.right())
			px=ret.right() - this->width();

		if(py<0)
			py=0;
		else if((py + this->height()) > ret.bottom())
			py=ret.bottom() - this->height();

		this->move(px,py);
	}
}

void ModelObjectsWidget::resizeEvent(QResizeEvent *)
{
	objectstree_tw->header()->setMinimumSectionSize(objectstree_tw->width());
	objectstree_tw->header()->setDefaultSectionSize(objectstree_tw->width());
}

void ModelObjectsWidget::saveTreeState(bool value)
{
	save_tree_state=(!simplified_view && value);
}

void ModelObjectsWidget::clearSelectedObject()
{
	objectstree_tw->blockSignals(true);
	objectslist_tbw->blockSignals(true);
	objectstree_tw->clearSelection();
	objectslist_tbw->clearSelection();
	objectstree_tw->blockSignals(false);
	objectslist_tbw->blockSignals(false);
	selected_objs.clear();
	model_wgt->configurePopupMenu(nullptr);
	model_wgt->emitSceneInteracted();
}

void ModelObjectsWidget::saveTreeState(vector<BaseObject *> &tree_items)
{
	QTreeWidgetItemIterator itr(objectstree_tw);
	BaseObject *obj=nullptr;
	QTreeWidgetItem *item=nullptr;

	while(*itr)
	{
		item=*itr;
		obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

		if(obj && item->parent() && item->parent()->isExpanded())
			tree_items.push_back(obj);

		++itr;
	}
}

void ModelObjectsWidget::restoreTreeState(vector<BaseObject *> &tree_items)
{
	QTreeWidgetItem *item=nullptr, *parent_item=nullptr;

	objectslist_tbw->setUpdatesEnabled(false);

	while(!tree_items.empty())
	{
		item=getTreeItem(tree_items.back());

		if(item)
		{
			parent_item=item->parent();

			if(parent_item)
				objectstree_tw->expandItem(parent_item);

			if(parent_item && parent_item->parent())
				objectstree_tw->expandItem(parent_item->parent());
		}

		tree_items.pop_back();
	}

	objectslist_tbw->setUpdatesEnabled(true);
}

QTreeWidgetItem *ModelObjectsWidget::getTreeItem(BaseObject *object)
{
	if(object)
	{
		QTreeWidgetItemIterator itr(objectstree_tw);
		BaseObject *aux_obj=nullptr;
		QTreeWidgetItem *item=nullptr;

		while(*itr)
		{
			aux_obj=reinterpret_cast<BaseObject *>((*itr)->data(0,Qt::UserRole).value<void *>());

			if(aux_obj==object)
			{
				item=*itr;
				break;
			}

			++itr;
		}

		return item;
	}
	else
		return nullptr;
}

void ModelObjectsWidget::selectCreatedObject(BaseObject *obj)
{
	updateObjectsView();
	QTreeWidgetItem *item = getTreeItem(obj);

	if(item)
	{
		objectstree_tw->blockSignals(true);
		item->setSelected(true);
		objectstree_tw->setCurrentItem(item);
		objectstree_tw->scrollToItem(item);
		select_tb->setFocus();
		objectstree_tw->blockSignals(false);
	}
}
