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
\class ObjectFinderWidget
\brief Implements the operations to find objects over the model
*/

#ifndef OBJECT_FINDER_WIDGET_H
#define OBJECT_FINDER_WIDGET_H

#include "ui_objectfinderwidget.h"
#include "pgmodelerns.h"
#include "modelwidget.h"
#include <QWidget>

class ObjectFinderWidget : public QWidget, public Ui::ObjectFinderWidget {
	private:
		Q_OBJECT

		//! \brief Reference model widget
		ModelWidget *model_wgt;

		//! \brief Stores the selected object on the result list
		BaseObject *selected_obj;

		//! \brief Captures the ENTER press to execute search
		bool eventFilter(QObject *object, QEvent *event);

		void showEvent(QShowEvent *);

	public:
		ObjectFinderWidget(QWidget *parent = 0);

		//! \brief Lists all valid types in a QListWidget
		static void updateObjectTypeList(QListWidget *list_wgt);

		/*! \brief Lists the objects of a vector in a QTableWidget. Any row of table contains the
		reference to the object on the first column */
		static void updateObjectTable(QTableWidget *tab_wgt, vector<BaseObject *> &objects);
		
		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

	signals:
		void s_visibilityChanged(bool);
		
	public slots:
		void hide(void);
		void clearResult(void);
		void findObjects(void);

	private slots:
		//! \brief Opens the editing form of the selected object
		void editObject(void);

		//! \brief Selects a object on result list
		void selectObject(void);

		void setAllObjectsChecked(void);
		
};

#endif
