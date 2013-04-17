/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class ObjectTableWidget
\brief Implements a basic table (grid) which can be used commonly to store
objects references and show information about them on it's rows. This
class also implements operations to handle insertion/deletion/update/movements
of rows on the table.
*/

#ifndef OBJECT_TABLE_WIDGET_H
#define OBJECT_TABLE_WIDGET_H

#include "ui_objecttablewidget.h"
#include "messagebox.h"
#include "baseobjectwidget.h"

class ObjectTableWidget: public QWidget, public Ui::ObjectTableWidget {
	private:
		Q_OBJECT

		/*! \brief Indicates that a confirmation message must be shown when the user try
		to remove an element from table. By default, the exclusions are made without confirmation */
		bool conf_exclusion;

	public:
		//! \brief Constants used to configure the table buttons
		const static unsigned ADD_BUTTON=1,
													REMOVE_BUTTON=2,
													UPDATE_BUTTON=4,
													MOVE_BUTTONS=8,
													EDIT_BUTTON=16,
													REMOVE_ALL_BUTTON=32,
													ALL_BUTTONS=63,
													NO_BUTTONS=0;

		ObjectTableWidget(unsigned button_conf=ALL_BUTTONS,
											bool conf_exclusion=false, QWidget * parent = 0);

		//! \brief Sets the table's column count
		void setColumnCount(unsigned col_count);

		//! \brief Sets the specified column header label
		void setHeaderLabel(const QString &label, unsigned col_idx);

		//! \brief Sets the specified column header icon
		void setHeaderIcon(const QIcon &icon, unsigned col_idx);

		//! \brief Sets the icon of the specified cell
		void setCellIcon(const QIcon &icon, unsigned row_idx, unsigned col_idx);

		//! \brief Sets the text of the specified cell
		void setCellText(const QString &text, unsigned row_idx, unsigned col_idx);

		//! \brief Sets the data which the specified row stores
		void setRowData(const QVariant &data, unsigned row_idx);

		//! \brief Sets a individual font configuration for the specified row
		void setRowFont(int row_idx, const QFont &font, const QColor &fg_color, const QColor &bg_color);

		//! \brief Returns the table column count
		unsigned getColumnCount(void);

		//! \brief Returns the table row count
		unsigned getRowCount(void);

		//! \brief Returns the specified column header label
		QString getHeaderLabel(unsigned col_idx);

		//! \brief Returns the specified cell text
		QString getCellText(unsigned row_idx, unsigned col_idx);

		//! \brief Returns the data relative to the specified row
		QVariant getRowData(unsigned row_idx);

		//! \brief Remove the specified column from table
		void removeColumn(unsigned col_idx);

		//! \brief Adds a new column at the specified index
		void addColumn(unsigned col_idx);

		//! \brief Adds a new row at the specified index
		void addRow(unsigned lin_idx);

		//! \brief Returns the row index currently selected
		int getSelectedRow(void);

		/*! \brief Returns the row index search it through the specified row data. If
		no row is found returns -1 */
		int getRowIndex(const QVariant &data);

		//! \brief Clears the specified cell text
		void clearCellText(unsigned row_idx, unsigned col_idx);

		//! \brief Sets the table button configuration. Use the constants ???_BUTTON combined via bitwise operation.
		void setButtonConfiguration(unsigned button_conf);

	private slots:
		//! \brief Moves a row up or down according to the button that triggers the slot
		void moveRows(void);

		//! \brief Removes the currently selected line
		void removeRow(void);

		/*! \brief This method does not execute any action, only emit a signal indicating that the row
		is ready to the edition. The edit operation must be implemented by the user and be connected to
		the emitted signal. */
		void editRow(void);

		/*! \brief This method does not execute any action, only emit a signal indicating that the row
		is ready to the update. The update operation must be implemented by the user and be connected to
		the emitted signal. */
		void updateRow(void);

		//! \brief Enables the handle buttons according to the selected row
		void setButtonsEnabled(void);

	public slots:
		//! \brief Adds a new row at the end of the table
		void addRow(void);

		//! \brief Removes all the rows from table
		void removeRows(void);

		//! \brief Remove the specified row
		void removeRow(unsigned row_idx);

		//! \brief Clears the row selection disabling the buttons if necessary
		void clearSelection(void);

		//! \brief Selects the specified row
		void selectRow(int lin_idx);

		//! \brief Controls the enable state of each button
		void setButtonsEnabled(unsigned button_conf, bool value);

	signals:
		//! \brief Signal emitted when a new row is added. The new row index is send with the signal
		void s_rowAdded(int);

		//! \brief Signal emitted when two rows are swapped. The rows indexes are send together with the signal
		void s_rowsMoved(int,int);

		//! \brief Signal emitted when all rows are removed from table
		void s_rowsRemoved(void);

		//! \brief Signal emitted when a single row is removed. The row index is send together with the signal
		void s_rowRemoved(int);

		//! \brief Signal emitted when a row is selected. The row index is send together with the signal
		void s_rowSelected(int);

		//! \brief Signal emitted when a row is edited. The row index is send together with the signal
		void s_rowEdited(int);

		//! \brief Signal emitted when a row is updated. The row index is send together with the signal
		void s_rowUpdated(int);

		//! \brief Signal emitted when a column is removed. The column index is send together with the signal
		void s_columnRemoved(int);

		//! \brief Signal emitted when a column is added. The column index is send together with the signal
		void s_columnAdded(int);
};

#endif
