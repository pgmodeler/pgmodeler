/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Tipo que é usada para
#             gerar os códigos SQL pertinentes aos tipos
#             definidos pelo usuário
#
# Creation date: 05/06/2008
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
#ifndef TIPO_H
#define TIPO_H

#include "baseobject.h"
#include "schema.h"
#include "function.h"
#include "role.h"

class Tipo: public BaseObject {
 private:
  static unsigned type_id;

  //Configuração do tipo
  unsigned config;

  //Atributos para a configuração TIPO_COMPOSTO
  vector<Parameter> attributes;

  //Atributos para a configuração TIPO_ENUMERACAO
  vector<QString> enumeracoes;

  //Atributos para a configuração TIPO_BASE
  //Funções usadas pelo tipo
  Function *funcoes[7];

  //Comprimento interno ( > 0 - TAMANHO FIXO, 0 - TAMANHO VARIÁVEL)
  unsigned comp_interno; //INTERNALLENGTH

  //Indica se o tipo é passado por valor
  bool por_valor; //PASSEDBYVALUE

  //Alinhamento de armazenamento (char, smallint (int2), integer (int4) ou double precision)
  TipoPgSQL alinhamento, //ALIGNMENT
            elemento; //ELEMENT

  //Tipo de armazenamento (plain, main, extended, external)
  TipoArmazenamento armazenamento; //STORAGE

  //Valor padrão do tipo
  QString valor_padrao; //DEFAULT

  //** Atributos Adicionados devido a  alteraçõpes na versão 8.4 **/
  //Categoria do tipo (apenas para tipo base) - PADRÃO 'U'
  TipoCategoria categoria; //CATEGORY

  //Usado em conjunto ao tipo de categoria (apenas para tipo base) - Padrão FALSE
  bool preferido; //PREFERRED

  /* Tipo o qual terá alguns de seus atributos copiados para o tipo atual
     (apenas para tipo base). Caso o tipo_copia seja 'any' significa que o tipo
     atual não copia atributos de tipo algum */
  TipoPgSQL tipo_copia; //LIKE

  //Caractere delimitador de valores para arrays do tipo criado
  char delimitador; //DELIMITER

  //Verifica se um atibuto com o nome no parâmetro já existe
  bool atributoExiste(const QString &nome_atrib);

  //Verifica se uma enumeração com o nome no parâmetro já existe
  bool enumeracaoExiste(const QString &nome_enum);

  /* Formata a QString de elementos usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void definirAtributoElementos(unsigned tipo_def);

  /* Formata a QString de enumerações usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void definirAtributoEnumeracoes(unsigned tipo_def);

  /* Efetua a conversão dos parâmetros e tipos de retornos, de funções.
     Caso o parametro 'conv_inversa' estiver marcado, o método
     faz a conversão de 'nome_do_tipo' para 'any' caso contrário
     faz a conversão de 'any' para 'nome_do_tipo'. Este método é usado
     quando se gera os codigos SQL do tipo definido pelo usuário */
  void converterParametrosFuncoes(bool conv_inversa=false);

 public:
  static const unsigned TIPO_BASE=10,
                        TIPO_ENUMERACAO=20,
                        TIPO_COMPOSTO=30;

  static const unsigned FUNCAO_INPUT=0,
                        FUNCAO_OUTPUT=1,
                        FUNCAO_RECV=2,
                        FUNCAO_SEND=3,
                        FUNCAO_TPMOD_IN=4,
                        FUNCAO_TPMOD_OUT=5,
                        FUNCAO_ANALYZE=6;

  Tipo(void);
  ~Tipo(void);

  //Atribui um nome ao tipo
  void setName(const QString &obj_name);

  //Atribui um esquema ao tipo
  void setSchema(BaseObject *schema);

  //Define a configuração do tipo
  void definirConfiguracao(unsigned conf);

  //Métodos de configuração para TIPO_COMPOSTO
  void adicionarAtributo(Parameter atrib);
  void removerAtributo(unsigned idx_atrib);
  void removerAtributos(void);

  //Métodos de configuração para TIPO_ENUMERACAO
  void adicionarEnumeracao(const QString &enumer);
  void removerEnumeracao(unsigned idx_enum);
  void removerEnumeracoes(void);

  //Métodos de configuração para TIPO_BASE
  void definirFuncao(unsigned conf_func, Function *funcao);
  void definirCompInterno(unsigned comp);
  void definirPorValor(bool valor);
  void definirAlinhamento(TipoPgSQL tipo);
  void definirArmazenamento(TipoArmazenamento armaz);
  void definirValorPadrao(const QString &valor_padrao);
  void definirElemento(TipoPgSQL elemento);
  void definirDelimitador(char delim);
  void definirCategoria(TipoCategoria categoria);
  void definirPreferido(bool preferido);
  void definirTipoCopia(TipoPgSQL tipo_copia);

  Parameter obterAtributo(unsigned idx_atrib);
  unsigned obterNumAtributos(void);
  QString obterEnumeracao(unsigned idx_enum);
  unsigned obterNumEnumeracoes(void);
  unsigned obterConfiguracao(void);
  TipoCategoria obterCategoria(void);
  bool tipoPreferido(void);
  TipoPgSQL obterTipoCopia(void);



  //Métodos de obtenção para TIPO_BASE
  Function *obterFuncao(unsigned conf_func);
  unsigned obterCompInterno(void);
  bool passadoPorValor(void);
  TipoPgSQL obterAlinhamento(void);
  TipoArmazenamento obterArmazenamento(void);
  QString obterValorPadrao(void);
  TipoPgSQL obterElemento(void);
  char obterDelimitador(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def, bool forma_reduzida);
  QString getCodeDefinition(unsigned tipo_def);

  void operator = (Tipo &tipo);

  friend class ModeloBD;
};

#endif
