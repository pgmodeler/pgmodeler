/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libcore/pgsqltypes
\class CategoryType
\brief Implements the operations to manipulate base types categories.
**/

#ifndef CATEGORY_TYPE
#define CATEGORY_TYPE

#include "templatetype.h"

class __libcore CategoryType: public TemplateType<CategoryType>{
	private:
		inline static const QStringList type_names {
			"", // Reserved for null value

			//See table 44-43 on PostgreSQL documentation
			"U", //User-defined types
			"A", //Array types
			"B", //Boolean types
			"C", //Composite types
			"D", //Date/time types
			"E", //Enum types
			"G", //Geometric types
			"I", //Network address types
			"N", //Numeric types
			"P", //Pseudo-types
			"S", //String types
			"T", //Timespan types
			"V", //Bit-string types
			"X", //Unknown type
		};

	public:
		static constexpr unsigned UserDefined = 1,
		Array = 2,
		Boolean = 3,
		Composite = 4,
		DateTime = 5,
		Enumeration = 6,
		Geometric = 7,
		NetworkAddr = 8,
		Numeric = 9,
		PseudoTypes = 10,
		Stringt = 11,
		Timespan = 12,
		BitString = 13,
		Unknown = 14;

		CategoryType(unsigned type_id);
		CategoryType(const QString &type_name);
		CategoryType();

		static QStringList getTypes();

		unsigned setType(unsigned type_id) override;
		unsigned setType(const QString &type_name) override;
		QString getTypeName(unsigned type_id) override;
};

#endif
