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

#include "databaseimportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"
#include "taskprogresswidget.h"

extern ConfigurationForm *configuration_form;
extern TaskProgressWidget *task_prog_wgt;

DatabaseImportForm::DatabaseImportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	progress=0;
	setupUi(this);
	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(reject(void)));
	connect(import_btn, SIGNAL(clicked(bool)), this, SLOT(importDatabase(void)));
	connect(connect_tb, SIGNAL(clicked(bool)), this, SLOT(listDatabases(void)));
	connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listObjects(void)));
	connect(expand_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(expandAll(void)));
	connect(collapse_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(collapseAll(void)));
	connect(db_objects_tw, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(setItemCheckState(QTreeWidgetItem*,int)));
	connect(select_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState(void)));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState(void)));
}

void DatabaseImportForm::updateProgress(int progress, QString msg)
{
 progress_lbl->setText(msg);
 progress_pb->setValue(progress);
 this->repaint();
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, int)
{
	db_objects_tw->blockSignals(true);
	setItemCheckState(item, item->checkState(0));
	setParentItemChecked(item->parent());
	import_btn->setEnabled(hasCheckedItems());
	db_objects_tw->blockSignals(false);
}

void DatabaseImportForm::setItemsCheckState()
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	Qt::CheckState chk_state=(sender()==select_all_tb ? Qt::Checked : Qt::Unchecked);

	db_objects_tw->blockSignals(true);
	while(*itr)
	{
		if(!(*itr)->isDisabled())
		(*itr)->setCheckState(0, chk_state);
		++itr;
	}
	db_objects_tw->blockSignals(false);
	import_btn->setEnabled(hasCheckedItems());
}

void DatabaseImportForm::importDatabase(void)
{
	try
	{
		vector<unsigned> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;

		getCheckedItems(obj_oids, col_oids);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QTreeWidgetItem *> DatabaseImportForm::updateObjectsTree(vector<ObjectType> types, QTreeWidgetItem *root, const QString &schema, const QString &table)
{
	QTreeWidgetItem *group=nullptr, *item=nullptr;
	QFont grp_fnt=db_objects_tw->font();
	attribs_map objects;
	attribs_map::iterator itr;
	vector<QTreeWidgetItem *> items_vect;

	grp_fnt.setItalic(true);
	db_objects_tw->blockSignals(true);

	for(unsigned i=0; i < types.size(); i++)
	{
		//Retrieve the objects of the current type from the database
		objects=dbimport_helper.getObjects(types[i], schema, table);

		//Create a group item for the current type
		group=new QTreeWidgetItem(root);
		group->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString("_grp.png")));
		group->setText(0, BaseObject::getTypeName(types[i]) + QString(" (%1)").arg(objects.size()));
		group->setFont(0, grp_fnt);
		group->setData(0, Qt::UserRole, 0);

		//Disables the item if no object were retrieved from database
		group->setDisabled(objects.empty());
		group->setCheckState(0, (!group->isDisabled() ? Qt::Checked : Qt::Unchecked));

		db_objects_tw->addTopLevelItem(group);

		//Creates individual items for each object of the current type
		itr=objects.begin();
		while(itr!=objects.end())
		{
			item=new QTreeWidgetItem(group);
			item->setCheckState(0, Qt::Checked);
			item->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString(".png")));
			item->setText(0, itr->second);

			//Stores the object's OID as the first data of the item
			item->setData(0, Qt::UserRole, itr->first.toUInt());

			//Stores the object's type as the second data of the item
			item->setData(1, Qt::UserRole, types[i]);

			if(types[i]==OBJ_SCHEMA || types[i]==OBJ_TABLE)
				items_vect.push_back(item);

			itr++;
		}

		emit s_objectsRetrieved(progress,
														trUtf8("Retrieving objects... <strong><em>%1</em><strong>")
														.arg(BaseObject::getTypeName(types[i])),
														types[i]);
	}

	db_objects_tw->blockSignals(false);
	return(items_vect);
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, Qt::CheckState chk_state)
{
	for(int i=0; i < item->childCount(); i++)
	{
		if(!item->child(i)->isDisabled())
			item->child(i)->setCheckState(0, chk_state);

		setItemCheckState(item->child(i), chk_state);
	}
}

void DatabaseImportForm::setParentItemChecked(QTreeWidgetItem *item)
{
	if(item && !item->isDisabled())
	{
		if(item->checkState(0)!=Qt::Checked)
			item->setCheckState(0, Qt::Checked);

		setParentItemChecked(item->parent());
	}
}

bool DatabaseImportForm::hasCheckedItems(void)
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	bool selected=false;

	while(*itr && !selected)
	{
		//Only valid items (OID > 0) and with Checked state are considered as selected
		selected=((*itr)->checkState(0)==Qt::Checked && (*itr)->data(0, Qt::UserRole).value<unsigned>() > 0);
		++itr;
	}

	return(selected);
}

void DatabaseImportForm::getCheckedItems(vector<unsigned> &obj_oids, map<unsigned, vector<unsigned>> &col_oids)
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	unsigned tab_oid=0;

	obj_oids.clear();
	col_oids.clear();

	while(*itr)
	{
		//If the item is checked and its OID is valid
		if((*itr)->checkState(0)==Qt::Checked && (*itr)->data(0, Qt::UserRole).value<unsigned>() > 0)
		{
			//If the object is not a column store it on general object list
			if((*itr)->data(1, Qt::UserRole).value<unsigned>() != OBJ_COLUMN)
			 obj_oids.push_back((*itr)->data(0, Qt::UserRole).value<unsigned>());
			//If its a column
			else
			{
				//Get the table's oid from the parent item
				tab_oid=(*itr)->parent()->parent()->data(0, Qt::UserRole).value<unsigned>();

				//Store the column oid on the selected colums map using the table oid as key
				col_oids[tab_oid].push_back((*itr)->data(0, Qt::UserRole).value<unsigned>());
			}
		}

		++itr;
	}

	std::sort(obj_oids.begin(), obj_oids.end());
}

void DatabaseImportForm::listObjects(void)
{
	try
	{
		db_objects_tw->clear();

		if(database_cmb->currentIndex() > 0)
		{
			vector<QTreeWidgetItem *> sch_items, tab_items;
			int inc=0, inc1=0;

			//Configuring the task progress widget to show the object retrieving progress
			connect(this, SIGNAL(s_objectsRetrieved(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
			task_prog_wgt->setWindowTitle(trUtf8("Retrieving objects from database..."));
			task_prog_wgt->show();

			//Set the working database on import helper
			dbimport_helper.setCurrentDatabase(database_cmb->currentText());

			//Retrieving and listing the cluster scoped objects
			progress=0;
			sch_items=updateObjectsTree({OBJ_ROLE, OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_CAST});
			progress=10;

			inc=40/static_cast<float>(sch_items.size());
			while(!sch_items.empty())
			{
				//Retrieving and listing the schema scoped objects
				tab_items=updateObjectsTree({OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN, OBJ_EXTENSION,
																		 OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLE, OBJ_VIEW},
																		sch_items.back(), sch_items.back()->text(0));

				while(!tab_items.empty())
				{
					inc1=(30/static_cast<float>(tab_items.size()))/static_cast<float>(sch_items.size());

					//Retrieving and listing the table scoped objects
					updateObjectsTree({ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX }, tab_items.back(),
														sch_items.back()->text(0), tab_items.back()->text(0));
					tab_items.pop_back();

					progress+=inc1;
				}

				progress+=inc;
				sch_items.pop_back();
			}

			progress=0;
			db_objects_tw->sortItems(0, Qt::AscendingOrder);

			emit s_objectsRetrieved(100, trUtf8("Retrieving objects... done!"),	BASE_OBJECT);
			task_prog_wgt->close();
			disconnect(this, nullptr, task_prog_wgt, nullptr);
		}

		//Enable the control buttons only when objects were retrieved
		select_all_tb->setEnabled(db_objects_tw->topLevelItemCount() > 0);
		clear_all_tb->setEnabled(db_objects_tw->topLevelItemCount() > 0);
		expand_all_tb->setEnabled(db_objects_tw->topLevelItemCount() > 0);
		collapse_all_tb->setEnabled(db_objects_tw->topLevelItemCount() > 0);
		import_btn->setEnabled(hasCheckedItems());
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(this, nullptr, task_prog_wgt, nullptr);
		db_objects_tw->clear();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::listDatabases(void)
{
	try
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		attribs_map db_attribs;
		attribs_map::iterator itr;
		QStringList list;

		//List the available databases using the selected connection
		dbimport_helper.setConnection(*conn);
		db_attribs=dbimport_helper.getObjects(OBJ_DATABASE);

		db_objects_tw->clear();
		database_cmb->clear();

		if(db_attribs.empty())
		{
			database_cmb->addItem(QString("No databases found"));
			database_gb->setEnabled(false);
		}
		else
		{
			database_cmb->blockSignals(true);

			itr=db_attribs.begin();
			while(itr!=db_attribs.end())
			{
				list.push_back(itr->second);
				itr++;
			}

			list.sort();
			database_cmb->addItems(list);

			for(int i=0; i < list.count(); i++)
				database_cmb->setItemIcon(i, QPixmap(":/icones/icones/" + BaseObject::getSchemaName(OBJ_DATABASE) + ".png"));

			database_cmb->insertItem(0, QString("Found %1 database(s)").arg(db_attribs.size()));
			database_cmb->setCurrentIndex(0);
			database_cmb->blockSignals(false);
			database_gb->setEnabled(true);
		}

		database_gb->setEnabled(database_cmb->count() > 1);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::hideProgress(void)
{
	ln2_frm->setVisible(false);
	progress_lbl->setVisible(false);
	progress_pb->setVisible(false);
	ico_lbl->setVisible(false);
	this->resize(this->minimumSize());
}

void DatabaseImportForm::showEvent(QShowEvent *)
{
	map<QString, Connection *> connections;
	map<QString, Connection *>::iterator itr;

	//Get the current connections configured on the connections widget
	dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->getConnections(connections);

	connections_cmb->clear();
	itr=connections.begin();

	//Add the connections to the combo
	while(itr!=connections.end())
	{
		connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
		itr++;
	}

	hideProgress();
}
