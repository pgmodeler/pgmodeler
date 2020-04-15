/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "numberedtexteditor.h"
#include "databaseexplorerwidget.h"
#include "sqlexecutionwidget.h"

class SQLToolWidget: public QWidget, public Ui::SQLToolWidget {
	private:
		Q_OBJECT

		QToolButton *sql_exec_corner_btn;

		NumberedTextEditor *sourcecode_txt;

		SyntaxHighlighter *sourcecode_hl;

		//! \brief Database import helper used to list objects from current connection
		DatabaseImportHelper import_helper;

		/*! \brief Controls the link between a database explorer instance and SQL execution widgets.
		When a database explorer is closed all the SQL execution panes related to it are closed too.
		(see addSQLExecutionTab and closeSQLExecutionTab for deitails) */
		QMap<QWidget *, QWidgetList> sql_exec_wgts;

		bool eventFilter(QObject *object, QEvent *event);

	public:
		SQLToolWidget(QWidget * parent = nullptr);
		virtual ~SQLToolWidget();

		//! \brief Force the update of the sql command input field and the syntax highligter attached to the opened tabs
		void updateTabs();

		//! \brief Indicates if there is at least one database being browsed through explorer widget
		bool hasDatabasesBrowsed();

	public slots:
		void configureSnippets();
		void clearDatabases();

		//! \brief Add a tab to permit the SQL execution for the current database being browsed
		SQLExecutionWidget *addSQLExecutionTab(const QString &sql_cmd = QString());

	protected slots:
		//! \brief Add a tab by browsing a database in the specified connection, loads the sql file and put its contents on a SQL execution
		void addSQLExecutionTab(const QString &conn_id, const QString &database, const QString &sql_file);

	private slots:
		//! \brief Opens a connection to the selected server
		void connectToServer();

		//! \brief Disconnect from server and close any opened database explorer or sql execution tab
		void disconnectFromDatabases();

		//! \brief Removes all DatabaseExplorerWidget instances linked to the dropped database
		void handleDatabaseDropped(const QString &dbname);

		//! \brief Open the current database in a database explorer instance
		DatabaseExplorerWidget *browseDatabase();

		//! \brief Show the selected snippet on the current opened SQL execution tab
		void showSnippet(const QString &snip);

		//! \brief Close the database explorer specified by its index. Also, closes any SQL exec. tab related to it
		void closeDatabaseExplorer(int idx);

		//! \brief Close the SQL execution tab specified by its index
		void closeSQLExecutionTab(int idx);

	signals:
		/*! \brief This signal is emitted whenever the user changes the connections settings
		within this widget without use the main configurations dialog */
		void s_connectionsUpdateRequest();

		friend class MainWindow;
};

#endif
