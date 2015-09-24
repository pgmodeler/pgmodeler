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

#include "function.h"

Function::Function(void)
{
  return_type=PgSQLType(QString("void"));
	language=nullptr;
	returns_setof=false;
	is_wnd_function=false;
	is_leakproof=false;
	obj_type=OBJ_FUNCTION;

	execution_cost=100;
	row_amount=1000;

	attributes[ParsersAttributes::PARAMETERS]=QString();
	attributes[ParsersAttributes::EXECUTION_COST]=QString();
	attributes[ParsersAttributes::ROW_AMOUNT]=QString();
	attributes[ParsersAttributes::RETURN_TYPE]=QString();
	attributes[ParsersAttributes::FUNCTION_TYPE]=QString();
	attributes[ParsersAttributes::LANGUAGE]=QString();
	attributes[ParsersAttributes::RETURNS_SETOF]=QString();
	attributes[ParsersAttributes::SECURITY_TYPE]=QString();
	attributes[ParsersAttributes::BEHAVIOR_TYPE]=QString();
	attributes[ParsersAttributes::DEFINITION]=QString();
	attributes[ParsersAttributes::SIGNATURE]=QString();
	attributes[ParsersAttributes::REF_TYPE]=QString();
	attributes[ParsersAttributes::WINDOW_FUNC]=QString();
	attributes[ParsersAttributes::RETURN_TABLE]=QString();
	attributes[ParsersAttributes::LIBRARY]=QString();
	attributes[ParsersAttributes::SYMBOL]=QString();
	attributes[ParsersAttributes::LEAKPROOF]=QString();
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
                    .arg(param.getName())
                    .arg(this->signature),
										ERR_ASG_DUPLIC_PARAM_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Inserts the parameter in the function
	parameters.push_back(param);
	createSignature();
}

void Function::addReturnedTableColumn(const QString &name, PgSQLType type)
{
	//Raises an error if the column name is empty
  if(name.isEmpty())
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
                    .arg(name)
                    .arg(this->signature),
										ERR_INS_DUPLIC_RET_TAB_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(language->getName().toLower()!=~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                    .arg(this->getSignature()),
										ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->library != library);
	this->library=library;
}

void Function::setSymbol(const QString &symbol)
{
	if(language->getName().toLower()!=~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                    .arg(this->getSignature()),
										ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->symbol != symbol);
	this->symbol=symbol;
}

void Function::setReturnType(PgSQLType type)
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
		throw Exception(ERR_ASG_NOT_ALOC_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the language object is invalid
	else if(language->getObjectType()!=OBJ_LANGUAGE)
		throw Exception(ERR_ASG_INV_LANGUAGE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(language && language->getName().toLower()==~LanguageType("c"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_CODE_FUNC_C_LANGUAGE)
                    .arg(this->getSignature()),
										ERR_ASG_CODE_FUNC_C_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->source_code != src_code);
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

bool Function::isLeakProof(void)
{
	return(is_leakproof);
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
	setCodeInvalidated(true);
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
	setCodeInvalidated(true);
}

QString Function::getSignature(bool)
{
	return(signature);
}

void Function::createSignature(bool format, bool prepend_schema)
{
	QString str_param;
	unsigned i, count;

	count=parameters.size();
	for(i=0; i < count; i++)
	{
		//OUT parameters is not part of function's signature
    if(!parameters[i].isOut() || parameters[i].isVariadic() ||
			 (parameters[i].isIn() && parameters[i].isOut()) ||
       (parameters[i].isIn() && !parameters[i].isOut()))
		{
			str_param+=parameters[i].getCodeDefinition(SchemaParser::SQL_DEFINITION, true).trimmed();
			parameters[i].setCodeInvalidated(true);
		}
	}

	str_param.remove(str_param.length()-1, 1);

	//Signature format NAME(IN|OUT PARAM1_TYPE,IN|OUT PARAM2_TYPE,...,IN|OUT PARAMn_TYPE)
  signature=this->getName(format, prepend_schema) + QString("(") + str_param + QString(")");
	this->setCodeInvalidated(true);
}

QString Function::getCodeDefinition(unsigned def_type)
{
  return(this->getCodeDefinition(def_type, false));
}

QString Function::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return(code_def);

	setParametersAttribute(def_type);

	attributes[ParsersAttributes::EXECUTION_COST]=QString("%1").arg(execution_cost);
	attributes[ParsersAttributes::ROW_AMOUNT]=QString("%1").arg(row_amount);
	attributes[ParsersAttributes::FUNCTION_TYPE]=(~function_type);

	if(language)
	{
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
	}

	setTableReturnTypeAttribute(def_type);

	attributes[ParsersAttributes::RETURNS_SETOF]=(returns_setof ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::WINDOW_FUNC]=(is_wnd_function ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::LEAKPROOF]=(is_leakproof ? ParsersAttributes::_TRUE_ : QString());
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

QString Function::getAlterDefinition(BaseObject *object)
{
  try
  {
    Function *func=dynamic_cast<Function *>(object);
    attribs_map attribs;

    attributes[ParsersAttributes::ALTER_CMDS]=BaseObject::getAlterDefinition(object);

    if(this->source_code.simplified()!=func->source_code.simplified() ||
       this->library!=func->library ||
       this->symbol!=func->symbol)
    {
      attribs[ParsersAttributes::DEFINITION]=func->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      attribs[ParsersAttributes::DEFINITION].replace(QString("CREATE FUNCTION"), QString("CREATE OR REPLACE FUNCTION"));
      attribs[ParsersAttributes::DEFINITION].size();
    }
    else
    {
      if(this->execution_cost!=func->execution_cost)
        attribs[ParsersAttributes::EXECUTION_COST]=QString::number(func->execution_cost);

      if(this->returns_setof && func->returns_setof && this->row_amount!=func->row_amount)
      {
        attribs[ParsersAttributes::RETURNS_SETOF]=ParsersAttributes::_TRUE_;
        attribs[ParsersAttributes::ROW_AMOUNT]=QString::number(row_amount);
      }

      if(this->function_type!=func->function_type)
        attribs[ParsersAttributes::FUNCTION_TYPE]=~func->function_type;

      if(this->is_leakproof!=func->is_leakproof)
        attribs[ParsersAttributes::LEAKPROOF]=(func->is_leakproof ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);

      if(this->security_type!=func->security_type)
        attribs[ParsersAttributes::SECURITY_TYPE]=~func->security_type;

      if((this->behavior_type!=func->behavior_type) &&
         ((this->behavior_type==BehaviorType::called_on_null_input) ||
          ((this->behavior_type==BehaviorType::strict || this->behavior_type==BehaviorType::returns_null_on_null_input) &&
           func->function_type==BehaviorType::called_on_null_input)))
        attribs[ParsersAttributes::BEHAVIOR_TYPE]=~func->behavior_type;
    }

    copyAttributes(attribs);

    return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
