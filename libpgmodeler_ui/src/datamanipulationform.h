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
\class DataGridForm
\brief Implements the operations to handle table's data
*/

#ifndef DATA_MANIPULATION_FORM_H
#define DATA_MANIPULATION_FORM_H

#include "ui_datamanipulationform.h"
#include "catalog.h"
#include "syntaxhighlighter.h"
#include "codecompletionwidget.h"
#include "csvloadwidget.h"

class DataManipulationForm: public QDialog, public Ui::DataManipulationForm {
	private:
		Q_OBJECT
		
		//! \brief Constants used to mark the type of operation performed on rows
		static constexpr unsigned NoOperation=0,
		OpInsert=1,
		OpUpdate=2,
		OpDelete=3;

		//! \brief Default row colors for each operation type
		static const QColor RowColors[3];

		CsvLoadWidget *csv_load_wgt;

		SyntaxHighlighter *filter_hl;
		
		CodeCompletionWidget *code_compl_wgt;

		QMenu fks_menu, copy_menu, truncate_menu, paste_menu;

		//! \brief Store the template connection params to be used by catalogs and command execution connections
		attribs_map tmpl_conn_params;
		
		//! \brief Current editing table columns names
		QStringList col_names,
		
		//! \brief Current editing table pk columns names
		pk_col_names;

		//! \brief Stores the current table's name (schema.table)
		QString curr_table_name;

		/*! \brief Stores the current opened table's oid. This attribute is filled only the table has an primary
		and it is used to retrieve all foreign keys that references the current table */
		unsigned table_oid;
		
		//! \brief Stores the ids of changed rows. These ids are handled on saveChanges() method
		vector<int> changed_rows;
		
		//! \brief Stores the previous color of the rows before being marked with some operation
		map<int, QBrush> prev_row_colors;

		//! \brief Stores the fk informations about referenced tables
		map<QString, attribs_map> fk_infos,

		//! \brief Stores the fk informations about referencing tables
		ref_fk_infos;
		
		//! \brief Fills a combobox with the names of objects retrieved from catalog
		void listObjects(QComboBox *combo, vector<ObjectType> obj_types, const QString &schema=QString());
		
		//! \brief Retrieve the primary key column ids for the specified table
		void retrievePKColumns(const QString &schema, const QString &table);

		/*! \brief Retrieve the foreign key columns info for the specified table. These data is used to browse referenced tables in the data
		 that the selected line holds */
		void retrieveFKColumns(const QString &schema, const QString &table);
		
		/*! \brief Mark the line as changed, changing its background color and applying the respective operation (see OP_??? constant)
				when user call saveChanged() */
		void markOperationOnRow(unsigned operation, int row);
		
		//! \brief Generates a DML command for the row depending on the it's operation type
		QString getDMLCommand(int row);
		
		//! \brief Remove the rows marked as OP_INSERT which ids are specified on the parameter vector
		void removeNewRows(vector<int> ins_rows);
		
		//! \brief Reset the state of changed rows, clearing all attributes used to control the modifications on them
		void clearChangedRows();

		//! brief Browse a referenced or referencing table by the provided foreign key name
		void browseTable(const QString &fk_name, bool browse_ref_tab);

		void resizeEvent(QResizeEvent *event);

		void closeEvent(QCloseEvent *);

		void setColumnsCheckState(Qt::CheckState state);

	public:
		DataManipulationForm(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::Widget);
		
		//! \brief Defines the connection and current schema and table to be handled, this method should be called before show the dialog
		void setAttributes(Connection conn, const QString curr_schema=QString("public"), const QString curr_table_name=QString(), const QString &filter=QString());

	private slots:
		void reject();

		void clearItemsText();

		void sortResults(int column, Qt::SortOrder order);

		//! \brief List the tables based upon the current schema
		void listTables();
		
		//! \brief List the columns based upon the current table
		void listColumns();
		
		//! \brief Retrieve the data for the current table filtering the data as configured on the advanced tab
		void retrieveData();
		
		//! \brief Disable the buttons used to handle data
		void disableControlButtons();

		//! \brief Enables the delete/duplicate/copy buttons depending on the selected rows
		void enableRowControlButtons();
		
		//! \brief Reset the state of advaced tab's controls
		void resetAdvancedControls();
		
		//! \brief Enables/disables the buttons of the order by list depending on the state of it
		void enableColumnControlButtons();
		
		//! \brief Add a column to the "order by" list
		void addSortColumnToList();
		
		//! \brief Remove a column from the "order by" list
		void removeSortColumnFromList();
		
		//! \brief Clears the "order by" list
		void clearSortColumnList();
		
		//! \brief Toggles the sort mode between ASC and DESC when right clicking on a element at order by list
		void changeOrderMode(QListWidgetItem *item);
		
		//! \brief Mark the entire row in which the item resides
		void markUpdateOnRow(QTableWidgetItem *item);
		
		//! \brief Mark a seleciton of rows to be delete. New rows are automatically removed
		void markDeleteOnRows();
		
		//! \brief Add a new row on the grid with the first column with edition enabled
		void addRow(bool focus_new_row = true);
		
		//! \brief Duplicate the selected rows creating new ones with the same values as the selection
		void duplicateRows();

		//! \brief Undo the operation made on all rows or in a set of selected rows
		void undoOperations();
		
		//! \brief Insert a new row as the user press tab key on the last column at last row
		void insertRowOnTabPress(int curr_row, int curr_col, int prev_row, int prev_col);
		
		//! \brief Commit all changes made on the rows rolling back changes when some error is triggered
		void saveChanges();
		
		//! \brief Swap two rows on the order by list
		void swapColumns();

		//! \brief Add new rows to the grid based upon the CSV loaded
		void loadDataFromCsv(bool load_from_clipboard = false, bool force_csv_parsing = false);

		//! \brief Browse the referenced table data using the selected row in the results grid
		void browseReferencedTable();

		//! \brief Browse the referencing table data using the selected row in the results grid
		void browseReferrerTable();

		//! \brief Truncates the browsed table
		void truncateTable();

		//! \brief Display or hides a column when the related item is interacted in the column list at filter section
		void toggleColumnDisplay(QListWidgetItem *item);

		//! \brief Opens a new data manipulation windows
		void openNewWindow();

		//! \brief Shows the popup menu over the current selection
		void showPopupMenu();
};

#endif
