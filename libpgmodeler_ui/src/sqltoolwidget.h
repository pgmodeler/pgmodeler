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

/**
\ingroup libpgmodeler_ui
\class SQLToolWidget
\brief Implements the operations that permit user to perform basic SQL commands directly on a server
*/

#ifndef SQL_TOOL_WIDGET_H
#define SQL_TOOL_WIDGET_H

#include "ui_sqltoolwidget.h"
#include "syntaxhighlighter.h"
#include "connection.h"
#include "databaseimportform.h"
#include "datamanipulationform.h"
#include "findreplacewidget.h"
#include "codecompletionwidget.h"

class SQLToolWidget: public QWidget, public Ui::SQLToolWidget {
	private:
		Q_OBJECT

		//! brief Database import helper used to list objects from current connection
		DatabaseImportHelper import_helper;

  public:
		SQLToolWidget(QWidget * parent = 0);
    ~SQLToolWidget(void);

		//! \brief Updates the connections combo
		void updateConnections(map<QString, Connection *> &conns);

  private slots:
    //! brief Opens a connection to the selected server
    void connectToServer(void);

    void disconnectFromServer(void);

		//! brief Drop the current selected database
		void dropDatabase(void);

		//! brief Show the widget to handle data in tables
    void openDataGrid(const QString &schema=QString("public"), const QString &table=QString(), bool hide_views=true);

    //! brief Open the current database in a database explorer instance
    void browseDatabase(void);

    void executeSQLCommands(void);

    void closeDatabaseExplorer(int idx);

    void setCurrentDatabase(int idx);
};

#endif
