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

#include "cast.h"

Cast::Cast(void)
{
	obj_type=OBJ_CAST;
	cast_function=NULL;
	cast_type=IMPLICIT;
	is_in_out=false;
	attributes[ParsersAttributes::SOURCE_TYPE]="";
	attributes[ParsersAttributes::DEST_TYPE]="";
	attributes[ParsersAttributes::SIGNATURE]="";
	attributes[ParsersAttributes::CAST_TYPE]="";
	attributes[ParsersAttributes::IO_CAST]="";
}

void Cast::setDataType(unsigned type_idx, PgSQLType type)
{
	//Check if the type index is valid
	if(type_idx<=DST_TYPE)
	{
		//Raises an error if the passed data type is null
		if((*type)=="")
			throw Exception(Exception::getErrorMessage(ERR_ASG_NULL_TYPE_OBJECT)
											.arg(Utf8String::create(this->getName()))
											.arg(BaseObject::getTypeName(OBJ_CAST)),
											ERR_ASG_NULL_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Assign the passed type to the cast types vector on the specified index
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
	if(cast_type!=ASSIGNMENT && cast_type!=IMPLICIT)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->cast_type=cast_type;
}

void Cast::setInOut(bool value)
{
	is_in_out=value;
}

void Cast::setCastFunction(Function *cast_func)
{
	unsigned param_count;
	bool error=false;

	if(!cast_func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_CAST)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Retrieve the cast function parameter count for specific validations
	param_count=cast_func->getParameterCount();

	//Raises an error if the function don't have at least 1 parameter or a maximum of 3
	if(param_count==0 || param_count > 3)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_CAST)),
										ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		/* Error condition 1: Check if the first function parameter data type differs
		 from cast source data type */
		error=(cast_func->getParameter(0).getType()!=this->types[SRC_TYPE]);

		/* Error condition 2: Check if the second function parameter data type
		 is different from 'integer' */
		if(!error && param_count>=2)
			error=(cast_func->getParameter(1).getType()!="integer");

		/* Error condition 3: Check if the third function parameter data type is
		 different from 'boolean' */
		if(!error && param_count==3)
			error=(cast_func->getParameter(2).getType()!="boolean");

		//In case some error condition is reached raises an error
		if(error)
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
											.arg(Utf8String::create(this->getName()))
											.arg(BaseObject::getTypeName(OBJ_CAST)),
											ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	//Raises an error if the return type of the function differs from the destination data type
	if(cast_func->getReturnType()!=this->types[DST_TYPE])
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_CAST)),
										ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->cast_function=cast_func;
}

PgSQLType Cast::getDataType(unsigned type_idx)
{
	if(type_idx<=DST_TYPE)
		return(this->types[type_idx]);
	else
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

QString Cast::getCodeDefinition(unsigned def_type)
{
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

	if(cast_function)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::SIGNATURE]=cast_function->getSignature();
		else
			attributes[ParsersAttributes::SIGNATURE]=cast_function->getCodeDefinition(def_type, true);
	}

	if(cast_type==ASSIGNMENT)
		attributes[ParsersAttributes::CAST_TYPE]=ParsersAttributes::ASSIGNMENT;
	else
		attributes[ParsersAttributes::CAST_TYPE]=ParsersAttributes::IMPLICIT;

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::CAST_TYPE]=attributes[ParsersAttributes::CAST_TYPE].toUpper();

	attributes[ParsersAttributes::IO_CAST]=(is_in_out ? "1" : "");

	return(BaseObject::__getCodeDefinition(def_type));
}

