/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Permission
# Description: Implements the operations to manipulate object permissions.
# Creation date: 16/09/2010
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
#ifndef PERMISSION_H
#define PERMISSION_H

#include "baseobject.h"
#include "function.h"
#include "role.h"
#include <algorithm>
#include <QTextStream>

class Permission: public BaseObject {
 /* Permissions on PostgreSQL are only applied to the following
    object type:

    * table
    * column
    * view
    * sequence
    * database
    * foreign-data wrapper (not implemented)
    * foreign server (not implemented)
    * large objects (not implemented)
    * function
    * aggregate
    * linguage
    * schema
    * tablespace */
 private:
   //Object which the permission is applied
   BaseObject *object;

   /* Roles that has permissions over the object. This vector can be
      empty indicating that all roles on the cluster has permission over
      the object. */
   vector<Role *> roles;

   //Privileges set applied to the object (Accessed via constants PRIV_???)
   bool privileges[13];

   /* Indicates whether a privilege with given index can be assigned to other roles
      over the same object (WITH GRANT OPTION). This attribute is not applicable
      when there is no specified role (PUBLIC). This attribute is ignored
      when there is no defined role as holder of the privilege. */
   bool grant_option[13];

   /* Generates a unique identifier for permission using the attribute
      'name' of base class BaseObject. This is only used to avoid
       duplicate permissions in the model */
   void generatePermissionId(void);

 public:
   //Constants used to reference the privileges
   static const unsigned PRIV_SELECT=0,
                         PRIV_INSERT=1,
                         PRIV_UPDATE=2,
                         PRIV_DELETE=3,
                         PRIV_TRUNCATE=4,
                         PRIV_REFERENCES=5,
                         PRIV_TRIGGER=6,
                         PRIV_CREATE=7,
                         PRIV_CONNECT=8,
                         PRIV_TEMPORARY=9,
                         PRIV_EXECUTE=10,
                         PRIV_USAGE=11;

  /* In the constructor is required to specify which object will receive
     the permissions this can not be changed after the object instance of
     the class is created. */
  Permission(BaseObject *obj);

  //Adds a role that will have privileges over the object
  void addRole(Role *role);

  //Sets the state of one permission's privilege (Accessed via constants PRIV_???)
  void setPrivilege(unsigned priv_id, bool value, bool grant_op);

  //Remove a role using its index
  void removeRole(unsigned role_idx);

  //Remove all roles from the permission
  void removeRoles(void);

  //Gets the role count associated to the permission
  unsigned getRoleCount(void);

  //Gets one role from permission using its index
  Role *getRole(unsigned role_idx);

  //Gets the object that is subject to the privileges
  BaseObject *getObject(void);

  //Gets the actual state of the GRANT OPTION for the given privilege
  bool getGrantOption(unsigned priv_id);

  //Gets the current state for the given privilege
  bool getPrivilege(unsigned priv_id);

  /* Returns a string containing all the privileges
     configured as the internal format of permissions
     as documented on PostgreSQL GRANT command */
  QString getPrivilegeString(void);

  //Indicates whether the role is present on the permission
  bool isRoleExists(Role *role);

  //Returns the SQL / XML definition for the permission
  QString getCodeDefinition(unsigned def_type);
};

#endif
