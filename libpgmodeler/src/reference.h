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
\ingroup libpgmodeler
\class Reference
\brief This auxiliary class is used to define SQL/XML for views
\note <strong>Creation date:</strong> 05/09/2008
*/

#ifndef REFERENCE_H
#define REFERENCE_H

#include "table.h"
#include "schema.h"

class Reference {
	private:
		//! \brief Stores the table used by the reference
		Table *table;

		//! \brief Stores the column used by the reference
		Column *column;

		//! \brief Stores the expression that defines one reference
		QString expression,
		//! \brief Stores the alias to the expression or table
		alias,
		//! \brief Stores only the alias for the column
		column_alias;

		//! \brief Indicates if the expression is used as entire view definition
		bool is_def_expr;

	public:
		//! \brief Constants used to define the reference type
		static const unsigned REFER_COLUMN=0, //! \brief The reference is based on a table column
													REFER_EXPRESSION=1; //! \brief The reference is based on an expression

		//! \brief Constants used on the view code generation
		static const unsigned SQL_REFER_WHERE=10,
													SQL_REFER_SELECT=20,
													SQL_REFER_FROM=30,
													SQL_VIEW_DEFINITION=40;

		Reference(void);

		//! \brief Creates a reference based on a table column
		Reference(Table *table, Column *column, const QString &tab_alias, const QString &col_alias);

		//! \brief Creates a reference based on a expression
		Reference(const QString &expression, const QString &expr_alias);

		/*! \brief Changes the behavior of the expression. Calling this method will cause the
		reference to be used as the entire view SQL definition */
		void setDefinitionExpression(bool value);

		//! \brief Gets the referenced table
		Table *getTable(void);

		//! \brief Gets the referenced column
		Column *getColumn(void);

		//! \brief Returns the alias for the referenced column
		QString getColumnAlias(void);

		//! \brief Returs the reference for the table or expression
		QString getAlias(void);

		//! \brief Returns the expression for the reference
		QString getExpression(void);

		//! \brief Returns the reference typ (see REFER_??? constants)
		unsigned getReferenceType(void);

		//! \brief Returns the SQL code definition
		QString getSQLDefinition(unsigned sql_type);

		//! \brief Returns the XML code definition
		QString getXMLDefinition(void);

		//! \brief Indicates if the reference when used as expression defines the entire view (raw sql definition)
		bool isDefinitionExpression(void);

		/*! \brief Compare the attributes of two references returning true
		 when they have the same values */
		bool operator == (Reference &refer);
};

#endif
