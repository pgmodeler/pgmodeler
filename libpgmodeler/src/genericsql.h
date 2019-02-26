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
\ingroup libpgmodeler
\class GenericSQL
\brief This class is used to represent generic sql commands as database objects
*/

#ifndef GENERIC_SQL_H
#define GENERIC_SQL_H

#include "baseobject.h"

class GenericSQL: public BaseObject{
	private:
		QString definition;

	public:
		GenericSQL(void);

		void setDefinition(const QString &def);
		QString getDefinition(void);

		//! \brief Copies the attributes between generic sql objects
		void operator = (GenericSQL &genericsql);

		virtual QString getCodeDefinition(unsigned def_type);
};

#endif
