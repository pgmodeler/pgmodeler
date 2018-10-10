/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "catalog.h"

const QString Catalog::QUERY_LIST=QString("list");
const QString Catalog::QUERY_ATTRIBS=QString("attribs");
const QString Catalog::CATALOG_SCH_DIR=QString("catalog");
const QString Catalog::PGSQL_TRUE=QString("t");
const QString Catalog::PGSQL_FALSE=QString("f");
const QString Catalog::BOOL_FIELD=QString("_bool");
const QString Catalog::ARRAY_PATTERN=QString("((\\[)[0-9]+(\\:)[0-9]+(\\])=)?(\\{)((.)+(,)*)*(\\})$");
const QString Catalog::GET_EXT_OBJS_SQL=QString("SELECT objid AS oid FROM pg_depend WHERE objid > 0 AND refobjid > 0 AND deptype='e'");
const QString Catalog::PGMODELER_TEMP_DB_OBJ=QString("__pgmodeler_tmp");

attribs_map Catalog::catalog_queries;

map<ObjectType, QString> Catalog::oid_fields=
{ {OBJ_DATABASE, "oid"}, {OBJ_ROLE, "oid"}, {OBJ_SCHEMA,"oid"},
  {OBJ_LANGUAGE, "oid"}, {OBJ_TABLESPACE, "oid"}, {OBJ_EXTENSION, "ex.oid"},
  {OBJ_FUNCTION, "pr.oid"}, {OBJ_AGGREGATE, "pr.oid"}, {OBJ_OPERATOR, "op.oid"},
  {OBJ_OPCLASS, "op.oid"}, {OBJ_OPFAMILY, "op.oid"}, {OBJ_COLLATION, "cl.oid"},
  {OBJ_CONVERSION, "cn.oid"}, {OBJ_CAST, "cs.oid"}, {OBJ_VIEW, "vw.oid"},
  {OBJ_SEQUENCE, "sq.oid"}, {OBJ_DOMAIN, "dm.oid"}, {OBJ_TYPE, "tp.oid"},
  {OBJ_TABLE, "tb.oid"}, {OBJ_COLUMN, "cl.oid"}, {OBJ_CONSTRAINT, "cs.oid"},
	{OBJ_RULE, "rl.oid"}, {OBJ_TRIGGER, "tg.oid"}, {OBJ_INDEX, "id.indexrelid"},
	{OBJ_EVENT_TRIGGER, "et.oid"}, {OBJ_POLICY, "pl.oid"}
};

map<ObjectType, QString> Catalog::ext_oid_fields={
	{OBJ_CONSTRAINT, "cs.conrelid"},
	{OBJ_INDEX, "id.indexrelid"},
	{OBJ_TRIGGER, "tg.tgrelid"},
	{OBJ_RULE, "rl.ev_class"},
	{OBJ_POLICY, "pl.polrelid"}
};

map<ObjectType, QString> Catalog::name_fields=
{ {OBJ_DATABASE, "datname"}, {OBJ_ROLE, "rolname"}, {OBJ_SCHEMA,"nspname"},
	{OBJ_LANGUAGE, "lanname"}, {OBJ_TABLESPACE, "spcname"}, {OBJ_EXTENSION, "extname"},
	{OBJ_FUNCTION, "proname"}, {OBJ_AGGREGATE, "proname"}, {OBJ_OPERATOR, "oprname"},
	{OBJ_OPCLASS, "opcname"}, {OBJ_OPFAMILY, "opfname"}, {OBJ_COLLATION, "collname"},
	{OBJ_CONVERSION, "conname"}, {OBJ_CAST, ""}, {OBJ_VIEW, "relname"},
	{OBJ_SEQUENCE, "relname"}, {OBJ_DOMAIN, "typname"}, {OBJ_TYPE, "typname"},
	{OBJ_TABLE, "relname"}, {OBJ_COLUMN, "attname"}, {OBJ_CONSTRAINT, "conname"},
	{OBJ_RULE, "rulename"}, {OBJ_TRIGGER, "tgname"}, {OBJ_INDEX, "relname"},
	{OBJ_EVENT_TRIGGER, "evtname"}, {OBJ_POLICY, "polname"}
};

Catalog::Catalog(void)
{
	last_sys_oid=0;
	setFilter(EXCL_EXTENSION_OBJS | EXCL_SYSTEM_OBJS);
}

Catalog::Catalog(const Catalog &catalog)
{
	(*this)=catalog;
}

void Catalog::setConnection(Connection &conn)
{
	try
	{
		ResultSet res;
		QStringList ext_obj;

		connection.close();
		connection.setConnectionParams(conn.getConnectionParams());
		connection.connect();

		//Retrieving the last system oid
		executeCatalogQuery(QUERY_LIST, OBJ_DATABASE, res, true,
		{{ParsersAttributes::NAME, conn.getConnectionParam(Connection::PARAM_DB_NAME)}});

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			attribs_map attribs=changeAttributeNames(res.getTupleValues());
			last_sys_oid=attribs[ParsersAttributes::LAST_SYS_OID].toUInt();
		}

		//Retrieving the list of objects created by extensions
		this->connection.executeDMLCommand(GET_EXT_OBJS_SQL, res);
		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				ext_obj.push_back(res.getColumnValue(QString("oid")));
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));

			ext_obj_oids=ext_obj.join(',');
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Catalog::closeConnection(void)
{
	connection.close();
}

void Catalog::setFilter(unsigned filter)
{
	bool list_all=(LIST_ALL_OBJS & filter) == LIST_ALL_OBJS;

	this->filter=filter;
	list_only_sys_objs=false;
	exclude_array_types=(EXCL_BUILTIN_ARRAY_TYPES & filter) == EXCL_BUILTIN_ARRAY_TYPES;
	exclude_ext_objs=(EXCL_EXTENSION_OBJS & filter) == EXCL_EXTENSION_OBJS;
	exclude_sys_objs=(EXCL_SYSTEM_OBJS & filter) == EXCL_SYSTEM_OBJS;

	if(!list_all)
	{
		list_only_sys_objs=(LIST_ONLY_SYS_OBJS & filter) == LIST_ONLY_SYS_OBJS;

		if(list_only_sys_objs)
		{
			exclude_ext_objs=true;
			exclude_sys_objs=false;
		}
	}
}

unsigned Catalog::getLastSysObjectOID(void)
{
	return(last_sys_oid);
}

bool Catalog::isSystemObject(unsigned oid)
{
	return(oid <= last_sys_oid);
}

bool Catalog::isExtensionObject(unsigned oid)
{
	return(ext_obj_oids.contains(QString::number(oid)));
}

void Catalog::loadCatalogQuery(const QString &qry_id)
{
	if(catalog_queries.count(qry_id)==0)
	{
		QFile input;
		input.setFileName(GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
							CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
							qry_id + GlobalAttributes::SCHEMA_EXT);

		if(!input.open(QFile::ReadOnly))
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(input.fileName()),
							ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		catalog_queries[qry_id]=QString(input.readAll());
		input.close();
	}

	schparser.loadBuffer(catalog_queries[qry_id]);
}

QString Catalog::getCatalogQuery(const QString &qry_type, ObjectType obj_type, bool single_result, attribs_map attribs)
{
	QString sql, custom_filter;

	/* Escaping apostrophe (') in the attributes values to avoid SQL errors
	 * due to support to this char in the middle of objects' names */
	for(auto &attr : attribs)
	{
		if(attr.first != ParsersAttributes::CUSTOM_FILTER && attr.second.contains(QChar('\'')))
			attr.second.replace(QChar('\''), QString("''"));
	}

	schparser.setPgSQLVersion(connection.getPgSQLVersion(true));
	attribs[qry_type]=ParsersAttributes::_TRUE_;

	if(exclude_sys_objs || list_only_sys_objs)
		attribs[ParsersAttributes::LAST_SYS_OID]=QString("%1").arg(last_sys_oid);

	if(list_only_sys_objs)
		attribs[ParsersAttributes::OID_FILTER_OP]=QString("<=");
	else
		attribs[ParsersAttributes::OID_FILTER_OP]=QString(">");

	if(obj_type==OBJ_TYPE && exclude_array_types)
		attribs[ParsersAttributes::EXC_BUILTIN_ARRAYS]=ParsersAttributes::_TRUE_;

	//Checking if the custom filter expression is present
	if(attribs.count(ParsersAttributes::CUSTOM_FILTER))
	{
		custom_filter=attribs[ParsersAttributes::CUSTOM_FILTER];
		attribs.erase(ParsersAttributes::CUSTOM_FILTER);
	}

	if(exclude_ext_objs && obj_type!=OBJ_DATABASE &&	obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE && obj_type!=OBJ_EXTENSION)
	{
		if(ext_oid_fields.count(obj_type)==0)
			attribs[ParsersAttributes::NOT_EXT_OBJECT]=getNotExtObjectQuery(oid_fields[obj_type]);
		else
			attribs[ParsersAttributes::NOT_EXT_OBJECT]=getNotExtObjectQuery(ext_oid_fields[obj_type]);
	}

	loadCatalogQuery(BaseObject::getSchemaName(obj_type));
	schparser.ignoreUnkownAttributes(true);
	schparser.ignoreEmptyAttributes(true);

	attribs[ParsersAttributes::PGSQL_VERSION]=schparser.getPgSQLVersion();
	sql=schparser.getCodeDefinition(attribs).simplified();

	//Appeding the custom filter to the whole catalog query
	if(!custom_filter.isEmpty())
	{
		int order_by_idx = sql.indexOf(QString("ORDER BY"), 0, Qt::CaseInsensitive);

		if(order_by_idx < 0)
			order_by_idx = sql.length();

		if(!sql.contains(QString("WHERE"), Qt::CaseInsensitive))
			sql.insert(order_by_idx, QString(" WHERE ") + custom_filter);
		else
			sql.insert(order_by_idx, QString(" AND (%1) ").arg(custom_filter));
	}

	//Append a LIMIT clause when the single_result is set
	if(single_result)
	{
		if(sql.endsWith(';'))	sql.remove(sql.size()-1, 1);
		sql+=QString(" LIMIT 1");
	}

	return(sql);
}

void Catalog::executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, bool single_result, attribs_map attribs)
{
	try
	{
		connection.executeDMLCommand(getCatalogQuery(qry_type, obj_type, single_result, attribs), result);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(BaseObject::getSchemaName(obj_type)));
	}
}

unsigned Catalog::getObjectCount(ObjectType obj_type, const QString &sch_name, const QString &tab_name, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;

		extra_attribs[ParsersAttributes::SCHEMA]=sch_name;
		extra_attribs[ParsersAttributes::TABLE]=tab_name;

		executeCatalogQuery(QUERY_LIST, obj_type, res, false, extra_attribs);
		res.accessTuple(ResultSet::FIRST_TUPLE);
		return(res.getTupleCount());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned Catalog::getFilter(void)
{
	return(filter);
}

void Catalog::getObjectsOIDs(map<ObjectType, vector<unsigned> > &obj_oids, map<unsigned, vector<unsigned> > &col_oids, attribs_map extra_attribs)
{
	try
	{
		vector<ObjectType> types=BaseObject::getObjectTypes(true, { OBJ_DATABASE, OBJ_RELATIONSHIP, BASE_RELATIONSHIP,
																																OBJ_TEXTBOX, OBJ_TAG, OBJ_COLUMN, OBJ_PERMISSION,
																																OBJ_GENERIC_SQL });
		attribs_map attribs, col_attribs, sch_names;
		vector<attribs_map> tab_attribs;
		unsigned tab_oid=0;

		for(ObjectType type : types)
		{
			attribs=getObjectsNames(type, QString(), QString(), extra_attribs);

			for(auto &attr : attribs)
			{
				obj_oids[type].push_back(attr.first.toUInt());

				//Store the schemas names in order to retrieve the tables' columns correctly
				if(type==OBJ_SCHEMA)
					sch_names[attr.first]=attr.second;
				else if(type==OBJ_TABLE)
				{
					//Get the full set of attributes of the table
					tab_oid=attr.first.toUInt();
					tab_attribs=getObjectsAttributes(type, QString(), QString(), { tab_oid });

					//Retrieve the oid and names of the table's columns
					col_attribs=getObjectsNames(OBJ_COLUMN, sch_names[tab_attribs[0][ParsersAttributes::SCHEMA]], attr.second);

					for(auto &col_attr : col_attribs)
						col_oids[tab_oid].push_back(col_attr.first.toUInt());
				}
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getObjectsNames(ObjectType obj_type, const QString &sch_name, const QString &tab_name, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map objects;

		extra_attribs[ParsersAttributes::SCHEMA]=sch_name;
		extra_attribs[ParsersAttributes::TABLE]=tab_name;
		executeCatalogQuery(QUERY_LIST, obj_type, res, false, extra_attribs);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				objects[res.getColumnValue(ParsersAttributes::OID)]=res.getColumnValue(ParsersAttributes::NAME);
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		return(objects);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<attribs_map> Catalog::getObjectsNames(vector<ObjectType> obj_types, const QString &sch_name, const QString &tab_name, attribs_map extra_attribs, bool sort_results)
{
	try
	{
		ResultSet res;
		vector<attribs_map> objects;
		QString sql, select_kw=QString("SELECT");
		QStringList queries;
		attribs_map attribs;

		extra_attribs[ParsersAttributes::SCHEMA]=sch_name;
		extra_attribs[ParsersAttributes::TABLE]=tab_name;

		for(ObjectType obj_type : obj_types)
		{
			//Build the catalog query for the specified object type
			sql=getCatalogQuery(QUERY_LIST, obj_type, false, extra_attribs);

			/* For certain objects the catalog query will be empty due to the
			absence of that kind of element in the version of the database.
			E.g.: Event triggers does not exists in PgSQL < 9.3 */
			if(!sql.isEmpty())
			{
				//Injecting the object type integer code in order to sort the final result
				sql.replace(sql.indexOf(select_kw), select_kw.size(),
										QString("%1 %2 AS object_type, ").arg(select_kw).arg(obj_type));

				sql+=QChar('\n');
				queries.push_back(sql);
			}
		}

		//Joining the generated queries by using union in order to retrieve all results at once
		sql = QChar('(') +  queries.join(QString(") UNION (")) + QChar(')');

		if(sort_results)
			sql += QString(" ORDER BY oid, object_type");

		connection.executeDMLCommand(sql, res);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				attribs[ParsersAttributes::OID]=res.getColumnValue(ParsersAttributes::OID);
				attribs[ParsersAttributes::NAME]=res.getColumnValue(ParsersAttributes::NAME);
				attribs[ParsersAttributes::OBJECT_TYPE]=res.getColumnValue(QString("object_type"));
				objects.push_back(attribs);
				attribs.clear();
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		return(objects);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map obj_attribs;

		//Add the name of the object as extra attrib in order to retrieve the data only for it
		extra_attribs[ParsersAttributes::NAME]=obj_name;
		executeCatalogQuery(QUERY_ATTRIBS, obj_type, res, true, extra_attribs);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
			obj_attribs=changeAttributeNames(res.getTupleValues());

		/* Insert the object type as an attribute of the query result to facilitate the
		import process on the classes that uses the Catalog */
		obj_attribs[ParsersAttributes::OBJECT_TYPE]=QString("%1").arg(obj_type);

		return(obj_attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<attribs_map> Catalog::getMultipleAttributes(ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map tuple;
		vector<attribs_map> obj_attribs;

		executeCatalogQuery(QUERY_ATTRIBS, obj_type, res, false, extra_attribs);
		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				tuple=changeAttributeNames(res.getTupleValues());

				/* Insert the object type as an attribute of the query result to facilitate the
				import process on the classes that uses the Catalog */
				tuple[ParsersAttributes::OBJECT_TYPE]=QString("%1").arg(obj_type);

				obj_attribs.push_back(tuple);
				tuple.clear();
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		return(obj_attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<attribs_map> Catalog::getMultipleAttributes(const QString &catalog_sch, attribs_map attribs)
{
	try
	{
		ResultSet res;
		attribs_map tuple;
		vector<attribs_map> obj_attribs;

		loadCatalogQuery(catalog_sch);
		schparser.ignoreUnkownAttributes(true);
		schparser.ignoreEmptyAttributes(true);

		attribs[ParsersAttributes::PGSQL_VERSION]=schparser.getPgSQLVersion();
		connection.executeDMLCommand(schparser.getCodeDefinition(attribs).simplified(), res);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				tuple=changeAttributeNames(res.getTupleValues());
				obj_attribs.push_back(tuple);
				tuple.clear();
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		return(obj_attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString Catalog::getCommentQuery(const QString &oid_field, bool is_shared_obj)
{
	QString query_id=ParsersAttributes::COMMENT;

	try
	{
		attribs_map attribs={{ParsersAttributes::OID, oid_field},
												 {ParsersAttributes::SHARED_OBJ, (is_shared_obj ? ParsersAttributes::_TRUE_ : QString())}};

		loadCatalogQuery(query_id);
		return(schparser.getCodeDefinition(attribs).simplified());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(query_id));
	}
}

QString Catalog::getNotExtObjectQuery(const QString &oid_field)
{
	QString query_id=QString("notextobject");

	try
	{
		attribs_map attribs={{ParsersAttributes::OID, oid_field},
							 {ParsersAttributes::EXT_OBJ_OIDS, ext_obj_oids}};


		loadCatalogQuery(query_id);
		return(schparser.getCodeDefinition(attribs).simplified());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(query_id));
	}
}

attribs_map Catalog::changeAttributeNames(const attribs_map &attribs)
{
	attribs_map::const_iterator itr=attribs.begin();
	attribs_map new_attribs;
	QString attr_name, value;

	while(itr!=attribs.end())
	{
		attr_name=itr->first;
		value=itr->second;

		if(attr_name.endsWith(BOOL_FIELD))
		{
			attr_name.remove(BOOL_FIELD);
			if(value==PGSQL_FALSE) value.clear();
			else value=ParsersAttributes::_TRUE_;
		}

		attr_name.replace('_','-');
		new_attribs[attr_name]=value;
		itr++;
	}

	return(new_attribs);
}

QString Catalog::createOidFilter(const vector<unsigned> &oids)
{
	QString filter;

	for(unsigned i=0; i < oids.size(); i++)
		filter+=QString("%1,").arg(oids.at(i));

	if(!filter.isEmpty())
		filter.remove(filter.size()-1,1);

	return(filter);
}

vector<attribs_map> Catalog::getObjectsAttributes(ObjectType obj_type, const QString &schema, const QString &table, const vector<unsigned> &filter_oids, attribs_map extra_attribs)
{
	try
	{
		bool is_shared_obj=(obj_type==OBJ_DATABASE ||	obj_type==OBJ_ROLE ||	obj_type==OBJ_TABLESPACE ||
												obj_type==OBJ_LANGUAGE || obj_type==OBJ_CAST);

		extra_attribs[ParsersAttributes::SCHEMA]=schema;
		extra_attribs[ParsersAttributes::TABLE]=table;

		if(!filter_oids.empty())
			extra_attribs[ParsersAttributes::FILTER_OIDS]=createOidFilter(filter_oids);

		//Retrieve the comment catalog query. Only columns need to retreive comments in their own catalog query file
		if(obj_type != OBJ_COLUMN)
			extra_attribs[ParsersAttributes::COMMENT]=getCommentQuery(oid_fields[obj_type], is_shared_obj);

		return(getMultipleAttributes(obj_type, extra_attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}

attribs_map Catalog::getObjectAttributes(ObjectType obj_type, unsigned oid, const QString sch_name, const QString tab_name, attribs_map extra_attribs)
{
	try
	{
		vector<attribs_map> attribs_vect=getObjectsAttributes(obj_type, sch_name, tab_name, { oid }, extra_attribs);
		return(attribs_vect.empty() ? attribs_map() : attribs_vect[0]);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}

QString Catalog::getObjectOID(const QString &name, ObjectType obj_type, const QString &schema, const QString &table)
{
	try
	{
		attribs_map attribs;
		ResultSet res;

		attribs[ParsersAttributes::CUSTOM_FILTER] = QString("%1 = E'%2'").arg(name_fields[obj_type]).arg(name);
		attribs[ParsersAttributes::SCHEMA] = schema;
		attribs[ParsersAttributes::TABLE] = table;
		executeCatalogQuery(QUERY_LIST, obj_type, res, false, attribs);

		if(res.getTupleCount() > 1)
			throw Exception(QApplication::translate("Catalog","The catalog query returned more than one OID!","", -1),
											ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		else if(res.isEmpty())
			return("0");
		else
		{
			res.accessTuple(ResultSet::FIRST_TUPLE);
			return(res.getColumnValue(ParsersAttributes::OID));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}

attribs_map Catalog::getServerAttributes(void)
{
	attribs_map attribs;

	try
	{
		ResultSet res = ResultSet();
		QString sql, attr_name;
		attribs_map tuple, attribs_aux;

		loadCatalogQuery(QString("server"));
		schparser.ignoreUnkownAttributes(true);
		schparser.ignoreEmptyAttributes(true);
		sql = schparser.getCodeDefinition(attribs).simplified();
		connection.executeDMLCommand(sql, res);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				tuple=res.getTupleValues();
				attr_name = tuple[ParsersAttributes::ATTRIBUTE];
				attr_name.replace('_','-');
				attribs[attr_name]=tuple[ParsersAttributes::VALUE];
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));

			attribs[ParsersAttributes::CONNECTION] = connection.getConnectionId();
			attribs_aux = connection.getServerInfo();
			attribs.insert(attribs_aux.begin(), attribs_aux.end()) ;
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: server","", -1));
	}

	return(attribs);
}

QStringList Catalog::parseArrayValues(const QString &array_val)
{
	QStringList list;

	if(QRegExp(ARRAY_PATTERN).exactMatch(array_val))
	{
		//Detecting the position of { and }
		int start=array_val.indexOf('{')+1,
				end=array_val.lastIndexOf('}')-1;
		QString value=array_val.mid(start, (end - start)+1);

		if(value.contains('"'))
			list=parseDefaultValues(value, QString("\""), QString(","));
		else
			list=value.split(',', QString::SkipEmptyParts);
	}

	return(list);
}

QStringList Catalog::parseDefaultValues(const QString &def_vals, const QString &str_delim, const QString &val_sep)
{
	int idx=0, delim_start, delim_end, sep_idx, pos=0;
	QStringList values;

	while(idx < def_vals.size())
	{
		//Get the index of string delimiters (default: ')
		delim_start=def_vals.indexOf(str_delim, idx);
		delim_end=def_vals.indexOf(str_delim, delim_start + 1);

		/* Get the index of value separator on default value string
			 (by default the pg_get_expr separates values by comma and space (, ) */
		sep_idx=def_vals.indexOf(val_sep, idx);

		/* If there is no separator on string (only one value) or the is
			 beyond the string delimiters or even there is no string delimiter on string */
		if(sep_idx < 0 ||
				(sep_idx >=0 && delim_start >= 0 && delim_end >= 0 &&
				 (sep_idx < delim_start || sep_idx > delim_end)) ||
				(sep_idx >=0 && (delim_start < 0 || delim_end < 0)))
		{
			//Extract the value from the current position
			values.push_back(def_vals.mid(pos, sep_idx-pos).trimmed());

			//If there is no separator on string indicates that it contains only one value
			if(sep_idx < 0)
				//Forcing the loop abort
				idx=def_vals.size();
			else
			{
				//Passing to the next value right after the separator
				pos=sep_idx+1;
				idx=pos;
			}
		}
		/* If the separator is between a string delimitation e.g.'abc, def' it will be ignored
		and the current postion will be moved to the first char after string delimiter */
		else if(delim_start>=0 && delim_end >= 0 &&
				sep_idx >= delim_start && sep_idx <=delim_end)
		{
			idx=delim_end+1;

			/* If the index reaches the end of string but the cursor (pos) isn't at end
			indicates that the last values wasn't retrieved, this way, the value will be
			pushed to list of values */
			if(idx >= def_vals.size() && pos < def_vals.size())
				values.push_back(def_vals.mid(pos, def_vals.size()));
		}
		else
			idx++;
	}

	return(values);
}

QStringList Catalog::parseRuleCommands(const QString &cmds)
{
	int start=-1, end=-1;
	QRegExp cmd_regexp(QString("(DO)( )*(INSTEAD)*( )+"));

	start=cmd_regexp.indexIn(cmds) + cmd_regexp.matchedLength();
	end=cmds.lastIndexOf(';');
	return(cmds.mid(start,(end - start) + 1).split(';', QString::SkipEmptyParts));
}

void Catalog::operator = (const Catalog &catalog)
{
	try
	{
		this->ext_obj_oids=catalog.ext_obj_oids;
		this->connection.setConnectionParams(catalog.connection.getConnectionParams());
		this->last_sys_oid=catalog.last_sys_oid;
		this->filter=catalog.filter;
		this->exclude_ext_objs=catalog.exclude_ext_objs;
		this->exclude_sys_objs=catalog.exclude_sys_objs;
		this->exclude_array_types=catalog.exclude_array_types;
		this->list_only_sys_objs=catalog.list_only_sys_objs;
		this->connection.connect();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
