/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Implements the operations to manipulate stored
#              procedures (function) on the database.
# Creation date: 28/09/2006
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
#ifndef FUNCAO_H
#define FUNCAO_H

#include "baseobject.h"
#include "schema.h"
#include "papel.h"
#include "column.h"

class Parameter: public Column {
 private:
 /* This attributes indicates whether the parameter
    is in, out or both (IN, OUT, INOUT) */
  bool is_in, is_out;

 public:
  Parameter(void);

  void setType(TipoPgSQL type);

  void setIn(bool value);
  void setOut(bool value);

  bool isIn(void);
  bool isOut(void);

  //Returns the SQL / XML code definition for the parameter
  QString getCodeDefinition(unsigned def_type);

  void operator = (const Parameter &param);
};



class Function: public BaseObject {
 private:
  static unsigned function_id;

  //Assinatura da função
  QString signature;

  //Biblioteca C que armazena a função.
  QString library;

  //Simbolo que identifica a função na Biblioteca C
  QString symbol;

  //Definição (comandos que esta executa)
  QString source_code;

  //Linguagem em que foi escrita a função
  BaseObject *language;

  //Parâmetros da função
  vector<Parameter> parameters;

  //Flag que indica se a função retorna um conjunto da dados
  bool returns_setof;

  //Indica se a função janela (WINDOW)
  bool is_wnd_function;

  //Tipo de comportamento da função
  TipoComportamento behavior_type;

  //Tipo da função (volátil, imutável, estável)
  TipoFuncao function_type;

  //Tipo de retorno da função
  TipoPgSQL return_type;

  /* Vetor usado para armazenas as colunas retornadas
     pela clausula RETURNS TABLE. Esta cláusula, ao invés de retornar
     um elemento em específico, retorna uma tabela inteira.
     Esta estrutura é usada apenas para geração de SQL da versão >=8.4 */
  vector<Parameter> table_return_types;

  //Tipo de segurança da função (SECURITY [INVOKER | DEFINER])
  TipoSeguranca security_type;

  //Custo de execução da função
  unsigned execution_cost;

  //Quantidade estimada de linhas retornadas pela função
  unsigned row_amount;

  /* Formata a string de parâmetros usada pelo parser de esquema
     na geração da definição SQL da função */
  void setParametersAttribute(unsigned def_type);

  /* Formata a string de tipos de retorno usada pelo parser de esquema
     na geração da definição da função */
  void setTableReturnTypeAttribute(unsigned def_type);

 public:
  Function(void);

  //Define o nome da função
  void setName(const QString &name);

  //Define o esquema ao qual a função faz parte
  void setSchema(BaseObject *schema);

  //Adiciona um parâmetr  função (com nome e tipo especificados)
  void addParameter(Parameter param);

  //Adiciona um tipo de retorno específico para a cláusula RETURNS TABLE
  void addTableReturnType(const QString &name, TipoPgSQL type);

  //Define o corpo da função (comandos que serão executados por esta)
  void setSourceCode(const QString &src_code);

  //Define a biblioteca que define a função em C
  void setLibrary(const QString &library);

  //Define o simbolo que identifica a biblioteca na função
  void setSymbol(const QString &symbol);

  //Define a linguagem que a função usará como base
  void setLanguage(BaseObject *language);

  //Define o custo de execução da função
  void setExecutionCost(unsigned exec_cost);

  //Define o custo de execução da função
  void setRowAmount(unsigned row_amount);

  //Define se a função retora um conjunto de valores
  void setReturnSetOf(bool value);

  //Define se a função é do tipo janela
  void setWindowFunction(bool value);

  //Define a configuração de retorno de valores da função
  void setBehaviorType(TipoComportamento behav_type);

  //Define o tipo da função (volátil, imutável, estável)
  void setFunctionType(TipoFuncao func_type);

  //Define o tipo de retorno da função
  void setReturnType(TipoPgSQL type);

  //Define o tipo de segurança ao chamar a função no SGBD
  void setSecurityType(TipoSeguranca sec_type);

  //Retorna o corpo da função
  QString getSourceCode(void);

  //Retorna a biblioteca que define a função
  QString getLibrary(void);

  //Retorna o simbolo que define a função na biblioteca
  QString getSymbol(void);

  //Retorna o nome da linguagem usada pela função
  BaseObject *getLanguage(void);

  //Retorna o número de parâmetros da função
  unsigned getParameterCount(void);

  //Retorna o número de colunas na tabela de retorno
  unsigned getTableReturnTypeCount(void);

  //Obtém um parâmetro através de seu índice
  Parameter getParameter(unsigned param_idx);

  //Obtém um tipo retorno de tabela através de seu índice
  Parameter getTableReturnType(unsigned type_idx);

  //Indica se a função retorna ou não um conjunto de dados
  bool isReturnSetOf(void);

  //Indica se a função retorna ou não uma tabela
  bool isReturnTable(void);

  //Indica se a função é do tipo janela
  bool isWindowFunction(void);

  //Retorna o tipo de comportamento da função
  TipoComportamento getBehaviorType(void);

  //Retorna o tipo da função
  TipoFuncao getFunctionType(void);

  //Obtém o tipo de retorno da função
  TipoPgSQL getReturnType(void);

  //Retorna o tipo de segurança da função
  TipoSeguranca getSecurityType(void);

  //Retorna o custo de execução da função
  unsigned getExecutionCost(void);

  //Retorna a quantidade de linhas retornadas
  unsigned getRowAmount(void);

  //Remove um parâmetro através de seu nome e tipo
  void removeParameter(const QString &name, TipoPgSQL type);

  //Remove um parâmetro através de seu índice
  void removeParameter(unsigned param_idx);

  //Remove todos os parâmetros da função
  void removeParameters(void);

  //Remove um tipo de retorno de tabela através de seu índice
  void removeTableReturnType(unsigned type_idx);

  //Remove todos os parâmetros da tabela de retorno da função
  void removeTableReturnTypes(void);

  //Retorna a assinatura da função nome completo + tipo de parâmetros
  QString getSignature(void);

  /* Formata a string de assinaturada da função, o parâmetro padrão
     'formatar' indica se o nome da função deve ser formatado.
     Por padrão, a formatação é feita */
  void createSignature(bool format=true);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);
};

#endif
