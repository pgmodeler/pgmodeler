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

#include "indexelement.h"

IndexElement::IndexElement(void)
{
	column=NULL;
	operator_class=NULL;
	sorting_attibs[NULLS_FIRST]=false;
	sorting_attibs[ASC_ORDER]=true;
	sorting_enabled=false;
}

void IndexElement::setColumn(Column *column)
{
	if(column)
	{
		this->column=column;
		this->expression="";
	}
}

void IndexElement::setExpression(const QString &expression)
{
	if(!expression.isEmpty())
	{
		this->expression=expression;
		this->column=NULL;
	}
}

void IndexElement::setOperatorClass(OperatorClass *oper_class)
{
	this->operator_class=oper_class;
}

void IndexElement::setSortingAttribute(unsigned attrib, bool value)
{
	if(attrib > NULLS_FIRST)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	sorting_attibs[attrib]=value;
}

void IndexElement::setSortingEnabled(bool value)
{
	sorting_enabled=value;
}

bool IndexElement::isSortingEnabled(void)
{
	return(sorting_enabled);
}

bool IndexElement::getSortingAttribute(unsigned attrib)
{
	if(attrib > NULLS_FIRST)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(sorting_attibs[attrib]);
}

Column *IndexElement::getColumn(void)
{
	return(column);
}

QString IndexElement::getExpression(void)
{
	return(expression);
}

OperatorClass *IndexElement::getOperatorClass(void)
{
	return(operator_class);
}

QString IndexElement::getCodeDefinition(unsigned def_type)
{
	map<QString, QString> attributes;

	attributes[ParsersAttributes::COLUMN]="";
	attributes[ParsersAttributes::EXPRESSION]="";
	attributes[ParsersAttributes::OP_CLASS]="";

	attributes[ParsersAttributes::USE_SORTING]=(this->sorting_enabled ? "1" : "");
	attributes[ParsersAttributes::NULLS_FIRST]=(this->sorting_enabled && this->sorting_attibs[NULLS_FIRST] ? "1" : "");
	attributes[ParsersAttributes::ASC_ORDER]=(this->sorting_enabled && this->sorting_attibs[ASC_ORDER] ? "1" : "");


	if(column)
		attributes[ParsersAttributes::COLUMN]=column->getName(true);
	else
		attributes[ParsersAttributes::EXPRESSION]=expression;

	if(operator_class)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::OP_CLASS]=operator_class->getName(true);
		else
			attributes[ParsersAttributes::OP_CLASS]=operator_class->getCodeDefinition(def_type, true);
	}

	return(SchemaParser::getCodeDefinition(ParsersAttributes::INDEX_ELEMENT,attributes, def_type));
}

