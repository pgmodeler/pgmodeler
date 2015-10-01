/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "sqlexecutionwidget.h"
#include "taskprogresswidget.h"
#include "databaseexplorerwidget.h"
#include "snippetsconfigwidget.h"
#include "pgmodeleruins.h"

SQLExecutionWidget::SQLExecutionWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

  sql_cmd_txt=PgModelerUiNS::createNumberedTextEditor(sql_cmd_wgt);

  sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, false);
  sql_cmd_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

	h_splitter1->setSizes({1000, 250});
	results_parent->setVisible(false);
	cmd_history_gb->setVisible(false);

  sql_file_dlg.setDefaultSuffix(QString("sql"));
	sql_file_dlg.setFileMode(QFileDialog::AnyFile);
  sql_file_dlg.setNameFilter(trUtf8("SQL file (*.sql);;All files (*.*)"));
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
  output_tb->setToolTip(output_tb->toolTip() + QString(" (%1)").arg(output_tb->shortcut().toString()));

  ro_item_del=new ReadOnlyItemDelegate(this);
  results_tbw->setItemDelegate(ro_item_del);

	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
	connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
	connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
	connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveCommands(void)));
	connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadCommands(void)));
	connect(history_tb, SIGNAL(toggled(bool)), cmd_history_gb, SLOT(setVisible(bool)));
	connect(clear_history_btn, SIGNAL(clicked(void)), cmd_history_lst, SLOT(clear(void)));
	connect(find_tb, SIGNAL(toggled(bool)), find_wgt_parent, SLOT(setVisible(bool)));
  connect(output_tb, SIGNAL(toggled(bool)), this, SLOT(toggleOutputPane(bool)));

	//Signal handling with C++11 lambdas Slots
	connect(clear_history_btn, &QPushButton::clicked,
					[=](){ clear_history_btn->setDisabled(true); });

	connect(cmd_history_lst, &QListWidget::itemDoubleClicked,
          [=](){ sql_cmd_txt->setPlainText(cmd_history_lst->currentItem()->data(Qt::UserRole).toString()); });

	connect(results_tbw, &QTableWidget::itemPressed,
          [=](){ SQLExecutionWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
          [=](){ SQLExecutionWidget::exportResults(results_tbw); });

  connect(&snippets_menu, SIGNAL(triggered(QAction*)), this, SLOT(selectSnippet(QAction *)));

  connect(code_compl_wgt, SIGNAL(s_wordSelected(QString)), this, SLOT(handleSelectedWord(QString)));

  configureSnippets();
  toggleOutputPane(false);
}

void SQLExecutionWidget::setConnection(Connection conn)
{
  sql_cmd_conn=conn;
  db_name_lbl->setText(QString("<strong>%1</strong>@<em>%2:%3</em>")
                       .arg(conn.getConnectionParam(Connection::PARAM_DB_NAME))
                       .arg(conn.getConnectionParam(Connection::PARAM_SERVER_IP).isEmpty() ?
                            conn.getConnectionParam(Connection::PARAM_SERVER_FQDN) : conn.getConnectionParam(Connection::PARAM_SERVER_IP))
                       .arg(conn.getConnectionParam(Connection::PARAM_PORT)));
}

void SQLExecutionWidget::enableCommandButtons(void)
{
	run_sql_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
  find_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
	clear_btn->setEnabled(run_sql_tb->isEnabled());
	save_tb->setEnabled(run_sql_tb->isEnabled());
}

void SQLExecutionWidget::fillResultsTable(ResultSet &res)
{
	try
	{
    Catalog catalog;
    Connection aux_conn=sql_cmd_conn;

    rows_ret_lbl->setText(QString("[<em>%1</em>] Rows returned:").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))));
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

void SQLExecutionWidget::showEvent(QShowEvent *)
{
  sql_cmd_txt->setFocus();
}

void SQLExecutionWidget::fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw, bool store_data)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		int col=0, row=0, col_cnt=res.getColumnCount();
		QTableWidgetItem *item=nullptr;
		vector<unsigned> type_ids;
    vector<unsigned>::iterator end;
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
    std::sort(type_ids.begin(), type_ids.end());
    end=std::unique(type_ids.begin(), type_ids.end());
    type_ids.erase(end, type_ids.end());

    types=catalog.getObjectsAttributes(OBJ_TYPE, QString(), QString(), type_ids);

    for(auto &tp : types)
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

void SQLExecutionWidget::showError(Exception &e)
{
  QListWidgetItem *item=new QListWidgetItem(QIcon(QString(":/icones/icones/msgbox_erro.png")), e.getErrorMessage());
	msgoutput_lst->clear();
	msgoutput_lst->addItem(item);
	msgoutput_lst->setVisible(true);
	results_parent->setVisible(false);
	export_tb->setEnabled(false);
}

void SQLExecutionWidget::registerSQLCommand(const QString &cmd)
{
	if(!cmd.isEmpty())
	{
		QListWidgetItem *item=new QListWidgetItem;
		item->setData(Qt::UserRole, QVariant(cmd));

		if(cmd.size() > 500)
      item->setText(cmd.mid(0, 500) + QString("..."));
		else
			item->setText(cmd);

		if(cmd_history_lst->count() > 100)
			cmd_history_lst->clear();

		cmd_history_lst->addItem(item);
		clear_history_btn->setEnabled(true);
	}
}

void SQLExecutionWidget::runSQLCommand(void)
{
	try
	{
		ResultSet res;
    QString cmd=sql_cmd_txt->textCursor().selectedText();

    output_tb->setChecked(true);

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
      QLabel *label=new QLabel(trUtf8("[<em>%1</em>] SQL command successfully executed. <em>Rows affected <strong>%2</strong></em>").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))).arg(res.getTupleCount()));
			QListWidgetItem *item=new QListWidgetItem;

      item->setIcon(QIcon(QString(":/icones/icones/msgbox_info.png")));
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

void SQLExecutionWidget::saveCommands(void)
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

void SQLExecutionWidget::loadCommands(void)
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

void SQLExecutionWidget::exportResults(QTableWidget *results_tbw)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QFileDialog csv_file_dlg;

  csv_file_dlg.setDefaultSuffix(QString("csv"));
	csv_file_dlg.setFileMode(QFileDialog::AnyFile);
	csv_file_dlg.setWindowTitle(trUtf8("Save CSV file"));
  csv_file_dlg.setNameFilter(trUtf8("Comma-separated values file (*.csv);;All files (*.*)"));
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

QByteArray SQLExecutionWidget::generateCSVBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt)
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

void SQLExecutionWidget::clearAll(void)
{
	Messagebox msg_box;

  msg_box.show(trUtf8("The SQL input field and the results grid will be cleared! Want to proceed?"),
							 Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
    sql_cmd_txt->setPlainText(QString());
		msgoutput_lst->clear();
		msgoutput_lst->setVisible(true);
		results_parent->setVisible(false);
		export_tb->setEnabled(false);
	}
}

void SQLExecutionWidget::copySelection(QTableWidget *results_tbw, bool use_popup)
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

void SQLExecutionWidget::selectSnippet(QAction *act)
{
  QTextCursor cursor=sql_cmd_txt->textCursor();
  cursor.movePosition(QTextCursor::End);

  sql_cmd_txt->appendPlainText(SnippetsConfigWidget::getParsedSnippet(act->text()));
  sql_cmd_txt->setTextCursor(cursor);
}

void SQLExecutionWidget::handleSelectedWord(QString word)
{
  if(SnippetsConfigWidget::isSnippetExists(word))
  {
    QTextCursor tc=sql_cmd_txt->textCursor();
    tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    tc.removeSelectedText();
    tc.insertText(SnippetsConfigWidget::getParsedSnippet(word));
  }
}

void SQLExecutionWidget::toggleOutputPane(bool visible)
{
  if(!visible)
  {
    v_splitter->handle(1)->setCursor(Qt::ArrowCursor);
    v_splitter->handle(1)->setEnabled(false);
  }
  else
    v_splitter->handle(1)->setCursor(Qt::SplitVCursor);

  v_splitter->handle(1)->setEnabled(visible);
  output_wgt->setVisible(visible);

  if(!visible)
    /* Force the splitter size to be the same as the sql_cmd_wgt maximum height
       in order to force the splitter to the bottom, hiding the output pane */
    v_splitter->setSizes({sql_cmd_wgt->maximumHeight(), 0});
  else
    //Restore the splitter to the default size
    v_splitter->setSizes({700, 300});
}

void SQLExecutionWidget::configureSnippets(void)
{ 
  SnippetsConfigWidget::configureSnippetsMenu(&snippets_menu);
  code_compl_wgt->configureCompletion(nullptr, sql_cmd_hl);
  code_compl_wgt->clearCustomItems();
  code_compl_wgt->insertCustomItems(SnippetsConfigWidget::getAllSnippetsAttribute(ParsersAttributes::ID),
                                    SnippetsConfigWidget::getAllSnippetsAttribute(ParsersAttributes::LABEL),
                                    QPixmap(QString(":/icones/icones/codesnippet.png")));
}

void SQLExecutionWidget::enableSQLExecution(bool enable)
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
