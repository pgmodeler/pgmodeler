/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\brief Implements the basic operations to manipulate functions/stored procedures on the database.
*/

#ifndef BASE_FUNCTION_H
#define BASE_FUNCTION_H

#include "schema.h"
#include "role.h"
#include "parameter.h"
#include "pgsqltypes/behaviortype.h"
#include "pgsqltypes/functiontype.h"
#include "pgsqltypes/securitytype.h"

class BaseFunction: public BaseObject {
	protected:
		//! \brief Function unique signature
		QString signature;

		//! \brief Shared library (commonly developed in C language) that stores the function routine
		QString library;

		//! \brief Symbol that identify the function on the shared library
		QString symbol;

		//! \brief Function source code (except for C language defined functions)
		QString source_code;

		//! \brief Language that is used to construct the language
		BaseObject *language;

		//! \brief Function parameters
		vector<Parameter> parameters;

		//! \brief Function security type. It can be SECURITY [INVOKER | DEFINER]
		SecurityType security_type;

		//! \brief Formats the function parameter attribute to be used by the SchemaParser
		void setParametersAttribute(unsigned def_type);

		//! \brief Formats the basic function's attributes to used in the code generation
		void setBasicFunctionAttributes(unsigned def_type);

		virtual void configureSearchAttributes();

	public:
		BaseFunction();

		//! \brief Sets the function name updating its signature
		void setName(const QString &name);

		//! \brief Sets the function schema updating its signature
		void setSchema(BaseObject *schema);

		//! \brief Adds a parameter to the function
		virtual void addParameter(Parameter param);

		//! \brief Defines the function source code (if its not use the C language)
		void setSourceCode(const QString &src_code);

		//! \brief Defines the shared library that stores the function routine
		void setLibrary(const QString &library);

		//! \brief Defines the symbol that identifies the function on shared library
		void setSymbol(const QString &symbol);

		//! \brief Defines the language used to construct the function
		void setLanguage(BaseObject *language);

		//! \brief Defines the security type of the function
		void setSecurityType(SecurityType sec_type);

		//! \brief Returns the function's source code
		QString getSourceCode();

		//! \brief Returns the shared library that stores the function definition
		QString getLibrary();

		//! \brief Returns the symbol that identifies the function on the shared library
		QString getSymbol();

		//! \brief Returns the language used to construct the function
		BaseObject *getLanguage();

		//! \brief Returns the function parameter count
		unsigned getParameterCount();

		//! \brief Returns a parameter using its index
		Parameter getParameter(unsigned param_idx);

		//! \brief Returns the security type used by the function
		SecurityType getSecurityType();

		//! \brief Removes a parameter using its name and type
		void removeParameter(const QString &name, PgSqlType type);

		//! \brief Removes a parameter using its index
		void removeParameter(unsigned param_idx);

		//! \brief Removes all the parameters from the function
		void removeParameters();

		//! \brief Returns the complete function signature
		virtual QString getSignature(bool=true);

		/*! \brief Generates the function's signature. The 'format' parameter is used
		 to adequately format the function and parameters names. By default
		 this formating is always done. */
		void createSignature(bool format=true, bool prepend_schema=true);

		virtual QString getCodeDefinition(unsigned def_type, bool) = 0;
		virtual QString getCodeDefinition(unsigned def_type) = 0;
		virtual QString getAlterDefinition(BaseObject *object) = 0;
};

#endif
