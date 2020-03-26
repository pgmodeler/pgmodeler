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
\class Role
\brief Implements the operations to manipulate roles on the database.
\note <strong>Creation date:</strong> 12/05/2008
*/

#ifndef ROLE_H
#define ROLE_H

#include "baseobject.h"

class Role: public BaseObject {
	private:
		static unsigned role_id;

		/*! \brief Options for the role (SUPERUSER, CREATEDB, CREATEROLE,
		 INHERIT, LOGIN, ENCRYPTED, REPLICATION, BYPASSRLS) */
		bool options[8];

		//! \brief Connection limit for the role
		int conn_limit;

		//! \brief Validity date for the role
		QString validity,

		//! \brief Authentication password
		password;

		//! \brief Roles that has the 'this' role as member
		vector<Role *> ref_roles, //! \brief IN ROLE

		//! \brief Member roles of 'this' role
		member_roles, //! \brief ROLE

		//! \brief Member roles of 'this' role whit admin privileges
		admin_roles; //! \brief ADMIN

		//! \brief Formats the role attribute to be used by the SchemaParser
		void setRoleAttribute(unsigned role_type);

	public:
		//! \brief Constants used to reference the available options for the role
		static constexpr unsigned OpSuperuser=0,
		OpCreateDb=1,
		OpCreateRole=2,
		OpInherit=3,
		OpLogin=4,
		OpEncrypted=5,
		OpReplication=6,
		OpBypassRls=7;

		//! \brief Constants used to reference the internal role lists of the class
		static constexpr unsigned RefRole=10,
		MemberRole=20,
		AdminRole=30;

		Role();

		//! \brief Sets one option for the role (Via OP_??? constants)
		void setOption(unsigned op_type, bool value);

		//! \brief Adds one role to the internal role list (Via ???_ROLE constants)
		void addRole(unsigned role_type, Role *role);

		//! \brief Defines the connection limit for the role
		void setConnectionLimit(int limit);

		//! \brief Defines the validity date for the role
		void setValidity(const QString &date);

		//! \brief Sets the password for the role
		void setPassword(const QString &passwd);

		//! \brief Gets on option for the role (Via OP_??? constants)
		bool getOption(unsigned op_type);

		//! \brief Remove one role from internal role list (Via ???_ROLE constants)
		void removeRole(unsigned role_type, unsigned role_idx);

		//! \brief Remove all roles from one iternal list (Via ???_ROLE constants)
		void removeRoles(unsigned role_type);

		/*! \brief Gets one role from internal list (Via ???_ROLE constants) referencing
		 the object by its index */
		Role *getRole(unsigned role_type, unsigned role_idx);

		//! \brief Returns whether the role exists on the internal lists (Via ???_ROLE constants)
		bool isRoleExists(unsigned role_type, Role *role);

		//! \brief Gets the role count on the specified internal list (Via ???_ROLE constants)
		unsigned getRoleCount(unsigned role_type);

		//! \brief Returns the connection limit for the role
		unsigned getConnectionLimit();

		//! \brief Returns the validity date for the role
		QString getValidity();

		//! \brief Returns the role password
		QString getPassword();

		//! \brief Returns the SQL / XML definition for the role
		virtual QString getCodeDefinition(unsigned def_type) final;
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;

		virtual QString getAlterDefinition(BaseObject *object, bool ignore_name_diff=false) final;
};

#endif
