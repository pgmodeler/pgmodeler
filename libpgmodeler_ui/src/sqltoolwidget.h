/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

		//! brief Stores the actions to drop and show object's data
		QMenu handle_menu;

		QAction *copy_action, *drop_action, *drop_cascade_action, *show_data_action, *refresh_action;

		/*! brief Enables/Disables the fields for sql input and execution.
				When enabling a new connection to server will be opened. */
		void enableSQLExecution(bool enable);

		//! brief Drops the object represented by the specified item
		void dropObject(QTreeWidgetItem *item, bool cascade);

		//! brief Shows the data of the object represented by the specified item
		void showObjectData(QTreeWidgetItem *item);

		//! brief Stores the command on the sql command history
		void registerSQLCommand(const QString &cmd);

		void showError(Exception &e);

		bool eventFilter(QObject *object, QEvent *event);

		void configureImportHelper(void);

		void fillResultsTable(ResultSet &res);

	public:
		SQLToolWidget(QWidget * parent = 0);

		//! \brief Updates the connections combo
		void updateConnections(map<QString, Connection *> &conns);

		//! brief Fills up the results grid based upon the specified result set
		static void fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw);

		//! brief Copy to clipboard (in csv format) the current selected items on results grid
		static void copySelection(QTableWidget *results_tbw);

		//! brief Generates a CSV buffer based upon the selection on the results grid
		static QByteArray generateCSVBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt);

		//! brief Exports the results to csv file
		static void exportResults(QTableWidget *results_tbw);

	public slots:
		void hide(void);

	private slots:
		//! brief Opens a connection to the selected database
		void connectToDatabase(void);

		void disconnectFromDatabase(void);

		//! brief Lists all objects for the current selected database
		void listObjects(void);

		//! brief Updates on the tree under the current selected object
		void updateCurrentItem(void);

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

		//! brief Shows the menu to drop/show data
		void handleObject(QTreeWidgetItem *item, int);

		//! brief Drop the current selected database
		void dropDatabase(void);

		//! brief Show the widget to handle data in tables
		void openDataGrid(void);

		void enableObjectTreeControls(bool enable);

	signals:
		void s_visibilityChanged(bool);
};

#endif
