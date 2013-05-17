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
\class ObjectsScene
\brief Implements the basic operations to visualize and manipulate the graphical objects.
*/

#ifndef OBJECTS_SCENE_H
#define OBJECTS_SCENE_H

#include <QtWidgets>
#include <QPrinter>
#include "relationshipview.h"
#include "graphicalview.h"
#include "tableview.h"
#include "schemaview.h"

class ObjectsScene: public QGraphicsScene {
	private:
		Q_OBJECT

		//! \brief Object alignemnt, grid showing, page delimiter showing options
		static bool align_objs_grid, show_grid, show_page_delim;

		//! \brief Scene grid size
		static unsigned grid_size;

		//! \brief Paper size, used to segmentate the view (via page delimiters) and printing the model
		static QPrinter::PaperSize paper_size;

		//! \brief Page orientation (landscape / portrait)
		static QPrinter::Orientation page_orientation;

		//! \brief Page margins (applied to paper total size)
		static QRectF page_margins;

		//! \brief Indicates that there are objects being moved and the signal s_objectsMoved must be emitted
		bool moving_objs;

		//! \brief Initial point of selection rectangle
		QPointF sel_ini_pnt;

		//! \brief Rectangle used to select several objects on the scene
		QGraphicsPolygonItem *selection_rect;

		//! \brief Line used as a guide when inserting new relationship
		QGraphicsLineItem *rel_line;

		//! \brief Aligns the specified point in relation to the grid
		static QPointF alignPointToGrid(const QPointF &pnt);

	protected:
		//! \brief Brush used to draw the grid over the scene
		static QBrush grid;

		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

		//! \brief Draws a line from the point 'p_start' to the cursor position and simulates the relationship creation
		void showRelationshipLine(bool value, const QPointF &p_start=QPointF(NAN,NAN));

	public:
		ObjectsScene(void);
		~ObjectsScene(void);

		static void setGridSize(unsigned size);
		static void setGridOptions(bool show_grd, bool align_objs_grd, bool show_page_dlm);
		static void getGridOptions(bool &show_grd, bool &align_objs_grd, bool &show_pag_dlm);

		static void setPageConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins);
		static void getPageConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins);

		void addItem(QGraphicsItem *item);
		void removeItem(QGraphicsItem *item);
		void setSceneRect(const QRectF &rect);

	public slots:
		void alignObjectsToGrid(void);
		void update(void);

	private slots:
		//! \brief Handles and redirects the signal emitted by the modified object
		void emitObjectModification(BaseGraphicObject *object);

		//! \brief Handles and redirects the signal emitted by the selected child object
		void emitChildObjectSelection(TableObject *child_obj);

		//! \brief Handles and redirects the signal emitted by the selected object
		void emitObjectSelection(BaseGraphicObject *object, bool selected);

	signals:
		//! \brief Signal emitted when the user start or finalizes a object movement.
		void s_objectsMoved(bool end_moviment);

		//! \brief Signal emitted when a object is modified on scene
		void s_objectModified(BaseGraphicObject *objeto);

		//! \brief Signal emitted when the user right-click a specific object on the scene requesting the popup menu
		void s_popupMenuRequested(BaseObject *);

		//! \brief Signal emitted when the user right-click one or more objects on the scene requesting the popup menu
		void s_popupMenuRequested(void);

		//! \brief Signal emitted when the user double-click a object
		void s_objectDoubleClicked(BaseGraphicObject *objeto);

		//! \brief Signal emitted when a object is selected
		void s_objectSelected(BaseGraphicObject *objeto, bool selecionado);

		friend class ModelWidget;
};

#endif
