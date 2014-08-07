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

const QColor DataManipulationForm::ROW_COLORS[3]={ QColor("#C0FFC0"), QColor("#FFFFC0"), QColor("#FFC0C0")  };

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
	connect(add_ord_col_tb, SIGNAL(clicked()), this, SLOT(addColumnToList()));
	connect(ord_columns_lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeColumnFromList()));
	connect(ord_columns_lst, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(changeOrderMode(QListWidgetItem*)));
	connect(rem_ord_col_tb, SIGNAL(clicked()), this, SLOT(removeColumnFromList()));
	connect(clear_ord_cols_tb, SIGNAL(clicked()), this, SLOT(clearColumnList()));
	connect(results_tbw, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(markUpdateOnRow(QTableWidgetItem *)));
	connect(delete_tb, SIGNAL(clicked()), this, SLOT(markDeleteOnRows()));
	connect(add_tb, SIGNAL(clicked()), this, SLOT(insertRow()));
	connect(undo_tb, SIGNAL(clicked()), this, SLOT(undoOperations()));

	connect(ord_columns_lst, &QListWidget::currentRowChanged,
					[=](){ rem_ord_col_tb->setEnabled(ord_columns_lst->currentRow() >= 0); });

	connect(results_tbw, &QTableWidget::itemPressed,
					[=](){ SQLToolWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
					[=](){ SQLToolWidget::exportResults(results_tbw); });

	connect(results_tbw, &QTableWidget::itemSelectionChanged,
					[=](){ 	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();
									delete_tb->setEnabled(!sel_ranges.isEmpty() &&
																				 (sel_ranges[0].leftColumn()==0 &&
																					sel_ranges[sel_ranges.count()-1].rightColumn()==results_tbw->columnCount() - 1));	});
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

		ord_column_cmb->addItems(col_names);
	}

	add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
}

void DataManipulationForm::retrieveData(void)
{
	try
	{
		QString query=QString("SELECT * FROM \"%1\".\"%2\"").arg(schema_cmb->currentText()).arg(table_cmb->currentText());
		ResultSet res;
		unsigned limit=limit_edt->text().toUInt();

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

		SQLToolWidget::fillResultsTable(catalog, res, results_tbw, true);
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
	ord_column_cmb->clear();
	ord_columns_lst->clear();
	add_ord_col_tb->setEnabled(false);
	filter_txt->clear();
	asc_rb->setChecked(true);
	clear_ord_cols_tb->setEnabled(false);
}

void DataManipulationForm::addColumnToList(void)
{
	if(ord_column_cmb->count() > 0)
	{
		QString item_text;

		item_text=ord_column_cmb->currentText();
		item_text+=(asc_rb->isChecked() ? " ASC" : " DESC");

		ord_columns_lst->addItem(item_text);
		ord_column_cmb->removeItem(ord_column_cmb->currentIndex());
		clear_ord_cols_tb->setEnabled(ord_columns_lst->count() > 0);
		add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
	}
}

void DataManipulationForm::removeColumnFromList(void)
{
	if(qApp->mouseButtons()==Qt::NoButton || qApp->mouseButtons()==Qt::LeftButton)
	{
		QStringList items=col_names;
		int idx=0;

		ord_columns_lst->takeItem(ord_columns_lst->currentRow());

		while(idx < ord_columns_lst->count())
			items.removeOne(ord_columns_lst->item(idx++)->text());

		ord_column_cmb->clear();
		ord_column_cmb->addItems(items);

		clear_ord_cols_tb->setEnabled(ord_columns_lst->count() > 0);
		add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
	}
}

void DataManipulationForm::clearColumnList(void)
{
	ord_column_cmb->clear();
	ord_column_cmb->addItems(col_names);
	ord_columns_lst->clear();

	clear_ord_cols_tb->setEnabled(false);
	add_ord_col_tb->setEnabled(true);
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

			results_tbw->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);
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

void DataManipulationForm::markOperationOnRow(unsigned operation, int row)
{
	if(row < results_tbw->rowCount() &&
		 (operation==NO_OPERATION || results_tbw->verticalHeaderItem(row)->data(Qt::UserRole)!=OP_INSERT))
	{
		QTableWidgetItem *item=nullptr, *header_item=results_tbw->verticalHeaderItem(row);
		QString tooltip=trUtf8("This row is marked to be %1");
		QFont fnt=results_tbw->font();

		if(operation==OP_DELETE)
			tooltip=tooltip.arg(trUtf8("deleted"));
		else if(operation==OP_UPDATE)
			tooltip=tooltip.arg(trUtf8("updated"));
		else if(operation==OP_INSERT)
			tooltip=tooltip.arg(trUtf8("inserted"));
		else
			tooltip.clear();

		results_tbw->blockSignals(true);

		for(int col=0; col < results_tbw->columnCount(); col++)
		{
			item=results_tbw->item(row, col);			
			item->setToolTip(tooltip);

			if(operation==NO_OPERATION || operation==OP_DELETE)
			{
				item->setFont(fnt);
				item->setText(item->data(Qt::UserRole).toString());
			}

			if(operation==NO_OPERATION)
				item->setBackground(prev_row_colors[row]);
			else
			{
				if(header_item->data(Qt::UserRole)!=OP_DELETE &&
					 header_item->data(Qt::UserRole)!=OP_UPDATE)
					prev_row_colors[row]=item->background();

				item->setBackground(ROW_COLORS[operation - 1]);
			}
		}

		if(operation==NO_OPERATION)
			changed_rows.erase(std::find(changed_rows.begin(), changed_rows.end(), row));
		else
			changed_rows.push_back(row);

		//results_tbw->verticalHeaderItem(row)->setData(Qt::UserRole, operation);
		header_item->setData(Qt::UserRole, operation);
		results_tbw->blockSignals(false);

		undo_tb->setEnabled(!changed_rows.empty());
	}
}

void DataManipulationForm::markUpdateOnRow(QTableWidgetItem *item)
{
	if(results_tbw->verticalHeaderItem(item->row())->data(Qt::UserRole)!=OP_INSERT)
	{
		bool items_changed=false;
		QTableWidgetItem *aux_item=nullptr;
		QFont fnt=item->font();

		for(int col=0; col < results_tbw->columnCount(); col++)
		{
			aux_item=results_tbw->item(item->row(), col);
			if(!items_changed && aux_item->text()!=aux_item->data(Qt::UserRole))
			{
				items_changed=true;
				break;
			}
		}

		fnt.setBold(items_changed);
		item->setFont(fnt);
		markOperationOnRow((items_changed ? OP_UPDATE : NO_OPERATION), item->row());
	}
}

void DataManipulationForm::markDeleteOnRows(void)
{
	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();
	QTableWidgetItem *item=nullptr;
	vector<int> ins_rows;

	for(int row=sel_ranges[0].topRow(); row <= sel_ranges[sel_ranges.count()-1].bottomRow(); row++)
	{
		item=results_tbw->verticalHeaderItem(row);

		if(item->data(Qt::UserRole)==OP_INSERT)
			ins_rows.push_back(row);
		else
			markOperationOnRow(OP_DELETE, row);
	}

	for(unsigned idx=0; idx < ins_rows.size(); idx++)
		results_tbw->removeRow(ins_rows[0]);

	results_tbw->clearSelection();
}

void DataManipulationForm::insertRow(void)
{
	int row=results_tbw->rowCount();

	results_tbw->blockSignals(true);
	results_tbw->insertRow(row);

	for(int col=0; col < results_tbw->columnCount(); col++)
		results_tbw->setItem(row, col, new QTableWidgetItem);

	results_tbw->setVerticalHeaderItem(row, new QTableWidgetItem("*"));
	results_tbw->blockSignals(false);
	results_tbw->scrollToItem(results_tbw->item(row, 0));

	markOperationOnRow(OP_INSERT, row);
}

void DataManipulationForm::undoOperations(void)
{
	QTableWidgetItem *item=nullptr;
	vector<int> rows=changed_rows;

	for(auto row : rows)
	{
		item=results_tbw->verticalHeaderItem(row);
		if(item && item->data(Qt::UserRole)!=OP_INSERT)
			markOperationOnRow(NO_OPERATION, row);
	}

	if(results_tbw->rowCount() > 0 &&
		 results_tbw->verticalHeaderItem(results_tbw->rowCount()-1)->data(Qt::UserRole)==OP_INSERT)
	{
		do
		{
			results_tbw->removeRow(results_tbw->rowCount()-1);
			item=results_tbw->verticalHeaderItem(results_tbw->rowCount()-1);
		}
		while(item && item->data(Qt::UserRole)==OP_INSERT);
	}

	changed_rows.clear();
	prev_row_colors.clear();
	undo_tb->setEnabled(false);
}
