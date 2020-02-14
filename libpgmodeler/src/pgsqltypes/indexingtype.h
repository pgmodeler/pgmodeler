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
\class IndexingType
\brief Implements the operations to manipulate index access methods (indexing types).
**/

#ifndef INDEXING_TYPE
#define INDEXING_TYPE

#include "basetype.h"

class IndexingType: public BaseType{
	private:
		static constexpr unsigned Offset=21;
		static constexpr unsigned TypesCount=6;

	public:
		static constexpr unsigned Btree=Offset;
		static constexpr unsigned Gist=Offset+1;
		static constexpr unsigned Hash=Offset+2;
		static constexpr unsigned Gin=Offset+3;
		static constexpr unsigned Spgist=Offset+4;
		static constexpr unsigned Brin=Offset+5;

		IndexingType(const QString &type_name);
		IndexingType(unsigned type_id);
		IndexingType();

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		virtual QString operator ~();
};

#endif 
