/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "styledtextboxview.h"

class ObjectsScene: public QGraphicsScene {
	private:
		Q_OBJECT

		vector<BaseObjectView *> removed_objs;

		//! \brief Indicates if the corner move is enabled for the scene
		static bool corner_move,

		/*! \brief Indicates that panning mode and range selection model are activate in inverse mode.
		By default panning model is activated with a single left-click and range selection with SHIFT + left-click */
		invert_rangesel_trigger;

		//! \brief Indicates if the scene need to be moved
		bool move_scene;

		static const int SCENE_MOVE_STEP=20,
		SCENE_MOVE_TIMEOUT=50,
		SCENE_MOVE_THRESHOLD=30;

		//! \brief Timer responsible to move the scene
		QTimer scene_move_timer,

		/*! \brief Timer responsible to check if the user puts cursor at corners for a certain amount of time.
		When this timeout the scene_move_timer will be triggered and the scene will be moved */
		corner_hover_timer,

		/*! brief Timer responsible to control the interval between key presses/releases when moving objects with the keyboard.
		This timer is started whenever the user releases the arrow keys. If the timer emits timeout() signal
		indicates that the user take more than 500ms to press and release the arrow keys again, this indicates to the scene
		to emit the signal s_objectsMove(true) to alert the end of the objects moving. If the user presses/releases the keys when
		the timer is still running the same will be restarted, until its timeout is reached. This trick avoids 'spamming' the
		operation list in ModelWidget creating registries only when the user starts and ends the objects movement. Intermediate
		key presses/releases aren't not registered in the operation history */
		object_move_timer;

		//! \brief Attributes used to control the direction of scene movement when user puts cursor at corners
		int scene_move_dx, scene_move_dy;

		//! \brief Object alignemnt, grid showing, page delimiter showing options
		static bool align_objs_grid, show_grid, show_page_delim;

		//! \brief Scene grid size
		static unsigned grid_size;

		//! \brief Paper size, used to segmentate the view (via page delimiters) and printing the model
		static QPrinter::PaperSize paper_size;

		//! \brief Used to store the custom paper size. This attribute is used only when paper_size=QPrinter::Custom
		static QSizeF custom_paper_size;

		//! \brief Page orientation (landscape / portrait)
		static QPrinter::Orientation page_orientation;

		//! \brief Page margins (applied to paper total size)
		static QRectF page_margins;

		//! \brief Indicates that there are objects being moved and the signal s_objectsMoved must be emitted
		bool moving_objs,

		//! \brief Indicates if the range selection (selection using a rectangle drawn on the canvas)
		enable_range_sel;

		//! \brief Initial point of selection rectangle
		QPointF sel_ini_pnt;

		//! \brief Rectangle used to select several objects on the scene
		QGraphicsPolygonItem *selection_rect;

		//! \brief Line used as a guide when inserting new relationship
		QGraphicsLineItem *rel_line;

		//! \brief Aligns the specified point in relation to the grid
		static QPointF alignPointToGrid(const QPointF &pnt);

		/*! \brief Indicates if the mouse cursor is under a move spot portion of scene.
		Additionally this method configures the direction of movement when returning true */
		bool mouseIsAtCorner(void);

		QGraphicsView *getActiveViewport(void);

		//! brief Performs the final steps when moving the objects like adjusting position to grid, moving children object, etc
		void finishObjectsMove(const QPointF &pnt_end);

		/*! brief Performs the scene position adjustment when pressing/releasing the arrow keys to avoid objects to be hidden in the corners.
		This method will adjust the scene size automatically when moving objects right or down if the selected items bounding rects
		exceeds the scene's size limits */
		void adjustScenePositionOnKeyEvent(int key);

	protected:
		//! \brief Brush used to draw the grid over the scene
		static QBrush grid;

		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

		//! \brief Draws a line from the point 'p_start' to the cursor position and simulates the relationship creation
		void showRelationshipLine(bool value, const QPointF &p_start=QPointF(NAN,NAN));

		void blockItemsSignals(bool block);

	public:
		ObjectsScene(void);
		~ObjectsScene(void);

		static void setEnableCornerMove(bool enable);
		static void setInvertRangeSelectionTrigger(bool invert);
		static bool isCornerMoveEnabled(void);

		static void setGridSize(unsigned size);
		static void setGridOptions(bool show_grd, bool align_objs_grd, bool show_page_dlm);
		static void getGridOptions(bool &show_grd, bool &align_objs_grd, bool &show_pag_dlm);

		static void setPaperConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins, QSizeF custom_size=QSizeF(0,0));
		static void getPaperConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins, QSizeF &custom_size);

		static void configurePrinter(QPrinter *printer);
		static void configurePrinter(QPrinter *printer, const QSizeF &custom_size, QPrinter::Orientation orient);

		void addItem(QGraphicsItem *item);
		void removeItem(QGraphicsItem *item);
		void setSceneRect(const QRectF &rect);

		/*! \brief Returns the items bounding rect. By default the method returns the same as QGraphicsScene::itemsBoundingRect.
		If the parameter seek_only_db_objs is true the returned rect will have the origin point calculated based upon the
		visible objects that inherits BaseObjectView and are database model objects (tables, views, textboxes, schemas and relationships).

		If the paramenter selected_only is true only selected objects will have the bounding rect calculated.
		Currently this parameter is ignored when using seek_only_db_objs = false

		Note: using this method with seek_only_db_objs=true can be time expensive depending on the size of the model so use it wisely. */
		QRectF itemsBoundingRect(bool seek_only_db_objs=false, bool selected_only = false);

		//! \brief Returns a vector containing all the page rects.
		vector<QRectF> getPagesForPrinting(const QSizeF &paper_size, const QSizeF &margin, unsigned &h_page_cnt, unsigned &v_page_cnt);

		bool isRangeSelectionEnabled(void);
		bool isRangeSelectionTriggerInverted(void);
		bool isRelationshipLineVisible(void);
		bool isMovingObjects(void);

	public slots:
		void alignObjectsToGrid(void);
		void update(void);

		//! \brief Toggles the object range selection
		void enableRangeSelection(bool value);

	private slots:
		/*! \brief Start/stop the timer responsible to move the scene. This method is called with true param
		whenever the user stay with the cursor at corner in a certain amount of time */
		void enableSceneMove(bool value=true);

		//! \brief Moves the scene when the user puts the mouse cursor on one of scene's edges
		void moveObjectScene(void);

		//! \brief Handles and redirects the signal emitted by the modified object
		void emitObjectModification(BaseGraphicObject *object);

		//! \brief Handles and redirects the signal emitted by the selected child object
		void emitChildObjectSelection(TableObject *child_obj);

		//! \brief Handles and redirects the signal emitted by the selected object
		void emitObjectSelection(BaseGraphicObject *object, bool selected);

		//! \brief Handles and redirects the signal emitted by the tables/views when the extended attributes are toggled
		void emitExtAttributesToggled(void);

	signals:
		//! \brief Signal emitted when the user start or finalizes a object movement.
		void s_objectsMoved(bool end_moviment);

		//! \brief Signal emitted when a object is modified on scene
		void s_objectModified(BaseGraphicObject *objeto);

		//! \brief Signal emitted when the user toggles a table's extended attributes in the scene
		void s_extAttributesToggled(void);

		//! \brief Signal emitted when the user right-click a specific object on the scene requesting the popup menu
		void s_popupMenuRequested(BaseObject *);

		//! \brief Signal emitted when the user right-click one or more objects on the scene requesting the popup menu
		void s_popupMenuRequested(void);

		//! \brief Signal emitted when the user double-click a object
		void s_objectDoubleClicked(BaseGraphicObject *objeto);

		//! \brief Signal emitted when a object is selected
		void s_objectSelected(BaseGraphicObject *objeto, bool selecionado);

		//! \brief Signal emitted when objects are selected via range selection
		void s_objectsSelectedInRange(void);

		//! \brief Signal emtted when a blank area of the canvas is pressed
		void s_objectsScenePressed(Qt::MouseButtons);

		friend class ModelWidget;
};

#endif
