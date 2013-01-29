/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Rule
# Description: Implements the operations to manipulate table rules.
# Creation date: 26/09/2006
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
#ifndef RULE_H
#define RULE_H

#include "tableobject.h"
#include "column.h"

class Rule: public TableObject{
 private:
  /// @details Commands executed by the rule when activated
  vector<QString> commands;

  /// @details Conditional expression for the rule activation
  QString conditional_expr;

  /// @details Rule execution type (ALSO or INSTEAD)
  ExecutionType execution_type;

  /// @details Event when the rule is triggered (ON SELECT, NO UPDATE, ON INSERT, ON DELETE)
  EventType event_type;

  /// @details Formats the commands string to be used by the SchemaParser
  void setCommandsAttribute(void);

 public:
  Rule(void);

  /// @details Adds the SQL command to be executed by the rule
  void addCommand(const QString &cmd);

  /// @details Sets the conditional expression for the rule
  void setConditionalExpression(const QString &expr);

  /// @details Sets the rule execution type (ALSO, INSTEAD)
  void setExecutionType(ExecutionType type);

  /// @details Defines the event when the rule is triggered
  void setEventType(EventType type);

  /// @details Returns one command executed by the rule using its index
  QString getCommand(unsigned cmd_idx);

  /// @details Returns the SQL command count
  unsigned getCommandCount(void);

  /// @details Returns the conditional expression for the rule
  QString getConditionalExpression(void);

  /// @details Returns the event when the rule is triggered
  EventType getEventType(void);

  /// @details Returns the execution type for the rule
  ExecutionType getExecutionType(void);

  /// @details Removes one command form the rule using its index
  void removeCommand(unsigned cmd_idx);

  /// @details Remove all commands from the rule
  void removeCommands(void);

  /// @details Returns the SQL / XML definition for the rule
  QString getCodeDefinition(unsigned def_type);
};

#endif
