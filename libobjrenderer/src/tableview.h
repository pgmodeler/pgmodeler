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
\class TableView
\brief Implements the graphical representation for tables.
*/

#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include "table.h"
#include "basetableview.h"
#include "tabletitleview.h"
#include "tableobjectview.h"

class TableView: public BaseTableView {
	private:
		Q_OBJECT

		//! \brief Extended table attributes (indexes, rules, triggers) section body
		QGraphicsPolygonItem  *ext_attribs_body;

		//! \brief Item groups that stores columns and extended attributes, respectively
		QGraphicsItemGroup *columns, *ext_attribs;

		//! \brief Stores the reference to the child object currently selected on table
		TableObject *sel_child_obj;

		void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	public:
		TableView(Table *table);
		~TableView(void);

	private slots:
		void configureObject(void);

	signals:
		//! \brief Signal emitted when the user right-click a focused table child object
		void s_childObjectSelected(TableObject *);
};

#endif
