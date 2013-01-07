/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Graphical objects renderer (libobjrenderer)
# Class: ObjectsScene
# Description: Implements the basic operations to visualize and manipulate the graphical objects.
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
#ifndef OBJECTS_SCENE_H
#define OBJECTS_SCENE_H

#include <QtGui>
#include "relationshipview.h"
#include "graphicalview.h"
#include "tableview.h"

class ObjectsScene: public QGraphicsScene {
 private:
  Q_OBJECT

  //Opções de alinhamento de objetos, exibição da grade e limites das paginas
  static bool align_objs_grid, show_grid, show_page_delim;

  //Tamanho da grade da cena
  static unsigned grid_size;

  //Tamanho do papel (vide QPrinter::PaperSize)
  static QPrinter::PaperSize paper_size;

  //Orientacao do papel (Landscape ou portrait, QPrinter::Orientation)
  static QPrinter::Orientation page_orientation;

  //Margens do papel
  static QRectF page_margins;

  /* Flag que indica que está ocorrendo a movimentção de objetos.
     Isso indica quando o sinal s_inicioMovimentoObjetos() ou
     s_finalMovimentoObjetos() deve ser disparado */
  bool moving_objs;

  //Ponto inicial do retângulo de seleção
  QPointF sel_ini_pnt;

  //Retângulo de seleção de objetos na cena
  QGraphicsPolygonItem *selection_rect;

  QGraphicsLineItem *rel_line;

  //Método utilitário para alinhar um pont �  grade
  static QPointF alignPointToGrid(const QPointF &pnt);

 protected:
  //Brush que define o padrão da grade
  static QBrush grid;

  /* Método sobrecarregados de eventos do mouse. Executa operações adicionais antes das
     operações de mouse padrão da classe superior QGraphicsScene */
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  //Exibe uma linha a partir do ponto 'p' que simula a criação de um relacionamento entre tabelas
  void showRelationshipLine(bool value, const QPointF &p=QPointF(NAN,NAN));

 public:
  ObjectsScene(void);
  ~ObjectsScene(void);

  /* Métodos estáticos que definem o tamanho da grade e as opções de
     alinhamento de objetos   grade e exibição da própria grade. */
  static void setGridSize(unsigned size);
  static void setGridOptions(bool show_grd, bool align_objs_grd, bool show_page_dlm);
  static void getGridOptions(bool &show_grd, bool &align_objs_grd, bool &show_pag_dlm);

  /* Métodos de configuração das propriedades da página. Obs.: O usuário precisa chamar o método definirGrade
     para atualiza as configurações da página */
  static void setPageConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins);
  static void getPageConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins);

  void addItem(QGraphicsItem *item);
  void removeItem(QGraphicsItem *item);
  void setSceneRect(const QRectF &rect);

 public slots:
  //Alinha todos os objetos do modelo   grade
  void alignObjectsToGrid(void);
  void update(void);

 private slots:
  /* Estes slots tratam sinais os quais são emitidos individualmente por objetos
     da cena (ex.: OGTabela::s_objetoFilhoSelecionado ou OGRelacionamento::s_relacionamentoModificado ou
     s_objetoSelecionado)
     e estes sinais por sua vez são de certa forma encaminhados para classe superiores
     através do sinais s_menupopRequisitado, s_objetoModificado ou s_objetoSelecionado. Ex.: A classe ModeloWidget precisa
     tratar o sinal s_objetoFilhoSelecionado exibindo um menupop, somente com o auxilio dos métodos
     abaixo isso é possível */
  void emitObjectModification(BaseGraphicObject *object);
  void emitChildObjectSelection(TableObject *child_obj);
  void emitObjectSelection(BaseGraphicObject *object, bool selected);

 signals:
  /* Este sinal é emitdo sempre quando se inicia ou finaliza o movimento de objetos.
     o parâmetro 'fim_movimento' indica se o sinal foi emitido ou não no término
     do movimento */
  void s_objectsMoved(bool fim_movimento);

  //Emitido quando um objeto é modificado
  void s_objectModified(BaseGraphicObject *objeto);

  //Emitido quando um usuário pressiona botão direito do mouse na cena
  void s_popupMenuRequested(vector<BaseObject *>);

  //Emitido quando o usuário dá um duplo clique com esquerdo no objeto
  void s_objectDoubleClicked(BaseGraphicObject *objeto);

  /* Emitido quando um objeto é selecionado no modelo, este sinal pode ser usado
     quando se deseja obter o objeto selecionado no momento do clique, pois o sinal
     CenaObjetos::selectionChanged() não dá esta possibilidade. */
  void s_objectSelected(BaseGraphicObject *objeto, bool selecionado);

  friend class ModeloWidget;
};

#endif
