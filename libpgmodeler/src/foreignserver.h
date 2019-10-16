/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\brief Implements the operations to manipulate foreign servers on the database.
*/

#ifndef FOREIGN_SERVER_H
#define FOREIGN_SERVER_H

#include "baseobject.h"
#include "foreigndatawrapper.h"

class ForeignServer: public BaseObject, public ForeignObject {
	private:
		//! \brief The foreign data wrapper which manages the server
		ForeignDataWrapper *fdata_wrapper;

		QString type, version;

	public:
		ForeignServer(void);

		void setType(const QString &type);
		void setVersion(const QString &version);
		void setForeignDataWrapper(ForeignDataWrapper *fdw);

		QString getType(void);
		QString getVersion(void);
		ForeignDataWrapper *getForeignDataWrapper(void);

		virtual QString getCodeDefinition(unsigned def_type);
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form);
		virtual QString getAlterDefinition(BaseObject *object);
};

#endif
