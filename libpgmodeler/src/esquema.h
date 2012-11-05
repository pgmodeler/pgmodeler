/* 
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)m
# Description: Definição da classe Esquema que é usado para
#             representar graficamente os esquemas em banco de dados
#             e gerar os códigos SQL pertinentes a estes.
# Creation date: 07/04/2008
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
#ifndef ESQUEMA_H
#define ESQUEMA_H

#include "baseobject.h"

class Esquema: public BaseObject {
 private:
  static unsigned schema_id;

 public:
  Esquema(void);

  //Define o nome do esquema
  void setName(const QString &obj_name);
};

#endif
