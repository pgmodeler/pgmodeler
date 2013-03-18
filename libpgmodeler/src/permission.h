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
\class Permission
\brief Implements the operations to manipulate object permissions.
\note <strong>Creation date:</strong> 16/09/2010
*/

#ifndef PERMISSION_H
#define PERMISSION_H

#include "baseobject.h"
#include "function.h"
#include "role.h"
#include <algorithm>
#include <QTextStream>

class Permission: public BaseObject {
		/*! \brief Permissions on PostgreSQL are only applied to the following
				object type:

				* table
				* column
				* view
				* sequence
				* database
				* foreign-data wrapper (not implemented)
				* foreign server (not implemented)
				* large objects (not implemented)
				* function
				* aggregate
				* linguage
				* schema
				* tablespace */
	private:
		//! \brief Object which the permission is applied
		BaseObject *object;

		/*! \brief Roles that has permissions over the object. This vector can be
			empty indicating that all roles on the cluster has permission over
			the object. */
		vector<Role *> roles;

		//! \brief Privileges set applied to the object (Accessed via constants PRIV_???)
		bool privileges[13],

		//! \brief Indicates if the permission treats a REVOKE command.
		revoke,

		//! \brief Indicates if the revoke is applied in a cascade way.
		cascade;

		/*! \brief Indicates whether a privilege with given index can be assigned to other roles
			over the same object (WITH GRANT OPTION). This attribute is not applicable
			when there is no specified role (PUBLIC). This attribute is ignored
			when there is no defined role as holder of the privilege. */
		bool grant_option[13];

		/*! \brief Generates a unique identifier for permission using the attribute
			'name' of base class BaseObject. This is only used to avoid
			 duplicate permissions in the model */
		void generatePermissionId(void);

	public:
		//! \brief Constants used to reference the privileges
		static const unsigned PRIV_SELECT=0,
													PRIV_INSERT=1,
													PRIV_UPDATE=2,
													PRIV_DELETE=3,
													PRIV_TRUNCATE=4,
													PRIV_REFERENCES=5,
													PRIV_TRIGGER=6,
													PRIV_CREATE=7,
													PRIV_CONNECT=8,
													PRIV_TEMPORARY=9,
													PRIV_EXECUTE=10,
													PRIV_USAGE=11;

		/*! \brief In the constructor is required to specify which object will receive
		 the permissions this can not be changed after the object instance of
		 the class is created. */
		Permission(BaseObject *obj);

		//! \brief Adds a role that will have privileges over the object
		void addRole(Role *role);

		//! \brief Sets the state of one permission's privilege (Accessed via constants PRIV_???)
		void setPrivilege(unsigned priv_id, bool value, bool grant_op);

		void setRevoke(bool value);

		void setCascade(bool value);

		//! \brief Remove a role using its index
		void removeRole(unsigned role_idx);

		//! \brief Remove all roles from the permission
		void removeRoles(void);

		//! \brief Gets the role count associated to the permission
		unsigned getRoleCount(void);

		//! \brief Gets one role from permission using its index
		Role *getRole(unsigned role_idx);

		//! \brief Gets the object that is subject to the privileges
		BaseObject *getObject(void);

		//! \brief Gets the actual state of the GRANT OPTION for the given privilege
		bool getGrantOption(unsigned priv_id);

		//! \brief Gets the current state for the given privilege
		bool getPrivilege(unsigned priv_id);

		/*! \brief Returns a string containing all the privileges
		 configured as the internal format of permissions
		 as documented on PostgreSQL GRANT command */
		QString getPrivilegeString(void);

		//! \brief Indicates whether the role is present on the permission
		bool isRoleExists(Role *role);

		bool isRevoke(void);

		bool isCascade(void);

		//! \brief Returns if the passed object type accepts permission
		static bool objectAcceptsPermission(ObjectType obj_type);

		//! \brief Returns the SQL / XML definition for the permission
		QString getCodeDefinition(unsigned def_type);
};

#endif
