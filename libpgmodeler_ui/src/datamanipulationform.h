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
\class DataGridForm
\brief Implements the operations to handle table's data
*/

#ifndef DATA_MANIPULATION_FORM_H
#define DATA_MANIPULATION_FORM_H

#include "ui_datamanipulationform.h"
#include "catalog.h"
#include "syntaxhighlighter.h"

class DataManipulationForm: public QDialog, public Ui::DataManipulationForm {
	private:
		Q_OBJECT

		SyntaxHighlighter *filter_hl;

		Connection connection;

		Catalog catalog;

		void listObjects(QComboBox *combo, vector<ObjectType> obj_types, const QString &schema="");
		QStringList retrievePKColumns(const QString &schema, const QString &table);

	public:
		DataManipulationForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

		void setAttributes(Connection conn, const QString curr_schema="public", const QString curr_table="");

	public slots:

	private slots:
		void listTables(void);
		void listColumns(void);
		void retrieveData(void);
		void disableControlButtons(void);
};

#endif
