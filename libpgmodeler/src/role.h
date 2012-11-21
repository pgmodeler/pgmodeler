/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Implements the operations to manipulate roles on the database.
# Creation date: 12/05/2008
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
#ifndef ROLE_H
#define ROLE_H

#include "baseobject.h"

class Role: public BaseObject {
 private:
  static unsigned role_id;

  //Role id
  unsigned sysid;

  /* Options for the role (SUPERUSER, CREATEDB, CREATEROLE,
     INHERIT, LOGIN, ENCRYPTED) */
  bool options[6];

  //Connection limit for the role
  int conn_limit;

  //Validity date for the role
  QString validity,

         //Authentication password
         password;

                 //Roles that has the 'this' role as member
  vector<Role *> ref_roles, //IN ROLE

                 //Member roles of 'this' role
                 member_roles, //ROLE

                 //Member roles of 'this' role whit admin privileges
                 admin_roles; //ADMIN

  //Formats the role attribute to be used by the SchemaParser
  void setRoleAttribute(unsigned role_type);

 public:
  //Constants used to reference the available options for the role
  static const unsigned OP_SUPERUSER=0,
                        OP_CREATEDB=1,
                        OP_CREATEROLE=2,
                        OP_INHERIT=3,
                        OP_LOGIN=4,
                        OP_ENCRYPTED=5;

  //Constants used to reference the internal role lists of the class
  static const unsigned REF_ROLE=10,
                        MEMBER_ROLE=20,
                        ADMIN_ROLE=30;

  Role(void);

  //Sets the role id
  void setSysid(unsigned uid);

  //Sets one option for the role (Use constants OP_???)
  void setOption(unsigned op_type, bool value);

  //Define um papel como pertencente a uma das listas de papéis internos
  void addRole(unsigned role_type, Role *role);

  //Define o limite de conexões que um papel pode fazer
  void setConnectionLimit(int limit);

  //Define a validade do papel
  void setValidity(const QString &date);

  //Define a senha do papel
  void setPassword(const QString &passwd);

  //Obtém uma opção do papel usando as constantes OP_???
  bool getOption(unsigned op_type);

  //Remove um papel do tipo especificado no índice informado
  void removeRole(unsigned role_type, unsigned role_idx);

  //Remove todos os papéis membros no tipo especificado
  void removeRoles(unsigned role_type);

  /* Obtém um papel das listas internas usando o tipo da lista (PAPEL_???)
     e o índice do elemento na lista */
  Role *getRole(unsigned role_type, unsigned role_idx);

  //Retorna se o papel informado está dento de uma das listas de papeis
  bool isRoleExists(unsigned role_type, Role *role);

  //Obtém o número de papéis presentes em uma determinada lista
  unsigned getRoleCount(unsigned role_type);

  //Obtém o limite de conexões para o papel
  unsigned getConnectionLimit(void);

  //Obtém a validade do papel
  QString getValidity(void);

  //Obtém a senha do papel
  QString getPassword(void);

  //Obtém o id de usuário do papel
  unsigned getSysid(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type);
};

#endif
