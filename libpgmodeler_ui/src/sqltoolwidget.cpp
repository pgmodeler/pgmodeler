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

#include "sqltoolwidget.h"
#include "taskprogresswidget.h"
#include "databaseexplorerwidget.h"
#include "snippetsconfigwidget.h"
#include "sqlexecutionwidget.h"

SQLToolWidget::SQLToolWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);
  h_splitter->setSizes({0, 10000});

  connect(connect_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(disconnect_tb, SIGNAL(clicked(void)), this, SLOT(disconnectFromServer(void)));
  connect(connections_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disconnectFromServer()));
  connect(browse_tb, SIGNAL(clicked(void)), this, SLOT(browseDatabase(void)));
  connect(databases_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseExplorer(int)));
  connect(sql_exec_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeSQLExecutionTab(int)));

  connect(database_cmb, &QComboBox::currentTextChanged,
          [=](){ 	browse_tb->setEnabled(database_cmb->currentIndex() > 0); });
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
}

void SQLToolWidget::updateTabs(void)
{
  SQLExecutionWidget *sql_exec_wgt=nullptr;

  for(int i=0; i < sql_exec_tbw->count(); i++)
  {
    sql_exec_wgt=dynamic_cast<SQLExecutionWidget *>(sql_exec_tbw->widget(i));
    sql_exec_wgt-> sql_cmd_txt->updateLineNumbersSize();
    sql_exec_wgt-> sql_cmd_txt->updateLineNumbers();
    sql_exec_wgt->sql_cmd_hl->rehighlight();
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

    if(sender()==connect_tb && conn->isAutoBrowseDB())
    {
      database_cmb->setCurrentText(conn->getConnectionParam(Connection::PARAM_DB_NAME));
      browseDatabase();
    }
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

void SQLToolWidget::dropDatabase(const QString &dbname)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("Warning"),
               trUtf8("<strong>CAUTION:</strong> You are about to drop the entire database <strong>%1</strong>! All data will be completely wiped out. Do you really want to proceed?").arg(dbname),
							 Messagebox::ALERT_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		Connection aux_conn=(*conn);

		try
		{
      //Closing tabs related to the database to be dropped
      for(int i=0; i < databases_tbw->count(); i++)
      {
        if(databases_tbw->tabText(i)==dbname)
        {
          closeDatabaseExplorer(i);
          i=-1;
        }
      }

			aux_conn.connect();
      aux_conn.executeDDLCommand(QString("DROP DATABASE \"%1\";").arg(dbname));
			aux_conn.close();
      connectToServer();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void SQLToolWidget::openDataGrid(const QString &dbname, const QString &schema, const QString &table, bool hide_views)
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

      conn.setConnectionParam(Connection::PARAM_DB_NAME, (dbname.isEmpty() ? database_cmb->currentText() : dbname));
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

    connect(db_explorer_wgt, SIGNAL(s_dataGridOpenRequested(QString,QString,QString,bool)), this, SLOT(openDataGrid(QString,QString,QString,bool)));
    connect(db_explorer_wgt, SIGNAL(s_databaseDropRequested(QString)), this, SLOT(dropDatabase(QString)));
    connect(db_explorer_wgt, SIGNAL(s_sqlExecutionRequested()), this, SLOT(addSQLExecutionTab()));
    connect(db_explorer_wgt, SIGNAL(s_snippetShowRequested(QString)), this, SLOT(showSnippet(QString)));

    /* Forcing the signal s_sqlExecutionRequested to be emitted to properly register the
       new tab on the map of sql panes related to the database explorer */
    db_explorer_wgt->runsql_tb->click();
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
    Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
    SQLExecutionWidget *sql_exec_wgt=new SQLExecutionWidget;
    QString db_name=databases_tbw->tabText(databases_tbw->currentIndex());
    DatabaseExplorerWidget *db_explorer_wgt=dynamic_cast<DatabaseExplorerWidget *>(sender());

    conn.setConnectionParam(Connection::PARAM_DB_NAME, db_name);
    sql_exec_wgt->setConnection(conn);

    sql_exec_tbw->addTab(sql_exec_wgt, db_name);
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
    sql_exec_tbw->removeTab(sql_exec_tbw->indexOf(wgt));

  sql_exec_wgts.remove(db_explorer);

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
  sql_exec_wgt->sql_cmd_txt->setPlainText(snip);
}
