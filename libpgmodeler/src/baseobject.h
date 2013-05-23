/*
# PostgreSQL Database Modeler (pgModeler)
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
\class BaseObject
\brief Implements the most important operations to define, maintain and generate code (SQL or XML) of database objects
\note <strong>Creation date:</strong> 12/09/2006
\ingroup libpgmodeler
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
#include <QTextStream>

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
	OBJ_COLLATION,
	OBJ_EXTENSION,
	OBJ_RELATIONSHIP,
	OBJ_TEXTBOX,
	OBJ_PERMISSION,
	OBJ_PARAMETER,
	OBJ_TYPE_ATTRIBUTE,
	BASE_RELATIONSHIP,
	BASE_OBJECT,
	BASE_TABLE
};

class BaseObject {
	private:
		//! \brief Stores the database wich the object belongs
		BaseObject *database;

	protected:
		/*! \brief This static attribute is used to generate the unique identifier for objects.
		 As object instances are created this value ​​are incremented. In some classes
		 like Schema, DatabaseModel, Tablespace, Role, Type and Function id generators are
		 used each with a custom different numbering range (see cited classes declaration). */
		static unsigned global_id;

		/*! \brief Stores the unique identifier for the object. This id is nothing else
		 than the current value of global_id. This identifier is used
		 to know the chronological order of the creation of each object in the model
		 because the generation and reading of the XML code is completely tied to the order
		 in which the objects were created */
		unsigned object_id;

		//! \brief Objects type count declared on enum ObjectType (excluding BASE_OBJECT and BASE_TABLE).
		static const int OBJECT_TYPE_COUNT=30;

		/*! \brief Indicates whether the object is protected or not.
		 A protected object indicates that it can not suffer changes in position
		 (e.g. can not be moved or rotated) can not have your name / text changed,
		 and deleted. This is only a flag, the cited operations are controled in a
		 upper class layer */
		bool is_protected,

		/*! \brief This property indicates that the object is a system protected object and cannot be modified
		by the user. Additionally, the this attribute is true the SQL/XML code for the object is not generated */
		system_obj,

		/*! \brief Indicates if the generated SQL code is disable. When this flag is true
		the object's SQL code is created normally but is commented. This is useful when using
		the role only as a reference since it already exists on the destination server. */
		sql_disabled;

		/*! \brief This map stores the name of each object type associated to a schema file
		 that generates the object's code definition */
		static QString objs_schemas[OBJECT_TYPE_COUNT];

		/*! \brief This map associates the object type to a keyword on
		 SQL language that represents the object */
		static QString objs_sql[OBJECT_TYPE_COUNT];

		/*! \brief Stores the name of the type of objects to be used in error messages formatting
		 and others operations that envolves object type name */
		static QString obj_type_names[OBJECT_TYPE_COUNT];

		/*! \brief Role that is owner of the object. Some objects cannot be associated to a role
		 so if one is assigned to the object an error will be raised */
		BaseObject *owner;

		/*! \brief Schema the objects belongs. Some objects cannot be associated to a schema
		 so if one is assigned to the object an error will be raised */
		BaseObject *schema;

		/*! \brief Tablespace to which the object is part. Some objects cannot be associated to a tablespace
		 so if one is assigned to the object an error will be raised */
		BaseObject *tablespace;

		/*! \brief Collation referenced by the object. Some objects cannot be associated to a collation
		 so if one is assigned to the object an error will be raised */
		BaseObject *collation;

		//! \brief Comments related to object
		QString comment,
						//! \brief Object's name (in PostgreSQL accepted format)
						obj_name;

		/*! \brief Type of object, may have one of the values ​​of the enum ObjectType OBJ_*
		 It was used a numeric type to avoid the use excessive of RTTI. */
		ObjectType obj_type;

		/*! \brief Stores the attributes and their values ​​shaped in strings to be used
		 by SchemaParser on the object's code definition creation. The attribute
		 name related to model objects are defined in ParsersAttributes namespace. */
		map<QString, QString> attributes;

		/*! \brief This method calls the getCodeDefinition(unsigned, bool) method with the 'reduced_form' defined as 'false',
		 This is the real implementation of the virtual method getCodeDefinition(unsigned). */
		QString __getCodeDefinition(unsigned def_type);

		/*! \brief Set the database that owns the object
		ATTENTION: calling this method with a nullptr parameter doesn't means that the object will
							 be removed from the database, only the attribute will be set as nullptr and
							 if the user calls getDatabase() in further operations may result in crash */
		void setDatabase(BaseObject *db);

	public:
		//! \brief Maximum number of characters that an object name on PostgreSQL can have
		static const int OBJECT_NAME_MAX_LENGTH=63;

		BaseObject(void);
		BaseObject(bool system_obj);
		virtual ~BaseObject(void){}

		//! \brief Returns the reference to the database that owns the object
		BaseObject *getDatabase(void);

		/*! \brief Defines a specific attribute in the attribute list used to generate the code definition.
		 This method can be used when a class needs to directly write some attributes of
		 another class but does not have permission. */
		void setAttribute(const QString &attrib, const QString &value);

		/*! \brief Returns whether the object name is in conformity with the PostgreSQL object naming rule.
		 (e.g. 63 bytes long and chars in set [a-zA-z0-9_] */
		static bool isValidName(const QString &name);

		/*! \brief Formats the passed name following the PostgreSQL object naming rule.
		 The 'is_operator' parameter indicates that the passed name is a for an operator
		 this is the only type of object that accepts characters outside of the alphabet
		 (e.g. mathematical signs) on the composition of its name.
		 In this case, the formatting function just ignores some validations if the parameter is checked */
		static QString formatName(const QString &name, bool is_operator=false);

		//! \brief Returns the object's type name related to the passed object type
		static QString getTypeName(ObjectType obj_type);

		/*! \brief Returns the schema identifier used to generate the code definition related to the
		 passed object type */
		static QString getSchemaName(ObjectType obj_type);

		//! \brief Returns the keyword related to the object on SQL language
		static QString getSQLName(ObjectType obj_type);

		//! \brief Returns the current value of the global object id counter
		static unsigned getGlobalId(void);

		//! \brief Defines the comment of the object that will be attached to its SQL definition
		virtual void setComment(const QString &comment);

		//! \brief Defines the objects name. If the passed name isn't valid it'll raise an error
		virtual void setName(const QString &name);

		/*! \brief Defines the schema that the object belongs. An error is raised if the
		 passed schema is not valid or the object does not accepts the use of schemas. */
		virtual void setSchema(BaseObject *schema);

		/*! \brief Defines the owner of the object. An error is raised if the
		 passed owner is not valid or the object does not accepts the use of owners. */
		virtual void setOwner(BaseObject *owner);

		/*! \brief Defines the tablespace which the objects will use. An error is raised if the
		 passed tablespace is not valid or the object does not accepts the use of tablespaces. */
		virtual void setTablespace(BaseObject *tablespace);

		//! \brief Toggles the object's modify protection
		virtual void setProtected(bool value);

		/*! \brief Defines the collation which the objects will use. An error is raised if the
		 passed collation is not valid or the object does not accepts the use of collations. */
		virtual void setCollation(BaseObject *collation);

		//! \brief Disables the SQL code commenting it on generation
		void setSQLDisabled(bool value);

		//! \brief Returns if the generated SQL is commented
		bool isSQLDisabled(void);

		//! \brief Defines if the object is a system protected object
		virtual void setSystemObject(bool value);

		//! \brief Returns if the object is a system protected object
		bool isSystemObject(void);

		/*! \brief Returns the object's name. The parameter 'format' is used to get
		 the name properly formated (using quotes when there is uppercase char or extended utf-8),
		 the parameter 'prepend_schema' includes the schema name on the objects name (defult) */
		QString getName(bool format=false, bool prepend_schema=true);

		//! \brief Retorns the object's comment
		QString getComment(void);

		//! \brief Returns the object's type
		ObjectType getObjectType(void);

		//! \brief Returns the object's type name
		QString getTypeName(void);

		//! \brief Returns the object's schema name used to generate code definition
		QString getSchemaName(void);

		//! \brief Returns the keyword related to the object type
		QString getSQLName(void);

		//! \brief Returns the schema that the objects is part
		BaseObject *getSchema(void);

		//! \brief Returns the owner of the object
		BaseObject *getOwner(void);

		//! \brief Returns the tablespace that the object is part
		BaseObject *getTablespace(void);

		//! \brief Returns the collation that the object makes use
		BaseObject *getCollation(void);

		//! \brief Returns the object's generated id
		unsigned getObjectId(void);

		//! \brief Returns if the object is protected or not
		bool isProtected(void);

		//! \brief Checks if the objects name is the same as the passed name
		bool operator == (const QString &obj_name);

		//! \brief Checks if the objects name differs from the passed name
		bool operator != (const QString &obj_name);

		//! \brief Assigns an object to other copiyng all the attributes correctly
		void operator = (BaseObject &obj);

		//! \brief Clears all the attributes used by the SchemaParser
		void clearAttributes(void);

		/*! \brief Forcing the class to be virtual. This means that derivated classes may
		 override this method in order to be possible its instatiation. */
		virtual QString getCodeDefinition(unsigned)=0;

		/*! \brief Returns the object's SQL or XML code definition. The attribute 'reduced_form'
		 indicates that the code generation will be an XML minimum representation
		 of the object. See schema file for: functions, schemas, domains, types. */
		QString getCodeDefinition(unsigned def_type, bool reduced_form);

		//! \brief Returns if the specified type accepts to have a schema assigned
		static bool acceptsSchema(ObjectType obj_type);

		//! \brief Returns if the specified type accepts to have an owner assigned
		static bool acceptsOwner(ObjectType obj_type);

		//! \brief Returns if the specified type accepts to have a tablespace assigned
		static bool acceptsTablespace(ObjectType obj_type);

		//! \brief Returns if the specified type accepts to have a collation assigned
		static bool acceptsCollation(ObjectType obj_type);

		//! \brief Returns if the object accepts to have a schema assigned
		bool acceptsSchema(void);

		//! \brief Returns if the object accepts to have an owner assigned
		bool acceptsOwner(void);

		//! \brief Returns if the object accepts to have a tablespace assigned
		bool acceptsTablespace(void);

		//! \brief Returns if the object accepts to have a collation assigned
		bool acceptsCollation(void);

		//! \brief Swap the the ids of the specified objects
		static void swapObjectsIds(BaseObject *obj1, BaseObject *obj2);

		/*! \brief Returns the valid object types in a vector. The types
		BASE_OBJECT, TYPE_ATTRIBUTE and BASE_TABLE aren't included in return vector */
		static vector<ObjectType> getObjectTypes(void);

		friend class DatabaseModel;
};

#endif
