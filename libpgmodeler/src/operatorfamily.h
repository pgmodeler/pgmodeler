/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: OperatorFamily
# Description: Implements the operations to manipulate operator family on the database.
# Creation date: 08/07/2008
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
#ifndef OPERATOR_FAMILY_H
#define OPERATOR_FAMILY_H

#include "baseobject.h"

class OperatorFamily: public BaseObject {
 private:
  //Indexing type used by the operator family
  IndexingType indexing_type;

 public:
  OperatorFamily(void);

  //Sets the indexing type of the operator family
  void setIndexingType(IndexingType idx_type);

  //Returns the indexing type of the operator family
  IndexingType getIndexingType(void);

  //Returns the SQL / XML code definition for the operator family
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);
};

#endif
