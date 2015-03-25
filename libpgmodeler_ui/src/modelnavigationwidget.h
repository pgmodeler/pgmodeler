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
\class ModelNavigationWidget
\brief Implements the operations to navigate through openend models without use tabs.
*/

#ifndef MODEL_NAVIGATION_WIDGET_H
#define MODEL_NAVIGATION_WIDGET_H

#include <QWidget>
#include "modelwidget.h"
#include "ui_modelnavigationwidget.h"

class LIBPGMODELER_UI_EXPORT ModelNavigationWidget: public QWidget, public Ui::ModelNavigationWidget {
	private:
		Q_OBJECT

		void enableNavigationButtons(void);

	public:
		ModelNavigationWidget(QWidget * parent = 0);

		//! brief Returns the combo's current index
		int getCurrentIndex(void);

		//! brief Returns the text of the combo's current index
		QString getText(int idx);

	public slots:
		//! brief Adds the model to the listing
		void addModel(ModelWidget *model);

		//! brief Updates a model's info on the listing
		void updateModelText(int idx, const QString &text, const QString &filename);

		//! brief Remove an entry for a model using its index
		void removeModel(int idx);

	private slots:
		void setCurrentModel(void);
    void showTooltip(int idx);

  signals:
		void s_modelRemoved(int idx);
		void s_modelCloseRequested(int idx);
		void s_currentModelChanged(int idx);
};

#endif
