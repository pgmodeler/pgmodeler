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
\class DatabaseModel
\brief This class reunites and manages all the other object types.
Additionally, this class, saves, loads and generates the XML/SQL definition of a complete database.
\note <strong>Creation date:</strong> 20/10/2006
*/

#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include <QFile>
#include <QObject>
#include <QStringList>
#include "baseobject.h"
#include "table.h"
#include "function.h"
#include "schema.h"
#include "view.h"
#include "sequence.h"
#include "relationship.h"
#include "textbox.h"
#include "role.h"
#include "type.h"
#include "tablespace.h"
#include "language.h"
#include "aggregate.h"
#include "cast.h"
#include "conversion.h"
#include "operatorclass.h"
#include "xmlparser.h"
#include "permission.h"
#include "domain.h"
#include "collation.h"
#include "extension.h"
#include <algorithm>
#include <locale.h>

class DatabaseModel:  public QObject, public BaseObject {
	private:
		Q_OBJECT

		static unsigned dbmodel_id;

		//! \brief Database encoding
		EncodingType encoding;

		//! \brief Template database
		QString template_db,
						//! \brief Model's author
						author,
						//! \brief Database localizations (LC_CTYPE, LC_COLLATE)
						localizations[2];

		//! \brief Maximum number of connections
		int conn_limit;

		//! \brief Vectors that stores all the objects types
		vector<BaseObject *> textboxes;
		vector<BaseObject *> relationships;
		vector<BaseObject *> base_relationships;
		vector<BaseObject *> functions;
		vector<BaseObject *> schemas;
		vector<BaseObject *> views;
		vector<BaseObject *> tables;
		vector<BaseObject *> types;
		vector<BaseObject *> roles;
		vector<BaseObject *> tablespaces;
		vector<BaseObject *> languages;
		vector<BaseObject *> aggregates;
		vector<BaseObject *> casts;
		vector<BaseObject *> conversions;
		vector<BaseObject *> operators;
		vector<BaseObject *> op_classes;
		vector<BaseObject *> op_families;
		vector<BaseObject *> domains;
		vector<BaseObject *> sequences;
		vector<BaseObject *> permissions;
		vector<BaseObject *> collations;
		vector<BaseObject *> extensions;

		/*! \brief Stores the xml definition for special objects. This map is used
		 when revalidating the relationships */
		map<unsigned, QString> xml_special_objs;

		//! \brief Indicates if the model is being loaded
		bool loading_model,

		/*! \brief Indicates if the model was invalidated due to operations like insert / remove objects.
		When this flag is set it's recommend to revalidate the model using the Model validation tool */
		invalidated,

		//! \brief Indicates that appended SQL commands must be put at the very end of model definition
		append_at_eod;

		/*! \brief Returns an object seaching it by its name and type. The third parameter stores
		 the object index */
		BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

		//! \brief Generic method that adds an object to the model
		void __addObject(BaseObject *object, int obj_idx=-1);

		//! \brief Generic method that removes an object to the model
		void __removeObject(BaseObject *object, int obj_idx=-1);

		//! \brief Recreates the special object from the passed xml code buffer
		void createSpecialObject(const QString &xml_def, unsigned obj_id=0);

		//! \brief Removes an user defined type (domain or type)
		void removeUserType(BaseObject *object, int obj_idx);

		/*! \brief Returns the object on the model that represents the base pgsql type. The possible
		 returned object can be: table, sequence, domain or type */
		BaseObject *getObjectPgSQLType(PgSQLType type);

		//! \brief Creates a IndexElement or ExcludeElement from XML depending on type of the 'elem' param.
		void createElement(Element &elem, TableObject *tab_obj, BaseObject *parent_obj);

	protected:
		//! \brief Indicate if the model invalidated
		void setInvalidated(bool value);

	public:
		DatabaseModel(void);
		~DatabaseModel(void);

		//! \brief Returns the complete object list according to the type
		vector<BaseObject *> *getObjectList(ObjectType obj_type);

		//! \brief Disconnects all the relationships in a ordered way
		void disconnectRelationships(void);

		/*! \brief Detects and stores the XML for special objects (that is referencing columns created
		 by relationship) in order to be reconstructed in a posterior moment */
		void storeSpecialObjectsXML(void);

		//! \brief Validates all the relationship, propagating all column modifications over the tables
		void validateRelationships(void);

		//! \brief Returns the list of specified object type that belongs to the passed schema
		vector<BaseObject *> getObjects(ObjectType obj_type, BaseObject *schema=nullptr);

		//! \brief Returns the list of objects (all types) that belongs to the passed schema
		vector<BaseObject *> getObjects(BaseObject *schema);

		//! \brief Returns the object index searching by its name
		int getObjectIndex(const QString &name, ObjectType obj_type);

		//! \brief Retuns the passed object index
		int getObjectIndex(BaseObject *object);

		//! \brief Adds an object to the model
		void addObject(BaseObject *object, int obj_idx=-1);

		//! \brief Removes an object from the model
		void removeObject(BaseObject *object, int obj_idx=-1);

		//! \brief Removes an object using its index and type
		void removeObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Returns an object from the model using its index and type
		BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Loads a database model from a file
		void loadModel(const QString &filename);

		//! \brief Sets the database encoding
		void setEncoding(EncodingType encod);

		//! \brief Sets the database localizations
		void setLocalization(int localiz_id, const QString &value);

		//! \brief Sets the connections limit
		void setConnectionLimit(int conn_lim);

		//! \brief Sets the template database
		void setTemplateDB(const QString &temp_db);

		//! \brief Sets the model's author
		void setAuthor(const QString &author);

		//! \brief Sets the protection for all objects on the model
		void setProtected(bool value);

		//! \brief Sets the sql appending at end of entire model definition
		void setAppendAtEOD(bool value);

		//! \brief Returns the current state of the sql appeding at end of entire model definition
		bool isAppendAtEOD(void);

		//! \brief Destroys all the objects
		void destroyObjects(void);

		//! \brief Returns the object count for the specified type
		unsigned getObjectCount(ObjectType obj_type);

		//! \brief Returns the object count for all object types
		unsigned getObjectCount(void);

		//! \brief Retuns the specified localization value
		QString getLocalization(int localiz_id);

		//! \brief Returns the connection limit
		int getConnectionLimit(void);

		//! \brief Returns the template database
		QString getTemplateDB(void);

		//! \brief Returns the model's author
		QString getAuthor(void);

		//! \brief Returns the database enconding
		EncodingType getEncoding(void);

		//! \brief Returns if the model is invalidated. When true its recommended to validate model using Model validation tool
		bool isInvalidated(void);

		//! \brief Saves the specified code definition for the model on the specified filename
		void saveModel(const QString &filename, unsigned def_type);

		/*! \brief Returns the complete SQL/XML defintion for the entire model (including all the other objects).
		 The parameter 'export_file' is used to format the generated code in a way that can be saved
		 in na SQL file and executed later on the DBMS server. This parameter is only used for SQL definition. */
		virtual QString getCodeDefinition(unsigned def_type, bool export_file) final;

		//! \brief Returns the complete SQL/XML definition for the entire model (including all the other objects).
		virtual QString getCodeDefinition(unsigned def_type) final;

		//! \brief Returns the code definition only for the database (excluding the definition of the other objects)
		QString __getCodeDefinition(unsigned def_type);

		void addRelationship(BaseRelationship *rel, int obj_idx=-1);
		void removeRelationship(BaseRelationship *rel, int obj_idx=-1);
		BaseRelationship *getRelationship(unsigned obj_idx, ObjectType rel_type);

		/*! \brief Searchs and returns the relationship between the specified tables. If the second parameter
		 is ommited (nullptr), the method returns the first relationship where the source table is
		 participating */
		BaseRelationship *getRelationship(BaseTable *src_tab, BaseTable *dst_tab);

		void addTextbox(Textbox *txtbox, int obj_idx=-1);
		void removeTextbox(Textbox *txtbox, int obj_idx=-1);
		Textbox *getTextbox(unsigned obj_idx);

		void addFunction(Function *func, int obj_idx=-1);
		void removeFunction(Function *func, int obj_idx=-1);
		Function *getFunction(unsigned obj_idx);

		void addSchema(Schema *schema, int obj_idx=-1);
		void removeSchema(Schema *schema, int obj_idx=-1);
		Schema *getSchema(unsigned obj_idx);

		void addView(View *view, int obj_idx=-1);
		void removeView(View *view, int obj_idx=-1);
		View *getView(unsigned obj_idx);

		void addTable(Table *table, int obj_idx=-1);
		void removeTable(Table *table, int obj_idx=-1);
		Table *getTable(unsigned obj_idx);

		void addType(Type *type, int obj_idx=-1);
		void removeType(Type *type, int obj_idx=-1);
		Type *getType(unsigned obj_idx);

		void addRole(Role *role, int obj_idx=-1);
		void removeRole(Role *role, int obj_idx=-1);
		Role *getRole(unsigned obj_idx);

		void addTablespace(Tablespace *tabspc, int obj_idx=-1);
		void removeTablespace(Tablespace *tabspc, int obj_idx=-1);
		Tablespace *getTablespace(unsigned obj_idx);

		void addLanguage(Language *lang, int obj_idx=-1);
		void removeLanguage(Language *lang, int obj_idx=-1);
		Language *getLanguage(unsigned obj_idx);

		void addAggregate(Aggregate *aggreg, int obj_idx=-1);
		void removeAggregate(Aggregate *aggreg, int obj_idx=-1);
		Aggregate *getAggregate(unsigned obj_idx);

		void addCast(Cast *cast, int obj_idx=-1);
		void removeCast(Cast *cast, int obj_idx=-1);
		Cast *getCast(unsigned obj_idx);

		void addConversion(Conversion *conv, int obj_idx=-1);
		void removeConversion(Conversion *conv, int obj_idx=-1);
		Conversion *getConversion(unsigned obj_idx);

		void addOperator(Operator *oper, int obj_idx=-1);
		void removeOperator(Operator *oper, int obj_idx=-1);
		Operator *getOperator(unsigned obj_idx);

		void addOperatorClass(OperatorClass *op_class, int obj_idx=-1);
		void removeOperatorClass(OperatorClass *op_class, int obj_idx=-1);
		OperatorClass *getOperatorClass(unsigned obj_idx);

		void addOperatorFamily(OperatorFamily *familia_op, int obj_idx=-1);
		void removeOperatorFamily(OperatorFamily *op_family, int obj_idx=-1);
		OperatorFamily *getOperatorFamily(unsigned obj_idx);

		void addDomain(Domain *domain, int obj_idx=-1);
		void removeDomain(Domain *dominio, int obj_idx=-1);
		Domain *getDomain(unsigned obj_idx);

		void addSequence(Sequence *sequence, int obj_idx=-1);
		void removeSequence(Sequence *sequence, int obj_idx=-1);
		Sequence *getSequence(unsigned obj_idx);

		void addCollation(Collation *collation, int obj_idx=-1);
		void removeCollation(Collation *collation, int obj_idx=-1);
		Collation *getCollation(unsigned obj_idx);

		void addExtension(Extension *extension, int obj_idx=-1);
		void removeExtension(Extension *extension, int obj_idx=-1);
		Extension *getExtension(unsigned obj_idx);

		void addPermission(Permission *perm);
		void removePermission(Permission *perm);
		int getPermissionIndex(Permission *perm);

		//! \brief Inserts a list of permissions into the model
		void addPermissions(vector<Permission *> &perms);

		//! \brief Removes all the permission related to the passed object
		void removePermissions(BaseObject *object);

		//! \brief Returns all the permissions related to the passed object
		void getPermissions(BaseObject *object, vector<Permission *> &perms);

		//! \brief Returns the object searching by its name and type
		BaseObject *getObject(const QString &name, ObjectType obj_type);

		ObjectType getObjectType(const QString &type_name);
		void setBasicAttributes(BaseObject *object);

		PgSQLType createPgSQLType(void);
		BaseObject *createObject(ObjectType obj_type);
		Role *createRole(void);
		Tablespace *createTablespace(void);
		Schema *createSchema(void);
		Language *createLanguage(void);
		Function *createFunction(void);
		Parameter createParameter(void);
		TypeAttribute createTypeAttribute(void);
		Type *createType(void);
		Domain *createDomain(void);
		Cast *createCast(void);
		Conversion *createConversion(void);
		Operator *createOperator(void);
		OperatorFamily *createOperatorFamily(void);
		OperatorClass *createOperatorClass(void);
		Aggregate *createAggregate(void);
		Table *createTable(void);
		Column *createColumn(void);
		Rule *createRule(void);
		Sequence *createSequence(bool ignore_onwer=false);
		View *createView(void);
		Collation *createCollation(void);
		Extension *createExtension(void);
		Permission *createPermission(void);
		Textbox *createTextbox(void);
		BaseRelationship *createRelationship(void);
		Constraint *createConstraint(BaseObject *parent_obj);
		Index *createIndex(Table *table);
		Trigger *createTrigger(BaseTable *table);

		//! \brief Creates/removes the relationship between the passed view and the referecend tables
		void updateViewRelationships(View *view);

		//! \brief Creates/removes the relationship between the passed table and the referecend tables on its foreign keys
		void updateTableFKRelationships(Table *table);

		/*! \brief Validates the removal of the specified column raising errors when the passed object
		 is still being referecend */
		void validateColumnRemoval(Column *column);

		//! \brief Validates the relationship to reflect the modifications on the column/constraint of the passed table
		void validateRelationships(TableObject *object, Table *parent_tab);

		/*! \brief Checks if from the passed relationship some redundacy is found. Redundancy generates infinite column
		 propagation over the tables. This method raises an error when found some. */
		void checkRelationshipRedundancy(Relationship *rel);

		/*! \brief Returns all the objects that the object depends on. The boolean paramenter is used to include the
		 indirect dependencies on the search. Indirect dependencies are objects that is not linked directly to
		 the informed object, e.g., a schema linked to a table that is referenced in a view */
		void getObjectDependecies(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_indirect_deps=false);

		/*! \brief Returns all the objects that references the passed object. The boolean paramenter is used to performance purpose,
		 generally applied when excluding objects, this means that the method will stop the search when the first
		 reference is found */
		void getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode=false);

		//! \brief Marks all the graphical objects as modified forcing their redraw
		void setObjectsModified(void);

		/*! \brief Updates the user type names which belongs to the passed schema. This method must be executed whenever
		 the schema is renamed to propagate the new name to the user types on the PgSQLTypes list. Additionally
		 the previous schema name must be informed in order to rename the types correctly */
		void validateSchemaRenaming(Schema *schema, const QString &prev_sch_name);

		/*! \brief Creates the system objects: public schema and languages C, SQL and plpgsql. This method ignores one of these
		objects if some of them already exists */
		void createSystemObjects(bool create_public);

		/*! \brief Returns a list of object searching them using the specified pattern. The search can be delimited by filtering the object's types.
		The additional bool params are: case sensitive name search, name pattern is a regexp, exact match for names. */
		vector<BaseObject *> findObjects(const QString &pattern, vector<ObjectType> types, bool format_obj_names,
																		 bool case_sensitive, bool is_regexp, bool exact_match);

	signals:
		//! \brief Signal emitted when a new object is added to the model
		void s_objectAdded(BaseObject *objeto);

		//! \brief Signal emitted when an object is removed from the model
		void s_objectRemoved(BaseObject *objeto);

		//! \brief Signal emitted when an object is created from a xml code
		void s_objectLoaded(int progress, QString object_id, unsigned icon_id);

	friend class ModelValidationHelper;
};

#endif
