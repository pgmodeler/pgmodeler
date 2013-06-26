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
\class Column
\brief Implements basic operations to manipulate table columns.
\note <strong>Creation date:</strong> 12/09/2006
*/

#ifndef COLUMN_H
#define COLUMN_H

#include "tableobject.h"

class Column: public TableObject{
	protected:
		/*! \brief Stores the previous name of the column before its name has changed.
		 This attribute assists in the process of reference columns added
		 by relationships. */
		QString old_name;

		//! \brief Indicate that the column accpets null values or not
		bool not_null;

		//! \brief Data type of the column
		PgSQLType type;

		/*! \brief Default value of the column.
		 Note: The user must format the default value in
					 accordance with the requirements for each data type.
					 E.g.: for a varchar(10) default value should be 'abcdef' (including apostrophe)
					 for a date the defaul value should be '2006-09-12 ' and so on. */
		QString default_value;

		//! \brief Stores a reference to the relationship that generates the column
		BaseObject *parent_rel;

	public:
		Column(void);

		//! \brief Defines if the column accepts null values or not
		void setNotNull(bool value);

		//! \brief Defines the column data type
		void setType(PgSQLType type);

		/*! \brief Sets the default value of the column. Must be informed together with the value
		 the particularities of each type, such as quotation marks, hyphens, etc. */
		void setDefaultValue(const QString &value);

		/*! \brief Defines the column name. This method keeps the last column name
		 stored to assist the objects like constraints / indixes and sequences
		 that is referencing the column by its old name. */
		void setName(const QString &name);

		//! \brief Returns the not null state of the column
		bool isNotNull(void);

		//! \brief Returns the data type of the column
		PgSQLType getType(void);

		//! \brief Returns the default value of the column
		QString getDefaultValue(void);

		//! \brief Returns the SQL/XML code definition for the column
		virtual QString getCodeDefinition(unsigned def_type);

		/*! \brief Returns the old column name. The parameter 'format' indicates
		 whether the name must be formatted or not */
		QString getOldName(bool format=false);

		//! \brief Returns the reference to the column type in the form [schema].table.column_name%TYPE
		QString getTypeReference(void);

		void setParentRelationship(BaseObject *parent_rel);

		BaseObject *getParentRelationship(void);

		//! \brief Copies on column to other
		void operator = (Column &col);
};

#endif
