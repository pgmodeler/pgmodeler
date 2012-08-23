/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ElemClasseOperadores que é usada para
#             armazenar e gerar os códigos SQL pertinentes aos atributos
#             usados nas de classes de operadores.
# Data de Criação: 08/07/2008
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
#ifndef ELEMCLASSEOPERADORES_H
#define ELEMCLASSEOPERADORES_H

#include "objetobase.h"
#include "operador.h"
#include "funcao.h"
//***********************************************************
class ElemClasseOperadores {
 private:
  unsigned tipo_elemento;

  //Função usado no elemento (caso o tipo seja ELEM_FUNCAO)
  Funcao *funcao;

  //Operador usado no elemento (caso o tipo seja ELEM_OPERADOR)
  Operador *operador;

  /* Tipo de armazenamento do método de indexação (apenas
     se o tipo do elemento for ELEM_ARMAZENAMENTO) */
  TipoPgSQL tipo_armaz;

  /* Indica que a rechecagem das linhas obtidas é obrigatória, caso
     seja usada a indexação atribuída ao elemento atual.
     A rechecagem só é usada quando o tipo de elemento é ELEM_OPERADOR*/
  bool rechecar;

  /* Número de estratégia para os métodos de indexação e
     número de suporte para funções. O valor desse elemento
     deve ser maior que 0 (para ELEM_OPERADOR e ELEM_FUNCAO) */
  unsigned num_estrategia;

 public:
  /* Constantes usadas para referenciar o tipo de elemento de
     classe de operadores */
  static const unsigned ELEM_OPERADOR=0,
                        ELEM_FUNCAO=1,
                        ELEM_ARMAZENAMENTO=2;

  ElemClasseOperadores(void);

  //Define o elemento como sendo uma cláusula de função
  void definirFuncao(Funcao *funcao, unsigned num_estrategia);

  //Define o elemento como sendo uma cláusula de operador
  void definirOperador(Operador *operador, unsigned num_estrategia, bool rechecar);

  //Define o elemento como sendo uma cláusula de armazenamento
  void definirArmazenamento(TipoPgSQL tipo_armaz);

  //Obtém o tipo do elemento
  unsigned obterTipoElemento(void);

  //Obtém a função usada no elemento
  Funcao *obterFuncao(void);

  //Obtém o operador usado no elemento
  Operador *obterOperador(void);

  //Obtém o tipo de armazenamento
  TipoPgSQL obterTipoArmazenamento(void);

  //Retorna se o elemento necessita ser rechecado ou não
  bool rechecarElemento(void);

  //Obtém o número de estratégia/suporte do elemento
  unsigned obterNumEstrategia(void);

  //Obtém a definição sql do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);

  //Operador de comparação entre dois elementos
  bool operator == (ElemClasseOperadores &elem);
};
//***********************************************************
#endif
