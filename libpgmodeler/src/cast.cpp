/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

Cast::Cast(void)
{
	obj_type=OBJ_CAST;
	cast_function=nullptr;
	cast_type=EXPLICIT;
	is_in_out=false;
	attributes[ParsersAttributes::SOURCE_TYPE]=QString();
	attributes[ParsersAttributes::DEST_TYPE]=QString();
	attributes[ParsersAttributes::CAST_TYPE]=QString();
	attributes[ParsersAttributes::IO_CAST]=QString();
	attributes[ParsersAttributes::FUNCTION]=QString();
}

void Cast::setDataType(unsigned type_idx, PgSQLType type)
{
	//Check if the type index is valid
	if(type_idx<=DST_TYPE)
	{
		//Raises an error if the passed data type is null
		if((*type).isEmpty())
			throw Exception(Exception::getErrorMessage(ERR_ASG_NULL_TYPE_OBJECT)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(OBJ_CAST)),
							ERR_ASG_NULL_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		setCodeInvalidated(this->types[type_idx] != type);
		this->types[type_idx]=type;
	}
	else
		//Raises an error if the type index is invalid
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Configures the cast name (in form of signature: cast(src_type, dst_type) )
	this->obj_name=QString("cast(%1,%2)").arg(~types[SRC_TYPE]).arg(~types[DST_TYPE]);
}

void Cast::setCastType(unsigned cast_type)
{
	//Raises an error if the user tries to assign an invalid cast type
	if(cast_type > IMPLICIT)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	PgSQLType ret_type;

	if(!cast_func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(OBJ_CAST)),
						ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Retrieve the cast function parameter count for specific validations
	param_count=cast_func->getParameterCount();

	//Raises an error if the function don't have at least 1 parameter or a maximum of 3
	if(param_count==0 || param_count > 3)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(OBJ_CAST)),
						ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		/* Error condition 1: Check if the first function parameter data type differs
		 from cast source data type */
		error=(cast_func->getParameter(0).getType()!=this->types[SRC_TYPE] &&
					!cast_func->getParameter(0).getType().canCastTo(this->types[SRC_TYPE]));

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
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(OBJ_CAST)),
							ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	/* Raises an error if the return type of the function differs from the destination data type.
			If the types can be casted between them no error is returned */
	ret_type=cast_func->getReturnType();

	if(ret_type!=this->types[DST_TYPE] && !ret_type.canCastTo(this->types[DST_TYPE]))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(OBJ_CAST)),
						ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(cast_function != cast_func);
	this->cast_function=cast_func;
}

PgSQLType Cast::getDataType(unsigned type_idx)
{
	if(type_idx > DST_TYPE)
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(this->types[type_idx]);
}

bool Cast::isInOut(void)
{
	return(is_in_out);
}

Function *Cast::getCastFunction(void)
{
	return(cast_function);
}

unsigned Cast::getCastType(void)
{
	return(cast_type);
}

QString Cast::getDropDefinition(bool cascade)
{
	attributes[ParsersAttributes::SIGNATURE].replace(QString(","), QString(" AS "));
	return(BaseObject::getDropDefinition(cascade));
}

QString Cast::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		attributes[ParsersAttributes::SOURCE_TYPE]=(*types[SRC_TYPE]);
		attributes[ParsersAttributes::DEST_TYPE]=(*types[DST_TYPE]);
	}
	else
	{
		attributes[ParsersAttributes::SOURCE_TYPE]=types[SRC_TYPE].getCodeDefinition(def_type);
		attributes[ParsersAttributes::DEST_TYPE]=types[DST_TYPE].getCodeDefinition(def_type);
	}

	if(!is_in_out && cast_function)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::FUNCTION]=cast_function->getSignature();
		else
			attributes[ParsersAttributes::FUNCTION]=cast_function->getCodeDefinition(def_type, true);
	}
	else
		attributes[ParsersAttributes::IO_CAST]=(is_in_out ? ParsersAttributes::_TRUE_ : QString());

	if(cast_type==ASSIGNMENT)
		attributes[ParsersAttributes::CAST_TYPE]=ParsersAttributes::ASSIGNMENT;
	else if(cast_type==IMPLICIT)
		attributes[ParsersAttributes::CAST_TYPE]=ParsersAttributes::IMPLICIT;
	else
		attributes[ParsersAttributes::CAST_TYPE]=QString();

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::CAST_TYPE]=attributes[ParsersAttributes::CAST_TYPE].toUpper();

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Cast::getSignature(bool)
{
	attributes[ParsersAttributes::SIGNATURE]=this->getName().remove(QString("cast"));
	return(BaseObject::getSignature(false));
}

