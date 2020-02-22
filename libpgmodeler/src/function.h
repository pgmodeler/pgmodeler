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
\brief Implements the operations to manipulate stored procedures (function) on the database.
\note <strong>Creation date:</strong> 28/09/2006
*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "schema.h"
#include "role.h"
#include "parameter.h"
#include "pgsqltypes/behaviortype.h"
#include "pgsqltypes/functiontype.h"
#include "pgsqltypes/securitytype.h"

class Function: public BaseObject {
	private:
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

		//! \brief Indicates whether the function returns a set of data (RETURNS SET OF)
		bool returns_setof,

		//! \brief Indicate whether the object is a windows function or not
		is_wnd_function,

		//! \brief Indicate whether the object is leak proof or not
		is_leakproof;

		//! \brief Type of execution behavior applied to the function
		BehaviorType behavior_type;

		//! \brief Function type. It can be VOLATILE, IMMUTABLE, STABLE
		FunctionType function_type;

		//! \brief Function return type
		PgSqlType return_type;

		/*! \brief Stores the table columns returned by the clause RETURNS TABLE. This clause instead
		 of return a specific element returns a whole table. This structure is available
		 only on PostgreSQL 8.4 and later */
		vector<Parameter> ret_table_columns;

		//! \brief Function security type. It can be SECURITY [INVOKER | DEFINER]
		SecurityType security_type;

		//! \brief Execution cost for the function
		unsigned execution_cost;

		//! \brief Estimated returned row (tuple) amount
		unsigned row_amount;

		//! \brief Formats the function parameter attribute to be used by the SchemaParser
		void setParametersAttribute(unsigned def_type);

		//! \brief Formats the function return type to be used by the SchemaParser
		void setTableReturnTypeAttribute(unsigned def_type);

	protected:
		virtual void configureSearchAttributes();

	public:
		Function();

		//! \brief Sets the function name updating its signature
		void setName(const QString &name);

		//! \brief Sets the function schema updating its signature
		void setSchema(BaseObject *schema);

		//! \brief Adds a parameter to the function
		void addParameter(Parameter param);

		//! \brief Adds a column to the function returned table
		void addReturnedTableColumn(const QString &name, PgSqlType type);

		//! \brief Defines the function source code (if its not use the C language)
		void setSourceCode(const QString &src_code);

		//! \brief Defines the shared library that stores the function routine
		void setLibrary(const QString &library);

		//! \brief Defines the symbol that identifies the function on shared library
		void setSymbol(const QString &symbol);

		//! \brief Defines the language used to construct the function
		void setLanguage(BaseObject *language);

		//! \brief Defines the execution cost for the function
		void setExecutionCost(unsigned exec_cost);

		//! \brief Defines the estimated amount of rows (tuples) returned by the function
		void setRowAmount(unsigned row_amount);

		//! \brief Defines whether returns a set of values or not
		void setReturnSetOf(bool value);

		//! \brief Defines whether the function is a windows function or not
		void setWindowFunction(bool value);

		//! \brief Defines whether the function is leak proof or not
		void setLeakProof(bool value);

		//! \brief Defines the function's execution behavior
		void setBehaviorType(BehaviorType behav_type);

		//! \brief Defines the function type (VOLATILE, IMMUTABLE, STABLE)
		void setFunctionType(FunctionType func_type);

		//! \brief Defines the function return type
		void setReturnType(PgSqlType type);

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

		//! \brief Returns the returned table column count
		unsigned getReturnedTableColumnCount();

		//! \brief Returns a parameter using its index
		Parameter getParameter(unsigned param_idx);

		//! \brief Returns a column from returned table using its index
		Parameter getReturnedTableColumn(unsigned column_idx);

		//! \brief Indicates whether the function returns setof or not
		bool isReturnSetOf();

		//! \brief Indicates whether the fucntion returns a table or not
		bool isReturnTable();

		//! \brief Indicates whether the function is window or not
		bool isWindowFunction();

		//! \brief Indicates whether the function is leak proof or not
		bool isLeakProof();

		//! \brief Returns the function execution behavior
		BehaviorType getBehaviorType();

		//! \brief Returns the function type
		FunctionType getFunctionType();

		//! \brief Returns the function returned type
		PgSqlType getReturnType();

		//! \brief Returns the security type used by the function
		SecurityType getSecurityType();

		//! \brief Returns the function's execution cost
		unsigned getExecutionCost();

		//! \brief Returns the estimated returned row amount
		unsigned getRowAmount();

		//! \brief Removes a parameter using its name and type
		void removeParameter(const QString &name, PgSqlType type);

		//! \brief Removes a parameter using its index
		void removeParameter(unsigned param_idx);

		//! \brief Removes all the parameters from the function
		void removeParameters();

		//! \brief Removes a column from returned table using its index
		void removeReturnedTableColumn(unsigned column_idx);

		//! \brief Removes all the columns from returned table column
		void removeReturnedTableColumns();

		//! \brief Returns the complete function signature
		virtual QString getSignature(bool=true);

		/*! \brief Generates the function's signature. The 'format' parameter is used
		 to adequately format the function and parameters names. By default
		 this formating is always done. */
		void createSignature(bool format=true, bool prepend_schema=true);

		//! \brief Returns the SQL / XML code definition for the function
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;

		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getAlterDefinition(BaseObject *object) final;
};

#endif
