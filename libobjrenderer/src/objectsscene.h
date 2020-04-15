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

		//! \brief Holds the names of the layers on the scene used to separate in the objects on the canvas
		QStringList layers, active_layers;

		vector<BaseObjectView *> removed_objs;

		//! \brief Holds the tables/views which have selected children objects
		QList<BaseTableView *> tabs_sel_children;

		//! \brief Indicates if the corner move is enabled for the scene
		static bool corner_move,

		/*! \brief Indicates that panning mode and range selection model are activate in inverse mode.
		By default panning model is activated with a single left-click and range selection with SHIFT + left-click */
		invert_rangesel_trigger;

		//! \brief Indicates if the scene need to be moved
		bool move_scene;

		static constexpr int SceneMoveStep=20,
		SceneMoveTimeout=50,
		SceneMoveThreshold=30;

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

		/*! \brief Indicates if the mouse cursor is under a move spot portion of scene.
		Additionally this method configures the direction of movement when returning true */
		bool mouseIsAtCorner();

		QGraphicsView *getActiveViewport();

		//! \brief Performs the final steps when moving the objects like adjusting position to grid, moving children object, etc
		void finishObjectsMove(const QPointF &pnt_end);

		/*! \brief Performs the scene position adjustment when pressing/releasing the arrow keys to avoid objects to be hidden in the corners.
		This method will adjust the scene size automatically when moving objects right or down if the selected items bounding rects
		exceeds the scene's size limits */
		void adjustScenePositionOnKeyEvent(int key);

		//! \brief Formats the name of the layer removing any invalid chars and doing the desambiguation in case the name already exists
		QString formatLayerName(const QString &name);

		void clearTablesChildrenSelection();

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
		void showRelationshipLine(bool value, const QPointF &p_start=QPointF(DNaN,DNaN));

		void blockItemsSignals(bool block);

	public:
		static constexpr unsigned DefaultLayer = 0,
		InvalidLayer = UINT_MAX;

		ObjectsScene();
		virtual ~ObjectsScene();

		/*! \brief Add a new layer to the scene. In case of duplicated name this method
		 * automatically does the desambiguation. The name of the new layer is returned. */
		QString addLayer(const QString &name);

		/*! \brief Rename the layer of the provided index. In case of duplicated name this method
		 * 	automatically does the desambiguation. */
		QString renameLayer(unsigned idx, const QString &name);

		//! \brief Remove the named layer. Objects in the destroyed layer are automatically moved to the default one
		void removeLayer(const QString &name);

		//! \brief Destroy all layers (except the default one) moving all objects from the destroyed layers to the default one
		void removeLayers();

		//! \brief Set the named layers as active. Activating a layer causes objects attached to it to be visible
		void setActiveLayers(QStringList act_layers);

		//! \brief Set the layers with the provided indexes as active. Activating a layer causes objects attached to it to be visible
		void setActiveLayers(QList<unsigned> ids);

		/*! \brief Move the objects from a layer to another. This method automatically hides/show the objects in the new layer
		 * according to the activation status of the destination layer */
		void moveObjectsToLayer(unsigned old_layer, unsigned new_layer);

		//! \brief Returns true when the named layer is currenctly activated
		bool isLayerActive(const QString &name);

		//! \brief Returns true when the layer with the provided id is currenctly activated
		bool isLayerActive(unsigned layer_id);

		//! \brief Returns a list containing the names of the active layers
		QStringList getActiveLayers();

		//! \brief Returns a list containing the ids of the active layers
		QList<unsigned> getActiveLayersIds();

		//! \brief Returns a list containing the names of all layers in the scene
		QStringList getLayers();

		//! \brief Returns the id of the named layer. If the layer does not exist the constant ObjectsScene::InvalidLayer is returned
		unsigned getLayerId(const QString &name);

		//! \brief This method causes objects in the active layers to have their visibility state updated.
		void updateActiveLayers();

		static void setEnableCornerMove(bool enable);
		static void setInvertRangeSelectionTrigger(bool invert);
		static bool isCornerMoveEnabled();

		static void setGridSize(unsigned size);
		static void setGridOptions(bool show_grd, bool align_objs_grd, bool show_page_dlm);

		static bool isAlignObjectsToGrid();
		static bool isShowGrid();
		static bool isShowPageDelimiters();

		static void setPaperConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins, QSizeF custom_size=QSizeF(0,0));
		static void getPaperConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins, QSizeF &custom_size);

		static void configurePrinter(QPrinter *printer);
		static void configurePrinter(QPrinter *printer, const QSizeF &custom_size, QPrinter::Orientation orient);

		void addItem(QGraphicsItem *item);
		void removeItem(QGraphicsItem *item);
		void setSceneRect(const QRectF &rect);

		//! \brief Aligns the specified point in relation to the grid
		static QPointF alignPointToGrid(const QPointF &pnt);

		/*! \brief Returns the items bounding rect. By default the method returns the same as QGraphicsScene::itemsBoundingRect.
		If the parameter seek_only_db_objs is true the returned rect will have the origin point calculated based upon the
		visible objects that inherits BaseObjectView and are database model objects (tables, views, textboxes, schemas and relationships).

		If the paramenter selected_only is true only selected objects will have the bounding rect calculated.
		Currently this parameter is ignored when using seek_only_db_objs = false

		Note: using this method with seek_only_db_objs=true can be time expensive depending on the size of the model so use it wisely. */
		QRectF itemsBoundingRect(bool seek_only_db_objs=false, bool selected_only = false);

		//! \brief Returns a vector containing all the page rects.
		vector<QRectF> getPagesForPrinting(const QSizeF &paper_size, const QSizeF &margin, unsigned &h_page_cnt, unsigned &v_page_cnt);

		bool isRangeSelectionEnabled();
		bool isRangeSelectionTriggerInverted();
		bool isRelationshipLineVisible();
		bool isMovingObjects();

		QList<QGraphicsItem *> selectedItems(void) const;
		bool hasOnlyTableChildrenSelection(void) const;

	public slots:
		void alignObjectsToGrid();
		void update();
		void clearSelection();

		//! \brief Toggles the object range selection
		void enableRangeSelection(bool value);

	private slots:
		/*! \brief Start/stop the timer responsible to move the scene. This method is called with true param
		whenever the user stay with the cursor at corner in a certain amount of time */
		void enableSceneMove(bool value=true);

		//! \brief Moves the scene when the user puts the mouse cursor on one of scene's edges
		void moveObjectScene();

		//! \brief Handles and redirects the signal emitted by the selected child object
		void handlePopupMenuRequested(TableObject *child_obj);

		//! \brief Handles and redirects the signal emitted by the selected object
		void handleObjectSelection(BaseGraphicObject *object, bool selected);

		//! \brief Handles the tables children objects selection changes
		void handleChildrenSelectionChanged();

	signals:
		//! \brief Signal emitted when the user start or finalizes a object movement.
		void s_objectsMoved(bool end_moviment);

		//! \brief Signal emitted when a object is modified on scene
		void s_objectModified(BaseGraphicObject *objeto);

		//! \brief Signal emitted when the user toggles a table's collapse mode in the scene
		void s_collapseModeChanged();

		//! \brief Signal emitted when the user toggles a table's attributes pagination
		void s_paginationToggled();

		//! \brief Signal emitted when the user changes a table's attributes page
		void s_currentPageChanged();

		//! \brief Signal emitted when the user right-click a specific object on the scene requesting the popup menu
		void s_popupMenuRequested(BaseObject *);

		//! \brief Signal emitted when the user right-click one or more objects on the scene requesting the popup menu
		void s_popupMenuRequested();

		//! \brief Signal emitted when the user double-click a object
		void s_objectDoubleClicked(BaseGraphicObject *objeto);

		//! \brief Signal emitted when a object is selected
		void s_objectSelected(BaseGraphicObject *objeto, bool selecionado);

		//! \brief Signal emitted when objects are selected via range selection
		void s_objectsSelectedInRange();

		//! \brief Signal emitted when a blank area of the canvas is pressed
		void s_objectsScenePressed(Qt::MouseButtons);

		//! \brief Signal emitted when the active layers change
		void s_activeLayersChanged();

		//! \brief Signal emitted when the layers change (add, remove, rename)
		void s_layersChanged();

		//! \brief Signal emitted when objects are moved from a layer to another
		void s_objectsMovedLayer();

		//! \brief Signal emitted when tables children objects have their selection statuses changed
		void s_childrenSelectionChanged();

		friend class ModelWidget;
};

#endif
