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
\class ActionType
\brief Implements the operations to manipulate constraint actions.
**/

#ifndef ACTION_TYPE
#define ACTION_TYPE

#include "basetype.h"

class ActionType: public BaseType{
	private:
		//! \brief Initial position of the names related to the class on BaseType::type_list
		static constexpr unsigned Offset=1;

		//! \brief Type count for the class related to the list
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned NoAction=Offset;
		static constexpr unsigned Restrict=Offset+1;
		static constexpr unsigned Cascade=Offset+2;
		static constexpr unsigned SetNull=Offset+3;
		static constexpr unsigned SetDefault=Offset+4;

		ActionType(const QString &type_name);
		ActionType(unsigned type_id);
		ActionType();

		static void getTypes(QStringList &type_list);

		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif
