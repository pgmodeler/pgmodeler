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

  //Id de usuário
  unsigned sysid;

  //Opções do papel
  bool options[6];

  //Limite de conexões do papel
  int conn_limit;

         //Validade do papel
  QString validity,

         //Senha de autenticação do papel
         password;

                 //Papéis nos quais terão como membro o papel atual
  vector<Role *> ref_roles, //IN ROLE

                 //Papéis os quais serão inseridos como membros do atual
                 member_roles, //ROLE

                 /* Papéis os quais serão inseridos como membros do atual
                   com o privilégio de adminstrador */
                 admin_roles; //ADMIN

  /* Formata as QStrings de papéis usadas pelo parser de esquema
     na geração da definição SQL do papel */
  void setRoleAttribute(unsigned tipo_papel);

 public:
  //Constantes usadas para referenciar as opções do Papel
  static const unsigned OP_SUPERUSER=0, //Mesmo que CREATEUSER
                        OP_CREATEDB=1,
                        OP_CREATEROLE=2,
                        OP_INHERIT=3,
                        OP_LOGIN=4,
                        OP_ENCRYPTED=5;

  //Constantes usadas para referenciar a lista de papéis internas da classe
  static const unsigned REF_ROLE=10,
                        MEMBER_ROLE=20,
                        ADMIN_ROLE=30;

  Role(void);

  //Define o id de usuário
  void setSysid(unsigned uid);

  //Define uma opção do papel
  void setOption(unsigned tipo_op, bool valor);

  //Define um papel como pertencente a uma das listas de papéis internos
  void addRole(unsigned tipo_papel, Role *papel);

  //Define o limite de conexões que um papel pode fazer
  void setConnectionLimit(int limite);

  //Define a validade do papel
  void setValidity(const QString &data);

  //Define a senha do papel
  void setPassword(const QString &password);

  //Obtém uma opção do papel usando as constantes OP_???
  bool getOption(unsigned tipo_op);

  //Remove um papel do tipo especificado no índice informado
  void removeRole(unsigned tipo_papel, unsigned idx_papel);

  //Remove todos os papéis membros no tipo especificado
  void removeRoles(unsigned tipo_papel);

  /* Obtém um papel das listas internas usando o tipo da lista (PAPEL_???)
     e o índice do elemento na lista */
  Role *getRole(unsigned tipo_papel, unsigned idx_papel);

  //Retorna se o papel informado está dento de uma das listas de papeis
  bool isRoleExists(unsigned tipo_papel, Role *papel);

  //Obtém o número de papéis presentes em uma determinada lista
  unsigned getRoleCount(unsigned tipo_papel);

  //Obtém o limite de conexões para o papel
  unsigned getConnectionLimit(void);

  //Obtém a validade do papel
  QString getValidity(void);

  //Obtém a senha do papel
  QString getPassword(void);

  //Obtém o id de usuário do papel
  unsigned getSysid(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
