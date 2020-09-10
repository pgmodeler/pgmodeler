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
\class Transform
\brief Implements the operations to manipulate transform object on the database
*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "baseobject.h"
#include "function.h"
#include "language.h"

class Transform : public BaseObject {
	private:
		PgSqlType type;

		Language *language;

		Function *functions[2];

		void validateFunction(Function *func, unsigned func_id);

	public:
		static constexpr unsigned
		FromSqlFunc = 0,
		ToSqlFunc = 1;

		Transform();

		//! \brief This method has a hardcoded way to generated the transform's name. It'll reject any value passed by its parameter
		virtual void setName(const QString &);

		void setType(PgSqlType tp);
		void setLanguage(Language *lang);
		void setFunction(Function *func, unsigned func_id);

		PgSqlType getType();
		Language *getLanguage();		
		Function *getFunction(unsigned func_id);

		void operator = (Transform &transf);
		virtual QString getCodeDefinition(unsigned def_type) final;
		virtual QString getSignature(bool = false) final;
		virtual QString getDropDefinition(bool cascade) final;
};

#endif
