/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

		//! \brief Reference database model
		DatabaseModel *model;

		//! \brief Source and destination object selectors
		ObjectSelectorWidget *src_object_sel, *dst_object_sel;

		void fillCreationOrderGrid(void);

	public:
		SwapObjectsIdsWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
		~SwapObjectsIdsWidget(void);
		void setModel(DatabaseModel *model);

	private slots:
		void showObjectId(void);
		void swapObjectsIds(void);
		void filterObjects(void);

	signals:
		void s_objectsIdsSwapped(void);
};

#endif
