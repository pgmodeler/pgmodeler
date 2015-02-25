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
\class ModelValidationHelper
\brief Implements the base operations to validate models
*/

#ifndef MODEL_VALIDATION_HELPER_H
#define MODEL_VALIDATION_HELPER_H

#include <QObject>
#include "validationinfo.h"
#include "databasemodel.h"
#include "connection.h"
#include "modelexporthelper.h"

class ModelValidationHelper: public QObject {
	private:
		Q_OBJECT

		//! \brief Reference database model
		DatabaseModel *db_model;

		//! \brief Connection used to validate model on DBMS
		Connection *conn;

		//! \brief Selected PostgreSQL syntax to validate model on DBMS
		QString pgsql_ver;

		//! \brief DBMS export helper used to execute SQL code validation
		ModelExportHelper export_helper;

		QThread *export_thread;

		//! \brief Warning and error counters
		unsigned warn_count, error_count;

		//! \brief Validation progress
		int progress;

		//! \brief Indicates if the validation was canceled by the user
		bool  valid_canceled,

		//! \brief Indicates if the validation is on fix mode.
    fix_mode,

    use_tmp_names;

		/*! \brief Stores the validation infos generated during validation steps.
		This vector is read when applying fixes */
		vector<ValidationInfo> val_infos;

    //! brief Stores the analyzed relationship marked as invalidated
    vector<BaseObject *> inv_rels;

		/*! \brief When the execution of the instance of this class is in another thread instead of main app
		thread puts the parent thread to sleep for [msecs] ms to give time to external operationsto be correctly
		finished before completely quit the thread itself otherwise the method don't do anything. */
		void sleepThread(unsigned msecs);

		void generateValidationInfo(unsigned val_type, BaseObject *object, vector<BaseObject *> refs);

	public:
		ModelValidationHelper(void);

		/*! \brief Validates the specified model. If a connection is specifies executes the
		SQL validation directly on DBMS */
    void setValidationParams(DatabaseModel *model, Connection *conn=nullptr, const QString &pgsql_ver=QString(), bool use_tmp_names=false);

		//! \brief Switch the validator to fix mode
		void switchToFixMode(bool value);

		//! \brief Returns if the validator is on fix mode
		bool isInFixMode(void);

		//! \brief Returns the error count (only when executing SQL validation)
		unsigned getErrorCount(void);

		//! \brief Returns the warning count
		unsigned getWarningCount(void);

		//! \brief Try to resolve the conflict specified by validation info
		void resolveConflict(ValidationInfo &info);

	private slots:
		void redirectExportProgress(int prog, QString msg, ObjectType obj_type, QString cmd);
		void captureThreadError(Exception e);
		void emitValidationCanceled(void);
		void emitValidationFinished(void);

	public slots:
		void validateModel(void);
		void applyFixes(void);
		void cancelValidation(void);

	signals:
		//! \brief This signal is emitted when a validation info is generated
		void s_validationInfoGenerated(ValidationInfo val_info);

		//! \brief This signal is emitted when the validation progress changes
		void s_progressUpdated(int prog, QString msg, ObjectType obj_type=BASE_OBJECT, QString cmd=QString());

		//! \brief This signal is emitted when the object is processed by the validator
		void s_objectProcessed(QString obj_name, ObjectType obj_type);

		//! \brief This signal is emitted when the validation was sucessfully finished
		void s_validationFinished(void);

		//! \brief This signal is emitted when the validation was canceled by user
		void s_validationCanceled(void);

		//! \brief This signal is emitted when the dbms export thread start to run
		void s_sqlValidationStarted(bool);

		//! \brief This signal is emitted when the validator applied some fix on validation info
		void s_fixApplied(void);

    /*! brief This signal is emitted when the validator need the validation of relationship.
        This process must be performed outside the current thread, this explains the usage of this signal */
    void s_relsValidationRequested(void);
};

#endif
