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

#include "operatorclasselement.h"

OperatorClassElement::OperatorClassElement(void)
{
	element_type=OPERATOR_ELEM;
	function=NULL;
	_operator=NULL;
	recheck=false;
	strategy_number=0;
}

void OperatorClassElement::setFunction(Function *func, unsigned stg_number)
{
	//Raises an exception case the function is not allocated
	if(!func)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error case the stratagy number is zero (invalid)
	else if(stg_number==0)
		throw Exception(ERR_ASG_INV_SUPSTG_NUMBER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Clear the attributes not related to the FUNCTION element type
	this->_operator=NULL;

	//Configure the attributes that belongs to the element type
	this->function=func;
	this->strategy_number=stg_number;
	this->element_type=FUNCTION_ELEM;
}

void OperatorClassElement::setOperator(Operator *oper, unsigned stg_number, bool recheck)
{
	//Raises an error in case the operator is not allocated
	if(!oper)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error case the stratagy number is zero (invalid)
	else if(stg_number==0)
		throw Exception(ERR_ASG_INV_SUPSTG_NUMBER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Clear the attributes not related to the OPERATOR element type
	this->function=NULL;

	//Configure the attributes that belongs to the element type
	this->_operator=oper;
	this->strategy_number=stg_number;
	this->recheck=recheck;
	this->element_type=OPERATOR_ELEM;
}

void OperatorClassElement::setStorage(PgSQLType storage)
{
	//Clear the attributes not related to the STORAGE element type
	this->function=NULL;
	this->_operator=NULL;
	this->strategy_number=0;

	//Configure the attributes that belongs to the element type
	this->storage=storage;
	this->element_type=STORAGE_ELEM;
}

unsigned OperatorClassElement::getElementType(void)
{
	return(element_type);
}

Function *OperatorClassElement::getFunction(void)
{
	return(function);
}

Operator *OperatorClassElement::getOperator(void)
{
	return(_operator);
}

PgSQLType OperatorClassElement::getStorage(void)
{
	return(storage);
}

bool OperatorClassElement::isRecheck(void)
{
	return(recheck);
}

unsigned OperatorClassElement::getStrategyNumber(void)
{
	return(strategy_number);
}

QString OperatorClassElement::getCodeDefinition(unsigned def_type)
{
	map<QString, QString> attributes;

	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::STRATEGY_NUM]="";
	attributes[ParsersAttributes::SIGNATURE]="";
	attributes[ParsersAttributes::RECHECK]="";
	attributes[ParsersAttributes::FUNCTION]="";
	attributes[ParsersAttributes::OPERATOR]="";
	attributes[ParsersAttributes::STORAGE]="";
	attributes[ParsersAttributes::DEFINITION]="";

	if(element_type==FUNCTION_ELEM && function && strategy_number > 0)
	{
		//FUNCTION support_number [ ( op_type [ , op_type ] ) ] funcname ( argument_type [, ...] )
		attributes[ParsersAttributes::FUNCTION]="1";
		attributes[ParsersAttributes::STRATEGY_NUM]=QString("%1").arg(strategy_number);

		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::SIGNATURE]=function->getSignature();
		else
			attributes[ParsersAttributes::DEFINITION]=function->getCodeDefinition(def_type,true);
	}
	else if(element_type==OPERATOR_ELEM && _operator && strategy_number > 0)
	{
		//OPERATOR strategy_number operator_name [ ( op_type, op_type ) ] [ RECHECK ]
		attributes[ParsersAttributes::OPERATOR]="1";
		attributes[ParsersAttributes::STRATEGY_NUM]=QString("%1").arg(strategy_number);
		if(recheck) attributes[ParsersAttributes::RECHECK]="1";

		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::SIGNATURE]=_operator->getSignature();
		else
			attributes[ParsersAttributes::DEFINITION]=_operator->getCodeDefinition(def_type,true);
	}
	else if(element_type==STORAGE_ELEM && storage!=PgSQLType::null)
	{
		//STORAGE storage_type
		attributes[ParsersAttributes::STORAGE]="1";

		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::TYPE]=(*storage);
		else
			attributes[ParsersAttributes::DEFINITION]=storage.getCodeDefinition(def_type);
	}

	return(SchemaParser::getCodeDefinition(ParsersAttributes::ELEMENT,attributes, def_type));
}

bool OperatorClassElement::operator == (OperatorClassElement &elem)
{
	return(this->element_type == elem.element_type &&
				 this->storage == elem.storage &&
				 this->function == elem.function &&
				 this->_operator == elem._operator &&
				 this->strategy_number == elem.strategy_number);
}

