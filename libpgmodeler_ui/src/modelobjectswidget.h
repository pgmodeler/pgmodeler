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
\ingroup libpgmodeler_ui
\class ModelObjectsWidget
\brief Implements a widget that permits an tree and list view of all model objects.
*/

#ifndef MODEL_OBJECTS_WIDGET_H
#define MODEL_OBJECTS_WIDGET_H

#include <QtWidgets>
#include "ui_modelobjectswidget.h"
#include "modelwidget.h"
#include "messagebox.h"
#include "objectfinderwidget.h"

class LIBPGMODELER_UI_EXPORT ModelObjectsWidget: public QWidget, public Ui::ModelObjectsWidget {
	private:
		Q_OBJECT

		/*! \brief Indicates if the widget must be used as a simplified view (without the most interactions).
		The purpose to use it as simplified view is to be serve as a object pick commonly used on the
		object selectors. See ObjectSelectorWidget for details. */
		bool	simplified_view,

					/*! \brief Indicates if the object tree state must be saved, this means, that the current item
					expansion is memorized and can be restored at any moment via restoreTreeState() method */
					save_tree_state;

		//! \brief Stores the reference to the object currently selected on the tree/list
		BaseObject *selected_object;

		/*! \brief Stores the  initial splitter size to be used in conjunction with the
		object type visualization buttons */
		QSettings widgets_conf;

		//! \brief Reference model widget. This is the object used to populate the tree and list
		ModelWidget *model_wgt;

		//! \brief Reference database model. This is the object used to populate the tree and list
		DatabaseModel *db_model;

		//! \brief Stores which object types are visible on the view
		map<ObjectType, bool> visible_objs_map;

		//! \brief Updates only a schema tree starting from the 'root' item
		void updateSchemaTree(QTreeWidgetItem *root);

		//! \brief Updates only a table tree starting from the 'root' item
		void updateTableTree(QTreeWidgetItem *root, BaseObject *schema);

		//! \brief Updates only a view tree starting from the 'root' item
		void updateViewTree(QTreeWidgetItem *root, BaseObject *schema);

		//! \brief Updates only the permission tree related to the specified object
		void updatePermissionTree(QTreeWidgetItem *root, BaseObject *object);

		//! \brief Updates the whole database object tree
		void updateDatabaseTree(void);

		//! \brief Updates the whole object list
		void updateObjectsList(void);

		//! \brief Returns an item from the tree related to the specified object reference
		QTreeWidgetItem *getTreeItem(BaseObject *object);

		//! \brief Generates a QVariant containing the passed object reference as data
		QVariant generateItemValue(BaseObject *object);

    QTreeWidgetItem *createItemForObject(BaseObject *object, QTreeWidgetItem *root=nullptr, bool update_perms=true);

		void mouseMoveEvent(QMouseEvent *);
		void resizeEvent(QResizeEvent *);
		void closeEvent(QCloseEvent *);
		void showEvent(QShowEvent *);
    bool eventFilter(QObject *object, QEvent *event);

  public:
		ModelObjectsWidget(bool simplified_view=false, QWidget * parent = 0);

		BaseObject *getSelectedObject(void);

	protected:
		//! \brief Saves the currently expanded items on the specified vector
		void saveTreeState(vector<BaseObject *> &tree_items);

		//! \brief Restores the tree at a previous state when the specified items were expanded
		void restoreTreeState(vector<BaseObject *> &tree_items);

		//! \brief Defines if the widget must save/restore the tree state automaticaly
		void saveTreeState(bool value);

	public slots:
		void setModel(ModelWidget *model_wgt);
		void setModel(DatabaseModel *db_model);
		void changeObjectsView(void);
		void updateObjectsView(void);
		void setObjectVisible(ObjectType obj_type, bool visible);
		void close(void);
		void hide(void);

	private slots:
		void setObjectVisible(QListWidgetItem *item);
		void setAllObjectsVisible(bool value);
		void selectObject(void);
		void showObjectMenu(void);
		void editObject(void);
		void collapseAll(void);
		void filterObjects(void);

	signals:
		void s_visibilityChanged(BaseObject *,bool);
		void s_visibilityChanged(bool);

		friend class MainWindow;
};

#endif
