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
\class DataGridForm
\brief Implements the operations to handle table's data
*/

#ifndef DATA_MANIPULATION_FORM_H
#define DATA_MANIPULATION_FORM_H

#include "ui_datamanipulationform.h"
#include "catalog.h"
#include "syntaxhighlighter.h"
#include "codecompletionwidget.h"

class LIBPGMODELER_UI_EXPORT DataManipulationForm: public QDialog, public Ui::DataManipulationForm {
	private:
		Q_OBJECT

		//! brief Constants used to mark the type of operation performed on rows
		static const unsigned NO_OPERATION,	OP_INSERT, OP_UPDATE, OP_DELETE;

		//! brief Default row colors for each operation type
		static const QColor ROW_COLORS[3];

		//! brief Default char used as unescaped value delimiter (< and >)
		static const QChar UNESC_VALUE_START, UNESC_VALUE_END;

		SyntaxHighlighter *filter_hl;

		CodeCompletionWidget *code_compl_wgt;

		Connection connection;

		//! brief Catalog object used to list database objects
		Catalog catalog;

		//! brief Current editing table columns names
		QStringList col_names;

		//! brief Stores the ids of changed rows. These ids are handled on saveChanges() method
		vector<int> changed_rows,

		//! brief Stores the ids of primary key columns. These ids are handled on getDMLCommand() method
		pk_col_ids;

		//! brief Stores the previous color of the rows before being marked with some operation
		map<int, QBrush> prev_row_colors;

		//! brief Fills a combobox with the names of objects retrieved from catalog
		void listObjects(QComboBox *combo, vector<ObjectType> obj_types, const QString &schema=QString());

		//! brief Retrieve the primary key column ids for the specified table
		void retrievePKColumns(const QString &schema, const QString &table);

		/*! brief Mark the line as changed, changing its background color and applying the respective operation (see OP_??? constant)
				when user call saveChanged() */
		void markOperationOnRow(unsigned operation, int row);

		//! brief Generates a DML command for the row depending on the it's operation type
		QString getDMLCommand(int row);

		//! brief Remove the rows marked as OP_INSERT which ids are specified on the parameter vector
		void removeNewRows(vector<int> &ins_rows);

		//! brief Reset the state of changed rows, clearing all attributes used to control the modifications on them
		void clearChangedRows(void);

	public:
		DataManipulationForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

		//! brief Defines the connection and current schema and table to be handled, this method should be called before show the dialog
    void setAttributes(Connection conn, const QString curr_schema=QString("public"), const QString curr_table=QString());

	private slots:
		//! brief List the tables based upon the current schema
		void listTables(void);

		//! brief List the columns based upon the current table
		void listColumns(void);

		//! brief Retrieve the data for the current table filtering the data as configured on the advanced tab
		void retrieveData(void);

		//! brief Disable the buttons used to handle data
		void disableControlButtons(void);

		//! brief Reset the state of advaced tab's controls
		void resetAdvancedControls(void);

		//! brief Enables/disables the buttons of the order by list depending on the state of it
		void enableColumnControlButtons(void);

		//! brief Add a column to the "order by" list
		void addColumnToList(void);

		//! brief Remove a column from the "order by" list
		void removeColumnFromList(void);

		//! brief Clears the "order by" list
		void clearColumnList(void);

		//! brief Toggles the sort mode between ASC and DESC when right clicking on a element at order by list
		void changeOrderMode(QListWidgetItem *item);

		//! brief Mark the entire row in which the item resides
		void markUpdateOnRow(QTableWidgetItem *item);

		//! brief Mark a seleciton of rows to be delete. New rows are automatically removed
		void markDeleteOnRows(void);

		//! brief Insert a new row on the grid with the first column with edition enabled
		void insertRow(void);

		//! brief Undo the operation made on all rows or in a set of selected rows
		void undoOperations(void);

		//! brief Insert a new row as the user press tab key on the last column at last row
		void insertRowOnTabPress(int curr_row, int curr_col, int prev_row, int prev_col);

		//! brief Commit all changes made on the rows rolling back changes when some error is triggered
		void saveChanges(void);

		//! brief Swap two rows on the order by list
		void swapColumns(void);
};

#endif
