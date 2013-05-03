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
\ingroup libpgmodeler_ui
\class ModelWidget
\brief Implements the widget that permits the direct user interation over the database model.
*/

#ifndef MODEL_WIDGET_H
#define MODEL_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "operationlist.h"
#include "messagebox.h"
#include "baseform.h"
#include "objectsscene.h"
#include "taskprogresswidget.h"

class ModelWidget: public QWidget {
	private:
		Q_OBJECT

		//! \brief Message box used to show error/confirmation/alert messages
		MessageBox msg_box;

		//! \brief Current zoom aplied to the scene
		float current_zoom;

		//! \brief Indicates if the model was modified by some operation
		bool modified;

		//! \brief Configures the submenu related to the object
		void configureSubmenu(BaseObject *obj);

		/*! \brief Indicates if the cut operation is currently activated. This flag modifies
		the way the methods copyObjects() and removeObject() works. */
		static bool cut_operation;

		/*! \brief Stores the model that generates the copy/cut operation. This model is updated
		from the destination model whenever a past/cut operation is done. */
		static ModelWidget *src_model;

		//! \brief Copied object on the source model
		static vector<BaseObject *> copied_objects;

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
					owners_menu;

		QAction *action_source_code,
						*action_edit,
						*action_protect,
						*action_unprotect,
						*action_remove,
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
						*action_highlight_object;

		//! \brief Actions used to create new objects on the model
		map<ObjectType, QAction *> actions_new_objects;

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

	protected:
		static const float	MINIMUM_ZOOM=0.35f,
												MAXIMUM_ZOOM=4.0f,
												ZOOM_INCREMENT=0.05f;

		QMenu *rels_menu;

		/*! \brief Configures the scene aligning the object to the grid and resizing the scene
		rect when some object is out of bound */
		void adjustSceneSize(void);

		void resizeEvent(QResizeEvent *);
		void mousePressEvent(QMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void focusInEvent(QFocusEvent *event);
		void keyReleaseEvent(QKeyEvent *event);
		void wheelEvent(QWheelEvent * event);

		//! \brief Captures and handles the QWeelEvent raised on the viewport scrollbars
		bool eventFilter(QObject *object, QEvent *event);

		//! \brief Cancel the creation of a new object (only for graphical objects)
		void cancelObjectAddition(void);

		//! \brief Desables the model actions when some new object action is active
		void disableModelActions(void);

	public:
		ModelWidget(QWidget *parent = 0);
		~ModelWidget(void);

		//! \brief Set the model as modified forcing it to be redrawn
		void setModified(bool value);

		//! \brief Returns the loaded database model filename
		QString getFilename(void);

		//! \brief Returns the temporary (security copy) of the currently loaded model
		QString getTempFilename(void);

		//! \brief Shows the editing form according to the passed object type
		void showObjectForm(ObjectType obj_type, BaseObject *object=NULL, BaseObject *parent_obj=NULL, QPointF pos=QPointF(NAN, NAN));

		//! \brief Applies a zoom factor to the model
		void applyZoom(float zoom);

		//! \brief Returns the current zoom factor applied to the model
		float currentZoom(void);

		//! \brief Returns if the model is modified or not
		bool isModified(void);

		//! \brief Returns the reference database model
		DatabaseModel *getDatabaseModel(void);

		//! \brief Returns the operation list used by database model
		OperationList *getOperationList(void);

	private slots:
		/*! \brief Os slots manipular*() gerenciam os sinais enviados pela cena e modelo para execução
		 de operações adicionais como incluir objetos modificados na lista de operações, criar
		 objetos na cena e remover objetos da cena de forma automática */

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

		//! \brief Configures the popup menu according the the selected objects list
		void configurePopupMenu(vector<BaseObject *> objects=vector<BaseObject *>());

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

		//! \brief Triggers the permission editing form
		void editPermissions(void);

		//! \brief Selects all the graphical objects under the selected schema
		void selectSchemaChildren(void);

		//! \brief Removes the selected objects
		void removeObjects(void);

		//! \brief Selects all the graphical objects on the scene
		void selectAllObjects(void);

		//! \brief Copies all the selected objects
		void copyObjects(void);

		//! \brief Paste all the objects copied previously
		void pasteObjects(void);

		//! \brief Cuts the selected objects. The effective removal is made when the cutted objects are pasted.
		void cutObjects(void);

		//! \brief Converts the Many to Many relationship generating a table and two additional relationships.
		void convertRelationshipNN(void);

		//! \brief Loads the selected object source code on the source code widget.
		void showSourceCode(void);

		//! \brief Adds a new object onto the selected model/table/schema
		void addNewObject(void);

		//! \brief Configures the selected object vector whenever the selection changes on the scene
		void configureObjectSelection(void);

		//! \brief Returns a vector containing all the page rects.
		vector<QRectF> getPagesForPrinting(const QSizeF &paper_size, unsigned &h_page_cnt, unsigned &v_page_cnt);

		//! \brief Highlights the object stored on the action that triggers the slot
		void highlightObject(void);

	public slots:
		void loadModel(const QString &filename);
		void saveModel(const QString &filename);
		void saveModel(void);
		void printModel(QPrinter *printer, bool print_grid, bool print_page_nums);

	signals:
		void s_objectModified(void);
		void s_objectsMoved(void);
		void s_objectCreated(void);
		void s_objectRemoved(void);
		void s_zoomModified(float);
		void s_modelResized(void);

		friend class MainWindow;
		friend class ModelExportForm;
		friend class OperationListWidget;
		friend class ModelObjectsWidget;
		friend class ModelOverviewWidget;
		friend class ExportHelper;
};

#endif
