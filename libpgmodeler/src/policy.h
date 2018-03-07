/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class Policy
\brief Implements the operations to manipulate row level security (RLS).
*/

#ifndef POLICY_H
#define POLICY_H

#include "table.h"

class Policy : public BaseObject {
	private:
		//! \brief Table in which the policy is applied
		Table *table;

		/*! \brief Roles that has permissions over the object. This vector can be
			empty indicating that all roles on the cluster has permission over
			the object. */
		vector<Role *> roles;

		//! \brief Defines the USING expression applied to queries referencing the table which policy is applied
		QString using_expr,

		//! \brief Defines the CHECK expression applied to INSERT/UPDATE queries
		check_expr;

		/*! \brief Generates a unique identifier for policy using the attribute
			'name' of base class BaseObject. This is only used to avoid
			 duplicate permissions ids in the model */
		void generatePolicyId(void);

	public:
		//! \brief Constants used to reference the commands affected by the policy
		static const unsigned CMD_SELECT=1,
		CMD_INSERT=2,
		CMD_UPDATE=4,
		CMD_DELETE=8,
		CMD_ALL=15;

		Policy(Table *table);

		//! \brief Defines the commands (CMD_???) affected by the policy. The commands need to be specified via bit wise operation
		void setAffectedCommands(unsigned cmds);

		//! \brief Returns if the specified command (CMD_???) is affected
		bool isCommandAffected(unsigned cmd);

		//! \brief Defines the USING expresion of the policy
		void setUsingExpression(const QString &expr);

		//! \brief Returns the USING expresion of the policy
		QString getUsingExpression(void);

		//! \brief Defines the CHECK expresion of the policy
		void setCheckExpression(const QString &expr);

		//! \brief Returns the CHECK expresion of the policy
		QString getCheckExpression(void);

		//! \brief Adds a role that will have privileges over the object
		void addRole(Role *role);

		//! \brief Remove all roles from the policy
		void removeRoles(void);

		//! \brief Returns all the roles that is used by the policy
		vector<Role *> getRoles(void);

		//! \brief Returns the table which policy is applied
		Table *getTable(void);

		//! \brief Returns the SQL / XML definition for the policy
		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getSignature(bool format=false) final;

		virtual QString getDropDefinition(bool cascade) final;
};

#endif
