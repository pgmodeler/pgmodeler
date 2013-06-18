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

#include "function.h"

Function::Function(void)
{
	return_type=PgSQLType("void");
	language=nullptr;
	returns_setof=false;
	is_wnd_function=false;
	obj_type=OBJ_FUNCTION;

	execution_cost=100;
	row_amount=1000;

	attributes[ParsersAttributes::PARAMETERS]="";
	attributes[ParsersAttributes::EXECUTION_COST]="";
	attributes[ParsersAttributes::ROW_AMOUNT]="";
	attributes[ParsersAttributes::RETURN_TYPE]="";
	attributes[ParsersAttributes::FUNCTION_TYPE]="";
	attributes[ParsersAttributes::LANGUAGE]="";
	attributes[ParsersAttributes::RETURNS_SETOF]="";
	attributes[ParsersAttributes::SECURITY_TYPE]="";
	attributes[ParsersAttributes::BEHAVIOR_TYPE]="";
	attributes[ParsersAttributes::DEFINITION]="";
	attributes[ParsersAttributes::SIGNATURE]="";
	attributes[ParsersAttributes::REF_TYPE]="";
	attributes[ParsersAttributes::WINDOW_FUNC]="";
	attributes[ParsersAttributes::RETURN_TABLE]="";
	attributes[ParsersAttributes::LIBRARY]="";
	attributes[ParsersAttributes::SYMBOL]="";
}

void Function::setName(const QString &name)
{
	BaseObject::setName(name);
	createSignature();
}

void Function::setSchema(BaseObject *schema)
{
	BaseObject::setSchema(schema);
	createSignature();
}

void Function::addParameter(Parameter param)
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
		throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PARAM_FUNCTION)
										.arg(Utf8String::create(param.getName()))
										.arg(Utf8String::create(this->signature)),
										ERR_ASG_DUPLIC_PARAM_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Inserts the parameter in the function
	parameters.push_back(param);

	createSignature();
}

void Function::addReturnedTableColumn(const QString &name, PgSQLType type)
{
	//Raises an error if the column name is empty
	if(name=="")
		throw Exception(ERR_ASG_EMPTY_NAME_RET_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr,itr_end;
	bool found=false;

	itr=ret_table_columns.begin();
	itr_end=ret_table_columns.end();

	//Checks the duplicity of table column names
	while(itr!=itr_end && !found)
	{
		/* Compares the column name storing in the 'found' flag
		 if already exists in the returned table */
		found=(itr->getName()==name);
		itr++;
	}

	//Raises an error if the column is duplicated
	if(found)
		throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_RET_TAB_TYPE)
										.arg(Utf8String::create(name))
										.arg(Utf8String::create(this->signature)),
										ERR_INS_DUPLIC_RET_TAB_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	Parameter p;
	p.setName(name);
	p.setType(type);
	ret_table_columns.push_back(p);
}

void Function::setParametersAttribute(unsigned def_type)
{
	QString str_param;
	unsigned i, count;

	count=parameters.size();
	for(i=0; i < count; i++)
	{
		str_param+=parameters[i].getCodeDefinition(def_type);
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
		str_param.remove(str_param.size()-2,2);

	attributes[ParsersAttributes::PARAMETERS]=str_param;
}

void Function::setTableReturnTypeAttribute(unsigned def_type)
{
	QString str_type;
	unsigned i, count;

	count=ret_table_columns.size();
	for(i=0; i < count; i++)
	{
		str_type+=ret_table_columns[i].getCodeDefinition(def_type);
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
		str_type.remove(str_type.size()-2,2);

	attributes[ParsersAttributes::RETURN_TABLE]=str_type;
}

void Function::setExecutionCost(unsigned exec_cost)
{
	execution_cost=exec_cost;
}

void Function::setRowAmount(unsigned row_amount)
{
	this->row_amount=row_amount;
}

void Function::setLibrary(const QString &library)
{
	if(language->getName().toLower()!=~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
										.arg(Utf8String::create(this->getSignature())),
										ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->library=library;
}

void Function::setSymbol(const QString &symbol)
{
	if(language->getName().toLower()!=~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
										.arg(Utf8String::create(this->getSignature())),
										ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->symbol=symbol;
}

void Function::setReturnType(PgSQLType type)
{
	return_type=type;
}

void Function::setFunctionType(FunctionType func_type)
{
	function_type=func_type;
}

void Function::setLanguage(BaseObject *language)
{
	//Raises an error if the language is not allocated
	if(!language)
		throw Exception(ERR_ASG_NOT_ALOC_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the language object is invalid
	else if(language->getObjectType()!=OBJ_LANGUAGE)
		throw Exception(ERR_ASG_INV_LANGUAGE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->language=language;
}

void Function::setReturnSetOf(bool value)
{
	returns_setof=value;
}

void Function::setWindowFunction(bool value)
{
	is_wnd_function=value;
}

void Function::setSecurityType(SecurityType sec_type)
{
	security_type=sec_type;
}

void Function::setBehaviorType(BehaviorType behav_type)
{
	behavior_type=behav_type;
}

void Function::setSourceCode(const QString &src_code)
{
	if(language->getName().toLower()==~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_CODE_FUNC_C_LANGUAGE)
										.arg(Utf8String::create(this->getSignature())),
										ERR_ASG_CODE_FUNC_C_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->source_code=src_code;
}

PgSQLType Function::getReturnType(void)
{
	return(return_type);
}

FunctionType Function::getFunctionType(void)
{
	return(function_type);
}

BaseObject *Function::getLanguage(void)
{
	return(language);
}

unsigned Function::getParameterCount(void)
{
	return(parameters.size());
}

unsigned Function::getReturnedTableColumnCount(void)
{
	return(ret_table_columns.size());
}

bool Function::isReturnSetOf(void)
{
	return(returns_setof);
}

bool Function::isReturnTable(void)
{
	return(ret_table_columns.size() > 0);
}

bool Function::isWindowFunction(void)
{
	return(is_wnd_function);
}

SecurityType Function::getSecurityType(void)
{
	return(security_type);
}

BehaviorType Function::getBehaviorType(void)
{
	return(behavior_type);
}

QString Function::getSourceCode(void)
{
	return(source_code);
}

Parameter Function::getParameter(unsigned param_idx)
{
	//Raises an error if the parameter index is out of bound
	if(param_idx>=parameters.size())
		throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(parameters[param_idx]);
}

Parameter Function::getReturnedTableColumn(unsigned column_idx)
{
	//Raises an error if the column index is out of bound
	if(column_idx>=ret_table_columns.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(ret_table_columns[column_idx]);
}

unsigned Function::getExecutionCost(void)
{
	return(execution_cost);
}

unsigned Function::getRowAmount(void)
{
	return(row_amount);
}

QString Function::getLibrary(void)
{
	return(library);
}

QString Function::getSymbol(void)
{
	return(symbol);
}

void Function::removeParameters(void)
{
	parameters.clear();
	createSignature();
}

void Function::removeReturnedTableColumns(void)
{
	ret_table_columns.clear();
}

void Function::removeParameter(const QString &name, PgSQLType type)
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

void Function::removeParameter(unsigned param_idx)
{
	//Raises an error if parameter index is out of bound
	if(param_idx>=parameters.size())
		throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=parameters.begin()+param_idx;
	parameters.erase(itr);

	createSignature();
}

void Function::removeReturnedTableColumn(unsigned column_idx)
{
	if(column_idx>=ret_table_columns.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=ret_table_columns.begin()+column_idx;
	ret_table_columns.erase(itr);
}

QString Function::getSignature(void)
{
	return(signature);
}

void Function::createSignature(bool format, bool prepend_schema)
{
	QString str_param;
	unsigned i, count;

	count=parameters.size();
	for(i=0; i < count; i++)
		str_param+=parameters[i].getCodeDefinition(SchemaParser::SQL_DEFINITION, true).trimmed();

	str_param.remove(str_param.length()-1, 1);

	//Signature format NAME(IN|OUT PARAM1_TYPE,IN|OUT PARAM2_TYPE,...,IN|OUT PARAMn_TYPE)
	signature=this->getName(format, prepend_schema) + QString("(") + str_param + QString(")");
}

QString Function::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString Function::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	setParametersAttribute(def_type);

	attributes[ParsersAttributes::EXECUTION_COST]=QString("%1").arg(execution_cost);
	attributes[ParsersAttributes::ROW_AMOUNT]=QString("%1").arg(row_amount);
	attributes[ParsersAttributes::FUNCTION_TYPE]=(~function_type);

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		attributes[ParsersAttributes::LANGUAGE]=language->getName(false);
		attributes[ParsersAttributes::RETURN_TYPE]=(*return_type);
	}
	else
	{
		attributes[ParsersAttributes::LANGUAGE]=language->getCodeDefinition(def_type,true);
		attributes[ParsersAttributes::RETURN_TYPE]=return_type.getCodeDefinition(def_type);
	}

	setTableReturnTypeAttribute(def_type);

	attributes[ParsersAttributes::RETURNS_SETOF]=(returns_setof ? "1" : "");
	attributes[ParsersAttributes::WINDOW_FUNC]=(is_wnd_function ? "1" : "");
	attributes[ParsersAttributes::SECURITY_TYPE]=(~security_type);
	attributes[ParsersAttributes::BEHAVIOR_TYPE]=(~behavior_type);
	attributes[ParsersAttributes::DEFINITION]=source_code;

	if(language->getName()==~LanguageType(LanguageType::c))
	{
		attributes[ParsersAttributes::SYMBOL]=symbol;
		attributes[ParsersAttributes::LIBRARY]=library;
	}

	attributes[ParsersAttributes::SIGNATURE]=signature;
	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

