/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definição da classe ConversaoCodificacao que é usada para
#             gerar os códigos SQL pertinentes aos conversions entre tipos
#             de codificação de caracteres no banco de dados.
#
# Creation date:o: 04/06/2008
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
#ifndef CONVERSAOCODIFICACAO_H
#define CONVERSAOCODIFICACAO_H

#include "objetobase.h"
#include "funcao.h"
#include "papel.h"

class ConversaoCodificacao: public ObjetoBase {
 private:
 /* Tipos de codificação da conversão:
    0 -> Codificação a ser convertido
    1 -> Codificação para a qual será convertida */
  TipoCodificacao codificacoes[2];

  //Função usada para converter
  Funcao *funcao_conv;

  /* Indica que a conversão é padrão entre os
     tipos de codificação */
  bool padrao;

 public:
  /* Constantes usadas para acessar os tipos de codificação
     da origem e do destino */
  static const unsigned CONV_COD_ORIGEM=0,
                        CONV_COD_DESTINO=1;

  ConversaoCodificacao(void);

  /* Define se a conversão é padrão ou não para os tipos de
     codificação envolvidos */
  void definirPadrao(bool valor);

  //Define o tipo de uma das codificações envolvidas (de origem ou destino)
  void definirCodificacao(unsigned idx_tipo, TipoCodificacao tipo_cod);

  //Define a função de conversão do objeto
  void definirFuncaoConversao(Funcao *funcao_conv);

  /* Obtém um tipo de codificação de acordo com o índice
     especificado (origem ou destino) */
  TipoCodificacao obterCodificacao(unsigned idx_tipo);

  //Obtém a função de conversão do objeto
  Funcao *obterFuncaoConversao(void);

  //Retorna se a conversão é padrão
  bool conversaoPadrao(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};

#endif
