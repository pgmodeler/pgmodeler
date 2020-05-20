/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "tag.h"
#include "eventtrigger.h"
#include "genericsql.h"
#include "foreigndatawrapper.h"
#include "foreignserver.h"
#include "usermapping.h"
#include "foreigntable.h"
#include <algorithm>
#include <locale.h>

class ModelWidget;

class DatabaseModel:  public QObject, public BaseObject {
	private:
		Q_OBJECT

		/*! \brief Stores the references of all object lists of each type. This map is used by getObjectList() in order
		 * to return the list according to the provided type */
		map<ObjectType, vector<BaseObject *> *> obj_lists;

		static unsigned dbmodel_id;

		XmlParser xmlparser;

		//! \brief Stores the layers names and active layer to write them on XML code
		QStringList layers;

		QList<unsigned> active_layers;

		//! \brief Stores the model widget that is managing this database model instance
		ModelWidget *model_wgt;

		//! \brief Database encoding
		EncodingType encoding;

		//! \brief Template database
		QString template_db,

		//! \brief Model's author
		author,

		//! \brief Database localizations (LC_CTYPE, LC_COLLATE)
		localizations[2];

		//! \brief Stores the objects of each type that are considered the default ones associated to new objects
		map<ObjectType, BaseObject *> default_objs;

		//! \brief Maximum number of connections
		int conn_limit;

		//! \brief Indicates if the database can be used as template
		bool is_template,

		//! \brief Indicates if the database accepts connection
		allow_conns;

		//! \brief Vectors that stores all the objects types
		vector<BaseObject *> textboxes,
		relationships,
		base_relationships,
		functions,
		schemas,
		views,
		tables,
		types,
		roles,
		tablespaces,
		languages,
		aggregates,
		casts,
		conversions,
		operators,
		op_classes,
		op_families,
		domains,
		sequences,
		permissions,
		collations,
		extensions,
		tags,
		eventtriggers,
		genericsqls,
		fdata_wrappers,
		foreign_servers,
		usermappings,
		foreign_tables;

		/*! \brief Stores the xml definition for special objects. This map is used
		 when revalidating the relationships */
		map<unsigned, QString> xml_special_objs;

		/*! \brief Stores the special objects considered invalid after a relationships revalidation.
		 * This vector is destroyed only when the model is destroyed too in order to avoid segfaults */
		vector<BaseObject *> invalid_special_objs;

		//! \brief Indicates if the model is being loaded
		bool loading_model,

		/*! \brief Indicates if the model was invalidated due to operations like insert / remove objects.
		When this flag is set it's recommend to revalidate the model using the Model validation tool */
		invalidated,

		//! \brief Indicates that appended SQL commands must be put at the very end of model definition
		append_at_eod,

		//! \brief Indicates that prepended SQL commands must be put at the very beginning of model definition
		prepend_at_bod;

		//! \brief Stores the last position on the model where the user was editing objects
		QPoint last_pos;

		double last_zoom;

		//! \brief Returns an object seaching it by its name and type. The third parameter stores the object index
		BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

		//! \brief Generic method that adds an object to the model
		void __addObject(BaseObject *object, int obj_idx=-1);

		/*! \brief Generic method that removes an object to the model. The boolean param is used
		to enable/disable reference checking before remove the object from model. */
		void __removeObject(BaseObject *object, int obj_idx=-1, bool check_refs=true);

		//! \brief Recreates the special object from the passed xml code buffer
		void createSpecialObject(const QString &xml_def, unsigned obj_id=0);

		//! \brief Removes an user defined type (domain or type)
		void removeUserType(BaseObject *object, int obj_idx);

		/*! \brief Returns the object on the model that represents the base pgsql type. The possible
		 returned object can be: table, sequence, domain or type */
		BaseObject *getObjectPgSQLType(PgSqlType type);

		//! \brief Creates a IndexElement or ExcludeElement from XML depending on type of the 'elem' param.
		void createElement(Element &elem, TableObject *tab_obj, BaseObject *parent_obj);

		//! \brief Returns extra error info when loading database models
		QString getErrorExtraInfo();

		/*! \brief This method forces the indication that the model is being loaded or not by setting the attribute loading_model.
		 * The attribute loading_model causes the model perform certain operations only when model starts/ends the loading process,
		 * for instance, if loading_model = true graphical objects will be rendered only when the loading process finishes (loading_model =false)
		 * otherwise the objects are rendered as they are added to the model. The drawback of this approach is, depending on the operation being used after
		 * calling this method, the user is obligated to call the methdo setObjectsModified() to force the graphical objects rendering. */
		void setLoadingModel(bool value);

		//! \brief Set the initial capacity of the objects list for a optimized memory usage
		void setObjectListsCapacity(unsigned capacity);

		void getViewReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode);
		void getPhysicalTableReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getFunctionReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getSchemaReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getUserDefTypesReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getRoleReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getTablespaceReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getLanguageReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getOpClassReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getOperatorReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getCollationReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getOpFamilyReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getColumnReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getTagReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getSequenceReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getFdwReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);
		void getServerReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode);

		void getOpClassDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getDomainDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getCastDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getFunctionDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getAggregateDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getLanguageDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getOperatorDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getRoleDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getRelationshipDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getSequenceDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getColumnDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getTriggerDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getIndexDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getPolicyDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getPhysicalTableDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getTypeDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getViewDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);
		void getGenericSQLDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps);


	protected:
		void setLayers(const QStringList &layers);
		void setActiveLayers(const QList<unsigned> &layers);
		QStringList getLayers();
		QList<unsigned> getActiveLayers();

	public:
		static constexpr unsigned MetaDbAttributes=1,	//! \brief Handle database model attribute when save/load metadata file
		MetaObjsPositioning=2,	//! \brief Handle objects' positioning when save/load metadata file
		MetaObjsProtection=4,	//! \brief Handle objects' protection status when save/load metadata file
		MetaObjsSqlDisabled=8,	//! \brief Handle objects' sql disabled status when save/load metadata file
		MetaObjsCustomSql=16,	//! \brief Handle object's custom sql when save/load metadata file
		MetaObjsCustomColors=32,	//! \brief Handle object's custom colors when save/load metadata file
		MetaObjsFadeOut=64,	//! \brief Handle graphical object's fade out status when save/load metadata file
		MetaObjsCollapseMode=128,	//! \brief Handle tables and views collapse mode when save/load metadata file
		MetaTextboxObjs=256,	//! \brief Handle textboxes object when save/load metadata file
		MetaTagObjs=512,	//! \brief Handle tags object when save/load metadata file
		MetaGenericSqlObjs=1024,	//! \brief Handle generic sql object when save/load metadata file
		MetaObjsAliases=2048,	//! \brief Handle the object's aliases (graphical objects and table children objects) when save/load metadata file
		MetaObjsZStackValue=4096,	//! \brief Handle the object's Z stack value
		MetaAllInfo=8191;	//! \brief Handle all metadata information about objects when save/load metadata file

		DatabaseModel();

		//! \brief Creates a database model and assign the model widget which will manage this instance
		explicit DatabaseModel(ModelWidget *model_wgt);

		virtual ~DatabaseModel();

		//! \brief Returns the model widget that is managing the current database instance
		ModelWidget *getModelWidget();

		//! \brief Returns the complete object list according to the type
		vector<BaseObject *> *getObjectList(ObjectType obj_type);

		//! \brief Disconnects all the relationships in a ordered way
		void disconnectRelationships();

		/*! \brief Detects and stores the XML for special objects (that is referencing columns created
		 by relationship) in order to be reconstructed in a posterior moment */
		void storeSpecialObjectsXML();

		//! \brief Validates all the relationship, propagating all column modifications over the tables
		void validateRelationships();

		/*! \brief Returns an object seaching it by its name and on the group objects specified by "types".
		 * If the types list is empty the method will return nullptr. */
		BaseObject *getObject(const QString &name, const vector<ObjectType> &types);

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

		/*! \brief Loads a database model from a file. In case of loading errors
		the objects in the model will not be destroyed automatically. The user need to call
		destroyObjects() or delete the entire model */
		void loadModel(const QString &filename);

		//! \brief Sets the database encoding
		void setEncoding(EncodingType encod);

		//! \brief Sets the database localizations
		void setLocalization(unsigned localiz_id, const QString &value);

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

		//! \brief Sets the sql prepeding at beginning of entire model definition
		void setPrependAtBOD(bool value);

		void setDefaultObject(BaseObject *object, ObjectType obj_type=ObjectType::BaseObject);

		void setIsTemplate(bool value);

		void setAllowConnections(bool value);

		//! \brief Returns the current state of the sql appeding at end of entire model definition
		bool isAppendAtEOD();

		//! \brief Returns the current state of the sql prepeding at beginning of entire model definition
		bool isPrependedAtBOD();

		bool isTemplate();

		bool isAllowConnections();

		//! \brief Destroys all the objects
		void destroyObjects();

		//! \brief Returns the object count for the specified type
		unsigned getObjectCount(ObjectType obj_type);

		//! \brief Returns the object count for all object types.
		unsigned getObjectCount();

		unsigned getMaxObjectCount();

		//! \brief Retuns the specified localization value
		QString getLocalization(unsigned localiz_id);

		//! \brief Returns the connection limit
		int getConnectionLimit();

		//! \brief Returns the template database
		QString getTemplateDB();

		//! \brief Returns the model's author
		QString getAuthor();

		//! \brief Returns the database enconding
		EncodingType getEncoding();

		BaseObject *getDefaultObject(ObjectType obj_type);

		//! \brief Returns if the model is invalidated. When true its recommended to validate model using Model validation tool
		bool isInvalidated();

		//! \brief Indicate if the model invalidated
		void setInvalidated(bool value);

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

		/*! \brief Returns the creation order of objects in each definition type (SQL or XML).

		The parameter incl_relnn_objs when 'true' includes the generated objects (table and constraint)
		of the many-to-many relationships instead of the relationships themselves. The incl_relnn_objs is
		is accepted only when the creation order for SQL code is being generated, for XML, it'll simply ignored.

		The parameter incl_rel1n_constr when 'true' includes the generated foreign and unique keys
		of one-to-one|many relationships instead of the relationships themselves. This parameter is
		is accepted only when the creation order for SQL code is being generated, for XML, it'll simply ignored. */
		map<unsigned, BaseObject *> getCreationOrder(unsigned def_type, bool incl_relnn_objs=false, bool incl_rel1n_constrs=false);

		/*! \brief Returns a list containig all the object need to create the 'object' in the proper order.
		If 'only_children' is set only children objects will be included in the list (for tables, views or schemas).
		If 'only_children' is not set, the method will automatically include dependencies, children and permissions of
		the object. */
		vector<BaseObject *> getCreationOrder(BaseObject *object, bool only_children);

		void addRelationship(BaseRelationship *rel, int obj_idx=-1);
		void removeRelationship(BaseRelationship *rel, int obj_idx=-1);
		BaseRelationship *getRelationship(unsigned obj_idx, ObjectType rel_type);
		BaseRelationship *getRelationship(const QString &name);

		/*! \brief Searchs and returns the relationship between the specified tables. If the second parameter
		 is ommited (nullptr), the method returns the first relationship where the source table is
		 participating. The optional parameter ref_fk will search for foreign key relationships which the reference foreign key
		is the one provided */
		BaseRelationship *getRelationship(BaseTable *src_tab, BaseTable *dst_tab, Constraint *ref_fk = nullptr);

		//! \brief Searchs and returns all the relationships that the specified table participates
		vector<BaseRelationship *> getRelationships(BaseTable *tab);

		void addTextbox(Textbox *txtbox, int obj_idx=-1);
		void removeTextbox(Textbox *txtbox, int obj_idx=-1);
		Textbox *getTextbox(unsigned obj_idx);
		Textbox *getTextbox(const QString &name);

		void addFunction(Function *func, int obj_idx=-1);
		void removeFunction(Function *func, int obj_idx=-1);
		Function *getFunction(unsigned obj_idx);
		Function *getFunction(const QString &signature);

		void addSchema(Schema *schema, int obj_idx=-1);
		void removeSchema(Schema *schema, int obj_idx=-1);
		Schema *getSchema(unsigned obj_idx);
		Schema *getSchema(const QString &name);

		void addView(View *view, int obj_idx=-1);
		void removeView(View *view, int obj_idx=-1);
		View *getView(unsigned obj_idx);
		View *getView(const QString &name);

		void addTable(Table *table, int obj_idx=-1);
		void removeTable(Table *table, int obj_idx=-1);
		Table *getTable(unsigned obj_idx);
		Table *getTable(const QString &name);

		void addType(Type *type, int obj_idx=-1);
		void removeType(Type *type, int obj_idx=-1);
		Type *getType(unsigned obj_idx);
		Type *getType(const QString &name);

		void addRole(Role *role, int obj_idx=-1);
		void removeRole(Role *role, int obj_idx=-1);
		Role *getRole(unsigned obj_idx);
		Role *getRole(const QString &name);

		void addTablespace(Tablespace *tabspc, int obj_idx=-1);
		void removeTablespace(Tablespace *tabspc, int obj_idx=-1);
		Tablespace *getTablespace(unsigned obj_idx);
		Tablespace *getTablespace(const QString &name);

		void addLanguage(Language *lang, int obj_idx=-1);
		void removeLanguage(Language *lang, int obj_idx=-1);
		Language *getLanguage(unsigned obj_idx);
		Language *getLanguage(const QString &name);

		void addAggregate(Aggregate *aggreg, int obj_idx=-1);
		void removeAggregate(Aggregate *aggreg, int obj_idx=-1);
		Aggregate *getAggregate(unsigned obj_idx);
		Aggregate *getAggregate(const QString &name);

		void addCast(Cast *cast, int obj_idx=-1);
		void removeCast(Cast *cast, int obj_idx=-1);
		Cast *getCast(unsigned obj_idx);
		Cast *getCast(const QString &name);

		void addConversion(Conversion *conv, int obj_idx=-1);
		void removeConversion(Conversion *conv, int obj_idx=-1);
		Conversion *getConversion(unsigned obj_idx);
		Conversion *getConversion(const QString &name);

		void addOperator(Operator *oper, int obj_idx=-1);
		void removeOperator(Operator *oper, int obj_idx=-1);
		Operator *getOperator(unsigned obj_idx);
		Operator *getOperator(const QString &signature);

		void addOperatorClass(OperatorClass *op_class, int obj_idx=-1);
		void removeOperatorClass(OperatorClass *op_class, int obj_idx=-1);
		OperatorClass *getOperatorClass(unsigned obj_idx);
		OperatorClass *getOperatorClass(const QString &name);

		void addOperatorFamily(OperatorFamily *familia_op, int obj_idx=-1);
		void removeOperatorFamily(OperatorFamily *op_family, int obj_idx=-1);
		OperatorFamily *getOperatorFamily(unsigned obj_idx);
		OperatorFamily *getOperatorFamily(const QString &name);

		void addDomain(Domain *domain, int obj_idx=-1);
		void removeDomain(Domain *dominio, int obj_idx=-1);
		Domain *getDomain(unsigned obj_idx);
		Domain *getDomain(const QString &name);

		void addSequence(Sequence *sequence, int obj_idx=-1);
		void removeSequence(Sequence *sequence, int obj_idx=-1);
		Sequence *getSequence(unsigned obj_idx);
		Sequence *getSequence(const QString &name);

		void addCollation(Collation *collation, int obj_idx=-1);
		void removeCollation(Collation *collation, int obj_idx=-1);
		Collation *getCollation(unsigned obj_idx);
		Collation *getCollation(const QString &name);

		void addExtension(Extension *extension, int obj_idx=-1);
		void removeExtension(Extension *extension, int obj_idx=-1);
		Extension *getExtension(unsigned obj_idx);
		Extension *getExtension(const QString &name);

		void addTag(Tag *tag, int obj_idx=-1);
		void removeTag(Tag *tag, int obj_idx=-1);
		Tag *getTag(unsigned obj_idx);
		Tag *getTag(const QString &name);

		void addEventTrigger(EventTrigger *evnttrig, int obj_idx=-1);
		void removeEventTrigger(EventTrigger *evnttrig, int obj_idx=-1);
		EventTrigger *getEventTrigger(unsigned obj_idx);
		EventTrigger *getEventTrigger(const QString &name);

		void addGenericSQL(GenericSQL *genericsql, int obj_idx=-1);
		void removeGenericSQL(GenericSQL *genericsql, int obj_idx=-1);
		GenericSQL *getGenericSQL(unsigned obj_idx);
		GenericSQL *getGenericSQL(const QString &name);

		void addForeignDataWrapper(ForeignDataWrapper *fdata_wrapper, int obj_idx=-1);
		void removeForeignDataWrapper(ForeignDataWrapper *fdata_wrapper, int obj_idx=-1);
		ForeignDataWrapper *getForeignDataWrapper(unsigned obj_idx);
		ForeignDataWrapper *getForeignDataWrapper(const QString &name);

		void addForeignServer(ForeignServer *server, int obj_idx=-1);
		void removeForeignServer(ForeignServer *server, int obj_idx=-1);
		ForeignServer *getForeignServer(unsigned obj_idx);
		ForeignServer *getForeignServer(const QString &name);

		void addUserMapping(UserMapping *usrmap, int obj_idx=-1);
		void removeUserMapping(UserMapping *usrmap, int obj_idx=-1);
		UserMapping *getUserMapping(unsigned obj_idx);
		UserMapping *getUserMapping(const QString &name);

		void addForeignTable(ForeignTable *table, int obj_idx=-1);
		void removeForeignTable(ForeignTable *table, int obj_idx=-1);
		ForeignTable *getForeignTable(unsigned obj_idx);
		ForeignTable *getForeignTable(const QString &name);

		void addPermission(Permission *perm);
		void removePermission(Permission *perm);

		/*! \brief Returns the specified permission's index. If exact_match is true
		then all the contents of the permission are compared against other permission,
		otherwise checks only if the roles and the objects are the same between the specified
		permission and the ones existent in the model */
		int getPermissionIndex(Permission *perm, bool exact_match);

		//! \brief Inserts a list of permissions into the model
		void addPermissions(const vector<Permission *> &perms);

		//! \brief Removes all the permission related to the passed object
		void removePermissions(BaseObject *object);

		//! \brief Returns all the permissions related to the passed object
		void getPermissions(BaseObject *object, vector<Permission *> &perms);

		//! \brief Returns the object searching by its name and type
		BaseObject *getObject(const QString &name, ObjectType obj_type);

		void setBasicAttributes(BaseObject *object);

		void configureDatabase(attribs_map &attribs);
		PgSqlType createPgSQLType();
		BaseObject *createObject(ObjectType obj_type);
		Role *createRole();
		Tablespace *createTablespace();
		Schema *createSchema();
		Language *createLanguage();
		Function *createFunction();
		Parameter createParameter();
		TypeAttribute createTypeAttribute();
		Type *createType();
		Domain *createDomain();
		Cast *createCast();
		Conversion *createConversion();
		Operator *createOperator();
		OperatorFamily *createOperatorFamily();
		OperatorClass *createOperatorClass();
		Aggregate *createAggregate();
		Table *createTable();
		Column *createColumn();
		Sequence *createSequence(bool ignore_onwer=false);
		View *createView();
		Collation *createCollation();
		Extension *createExtension();
		Tag *createTag();
		Permission *createPermission();
		Textbox *createTextbox();
		BaseRelationship *createRelationship();
		Constraint *createConstraint(BaseObject *parent_obj);
		Rule *createRule();
		Index *createIndex();
		Trigger *createTrigger();
		Policy *createPolicy();
		EventTrigger *createEventTrigger();
		GenericSQL *createGenericSQL();
		ForeignDataWrapper *createForeignDataWrapper();
		ForeignServer *createForeignServer();
		UserMapping *createUserMapping();
		ForeignTable *createForeignTable();

		template<class TableClass>
		TableClass *createPhysicalTable();

		//! \brief Update views that reference the provided table forcing the column name deduction and redraw of the former objects
		void updateViewsReferencingTable(PhysicalTable *table);

		//! \brief Creates/removes the relationship between the passed view and the referecend tables
		void updateViewRelationships(View *view, bool force_rel_removal=false);

		//! \brief Creates/removes the relationship between the passed table and the referecend tables on its foreign keys
		void updateTableFKRelationships(Table *table);

		//! \brief Updates the fk relationships for all table on the model
		void updateTablesFKRelationships();

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

		/*! \brief Recursive version of getObjectDependencies. Returns all the dependencies of the specified object but
		additionally its children objects (for schemas, tables or views) as well permissions.
		This method is less efficient than the non recursive version and is used only as an auxiliary operation for
		getCreationOrder(BaseObject *object) */
		void __getObjectDependencies(BaseObject *object, vector<BaseObject *> &objs);

		/*! \brief Returns all the objects that references the passed object. The boolean exclusion_mode is used to performance purpose,
		 generally applied when excluding objects, this means that the method will stop the search when the first
		 reference is found. The exclude_perms parameter when true will not include permissions in the references list. */
		void getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode=false, bool exclude_perms=false);

		/*! \brief Recursive version of getObjectReferences. The only difference here is that the method does not runs in exclusion mode,
		meaning that ALL objects directly or inderectly linked to the 'object' are retrieved. */
		void __getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclude_perms=false);

		/*! \brief Marks the graphical objects of the provided types as modified forcing their redraw. User can specify only a set of
	 graphical objects to be marked */
		void setObjectsModified(vector<ObjectType> types={});

		//! \brief Marks the graphical objects in the list as modified forcing their redraw.
		void setObjectsModified(vector<BaseObject *> &objects);

		/*! \brief Marks the objects with code invalidated forcing their code regeneration. User can specify only a set of
		 graphical objects to be marked */
		void setCodesInvalidated(vector<ObjectType> types={});

		/*! \brief Updates the user type names which belongs to the passed schema. This method must be executed whenever
		 the schema is renamed to propagate the new name to the user types on the PgSQLTypes list. Additionally
		 the previous schema name must be informed in order to rename the types correctly */
		void validateSchemaRenaming(Schema *schema, const QString &prev_sch_name);

		/*! \brief Creates the system objects: public schema and languages C, SQL and plpgsql. This method ignores one of these
		objects if some of them already exists */
		void createSystemObjects(bool create_public);

		/*! \brief Returns a list of object searching them using the specified pattern. The search can be delimited by filtering the object's types.
		The additional bool params are: case sensitive name search, name pattern is a regexp, exact match for names. */
		vector<BaseObject *> findObjects(const QString &pattern, vector<ObjectType> types,
																		 bool case_sensitive, bool is_regexp, bool exact_match,
																		 const QString &search_attr = Attributes::Name);

		void setLastPosition(const QPoint &pnt);
		QPoint getLastPosition();

		void setLastZoomFactor(double zoom);
		double getLastZoomFactor();

		/*! \brief This method exposes the XML parser for the outside world. In order to create objects from xml code inside the current
		 database model you need first get the parser (through this method), populate the parser with the desired XML and then call
		 the create* method.

		\note: This is not the better approach and certainly will be changed in future releases */
		XmlParser *getXMLParser();

		//! \brief Returns the ALTER definition between the current model and the provided one
		virtual QString getAlterDefinition(BaseObject *object) final;

		//! \brief Returns the data dictionary of all tables in a single HTML code
		void getDataDictionary(attribs_map &datadict, bool browsable, bool splitted);

		//! \brief Saves the data dictionary of all tables in a single HTML file or splitted in several files for each table
		void saveDataDictionary(const QString &path, bool browsable, bool splitted);

		/*! \brief Save the graphical objects positions, custom colors and custom points (for relationship lines) to an special file
				that can be loaded by another model in order to change their objects position */
		void saveObjectsMetadata(const QString &filename, unsigned options=MetaAllInfo);

		//! \brief Load the file containing the objects positioning to be applied to the model
		void loadObjectsMetadata(const QString &filename, unsigned options=MetaAllInfo);

	signals:
		//! \brief Signal emitted when a new object is added to the model
		void s_objectAdded(BaseObject *object);

		//! \brief Signal emitted when an object is removed from the model
		void s_objectRemoved(BaseObject *object);

		//! \brief Signal emitted when an object is created from a xml code
		void s_objectLoaded(int progress, QString object_id, unsigned obj_type);

	friend class DatabaseImportHelper;
	friend class ModelWidget;
	friend class PgModelerCliApp;
};

#endif
