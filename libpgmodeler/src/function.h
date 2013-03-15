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
\brief Implements the operations to manipulate stored procedures (function) on the database.
\note <strong>Creation date:</strong> 28/09/2006
*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "baseobject.h"
#include "schema.h"
#include "role.h"
#include "column.h"

class Parameter: public Column {
	private:
		/*! \brief This attributes indicates whether the parameter
		is in, out or both (IN, OUT, INOUT) */
		bool is_in, is_out;

	public:
		Parameter(void);

		void setType(PgSQLType type);

		void setIn(bool value);
		void setOut(bool value);

		bool isIn(void);
		bool isOut(void);

		//! \brief Returns the SQL / XML code definition for the parameter
		QString getCodeDefinition(unsigned def_type);
		QString getCodeDefinition(unsigned def_type, bool reduced_form);

		void operator = (const Parameter &param);
};


class Function: public BaseObject {
	private:
		static unsigned function_id;

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
		bool returns_setof;

		//! \brief Indicate whether the object is a windows function or not
		bool is_wnd_function;

		//! \brief Type of execution behavior applied to the function
		BehaviorType behavior_type;

		//! \brief Function type. It can be VOLATILE, IMMUTABLE, STABLE
		FunctionType function_type;

		//! \brief Function return type
		PgSQLType return_type;

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

	public:
		Function(void);

		//! \brief Sets the function name updating its signature
		void setName(const QString &name);

		//! \brief Sets the function schema updating its signature
		void setSchema(BaseObject *schema);

		//! \brief Adds a parameter to the function
		void addParameter(Parameter param);

		//! \brief Adds a column to the function returned table
		void addReturnedTableColumn(const QString &name, PgSQLType type);

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

		//! \brief Defines the function's execution behavior
		void setBehaviorType(BehaviorType behav_type);

		//! \brief Defines the function type (VOLATILE, IMMUTABLE, STABLE)
		void setFunctionType(FunctionType func_type);

		//! \brief Defines the function return type
		void setReturnType(PgSQLType type);

		//! \brief Defines the security type of the function
		void setSecurityType(SecurityType sec_type);

		//! \brief Returns the function's source code
		QString getSourceCode(void);

		//! \brief Returns the shared library that stores the function definition
		QString getLibrary(void);

		//! \brief Returns the symbol that identifies the function on the shared library
		QString getSymbol(void);

		//! \brief Returns the language used to construct the function
		BaseObject *getLanguage(void);

		//! \brief Returns the function parameter count
		unsigned getParameterCount(void);

		//! \brief Returns the returned table column count
		unsigned getReturnedTableColumnCount(void);

		//! \brief Returns a parameter using its index
		Parameter getParameter(unsigned param_idx);

		//! \brief Returns a column from returned table using its index
		Parameter getReturnedTableColumn(unsigned column_idx);

		//! \brief Indicates whether the function returns setof or not
		bool isReturnSetOf(void);

		//! \brief Indicates whether the fucntion returns a table or not
		bool isReturnTable(void);

		//! \brief Indicates whether the function is window or not
		bool isWindowFunction(void);

		//! \brief Returns the function execution behavior
		BehaviorType getBehaviorType(void);

		//! \brief Returns the function type
		FunctionType getFunctionType(void);

		//! \brief Returns the function returned type
		PgSQLType getReturnType(void);

		//! \brief Returns the security type used by the function
		SecurityType getSecurityType(void);

		//! \brief Returns the function's execution cost
		unsigned getExecutionCost(void);

		//! \brief Returns the estimated returned row amount
		unsigned getRowAmount(void);

		//! \brief Removes a parameter using its name and type
		void removeParameter(const QString &name, PgSQLType type);

		//! \brief Removes a parameter using its index
		void removeParameter(unsigned param_idx);

		//! \brief Removes all the parameters from the function
		void removeParameters(void);

		//! \brief Removes a column from returned table using its index
		void removeReturnedTableColumn(unsigned column_idx);

		//! \brief Removes all the columns from returned table column
		void removeReturnedTableColumns(void);

		//! \brief Returns the complete function signature
		QString getSignature(void);

		/*! \brief Generates the function's signature. The 'format' parameter is used
		 to adequately format the function and parameters names. By default
		 this formating is always done. */
		void createSignature(bool format=true);

		//! \brief Returns the SQL / XML code definition for the function
		QString getCodeDefinition(unsigned def_type, bool reduced_form);
		QString getCodeDefinition(unsigned def_type);
};

#endif
