/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class TypeArgument
\brief Implements the operations to manipulate user defined types arguments.
*/

#ifndef TYPE_ARGUMENT_H
#define TYPE_ARGUMENT_H

#include "baseobject.h"

class TypeAttribute: public BaseObject {
	private:
		PgSQLType type;

		QString getCodeDefinition(unsigned, bool){ return(""); }

	public:
		TypeAttribute(void);

		void setType(PgSQLType type);
		PgSQLType getType(void);

		//! \brief Returns the SQL / XML code definition for the parameter
		virtual QString getCodeDefinition(unsigned def_type) final;
		void operator = (const TypeAttribute &tpattrib);
};

#endif
