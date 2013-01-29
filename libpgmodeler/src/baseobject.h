/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseObject
# Description: Implements the most important operations to define,
#              maintain and generate code (SQL or XML) of database objects
# Creation date: 12/09/2006
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
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "parsersattributes.h"
#include "exception.h"
#include "pgsqltypes.h"
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
 private:
  /// @details Stores the database wich the object belongs
  BaseObject *database;

 protected:
 /** @details This static attribute is used to generate the unique identifier for objects.
     As object instances are created this value ​​are incremented. In some classes
     like Schema, DBModel, Tablespace, Role, Type and Function id generators are
     used each with a custom different numbering range (see cited classes declaration). */
 static unsigned global_id;

 /** @details Stores the unique identifier for the object. This id is nothing else
     than the current value of global_id. This identifier is used
     to know the chronological order of the creation of each object in the model
     because the generation and reading of the XML code is completely tied to the order
     in which the objects were created */
 unsigned object_id;

 /// @details Objects type count declared on enum ObjectType.
 static const int OBJECT_TYPE_COUNT=27;

 /** @details Indicates whether the object is protected or not.
     A protected object indicates that it can not suffer changes in position
     (e.g. can not be moved or rotated) can not have your name / text changed,
     and deleted. This is only a flag, the cited operations are controled in a
     upper class layer */
 bool is_protected;

 /** @details This map stores the name of each object type associated to a schema file
     that generates the object's code definition */
 static QString objs_schemas[OBJECT_TYPE_COUNT];

 /** @details This map associates the object type to a keyword on
     SQL language that represents the object */
 static QString objs_sql[OBJECT_TYPE_COUNT];

 /** @details Stores the name of the type of objects to be used in error messages formatting
     and others operations that envolves object type name */
 static QString obj_type_names[OBJECT_TYPE_COUNT];

 /** @details Role that is owner of the object. Some objects cannot be associated to a role
     so if one is assigned to the object an error will be raised */
 BaseObject *owner;

 /** @details Schema the objects belongs. Some objects cannot be associated to a schema
     so if one is assigned to the object an error will be raised */
 BaseObject *schema;

 /** @details Tablespace to which the object is part. Some objects cannot be associated to a tablespace
     so if one is assigned to the object an error will be raised */
 BaseObject *tablespace;

 /// @details Maximum number of characters that an object name on PostgreSQL can have
 static const int OBJECT_NAME_MAX_LENGTH=63;

 /// @details Comments related to object
 QString comment,
 /// @details Object's name (in PostgreSQL accepted format)
 obj_name;

 /** @details Type of object, may have one of the values ​​of the enum ObjectType OBJ_*
     It was used a numeric type to avoid the use excessive of RTTI. */
 ObjectType obj_type;

 /** @details Stores the attributes and their values ​​shaped in strings to be used
     by SchemaParser on the object's code definition creation. The attribute
     name related to model objects are defined in ParsersAttributes namespace. */
 map<QString, QString> attributes;

 /** @details This method calls the getCodeDefinition(unsigned, bool) method with the 'reduced_form' defined as 'false',
     This is the real implementation of the virtual method getCodeDefinition(unsigned). */
 virtual QString __getCodeDefinition(unsigned def_type);

 /** @details Set the database that owns the object
    ATTENTION: calling this method with a NULL parameter doesn't means that the object will
               be removed from the database, only the attribute will be set as NULL and
               if the user calls getDatabase() in further operations may result in crash */
 void setDatabase(BaseObject *db);

public:
 BaseObject(void);
 virtual ~BaseObject(void){}

 /// @details Returns the reference to the database that owns the object
 BaseObject *getDatabase(void);

 /** @details Defines a specific attribute in the attribute list used to generate the code definition.
     This method can be used when a class needs to directly write some attributes of
     another class but does not have permission. */
 void setAttribute(const QString &attrib, const QString &value);

 /** @details Returns whether the object name is in conformity with the
      PostgreSQL object naming rule.
     (e.g. 63 bytes long and chars in set [a-zA-z0-9_] */
 static bool isValidName(const QString &name);

 /** @details Formats the passed name following the PostgreSQL object naming rule.
     The 'is_operator' parameter indicates that the passed name is a for an operator
     this is the only type of object that accepts characters outside of the alphabet
     (e.g. mathematical signs) on the composition of its name.
     In this case, the formatting function just ignores some validations if the parameter is checked */
 static QString formatName(const QString &name, bool is_operator=false);

 /// @details Returns the object's type name related to the passed object type
 static QString getTypeName(ObjectType obj_type);

 /** @details Returns the schema identifier used to generate the code definition related to the
     passed object type */
 static QString getSchemaName(ObjectType obj_type);

 /// @details Returns the keyword related to the object on SQL language
 static QString getSQLName(ObjectType obj_type);

 /// @details Returns the current value of the global object id counter
 static unsigned getGlobalId(void);

 /// @details Defines the comment of the object that will be attached to its SQL definition
 virtual void setComment(const QString &comment);

 /// @details Defines the objects name. If the passed name isn't valid it'll raise an error
 virtual void setName(const QString &name);

 /** @details Defines the schema that the object belongs. An error is raised if the
     passed schema is not valid */
 virtual void setSchema(BaseObject *schema);

 /** @details Defines the owner of the object. An error is raised if the
     passed owner is not valid */
 virtual void setOwner(BaseObject *owner);

 /** @details Defines the tablespace which the objects will use. An error is raised if the
     passed tablespace is not valid */
 virtual void setTablespace(BaseObject *tablespace);

 /// @details Toggles the object's modify protection
 virtual void setProtected(bool value);

 /** @details Returns the object's name. The parameter 'format' is used to get
     the name properly formated (schema qualified and using quotes) */
 QString getName(bool format=false);

 /// @details Retorns the object's comment
 QString getComment(void);

 /// @details Returns the object's type
 ObjectType getObjectType(void);

 /// @details Returns the object's type name
 QString getTypeName(void);

 /// @details Returns the object's schema name used to generate code definition
 QString getSchemaName(void);

 /// @details Returns the keyword related to the object type
 QString getSQLName(void);

 /// @details Returns the schema that the objects is part
 BaseObject *getSchema(void);

 /// @details Returns the owner of the object
 BaseObject *getOwner(void);

 /// @details Returns the tablespace that the object is part
 BaseObject *getTablespace(void);

 /// @details Returns the object's generated id
 unsigned getObjectId(void);

 /// @details Returns if the object is protected or not
 bool isProtected(void);

 /// @details Checks if the objects name is the same as the passed name
 bool operator == (const QString &obj_name);

 /// @details Checks if the objects name differs from the passed name
 bool operator != (const QString &obj_name);

 /// @details Assigns an object to other copiyng all the attributes correctly
 virtual void operator = (BaseObject &obj);

 /// @details Clears all the attributes used by the SchemaParser
 void clearAttributes(void);

 /** @details Forcing the class to be virtual. This means that derivated classes may
     override this method in order to be possible its instatiation. */
 virtual QString getCodeDefinition(unsigned)=0;

 /** @details Returns the object's SQL or XML code definition. The attribute 'reduced_form'
     indicates that the code generation will be an XML minimum representation
     of the object. See schema file for: functions, schemas, domains, types. */
 virtual QString getCodeDefinition(unsigned def_type, bool reduced_form);

 /// @details Returns if the object accepts to has a schema assigned
 bool acceptsSchema(void);

 /// @details Returns if the object accepts to has an owner assigned
 bool acceptsOwner(void);

 /// @details Returns if the object accepts to has a tablespace assigned
 bool acceptsTablespace(void);

 friend class DatabaseModel;
};

#endif
