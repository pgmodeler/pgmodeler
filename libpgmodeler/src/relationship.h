/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Relationship
# Description: This class implements the operations to create relationship between tables
#              also generates the SQL code definition to represente the table link on PostgreSQL.
# Creation date: 20/11/2006
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
#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "baserelationship.h"
#include "tabela.h"
#include "textbox.h"

/*
### Relationship implementation rules ###

[One to One relationship (1:1)]

(0,1) ---<>--- (0,1)
   >> Columns are added on the destination table in order to represent the relationship.
   >> The columns that represents the foreign key must accept null values.
   >> The policy for DELETE and UPDATE events on the foreign key must be SET NULL.
   >> The relationship attributes and constraints are added on same table where the
      foreign key columns are added.

(1,1) ---<>--- (0,1)
(0,1) ---<>--- (1,1)
   >> Columns ared added on the table which minimum cardinality is 0.
   >> The columns that represents the foreign key must accept null values.
   >> The policy for DELETE and UPDATE events on the foreign key must be SET NULL.
   >> The relationship attributes and constraints are added on same table where the
      foreign key columns are added.

(1,1) ---<>--- (1,1)
   >> Not implemented because breaks the user modeling since its necessary to
      merge the envolved tables. If the user persists in creation of this type
      of relationship an error is raised.

------

[One to Many relationship (1:n)]

(0,1) ---<>--- (0,n)
(0,1) ---<>--- (1,n)
(1,1) ---<>--- (0,n)
(1,1) ---<>--- (1,n)

  >> Columns are added on the destination table in any cardinality combination.
  >> When the minimum cardinality is 0 on the "One" side (e.g. (0,1)) the foreign key columns
     must accept null values in order to represent the optionality of the relationship. Also
     the foreign key ON DELETE and ON UPDATE policy must be SET NULL.
  >> When the minimum cardinality is 1 on the "One" size (e.g. (1,1)) the foreign key columns
     mustn't accept null values (NOT NULL) to represent the entity mandatory participation. Also
     the foreign key ON DELETE and ON UPDATE policy must be RESTRICT.
  >> For identifier relationships the ON DELETE and ON UPDATE for the foreign key must be CASCADE
     because the weak entity only exists if the strong exists too.

------

[Many to Many relationship (n:n)]

(n) ---<>--- (n)

  >> One table is created to represent the relationship. In the relationship type there is no
     cardinality this means that the relationship is alway represented as (n) ---<>--- (n).
  >> Two foreign key is created on the new table each pointing to one of the envolved tables.
  >> The ON DELETE policy for the foreign keys must be RESTRICT.
  >> The ON UPDATE policy for the foreign keys must be UPDATE.
  >> The created table primary key is the union of the two foreign key columns.

------

[Relationships of degree > 2]

Must be manually simulated using the available binary relationships.

------

[NOTE]

* Case one relationship attribute is the identifier (primary key) for the "Many to Many" table
  relationship the fisrt will be merge with the automaticaly created primary key.

* "Many to Many" cannot be created as identifier because the two entities (tables)
  are always strong (in pgModeler's implementation).

* Case the relationship is identifier, this means, there is a weak entity
  in one side of the relatioship must be used the following rule:

   1) The weak entity primary key must be composed with the columns used
      to reference the strong entity (foreign key columns)

   2) The ON DELETE and ON UPDATE policy for the foreign key must be CASCADE, because
      for instance, in the case of one tuple on the strong entity is deleted the
      weak entity tuples must be destroyed also because this latter exists only because
      the first. */

class Relationship: public BaseRelationship {
 private:
  /* Indicates that the relationship invalid because one or more critical attributes
     where modified needing to be revalidated */
  bool invalidated,

       /* Indicates that the column suffixes must be created automaticaly based
          on the envolved table names */
       auto_suffix;

  /* Stores the number of columns which were rejected at the time of relationship
     connection. This is used only for generalization relationships because, according
     to the rule a column is rejected in inheritance when both tables have the same column
     of the same name and type. This attribute is used as an support to check if the relationship
     is invalidated or not. */
  unsigned rejected_col_count;

   /* Stores the columns which are created to be referenced by foreign keys
      in order to represent the relationship */
  vector<Column *> ref_columns,

     /* Stores the primary key columns which from the referenced table
        This vector is used by the relationship validation method. */
     pk_columns;

  /* Stores the suffixes generated for the columns created by the relationship.
     The suffixes can be of type: [src_suffix | dst_suffix]*[0-9]*
     where the numeric part of the above expression is a code generated by
     the column copy method when a column in the source table (reference) already
     exists in destination (receiver) and to avoid duplication of columns in the destination,
     a number is generated to identify the new column.  See copyColumns() */
   vector<QString> col_suffixes;

   /* Stores the names of the columns referenced earlier, where
      the key of the map is the identifier of each column created by
      the relationship. This map is filled when the relationship is
      of type 1-1 and 1-n and when it is disconnected and its referenced
      columns are deallocated, because in this the column name history
      is lost and may cause the breakdown of references. Yet at the time
      of connection the old names stored on this map are assigned to columns
      related to each primary key column that generates the referenced columns
      and finally this map is cleared */
  map<unsigned, QString> prev_ref_col_names;

  //Relationship attributes (used as columns on the receiver table)
  vector<TableObject *> rel_attributes;

  //Relationship constraint (used as constraints on the receiver table)
  vector<TableObject *> rel_constraints;

  //Foreign key that represents the 1-n relationship
  Constraint *fk_rel1n,

             /* Stores reference to the primary key automatically created when the
                relationship is identifier and the weak entity does not have
                a primary key */
             *pk_relident,

             /* Stores the reference to the special primary key. This constraint is only available
                to generalization / copy relationships */
             *pk_special,

             //Stores the unique key that represents the 1-1 relationship (including the fk_rel1n)
             *uq_rel11;

  //Table created by the relationship n-n
  Tabela *table_relnn;

  //Tables suffixes used to create unique names for the created columns
  QString src_suffix, dst_suffix;

  //Indicates if the relationship is identifier (when there is a strong and a weak entity)
  bool identifier;

  //Indicates if the foreign key (for 1-1, 1-n relationships) is deferrable
  bool deferrable;

  //Deferral type used by the foreign key when this is deferrable
  DeferralType deferral_type;

  /* This vector allows the user to define which columns inherited / copied (via its indexes)
     will be used on the special primary key in the receiver table */
  vector<unsigned> column_ids_pk_rel;

  //Name assigned to the generated table on n-n relationships
  QString tab_name_relnn;

  //Indicates if the column exists on the referenced column list
  bool isColumnExists(Column *column);

  //Executes the column addition for 1-1 relationship
  void addColumnsRel11(void);

  //Executes the column addition for 1-n relationship
  void addColumnsRel1n(void);

  //Executes the column addition for n-n relationship
  void addColumnsRelNn(void);

  /* Copy columns from one table to another. This operation is done in
     relationships of type copy / generalization. It is necessary
     to check duplicate names and incompatible types of columns */
  void addColumnsRelGen(void);

  /* Creates the foreign key that represents the relationship and adds it
     to the receiver table. Must be specified the actions ON DELETE and UPDATE. */
  void addForeignKey(Tabela *ref_tab, Tabela *recv_tab, ActionType del_act, ActionType upd_act);

  /* Creates the unique key that represents the 1-1 relationship e adds it to
     the receiver table */
  void addUniqueKey(Tabela *ref_tab, Tabela *recv_tab);

  //Adds the relationship attributes (columns) into receiver table
  void addAttributes(Tabela *recv_tab);

  /* Adds relationship constraints on the receiver table. If the relationship is
     of type n-n, constraints will be added to the created table. If among the constraints
     there is a primary key, then it will be merged with the primary key of receiver table */
  void addConstraints(Tabela *dst_tab);

  /* Executes adicional configurations on receiver table primary key when the
     relationship is identifier */
  void configureIndentifierRel(Tabela *dst_tab);

  /* Copy the columns from the reference table to the receiver table. The parameter not_null indicates
     that the columns must not accept null values */
  void copyColumns(Tabela *ref_tab, Tabela *recv_tab, bool not_null);

  /* This method is always executed before disconnection of the relationship.
     Its function is to remove from the specified table all the attributes which
     references any relationship generated column avoiding reference break */
  void removeTableObjectsRefCols(Tabela *table);

  //Creates the special primary key using the names stored in the 'column_ids_pk_rel' vector
  void createSpecialPrimaryKey(void);

  //Removes the columns created by the relationship from the specified table primary key if exists.
  void removeColumnsFromTablePK(Tabela *table);

 protected:
  //Destroy all the relationship attributes and constraints
  void destroyObjects(void);

 public:
  //String used as the name suffix separator. Default '_'
  static const QString SUFFIX_SEPARATOR;

  Relationship(Relationship *rel);

  Relationship(unsigned rel_type,
                 Tabela *src_tab, Tabela *dst_tab,
                 bool src_mdtry=false, bool dst_mdtry=false,
                 bool auto_suffix=true,
                 const QString &src_suffix="", const QString &dst_suffix="",
                 bool identifier=false, bool deferrable=false,
                 DeferralType deferral_type=DeferralType::immediate);

   // Connects the relationship making the configuration according to its type
   void connectRelationship(void);

   /* Disconnects the relationship from the tables removing all the attributes / constraints
      deallocating all the created object. */
   void disconnectRelationship(bool rem_tab_objs=true);

   //Returns the relationship generated columns names
   vector<QString> getRelationshipColumns(void);

   //Defines the mandatory participation for the specified table
   void setMandatoryTable(unsigned table_id, bool value);

   //Defines the automatic suffix generation for column names
   void setAutomaticSuffix(bool value);

   //Returns the current state for automatic column name suffix generation
   bool isAutomaticSuffix(void);

   /* Warning: This method is effective only when the relationship is connected. Only
      in this situation you obtain the columns generated by connecting relationship.

      This method defines what columns inherited / copied will be used
      as special primary key on receiver table. When the relationship is connected
      the special primary key is created using the columns inherited / copied.

      This special primary key is always destroyed when the relationship is disconnected and
      recreated when connected. This is a way available to the user create primary keys
      using the relationship included columns because on the current modeling, its not possibl
      directly create primary keys in the receiver table using columns created by the relationship.

      ** This feature can only be used in generalization / copy relationships ** */
   void setSpecialPrimaryKeyCols(vector<unsigned> &cols);

   //Returns the column indexes used by the special primary key
   vector<unsigned> getSpecialPrimaryKeyCols(void);

   //Defines the name of the auto generated table on the n-n relationship
   void setTableNameRelNN(const QString &name);

   //Returns the name of the table auto generated by the n-n relationship
   QString getTableNameRelNN(void);

   //Defines the suffix for the specified table id
   void setTableSuffix(unsigned table_id, const QString &suffix);

   //Returns the suffix for the specified table id
   QString getTableSuffix(unsigned table_id);

   //Defines if the created foreign key  is deferrable (only for 1-1, 1-n relationships)
   void setDeferrable(bool value);

   //Returns if the created foreign key is deferrable (only for 1-1, 1-n relationships)
   bool isDeferrable(void);

   //Defines the deferral type for the created foreign key (only for 1-1, 1-n relationships)
   void setDeferralType(DeferralType defer_type);

   //Returns the deferral tyep for the created foreign key (only for 1-1, 1-n relationships)
   DeferralType getDeferralType(void);

   //Defines if the relationship is identifier
   void setIdentifier(bool value);

   //Returns if the relationship is identifier
   bool isIdentifier(void);

   /* Returns if the relationship is invalidated in relation to propagation of columns.
      This method makes a series of verifications for each type of relationship,
      and if in any condition this method returns 'true' indicates that the relationship
      is no longer valid and must be reconnected. The reconnection operation is
      made on de model class ​​only because it treats all cases of invalidity at once. */
  bool isInvalidated(void);

  //Adds an attribute or constaint to the relationship.
  void addObject(TableObject *tab_obj, int obj_idx=-1);

  //Returns one attribute or constraint using its name
  TableObject *getObject(const QString &name, ObjectType obj_type);

  //Returns one attribute or constraint using its index
  TableObject *getObject(unsigned obj_idx, ObjectType obj_type);

  //Removes on attribute or constraint using its index
  void removeObject(unsigned obj_id, ObjectType obj_type);

  //Removes the specified attribute or constraint from the relationship
  void removeObject(TableObject *object);

  //Removes all the attributes and constraints from the relationship
  void removeObjects(void);

  //Remove one attribute using its index
  void removeAttribute(unsigned attrib_idx);

  //Remove one constraint using its index
  void removeConstraint(unsigned constr_idx);

  //Gets an attribute using its index
  Column *getAttribute(unsigned attrib_idx);

  //Gets an attribute using its name
  Column *getAttribute(const QString &name);

  //Gets an constraint using its index
  Constraint *getConstraint(unsigned constr_idx);

  //Gets an constraint using its name
  Constraint *getConstraint(const QString &name);

  //Returns a referenced column using its name
  Column *getReferencedColumn(const QString &col_name);

  /* Returns the index of a relationship attribute or constraint. Returns -1
     when the object doesn't exists */
  int getObjectIndex(TableObject *object);

  //Returns the attribute count
  unsigned getAttributeCount(void);

  //Returns the constraint count
  unsigned getConstraintCount(void);

  //Returns the attribute or constraint count
  unsigned getObjectCount(ObjectType obj_type);

  /* Returns if the relationship has one or more identifier attributes.
     Identifier attributes are attributes which belongs to a primary key
     added by the user. This method only scans the list of constraints
     on the relationship searching for primary keys created by the user.
     If found returns true. */
  bool hasIndentifierAttribute(void);

  /* Returns table that receives a copy of the columns that represent the relationship
     according to its configuration.

     WARNING: Not necessarily this method returns the destination table this because not in all
     relationship configuration the receiver is the destination table */
  Tabela *getReceiverTable(void);

  /* Returns table which serves as a reference when coping the columns to the
     receiver table.

     WARNING: Not necessarily this method returns the source table
     because not in all relationship configuration the reference is the source table

     For n-n relationships this method returns NULL as this type of relationship
     has 2 reference tables, which may be obtained by the method BaseRelationship::getTable() */
  Tabela *getReferenceTable(void);

  //Returns SQL / XML definition for the relationship.
  QString getCodeDefinition(unsigned def_type);

  //Copies the attributes from one relationship to another
  void operator = (Relationship &rel);

  friend class ModeloBD;
};

#endif
