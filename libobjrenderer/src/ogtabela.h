/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libobjrenderer
# Classe: OGTabela
# Description:Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            da classe Tabela. O sufixo OG foi adicionado ao nome da classe para evitar conflitos
#            de nomes entra a classe Tabela da biblioteca libpgmodeler e a classe abaixo definida.
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
#ifndef OG_TABELA_H
#define OG_TABELA_H

#include "table.h"
#include "basetableview.h"
#include "tabletitleview.h"
#include "tableobjectview.h"

class OGTabela: public BaseTableView {
 private:
  Q_OBJECT

  /* Objetos poligonal (retangulares) que define o corpo dos
     atributos extendidos (indices, regras e gatilho) */
  QGraphicsPolygonItem  *corpo_atribs_ext;

  //Grupo de itens que armazena, respectivamente, os subitens de colunas e de atributos extendidos
  QGraphicsItemGroup *colunas, *atributos_ext;

  //Armazena a referência ao objeto de tabela selecionado na tabela
  TableObject *obj_filho_sel;

  //Métodos que controlam o foco/seleção subitens da tabela
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *evento);
  void hoverMoveEvent(QGraphicsSceneHoverEvent *evento);
  void mousePressEvent(QGraphicsSceneMouseEvent *evento);

  //Sobrecarga do método OGTabelaBase::itemChange()
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

 public:
  OGTabela(Table *tabela);
  ~OGTabela(void);

 private slots:
  void configureObject(void);

 signals:
  /* Este sinal sempre é disparado quando o usuário clica com o botão direito
     sobre um item da tabela em foco */
  void s_objetoFilhoSelecionado(TableObject *);
};

#endif
