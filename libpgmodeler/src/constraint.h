/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Constraint
# Description: Implements the operations to manipulate table constraints
#              (primary keys, foreign key, unique and check).
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
#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "tableobject.h"
#include "tablespace.h"
#include "column.h"

class Constraint: public TableObject{
 private:
  //Tipo da constraint
  TipoRestricao constr_type;

  //Indica se a restrição é postergável ou não (apenas para fk)
  bool deferrable;

  //Tipo de postergacao da restrição (apenas para fk)
  TipoPostergacao deferral_type;

  //Tipo de comparação da restrição (apenas para fk)
  TipoComparacao match_type;

  /* Fator de preenchimento do método usado pela restrição (FILLFACTOR)
     usado apenas por chaves primárias e chaves únicas */
  unsigned fill_factor;

  /* (Apenas para chaves estrageiras) Especifica que tipo de
     ação deve ser tomada ao excluir (acao_del) e
     ao atualizar (acao_upd) */
  TipoAcao del_action, upd_action;

  //Armazena as colunas que formam a constraint (não utilizado para ck)
  vector<Column *> columns;

  /* (Apenas para chaves estrageiras) Armazena as colunas que
     são referenciadas na tabela referenciada */
  vector<Column *> ref_columns;

  /* (Apenas para constraints CHECK) Armazena a expressão
     que será usada como checagem */
  QString check_expr;

  /* (Apenas para chaves estrangeiras) Armazena o endereço
     da tabela de destino das referências. Na modelagem desta classe
     a classe Tabela era utilizada mas por limitações da linguagem a classe
     ObjetoBase está sendo usada que de qualquer forma serve como
     referencia a uma tabela, pois esta ultima herda as propriedades de
     ObjetoGraficoBase. */
  BaseObject *ref_table;

  /* Formata a QString de colunas usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void setColumnsAttribute(unsigned col_type, unsigned def_type, bool inc_addedbyrel=false);

 public:
  //Constantes utilizadas para referenciar as colunas de origem e destino
  static const unsigned SOURCE_COL=0,
                        REFERENCED_COL=1;

  Constraint(void);
  ~Constraint(void);

  /* Adiciona uma coluna a uma das listas de colunas da constraint
     Caso o parâmetro col_dest estiver como true a lista 'colunas_dest'
     será usada caso contrário 'colunas' é que será usada.  */
  void addColumn(Column *column, unsigned col_type);

  //Define o tipo da constraint
  void setConstraintType(TipoRestricao constr_type);

  /* Define o tipo de ação nas chaves estrangeiras (ON DELETE e ON UPDATE).
     Caso o parâmetro 'upd' esteja como true, o atributo 'acao_upd' é que
     receberá o valor do parametro 'tipo' caso contrário será 'acao_del'
     que receberá. */
  void setActionType(TipoAcao action_type, bool upd);

  //Define o tipo de postergação da restrição
  void setDeferralType(TipoPostergacao deferral_type);

  //Define se a restrição é postergavel ou não
  void setDeferrable(bool value);

  //Define o tipo de comparação da restrição
  void setMatchType(TipoComparacao constr_type);

  //Define a expressão de checagem caso a constraint seja do tipo CHECK
  void setCheckExpression(const QString &expr);

  //Define a tabela de destino nas referências da chave estrangeira.
  void setReferencedTable(BaseObject *tab_ref);

  /* Define o espaço de tabela usado pela restrição.
     Espaços de tabelas só podem ser atribuídas a restrições
     do tipo UNIQUE ou PRIMARY KEY */
  void setTablespace(Tablespace *tabspc);

  //Define o fator preenchimento da restrição
  void setFillFactor(unsigned factor);

  //Retorna o fator de preenchimento da restrição
  unsigned getFillFactor(void);

  /* Obtém o tipo de ação da chave estrangeira.
     Caso 'upd' esteja como true, o atributo 'acao_upd' será retornado
     caso contrário 'acao_del' é que será retornado. */
  TipoAcao getActionType(bool upd);

  /* Obtém uma coluna através de seu índice. Caso 'col_dest' esteja como
     true, uma coluna da lista 'colunas_dest' será retornada, caso contrário
     uma coluna da lista 'colunas' será retornada. */
  //Coluna *obterColuna(unsigned idx_col, bool col_dest=false);
  Column *getColumn(unsigned col_idx, unsigned col_type);

  /* Obtém uma coluna através de seu nome. Caso 'col_dest' esteja como
     true, uma coluna da lista 'colunas_dest' será retornada, caso contrário
     uma coluna da lista 'colunas' será retornada. */
  //Coluna *obterColuna(QString nome, bool col_dest=false);
  Column *getColumn(const QString &name, unsigned col_type);

  /* Obtém o número de colunas de uma das listas. Caso 'col_dest' esteja como
     true, o número de colunas da lista 'colunas_dest' será retornado, caso contrário
     o número de colunas da lista 'colunas' será retornado. */
  //unsigned obterNumColunas(bool col_dest=false);
  unsigned getColumnCount(unsigned col_type);

 /* Remove uma coluna de uma das listas através de seu nome. Caso 'col_dest' esteja como
    true, uma coluna da lista 'colunas_dest' será removida, caso contrário
    uma coluna da lista 'colunas_dest' será removida. */
  void removeColumn(const QString &name, unsigned col_type);

  //Limpa as listas de colunas
  void removeColumns(void);

  //Obtém o tipo da restrição
  TipoRestricao getConstraintType(void);

  //Obtém a expressão de checagem da restrição
  QString getCheckExpression(void);

  //Obtém a tabela de destino
  BaseObject *getReferencedTable(void);

  //Obtém o tipo de postergação da restrição
  TipoPostergacao getDeferralType(void);

  //Obtém se a restrição é postergável ou não
  bool isDeferrable(void);

  /* Retorna se a restrição referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar restrições criadas pelo usuário as quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool isReferRelationshipColumn(void);

  //Obtém o tipo de comparação da restrição
  TipoComparacao getMatchType(void);

  //Retorna a definição SQL ou XML do objeto
  /* Para manter a compatibilidade com o método puramente virtual da classe ancestral
     esse método retorna por padrão da definição SQL/XML sem incluir os objetos protegidos */
  QString getCodeDefinition(unsigned def_type);

  //Retorna a definição SQL/XML filtrando por objetos incluídos por relacionamento ou não
  QString getCodeDefinition(unsigned def_type, bool inc_addedbyrel);

  //Indica se uma dada coluna existe na lista de colunas de origem ou de destino
  bool isColumnExists(Column *column, unsigned col_type);

  friend class Tabela;
  friend class Relacionamento;
};

#endif
