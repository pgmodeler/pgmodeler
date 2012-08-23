/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Tabela que é usado para
#             representar graficamente as tabelas e gerar os códigos SQL
#             pertinentes a esta.
# Data de Criação: 17/09/2006
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
#ifndef TABELA_H
#define TABELA_H

#include "objetograficobase.h"
#include "tabelabase.h"
#include "coluna.h"
#include "restricao.h"
#include "indice.h"
#include "regra.h"
#include "gatilho.h"
#include "funcao.h"
#include "papel.h"
//**************************************************************
class Tabela: public TabelaBase {
 private:
  //Armazena as colunas da tabela
  vector<ObjetoTabela *> colunas;

  //Armazena as constraints que se aplicam a  tabela
  vector<ObjetoTabela *> restricoes;

  //Armazena os índices que se aplicam a tabela
  vector<ObjetoTabela *> indices;

  //Armazena as regras que se aplicam a  tabela
  vector<ObjetoTabela *> regras;

  //Armazena os gatilhos que se aplicam a  tabela
  vector<ObjetoTabela *> gatilhos;

  //Armazena as tabelas das quais a tabela atual herda atributos
  vector<Tabela *> tabelas_pai;

  //Indica se as linhas da tabela aceitam OIDs
  bool aceita_oids;

  //Tabelas das quais serão clonadas as colunas
  vector<Tabela *> tabelas_copia;

   /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  ObjetoBase *obterObjeto(const QString &nome, TipoObjetoBase tipo_obj, int &idx_obj);

  /* Formata a QString de colunas usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void definirAtributoColunas(unsigned tipo_def);

  /* Formata a QString de restrições usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void definirAtributoRestricoes(unsigned tipo_def);

  /* Formata a QString de gatilhos usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void definirAtributoGatilhos(unsigned tipo_def);

  /* Formata a QString de índices usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void definirAtributoIndices(unsigned tipo_def);

  /* Formata a QString de regras usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void definirAtributoRegras(unsigned tipo_def);

 protected:

  void adicionarTabelaPai(Tabela *tab, int idx_tab=-1);
  void adicionarTabelaCopia(Tabela *tab, int idx_tab=-1);

  //Métodos que removem uma tabela descendente através de seu nome ou índice
  void removerTabelaPai(const QString &nome);
  void removerTabelaPai(unsigned idx_tab);

  //Métodos que removem uma tabela cópia através de seu nome ou índice
  void removerTabelaCopia(const QString &nome);
  void removerTabelaCopia(unsigned idx_tab);

 public:
   Tabela(void);
  ~Tabela(void);

  //Define se a tabela aceita oids ou não
  void definirAceitaOids(bool valor);

 /* Adiciona um objeto  tabela. Seu tipo deve ser especificado para se seja inserido
     na lista de objetos correta. */
  void adicionarObjeto(ObjetoBase *obj, int idx_obj=-1, bool tab_copia=false);

  /* Obtém um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  ObjetoBase *obterObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj);

  /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  ObjetoBase *obterObjeto(const QString &nome, TipoObjetoBase tipo_obj);

  /* Remove um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removerObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj);

  /* Remove um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removerObjeto(const QString &nome, TipoObjetoBase tipo_obj);

  //Remove um objeto da tabela através de seu endereço em memória
  void removerObjeto(ObjetoBase *objeto);

  /* Os métodos de adição de objetos abaixo são usados apenas por conveniência
     pois necessitam de apenas um parâmetro (o objeto a ser inserido). Internamente
     eles executam uma chamada ao método adicionarObjeto(OBJETO,TIPO_OBJETO) */
  void adicionarColuna(Coluna *col, int idx_col=-1);
  void adicionarRestricao(Restricao *constr, int idx_rest=-1);
  void adicionarGatilho(Gatilho *gat, int idx_gat=-1);
  void adicionarIndice(Indice *ind, int idx_ind=-1);
  void adicionarRegra(Regra *reg, int idx_reg=-1);

  /* Métodos que retornam uma coluna através de seu nome ou índice.
     O método o qual busca uma coluna pelo nome tem um segundo parâmetro
     booleano o qual é usado para buscar colunas referenciando não o nome
     atual mas o nome antigo da coluna. Este parâmetro é usado para auxiliar
     as operações de referência a colunas adicionadas por relacionamentos
     a tabela */
  Coluna *obterColuna(const QString &nome, bool ref_nome_antigo=false);
  Coluna *obterColuna(unsigned idx_col);

  //Métodos que retornam uma constraint através de seu nome ou índice
  Restricao *obterRestricao(const QString &nome);
  Restricao *obterRestricao(unsigned idx_constr);

  //Métodos que retornam um gatilho através de seu nome ou índice
  Gatilho *obterGatilho(const QString &nome);
  Gatilho *obterGatilho(unsigned idx_gat);

  //Métodos que retornam um índice através de seu nome ou índice
  Indice *obterIndice(const QString &nome);
  Indice *obterIndice(unsigned idx_ind);

  //Métodos que retornam o número de objetos na tabela
  unsigned obterNumColunas(void);
  unsigned obterNumRestricoes(void);
  unsigned obterNumGatilhos(void);
  unsigned obterNumIndices(void);
  unsigned obterNumRegras(void);
  unsigned obterNumTabelasPai(void);
  unsigned obterNumTabelasCopia(void);
  unsigned obterNumObjetos(TipoObjetoBase tipo_obj, bool inc_insporrelacao=true);

  //Métodos que retornam uma regra através de seu nome ou índice
  Regra *obterRegra(const QString &nome);
  Regra *obterRegra(unsigned idx_reg);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Tabela *obterTabelaPai(const QString &nome);
  Tabela *obterTabelaPai(unsigned idx_tab);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Tabela *obterTabelaCopia(const QString &nome);
  Tabela *obterTabelaCopia(unsigned idx_tab);

  //Métodos que removem uma coluna através de seu nome ou índice
  void removerColuna(const QString &nome);
  void removerColuna(unsigned idx_col);

  //Métodos que removem uma constraint através de seu nome ou índice
  void removerRestricao(const QString &nome);
  void removerRestricao(unsigned idx_const);

  //Métodos que removem um gatilho através de seu nome ou índice
  void removerGatilho(const QString &nome);
  void removerGatilho(unsigned idx_gat);

  //Métodos que removem um índice através de seu nome ou índice
  void removerIndice(const QString &nome);
  void removerIndice(unsigned idx_ind);

  //Métodos que removem uma regra através de seu nome ou índice
  void removerRegra(const QString &nome);
  void removerRegra(unsigned idx_reg);

  //Retorna a definição SQL ou XML do objeto
  virtual QString obterDefinicaoObjeto(unsigned tipo_def);

  //Obtém o índice de um determinado objeto através de seu nome
  int obterIndiceObjeto(const QString &nome, TipoObjetoBase tipo_obj);
  int obterIndiceObjeto(ObjetoTabela *objeto);

  //Retorna a chave primária da tabela se possuir
  Restricao *obterChavePrimaria(void);

  //Retorna se a a tabela aceita ou não OIDS
  bool aceitaOids(void);

  /* Sobrecarga do método de definição de objeto protegido
     da classe ObjetoBase. O metodo sobrecarregado protege
     ou desprotege todos os objetos da tabela em uma
     só chamada */
  void definirProtegido(bool protegido);

  //Retorna se a coluna é referenciada por uma das restrições do tipo especificado
  bool restricaoReferenciaColuna(Coluna *coluna, TipoRestricao tipo_rest);

  /* Método utilitário que faz a inversão de posição entre dois objetos na lista.
     Este método auxilia na ordenação dos objetos da tabela conforme o usuário desejar */
  void trocarIndicesObjetos(TipoObjetoBase tipo_obj, unsigned idx1, unsigned idx2);

  //Retorna se a tabela referencia colunas e restrições incluídas por relacionamentos
  bool referenciaObjetoIncRelacao(void);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (Tabela &tabela);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<ObjetoTabela *> *obterListaObjetos(TipoObjetoBase tipo_obj);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void obterReferenciasColuna(Coluna *coluna, vector<ObjetoTabela *> &vet_refs, bool modo_exclusao=false);

  friend class Relacionamento;
  friend class ListaOperacoes;
};
//***********************************************************
#endif
