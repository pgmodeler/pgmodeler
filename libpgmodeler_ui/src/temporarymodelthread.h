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

		//! \brief Model widget which thread operates
		ModelWidget *model_wgt;

		/*! \brief When this flag is false the methods run() and quit() has no effect.
		By default any instance of this class is enabled. But when disabled the user must
		explicity call  setEnabled(true) to reenable thread's execution before call start().
		This attribute is need because in MainWindow restoration mode the thread cannot
		be executed while models are being restored. */
		bool enabled;

	public:
		TemporaryModelThread(QObject *parent=0);
		void setModel(ModelWidget *model);
		bool isEnabled(void);

	public slots:
		void setEnabled(bool value);

	protected:
		void run(void) override;
};

#endif
