/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Operador que é usada para
#             gerar os códigos SQL pertinentes a operadores
# Creation date: 16/04/2008
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
#ifndef OPERATOR_H
#define OPERATOR_H

#include "baseobject.h"
#include "function.h"
#include "schema.h"
#include "papel.h"

class Operator: public BaseObject {
 private:
  //Armazena as referências para as funções do operador
  Function *functions[3];

  //Armazena os argumentos (esquerda e direita) do operador
  TipoPgSQL argument_types[2];

  //Armazena os operadores de comutação e negação do operador
  Operator *operators[6];

  bool hashes, //Indica que o operador pode executar um hash join
       merges;  //Indica que o operador pode executar um merge join

 public:
  static const unsigned FUNC_OPERATOR=0, //Índice da função base do operador
                        FUNC_JOIN=1,   //Índice da função de junção do operador
                        FUNC_RESTRICTION=2, //Índice da função de restrição do operador

                        ARG_LEFT=0,
                        ARG_RIGHT=1,

                        OPER_COMMUTATION=0,
                        OPER_NEGATION=1,
                        OPER_SORT1=2,
                        OPER_SORT2=3,
                        OPER_LESS=4,
                        OPER_GREATER=5;

  Operator(void);

  //Define o nome do operador
  void setName(const QString &name);

  //Define as funções usadas pelo operador (operador, junção, restrição)
  void setFunction(Function *func, unsigned func_type);

  //Define o tipo de dado dos argumentos (esquerda e direita) do operador
  void setArgumentType(TipoPgSQL arg_type, unsigned arg_id);

  //Define os operadores de negação e comutação
  void setOperator(Operator *oper, unsigned op_type);

  //Define se o operador aceita a operação HASHES
  void setHashes(bool value);

  //Define se o operador aceita a operação MERGES
  void setMerges(bool value);

  //Retorna uma determinada função do operador
  Function *getFunction(unsigned func_type);

  //Retorna o tipo de dado de um determinado argumento
  TipoPgSQL getArgumentType(unsigned arg_id);

  //Retorna um determinado operador
  Operator *getOperator(unsigned op_type);

  //Retorna se o operador aceita a operação HASHES
  bool isHashes(void);

  //Retorna se o operador aceita a operação MERGES
  bool isMerges(void);

  /* Verifica se o nome do operador é valido de acordo
     com as regras definidas na documentação */
  static bool isValidName(const QString &name);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);

  //Obtém a assinatura do operador
  QString getSignature(bool format_name=true);
};

#endif
