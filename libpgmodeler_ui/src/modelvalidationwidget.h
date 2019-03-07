/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "htmlitemdelegate.h"
#include "hinttextwidget.h"

/* Declaring the ValidationInfo class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(ValidationInfo)

class ModelValidationWidget: public QWidget, public Ui::ModelValidationWidget {
	private:
		Q_OBJECT

		HintTextWidget *sql_validation_ht, *use_unique_names_ht;

		//! \brief Custom delegate used to paint html texts in output tree
		HtmlItemDelegate *htmlitem_del;

		//! \brief Reference model widget
		ModelWidget *model_wgt;

		//! \brief Object that handles the model validation steps
		ModelValidationHelper *validation_helper;

		//! \brief Current fix step
		int curr_step;

		//! \brief Thread used to control the validation helper
		QThread *validation_thread;

		/*! \brief Stores the graphical objects that have their ids changed so that in the end of
		the validation they can be updated to reflect the new id in the tooltips and forms */
		vector<BaseGraphicObject *> graph_objects;

		void emitValidationInProgress(void);

		//! \brief Creates a new validation thread
		void createThread(void);

		void configureValidation(void);

	protected:
		void resizeEvent(QResizeEvent *event);

	public:
		ModelValidationWidget(QWidget * parent = nullptr);

		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

		//! \brief Returns if there is a validation in progress
		bool isValidationRunning(void);

	private slots:
		void applyFixes(void);
		void updateValidation(ValidationInfo val_info);
		void updateProgress(int prog, QString msg, ObjectType obj_type, QString cmd, bool is_code_gen);
		void updateObjectName(QString obj_name, ObjectType obj_type);
		void reenableValidation(void);
		void cancelValidation(void);
		void validateRelationships(void);
		void destroyThread(bool force=false);
		void updateGraphicalObjects(void);
		void editConnections(void);
		void handleSQLValidationStarted(void);
		void swapObjectsIds(void);

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
		void s_graphicalObjectsUpdated(void);

		/*! \brief This signal is emitted whenever the user changes the connections settings
		within this widget without use the main configurations dialog */
		void s_connectionsUpdateRequest(void);
};

#endif
