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
\class Collation
\brief Implements the operations to manipulate collations on the database.
**/

#ifndef COLLATION_H
#define COLLATION_H

#include "baseobject.h"
#include "pgsqltypes/encodingtype.h"

class Collation : public BaseObject {
	private:
		/*! \brief Base encoding for the collation.
		When setting the locale or lc_??? attributes the encoding name
		will be appended to the attributes. Example:

		encoding=UTF8
		locale=pt_BR
		LC_TYPE=pt_BR.utf8
		LC_CTYPE=pt_BR.utf8 */
		EncodingType encoding;

		//! \brief LC_CTYPE and LC_COLLATE attributes
		QString localization[2],

		/*! \brief This attribute sets at once the localization attribute. Using this attribute
						user cannot change localization attributes */
		locale;

	public:
		static constexpr unsigned LcCtype=0,
		LcCollate=1;

		Collation();

		/*! \brief Sets the collation locale and the base encoding. This method specifies at once the LC_CTYPE
		and LC_COLLATE attributes. When the user calls this method with 'locale' set the use of setLocalization()
		has no effect. To use custom localizations the user must reset 'locale' to a empty value. */
		void setLocale(const QString &locale);

		//! \brief Configures the LC_CTYPE and LC_COLLATE attributes and the default encoding for them.
		void setLocalization(unsigned lc_id, QString lc_name);

		/*! \brief Sets the collation from which this collation will copy attributes. The use of this method nullifies
		all the other collation's attributes */
		void setCollation(BaseObject *collation);

		//! \brief Defines the base encoding for the collation
		void setEncoding(EncodingType encoding);

		QString getLocale();
		QString getLocalization(unsigned lc_id);
		EncodingType getEncoding();

		//! \brief Returns the SQL / XML definition for the collation.
		virtual QString getCodeDefinition(unsigned def_type) final;
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;

		virtual QString getAlterDefinition(BaseObject *object);
};

#endif
