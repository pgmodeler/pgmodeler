/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Visao que é usada para
#             gerar os códigos SQL pertinentes a estas.
# Data de Criação: 07/04/2008
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
#ifndef VISAO_H
#define VISAO_H

#include "referencia.h"
#include "tabelabase.h"
//***********************************************************
class Visao: public TabelaBase {
 private:
  //Armazena as referências para objetos e expressões
  vector<Referencia> referencias;

  /* Vetores que armazenam os ponteiros para as referências
     da visão. Estes vetores significam as expressões na
     parte SELECT-FROM, FROM-WHERE e após WHERE. */
  vector<unsigned> exp_select,
                   exp_from,
                   exp_where;

  /* Define o atributo declaração usado pelo parser de esquema
     na geração da definição SQL da visão */
  void definirAtributoDeclaracao(void);

  /* Define o atributo de referências usado pelo parser de esquema
     na geração da definição XML da visão */
  void definirAtributoReferencias(void);

  /* Verifica se uma determinada referência já está cadastrada
     retornando -1 caso não exista ou seu índice caso exista */
  int obterIndiceReferencia(Referencia &refer);

 public:
  Visao(void);
  ~Visao(void);

  /* Adiciona um referência a  visão e ao mesmo tempo em um dos vetores
     de expressão usandos os parâmetros tipo_sql e idx_exp (opcional) */
  void adicionarReferencia(Referencia &refer, unsigned tipo_sql, int idx_exp=-1);

  /* Remove uma referência da visão, caso o elemento a ser removido esteja
     sendo referenciado pelos vetores de expressão, os elementos desses
     ultimos vetores também são removidos */
  void removerReferencia(unsigned id_ref);
  void removerReferencias(void);

  //Remove um elemento de uma das listas de expressão através do índice
  void removerReferencia(unsigned idx_exp, unsigned tipo_sql);

  //Obtém o número de referências da visão
  unsigned obterNumReferencias(void);

  /* Obtém o número de elementos em uma das listas de expressão.
     Caso o parâmetro tipo_ref estiver especificado (>= 0) o usuário
     pode filtar a contagem pelo tipo de referência (expressão ou objeto) */
  unsigned obterNumReferencias(unsigned tipo_sql, int tipo_ref=-1);

  //Obtém uma referência da visão através do índice
  Referencia obterReferencia(unsigned id_ref);

  /* Obtém uma referência numa determinada posição de uma lista
     de expressões da visão */
  Referencia obterReferencia(unsigned id_ref, unsigned tipo_sql);

  /* Retorna o índice de uma dada referência em uma das lista de
     expressões SQL: SELECT-FROM, FROM-WHERE ou após WHERE */
  int obterIndiceReferencia(Referencia &ref, unsigned tipo_sql);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);

  /* Retorna se a visão referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar visões criadas pelo usuário as quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool referenciaColunaIncRelacao(void);

  //Retorna se a visão referencia uma dada tabela
  bool referenciaTabela(Tabela *tab);

  //Retorna se a visão referencia uma dada coluna
  bool referenciaColuna(Coluna *col);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (Visao &visao);
};
//***********************************************************
#endif
