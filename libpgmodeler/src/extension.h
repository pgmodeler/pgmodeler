/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
		static constexpr unsigned CurVersion=0,
		OldVersion=1;
		Extension();

		void setName(const QString &name);
		void setSchema(BaseObject *schema);

		/*! \brief Defines if the extension handles a datatype. When setting to true
		the extension will be registered as a datatype on DatabaseModel class. This method has no effect when
		the extension was already inserted on the model. */
		void setHandlesType(bool value);

		//! \brief Set the versions of the extension
		void setVersion(unsigned ver, const QString &value);

		//! \brief Returns if the extension handles a datatype
		bool handlesType();

		//! \brief Returns on of the versions of the extension
		QString getVersion(unsigned ver);

		//! \brief Returns the SQL / XML code definition for the extension
		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getAlterDefinition(BaseObject *object) final;

		virtual QString getDropDefinition(bool cascade) final;

		virtual QString getSignature(bool format = true) final;

		virtual QString getName(bool format = false, bool = false) final;

		void operator = (Extension &ext);
};

#endif
