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
  /// @details Arguments passed to the function that trigger executes
  vector<QString> arguments;

  /** @details Column list used as the trigger firing condition. This attribute was
     introduced in PostgreSQL 9.1 and it is used only when the UPDATE event
     is assigned to trigger. */
  vector<Column *> upd_columns;

  /// @details Function that is excuted when the trigger is activated
  Function *function;

  /// @details Condition that guarantees the trigger's execution
  QString condition;

  /// @details Trigger firing mode (BEFORE, AFTER, INSTEAD OF)
  FiringType firing_type;

  /// @details Map that marks which events activates the trigger
  map<EventType, bool> events;

  /// @details Flag that indicates whether the function must be executed by row
  bool is_exec_per_row;

  /// @details Table referecend by the trigger (only for constraint trigger)
  BaseObject *referenced_table;

  /// @details Indicates whether the trigger is deferrable (only for constraint trigger)
  bool is_deferrable;

  /// @details Deferral type for the trigger (only for constraint trigger)
  DeferralType deferral_type;

  /// @details Formats the basic trigger attributes to be used by SchemaParser
  void setBasicAttributes(unsigned def_type);

  /// @details Format the function arguments to be used by the SchemaParser
  void setArgumentAttribute(unsigned tipo_def);


 public:
  Trigger(void);

  /** @details Adds a column as a firing condition (only when the event UPDATE is used).
     The columns added by this method must belongs to the trigger owner table. */
  void addColumn(Column *column);

  /// @details Adds an argument to the trigger
  void addArgument(const QString &arg);

  /// @details Defines in which events the trigger is executed
  void setEvent(EventType event, bool value);

  /// @details Defines the function to be executed by the trigger
  void setFunction(Function *func);

  /// @details Defines the firing condition for trigger
  void setCondition(const QString &cond);

  /// @details Defines the referenced table (only for constraint trigger)
  void setReferecendTable(BaseObject *ref_table);

  /// @details Defines the deferral type
  void setDeferralType(DeferralType tipo);

  /// @details Defines whether the trigger is deferrable or not
  void setDeferrable(bool valor);

  /// @details Changes the specified trigger agument replacing the current argument by the 'new_arg'
  void editArgument(unsigned arg_idx, const QString &new_arg);

  /// @details Defines the moment when the trigger must be executed
  void setFiringType(FiringType firing_type);

  /// @details Define wheter the trigger executes per row
  void setExecutePerRow(bool value);

  /// @details Returns true if the trigger executes on the passed event
  bool isExecuteOnEvent(EventType event);

  /// @details Gets one reference column by its index
  Column *getColumn(unsigned col_idx);

  /// @details Gets one argument by its index
  QString getArgument(unsigned arg_idx);

  /// @details Gets the trigger firing condition
  QString getCondition(void);

  /// @details Returns the function executed by the trigger
  Function *getFunction(void);

  /// @details Returns the trigger argument count
  unsigned getArgumentCount(void);

  /// @details Returns the reference column count
  unsigned getColumnCount(void);

  /// @details Returns when the trigger executes
  FiringType getFiringType(void);

  /// @details Remove an argument using its index
  void removeArgument(unsigned arg_idx);

  /// @details Remove all arguments
  void removeArguments(void);

  /// @details Remove all referenced columns
  void removeColumns(void);

  /// @details Returns the referenced table
  BaseObject *getReferencedTable(void);

  /// @details Returns the deferral type of the constraint trigger
  DeferralType getDeferralType(void);

  /// @details Returns if the constraint trigger is deferrable or not
  bool isDeferrable(void);

  /** @details Returns whether the trigger references columns added
     by relationship. This method is used as auxiliary
     to control which triggers reference columns added by the
     relationship in order to avoid referece breaking due constants
     connections and disconnections of relationships */
  bool isReferRelationshipAddedColumn(void);

  /// @details Returns the SQL / XML definition for the trigger
  QString getCodeDefinition(unsigned def_type);
};

#endif
