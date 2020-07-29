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

	try
	{
		if(functions[ToSqlFunc])
			validateFunction(functions[ToSqlFunc], ToSqlFunc);

		setCodeInvalidated(type != tp);
		type = tp;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void Transform::setLanguage(Language *lang)
{
	if(!lang)
		throw Exception(ErrorCode::AsgNotAllocatedLanguage, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	setCodeInvalidated(language != lang);
	language = lang;
}

void Transform::setFunction(Function *func, unsigned func_id)
{
	try
	{
		validateFunction(func, func_id);
		setCodeInvalidated(true);
		functions[func_id] = func;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void Transform::validateFunction(Function *func, unsigned func_id)
{
	if(func_id > ToSqlFunc)
		throw Exception(ErrorCode::RefFunctionInvalidType, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	// Unallocated function is prohibited
	if(!func)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	// The function to be assigned must have only one parameter
	if(func->getParameterCount() != 1)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	// The function must have a single parameter of the type "internal"
	if(func->getParameter(0).getType() != "internal")
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	/* ToSqlFunc must return a type that is equivalent to the type being handled by the transform
	 * FromSqlFunc must always return "internal" data type */
	if((func_id == ToSqlFunc && !func->getReturnType().isEquivalentTo(type)) ||
		 (func_id == FromSqlFunc && func->getReturnType() != "internal"))
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
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
