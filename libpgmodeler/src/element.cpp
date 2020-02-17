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

#include "element.h"

Element::Element()
{
	column=nullptr;
	operator_class=nullptr;
	sorting_attibs[NullsFirst]=false;
	sorting_attibs[AscOrder]=true;
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
	if(attrib > NullsFirst)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	sorting_attibs[attrib]=value;
}

void Element::setSortingEnabled(bool value)
{
	sorting_enabled=value;
}

bool Element::isSortingEnabled()
{
	return sorting_enabled;
}

bool Element::getSortingAttribute(unsigned attrib)
{
	if(attrib > NullsFirst)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return sorting_attibs[attrib];
}

Column *Element::getColumn()
{
	return column;
}

QString Element::getExpression()
{
	return expression;
}

OperatorClass *Element::getOperatorClass()
{
	return operator_class;
}

void Element::configureAttributes(attribs_map &attributes, unsigned def_type)
{
	attributes[Attributes::Column]=QString();
	attributes[Attributes::Expression]=QString();
	attributes[Attributes::OpClass]=QString();
	attributes[Attributes::UseSorting]=(this->sorting_enabled ? Attributes::True : QString());
	attributes[Attributes::NullsFirst]=(this->sorting_enabled && this->sorting_attibs[NullsFirst] ? Attributes::True : QString());
	attributes[Attributes::AscOrder]=(this->sorting_enabled && this->sorting_attibs[AscOrder] ? Attributes::True : QString());


	if(column)
		attributes[Attributes::Column]=column->getName(true);
	else
		attributes[Attributes::Expression]=expression;

	if(operator_class)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::OpClass]=operator_class->getName(true);
		else
			attributes[Attributes::OpClass]=operator_class->getCodeDefinition(def_type, true);
	}
}

bool Element::isEqualsTo(Element &elem)
{
  return (this->column == elem.column &&
		 this->expression == elem.expression &&
		 this->operator_class == elem.operator_class &&
		 this->sorting_enabled == elem.sorting_enabled &&
		 this->sorting_attibs[AscOrder] == elem.sorting_attibs[AscOrder] &&
		 this->sorting_attibs[NullsFirst] == elem.sorting_attibs[NullsFirst]);
}

bool Element::operator == (Element &elem)
{
	return isEqualsTo(elem);
}

bool Element::operator == (const Element &elem)
{
	return isEqualsTo(const_cast<Element &>(elem));
}
