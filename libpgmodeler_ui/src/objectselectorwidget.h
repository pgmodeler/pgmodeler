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
\class ObjectSelectorWidget
\brief Implements a simple object picker based upon the object view widget.
*/

#ifndef OBJECT_SELECTOR_WIDGET_H
#define OBJECT_SELECTOR_WIDGET_H

#include <QtWidgets>
#include "databasemodel.h"
#include "syntaxhighlighter.h"
#include "modelobjectswidget.h"
#include "ui_objectselectorwidget.h"

class ObjectSelectorWidget: public QWidget, public Ui::ObjectSelectorWidget {
	private:
		Q_OBJECT

		//! \brief An object view widget instance used as object picker
		ModelObjectsWidget *obj_view_wgt;

		/*! \brief Stores the selector instance currently focused. This attribute is used to
		prevent other selector in a same form to receive the selected object */
		static QObject *current_selector;

		//! \brief Selected object name highlighter
		SyntaxHighlighter *obj_name_hl;

		//! \brief Selected object reference
		BaseObject *selected_obj;

		//! \brief Object type that the selector is able to pick
		ObjectType sel_obj_type;

		//! \brief Stores the database model which the selector must search the object
		DatabaseModel *model;

	public:
		ObjectSelectorWidget(ObjectType sel_obj_type, bool install_highlighter, QWidget * parent = 0);
		~ObjectSelectorWidget(void);

		//! \brief Returns the reference to the selected object
		BaseObject *getSelectedObject(void);

		//! \brief Defines the initial selected object to be show on the selector
		void setSelectedObject(BaseObject *object);

		//! \brief Defines the model which the selector will search the objects
		void setModel(DatabaseModel *model);

	private slots:
		void showSelectedObject(BaseObject *obj_sel, bool=false);

		//! \brief Shows the object view widget (picker)
		void showObjectView(void);

	public slots:
		void clearSelector(void);

	signals:
		//! \brief Signal emitted when the user selects an object
		void s_objectSelected(void);

		//! \brief Signal emitted when the user clears the selector
		void s_selectorCleared(void);
};

#endif
