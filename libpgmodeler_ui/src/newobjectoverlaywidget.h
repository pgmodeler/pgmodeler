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
\class NewObjectOverlayWidget
\brief Implements the a basic overlay to quicly trigger action to create new objects.
*/

#ifndef NEW_OBJECT_OVERLAY_WIDGET_H
#define NEW_OBJECT_OVERLAY_WIDGET_H

#include <vector>
#include <QWidget>
#include "baseobject.h"
#include "ui_newobjectoverlaywidget.h"

//Adding the declaration of ModelWidget class in order to be visible to NewObjectOverlayWidget
class ModelWidget;

class NewObjectOverlayWidget: public QWidget, public Ui::NewObjectOverlayWidget {
	private:
		Q_OBJECT

	public:
		explicit NewObjectOverlayWidget(ModelWidget * parent);
		void setSelectedObjects(vector<BaseObject *> &sel_objs);
		void configureOverlayButtons();
};

#endif
