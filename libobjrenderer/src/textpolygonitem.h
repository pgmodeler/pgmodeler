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
\class TextPolygonItem
\brief This specialization of QGraphicsPolygonItem allows a text to be drawn over a polygonal element
*/

#ifndef TEXT_POLYGON_ITEM_H
#define TEXT_POLYGON_ITEM_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QFont>
#include <QStyleOptionGraphicsItem>

class TextPolygonItem : public QGraphicsPolygonItem {
	private:
		QGraphicsSimpleTextItem *text_item;

	public:
		TextPolygonItem(QGraphicsItem *parent = nullptr);
		virtual ~TextPolygonItem();

		//! \brief Defines the text displayed by the item
		void setText(const QString &text);

		//! \brief Defines the position of the text element (in local coordinate)
		void setTextPos(const QPointF &pos);
		void setTextPos(double x, double y);

		//! \brief Defines the pen used by the text element
		void setTextPen(const QPen &pen);

		//! \brief Defines the brush used by the text element
		void setTextBrush(const QBrush &brush);

		//! \brief Returns the bounding rect of the text item
		QRectF getTextBoundingRect();

		//! \brief Sets the font used by the text item
		void setFont(const QFont &fnt);

		QFont getFont();

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

		//! \brief Resizes to the specified dimension the passed polygon
		static void resizePolygon(QPolygonF &pol, double width, double height);
};

#endif
