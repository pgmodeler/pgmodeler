/* 
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Schema
# Description: Implements the operations to manipulate schemas on the database.
# Creation date: 07/04/2008
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
#ifndef SCHEMA_H
#define SCHEMA_H

#include "baseobject.h"

class Schema: public BaseObject {
 private:
  static unsigned schema_id;

 public:
  Schema(void);
  void setName(const QString &name);
  QString getCodeDefinition(unsigned def_type);
};

#endif
