/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Graphical objects renderer (libobjrenderer)
# Class: RelationshipView
# Description: Implements the graphical representation for relationships
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
#ifndef RELATIONSHIP_VIEW_H
#define RELATIONSHIP_VIEW_H

#include "textboxview.h"
#include "basetableview.h"
#include "relationship.h"

class RelationshipView: public BaseObjectView {
 private:
  Q_OBJECT

  //Graphical point radius
  static const float GRAPHIC_PNT_RADIUS=2.5f;

  /* Indicate that the line is being configured/updated. This flag is used to evict
     that the configureLine() method is exceedingly called during the table moving. */
  bool configuring_line;

  //Stores the graphical representation for labels
  TextboxView *labels[3];

  //Stores the graphical representation for the participant tables
  BaseTableView *tables[2];

  //Graphical representation for the user added points
  vector<QGraphicsPolygonItem *> graph_points;

  //Lines that represent the relationship
  vector<QGraphicsLineItem *> lines;

  //Stores the graphical representation for relationship attributes
  vector<QGraphicsItemGroup *> attributes;

  //Relationship descriptor (lozenge -> (1,n)-(1,n) relationship, triangle -> inheritance)
  QGraphicsPolygonItem *descriptor;

  //Stores the current hovered child object
  QGraphicsItem *sel_object;

  //Stores the initial (default) labels position
  QPointF labels_ini_pos[3];

  //Stores the selected child object index
  int sel_object_idx;

  //Configures the labels positioning
  void configureLabels(void);

  //Configures the descriptor form and positioning
  void configureDescriptor(void);

  //Configures the attributes positioning
  void configureAttributes(void);

  //Configures the position info object
  void configurePositionInfo(void);

 protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}

 public slots:
  //Configures the relationship line
  void configureLine(void);

  //Returns the label through its index
  TextboxView *getLabel(unsigned lab_idx);

 private slots:
  //Makes the comple relationship configuration
  void configureObject(void);

 public:
  RelationshipView(BaseRelationship *rel);
  ~RelationshipView(void);

  //Calculates the relationship bounding rect considering all the children objects dimension
  QRectF __boundingRect(void);

  //Returns the relationship that generates the graphical representation
  BaseRelationship *getSourceObject(void);

  //Disconnects the signal handled by the relationship which senders are the tables
  void disconnectTables(void);

 signals:
  void s_relationshipModified(BaseGraphicObject *rel);
};

#endif
