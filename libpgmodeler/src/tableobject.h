/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: TableObject
# Descritpoin: This class serves as a base class for objects that are embedded on tables
#              like columns, rules, triggers, indexes, constraints.
# Creation date: 24/07/2008
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
#ifndef TABLE_OBJECT_H
#define TABLE_OBJECT_H

#include "basetable.h"

class TableObject: public BaseObject {
 protected:
   //Stores the table that owns this object
   BaseTable *parent_table;

  /* Indicates that the object was included automaticaly by "one to many",
     "one to many", "many to many" relationships */
  bool add_by_linking;

  /* Indicates that the object was included automaticaly by "one to many",
     "generalization" relationships */
  bool add_by_generalization;

  /* Indicates that the object was included automaticaly by "one to many",
     "copy" relationships */
  bool add_by_copy;

  //Defines that the object is included by relationship (1-1, 1-n, n-n)
  void setAddedByLinking(bool value);

  //Defines that the object is included by generalization relationship
  void setAddedByGeneralization(bool value);

  //Defines that the object is include by copy relationship
  void setAddedByCopy(bool value);

 public:
  TableObject(void);

  //Defines the parent table for the object
  void setParentTable(BaseTable *table);

  //Returns the object parent table
  BaseTable *getParentTable(void);

  /* This method is purely virtual to force the derived classes
  overload this method. This also makes class TableObject
  not instantiable */
  virtual QString getCodeDefinition(unsigned def_type)=0;

  //Returns whether the object was added by relationship 1-1, 1-n, n-n
  bool isAddedByLinking(void);

  //Returns whether the object was added by generalization
  bool isAddedByGeneralization(void);

  //Returns whether the object was added by copy relationship
  bool isAddedByCopy(void);

  /* Returns whether the object was added by relationship (covering all the
     possible relationship types) */
  bool isAddedByRelationship(void);

  void operator = (TableObject &object);

  friend class Relationship;
};

#endif
