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

class ModelValidationHelper: public QObject {
	private:
		Q_OBJECT

		vector<ValidationInfo> val_infos;

		unsigned warn_count, error_count;

	public:
		ModelValidationHelper(void);

		void validateModel(DatabaseModel *model, DBConnection *conn);

		vector<ValidationInfo> getValidationInfos(void);

		unsigned getErrorCount(void);

		unsigned getWarningCount(void);

		void resolveConflicts(vector<ValidationInfo> &infos);

	signals:
		void s_validationInfoGenerated(ValidationInfo val_info);
		void s_updateProgress(int prog);
};

#endif
