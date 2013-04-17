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

#include "typeattribute.h"

TypeAttribute::TypeAttribute(void)
{
	obj_type=OBJ_TYPE_ATTRIBUTE;
}

void TypeAttribute::setType(PgSQLType type)
{
	this->type=type;
}

PgSQLType TypeAttribute::getType(void)
{
	return(type);
}

QString TypeAttribute::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::NAME]=BaseObject::formatName(obj_name);
	else
		attributes[ParsersAttributes::NAME]=obj_name;

	attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);

	if(collation)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::COLLATION]=collation->getName(true);
		else
			attributes[ParsersAttributes::COLLATION]=collation->getCodeDefinition(def_type, true);
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

void TypeAttribute::operator = (const TypeAttribute &tpattrib)
{
	this->obj_name=tpattrib.obj_name;
	this->type=tpattrib.type;
	this->collation=tpattrib.collation;
}
