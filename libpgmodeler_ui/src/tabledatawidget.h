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
\class TableDataWidget
\brief Form used to add initial data for tables.
*/

#ifndef TABLE_DATA_WIDGET_H
#define TABLE_DATA_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tabledatawidget.h"

class TableDataWidget: public BaseObjectWidget, public Ui::TableDataWidget {
	private:
		Q_OBJECT

		QMenu col_names_menu;

		void populateDataGrid(void);
		void configureColumnNamesMenu(void);
		void toggleWarningFrame(void);
		void setItemInvalid(QTableWidgetItem *item);

		QString generateDataBuffer(void);

	public:
		TableDataWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, Table *table);

	public slots:
		void applyConfiguration(void);

	private slots:
		void insertRowOnTabPress(int curr_row, int curr_col, int prev_row, int prev_col);
		void addRow(void);
		void addColumn(QAction *action);
		void duplicateRows(void);
		void deleteRows(void);
		void deleteColumns(void);
		void clearRows(bool confirm=true);
		void fixInvalidColumn(int col_idx);
		void enableButtons(void);
};

#endif
