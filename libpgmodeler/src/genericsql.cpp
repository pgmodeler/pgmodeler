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

#include "genericsql.h"

GenericSQL::GenericSQL(void)
{
	obj_type=ObjectType::GenericSql;
	attributes[Attributes::Definition] = QString();
}

void GenericSQL::setDefinition(const QString &def)
{
	setCodeInvalidated(definition != def);
	definition = def;
}

QString GenericSQL::getDefinition(void)
{
	return(definition);
}

QString GenericSQL::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[Attributes::Definition] = definition;
	return(this->BaseObject::__getCodeDefinition(def_type));
}

void GenericSQL::operator = (GenericSQL &genericsql)
{
	(*dynamic_cast<BaseObject *>(this))=reinterpret_cast<BaseObject &>(genericsql);
}
