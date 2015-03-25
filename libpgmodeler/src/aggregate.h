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
\ingroup libpgmodeler
\class Aggregate
\brief Implements the operations to manipulate aggregates on the database.
\note <strong>Creation date:</strong> 16/04/2008
**/

#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "baseobject.h"
#include "function.h"
#include "operator.h"

class LIBPGMODELER_EXPORT Aggregate: public BaseObject {
	private:
		/*! \brief List of types with which the aggregate operates.
		 If it is empty will be considered all possible types '*'

		 To maintain compatibility with the old syntax,
		 where the aggregate function accepts only one data type,
		 the list of types 'data_types' must have only one element.
		 To declare an aggregate function which works with several
		 types in the old syntax, the only element of the list must
		 be of type 'any' */
		vector<PgSQLType> data_types;

		/*! \brief Function that defines the aggregate behavior
		 0 -> Final function
		 1 -> Transition function */
		Function *functions[2];

		//! \brief Data type used as aggregate's state
		PgSQLType state_type;

		//! \brief Initial condition for the aggregate
		QString initial_condition;

		//! \brief Sort operator used by the aggregate
		Operator *sort_operator;

		//! \brief Formats the data types to be used as attribute by the SchemaParser
		void setTypesAttribute(unsigned def_type);

		//! \brief Checks if the passed function is valid according to the rule of aggregate definition
		bool isValidFunction(unsigned func_idx, Function *func);

	public:
		//! \brief Constants used to reference the functions used by the aggregate
		static const unsigned FINAL_FUNC=0;
		static const unsigned TRANSITION_FUNC=1;

		Aggregate(void);

		//! \brief Defines one of the functions used by the aggregate
		void setFunction(unsigned func_idx, Function *func);

		//! \brief Defines the state data type of the aggregate
		void setStateType(PgSQLType state_type);

		//! \brief Defines the initial condition for the aggregate
		void setInitialCondition(const QString &cond);

		//! \brief Defines the sort operator used by the aggregate
		void setSortOperator(Operator *sort_op);

		//! \brief Adds a data type in the group that is accepted by the aggregate
		void addDataType(PgSQLType type);

		//! \brief Removes one aggregate accepted data type
		void removeDataType(unsigned type_idx);

		//! \brief Removes all accepted data types from aggregate
		void removeDataTypes(void);

		//! \brief Checks whether the passed type exists in the aggregate's set of data types
		bool isDataTypeExist(PgSQLType type);

		Function *getFunction(unsigned func_idx);
		PgSQLType getStateType(void);
		QString getInitialCondition(void);
		Operator *getSortOperator(void);
		PgSQLType getDataType(unsigned type_idx);
		unsigned getDataTypeCount(void);

		//! \brief Returns the SQL / XML code definition for the aggregate
		virtual QString getCodeDefinition(unsigned def_type) final;

    virtual QString getDropDefinition(bool cascade) final;

    virtual QString getAlterDefinition(BaseObject *object) final;

    virtual QString getSignature(bool format=true) final;
};

#endif
