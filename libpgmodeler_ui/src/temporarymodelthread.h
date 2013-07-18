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
\class TemporaryModelThread
\brief Implements a thread that controls the temporary model files saving
*/

#ifndef TEMPORARY_MODEL_THREAD_H
#define TEMPORARY_MODEL_THREAD_H

#include "modelwidget.h"

class TemporaryModelThread: public QThread {
	private:
		Q_OBJECT
		ModelWidget *model_wgt;

	public:
		TemporaryModelThread(QObject *parent=0);
		void setModel(ModelWidget *model);
		void run(void) override;
};

#endif
