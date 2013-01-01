/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Language
# Description: Implements the operations to manipulate procedural languages on the database.
# Creation date: 19/05/2008
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
#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "baseobject.h"
#include "function.h"
#include "role.h"

class Language: public BaseObject{
 private:
 /* Indicates whether the language is trusted on the database which means
    that the object can be used by an unprivileged user without compromise
    the entire database security */
  bool is_trusted;

  //Functions that defines the language behavior
  Function *functions[3];

 public:
  /* Constants used to reference the language's functions:
      > VALIDATOR: Function that validates the code written in the language's syntax
      > HANDLER: Function that executes the functions written in the language's syntax
      > INLINE: Function that executes inline instructions (DO's) (only on PostgreSQL 9.x) */
  const static unsigned VALIDATOR_FUNC=0,
                        HANDLER_FUNC=1,
                        INLINE_FUNC=2;

  Language(void);

  //Sets the language name
  void setName(const QString &name);

  //Sets whether the language is trusted or not
  void setTrusted(bool value);

  //Sets one of the language auxiliary functions
  void setFunction(Function *func, unsigned func_type);

  //Returs the trusted state of the language
  bool isTrusted(void);

  //Returns one of the auxiliary functions
  Function *getFunction(unsigned func_type);

  //Returns the SQL / XML code definition for the language
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);
};

#endif
