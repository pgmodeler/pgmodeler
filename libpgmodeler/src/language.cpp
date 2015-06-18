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

#include "language.h"

Language::Language(void)
{
	obj_type=OBJ_LANGUAGE;
	is_trusted=false;

	for(unsigned i=VALIDATOR_FUNC; i <= INLINE_FUNC; i++)
		functions[i]=nullptr;

	attributes[ParsersAttributes::TRUSTED]=QString();
	attributes[ParsersAttributes::HANDLER_FUNC]=QString();
	attributes[ParsersAttributes::VALIDATOR_FUNC]=QString();
	attributes[ParsersAttributes::INLINE_FUNC]=QString();
}

void Language::setName(const QString &name)
{
	//Raises an error if the user try to set an system reserved language name (C, SQL)
	if(name.toLower()==~LanguageType("c") || name.toLower()==~LanguageType("sql"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                    .arg(this->getName())
										.arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
										ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setName(name);
}

void Language::setTrusted(bool value)
{
	setCodeInvalidated(is_trusted != value);
	is_trusted=value;
}

void  Language::setFunction(Function *func, unsigned func_type)
{
	LanguageType lang=LanguageType::c;

	if(!func ||
		 (func &&
			/* The handler function must be written in C and have
						 'language_handler' as return type */
			((func_type==HANDLER_FUNC &&
        func->getReturnType()==QString("language_handler") &&
				func->getParameterCount()==0 &&
				func->getLanguage()->getName()==(~lang)) ||
			 /* The validator function must be written in C and return 'void' also
							 must have only one parameter of the type 'oid' */
			 (func_type==VALIDATOR_FUNC &&
        func->getReturnType()==QString("void") &&
				func->getParameterCount()==1 &&
        func->getParameter(0).getType() == QString("oid") &&
				func->getLanguage()->getName()==(~lang)) ||
			 /* The inline function must be written in C and return 'void' also
							 must have only one parameter of the type 'internal' */
			 (func_type==INLINE_FUNC &&
        func->getReturnType()==QString("void") &&
				func->getParameterCount()==1 &&
        func->getParameter(0).getType() == QString("internal") &&
				func->getLanguage()->getName()==(~lang)) )))
	{
		setCodeInvalidated(functions[func_type] != func);
		this->functions[func_type]=func;
	}
	//Raises an error in case the function return type doesn't matches the required by each rule
  else if((func_type==HANDLER_FUNC && func->getReturnType()!=QString("language_handler")) ||
          ((func_type==VALIDATOR_FUNC || func_type==INLINE_FUNC) && func->getReturnType()!=QString("void")))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
										.arg(this->getName(true))
										.arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
										ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		//Raises an error in case the function has invalid parameters (count and types)
		throw Exception(ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Function * Language::getFunction(unsigned func_type)
{
	if(func_type > INLINE_FUNC)
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(functions[func_type]);
}

bool Language::isTrusted(void)
{
	return(is_trusted);
}

QString Language::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString Language::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return(code_def);

	unsigned i;
	QString attribs_func[3]={ParsersAttributes::VALIDATOR_FUNC,
													 ParsersAttributes::HANDLER_FUNC,
													 ParsersAttributes::INLINE_FUNC};

	attributes[ParsersAttributes::TRUSTED]=(is_trusted ? ParsersAttributes::_TRUE_ : QString());

	if(!reduced_form && def_type==SchemaParser::XML_DEFINITION)
		reduced_form=(!functions[VALIDATOR_FUNC] && !functions[HANDLER_FUNC] && !functions[INLINE_FUNC] && !this->getOwner());

	for(i=0; i < 3; i++)
	{
		if(functions[i])
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[attribs_func[i]]=functions[i]->getName(true);
			else
			{
				functions[i]->setAttribute(ParsersAttributes::REF_TYPE, attribs_func[i]);
				attributes[attribs_func[i]]=functions[i]->getCodeDefinition(def_type, true);
			}
		}
	}

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

