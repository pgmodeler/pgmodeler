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
\class SceneInfoWidget
\brief Implements an widget to show info about canvas (scene) and selected objects
*/

#ifndef SCENE_INFO_WIDGET_H
#define SCENE_INFO_WIDGET_H

#include "ui_sceneinfowidget.h"
#include "baseobjectview.h"

class SceneInfoWidget: public QWidget, public Ui::SceneInfoWidget {
	private:
		Q_OBJECT

	public:
		SceneInfoWidget(QWidget * parent = nullptr);

	public slots:
		void updateSelectedObject(BaseObjectView *object);
		void updateSelectedObjects(int obj_count, const QRectF &objs_rect);
		void updateMousePosition(const QPointF &mouse_pos);
		void updateSceneZoom(double zoom);
};

#endif
