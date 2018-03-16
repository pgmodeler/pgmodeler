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

#include "tableobject.h"
#include "role.h"

class Policy : public TableObject {
	private:
		/*! \brief Roles that has permissions over the object. This vector can be
			empty indicating that all roles on the cluster has permission over
			the object. */
		vector<BaseObject *> roles;

		//! \brief Defines the USING expression applied to queries referencing the table which policy is applied
		QString using_expr,

		//! \brief Defines the CHECK expression applied to INSERT/UPDATE queries
		check_expr;

		/*! \brief Indicates if the policy is retrictive. Restrictive policies are combined using AND operator by PostgreSQL
		 and reducing the set of records accessed by a query. By default, a policy is permissive. */
		bool restrictive,

		affected_cms[4];

		/*! \brief Generates a unique identifier for policy using the attribute
			'name' of base class BaseObject. This is only used to avoid
			 duplicate permissions ids in the model */
		void generatePolicyId(void);

	public:
		//! \brief Constants used to reference the commands affected by the policy
		static const unsigned CMD_SELECT=0,
		CMD_INSERT=1,
		CMD_UPDATE=2,
		CMD_DELETE=3;

		Policy(void);

		virtual void setParentTable(BaseTable *table) final;

		//! \brief Defines if the the policy is retrictive (true) or permissive (false)
		void setRestrictive(bool value);

		//! \brief Returns if the policy is restrictive (true) or permissive (false)
		bool isRestrictive(void);

		//! \brief Defines the commands (CMD_???) affected by the policy. The commands need to be specified via bit wise operation
		void setAffectedCommand(unsigned cmd, bool value);

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

		//! \brief Returns the SQL / XML definition for the policy
		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getSignature(bool format=false) final;

		virtual QString getDropDefinition(bool cascade) final;

		virtual QString getAlterDefinition(BaseObject *object) final;
};

#endif
