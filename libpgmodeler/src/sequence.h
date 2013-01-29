/*
# PostgreSQL Database Modeler (pgModeler)
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

/**
\ingroup libpgmodeler
\class Sequence
\brief Implements the operations to manipulate sequences on the database.
\note <strong>Creation date:</strong> 26/04/2008
*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "baseobject.h"
#include "table.h"

class Sequence: public BaseObject {
 private:
  /** @details Indicates taht the sequence is cyclic
     (the counter resets when maximum value is reached) */
  bool cycle;

          /// @details Minimum value
  QString min_value,
          /// @details Maximum value
          max_value,
          /// @details Current sequence value
          start,
          /// @details Sequence value increment
          increment,
          /// @details Sequence cache value
          cache;

  /// @details Column that owns the sequence
  Column *owner_col;

  /// @details Returns true when the passed value is a valid number
  bool isValidValue(const QString &value);

  /// @details Returns true when the passed value is null (zero)
  bool isNullValue(const QString &value);

  /// @details Returns the formated value excluding the aditional operators
  QString formatValue(const QString &value);

  /** @details Compares two values and returns:
     -1 when: value1 < value2
      0 when: value1 = value2
      1 when: value1 > value2 */
  int compareValues(QString value1, QString value2);

 public:
  /// @details Constants that indicates the maximum and minimum values accepted by sequence
  static const QString MAX_POSITIVE_VALUE;
  static const QString MAX_NEGATIVE_VALUE;

  Sequence(void);

  /// @details Defines if the sequence is a cycle
  void setCycle(bool value);

  /// @details Sets at once all the necessary fields to define a sequence
  void setValues(QString minv, QString maxv, QString inc, QString start, QString cache);

  /// @details Defines the owner column using a table and a column name
  void setOwnerColumn(Table *tabela, const QString &col_name);

  /// @details Defines the owner column using a column itself
  void setOwnerColumn(Column *column);

  /// @details Sets the sequence name
  void setName(const QString &name);

  /** @details Sets the schema that the sequence belongs. This method raises an error
     when there is a owner column and the schema to be set is different from
     the column parent table schema */
  void setSchema(BaseObject *schema);

  bool isCycle(void);
  QString getMaxValue(void);
  QString getMinValue(void);
  QString getIncrement(void);
  QString getStart(void);
  QString getCache(void);
  Column *getOwnerColumn(void);

  /** @details Returns whether the sequence references columns added
     by relationship. This method is used as auxiliary
     to control which sequence reference columns added by the
     relationship in order to avoid referece breaking due constants
     connections and disconnections of relationships */
  bool isReferRelationshipAddedColumn(void);

  /// @details Makes a copy between two sequences
  void operator = (Sequence &seq);

  /// @details Returns the SQL / XML definition for the sequence
  QString getCodeDefinition(unsigned def_type);
};

#endif
