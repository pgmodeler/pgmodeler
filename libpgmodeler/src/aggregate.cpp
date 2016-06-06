/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
	functions[0]=functions[1]=nullptr;
	sort_operator=nullptr;
	attributes[ParsersAttributes::TYPES]=QString();
	attributes[ParsersAttributes::TRANSITION_FUNC]=QString();
	attributes[ParsersAttributes::STATE_TYPE]=QString();
	attributes[ParsersAttributes::BASE_TYPE]=QString();
	attributes[ParsersAttributes::FINAL_FUNC]=QString();
	attributes[ParsersAttributes::INITIAL_COND]=QString();
	attributes[ParsersAttributes::SORT_OP]=QString();
}

void Aggregate::setFunction(unsigned func_idx, Function *func)
{
	//Case the function index is invalid raises an error
	if(func_idx!=FINAL_FUNC && func_idx!=TRANSITION_FUNC)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Checks if the function is valid, if not the case raises an error
	if(!isValidFunction(func_idx, func))
		throw Exception(Exception::getErrorMessage(ERR_USING_INV_FUNC_CONFIG)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(OBJ_AGGREGATE)),
						ERR_USING_INV_FUNC_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(functions[func_idx]!=func);
	functions[func_idx]=func;
}

bool Aggregate::isValidFunction(unsigned func_idx, Function *func)
{
	if(func)
	{
		if(func_idx==FINAL_FUNC)
		{
			/* According to docs the final function to be valid must have 1 parameter which type is the
					same as the state_type attribute. BUT when importing system aggregates some functions has
					different parameter counts so in order to import them correctly we remove the restriction
					of one paramenter for this function */

			/*return((func->getParameterCount()==1 ||
								func->getParameter(0).getType().canCastTo(state_type));*/

			return(func->getParameterCount() > 0 &&
						 func->getParameter(0).getType().canCastTo(state_type));
		}
		else
		{
			unsigned qtd, i;
			bool cond1,cond2=true;

			/* The transition function must have n+1 parameters, where n is the accepted data types list size.
			Also, the first parameter of the function and the return type must be the same as the 'state_type'
			attribute. Lastly, the other parameters must be the same as the accepted data types (the appearece order
			is important here).

			IMPORTANT: this is not documented by aggregate docs but when trying to import some catalog aggregates the
			majority of the functions used by them has polymorphic parameters so in order to accept that situation
			and recreate aggregates in the model we enable the usage of polymorphic functions here */

			cond1=(func->getReturnType().canCastTo(state_type)) &&
					((func->getParameterCount()==data_types.size() + 1) ||
						(func->getParameterCount() > 0 &&
							func->getParameter(func->getParameterCount()-1).getType().isPolymorphicType())) &&
					(func->getParameter(0).getType().canCastTo(state_type));

			qtd=func->getParameterCount();
			for(i=1 ; i < qtd && cond2; i++)
				cond2=(func->getParameter(i).getType().isPolymorphicType() ||
							 func->getParameter(i).getType().canCastTo(data_types[i-1]));

			return(cond1 && cond2);
		}
	}
	else return(true);
}

void Aggregate::setStateType(PgSQLType state_type)
{
	setCodeInvalidated(this->state_type != state_type);
	this->state_type=state_type;
}

void Aggregate::setInitialCondition(const QString &cond)
{
	setCodeInvalidated(initial_condition != cond);
	initial_condition=cond;
}

void Aggregate::setSortOperator(Operator *sort_op)
{
	if(sort_op)
	{
		Function *func=nullptr;

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

	setCodeInvalidated(sort_operator != sort_op);
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
			str_types+=data_types[i].getCodeDefinition(SchemaParser::SQL_DEFINITION);
			if(i < (count-1)) str_types+=',';
		}
		else str_types+=data_types[i].getCodeDefinition(def_type);
	}

	/* Case none data type is specified for the aggregate creates
		an aggregate that accepts any possible data '*' e.g. function(*) */
	if(str_types.isEmpty()) str_types='*';

	attributes[ParsersAttributes::TYPES]=str_types;
}

void Aggregate::addDataType(PgSQLType type)
{
	data_types.push_back(type);
	setCodeInvalidated(true);
}

void Aggregate::removeDataType(unsigned type_idx)
{
	//Raises an exception if the type index is out of bound
	if(type_idx >= data_types.size())
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Removes the type at the specified position
	data_types.erase(data_types.begin() + type_idx);
	setCodeInvalidated(true);
}

void Aggregate::removeDataTypes(void)
{
	data_types.clear();
	setCodeInvalidated(true);
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
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

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

	if(!initial_condition.isEmpty())
		attributes[ParsersAttributes::INITIAL_COND]=initial_condition;

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::STATE_TYPE]=*(state_type);
	else
		attributes[ParsersAttributes::STATE_TYPE]=state_type.getCodeDefinition(def_type,ParsersAttributes::STATE_TYPE);

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Aggregate::getDropDefinition(bool cascade)
{
	setTypesAttribute(SchemaParser::SQL_DEFINITION);
	return(BaseObject::getDropDefinition(cascade));
}

QString Aggregate::getAlterDefinition(BaseObject *object)
{
	try
	{
		setTypesAttribute(SchemaParser::SQL_DEFINITION);
		return(BaseObject::getAlterDefinition(object));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

QString Aggregate::getSignature(bool format)
{
	QStringList types;

	if(data_types.empty())
		types.push_back(QString("*"));
	else
	{
		for(auto &tp : data_types)
			types.push_back(tp.getCodeDefinition(SchemaParser::SQL_DEFINITION));
	}

	return(BaseObject::getSignature(format) + QString("(%1)").arg(types.join(',')));
}

