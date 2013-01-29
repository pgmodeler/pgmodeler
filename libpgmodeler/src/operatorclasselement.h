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

/**
\ingroup libpgmodeler
\class OperatorClassElement
\brief Implements the operations to manipulate operator class
 elements (storage, operator, function).
\note <strong>Creation date:</strong> 08/07/2008
*/

#ifndef OPERATOR_CLASS_ELEMENT_H
#define OPERATOR_CLASS_ELEMENT_H

#include "baseobject.h"
#include "operator.h"
#include "function.h"

class OperatorClassElement {
 private:
  /** @details Type of the operator class element.
     This can have 3 possible values:
     0 -> OPERATOR_ELEM
     1 -> FUNCTION_ELEM
     2 -> STORAGE_ELEM */
  unsigned element_type;

  /// @details Function used by the element (only for type FUNCTION_ELEM)
  Function *function;

  /// @details Operator used by the element (only for type OPERATOR_ELEM)
  Operator *_operator;

  /** @details PostgreSQL type used in the indexing method of operator class.
     (only for type STORAGE_ELEM) */
  PgSQLType storage;

  /// @details Inicates that the rechecking of retrieved lines is mandatory (only for OPERATOR_ELEM)
  bool recheck;

  /** @details Strategy number (or support number for functions). This attribute
     must have a value greater than 0 (only for OPERATOR_ELEM and FUNCTION_ELEM) */
  unsigned strategy_number;

 public:
  /// @details Constants used to reference the element types
  static const unsigned OPERATOR_ELEM=0,
                        FUNCTION_ELEM=1,
                        STORAGE_ELEM=2;

  OperatorClassElement(void);

  /// @details Defines the element as a function clause
  void setFunction(Function *func, unsigned stg_number);

  /// @details Defines the element as an operator clause
  void setOperator(Operator *oper, unsigned stg_number, bool recheck);

  /// @details Defines the element as a storage clause
  void setStorage(PgSQLType storage);

  /// @details Returns the element type
  unsigned getElementType(void);

  /** @details Returns the current assigned function.
     This method returns NULL when the element is not an FUNCTION_ELEM */
  Function *getFunction(void);

  /** @details Returns the current assigned operator.
     This method returns NULL when the element is not an OPERATOR_ELEM */
  Operator *getOperator(void);

  /// @details Storage type of the element
  PgSQLType getStorage(void);

  /// @details Returns whether the elements is to be rechecked or not
  bool isRecheck(void);

  /// @details Returns the strategy (support) number of the element
  unsigned getStrategyNumber(void);

  /// @details Returns the SQL / XML code definition for the element
  QString getCodeDefinition(unsigned def_type);

  /// @details Operator to compare two elements, returns true when all atributes has the same configuration
  bool operator == (OperatorClassElement &elem);
};

#endif
