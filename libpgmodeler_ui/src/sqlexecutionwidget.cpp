/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

	cmd_history_txt->setTabStopWidth(sql_cmd_txt->getTabWidth());
	cmd_history_txt->setContextMenuPolicy(Qt::CustomContextMenu);
	cmd_history_txt->installEventFilter(this);

	sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, false);
	sql_cmd_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

	cmd_history_hl=new SyntaxHighlighter(cmd_history_txt, false);
	cmd_history_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

	results_parent->setVisible(false);
	output_tbw->setTabEnabled(0, false);

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
	load_tb->setToolTip(load_tb->toolTip() + QString(" (%1)").arg(load_tb->shortcut().toString()));
	save_tb->setToolTip(save_tb->toolTip() + QString(" (%1)").arg(save_tb->shortcut().toString()));
	output_tb->setToolTip(output_tb->toolTip() + QString(" (%1)").arg(output_tb->shortcut().toString()));
	find_tb->setToolTip(find_tb->toolTip() + QString(" (%1)").arg(find_tb->shortcut().toString()));

	ro_item_del=new ReadOnlyItemDelegate(this);
	results_tbw->setItemDelegate(ro_item_del);

	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
	connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
	connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
	connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveCommands(void)));
	connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadCommands(void)));
	connect(find_tb, SIGNAL(toggled(bool)), find_wgt_parent, SLOT(setVisible(bool)));
	connect(output_tb, SIGNAL(toggled(bool)), this, SLOT(toggleOutputPane(bool)));

	//Signal handling with C++11 lambdas Slots
	connect(results_tbw, &QTableWidget::itemPressed,
			[=](){ SQLExecutionWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
			[=](){ SQLExecutionWidget::exportResults(results_tbw); });

	connect(&snippets_menu, SIGNAL(triggered(QAction*)), this, SLOT(selectSnippet(QAction *)));

	connect(code_compl_wgt, SIGNAL(s_wordSelected(QString)), this, SLOT(handleSelectedWord(QString)));

	connect(cmd_history_txt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showHistoryContextMenu()));

	configureSnippets();
	toggleOutputPane(false);
	v_splitter->handle(1)->installEventFilter(this);
}

bool SQLExecutionWidget::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonDblClick &&
			qobject_cast<QSplitterHandle *>(object) == v_splitter->handle(1))
	{
		output_tb->setChecked(!v_splitter->handle(1)->isEnabled());
		return(true);
	}
	else if(event->type()== QEvent::MouseButtonPress &&
					dynamic_cast<QMouseEvent *>(event)->button()==Qt::MiddleButton &&
					qobject_cast<QPlainTextEdit *>(object)==cmd_history_txt &&
					cmd_history_txt->textCursor().hasSelection())
	{
		sql_cmd_txt->appendPlainText(cmd_history_txt->textCursor().selectedText());
		return(true);
	}

	return(QWidget::eventFilter(object, event));
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
		Connection aux_conn;

		aux_conn.setConnectionParams(sql_cmd_conn.getConnectionParams());
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

void SQLExecutionWidget::resizeEvent(QResizeEvent *event)
{
	Qt::ToolButtonStyle style=Qt::ToolButtonTextBesideIcon;

	if(event->size().width() < this->baseSize().width())
		style=Qt::ToolButtonIconOnly;


	if(load_tb->toolButtonStyle()!=style)
	{
		load_tb->setToolButtonStyle(style);
		save_tb->setToolButtonStyle(style);
		run_sql_tb->setToolButtonStyle(style);
		clear_btn->setToolButtonStyle(style);
		find_tb->setToolButtonStyle(style);
		snippets_tb->setToolButtonStyle(style);
		export_tb->setToolButtonStyle(style);
		output_tb->setToolButtonStyle(style);
	}
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
			item=new QTableWidgetItem(res.getColumnName(col));
			item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			results_tbw->setHorizontalHeaderItem(col, item);
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
			item->setToolTip(res.getColumnName(col) + QString(" [%1]").arg(type_names[res.getColumnTypeId(col)]));
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
	QString time_str=QString("[%1]:").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")));

	msgoutput_lst->clear();

	PgModelerUiNS::createOutputListItem(msgoutput_lst,
																			QString("%1 %2").arg(time_str).arg(e.getErrorMessage()),
																			QPixmap(QString(":/icones/icones/msgbox_erro.png")), false);

	if(e.getErrorType()==ERR_CONNECTION_TIMEOUT ||
		 e.getErrorType()==ERR_CONNECTION_BROKEN)
	{
		PgModelerUiNS::createOutputListItem(msgoutput_lst,
																				QString("%1 %2").arg(time_str).arg(trUtf8("No results retrieved or changes done due to the error above.")),
																				QPixmap(QString(":/icones/icones/msgbox_alerta.png")), false);
	}

	msgoutput_lst->setVisible(true);
	results_parent->setVisible(false);
	export_tb->setEnabled(false);

	output_tbw->setTabText(0, trUtf8("Results"));
	output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));
	output_tbw->setCurrentIndex(1);
	output_tbw->setTabEnabled(0, false);
}

void SQLExecutionWidget::registerSQLCommand(const QString &cmd, unsigned rows, const QString &error)
{
	if(!cmd.isEmpty())
	{
		QString fmt_cmd;
		fmt_cmd=QString("--\n-- Executed at [%1] -- \n").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")));
		fmt_cmd+=cmd;
		fmt_cmd+=QChar('\n');

		if(!error.isEmpty())
			fmt_cmd+=QString("/*\n%1\n*/\n").arg(error);
		else
			fmt_cmd+=QString("-- Rows retrieved: %1\n").arg(rows);

		fmt_cmd+=QString("-- End of execution --\n--\n");

		cmd_history_txt->appendPlainText(fmt_cmd);
	}
}

void SQLExecutionWidget::runSQLCommand(void)
{
	QString cmd=sql_cmd_txt->textCursor().selectedText();

	try
	{
		ResultSet res;
		QStringList conn_notices;

		output_tb->setChecked(true);

		if(cmd.isEmpty())
			cmd=sql_cmd_txt->toPlainText();
		else
			cmd.replace(QChar::ParagraphSeparator, '\n');

		msgoutput_lst->clear();

		if(!sql_cmd_conn.isStablished())
		{
			sql_cmd_conn.setNoticeEnabled(true);
			sql_cmd_conn.connect();

			//The connection will break the execution if it keeps idle for one hour or more
			sql_cmd_conn.setSQLExecutionTimout(3600);
		}

		QApplication::setOverrideCursor(Qt::WaitCursor);
		sql_cmd_conn.executeDMLCommand(cmd, res);
		conn_notices=sql_cmd_conn.getNotices();

		registerSQLCommand(cmd, res.getTupleCount());

		output_tbw->setTabEnabled(0, !res.isEmpty());
		results_parent->setVisible(!res.isEmpty());
		export_tb->setEnabled(!res.isEmpty());

		if(!res.isEmpty())
		{
			fillResultsTable(res);
			output_tbw->setTabText(0, trUtf8("Results (%1)").arg(res.getTupleCount()));
			output_tbw->setCurrentIndex(0);
		}
		else
		{
			output_tbw->setTabText(0, trUtf8("Results"));
			output_tbw->setCurrentIndex(1);
		}

		msgoutput_lst->clear();

		for(QString notice : conn_notices)
		{
			PgModelerUiNS::createOutputListItem(msgoutput_lst,
																					PgModelerUiNS::formatMessage(QString("[%1]: %2").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))).arg(notice)),
																					QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
		}

		PgModelerUiNS::createOutputListItem(msgoutput_lst,
																				PgModelerUiNS::formatMessage(trUtf8("[%1]: SQL command successfully executed. <em>%2 <strong>%3</strong></em>")
																																		 .arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")))
																																		 .arg(res.isEmpty() ? trUtf8("Rows affected") :  trUtf8("Rows retrieved"))
																																		 .arg(res.getTupleCount())),
																				QPixmap(QString(":/icones/icones/msgbox_info.png")));

		output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));

		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		registerSQLCommand(cmd, 0, e.getErrorMessage());
		QApplication::restoreOverrideCursor();
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

	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

	if(sel_ranges.count()==1 && (!use_popup || (use_popup && QApplication::mouseButtons()==Qt::RightButton)))
	{
		QMenu copy_menu;

		if(use_popup)
			copy_menu.addAction(trUtf8("Copy selection"));

		if(!use_popup || (use_popup && copy_menu.exec(QCursor::pos())))
		{
			QTableWidgetSelectionRange selection=sel_ranges.at(0);

			//Generates the csv buffer and assigns it to application's clipboard
			QByteArray buf=generateCSVBuffer(results_tbw,
																			 selection.topRow(), selection.leftColumn(),
																			 selection.rowCount(), selection.columnCount());
			qApp->clipboard()->setText(buf);
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
		snippets_tb->setEnabled(enable);
		save_tb->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		clear_btn->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		run_sql_tb->setEnabled(enable && !sql_cmd_txt->toPlainText().isEmpty());
		find_tb->setEnabled(enable);
		find_wgt_parent->setEnabled(enable);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLExecutionWidget::showHistoryContextMenu(void)
{
	QMenu *ctx_menu=cmd_history_txt->createStandardContextMenu();
	QAction *act=new QAction(trUtf8("Clear history"), ctx_menu);

	ctx_menu->addSeparator();
	ctx_menu->addAction(act);

	if(ctx_menu->exec(QCursor::pos())==act)
		cmd_history_txt->clear();

	delete(ctx_menu);
}
