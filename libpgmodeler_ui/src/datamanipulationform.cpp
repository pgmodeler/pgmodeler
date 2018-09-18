/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "sqlexecutionwidget.h"
#include "pgmodeleruins.h"
#include "pgmodelerns.h"
#include "plaintextitemdelegate.h"
#include "baseform.h"
#include "bulkdataeditwidget.h"
#include "databaseexplorerwidget.h"
#include "generalconfigwidget.h"

const QColor DataManipulationForm::ROW_COLORS[3]={ QColor(QString("#C0FFC0")), QColor(QString("#FFFFC0")), QColor(QString("#FFC0C0"))  };
const unsigned DataManipulationForm::NO_OPERATION=0;
const unsigned DataManipulationForm::OP_INSERT=1;
const unsigned DataManipulationForm::OP_UPDATE=2;
const unsigned DataManipulationForm::OP_DELETE=3;
bool DataManipulationForm::has_csv_clipboard=false;

DataManipulationForm::DataManipulationForm(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f)
{
	QAction *act = nullptr;
	setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

	table_oid=0;

	PgModelerUiNS::configureWidgetFont(hint_lbl, PgModelerUiNS::MEDIUM_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(warning_lbl, PgModelerUiNS::MEDIUM_FONT_FACTOR);

	filter_hl=new SyntaxHighlighter(filter_txt);
	filter_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

	code_compl_wgt=new CodeCompletionWidget(filter_txt);
	code_compl_wgt->configureCompletion(nullptr, filter_hl);

	results_tbw->setItemDelegate(new PlainTextItemDelegate(this, false));
	browse_tabs_tb->setMenu(&fks_menu);

	act = copy_menu.addAction(trUtf8("Copy as CSV"));
	act->setShortcut(QKeySequence("Ctrl+C"));
	connect(act, &QAction::triggered, [&](){
		SQLExecutionWidget::copySelection(results_tbw, false, true);
		has_csv_clipboard = true;
		paste_tb->setEnabled(true);
	});

	act = copy_menu.addAction(trUtf8("Copy as text"));
	act->setShortcut(QKeySequence("Ctrl+Shift+C"));
	connect(act, &QAction::triggered,	[&](){
		SQLExecutionWidget::copySelection(results_tbw, false, false);
		has_csv_clipboard = false;
		paste_tb->setEnabled(true);
	});

	truncate_tb->setMenu(&truncate_menu);
	truncate_menu.addAction(QIcon(PgModelerUiNS::getIconPath("truncate")), trUtf8("Truncate"), this, SLOT(truncateTable()))->setData(QVariant::fromValue<bool>(false));
	truncate_menu.addAction(QIcon(PgModelerUiNS::getIconPath("trunccascade")), trUtf8("Truncate cascade"), this, SLOT(truncateTable()))->setData(QVariant::fromValue<bool>(true));

	copy_tb->setMenu(&copy_menu);
	refresh_tb->setToolTip(refresh_tb->toolTip() + QString(" (%1)").arg(refresh_tb->shortcut().toString()));
	save_tb->setToolTip(save_tb->toolTip() + QString(" (%1)").arg(save_tb->shortcut().toString()));
	undo_tb->setToolTip(undo_tb->toolTip() + QString(" (%1)").arg(undo_tb->shortcut().toString()));
	export_tb->setToolTip(export_tb->toolTip() + QString(" (%1)").arg(export_tb->shortcut().toString()));
	delete_tb->setToolTip(delete_tb->toolTip() + QString(" (%1)").arg(delete_tb->shortcut().toString()));
	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	duplicate_tb->setToolTip(duplicate_tb->toolTip() + QString(" (%1)").arg(duplicate_tb->shortcut().toString()));
	paste_tb->setToolTip(paste_tb->toolTip() + QString(" (%1)").arg(paste_tb->shortcut().toString()));
	result_info_wgt->setVisible(false);

	//Forcing the splitter that handles the bottom widgets to resize its children to their minimum size
	h_splitter->setSizes({500, 250, 500});
	filter_tbw->setVisible(false);
	csv_load_parent->setVisible(false);

	csv_load_wgt = new CsvLoadWidget(this, false);
	QVBoxLayout *layout = new QVBoxLayout;

	layout->addWidget(csv_load_wgt);
	layout->setContentsMargins(0,0,0,0);
	csv_load_parent->setLayout(layout);
	csv_load_parent->setMinimumSize(csv_load_wgt->minimumSize());

	connect(paste_tb, &QToolButton::clicked, [&]{
		loadDataFromCsv(true);
		paste_tb->setEnabled(false);
	});

	connect(columns_lst, &QListWidget::itemDoubleClicked, [&](QListWidgetItem *item){
	  if(item->checkState() == Qt::Checked)
		item->setCheckState(Qt::Unchecked);
	  else
		item->setCheckState(Qt::Checked);

	  toggleColumnDisplay(item);
	});

	connect(select_all_tb, &QToolButton::clicked, [&](){
	  setColumnsCheckState(Qt::Checked);
	});

	connect(clear_all_tb, &QToolButton::clicked, [&](){
	  setColumnsCheckState(Qt::Unchecked);
	});

	connect(columns_lst, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(toggleColumnDisplay(QListWidgetItem*)));
	connect(csv_load_tb, SIGNAL(toggled(bool)), csv_load_parent, SLOT(setVisible(bool)));
	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(schema_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listTables()));
	connect(hide_views_chk, SIGNAL(toggled(bool)), this, SLOT(listTables()));
	connect(schema_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disableControlButtons()));
	connect(table_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disableControlButtons()));
	connect(table_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listColumns()));
	connect(table_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(retrieveData()));
	connect(refresh_tb, SIGNAL(clicked()), this, SLOT(retrieveData()));
	connect(add_ord_col_tb, SIGNAL(clicked()), this, SLOT(addColumnToList()));
	connect(ord_columns_lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeColumnFromList()));
	connect(ord_columns_lst, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(changeOrderMode(QListWidgetItem*)));
	connect(rem_ord_col_tb, SIGNAL(clicked()), this, SLOT(removeColumnFromList()));
	connect(clear_ord_cols_tb, SIGNAL(clicked()), this, SLOT(clearColumnList()));
	connect(results_tbw, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(markUpdateOnRow(QTableWidgetItem *)));
	connect(delete_tb, SIGNAL(clicked()), this, SLOT(markDeleteOnRows()));
	connect(add_tb, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(duplicate_tb, SIGNAL(clicked()), this, SLOT(duplicateRows()));
	connect(undo_tb, SIGNAL(clicked()), this, SLOT(undoOperations()));
	connect(save_tb, SIGNAL(clicked()), this, SLOT(saveChanges()));
	connect(ord_columns_lst, SIGNAL(currentRowChanged(int)), this, SLOT(enableColumnControlButtons()));
	connect(move_down_tb, SIGNAL(clicked()), this, SLOT(swapColumns()));
	connect(move_up_tb, SIGNAL(clicked()), this, SLOT(swapColumns()));
	connect(filter_tb, SIGNAL(toggled(bool)), filter_tbw, SLOT(setVisible(bool)));
	connect(truncate_tb, SIGNAL(clicked(bool)), this, SLOT(truncateTable()));

	connect(bulkedit_tb, &QToolButton::clicked, [&](){
		PgModelerUiNS::bulkDataEdit(results_tbw);
	});

	connect(filter_tb, &QToolButton::toggled,
			[&](bool checked){

				v_splitter->setVisible(checked);

				if(checked)
					filter_txt->setFocus();
	});

	//Using the QueuedConnection here to avoid the "edit: editing failed" when editing and navigating through items using tab key
	connect(results_tbw, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(insertRowOnTabPress(int,int,int,int)), Qt::QueuedConnection);

	connect(results_tbw, &QTableWidget::itemPressed,
	[&](){
					if(QApplication::mouseButtons()==Qt::RightButton)
					{
						QMenu item_menu;
						QAction *act = nullptr;
						ObjectType obj_type=static_cast<ObjectType>(table_cmb->currentData().toUInt());

						act = item_menu.addAction(QIcon(PgModelerUiNS::getIconPath("copiar")), trUtf8("Copy items"));
						act->setMenu(&copy_menu);

						act = item_menu.addAction(QIcon(PgModelerUiNS::getIconPath("colar")), trUtf8("Pase items"));
						act->setShortcut(paste_tb->shortcut());
						connect(act, SIGNAL(triggered(bool)), paste_tb, SLOT(click()));
						act->setEnabled(!qApp->clipboard()->text().isEmpty() && obj_type == OBJ_TABLE);

						if(obj_type == OBJ_TABLE)
						{
							item_menu.addSeparator();
							act = item_menu.addAction(browse_tabs_tb->icon(), trUtf8("Browse tables"));
							act->setMenu(&fks_menu);
							act->setEnabled(browse_tabs_tb->isEnabled());

							item_menu.addSeparator();
							act = item_menu.addAction(duplicate_tb->icon(), trUtf8("Duplicate row(s)"), this, SLOT(duplicateRows()), duplicate_tb->shortcut());
							act->setEnabled(duplicate_tb->isEnabled());

							act = item_menu.addAction(delete_tb->icon(), trUtf8("Delete row(s)"), this, SLOT(markDeleteOnRows()), delete_tb->shortcut());
							act->setEnabled(delete_tb->isEnabled());

							act = item_menu.addAction(bulkedit_tb->icon(), trUtf8("Edit cell(s)"), bulkedit_tb, SLOT(click()), bulkedit_tb->shortcut());
							act->setEnabled(bulkedit_tb->isEnabled());
						}

						item_menu.exec(QCursor::pos());
					}
		});


	connect(export_tb, &QToolButton::clicked,
			[&](){ SQLExecutionWidget::exportResults(results_tbw); });

	connect(results_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(enableRowControlButtons()));
	connect(csv_load_wgt, SIGNAL(s_csvFileLoaded()), this, SLOT(loadDataFromCsv()));
}

void DataManipulationForm::setAttributes(Connection conn, const QString curr_schema, const QString curr_table, const QString &filter)
{
	try
	{
		QString db_name;

		tmpl_conn_params=conn.getConnectionParams();
		db_name=QString("<strong>%1</strong>@<em>%2:%3</em>").arg(conn.getConnectionParam(Connection::PARAM_DB_NAME))
				.arg(conn.getConnectionParam(Connection::PARAM_SERVER_IP).isEmpty() ?
						 conn.getConnectionParam(Connection::PARAM_SERVER_FQDN) : conn.getConnectionParam(Connection::PARAM_SERVER_IP))
				.arg(conn.getConnectionParam(Connection::PARAM_PORT));

		db_name_lbl->setText(db_name);
		db_name.remove(QRegExp("<(/)?(strong|em)>"));
		this->setWindowTitle(this->windowTitle() + QString(" - ") + db_name);

		schema_cmb->clear();
		listObjects(schema_cmb, { OBJ_SCHEMA });

		disableControlButtons();
		schema_cmb->setCurrentText(curr_schema);

		if(!filter.isEmpty() && !curr_schema.isEmpty() && !curr_table.isEmpty())
		{
			table_cmb->blockSignals(true);
			table_cmb->setCurrentText(curr_table);
			table_cmb->blockSignals(false);

			listColumns();
			filter_txt->setPlainText(filter);
			retrieveData();
			refresh_tb->setEnabled(true);
		}
		else
			table_cmb->setCurrentText(curr_table);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::setHasCsvClipboard(bool value)
{
  has_csv_clipboard = value;
}

void DataManipulationForm::reject(void)
{
  GeneralConfigWidget::saveWidgetGeometry(this);
  QDialog::reject();
}

void DataManipulationForm::listTables(void)
{
	table_cmb->clear();
	csv_load_tb->setChecked(false);

	if(schema_cmb->currentIndex() > 0)
	{
		if(hide_views_chk->isChecked())
			listObjects(table_cmb, { OBJ_TABLE }, schema_cmb->currentText());
		else
			listObjects(table_cmb, { OBJ_TABLE, OBJ_VIEW }, schema_cmb->currentText());
	}

	table_lbl->setEnabled(table_cmb->count() > 0);
	table_cmb->setEnabled(table_cmb->count() > 0);
	result_info_wgt->setVisible(false);
}

void DataManipulationForm::listColumns(void)
{
	Catalog catalog;
	Connection conn=Connection(tmpl_conn_params);

	try
	{
		resetAdvancedControls();
		col_names.clear();
		code_compl_wgt->clearCustomItems();

		if(table_cmb->currentIndex() > 0)
		{
			vector<attribs_map> cols;

			catalog.setConnection(conn);
			cols=catalog.getObjectsAttributes(OBJ_COLUMN, schema_cmb->currentText(), table_cmb->currentText());

			for(auto &col : cols)
			{
				col_names.push_back(col[ParsersAttributes::NAME]);
				code_compl_wgt->insertCustomItem(col[ParsersAttributes::NAME], {},
				QPixmap(PgModelerUiNS::getIconPath("column")));
			}

			ord_column_cmb->addItems(col_names);
		}

		add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
		filter_tb->setEnabled(ord_column_cmb->count() > 0);
	}
	catch(Exception &e)
	{
		catalog.closeConnection();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

}

void DataManipulationForm::retrieveData(void)
{
	if(table_cmb->currentIndex() <= 0)
		return;

	Messagebox msg_box;
	Catalog catalog;
	Connection conn_sql=Connection(tmpl_conn_params),
			conn_cat=Connection(tmpl_conn_params);

	try
	{
		if(!changed_rows.empty())
		{
			msg_box.show(trUtf8("<strong>WARNING: </strong> There are some changed rows waiting the commit! Do you really want to discard them and retrieve the data now?"),
						 Messagebox::ALERT_ICON, Messagebox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Rejected)
				return;
		}

		QString query=QString("SELECT * FROM \"%1\".\"%2\"").arg(schema_cmb->currentText()).arg(table_cmb->currentText());
		ResultSet res;
		unsigned limit=limit_spb->value();

		//Building the where clause
		if(!filter_txt->toPlainText().trimmed().isEmpty())
			query+=QString(" WHERE ") + filter_txt->toPlainText();

		//Building the order by clause
		if(ord_columns_lst->count() > 0)
		{
			QStringList ord_cols, col;

			query+=QString("\n ORDER BY ");

			for(int idx=0; idx < ord_columns_lst->count(); idx++)
			{
				col=ord_columns_lst->item(idx)->text().split(" ");
				ord_cols.push_back(QString("\"") + col[0] + QString("\" ") + col[1]);
			}

			query+=ord_cols.join(QString(", "));
		}

		//Building the limit clause
		if(limit > 0)
			query+=QString(" LIMIT %1").arg(limit);

		QApplication::setOverrideCursor(Qt::WaitCursor);

		catalog.setConnection(conn_cat);
		conn_sql.connect();
		conn_sql.executeDMLCommand(query, res);

		retrievePKColumns(schema_cmb->currentText(), table_cmb->currentText());
		retrieveFKColumns(schema_cmb->currentText(), table_cmb->currentText());
		SQLExecutionWidget::fillResultsTable(catalog, res, results_tbw, true);

		export_tb->setEnabled(results_tbw->rowCount() > 0);
		result_info_wgt->setVisible(results_tbw->rowCount() > 0);
		result_info_lbl->setText(QString("<em>[%1]</em> ").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))) +
								 trUtf8("Rows returned: <strong>%1</strong>&nbsp;&nbsp;&nbsp;").arg(results_tbw->rowCount()) +
								 trUtf8("<em>(Limit: <strong>%1</strong>)</em>").arg(limit_spb->value()==0 ? trUtf8("none") : QString::number(limit_spb->value())));

		//Reset the changed rows state
		clearChangedRows();

		//If the table is empty automatically creates a new row
		if(results_tbw->rowCount()==0 && table_cmb->currentData(Qt::UserRole).toUInt()==OBJ_TABLE)
			addRow();
		else
			results_tbw->setFocus();

		if(table_cmb->currentData(Qt::UserRole).toUInt()==OBJ_TABLE)
			csv_load_tb->setEnabled(!col_names.isEmpty());
		else
		{
			csv_load_tb->setEnabled(false);
			csv_load_tb->setChecked(false);
		}

		conn_sql.close();
		catalog.closeConnection();

		QApplication::restoreOverrideCursor();

		paste_tb->setEnabled(!qApp->clipboard()->text().isEmpty() &&
												 table_cmb->currentData().toUInt() == OBJ_TABLE &&
												 !col_names.isEmpty());

		truncate_tb->setEnabled(table_cmb->currentData().toUInt() == OBJ_TABLE &&
														res.getTupleCount() > 0 &&
														!col_names.isEmpty());

		code_compl_wgt->clearCustomItems();
		code_compl_wgt->insertCustomItems(col_names, trUtf8("Column"), OBJ_COLUMN);

		columns_lst->clear();
		QListWidgetItem *item = nullptr;

		for(auto &col : col_names)
		{
		  columns_lst->addItem(col);
		  item = columns_lst->item(columns_lst->count() - 1);
		  item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		  item->setCheckState(Qt::Checked);
		  item->setData(Qt::UserRole, item->checkState());
		}

		//Restoring the visibily of the columns
		for(int idx = 0; idx < results_tbw->horizontalHeader()->count(); idx++)
		   results_tbw->horizontalHeader()->setSectionHidden(idx, false);
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		conn_sql.close();
		catalog.closeConnection();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::disableControlButtons(void)
{
	refresh_tb->setEnabled(schema_cmb->currentIndex() > 0 && table_cmb->currentIndex() > 0);
	results_tbw->setRowCount(0);
	results_tbw->setColumnCount(0);
	warning_frm->setVisible(false);
	hint_frm->setVisible(false);
	add_tb->setEnabled(false);
	duplicate_tb->setEnabled(false);
	export_tb->setEnabled(false);
	paste_tb->setEnabled(false);
	truncate_tb->setEnabled(false);
	csv_load_tb->setEnabled(false);
	csv_load_tb->setChecked(false);
	clearChangedRows();
}

void DataManipulationForm::enableRowControlButtons(void)
{
	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();
	bool cols_selected, rows_selected;

	cols_selected = rows_selected = !sel_ranges.isEmpty();

	for(auto &sel_rng : sel_ranges)
	{
		cols_selected &= (sel_rng.columnCount() == results_tbw->columnCount());
		rows_selected &= (sel_rng.rowCount() == results_tbw->rowCount());
	}

	delete_tb->setEnabled(cols_selected);
	duplicate_tb->setEnabled(cols_selected);
	copy_tb->setEnabled(sel_ranges.count() != 0);
	paste_tb->setEnabled(!qApp->clipboard()->text().isEmpty() &&
											 table_cmb->currentData().toUInt() == OBJ_TABLE  &&
											 !col_names.isEmpty());
	browse_tabs_tb->setEnabled((!fk_infos.empty() || !ref_fk_infos.empty()) && sel_ranges.count() == 1 && sel_ranges.at(0).rowCount() == 1);
	bulkedit_tb->setEnabled(sel_ranges.count() != 0);
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
		item_text+=(asc_rb->isChecked() ? QString(" ASC") : QString(" DESC"));

		ord_columns_lst->addItem(item_text);
		ord_column_cmb->removeItem(ord_column_cmb->currentIndex());
		enableColumnControlButtons();
	}
}

void DataManipulationForm::enableColumnControlButtons(void)
{
	clear_ord_cols_tb->setEnabled(ord_columns_lst->count() > 0);
	add_ord_col_tb->setEnabled(ord_column_cmb->count() > 0);
	rem_ord_col_tb->setEnabled(ord_columns_lst->currentRow() >= 0);
	move_up_tb->setEnabled(ord_columns_lst->count() > 1 && ord_columns_lst->currentRow() > 0);
	move_down_tb->setEnabled(ord_columns_lst->count() > 1 &&
							 ord_columns_lst->currentRow() >= 0 &&
							 ord_columns_lst->currentRow() <= ord_columns_lst->count() - 2);
}

void DataManipulationForm::swapColumns(void)
{
	int curr_idx=0, new_idx=0;
	QStringList items;

	curr_idx=new_idx=ord_columns_lst->currentRow();

	if(sender()==move_up_tb)
		new_idx--;
	else
		new_idx++;

	for(int i=0; i < ord_columns_lst->count(); i++)
		items.push_back(ord_columns_lst->item(i)->text());

	items.move(curr_idx, new_idx);
	ord_columns_lst->blockSignals(true);
	ord_columns_lst->clear();
	ord_columns_lst->addItems(items);
	ord_columns_lst->blockSignals(false);
	ord_columns_lst->setCurrentRow(new_idx);
}

void DataManipulationForm::loadDataFromCsv(bool load_from_clipboard)
{
	QList<QStringList> rows;
	QStringList cols;
	int row_id = 0, col_id = 0;

	if(load_from_clipboard)
	{
		if(qApp->clipboard()->text().isEmpty())
			return;

		if(has_csv_clipboard)
			rows = CsvLoadWidget::loadCsvFromBuffer(qApp->clipboard()->text(), QString(";"), QString("\""), true, cols);
		else
			rows = CsvLoadWidget::loadCsvFromBuffer(qApp->clipboard()->text(), QString("\t"), QString(), false, cols);

		has_csv_clipboard = false;
		qApp->clipboard()->clear();
	}
	else
	{
		rows = csv_load_wgt->getCsvRows();
		cols = csv_load_wgt->getCsvColumns();
	}

	/* If there is only one empty row in the grid, this one will
	be removed prior the csv loading */
	if(results_tbw->rowCount()==1)
	{
		bool is_empty=true;

		for(int col=0; col < results_tbw->columnCount(); col++)
		{
			if(!results_tbw->item(0, col)->text().isEmpty())
			{
				is_empty=false;
				break;
			}
		}

		if(is_empty)
			removeNewRows({0});
	}

	for(QStringList &values : rows)
	{
		addRow();
		row_id=results_tbw->rowCount() - 1;

		for(int i = 0; i < values.count() && i < cols.count(); i++)
		{
			if((!load_from_clipboard && csv_load_wgt->isColumnsInFirstRow()) ||
				 (load_from_clipboard && !cols.isEmpty()))
			{
				//First we need to get the index of the column by its name
				col_id=col_names.indexOf(cols[i]);

				//If a matching column is not found we add the value at the current position
				if(col_id < 0)
					col_id = i;

				if(col_id >= 0 && col_id < results_tbw->columnCount())
					results_tbw->item(row_id, col_id)->setText(values.at(i));
			}
			else if(i < results_tbw->columnCount())
			{
				//Insert the value to the cell in order of appearance
				results_tbw->item(row_id, i)->setText(values.at(i));
			}
		}
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
		enableColumnControlButtons();
	}
}

void DataManipulationForm::clearColumnList(void)
{
	ord_column_cmb->clear();
	ord_column_cmb->addItems(col_names);
	ord_columns_lst->clear();

	clear_ord_cols_tb->setEnabled(false);
	move_up_tb->setEnabled(false);
	move_down_tb->setEnabled(false);
	add_ord_col_tb->setEnabled(true);
}

void DataManipulationForm::changeOrderMode(QListWidgetItem *item)
{
	if(qApp->mouseButtons()==Qt::RightButton)
	{
		QStringList texts=item->text().split(QString(" "));

		if(texts.size() > 1)
			texts[1]=(texts[1]==QString("ASC") ? QString("DESC") : QString("ASC"));

		item->setText(texts[0] + QString(" ") + texts[1]);
	}
}

void DataManipulationForm::listObjects(QComboBox *combo, vector<ObjectType> obj_types, const QString &schema)
{
	Catalog catalog;
	Connection conn=Connection(tmpl_conn_params);

	try
	{
		attribs_map objects;
		QStringList items;
		int idx=0, count=0;

		QApplication::setOverrideCursor(Qt::WaitCursor);

		catalog.setConnection(conn);
		catalog.setFilter(Catalog::LIST_ALL_OBJS);
		combo->blockSignals(true);
		combo->clear();

		for(auto &obj_type : obj_types)
		{
			objects=catalog.getObjectsNames(obj_type, schema);

			for(auto &attr : objects)
				items.push_back(attr.second);

			items.sort();
			combo->addItems(items);
			count+=items.size();
			items.clear();

			for(; idx < count; idx++)
			{
				combo->setItemIcon(idx, QPixmap(PgModelerUiNS::getIconPath(obj_type)));
				combo->setItemData(idx, obj_type);
			}

			idx=count;
		}

		if(combo->count()==0)
			combo->insertItem(0, trUtf8("No objects found"));
		else
			combo->insertItem(0, trUtf8("Found %1 object(s)").arg(combo->count()));

		combo->setCurrentIndex(0);
		combo->blockSignals(false);
		catalog.closeConnection();

		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		catalog.closeConnection();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::retrievePKColumns(const QString &schema, const QString &table)
{
	Catalog catalog;
	Connection conn=Connection(tmpl_conn_params);

	try
	{
		vector<attribs_map> pks, columns;
		ObjectType obj_type=static_cast<ObjectType>(table_cmb->currentData().toUInt());

		table_oid = 0;

		if(obj_type==OBJ_VIEW)
		{
			warning_frm->setVisible(true);
			warning_lbl->setText(trUtf8("Views can't have their data handled through this grid, this way, all operations are disabled."));
		}
		else
		{
			catalog.setConnection(conn);
			//Retrieving the constraints from catalog using a custom filter to select only primary keys (contype=p)
			pks=catalog.getObjectsAttributes(OBJ_CONSTRAINT, schema, table, {}, {{ParsersAttributes::CUSTOM_FILTER, QString("contype='p'")}});

			warning_frm->setVisible(pks.empty());

			if(pks.empty())
				warning_lbl->setText(trUtf8("The selected table doesn't owns a primary key! Updates and deletes will be performed by considering all columns as primary key. <strong>WARNING:</strong> those operations can affect more than one row."));
			else
				table_oid = pks[0][ParsersAttributes::TABLE].toUInt();
		}

		hint_frm->setVisible(obj_type==OBJ_TABLE);
		add_tb->setEnabled(obj_type==OBJ_TABLE && !col_names.empty());
		pk_col_names.clear();

		if(!pks.empty())
		{
			QStringList col_str_ids=Catalog::parseArrayValues(pks[0][ParsersAttributes::COLUMNS]);
			vector<unsigned> col_ids;

			for(QString id : col_str_ids)
				col_ids.push_back(id.toUInt());

			columns=catalog.getObjectsAttributes(OBJ_COLUMN, schema, table, col_ids);

			for(auto &col : columns)
				pk_col_names.push_back(col[ParsersAttributes::NAME]);
		}

		catalog.closeConnection();

		//For tables, even if there is no pk the user can manipulate data
		if(obj_type==OBJ_TABLE)
			results_tbw->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);
		else
			results_tbw->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
	catch(Exception &e)
	{
		catalog.closeConnection();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DataManipulationForm::retrieveFKColumns(const QString &schema, const QString &table)
{
	Catalog catalog;
	Connection conn=Connection(tmpl_conn_params);

	try
	{
		QAction *action = nullptr;
		vector<attribs_map> fks, ref_fks;
		ObjectType obj_type=static_cast<ObjectType>(table_cmb->currentData().toUInt());
		QString fk_name;

		fks_menu.clear();
		ref_fk_infos.clear();
		fk_infos.clear();

		if(obj_type==OBJ_VIEW)
			return;

		catalog.setConnection(conn);

		//Retrieving the constraints from catalog using a custom filter to select only foreign keys (contype=f)
		fks=catalog.getObjectsAttributes(OBJ_CONSTRAINT, schema, table, {}, {{ParsersAttributes::CUSTOM_FILTER, QString("contype='f'")}});
		ref_fks=catalog.getObjectsAttributes(OBJ_CONSTRAINT, QString(), QString(), {}, {{ParsersAttributes::CUSTOM_FILTER, QString("contype='f' AND cs.confrelid=%1").arg(table_oid)}});

		if(!fks.empty() || !ref_fks.empty())
		{
			vector<unsigned> col_ids;
			QMenu *submenu = nullptr;

			attribs_map aux_table, aux_schema;
			QStringList name_list;

			submenu = new QMenu(this);
			fks_menu.addAction(QPixmap(PgModelerUiNS::getIconPath("referenced")), trUtf8("Referenced tables"))->setMenu(submenu);

			if(fks.empty())
				submenu->addAction(trUtf8("(none)"))->setEnabled(false);

			for(auto &fk : fks)
			{				
				aux_table = catalog.getObjectAttributes(OBJ_TABLE, fk[ParsersAttributes::REF_TABLE].toUInt());
				aux_schema = catalog.getObjectAttributes(OBJ_SCHEMA, aux_table[ParsersAttributes::SCHEMA].toUInt());
				fk_name = QString("%1.%2.%3")
									.arg(aux_schema[ParsersAttributes::NAME])
									.arg(aux_table[ParsersAttributes::NAME])
									.arg(fk[ParsersAttributes::NAME]);

				//Store the referenced schema and table names
				fk_infos[fk_name][ParsersAttributes::REF_TABLE] = aux_table[ParsersAttributes::NAME];
				fk_infos[fk_name][ParsersAttributes::SCHEMA] = aux_schema[ParsersAttributes::NAME];
				action = submenu->addAction(QPixmap(PgModelerUiNS::getIconPath("table")),
																		QString("%1.%2 (%3)").arg(aux_schema[ParsersAttributes::NAME])
																													.arg(aux_table[ParsersAttributes::NAME])
																													.arg(fk[ParsersAttributes::NAME]), this, SLOT(browseReferencedTable()));
				action->setData(fk_name);

				col_ids.clear();
				name_list.clear();

				//Storing the source columns in a string
				for(QString id : Catalog::parseArrayValues(fk[ParsersAttributes::SRC_COLUMNS]))
					col_ids.push_back(id.toUInt());

				for(auto &col : catalog.getObjectsAttributes(OBJ_COLUMN, schema, table, col_ids))
					name_list.push_back(BaseObject::formatName(col[ParsersAttributes::NAME]));

				fk_infos[fk_name][ParsersAttributes::SRC_COLUMNS] = name_list.join(Table::DATA_SEPARATOR);

				col_ids.clear();
				name_list.clear();

				//Storing the referenced columns in a string
				for(QString id : Catalog::parseArrayValues(fk[ParsersAttributes::DST_COLUMNS]))
					col_ids.push_back(id.toUInt());

				for(auto &col : catalog.getObjectsAttributes(OBJ_COLUMN, aux_schema[ParsersAttributes::NAME], aux_table[ParsersAttributes::NAME], col_ids))
					name_list.push_back(BaseObject::formatName(col[ParsersAttributes::NAME]));

				fk_infos[fk_name][ParsersAttributes::DST_COLUMNS] = name_list.join(Table::DATA_SEPARATOR);
			}

			submenu = new QMenu(this);
			fks_menu.addAction(QPixmap(PgModelerUiNS::getIconPath("referrer")), trUtf8("Referrer tables"))->setMenu(submenu);

			if(ref_fks.empty())
				submenu->addAction(trUtf8("(none)"))->setEnabled(false);

			for(auto &fk : ref_fks)
			{
				col_ids.clear();
				name_list.clear();

				aux_table = catalog.getObjectAttributes(OBJ_TABLE, fk[ParsersAttributes::TABLE].toUInt());
				aux_schema = catalog.getObjectAttributes(OBJ_SCHEMA, aux_table[ParsersAttributes::SCHEMA].toUInt());
				fk_name = QString("%1.%2.%3")
									.arg(aux_schema[ParsersAttributes::NAME])
									.arg(aux_table[ParsersAttributes::NAME])
									.arg(fk[ParsersAttributes::NAME]);

				//Storing the source columns in a string
				for(QString id : Catalog::parseArrayValues(fk[ParsersAttributes::SRC_COLUMNS]))
					col_ids.push_back(id.toUInt());

				for(auto &col : catalog.getObjectsAttributes(OBJ_COLUMN, aux_schema[ParsersAttributes::NAME], aux_table[ParsersAttributes::NAME], col_ids))
					name_list.push_back(BaseObject::formatName(col[ParsersAttributes::NAME]));

				action = submenu->addAction(QPixmap(PgModelerUiNS::getIconPath("table")),
																		QString("%1.%2 (%3)").arg(aux_schema[ParsersAttributes::NAME])
																													.arg(aux_table[ParsersAttributes::NAME])
																													.arg(fk[ParsersAttributes::NAME]), this, SLOT(browseReferrerTable()));
				action->setData(fk_name);

				ref_fk_infos[fk_name][ParsersAttributes::SRC_COLUMNS] = name_list.join(Table::DATA_SEPARATOR);
				ref_fk_infos[fk_name][ParsersAttributes::TABLE] = aux_table[ParsersAttributes::NAME];
				ref_fk_infos[fk_name][ParsersAttributes::SCHEMA] = aux_schema[ParsersAttributes::NAME];
			}
		}

		catalog.closeConnection();
	}
	catch(Exception &e)
	{
		catalog.closeConnection();
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
		int marked_cols=0;

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

			if(results_tbw->horizontalHeaderItem(col)->data(Qt::UserRole)!=QString("bytea"))
			{
				item->setToolTip(tooltip);

				//Restore the item's font and text when the operation is delete or none
				if(operation==NO_OPERATION || operation==OP_DELETE)
				{
					item->setFont(fnt);
					item->setText(item->data(Qt::UserRole).toString());
				}

				if(operation==NO_OPERATION)
					//Restore the item's background
					item->setBackground(prev_row_colors[row]);
				else
				{
					//Saves the item's background if it isn't already marked
					if(header_item->data(Qt::UserRole)!=OP_DELETE &&
							header_item->data(Qt::UserRole)!=OP_UPDATE)
						prev_row_colors[row]=item->background();

					//Changes the item's background according to the operation
					item->setBackground(ROW_COLORS[operation - 1]);
				}

				marked_cols++;
			}
		}

		if(marked_cols > 0)
		{
			auto itr=std::find(changed_rows.begin(), changed_rows.end(), row);

			if(operation==NO_OPERATION && itr!=changed_rows.end())
			{
				changed_rows.erase(std::find(changed_rows.begin(), changed_rows.end(), row));
				prev_row_colors.erase(row);
			}
			else if(operation!=NO_OPERATION && itr==changed_rows.end())
				changed_rows.push_back(row);

			header_item->setData(Qt::UserRole, operation);
			undo_tb->setEnabled(!changed_rows.empty());
			save_tb->setEnabled(!changed_rows.empty());
			std::sort(changed_rows.begin(), changed_rows.end());
		}

		results_tbw->blockSignals(false);
	}
}

void DataManipulationForm::markUpdateOnRow(QTableWidgetItem *item)
{
	if(results_tbw->verticalHeaderItem(item->row())->data(Qt::UserRole)!=OP_INSERT)
	{
		bool items_changed=false;
		QTableWidgetItem *aux_item=nullptr;
		QFont fnt=item->font();

		//Before mark the row to update it's needed to check if some item was changed
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

	for(auto &sel_rng : sel_ranges)
	{
		for(int row=sel_rng.topRow(); row <= sel_rng.bottomRow(); row++)
		{
			item=results_tbw->verticalHeaderItem(row);

			if(item->data(Qt::UserRole)==OP_INSERT)
				ins_rows.push_back(row);
			else
				markOperationOnRow(OP_DELETE, row);
		}
	}

	removeNewRows(ins_rows);
	results_tbw->clearSelection();
}

void DataManipulationForm::addRow(bool focus_new_row)
{
	int row=results_tbw->rowCount();
	QTableWidgetItem *item=nullptr;

	results_tbw->blockSignals(true);
	results_tbw->insertRow(row);

	for(int col=0; col < results_tbw->columnCount(); col++)
	{
		item=new QTableWidgetItem;

		//bytea (binary data) columns can't be handled this way the new item is disabled
		if(results_tbw->horizontalHeaderItem(col)->data(Qt::UserRole)==QString("bytea"))
		{
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			item->setText(trUtf8("[binary data]"));
		}
		else
			item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		results_tbw->setItem(row, col, item);
	}

	results_tbw->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
	results_tbw->blockSignals(false);

	markOperationOnRow(OP_INSERT, row);
	item=results_tbw->item(row, 0);
	hint_frm->setVisible(true);

	if(focus_new_row)
	{
		results_tbw->setFocus();
		results_tbw->setCurrentCell(row, 0, QItemSelectionModel::ClearAndSelect);
		results_tbw->editItem(item);
	}
}

void DataManipulationForm::duplicateRows(void)
{
	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

	if(!sel_ranges.isEmpty())
	{
		for(auto &sel_rng : sel_ranges)
		{
			for(int row=sel_rng.topRow(); row <= sel_rng.bottomRow(); row++)
			{
				addRow(false);

				for(int col=0; col < results_tbw->columnCount(); col++)
				{
					results_tbw->item(results_tbw->rowCount() - 1, col)
							->setText(results_tbw->item(row, col)->text());
				}
			}
		}

		results_tbw->setCurrentItem(results_tbw->item(results_tbw->rowCount() - 1, 0), QItemSelectionModel::ClearAndSelect);
	}
}

void DataManipulationForm::removeNewRows(const vector<int> &ins_rows)
{
	if(!ins_rows.empty())
	{
		unsigned idx=0, cnt=ins_rows.size();
		int row_idx=0;
		vector<int>::reverse_iterator itr, itr_end;

		//Mark the rows as no-op to remove their indexes from changed rows set
		for(idx=0; idx < cnt; idx++)
			markOperationOnRow(NO_OPERATION, ins_rows[idx]);

		//Remove the rows
		for(idx=0; idx < cnt; idx++)
			results_tbw->removeRow(ins_rows[0]);

		//Reorganizing the changed rows vector to avoid row index out-of-bound errors
		row_idx=results_tbw->rowCount() - 1;
		itr=changed_rows.rbegin();
		itr_end=changed_rows.rend();

		while(itr!=itr_end)
		{
			if((*itr) > row_idx)
			{
				(*itr)=row_idx;
				results_tbw->verticalHeaderItem(row_idx)->setText(QString::number(row_idx + 1));
				row_idx--;
			}
			else break;

			itr++;
		}
	}
}

void DataManipulationForm::clearChangedRows(void)
{
	changed_rows.clear();
	prev_row_colors.clear();
	undo_tb->setEnabled(false);
	save_tb->setEnabled(false);
}

void DataManipulationForm::browseTable(const QString &fk_name, bool browse_ref_tab)
{
	QString value, schema, table;
	DataManipulationForm *data_manip = new DataManipulationForm;
	Connection conn = Connection(tmpl_conn_params);
	QStringList filter, src_cols, ref_cols;

	if(browse_ref_tab)
	{
		src_cols =  pk_col_names;
		ref_cols = ref_fk_infos[fk_name][ParsersAttributes::SRC_COLUMNS].split(Table::DATA_SEPARATOR);
		schema = ref_fk_infos[fk_name][ParsersAttributes::SCHEMA];
		table = ref_fk_infos[fk_name][ParsersAttributes::TABLE];
	}
	else
	{
		src_cols =  fk_infos[fk_name][ParsersAttributes::SRC_COLUMNS].split(Table::DATA_SEPARATOR);
		ref_cols = fk_infos[fk_name][ParsersAttributes::DST_COLUMNS].split(Table::DATA_SEPARATOR);
		schema = fk_infos[fk_name][ParsersAttributes::SCHEMA];
		table = fk_infos[fk_name][ParsersAttributes::REF_TABLE];
	}

	for(QString col_name : src_cols)
	{
		value = results_tbw->item(results_tbw->currentRow(), col_names.indexOf(col_name))->text();

		if(value.isEmpty())
			filter.push_back(QString("%1 IS NULL").arg(ref_cols.front()));
		else
			filter.push_back(QString("%1 = '%2'").arg(ref_cols.front()).arg(value));

		ref_cols.pop_front();
	}

	data_manip->setWindowModality(Qt::NonModal);
	data_manip->setAttribute(Qt::WA_DeleteOnClose, true);
	data_manip->setAttributes(conn, schema, table, filter.join(QString("AND")));

	PgModelerUiNS::resizeDialog(data_manip);
	data_manip->show();
}

void DataManipulationForm::browseReferrerTable(void)
{
	browseTable(qobject_cast<QAction *>(sender())->data().toString(), true);
}

void DataManipulationForm::browseReferencedTable(void)
{
	browseTable(qobject_cast<QAction *>(sender())->data().toString(), false);
}

void DataManipulationForm::undoOperations(void)
{
	QTableWidgetItem *item=nullptr;
	vector<int> rows, ins_rows;
	QList<QTableWidgetSelectionRange> sel_range=results_tbw->selectedRanges();

	if(!sel_range.isEmpty())
	{
		for(int row=sel_range[0].topRow(); row <= sel_range[0].bottomRow(); row++)
		{
			if(results_tbw->verticalHeaderItem(row)->data(Qt::UserRole).toUInt()==OP_INSERT)
				ins_rows.push_back(row);
			else
				rows.push_back(row);
		}
	}
	else
	{
		sel_range.clear();
		rows=changed_rows;
	}

	//Marking rows to be deleted/updated as no-op
	for(auto &row : rows)
	{
		item=results_tbw->verticalHeaderItem(row);
		if(item->data(Qt::UserRole).toUInt()!=OP_INSERT)
			markOperationOnRow(NO_OPERATION, row);
	}

	//If there is no selection, remove all new rows
	if(sel_range.isEmpty())
	{
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

		clearChangedRows();
	}
	else
		//Removing just the selected new rows
		removeNewRows(ins_rows);


	results_tbw->clearSelection();
	hint_frm->setVisible(results_tbw->rowCount() > 0);
}

void DataManipulationForm::insertRowOnTabPress(int curr_row, int curr_col, int prev_row, int prev_col)
{
	if(qApp->mouseButtons()==Qt::NoButton &&
			curr_row==0 && curr_col==0 &&
			prev_row==results_tbw->rowCount()-1 && prev_col==results_tbw->columnCount()-1)
		addRow();
}

void DataManipulationForm::saveChanges(void)
{
#ifdef DEMO_VERSION
#warning "DEMO VERSION: data manipulation save feature disabled warning."
	Messagebox msg_box;
	msg_box.show(trUtf8("Warning"),
				 trUtf8("You're running a demonstration version! The save feature of the data manipulation form is available only in the full version!"),
				 Messagebox::ALERT_ICON, Messagebox::OK_BUTTON);
#else
	int row=0;
	Connection conn=Connection(tmpl_conn_params);

	try
	{
		QString cmd;
		Messagebox msg_box;

		msg_box.show(trUtf8("<strong>WARNING:</strong> Once commited its not possible to undo the changes! Proceed with saving?"),
					 Messagebox::ALERT_ICON,
					 Messagebox::YES_NO_BUTTONS);

		if(msg_box.result()==QDialog::Accepted)
		{

			//Forcing the cell editor to be closed by selecting an unexistent cell and clearing the selection
			results_tbw->setCurrentCell(-1,-1, QItemSelectionModel::Clear);

			conn.connect();
			conn.executeDDLCommand(QString("START TRANSACTION"));

			for(unsigned idx=0; idx < changed_rows.size(); idx++)
			{
				row=changed_rows[idx];
				cmd=getDMLCommand(row);
				conn.executeDDLCommand(cmd);
			}

			conn.executeDDLCommand(QString("COMMIT"));
			conn.close();

			changed_rows.clear();
			retrieveData();
			undo_tb->setEnabled(false);
			save_tb->setEnabled(false);
		}
	}
	catch(Exception &e)
	{
		map<unsigned, QString> op_names={{ OP_DELETE, trUtf8("delete") },
										 { OP_UPDATE, trUtf8("update") },
										 { OP_INSERT, trUtf8("insert") }};

		QString tab_name=QString("%1.%2")
						 .arg(schema_cmb->currentText())
						 .arg(table_cmb->currentText());

		unsigned op_type=results_tbw->verticalHeaderItem(row)->data(Qt::UserRole).toUInt();

		if(conn.isStablished())
		{
			conn.executeDDLCommand(QString("ROLLBACK"));
			conn.close();
		}

		results_tbw->selectRow(row);
		results_tbw->scrollToItem(results_tbw->item(row, 0));

		throw Exception(Exception::getErrorMessage(ERR_ROW_DATA_NOT_MANIPULATED)
						.arg(op_names[op_type]).arg(tab_name).arg(row + 1).arg(e.getErrorMessage()),
						ERR_ROW_DATA_NOT_MANIPULATED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
#endif
}

QString DataManipulationForm::getDMLCommand(int row)
{
	if(row < 0 || row >= results_tbw->rowCount())
		return(QString());

	QString tab_name=QString("\"%1\".\"%2\"").arg(schema_cmb->currentText()).arg(table_cmb->currentText()),
			upd_cmd=QString("UPDATE %1 SET %2 WHERE %3"),
			del_cmd=QString("DELETE FROM %1 WHERE %2"),
			ins_cmd=QString("INSERT INTO %1(%2) VALUES (%3)"),
			fmt_cmd;
	QTableWidgetItem *item=nullptr;
	unsigned op_type=results_tbw->verticalHeaderItem(row)->data(Qt::UserRole).toUInt();
	QStringList val_list, col_list, flt_list;
	QString col_name, value;
	QVariant data;

	if(op_type==OP_DELETE || op_type==OP_UPDATE)
	{
		if(pk_col_names.isEmpty())
		{
			//Considering all columns as pk when the tables doesn't has one (except bytea columns)
			for(int col=0; col < results_tbw->columnCount(); col++)
			{
				if(results_tbw->horizontalHeaderItem(col)->data(Qt::UserRole)!=QString("bytea"))
					pk_col_names.push_back(results_tbw->horizontalHeaderItem(col)->text());
			}
		}

		//Creating the where clause with original column's values
		for(QString pk_col : pk_col_names)
		{
			data = results_tbw->item(row,  col_names.indexOf(pk_col))->data(Qt::UserRole);

			if(data.toString() == SQLExecutionWidget::COLUMN_NULL_VALUE)
				flt_list.push_back(QString("\"%1\" IS NULL").arg(pk_col));
			else
				flt_list.push_back(QString("\"%1\"='%2'").arg(pk_col).arg(data.toString().replace("\'","''")));
		}
	}

	if(op_type==OP_DELETE)
	{
		fmt_cmd=QString(del_cmd).arg(tab_name).arg(flt_list.join(QString(" AND ")));
	}
	else if(op_type==OP_UPDATE || op_type==OP_INSERT)
	{
		fmt_cmd=(op_type==OP_UPDATE ? upd_cmd : ins_cmd);

		for(int col=0; col < results_tbw->columnCount(); col++)
		{
			item=results_tbw->item(row, col);

			//bytea columns are ignored
			if(results_tbw->horizontalHeaderItem(col)->data(Qt::UserRole)!=QString("bytea"))
			{
				value=item->text();
				col_name=results_tbw->horizontalHeaderItem(col)->text();

				if(op_type==OP_INSERT || (op_type==OP_UPDATE && value!=item->data(Qt::UserRole)))
				{
					//Checking if the value is a malformed unescaped value, e.g., {value, value}, {value\}
					if((value.startsWith(PgModelerNS::UNESC_VALUE_START) && value.endsWith(QString("\\") + PgModelerNS::UNESC_VALUE_END)) ||
							(value.startsWith(PgModelerNS::UNESC_VALUE_START) && !value.endsWith(PgModelerNS::UNESC_VALUE_END)) ||
							(!value.startsWith(PgModelerNS::UNESC_VALUE_START) && !value.endsWith(QString("\\") + PgModelerNS::UNESC_VALUE_END) && value.endsWith(PgModelerNS::UNESC_VALUE_END)))
						throw Exception(Exception::getErrorMessage(ERR_MALFORMED_UNESCAPED_VALUE)
										.arg(row + 1).arg(col_name),
										ERR_MALFORMED_UNESCAPED_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					col_list.push_back(QString("\"%1\"").arg(col_name));

					//Empty values as considered as DEFAULT
					if(value.isEmpty())
					{
						value=QString("DEFAULT");
					}
					//Unescaped values will not be enclosed in quotes
					else if(value.startsWith(PgModelerNS::UNESC_VALUE_START) && value.endsWith(PgModelerNS::UNESC_VALUE_END))
					{
						value.remove(0,1);
						value.remove(value.length()-1, 1);
					}
					//Quoting value
					else
					{
						value.replace(QString("\\") + PgModelerNS::UNESC_VALUE_START, PgModelerNS::UNESC_VALUE_START);
						value.replace(QString("\\") + PgModelerNS::UNESC_VALUE_END, PgModelerNS::UNESC_VALUE_END);
						value.replace("\'","''");
						value=QString("E'") + value + QString("'");
					}

					if(op_type==OP_INSERT)
						val_list.push_back(value);
					else
						val_list.push_back(QString("\"%1\"=%2").arg(col_name).arg(value));
				}
			}
		}

		if(col_list.isEmpty())
			return(QString());
		else
		{
			if(op_type==OP_UPDATE)
				fmt_cmd=fmt_cmd.arg(tab_name).arg(val_list.join(QString(", "))).arg(flt_list.join(QString(" AND ")));
			else
				fmt_cmd=fmt_cmd.arg(tab_name).arg(col_list.join(QString(", "))).arg(val_list.join(QString(", ")));
		}
	}

	return(fmt_cmd);
}

void DataManipulationForm::resizeEvent(QResizeEvent *event)
{
	Qt::ToolButtonStyle style = Qt::ToolButtonIconOnly;
	QToolButton *btn = nullptr;

	if(event->size().width() > this->baseSize().width())
		style = Qt::ToolButtonTextBesideIcon;

	if(refresh_tb->toolButtonStyle() != style)
	{
		for(auto obj : bnts_parent_wgt->children())
		{
			btn = qobject_cast<QToolButton *>(obj);

			if(btn)
				btn->setToolButtonStyle(style);
		}
	}
}

void DataManipulationForm::closeEvent(QCloseEvent *)
{
  GeneralConfigWidget::saveWidgetGeometry(this);
}

void DataManipulationForm::setColumnsCheckState(Qt::CheckState state)
{
  QListWidgetItem *item = nullptr;

  results_tbw->blockSignals(true);

  for(int idx = 0; idx < columns_lst->count(); idx++)
  {
	item = columns_lst->item(idx);
	item->setCheckState(state);
	toggleColumnDisplay(item);
  }

  results_tbw->blockSignals(false);
}

void DataManipulationForm::truncateTable(void)
{
	try
	{
		QAction *act = dynamic_cast<QAction *>(sender());

		if(DatabaseExplorerWidget::truncateTable(schema_cmb->currentText(), table_cmb->currentText(),
																						 act->data().toBool(), Connection(tmpl_conn_params)))
			retrieveData();
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
  }
}

void DataManipulationForm::toggleColumnDisplay(QListWidgetItem *item)
{
  if(!item)
	return;

  if(item->checkState() != static_cast<Qt::CheckState>(item->data(Qt::UserRole).toInt()))
  {
	int idx = 0;
	bool hide = false;

	idx = col_names.indexOf(item->text());
	hide = item->checkState() == Qt::Unchecked;
	results_tbw->horizontalHeader()->setSectionHidden(idx, hide);
	item->setCheckState(hide ? Qt::Unchecked : Qt::Checked);
	item->setData(Qt::UserRole, item->checkState());
  }
}
