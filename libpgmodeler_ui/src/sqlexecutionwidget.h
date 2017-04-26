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

/**
\ingroup libpgmodeler_ui
\class SQLExecutionWidget
\brief Implements the operations that permit user to perform basic SQL commands directly on a server
*/

#ifndef SQL_EXECUTION_WIDGET_H
#define SQL_EXECUTION_WIDGET_H

#include "ui_sqlexecutionwidget.h"
#include "syntaxhighlighter.h"
#include "connection.h"
#include "databaseimportform.h"
#include "findreplacewidget.h"
#include "codecompletionwidget.h"
#include "numberedtexteditor.h"
#include "findreplacewidget.h"

class SQLExecutionWidget: public QWidget, public Ui::SQLExecutionWidget {
	private:
		Q_OBJECT

		static map<QString, QString> cmd_history;

		static int cmd_history_max_len;

		SchemaParser schparser;

		//! \brief Syntax highlighter for sql input field
		SyntaxHighlighter *sql_cmd_hl,

		//! \brief Syntax highlighter for command history field
		*cmd_history_hl;

		//! \brief Connection used to run commands specified on sql input field
		Connection sql_cmd_conn;

		//! \brief Dialog for SQL save/load
		QFileDialog sql_file_dlg;

		QMenu snippets_menu,

		file_menu;

		QAction *action_save, *action_save_as, *action_load;

		FindReplaceWidget *find_replace_wgt;

		CodeCompletionWidget *code_compl_wgt;

		FindReplaceWidget *find_history_wgt;

		/*! \brief Enables/Disables the fields for sql input and execution.
				When enabling a new connection to server will be opened. */
		void enableSQLExecution(bool enable);

		//! \brief Stores the command on the sql command history
		void addToSQLHistory(const QString &cmd, unsigned rows=0, const QString &error=QString());

		//! \brief Show the exception message in the output widget
		void showError(Exception &e);

		//! \brief Fills the result grid with the specified result set
		void fillResultsTable(ResultSet &res);

		static void validateSQLHistoryLength(const QString &conn_id, const QString &fmt_cmd = QString(), NumberedTextEditor *cmd_history_txt = nullptr);

	protected:
		//! \brief Widget that serves as SQL commands input
		NumberedTextEditor *sql_cmd_txt,

		*cmd_history_txt;

		void showEvent(QShowEvent *);
		void resizeEvent(QResizeEvent *);
		bool eventFilter(QObject *object, QEvent *event);

	public:
		SQLExecutionWidget(QWidget * parent = 0);

		//! \brief Configures the connection to query the server
		void setConnection(Connection conn);

		/*! \brief Fills up the results grid based upon the specified result set.
				The parameter store_data will make each item store the text as its data */
		static void fillResultsTable(Catalog &catalog, ResultSet &res, QTableWidget *results_tbw, bool store_data=false);

		//! \brief Copy to clipboard (in csv format) the current selected items on results grid
		static void copySelection(QTableWidget *results_tbw, bool use_popup=true);

		//! \brief Generates a CSV buffer based upon the selection on the results grid
		static QByteArray generateCSVBuffer(QTableWidget *results_tbw, int start_row, int start_col, int row_cnt, int col_cnt);

		//! \brief Exports the results to csv file
		static void exportResults(QTableWidget *results_tbw);

	public slots:
		void configureSnippets(void);

		//! \brief Save the history of all connections open in the SQL Execution to the sql-history.conf
		static void saveSQLHistory(void);

		//! \brief Load the history from the file sql-history.conf
		static void loadSQLHistory(void);

		static void destroySQLHistory(void);

		static void setSQLHistoryMaxLength(int len);

		static int getSQLHistoryMaxLength(void);

	private slots:
		//! \brief Enables the command buttons when user fills the sql field
		void enableCommandButtons(void);

		//! \brief Runs the current typed sql command
		void runSQLCommand(void);

		//! \brief Save the current typed sql command on a file
		void saveCommands(void);

		//! \brief Load a sql command from a file
		void loadCommands(void);

		//! \brief Clears the input field as well the results grid
		int clearAll(void);

		void selectSnippet(QAction *act);

		void handleSelectedWord(QString word);

		void toggleOutputPane(bool visible);

		void showHistoryContextMenu(void);

		friend class SQLToolWidget;
};

#endif
