/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
		BaseObject *object,

		/*! \brief Object which was analyzed and contains an implicit inconsistency related to the "object" attribute.
		 *  An example of proxy object is a foreign key constraint in this case the foreign key is analyzed but the
				inconsistency is on to its parent table which is referencing another table (through the fk)
				and this last table is being created before the first one. The same analogy applies to relationship objects. */
		*proxy_object;

		/*! \brief References to the validated object. This vector isn't filled when
		the info is about SQL validation */
		vector<BaseObject *> references;

		QStringList errors;

	public:
		static const unsigned NO_UNIQUE_NAME=0,
                          BROKEN_REFERENCE=1,
                          SP_OBJ_BROKEN_REFERENCE=2,
                          SQL_VALIDATION_ERR=3,
                          VALIDATION_ABORTED=4;

		ValidationInfo(void);
		ValidationInfo(unsigned val_type, BaseObject *object, vector<BaseObject *> references, BaseObject *proxy_object=nullptr);
		ValidationInfo(const QString &msg);
		ValidationInfo(Exception e);

		//! \brief Returns the validation type
		unsigned getValidationType(void);

		//! \brief Returns the object which the validation info belongs to
		BaseObject *getObject(void);

		//! \brief Returns the prooxy object which parent or children contains the inconsistency
		BaseObject *getProxyObject(void);

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
