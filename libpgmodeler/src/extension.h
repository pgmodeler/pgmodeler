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
\class Extension
\brief Implements the operations to manipulate extensions on the database.
**/

#ifndef EXTENSION_H
#define EXTENSION_H

#include "baseobject.h"

class Extension: public BaseObject {
	private:
		/*! \brief Indicates if the extension handles a datatype. When
		this attribute is set pgModeler will consider the extension as
		a data type and will register it on PgSqlType class. */
		bool handles_type;

		/*! \brief Versions of the installed extension. The first one is the
		current version and the last is the old version from which the extension
		is being updated */
		QString versions[2];

	public:
		static const unsigned CUR_VERSION=0,
													OLD_VERSION=1;
		Extension(void);
		~Extension(void);

		void setName(const QString &name);
		void setSchema(BaseObject *schema);

		/*! \brief Defines if the extension handles a datatype. When setting to true
		the extension will be registered as a datatype on DatabaseModel class. It's not
		recommended to call this method if the extension already handles a type since can cause
		reference breaking on any objects that uses this extension as data type */
		void setHandlesType(bool value);

		//! \brief Set the versions of the extension
		void setVersion(unsigned ver, const QString &value);

		//! \brief Returns if the extension handles a datatype
		bool handlesType(void);

		//! \brief Returns on of the versions of the extension
		QString getVersion(unsigned ver);

		//! \brief Returns the SQL / XML code definition for the extension
		QString getCodeDefinition(unsigned def_type);

		void operator = (Extension &ext);
};

#endif
