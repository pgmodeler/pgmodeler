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
\class ModelValidationWidget
\brief Implements the operations to validate models.
*/

#ifndef MODEL_VALIDATION_WIDGET_H
#define MODEL_VALIDATION_WIDGET_H

#include "ui_modelvalidationwidget.h"
#include "dbconnection.h"
#include "modelwidget.h"
#include "modelvalidationhelper.h"

class ModelValidationWidget: public QWidget, public Ui::ModelValidationWidget {
	private:
		Q_OBJECT
		ModelWidget *model_wgt;
		ModelValidationHelper validation_helper;

	public:
		ModelValidationWidget(QWidget * parent = 0);
		void setModel(ModelWidget *model_wgt);
		void updateConnections(map<QString, DBConnection *> &conns);

	private slots:
		void validateModel(void);
		void updateValidation(ValidationInfo val_info);

	public slots:
		void hide(void);

	signals:
		void s_visibilityChanged(bool);
};

#endif
