/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Trigger
# Description: Implements the operations to manipulate triggers on the database.
# Creation date: 11/10/2006
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
#ifndef TRIGGER_H
#define TRIGGER_H

#include "tableobject.h"
#include "function.h"

class Trigger: public TableObject{
 protected:
  /// Arguments passed to the function that trigger executes
  vector<QString> arguments;

  /** Column list used as the trigger firing condition. This attribute was
     introduced in PostgreSQL 9.1 and it is used only when the UPDATE event
     is assigned to trigger. */
  vector<Column *> upd_columns;

  /// Function that is excuted when the trigger is activated
  Function *function;

  /// Condition that guarantees the trigger's execution
  QString condition;

  /// Trigger firing mode (BEFORE, AFTER, INSTEAD OF)
  FiringType firing_type;

  /// Map that marks which events activates the trigger
  map<EventType, bool> events;

  /// Flag that indicates whether the function must be executed by row
  bool is_exec_per_row;

  /// Table referecend by the trigger (only for constraint trigger)
  BaseObject *referenced_table;

  /// Indicates whether the trigger is deferrable (only for constraint trigger)
  bool is_deferrable;

  /// Deferral type for the trigger (only for constraint trigger)
  DeferralType deferral_type;

  /// Formats the basic trigger attributes to be used by SchemaParser
  void setBasicAttributes(unsigned def_type);

  /// Format the function arguments to be used by the SchemaParser
  void setArgumentAttribute(unsigned tipo_def);


 public:
  Trigger(void);

  /** Adds a column as a firing condition (only when the event UPDATE is used).
     The columns added by this method must belongs to the trigger owner table. */
  void addColumn(Column *column);

  /// Adds an argument to the trigger
  void addArgument(const QString &arg);

  /// Defines in which events the trigger is executed
  void setEvent(EventType event, bool value);

  /// Defines the function to be executed by the trigger
  void setFunction(Function *func);

  /// Defines the firing condition for trigger
  void setCondition(const QString &cond);

  /// Defines the referenced table (only for constraint trigger)
  void setReferecendTable(BaseObject *ref_table);

  /// Defines the deferral type
  void setDeferralType(DeferralType tipo);

  /// Defines whether the trigger is deferrable or not
  void setDeferrable(bool valor);

  /// Changes the specified trigger agument replacing the current argument by the 'new_arg'
  void editArgument(unsigned arg_idx, const QString &new_arg);

  /// Defines the moment when the trigger must be executed
  void setFiringType(FiringType firing_type);

  /// Define wheter the trigger executes per row
  void setExecutePerRow(bool value);

  /// Returns true if the trigger executes on the passed event
  bool isExecuteOnEvent(EventType event);

  /// Gets one reference column by its index
  Column *getColumn(unsigned col_idx);

  /// Gets one argument by its index
  QString getArgument(unsigned arg_idx);

  /// Gets the trigger firing condition
  QString getCondition(void);

  /// Returns the function executed by the trigger
  Function *getFunction(void);

  /// Returns the trigger argument count
  unsigned getArgumentCount(void);

  /// Returns the reference column count
  unsigned getColumnCount(void);

  /// Returns when the trigger executes
  FiringType getFiringType(void);

  /// Remove an argument using its index
  void removeArgument(unsigned arg_idx);

  /// Remove all arguments
  void removeArguments(void);

  /// Remove all referenced columns
  void removeColumns(void);

  /// Returns the referenced table
  BaseObject *getReferencedTable(void);

  /// Returns the deferral type of the constraint trigger
  DeferralType getDeferralType(void);

  /// Returns if the constraint trigger is deferrable or not
  bool isDeferrable(void);

  /** Returns whether the trigger references columns added
     by relationship. This method is used as auxiliary
     to control which triggers reference columns added by the
     relationship in order to avoid referece breaking due constants
     connections and disconnections of relationships */
  bool isReferRelationshipAddedColumn(void);

  /// Returns the SQL / XML definition for the trigger
  QString getCodeDefinition(unsigned def_type);
};

#endif
