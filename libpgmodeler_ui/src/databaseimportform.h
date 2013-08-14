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
\class DatabaseImportForm
\brief Implements the form to execute the reverse engineering operations
*/

#ifndef DATABASE_IMPORT_FORM_H
#define DATABASE_IMPORT_FORM_H

#include "ui_databaseimportform.h"
#include "databaseimporthelper.h"

class DatabaseImportForm: public QDialog, public Ui::DatabaseImportForm {
	private:
		Q_OBJECT

		//! \brief Database importer helper
		DatabaseImportHelper dbimport_helper;

		//! \brief This attribute controls the general import progress
		int progress;

		/*! \brief Retrieve the specified objects from the database and insert them onto the tree view.
		The "root" parameter is used to associate the group of objects as child of it.
		The "schema" and "table" parameter are used to filter objects by schema and/or table.
		This method automatically returns a list of QTreeWidgetItem when the vector "types" contains OBJ_SCHEMA or OBJ_TABLE */
		vector<QTreeWidgetItem *> updateObjectsTree(vector<ObjectType> types, QTreeWidgetItem *root=nullptr,
																								const QString &schema="", const QString &table="");

		/*! \brief Toggles the checked state for the specified item. This method recursively
		changes the check state for the children items */
		void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState chk_state);

		//! \brief Checks the item's parent when its checked
		void setParentItemChecked(QTreeWidgetItem *item);

		//! \brief Returns true when there is at least one item checked on the objects tree
		bool hasCheckedItems(void);

		/*! \brief Returns the checked items oids on "obj_oids" vector. The second parameter
		"col_oids" stores the columns oids for each selected table */
		void getCheckedItems(vector<unsigned> &obj_oids, map<unsigned, vector<unsigned>> &col_oids);

		void showEvent(QShowEvent *);

	public:
		DatabaseImportForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

	private slots:
		void importDatabase(void);
		void listObjects(void);
		void listDatabases(void);
		void hideProgress(void);
		void updateProgress(int progress, QString msg);

		//! \brief Toggles the check state for the specified item
		void setItemCheckState(QTreeWidgetItem *item,int);

		//! \brief Toggles the check state for all items
		void setItemsCheckState(void);

	signals:
		//! \brief This signal is emitted when a object is retrieved from database
		void s_objectsRetrieved(int prog,QString msg,unsigned obj_id);
};

#endif
