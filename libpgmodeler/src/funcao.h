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
#include "esquema.h"
#include "papel.h"
#include "column.h"

class Parametro: public Column {
 private:
  /* Atributos que indicam que o parâmetro é de
     entrada e saida (IN, OUT, INOUT) */
  bool entrada, saida;

 public:
  Parametro(void);
  ~Parametro(void){}

  void setType(TipoPgSQL type);

  void definirEntrada(bool valor);
  void definirSaida(bool valor);

  bool parametroEntrada(void);
  bool parametroSaida(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
  void operator = (const Parametro &param);
};

class Funcao: public BaseObject {
 private:
  static unsigned function_id;

  //Assinatura da função
  QString assinatura;

  //Biblioteca C que armazena a função.
  QString biblioteca;

  //Simbolo que identifica a função na Biblioteca C
  QString simbolo;

  //Definição (comandos que esta executa)
  QString codigo_fonte;

  //Linguagem em que foi escrita a função
  BaseObject *linguagem;

  //Parâmetros da função
  vector<Parametro> params;

  //Flag que indica se a função retorna um conjunto da dados
  bool retorna_setof;

  //Indica se a função janela (WINDOW)
  bool funcao_janela;

  //Tipo de comportamento da função
  TipoComportamento tipo_comportamento;

  //Tipo da função (volátil, imutável, estável)
  TipoFuncao tipo_funcao;

  //Tipo de retorno da função
  TipoPgSQL tipo_retorno;

  /* Vetor usado para armazenas as colunas retornadas
     pela clausula RETURNS TABLE. Esta cláusula, ao invés de retornar
     um elemento em específico, retorna uma tabela inteira.
     Esta estrutura é usada apenas para geração de SQL da versão >=8.4 */
  vector<Parametro> tipos_ret_tabela;

  //Tipo de segurança da função (SECURITY [INVOKER | DEFINER])
  TipoSeguranca tipo_seg;

  //Custo de execução da função
  unsigned custo_execucao;

  //Quantidade estimada de linhas retornadas pela função
  unsigned qtd_linhas;

  /* Formata a string de parâmetros usada pelo parser de esquema
     na geração da definição SQL da função */
  void definirAtributoParametros(unsigned tipo_def);

  /* Formata a string de tipos de retorno usada pelo parser de esquema
     na geração da definição da função */
  void definirAtributoTipoRetTabela(unsigned tipo_def);

 public:
  Funcao(void);

  //Define o nome da função
  void setName(const QString &obj_name);

  //Define o esquema ao qual a função faz parte
  void setSchema(BaseObject *schema);

  //Adiciona um parâmetr  função (com nome e tipo especificados)
  void adicionarParametro(Parametro param);

  //Adiciona um tipo de retorno específico para a cláusula RETURNS TABLE
  void adicionarTipoRetTabela(const QString &obj_name, TipoPgSQL tipo);

  //Define o corpo da função (comandos que serão executados por esta)
  void definirCodigoFonte(const QString &codigo);

  //Define a biblioteca que define a função em C
  void definirBiblioteca(const QString &biblioteca);

  //Define o simbolo que identifica a biblioteca na função
  void definirSimbolo(const QString &simbolo);

  //Define a linguagem que a função usará como base
  void definirLinguagem(BaseObject *linguagem);

  //Define o custo de execução da função
  void definirCustoExecucao(unsigned custo);

  //Define o custo de execução da função
  void definirQuantidadeLinhas(unsigned qtd_linhas);

  //Define se a função retora um conjunto de valores
  void definirRetornaSetOf(bool valor);

  //Define se a função é do tipo janela
  void definirFuncaoJanela(bool valor);

  //Define a configuração de retorno de valores da função
  void definirTipoComportamento(TipoComportamento tipo);

  //Define o tipo da função (volátil, imutável, estável)
  void definirTipoFuncao(TipoFuncao tipo);

  //Define o tipo de retorno da função
  void definirTipoRetorno(TipoPgSQL tipo);

  //Define o tipo de segurança ao chamar a função no SGBD
  void definirTipoSeguranca(TipoSeguranca tipo);

  //Retorna o corpo da função
  QString obterCodigoFonte(void);

  //Retorna a biblioteca que define a função
  QString obterBiblioteca(void);

  //Retorna o simbolo que define a função na biblioteca
  QString obterSimbolo(void);

  //Retorna o nome da linguagem usada pela função
  BaseObject *obterLinguagem(void);

  //Retorna o número de parâmetros da função
  unsigned obterNumParams(void);

  //Retorna o número de colunas na tabela de retorno
  unsigned obterNumTiposRetTabela(void);

  //Obtém um parâmetro através de seu índice
  Parametro obterParametro(unsigned idx_param);

  //Obtém um tipo retorno de tabela através de seu índice
  Parametro obterTipoRetTabela(unsigned idx_tipo);

  //Indica se a função retorna ou não um conjunto de dados
  bool retornaSetOf(void);

  //Indica se a função retorna ou não uma tabela
  bool retornaTabela(void);

  //Indica se a função é do tipo janela
  bool funcaoJanela(void);

  //Retorna o tipo de comportamento da função
  TipoComportamento obterTipoComportamento(void);

  //Retorna o tipo da função
  TipoFuncao obterTipoFuncao(void);

  //Obtém o tipo de retorno da função
  TipoPgSQL obterTipoRetorno(void);

  //Retorna o tipo de segurança da função
  TipoSeguranca obterTipoSeguranca(void);

  //Retorna o custo de execução da função
  unsigned obterCustoExecucao(void);

  //Retorna a quantidade de linhas retornadas
  unsigned obterQuantidadeLinhas(void);

  //Remove um parâmetro através de seu nome e tipo
  void removerParametro(const QString &obj_name, TipoPgSQL tipo);

  //Remove um parâmetro através de seu índice
  void removerParametro(unsigned idx_param);

  //Remove todos os parâmetros da função
  void removerParametros(void);

  //Remove um tipo de retorno de tabela através de seu índice
  void removerTipoRetTabela(unsigned idx_tipo);

  //Remove todos os parâmetros da tabela de retorno da função
  void removerTiposRetTabela(void);

  //Retorna a assinatura da função nome completo + tipo de parâmetros
  QString obterAssinatura(void);

  /* Formata a string de assinaturada da função, o parâmetro padrão
     'formatar' indica se o nome da função deve ser formatado.
     Por padrão, a formatação é feita */
  void criarAssinatura(bool formatar=true);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def, bool forma_reduzida);
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
