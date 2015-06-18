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

#include "conversion.h"

Conversion::Conversion(void)
{
	obj_type=OBJ_CONVERSION;
	conversion_func=nullptr;
	is_default=false;
	attributes[ParsersAttributes::DEFAULT]=QString();
	attributes[ParsersAttributes::SRC_ENCODING]=QString();
	attributes[ParsersAttributes::DST_ENCODING]=QString();
	attributes[ParsersAttributes::FUNCTION]=QString();
}

void Conversion::setEncoding(unsigned encoding_idx, EncodingType encoding_type)
{
	//Checks if the encoding index is valid
	if(encoding_idx<=DST_ENCODING)
	{
		//If the passed enconding type is null an error is raised
    if((~encoding_type).isEmpty())
			throw Exception(Exception::getErrorMessage(ERR_ASG_NULL_TYPE_OBJECT)
                      .arg(this->getName())
											.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
											ERR_ASG_NULL_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Assigns the encoding to the conversion in the specified index
		this->encodings[encoding_idx]=encoding_type;
	}
	else
		//Raises an error if the encoding index is invalid
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Conversion::setConversionFunction(Function *conv_func)
{
	//Raises an error in case the passed conversion function is null
	if(!conv_func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                    .arg(this->getName(true))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* The conversion function must have 5 parameters if it's not the case
		raises an error. */
	else if(conv_func->getParameterCount()!=5)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                    .arg(this->getName(true))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* Raises an error if the function parameters does not following the type order:
		interger, integer, cstring, internal, integer */
  else if(conv_func->getParameter(0).getType()!=QString("integer") ||
          conv_func->getParameter(1).getType()!=QString("integer") ||
          conv_func->getParameter(2).getType()!=QString("cstring") ||
          conv_func->getParameter(3).getType()!=QString("internal") ||
          conv_func->getParameter(4).getType()!=QString("integer"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
                    .arg(this->getName(true))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the conversion function return type is not 'void'
  else if(conv_func->getReturnType()!=QString("void"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
                    .arg(this->getName(true))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(conversion_func != conv_func);
	this->conversion_func=conv_func;
}

void Conversion::setDefault(bool value)
{
	setCodeInvalidated(is_default != value);
	is_default=value;
}

EncodingType Conversion::getEncoding(unsigned encoding_idx)
{
	if(encoding_idx > DST_ENCODING)
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(this->encodings[encoding_idx]);
}

Function *Conversion::getConversionFunction(void)
{
	return(conversion_func);
}

bool Conversion::isDefault(void)
{
	return(is_default);
}

QString Conversion::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[ParsersAttributes::DEFAULT]=(is_default ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::SRC_ENCODING]=(~encodings[SRC_ENCODING]);
	attributes[ParsersAttributes::DST_ENCODING]=(~encodings[DST_ENCODING]);

	if(conversion_func)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::FUNCTION]=conversion_func->getName(true);
		else
			attributes[ParsersAttributes::FUNCTION]=conversion_func->getCodeDefinition(def_type, true);
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

