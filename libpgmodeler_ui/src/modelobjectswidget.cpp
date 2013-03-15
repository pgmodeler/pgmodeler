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

ModelObjectsWidget::ModelObjectsWidget(bool simplified_view, QWidget *parent, Qt::WindowFlags f) : QDockWidget(parent, f)
{
	ObjectType type[]={  OBJ_DATABASE, OBJ_TABLE, OBJ_FUNCTION, OBJ_VIEW, OBJ_DOMAIN,
												OBJ_SCHEMA, OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_SEQUENCE,
												OBJ_ROLE, OBJ_CONVERSION, OBJ_CAST, OBJ_LANGUAGE,
												OBJ_TYPE, OBJ_TABLESPACE, OBJ_OPFAMILY, OBJ_OPCLASS,
												OBJ_RELATIONSHIP, OBJ_TEXTBOX, OBJ_COLUMN, OBJ_CONSTRAINT,
												OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE };
	int type_id, type_count=24;
	QListWidgetItem *item=NULL;
	QPixmap icon;
	QString str_aux;

	setupUi(this);

	model_wgt=NULL;
	db_model=NULL;
	this->setModel(db_model);

	this->simplified_view=simplified_view;
	this->save_tree_state=!simplified_view;

	select_tb->setVisible(simplified_view);
	cancel_tb->setVisible(simplified_view);
	options_tb->setVisible(!simplified_view);
	visibleobjects_grp->setVisible(false);

	//Creating the visible object list
	for(type_id=0; !simplified_view && type_id < type_count; type_id++)
	{
		item=new QListWidgetItem;

		if(type[type_id]==BASE_RELATIONSHIP)
			str_aux=QString(BaseObject::getSchemaName(type[type_id])) + "tv";
		else
			str_aux=QString(BaseObject::getSchemaName(type[type_id]));

		icon=QPixmap(Utf8String::create(":/icones/icones/") + str_aux + QString(".png"));

		item->setText(Utf8String::create(BaseObject::getTypeName(type[type_id])));
		item->setIcon(icon);

		//By default all object type is visible (checked)
		item->setCheckState(Qt::Checked);
		item->setData(Qt::UserRole, QVariant(type[type_id]));

		visibleobjects_lst->insertItem(type_id, item);
		visible_objs_map[type[type_id]]=true;
	}

	selected_object=NULL;
	splitter->handle(1)->setEnabled(false);

	connect(objectstree_tw,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this, SLOT(selectObject(void)));
	connect(objectslist_tbw,SIGNAL(itemPressed(QTableWidgetItem*)),this, SLOT(selectObject(void)));

	if(!simplified_view)
	{
		widgets_conf.setValue("splitterSize", splitter->saveState());

		connect(options_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
		connect(visibleobjects_lst,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setObjectVisible(QListWidgetItem*)));
		connect(select_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(clear_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(editObject(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(editObject(void)));
	}
	else
	{
		setWindowModality(Qt::ApplicationModal);
		setAllowedAreas(Qt::NoDockWidgetArea);
		setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
		setFeatures(QDockWidget::DockWidgetClosable);
		setFloating(true);

		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(close(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(close(void)));
		connect(select_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
		connect(cancel_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
	}

	connect(tree_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
	connect(list_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
}

void ModelObjectsWidget::showObjectMenu(void)
{
	if(selected_object && QApplication::mouseButtons()==Qt::RightButton && model_wgt && !simplified_view)
		model_wgt->popup_menu.exec(QCursor::pos());
}

void ModelObjectsWidget::editObject(void)
{
	if(selected_object && model_wgt && !simplified_view)
		model_wgt->editObject();
}

void ModelObjectsWidget::selectObject(void)
{
	if(tree_view_tb->isChecked())
	{
		QTreeWidgetItem *tree_item=objectstree_tw->currentItem();

		if(tree_item)
			selected_object=reinterpret_cast<BaseObject *>(tree_item->data(0,Qt::UserRole).value<void *>());
	}
	else
	{
		QTableWidgetItem *tab_item=objectslist_tbw->currentItem();

		if(tab_item)
			selected_object=reinterpret_cast<BaseObject *>(tab_item->data(Qt::UserRole).value<void *>());
	}

	if(selected_object && !simplified_view)
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

		if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT || obj_type==OBJ_RULE ||
			 obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX)
			visible_objs_map[OBJ_TABLE]=visible_objs_map[OBJ_SCHEMA]=true;

		if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW || obj_type==OBJ_FUNCTION ||
			 obj_type==OBJ_AGGREGATE || obj_type==OBJ_DOMAIN || obj_type==OBJ_TYPE ||
			 obj_type==OBJ_CONVERSION || obj_type==OBJ_OPERATOR || obj_type==OBJ_OPFAMILY ||
			 obj_type==OBJ_OPCLASS || obj_type==OBJ_SEQUENCE)
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

void ModelObjectsWidget::setAllObjectsVisible(bool)
{
	int count, i;
	ObjectType obj_type;
	QListWidgetItem *item=NULL;
	bool checked;

	checked=(sender()==select_all_tb);
	count=visibleobjects_lst->count();

	for(i=0; i < count; i++)
	{
		item=visibleobjects_lst->item(i);
		obj_type=static_cast<ObjectType>(item->data(Qt::UserRole).toInt());
		visible_objs_map[obj_type]=checked;

		if(checked)
			item->setCheckState(Qt::Checked);
		else
			item->setCheckState(Qt::Unchecked);
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
		BaseObject *object=NULL, *schema=NULL;
		TableObject *tab_object=NULL;
		QTableWidgetItem *tab_item=NULL, *tab_item1=NULL;
		Table *table=NULL;
		Function *func=NULL;
		Operator *oper=NULL;
		QPixmap icon;
		QFont font;
		QString str_aux;
		unsigned rel_type;
		vector<BaseObject *> obj_list;
		ObjectType types[]={  OBJ_DATABASE, OBJ_TABLE, OBJ_FUNCTION, OBJ_VIEW, OBJ_DOMAIN,
													OBJ_SCHEMA, OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_SEQUENCE,
													OBJ_ROLE, OBJ_CONVERSION, OBJ_CAST, OBJ_LANGUAGE,
													OBJ_TYPE, OBJ_TABLESPACE, OBJ_OPFAMILY, OBJ_OPCLASS,
													OBJ_RELATIONSHIP, OBJ_TEXTBOX/*, BASE_RELATIONSHIP */ },
				subtypes[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
										 OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE };

		int type_cnt=19, subtype_cnt=5, type_id, count, count1, idx, tab_id;

		try
		{
			//Temporary disables the item sorting for correctly create them
			objectslist_tbw->setSortingEnabled(false);

			for(type_id=0; type_id < type_cnt; type_id++)
			{
				if(types[type_id]==OBJ_DATABASE)
					count=1;
				else
				{
					//Get the current object list from  database model
					obj_list=(*db_model->getObjectList(types[type_id]));

					//Special case for relationship, merging the base relationship list to the relationship list
					if(types[type_id]==OBJ_RELATIONSHIP)
					{
						vector<BaseObject *> obj_list_aux;
						obj_list_aux=(*db_model->getObjectList(BASE_RELATIONSHIP));
						obj_list.insert(obj_list.end(), obj_list_aux.begin(), obj_list_aux.end());
					}

					count=obj_list.size();
				}

				//If the current object type is visible and there is at least one object to be showed
				for(idx=0; visible_objs_map[types[type_id]] && idx < count; idx++)
				{
					objectslist_tbw->insertRow(idx);

					if(types[type_id]!=OBJ_DATABASE)
						object=obj_list.at(idx);
					else
						object=db_model;

					//Creating the table item for the current object
					tab_item=new QTableWidgetItem;
					tab_item->setData(Qt::UserRole, generateItemValue(object));
					objectslist_tbw->setItem(idx, 0, tab_item);

					//Changes the item foreground color if the object is protected
					if(object->isProtected())
					{
						font=tab_item->font();
						font.setItalic(true);
						tab_item->setFont(font);
						tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
					}

					if(types[type_id]!=OBJ_FUNCTION && types[type_id]!=OBJ_OPERATOR)
					{
						tab_item->setText(Utf8String::create(object->getName()));
						tab_item->setToolTip(Utf8String::create(object->getName()));
					}
					else if(types[type_id]==OBJ_FUNCTION)
					{
						func=dynamic_cast<Function *>(object);
						func->createSignature(false);
						tab_item->setText(Utf8String::create(func->getSignature()));
						tab_item->setToolTip(Utf8String::create(func->getSignature()));
						func->createSignature(true);
					}
					else
					{
						oper=dynamic_cast<Operator *>(object);
						tab_item->setText(Utf8String::create(oper->getSignature(false)));
						tab_item->setToolTip(Utf8String::create(oper->getSignature(false)));
					}

					//Creating the object type item
					tab_item=new QTableWidgetItem;
					tab_item->setData(Qt::UserRole, generateItemValue(object));

					if(types[type_id]==OBJ_RELATIONSHIP)
					{
						str_aux=QString(BaseObject::getSchemaName(object->getObjectType()));

						if(object->getObjectType()==BASE_RELATIONSHIP)
						{
							if(dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
								str_aux+="fk";
							else
								str_aux+="tv";
						}
						else
						{
							rel_type=dynamic_cast<Relationship *>(object)->getRelationshipType();
							if(rel_type==Relationship::RELATIONSHIP_11)
								str_aux+="11";
							else if(rel_type==Relationship::RELATIONSHIP_1N)
								str_aux+="1n";
							else if(rel_type==Relationship::RELATIONSHIP_NN)
								str_aux+="nn";
							else if(rel_type==Relationship::RELATIONSHIP_DEP)
								str_aux+="dep";
							else if(rel_type==Relationship::RELATIONSHIP_GEN)
								str_aux+="gen";
						}
					}
					else
						str_aux=QString(BaseObject::getSchemaName(object->getObjectType()));

					icon=QPixmap(QString(":/icones/icones/") + str_aux + QString(".png"));

					objectslist_tbw->setItem(idx, 1, tab_item);
					tab_item->setText(Utf8String::create(object->getTypeName()));

					tab_item->setIcon(icon);
					font=tab_item->font();
					font.setItalic(true);
					tab_item->setFont(font);

					//Creates the item that describes the object parent
					tab_item=new QTableWidgetItem;
					tab_item1=new QTableWidgetItem;
					font=tab_item1->font();
					font.setItalic(true);
					tab_item1->setFont(font);

					objectslist_tbw->setItem(idx, 2, tab_item);
					objectslist_tbw->setItem(idx, 3, tab_item1);
					tab_item->setData(Qt::UserRole, generateItemValue(object));
					tab_item1->setData(Qt::UserRole, generateItemValue(object));


					//Configuring the parent object name
					switch(types[type_id])
					{
						//The database object has no parent
						case OBJ_DATABASE:
							tab_item->setText("-");
							tab_item1->setText("-");
						break;

						//Objects which has a schema as parent
						case OBJ_FUNCTION:
						case OBJ_TABLE:
						case OBJ_VIEW:
						case OBJ_DOMAIN:
						case OBJ_AGGREGATE:
						case OBJ_OPERATOR:
						case OBJ_SEQUENCE:
						case OBJ_CONVERSION:
						case OBJ_TYPE:
						case OBJ_OPFAMILY:
						case OBJ_OPCLASS:
							//Creating the schema icon
							icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
														QString(".png"));

							tab_item->setText(Utf8String::create(object->getSchema()->getName()));
							tab_item1->setIcon(icon);
							tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_SCHEMA)));

							schema=object->getSchema();

							//Changing the schema foreground color if it is protected
							if(schema && schema->isProtected())
							{
								font=tab_item->font();
								font.setItalic(true);
								tab_item->setFont(font);
								tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}
						break;

						//For the other object types the parent object is always the database
						default:
							icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_DATABASE)) +
														QString(".png"));
							tab_item->setText(Utf8String::create(db_model->getName()));
							tab_item1->setIcon(icon);
							tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_DATABASE)));
						break;
					}
				}
			}

			//Inserting the table object (columns, constraints, indexes, triggers) onto the list
			count=db_model->getObjectCount(OBJ_TABLE);

			for(tab_id=0; tab_id < count; tab_id++)
			{
				table=dynamic_cast<Table *>(db_model->getTable(tab_id));

				for(type_id=0; type_id < subtype_cnt; type_id++)
				{
					//Get the current table object count
					count1=table->getObjectCount(subtypes[type_id]);

					for(idx=0; visible_objs_map[subtypes[type_id]] && idx < count1; idx++)
					{
						objectslist_tbw->insertRow(idx);
						tab_object=dynamic_cast<TableObject *>(table->getObject(idx, subtypes[type_id]));

						//Creating the item for object name
						tab_item=new QTableWidgetItem;
						objectslist_tbw->setItem(idx, 0, tab_item);
						tab_item->setText(Utf8String::create(tab_object->getName()));
						tab_item->setToolTip(Utf8String::create(tab_object->getName()));
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));

						//Changing the item foreground color if its added by relationship
						if(tab_object->isAddedByRelationship())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::INH_COLUMN).foreground());
						}
						//Changing the item foreground color if its protected
						else  if(tab_object->isProtected())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
						}

						//Creating the item that describes the object type
						tab_item=new QTableWidgetItem;
						icon=QPixmap(QString(":/icones/icones/") +
													QString(BaseObject::getSchemaName(tab_object->getObjectType())) +
													QString(".png"));
						objectslist_tbw->setItem(idx, 1, tab_item);
						tab_item->setText(Utf8String::create(tab_object->getTypeName()));
						tab_item->setIcon(icon);
						font=tab_item->font();
						font.setItalic(true);
						tab_item->setFont(font);
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));


						//Creating the item that describes the object parent
						tab_item=new QTableWidgetItem;
						tab_item1=new QTableWidgetItem;
						font=tab_item1->font();
						font.setItalic(true);
						tab_item1->setFont(font);
						tab_item1->setData(Qt::UserRole, generateItemValue(tab_object));

						//Changing the parent item descriptor if the parent object is protected
						if(table->isProtected())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
						}

						objectslist_tbw->setItem(idx, 2, tab_item);
						objectslist_tbw->setItem(idx, 3, tab_item1);
						tab_item->setText(Utf8String::create(table->getName()));
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));

						icon=QPixmap(QString(":/icones/icones/") +
													QString(BaseObject::getSchemaName(OBJ_TABLE)) +
													QString(".png"));

						tab_item1->setIcon(icon);
						tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_TABLE)));
						tab_item1->setData(Qt::UserRole, generateItemValue(tab_object));
					}
				}
			}
			objectslist_tbw->setSortingEnabled(true);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updatedSchemaTree(QTreeWidgetItem *root)
{
	if(db_model && visible_objs_map[OBJ_SCHEMA])
	{
		BaseObject *object=NULL, *schema=NULL;
		Function *func=NULL;
		Operator *oper=NULL;
		vector<BaseObject *> obj_list;
		QFont font;
		QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL, *item4=NULL;
		int count, count2, i, i1, i2;
		ObjectType types[]={ OBJ_VIEW, OBJ_FUNCTION, OBJ_AGGREGATE,
												 OBJ_DOMAIN, OBJ_TYPE, OBJ_CONVERSION,
												 OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
												 OBJ_SEQUENCE };

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
					schema=NULL;
				}
				else
				{
					schema=db_model->getObject(i,OBJ_SCHEMA);
					item2=new QTreeWidgetItem(item);
					item2->setText(0,Utf8String::create(schema->getName()));
					item2->setToolTip(0,Utf8String::create(schema->getName()));
					item2->setIcon(0,sch_icon);
					item2->setData(0, Qt::UserRole, generateItemValue(schema));
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

				//Creates the object group at schema level (function, domain, sequences, etc)
				for(i1=0; i1 < 10; i1++)
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
						font=item3->font(0);
						font.setItalic(true);
						item3->setFont(0, font);

						for(i2=0; i2 < count2; i2++)
						{
							object=obj_list[i2];
							item4=new QTreeWidgetItem(item3);
							item4->setData(0, Qt::UserRole, generateItemValue(object));

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
		BaseObject *object=NULL;
		vector<BaseObject *> obj_list;
		Table *table=NULL;
		QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL;
		int count, count1, i, i1, i2;
		QString str_aux;
		QFont font;
		ConstraintType constr_type;
		ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE,
												 OBJ_TRIGGER, OBJ_INDEX };
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

				if(table->isProtected())
				{
					font=item1->font(0);
					font.setItalic(true);
					item1->setFont(0,font);
					item1->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Creating the group for the child objects (column, rules, triggers, indexes and constraints)
				for(i1=0; i1 < 5; i1++)
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
										str_aux="_pk";
									else if(constr_type==ConstraintType::foreign_key)
										str_aux="_fk";
									else if(constr_type==ConstraintType::check)
										str_aux="_ck";
									else if(constr_type==ConstraintType::unique)
										str_aux="_uq";
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

void ModelObjectsWidget::updateDatabaseTree(void)
{
	if(!db_model)
		objectstree_tw->clear();
	else
	{
		QString str_aux;
		BaseObject *object=NULL;
		unsigned count, i, i1, rel_type;
		QTreeWidgetItem *root=NULL,*item1=NULL, *item2=NULL;
		QFont font;
		vector<BaseObject *> tree_state, obj_list;
		ObjectType types[]={ OBJ_ROLE, OBJ_TABLESPACE,
												 OBJ_LANGUAGE, OBJ_CAST, OBJ_TEXTBOX,
												 OBJ_RELATIONSHIP };

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

				root->setText(0,Utf8String::create(/*modelo_wgt->*/db_model->getName()));
				root->setToolTip(0,Utf8String::create(/*modelo_wgt->*/db_model->getName()));
				root->setData(0, Qt::UserRole, generateItemValue(/*modelo_wgt->*/db_model));

				if(db_model->isProtected())
				{
					font=root->font(0);
					font.setItalic(true);
					root->setFont(0,font);
					root->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				updatedSchemaTree(root);

				for(i=0; i < 6; i++)
				{
					if(visible_objs_map[types[i]])
					{
						item1=new QTreeWidgetItem(root);
						str_aux=QString(BaseObject::getSchemaName(types[i]));

						item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 str_aux + QString("_grp") + QString(".png")));

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
		selected_object=NULL;
	else
	{
		QVariant data;

		if(tree_view_tb->isChecked() && objectstree_tw->currentItem())
			data=objectstree_tw->currentItem()->data(0,Qt::UserRole);
		else if(objectslist_tbw->currentItem())
			data=objectslist_tbw->currentItem()->data(Qt::UserRole);

		selected_object=reinterpret_cast<BaseObject *>(data.value<void *>());
	}

	QDockWidget::close();
}

void ModelObjectsWidget::setModel(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;

	if(model_wgt)
		setModel(model_wgt->db_model);
}

void ModelObjectsWidget::setModel(DatabaseModel *db_model)
{
	this->db_model=db_model;

	if(db_model)
		dockWidgetContents->setEnabled(true);
	else
		dockWidgetContents->setEnabled(false);

	updateObjectsView();
	visaoobjetos_stw->setEnabled(true);
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
			this->setGeometry(QRect(QPoint(x,y), this->size()));
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
	BaseObject *obj=NULL;
	QTreeWidgetItem *item=NULL;

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
	QTreeWidgetItem *item=NULL, *parent_item=NULL;

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
		BaseObject *aux_obj=NULL;
		QTreeWidgetItem *item=NULL;

		items=objectstree_tw->findItems("*",Qt::MatchWildcard | Qt::MatchRecursive,0);

		while(!items.isEmpty())
		{
			item=items.front();
			aux_obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

			if(aux_obj==object)
				break;
			else
				item=NULL;

			items.pop_front();
		}

		return(item);
	}
	else
		return(NULL);
}
