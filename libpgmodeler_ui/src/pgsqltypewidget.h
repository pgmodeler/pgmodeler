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
\class PgSQLTypeWidget
\brief Implements the operations to configure PostgreSQL types via form.
*/

#ifndef PGSQL_TYPE_WIDGET_H
#define PGSQL_TYPE_WIDGET_H

#include <QtWidgets>
#include "ui_pgsqltypewidget.h"
#include "pgsqltypes.h"
#include "syntaxhighlighter.h"
#include "databasemodel.h"

class PgSQLTypeWidget: public QWidget, public Ui::PgSQLTypeWidget {
	private:
		Q_OBJECT

		//! \brief Stores the PostgreSQL type configured on the form
		PgSQLType type;

		//! \brief Syntax highlighter used on the format field
		SyntaxHighlighter *format_hl;

	public:
		PgSQLTypeWidget(QWidget * parent = 0, const QString &label="");

		/*! \brief Lists the PostgreSQL types on the specified combo. The user can configure which types
		must be shown using the last tree parameters. The DatabaseModel parameter is used to
		gather the user-defined types of the specified model. */
		static void listPgSQLTypes(QComboBox *combo, DatabaseModel *model,
															 unsigned user_type_conf=UserTypeConfig::ALL_USER_TYPES,
															 bool oid_types=true, bool pseudo_types=true);

	private slots:
		void updateTypeFormat(void);

	public slots:
		void setAttributes(PgSQLType type, DatabaseModel *model,
											 unsigned usr_type_conf=UserTypeConfig::ALL_USER_TYPES,
											 bool oid_types=true, bool pseudo_types=true);

		//! \brief Returns the PostgreSQL type configured via form
		PgSQLType getPgSQLType(void);
};

#endif
