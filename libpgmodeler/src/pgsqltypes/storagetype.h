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
\class StorageType
\brief Implements the operations to manipulate base type storage modes.
**/

#ifndef STORAGE_TYPE
#define STORAGE_TYPE

#include "basetype.h"

class StorageType: public BaseType{
	private:
		static constexpr unsigned Offset=199;
		static constexpr unsigned TypesCount=4;

	public:
		static constexpr unsigned Plain=Offset;
		static constexpr unsigned External=Offset+1;
		static constexpr unsigned Extended=Offset+2;
		static constexpr unsigned Main=Offset+3;

		StorageType();
		StorageType(const QString &type_name);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (const QString &type_name);
		bool operator == (const char *type_name);
		bool operator != (const QString &type_name);
		bool operator != (StorageType type);
		virtual QString operator ~();
};


#endif 
