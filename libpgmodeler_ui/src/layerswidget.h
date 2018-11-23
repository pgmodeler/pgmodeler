/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class LayersWidget
\brief Implements the widget that allows the user to handle scene layers
*/

#ifndef LAYERS_WIDGET_H
#define LAYERS_WIDGET_H

#include <QWidget>
#include "ui_layerswidget.h"
#include "modelwidget.h"

class LayersWidget : public QWidget, Ui::LayersWidget {
	private:
		Q_OBJECT

		ModelWidget *model;

		QListWidgetItem *curr_item;

		QString curr_text;

		int curr_row;

		void setLayers(const QStringList &layers);

		bool eventFilter(QObject *watched, QEvent *event);

	public:
		explicit LayersWidget(QWidget *parent = nullptr);

		void setModel(ModelWidget *model);

	private slots:
		QListWidgetItem *addLayer(const QString &name = QString());
		void startLayerRenaming(QListWidgetItem *item);
		void finishLayerRenaming(void);
		void updateActiveLayers(void);
		void removeLayer(bool clear = false);
		void enableButtons(void);

	public slots:
		void setVisible(bool value);

	signals:
		void s_visibilityChanged(bool);
};

#endif
