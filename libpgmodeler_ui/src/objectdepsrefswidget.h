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
\class ObjectDepsRefsWidget
\brief Implements the form to show the object's dependeces and references
*/

#ifndef OBJECT_DEPS_REFS_WIDGET_H
#define OBJECT_DEPS_REFS_WIDGET_H

#include "ui_objectdepsrefswidget.h"
#include "baseobjectwidget.h"
#include "baseobjectview.h"

class ObjectDepsRefsWidget: public BaseObjectWidget, public Ui::ObjectDepsRefsWidget {
	private:
		Q_OBJECT

		ModelWidget *model_wgt;

		void setAttributes(DatabaseModel *model, BaseObject *object, BaseObject *parent_obj);

	public:
		ObjectDepsRefsWidget(QWidget * parent = nullptr);
		void setAttributes(ModelWidget *model_wgt, BaseObject *object, BaseObject *parent_obj=nullptr);

	public slots:
		void applyConfiguration();

	private slots:
		void clearTables();
		void updateObjectTables();
		void handleItemSelection(QTableWidgetItem *item);
};

#endif
