/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "sqltoolwidget.h"
#include "taskprogresswidget.h"
#include "databaseexplorerwidget.h"
#include "snippetsconfigwidget.h"

SQLToolWidget::SQLToolWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

  sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, true, false);
  sql_cmd_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

  h_splitter->setSizes({0, 10000});
	h_splitter1->setSizes({1000, 250});
	results_parent->setVisible(false);
	cmd_history_gb->setVisible(false);

	sql_file_dlg.setDefaultSuffix("sql");
	sql_file_dlg.setFileMode(QFileDialog::AnyFile);
	sql_file_dlg.setNameFilter(tr("SQL file (*.sql);;All files (*.*)"));
	sql_file_dlg.setModal(true);

  snippets_tb->setMenu(&snippets_menu);

  code_compl_wgt=new CodeCompletionWidget(sql_cmd_txt);

	find_replace_wgt=new FindReplaceWidget(sql_cmd_txt, find_wgt_parent);
	QHBoxLayout *hbox=new QHBoxLayout(find_wgt_parent);
	hbox->setContentsMargins(0,0,0,0);
	hbox->addWidget(find_replace_wgt);
	find_wgt_parent->setVisible(false);

  run_sql_tb->setToolTip(run_sql_tb->toolTip() + QString(" (%1)").arg(run_sql_tb->shortcut().toString()));
	export_tb->setToolTip(export_tb->toolTip() + QString(" (%1)").arg(export_tb->shortcut().toString()));
	history_tb->setToolTip(history_tb->toolTip() + QString(" (%1)").arg(history_tb->shortcut().toString()));
	load_tb->setToolTip(load_tb->toolTip() + QString(" (%1)").arg(load_tb->shortcut().toString()));
	save_tb->setToolTip(save_tb->toolTip() + QString(" (%1)").arg(save_tb->shortcut().toString()));
	data_grid_tb->setToolTip(data_grid_tb->toolTip() + QString(" (%1)").arg(data_grid_tb->shortcut().toString()));

	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
  connect(connect_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(disconnect_tb, SIGNAL(clicked(void)), this, SLOT(disconnectFromServer(void)));
  connect(connections_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disconnectFromServer()));
  connect(browse_tb, SIGNAL(clicked(void)), this, SLOT(browseDatabase(void)));
	connect(drop_db_tb, SIGNAL(clicked(void)), this, SLOT(dropDatabase(void)));
	connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
	connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
	connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveCommands(void)));
	connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadCommands(void)));
	connect(history_tb, SIGNAL(toggled(bool)), cmd_history_gb, SLOT(setVisible(bool)));
	connect(clear_history_btn, SIGNAL(clicked(void)), cmd_history_lst, SLOT(clear(void)));
	connect(data_grid_tb, SIGNAL(clicked()), this, SLOT(openDataGrid()));
	connect(find_tb, SIGNAL(toggled(bool)), find_wgt_parent, SLOT(setVisible(bool)));

  connect(databases_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseExplorer(int)));
  connect(databases_tbw, SIGNAL(currentChanged(int)), this, SLOT(setCurrentDatabase(int)));

	//Signal handling with C++11 lambdas Slots
	connect(clear_history_btn, &QPushButton::clicked,
					[=](){ clear_history_btn->setDisabled(true); });

	connect(cmd_history_lst, &QListWidget::itemDoubleClicked,
					[=](){ sql_cmd_txt->setText(cmd_history_lst->currentItem()->data(Qt::UserRole).toString()); });

	connect(database_cmb, &QComboBox::currentTextChanged,
          [=](){ 	browse_tb->setEnabled(database_cmb->currentIndex() > 0);
									drop_db_tb->setEnabled(database_cmb->currentIndex() > 0);
                  data_grid_tb->setEnabled(database_cmb->currentIndex() > 0); });

	connect(results_tbw, &QTableWidget::itemPressed,
					[=](){ SQLToolWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
          [=](){ SQLToolWidget::exportResults(results_tbw); });

  connect(&snippets_menu, SIGNAL(triggered(QAction*)), this, SLOT(selectSnippet(QAction *)));

  connect(code_compl_wgt, SIGNAL(s_wordSelected(QString)), this, SLOT(handleSelectedWord(QString)));
}

SQLToolWidget::~SQLToolWidget(void)
{
  while(databases_tbw->count() > 0)
    closeDatabaseExplorer(0);
}

void SQLToolWidget::updateConnections(map<QString, Connection *> &conns)
{
	map<QString, Connection *>::iterator itr=conns.begin();
	connections_cmb->clear();

	//Add the connections to the combo
	while(itr!=conns.end())
	{
		connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
		itr++;
	}

	connect_tb->setEnabled(connections_cmb->count() > 0);
	enableSQLExecution(false);
}

void SQLToolWidget::connectToServer(void)
{
	try
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

		import_helper.setConnection(*conn);
    DatabaseImportForm::listDatabases(import_helper, database_cmb);
		database_cmb->setEnabled(database_cmb->count() > 1);
		import_helper.closeConnection();

		connections_cmb->setEnabled(false);
		connect_tb->setEnabled(false);
		disconnect_tb->setEnabled(true);
    refresh_tb->setEnabled(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::disconnectFromServer(void)
{
	try
	{
		database_cmb->clear();
		connections_cmb->setEnabled(true);
		connect_tb->setEnabled(true);
		disconnect_tb->setEnabled(false);
    refresh_tb->setEnabled(false);
		enableSQLExecution(false);

    while(databases_tbw->count() > 0)
    {
      databases_tbw->blockSignals(true);
      closeDatabaseExplorer(0);
      databases_tbw->blockSignals(false);
    }
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::enableCommandButtons(void)
{
	run_sql_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
	clear_btn->setEnabled(run_sql_tb->isEnabled());
	save_tb->setEnabled(run_sql_tb->isEnabled());
}

void SQLToolWidget::fillResultsTable(ResultSet &res)
{
	try
	{
		Catalog catalog;
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()),
							 aux_conn=(*conn);

		row_cnt_lbl->setText(QString::number(res.getTupleCount()));
		export_tb->setEnabled(res.getTupleCount() > 0);

		catalog.setConnection(aux_conn);
		fillResultsTable(catalog, res, results_tbw);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw, bool store_data)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		int col=0, row=0, col_cnt=res.getColumnCount();
		QTableWidgetItem *item=nullptr;
		vector<unsigned> type_ids;
		vector<attribs_map> types;
		map<unsigned, QString> type_names;
		unsigned orig_filter=catalog.getFilter();

		results_tbw->setRowCount(0);
		results_tbw->setColumnCount(col_cnt);
		results_tbw->verticalHeader()->setVisible(true);
		results_tbw->blockSignals(true);

		//Configuring the grid columns with the names of retrived table columns
		for(col=0; col < col_cnt; col++)
		{
			type_ids.push_back(res.getColumnTypeId(col));
			results_tbw->setHorizontalHeaderItem(col, new QTableWidgetItem(res.getColumnName(col)));
		}

		//Retrieving the data type names for each column
		catalog.setFilter(Catalog::LIST_ALL_OBJS);
		std::unique(type_ids.begin(), type_ids.end());
		types=catalog.getObjectsAttributes(OBJ_TYPE, "", "", type_ids);

		for(auto tp : types)
			type_names[tp[ParsersAttributes::OID].toUInt()]=tp[ParsersAttributes::NAME];

		catalog.setFilter(orig_filter);

		//Assinging the type names as tooltip on header items
		for(col=0; col < col_cnt; col++)
		{
			item=results_tbw->horizontalHeaderItem(col);
			item->setToolTip(type_names[res.getColumnTypeId(col)]);
			item->setData(Qt::UserRole, type_names[res.getColumnTypeId(col)]);
		}

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			results_tbw->setRowCount(res.getTupleCount());

			do
			{
				//Fills the current row with the values of current tuple
				for(col=0; col < col_cnt; col++)
				{
					item=new QTableWidgetItem;

					if(res.isColumnBinaryFormat(col))
					{
						//Binary columns can't be edited by user
						item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
						item->setText(trUtf8("[binary data]"));
					}
					else
					{
						item->setText(res.getColumnValue(col));

						if(store_data)
							item->setData(Qt::UserRole, item->text());
					}

					results_tbw->setItem(row, col, item);
				}

				//Configure the vertical header to show the current tuple id
				results_tbw->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
				row++;
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		results_tbw->blockSignals(false);
		results_tbw->resizeColumnsToContents();
		results_tbw->resizeRowsToContents();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::showError(Exception &e)
{
	QListWidgetItem *item=new QListWidgetItem(QIcon(":/icones/icones/msgbox_erro.png"), e.getErrorMessage());
	msgoutput_lst->clear();
	msgoutput_lst->addItem(item);
	msgoutput_lst->setVisible(true);
	results_parent->setVisible(false);
	export_tb->setEnabled(false);
}

void SQLToolWidget::registerSQLCommand(const QString &cmd)
{
	if(!cmd.isEmpty())
	{
		QListWidgetItem *item=new QListWidgetItem;
		item->setData(Qt::UserRole, QVariant(cmd));

		if(cmd.size() > 500)
			item->setText(cmd.mid(0, 500) + "...");
		else
			item->setText(cmd);

		if(cmd_history_lst->count() > 100)
			cmd_history_lst->clear();

		cmd_history_lst->addItem(item);
		clear_history_btn->setEnabled(true);
	}
}

void SQLToolWidget::runSQLCommand(void)
{
	try
	{
		ResultSet res;
    QString cmd=sql_cmd_txt->textCursor().selectedText();

		if(cmd.isEmpty())
			cmd=sql_cmd_txt->toPlainText();
    else
      cmd.replace(QChar::ParagraphSeparator, '\n');

		msgoutput_lst->clear();

    sql_cmd_conn.connect();
		sql_cmd_conn.executeDMLCommand(cmd, res);
    sql_cmd_conn.close();

		registerSQLCommand(cmd);

		results_parent->setVisible(!res.isEmpty());
		export_tb->setEnabled(!res.isEmpty());
		msgoutput_lst->setVisible(res.isEmpty());

		if(results_tbw->isVisible())
			fillResultsTable(res);
		else
		{
			QLabel *label=new QLabel(trUtf8("[<strong>%1</strong>] SQL command successfully executed. <em>Rows affected <strong>%2</strong></em>").arg(QTime::currentTime().toString()).arg(res.getTupleCount()));
			QListWidgetItem *item=new QListWidgetItem;

			item->setIcon(QIcon(":/icones/icones/msgbox_info.png"));
			msgoutput_lst->clear();
			msgoutput_lst->addItem(item);
			msgoutput_lst->setItemWidget(item, label);
		}
	}
	catch(Exception &e)
	{
    sql_cmd_conn.close();
		showError(e);
	}
}

void SQLToolWidget::saveCommands(void)
{
	sql_file_dlg.setWindowTitle(trUtf8("Save SQL commands"));
	sql_file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	sql_file_dlg.exec();

	if(sql_file_dlg.result()==QDialog::Accepted)
	{
		QFile file;
		file.setFileName(sql_file_dlg.selectedFiles().at(0));

		if(!file.open(QFile::WriteOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
											.arg(sql_file_dlg.selectedFiles().at(0))
											, ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		file.write(sql_cmd_txt->toPlainText().toUtf8());
		file.close();
	}
}

void SQLToolWidget::loadCommands(void)
{
	sql_file_dlg.setWindowTitle(trUtf8("Load SQL commands"));
	sql_file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	sql_file_dlg.exec();

	if(sql_file_dlg.result()==QDialog::Accepted)
	{
		QFile file;
		file.setFileName(sql_file_dlg.selectedFiles().at(0));

		if(!file.open(QFile::ReadOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
											.arg(sql_file_dlg.selectedFiles().at(0))
											,ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		sql_cmd_txt->clear();
		sql_cmd_txt->setPlainText(file.readAll());
		file.close();
	}
}

void SQLToolWidget::exportResults(QTableWidget *results_tbw)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QFileDialog csv_file_dlg;

	csv_file_dlg.setDefaultSuffix("csv");
	csv_file_dlg.setFileMode(QFileDialog::AnyFile);
	csv_file_dlg.setWindowTitle(trUtf8("Save CSV file"));
	csv_file_dlg.setNameFilter(tr("Comma-separated values file (*.csv);;All files (*.*)"));
	csv_file_dlg.setModal(true);
	csv_file_dlg.setAcceptMode(QFileDialog::AcceptSave);

	csv_file_dlg.exec();

	if(csv_file_dlg.result()==QDialog::Accepted)
	{
		QFile file;
		file.setFileName(csv_file_dlg.selectedFiles().at(0));

		if(!file.open(QFile::WriteOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
											.arg(csv_file_dlg.selectedFiles().at(0))
											, ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		file.write(generateCSVBuffer(results_tbw, 0, 0, results_tbw->rowCount(), results_tbw->columnCount()));
		file.close();
	}
}

QByteArray SQLToolWidget::generateCSVBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QByteArray buf;

	//If the selection interval is valid
	if(start_row >=0 && start_col >=0 &&
		 start_row + row_cnt <= results_tbw->rowCount() &&
		 start_col + col_cnt <= results_tbw->columnCount())
	{
		int col=0, row=0,
				max_col=start_col + col_cnt,
				max_row=start_row + row_cnt;

		//Creating the header of csv
		for(col=start_col; col < max_col; col++)
		{
			buf.append(QString("\"%1\"").arg(results_tbw->horizontalHeaderItem(col)->text()));
			buf.append(';');
		}

		buf.append('\n');

		//Creating the content
		for(row=start_row; row < max_row; row++)
		{
			for(col=start_col; col < max_col; col++)
			{
				buf.append(QString("\"%1\"").arg(results_tbw->item(row, col)->text()));
				buf.append(';');
			}

			buf.append('\n');
		}
	}

	return(buf);
}

void SQLToolWidget::clearAll(void)
{
	Messagebox msg_box;

  msg_box.show(trUtf8("The SQL input field and the results grid will be cleared! Want to proceed?"),
							 Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		sql_cmd_txt->setText("");
		msgoutput_lst->clear();
		msgoutput_lst->setVisible(true);
		results_parent->setVisible(false);
		export_tb->setEnabled(false);
	}
}

void SQLToolWidget::copySelection(QTableWidget *results_tbw, bool use_popup)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!use_popup || (use_popup && QApplication::mouseButtons()==Qt::RightButton))
	{
		QMenu copy_menu;

		if(use_popup)
			copy_menu.addAction(trUtf8("Copy selection"));

		if(!use_popup || (use_popup && copy_menu.exec(QCursor::pos())))
		{
			QList<QTableWidgetSelectionRange> sel_range=results_tbw->selectedRanges();

			if(!sel_range.isEmpty())
			{
				QTableWidgetSelectionRange selection=sel_range.at(0);

				//Generates the csv buffer and assigns it to application's clipboard
				QByteArray buf=generateCSVBuffer(results_tbw,
																				 selection.topRow(), selection.leftColumn(),
																				 selection.rowCount(), selection.columnCount());
				qApp->clipboard()->setText(buf);
			}
		}
	}
}

void SQLToolWidget::dropDatabase(void)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("Warning"),
               trUtf8("<strong>CAUTION:</strong> You are about to drop the entire database <strong>%1</strong>! All data will be completely wiped out. Do you really want to proceed?").arg(database_cmb->currentText()),
							 Messagebox::ALERT_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		Connection aux_conn=(*conn);

		try
		{
			enableSQLExecution(false);

      //Closing tabs related to the database to be dropped
      for(int i=0; i < databases_tbw->count(); i++)
      {
        if(databases_tbw->tabText(i)==database_cmb->currentText())
        {
          closeDatabaseExplorer(i);
          i=-1;
        }
      }

      sql_cmd_conn.close();
			aux_conn.connect();
			aux_conn.executeDDLCommand(QString("DROP DATABASE \"%1\";").arg(database_cmb->currentText()));
			aux_conn.close();
      connectToServer();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void SQLToolWidget::openDataGrid(const QString &schema, const QString &table, bool hide_views)
{
	#ifdef DEMO_VERSION
		#warning "DEMO VERSION: data manipulation feature disabled warning."
		Messagebox msg_box;
		msg_box.show(trUtf8("Warning"),
								trUtf8("You're running a demonstration version! The data manipulation feature is available only in the full version!"),
								Messagebox::ALERT_ICON, Messagebox::OK_BUTTON);
	#else
      DataManipulationForm *data_manip=new DataManipulationForm;
      Connection conn=*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

      data_manip->setWindowModality(Qt::NonModal);
      data_manip->setAttribute(Qt::WA_DeleteOnClose, true);
      data_manip->hide_views_chk->setChecked(hide_views);

      conn.setConnectionParam(Connection::PARAM_DB_NAME, database_cmb->currentText());
      data_manip->setAttributes(conn, schema, table);
      data_manip->show();
#endif
}

void SQLToolWidget::browseDatabase(void)
{
  try
  {
    Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
    DatabaseExplorerWidget *db_explorer_wgt=new DatabaseExplorerWidget;

    conn.setConnectionParam(Connection::PARAM_DB_NAME, database_cmb->currentText());
    db_explorer_wgt->setConnection(conn);
    db_explorer_wgt->listObjects();

    databases_tbw->addTab(db_explorer_wgt, database_cmb->currentText());
    databases_tbw->setCurrentWidget(db_explorer_wgt);

    connect(db_explorer_wgt, SIGNAL(s_dataGridOpenRequested(QString,QString,bool)), this, SLOT(openDataGrid(QString,QString,bool)));
    connect(db_explorer_wgt, SIGNAL(s_snippetShowRequested(QString)), sql_cmd_txt, SLOT(setPlainText(QString)));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void SQLToolWidget::closeDatabaseExplorer(int idx)
{
  DatabaseExplorerWidget *db_explorer=dynamic_cast<DatabaseExplorerWidget *>(databases_tbw->widget(idx));
  databases_tbw->removeTab(idx);

  if(db_explorer)
    delete(db_explorer);

  enableSQLExecution(databases_tbw->count()!=0);
}

void SQLToolWidget::setCurrentDatabase(int idx)
{
  try
  {
    DatabaseExplorerWidget *db_explorer=dynamic_cast<DatabaseExplorerWidget *>(databases_tbw->widget(idx));
    sql_cmd_conn.close();

    if(db_explorer)
    {
      sql_cmd_conn=db_explorer->getConnection();
      enableSQLExecution(true);
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void SQLToolWidget::selectSnippet(QAction *act)
{
  sql_cmd_txt->setPlainText(SnippetsConfigWidget::getParsedSnippet(act->text()));
}

void SQLToolWidget::handleSelectedWord(QString word)
{
  if(SnippetsConfigWidget::isSnippetExists(word))
  {
    QTextCursor tc=sql_cmd_txt->textCursor();
    tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    tc.removeSelectedText();
    tc.insertText(SnippetsConfigWidget::getParsedSnippet(word));
  }
}

void SQLToolWidget::configureSnippets(void)
{ 
  SnippetsConfigWidget::configureSnippetsMenu(&snippets_menu);
  code_compl_wgt->configureCompletion(nullptr, sql_cmd_hl);
  code_compl_wgt->clearCustomItems();
  code_compl_wgt->insertCustomItems(SnippetsConfigWidget::getAllSnippetsAttribute(ParsersAttributes::ID),
                                    SnippetsConfigWidget::getAllSnippetsAttribute(ParsersAttributes::LABEL),
                                    QPixmap(":/icones/icones/codesnippet.png"));
}

void SQLToolWidget::enableSQLExecution(bool enable)
{
	try
	{
		sql_cmd_txt->setEnabled(enable);
		load_tb->setEnabled(enable);
		history_tb->setEnabled(enable);
    snippets_tb->setEnabled(enable);
		save_tb->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		clear_btn->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		run_sql_tb->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		find_tb->setEnabled(enable);
		find_wgt_parent->setEnabled(enable);

		if(history_tb->isChecked() && !enable)
			history_tb->setChecked(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
