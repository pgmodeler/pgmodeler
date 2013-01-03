/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libobjrenderer
# Classe: OGSubItemObjeto
# Description:Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            de itens (colunas) de tabela/visão.
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef TABLE_OBJECT_VIEW_H
#define TABLE_OBJECT_VIEW_H

#include "view.h"
#include "table.h"
#include "baseobjectview.h"

class TableObjectView: public BaseObjectView
{
 private:
  Q_OBJECT

  //Objeto descritor do subitem
  QGraphicsItem *descriptor;

  //Rótulos do subitem (nome, tipo e restrições/alias)
  QGraphicsSimpleTextItem *lables[3];

  /* Contanstes usadas para a montagem dos rótulos
   das colunas das tabelas quando desenhadas no viewport */
                       //String que finaliza um bloco de constraints da coluna
  static const QString DELIMITADOR_REST_FIM,
                       //String que inicia um bloco de constraints da coluna
                       DELIMITADOR_REST_INI,
                       TYPE_SEPARATOR,
                       //String que separa os tipos de constraints que se aplicam a coluna
                       CONSTR_SEPARATOR,
                       //String que indica a constraint FOREIGN KEY
                       TXT_FOREIGN_KEY,
                       //String que indica a constraint NOT NULL
                       TXT_NOT_NULL,
                       //String que indica a constraint PRIMARY KEY
                       TXT_PRIMARY_KEY,
                       //String que indica a constraint UNIQUE
                       TXT_UNIQUE;

  //Configura o descritor do subitem conforme o tipo do objeto de origem
  void configureDescriptor(ConstraintType constr_type=BaseType::null);

  /* Sobrecarga do método da classe QGraphicsItemGroup::itemChange() e sua implentação
     controla as alterações durante a seleção e movimentação do objeto. Esta nova implementação
     simplismente ignora qualquer mudança no item pois estas são controladas na classe OGTabela/OGVisao */
  QVariant itemChange(GraphicsItemChange, const QVariant &value)
  {
   return(value);
  }

 public:
  TableObjectView(TableObject *object=NULL);
  ~TableObjectView(void);

  //Configura o subitem como uma referência de visão
  void configureObject(Reference reference);

  //Configura o subitem como um objeto de tabela
  void configureObject(void);

  //Define a posição horizontal de um objeto filho
  void setChildObjectXPos(unsigned obj_idx, float px);

  //Obtém o objeto filho que compõem o grupo através o índice
  QGraphicsItem *getChildObject(unsigned obj_idx);

  /* Retorna uma string formatada para exibição, contendo as configurações de
     restrições de uma coluna. Coluna alocadas porém não atribuídas a nenhuma tabela
     são ignoradas pelo método */
  static QString getConstraintString(Column *column);
};

#endif
