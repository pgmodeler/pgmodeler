/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: DatabaseModel
# Description: This class reunites and manages all the other object types.
#              Additionally, this class, saves, loads and generates the
#              XML/SQL definition of a complete database.
# Creation date: 20/10/2006
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
#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include <QFile>
#include <QObject>
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
#include <algorithm>
#include <locale.h>

class DatabaseModel:  public QObject, public BaseObject {
 private:
  Q_OBJECT

  static unsigned dbmodel_id;

  //Database encoding
  EncodingType encoding;

  //Template database
  QString template_db,
          //Model's author
          author,
          //Database localizations (LC_CTYPE, LC_COLLATE)
          localizations[2];

  //Maximum number of connections
  int conn_limit;

  //Vectors that stores all the objects types
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

  /* Stores the xml definition for special objects. This map is used
     when revalidating the relationships */
  map<unsigned, QString> xml_special_objs;

  //Indicates if the model is being loaded
  bool loading_model;

  /* Returns an object seaching it by its name and type. The third parameter stores
     the object index */
  BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

  //Generic method that adds an object to the model
  void __addObject(BaseObject *object, int obj_idx=-1);

  //Generic method that removes an object to the model
  void __removeObject(BaseObject *object, int obj_idx=-1);

  //Recreates the special object from the passed xml code buffer
  void createSpecialObject(const QString &xml_def, unsigned obj_id=0);

  //Removes an user defined type (domain or type)
  void removeUserType(BaseObject *object, int obj_idx);

  /* Returns the object on the model that represents the base pgsql type. The possible
     returned object can be: table, sequence, domain or type */
  BaseObject *getObjectPgSQLType(PgSQLType type);

 public:
  DatabaseModel(void);
  ~DatabaseModel(void);

  //Returns the complete object list according to the type
  vector<BaseObject *> *getObjectList(ObjectType obj_type);

  //Disconnects all the relationships in a ordered way
  void disconnectRelationships(void);

  /* Detects and stores the XML for special objects (that is referencing columns created
     by relationship) in order to be reconstructed in a posterior moment */
  void storeSpecialObjectsXML(void);

  //Validates all the relationship, propagating all column modifications over the tables
  void validateRelationships(void);

  //Validates the code definition for the passed object
  static QString validateObjectDefinition(BaseObject *object, unsigned def_type);

  //Returns the list of objects that belongs to the passed schema
  vector<BaseObject *> getObjects(ObjectType obj_type, BaseObject *schema=NULL);

  //Returns the object index searching by its name
  int getObjectIndex(const QString &name, ObjectType obj_type);

  //Retuns the passed object index
  int getObjectIndex(BaseObject *object);

  //Adds an object to the model
  void addObject(BaseObject *object, int obj_idx=-1);

  //Removes an object from the model
  void removeObject(BaseObject *object, int obj_idx=-1);

  //Removes an object using its index and type
  void removeObject(unsigned obj_idx, ObjectType obj_type);

  //Returns an object from the model using its index and type
  BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

  //Loads a database model from a file
  void loadModel(const QString &filename);

  //Sets the database encoding
  void setEncoding(EncodingType encod);

  //Sets the database localizations
  void setLocalization(int localiz_id, const QString &value);

  //Sets the connections limit
  void setConnectionLimit(int conn_lim);

  //Sets the template database
  void setTemplateDB(const QString &temp_db);

  //Sets the model's author
  void setAuthor(const QString &author);

  //Sets the protection for all objects on the model
  void setProtected(bool value);

  //Destroys all the objects
  void destroyObjects(void);

  //Returns the object count for the specified type
  unsigned getObjectCount(ObjectType obj_type);

  //Returns the object count for all object types
  unsigned getObjectCount(void);

  //Retuns the specified localization value
  QString getLocalization(int localiz_id);

  //Returns the connection limit
  int getConnectionLimit(void);

  //Returns the template database
  QString getTemplateDB(void);

  //Returns the model's author
  QString getAuthor(void);

  //Returns the database enconding
  EncodingType getEncoding(void);

  //Saves the specified code definition for the model on the specified filename
  void saveModel(const QString &filename, unsigned def_type);

  /* Returns the complete SQL/XML defintion for the entire model (including all the other objects).
     The parameter 'export_file' is used to format the generated code in a way that can be saved
     in na SQL file and executed later on the DBMS server. This parameter is only used for SQL definition. */
  QString getCodeDefinition(unsigned def_type, bool export_file);

  //Returns the complete SQL/XML definition for the entire model (including all the other objects).
  QString getCodeDefinition(unsigned def_type);

  //Returns the code definition only for the database (excluding the definition of the other objects)
  QString __getCodeDefinition(unsigned def_type);

  void addRelationship(BaseRelationship *rel, int obj_idx=-1);
  void removeRelationship(BaseRelationship *rel, int obj_idx=-1);
  BaseRelationship *getRelationship(unsigned obj_idx, ObjectType rel_type);

  /* Searchs and returns the relationship between the specified tables. If the second parameter
     is ommited (NULL), the method returns the first relationship where the source table is
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

  void addPermission(Permission *perm);
  void removePermission(Permission *perm);
  int getPermissionIndex(Permission *perm);

  //Removes all the permission related to the passed object
  void removePermissions(BaseObject *object);

  //Returns all the permissions related to the passed object
  void getPermissions(BaseObject *object, vector<Permission *> &perms);

  //Returns the object searching by its name and type
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
  Permission *createPermission(void);
  Textbox *createTextbox(void);
  BaseRelationship *createRelationship(void);
  Constraint *createConstraint(BaseObject *object);
  Index *createIndex(Table *table);
  Trigger *createTrigger(Table *table);

  //Creates/removes the relationship between the passed view and the referecend tables
  void updateViewRelationships(View *view);

  //Creates/removes the relationship between the passed table and the referecend tables on its foreign keys
  void updateTableFKRelationships(Table *table);

  /* Validates the removal of the specified column raising errors when the passed object
     is still being referecend */
  void validateColumnRemoval(Column *column);

  //Validates the relationship to reflect the modifications on the column/constraint of the passed table
  void validateRelationships(TableObject *object, Table *parent_tab);

  /* Checks if from the passed relationship some redundacy is found. Redundancy generates infinite column
     propagation over the tables. This method raises an error when found some. */
  void checkRelationshipRedundancy(Relationship *rel);

  /* Returns all the objects that the object depends on. The boolean paramenter is used to include the
     indirect dependencies on the search. Indirect dependencies are objects that is not linked directly to
     the informed object, e.g., a schema linked to a table that is referenced in a view */
  void getObjectDependecies(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_indirect_deps=false);

  /* Returns all the objects that references the passed object. The boolean paramenter is used to performance purpose,
     generally applied when excluding objects, this means that the method will stop the search when the first
     reference is found */
  void getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode=false);

  //Marks all the graphical objects as modified forcing their redraw
  void setObjectsModified(void);

 signals:
  //Signal emitted when a new object is added to the model
  void s_objectAdded(BaseObject *objeto);

  //Signal emitted when an object is removed from the model
  void s_objectRemoved(BaseObject *objeto);

  //Signal emitted when an object is created from a xml code
  void s_objectLoaded(int progresso, QString object_id, unsigned id_icone);
};

#endif
