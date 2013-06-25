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
\class BaseObjectView
\brief Implements the basic routines to graphically represent the objects: table, relationship, textbox, view.
*/

#ifndef BASE_OBJECT_VIEW_H
#define BASE_OBJECT_VIEW_H

#include <QtWidgets>
#include "basegraphicobject.h"
#include "baserelationship.h"
#include "xmlparser.h"

class BaseObjectView: public QObject, public QGraphicsItemGroup {
	private:
		Q_OBJECT

	protected:
		/*! \brief Stores the global selection order of objects. This attributes
		 is incremented each time an object is selected. */
		static unsigned global_sel_order;

		/*! \brief Stores the selection order of the current object. This attribute is used to
		 know when an item was selected before another in the scene because the implementation of
		 the method QGraphicsScene :: selectedItem() the selected objects are returned without
		 any sort, but for the database model objects is the ESSENTIAL to know the selection order
		 mainly when creating relationships between tables. */
		unsigned sel_order;

		//! \brief Graphical text for the position info
		QGraphicsSimpleTextItem *pos_info_txt;

		//! \brief Graphical object (rectangle) of the position info
		QGraphicsPolygonItem *pos_info_pol;

		//! \brief Stores the objects bounding rect
		QRectF bounding_rect;

		//! \brief Graphical object that represents the object selection
		QGraphicsPolygonItem *obj_selection;

		//! \brief Icon that represent the object protection
		QGraphicsItemGroup *protected_icon;

		//! \brief Graphical object that represents the current object shadow
		QGraphicsPolygonItem *obj_shadow;

		//! \brief Stores the object font configuration
		static map<QString, QTextCharFormat> font_config;

		//! \brief Stores the object colors configuration
		static map<QString, QColor*> color_config;

		//! \brief Resizes to the specified dimension the passed polygon
		void resizePolygon(QPolygonF &pol, float width, float height);

		//! \brief Configures the objects shadow polygon
		void configureObjectShadow(void);

		//! \brief Configures the object selection polygon
		void configureObjectSelection(void);

		//! \brief Configures the polygons used to show the current object position
		void configurePositionInfo(QPointF pos);

		//! \brief Configures the icon that denotes the object's protection
		void configureProtectedIcon(void);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void setSelectionOrder(bool selected);

	public:
		static constexpr float VERT_SPACING=2.0f,
													 HORIZ_SPACING=2.0f,
													 DEFAULT_FONT_SIZE=9.0f,
													 OBJ_BORDER_WIDTH=0.80f;

		BaseObjectView(BaseObject *object=nullptr);
		~BaseObjectView(void);

		//! \brief Returns the object selection order
		unsigned getSelectionOrder(void);

		//! \brief Controls the changes during the object's selection and moving
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

		//! \brief Returns the object that is representend by the graphical object
		BaseObject *getSourceObject(void);

		//! \brief Loads the font / color styels for the objects from a XML configuration file
		static void loadObjectsStyle(void);

		//! \brief Returns the objects bounding rect in local coordination
		QRectF boundingRect(void) const;

		//! \brief Returns the fill style in a form of gradient for the specified element id
		static QLinearGradient getFillStyle(const QString &id);

		/*! \brief Returns fill style storing the colors on the specified parameters color1 and color2
		 for the specified element id */
		static void getFillStyle(const QString &id, QColor &color1, QColor &color2);

		//! \brief Returns the border style for the specified element id
		static QPen getBorderStyle(const QString &id);

		//! \brief Returns the font style for the specified element id
		static QTextCharFormat getFontStyle(const QString &id);

		//! \brief Sets the  font style for the specified element id
		static void setFontStyle(const QString &id, QTextCharFormat font_fmt);

		//! \brief Sets the color for the specified element id (used to set color for objects and font)
		static void setElementColor(const QString &id, QColor color, unsigned color_id);

		//! \brief Defines the object that the view represents
		void setSourceObject(BaseObject *object);

		//! \brief Pure virtual object (the derived classes must implement it)
		virtual void configureObject(void)=0;

		/*! \brief Returns the center point of the whole object.
		Note: this is not the same as calling boundingRect()->center(). Instead, this
		method calculates the center point based upon the current object's position */
		virtual QPointF getCenter(void);

	protected slots:
		//! \brief Make the basic object operations
		void __configureObject(void);

		//! \brief Toggles the protection icon
		void toggleProtectionIcon(bool value);

	signals:
		//! \brief Signal emmited when the object is (un)selected
		void s_objectSelected(BaseGraphicObject *object, bool selected);
};

#endif
