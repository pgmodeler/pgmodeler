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


/* Possible way to determine the order to import object from database:
 * 1) Order object by type (list below)
 * 2) For each type objects by name

ROLE						(pg_role)
TABLESPACE			(pg_tablespace)
DATABASE				(pg_database)
SCHEMA					(pg_namespace)
EXTENSION				(pg_extension)
USERTYPE				(pg_type)
FUNCTION				(pg_proc)
LANGUAGE				(pg_language) [user-defined]
FUNCTION				(pg_proc)			[user-defined]
AGGREGATE				(pg_aggregate)
OPERATOR				(pg_operator)
OPCLASS					(pg_opclass)
OPFAMILY				(pg_opfamily)
COLLATION				(pg_collation)
CONVERSION			(pg_conversion)
TABLE						(pg_tables)
COLUMN					(pg_attribute, pg_attrdef)
INDEX						(pg_index)
RULE						(pg_rule)
TRIGGER					(pg_trigger)
CONSTRAINT			(pg_constraints)
CAST						(pg_cast)
TABLE INHERITS	(pg_inherits)
VIEW						(pg_view)
PERMISSION			(???)

** Details can be found on pg_dump_sort.c line 82 **/

#include "connection.h"
#include "baseobject.h"

class Catalog {
	private:
		static const QString QUERY_LIST,	//! \brief Executes a list command on catalog
		QUERY_ATTRIBS, //! \brief Executes a attribute retrieving command on catalog
		CATALOG_SCH_DIR, //! \brief Default catalog schemas directory
		PGSQL_TRUE, //! \brief Replacement for true 't' boolean value
		PGSQL_FALSE; //! \brief Replacement for false 'f' boolean value

		//! \brief Connection used to query the pg_catalog
		Connection connection;

		/*! \brief Executes a query on the catalog for the specified object type. If the parameter 'single_result' is true
		the query will return only one tuple on the result set. Additional attributes can be passed so that SchemaParser will
		use them when parsing the schema file for the object */
		void executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result,
														 bool single_result=false, attribs_map attribs=attribs_map());

		/*! \brief Recreates the attribute map in such way that attribute names that have
		underscores have this char replaced by dashes */
		attribs_map changeAttributeNames(const attribs_map &attribs);

		//! \brief Returns a attribute set for the specified object type and name
		attribs_map getAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs=attribs_map());

	public:
		Catalog(void){}
		Catalog(Connection &connection);

		//! \brief Changes the current connection used by the catalog
		void setConnection(Connection &conn);

		//! \brief Returns the count for the specified object type
		unsigned getObjectCount(ObjectType obj_type, const QString &sch_name="");

		//! \brief Returns a list containing the names of the objects of the specified type
		vector<QString> getObjects(ObjectType obj_type, const QString &sch_name="");

		//! \brief Returns a set of multiple attributes (several tuples)
		vector<attribs_map> getMultipleAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		//! \brief Retrieve the attributes for the specified database
		attribs_map getDatabaseAttributes(const QString &db_name);

		//! \brief Retrieve the attributes for the specified role
		attribs_map getRoleAttributes(const QString &rol_name);

		//! \brief Retrieve the attributes for the specified schema
		attribs_map getSchemaAttributes(const QString &sch_name);

		//! \brief Retrieve the attributes for the specified tablespace
		attribs_map getTablespaceAttributes(const QString &spc_name);

		//! \brief Retrieve the attributes for the specified extension the parent's schema name must be informed
		attribs_map getExtensionAttributes(const QString &ext_name, const QString &sch_name);

		//! \brief Retrieve the attributes for the specified function
		//attribs_map getFunctionAttributes(const QString &func_name);
};

#endif
