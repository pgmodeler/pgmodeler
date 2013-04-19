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
\class BaseTableView
\brief Base class shared by the classes TableView and GraphicalView
*/

#ifndef BASE_TABLE_VIEW_H
#define BASE_TABLE_VIEW_H

#include "baseobjectview.h"
#include "basetable.h"
#include "tabletitleview.h"
#include "tableobjectview.h"

class BaseTableView: public BaseObjectView {
	private:
		Q_OBJECT

	protected:
		//! \brief Item groups that stores columns and extended attributes, respectively
		QGraphicsItemGroup *columns,

		*ext_attribs;

		//! \brief Polygonal object that defines the table body
		QGraphicsPolygonItem *body,

		//! \brief Extended table attributes (indexes, rules, triggers) section body
		*ext_attribs_body;

		//! \brief Stores the reference to the child object currently selected on table
		TableObject *sel_child_obj;

		//! \brief Table title
		TableTitleView *title;

		//! \brief Stores the original table's tool tip
		QString table_tooltip;

		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	public:
		BaseTableView(BaseTable *base_tab);
		virtual ~BaseTableView(void);

		void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

	signals:
		//! \brief Signal emitted when a table is moved over the scene
		void s_objectMoved(void);

		//! \brief Signal emitted when the user right-click a focused table child object
		void s_childObjectSelected(TableObject *);
};

#endif
