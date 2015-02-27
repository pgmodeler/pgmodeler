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
  //data_grid_tb->setToolTip(data_grid_tb->toolTip() + QString(" (%1)").arg(data_grid_tb->shortcut().toString()));

  connect(connect_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(connectToServer(void)));
  connect(disconnect_tb, SIGNAL(clicked(void)), this, SLOT(disconnectFromServer(void)));
  connect(connections_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(disconnectFromServer()));
  connect(browse_tb, SIGNAL(clicked(void)), this, SLOT(browseDatabase(void)));
  //connect(runsql_tb, SIGNAL(clicked(void)), this, SLOT(executeSQLCommands(void)));
	connect(drop_db_tb, SIGNAL(clicked(void)), this, SLOT(dropDatabase(void)));
  //connect(data_grid_tb, SIGNAL(clicked()), this, SLOT(openDataGrid()));
  connect(databases_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseExplorer(int)));
  connect(databases_tbw, SIGNAL(currentChanged(int)), this, SLOT(setCurrentDatabase(int)));

  connect(database_cmb, &QComboBox::currentTextChanged,
          [=](){ 	browse_tb->setEnabled(database_cmb->currentIndex() > 0);
									drop_db_tb->setEnabled(database_cmb->currentIndex() > 0);
                  //data_grid_tb->setEnabled(database_cmb->currentIndex() > 0);
                  /*runsql_tb->setEnabled(database_cmb->currentIndex() > 0); */ });
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
      //Closing tabs related to the database to be dropped
      for(int i=0; i < databases_tbw->count(); i++)
      {
        if(databases_tbw->tabText(i)==database_cmb->currentText())
        {
          closeDatabaseExplorer(i);
          i=-1;
        }
      }

			aux_conn.connect();
			aux_conn.executeDDLCommand(QString("DROP DATABASE \"%1\";").arg(database_cmb->currentText()));
			aux_conn.close();
      connectToServer();

      setCurrentDatabase(databases_tbw->currentIndex());
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
    connect(db_explorer_wgt, SIGNAL(s_sqlExecutionRequested()), this, SLOT(executeSQLCommands()));
    //connect(db_explorer_wgt, SIGNAL(s_snippetShowRequested(QString)), sql_cmd_txt, SLOT(setPlainText(QString)));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void SQLToolWidget::executeSQLCommands(void)
{
  try
  {
    Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
    SQLExecutionWidget *sql_exec_wgt=new SQLExecutionWidget;

    conn.setConnectionParam(Connection::PARAM_DB_NAME, database_cmb->currentText());
    sql_exec_wgt->setConnection(conn);

    sqlexec_tbw->addTab(sql_exec_wgt, database_cmb->currentText());
    sqlexec_tbw->setCurrentWidget(sql_exec_wgt);
    sqlexec_tbw->currentWidget()->layout()->setContentsMargins(4,4,4,4);
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

  //enableSQLExecution(databases_tbw->count()!=0);
}

void SQLToolWidget::setCurrentDatabase(int idx)
{
  try
  {
    DatabaseExplorerWidget *db_explorer=dynamic_cast<DatabaseExplorerWidget *>(databases_tbw->widget(idx));
    //sql_cmd_conn.close();

    //if(db_explorer)
//      sql_cmd_conn=db_explorer->getConnection();

 //   enableSQLExecution(db_explorer!=nullptr);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
