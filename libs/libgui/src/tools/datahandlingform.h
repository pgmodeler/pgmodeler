/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class DataHandlingForm
\brief Implements the operations to handle several tables' data in a tabbed way
*/

#ifndef DATA_HANDLING_FORM_H
#define DATA_HANDLING_FORM_H

#include "ui_datahandlingform.h"
#include "guiglobal.h"
#include "catalog.h"
#include "baseobject.h"

class DataGridWidget;

class __libgui DataHandlingForm: public QDialog, public Ui::DataHandlingForm {
	private:
		Q_OBJECT
		
		DataGridWidget *curr_grid_wgt;

		//! \brief Store the template connection params to be used by catalogs and command execution connections
		attribs_map tmpl_conn_params;

		QString connection_id;

		Catalog catalog;

		void resizeEvent(QResizeEvent *event);

		void closeEvent(QCloseEvent *event);

		bool eventFilter(QObject *object, QEvent *event);

		//! \brief Shows a confirmation message before closing the form when there are pending operations in the grid
		int confirmFormClose();

		/*! \brief Shows a confirmation message before closing the tab identified by its index.
		 *  This method returns a flag indication the confirmation message display (in case the data grid is modified)
		 *  and the confirmation result */
		std::pair<bool, int> confirmDataGridClose(int idx);

		//! \brief Fills a combobox with the names of objects retrieved from catalog
		void listObjects(QComboBox *combo, std::vector<ObjectType> obj_types, const QString &schema = "");

		void closeDataGrid(int idx, bool confirm_close);

	public:
		DataHandlingForm(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::Widget);

		virtual ~DataHandlingForm();
		
		//! \brief Defines the connection and current schema and table to be handled, this method should be called before show the dialog
		void setAttributes(const attribs_map &conn_params, const QString curr_schema="public", const QString curr_table_name="", const QString &filter="");

	private slots:
		void setCurrentDataGrid(int tab_idx);

		void reject();

		void addDataGrid(const QString &schema, const QString &table, const QString &filter, ObjectType obj_type);

		//! \brief List the tables based upon the current schema
		void listTables();
		
		//! \brief Disable the buttons used to handle data
		void enableRefreshButton();

		//! \brief Opens a new data manipulation windows
		void openNewWindow();

		void setDataGridModified(bool changed);
};

#endif
