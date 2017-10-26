/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "plaintextitemdelegate.h"
#include "datamanipulationform.h"

map<QString, QString> SQLExecutionWidget::cmd_history;

int SQLExecutionWidget::cmd_history_max_len = 1000;
const QString SQLExecutionWidget::COLUMN_NULL_VALUE = QString("␀");

SQLExecutionWidget::SQLExecutionWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	sql_cmd_txt=PgModelerUiNS::createNumberedTextEditor(sql_cmd_wgt);
	cmd_history_txt=PgModelerUiNS::createNumberedTextEditor(cmd_history_parent);
	cmd_history_txt->setCustomContextMenuEnabled(false);

	cmd_history_txt->setTabStopWidth(sql_cmd_txt->getTabWidth());
	cmd_history_txt->setContextMenuPolicy(Qt::CustomContextMenu);
	cmd_history_txt->setReadOnly(true);
	cmd_history_txt->installEventFilter(this);

	output_tbw->widget(2)->installEventFilter(this);

	find_history_wgt = new FindReplaceWidget(cmd_history_txt, find_history_parent);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(find_history_wgt);
	find_history_parent->setLayout(layout);
	find_history_parent->setVisible(false);

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
	code_compl_wgt=new CodeCompletionWidget(sql_cmd_txt, true);

	find_replace_wgt=new FindReplaceWidget(sql_cmd_txt, find_wgt_parent);
	QHBoxLayout *hbox=new QHBoxLayout(find_wgt_parent);
	hbox->setContentsMargins(0,0,0,0);
	hbox->addWidget(find_replace_wgt);
	find_wgt_parent->setVisible(false);

	run_sql_tb->setToolTip(run_sql_tb->toolTip() + QString(" (%1)").arg(run_sql_tb->shortcut().toString()));
	export_tb->setToolTip(export_tb->toolTip() + QString(" (%1)").arg(export_tb->shortcut().toString()));
	file_tb->setToolTip(file_tb->toolTip() + QString(" (%1)").arg(file_tb->shortcut().toString()));
	output_tb->setToolTip(output_tb->toolTip() + QString(" (%1)").arg(output_tb->shortcut().toString()));
	find_tb->setToolTip(find_tb->toolTip() + QString(" (%1)").arg(find_tb->shortcut().toString()));

	results_tbw->setItemDelegate(new PlainTextItemDelegate(this, true));

	action_load=new QAction(QIcon(PgModelerUiNS::getIconPath("abrir")), trUtf8("Load"), this);
	action_save=new QAction(QIcon(PgModelerUiNS::getIconPath("salvar")), trUtf8("Save"), this);
	action_save_as=new QAction(QIcon(PgModelerUiNS::getIconPath("salvar_como")), trUtf8("Save as"), this);

	file_menu.addAction(action_load);
	file_menu.addAction(action_save);
	file_menu.addAction(action_save_as);
	file_tb->setMenu(&file_menu);

	connect(action_load, SIGNAL(triggered(bool)), this, SLOT(loadCommands()));
	connect(action_save, SIGNAL(triggered(bool)), this, SLOT(saveCommands()));
	connect(action_save_as, SIGNAL(triggered(bool)), this, SLOT(saveCommands()));

	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
	connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
	connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
	connect(find_tb, SIGNAL(toggled(bool)), find_wgt_parent, SLOT(setVisible(bool)));
	connect(output_tb, SIGNAL(toggled(bool)), this, SLOT(toggleOutputPane(bool)));

	//Signal handling with C++11 lambdas Slots
	connect(results_tbw, &QTableWidget::itemPressed,
			[&](){ SQLExecutionWidget::copySelection(results_tbw); });

	connect(export_tb, &QToolButton::clicked,
			[&](){ SQLExecutionWidget::exportResults(results_tbw); });

	connect(close_file_tb, &QToolButton::clicked,
	[&](){
			if(clearAll() == QDialog::Accepted)
			{
				filename_edt->clear();
				filename_wgt->setVisible(false);
			}
	});

	connect(&snippets_menu, SIGNAL(triggered(QAction*)), this, SLOT(selectSnippet(QAction *)));
	connect(cmd_history_txt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showHistoryContextMenu()));

	configureSnippets();
	toggleOutputPane(false);
	filename_wgt->setVisible(false);
	v_splitter->handle(1)->installEventFilter(this);
}

bool SQLExecutionWidget::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonDblClick && object == v_splitter->handle(1))
	{
		output_tb->setChecked(!v_splitter->handle(1)->isEnabled());
		return(true);
	}
	else if(event->type()== QEvent::MouseButtonPress &&
					dynamic_cast<QMouseEvent *>(event)->button()==Qt::MiddleButton &&
					object == cmd_history_txt &&
					cmd_history_txt->textCursor().hasSelection())
	{
		sql_cmd_txt->appendPlainText(cmd_history_txt->textCursor().selectedText());
		return(true);
	}
	else if(event->type() == QEvent::Show && object == output_tbw->widget(2))
	{
		if(cmd_history_txt->toPlainText().count(QChar('\n')) !=
			 cmd_history[sql_cmd_conn.getConnectionId(true,true)].count(QChar('\n')))
		{
			cmd_history_txt->clear();
			cmd_history_txt->appendPlainText(cmd_history[sql_cmd_conn.getConnectionId(true,true)]);
			cmd_history_txt->updateLineNumbers();
		}

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


	if(file_tb->toolButtonStyle()!=style)
	{
		file_tb->setToolButtonStyle(style);
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

						/* When storing column values in the QTableWidget items we need distinguish empty from null values
						 * Since it may affect the generation of SQL like delete when the field value is used somehow (see DataManipulationForm::getDMLCommand) */
						if(store_data)
							item->setData(Qt::UserRole, res.isColumnValueNull(col) ? COLUMN_NULL_VALUE : item->text());
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
																			QPixmap(PgModelerUiNS::getIconPath("msgbox_erro")), false);

	if(e.getErrorType()==ERR_CONNECTION_TIMEOUT ||
		 e.getErrorType()==ERR_CONNECTION_BROKEN)
	{
		PgModelerUiNS::createOutputListItem(msgoutput_lst,
																				QString("%1 %2").arg(time_str).arg(trUtf8("No results retrieved or changes done due to the error above.")),
																				QPixmap(PgModelerUiNS::getIconPath("msgbox_alerta")), false);
	}

	msgoutput_lst->setVisible(true);
	results_parent->setVisible(false);
	export_tb->setEnabled(false);

	output_tbw->setTabText(0, trUtf8("Results"));
	output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));
	output_tbw->setCurrentIndex(1);
	output_tbw->setTabEnabled(0, false);
}

void SQLExecutionWidget::addToSQLHistory(const QString &cmd, unsigned rows, const QString &error)
{
	if(!cmd.isEmpty())
	{
		QString fmt_cmd;

		if(!cmd_history_txt->toPlainText().isEmpty())
			fmt_cmd += QString("\n");

		fmt_cmd += QString("-- Executed at [%1] -- \n").arg(QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss.zzz")));
		fmt_cmd += cmd;
		fmt_cmd += QChar('\n');

		if(!error.isEmpty())
		{
			fmt_cmd += QString("-- Query failed --\n");
			fmt_cmd += QString("/*\n%1\n*/\n").arg(error);
		}
		else
			fmt_cmd += QString("-- Rows retrieved: %1\n").arg(rows);

		if(!fmt_cmd.trimmed().endsWith(ParsersAttributes::DDL_END_TOKEN))
			fmt_cmd += ParsersAttributes::DDL_END_TOKEN + QChar('\n');

		SQLExecutionWidget::validateSQLHistoryLength(sql_cmd_conn.getConnectionId(true,true), fmt_cmd, cmd_history_txt);
	}
}

void SQLExecutionWidget::validateSQLHistoryLength(const QString &conn_id, const QString &fmt_cmd, NumberedTextEditor *cmd_history_txt)
{
	QString cmds;
	int ln_count = 0;

	cmds = cmd_history[conn_id];
	ln_count = cmds.count(QChar('\n'));
	ln_count += fmt_cmd.count(QChar('\n'));

	if(ln_count > cmd_history_max_len)
	{
		QStringList buffer = cmds.split(QChar('\n'));
		cmds = buffer.mid(buffer.size()/2).join(QChar('\n'));
		cmds = cmds.mid(cmds.indexOf(ParsersAttributes::DDL_END_TOKEN) + ParsersAttributes::DDL_END_TOKEN.length());
		cmd_history[conn_id] = cmds.trimmed();

		if(cmd_history_txt)
		{
			cmd_history_txt->clear();
			cmd_history_txt->appendPlainText(cmds);
		}
	}

	cmd_history[conn_id].append(fmt_cmd);

	if(cmd_history_txt)
		cmd_history_txt->appendPlainText(fmt_cmd);
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

		addToSQLHistory(cmd, res.getTupleCount());

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
																					QPixmap(PgModelerUiNS::getIconPath("msgbox_alerta")));
		}

		PgModelerUiNS::createOutputListItem(msgoutput_lst,
																				PgModelerUiNS::formatMessage(trUtf8("[%1]: SQL command successfully executed. <em>%2 <strong>%3</strong></em>")
																																		 .arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")))
																																		 .arg(res.isEmpty() ? trUtf8("Rows affected") :  trUtf8("Rows retrieved"))
																																		 .arg(res.getTupleCount())),
																				QPixmap(PgModelerUiNS::getIconPath("msgbox_info")));

		output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));

		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		addToSQLHistory(cmd, 0, e.getErrorMessage());
		QApplication::restoreOverrideCursor();
		sql_cmd_conn.close();
		showError(e);
	}
}

void SQLExecutionWidget::saveCommands(void)
{
	bool browse_file = (sender() == action_save_as || filename_edt->text().isEmpty());
	QString filename = filename_edt->text();

	if(browse_file)
	{
		sql_file_dlg.setWindowTitle(trUtf8("Save SQL commands"));
		sql_file_dlg.setAcceptMode(QFileDialog::AcceptSave);
		sql_file_dlg.exec();

		if(sql_file_dlg.result()==QDialog::Accepted)
			filename = sql_file_dlg.selectedFiles().at(0);
	}
	else
		filename = filename_edt->text();

	if(!filename.isEmpty())
	{
		QFile file;
		file.setFileName(filename);

		if(!file.open(QFile::WriteOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(filename),
											ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		file.write(sql_cmd_txt->toPlainText().toUtf8());
		file.close();

		filename_edt->setText(filename);
		filename_wgt->setVisible(true);
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

		filename_edt->setText(sql_file_dlg.selectedFiles().at(0));
		filename_wgt->setVisible(true);
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
	QStringList line;

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
			line.append(QString("\"%1\"").arg(results_tbw->horizontalHeaderItem(col)->text()));

		buf.append(line.join(';'));
		buf.append('\n');
		line.clear();

		//Creating the content
		for(row=start_row; row < max_row; row++)
		{
			for(col=start_col; col < max_col; col++)
				line.append(QString("\"%1\"").arg(results_tbw->item(row, col)->text()));

			buf.append(line.join(';'));
			line.clear();
			buf.append('\n');
		}
	}

	return(buf);
}

QByteArray SQLExecutionWidget::generateTextBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QByteArray buf;
	QStringList line;

	//If the selection interval is valid
	if(start_row >=0 && start_col >=0 &&
			start_row + row_cnt <= results_tbw->rowCount() &&
			start_col + col_cnt <= results_tbw->columnCount())
	{
		int col=0, row=0,
				max_col=start_col + col_cnt,
				max_row=start_row + row_cnt;

		//Creating the content
		for(row=start_row; row < max_row; row++)
		{
			for(col=start_col; col < max_col; col++)
			{
				line.push_back(results_tbw->item(row, col)->text());
			}

			buf.append(line.join('\t'));
			line.clear();
			buf.append('\n');
		}
	}

	return(buf);
}

int SQLExecutionWidget::clearAll(void)
{
	Messagebox msg_box;
	int res = 0;

	msg_box.show(trUtf8("The SQL input field and the results grid will be cleared! Want to proceed?"),
							 Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	res = msg_box.result();

	if(res==QDialog::Accepted)
	{
		sql_cmd_txt->setPlainText(QString());
		msgoutput_lst->clear();
		msgoutput_lst->setVisible(true);
		results_parent->setVisible(false);
		export_tb->setEnabled(false);
	}

	return(res);
}

void SQLExecutionWidget::copySelection(QTableWidget *results_tbw, bool use_popup, bool csv_is_default)
{
	if(!results_tbw)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

	if(sel_ranges.count()==1 && (!use_popup || (use_popup && QApplication::mouseButtons()==Qt::RightButton)))
	{
		QMenu copy_menu, copy_mode_menu;
		QAction *act = nullptr, *act_csv = nullptr, *act_txt = nullptr;

		if(use_popup)
		{
			act = copy_menu.addAction(trUtf8("Copy selection"));
			act_txt = copy_mode_menu.addAction(trUtf8("Plain format"));
			act_csv = copy_mode_menu.addAction(trUtf8("CVS format"));
			act->setMenu(&copy_mode_menu);
			act = copy_menu.exec(QCursor::pos());
		}

		if(!use_popup || act)
		{
			QTableWidgetSelectionRange selection=sel_ranges.at(0);
			QByteArray buf;

			if((use_popup && act == act_csv) || (!use_popup && csv_is_default))
			{
				//Generates the csv buffer and assigns it to application's clipboard
				buf=generateCSVBuffer(results_tbw,
															selection.topRow(), selection.leftColumn(),
															selection.rowCount(), selection.columnCount());

				/* Making DataManipulationForm instances know that the clipboard has csv buffer
				 * in order to paste the contents properly */
				DataManipulationForm::setHasCsvClipboard(true);
			}
			else if((use_popup && act == act_txt) || (!use_popup && !csv_is_default))
			{
				buf=generateTextBuffer(results_tbw,
															 selection.topRow(), selection.leftColumn(),
															 selection.rowCount(), selection.columnCount());
			}

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

/*void SQLExecutionWidget::handleSelectedWord(QString word)
{
	if(SnippetsConfigWidget::isSnippetExists(word))
	{
		QTextCursor tc=sql_cmd_txt->textCursor();
		tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
		tc.removeSelectedText();
		tc.insertText(SnippetsConfigWidget::getParsedSnippet(word));
	}
}*/

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
}

void SQLExecutionWidget::saveSQLHistory(void)
{
	try
	{
		SchemaParser schparser;
		attribs_map attribs;
		QString commands;
		QByteArray buffer;
		QFile file;

		for(auto hist : cmd_history)
		{
			attribs[ParsersAttributes::CONNECTION] = hist.first;
			attribs[ParsersAttributes::COMMANDS] = hist.second;
			schparser.ignoreEmptyAttributes(true);
			commands += schparser.getCodeDefinition(GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
																							GlobalAttributes::DIR_SEPARATOR +
																							GlobalAttributes::SCHEMAS_DIR +
																							GlobalAttributes::DIR_SEPARATOR +
																							ParsersAttributes::COMMANDS +
																							GlobalAttributes::SCHEMA_EXT, attribs);
		}

		schparser.loadFile(GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
											 GlobalAttributes::DIR_SEPARATOR +
											 GlobalAttributes::SCHEMAS_DIR +
											 GlobalAttributes::DIR_SEPARATOR +
											 GlobalAttributes::SQL_HISTORY_CONF +
											 GlobalAttributes::SCHEMA_EXT);

		attribs.clear();
		attribs[ParsersAttributes::COMMANDS] = commands;
		buffer.append(schparser.getCodeDefinition(attribs));


		file.setFileName(GlobalAttributes::CONFIGURATIONS_DIR +
										 GlobalAttributes::DIR_SEPARATOR +
										 GlobalAttributes::SQL_HISTORY_CONF +
										 GlobalAttributes::CONFIGURATION_EXT);

		if(!file.open(QFile::WriteOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(file.fileName()),
											ERR_FILE_DIR_NOT_ACCESSED, __PRETTY_FUNCTION__, __FILE__ ,__LINE__);

		file.write(buffer);
		file.close();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLExecutionWidget::loadSQLHistory(void)
{
	try
	{
		XMLParser xmlparser;
		attribs_map attribs;

		xmlparser.setDTDFile(GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
												 GlobalAttributes::DIR_SEPARATOR +
												 GlobalAttributes::OBJECT_DTD_DIR +
												 GlobalAttributes::DIR_SEPARATOR +
												 GlobalAttributes::SQL_HISTORY_CONF +
												 GlobalAttributes::OBJECT_DTD_EXT,
												 GlobalAttributes::SQL_HISTORY_CONF);

		xmlparser.loadXMLFile(GlobalAttributes::CONFIGURATIONS_DIR +
													GlobalAttributes::DIR_SEPARATOR +
													GlobalAttributes::SQL_HISTORY_CONF +
													GlobalAttributes::CONFIGURATION_EXT);

		cmd_history.clear();

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementName() == ParsersAttributes::COMMANDS)
				{
					xmlparser.getElementAttributes(attribs);
					xmlparser.savePosition();

					if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
						cmd_history[attribs[ParsersAttributes::CONNECTION]].append(xmlparser.getElementContent());

					xmlparser.restorePosition();
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLExecutionWidget::destroySQLHistory(void)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("This action will wipe out all the SQL commands history for all connections! Do you really want to proceed?"),
								Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result() == QDialog::Accepted)
	{
		QFile::remove(GlobalAttributes::CONFIGURATIONS_DIR +
									GlobalAttributes::DIR_SEPARATOR +
									GlobalAttributes::SQL_HISTORY_CONF +
									GlobalAttributes::CONFIGURATION_EXT);

		SQLExecutionWidget::cmd_history.clear();
	}
}

void SQLExecutionWidget::setSQLHistoryMaxLength(int len)
{
	if(len < 100 || len > 2000)
		len = 1000;

	SQLExecutionWidget::cmd_history_max_len = len;
}

int SQLExecutionWidget::getSQLHistoryMaxLength(void)
{
	return(SQLExecutionWidget::cmd_history_max_len);
}

void SQLExecutionWidget::enableSQLExecution(bool enable)
{
	try
	{
		sql_cmd_txt->setEnabled(enable);
		snippets_tb->setEnabled(enable);
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
	QAction *action_clear = new QAction(QPixmap(PgModelerUiNS::getIconPath("limpartexto")), trUtf8("Clear history"), ctx_menu),
			*action_save = new QAction(QPixmap(PgModelerUiNS::getIconPath("salvar")), trUtf8("Save history"), ctx_menu),
			*action_reload = new QAction(QPixmap(PgModelerUiNS::getIconPath("atualizar")), trUtf8("Reload history"), ctx_menu),
			*action_toggle_find = nullptr,
			*exec_act = nullptr;

	if(!find_history_parent->isVisible())
		action_toggle_find = new QAction(QPixmap(PgModelerUiNS::getIconPath("buscar")), trUtf8("Find in history"), ctx_menu);
	else
		action_toggle_find = new QAction(trUtf8("Hide find tool"), ctx_menu);

	ctx_menu->addSeparator();
	ctx_menu->addAction(action_toggle_find);
	ctx_menu->addAction(action_reload);
	ctx_menu->addAction(action_save);
	ctx_menu->addSeparator();
	ctx_menu->addAction(action_clear);

	exec_act = ctx_menu->exec(QCursor::pos());

	if(exec_act == action_clear)
	{
		Messagebox msg_box;

		msg_box.show(trUtf8("This action will wipe out all the SQL commands history for the current connection! Do you really want to proceed?"),
									Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

		if(msg_box.result() == QDialog::Accepted)
		{
			cmd_history_txt->clear();
			cmd_history[sql_cmd_conn.getConnectionId(true,true)].clear();
		}
	}
	else if(exec_act == action_save)
		SQLExecutionWidget::saveSQLHistory();
	else if(exec_act == action_reload)
	{
		SQLExecutionWidget::loadSQLHistory();
		cmd_history_txt->clear();
		cmd_history_txt->appendPlainText(cmd_history[sql_cmd_conn.getConnectionId(true,true)]);
		cmd_history_hl->rehighlight();
	}
	else if(exec_act == action_toggle_find)
		find_history_parent->setVisible(!find_history_parent->isVisible());

	delete(ctx_menu);
}
