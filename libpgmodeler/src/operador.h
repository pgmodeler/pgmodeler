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
#ifndef OPERADOR_H
#define OPERADOR_H

#include "baseobject.h"
#include "function.h"
#include "schema.h"
#include "papel.h"

class Operador: public BaseObject {
 private:
  //Armazena as referências para as funções do operador
  Function *funcoes[3];

  //Armazena os argumentos (esquerda e direita) do operador
  TipoPgSQL tipo_args[2];

  //Armazena os operadores de comutação e negação do operador
  Operador *operadores[6];

  bool hashes, //Indica que o operador pode executar um hash join
       merges;  //Indica que o operador pode executar um merge join

 public:
  static const unsigned FUNC_OPERADOR=0, //Índice da função base do operador
                        FUNC_JUNCAO=1,   //Índice da função de junção do operador
                        FUNC_RESTRICAO=2, //Índice da função de restrição do operador

                        ARG_ESQUERDA=0,
                        ARG_DIREITA=1,

                        OPER_COMUTACAO=0,
                        OPER_NEGACAO=1,
                        OPER_ORDENACAO1=2,
                        OPER_ORDENACAO2=3,
                        OPER_MENOR=4,
                        OPER_MAIOR=5;

  Operador(void);

  //Define o nome do operador
  void setName(const QString &obj_name);

  //Define as funções usadas pelo operador (operador, junção, restrição)
  void definirFuncao(Function *funcao, unsigned tipo_funcao);

  //Define o tipo de dado dos argumentos (esquerda e direita) do operador
  void definirTipoDadoArgumento(TipoPgSQL tipo_dado, unsigned tipo_arg);

  //Define os operadores de negação e comutação
  void definirOperador(Operador *op, unsigned tipo_op);

  //Define se o operador aceita a operação HASHES
  void definirHashes(bool valor);

  //Define se o operador aceita a operação MERGES
  void definirMerges(bool valor);

  //Retorna uma determinada função do operador
  Function *obterFuncao(unsigned tipo_funcao);

  //Retorna o tipo de dado de um determinado argumento
  TipoPgSQL obterTipoDadoArgumento(unsigned tipo_arg);

  //Retorna um determinado operador
  Operador *obterOperador(unsigned tipo_op);

  //Retorna se o operador aceita a operação HASHES
  bool aceitaHashes(void);

  //Retorna se o operador aceita a operação MERGES
  bool aceitaMerges(void);

  /* Verifica se o nome do operador é valido de acordo
     com as regras definidas na documentação */
  static bool isValidName(const QString &obj_name);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def, bool forma_reduzida);
  QString getCodeDefinition(unsigned tipo_def);

  //Obtém a assinatura do operador
  QString obterAssinatura(bool formatar_nome=true);
};

#endif
