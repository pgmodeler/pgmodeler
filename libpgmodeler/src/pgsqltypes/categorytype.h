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
\class CategoryType
\brief Implements the operations to manipulate base types categories.
**/

#ifndef CATEGORY_TYPE
#define CATEGORY_TYPE

#include "basetype.h"

class CategoryType: public BaseType{
	private:
		static constexpr unsigned Offset=208;
		static constexpr unsigned TypesCount=14;

	public:
		static constexpr unsigned UserDefined=Offset;
		static constexpr unsigned Array=Offset+1;
		static constexpr unsigned Boolean=Offset+2;
		static constexpr unsigned Composite=Offset+3;
		static constexpr unsigned DateTime=Offset+4;
		static constexpr unsigned Enumeration=Offset+5;
		static constexpr unsigned Geometric=Offset+6;
		static constexpr unsigned NetworkAddr=Offset+7;
		static constexpr unsigned Numeric=Offset+8;
		static constexpr unsigned PseudoTypes=Offset+9;
		static constexpr unsigned Stringt=Offset+10;
		static constexpr unsigned Timespan=Offset+11;
		static constexpr unsigned BitString=Offset+12;
		static constexpr unsigned Unknown=Offset+13;

		CategoryType(unsigned type_id);
		CategoryType(const QString &type_name);
		CategoryType();

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif
