/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
  is_in=is_out=is_variadic=false;
}

void Parameter::setType(PgSQLType type)
{
	if(!type.isArrayType() && is_variadic)
		throw Exception(ERR_INV_USE_VARIADIC_PARAM_MODE ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->type != type);
	this->type=type;
}

void Parameter::setIn(bool value)
{
	setCodeInvalidated(is_in != value);
	is_in=value;
	is_variadic=false;
}

void Parameter::setOut(bool value)
{
	setCodeInvalidated(is_out != value);
	is_out=value;
	is_variadic=false;
}

void Parameter::setVariadic(bool value)
{
	if(value && !type.isArrayType())
		throw Exception(ERR_INV_USE_VARIADIC_PARAM_MODE ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(is_variadic != value);
	is_variadic=value;
	if(value)	is_in=is_out=false;
}

bool Parameter::isIn(void)
{
	return(is_in);
}

bool Parameter::isOut(void)
{
	return(is_out);
}

bool Parameter::isVariadic(void)
{
	return(is_variadic);
}

void Parameter::operator = (const Parameter &param)
{
	this->obj_name=param.obj_name;
	this->type=param.type;
	this->default_value=param.default_value;
	this->is_in=param.is_in;
	this->is_out=param.is_out;
	this->is_variadic=param.is_variadic;
	setCodeInvalidated(true);
}

QString Parameter::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	return(this->getCodeDefinition(def_type, false));
}

QString Parameter::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::NAME]=BaseObject::formatName(obj_name);
	else
		attributes[ParsersAttributes::NAME]=obj_name;

	attributes[ParsersAttributes::PARAM_IN]=(is_in ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::PARAM_OUT]=(is_out ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::PARAM_VARIADIC]=(is_variadic ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
	attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}
