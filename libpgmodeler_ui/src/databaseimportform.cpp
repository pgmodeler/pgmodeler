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

extern ConfigurationForm *configuration_form;

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

void DatabaseImportForm::listObjects(void)
{
	try
	{
		ObjectType shared_types[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_CAST },
							 sch_types[]={ OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN, OBJ_EXTENSION,
														 OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_SEQUENCE, OBJ_TYPE,
														 OBJ_TABLE, OBJ_VIEW },
							 tab_types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX };
		unsigned i, cnt=sizeof(shared_types)/sizeof(ObjectType),
								cnt1,
								cnt2;
		QTreeWidgetItem *group=nullptr, *item=nullptr;
		attribs_map objects, child_objs;
		attribs_map::iterator itr;
		QFont grp_fnt=db_objects_tw->font();

		db_objects_tw->clear();
		dbimport_helper.setCurrentDatabase(database_cmb->currentText());
		grp_fnt.setItalic(true);

		for(i=0; i < cnt; i++)
		{
			objects=dbimport_helper.getObjects(shared_types[i]);
			group=new QTreeWidgetItem;
			group->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(shared_types[i]) + QString("_grp.png")));
			group->setText(0, BaseObject::getTypeName(shared_types[i]) + QString(" (%1)").arg(objects.size()));
			group->setFont(0, grp_fnt);
			db_objects_tw->addTopLevelItem(group);

			itr=objects.begin();
			while(itr!=objects.end())
			{
				item=new QTreeWidgetItem(group);
				item->setCheckState(0, Qt::Checked);
				item->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(shared_types[i]) + QString(".png")));
				item->setText(0, itr->second);

				if(shared_types[i]==OBJ_SCHEMA)
				{

				}

				itr++;
			}

			objects.clear();
		}

		db_objects_tw->sortItems(0, Qt::AscendingOrder);
	}
	catch(Exception &e)
	{
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

		listObjects();
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
