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

#include "language.h"
#include "defaultlanguages.h"

Language::Language()
{
	obj_type=ObjectType::Language;
	is_trusted=false;

	for(unsigned i=ValidatorFunc; i <= InlineFunc; i++)
		functions[i]=nullptr;

	attributes[Attributes::Trusted]=QString();
	attributes[Attributes::HandlerFunc]=QString();
	attributes[Attributes::ValidatorFunc]=QString();
	attributes[Attributes::InlineFunc]=QString();
}

void Language::setName(const QString &name)
{
	//Raises an error if the user try to set an system reserved language name (C, SQL)
	if(name.toLower() == DefaultLanguages::C || name.toLower() == DefaultLanguages::Sql)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgReservedName)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::Language)),
						ErrorCode::AsgReservedName,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setName(name);
}

void Language::setTrusted(bool value)
{
	setCodeInvalidated(is_trusted != value);
	is_trusted=value;
}

void Language::setFunction(Function *func, unsigned func_type)
{
	if(!func ||
			(func &&
			 /* The handler function must be written in C and have
								  'language_handler' as return type */
			 ((func_type==HandlerFunc &&
			   func->getReturnType()==QString("language_handler") &&
			   func->getParameterCount()==0 &&
				 func->getLanguage()->getName().toLower() == DefaultLanguages::C) ||
			  /* The validator function must be written in C and return 'void' also
									   must have only one parameter of the type 'oid' */
			  (func_type==ValidatorFunc &&
			   func->getReturnType()==QString("void") &&
			   func->getParameterCount()==1 &&
			   func->getParameter(0).getType() == QString("oid") &&
				 func->getLanguage()->getName().toLower() == DefaultLanguages::C) ||
			  /* The inline function must be written in C and return 'void' also
									   must have only one parameter of the type 'internal' */
			  (func_type==InlineFunc &&
			   func->getReturnType()==QString("void") &&
			   func->getParameterCount()==1 &&
			   func->getParameter(0).getType() == QString("internal") &&
				 func->getLanguage()->getName().toLower() == DefaultLanguages::C) )))
	{
		setCodeInvalidated(functions[func_type] != func);
		this->functions[func_type]=func;
	}
	//Raises an error in case the function return type doesn't matches the required by each rule
	else if((func_type==HandlerFunc && func->getReturnType()!=QString("language_handler")) ||
			((func_type==ValidatorFunc || func_type==InlineFunc) && func->getReturnType()!=QString("void")))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Language)),
						ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		//Raises an error in case the function has invalid parameters (count and types)
		throw Exception(ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Function * Language::getFunction(unsigned func_type)
{
	if(func_type > InlineFunc)
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return functions[func_type];
}

bool Language::isTrusted()
{
	return is_trusted;
}

QString Language::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, false);
}

QString Language::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	unsigned i;
	QString attribs_func[3]={Attributes::ValidatorFunc,
							 Attributes::HandlerFunc,
							 Attributes::InlineFunc};

	attributes[Attributes::Trusted]=(is_trusted ? Attributes::True : QString());

	if(!reduced_form && def_type==SchemaParser::XmlDefinition)
		reduced_form=(!functions[ValidatorFunc] && !functions[HandlerFunc] && !functions[InlineFunc] && !this->getOwner());

	for(i=0; i < 3; i++)
	{
		if(functions[i])
		{
			if(def_type==SchemaParser::SqlDefinition)
				attributes[attribs_func[i]]=functions[i]->getName(true);
			else
			{
				functions[i]->setAttribute(Attributes::RefType, attribs_func[i]);
				attributes[attribs_func[i]]=functions[i]->getCodeDefinition(def_type, true);
			}
		}
	}

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

