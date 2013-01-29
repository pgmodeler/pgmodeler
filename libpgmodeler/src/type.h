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
\class Type
\brief Implements the operations to manipulate user defined types on database.
\note <strong>Creation date:</strong> 05/06/2008
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

  /// @details Type configuration (BASE | ENUMERATION | COMPOSITE)
  unsigned config;

  /// @details Attributes for composite type
  vector<Parameter> attributes;

  /// @details Enumerations of enumeration type
  vector<QString> enumerations;

  /// @details Functions used by the base type
  Function *functions[7];

  /// @details Type's internal length ( > 0 - Fixed length, 0 - Variable length)
  unsigned internal_len; /// @details INTERNALLENGTH

  /// @details Indicates that the type can be passed by value
  bool by_value; /// @details PASSEDBYVALUE

  /// @details Storage alignmnet (char, smallint (int2), integer (int4) ou double precision)
  PgSQLType alignment, /// @details ALIGNMENT
            element; /// @details ELEMENT

  /// @details Type's storage
  StorageType storage; /// @details STORAGE

  /// @details Default value for the type
  QString default_value; /// @details DEFAULT

  /// @details Type's category (only for base type) - Default 'U'
  CategoryType category; /// @details CATEGORY

  /// @details Used with the category attribute (only for base type) - Default FALSE
  bool preferred; /// @details PREFERRED

  /** @details Type which will have some of its attributes copied to the current type
     (only for base type). If like_type is 'any' means that the
     current type does not copy attributes of any type */
  PgSQLType like_type; /// @details LIKE

  /// @details Character used as value delimiter when the type is used as array
  char delimiter; /// @details DELIMITER

  /// @details Checks if the named attribute exists
  bool isAttributeExists(const QString &attrib_name);

  /// @details Checks if the named enumeration exists
  bool isEnumerationExists(const QString &enum_name);

  /// @details Formats the elements string used by the SchemaParser
  void setElementsAttribute(unsigned def_type);

  /// @details Formats the enumeration string used by the SchemaParser
  void setEnumerationsAttribute(unsigned def_type);

  /** @details Performs the conversion of parameters and return types of functions.
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

  /// @details Sets the type name
  void setName(const QString &name);

  /// @details Sets the type schema
  void setSchema(BaseObject *schema);

  /// @details Defines the type configuration (BASE | ENUMARATION | COMPOSITE)
  void setConfiguration(unsigned conf);

  /// @details Adds an attribute to the type (only for composite type)
  void addAttribute(Parameter attrib);

  /// @details Removes an attribute from the type (only for composite type)
  void removeAttribute(unsigned attrib_idx);

  /// @details Removes all attributes from the type (only for composite type)
  void removeAttributes(void);

  /// @details Adds an enumeration to the type (only for enumeration type)
  void addEnumeration(const QString &enum_name);

  /// @details Removes an enumeration from the type (only for enumeration type)
  void removeEnumeration(unsigned enum_idx);

  /// @details Removes all enumerations from the type (only for enumeration type)
  void removeEnumerations(void);

  /// @details Sets on function to the type (only for base type)
  void setFunction(unsigned func_id, Function *func);

  /// @details Sets the type internal length (only for base type)
  void setInternalLength(unsigned length);

  /// @details Sets if the type can be passed by value (only for base type)
  void setByValue(bool value);

  /// @details Sets the alignment for the type (only for base type)
  void setAlignment(PgSQLType type);

  /// @details Sets the storage type (only for base type)
  void setStorage(StorageType strg);

  /// @details Sets the default value for the type (only for base type)
  void setDefaultValue(const QString &value);

  /// @details Sets the element for the type (only for base type)
  void setElement(PgSQLType elem);

  /// @details Sets the delimiter for the type (only for base type)
  void setDelimiter(char delim);

  /// @details Sets the category for the type (only for base type)
  void setCategory(CategoryType categ);

  /// @details Sets if the type is preferred (only for base type)
  void setPreferred(bool value);

  /// @details Sets the type that will be used as template (only for base type)
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

  /** @details Returns the SQL / XML definition for the type. If the boolean
     parameter is set the code definition is generated in a
     reduced form (XML only) */
  QString getCodeDefinition(unsigned def_type, bool reduced_form);

  /// @details Returns the SQL / XML definition for the type
  QString getCodeDefinition(unsigned def_type);

  /// @details Makes a copy between two type
  void operator = (Type &tipo);

  friend class DatabaseModel;
};

#endif
