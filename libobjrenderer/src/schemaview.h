/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class SchemaView
\brief Represents the schema in a graphical way on the object scene
*/

#ifndef SCHEMA_VIEW_H
#define SCHEMA_VIEW_H

#include "schema.h"
#include "databasemodel.h"
#include "baseobjectview.h"
#include "textboxview.h"
#include "roundedrectitem.h"

class LIBOBJECTRENDERER_EXPORT SchemaView: public BaseObjectView
{
	private:
		Q_OBJECT

		QGraphicsSimpleTextItem *sch_name;

    RoundedRectItem *box;

		QPointF last_pos;

		//! \brief Indicates if all children objects are selected
		bool all_selected;

		//! \brief Stores the views and tables that belongs to this schema
		QList<BaseObjectView *> children;

		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

		//! \brief Fills the children vector
		void fetchChildren(void);

		//! \brief Returns if all children are selected
		bool isChildrenSelected(void);

		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	public:
		SchemaView(Schema *schema);
		~SchemaView(void);

		//! \brief Visually selects all the schema children
		void selectChildren(void);
		unsigned getChildrenCount(void);
    QList<BaseObjectView *> getChildren(void);

	public slots:
		void configureObject(void);
};

#endif
