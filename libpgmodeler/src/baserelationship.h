/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
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
  vector<QPointF> points;

  //Indica que o relacionamento está conectado
  bool connected;

  /*Indica a obrigatoriedade da participação da entidade
    (tabela) no relacionamento. (Padrão: false) */
  bool src_mandatory,
       dst_mandatory;

  /* Rótulos do relacionamento:
     0 - Rótulo Cardinalidade Origem
     1 - Rótulo Cardinalidade Destino
     2 - Rótulo Nome do Relacionamento */
  Textbox *lables[3];

  //Armazena a distâncias dos rótulos de suas origens
  QPointF lables_dist[3];

  //Entidades envolvidas no relacionamento
  BaseTable *src_table,
            *dst_table;

  //Tipo do relacionamento (1-1, 1-n, n-n, gen ou dep)
  unsigned rel_type;

  //Define os atributos usados na obtenção do código XML do relacionamento
  void setRelationshipAttributes(void);

  /* Efetua a configuração do relacionamento, criando o objeto gráfico,
     a linha do relacionamento e os demais objetos necessários a sua
     exibição */
  void configureRelationship(void);

  /* Cria as linhas de conexão do relacionamento (linhas
     que se conectam as tabelas) */
  void connectRelationship(void);

  //Remove as linhas de conexão do relacionamento
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
