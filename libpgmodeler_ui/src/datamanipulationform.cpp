/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "datamanipulationform.h"
#include "sqltoolwidget.h"

DataManipulationForm::DataManipulationForm(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f)
{
	setupUi(this);
	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));
	setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	connect(schema_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listTables()));
	connect(refresh_tb, SIGNAL(clicked()), this, SLOT(retrieveData()));

	connect(table_cmb, &QComboBox::currentTextChanged,
					[=](){ refresh_tb->setEnabled(table_cmb->currentIndex() > 0); });
}

void DataManipulationForm::setAttributes(Connection conn, const QString curr_schema, const QString curr_table)
{
	try
	{
		connection=conn;
		catalog.setConnection(conn);
		listObjects(schema_cmb, OBJ_SCHEMA);

		schema_cmb->setCurrentText(curr_schema);
		table_cmb->setCurrentText(curr_table);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::listTables(void)
{
	if(schema_cmb->currentIndex() > 0)
		listObjects(table_cmb, OBJ_TABLE, schema_cmb->currentText());

	table_lbl->setEnabled(table_cmb->count() > 0);
	table_cmb->setEnabled(table_cmb->count() > 0);
}

void DataManipulationForm::retrieveData(void)
{
	try
	{
		QString query=QString("SELECT * FROM %1").arg(table_cmb->currentText());
		ResultSet res;

		connection.connect();
		connection.executeDMLCommand(query, res);

		SQLToolWidget::fillResultsTable(res, results_tbw);
		retrievePKColumns(schema_cmb->currentText(), table_cmb->currentText());

		connection.close();
	}
	catch(Exception &e)
	{
		if(connection.isStablished())
			connection.close();

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::listObjects(QComboBox *combo, ObjectType obj_type, const QString &schema)
{
	try
	{
		attribs_map objects;
		QStringList items;

		catalog.setFilter(Catalog::LIST_ALL_OBJS);
		objects=catalog.getObjectsNames(obj_type, schema);

		combo->blockSignals(true);
		combo->clear();

		for(auto attr : objects)
			items.push_back(attr.second);

		items.sort();
		combo->addItems(items);

		for(int idx=0; idx < combo->count(); idx++)
		 combo->setItemIcon(idx, QPixmap(":/icones/icones/" + BaseObject::getSchemaName(obj_type) + ".png"));

		if(objects.empty())
			combo->insertItem(0, trUtf8("No objects found"));
		else
			combo->insertItem(0, trUtf8("Found %1 object(s)").arg(objects.size()));

		combo->setCurrentIndex(0);
		combo->blockSignals(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QStringList DataManipulationForm::retrievePKColumns(const QString &schema, const QString &table)
{
	try
	{
		vector<attribs_map> constraints, columns;
		QStringList col_names;

		//Retrieving the constraints from catalog using a custom filter to select only primary keys (contype=p)
		constraints=catalog.getObjectsAttributes(OBJ_CONSTRAINT, schema, table, {}, {{ParsersAttributes::CUSTOM_FILTER, "contype='p'"}});

		if(!constraints.empty())
		{
			QStringList col_str_ids=Catalog::parseArrayValues(constraints[0][ParsersAttributes::COLUMNS]);
			vector<unsigned> col_ids;

			for(auto id : col_str_ids)
				col_ids.push_back(id.toUInt());

			columns=catalog.getObjectsAttributes(OBJ_COLUMN, schema, table, col_ids);

			for(auto col : columns)
				col_names.push_back(col[ParsersAttributes::NAME]);
		}

		return(col_names);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
