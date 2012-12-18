/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: OperatorClass
# Description: This class is used to generate the XML/SQL code related to PostgreSQL
#              operator classes. This type of object indicates to RDBMS how a data type
#              can be used in a particular indexing method.
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
#ifndef OPERATOR_CLASS_H
#define OPERATOR_CLASS_H
                                                                                                                                                                                                                   
#include "baseobject.h"                                                                                                                                                                                            
#include "operatorclasselement.h"                                                                                                                                                                                  
#include "operatorfamily.h"
                                                                                                                                                      
class OperatorClass: public BaseObject {
 private:                                                                                                                                                                                                          
  //Data type witch the operator class acts on
  TipoPgSQL data_type;

  //Operator family that the operator class is part
  OperatorFamily *family;

  //Indexing type (method) adopted by the operator class
  IndexingType indexing_type;

  /* Indicates if the operator class is the default for the
     specified data type */
  bool is_default;

  //Operator class attributes list
  vector<OperatorClassElement> elements;

  /* Formats the elements string used by the SchemaParser on the
     SQL/XML code definition for operator classes */
  void setElementsAttribute(unsigned def_type);

 public:
  OperatorClass(void);
  ~OperatorClass(void);

  //Sets the data type that the operator class works on
  void setDataType(TipoPgSQL data_type);

  //Sets the operator family that the operator class is part
  void setFamily(OperatorFamily *family);

  //Set the indexing type used by the operator class
  void setIndexingType(IndexingType index_type);

  //Sets if the opertor class is default for the specified data type
  void setDefault(bool value);

  //Methods used to manipulate the elements FUNCTION, OPERATOR and STORAGE
  void addElement(OperatorClassElement elem);
  void removeElement(unsigned elem_idx);
  OperatorClassElement getElement(unsigned elem_idx);
  bool isElementExists(OperatorClassElement elem);
  void removeElements(void);

  //Methods used to obtain the class attributes
  TipoPgSQL getDataType(void);
  OperatorFamily *getFamily(void);
  IndexingType getIndexingType(void);
  bool isDefault(void);
  unsigned getElementCount(void);

  //Returns the SQL/XML code definition for operator class
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);
};

#endif
