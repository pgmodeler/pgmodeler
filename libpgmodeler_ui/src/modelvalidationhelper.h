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
\class ModelValidationHelper
\brief Implements the base operations to validate models
*/

#ifndef MODEL_VALIDATION_HELPER_H
#define MODEL_VALIDATION_HELPER_H

#include <QObject>
#include "validationinfo.h"
#include "databasemodel.h"
#include "dbconnection.h"
#include "modelexporthelper.h"

class ModelValidationHelper: public QObject {
	private:
		Q_OBJECT

		//! \brief Reference database model
		DatabaseModel *db_model;

		//! \brief DBMS export helper used to execute SQL code validation
		ModelExportHelper export_helper;

		//! \brief Warning and error counters
		unsigned warn_count, error_count;

		int progress;

	public:
		ModelValidationHelper(void);

		/*! \brief Validates the specified model. If a connection is specifies executes the
		SQL validation directly on DBMS */
		void validateModel(DatabaseModel *model, DBConnection *conn=nullptr, const QString &pgsql_ver="");

		//! \brief Returns the error count (only when executing SQL validation)
		unsigned getErrorCount(void);

		//! \brief Returns the warning count
		unsigned getWarningCount(void);

		//! \brief Try to resolve the conflict specified by validation info
		void resolveConflict(ValidationInfo &info);

	private slots:
		void redirectExportProgress(int prog, QString msg);

	signals:
		//! \brief This signal is emitted when a validation info is generated
		void s_validationInfoGenerated(ValidationInfo val_info);

		//! \brief This signal is emitted when the validation progress changes
		void s_progressUpdated(int prog, QString msg);
};

#endif
