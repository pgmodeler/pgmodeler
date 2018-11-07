/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class AttributesTogglerItem
\brief Implements a basic table's attributes toggler used by BaseTableView.
*/

#ifndef ATTRIBUTES_TOGGLER_ITEM_H
#define ATTRIBUTES_TOGGLER_ITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include "roundedrectitem.h"
#include "basetable.h"

class AttributesTogglerItem: public QObject, public RoundedRectItem {
	private:
		Q_OBJECT

		CollapseMode collapse_mode;

		QGraphicsPolygonItem *arrows[3];

		bool arrows_selected[3];

		bool toggler_inverted;

		static constexpr unsigned AttribsToggler=0,
		NextAttribsArrow=1,
		PrevAttribsArrow=2;

		void configureButtons(const QRectF &rect);

	public:
		AttributesTogglerItem(QGraphicsItem *parent = nullptr);

		void setArrowsBrush(const QBrush &brush);
		void setArrowsPen(const QPen &pen);
		void setRect(const QRectF &rect);
		void setCollapseMode(CollapseMode coll_mode);
		void setArrowSelected(const QPointF &pnt, bool clicked = false);
		void clearArrowSelection(void);

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	signals:
		void s_collapseModeChanged(CollapseMode);
};

#endif
