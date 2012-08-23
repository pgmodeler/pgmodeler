/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ConversaoTipo que é usada para
#             gerar os códigos SQL pertinentes ao casts entre tipos
#             do banco de dados.
#
# Data de Criação: 03/06/2008
#
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License

#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef CONVERSAOTIPO_H
#define CONVERSAOTIPO_H

#include "objetobase.h"
#include "funcao.h"
//***********************************************************
class ConversaoTipo: public ObjetoBase {
 private:
 /* Tipos de dados da conversão:
    0 -> Tipo de dado a ser convertido
    1 -> Tipo para o qual será convertido */
  TipoPgSQL tipos[2];

  //Tipo da conversão (ASSIGNMENT ou IMPLICIT)
  unsigned tipo;

  //Função usada para converter os tipos envolvidos
  Funcao *funcao_conv;

  /* Indica que a conversão é de entrada e saída, ou seja,
     a função chamada pelo cast não será a função atribuída
     ao mesmo mas sim a função output do tipo de entrada */
  bool entrada_saida;

 public:
  //Constantes usadas para referenciar os tipos envolvidos na conversão
  static const unsigned CONV_TIPO_ORIGEM=0,
                        CONV_TIPO_DESTINO=1,
  //Constantes para definir o tipo da conversão
                        CONV_ATRIBUICAO=10,
                        CONV_IMPLICITA=20;

  ConversaoTipo(void);

  //Define um dos tipos de dados envolvidos na conversão
  void definirTipoDado(unsigned idx_tipo, TipoPgSQL tipo_dado);

  //Define o tipo da conversão (implicita ou atribuição)
  void definirTipoConversao(unsigned tipo);

  //Define a função usada para conversão entre os tipos
  void definirFuncaoConversao(Funcao *funcao_conv);

  /* Define se a conversão é de entrada e saída. Caso este
     parâmetro esteja especificado, a função de conversão especificada
     é desprezada na geração do código sql */
  void definirEntradaSaida(bool valor);

  //Obtém um tipo da dado envolvido na conversão
  TipoPgSQL obterTipoDado(unsigned idx_tipo);

  //Obtém o tipo da conversão
  unsigned obterTipoConversao(void);

  //Obtém a função de conversão
  Funcao *obterFuncaoConversao(void);

  //Obtém se a conversão é de entrada e saída
  bool obterEntradaSaida(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
