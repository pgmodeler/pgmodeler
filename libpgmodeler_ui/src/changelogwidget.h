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
\class ChangelogWidget
\brief Implements the widget that allows the user to handle model's changelog
*/

#ifndef CHANGELOG_WIDGET_H
#define CHANGELOG_WIDGET_H

#include <QWidget>
#include "ui_changelogwidget.h"
#include "modelwidget.h"

class ChangelogWidget : public QWidget, Ui::ChangelogWidget {
	private:
		Q_OBJECT

		ModelWidget *model;

	public:
		explicit ChangelogWidget(QWidget *parent = nullptr);

		//! \brief Defines the model in which the widget will work
		void setModel(ModelWidget *model);

	public slots:
		void setVisible(bool value) override;
		void updateChangelogInfo();
		void clearChangelog();

	signals:
		//! \brief Signal emitted whenever the widget changes its visibility
		void s_visibilityChanged(bool);
};

#endif
