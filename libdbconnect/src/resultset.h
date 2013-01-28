/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: libdbconnect library
# Class: ResultSet
# Description: This class encapsulates some functions implemented by libpq
#              for manipulation of resultsets returned by PostgreSQL SQL commands
# Creation date: 14/07/2009
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
#ifndef RESULTSET_H
#define RESULTSET_H

#include "exception.h"
#include <libpq-fe.h>
#include <cstdlib>

class ResultSet {
 private:
  /** Indicates whether the result was copied, this flag is used
     to avoid segmentation faults when calling the destructor.
     As the pointer 'sql_result' is copied
     to other elements if it is destroyed can cause
     reference fails. Thus, such a pointer is only deleted
     when this flag is marked as false */
  bool is_res_copied;

  void destroyResultSet(void);

 protected:
  /// Stores the current tuple index, just for navigation
  int current_tuple;

  /** Indicates that the result was generated from a command
     which generates no tuples. Example: INSERT, DELETE, CREATE .. */
  bool empty_result;

  /// Stores the result object of a SQL command
  PGresult *sql_result;

  /** This class may be constructed from a
     result of SQL command generated in DBConnection class */
  ResultSet(PGresult *sql_result);

 public:
   /// Constants used to navigate through the resultset
   static const unsigned FIRST_TUPLE=0,
                         LAST_TUPLE=1,
                         PREVIOUS_TUPLE=2,
                         NEXT_TUPLE=3;

   ResultSet(void);
  ~ResultSet(void);

  /// Returns the value of a column (searching by name or index)
  char *getColumnValue(const QString &column_name);
  char *getColumnValue(int column_idx);

  /// Returns the data allocated size of a column (searching by name or index)
  int getColumnSize(const QString &column_name);
  int getColumnSize(int column_idx);

  /** Returns the number of rows affected by the command that generated
     the result if it is an INSERT, DELETE, UPDATE or the number of
     tuples returned if the command was a SELECT */
  int getTupleCount(void);

  /// Returns the column count present in one tuple
  int getColumnCount(void);

  /// Returns the name of the column specified by it's index
  QString getColumnName(int column_idx);

  /// Returns the index of the column specified by it's name
  int getColumnIndex(const QString &column_name);

  /// Returns the current tuple where the navigation is
  int getCurrentTuple(void);

  /// Informs if the column is in binary format
  bool isColumnBinaryFormat(const QString &column_name);
  bool isColumnBinaryFormat(int column_idx);

  /// Access on tuple on result set via navigation constants
  bool accessTuple(unsigned tuple_type);

  /// Make a copy between two resultsets
  void operator = (ResultSet &res);

  friend class DBConnection;
};

#endif
