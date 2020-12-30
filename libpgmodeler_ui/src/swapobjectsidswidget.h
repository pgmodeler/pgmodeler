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
\ingroup libpgmodeler_ui
\class ChangeObjectOrderWidget
\brief Implements the operations to swap objects ids to cover the cases where model validation cannot reach.
*/

#ifndef SWAP_OBJECTS_IDS_WIDGET_H
#define SWAP_OBJECTS_IDS_WIDGET_H

#include <QWidget>
#include "ui_swapobjectsidswidget.h"
#include "objectselectorwidget.h"

class SwapObjectsIdsWidget: public QWidget, public Ui::SwapObjectsIdsWidget {
	private:
		Q_OBJECT

		static const QString IdLabel;
		unsigned selector_idx = 0;

		//! \brief Reference database model
		DatabaseModel *model;

		//! \brief Source and destination object selectors
		ObjectSelectorWidget *src_object_sel, *dst_object_sel;

		void fillCreationOrderGrid();
		bool eventFilter(QObject *object, QEvent *event);

	public:
		SwapObjectsIdsWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::Widget);
		virtual ~SwapObjectsIdsWidget();
		void setModel(DatabaseModel *model);
		void setSelectedObjects(BaseObject *src_object, BaseObject *dst_objct);

	private slots:
		void showObjectId();
		void filterObjects();
		void selectItem(QTableWidgetItem *item);
		void clearSelectors();

	public slots:
		void swapObjectsIds();

	signals:
		void s_objectsIdsSwapped();
		void s_objectsIdsSwapReady(bool);
};

#endif
