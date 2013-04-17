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

#include "parameter.h"

Parameter::Parameter(void)
{
	obj_type=OBJ_PARAMETER;
	is_in=is_out=false;
}

void Parameter::setType(PgSQLType type)
{
	this->type=type;
}

void Parameter::setIn(bool value)
{
	is_in=value;
}

void Parameter::setOut(bool value)
{
	is_out=value;
}

bool Parameter::isIn(void)
{
	return(is_in);
}

bool Parameter::isOut(void)
{
	return(is_out);
}

void Parameter::operator = (const Parameter &param)
{
	this->obj_name=param.obj_name;
	this->type=param.type;
	this->default_value=param.default_value;
	this->is_in=param.is_in;
	this->is_out=param.is_out;
}

QString Parameter::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString Parameter::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::NAME]=BaseObject::formatName(obj_name);
	else
		attributes[ParsersAttributes::NAME]=obj_name;

	attributes[ParsersAttributes::PARAM_IN]=(is_in ? "1" : "");
	attributes[ParsersAttributes::PARAM_OUT]=(is_out ? "1" : "");
	attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
	attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}
