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
\ingroup libpgmodeler
\class Domain
\brief Implements the operations to manipulate domains on the database.
\note <strong>Creation date:</strong> 07/04/2008
*/

#ifndef DOMAIN_H
#define DOMAIN_H

#include "baseobject.h"
#include "schema.h"

class Domain: public BaseObject{
	private:
		//! \brief Constraint name used by the domain
		QString constraint_name,
						//! \brief Validation expression used by domain (check) constraint
						expression,
						//! \brief Domain default data type value
						default_value;

		//! \brief Indicates that the domains accepts null values or not
		bool not_null;

		//! \brief Domain data type
		PgSQLType type;

	public:
		Domain(void);
		~Domain(void);

		//! \brief Sets the domain's constraint name
		void setConstraintName(const QString &constr_name);

		//! \brief Sets the check expression of the domain
		void setExpression(const QString &expr);

		//! \brief Sets the default value of the domain
		void setDefaultValue(const QString &default_val);

		//! \brief Defines whether the domain accepts null values or not
		void setNotNull(bool value);

		//! \brief Defines the domain data type
		void setType(PgSQLType type);

		/*! \brief Overloaded BaseObject name definition method.
		 Updates the reference of the domain as a PostgreSQL data type */
		void setName(const QString &name);

		/*! \brief Overloaded BaseObject schema definition method.
		 Updates the reference of the domain as a PostgreSQL data type */
		void setSchema(BaseObject *schema);

		//! \brief Methods to access domain's attributes
		QString getConstraintName(void);
		QString getExpression(void);
		QString getDefaultValue(void);
		bool isNotNull(void);
		PgSQLType getType(void);

		//! \brief Returns the SQL / XML code definition for the domain
		virtual QString getCodeDefinition(unsigned def_type) final;

		void operator = (Domain &domain);
};

#endif
