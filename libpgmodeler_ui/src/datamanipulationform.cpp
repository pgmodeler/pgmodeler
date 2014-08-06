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
	setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	filter_hl=new SyntaxHighlighter(filter_txt, false);
	filter_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
															 GlobalAttributes::DIR_SEPARATOR +
															 GlobalAttributes::SQL_HIGHLIGHT_CONF +
															 GlobalAttributes::CONFIGURATION_EXT);

	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(schema_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listTables()));
	connect(hide_views_chk, SIGNAL(toggled(bool)), this, SLOT(listTables()));
	connect(schema_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disableControlButtons()));
	connect(table_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disableControlButtons()));
	connect(table_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listColumns()));
	connect(refresh_tb, SIGNAL(clicked()), this, SLOT(retrieveData()));
	connect(add_sel_col_tb, SIGNAL(clicked()), this, SLOT(addColumnToList()));
	connect(add_ord_col_tb, SIGNAL(clicked()), this, SLOT(addColumnToList()));
	connect(sel_columns_lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeColumnFromList()));
	connect(ord_columns_lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeColumnFromList()));
	connect(ord_columns_lst, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(changeOrderMode(QListWidgetItem*)));
	connect(rem_sel_col_tb, SIGNAL(clicked()), this, SLOT(removeColumnFromList()));
	connect(rem_ord_col_tb, SIGNAL(clicked()), this, SLOT(removeColumnFromList()));
	connect(clear_sel_cols_tb, SIGNAL(clicked()), this, SLOT(clearColumnList()));
	connect(clear_ord_cols_tb, SIGNAL(clicked()), this, SLOT(clearColumnList()));

	connect(sel_columns_lst, &QListWidget::currentRowChanged,
					[=](){ rem_sel_col_tb->setEnabled(sel_columns_lst->currentRow() >= 0); });

	connect(ord_columns_lst, &QListWidget::currentRowChanged,
					[=](){ rem_ord_col_tb->setEnabled(ord_columns_lst->currentRow() >= 0); });

	connect(results_tbw, &QTableWidget::itemPressed,
					[=](){ SQLToolWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
					[=](){ SQLToolWidget::exportResults(results_tbw); });
}

void DataManipulationForm::setAttributes(Connection conn, const QString curr_schema, const QString curr_table)
{
	try
	{
		QString db_name;

		connection=conn;
		catalog.setConnection(conn);

		db_name=QString("%1@%2:%3").arg(conn.getConnectionParam(Connection::PARAM_DB_NAME))
															 .arg(conn.getConnectionParam(Connection::PARAM_SERVER_IP).isEmpty() ?
																		conn.getConnectionParam(Connection::PARAM_SERVER_FQDN) : conn.getConnectionParam(Connection::PARAM_SERVER_IP))
															 .arg(conn.getConnectionParam(Connection::PARAM_PORT));

		this->setWindowTitle(this->windowTitle() + " - " + db_name);
		db_name="<strong>" + db_name;
		db_name=db_name.replace("@","</strong><em>@");
		db_name+="</em>";
		db_name_lbl->setText(db_name);

		schema_cmb->clear();
		listObjects(schema_cmb, { OBJ_SCHEMA });

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
	table_cmb->clear();

	if(schema_cmb->currentIndex() > 0)
	{
		if(hide_views_chk->isChecked())
			listObjects(table_cmb, { OBJ_TABLE }, schema_cmb->currentText());
		else
			listObjects(table_cmb, { OBJ_TABLE, OBJ_VIEW }, schema_cmb->currentText());
	}

	table_lbl->setEnabled(table_cmb->count() > 0);
	table_cmb->setEnabled(table_cmb->count() > 0);
	row_cnt_lbl->setVisible(false);
	rows_ret_lbl->setVisible(false);
}

void DataManipulationForm::listColumns(void)
{
	resetAdvancedControls();
	col_names.clear();

	if(table_cmb->currentIndex() > 0)
	{
		//QStringList col_names;
		vector<attribs_map> cols;

		cols=catalog.getObjectsAttributes(OBJ_COLUMN, schema_cmb->currentText(), table_cmb->currentText());

		for(auto col : cols)
			col_names.push_back(col[ParsersAttributes::NAME]);

		//col_names.sort();
		ord_column_cmb->addItems(col_names);
		sel_column_cmb->addItems(col_names);
		//col_names.clear();
	}

	add_sel_col_tb->setEnabled(sel_column_cmb->count() > 0);
	add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
}

void DataManipulationForm::retrieveData(void)
{
	try
	{
		QString query="SELECT ";
		ResultSet res;
		unsigned limit=limit_edt->text().toUInt();

		if(sel_columns_lst->count()==0)
			query+="*";
		else
		{
			QStringList cols;

			for(int idx=0; idx < sel_columns_lst->count(); idx++)
				cols.push_back("\"" + sel_columns_lst->item(idx)->text() + "\"");

			query+=cols.join(", ");
		}

		query+=QString(" FROM \"%1\".\"%2\"").arg(schema_cmb->currentText()).arg(table_cmb->currentText());

		if(!filter_txt->toPlainText().isEmpty())
			query+=" WHERE " + filter_txt->toPlainText();


		if(ord_columns_lst->count() > 0)
		{
			QStringList ord_cols, col;

			query+=" ORDER BY ";

			for(int idx=0; idx < ord_columns_lst->count(); idx++)
			{
				col=ord_columns_lst->item(idx)->text().split(" ");
				ord_cols.push_back("\"" + col[0] + "\" " + col[1]);
			}

			query+=ord_cols.join(", ");
		}

		if(limit > 0)
			query+=QString(" LIMIT %1").arg(limit);

		connection.connect();
		connection.executeDMLCommand(query, res);

		SQLToolWidget::fillResultsTable(catalog, res, results_tbw);
		retrievePKColumns(schema_cmb->currentText(), table_cmb->currentText());

		export_tb->setEnabled(results_tbw->rowCount() > 0);
		rows_ret_lbl->setVisible(results_tbw->rowCount() > 0);
		row_cnt_lbl->setVisible(results_tbw->rowCount() > 0);
		row_cnt_lbl->setText(QString::number(results_tbw->rowCount()));

		connection.close();
	}
	catch(Exception &e)
	{
		if(connection.isStablished())
			connection.close();

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::disableControlButtons(void)
{
	refresh_tb->setEnabled(schema_cmb->currentIndex() > 0 && table_cmb->currentIndex() > 0);
	results_tbw->setRowCount(0);
	results_tbw->setColumnCount(0);
	no_pk_alert_frm->setVisible(false);
	add_tb->setEnabled(false);
	export_tb->setEnabled(false);
}

void DataManipulationForm::resetAdvancedControls(void)
{
	sel_column_cmb->clear();
	ord_column_cmb->clear();
	sel_columns_lst->clear();
	ord_columns_lst->clear();
	add_ord_col_tb->setEnabled(false);
	add_sel_col_tb->setEnabled(false);
	filter_txt->clear();
	asc_rb->setChecked(true);
	clear_ord_cols_tb->setEnabled(false);
	clear_sel_cols_tb->setEnabled(false);
}

void DataManipulationForm::addColumnToList(void)
{
	bool handling_sel_cols=(sender()==add_sel_col_tb);
	QListWidget *list=(handling_sel_cols ? sel_columns_lst : ord_columns_lst);
	QComboBox *combo=(handling_sel_cols ? sel_column_cmb : ord_column_cmb);
	QToolButton *clear_btn=(handling_sel_cols ? clear_sel_cols_tb : clear_ord_cols_tb),
	*add_btn=(handling_sel_cols ? add_sel_col_tb : add_ord_col_tb);

	if(combo->count() > 0)
	{
		QString item_text;

		item_text=combo->currentText();

		if(!handling_sel_cols)
			item_text+=(asc_rb->isChecked() ? " ASC" : " DESC");

		list->addItem(item_text);
		combo->removeItem(combo->currentIndex());
		clear_btn->setEnabled(list->count() > 0);
		add_btn->setEnabled(combo->count() > 0);
	}
}

void DataManipulationForm::removeColumnFromList(void)
{
	if(qApp->mouseButtons()==Qt::NoButton || qApp->mouseButtons()==Qt::LeftButton)
	{
		bool handling_sel_cols=(sender()==rem_sel_col_tb || sender()==sel_columns_lst);
		QListWidget *list=(handling_sel_cols ? sel_columns_lst : ord_columns_lst);
		QComboBox *combo=(handling_sel_cols ? sel_column_cmb : ord_column_cmb);
		QToolButton *clear_btn=(handling_sel_cols ? clear_sel_cols_tb : clear_ord_cols_tb),
				*add_btn=(handling_sel_cols ? add_sel_col_tb : add_ord_col_tb);
		QStringList items=col_names;
		int idx=0;

		list->takeItem(list->currentRow());

		while(idx < list->count())
			items.removeOne(list->item(idx++)->text());

		combo->clear();
		combo->addItems(items);

		clear_btn->setEnabled(list->count() > 0);
		add_btn->setEnabled(combo->count() > 0);
	}
}

void DataManipulationForm::clearColumnList(void)
{
	bool handling_sel_cols=(sender()==clear_sel_cols_tb);
	QListWidget *list=(handling_sel_cols ? sel_columns_lst : ord_columns_lst);
	QComboBox *combo=(handling_sel_cols ? sel_column_cmb : ord_column_cmb);
	QToolButton *clear_btn=(handling_sel_cols ? clear_sel_cols_tb : clear_ord_cols_tb),
	*add_btn=(handling_sel_cols ? add_sel_col_tb : add_ord_col_tb);

	combo->clear();
	combo->addItems(col_names);
	list->clear();

	clear_btn->setEnabled(false);
	add_btn->setEnabled(true);
}

void DataManipulationForm::changeOrderMode(QListWidgetItem *item)
{
	if(qApp->mouseButtons()==Qt::RightButton)
	{
		QStringList texts=item->text().split(" ");

		if(texts.size() > 1)
			texts[1]=(texts[1]=="ASC" ? "DESC" : "ASC");

		item->setText(texts[0] + " " + texts[1]);
	}
}

void DataManipulationForm::listObjects(QComboBox *combo, vector<ObjectType> obj_types, const QString &schema)
{
	try
	{
		attribs_map objects;
		QStringList items;
		int idx=0, count=0;

		catalog.setFilter(Catalog::LIST_ALL_OBJS);
		combo->blockSignals(true);
		combo->clear();

		for(auto obj_type : obj_types)
		{
			objects=catalog.getObjectsNames(obj_type, schema);

			for(auto attr : objects)
				items.push_back(attr.second);

			items.sort();
			combo->addItems(items);
			count+=items.size();
			items.clear();

			for(; idx < count; idx++)
			{
				combo->setItemIcon(idx, QPixmap(":/icones/icones/" + BaseObject::getSchemaName(obj_type) + ".png"));
				combo->setItemData(idx, obj_type);
			}

			idx=count;
		}

		if(objects.empty())
			combo->insertItem(0, trUtf8("No objects found"));
		else
			combo->insertItem(0, trUtf8("Found %1 object(s)").arg(combo->count()));

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
		vector<attribs_map> pks, pk_cols;
		QStringList col_names;

		//Retrieving the constraints from catalog using a custom filter to select only primary keys (contype=p)
		pks=catalog.getObjectsAttributes(OBJ_CONSTRAINT, schema, table, {}, {{ParsersAttributes::CUSTOM_FILTER, "contype='p'"}});

		no_pk_alert_frm->setVisible(pks.empty());
		add_tb->setEnabled(table_cmb->currentData().toUInt()==OBJ_TABLE);

		if(!pks.empty())
		{
			QStringList col_str_ids=Catalog::parseArrayValues(pks[0][ParsersAttributes::COLUMNS]);
			vector<unsigned> col_ids;

			for(auto id : col_str_ids)
				col_ids.push_back(id.toUInt());

			pk_cols=catalog.getObjectsAttributes(OBJ_COLUMN, schema, table, col_ids);

			for(auto col : pk_cols)
				col_names.push_back(col[ParsersAttributes::NAME]);

			results_tbw->setEditTriggers(QAbstractItemView::DoubleClicked);
		}
		else
			results_tbw->setEditTriggers(QAbstractItemView::NoEditTriggers);

		return(col_names);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
