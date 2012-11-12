/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Tabela que é usado para
#             representar graficamente as tabelas e gerar os códigos SQL
#             pertinentes a esta.
# Creation date: 17/09/2006
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
#ifndef TABELA_H
#define TABELA_H

#include "basegraphicobject.h"
#include "basetable.h"
#include "column.h"
#include "restricao.h"
#include "index.h"
#include "regra.h"
#include "trigger.h"
#include "function.h"
#include "papel.h"

class Tabela: public BaseTable {
 private:
  //Armazena as colunas da tabela
  vector<TableObject *> colunas;

  //Armazena as constraints que se aplicam a  tabela
  vector<TableObject *> restricoes;

  //Armazena os índices que se aplicam a tabela
  vector<TableObject *> indices;

  //Armazena as regras que se aplicam a  tabela
  vector<TableObject *> regras;

  //Armazena os gatilhos que se aplicam a  tabela
  vector<TableObject *> gatilhos;

  //Armazena as tabelas das quais a tabela atual herda atributos
  vector<Tabela *> tabelas_pai;

  //Indica se as linhas da tabela aceitam OIDs
  bool aceita_oids;

  //Tabelas das quais serão clonadas as colunas
  vector<Tabela *> tabelas_copia;

   /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *obterObjeto(const QString &obj_name, ObjectType tipo_obj, int &idx_obj);

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
  void removerTabelaPai(const QString &obj_name);
  void removerTabelaPai(unsigned idx_tab);

  //Métodos que removem uma tabela cópia através de seu nome ou índice
  void removerTabelaCopia(const QString &obj_name);
  void removerTabelaCopia(unsigned idx_tab);

 public:
   Tabela(void);
  ~Tabela(void);

  void setName(const QString &obj_name);
  void setSchema(BaseObject *schema);

  //Define se a tabela aceita oids ou não
  void definirAceitaOids(bool valor);

 /* Adiciona um objeto  tabela. Seu tipo deve ser especificado para se seja inserido
     na lista de objetos correta. */
  void adicionarObjeto(BaseObject *obj, int idx_obj=-1, bool tab_copia=false);

  /* Obtém um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *obterObjeto(unsigned idx_obj, ObjectType tipo_obj);

  /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *obterObjeto(const QString &obj_name, ObjectType tipo_obj);

  /* Remove um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removerObjeto(unsigned idx_obj, ObjectType tipo_obj);

  /* Remove um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removerObjeto(const QString &obj_name, ObjectType tipo_obj);

  //Remove um objeto da tabela através de seu endereço em memória
  void removerObjeto(BaseObject *objeto);

  /* Os métodos de adição de objetos abaixo são usados apenas por conveniência
     pois necessitam de apenas um parâmetro (o objeto a ser inserido). Internamente
     eles executam uma chamada ao método adicionarObjeto(OBJETO,TIPO_OBJETO) */
  void adicionarColuna(Column *col, int idx_col=-1);
  void adicionarRestricao(Restricao *constr, int idx_rest=-1);
  void adicionarGatilho(Gatilho *gat, int idx_gat=-1);
  void adicionarIndice(Index *ind, int idx_ind=-1);
  void adicionarRegra(Regra *reg, int idx_reg=-1);

  /* Métodos que retornam uma coluna através de seu nome ou índice.
     O método o qual busca uma coluna pelo nome tem um segundo parâmetro
     booleano o qual é usado para buscar colunas referenciando não o nome
     atual mas o nome antigo da coluna. Este parâmetro é usado para auxiliar
     as operações de referência a colunas adicionadas por relacionamentos
     a tabela */
  Column *obterColuna(const QString &obj_name, bool ref_nome_antigo=false);
  Column *obterColuna(unsigned idx_col);

  //Métodos que retornam uma constraint através de seu nome ou índice
  Restricao *obterRestricao(const QString &obj_name);
  Restricao *obterRestricao(unsigned idx_constr);

  //Métodos que retornam um gatilho através de seu nome ou índice
  Gatilho *obterGatilho(const QString &obj_name);
  Gatilho *obterGatilho(unsigned idx_gat);

  //Métodos que retornam um índice através de seu nome ou índice
  Index *obterIndice(const QString &obj_name);
  Index *obterIndice(unsigned idx_ind);

  //Métodos que retornam o número de objetos na tabela
  unsigned obterNumColunas(void);
  unsigned obterNumRestricoes(void);
  unsigned obterNumGatilhos(void);
  unsigned obterNumIndices(void);
  unsigned obterNumRegras(void);
  unsigned obterNumTabelasPai(void);
  unsigned obterNumTabelasCopia(void);
  unsigned obterNumObjetos(ObjectType tipo_obj, bool inc_insporrelacao=true);

  //Métodos que retornam uma regra através de seu nome ou índice
  Regra *obterRegra(const QString &obj_name);
  Regra *obterRegra(unsigned idx_reg);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Tabela *obterTabelaPai(const QString &obj_name);
  Tabela *obterTabelaPai(unsigned idx_tab);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Tabela *obterTabelaCopia(const QString &obj_name);
  Tabela *obterTabelaCopia(unsigned idx_tab);

  //Métodos que removem uma coluna através de seu nome ou índice
  void removerColuna(const QString &obj_name);
  void removerColuna(unsigned idx_col);

  //Métodos que removem uma constraint através de seu nome ou índice
  void removerRestricao(const QString &obj_name);
  void removerRestricao(unsigned idx_const);

  //Métodos que removem um gatilho através de seu nome ou índice
  void removerGatilho(const QString &obj_name);
  void removerGatilho(unsigned idx_gat);

  //Métodos que removem um índice através de seu nome ou índice
  void removerIndice(const QString &obj_name);
  void removerIndice(unsigned idx_ind);

  //Métodos que removem uma regra através de seu nome ou índice
  void removerRegra(const QString &obj_name);
  void removerRegra(unsigned idx_reg);

  //Retorna a definição SQL ou XML do objeto
  virtual QString getCodeDefinition(unsigned tipo_def);

  //Obtém o índice de um determinado objeto através de seu nome
  int obterIndiceObjeto(const QString &obj_name, ObjectType tipo_obj);
  int obterIndiceObjeto(TableObject *objeto);

  //Retorna a chave primária da tabela se possuir
  Restricao *obterChavePrimaria(void);

  //Retorna se a a tabela aceita ou não OIDS
  bool aceitaOids(void);

  /* Sobrecarga do método de definição de objeto protegido
     da classe ObjetoBase. O metodo sobrecarregado protege
     ou desprotege todos os objetos da tabela em uma
     só chamada */
  void setProtected(bool is_protected);

  //Retorna se a coluna é referenciada por uma das restrições do tipo especificado
  bool restricaoReferenciaColuna(Column *coluna, TipoRestricao tipo_rest);

  /* Método utilitário que faz a inversão de posição entre dois objetos na lista.
     Este método auxilia na ordenação dos objetos da tabela conforme o usuário desejar */
  void trocarIndicesObjetos(ObjectType tipo_obj, unsigned idx1, unsigned idx2);

  //Retorna se a tabela referencia colunas e restrições incluídas por relacionamentos
  bool referenciaObjetoIncRelacao(void);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (Tabela &tabela);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<TableObject *> *obterListaObjetos(ObjectType tipo_obj);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void obterReferenciasColuna(Column *coluna, vector<TableObject *> &vet_refs, bool modo_exclusao=false);

  friend class Relacionamento;
  friend class ListaOperacoes;
};

#endif
