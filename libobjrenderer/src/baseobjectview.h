/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "roundedrectitem.h"
#include "textpolygonitem.h"

class BaseObjectView: public QObject, public QGraphicsItemGroup {
	private:
		Q_OBJECT

	protected:
		/*! \brief Indicates if the placeholder object must be used when moving objects.
		Place holder objects when enabled causes a significant performance gain mainly when
		moving tables linked to relationships because the relationships will be updated only
		when the table moviment ends and not during it */
		static bool use_placeholder;

		/*! \brief Stores the global selection order of objects. This attributes
		 is incremented each time an object is selected. */
		static unsigned global_sel_order;

		/*! \brief Stores the current status for object's compact view display.
		 * In compact view the alias of object's are shown instead of name. Also, data types and constraint types are hidden,
		 * as well the table's extended area is collapsed by default */
		static bool compact_view;

		/*! \brief Stores the selection order of the current object. This attribute is used to
		 know when an item was selected before another in the scene because the implementation of
		 the method QGraphicsScene :: selectedItem() the selected objects are returned without
		 any sort, but for the database model objects is the ESSENTIAL to know the selection order
		 mainly when creating relationships between tables. */
		unsigned sel_order;

		//! \brief This item display the current object position on the scene
		TextPolygonItem *pos_info_item;

		//! \brief Stores the objects bounding rect
		QRectF bounding_rect;

		//! \brief Graphical object that represents the object selection
		QGraphicsItem *obj_selection;

		//! \brief Icon that represent the object protection
		QGraphicsItemGroup *protected_icon;

		//! \brief Graphical object that represents the current object shadow
		QGraphicsItem *obj_shadow;

		//! \brief Graphical object that represents the placeholder when the object is being moved
		RoundedRectItem *placeholder;

		//! \brief Graphical object of the sql disabled info
		QGraphicsRectItem *sql_disabled_box;

		//! \brief Graphical text for the sql disabled info
		QGraphicsSimpleTextItem *sql_disabled_txt;

		//! \brief This items is used to display the sql disabled status of the object
		TextPolygonItem *sql_disabled_item;

		//! \brief Stores the object font configuration
		static map<QString, QTextCharFormat> font_config;

		//! \brief Stores the object colors configuration
		static map<QString, vector<QColor>> color_config;

		//! \brief Configures the objects shadow polygon
		void configureObjectShadow(void) {}

		//! \brief Configures the object selection polygon
		void configureObjectSelection();

		//! \brief Configures the polygons used to show the current object position
		void configurePositionInfo(QPointF pos);

		//! \brief Configures the rectangle used to show the sql disabled status
		void configureSQLDisabledInfo();

		//! \brief Configures the icon that denotes the object's protection
		void configureProtectedIcon();

		void configurePlaceholder();

		void mousePressEvent(QGraphicsSceneMouseEvent *event);

		void setSelectionOrder(bool selected);

		//! \brief Defines in which layer the object is visible
		void setLayer(unsigned layer);

	public:
		static constexpr double VertSpacing=2.0,
		HorizSpacing=2.0,
		DefaultFontSize=9.0,
		ObjectBorderWidth=0.85;

		static constexpr int ObjectAlphaChannel=128;

		BaseObjectView(BaseObject *object=nullptr);
		virtual ~BaseObjectView();

		//! \brief Returns the object selection order
		unsigned getSelectionOrder();

		//! \brief Controls the changes during the object's selection and moving
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

		//! \brief Returns the object that is representend by the graphical object
		BaseObject *getUnderlyingObject();

		//! \brief Loads the font / color styels for the objects from a XML configuration file
		static void loadObjectsStyle();

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

		static void setPlaceholderEnabled(bool value);

		static bool isPlaceholderEnabled();

		static void setCompactViewEnabled(bool value);

		static bool isCompactViewEnabled();

		//! \brief Sets the  font style for the specified element id
		static void setFontStyle(const QString &id, QTextCharFormat font_fmt);

		//! \brief Sets the color for the specified element id (used to set color for objects and font)
		static void setElementColor(const QString &id, QColor color, unsigned color_id);

		//! \brief Returns the color for the specified element id (used to get color for objects and font)
		static QColor getElementColor(const QString &id, unsigned color_id);

		//! \brief Defines the object that the view represents
		void setSourceObject(BaseObject *object);

		//! \brief Pure virtual object (the derived classes must implement it)
		virtual void configureObject(void)=0;

		/*! \brief Returns the center point of the whole object.
		Note: this is not the same as calling boundingRect()->center(). Instead, this
		method calculates the center point based upon the current object's position */
		virtual QPointF getCenter();

		//! \brief Toggles the wireframe display
		virtual void togglePlaceholder(bool visible);

		/*! \brief Returns the current font DPI factor of the screen. This factor is used to resize
		 * objects according to the screen's resolution/font dpi */
		static double getScreenDpiFactor();

		//! \brief Returns the current factor between the default font size and the current defined one
		static double getFontFactor();

		//! \brief Returns the layer in which the object is visible
		unsigned getLayer();

	protected slots:
		//! \brief Make the basic object operations
		void __configureObject();

		//! \brief Toggles the protection icon
		void toggleProtectionIcon(bool value);

	signals:
		//! \brief Signal emmited when the object is (un)selected
		void s_objectSelected(BaseGraphicObject *object, bool selected);

		//! \brief Signal emmited whenever the width or height of the table changes
		void s_objectDimensionChanged();

	friend class ObjectsScene;
};

#endif
