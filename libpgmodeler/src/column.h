/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Column
# Description: Implements basic operations to manipulate table columns.
# Creation date: 12/09/2006
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
#ifndef COLUMN_H
#define COLUMN_H

#include "tableobject.h"

class Column: public TableObject{
 protected:
  /** @details Stores the previous name of the column before its name has changed.
     This attribute assists in the process of reference columns added
     by relationships. */
  QString old_name;

  /// @details Indicate that the column accpets null values or not
  bool not_null;

  /// @details Data type of the column
  PgSQLType type;

  /** @details Default value of the column.
     Note: The user must format the default value in
           accordance with the requirements for each data type.
           E.g.: for a varchar(10) default value should be 'abcdef' (including apostrophe)
           for a date the defaul value should be '2006-09-12 ' and so on. */
  QString default_value;

 public:
  Column(void);

  /// @details Defines if the column accepts null values or not
  void setNotNull(bool value);

  /// @details Defines the column data type
  void setType(PgSQLType type);

  /** @details Sets the default value of the column. Must be informed together with the value
     the particularities of each type, such as quotation marks, hyphens, etc. */
  void setDefaultValue(const QString &value);

    /** @details Define o nome da coluna. Este método mantém o último nome da coluna
     armazenado para auxiliar os métodos de relacionamento de colunas
     adicionadas por relacionamentos com restrições/indices e sequencias */

  /** @details Defines the column name. This method keeps the last column name
     stored to assist the objects like constraints / indixes and sequences
     that is referencing the column by its old name. */
  void setName(const QString &name);

  /// @details Returns the not null state of the column
  bool isNotNull(void);

  /// @details Returns the data type of the column
  PgSQLType getType(void);

  /// @details Returns the default value of the column
  QString getDefaultValue(void);

  /// @details Returns the SQL/XML code definition for the column
  QString getCodeDefinition(unsigned def_type);

  /** @details Returns the old column name. The parameter 'format' indicates
     whether the name must be formatted or not */
  QString getOldName(bool format=false);

  /// @details Returns the reference to the column type in the form [schema].table.column_name%TYPE
  QString getTypeReference(void);

  /// @details Copies on column to other
  void operator = (Column &col);

  friend class Tabela;
};

#endif
