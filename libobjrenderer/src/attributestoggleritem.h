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

		QGraphicsRectItem *sel_rect;

		CollapseMode collapse_mode;

		QGraphicsPolygonItem *arrows[5];

		bool arrows_selected[5],

		has_ext_attribs,

		pagination_enabled;

		double arrows_width, arrows_height;

		unsigned current_page,

		max_pages;

		static constexpr unsigned AttribsExpandArrow=0,
		AttribsCollapseArrow=1,
		NextAttribsArrow=2,
		PrevAttribsArrow=3,
		PaginationTogglerBtn=4;

		void configureButtons(const QRectF &rect);
		void configureButtonsState(void);

	public:
		AttributesTogglerItem(QGraphicsItem *parent = nullptr);
		~AttributesTogglerItem(void);

		void setArrowsBrush(const QBrush &brush);
		void setArrowsPen(const QPen &pen);
		void setRect(const QRectF &rect);
		void setCollapseMode(CollapseMode coll_mode);
		void setArrowSelected(const QPointF &pnt, bool clicked = false);
		void setHasExtAttributes(bool value);
		void setPaginationEnabled(bool value, bool hide_pag_toggler = false);
		void setPaginationValues(unsigned curr_page, unsigned max_page);
		void clearArrowSelection(void);
		double getArrowsWidth(void);

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	signals:
		void s_collapseModeChanged(CollapseMode);
		void s_currentPageChanged(unsigned);
		void s_paginationToggled(bool);
};

#endif
