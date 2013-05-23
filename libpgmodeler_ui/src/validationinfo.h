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
\class ValidationInfo
\brief Stores information of a single object when model is being validated. Instances of this class
are emitted during the validation process on ModelValidationHelper class.
*/

#ifndef VALIDATION_INFO_H
#define VALIDATION_INFO_H

#include <QString>
#include "baseobject.h"
#include <vector>

class ValidationInfo {
	private:
		//! \brief Validation type (see constants below)
		unsigned val_type;

		//! \brief Object which the validation info belongs to
		BaseObject *object;

		/*! \brief References to the validated object. This vector isn't filled when
		the info is about SQL validation */
		vector<BaseObject *> references;

		QStringList errors;

	public:
		static constexpr unsigned NO_UNIQUE_NAME=0,
															BROKEN_REFERENCE=1,
															SQL_VALIDATION_ERR=2,
															VALIDATION_ABORTED=3;

		ValidationInfo(void);
		ValidationInfo(unsigned val_type, BaseObject *object, vector<BaseObject *> references);
		ValidationInfo(const QString &msg);
		ValidationInfo(Exception e);

		//! \brief Returns the validation type
		unsigned getValidationType(void);

		//! \brief Returns the object which the validation info belongs to
		BaseObject *getObject(void);

		//! \brief Returns the objects that somehow references the validation info object
		vector<BaseObject *> getReferences(void);

		//! \brief Returns the SQL errors generated during the SQL validation
		QStringList getErrors(void);

		/*! \brief Returns if the content of validation info is valid. To be valid
		the info must have the object not null when it is NO_UNIQUE_NAME or BROKEN_REFERENCE,
		and when its SQL_VALIDATION_ERR the sql error message must be set */
		bool isValid(void);

		friend class ModelValidationHelper;
		friend class QVariant;
};

#endif
