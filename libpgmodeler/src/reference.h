/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Reference
# Description: This auxiliary class is used to define SQL/XML for views
# Creation date: 05/09/2008
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
#ifndef REFERENCE_H
#define REFERENCE_H

#include "table.h"
#include "schema.h"

class Reference {
 private:
 //Stores the table used by the reference
  Table *table;

  //Stores the column used by the reference
  Column *column;

  //Stores the expression that defines one reference
  QString expression,
         //Stores the alias to the expression or table
         alias,
         //Stores only the alias for the column
         column_alias;

 public:
  //Constants used to define the reference type
  static const unsigned REFER_COLUMN=0, //The reference is based on a table column
                        REFER_EXPRESSION=1; //The reference is based on an expression

  //Constants used on the view code generation
  static const unsigned SQL_REFER_WHERE=10,
                        SQL_REFER_SELECT=20,
                        SQL_REFER_FROM=30;

  Reference(void);

  //Creates a reference based on a table column
  Reference(Table *table, Column *column, const QString &tab_alias, const QString &col_alias);

  //Creates a reference based on a expression
  Reference(const QString &expression, const QString &expr_alias);

  //Gets the referenced table
  Table *getTable(void);

  //Gets the referenced column
  Column *getColumn(void);

  //Returns the alias for the referenced column
  QString getColumnAlias(void);

  //Returs the reference for the table or expression
  QString getAlias(void);

  //Returns the expression for the reference
  QString getExpression(void);

  //Returns the reference typ (see REFER_??? constants)
  unsigned getReferenceType(void);

  //Returns the SQL code definition
  QString getSQLDefinition(unsigned sql_type);

  //Returns the XML code definition
  QString getXMLDefinition(void);

  /* Compare the attributes of two references returning true
     when they have the same values */
  bool operator == (Reference &refer);
};

#endif
