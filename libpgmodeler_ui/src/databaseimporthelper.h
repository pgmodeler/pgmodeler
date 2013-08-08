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
\class DatabaseImportHelper
\brief Implements the base operations to import existing databases into model (reverse engineering)
*/

#ifndef DATABASE_IMPORT_HELPER_H
#define DATABASE_IMPORT_HELPER_H

#include <QObject>
#include "catalog.h"

class DatabaseImportHelper: public QObject {
	private:
		Q_OBJECT

		Catalog catalog;

		Connection connection;

	public:
		DatabaseImportHelper(QObject *parent=0);

		void setConnection(Connection &conn);

		void setCurrentDatabase(const QString &dbname);

		/*! \brief Returns an attribute map for the specified object type. The parameters "schema" and "table"
				must be used only when retrieving table children objects.
				\note: The database used as reference is the same as the currently connection. So,
				if the user want a different database it must call Connection::switchToDatabase() method
				before assigne the connection to this class. */
		attribs_map getObjects(ObjectType obj_type, const QString schema="", const QString table="");

		//void importObjects(vector<QString> &oids);

	signals:
		
	public slots:
		
};

#endif
