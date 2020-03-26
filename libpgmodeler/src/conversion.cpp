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

#include "conversion.h"

Conversion::Conversion()
{
	obj_type=ObjectType::Conversion;
	conversion_func=nullptr;
	is_default=false;
	attributes[Attributes::Default]=QString();
	attributes[Attributes::SrcEncoding]=QString();
	attributes[Attributes::DstEncoding]=QString();
	attributes[Attributes::Function]=QString();
}

void Conversion::setEncoding(unsigned encoding_idx, EncodingType encoding_type)
{
	//Checks if the encoding index is valid
	if(encoding_idx<=DstEncoding)
	{
		//If the passed enconding type is null an error is raised
		if((~encoding_type).isEmpty())
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgNullTypeObject)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Conversion)),
							ErrorCode::AsgNullTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Assigns the encoding to the conversion in the specified index
		this->encodings[encoding_idx]=encoding_type;
	}
	else
		//Raises an error if the encoding index is invalid
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Conversion::setConversionFunction(Function *conv_func)
{
	//Raises an error in case the passed conversion function is null
	if(!conv_func)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Conversion)),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* The conversion function must have 5 parameters if it's not the case
		raises an error. */
	else if(conv_func->getParameterCount()!=5)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Conversion)),
						ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* Raises an error if the function parameters does not following the type order:
		interger, integer, cstring, internal, integer */
	else if(conv_func->getParameter(0).getType()!=QString("integer") ||
			conv_func->getParameter(1).getType()!=QString("integer") ||
			conv_func->getParameter(2).getType()!=QString("cstring") ||
			conv_func->getParameter(3).getType()!=QString("internal") ||
			conv_func->getParameter(4).getType()!=QString("integer"))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Conversion)),
						ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the conversion function return type is not 'void'
	else if(conv_func->getReturnType()!=QString("void"))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Conversion)),
						ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(encoding_idx > DstEncoding)
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return this->encodings[encoding_idx];
}

Function *Conversion::getConversionFunction()
{
	return conversion_func;
}

bool Conversion::isDefault()
{
	return is_default;
}

QString Conversion::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Default]=(is_default ? Attributes::True : QString());
	attributes[Attributes::SrcEncoding]=(~encodings[SrcEncoding]);
	attributes[Attributes::DstEncoding]=(~encodings[DstEncoding]);

	if(conversion_func)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Function]=conversion_func->getName(true);
		else
			attributes[Attributes::Function]=conversion_func->getCodeDefinition(def_type, true);
	}

	return BaseObject::__getCodeDefinition(def_type);
}

