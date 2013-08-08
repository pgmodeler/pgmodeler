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

		DatabaseImportHelper dbimport_helper;

		void showEvent(QShowEvent *);

	public:
		DatabaseImportForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

	private slots:
		void importDatabase(void);
		void listObjects(void);
		void listDatabases(void);
		void hideProgress(void);
		void updateProgress(int progress, QString msg);
};

#endif
