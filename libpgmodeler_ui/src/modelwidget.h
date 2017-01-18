/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\ingroup libpgmodeler_ui
\class ModelWidget
\brief Implements the widget that permits the direct user interation over the database model.
*/

#ifndef MODEL_WIDGET_H
#define MODEL_WIDGET_H

#include <QtWidgets>
#include "databasemodel.h"
#include "operationlist.h"
#include "messagebox.h"
#include "objectsscene.h"
#include "taskprogresswidget.h"
#include "newobjectoverlaywidget.h"

class ModelWidget: public QWidget {
	private:
		Q_OBJECT

		XMLParser *xmlparser;

		NewObjectOverlayWidget *new_obj_overlay_wgt;

		//! \brief Current zoom aplied to the scene
		double current_zoom;

		//! \brief Indicates if the model was modified by some operation
		bool modified;

		/*! \brief Indicates if the cut operation is currently activated. This flag modifies
		the way the methods copyObjects() and removeObject() works. */
		static bool cut_operation;

		//! \brief Indicates if the last position and zoom must be saved/restored
		static bool save_restore_pos,

		//! \brief Indicates that graphical objects like table, view and textboxes can be created without click canvas (direclty from their editing form)
		simple_obj_creation,

		disable_render_smooth;

		/*! \brief Stores the model that generates the copy/cut operation. This model is updated
		from the destination model whenever a past/cut operation is done. */
		static ModelWidget *src_model;

		//! \brief Copied object on the source model
		static vector<BaseObject *> copied_objects;

		//! \brief Stores the cutted object on source model (only when executing cut command)
		static vector<BaseObject *> cutted_objects;

		//! \brief Frame that indicates if the model is protected
		QFrame *protected_model_frm;

		//! \brief Graphical objects scene
		ObjectsScene *scene;

		//! \brief Manages the objects scene
		QGraphicsView *viewport;

		//! \brief Model's general context menu
		QMenu popup_menu,

		//! \brief Stores the actions related to new objects creation
		new_object_menu,

		//! \brief Stores the quick actions
		quick_actions_menu,

		//! \brief Stores the schemas used by the "move to schema" operation
		schemas_menu,

		//! \brief Stores the role names used by the "change owner" operation
		owners_menu,

		//! \brief Stores the tags used by the "set tag" operation
		tags_menu,

		break_rel_menu,

		fade_menu,

		fade_in_menu,

		fade_out_menu;

		//! \brief Stores the selected object on the scene
		vector<BaseObject *> selected_objects;

		//! \brief Type of the object to be inserted on the model
		ObjectType new_obj_type;

		//! \brief Operation list that stores the modifications executed over the model
		OperationList *op_list;

		//! \brief Database model handle by the ModelWidget class. All operations are made over this attribute
		DatabaseModel *db_model;

		//! \brief Stores the loaded database model filename
		QString filename,

		//! \brief Stores the temporary database model filename
		tmp_filename;

		//! \brief This label shows the user the current applied zoom
		QLabel *zoom_info_lbl;

		//! \brief This timer controls the interval the zoom label is visible
		QTimer zoom_info_timer;

		//! \brief Creates a BaseForm instance and insert the widget into it. A custom configuration for dialog buttons can be passed
		int openEditingForm(QWidget *widget, unsigned button_conf = Messagebox::OK_CANCEL_BUTTONS);

		//! \brief Opens a editing form for objects at database level
		template<class Class, class WidgetClass>
		int openEditingForm(BaseObject *object);

		/*! \brief Opens a editing form for objects that can have a parent other than database, e.g., schema level objects,
				table child object or relationship attributes & constraints */
		template<class Class, class WidgetClass, class ParentClass>
		int openEditingForm(BaseObject *object, BaseObject *parent_obj);

		//! \brief Opens a editing form for objects that can have a position in the canvas area
		template<class Class, class WidgetClass, class ParentClass>
		int openEditingForm(BaseObject *object, BaseObject *parent_obj, const QPointF &pos);

		//! \brief Configures the popup menu according the the selected objects list
		void configurePopupMenu(vector<BaseObject *> objects=vector<BaseObject *>());

		//! \brief Configures the submenu related to the object
		void configureSubmenu(BaseObject *object);

		//! \brief Configures the submenu related to fade in/out operations
		void configureFadeMenu(void);

		void fadeObjects(QAction *action, bool fade_in);

	protected:
		static const unsigned BREAK_VERT_NINETY_DEGREES, //Break vertically the line in one 90° angle
		BREAK_HORIZ_NINETY_DEGREES, //Break horizontally the line in one 90° angle
		BREAK_VERT_2NINETY_DEGREES, //Break vertically the line in two 90° angles
		BREAK_HORIZ_2NINETY_DEGREES;//Break horizontally the line in two 90° angles

		QAction *action_source_code,
		*action_edit,
		*action_protect,
		*action_unprotect,
		*action_remove,
		*action_cascade_del,
		*action_select_all,
		*action_convert_relnn,
		*action_copy,
		*action_paste,
		*action_cut,
		*action_deps_refs,
		*action_new_object,
		*action_rename,
		*action_moveto_schema,
		*action_edit_perms,
		*action_change_owner,
		*action_quick_actions,
		*action_sel_sch_children,
		*action_highlight_object,
		*action_parent_rel,
		*action_append_sql,
		*action_create_seq_col,
		*action_conv_int_serial,
		*action_break_rel_line,
		*action_remove_rel_points,
		*action_set_tag,
		*action_disable_sql,
		*action_enable_sql,
		*action_duplicate,
		*action_fade,
		*action_fade_in,
		*action_fade_out;

		//! \brief Actions used to create new objects on the model
		map<ObjectType, QAction *> actions_new_objects;

		//! \brief Stores the relationship types menu
		QMenu *rels_menu;

		void resizeEvent(QResizeEvent *);
		void mousePressEvent(QMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void wheelEvent(QWheelEvent * event);

		//! \brief Captures and handles the QWeelEvent raised on the viewport scrollbars
		bool eventFilter(QObject *object, QEvent *event);

		//! \brief Cancel the creation of a new object (only for graphical objects)
		void cancelObjectAddition(void);

		//! \brief Disables the model actions when some new object action is active
		void enableModelActions(bool value);

		/*! \brief Reorganizes the schemas over the scene. The parameters are: an origin point,
		number of tables per row, schemas per row and a object spacing */
		void rearrangeSchemas(QPointF origin, unsigned tabs_per_row, unsigned sch_per_row, double obj_spacing);

		/*! \brief Reorganizes the tables of a specific schema over the scene. The parameter are:
		 the schema in which the tables will be rearranged, an origin point, number of tables per row
		 a object spacing */
		void rearrangeTables(Schema *schema, QPointF origin, unsigned tabs_per_row, double obj_spacing);

	public:
		static constexpr double MINIMUM_ZOOM=0.050000,
		MAXIMUM_ZOOM=4.000001,
		ZOOM_INCREMENT=0.050000;

		ModelWidget(QWidget *parent = 0);
		~ModelWidget(void);

		/*! \brief Configures the scene aligning the object to the grid and resizing the scene
		rect when some object is out of bound */
		void adjustSceneSize(void);

		//! \brief Set the model as modified forcing it to be redrawn
		void setModified(bool value);

		//! \brief Returns the loaded database model filename
		QString getFilename(void);

		//! \brief Returns the temporary (security copy) of the currently loaded model
		QString getTempFilename(void);

		//! \brief Shows the editing form according to the passed object type
		void showObjectForm(ObjectType obj_type, BaseObject *object=nullptr, BaseObject *parent_obj=nullptr, const QPointF &pos=QPointF(NAN, NAN));

		//! \brief Applies a zoom factor to the model
		void applyZoom(double zoom);

		//! \brief Returns the current zoom factor applied to the model
		double getCurrentZoom(void);

		//! \brief Returns if the model is modified or not
		bool isModified(void);

		//! \brief Returns the reference database model
		DatabaseModel *getDatabaseModel(void);

		//! \brief Returns the operation list used by database model
		OperationList *getOperationList(void);

		//! \brief Defines if any instance of ModelWidget must restore the last saved editing position on canvas
		static void setSaveLastCanvasPosition(bool value);

		//! \brief Defines if any instance of the class must disable rendering smoothness improving performance
		static void setRenderSmoothnessDisabled(bool value);

		//! \brief Defines if any instance of the class must simiplify the graphical object's creation
		static void setSimplifiedObjectCreation(bool value);

		//! \brief Restore the last editing position on canvas as well the zoom factor
		void restoreLastCanvasPosition(void);

		/*! \brief Save the last editing position on canvas as well the zoom factor. This method return true when
		the current values was saved on the database model */
		bool saveLastCanvasPosition(void);

		void setUpdatesEnabled(bool value);

		void updateRenderHints(void);

	private slots:
		//! \brief Handles the signals that indicates the object creation on the reference database model
		void handleObjectAddition(BaseObject *object);

		//! \brief Handles the signals that indicates the object removal on the reference database model
		void handleObjectRemoval(BaseObject *object);

		//! \brief Handles the signals that indicates the object moviment on the scene
		void handleObjectsMovement(bool end_moviment);

		//! \brief Handles the signals that indicates the object edition on the scene
		void handleObjectModification(BaseGraphicObject *object);

		//! \brief Handles the signals that indicates the object was double clicked on the scene
		void handleObjectDoubleClick(BaseGraphicObject *object);

		//! \brief Configures the popup menu specific for the passed object
		void configureObjectMenu(BaseObject *object);

		//! \brief Shows the configured popup menu
		void showObjectMenu(void);

		//! \brief Shows the widget containing the dependencies and references to the object
		void showDependenciesReferences(void);

		//! \brief Triggers the object editing
		void editObject(void);

		//! \brief Toggles the protection of the selected objects
		void protectObject(void);

		//! \brief Triggers the quick rename action
		void renameObject(void);

		//! \brief Move the selected object to a schema (selectable via menu)
		void moveToSchema(void);

		//! \brief Quickly changes the object's owner via popup menu
		void changeOwner(void);

		//! \brief Quickly sets the table's tag via popup menu
		void setTag(void);

		//! \brief Triggers the permission editing form
		void editPermissions(void);

		//! \brief Selects all the graphical objects under the selected schema
		void selectSchemaChildren(void);

		//! \brief Removes the selected objects
		void removeObjects(bool cascade = false);

		//! \brief Removes the selected objects in cascade model. This is the same as call removeObjects(true).
		void removeObjectsCascade(void);

		//! \brief Selects all the graphical objects on the scene
		void selectAllObjects(void);

		//! \brief Copies all the selected objects
		void copyObjects(bool duplicate_mode = false);

		//! \brief Paste all the objects copied previously
		void pasteObjects(void);

		//! \brief Duplicate the selected table object in its parent table
		void duplicateObject(void);

		//! \brief Cuts the selected objects. The effective removal is made when the cutted objects are pasted.
		void cutObjects(void);

		//! \brief Converts the Many to Many relationship generating a table and two additional relationships.
		void convertRelationshipNN(void);

		//! \brief Loads the selected object source code on the source code widget.
		void showSourceCode(void);

		//! \brief Adds a new object onto the selected model/table/schema
		void addNewObject(void);

		//! \brief Triggers the sql insertion widget for the current database model
		void editCustomSQL(void);

		//! \brief Configures the selected object vector whenever the selection changes on the scene
		void configureObjectSelection(void);

		/*! \brief Creates a sequence based upon the selected column. This method changes the default value
		for the column as well the type. */
		void createSequenceFromColumn(void);

		/*! \brief Creates a serial data type based upon the selected column data type. The prerequisite to create a serial data type
		is that the column's type is an integer one (smallint, int, bigint) and the default value is a function call
		to nextval('myseq'::regclass) */
		void convertIntegerToSerial(void);

		//! \brief Break the relationship line in one or two straight angles (see BREAK_??? constants)
		void breakRelationshipLine(void);

		//! \brief Removes any user added point from relationship
		void removeRelationshipPoints(void);

		//! \brief Highlights the object stored on the action that triggers the slot
		void highlightObject(void);

		void toggleNewObjectOverlay(void);

		void adjustOverlayPosition(void);

		void toggleObjectSQL(void);

		void fadeObjectsIn(void);

		void fadeObjectsOut(void);

	public slots:
		void loadModel(const QString &filename);
		void saveModel(const QString &filename);
		void saveModel(void);
		void printModel(QPrinter *printer, bool print_grid, bool print_page_nums);
		void update(void);

	signals:
		void s_objectModified(void);
		void s_objectsMoved(void);
		void s_objectCreated(void);
		void s_objectRemoved(void);
		void s_zoomModified(double);
		void s_modelResized(void);

		//! \brief Signal emitted whenever a object is created / edited using the form
		void s_objectManipulated(void);

		/*! \brief Signal emitted whenever the user open an object in its editing form but cancel the operation
		by closing the form */
		void s_manipulationCanceled(void);

		friend class MainWindow;
		friend class ModelExportForm;
		friend class OperationListWidget;
		friend class ModelObjectsWidget;
		friend class ModelOverviewWidget;
		friend class ModelValidationWidget;
		friend class DatabaseImportForm;
		friend class ObjectFinderWidget;
		friend class NewObjectOverlayWidget;
};

#endif
