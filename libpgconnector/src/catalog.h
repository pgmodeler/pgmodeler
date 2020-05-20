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
\ingroup libpgconnector
\class Catalog
\brief \brief Implements the operations to read data from information_schema as well from pg_catalog schema.
This class is the basis for the reverse engineering feature.
*/

#ifndef CATALOG_H
#define CATALOG_H

#include "connection.h"
#include "baseobject.h"
#include "tableobject.h"
#include <QTextStream>
#include <QApplication>

class Catalog {
	private:
		SchemaParser schparser;

		static const QString QueryList,	//! \brief Executes a list command on catalog
		QueryAttribs, //! \brief Executes a attribute retrieving command on catalog
		PgSqlTrue, //! \brief Replacement for true 't' boolean value
		PgSqlFalse, //! \brief Replacement for false 'f' boolean value
		BoolField,     //! \brief Suffix for boolean fields.

		//! \brief Query used to retrieve extension objects.
		GetExtensionObjsSql,

		//! \brief This pattern matches the PostgreSQL array values in format [n:n]={a,b,c,d,...} or {a,b,c,d,...}
		ArrayPattern,

		//! \brief Holds a constant string used to mark invalid filter patterns
		InvFilterPattern,

		AliasPlaceholder;

		/*! \brief Stores in comma seperated way the oids of all objects created by extensions. This
		attribute is use when filtering objects that are created by extensions */
		QString ext_obj_oids;

		//! \brief Stores the name filters for each type of object. (See setObjectFilters())
		map<ObjectType, QStringList> obj_filters;

		/*! \brief This map stores the oid field name for each object type. The oid field name can be
		composed by the pg_[OBJECT_TYPE] table alias. Refer to catalog query schema files for details */
		static map<ObjectType, QString> oid_fields,

		/*! \brief This map stores the name field for each object type. Refer to catalog query schema files for details */
		name_fields,

		/*! \brief This map stores the oid field name that is used to check if the object (or its parent) is part of a extension
		(see getNotExtObjectQuery()). By default the attribute oid_fields is used instead for that purpose, but, for some objects,
		there are different fields that tells if the object (or its parent) is part of extension. */
		ext_oid_fields,

		/*! \brief This map stores the aliases that are used to reference the table (parent) on each table object catalog query.
		 * This is mainly used to force the filter of constraints/indexes/triggers/rules/policies in presence of one or more table
		 * filter (see setObjectFilter) */
		parent_aliases;

		//! \brief Store the cached catalog queries
		static attribs_map catalog_queries;

		//! \brief Connection used to query the pg_catalog
		Connection connection;

		//! \brief Stores the last system object identifier. This is used to filter system objects
		unsigned last_sys_oid,

		filter;

		//! \brief Indicates if the catalog must filter system objects
		bool exclude_sys_objs,

		//! \brief Indicates if the catalog must filter extension created objects
		exclude_ext_objs,

		//! \brief Indicates if the catalog must filter built-in array data types
		exclude_array_types,

		//! \brief Indicates if the catalog must list only system objects
		list_only_sys_objs;

		/*! \brief Load the schema parser buffer with the catalog query using identified by qry_id.
		The method will cache the catalog query if it's not cached yet (only when use_cached_queries=true) */
		void loadCatalogQuery(const QString &qry_id);

		/*! \brief Executes a query on the catalog for the specified object type. If the parameter 'single_result' is true
		the query will return only one tuple on the result set. Additional attributes can be passed so that SchemaParser will
		use them when parsing the schema file for the object. A special extra attribute is accepted but not passed to SchemaParser:
		ParsersAttributes::CUSTOM_FILTER that will be appended to the current filter expression */
		void executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, bool single_result=false, attribs_map attribs=attribs_map());

		//! \brief Returns the catalog query according to the type of the object type provided
		QString getCatalogQuery(const QString &qry_type, ObjectType obj_type, bool single_result=false, attribs_map attribs=attribs_map());

		/*! \brief Recreates the attribute map in such way that attribute names that have
		underscores have this char replaced by dashes. Another special operation made is to replace
		the values of fiels which suffix is _bool to '1' when 't' and to empty when 'f', this is because
		the resultant attribs_map will be passed to XMLParser/SchemaParser which understands bool values as 1 (one) or '' (empty) */
		attribs_map changeAttributeNames(const attribs_map &attribs);

		//! \brief Returns a attribute set for the specified object type and name
		attribs_map getAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		/*! \brief Returns the query to retrieve the information if the object (specified by its oid field) is part or not of
		 a extension. */
		QString getNotExtObjectQuery(const QString &oid_field);

		/*! \brief Returns the query that is used to retrieve an objects comment. The 'is_shared_object' is used
		to query the pg_shdescription instead of pg_description */
		QString getCommentQuery(const QString &oid_field, bool is_shared_obj=false);

		//! \brief Creates a comma separated string containing all the oids to be filtered
		QString createOidFilter(const vector<unsigned> &oids);

	public:
		Catalog();
		Catalog(const Catalog &catalog);

		//! \brief Stores the prefix of any temp object (in pg_temp) created during catalog reading by pgModeler
		static const QString PgModelerTempDbObj,

		//! \brief Indicates the ilike/wildcard filtering mode in the object listing
		FilterLike,

		//! \brief Indicates the regexp (POSIX) filtering mode in the object listing
		FilterRegExp,

		//! \brief Indicates the exact match filtering mode in the object listing
		FilterExact;

		//! \brief Indicates the character used to separate filter fields in the filtering string
		static const constexpr char FilterSeparator = ':';

		//! \brief Excludes the system objects from listing
		static constexpr unsigned ExclSystemObjs=1,

		//! \brief Excludes the extension generated objects from listing
		ExclExtensionObjs=2,

		//! \brief Excludes the builtin array types.
		ExclBuiltinArrayTypes=4,

		/*! \brief Shows only system objects. Using this filter will disable the other two filters.
		Using this filter implies the listing of extension objects */
		ListOnlySystemObjs=8,

		//! \brief Shows all objects including system objects and extension object.
		ListAllObjects=16;

		//! \brief Changes the current connection used by the catalog
		void setConnection(Connection &conn);

		/*! \brief Closes the connection used by the catalog.
	Once this method is called the user must call setConnection() again or the
	catalog queries will fail */
		void closeConnection();

		//! \brief Configures the catalog query filter
		void setQueryFilter(unsigned filter);

		/*! \brief Configures the objects name filtering.
		 * The parameter only_matching creates extra filters for the other kind of objects not provided by the user in order to avoid listing them.
		 * The tab_obj_types contains a list of table children object type names in which should be forcibly listed
		 * if the user provides table/view/foreign table filters. This is useful to retrieve tables with their children objects avoiding the need of
		 * provide specific filters for each table children object. This list has effect only when discard_non_matches is set to true.
		 * This method raises an exception when detecting malformed filters */
		void setObjectFilters(QStringList filters, bool only_matching, QStringList tab_obj_types = {});

		//! \brief Returns the last system object oid registered on the database
		unsigned getLastSysObjectOID();

		//! \brief Returns if the specified oid is amongst the system objects' oids
		bool isSystemObject(unsigned oid);

		//! \brief Returns if the specified oid is amongst the extension created objects' oids
		bool isExtensionObject(unsigned oid);

		/*! \brief Returns the count for the specified object type. A schema name can be specified
		in order to filter only objects of the specifed schema */
		unsigned getObjectCount(ObjectType obj_type, const QString &sch_name=QString(), const QString &tab_name=QString(), attribs_map extra_attribs=attribs_map());

		//! \brief Returns the current filter configuration for the catalog queries
		unsigned getQueryFilter();

		//! \brief Returns the configured objects a name filters
		map<ObjectType, QStringList> getObjectFilters();

		/*! \brief Returns a vector with all filtered object types.
		 * Invalid pattern filters containing the InvFilterPattern are discarded from the returning vector */
		vector<ObjectType> getFilteredObjectTypes();

		//! \brief Fills the specified maps with all object's oids querying the catalog with the specified filter
		void getObjectsOIDs(map<ObjectType, vector<unsigned> > &obj_oids, map<unsigned, vector<unsigned> > &col_oids, attribs_map extra_attribs=attribs_map());

		/*! \brief Returns a attributes map containing the oids (key) and names (values) of the objects from
		the specified type.	A schema name can be specified in order to filter only objects of the specifed schema */
		attribs_map getObjectsNames(ObjectType obj_type, const QString &sch_name=QString(), const QString &tab_name=QString(), attribs_map extra_attribs=attribs_map());

		/*! \brief Returns a vector of attributes map containing the oids (key) and names as well types of the objects from
		the specified list of types.	A schema name can be specified in order to filter only objects of the specifed schema */
		vector<attribs_map> getObjectsNames(vector<ObjectType> obj_types, const QString &sch_name=QString(), const QString &tab_name=QString(), attribs_map extra_attribs=attribs_map(), bool sort_results=false);

		//! \brief Returns a set of multiple attributes (several tuples) for the specified object type
		vector<attribs_map> getMultipleAttributes(ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		/*! \brief Returns a set of multiple attributes (several tuples) for the specified catalog schema file.
		 * This version of the method differs from the one in which the user need to provide the object type.
		 * This one, the user is responsible to provide all attributes that will be parsed together with the
		 * catalog file. */
		vector<attribs_map> getMultipleAttributes(const QString &catalog_sch, attribs_map attribs=attribs_map());

		/*! \brief Retrieve all available objects attributes for the specified type. Internally this method calls the get method for the
		specified type. User can filter items by oids (except for table child objects), by schema (in the object type is suitable to accept schema)
		and by table name (only when retriving child objects for a specific table) */
		vector<attribs_map> getObjectsAttributes(ObjectType obj_type, const QString &schema=QString(), const QString &table=QString(), const vector<unsigned> &filter_oids={}, attribs_map extra_attribs=attribs_map());

		//! \brief Returns the attributes for the object specified by its type and OID
		attribs_map getObjectAttributes(ObjectType obj_type, unsigned oid, const QString sch_name=QString(), const QString tab_name=QString(), attribs_map extra_attribs=attribs_map());

		/*! \brief Returns the OID of the named object. User can filter items by schema (if the object type is suitable to accept schema)
		and by table name (only when retriving child objects for a specific table). The method will raise an exception if the catalog query
		used returns more than one result. A zero OID is returned when no suitable object is found. */
		QString getObjectOID(const QString &name, ObjectType obj_type, const QString &schema = QString(), const QString &table = QString());

		//! \brief This special method returns some server's attributes read from pg_settings
		attribs_map getServerAttributes();

		/*! \brief This special method returns the amount of object in pg_class table.
		 * The parameter incl_sys_objs will also count the system objects not only used created ones */
		unsigned getObjectCount(bool incl_sys_objs);

		//! \brief Parse a PostgreSQL array value and return the elements in a string list
		static QStringList parseArrayValues(const QString &array_val);

		/*! \brief Parse a function's default value and return the elements in a string list.
		It can be specified the string delimiter as well the value separator if the input default value
		contains several values */
		static QStringList parseDefaultValues(const QString &def_vals, const QString &str_delim=QString("'"), const QString &val_sep=QString(", "));

		//! \brief Parse the raw commands of a rule retrieved by the catalog and returns only the relevant parts
		static QStringList parseRuleCommands(const QString &cmd);

		/*! \brief Parse a set of expressions related to an index returned by the pg_get_expr(oid) and separates
		 * them as a string list. */
		static QStringList parseIndexExpressions(const QString &expr);

		/*! \brief Enable/disable the use of cached catalog queries. When enabled, the schema files read for the first are stored in memory
		so in the next time the same catalog query must be used it'll be read right from the memory and not from the disk anymore */
		static void enableCachedQueries(bool value);

		//! \brief Returns the current status of cached catalog queries
		static bool isCachedQueriesEnabled();

		//! \brief Returns the object types that are able to be filtered
		static vector<ObjectType> getFilterableObjectTypes();

		//! \brief Returns the object schema names that are able to be filtered
		static QStringList getFilterableObjectNames();

		//! \brief Performs the copy between two catalogs
		void operator = (const Catalog &catalog);
};

#endif
