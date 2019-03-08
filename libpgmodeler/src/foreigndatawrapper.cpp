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

#include "foreigndatawrapper.h"

ForeignDataWrapper::ForeignDataWrapper(void)
{
	obj_type=ObjectType::ForeignDataWrapper;
	validator_func = handler_func = nullptr;
}

void ForeignDataWrapper::setHandlerFunction(Function *func)
{
	if(func)
	{
		if(func->getReturnType() != PgSqlType("fwd_handler"))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ErrorCode::AsgFunctionInvalidReturnType, __PRETTY_FUNCTION__, __FILE__, __LINE__);

		if(func->getParameterCount() != 0)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ErrorCode::AsgFunctionInvalidParamCount, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	handler_func = func;
}

void ForeignDataWrapper::setValidatorFunction(Function *func)
{
	if(func)
	{
		if(func->getParameterCount() != 2)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ErrorCode::AsgFunctionInvalidParamCount, __PRETTY_FUNCTION__, __FILE__, __LINE__);

		if(func->getParameter(0).getType() != PgSqlType("text", 1) ||
			 func->getParameter(1).getType() != PgSqlType("oid", 1))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ErrorCode::AsgFunctionInvalidParamCount, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	}

	validator_func = func;
}

Function *ForeignDataWrapper::getHandlerFunction(void)
{
	return(handler_func);
}

Function *ForeignDataWrapper::getValidatorFunction(void)
{
	return(validator_func);
}

void ForeignDataWrapper::setOptions(const attribs_map &options)
{
	this->options = options;
}

attribs_map ForeignDataWrapper::getOptions(void)
{
	return(options);
}

QString ForeignDataWrapper::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	return(this->BaseObject::__getCodeDefinition(def_type));
}
