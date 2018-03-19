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
		vector<Role *> roles;

		//! \brief Defines the USING expression applied to queries referencing the table which policy is applied
		QString using_expr,

		//! \brief Defines the CHECK expression applied to INSERT/UPDATE queries
		check_expr;

		/*! \brief Indicates if the policy is retrictive. Restrictive policies are combined using AND operator by PostgreSQL
		 and reducing the set of records accessed by a query. By default, a policy is permissive. */
		bool restrictive,

		//! \brief Stores which commands (CMD_???) are affected by the policy
		affected_cms[4],

		//! \brief Stores which special roles (ROL_???) are affected by the policy
		special_roles[3];

	public:
		//! \brief Constants used to reference the commands affected by the policy
		static const unsigned CMD_SELECT=0,
		CMD_INSERT=1,
		CMD_UPDATE=2,
		CMD_DELETE=3;

		//! \brief Constants used to reference the special roles affected by the policy
		static const unsigned ROLE_CURRENT_USER=0,
		ROLE_SESSION_USER=1,
		ROLE_PUBLIC=2;

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

		//! \brief Defines if special (system roles - CURRENT_USER and SESSION_USER) should be affected by the policy
		void setSpecialRole(unsigned rol_id, bool value);

		//! \brief Returns if the special (system roles - CURRENT_USER and SESSION_USER) are affected by the policy
		bool isSpecialRoleSet(unsigned rol_id);

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

		virtual QString getAlterDefinition(BaseObject *object) final;
};

#endif
