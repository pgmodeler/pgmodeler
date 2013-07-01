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
												 QUERY_ATTRIBS,
												 CATALOG_SCH_DIR;

		Connection connection;

		void executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, attribs_map attribs=attribs_map());
		attribs_map changeAttributeNames(const attribs_map &attribs);

	public:
		Catalog(void){}

		Catalog(Connection &connection);
		void setConnection(Connection &conn);

		unsigned getObjectCount(ObjectType obj_type);
		vector<QString> getObjects(ObjectType obj_type);
		vector<attribs_map> getObjectAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		attribs_map getDatabaseAttributes(const QString &db_name);
		attribs_map getRoleAttributes(const QString &rol_name);
		//attrib_map getSchemaAttributes(const QString &sch_name);
		//attrib_map getFunctionAttributes(const QString &func_name);
};

#endif
