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

const QString ForeignDataWrapper::OptionsSeparator = QString(",");
const QString ForeignDataWrapper::OptionValueSeparator = QString("•");

ForeignDataWrapper::ForeignDataWrapper(void)
{
	obj_type=ObjectType::ForeignDataWrapper;
	validator_func = handler_func = nullptr;

	attributes[Attributes::HandlerFunc] = QString();
	attributes[Attributes::ValidatorFunc] = QString();
	attributes[Attributes::Options] = QString();
}

void ForeignDataWrapper::setHandlerFunction(Function *func)
{
	if(func)
	{
		if(func->getReturnType() != PgSqlType("fdw_handler"))
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

		if(!func->getParameter(0).getType().isExactTo(PgSqlType("text", 1)) ||
			 !func->getParameter(1).getType().isExactTo(PgSqlType("oid")))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ErrorCode::AsgFunctionInvalidParameters, __PRETTY_FUNCTION__, __FILE__, __LINE__);
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

void ForeignDataWrapper::setOption(const QString &opt, const QString &value)
{
	if(opt.isEmpty())
		throw Exception(ErrorCode::AsgOptionInvalidName,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	options[opt] = value;
}

void ForeignDataWrapper::setOptions(const attribs_map &options)
{
	for(auto &itr : options)
	{
		if(itr.first.isEmpty())
			throw Exception(ErrorCode::AsgOptionInvalidName,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	this->options = options;
}

void ForeignDataWrapper::removeOption(const QString &opt)
{
	options.erase(opt);
}

void ForeignDataWrapper::removeOptions(void)
{
	options.clear();
}

attribs_map ForeignDataWrapper::getOptions(void)
{
	return(options);
}

QString ForeignDataWrapper::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	QStringList fmt_options;
	bool is_sql_def = (def_type == SchemaParser::SqlDefinition);

	if(handler_func)
	{
		handler_func->setAttribute(Attributes::RefType, Attributes::HandlerFunc);
		attributes[Attributes::HandlerFunc] = is_sql_def ? handler_func->getSignature() : handler_func->getCodeDefinition(def_type, true);
	}

	if(validator_func)
	{
		validator_func->setAttribute(Attributes::RefType, Attributes::ValidatorFunc);
		attributes[Attributes::ValidatorFunc] = is_sql_def ? validator_func->getSignature() : validator_func->getCodeDefinition(def_type, true);
	}

	for(auto &itr : options)
		fmt_options += is_sql_def ? QString("%1 '%2'").arg(itr.first).arg(itr.second) :
																QString("%1%2%3").arg(itr.first).arg(OptionValueSeparator).arg(itr.second);

	attributes[Attributes::Options] = fmt_options.join(OptionsSeparator);

	return(this->BaseObject::__getCodeDefinition(def_type));
}
