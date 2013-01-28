/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Implements the operations to manipulate index elements.
# Creation date: 26/05/2010
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
#ifndef ELEMENTO_INDICE_H
#define ELEMENTO_INDICE_H

#include "column.h"
#include "operatorclass.h"

class IndexElement {
 private:
  /** Column referenced by the index element. This attribute is
     mutually exclusive with the expression attribute, this means,
     when one is set the other has empty (null) value */
  Column *column;

  /** Expression referenced by the index element. This attribute is
     mutually exclusive with the column attribute, this means
     when one is set the other has empty (null) value */
  QString expression;

  /// Operator class used by the index element
  OperatorClass *operator_class;

  /** Sorting attributes of the element (ASC|DESC, NULLS [FIRST|LAST])
     This attibutes can be configured used the constants ASC_ORDER and NULLS_FIRST */
  bool sort_attibutes[2];

 public:
  /// Constants used to reference the sorting method of the element
  const static unsigned ASC_ORDER=0,
                        NULLS_FIRST=1;

   IndexElement(void);

   /// Element configuration methods
   void setColumn(Column *column);
   void setExpression(const QString &expression);
   void setOperatorClass(OperatorClass *oper_class);

   /// Sets the state of one of the element sorting method
   void setSortAttribute(unsigned attrib, bool value);

   /// Gets the curret state of the element sorting attribute
   bool getSortAttribute(unsigned attrib);

   Column *getColumn(void);
   QString getExpression(void);
   OperatorClass *getOperatorClass(void);

   /// Returns the SQL / XML code definition for the index element
   QString getCodeDefinition(unsigned def_type);
};

#endif
