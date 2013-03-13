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

#include "aggregate.h"

Aggregate::Aggregate(void)
{
	obj_type=OBJ_AGGREGATE;
	functions[0]=functions[1]=NULL;
	sort_operator=NULL;
	attributes[ParsersAttributes::TYPES]="";
	attributes[ParsersAttributes::TRANSITION_FUNC]="";
	attributes[ParsersAttributes::STATE_TYPE]="";
	attributes[ParsersAttributes::BASE_TYPE]="";
	attributes[ParsersAttributes::FINAL_FUNC]="";
	attributes[ParsersAttributes::INITIAL_COND]="";
	attributes[ParsersAttributes::SORT_OP]="";
}

void Aggregate::setFunction(unsigned func_idx, Function *func)
{
	//Case the function index is invalid raises an error
	if(func_idx!=FINAL_FUNC && func_idx!=TRANSITION_FUNC)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Checks if the function is valid, if not the case raises an error
	if(!isValidFunction(func_idx, func))
		throw Exception(Exception::getErrorMessage(ERR_USING_INV_FUNC_CONFIG)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_AGGREGATE)),
										ERR_USING_INV_FUNC_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	functions[func_idx]=func;
}

bool Aggregate::isValidFunction(unsigned func_idx, Function *func)
{
	if(func)
	{
		if(func_idx==FINAL_FUNC)
		{
			/* The final function to be valid must have 1 parameter which type is the
			same as the state_type attribute */
			return((func->getParameterCount()==1 &&
							func->getParameter(0).getType()==state_type));
		}
		else
		{
			unsigned qtd, i;
			bool cond1,cond2=true;

			/* The transition function must have n+1 parameters, where n is the accepted data types list size.
			Also, the first parameter of the function and the return type must be the same as the 'state_type'
			attribute. Lastly, de other parameters must be the same as the accepted data types (the appearece order
			is important here) */
			cond1=(func->getReturnType()==state_type) &&
						(func->getParameterCount()==data_types.size() + 1) &&
						(func->getParameter(0).getType()==state_type);

			qtd=func->getParameterCount();
			for(i=1 ; i < qtd && cond2; i++)
				cond2=(func->getParameter(i).getType()==data_types[i-1]);

			return(cond1 && cond2);
		}
	}
	else return(true);
}

void Aggregate::setStateType(PgSQLType state_type)
{
	this->state_type=state_type;
}

void Aggregate::setInitialCondition(const QString &cond)
{
	initial_condition=cond;
}

void Aggregate::setSortOperator(Operator *sort_op)
{
	if(sort_op)
	{
		Function *func=NULL;

		/* Accordingly to the documentation, a sort operator only can be assigned
		 to the aggregate when:

		 1) The aggregate accepts only one data type
		 2) The function that defines the operator has the parameter types identical
				as the input data type of the aggregate  */
		func=sort_op->getFunction(Operator::FUNC_OPERATOR);
		//Validating the condition 1
		if(data_types.size()!=1)
			throw Exception(ERR_ASG_INV_OPER_ARGS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Validating the condition 2
		else if(func->getParameter(0).getType()!=data_types[0] ||
						(func->getParameterCount()==2 && func->getParameter(1).getType()!=data_types[0]))
			throw Exception(ERR_ASG_INV_OPERATOR_TYPES,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	this->sort_operator=sort_op;
}

void Aggregate::setTypesAttribute(unsigned def_type)
{
	QString str_types;
	unsigned i, count;

	count=data_types.size();
	for(i=0; i < count; i++)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
		{
			str_types+=*(data_types[i]);
			if(i < (count-1)) str_types+=",";
		}
		else str_types+=data_types[i].getCodeDefinition(def_type);
	}

	/* Case none data type is specified for the aggregate creates
		an aggregate that accepts any possible data '*' e.g. function(*) */
	if(str_types.isEmpty()) str_types="*";

	attributes[ParsersAttributes::TYPES]=str_types;
}

void Aggregate::addDataType(PgSQLType type)
{
	//Case the data type already exists in the aggregate raise an exception
	if(isDataTypeExist(type))
		throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_TYPE)
										.arg(Utf8String::create(~type))
										.arg(Utf8String::create(this->getName(true))),
										ERR_INS_DUPLIC_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	data_types.push_back(type);
}

void Aggregate::removeDataType(unsigned type_idx)
{
	//Raises an exception if the type index is out of bound
	if(type_idx >= data_types.size())
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Removes the type at the specified position
	data_types.erase(data_types.begin() + type_idx);
}

void Aggregate::removeDataTypes(void)
{
	data_types.clear();
}

bool Aggregate::isDataTypeExist(PgSQLType type)
{
	vector<PgSQLType>::iterator itr, itr_end;
	bool enc=false;

	itr=data_types.begin();
	itr_end=data_types.end();

	while(itr!=itr_end && !enc)
	{
		enc=((*itr)==type);
		itr++;
	}

	return(enc);
}

unsigned Aggregate::getDataTypeCount(void)
{
	return(data_types.size());
}

Function *Aggregate::getFunction(unsigned func_idx)
{
	//Raises an exception if the function index is invalid
	if(func_idx!=FINAL_FUNC && func_idx!=TRANSITION_FUNC)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(functions[func_idx]);
}

PgSQLType Aggregate::getStateType(void)
{
	return(state_type);
}

QString Aggregate::getInitialCondition(void)
{
	return(initial_condition);
}

Operator *Aggregate::getSortOperator(void)
{
	return(sort_operator);
}

PgSQLType Aggregate::getDataType(unsigned type_idx)
{
	//Raises an exception if the type index is out of bound
	if(type_idx >= data_types.size())
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(data_types[type_idx]);
}

QString Aggregate::getCodeDefinition(unsigned def_type)
{
	setTypesAttribute(def_type);

	if(functions[TRANSITION_FUNC])
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::TRANSITION_FUNC]=functions[TRANSITION_FUNC]->getSignature();
		else
		{
			functions[TRANSITION_FUNC]->setAttribute(ParsersAttributes::REF_TYPE,
																							 ParsersAttributes::TRANSITION_FUNC);
			attributes[ParsersAttributes::TRANSITION_FUNC]=functions[TRANSITION_FUNC]->getCodeDefinition(def_type,true);
		}
	}

	if(functions[FINAL_FUNC])
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::FINAL_FUNC]=functions[FINAL_FUNC]->getSignature();
		else
		{
			functions[FINAL_FUNC]->setAttribute(ParsersAttributes::REF_TYPE,
																					ParsersAttributes::FINAL_FUNC);
			attributes[ParsersAttributes::FINAL_FUNC]=functions[FINAL_FUNC]->getCodeDefinition(def_type,true);
		}
	}

	if(sort_operator)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::SORT_OP]=sort_operator->getName(true);
		else
			attributes[ParsersAttributes::SORT_OP]=sort_operator->getCodeDefinition(def_type,true);
	}

	if(initial_condition!="")
		attributes[ParsersAttributes::INITIAL_COND]=initial_condition;

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::STATE_TYPE]=*(state_type);
	else
		attributes[ParsersAttributes::STATE_TYPE]=state_type.getCodeDefinition(def_type,ParsersAttributes::STATE_TYPE);

	return(BaseObject::__getCodeDefinition(def_type));
}

