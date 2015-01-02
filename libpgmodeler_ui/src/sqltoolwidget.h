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

		SchemaParser schparser;

		//! brief Syntax highlighter for sql input field
		SyntaxHighlighter *sql_cmd_hl;

		//! brief Database import helper used to list objects from current connection
		DatabaseImportHelper import_helper;

		//! brief Connection used to run commands specified on sql input field
		Connection sql_cmd_conn;

		//! brief Dialog for SQL save/load
		QFileDialog sql_file_dlg;

    QMenu snippets_menu;

		FindReplaceWidget *find_replace_wgt;

		CodeCompletionWidget *code_compl_wgt;

		/*! brief Enables/Disables the fields for sql input and execution.
				When enabling a new connection to server will be opened. */
		void enableSQLExecution(bool enable);

		//! brief Stores the command on the sql command history
		void registerSQLCommand(const QString &cmd);

		void showError(Exception &e);

		void fillResultsTable(ResultSet &res);

	public:
		SQLToolWidget(QWidget * parent = 0);
    ~SQLToolWidget(void);

		//! \brief Updates the connections combo
		void updateConnections(map<QString, Connection *> &conns);

		/*! brief Fills up the results grid based upon the specified result set.
				The parameter store_data will make each item store the text as its data */
		static void fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw, bool store_data=false);

		//! brief Copy to clipboard (in csv format) the current selected items on results grid
		static void copySelection(QTableWidget *results_tbw, bool use_popup=true);

		//! brief Generates a CSV buffer based upon the selection on the results grid
		static QByteArray generateCSVBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt);

		//! brief Exports the results to csv file
		static void exportResults(QTableWidget *results_tbw);

  public slots:
    void configureSnippets(void);

  private slots:
    //! brief Opens a connection to the selected server
    void connectToServer(void);

    void disconnectFromServer(void);

		//! brief Enables the command buttons when user fills the sql field
		void enableCommandButtons(void);

		//! brief Runs the current typed sql command
		void runSQLCommand(void);

		//! brief Save the current typed sql command on a file
		void saveCommands(void);

		//! brief Load a sql command from a file
		void loadCommands(void);

		//! brief Clears the input field as well the results grid
		void clearAll(void);

		//! brief Drop the current selected database
		void dropDatabase(void);

		//! brief Show the widget to handle data in tables
		void openDataGrid(const QString &schema="public", const QString &table="", bool hide_views=true);

    //! brief Open the current database in a database explorer instance
    void browseDatabase(void);

    void closeDatabaseExplorer(int idx);

    void setCurrentDatabase(int idx);

    void selectSnippet(QAction *act);

    void handleSelectedWord(QString word);
};

#endif
