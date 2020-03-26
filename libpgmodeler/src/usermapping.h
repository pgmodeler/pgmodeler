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
\class Server
\brief Implements the operations to manipulate user mappings on the database.
*/

#ifndef USER_MAPPING_H
#define USER_MAPPING_H

#include "baseobject.h"
#include "foreignserver.h"

class UserMapping: public BaseObject, public ForeignObject {
	private:
		//! \brief The foreign server which is managed by this user mapping the server
		ForeignServer *foreign_server;

	public:
		UserMapping();

		void setForeignServer(ForeignServer *server);
		ForeignServer *getForeignServer();

		virtual void setName(const QString &);
		virtual void setOwner(BaseObject *role);
		virtual QString getName(bool = false, bool = false);
		virtual QString getSignature(bool = false);
		virtual QString getCodeDefinition(unsigned def_type);
		virtual QString getAlterDefinition(BaseObject *object);
		virtual QString getDropDefinition(bool);
};

#endif
