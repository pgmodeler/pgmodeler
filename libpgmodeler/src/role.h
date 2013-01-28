/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Role
# Description: Implements the operations to manipulate roles on the database.
# Creation date: 12/05/2008
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
#ifndef ROLE_H
#define ROLE_H

#include "baseobject.h"

class Role: public BaseObject {
 private:
  static unsigned role_id;

  /// Role id
  unsigned sysid;

  /** Options for the role (SUPERUSER, CREATEDB, CREATEROLE,
     INHERIT, LOGIN, ENCRYPTED) */
  bool options[6];

  /// Connection limit for the role
  int conn_limit;

  /// Validity date for the role
  QString validity,

         /// Authentication password
         password;

                 /// Roles that has the 'this' role as member
  vector<Role *> ref_roles, /// IN ROLE

                 /// Member roles of 'this' role
                 member_roles, /// ROLE

                 /// Member roles of 'this' role whit admin privileges
                 admin_roles; /// ADMIN

  /// Formats the role attribute to be used by the SchemaParser
  void setRoleAttribute(unsigned role_type);

 public:
  /// Constants used to reference the available options for the role
  static const unsigned OP_SUPERUSER=0,
                        OP_CREATEDB=1,
                        OP_CREATEROLE=2,
                        OP_INHERIT=3,
                        OP_LOGIN=4,
                        OP_ENCRYPTED=5;

  /// Constants used to reference the internal role lists of the class
  static const unsigned REF_ROLE=10,
                        MEMBER_ROLE=20,
                        ADMIN_ROLE=30;

  Role(void);

  /// Sets the role id
  void setSysid(unsigned uid);

  /// Sets one option for the role (Via OP_??? constants)
  void setOption(unsigned op_type, bool value);

  /// Adds one role to the internal role list (Via ???_ROLE constants)
  void addRole(unsigned role_type, Role *role);

  /// Defines the connection limit for the role
  void setConnectionLimit(int limit);

  /// Defines the validity date for the role
  void setValidity(const QString &date);

  /// Sets the password for the role
  void setPassword(const QString &passwd);

  /// Gets on option for the role (Via OP_??? constants)
  bool getOption(unsigned op_type);

  /// Remove one role from internal role list (Via ???_ROLE constants)
  void removeRole(unsigned role_type, unsigned role_idx);

  /// Remove all roles from one iternal list (Via ???_ROLE constants)
  void removeRoles(unsigned role_type);

  /** Gets one role from internal list (Via ???_ROLE constants) referencing
     the object by its index */
  Role *getRole(unsigned role_type, unsigned role_idx);

  /// Returns whether the role exists on the internal lists (Via ???_ROLE constants)
  bool isRoleExists(unsigned role_type, Role *role);

  /// Gets the role count on the specified internal list (Via ???_ROLE constants)
  unsigned getRoleCount(unsigned role_type);

  /// Returns the connection limit for the role
  unsigned getConnectionLimit(void);

  /// Returns the validity date for the role
  QString getValidity(void);

  /// Returns the role password
  QString getPassword(void);

  /// Returns the role id on the system
  unsigned getSysid(void);

  /// Returns the SQL / XML definition for the role
  QString getCodeDefinition(unsigned def_type);
};

#endif
