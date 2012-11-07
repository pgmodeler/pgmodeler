/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Constraint que é usado para
#              configurar e gerar os códigos SQL referentes a chaves primárias
#              e estrangeiras, de checagem, únicas.
# Creation date: 23/09/2006
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
#ifndef INDICE_H
#define INDICE_H

#include "tableobject.h"
#include "elementoindice.h"

class Indice: public TableObject{
 private:
  /* Vetor de classe de operadores o qual relacionada cada
     coluna ou expressão a um elemento deste container */
  vector<ElementoIndice> elementos;

  //Expressão condicional do índice
  QString exp_condicional;

  //Fator de preenchimento do método usado pelo índice (FILLFACTOR)
  unsigned fator_preenc;

  //Tipo de método de indexação do índice
  TipoIndexacao tipo_indexacao;

  //Atributos booleanos do indice (UNIQUE, CONCORRENTE, ATUAL_RAPIDA)
  bool atrib_indice[3];

  /* Formata a QString de colunas usada pelo parser de esquema
     na geração da definição SQL do índice */
  void definirAtributoElementos(unsigned formatar);

 public:
  const static unsigned UNIQUE=0;
  const static unsigned CONCORRENTE=1;
  const static unsigned ATUAL_RAPIDA=2;

  Indice(void);

  //Adiciona um elemento ao índice
  void adicionarElemento(Column *coluna, OperatorClass *classe_oper, bool ordem_asc, bool nulos_primeiro);
  void adicionarElemento(const QString &expressao, OperatorClass *classe_oper, bool ordem_asc, bool nulos_primeiro);
  void adicionarElemento(ElementoIndice elem);

  /* Retorna se um dado elemento existe no índice. O método
     retorna -1 quando o mesmo não é encontrado caso contrário
     retorna o próprio indice do mesmo */
  int elementoExiste(Column *coluna);
  int elementoExiste(const QString &expressao);

  //Obtém um elemento através de seu índice
  ElementoIndice obterElemento(unsigned idx_elem);

  //Remove um elemento através de seu índice
  void removerElemento(unsigned idx_elem);
  void removerElementos(void);

  //Define a expressão condicional do índice
  void definirExpCondicional(const QString &exp);

  //Define o tipo do método de indexação
  void definirTipoIndexacao(TipoIndexacao tipo_indexacao);

  //Define o estado de uma das 2 configurações do índice
  void definirAtributo(unsigned id_atrib, bool valor);

  //Define o fator preenchimento do índice
  void definirFatorPreenchimento(unsigned fator);

  //Obtém a expressão condicional do índice
  QString obterExpCondicional(void);

  //Obtém o número de elementos do índice
  unsigned obterNumElementos(void);

  //Obtém o tipo de método de indexação
  TipoIndexacao obterTipoIndexacao(void);

  //Obtém uma das configurações do indice
  bool obterAtributo(unsigned id_conf);

  //Obtém o fator de preenchimento do índice
  unsigned obterFatorPreenchimento(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);

  /* Retorna se o índice referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar índices criadas pelo usuário os quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool referenciaColunaIncRelacao(void);
};

#endif
