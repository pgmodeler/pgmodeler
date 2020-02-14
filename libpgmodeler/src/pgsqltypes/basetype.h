/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libpgmodeler/pgsqltypes
\class BaseType
\brief Implements the basic operations to manipulate PostgreSQL types based upon reserved keywords.
**/

#ifndef BASE_TYPE
#define BASE_TYPE

#include "exception.h"
#include "attributes.h"
#include "schemaparser.h"
#include <vector>
#include <QRegExp>

class BaseType{
	protected:
		static constexpr unsigned TypesCount=262;
		static QString type_list[TypesCount];

		//! \brief Index of the type on the type_list vector
		unsigned type_idx;

		/*! \brief Sets an id to the type according to the limit stablished by the attribute
		 offset and type_count from each class */
		void setType(unsigned type_id, unsigned offset, unsigned count);

		//! \brief Checks if the type id is valid according to the offset/count for the class
		bool isTypeValid(unsigned type_id, unsigned offset, unsigned count);

		//! \brief Returns the string list for all types on the specified interval (offset-count)
		static void getTypes(QStringList &types, unsigned offset, unsigned count);

		//! \brief Returns the type id searching by its name. Returns BaseType::null when not found
		static unsigned getType(const QString &type_name, unsigned offset, unsigned count);

	public:
		static constexpr unsigned Null=0;

		BaseType();

		//! \brief Returns the name of the type
		QString operator ~ ();

		//! \brief Returns the code (id) of the type
		unsigned operator ! ();

		//! \brief Returns the code (id) of the type
		unsigned getTypeId();

		//! \brief Returns the name of the type
		QString getTypeName();

		bool operator == (BaseType &type);
		bool operator == (unsigned type_id);
		bool operator != (BaseType &type);
		bool operator != (unsigned type_id);

		static QString getTypeString(unsigned type_id);
};

#endif
