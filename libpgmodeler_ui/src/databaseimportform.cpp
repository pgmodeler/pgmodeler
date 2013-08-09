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
	setupUi(this);
	connect(connect_tb, SIGNAL(clicked(bool)), this, SLOT(listDatabases(void)));
	connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listObjects(void)));
}

void DatabaseImportForm::updateProgress(int progress, QString msg)
{
 progress_lbl->setText(msg);
 progress_pb->setValue(progress);
 this->repaint();
}

void DatabaseImportForm::importDatabase(void)
{
	try
	{

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QTreeWidgetItem *> DatabaseImportForm::updateObjectsTree(vector<ObjectType> types, QTreeWidgetItem *root, ObjectType filter_type, const QString &schema, const QString &table)
{
	QTreeWidgetItem *group=nullptr, *item=nullptr;
	QFont grp_fnt=db_objects_tw->font();
	attribs_map objects;
	attribs_map::iterator itr;
	vector<QTreeWidgetItem *> items_vect;

	grp_fnt.setItalic(true);

	for(unsigned i=0; i < types.size(); i++)
	{
		objects=dbimport_helper.getObjects(types[i], schema, table);
		group=new QTreeWidgetItem(root);
		group->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString("_grp.png")));
		group->setText(0, BaseObject::getTypeName(types[i]) + QString(" (%1)").arg(objects.size()));
		group->setFont(0, grp_fnt);
		db_objects_tw->addTopLevelItem(group);

		itr=objects.begin();
		while(itr!=objects.end())
		{
			item=new QTreeWidgetItem(group);
			item->setCheckState(0, Qt::Checked);
			item->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString(".png")));
			item->setText(0, itr->second);

			if(filter_type==types[i])
				items_vect.push_back(item);

			itr++;
		}

		emit s_objectsRetrieved(((i/static_cast<float>(types.size())) * 100),
														trUtf8("Retrieving objects... <em>%1</em>")
														.arg(BaseObject::getTypeName(types[i])),
														types[i]);
	}

	return(items_vect);
}

void DatabaseImportForm::listObjects(void)
{
	try
	{
		vector<QTreeWidgetItem *> sch_items, tab_items;

		connect(this, SIGNAL(s_objectsRetrieved(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt->setWindowTitle(trUtf8("Retrieving objects from database..."));
		task_prog_wgt->show();

		db_objects_tw->clear();
		dbimport_helper.setCurrentDatabase(database_cmb->currentText());

		sch_items=updateObjectsTree({OBJ_ROLE, OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_CAST}, nullptr, OBJ_SCHEMA);

		while(!sch_items.empty())
		{
			tab_items=updateObjectsTree({OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN, OBJ_EXTENSION,
																	 OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLE, OBJ_VIEW},
																	sch_items.back(), OBJ_TABLE, sch_items.back()->text(0));

			while(!tab_items.empty())
			{
				updateObjectsTree({ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX }, tab_items.back(),
													BASE_OBJECT, sch_items.back()->text(0), tab_items.back()->text(0));
				tab_items.pop_back();
			}

			sch_items.pop_back();
		}

		db_objects_tw->sortItems(0, Qt::AscendingOrder);
		task_prog_wgt->close();
		disconnect(this, nullptr, task_prog_wgt, nullptr);
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(this, nullptr, task_prog_wgt, nullptr);
		db_objects_tw->clear();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::listDatabases()
{
	try
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		attribs_map db_attribs;
		attribs_map::iterator itr;

		dbimport_helper.setConnection(*conn);
		db_attribs=dbimport_helper.getObjects(OBJ_DATABASE);

		database_cmb->clear();

		itr=db_attribs.begin();
		while(itr!=db_attribs.end())
		{
			database_cmb->addItem(QPixmap(":/icones/icones/" + BaseObject::getSchemaName(OBJ_DATABASE) + ".png"),
														itr->second, itr->first);
			itr++;
		}
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

	import_btn->setEnabled(connections_cmb->count() > 0);
	database_gb->setEnabled(connections_cmb->count() > 0);
}
