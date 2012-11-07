/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: OperatorClassElement
# Description: Implements the operations to manipulate operator class
#              elements (storage, operator, function).
# Creation date: 08/07/2008
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef OPERATOR_CLASS_ELEMENT_H
#define OPERATOR_CLASS_ELEMENT_H

#include "baseobject.h"
#include "operador.h"
#include "funcao.h"

class OperatorClassElement {
 private:
  /* Type of the operator class element.
     This can have 3 possible values:
     0 -> OPERATOR_ELEM
     1 -> FUNCTION_ELEM
     2 -> STORAGE_ELEM */
  unsigned element_type;

  //Function used by the element (only for type FUNCTION_ELEM)
  Funcao *function;

  //Operator used by the element (only for type OPERATOR_ELEM)
  Operador *_operator;

  /* PostgreSQL type used in the indexing method of operator class.
     (only for type STORAGE_ELEM) */
  TipoPgSQL storage;

  //Inicates that the rechecking of retrieved lines is mandatory (only for OPERATOR_ELEM)
  bool recheck;

  /* Strategy number (or support number for functions). This attribute
     must have a value greater than 0 (only for OPERATOR_ELEM and FUNCTION_ELEM) */
  unsigned strategy_number;

 public:
  //Constants used to reference the element types
  static const unsigned OPERATOR_ELEM=0,
                        FUNCTION_ELEM=1,
                        STORAGE_ELEM=2;

  OperatorClassElement(void);

  //Defines the element as a function clause
  void setFunction(Funcao *func, unsigned stg_number);

  //Defines the element as an operator clause
  void setOperator(Operador *oper, unsigned stg_number, bool recheck);

  //Defines the element as a storage clause
  void setStorage(TipoPgSQL storage);

  //Returns the element type
  unsigned getElementType(void);

  /* Returns the current assigned function.
     This method returns NULL when the element is not an FUNCTION_ELEM */
  Funcao *getFunction(void);

  /* Returns the current assigned operator.
     This method returns NULL when the element is not an OPERATOR_ELEM */
  Operador *getOperator(void);

  //Storage type of the element
  TipoPgSQL getStorage(void);

  //Returns whether the elements is to be rechecked or not
  bool isRecheck(void);

  //Returns the strategy (support) number of the element
  unsigned getStrategyNumber(void);

  //Returns the SQL / XML code definition for the element
  QString getCodeDefinition(unsigned def_type);

  //Operator to compare two elements, returns true when all atributes has the same configuration
  bool operator == (OperatorClassElement &elem);
};

#endif
