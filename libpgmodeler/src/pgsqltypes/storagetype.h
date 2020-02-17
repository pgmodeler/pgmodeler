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
\ingroup libpgmodeler/pgsqltypes
\class StorageType
\brief Implements the operations to manipulate base type storage modes.
**/

#ifndef STORAGE_TYPE
#define STORAGE_TYPE

#include "templatetype.h"

class StorageType: public TemplateType<StorageType>{
	public:
		static constexpr unsigned Plain = 1,
		External = 2,
		Extended = 3,
		Main = 4;

		StorageType();
		StorageType(const QString &type_name);
		StorageType(unsigned type_id);

		bool operator == (const QString &type_name);
		bool operator == (const char *type_name);
		bool operator != (const QString &type_name);
		bool operator != (StorageType type);
};


#endif 
