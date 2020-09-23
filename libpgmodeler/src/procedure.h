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
\ingroup libpgmodeler
\brief Implements the operations to manipulate procedures on the database.
*/

#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "basefunction.h"

class Procedure: public BaseFunction {
	public:
		Procedure();

		virtual void addParameter(Parameter param);
		virtual QString getCodeDefinition(unsigned def_type, bool) final;
		virtual QString getCodeDefinition(unsigned def_type) final;
		virtual QString getAlterDefinition(BaseObject *object) final;
};

#endif
