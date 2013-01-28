/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Type
# Description: Implements the operations to manipulate user defined types on database.
# Creation date: 05/06/2008
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
#ifndef TYPE_H
#define TYPE_H

#include "baseobject.h"
#include "schema.h"
#include "function.h"
#include "role.h"

class Type: public BaseObject {
 private:
  static unsigned type_id;

  /// Type configuration (BASE | ENUMERATION | COMPOSITE)
  unsigned config;

  /// Attributes for composite type
  vector<Parameter> attributes;

  /// Enumerations of enumeration type
  vector<QString> enumerations;

  /// Functions used by the base type
  Function *functions[7];

  /// Type's internal length ( > 0 - Fixed length, 0 - Variable length)
  unsigned internal_len; /// INTERNALLENGTH

  /// Indicates that the type can be passed by value
  bool by_value; /// PASSEDBYVALUE

  /// Storage alignmnet (char, smallint (int2), integer (int4) ou double precision)
  PgSQLType alignment, /// ALIGNMENT
            element; /// ELEMENT

  /// Type's storage
  StorageType storage; /// STORAGE

  /// Default value for the type
  QString default_value; /// DEFAULT

  /// Type's category (only for base type) - Default 'U'
  CategoryType category; /// CATEGORY

  /// Used with the category attribute (only for base type) - Default FALSE
  bool preferred; /// PREFERRED

  /** Type which will have some of its attributes copied to the current type
     (only for base type). If like_type is 'any' means that the
     current type does not copy attributes of any type */
  PgSQLType like_type; /// LIKE

  /// Character used as value delimiter when the type is used as array
  char delimiter; /// DELIMITER

  /// Checks if the named attribute exists
  bool isAttributeExists(const QString &attrib_name);

  /// Checks if the named enumeration exists
  bool isEnumerationExists(const QString &enum_name);

  /// Formats the elements string used by the SchemaParser
  void setElementsAttribute(unsigned def_type);

  /// Formats the enumeration string used by the SchemaParser
  void setEnumerationsAttribute(unsigned def_type);

  /** Performs the conversion of parameters and return types of functions.
     If the parameter 'inverse_conv' is selected, the method
     makes the conversion from 'type_name' to 'any' otherwise
     makes the conversion from 'any' to 'type_name'. This method is used
     when generating the SQL codes for user-defined type */
  void convertFunctionParameters(bool inverse_conv=false);

 public:
  static const unsigned BASE_TYPE=10,
                        ENUMERATION_TYPE=20,
                        COMPOSITE_TYPE=30;

  static const unsigned INPUT_FUNC=0,
                        OUTPUT_FUNC=1,
                        RECV_FUNC=2,
                        SEND_FUNC=3,
                        TPMOD_IN_FUNC=4,
                        TPMOD_OUT_FUNC=5,
                        ANALYZE_FUNC=6;

  Type(void);
  ~Type(void);

  /// Sets the type name
  void setName(const QString &name);

  /// Sets the type schema
  void setSchema(BaseObject *schema);

  /// Defines the type configuration (BASE | ENUMARATION | COMPOSITE)
  void setConfiguration(unsigned conf);

  /// Adds an attribute to the type (only for composite type)
  void addAttribute(Parameter attrib);

  /// Removes an attribute from the type (only for composite type)
  void removeAttribute(unsigned attrib_idx);

  /// Removes all attributes from the type (only for composite type)
  void removeAttributes(void);

  /// Adds an enumeration to the type (only for enumeration type)
  void addEnumeration(const QString &enum_name);

  /// Removes an enumeration from the type (only for enumeration type)
  void removeEnumeration(unsigned enum_idx);

  /// Removes all enumerations from the type (only for enumeration type)
  void removeEnumerations(void);

  /// Sets on function to the type (only for base type)
  void setFunction(unsigned func_id, Function *func);

  /// Sets the type internal length (only for base type)
  void setInternalLength(unsigned length);

  /// Sets if the type can be passed by value (only for base type)
  void setByValue(bool value);

  /// Sets the alignment for the type (only for base type)
  void setAlignment(PgSQLType type);

  /// Sets the storage type (only for base type)
  void setStorage(StorageType strg);

  /// Sets the default value for the type (only for base type)
  void setDefaultValue(const QString &value);

  /// Sets the element for the type (only for base type)
  void setElement(PgSQLType elem);

  /// Sets the delimiter for the type (only for base type)
  void setDelimiter(char delim);

  /// Sets the category for the type (only for base type)
  void setCategory(CategoryType categ);

  /// Sets if the type is preferred (only for base type)
  void setPreferred(bool value);

  /// Sets the type that will be used as template (only for base type)
  void setLikeType(PgSQLType like_type);

  Parameter getAttribute(unsigned attrib_idx);
  unsigned getAttributeCount(void);
  QString getEnumeration(unsigned idx_enum);
  unsigned getEnumerationCount(void);
  unsigned getConfiguration(void);
  CategoryType getCategory(void);
  bool isPreferred(void);
  PgSQLType getLikeType(void);
  Function *getFunction(unsigned func_id);
  unsigned getInternalLength(void);
  bool isByValue(void);
  PgSQLType getAlignment(void);
  StorageType getStorage(void);
  QString getDefaultValue(void);
  PgSQLType getElement(void);
  char getDelimiter(void);

  /** Returns the SQL / XML definition for the type. If the boolean
     parameter is set the code definition is generated in a
     reduced form (XML only) */
  QString getCodeDefinition(unsigned def_type, bool reduced_form);

  /// Returns the SQL / XML definition for the type
  QString getCodeDefinition(unsigned def_type);

  /// Makes a copy between two type
  void operator = (Type &tipo);

  friend class DatabaseModel;
};

#endif
