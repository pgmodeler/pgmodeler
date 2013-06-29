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
\ingroup libpgconnector
\class Catalog
\brief \brief Implements the operations to read data from information_schema as well from pg_catalog schema.
This class is the basis for the reverse engineering feature.
*/

#ifndef CATALOG_H
#define CATALOG_H

#include "connection.h"
#include "baseobject.h"

class Catalog {
	private:
		static const QString QUERY_LIST,
												 QUERY_COUNT,
												 QUERY_ATTRIBS,
												 CATALOG_SCH_DIR;

		Connection connection;

		void executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, map<QString, QString> attribs=map<QString, QString>());

	public:
		Catalog(void){}

		Catalog(Connection &connection);
		void setConnection(Connection &conn);

		unsigned getObjectCount(ObjectType obj_type);
		vector<QString> getObjects(ObjectType obj_type);
		vector<map<QString, QString> > getObjectAttributes(const QString &obj_name, ObjectType obj_type, map<QString, QString> extra_attribs=map<QString,QString>());

		/*unsigned getDatabaseCount(void);
		vector<QString> getDatabases(void);
		map<QString, QString> getDatabaseAttributes(const QString &db_name);

		unsigned getRoleCount(void);
		vector<QString> getRoles(void);
		map<QString, QString> getRoleAttributes(const QString &rol_name);*/
};

#endif
