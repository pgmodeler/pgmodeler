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

#include "reference.h"

Reference::Reference(void)
{
	this->table=nullptr;
	this->column=nullptr;
	this->is_def_expr=false;
}

Reference::Reference(Table *table, Column *column, const QString &tab_alias, const QString &col_alias)
{
	//Raises an error if the table is not allocated
	if(!table)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the table/column alias has an invalid name
	else if((!tab_alias.isEmpty() && !BaseObject::isValidName(tab_alias)) ||
					(!col_alias.isEmpty() && !BaseObject::isValidName(col_alias)))
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the column parent table differs from the passed table
	else if(column && column->getParentTable()!=table)
		throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->table=table;
	this->column=column;
	this->alias=tab_alias;
	this->column_alias=col_alias;
	this->is_def_expr=false;
}

Reference::Reference(const QString &expression, const QString &expr_alias)
{
	//Raises an error if the user try to create an reference using an empty expression
	if(expression=="")
		throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the expression alias has an invalid name
	else if(!expr_alias.isEmpty() && !BaseObject::isValidName(expr_alias))
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	table=nullptr;
	column=nullptr;
	alias=expr_alias;
	this->expression=expression;
	this->is_def_expr=false;
}

void Reference::setDefinitionExpression(bool value)
{
	is_def_expr=value;
}

bool Reference::isDefinitionExpression(void)
{
	return(is_def_expr);
}

Table *Reference::getTable(void)
{
	return(table);
}

Column *Reference::getColumn(void)
{
	return(column);
}

QString Reference::getColumnAlias(void)
{
	return(column_alias);
}

QString Reference::getAlias(void)
{
	return(alias);
}

QString Reference::getExpression(void)
{
	return(expression);
}

unsigned Reference::getReferenceType(void)
{
	if(expression=="")
		return(REFER_COLUMN);
	else
		return(REFER_EXPRESSION);
}

QString Reference::getSQLDefinition(unsigned sql_type)
{
	QString sql_def, tab_name;
	unsigned refer_type;

	refer_type=getReferenceType();

	//Case the reference is between the SELECT-FROM keywords
	if(sql_type==SQL_REFER_SELECT)
	{
		//Case the reference is linked to a column
		if(refer_type==REFER_COLUMN)
		{
			/* Generated SQL definition:
			[TABLE_ALIAS.]{COLUMN_NAME | *} [AS COLUMN_ALIAS] */

			if(!alias.isEmpty())
				tab_name=BaseObject::formatName(alias) + ".";

			/* Case there is no column definede the default behavior is consider
			all the table columns (e.g. table.*) */
			if(!column)
				sql_def=tab_name + "*";
			else
			{
				//Case there is an column concatenates its name to the code definition
				sql_def=tab_name + column->getName(true);

				//Case there is a column alias concatenate it to the definition
				if(column_alias!="")
					sql_def+=" AS " + BaseObject::formatName(column_alias);
			}
		}
		//Case the reference is linked to an expression
		else
		{
			/* Generated SQL definition:
			{expression} [AS ALIAS] */
			sql_def=expression;
			if(alias!="")
				sql_def+=" AS " + BaseObject::formatName(alias);
		}
		sql_def+=", ";
	}
	//Case the reference is between the FROM-[JOIN | WHERE] keywords
	else if(sql_type==SQL_REFER_FROM)
	{
		/* Case the reference is linked to a column only the table name is used.
		 For expression the complete code is used thus the generated code is:

		 ... FROM {TABLE_NAME} [AS ALIAS] or
		 ... FROM {EXPRESSION} */
		if(refer_type==REFER_COLUMN)
		{
			sql_def+=table->getName(true);

			if(alias!="")
				sql_def+=" AS " + BaseObject::formatName(alias);
			sql_def+=", ";
		}
		else
			sql_def=expression;
	}
	//Case the reference is after [JOIN | WHERE] keywords
	else
	{
		//Case the column is allocated
		if(refer_type==REFER_COLUMN && column)
		{
			/* Generated SQL definition:
			... WHERE {TABLE_NAME | ALIAS}.{COLUMN_NAME} */

			if(alias=="")
				sql_def=table->getName(true);
			else
				sql_def=BaseObject::formatName(alias);

			sql_def+=".";

			if(column)
				sql_def+=column->getName(true);
		}
		else if(refer_type==REFER_EXPRESSION)
			sql_def=expression;
	}

	return(sql_def);
}

QString Reference::getXMLDefinition(void)
{
	map<QString, QString> attribs;

	attribs[ParsersAttributes::TABLE]="";
	attribs[ParsersAttributes::COLUMN]="";

	if(table)
		attribs[ParsersAttributes::TABLE]=table->getName(true);

	if(column)
		attribs[ParsersAttributes::COLUMN]=column->getName();

	attribs[ParsersAttributes::EXPRESSION]=expression;
	attribs[ParsersAttributes::ALIAS]=alias;
	attribs[ParsersAttributes::COLUMN_ALIAS]=column_alias;

	return(SchemaParser::getCodeDefinition(ParsersAttributes::REFERENCE,
																				 attribs, SchemaParser::XML_DEFINITION));
}

bool Reference::operator == (Reference &refer)
{
	unsigned ref_type;

	ref_type=this->getReferenceType();

	if(ref_type==refer.getReferenceType())
	{
		if(ref_type==REFER_COLUMN)
		{
			return(this->table==refer.table &&
						 this->column==refer.column &&
						 this->alias==refer.alias &&
						 this->column_alias==refer.column_alias);
		}
		else
		{
			return(this->expression==refer.expression &&
						 this->alias==refer.alias &&
						 this->is_def_expr==refer.is_def_expr);
		}
	}
	else
		return(false);
}

