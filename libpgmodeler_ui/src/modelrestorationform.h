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
\class ModelRestorationForm
\brief Implements the operation to retore models before a unexpected crash.
*/

#ifndef MODEL_RESTORATION_FORM_H
#define MODEL_RESTORATION_FORM_H

#include <QtWidgets>
#include "globalattributes.h"
#include "ui_modelrestorationform.h"

class ModelRestorationForm: public QDialog, public Ui::ModelRestorationForm {
	private:
		Q_OBJECT

	public:
		ModelRestorationForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

		//! \brief Returns the list of temporary files existant on tmp/ dir
		QStringList getTemporaryModels(void);

		//! \brief Returns the list of temporary files selected to be restored
		QStringList getSelectedModels(void);

	public slots:
		int exec(void);

		//! \brief Clears the tmp/ dir removing all temporary files
		void removeTemporaryModels(void);

		//! \brief Checks if there is at least one temporary file on tmp/ dir
		bool hasTemporaryModels(void);

	private slots:
		void enableRestoration(void);
};

#endif
