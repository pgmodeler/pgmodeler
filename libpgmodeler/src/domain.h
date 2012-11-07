/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Domain
# Description: Implements the operations to manipulate domains on the database.
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
#ifndef DOMAIN_H
#define DOMAIN_H

#include "baseobject.h"
#include "esquema.h"

class Domain: public BaseObject{
 private:
  //Constraint name used by the domain
  QString constraint_name,
         //Validation expression used by domain (check) constraint
         expression,
         //Domain default data type value
         default_value;

  //Indicates that the domains accepts null values or not
  bool not_null;

  //Domain data type
  TipoPgSQL type;

 public:
  Domain(void);
  ~Domain(void);

  //Sets the domain's constraint name
  void setConstraintName(const QString &constr_name);

  //Sets the check expression of the domain
  void setExpression(const QString &expr);

  //Sets the default value of the domain
  void setDefaultValue(const QString &default_val);

  //Defines whether the domain accepts null values or not
  void setNotNull(bool value);

  //Defines the domain data type
  void setType(TipoPgSQL type);

  /* Overloaded BaseObject name definition method.
     Updates the reference of the domain as a PostgreSQL data type */
  void setName(const QString &name);

  /* Overloaded BaseObject schema definition method.
     Updates the reference of the domain as a PostgreSQL data type */
  void setSchema(BaseObject *schema);

  //Methods to access domain's attributes
  QString getConstraintName(void);
  QString getExpression(void);
  QString getDefaultValue(void);
  bool isNotNull(void);
  TipoPgSQL getType(void);

  //Returns the SQL / XML code definition for the domain
  QString getCodeDefinition(unsigned tipo_def);

  void operator = (Domain &domain);
};

#endif
