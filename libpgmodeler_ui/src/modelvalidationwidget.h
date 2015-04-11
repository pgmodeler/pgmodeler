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
\class ModelValidationWidget
\brief Implements the operations to display to the user the validation operation performed by ValidationHelper
*/

#ifndef MODEL_VALIDATION_WIDGET_H
#define MODEL_VALIDATION_WIDGET_H

#include "ui_modelvalidationwidget.h"
#include "connection.h"
#include "modelwidget.h"
#include "modelvalidationhelper.h"
#include "swapobjectsidswidget.h"

/* Declaring the ValidationInfo class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(ValidationInfo)

class ModelValidationWidget: public QWidget, public Ui::ModelValidationWidget {
	private:
		Q_OBJECT

		//! \brief Reference model widget
		ModelWidget *model_wgt;

		//! \brief Object that handles the model validation steps
    ModelValidationHelper *validation_helper;

		//! \brief Object creation order modifier
		SwapObjectsIdsWidget *swapobjectsids_wgt;

		//! \brief Current fix step
		int curr_step;

		//! \brief Thread used to control the validation helper
		QThread *validation_thread;

		void emitValidationInProgress(void);    

    //! brief Creates a new validation thread
    void createThread(void);

    void configureValidation(void);

	public:
		ModelValidationWidget(QWidget * parent = 0);

		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

		//! \brief Updates the connections combo
		void updateConnections(map<QString, Connection *> &conns);

    //! brief Returns if there is a validation in progress
    bool isValidationRunning(void);

	private slots:
		void applyFixes(void);
		void updateValidation(ValidationInfo val_info);
    void updateProgress(int prog, QString msg, ObjectType obj_type, QString cmd, bool is_code_gen);
		void updateObjectName(QString obj_name, ObjectType obj_type);
		void reenableValidation(void);
		void cancelValidation(void);
		void swapObjectsIds(void);
    void validateRelationships(void);
    void destroyThread(bool force=false);

	public slots:
		void hide(void);
		void clearOutput(void);
    void validateModel(void);

	signals:
		void s_visibilityChanged(bool);
    void s_validationInProgress(bool);
    void s_validationFinished(bool);
    void s_validationCanceled(void);
    void s_fixApplied(void);
};

#endif
