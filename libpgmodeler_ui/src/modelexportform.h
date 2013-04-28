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
\class ModelExportForm
\brief Implements the operations to export a model to a file, image or directly to DBMS.
*/

#ifndef MODEL_EXPORT_FORM_H
#define MODEL_EXPORT_FORM_H

#include "ui_modelexportform.h"
#include "schemaparser.h"
#include "modelwidget.h"
#include "modelexporthelper.h"

class ModelExportForm: public QDialog, public Ui::ModelExportForm {
	private:
		Q_OBJECT

		//! \brief Stores the model widget which will be exported
		ModelWidget *model;

	public:
		ModelExportForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

	public slots:
		void show(ModelWidget *model);
		void hideEvent(QHideEvent *);

	private slots:
		void enableExportType(void);
		void exportModel(void);
		void selectOutputFile(void);
		void hideProgress(void);
		void updateProgress(int progress, QString msg);
};

#endif
