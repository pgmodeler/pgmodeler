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

#include "typeattribute.h"

TypeAttribute::TypeAttribute()
{
	obj_type=ObjectType::TypeAttribute;
}

void TypeAttribute::setType(PgSqlType type)
{
	setCodeInvalidated(this->type != type);
	this->type=type;
}

PgSqlType TypeAttribute::getType()
{
	return type;
}

QString TypeAttribute::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	if(def_type==SchemaParser::SqlDefinition)
		attributes[Attributes::Name]=BaseObject::formatName(obj_name);
	else
		attributes[Attributes::Name]=obj_name;

	attributes[Attributes::Type]=type.getCodeDefinition(def_type);

	if(collation)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Collation]=collation->getName(true);
		else
			attributes[Attributes::Collation]=collation->getCodeDefinition(def_type, true);
	}

	return BaseObject::__getCodeDefinition(def_type);
}

void TypeAttribute::operator = (const TypeAttribute &tpattrib)
{
	this->obj_name=tpattrib.obj_name;
	this->type=tpattrib.type;
	this->collation=tpattrib.collation;
}
