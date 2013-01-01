/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Graphical objects renderer (libobjrenderer)
# Class: TextboxView
# Description: Represents the textbox in a graphical way on the object scene
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
#ifndef TEXTBOX_VIEW_H
#define TEXTBOX_VIEW_H

#include "textbox.h"
#include "baseobjectview.h"

class TextboxView: public BaseObjectView {
 private:
  Q_OBJECT

  //Graphical item that represent the box
  QGraphicsPolygonItem *box;

  //Graphical item that represent the text
  QGraphicsSimpleTextItem *text;

 public:
  TextboxView(Textbox *txtbox, const QBrush &fill_style=Qt::NoBrush, const QPen &border_style=Qt::NoPen);
  ~TextboxView(void);

 protected slots:
  void configureObject(void);
};

#endif
