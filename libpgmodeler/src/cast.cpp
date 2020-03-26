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

#include "cast.h"

Cast::Cast()
{
	obj_type=ObjectType::Cast;
	cast_function=nullptr;
	cast_type=Explicit;
	is_in_out=false;
	attributes[Attributes::SourceType]=QString();
	attributes[Attributes::DestType]=QString();
	attributes[Attributes::CastType]=QString();
	attributes[Attributes::IoCast]=QString();
	attributes[Attributes::Function]=QString();
}

void Cast::setDataType(unsigned type_idx, PgSqlType type)
{
	//Check if the type index is valid
	if(type_idx<=DstType)
	{
		//Raises an error if the passed data type is null
		if((*type).isEmpty())
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgNullTypeObject)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Cast)),
							ErrorCode::AsgNullTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		setCodeInvalidated(this->types[type_idx] != type);
		this->types[type_idx]=type;
	}
	else
		//Raises an error if the type index is invalid
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Configures the cast name (in form of signature: cast(src_type, dst_type) )
	this->obj_name=QString("cast(%1,%2)").arg(~types[SrcType]).arg(~types[DstType]);
}

void Cast::setCastType(unsigned cast_type)
{
	//Raises an error if the user tries to assign an invalid cast type
	if(cast_type > Implicit)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->cast_type != cast_type);
	this->cast_type=cast_type;
}

void Cast::setInOut(bool value)
{
	setCodeInvalidated(is_in_out != value);
	is_in_out=value;
}

void Cast::setCastFunction(Function *cast_func)
{
	unsigned param_count;
	bool error=false;
	PgSqlType ret_type;

	if(!cast_func)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::Cast)),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Retrieve the cast function parameter count for specific validations
	param_count=cast_func->getParameterCount();

	//Raises an error if the function don't have at least 1 parameter or a maximum of 3
	if(param_count==0 || param_count > 3)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::Cast)),
						ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		/* Error condition 1: Check if the first function parameter data type differs
		 from cast source data type */
		error=(cast_func->getParameter(0).getType()!=this->types[SrcType] &&
					!cast_func->getParameter(0).getType().canCastTo(this->types[SrcType]));

		/* Error condition 2: Check if the second function parameter data type
		 is different from 'integer' */
		if(!error && param_count>=2)
			error=(cast_func->getParameter(1).getType()!=QString("integer"));

		/* Error condition 3: Check if the third function parameter data type is
		 different from 'boolean' */
		if(!error && param_count==3)
			error=(cast_func->getParameter(2).getType()!=QString("boolean"));

		//In case some error condition is reached raises an error
		if(error)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Cast)),
							ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	/* Raises an error if the return type of the function differs from the destination data type.
			If the types can be casted between them no error is returned */
	ret_type=cast_func->getReturnType();

	if(ret_type!=this->types[DstType] && !ret_type.canCastTo(this->types[DstType]))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::Cast)),
						ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(cast_function != cast_func);
	this->cast_function=cast_func;
}

PgSqlType Cast::getDataType(unsigned type_idx)
{
	if(type_idx > DstType)
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return this->types[type_idx];
}

bool Cast::isInOut()
{
	return is_in_out;
}

Function *Cast::getCastFunction()
{
	return cast_function;
}

unsigned Cast::getCastType()
{
	return cast_type;
}

QString Cast::getDropDefinition(bool cascade)
{
	attributes[Attributes::Signature].replace(QString(","), QString(" AS "));
	return BaseObject::getDropDefinition(cascade);
}

QString Cast::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	if(def_type==SchemaParser::SqlDefinition)
	{
		attributes[Attributes::SourceType]=(*types[SrcType]);
		attributes[Attributes::DestType]=(*types[DstType]);
	}
	else
	{
		attributes[Attributes::SourceType]=types[SrcType].getCodeDefinition(def_type);
		attributes[Attributes::DestType]=types[DstType].getCodeDefinition(def_type);
	}

	if(!is_in_out && cast_function)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Function]=cast_function->getSignature();
		else
			attributes[Attributes::Function]=cast_function->getCodeDefinition(def_type, true);
	}
	else
		attributes[Attributes::IoCast]=(is_in_out ? Attributes::True : QString());

	if(cast_type==Assignment)
		attributes[Attributes::CastType]=Attributes::Assignment;
	else if(cast_type==Implicit)
		attributes[Attributes::CastType]=Attributes::Implicit;
	else
		attributes[Attributes::CastType]=QString();

	if(def_type==SchemaParser::SqlDefinition)
		attributes[Attributes::CastType]=attributes[Attributes::CastType].toUpper();

	return BaseObject::__getCodeDefinition(def_type);
}

QString Cast::getSignature(bool)
{
	attributes[Attributes::Signature]=this->getName().remove(QString("cast"));
	return BaseObject::getSignature(false);
}

void Cast::configureSearchAttributes()
{
	QStringList arg_types;

	BaseObject::configureSearchAttributes();

	arg_types += *types[0];
	arg_types += *types[1];
	search_attribs[Attributes::Type] = arg_types.join("; ");
}
