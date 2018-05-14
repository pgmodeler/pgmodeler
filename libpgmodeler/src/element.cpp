/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "element.h"

Element::Element(void)
{
	column=nullptr;
	operator_class=nullptr;
	sorting_attibs[NULLS_FIRST]=false;
	sorting_attibs[ASC_ORDER]=true;
	sorting_enabled=false;
}

void Element::setColumn(Column *column)
{
	if(column)
	{
		this->column=column;
		this->expression=QString();
	}
}

void Element::setExpression(const QString &expression)
{
	if(!expression.isEmpty())
	{
		this->expression=expression;
		this->column=nullptr;
	}
}

void Element::setOperatorClass(OperatorClass *oper_class)
{
	this->operator_class=oper_class;
}

void Element::setSortingAttribute(unsigned attrib, bool value)
{
	if(attrib > NULLS_FIRST)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	sorting_attibs[attrib]=value;
}

void Element::setSortingEnabled(bool value)
{
	sorting_enabled=value;
}

bool Element::isSortingEnabled(void)
{
	return(sorting_enabled);
}

bool Element::getSortingAttribute(unsigned attrib)
{
	if(attrib > NULLS_FIRST)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(sorting_attibs[attrib]);
}

Column *Element::getColumn(void)
{
	return(column);
}

QString Element::getExpression(void)
{
	return(expression);
}

OperatorClass *Element::getOperatorClass(void)
{
	return(operator_class);
}

void Element::configureAttributes(attribs_map &attributes, unsigned def_type)
{
	attributes[ParsersAttributes::COLUMN]=QString();
	attributes[ParsersAttributes::EXPRESSION]=QString();
	attributes[ParsersAttributes::OP_CLASS]=QString();
	attributes[ParsersAttributes::USE_SORTING]=(this->sorting_enabled ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::NULLS_FIRST]=(this->sorting_enabled && this->sorting_attibs[NULLS_FIRST] ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::ASC_ORDER]=(this->sorting_enabled && this->sorting_attibs[ASC_ORDER] ? ParsersAttributes::_TRUE_ : QString());


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
}

bool Element::operator == (Element &elem)
{
	return(this->column == elem.column &&
		   this->expression == elem.expression &&
		   this->operator_class == elem.operator_class &&
		   this->sorting_enabled == elem.sorting_enabled &&
		   this->sorting_attibs[ASC_ORDER] == elem.sorting_attibs[ASC_ORDER] &&
		   this->sorting_attibs[NULLS_FIRST] == elem.sorting_attibs[NULLS_FIRST]);
}
