/*
# PostgreSQL Database Modeler (pgModeler)
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

/**
\ingroup libobjrenderer
\class TextboxView
\brief Represents the textbox in a graphical way on the object scene
*/

#ifndef SCHEMA_VIEW_H
#define SCHEMA_VIEW_H

#include "schema.h"
#include "databasemodel.h"
#include "baseobjectview.h"

class SchemaView: public BaseObjectView
{
 private:
  Q_OBJECT
  QGraphicsSimpleTextItem *sch_name;
  QGraphicsPolygonItem *box;

  /// @details Stores the views and tables that belongs to this schema
  QList<BaseObjectView *> children;

  void mousePressEvent(QGraphicsSceneMouseEvent *event);

  /// @details Fills the children vector
  void fetchChildren(void);

 public:
  SchemaView(Schema *schema);

  /// @details Visually selects all the schema children
  void selectChildren(void);

 public slots:
  void configureObject(void);
};

#endif
