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

#include "reference.h"

Reference::Reference()
{
	this->table=nullptr;
	this->column=nullptr;
	this->is_def_expr=false;
}

Reference::Reference(PhysicalTable *table, Column *column, const QString &tab_alias, const QString &col_alias)
{
	//Raises an error if the table is not allocated
	if(!table)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the table/column alias has an invalid name
	else if((!tab_alias.isEmpty() && !BaseObject::isValidName(tab_alias)) ||
					(!col_alias.isEmpty() && !BaseObject::isValidName(col_alias)))
		throw Exception(ErrorCode::AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the column parent table differs from the passed table
	else if(column && column->getParentTable()!=table)
		throw Exception(ErrorCode::AsgObjectBelongsAnotherTable ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->table=table;
	this->column=column;
	this->alias=tab_alias;
	this->column_alias=col_alias;
	this->is_def_expr=false;
}

Reference::Reference(const QString &expression, const QString &expr_alias)
{
	//Raises an error if the user try to create an reference using an empty expression
	if(expression.isEmpty())
		throw Exception(ErrorCode::AsgInvalidExpressionObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the expression alias has an invalid name
	else if(!expr_alias.isEmpty() && !BaseObject::isValidName(expr_alias))
		throw Exception(ErrorCode::AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	table=nullptr;
	column=nullptr;
	alias=expr_alias;
	this->expression=expression;
	this->is_def_expr=false;
}

void Reference::setDefinitionExpression(bool value)
{
	is_def_expr = value;
	if(!value) clearReferencedTables();
}

void Reference::addReferencedTable(PhysicalTable *ref_table)
{
	if(!ref_table)
		return;

	if(std::find(ref_tables.begin(), ref_tables.end(), ref_table) == ref_tables.end())
		ref_tables.push_back(ref_table);
}

int Reference::getReferencedTableIndex(PhysicalTable *ref_table)
{
	int idx = -1;
	vector<PhysicalTable *>::iterator itr = std::find(ref_tables.begin(), ref_tables.end(), ref_table);

	if(itr != ref_tables.end())
		idx = itr - ref_tables.begin();

	return idx;
}

void Reference::clearReferencedTables()
{
	ref_tables.clear();
}

vector<PhysicalTable *> Reference::getReferencedTables()
{
	return ref_tables;
}

bool Reference::isDefinitionExpression()
{
	return is_def_expr;
}

void Reference::addColumn(const QString &name, PgSqlType type, const QString &alias)
{
	QString aux_name = name;

	if(aux_name.startsWith(QChar('\"')) &&
		 aux_name.endsWith(QChar('\"')))
	{
		aux_name.remove(0, 1);
		aux_name.remove(aux_name.length(), 1);
	}

	// Validating the column name
	if(!BaseObject::isValidName(name))
	{
		if(aux_name.isEmpty())
			throw Exception(ErrorCode::AsgEmptyNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(aux_name.size() > BaseObject::ObjectNameMaxLength)
			throw Exception(ErrorCode::AsgLongNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
			throw Exception(ErrorCode::AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	// Checking if the column already exists
	for(auto &col : columns)
	{
		if(col.name == name)
			throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	columns.push_back(SimpleColumn(name, *type, alias));
}

void Reference::addColumn(Column *col)
{
	if(!col)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	addColumn(col->getName(), col->getType(), col->getAlias());
}

void Reference::removeColumns()
{
	columns.clear();
}

vector<SimpleColumn> Reference::getColumns()
{
	return columns;
}

PhysicalTable *Reference::getTable()
{
	return table;
}

Column *Reference::getColumn()
{
	return column;
}

QString Reference::getColumnAlias()
{
	return column_alias;
}

QString Reference::getAlias()
{
	return alias;
}

QString Reference::getExpression()
{
	return expression;
}

unsigned Reference::getReferenceType()
{
	if(expression.isEmpty())
		return ReferColumn;
	else
		return ReferExpression;
}

void Reference::setReferenceAlias(const QString &alias)
{
	if(alias.size() > BaseObject::ObjectNameMaxLength)
		throw Exception(ErrorCode::AsgLongNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	ref_alias = alias;
}

QString Reference::getReferenceAlias()
{
	return ref_alias;
}

QString Reference::getSQLDefinition(unsigned sql_type)
{
	QString sql_def, tab_name;
	unsigned refer_type;

	refer_type=getReferenceType();

	//Case the reference is between the SELECT-FROM keywords
	if(sql_type==SqlReferSelect)
	{
		//Case the reference is linked to a column
		if(refer_type==ReferColumn)
		{
			/* Generated SQL definition:
			[TABLE_ALIAS.]{COLUMN_NAME | *} [AS COLUMN_ALIAS] */

			if(!alias.isEmpty())
				tab_name=BaseObject::formatName(alias) + QString(".");
			else
				tab_name=table->getSignature() + QString(".");

			/* Case there is no column definede the default behavior is consider
			all the table columns (e.g. table.*) */
			if(!column)
				sql_def=tab_name + QString("*");
			else
			{
				//Case there is an column concatenates its name to the code definition
				sql_def=tab_name + column->getName(true);

				//Case there is a column alias concatenate it to the definition
				if(!column_alias.isEmpty())
					sql_def+=QString(" AS ") + BaseObject::formatName(column_alias);
			}
		}
		//Case the reference is linked to an expression
		else
		{
			/* Generated SQL definition:
			{expression} [AS ALIAS] */
			sql_def=expression;
			if(!alias.isEmpty())
				sql_def+=QString(" AS ") + BaseObject::formatName(alias);
		}
		sql_def+=QString(",\n");
	}
	//Case the reference is between the FROM-[JOIN | WHERE] keywords
	else if(sql_type==SqlReferFrom)
	{
		/* Case the reference is linked to a column only the table name is used.
		 For expression the complete code is used thus the generated code is:

		 ... FROM {TABLE_NAME} [AS ALIAS] or
		 ... FROM {EXPRESSION} */
		if(refer_type==ReferColumn)
		{
			sql_def+=table->getName(true);

			if(!alias.isEmpty())
				sql_def+=QString(" AS ") + BaseObject::formatName(alias);
		}
		else
			sql_def=expression;

		sql_def+=QString(",\n");
	}
	//Case the reference is after [JOIN | WHERE] keywords
	else
	{
		//Case the column is allocated
		if(refer_type==ReferColumn && column)
		{
			/* Generated SQL definition:
			... WHERE {TABLE_NAME | ALIAS}.{COLUMN_NAME} */

			if(alias.isEmpty())
				sql_def=table->getName(true);
			else
				sql_def=BaseObject::formatName(alias);

			sql_def+=QString(".");

			if(column)
				sql_def+=column->getName(true);
		}
		else if(refer_type==ReferExpression)
			sql_def=expression;
	}

	sql_def=QString("   ") + sql_def;
	return sql_def;
}

QString Reference::getXMLDefinition()
{
	attribs_map attribs, aux_attribs;
	SchemaParser schparser;
	Column col_aux;
	QStringList ref_tab_names;

	attribs[Attributes::Table]=QString();
	attribs[Attributes::Column]=QString();

	if(table)
		attribs[Attributes::Table]=table->getName(true);

	if(column)
		attribs[Attributes::Column]=column->getName();

	attribs[Attributes::RefAlias]=ref_alias;
	attribs[Attributes::Expression]=expression;
	attribs[Attributes::Alias]=alias;
	attribs[Attributes::ColumnAlias]=column_alias;
	attribs[Attributes::Columns]=QString();
	attribs[Attributes::RefTables]=QString();

	for(auto &col : columns)
	{
		col_aux.setName(col.name);
		col_aux.setType(PgSqlType::parseString(col.type));
		col_aux.setAlias(col.alias);
		attribs[Attributes::Columns]+=col_aux.getCodeDefinition(SchemaParser::XmlDefinition);
	}

	if(is_def_expr)
	{
		for(auto &tab : ref_tables)
		{
			aux_attribs[Attributes::Name] = tab->getSignature();
			attribs[Attributes::RefTables] += schparser.getCodeDefinition(Attributes::RefTableTag, aux_attribs, SchemaParser::XmlDefinition);
		}
	}

	return schparser.getCodeDefinition(Attributes::Reference, attribs, SchemaParser::XmlDefinition);
}

bool Reference::operator == (Reference &refer)
{
	unsigned ref_type;

	ref_type=this->getReferenceType();

	if(ref_type==refer.getReferenceType())
	{
		if(ref_type==ReferColumn)
		{
			return (this->table==refer.table &&
							this->column==refer.column &&
							this->alias==refer.alias &&
							this->column_alias==refer.column_alias);
		}
		else
		{
			return (this->expression==refer.expression &&
							this->alias==refer.alias &&
							this->is_def_expr==refer.is_def_expr);
		}
	}
	else
		return false;
}

