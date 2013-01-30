#include "conversion.h"

Conversion::Conversion(void)
{
	obj_type=OBJ_CONVERSION;
	conversion_func=NULL;
	is_default=false;
	attributes[ParsersAttributes::DEFAULT]="";
	attributes[ParsersAttributes::SRC_ENCODING]="";
	attributes[ParsersAttributes::DST_ENCODING]="";
	attributes[ParsersAttributes::FUNCTION]="";
}

void Conversion::setEncoding(unsigned encoding_idx, EncodingType encoding_type)
{
	//Checks if the encoding index is valid
	if(encoding_idx<=DST_ENCODING)
	{
		//If the passed enconding type is null an error is raised
		if((~encoding_type)=="")
			throw Exception(Exception::getErrorMessage(ERR_ASG_NULL_TYPE_OBJECT)
											.arg(QString::fromUtf8(this->getName()))
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
										.arg(QString::fromUtf8(this->getName(true)))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* The conversion function must have 5 parameters if it's not the case
		raises an error. */
	else if(conv_func->getParameterCount()!=5)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
										.arg(QString::fromUtf8(this->getName(true)))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* Raises an error if the function parameters does not following the type order:
		interger, integer, cstring, internal, integer */
	else if(conv_func->getParameter(0).getType()!="integer" ||
					conv_func->getParameter(1).getType()!="integer" ||
					conv_func->getParameter(2).getType()!="cstring" ||
					conv_func->getParameter(3).getType()!="internal" ||
					conv_func->getParameter(4).getType()!="integer")
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
										.arg(QString::fromUtf8(this->getName(true)))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the conversion function return type is not 'void'
	else if(conv_func->getReturnType()!="void")
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
										.arg(QString::fromUtf8(this->getName(true)))
										.arg(BaseObject::getTypeName(OBJ_CONVERSION)),
										ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->conversion_func=conv_func;
}

void Conversion::setDefault(bool value)
{
	is_default=value;
}

EncodingType Conversion::getEncoding(unsigned encoding_idx)
{
	if(encoding_idx<=DST_ENCODING)
		return(this->encodings[encoding_idx]);
	else
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
	attributes[ParsersAttributes::DEFAULT]=(is_default ? "1" : "");
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

