/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseTable
# Description: Base class used to define table like objects (views and tables)
# Creation date: 09/04/2008
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
#ifndef BASE_TABLE_H
#define BASE_TABLE_H

#include "basegraphicobject.h"

class BaseTable: public BaseGraphicObject {
 public:
  BaseTable(void);
  virtual QString getCodeDefinition(unsigned tipo_def)=0;

  friend class RelacionamentoBase;
  friend class DatabaseModel;
};

#endif
