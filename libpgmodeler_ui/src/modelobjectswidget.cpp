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

#include "modelobjectswidget.h"

ModelObjectsWidget::ModelObjectsWidget(bool simplified_view, QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	model_wgt=nullptr;
	db_model=nullptr;
	setModel(db_model);

	title_wgt->setVisible(!simplified_view);
	this->simplified_view=simplified_view;
	this->save_tree_state=!simplified_view;

	select_tb->setVisible(simplified_view);
	cancel_tb->setVisible(simplified_view);
	options_tb->setVisible(!simplified_view);
	visibleobjects_grp->setVisible(false);

	selected_object=nullptr;
	splitter->handle(1)->setEnabled(false);

	connect(objectstree_tw,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this, SLOT(selectObject(void)));
	connect(objectslist_tbw,SIGNAL(itemPressed(QTableWidgetItem*)),this, SLOT(selectObject(void)));
	connect(expand_all_tb, SIGNAL(clicked(void)), objectstree_tw, SLOT(expandAll(void)));
	connect(collapse_all_tb, SIGNAL(clicked(void)), this, SLOT(collapseAll(void)));

	if(!simplified_view)
	{
		widgets_conf.setValue("splitterSize", splitter->saveState());
		connect(options_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
		connect(visibleobjects_lst,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setObjectVisible(QListWidgetItem*)));
		connect(select_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(clear_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(editObject(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(editObject(void)));
		connect(hide_tb, SIGNAL(clicked(bool)), this, SLOT(hide(void)));

		ObjectFinderWidget::updateObjectTypeList(visibleobjects_lst);
		setAllObjectsVisible(true);
	}
	else
	{
		setWindowModality(Qt::ApplicationModal);
		setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(close(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(close(void)));
		connect(select_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
		connect(cancel_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
	}

	connect(tree_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
	connect(list_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
}

void ModelObjectsWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}


void ModelObjectsWidget::showObjectMenu(void)
{
	if(selected_object && QApplication::mouseButtons()==Qt::RightButton && model_wgt && !simplified_view)
		model_wgt->popup_menu.exec(QCursor::pos());
}

void ModelObjectsWidget::editObject(void)
{
	if(selected_object && model_wgt && !simplified_view)
	{
		//If the user double-clicked the item "Permission (n)" on tree view
		if(sender()==objectstree_tw && objectstree_tw->currentItem() &&
			 objectstree_tw->currentItem()->data(1, Qt::UserRole).toUInt()==OBJ_PERMISSION)
			model_wgt->showObjectForm(OBJ_PERMISSION,
																reinterpret_cast<BaseObject *>(objectstree_tw->currentItem()->data(0, Qt::UserRole).value<void *>()));
		//If the user double-clicked a permission on  list view
		else if(sender()==objectslist_tbw && objectslist_tbw->currentRow() >= 0)
		{
			BaseObject *obj=reinterpret_cast<Permission *>(objectslist_tbw->item(objectslist_tbw->currentRow(), 0)->data(Qt::UserRole).value<void *>());
			Permission *perm=dynamic_cast<Permission *>(obj);

			if(perm)
				model_wgt->showObjectForm(OBJ_PERMISSION,perm->getObject());
		}
		else
			model_wgt->editObject();
	}
}

void ModelObjectsWidget::selectObject(void)
{
	ObjectType obj_type=BASE_OBJECT;

	if(tree_view_tb->isChecked())
	{
		QTreeWidgetItem *tree_item=objectstree_tw->currentItem();

		if(tree_item)
		{
			obj_type=static_cast<ObjectType>(tree_item->data(1,Qt::UserRole).toUInt());
			selected_object=reinterpret_cast<BaseObject *>(tree_item->data(0,Qt::UserRole).value<void *>());
		}

		//If user select a group item popups a "New [OBJECT]" menu
		if(!selected_object && QApplication::mouseButtons()==Qt::RightButton &&
			 obj_type!=OBJ_COLUMN && obj_type!=OBJ_CONSTRAINT && obj_type!=OBJ_RULE &&
			 obj_type!=OBJ_INDEX && obj_type!=OBJ_TRIGGER && obj_type!=OBJ_PERMISSION)
		{
			QAction act(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")),
									trUtf8("New") + " " + BaseObject::getTypeName(obj_type), nullptr);
			QMenu popup;

			//If not a relationship, connect the action to the addNewObject method of the model wiget
			if(obj_type!=OBJ_RELATIONSHIP)
			{
				act.setData(QVariant(obj_type));
				connect(&act, SIGNAL(triggered()), model_wgt, SLOT(addNewObject()));
			}
			//Case is a relationship, insert the relationship menu of the model wiget into the action
			else
				act.setMenu(model_wgt->rels_menu);

			popup.addAction(&act);
			popup.exec(QCursor::pos());
			disconnect(&act,nullptr,model_wgt,nullptr);
		}
	}
	else
	{
		QTableWidgetItem *tab_item=objectslist_tbw->item(objectslist_tbw->currentRow(), 0);

		if(tab_item)
		{
			selected_object=reinterpret_cast<BaseObject *>(tab_item->data(Qt::UserRole).value<void *>());
			obj_type=selected_object->getObjectType();
		}
	}

	if(obj_type!=OBJ_PERMISSION && selected_object && !simplified_view)
	{
		vector<BaseObject *> vect;

		vect.push_back(selected_object);
		model_wgt->scene->clearSelection();
		model_wgt->configurePopupMenu(vect);
		showObjectMenu();
	}
}

QVariant ModelObjectsWidget::generateItemValue(BaseObject *object)
{
	return(QVariant::fromValue(reinterpret_cast<void *>(object)));
}

void ModelObjectsWidget::setObjectVisible(ObjectType obj_type, bool visible)
{
	if(obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE)
		visible_objs_map[obj_type]=visible;

	if(visible && simplified_view)
	{
		if(obj_type!=OBJ_DATABASE)
			visible_objs_map[OBJ_DATABASE]=true;

		if(PgModelerNS::isTableObject(obj_type))
			visible_objs_map[OBJ_TABLE]=visible_objs_map[OBJ_SCHEMA]=true;

		if(BaseObject::acceptsSchema(obj_type))
			visible_objs_map[OBJ_SCHEMA]=true;
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

void ModelObjectsWidget::changeObjectsView(void)
{
	if(sender()==list_view_tb)
	{
		if(!list_view_tb->isChecked())
			list_view_tb->setChecked(true);
		else
		{
			tree_view_tb->setChecked(false);
			visaoobjetos_stw->setCurrentIndex(1);
		}
	}
	else if(sender()==tree_view_tb)
	{
		if(!tree_view_tb->isChecked())
			tree_view_tb->setChecked(true);
		else
		{
			list_view_tb->setChecked(false);
			visaoobjetos_stw->setCurrentIndex(0);
		}
	}
	else if(sender()==options_tb)
	{
		visibleobjects_grp->setVisible(options_tb->isChecked());
		splitter->handle(1)->setEnabled(options_tb->isChecked());

		//Restore the splitter state if the options toolbutton is not toggled
		if(!options_tb->isChecked())
			splitter->restoreState(widgets_conf.value("splitterSize").toByteArray());
	}

	expand_all_tb->setEnabled(tree_view_tb->isChecked());
	collapse_all_tb->setEnabled(tree_view_tb->isChecked());
}

void ModelObjectsWidget::collapseAll(void)
{
	QTreeWidgetItem *root=objectstree_tw->topLevelItem(0);
	objectstree_tw->collapseAll();

	if(root)
		root->setExpanded(true);
}

void ModelObjectsWidget::updateObjectsView(void)
{
	updateDatabaseTree();
	updateObjectsList();
}

void ModelObjectsWidget::updateObjectsList(void)
{
	while(objectslist_tbw->rowCount() > 0)
		objectslist_tbw->removeRow(objectslist_tbw->rowCount()-1);

	if(db_model)
	{
		vector<BaseObject *> objects=db_model->findObjects("", BaseObject::getObjectTypes());
		ObjectFinderWidget::updateObjectTable(objectslist_tbw, objects);
	}
}

void ModelObjectsWidget::updateSchemaTree(QTreeWidgetItem *root)
{
	if(db_model && visible_objs_map[OBJ_SCHEMA])
	{
		BaseObject *object=nullptr, *schema=nullptr;
		Function *func=nullptr;
		Operator *oper=nullptr;
		vector<BaseObject *> obj_list;
		QFont font;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr, *item3=nullptr, *item4=nullptr;
		ObjectType types[]={ OBJ_FUNCTION, OBJ_AGGREGATE,
												 OBJ_DOMAIN, OBJ_TYPE, OBJ_CONVERSION,
												 OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
												 OBJ_SEQUENCE, OBJ_COLLATION, OBJ_EXTENSION };
		int count, count2, type_cnt=sizeof(types)/sizeof(ObjectType), i, i1, i2;

		QPixmap sch_icon=QPixmap(QString(":/icones/icones/") +
															QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
															QString(".png")),

				group_icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
														QString("_grp") +
														QString(".png"));

		//Get the current schema count on database
		count=(db_model->getObjectCount(OBJ_SCHEMA));
		item=new QTreeWidgetItem(root);
		item->setIcon(0,group_icon);
		item->setData(1, Qt::UserRole, QVariant::fromValue<unsigned>(OBJ_SCHEMA));

		//Create the schema group item
		item->setText(0,BaseObject::getTypeName(OBJ_SCHEMA) +
									QString(" (%1)").arg(count));
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
					schema=db_model->getObject(i,OBJ_SCHEMA);
					item2=new QTreeWidgetItem(item);
					item2->setText(0,Utf8String::create(schema->getName()));
					item2->setToolTip(0,Utf8String::create(schema->getName()));
					item2->setIcon(0,sch_icon);
					item2->setData(0, Qt::UserRole, generateItemValue(schema));
					updatePermissionTree(item2, schema);
				}

				if(schema && schema->isProtected())
				{
					font=item2->font(0);
					font.setItalic(true);
					item2->setFont(0,font);
					item2->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Updates the table subtree for the current schema
				updateTableTree(item2, schema);

				//Updates the view subtree for the current schema
				updateViewTree(item2, schema);

				//Creates the object group at schema level (function, domain, sequences, etc)
				for(i1=0; i1 < type_cnt; i1++)
				{
					if(visible_objs_map[types[i1]])
					{
						item3=new QTreeWidgetItem(item2);
						item3->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 QString(BaseObject::getSchemaName(types[i1])) +
																		 QString("_grp") +
																		 QString(".png")));

						//Get the objects that belongs to the current schema
						obj_list=db_model->getObjects(types[i1], schema);

						count2=obj_list.size();
						item3->setText(0,
													 BaseObject::getTypeName(types[i1]) +
													 QString(" (%1)").arg(count2));
						item3->setData(1, Qt::UserRole, QVariant::fromValue<unsigned>(types[i1]));

						font=item3->font(0);
						font.setItalic(true);
						item3->setFont(0, font);

						for(i2=0; i2 < count2; i2++)
						{
							object=obj_list[i2];
							item4=new QTreeWidgetItem(item3);
							item4->setData(0, Qt::UserRole, generateItemValue(object));

							updatePermissionTree(item4, object);

							if(object->isProtected())
							{
								font=item4->font(0);
								font.setItalic(true);
								item4->setFont(0,font);
								item4->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							if(types[i1]==OBJ_FUNCTION)
							{
								func=dynamic_cast<Function *>(object);
								func->createSignature(false);
								item4->setText(0,Utf8String::create(func->getSignature()));
								item4->setToolTip(0,Utf8String::create(func->getSignature()));
								func->createSignature(true);
							}
							else if(types[i1]==OBJ_OPERATOR)
							{
								oper=dynamic_cast<Operator *>(object);
								item4->setText(0, Utf8String::create(oper->getSignature(false)));
								item4->setToolTip(0, Utf8String::create(oper->getSignature(false)));
							}
							else
							{
								item4->setText(0,Utf8String::create(object->getName()));
								item4->setToolTip(0,Utf8String::create(object->getName()));
							}

							item4->setIcon(0,QPixmap(QString(":/icones/icones/") +
																			 QString(BaseObject::getSchemaName(types[i1])) +
																			 QString(".png")));
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateTableTree(QTreeWidgetItem *root, BaseObject *schema)
{
	if(db_model && visible_objs_map[OBJ_TABLE])
	{
		BaseObject *object=nullptr;
		vector<BaseObject *> obj_list;
		Table *table=nullptr;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr, *item3=nullptr;
		QString str_aux;
		QFont font;
		ConstraintType constr_type;
		ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE,
												 OBJ_TRIGGER, OBJ_INDEX };
		int count, count1, type_cnt=sizeof(types)/sizeof(ObjectType), i, i1, i2;
		QPixmap group_icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_TABLE)) +
														QString("_grp") + QString(".png"));

		try
		{
			//Get all tables that belongs to the specified schema
			obj_list=db_model->getObjects(OBJ_TABLE, schema);

			//Create a table group item
			item=new QTreeWidgetItem(root);
			item->setIcon(0,group_icon);
			item->setText(0,BaseObject::getTypeName(OBJ_TABLE) +
										QString(" (%1)").arg(obj_list.size()));
			item->setData(1, Qt::UserRole, QVariant::fromValue<unsigned>(OBJ_TABLE));

			font=item->font(0);
			font.setItalic(true);
			item->setFont(0, font);

			count=obj_list.size();
			for(i=0; i < count; i++)
			{
				table=dynamic_cast<Table *>(obj_list[i]);

				item1=new QTreeWidgetItem(item);
				item1->setText(0,Utf8String::create(table->getName()));
				item1->setToolTip(0,Utf8String::create(table->getName()));

				item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																 QString(BaseObject::getSchemaName(OBJ_TABLE)) +
																 QString(".png")));
				item1->setData(0, Qt::UserRole, generateItemValue(table));

				updatePermissionTree(item1, table);

				if(table->isProtected())
				{
					font=item1->font(0);
					font.setItalic(true);
					item1->setFont(0,font);
					item1->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Creating the group for the child objects (column, rules, triggers, indexes and constraints)
				for(i1=0; i1 < type_cnt; i1++)
				{
					if(visible_objs_map[types[i1]])
					{
						item2=new QTreeWidgetItem(item1);
						item2->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 QString(BaseObject::getSchemaName(types[i1])) +
																		 QString("_grp") +
																		 QString(".png")));
						font=item2->font(0);
						font.setItalic(true);
						item2->setFont(0, font);

						count1=table->getObjectCount(types[i1]);
						item2->setText(0,BaseObject::getTypeName(types[i1]) +
													 QString(" (%1)").arg(count1));

						for(i2=0; i2 < count1; i2++)
						{
							object=table->getObject(i2,types[i1]);

							item3=new QTreeWidgetItem(item2);
							item3->setText(0,Utf8String::create(object->getName()));
							item3->setToolTip(0,Utf8String::create(object->getName()));
							item3->setData(0, Qt::UserRole, generateItemValue(object));

							updatePermissionTree(item3, object);

							if(dynamic_cast<TableObject *>(object)->isAddedByRelationship())
							{
								font=item3->font(0);
								font.setItalic(true);
								item3->setFont(0,font);
								item3->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::INH_COLUMN).foreground());
							}
							else if(object->isProtected())
							{
								font=item3->font(0);
								font.setItalic(true);
								item3->setFont(0,font);
								item3->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							switch(types[i1])
							{
								case OBJ_CONSTRAINT:
									constr_type=dynamic_cast<Constraint *>(object)->getConstraintType();
									if(constr_type==ConstraintType::primary_key)
										str_aux=QString("_%1").arg(TableObjectView::TXT_PRIMARY_KEY);
									else if(constr_type==ConstraintType::foreign_key)
										str_aux=QString("_%1").arg(TableObjectView::TXT_FOREIGN_KEY);
									else if(constr_type==ConstraintType::check)
										str_aux=QString("_%1").arg(TableObjectView::TXT_CHECK);
									else if(constr_type==ConstraintType::unique)
										str_aux=QString("_%1").arg(TableObjectView::TXT_UNIQUE);
									else if(constr_type==ConstraintType::exclude)
										str_aux=QString("_%1").arg(TableObjectView::TXT_EXCLUDE);
								break;

								default:
									str_aux="";
								break;
							}

							str_aux=QString(BaseObject::getSchemaName(types[i1])) + str_aux;
							item3->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateViewTree(QTreeWidgetItem *root, BaseObject *schema)
{
	if(db_model && visible_objs_map[OBJ_VIEW])
	{
		BaseObject *object=nullptr;
		vector<BaseObject *> obj_list;
		View *view=nullptr;
		QTreeWidgetItem *item=nullptr, *item1=nullptr, *item2=nullptr, *item3=nullptr;
		QFont font;
		ObjectType types[]={ OBJ_RULE, OBJ_TRIGGER };
		int count, count1, type_cnt=sizeof(types)/sizeof(ObjectType), i, i1, i2;
		QPixmap group_icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_VIEW)) +
														QString("_grp") + QString(".png"));

		try
		{
			//Get all views that belongs to the specified schema
			obj_list=db_model->getObjects(OBJ_VIEW, schema);

			//Create a table group item
			item=new QTreeWidgetItem(root);
			item->setIcon(0,group_icon);
			item->setText(0,BaseObject::getTypeName(OBJ_VIEW) +
										QString(" (%1)").arg(obj_list.size()));
			item->setData(1, Qt::UserRole, QVariant::fromValue<unsigned>(OBJ_VIEW));

			font=item->font(0);
			font.setItalic(true);
			item->setFont(0, font);

			count=obj_list.size();
			for(i=0; i < count; i++)
			{
				view=dynamic_cast<View *>(obj_list[i]);

				item1=new QTreeWidgetItem(item);
				item1->setText(0,Utf8String::create(view->getName()));
				item1->setToolTip(0,Utf8String::create(view->getName()));

				item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																 QString(BaseObject::getSchemaName(OBJ_VIEW)) +
																 QString(".png")));
				item1->setData(0, Qt::UserRole, generateItemValue(view));

				updatePermissionTree(item1, view);

				if(view->isProtected())
				{
					font=item1->font(0);
					font.setItalic(true);
					item1->setFont(0,font);
					item1->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Creating the group for the child objects (rules, triggers)
				for(i1=0; i1 < type_cnt; i1++)
				{
					if(visible_objs_map[types[i1]])
					{
						item2=new QTreeWidgetItem(item1);
						item2->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 QString(BaseObject::getSchemaName(types[i1])) +
																		 QString("_grp") +
																		 QString(".png")));
						font=item2->font(0);
						font.setItalic(true);
						item2->setFont(0, font);

						count1=view->getObjectCount(types[i1]);
						item2->setText(0,BaseObject::getTypeName(types[i1]) +
													 QString(" (%1)").arg(count1));

						for(i2=0; i2 < count1; i2++)
						{
							object=view->getObject(i2,types[i1]);

							item3=new QTreeWidgetItem(item2);
							item3->setText(0,Utf8String::create(object->getName()));
							item3->setToolTip(0,Utf8String::create(object->getName()));
							item3->setData(0, Qt::UserRole, generateItemValue(object));


							if(object->isProtected())
							{
								font=item3->font(0);
								font.setItalic(true);
								item3->setFont(0,font);
								item3->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							item3->setIcon(0,QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i1]) + QString(".png")));
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updatePermissionTree(QTreeWidgetItem *root, BaseObject *object)
{
	try
	{
		if(db_model && visible_objs_map[OBJ_PERMISSION] &&
			 Permission::objectAcceptsPermission(object->getObjectType()))
		{
			vector<Permission *> perms;
			QTreeWidgetItem *item=new QTreeWidgetItem(root);
			QFont font=item->font(0);

			db_model->getPermissions(object, perms);
			item->setIcon(0,QPixmap(":/icones/icones/permission_grp.png"));

			font.setItalic(true);
			item->setFont(0, font);
			item->setText(0, QString("%1 (%2)")
										.arg(Utf8String::create(BaseObject::getTypeName(OBJ_PERMISSION)))
										.arg(perms.size()));

			item->setData(0, Qt::UserRole, generateItemValue(object));
			item->setData(1, Qt::UserRole, static_cast<unsigned>(OBJ_PERMISSION));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelObjectsWidget::updateDatabaseTree(void)
{
	if(!db_model)
		objectstree_tw->clear();
	else
	{
		QString str_aux;
		BaseObject *object=nullptr;
		QTreeWidgetItem *root=nullptr,*item1=nullptr, *item2=nullptr;
		QFont font;
		vector<BaseObject *> tree_state, obj_list;
		ObjectType types[]={ OBJ_ROLE, OBJ_TABLESPACE,
												 OBJ_LANGUAGE, OBJ_CAST, OBJ_TEXTBOX,
												 OBJ_RELATIONSHIP };
		unsigned count, i, i1, rel_type, type_cnt=sizeof(types)/sizeof(ObjectType);

		try
		{
			if(save_tree_state)
				saveTreeState(tree_state);

			objectstree_tw->clear();

			if(visible_objs_map[OBJ_DATABASE])
			{
				root=new QTreeWidgetItem;
				root->setIcon(0,QPixmap(Utf8String::create(":/icones/icones/") +
																QString(BaseObject::getSchemaName(OBJ_DATABASE)) +
																QString(".png")));
				objectstree_tw->insertTopLevelItem(0,root);

				root->setText(0,Utf8String::create(db_model->getName()));
				root->setToolTip(0,Utf8String::create(db_model->getName()));
				root->setData(0, Qt::UserRole, generateItemValue(db_model));

				updatePermissionTree(root, db_model);

				if(db_model->isProtected())
				{
					font=root->font(0);
					font.setItalic(true);
					root->setFont(0,font);
					root->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				updateSchemaTree(root);

				for(i=0; i < type_cnt; i++)
				{
					if(visible_objs_map[types[i]])
					{
						item1=new QTreeWidgetItem(root);
						str_aux=QString(BaseObject::getSchemaName(types[i]));

						item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 str_aux + QString("_grp") + QString(".png")));
						item1->setData(1, Qt::UserRole, QVariant::fromValue<unsigned>(types[i]));

						obj_list=(*db_model->getObjectList(types[i]));

						//Special case for relationship, merging the base relationship list to the relationship list
						if(types[i]==OBJ_RELATIONSHIP)
						{
							vector<BaseObject *> obj_list_aux;
							obj_list_aux=(*db_model->getObjectList(BASE_RELATIONSHIP));
							obj_list.insert(obj_list.end(), obj_list_aux.begin(), obj_list_aux.end());
						}

						count=obj_list.size();
						item1->setText(0,BaseObject::getTypeName(types[i]) +
													 QString(" (%1)").arg(count));
						font=item1->font(0);
						font.setItalic(true);
						item1->setFont(0, font);

						for(i1=0; i1 < count; i1++)
						{
							object=obj_list.at(i1);

							item2=new QTreeWidgetItem(item1);
							item2->setText(0,Utf8String::create(object->getName()));
							item2->setToolTip(0,Utf8String::create(object->getName()));
							item2->setData(0, Qt::UserRole, generateItemValue(object));

							updatePermissionTree(item2, object);

							if(object->isProtected())
							{
								font=item2->font(0);
								font.setItalic(true);
								item2->setFont(0,font);
								item2->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							switch(types[i])
							{
								case BASE_RELATIONSHIP:
								case OBJ_RELATIONSHIP:
									rel_type=dynamic_cast<BaseRelationship *>(object)->getRelationshipType();

									if(object->getObjectType()==BASE_RELATIONSHIP)
									{
									 if(rel_type==BaseRelationship::RELATIONSHIP_FK)
										str_aux="fk";
									 else
										str_aux="tv";
									}
									else if(rel_type==BaseRelationship::RELATIONSHIP_11)
										str_aux="11";
									else if(rel_type==BaseRelationship::RELATIONSHIP_1N)
										str_aux="1n";
									else if(rel_type==BaseRelationship::RELATIONSHIP_NN)
										str_aux="nn";
									else if(rel_type==BaseRelationship::RELATIONSHIP_DEP)
										str_aux="dep";
									else if(rel_type==BaseRelationship::RELATIONSHIP_GEN)
										str_aux="gen";
								break;

								default:
									str_aux="";
								break;
							}

							str_aux=QString(BaseObject::getSchemaName(types[i])) + str_aux;
							item2->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
						}
					}
				}

				objectstree_tw->expandItem(root);

				if(save_tree_state)
					restoreTreeState(tree_state);
				else if(simplified_view)
					objectstree_tw->expandAll();
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		objectstree_tw->sortByColumn(0, Qt::AscendingOrder);
	}
}

BaseObject *ModelObjectsWidget::getSelectedObject(void)
{
	return(selected_object);
}

void ModelObjectsWidget::close(void)
{
	QObject *obj_sender=sender();

	if(obj_sender==cancel_tb)
		selected_object=nullptr;
	else
	{
		QVariant data;

		if(tree_view_tb->isChecked() && objectstree_tw->currentItem())
			data=objectstree_tw->currentItem()->data(0,Qt::UserRole);
		else if(objectslist_tbw->currentItem())
			data=objectslist_tbw->currentItem()->data(Qt::UserRole);

		selected_object=reinterpret_cast<BaseObject *>(data.value<void *>());
	}

	QWidget::close();
}

void ModelObjectsWidget::setModel(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;

	if(model_wgt)
		setModel(model_wgt->db_model);
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
}

void ModelObjectsWidget::showEvent(QShowEvent *)
{
	if(simplified_view)
	{
		QWidget *wgt=QApplication::activeWindow();

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

	emit s_visibilityChanged(selected_object, !this->isVisible());
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
		QRect ret=desktop.screenGeometry();
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

void ModelObjectsWidget::saveTreeState(bool value)
{
	save_tree_state=(!simplified_view && value);
}

void ModelObjectsWidget::saveTreeState(vector<BaseObject *> &tree_items)
{
	QList<QTreeWidgetItem *> items;
	BaseObject *obj=nullptr;
	QTreeWidgetItem *item=nullptr;

	tree_items.clear();
	items=objectstree_tw->findItems("*",Qt::MatchWildcard | Qt::MatchRecursive,0);

	while(!items.isEmpty())
	{
		item=items.front();
		obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

		if(obj && item->parent() && item->parent()->isExpanded())
			tree_items.push_back(obj);

		items.pop_front();
	}

	unique(tree_items.begin(), tree_items.end());
}

void ModelObjectsWidget::restoreTreeState(vector<BaseObject *> &tree_items)
{
	QTreeWidgetItem *item=nullptr, *parent_item=nullptr;

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
}

QTreeWidgetItem *ModelObjectsWidget::getTreeItem(BaseObject *object)
{
	if(object)
	{
		QList<QTreeWidgetItem *> items;
		BaseObject *aux_obj=nullptr;
		QTreeWidgetItem *item=nullptr;

		items=objectstree_tw->findItems("*",Qt::MatchWildcard | Qt::MatchRecursive,0);

		while(!items.isEmpty())
		{
			item=items.front();
			aux_obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

			if(aux_obj==object)
				break;
			else
				item=nullptr;

			items.pop_front();
		}

		return(item);
	}
	else
		return(nullptr);
}
