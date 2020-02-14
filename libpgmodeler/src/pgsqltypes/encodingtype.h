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
\class EncodingType
\brief Implements the operations to manipulate database enconding types.
**/

#ifndef ENCODING_TYPE
#define ENCODING_TYPE

#include "basetype.h"

class EncodingType: public BaseType{
	private:
		static QStringList type_names;

	public:
		EncodingType();
		EncodingType(const QString &type);
		EncodingType(const unsigned type_id);

		static QStringList getTypes();
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (const QString &type_name);
		bool operator == (const char *type_name);
		bool operator != (const QString &type_name);
		bool operator != (EncodingType type);
		bool operator != (unsigned tipo_id);
		virtual QString operator ~();
};

#endif 
