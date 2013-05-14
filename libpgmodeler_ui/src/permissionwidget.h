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
\ingroup libpgmodeler_ui
\class PermissionWidget
\brief Implements the operations to create/edit permissions via form.
*/

#ifndef PERMISSION_WIDGET_H
#define PERMISSION_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_permissionwidget.h"
#include "objecttablewidget.h"
#include "modelobjectswidget.h"

class PermissionWidget: public BaseObjectWidget, public Ui::PermissionWidget {
	private:
		Q_OBJECT

		//! \brief Stores the permission to be edited
		Permission *permission;

		//! \brief Table widget that stores the roles that has permission over the object
		ObjectTableWidget *roles_tab,

													//! \brief Table widget that stores the permissions related to the object
												*permissions_tab;

		//! \brief Widget used to select roles on the database model
		ModelObjectsWidget *objectselection_wgt;

		/*! \brief Indicates if some permission was changed. This flag is used to know when
		emit the signal s_objectManipulated() */
		bool perms_changed;

		void hideEvent(QHideEvent *event);

	public:
		PermissionWidget(QWidget * parent = 0);
		~PermissionWidget(void);

		void setAttributes(DatabaseModel *model, BaseObject *objeto_pai, BaseObject *object);

	private slots:
		//! \brief Shows the object selection window
		void selectRole(void);

		//! \brief Select a permission which the index is specified on parameter
		void selectPermission(int perm_id);

		//! \brief Adds the configured permission onto the model
		void addPermission(void);

		//! \brief Configures a permission based upon the values assinged on the form
		void configurePermission(Permission *perm);

		//! \brief Fills the form with the selected permission attributes
		void editPermission(void);

		//! \brief Removes the selected permission
		void removePermission(void);

		//! \brief Removes all permissions from the permissions table
		void removePermissions(void);

		//! \brief Updates the attributes of the currently edited permission
		void updatePermission(void);

		//! \brief Lists all permissions related to the object
		void listPermissions(void);

		//! \brief Cancel de permission's creation/edition cleaning the form and blocking the edition buttons
		void cancelOperation(void);

		//! \brief Shows the selected role data on the role tables
		void showSelectedRoleData(void);

		void enableEditButtons(void);

		void checkPrivilege(void);

		void disableGrantOptions(void);
};

#endif
