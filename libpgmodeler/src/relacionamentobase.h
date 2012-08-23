/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe RelacionamentoBase que implementa operações
#             básicas de manipulação gráfica de relacionamento entre tabelas
# Data de Criação: 09/04/2008
#
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License
# Q Public License version 1.0 (QPL 1.0)
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
#ifndef RELACIONAMENTOBASE_H
#define RELACIONAMENTOBASE_H

#include "tabelabase.h"
#include "caixatexto.h"
#include <cmath>
//***********************************************************
class RelacionamentoBase: public ObjetoGraficoBase {
 protected:
  vector<QPointF> pontos;

  //Indica que o relacionamento está conectado
  bool conectado;

  /*Indica a obrigatoriedade da participação da entidade
    (tabela) no relacionamento. (Padrão: false) */
  bool obrig_dest,
       obrig_orig;

  /* Rótulos do relacionamento:
     0 - Rótulo Cardinalidade Origem
     1 - Rótulo Cardinalidade Destino
     2 - Rótulo Nome do Relacionamento */
  CaixaTexto *rotulos[3];

  //Armazena a distâncias dos rótulos de suas origens
  QPointF dist_rotulos[3];

  //Entidades envolvidas no relacionamento
  TabelaBase *tabela_orig,
             *tabela_dest;

  //Tipo do relacionamento (1-1, 1-n, n-n, gen ou dep)
  unsigned tipo_relac;

  //Define os atributos usados na obtenção do código XML do relacionamento
  void definirAtributosRelacionamento(void);

  /* Efetua a configuração do relacionamento, criando o objeto gráfico,
     a linha do relacionamento e os demais objetos necessários a sua
     exibição */
  void configurarRelacionamento(void);

  /* Cria as linhas de conexão do relacionamento (linhas
     que se conectam as tabelas) */
  void conectarRelacionamento(void);

  //Remove as linhas de conexão do relacionamento
  void desconectarRelacionamento(void);

 public:
  //Constantes usadas para referenciar os tipos de relacionamento
  static const unsigned RELACIONAMENTO_11=10, //Relacionamento 1-1
                        RELACIONAMENTO_1N=20, //Relacionamento 1-n
                        RELACIONAMENTO_NN=30, //Relacionamento n-n
                        RELACIONAMENTO_GEN=40, //Relacionamento de Generalização
                        RELACIONAMENTO_DEP=50; //Relacionamento de Dependência (tabela-visão) / Cópia (tabela-tabela)

  //Constantes utilizadas pelo método de movimentação de rótulos do relacionamento
  static const unsigned ROTULO_CARD_ORIGEM=0,
                        ROTULO_CARD_DESTINO=1,
                        ROTULO_NOME_RELAC=2;

  //Constantes utilizadas para referenciar tabelas de origem e destino
  static const unsigned TABELA_ORIGEM=3,
                        TABELA_DESTINO=4;

  RelacionamentoBase(RelacionamentoBase *relacao);

  RelacionamentoBase(const QString &nome, unsigned tipo_rel,
                 TabelaBase *tab_orig, TabelaBase *tab_dest,
                 bool obrig_orig, bool obrig_dest);
  ~RelacionamentoBase(void);


  //Método específico de definição do nome de relacionamentos
  void definirNome(const QString &nome);

  //Define a obrigatoriedade da participação de uma entidade no relacionamento
  void definirTabelaObrigatoria(unsigned id_tabela, bool valor);

  //Obtém um rótulo através de seu índice
  CaixaTexto *obterRotulo(unsigned idx_rot);

  //Obtém uma tabela participante da relação através de seu índice
  TabelaBase *obterTabela(unsigned id_tabela);

  //Retorna o tipo do relacionamento
  unsigned obterTipoRelacionamento(void);

  //Retorna se a tabela (origem ou destino) é obrigatória no relacionamento
  bool tabelaObrigatoria(unsigned id_tabela);

  //Retorna se o relacionamento está conectado ou não
  bool relacionamentoConectado(void);

  /* Reinicia a posição dos rótulos, fazendo que os mesmos sejam
     reajustados toda vez que a linha do relacionamento é
     modificada */
  void reiniciarPosicaoRotulos(void);

  //Retorna a definição XMl do relacionamento
  QString obterDefinicaoObjeto(void);

  /* Retorna se o relacionamento é um auto-relacionamento.
     Esse método evita a validação manual que é obter as
     duas tabelas e compará-las. Este método ja executa
     tal operação. */
  bool autoRelacionamento(void);

  /* Armazena os pontos que definem a linha do relacionamento.
     Este pontos são usados somente para gravar no arquivo XML
     a posição atual da linha do relacionamento */
  void definirPontos(const vector<QPointF> &pontos);

  //Retorna a lista de pontos que define a linha do relacionamento
  vector<QPointF> obterPontos(void);

  void definirDistanciaRotulo(unsigned idx_rot, QPointF dist_rotulo);
  QPointF obterDistanciaRotulo(unsigned idx_rot);

  //Operador que faz a atribuição entre um objeto e outro
  void operator = (RelacionamentoBase &rel);

  friend class ModeloBD;
};
//***********************************************************
#endif
