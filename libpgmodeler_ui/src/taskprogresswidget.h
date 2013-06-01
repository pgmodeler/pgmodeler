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
\class TaskProgressWidget
\brief Implements a widget that shows the progress of executed operations (e.g. loading a model file, generation source code)
*/

#ifndef TASK_PROGRESS_WIDGET_H
#define TASK_PROGRESS_WIDGET_H

#include <QtWidgets>
#include "ui_taskprogresswidget.h"
#include <map>
#include "utf8string.h"

using namespace std;

class TaskProgressWidget: public QDialog, public Ui::TaskProgressWidget
{
	private:
		Q_OBJECT

		//! \brief Stores the icons that are shown as the icons tokens are send via	updateProgress() slot
		map<unsigned, QIcon> icons;

	public:
		TaskProgressWidget(QWidget *parent=0, Qt::WindowFlags f=0);
		void addIcon(unsigned id, const QIcon &ico);

	public slots:
		void show(void);
		void close(void);
		void updateProgress(int progress, const QString &text, unsigned icon_id);
};

#endif
