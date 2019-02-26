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

#include "sqltoolwidget.h"
#include "taskprogresswidget.h"
#include "databaseexplorerwidget.h"
#include "snippetsconfigwidget.h"
#include "sqlexecutionwidget.h"
#include "connectionsconfigwidget.h"
#include "pgmodeleruins.h"

SQLToolWidget::SQLToolWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	h_splitter->setSizes({315, 10000});
	h_splitter->handle(1)->installEventFilter(this);
	v_splitter->setSizes({1000, 400});

	sql_exec_corner_btn = new QToolButton;
	sql_exec_corner_btn->setIcon(QPixmap(PgModelerUiNs::getIconPath("newtab")));
	sql_exec_corner_btn->setIconSize(QSize(18, 18));
	sql_exec_corner_btn->setStyleSheet("QToolButton { margin-left: 4px; margin-bottom: 4px; padding: 2px; }");
	sql_exec_corner_btn->setShortcut(QKeySequence("Ctrl+T"));
	sql_exec_corner_btn->setToolTip(trUtf8("Add a new execution tab for the current database (%1)").arg(sql_exec_corner_btn->shortcut().toString()));
	sql_exec_tbw->setCornerWidget(sql_exec_corner_btn, Qt::TopRightCorner);

	QVBoxLayout *vbox=new QVBoxLayout;
	sourcecode_txt=new NumberedTextEditor(sourcecode_gb);
	sourcecode_txt->setReadOnly(true);

	sourcecode_hl=new SyntaxHighlighter(sourcecode_txt);
	sourcecode_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

	vbox->setContentsMargins(4,4,4,4);
	vbox->addWidget(sourcecode_txt);
	sourcecode_gb->setLayout(vbox);

	connect(connections_cmb, SIGNAL(activated(int)), this, SLOT(connectToServer(void)));
	connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
	connect(databases_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseExplorer(int)));
	connect(sql_exec_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeSQLExecutionTab(int)));
	connect(database_cmb, SIGNAL(activated(int)), this, SLOT(browseDatabase()));
	connect(disconnect_tb, SIGNAL(clicked()), this, SLOT(disconnectFromDatabases()));
	connect(source_pane_tb, SIGNAL(toggled(bool)), sourcecode_gb, SLOT(setVisible(bool)));
	connect(sql_exec_corner_btn, SIGNAL(clicked(bool)), this, SLOT(addSQLExecutionTab()));

	connect(databases_tbw, &QTabWidget::currentChanged,
			[&](){
				DatabaseExplorerWidget *dbexplorer=qobject_cast<DatabaseExplorerWidget *>(databases_tbw->currentWidget());
				QMap<QWidget *, QWidgetList> ::iterator itr=sql_exec_wgts.begin();

				sourcecode_txt->clear();

				if(dbexplorer && dbexplorer->objects_trw->currentItem())
					sourcecode_txt->setPlainText(dbexplorer->objects_trw->currentItem()->
																			 data(DatabaseImportForm::ObjectSource, Qt::UserRole).toString());

				while(itr != sql_exec_wgts.end())
				{
					if(itr.key() != dbexplorer)
					{
						for(auto &wgt : itr.value())
							sql_exec_tbw->removeTab(sql_exec_tbw->indexOf(wgt));
					}
					else
					{
						for(auto &wgt : itr.value())
							sql_exec_tbw->addTab(wgt, dbexplorer->getConnection().getConnectionParam(Connection::ParamDbName));
					}

					itr++;
				}

				disconnect_tb->setEnabled(databases_tbw->count() > 0);
			});
}

SQLToolWidget::~SQLToolWidget(void)
{
	databases_tbw->blockSignals(true);

	while(databases_tbw->count() > 0)
		closeDatabaseExplorer(0);
}

bool SQLToolWidget::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonDblClick &&
		 qobject_cast<QSplitterHandle *>(object) == h_splitter->handle(1))
	{
		if(h_splitter->sizes().at(0) != 0)
			h_splitter->setSizes({0, 10000});
		else
			h_splitter->setSizes({315, 10000});

		return(true);
	}

	return(QWidget::eventFilter(object, event));
}

void SQLToolWidget::updateTabs(void)
{
	SQLExecutionWidget *sql_exec_wgt=nullptr;

	for(int i=0; i < sql_exec_tbw->count(); i++)
	{
		sql_exec_wgt=dynamic_cast<SQLExecutionWidget *>(sql_exec_tbw->widget(i));
		sql_exec_wgt->sql_cmd_txt->updateLineNumbersSize();
		sql_exec_wgt->sql_cmd_txt->updateLineNumbers();
		sql_exec_wgt->sql_cmd_hl->rehighlight();

		//Forcing the update of the sql history widget (see SQLExecutionWidget::eventFilter)
		sql_exec_wgt->output_tbw->widget(2)->hide();
		sql_exec_wgt->output_tbw->widget(2)->show();
	}
}

void SQLToolWidget::configureSnippets(void)
{
	SQLExecutionWidget *sql_exec_wgt=nullptr;

	for(int i=0; i < sql_exec_tbw->count(); i++)
	{
		sql_exec_wgt=dynamic_cast<SQLExecutionWidget *>(sql_exec_tbw->widget(i));
		sql_exec_wgt->configureSnippets();
	}
}

void SQLToolWidget::clearDatabases()
{
	database_cmb->clear();
	database_cmb->setEnabled(false);
	refresh_tb->setEnabled(false);
}

void SQLToolWidget::connectToServer(void)
{
	try
	{
		if(connections_cmb->currentIndex()==connections_cmb->count()-1 &&
				ConnectionsConfigWidget::openConnectionsConfiguration(connections_cmb, true))
		{
			emit s_connectionsUpdateRequest();
		}
		else
		{
			Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
			clearDatabases();

			if(conn)
			{
				import_helper.setConnection(*conn);
				DatabaseImportForm::listDatabases(import_helper, database_cmb);
				import_helper.closeConnection();

				if(sender()==connections_cmb && conn->isAutoBrowseDB())
				{
					database_cmb->setCurrentText(conn->getConnectionParam(Connection::ParamDbName));
					browseDatabase();
				}
			}

			database_cmb->setEnabled(database_cmb->count() > 1);
			refresh_tb->setEnabled(database_cmb->isEnabled());
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::disconnectFromDatabases(void)
{
	try
	{
		Messagebox msg_box;

		msg_box.show(trUtf8("Warning"),
					 trUtf8("<strong>ATTENTION:</strong> Disconnect from all databases will close any opened tab in this view! Do you really want to proceed?"),
					 Messagebox::AlertIcon, Messagebox::YesNoButtons);

		if(msg_box.result()==QDialog::Accepted)
		{
			database_cmb->clear();
			connections_cmb->setEnabled(true);
			refresh_tb->setEnabled(false);

			while(databases_tbw->count() > 0)
			{
				databases_tbw->blockSignals(true);
				closeDatabaseExplorer(0);
				databases_tbw->blockSignals(false);
			}

			connections_cmb->setCurrentIndex(0);
			disconnect_tb->setEnabled(false);
			sourcecode_txt->clear();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLToolWidget::handleDatabaseDropped(const QString &dbname)
{
		try
		{
			//Closing tabs related to the database to be dropped
			for(int i=0; i < databases_tbw->count(); i++)
			{
				if(databases_tbw->tabText(i).remove('&') == dbname)
				{
					closeDatabaseExplorer(i);
					i=-1;
				}
			}

			connectToServer();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
}

void SQLToolWidget::browseDatabase(void)
{
	try
	{
		//If the selected database is already being browse do not create another explorer instance
		if(database_cmb->currentIndex() > 0)
		{
			Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
			QString maintainance_db=conn.getConnectionParam(Connection::ParamDbName);
			DatabaseExplorerWidget *db_explorer_wgt=new DatabaseExplorerWidget;

			db_explorer_wgt->setObjectName(database_cmb->currentText());
			conn.setConnectionParam(Connection::ParamDbName, database_cmb->currentText());
			db_explorer_wgt->setConnection(conn, maintainance_db);
			db_explorer_wgt->listObjects();

			databases_tbw->addTab(db_explorer_wgt, database_cmb->currentText());
			databases_tbw->setCurrentWidget(db_explorer_wgt);

			connect(db_explorer_wgt, SIGNAL(s_databaseDropped(QString)), this, SLOT(handleDatabaseDropped(QString)));
			connect(db_explorer_wgt, SIGNAL(s_sqlExecutionRequested()), this, SLOT(addSQLExecutionTab()));
			connect(db_explorer_wgt, SIGNAL(s_snippetShowRequested(QString)), this, SLOT(showSnippet(QString)));
			connect(db_explorer_wgt, SIGNAL(s_sourceCodeShowRequested(QString)), sourcecode_txt, SLOT(setPlainText(QString)));

			connect(attributes_tb, SIGNAL(toggled(bool)), db_explorer_wgt->attributes_wgt, SLOT(setVisible(bool)));
			db_explorer_wgt->attributes_wgt->setVisible(attributes_tb->isChecked());

			/* Forcing the signal s_sqlExecutionRequested to be emitted to properly register the
			new tab on the map of sql panes related to the database explorer */
			db_explorer_wgt->runsql_tb->click();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void SQLToolWidget::addSQLExecutionTab(void)
{
	try
	{
		SQLExecutionWidget *sql_exec_wgt=new SQLExecutionWidget;
		DatabaseExplorerWidget *db_explorer_wgt=dynamic_cast<DatabaseExplorerWidget *>(databases_tbw->currentWidget());
		Connection conn;

		if(!db_explorer_wgt)
		  return;

		conn = db_explorer_wgt->getConnection();
		sql_exec_wgt->setConnection(conn);
		sql_exec_tbw->addTab(sql_exec_wgt, conn.getConnectionParam(Connection::ParamDbName));
		sql_exec_tbw->setCurrentWidget(sql_exec_wgt);
		sql_exec_tbw->currentWidget()->layout()->setContentsMargins(4,4,4,4);
		sql_exec_wgts[db_explorer_wgt].push_back(sql_exec_wgt);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void SQLToolWidget::closeDatabaseExplorer(int idx)
{
	DatabaseExplorerWidget *db_explorer=dynamic_cast<DatabaseExplorerWidget *>(databases_tbw->widget(idx));

	//Closing sql execution tabs related to the database to be closed
	for(QWidget *wgt : sql_exec_wgts[db_explorer])
	{
		sql_exec_tbw->removeTab(sql_exec_tbw->indexOf(wgt));
		delete(wgt);
	}

	sql_exec_wgts.remove(db_explorer);
	databases_tbw->removeTab(idx);

	if(db_explorer)
		delete(db_explorer);
}

void SQLToolWidget::closeSQLExecutionTab(int idx)
{
	SQLExecutionWidget *sql_exec_wgt=dynamic_cast<SQLExecutionWidget *>(sql_exec_tbw->widget(idx));
	QMap<QWidget *, QWidgetList> ::iterator itr=sql_exec_wgts.begin();
	int idx1=-1;

	//Removing the widget from the list it belongs
	while(itr!=sql_exec_wgts.end())
	{
		idx1=itr.value().indexOf(sql_exec_wgt);

		if(idx1 >= 0)
		{
			itr.value().removeAt(idx1);
			break;
		}

		itr++;
	}

	sql_exec_tbw->removeTab(idx);

	if(sql_exec_wgt)
		delete(sql_exec_wgt);

}

void SQLToolWidget::showSnippet(const QString &snip)
{
	SQLExecutionWidget *sql_exec_wgt=nullptr;

	if(sql_exec_tbw->count()==0)
		addSQLExecutionTab();

	sql_exec_wgt=dynamic_cast<SQLExecutionWidget *>(sql_exec_tbw->currentWidget());

	if(sql_exec_wgt->sql_cmd_txt->isEnabled())
	{
		QTextCursor cursor=sql_exec_wgt->sql_cmd_txt->textCursor();
		cursor.movePosition(QTextCursor::End);

		sql_exec_wgt->sql_cmd_txt->appendPlainText(snip);
		sql_exec_wgt->sql_cmd_txt->setTextCursor(cursor);
	}
}
