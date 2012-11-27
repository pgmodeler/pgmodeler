/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseRelationship
# Description: Definição da classe RelacionamentoBase que implementa operações
#             básicas de manipulação gráfica de relacionamento entre tabelas
# Creation date: 09/04/2008
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
#ifndef BASE_RELATIONSHIP_H
#define BASE_RELATIONSHIP_H

#include "basetable.h"
#include "textbox.h"
#include <cmath>

class BaseRelationship: public BaseGraphicObject {
 private:
   QString getCodeDefinition(unsigned){ return(""); }

 protected:
  //Represents the points added by the user on the relationship line
  vector<QPointF> points;

  //Indicates whether the relationship is linked to the tables
  bool connected;

  //Indicates the mandatory participation of source and destination tables
  bool src_mandatory, dst_mandatory;

  /* Relationship lables:
     0 - Source cardinality
     1 - Destination cardinality
     2 - Relationship name */
  Textbox *lables[3];

  /* Stores the distances of the labels from its respectively origins.
     This is used to controle de position of the labels when they are
     moved by the user */
  QPointF lables_dist[3];

  //Entidades envolvidas no relacionamento
  BaseTable *src_table,
            *dst_table;

  /* Relationship type. It can be "One to One", "One to Many", "Many to Many",
     "Generalization", "Dependecy". The constants RELATIONSHIP_??? are used
     to assign a type to the relationship */
  unsigned rel_type;

  //Sets the attributes used on the generation of XML definition for relationship
  void setRelationshipAttributes(void);

  //Makes the initial configuration creating the labels
  void configureRelationship(void);

  //Marks the flag indicating that relationship is connected
  void connectRelationship(void);

  //Uncheck the flag indicating that relationship is disconnected
  void disconnectRelationship(void);

 public:
  //Constantes usadas para referenciar os tipos de relacionamento
  static const unsigned RELATIONSHIP_11=10, //Relacionamento 1-1
                        RELATIONSHIP_1N=20, //Relacionamento 1-n
                        RELATIONSHIP_NN=30, //Relacionamento n-n
                        RELATIONSHIP_GEN=40, //Relacionamento de Generalização
                        RELATIONSHIP_DEP=50; //Relacionamento de Dependência (tabela-visão) / Cópia (tabela-tabela)

  //Constantes utilizadas pelo método de movimentação de rótulos do relacionamento
  static const unsigned LABEL_SRC_CARD=0,
                        LABEL_DST_CARD=1,
                        LABEL_REL_NAME=2;

  //Constantes utilizadas para referenciar tabelas de origem e destino
  static const unsigned SRC_TABLE=3,
                        DST_TABLE=4;

  BaseRelationship(BaseRelationship *rel);

  BaseRelationship(unsigned rel_type, BaseTable *src_tab, BaseTable *dst_tab,
                     bool src_mandatory, bool dst_mandatory);
  ~BaseRelationship(void);


  //Método específico de definição do nome de relacionamentos
  void setName(const QString &name);

  //Define a obrigatoriedade da participação de uma entidade no relacionamento
  void setMandatoryTable(unsigned table_id, bool value);

  //Obtém um rótulo através de seu índice
  Textbox *getLabel(unsigned label_id);

  //Obtém uma tabela participante da relação através de seu índice
  BaseTable *getTable(unsigned table_id);

  //Retorna o tipo do relacionamento
  unsigned getRelationshipType(void);

  //Retorna se a tabela (origem ou destino) é obrigatória no relacionamento
  bool isTableMandatory(unsigned table_id);

  //Retorna se o relacionamento está conectado ou não
  bool isRelationshipConnected(void);

  /* Reinicia a posição dos rótulos, fazendo que os mesmos sejam
     reajustados toda vez que a linha do relacionamento é
     modificada */
  void resetLabelsPosition(void);

  //Retorna a definição XMl do relacionamento
  QString getCodeDefinition(void);

  /* Retorna se o relacionamento é um auto-relacionamento.
     Esse método evita a validação manual que é obter as
     duas tabelas e compará-las. Este método ja executa
     tal operação. */
  bool isSelfRelationship(void);

  /* Armazena os pontos que definem a linha do relacionamento.
     Este pontos são usados somente para gravar no arquivo XML
     a posição atual da linha do relacionamento */
  void setPoints(const vector<QPointF> &points);

  //Retorna a lista de pontos que define a linha do relacionamento
  vector<QPointF> getPoints(void);

  void setLabelDistance(unsigned label_id, QPointF label_dist);
  QPointF getLabelDistance(unsigned label_id);

  //Operador que faz a atribuição entre um objeto e outro
  void operator = (BaseRelationship &rel);

  friend class ModeloBD;
};

#endif
