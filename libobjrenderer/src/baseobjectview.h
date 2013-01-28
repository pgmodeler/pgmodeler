/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Graphical objects renderer (libobjrenderer)
# Class: BaseObjectView
# Description: Implements the basic routines to graphically represent the objects: table, relationship, textbox, view
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
#ifndef BASE_OBJECT_VIEW_H
#define BASE_OBJECT_VIEW_H

#include <QtGui>
#include "basegraphicobject.h"
#include "baserelationship.h"
#include "xmlparser.h"

class BaseObjectView: public QObject, public QGraphicsItemGroup {
 private:
  Q_OBJECT

 protected:
  /** Stores the global selection order of objects. This attributes
     is incremented each time an object is selected. */
  static unsigned global_sel_order;

  /** Stores the selection order of the current object. This attribute is used to
     know when an item was selected before another in the scene because the implementation of
     the method QGraphicsScene :: selectedItem() the selected objects are returned without
     any sort, but for the database model objects is the ESSENTIAL to know the selection order
     mainly when creating relationships between tables. */
  unsigned sel_order;

  /// Graphical text for the position info
  QGraphicsSimpleTextItem *pos_info_txt;

  /// Graphical object (rectangle) of the position info
  QGraphicsPolygonItem *pos_info_pol;

  /// Stores the objects bounding rect
  QRectF bounding_rect;

  /// Graphical object that represents the object selection
  QGraphicsPolygonItem *obj_selection;

  /// Icon that represent the object protection
  QGraphicsItemGroup *protected_icon;

  /// Graphical object that represents the current object shadow
  QGraphicsPolygonItem *obj_shadow;

  /// Stores the object font configuration
  static map<QString, QTextCharFormat> font_config;

  /// Stores the object colors configuration
  static map<QString, QColor*> color_config;

  /// Resizes to the specified dimension the passed polygon
  void resizePolygon(QPolygonF &pol, float width, float height);

  /// Configures the objects shadow polygon
  void configureObjectShadow(void);

  /// Configures the object selection polygon
  void configureObjectSelection(void);

  /// Configures the polygons used to show the current object position
  void configurePositionInfo(QPointF pos);

  /// Configures the icon that denotes the object's protection
  void configureProtectedIcon(void);

 public:
  static const float VERT_SPACING=2.0f,
                     HORIZ_SPACING=2.0f,
                     DEFAULT_FONT_SIZE=9.0f,
                     OBJ_BORDER_WIDTH=0.80f;

  BaseObjectView(BaseObject *object=NULL);
  ~BaseObjectView(void);

  /// Returns the object selection order
  unsigned getSelectionOrder(void);

  /// Controls the changes during the object's selection and moving
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  /// Returns the object that is representend by the graphical object
  BaseObject *getSourceObject(void);

  /// Loads the font / color styels for the objects from a XML configuration file
  static void loadObjectsStyle(void);

  /// Returns the objects bounding rect in local coordination
  QRectF boundingRect(void) const;

  /// Returns the fill style in a form of gradient for the specified element id
  static QLinearGradient getFillStyle(const QString &id);

  /** Returns fill style storing the colors on the specified parameters color1 and color2
     for the specified element id */
  static void getFillStyle(const QString &id, QColor &color1, QColor &color2);

  /// Returns the border style for the specified element id
  static QPen getBorderStyle(const QString &id);

  /// Returns the font style for the specified element id
  static QTextCharFormat getFontStyle(const QString &id);

  /// Sets the  font style for the specified element id
  static void setFontStyle(const QString &id, QTextCharFormat font_fmt);

  /// Sets the color for the specified element id (used to set color for objects and font)
  static void setElementColor(const QString &id, QColor color, unsigned color_id);

  /// Defines the object that the view represents
  void setSourceObject(BaseObject *object);

  /// Pure virtual object (the derived classes must implement it)
  virtual void configureObject(void)=0;

 protected slots:
  /// Make the basic object operations
  void __configureObject(void);

  /// Toggles the protection icon
  void toggleProtectionIcon(bool value);

 signals:
  /// Signal emmited when the object is (un)selected
  void s_objectSelected(BaseGraphicObject *object, bool selected);
};

#endif
