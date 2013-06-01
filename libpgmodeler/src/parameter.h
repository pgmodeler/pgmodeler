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
\brief Implements the operations to manipulate stored procedures (function) parameters.
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include "column.h"

class Parameter: public Column {
	private:
		/*! \brief This attributes indicates whether the parameter
		is in, out or both (IN, OUT, INOUT) */
		bool is_in, is_out;

	public:
		Parameter(void);

		void setType(PgSQLType type);

		void setIn(bool value);
		void setOut(bool value);

		bool isIn(void);
		bool isOut(void);

		//! \brief Returns the SQL / XML code definition for the parameter
		virtual QString getCodeDefinition(unsigned def_type) final;
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;

		void operator = (const Parameter &param);
};

#endif
