/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
const QString SQLExecutionWidget::ColumnNullValue = QString("␀");

SQLExecutionWidget::SQLExecutionWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	sql_cmd_txt=PgModelerUiNs::createNumberedTextEditor(sql_cmd_wgt);
	cmd_history_txt=PgModelerUiNs::createNumberedTextEditor(cmd_history_parent);
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
	connect(find_history_wgt->hide_tb, SIGNAL(clicked(bool)), find_history_parent, SLOT(hide()));

	sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, false);
	sql_cmd_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

	cmd_history_hl=new SyntaxHighlighter(cmd_history_txt, false);
	cmd_history_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

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
	connect(find_replace_wgt->hide_tb, SIGNAL(clicked(bool)), find_tb, SLOT(toggle()));

	run_sql_tb->setToolTip(run_sql_tb->toolTip() + QString(" (%1)").arg(run_sql_tb->shortcut().toString()));
	stop_tb->setToolTip(stop_tb->toolTip() + QString(" (%1)").arg(stop_tb->shortcut().toString()));
	export_tb->setToolTip(export_tb->toolTip() + QString(" (%1)").arg(export_tb->shortcut().toString()));
	file_tb->setToolTip(file_tb->toolTip() + QString(" (%1)").arg(file_tb->shortcut().toString()));
	output_tb->setToolTip(output_tb->toolTip() + QString(" (%1)").arg(output_tb->shortcut().toString()));
	find_tb->setToolTip(find_tb->toolTip() + QString(" (%1)").arg(find_tb->shortcut().toString()));
	filter_tb->setToolTip(filter_tb->toolTip() + QString(" (%1)").arg(filter_tb->shortcut().toString()));

	results_tbw->setItemDelegate(new PlainTextItemDelegate(this, true));

	action_load=new QAction(QIcon(PgModelerUiNs::getIconPath("abrir")), trUtf8("Load"), this);
	action_save=new QAction(QIcon(PgModelerUiNs::getIconPath("salvar")), trUtf8("Save"), this);
	action_save_as=new QAction(QIcon(PgModelerUiNs::getIconPath("salvar_como")), trUtf8("Save as"), this);

	file_menu.addAction(action_load);
	file_menu.addAction(action_save);
	file_menu.addAction(action_save_as);
	file_tb->setMenu(&file_menu);

	filter_wgt->setVisible(false);

	connect(columns_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(filterResults()));
	connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterResults()));
	connect(hide_tb, SIGNAL(clicked(bool)), filter_tb, SLOT(click()));
	connect(filter_tb, SIGNAL(toggled(bool)), filter_wgt, SLOT(setVisible(bool)));

	connect(exact_chk, SIGNAL(toggled(bool)), this, SLOT(filterResults()));
	connect(exact_chk, &QCheckBox::toggled, [&](bool checked){
		regexp_chk->setChecked(false);
		regexp_chk->setEnabled(!checked);
		case_sensitive_chk->setChecked(false);
		case_sensitive_chk->setEnabled(!checked);
	});

	connect(regexp_chk, SIGNAL(toggled(bool)), this, SLOT(filterResults()));
	connect(case_sensitive_chk, SIGNAL(toggled(bool)), this, SLOT(filterResults()));

	connect(action_load, SIGNAL(triggered(bool)), this, SLOT(loadCommands()));
	connect(action_save, SIGNAL(triggered(bool)), this, SLOT(saveCommands()));
	connect(action_save_as, SIGNAL(triggered(bool)), this, SLOT(saveCommands()));

	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
	connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
	connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
	connect(find_tb, SIGNAL(toggled(bool)), find_wgt_parent, SLOT(setVisible(bool)));
	connect(output_tb, SIGNAL(toggled(bool)), this, SLOT(toggleOutputPane(bool)));

	//Signal handling with C++11 lambdas Slots
	connect(results_tbw, &QTableView::pressed,
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

	stop_tb->setVisible(false);
	sql_exec_hlp.moveToThread(&sql_exec_thread);

	connect(&sql_exec_thread, SIGNAL(started()), &sql_exec_hlp, SLOT(executeCommand()));
	connect(&sql_exec_hlp, SIGNAL(s_executionFinished(int)), this, SLOT(finishExecution(int)));
	connect(&sql_exec_hlp, SIGNAL(s_executionAborted(Exception)), &sql_exec_thread, SLOT(quit()));
	connect(&sql_exec_hlp, SIGNAL(s_executionAborted(Exception)), this, SLOT(handleExecutionAborted(Exception)));
	connect(stop_tb, SIGNAL(clicked(bool)), &sql_exec_hlp, SLOT(cancelCommand()), Qt::DirectConnection);
}

SQLExecutionWidget::~SQLExecutionWidget(void)
{
	if(sql_exec_thread.isRunning())
	{
		sql_exec_hlp.cancelCommand();
		sql_exec_thread.quit();
		sql_exec_thread.wait();
	}

	destroyResultModel();
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
	sql_exec_hlp.setConnection(conn);
	sql_cmd_conn = conn;

	db_name_lbl->setText(QString("<strong>%1</strong>@<em>%2:%3</em>")
						 .arg(conn.getConnectionParam(Connection::ParamDbName))
						 .arg(conn.getConnectionParam(Connection::ParamServerIp).isEmpty() ?
								  conn.getConnectionParam(Connection::ParamServerFqdn) : conn.getConnectionParam(Connection::ParamServerIp))
						 .arg(conn.getConnectionParam(Connection::ParamPort)));
}

void SQLExecutionWidget::enableCommandButtons(void)
{
	run_sql_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
	find_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
	clear_btn->setEnabled(run_sql_tb->isEnabled());
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
		stop_tb->setToolButtonStyle(style);
		filter_tb->setToolButtonStyle(style);
	}
}

void SQLExecutionWidget::fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw, bool store_data)
{
	if(!results_tbw)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		results_tbw->setSortingEnabled(false);
		results_tbw->blockSignals(true);
		results_tbw->setUpdatesEnabled(false);

		//Configuring the grid columns with the names of retrived table columns
		for(col=0; col < col_cnt; col++)
		{
			type_ids.push_back(res.getColumnTypeId(col));
			item=new QTableWidgetItem(res.getColumnName(col));
			item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			results_tbw->setHorizontalHeaderItem(col, item);
		}

		//Retrieving the data type names for each column
		catalog.setFilter(Catalog::ListAllObjects);
		std::sort(type_ids.begin(), type_ids.end());
		end=std::unique(type_ids.begin(), type_ids.end());
		type_ids.erase(end, type_ids.end());

		types=catalog.getObjectsAttributes(ObjectType::Type, QString(), QString(), type_ids);

		for(auto &tp : types)
			type_names[tp[Attributes::Oid].toUInt()]=tp[Attributes::Name];

		catalog.setFilter(orig_filter);

		//Assinging the type names as tooltip on header items
		for(col=0; col < col_cnt; col++)
		{
			item=results_tbw->horizontalHeaderItem(col);
			item->setToolTip(type_names[res.getColumnTypeId(col)]);
			item->setData(Qt::UserRole, type_names[res.getColumnTypeId(col)]);
		}

		if(res.accessTuple(ResultSet::FirstTuple))
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
							item->setData(Qt::UserRole, res.isColumnValueNull(col) ? ColumnNullValue : item->text());
					}

					results_tbw->setItem(row, col, item);
				}

				//Configure the vertical header to show the current tuple id
				results_tbw->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
				row++;
			}
			while(res.accessTuple(ResultSet::NextTuple));
		}

		results_tbw->setUpdatesEnabled(true);
		results_tbw->blockSignals(false);
		results_tbw->resizeColumnsToContents();
		results_tbw->resizeRowsToContents();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLExecutionWidget::handleExecutionAborted(Exception e)
{
	QString time_str=QString("[%1]:").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")));

	switchToExecutionMode(false);
	msgoutput_lst->clear();

	PgModelerUiNs::createOutputListItem(msgoutput_lst,
										QString("%1 %2").arg(time_str).arg(e.getErrorMessage()),
										QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")), false);

	if(e.getErrorType()==ErrorCode::ConnectionTimeout ||
		 e.getErrorType()==ErrorCode::ConnectionBroken)
	{
		PgModelerUiNs::createOutputListItem(msgoutput_lst,
											QString("%1 %2").arg(time_str).arg(trUtf8("No results retrieved or changes done due to the error above! Run the command again.")),
											QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), false);
	}

	msgoutput_lst->setVisible(true);
	results_parent->setVisible(false);
	export_tb->setEnabled(false);
	filter_tb->setEnabled(false);
	filter_tb->setChecked(false);

	output_tbw->setTabText(0, trUtf8("Results"));
	output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));
	output_tbw->setCurrentIndex(1);
	output_tbw->setTabEnabled(0, false);

	addToSQLHistory(sql_cmd_txt->toPlainText(), 0, e.getErrorMessage());
}

void SQLExecutionWidget::finishExecution(int rows_affected)
{
	if(sql_exec_hlp.isCancelled())
		destroyResultModel();
	else
	{
		bool empty = false;
		ResultSetModel *res_model = sql_exec_hlp.getResultSetModel();

		results_tbw->setSortingEnabled(false);
		results_tbw->blockSignals(true);
		results_tbw->setUpdatesEnabled(false);

		destroyResultModel();

		results_tbw->setModel(res_model);
		results_tbw->resizeColumnsToContents();
		results_tbw->setUpdatesEnabled(true);
		results_tbw->blockSignals(false);

		filter_edt->blockSignals(true);
		filter_edt->clear();
		filter_edt->blockSignals(false);

		columns_cmb->blockSignals(true);
		columns_cmb->clear();

		for(int col = 0; col < res_model->columnCount(QModelIndex()); col++)
			columns_cmb->addItem(res_model->headerData(col, Qt::Horizontal, Qt::DisplayRole).toString());

		columns_cmb->blockSignals(false);

		end_exec=QDateTime::currentDateTime().toMSecsSinceEpoch();
		total_exec = end_exec - start_exec;

		addToSQLHistory(sql_cmd_txt->toPlainText(), rows_affected);

		empty = (!res_model || res_model->rowCount() == 0);
		output_tbw->setTabEnabled(0, !empty);
		results_parent->setVisible(!empty);
		export_tb->setEnabled(!empty);
		filter_tb->setEnabled(!empty);

		if(!empty)
		{
			output_tbw->setTabText(0, trUtf8("Results (%1)").arg(res_model->rowCount()));
			output_tbw->setCurrentIndex(0);
		}
		else
		{
			output_tbw->setTabText(0, trUtf8("Results"));
			output_tbw->setCurrentIndex(1);
		}

		msgoutput_lst->clear();

		for(QString notice : sql_exec_hlp.getNotices())
		{
			PgModelerUiNs::createOutputListItem(msgoutput_lst,
																					QString("[%1]: %2").arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))).arg(notice.trimmed()),
																					QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), false);
		}

		PgModelerUiNs::createOutputListItem(msgoutput_lst,
																				PgModelerUiNs::formatMessage(trUtf8("[%1]: SQL command successfully executed in <em><strong>%2</strong></em>. <em>%3 <strong>%4</strong></em>")
																																		 .arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")))
																																		 .arg(total_exec >= 1000 ? QString("%1 s").arg(total_exec/1000) : QString("%1 ms").arg(total_exec))
																																		 .arg(!res_model ? trUtf8("Rows affected") :  trUtf8("Rows retrieved"))
																																		 .arg(rows_affected)),
																				QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));

		output_tbw->setTabText(1, trUtf8("Messages (%1)").arg(msgoutput_lst->count()));
	}

	switchToExecutionMode(false);
	sql_exec_thread.quit();
}

void SQLExecutionWidget::filterResults(void)
{
	QModelIndexList list;
	Qt::MatchFlags flags = Qt::MatchStartsWith;
	int rows_cnt = results_tbw->model()->rowCount();

	if(exact_chk->isChecked())
		flags = Qt::MatchExactly;
	else if(regexp_chk->isChecked())
			flags = Qt::MatchRegExp;

	if(case_sensitive_chk->isChecked())
		flags |= Qt::MatchCaseSensitive;

	list = results_tbw->model()->match(results_tbw->model()->index(0, columns_cmb->currentIndex()),
																		 Qt::DisplayRole, filter_edt->text(), -1, flags);

	results_tbw->blockSignals(true);
	results_tbw->setUpdatesEnabled(false);

	for(int row = 0; row < rows_cnt; row++)
		results_tbw->hideRow(row);

	if(!list.isEmpty())
	{
		for(auto &idx : list)
			results_tbw->showRow(idx.row());
	}

	results_tbw->blockSignals(false);
	results_tbw->setUpdatesEnabled(true);
	results_tbw->update();
}

void SQLExecutionWidget::addToSQLHistory(const QString &cmd, unsigned rows, const QString &error)
{
	if(!cmd.isEmpty())
	{
		QString fmt_cmd;

		if(!cmd_history_txt->toPlainText().isEmpty())
			fmt_cmd += QString("\n");

		fmt_cmd += QString("-- %1 [%2] -- \n")
							 .arg(trUtf8("Executed at"))
							 .arg(QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss.zzz")));
		fmt_cmd += cmd;
		fmt_cmd += QChar('\n');

		if(!error.isEmpty())
		{
			fmt_cmd += QString("-- %1 --\n").arg(trUtf8("Command failed"));
			fmt_cmd += QString("/*\n%1\n*/\n").arg(error);
		}
		else
			fmt_cmd += QString("-- %1 %2\n").arg(trUtf8("Rows:")).arg(rows);

		if(!fmt_cmd.trimmed().endsWith(Attributes::DdlEndToken))
			fmt_cmd += Attributes::DdlEndToken + QChar('\n');

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
		cmds = cmds.mid(cmds.indexOf(Attributes::DdlEndToken) + Attributes::DdlEndToken.length());
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

void SQLExecutionWidget::switchToExecutionMode(bool value)
{
	run_sql_tb->setVisible(!value);
	stop_tb->setVisible(value);
	file_tb->setEnabled(!value);
	find_tb->setEnabled(!value);
	clear_btn->setEnabled(!value);
	snippets_tb->setEnabled(!value);
	export_tb->setEnabled(!value);
	output_tb->setEnabled(!value);
	sql_cmd_txt->setEnabled(!value);
	cmd_history_parent->setEnabled(!value);
	find_history_parent->setEnabled(!value);
	filter_tb->setEnabled(!value);

	if(value)
	{
		this->setCursor(Qt::WaitCursor);
		sql_cmd_txt->setCursor(Qt::WaitCursor);
		sql_cmd_txt->clearFocus();
	}
	else
	{
		this->setCursor(Qt::ArrowCursor);
		sql_cmd_txt->setCursor(Qt::ArrowCursor);
		sql_cmd_txt->setFocus();
	}
}

void SQLExecutionWidget::destroyResultModel(void)
{
	if(results_tbw->model())
	{
		ResultSetModel *result_model = dynamic_cast<ResultSetModel *>(results_tbw->model());
		results_tbw->blockSignals(true);
		results_tbw->setModel(nullptr);
		delete(result_model);
		results_tbw->blockSignals(false);
	}
}

void SQLExecutionWidget::runSQLCommand(void)
{
	QString cmd=sql_cmd_txt->textCursor().selectedText();

	output_tb->setChecked(true);

	if(cmd.isEmpty())
		cmd=sql_cmd_txt->toPlainText();
	else
		cmd.replace(QChar::ParagraphSeparator, '\n');

	msgoutput_lst->clear();
	sql_exec_hlp.setCommand(cmd);
	start_exec=QDateTime::currentDateTime().toMSecsSinceEpoch();
	sql_exec_thread.start();
	switchToExecutionMode(true);

	output_tbw->setTabEnabled(0, false);
	output_tbw->setTabText(0, trUtf8("Results"));
	output_tbw->setCurrentIndex(1);
	PgModelerUiNs::createOutputListItem(msgoutput_lst,
																			PgModelerUiNs::formatMessage(trUtf8("[%1]: SQL command is running...")
																																	 .arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz")))),
																			QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));
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
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
											ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed)
											.arg(sql_file_dlg.selectedFiles().at(0)),
											ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		sql_cmd_txt->clear();
		sql_cmd_txt->setPlainText(file.readAll());
		file.close();

		filename_edt->setText(sql_file_dlg.selectedFiles().at(0));
		filename_wgt->setVisible(true);
	}
}

void SQLExecutionWidget::exportResults(QTableView *results_tbw)
{
	if(!results_tbw)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed)
											.arg(csv_file_dlg.selectedFiles().at(0)),
											ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QApplication::setOverrideCursor(Qt::WaitCursor);
		results_tbw->setUpdatesEnabled(false);
		results_tbw->blockSignals(true);
		results_tbw->selectAll();

		file.write(generateCSVBuffer(results_tbw));
		file.close();

		results_tbw->clearSelection();
		results_tbw->blockSignals(false);
		results_tbw->setUpdatesEnabled(true);
		QApplication::restoreOverrideCursor();
	}
}

int SQLExecutionWidget::clearAll(void)
{
	Messagebox msg_box;
	int res = 0;

	msg_box.show(trUtf8("The SQL input field and the results grid will be cleared! Want to proceed?"),
							 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

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

QByteArray SQLExecutionWidget::generateCSVBuffer(QTableView *results_tbw)
{
	return(generateBuffer(results_tbw, QChar(';'), true, true));
}

QByteArray SQLExecutionWidget::generateTextBuffer(QTableView *results_tbw)
{
	return(generateBuffer(results_tbw, QChar('\t'), false, false));
}

QByteArray SQLExecutionWidget::generateBuffer(QTableView *results_tbw, QChar separator, bool incl_col_names, bool use_quotes)
{
	if(!results_tbw)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!results_tbw->selectionModel())
		return (QByteArray());

	QAbstractItemModel *model = results_tbw->model();
	QModelIndexList sel_indexes = results_tbw->selectionModel()->selectedIndexes();
	QByteArray buf;
	QStringList line;
	QModelIndex index;
	QString str_pattern = use_quotes ? QString("\"%1\"") : QString("%1");
	int start_row = -1, start_col = -1,
			row_cnt = 0, col_cnt = 0;

	start_row = sel_indexes.at(0).row();
	start_col = sel_indexes.at(0).column();
	row_cnt = (sel_indexes.last().row() - start_row) + 1;
	col_cnt = (sel_indexes.last().column() - start_col) + 1;

	int col=0, row=0,
			max_col=start_col + col_cnt,
			max_row=start_row + row_cnt;

	if(incl_col_names)
	{
		//Creating the header
		for(col=start_col; col < max_col; col++)
			line.append(str_pattern.arg(model->headerData(col, Qt::Horizontal).toString()));

		buf.append(line.join(separator));
		buf.append('\n');
		line.clear();
	}

	//Creating the content
	for(row=start_row; row < max_row; row++)
	{
		for(col=start_col; col < max_col; col++)
		{
			index = model->index(row, col);
			line.append(str_pattern.arg(index.data().toString()));
		}

		buf.append(line.join(separator));
		line.clear();
		buf.append('\n');
	}

	return(buf);
}

void SQLExecutionWidget::copySelection(QTableView *results_tbw, bool use_popup, bool csv_is_default)
{
	if(!results_tbw)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QItemSelectionModel *selection = results_tbw->selectionModel();

	if(selection && (!use_popup || (use_popup && QApplication::mouseButtons()==Qt::RightButton)))
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
			QByteArray buf;

			if((use_popup && act == act_csv) || (!use_popup && csv_is_default))
			{
				//Generates the csv buffer and assigns it to application's clipboard
				buf=generateCSVBuffer(results_tbw);

				/* Making DataManipulationForm instances know that the clipboard has csv buffer
				 * in order to paste the contents properly */
				DataManipulationForm::setHasCsvClipboard(true);
			}
			else if((use_popup && act == act_txt) || (!use_popup && !csv_is_default))
			{
				buf=generateTextBuffer(results_tbw);
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
			attribs[Attributes::Connection] = hist.first;
			attribs[Attributes::Commands] = hist.second;
			schparser.ignoreEmptyAttributes(true);
			commands += schparser.getCodeDefinition(GlobalAttributes::TmplConfigurationDir +
																							GlobalAttributes::DirSeparator +
																							GlobalAttributes::SchemasDir +
																							GlobalAttributes::DirSeparator +
																							Attributes::Commands +
																							GlobalAttributes::SchemaExt, attribs);
		}

		schparser.loadFile(GlobalAttributes::TmplConfigurationDir +
											 GlobalAttributes::DirSeparator +
											 GlobalAttributes::SchemasDir +
											 GlobalAttributes::DirSeparator +
											 GlobalAttributes::SQLHistoryConf +
											 GlobalAttributes::SchemaExt);

		attribs.clear();
		attribs[Attributes::Commands] = commands;
		buffer.append(schparser.getCodeDefinition(attribs));


		file.setFileName(GlobalAttributes::ConfigurationsDir +
										 GlobalAttributes::DirSeparator +
										 GlobalAttributes::SQLHistoryConf +
										 GlobalAttributes::ConfigurationExt);

		if(!file.open(QFile::WriteOnly))
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(file.fileName()),
											ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__ ,__LINE__);

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
		XmlParser xmlparser;
		attribs_map attribs;

		xmlparser.setDTDFile(GlobalAttributes::TmplConfigurationDir +
												 GlobalAttributes::DirSeparator +
												 GlobalAttributes::ObjectDTDDir +
												 GlobalAttributes::DirSeparator +
												 GlobalAttributes::SQLHistoryConf +
												 GlobalAttributes::ObjectDTDExt,
												 GlobalAttributes::SQLHistoryConf);

		xmlparser.loadXMLFile(GlobalAttributes::ConfigurationsDir +
													GlobalAttributes::DirSeparator +
													GlobalAttributes::SQLHistoryConf +
													GlobalAttributes::ConfigurationExt);

		cmd_history.clear();

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementName() == Attributes::Commands)
				{
					xmlparser.getElementAttributes(attribs);
					xmlparser.savePosition();

					if(xmlparser.accessElement(XmlParser::ChildElement))
						cmd_history[attribs[Attributes::Connection]].append(xmlparser.getElementContent());

					xmlparser.restorePosition();
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
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
								Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Accepted)
	{
		QFile::remove(GlobalAttributes::ConfigurationsDir +
									GlobalAttributes::DirSeparator +
									GlobalAttributes::SQLHistoryConf +
									GlobalAttributes::ConfigurationExt);

		SQLExecutionWidget::cmd_history.clear();
	}
}

void SQLExecutionWidget::setSQLHistoryMaxLength(int len)
{
	if(len < 1000 || len > 20000)
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
	QAction *action_clear = new QAction(QPixmap(PgModelerUiNs::getIconPath("limpartexto")), trUtf8("Clear history"), ctx_menu),
			*action_save = new QAction(QPixmap(PgModelerUiNs::getIconPath("salvar")), trUtf8("Save history"), ctx_menu),
			*action_reload = new QAction(QPixmap(PgModelerUiNs::getIconPath("atualizar")), trUtf8("Reload history"), ctx_menu),
			*action_toggle_find = nullptr,
			*exec_act = nullptr;

	if(!find_history_parent->isVisible())
		action_toggle_find = new QAction(QPixmap(PgModelerUiNs::getIconPath("buscar")), trUtf8("Find in history"), ctx_menu);
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
									Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

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
