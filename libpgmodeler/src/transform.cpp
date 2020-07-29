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

#include "transform.h"

Transform::Transform()
{
	obj_type = ObjectType::Transform;
	language = nullptr;
	functions[FromSqlFunc] = nullptr;
	functions[ToSqlFunc] = nullptr;

	attributes[Attributes::Type] = "";
	attributes[Attributes::Language] = "";
	attributes[Attributes::ToSqlFunc] = "";
	attributes[Attributes::FromSqlFunc] = "";
}

void Transform::setType(PgSqlType tp)
{
	if(tp == PgSqlType::Null)
		throw Exception(ErrorCode::AsgInvalidTypeObject, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	setCodeInvalidated(type != tp);
	type = tp;
}

void Transform::setLanguage(Language *lang)
{

}

void Transform::setFunction(Function *func, unsigned func_id)
{

}

PgSqlType Transform::getType()
{

}

Language *Transform::getLanguage()
{

}

Function *Transform::getFunction(unsigned func_id)
{

}

QString Transform::getCodeDefinition(unsigned def_type)
{

}

QString Transform::getSignature(bool)
{

}

QString Transform::getDropDefinition(bool cascade)
{

}
