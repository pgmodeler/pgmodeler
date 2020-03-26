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

#include "function.h"
#include "defaultlanguages.h"

Function::Function()
{
	return_type=PgSqlType(QString("void"));
	language=nullptr;
	returns_setof=false;
	is_wnd_function=false;
	is_leakproof=false;
	obj_type=ObjectType::Function;

	execution_cost=100;
	row_amount=1000;

	attributes[Attributes::Parameters]=QString();
	attributes[Attributes::ExecutionCost]=QString();
	attributes[Attributes::RowAmount]=QString();
	attributes[Attributes::ReturnType]=QString();
	attributes[Attributes::FunctionType]=QString();
	attributes[Attributes::Language]=QString();
	attributes[Attributes::ReturnsSetOf]=QString();
	attributes[Attributes::SecurityType]=QString();
	attributes[Attributes::BehaviorType]=QString();
	attributes[Attributes::Definition]=QString();
	attributes[Attributes::Signature]=QString();
	attributes[Attributes::RefType]=QString();
	attributes[Attributes::WindowFunc]=QString();
	attributes[Attributes::ReturnTable]=QString();
	attributes[Attributes::Library]=QString();
	attributes[Attributes::Symbol]=QString();
	attributes[Attributes::LeakProof]=QString();
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
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedParameterFunction)
						.arg(param.getName())
						.arg(this->signature),
						ErrorCode::AsgDuplicatedParameterFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Inserts the parameter in the function
	parameters.push_back(param);
	createSignature();
}

void Function::addReturnedTableColumn(const QString &name, PgSqlType type)
{
	//Raises an error if the column name is empty
	if(name.isEmpty())
		throw Exception(ErrorCode::AsgEmptyNameTableReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedTableReturnType)
						.arg(name)
						.arg(this->signature),
						ErrorCode::InsDuplicatedTableReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	Parameter p;
	p.setName(name);
	p.setType(type);
	ret_table_columns.push_back(p);
	setCodeInvalidated(true);
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

	if(def_type==SchemaParser::SqlDefinition)
		str_param.remove(str_param.size()-2,2);

	attributes[Attributes::Parameters]=str_param;
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

	if(def_type==SchemaParser::SqlDefinition)
		str_type.remove(str_type.size()-2,2);

	attributes[Attributes::ReturnTable]=str_type;
}

void Function::setExecutionCost(unsigned exec_cost)
{
	setCodeInvalidated(execution_cost != exec_cost);
	execution_cost=exec_cost;
}

void Function::setRowAmount(unsigned row_amount)
{
	setCodeInvalidated(this->row_amount != row_amount);
	this->row_amount=row_amount;
}

void Function::setLibrary(const QString &library)
{
	if(language->getName().toLower() != DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgRefLibraryFuncLanguageNotC)
						.arg(this->getSignature()),
						ErrorCode::AsgRefLibraryFuncLanguageNotC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->library != library);
	this->library=library;
}

void Function::setSymbol(const QString &symbol)
{
	if(language->getName().toLower() != DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgRefLibraryFuncLanguageNotC)
						.arg(this->getSignature()),
						ErrorCode::AsgRefLibraryFuncLanguageNotC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->symbol != symbol);
	this->symbol=symbol;
}

void Function::setReturnType(PgSqlType type)
{
	setCodeInvalidated(return_type != type);
	return_type=type;
}

void Function::setFunctionType(FunctionType func_type)
{
	setCodeInvalidated(function_type != func_type);
	function_type=func_type;
}

void Function::setLanguage(BaseObject *language)
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

void Function::setReturnSetOf(bool value)
{
	setCodeInvalidated(returns_setof != value);
	returns_setof=value;
}

void Function::setWindowFunction(bool value)
{
	setCodeInvalidated(is_wnd_function != value);
	is_wnd_function=value;
}

void Function::setLeakProof(bool value)
{
	setCodeInvalidated(is_leakproof != value);
	is_leakproof=value;
}

void Function::setSecurityType(SecurityType sec_type)
{
	setCodeInvalidated(security_type != sec_type);
	security_type=sec_type;
}

void Function::setBehaviorType(BehaviorType behav_type)
{
	setCodeInvalidated(behavior_type != behav_type);
	behavior_type=behav_type;
}

void Function::setSourceCode(const QString &src_code)
{
	if(language && language->getName().toLower() == DefaultLanguages::C)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgSourceCodeFuncCLanguage)
						.arg(this->getSignature()),
						ErrorCode::AsgSourceCodeFuncCLanguage,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->source_code != src_code);
	this->source_code=src_code;
}

PgSqlType Function::getReturnType()
{
	return return_type;
}

FunctionType Function::getFunctionType()
{
	return function_type;
}

BaseObject *Function::getLanguage()
{
	return language;
}

unsigned Function::getParameterCount()
{
	return parameters.size();
}

unsigned Function::getReturnedTableColumnCount()
{
	return ret_table_columns.size();
}

bool Function::isReturnSetOf()
{
	return returns_setof;
}

bool Function::isReturnTable()
{
	return (ret_table_columns.size() > 0);
}

bool Function::isWindowFunction()
{
	return is_wnd_function;
}

bool Function::isLeakProof()
{
	return is_leakproof;
}

SecurityType Function::getSecurityType()
{
	return security_type;
}

BehaviorType Function::getBehaviorType()
{
	return behavior_type;
}

QString Function::getSourceCode()
{
	return source_code;
}

Parameter Function::getParameter(unsigned param_idx)
{
	//Raises an error if the parameter index is out of bound
	if(param_idx>=parameters.size())
		throw Exception(ErrorCode::RefParameterInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return parameters[param_idx];
}

Parameter Function::getReturnedTableColumn(unsigned column_idx)
{
	//Raises an error if the column index is out of bound
	if(column_idx>=ret_table_columns.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return ret_table_columns[column_idx];
}

unsigned Function::getExecutionCost()
{
	return execution_cost;
}

unsigned Function::getRowAmount()
{
	return row_amount;
}

QString Function::getLibrary()
{
	return library;
}

QString Function::getSymbol()
{
	return symbol;
}

void Function::removeParameters()
{
	parameters.clear();
	createSignature();
}

void Function::removeReturnedTableColumns()
{
	ret_table_columns.clear();
	setCodeInvalidated(true);
}

void Function::removeParameter(const QString &name, PgSqlType type)
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
		throw Exception(ErrorCode::RefParameterInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=parameters.begin()+param_idx;
	parameters.erase(itr);

	createSignature();
}

void Function::removeReturnedTableColumn(unsigned column_idx)
{
	if(column_idx>=ret_table_columns.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=ret_table_columns.begin()+column_idx;
	ret_table_columns.erase(itr);
	setCodeInvalidated(true);
}

QString Function::getSignature(bool)
{
	return signature;
}

void Function::createSignature(bool format, bool prepend_schema)
{
	QString str_params, aux_str;
	unsigned i, count;

	count=parameters.size();
	for(i=0; i < count; i++)
	{
		//OUT parameters is not part of function's signature
		if(!parameters[i].isOut() || parameters[i].isVariadic() ||
				(parameters[i].isIn() && parameters[i].isOut()) ||
				(parameters[i].isIn() && !parameters[i].isOut()))
		{
			/* Removing the arg mode IN from parameter signature because this is de default for any kind of parameter
			 * So in order to avoid signature conflicts (mainly whe diff functions) we remove it */
			aux_str=parameters[i].getCodeDefinition(SchemaParser::SqlDefinition, true).replace(QRegExp("^(IN)( )"),"");
			str_params+=aux_str.trimmed();
			parameters[i].setCodeInvalidated(true);
		}
	}

	str_params.remove(str_params.length()-1, 1);

	//Signature format NAME(IN|OUT PARAM1_TYPE,IN|OUT PARAM2_TYPE,...,IN|OUT PARAMn_TYPE)
	signature=this->getName(format, prepend_schema) + QString("(") + str_params + QString(")");
	this->setCodeInvalidated(true);
}

QString Function::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, false);
}

QString Function::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	setParametersAttribute(def_type);

	attributes[Attributes::ExecutionCost]=QString("%1").arg(execution_cost);
	attributes[Attributes::RowAmount]=QString("%1").arg(row_amount);
	attributes[Attributes::FunctionType]=(~function_type);

	if(language)
	{
		if(def_type==SchemaParser::SqlDefinition)
		{
			attributes[Attributes::Language]=language->getName(false);
			attributes[Attributes::ReturnType]=(*return_type);
		}
		else
		{
			attributes[Attributes::Language]=language->getCodeDefinition(def_type,true);
			attributes[Attributes::ReturnType]=return_type.getCodeDefinition(def_type);
		}

		if(language->getName().toLower() == DefaultLanguages::C)
		{
			attributes[Attributes::Symbol]=symbol;
			attributes[Attributes::Library]=library;
		}
	}

	setTableReturnTypeAttribute(def_type);

	attributes[Attributes::ReturnsSetOf]=(returns_setof ? Attributes::True : QString());
	attributes[Attributes::WindowFunc]=(is_wnd_function ? Attributes::True : QString());
	attributes[Attributes::LeakProof]=(is_leakproof ? Attributes::True : QString());
	attributes[Attributes::SecurityType]=~security_type;
	attributes[Attributes::BehaviorType]=~behavior_type;
	attributes[Attributes::Definition]=source_code;

	attributes[Attributes::Signature]=signature;
	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Function::getAlterDefinition(BaseObject *object)
{
	Function *func=dynamic_cast<Function *>(object);

	if(!func)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;

		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object);

		if(this->source_code.simplified() != func->source_code.simplified() ||
			 this->library!=func->library || this->symbol!=func->symbol)
		{
			attribs[Attributes::Definition]=func->getCodeDefinition(SchemaParser::SqlDefinition);
			attribs[Attributes::Definition].replace(QString("CREATE FUNCTION"), QString("CREATE OR REPLACE FUNCTION"));
		}
		else
		{
			if(this->execution_cost!=func->execution_cost)
				attribs[Attributes::ExecutionCost]=QString::number(func->execution_cost);

			if(this->returns_setof && func->returns_setof && this->row_amount!=func->row_amount)
			{
				attribs[Attributes::ReturnsSetOf]=Attributes::True;
				attribs[Attributes::RowAmount]=QString::number(row_amount);
			}

			if(this->function_type!=func->function_type)
				attribs[Attributes::FunctionType]=~func->function_type;

			if(this->is_leakproof!=func->is_leakproof)
				attribs[Attributes::LeakProof]=(func->is_leakproof ? Attributes::True : Attributes::Unset);

			if(this->security_type!=func->security_type)
				attribs[Attributes::SecurityType]=~func->security_type;

			if((this->behavior_type!=func->behavior_type) &&
					((this->behavior_type==BehaviorType::CalledOnNullInput) ||
					 ((this->behavior_type==BehaviorType::Strict || this->behavior_type==BehaviorType::ReturnsNullOnNullInput) &&
					  func->function_type==BehaviorType::CalledOnNullInput)))
				attribs[Attributes::BehaviorType]=~func->behavior_type;
		}

		copyAttributes(attribs);

		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Function::configureSearchAttributes()
{
	QStringList param_types;

	BaseObject::configureSearchAttributes();
	search_attribs[Attributes::ReturnType] = ret_table_columns.empty() ? *return_type : QString();

	for(auto &param : parameters)
		param_types += *param.getType();

	search_attribs[Attributes::Type] = param_types.join("; ");
}
