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
\class PgSqlType
\brief Implements the operations to manipulate policy command types.
**/

#ifndef POLICY_CMD_TYPE
#define POLICY_CMD_TYPE

#include "basetype.h"

class PolicyCmdType: public BaseType {
	private:
		static constexpr unsigned Offset=247;
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned All=Offset;
		static constexpr unsigned Select=Offset+1;
		static constexpr unsigned Insert=Offset+2;
		static constexpr unsigned Update=Offset+3;
		static constexpr unsigned Delete=Offset+4;

		PolicyCmdType(const QString &type_name);
		PolicyCmdType(unsigned type_id);
		PolicyCmdType();

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		virtual QString operator ~();
};

#endif 
