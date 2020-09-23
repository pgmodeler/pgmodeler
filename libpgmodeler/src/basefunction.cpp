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

#include "basefunction.h"
#include "defaultlanguages.h"

BaseFunction::BaseFunction()
{
	language=nullptr;
	obj_type=ObjectType::BaseObject;

	attributes[Attributes::Parameters]="";
	attributes[Attributes::Language]="";
	attributes[Attributes::SecurityType]="";
	attributes[Attributes::Definition]="";
	attributes[Attributes::Signature]="";
	attributes[Attributes::ReturnTable]="";
	attributes[Attributes::Library]="";
	attributes[Attributes::Symbol]="";
}

void BaseFunction::setName(const QString &name)
{
	BaseObject::setName(name);
	createSignature();
}

void BaseFunction::setSchema(BaseObject *schema)
{
	BaseObject::setSchema(schema);
	createSignature();
}

void BaseFunction::addParameter(Parameter param)
{
	vector<Parameter>::iterator itr,itr_end;
	bool found=false;

	itr=parameters.begin();
	itr_end=parameters.end();

	//Checks the duplicity of parameter names
	while(itr!=itr_end && !found)
	{
		/* Compares the parameters name storing in the 'found' flag
		 if already exists in the function */
		found=(itr->getName()==param.getName());
		itr++;
	}

	//If a duplicated parameter is found an error is raised
	if(found)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedParameterFunction)
						.arg(param.getName())
						.arg(this->signature),
						ErrorCode::AsgDuplicatedParameterFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Inserts the parameter in the function
	parameters.push_back(param);
	createSignature();
}

void BaseFunction::setParametersAttribute(unsigned def_type)
{
	QString str_param;
	QStringList fmt_params;

	for(auto &param : parameters)
		fmt_params.append(param.getCodeDefinition(def_type));

	str_param = fmt_params.join("");

	if(def_type==SchemaParser::SqlDefinition)
		str_param.remove(str_param.size()-2,2);

	attributes[Attributes::Parameters]=str_param;
}

void BaseFunction::setBasicFunctionAttributes(unsigned def_type)
{
	setParametersAttribute(def_type);

	if(language)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Language]=language->getName(false);
		else
			attributes[Attributes::Language]=language->getCodeDefinition(def_type, true);

		if(language->getName().toLower() == DefaultLanguages::C)
		{
			attributes[Attributes::Symbol]=symbol;
			attributes[Attributes::Library]=library;
		}
	}

	attributes[Attributes::SecurityType]=~security_type;
	attributes[Attributes::Definition]=source_code;
	attributes[Attributes::Signature]=signature;
}

void BaseFunction::configureSearchAttributes()
{
	QStringList param_types;

	BaseObject::configureSearchAttributes();

	for(auto &param : parameters)
		param_types += *param.getType();

	search_attribs[Attributes::Type] = param_types.join("; ");
}

void BaseFunction::setLibrary(const QString &library)
{
	if(language->getName().toLower() != DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgRefLibraryFuncLanguageNotC)
						.arg(this->getSignature()),
						ErrorCode::AsgRefLibraryFuncLanguageNotC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->library != library);
	this->library=library;
}

void BaseFunction::setSymbol(const QString &symbol)
{
	if(language->getName().toLower() != DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgRefLibraryFuncLanguageNotC)
						.arg(this->getSignature()),
						ErrorCode::AsgRefLibraryFuncLanguageNotC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->symbol != symbol);
	this->symbol=symbol;
}

void BaseFunction::setLanguage(BaseObject *language)
{
	//Raises an error if the language is not allocated
	if(!language)
		throw Exception(ErrorCode::AsgNotAllocatedLanguage,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the language object is invalid
	else if(language->getObjectType()!=ObjectType::Language)
		throw Exception(ErrorCode::AsgInvalidLanguageObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->language != language);
	this->language=language;
}

void BaseFunction::setSecurityType(SecurityType sec_type)
{
	setCodeInvalidated(security_type != sec_type);
	security_type=sec_type;
}

void BaseFunction::setSourceCode(const QString &src_code)
{
	if(language && language->getName().toLower() == DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgSourceCodeFuncCLanguage)
						.arg(this->getSignature()),
						ErrorCode::AsgSourceCodeFuncCLanguage,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->source_code != src_code);
	this->source_code=src_code;
}

BaseObject *BaseFunction::getLanguage()
{
	return language;
}

unsigned BaseFunction::getParameterCount()
{
	return parameters.size();
}

SecurityType BaseFunction::getSecurityType()
{
	return security_type;
}

QString BaseFunction::getSourceCode()
{
	return source_code;
}

Parameter BaseFunction::getParameter(unsigned param_idx)
{
	//Raises an error if the parameter index is out of bound
	if(param_idx>=parameters.size())
		throw Exception(ErrorCode::RefParameterInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return parameters[param_idx];
}

QString BaseFunction::getLibrary()
{
	return library;
}

QString BaseFunction::getSymbol()
{
	return symbol;
}

void BaseFunction::removeParameters()
{
	parameters.clear();
	createSignature();
}

void BaseFunction::removeParameter(const QString &name, PgSqlType type)
{
	vector<Parameter>::iterator itr,itr_end;

	itr=parameters.begin();
	itr_end=parameters.end();

	while(itr!=itr_end)
	{
		//Compares the iterator name and type with the passed name an type
		if(itr->getName()==name && itr->getType()==(~type))
		{
			parameters.erase(itr);
			break;
		}
		itr++;
	}

	//After remove the parameter is necessary updated the signature
	createSignature();
}

void BaseFunction::removeParameter(unsigned param_idx)
{
	//Raises an error if parameter index is out of bound
	if(param_idx>=parameters.size())
		throw Exception(ErrorCode::RefParameterInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=parameters.begin()+param_idx;
	parameters.erase(itr);

	createSignature();
}

QString BaseFunction::getSignature(bool)
{
	return signature;
}

void BaseFunction::createSignature(bool format, bool prepend_schema)
{
	QString aux_str;
	QStringList fmt_params;

	for(auto &param : parameters)
	{
		//OUT parameters is not part of function's signature
		if(!param.isOut() || param.isVariadic() ||
				(param.isIn() && param.isOut()) ||
				(param.isIn() && !param.isOut()))
		{
			/* Removing the arg mode IN from parameter signature because this is de default for any kind of parameter
			 * So in order to avoid signature conflicts (mainly whe diff functions) we remove it.
			 * The keyword OUT is also removed for IN OUT parameters, since removing the IN parameter the OUT keyword will remain which
			 * forms an invalid signature. */
			aux_str = param.getCodeDefinition(SchemaParser::SqlDefinition, true).replace(QRegExp("^(IN)?( )*(OUT)?( )"),"").trimmed();
			aux_str.remove(',');
			fmt_params.append(aux_str);
			param.setCodeInvalidated(true);
		}
	}

	//Signature format NAME(IN|OUT PARAM1_TYPE,IN|OUT PARAM2_TYPE,...,IN|OUT PARAMn_TYPE)
	signature=this->getName(format, prepend_schema) + QString("(") + fmt_params.join(",") + QString(")");
	this->setCodeInvalidated(true);
}
