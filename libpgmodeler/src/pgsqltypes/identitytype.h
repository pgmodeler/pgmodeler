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
\class IdentityType
\brief Implements the operations to manipulate identity columns behavior.
**/

#ifndef IDENTITY_TYPE
#define IDENTITY_TYPE

#include "templatetype.h"

class IdentityType: public TemplateType<IdentityType>{
	public:
		static constexpr unsigned Always = 1,
		ByDefault = 2;

		IdentityType(const QString &type_name);
		IdentityType(unsigned type_id);
		IdentityType();
};

#endif 
