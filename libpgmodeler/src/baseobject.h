/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseObject
# Description: Implements the most important operations to define,
#              maintain and generate code (SQL or XML) of database objects
# Creation date: 12/09/2006
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
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "parsersattributes.h"
#include "exception.h"
#include "tipospgsql.h"
#include "schemaparser.h"
#include "xmlparser.h"
#include <map>
#include <QRegExp>
#include <QStringList>

using namespace ParsersAttributes;

enum ObjectType {
  OBJ_COLUMN,
  OBJ_CONSTRAINT,
  OBJ_FUNCTION,
  OBJ_TRIGGER,
  OBJ_INDEX,
  OBJ_RULE,
  OBJ_TABLE,
  OBJ_VIEW,
  OBJ_DOMAIN,
  OBJ_SCHEMA,
  OBJ_AGGREGATE,
  OBJ_OPERATOR,
  OBJ_SEQUENCE,
  OBJ_ROLE,
  OBJ_CONVERSION,
  OBJ_CAST,
  OBJ_LANGUAGE,
  OBJ_TYPE,
  OBJ_TABLESPACE,
  OBJ_OPFAMILY,
  OBJ_OPCLASS,
  OBJ_DATABASE,
  OBJ_RELATIONSHIP,
  OBJ_TEXTBOX,
  OBJ_PERMISSION,
  OBJ_PARAMETER,
  BASE_RELATIONSHIP,
  BASE_OBJECT,
  BASE_TABLE
};

class BaseObject {
 protected:
  /* This static attribute is used to generate the unique identifier for objects.
     As object instances are created this value ​​are incremented. In some classes
     like Schema, DBModel, Tablespace, Role, Type and Function id generators are
     used each with a custom different numbering range (see cited classes declaration). */
  static unsigned global_id;

  /* Stores the unique identifier for the object. This id is nothing else
     than the current value of global_id. This identifier is used
     to know the chronological order of the creation of each object in the model
     because the generation and reading of the XML code is completely tied to the order
     in which the objects were created */
  unsigned object_id;

  //Objects type count declared on enum ObjectType.
  static const int OBJECT_TYPE_COUNT=27;

  /* Indicates whether the object is protected or not.
     A protected object indicates that it can not suffer changes in position
     (e.g. can not be moved or rotated) can not have your name / text changed,
     and deleted. This is only a flag, the cited operations are controled in a
     upper class layer */
  bool is_protected;

  /* This map stores the name of each object type associated to a schema file
     that generates the object's code definition */
  static QString objs_schemas[OBJECT_TYPE_COUNT];

  /* This map associates the object type to a keyword on
     SQL language that represents the object */
  static QString objs_sql[OBJECT_TYPE_COUNT];

  /* Stores the name of the type of objects to be used in error messages formatting
     and others operations that envolves object type name */
  static QString obj_type_names[OBJECT_TYPE_COUNT];

  /* Role that is owner of the object. Some objects cannot be associated to a role
     so if one is assigned to the object an error will be raised */
  BaseObject *owner;

  /* Schema the objects belongs. Some objects cannot be associated to a schema
     so if one is assigned to the object an error will be raised */
  BaseObject *schema;

  /* Tablespace to which the object is part. Some objects cannot be associated to a tablespace
     so if one is assigned to the object an error will be raised */
  BaseObject *tablespace;

  //Maximum number of characters that an object name on PostgreSQL can have
  static const int OBJECT_NAME_MAX_LENGTH=63;

  //Comments related to object
  QString comment,
         //Object's name (in PostgreSQL accepted format)
         obj_name;

  /* Type of object, may have one of the values ​​of the enum ObjectType OBJ_*
     It was used a numeric type to avoid the use excessive of RTTI. */
  ObjectType obj_type;

  /* Stores the attributes and their values ​​shaped in strings to be used
     by SchemaParser on the object's code definition creation. The attribute
     name related to model objects are defined in ParsersAttributes namespace. */
  map<QString, QString> attributes;

  /* This method calls the getCodeDefinition(unsigned, bool) method with the 'reduced_form' defined as 'false',
     This is the real implementation of the virtual method getCodeDefinition(unsigned). */
  virtual QString __getCodeDefinition(unsigned def_type);

 public:
  BaseObject(void);
  virtual ~BaseObject(void){}

  /* Defines a specific attribute in the attribute list used to generate the code definition.
     This method can be used when a class needs to directly write some attributes of
     another class but does not have permission. */
  void setAttribute(const QString &attrib, const QString &value);

   /* Returns whether the object name is in conformity with the
      PostgreSQL object naming rule.
     (e.g. 63 bytes long and chars in set [a-zA-z0-9_] */
  static bool isValidName(const QString &name);

  /* Formats the passed name following the PostgreSQL object naming rule.
     The 'is_operator' parameter indicates that the passed name is a for an operator
     this is the only type of object that accepts characters outside of the alphabet
     (e.g. mathematical signs) on the composition of its name.
     In this case, the formatting function just ignores some validations if the parameter is checked */
  static QString formatName(const QString &name, bool is_operator=false);

  //Returns the object's type name related to the passed object type
  static QString getTypeName(ObjectType obj_type);

  /* Returns the schema identifier used to generate the code definition related to the
     passed object type */
  static QString getSchemaName(ObjectType obj_type);

  //Returns the keyword related to the object on SQL language
  static QString getSQLName(ObjectType obj_type);

  //Returns the current value of the global object id counter
  static unsigned getGlobalId(void);

  //Defines the comment of the object that will be attached to its SQL definition
  virtual void setComment(const QString &comment);

  //Defines the objects name. If the passed name isn't valid it'll raise an error
  virtual void setName(const QString &name);

  /* Defines the schema that the object belongs. An error is raised if the
     passed schema is not valid */
  virtual void setSchema(BaseObject *schema);

  /* Defines the owner of the object. An error is raised if the
     passed owner is not valid */
  virtual void setOwner(BaseObject *owner);

  /* Defines the tablespace which the objects will use. An error is raised if the
     passed tablespace is not valid */
  virtual void setTablespace(BaseObject *tablespace);

  //Toggles the object's modify protection
  virtual void setProtected(bool value);

  /* Returns the object's name. The parameter 'format' is used to get
     the name properly formated (schema qualified and using quotes) */
  QString getName(bool format=false);

  //Retorns the object's comment
  QString getComment(void);

  //Returns the object's type
  ObjectType getObjectType(void);

  //Returns the object's type name
  QString getTypeName(void);

  //Returns the object's schema name used to generate code definition
  QString getSchemaName(void);

  //Returns the keyword related to the object type
  QString getSQLName(void);

  //Returns the schema that the objects is part
  BaseObject *getSchema(void);

  //Returns the owner of the object
  BaseObject *getOwner(void);

  //Returns the tablespace that the object is part
  BaseObject *getTablespace(void);

  //Returns the object's generated id
  unsigned getObjectId(void);

  //Returns if the object is protected or not
  bool isProtected(void);

  //Checks if the objects name is the same as the passed name
  bool operator == (const QString &obj_name);

  //Checks if the objects name differs from the passed name
  bool operator != (const QString &obj_name);

  //Assigns an object to other copiyng all the attributes correctly
  virtual void operator = (BaseObject &obj);

  //Clears all the attributes used by the SchemaParser
  void clearAttributes(void);

  /* Forcing the class to be virtual. This means that derivated classes may
     override this method in order to be possible its instatiation. */
  virtual QString getCodeDefinition(unsigned)=0;

  /* Returns the object's SQL or XML code definition. The attribute 'reduced_form'
     indicates that the code generation will be an XML minimum representation
     of the object. See schema file for: functions, schemas, domains, types. */
  virtual QString getCodeDefinition(unsigned def_type, bool reduced_form);

  friend class ModeloBD;
};

#endif
