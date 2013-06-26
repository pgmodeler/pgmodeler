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
\class ModelOverviewWidget
\brief Implements the model overview widget operations.
*/

#ifndef MODEL_OVERVIEW_WIDGET_H
#define MODEL_OVERVIEW_WIDGET_H

#include <QtWidgets>
#include "exception.h"
#include "ui_modeloverviewwidget.h"
#include "modelwidget.h"

class ModelOverviewWidget: public QWidget, public Ui::ModelOverviewWidget {
	private:
		Q_OBJECT

		//! \brief Model which object are drawn on the overview widget
		ModelWidget *model;

		//! \brief Zoom factor applied to the visualization
		float zoom_factor;

		//! \brief Resize factor applied to overview widgets (default: 20% of the scene original size)
		static constexpr float RESIZE_FACTOR=0.20f;

		void mouseDoubleClickEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void closeEvent(QCloseEvent *event);
		void showEvent(QShowEvent *event);

		/*! \brief Updates the overview with the last modifications on the scene. The bool parameter
		is used to force the update even if the overview widget is not visible */
		void updateOverview(bool force_update);

	public:
		ModelOverviewWidget(QWidget *parent = 0);

	public slots:
		//! \brief Updates the overview (only if the widget is visible)
		void updateOverview(void);

		//! \brief Resizes the frame that represents the visualization window
		void resizeWindowFrame(void);

		//! \brief Resizes the whole overview widget
		void resizeOverview(void);

		//! \brief Updates the overview zoom factor
		void updateZoomFactor(float zoom);

		//! \brief Shows the overview specifying the model to be drawn
		void show(ModelWidget *model);

	signals:
		//! \brief Signal emitted whenever the overview window change the visibility
		void s_overviewVisible(bool);
};

#endif
