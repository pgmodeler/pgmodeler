/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description:Definição da classe Funcao que é usado para
#            configurar e gerar os códigos SQL referentes a funções (procedimentos).
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
  /* Atributos que indicam que o parâmetro é de
     entrada e saida (IN, OUT, INOUT) */
  bool is_in, is_out;

 public:
  Parameter(void);

  void setType(TipoPgSQL type);

  void setIn(bool valor);
  void setOut(bool valor);

  bool isIn(void);
  bool isOut(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
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
  void setParametersAttribute(unsigned tipo_def);

  /* Formata a string de tipos de retorno usada pelo parser de esquema
     na geração da definição da função */
  void setTableReturnTypeAttribute(unsigned tipo_def);

 public:
  Function(void);

  //Define o nome da função
  void setName(const QString &obj_name);

  //Define o esquema ao qual a função faz parte
  void setSchema(BaseObject *schema);

  //Adiciona um parâmetr  função (com nome e tipo especificados)
  void addParameter(Parameter param);

  //Adiciona um tipo de retorno específico para a cláusula RETURNS TABLE
  void addTableReturnType(const QString &obj_name, TipoPgSQL tipo);

  //Define o corpo da função (comandos que serão executados por esta)
  void setSourceCode(const QString &codigo);

  //Define a biblioteca que define a função em C
  void setLibrary(const QString &library);

  //Define o simbolo que identifica a biblioteca na função
  void setSymbol(const QString &symbol);

  //Define a linguagem que a função usará como base
  void setLanguage(BaseObject *language);

  //Define o custo de execução da função
  void setExecutionCost(unsigned custo);

  //Define o custo de execução da função
  void setRowAmount(unsigned row_amount);

  //Define se a função retora um conjunto de valores
  void setReturnSetOf(bool valor);

  //Define se a função é do tipo janela
  void setWindowFunction(bool valor);

  //Define a configuração de retorno de valores da função
  void setBehaviorType(TipoComportamento tipo);

  //Define o tipo da função (volátil, imutável, estável)
  void setFunctionType(TipoFuncao tipo);

  //Define o tipo de retorno da função
  void setReturnType(TipoPgSQL tipo);

  //Define o tipo de segurança ao chamar a função no SGBD
  void setSecurityType(TipoSeguranca tipo);

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
  Parameter getParameter(unsigned idx_param);

  //Obtém um tipo retorno de tabela através de seu índice
  Parameter getTableReturnType(unsigned idx_tipo);

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
  void removeParameter(const QString &obj_name, TipoPgSQL tipo);

  //Remove um parâmetro através de seu índice
  void removeParameter(unsigned idx_param);

  //Remove todos os parâmetros da função
  void removeParameters(void);

  //Remove um tipo de retorno de tabela através de seu índice
  void removeTableReturnType(unsigned idx_tipo);

  //Remove todos os parâmetros da tabela de retorno da função
  void removeTableReturnTypes(void);

  //Retorna a assinatura da função nome completo + tipo de parâmetros
  QString getSignature(void);

  /* Formata a string de assinaturada da função, o parâmetro padrão
     'formatar' indica se o nome da função deve ser formatado.
     Por padrão, a formatação é feita */
  void createSignature(bool formatar=true);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def, bool forma_reduzida);
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
