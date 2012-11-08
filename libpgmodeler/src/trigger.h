/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Trigger
# Description: Implements the operations to manipulate triggers on the database.
# Creation date: 11/10/2006
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
#ifndef TRIGGER_H
#define TRIGGER_H

#include "tableobject.h"
#include "function.h"

class Gatilho: public TableObject{
 protected:
  //Argumentos que são passados a função que o gatilho executa
  vector<QString> arguments;

  /* Lista de colunas usadas como condição de disparo da trigger
     Este atributo foi adicionado no PostgreSQL 9.1 é usado somente
     quando o evento UPDATE está presente. */
  vector<Column *> upd_columns;

  //Função que será executada com o disparo do gatilho
  Function *function;

  //Condição que garante ou não a execução do gatilhos
  QString condition;

  //Momento de disparo da a trigger (BEFORE, AFTER, INSTEAD OF)
  TipoDisparo firing_type;

  //Mapa de flags que indica em que evento o gatilho de ver disparado
  map<unsigned, bool> events;

  //Flag que indica que a função do gatilho deve ser executada por linha da tabela
  bool is_exec_per_row;

  //Tabela referenciada pelo gatilho (apenas para gatilho restrição)
  BaseObject *referenced_table;

  //Indica se o gatílho é postergavel ou não (apenas para gatilho restrição)
  bool is_deferrable;

  //Tipo de postergação do gatilho (apenas para gatilho restrição)
  TipoPostergacao deferral_type;

  void setBasicAttributes(unsigned def_type);

  /* Formata a QString de argumentos usada pelo parser de esquema
     na geração da definição SQL do gatilho */
  void setArgumentAttribute(unsigned tipo_def);


 public:
  static const unsigned BEFORE_MODE=0;
  static const unsigned AFTER_MODE=1;
  static const unsigned INSTEADOF_MODE=2;

  Gatilho(void);

  /* Adiciona uma coluna como condição de disparo (apenas para evento update)
     As colunas adicionadas por esse método devem obrigatoriamente pertencer
       coluna dona do gatilho e nã  outras tabelas */
  void addColumn(Column *column);

  //Adiciona um argumento ao gatilho
  void addArgument(const QString &arg);

  //Define em quais eventos (INSERT, DELETE, UPDATE, TRUNCATE) o gatilho pode ser executado
  void setEvent(TipoEvento event, bool value);

  //Define a função que será executada quando o gatilho for chamado
  void setFunction(Function *func);

  //Define a condição de execução do gatilho
  void setCondition(const QString &cond);

  //Define a tabela referenciada
  void setReferecendTable(BaseObject *ref_table);

  //Define o tipo de deferimento
  void setDeferralType(TipoPostergacao tipo);

  //Define se o gatilho é postgergavel ou não
  void setDeferrable(bool valor);

  /* Edita um argumento através de seu índice, passando também
     o novo valor que este receberá */
  void editArgument(unsigned arg_idx, const QString &new_arg);

  //Define o momento de execução do gatilho
  void setFiringType(TipoDisparo firing_type);

  //Define se o gatlho deve ser executado por linha da tabela
  void setExecutePerRow(bool value);

  //Retorna se o gatlho é executado no evento informado
  bool isExecuteOnEvent(TipoEvento event);

  //Obtém uma coluna referenciada pelo gatilho através do índice
  Column *getColumn(unsigned col_idx);

  //Obtém um argumento através de seu índice
  QString getArgument(unsigned arg_idx);

  //Obtém a condição definida para execução do gatilho
  QString getCondition(void);

  //Obtém a função executada pelo gatilho chamado
  Function *getFunction(void);

  //Obtém o número de argumentos
  unsigned getArgumentCount(void);

  //Obtém o número de colunas
  unsigned getColumnCount(void);

  //Retorna se o gatilho executa antes de evento
  TipoDisparo getFiringType(void);

  //Remove um argumento pelo seu índice
  void removeArgument(unsigned arg_idx);
  void removeArguments(void);
  void removeColumns(void);

  //Obtém a tabela referenciada
  BaseObject *getReferencedTable(void);

  //Obtém o tipo de deferimento da restrição
  TipoPostergacao getDeferralType(void);

  //Obtém se a restrição é postgergavel ou não
  bool isDeferrable(void);

  /* Retorna se o gatilho referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar gatilhos os quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool isReferRelationshipColumn(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type);
};

#endif
