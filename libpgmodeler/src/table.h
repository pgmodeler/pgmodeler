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
#ifndef TABLE_H
#define TABLE_H

#include "basegraphicobject.h"
#include "basetable.h"
#include "column.h"
#include "constraint.h"
#include "index.h"
#include "rule.h"
#include "trigger.h"
#include "function.h"
#include "role.h"

class Table: public BaseTable {
 private:
  //Armazena as colunas da tabela
  vector<TableObject *> columns;

  //Armazena as constraints que se aplicam a  tabela
  vector<TableObject *> constraints;

  //Armazena os índices que se aplicam a tabela
  vector<TableObject *> indexes;

  //Armazena as regras que se aplicam a  tabela
  vector<TableObject *> rules;

  //Armazena os gatilhos que se aplicam a  tabela
  vector<TableObject *> triggers;

  //Armazena as tabelas das quais a tabela atual herda atributos
  vector<Table *> ancestor_tables;

  //Indica se as linhas da tabela aceitam OIDs
  bool with_oid;

  //Tabelas das quais serão clonadas as colunas
  vector<Table *> copy_tables;

   /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

  /* Formata a QString de colunas usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void setColumnsAttribute(unsigned def_type);

  /* Formata a QString de restrições usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void setConstraintsAttribute(unsigned def_type);

  /* Formata a QString de gatilhos usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void setTriggersAttribute(unsigned def_type);

  /* Formata a QString de índices usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void setIndexesAttribute(unsigned def_type);

  /* Formata a QString de regras usada pelo parser de esquema
     na geração da definição SQL da tabela */
  void setRulesAttribute(unsigned def_type);

  void setCommentAttribute(TableObject *tab_obj);

 protected:

  void addAncestorTable(Table *tab, int idx=-1);
  void addCopyTable(Table *tab, int idx=-1);

  //Métodos que removem uma tabela descendente através de seu nome ou índice
  void removeAncestorTable(const QString &name);
  void removeAncestorTable(unsigned idx);

  //Métodos que removem uma tabela cópia através de seu nome ou índice
  void removeCopyTable(const QString &name);
  void removeCopyTable(unsigned idx);

 public:
   Table(void);
  ~Table(void);

  void setName(const QString &name);

  void setSchema(BaseObject *schema);

  //Define se a tabela aceita oids ou não
  void setWithOIDs(bool value);

 /* Adiciona um objeto  tabela. Seu tipo deve ser especificado para se seja inserido
     na lista de objetos correta. */
  void addObject(BaseObject *obj, int obj_idx=-1, bool copy_tab=false);

  /* Obtém um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

  /* Obtém um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja buscado na lista de objetos correta. */
  BaseObject *getObject(const QString &name, ObjectType obj_type);

  /* Remove um objeto da tabela através de seu índice. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removeObject(unsigned obj_idx, ObjectType obj_type);

  /* Remove um objeto da tabela através de seu nome. Seu tipo deve ser especificado
     para se seja removido da lista de objetos correta. */
  void removeObject(const QString &name, ObjectType obj_type);

  //Remove um objeto da tabela através de seu endereço em memória
  void removeObject(BaseObject *obj);

  /* Os métodos de adição de objetos abaixo são usados apenas por conveniência
     pois necessitam de apenas um parâmetro (o objeto a ser inserido). Internamente
     eles executam uma chamada ao método adicionarObjeto(OBJETO,TIPO_OBJETO) */
  void addColumn(Column *col, int idx=-1);
  void addConstraint(Constraint *constr, int idx=-1);
  void addTrigger(Trigger *trig, int idx=-1);
  void addIndex(Index *ind, int idx=-1);
  void addRule(Rule *reg, int idx_reg=-1);

  /* Métodos que retornam uma coluna através de seu nome ou índice.
     O método o qual busca uma coluna pelo nome tem um segundo parâmetro
     booleano o qual é usado para buscar colunas referenciando não o nome
     atual mas o nome antigo da coluna. Este parâmetro é usado para auxiliar
     as operações de referência a colunas adicionadas por relacionamentos
     a tabela */
  Column *getColumn(const QString &name, bool ref_old_name=false);
  Column *getColumn(unsigned idx);

  //Métodos que retornam uma constraint através de seu nome ou índice
  Constraint *getConstraint(const QString &name);
  Constraint *getConstraint(unsigned idx);

  //Métodos que retornam um gatilho através de seu nome ou índice
  Trigger *getTrigger(const QString &name);
  Trigger *getTrigger(unsigned idx);

  //Métodos que retornam um índice através de seu nome ou índice
  Index *getIndex(const QString &name);
  Index *getIndex(unsigned idx);

  //Métodos que retornam o número de objetos na tabela
  unsigned getColumnCount(void);
  unsigned getConstraintCount(void);
  unsigned getTriggerCount(void);
  unsigned getIndexCount(void);
  unsigned getRuleCount(void);
  unsigned getAncestorTable(void);
  unsigned getCopyTable(void);
  unsigned getObjectCount(ObjectType obj_type, bool inc_added_by_rel=true);

  //Métodos que retornam uma regra através de seu nome ou índice
  Rule *getRule(const QString &name);
  Rule *getRule(unsigned idx);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Table *getAncestorTable(const QString &name);
  Table *getAncestorTable(unsigned idx);

  //Métodos que retornam uma tabela descendente através de seu nome ou índice
  Table *getCopyTable(const QString &name);
  Table *getCopyTable(unsigned idx);

  //Métodos que removem uma coluna através de seu nome ou índice
  void removeColumn(const QString &name);
  void removeColumn(unsigned idx);

  //Métodos que removem uma constraint através de seu nome ou índice
  void removeConstraint(const QString &name);
  void removeConstraint(unsigned idx);

  //Métodos que removem um gatilho através de seu nome ou índice
  void removeTrigger(const QString &name);
  void removeTrigger(unsigned idx);

  //Métodos que removem um índice através de seu nome ou índice
  void removeIndex(const QString &name);
  void removeIndex(unsigned idx);

  //Métodos que removem uma regra através de seu nome ou índice
  void removeRule(const QString &name);
  void removeRule(unsigned idx);

  //Retorna a definição SQL ou XML do objeto
  virtual QString getCodeDefinition(unsigned def_type);

  //Obtém o índice de um determinado objeto através de seu nome
  int getObjectIndex(const QString &name, ObjectType obj_type);
  int getObjectIndex(TableObject *obj);

  //Retorna a chave primária da tabela se possuir
  Constraint *getPrimaryKey(void);

  /* Retorna todas as chaves estrangeiras presentes na tabela. O parâmetro
     inc_insporrelacao pode ser usado para se incluir ou não as restrições
     adicionadas por relacionamento. Por padrão este método retorna o vetor
     somente com as chaves estrangeiras criadas pelo usuário (sem o uso de
     relacionamentos) */
  void getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel=false);

  //Retorna se a a tabela aceita ou não OIDS
  bool isWithOIDs(void);

  /* Sobrecarga do método de definição de objeto protegido
     da classe ObjetoBase. O metodo sobrecarregado protege
     ou desprotege todos os objetos da tabela em uma
     só chamada */
  void setProtected(bool value);

  //Retorna se a coluna é referenciada por uma das restrições do tipo especificado
  bool isConstraintRefColumn(Column *column, ConstraintType constr_type);

  /* Método utilitário que faz a inversão de posição entre dois objetos na lista.
     Este método auxilia na ordenação dos objetos da tabela conforme o usuário desejar */
  void swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2);

  //Retorna se a tabela referencia colunas e restrições incluídas por relacionamentos
  bool isReferRelationshipAddedObject(void);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (Table &tabela);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<TableObject *> *getObjectList(ObjectType obj_type);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode=false);

  /* Retorna se a tabela referencia a tabela passada em uma de suas chaves estrangeiras. Este método considera
     apenas chaves estrangeiras criadas pelo usuário. Este método é usado como auxiliar no controle dos
     relacionamentos originários de chaves estrangeiras no modelo. */
  bool isReferTableOnForeignKey(Table *ref_tab);

  friend class Relationship;
  friend class OperationList;
};

#endif
