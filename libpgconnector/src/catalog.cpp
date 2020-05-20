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
#include "catalog.h"

const QString Catalog::QueryList("list");
const QString Catalog::QueryAttribs("attribs");
const QString Catalog::PgSqlTrue("t");
const QString Catalog::PgSqlFalse("f");
const QString Catalog::BoolField("_bool");
const QString Catalog::ArrayPattern("((\\[)[0-9]+(\\:)[0-9]+(\\])=)?(\\{)((.)+(,)*)*(\\})$");
const QString Catalog::GetExtensionObjsSql("SELECT objid AS oid FROM pg_depend WHERE objid > 0 AND refobjid > 0 AND deptype='e'");
const QString Catalog::PgModelerTempDbObj("__pgmodeler_tmp");
const QString Catalog::FilterLike("like");
const QString Catalog::FilterRegExp("regexp");
const QString Catalog::FilterExact("exact");
const QString Catalog::InvFilterPattern("__invalid__pattern__");
const QString Catalog::AliasPlaceholder("$alias$");

attribs_map Catalog::catalog_queries;

map<ObjectType, QString> Catalog::oid_fields=
{ {ObjectType::Database, "oid"}, {ObjectType::Role, "oid"}, {ObjectType::Schema,"oid"},
	{ObjectType::Language, "oid"}, {ObjectType::Tablespace, "oid"}, {ObjectType::Extension, "ex.oid"},
	{ObjectType::Function, "pr.oid"}, {ObjectType::Aggregate, "pr.oid"}, {ObjectType::Operator, "op.oid"},
	{ObjectType::OpClass, "op.oid"}, {ObjectType::OpFamily, "op.oid"}, {ObjectType::Collation, "cl.oid"},
	{ObjectType::Conversion, "cn.oid"}, {ObjectType::Cast, "cs.oid"}, {ObjectType::View, "vw.oid"},
	{ObjectType::Sequence, "sq.oid"}, {ObjectType::Domain, "dm.oid"}, {ObjectType::Type, "tp.oid"},
	{ObjectType::Table, "tb.oid"}, {ObjectType::Column, "cl.oid"}, {ObjectType::Constraint, "cs.oid"},
	{ObjectType::Rule, "rl.oid"}, {ObjectType::Trigger, "tg.oid"}, {ObjectType::Index, "id.indexrelid"},
	{ObjectType::EventTrigger, "et.oid"}, {ObjectType::Policy, "pl.oid"}, {ObjectType::ForeignDataWrapper, "fw.oid"},
	{ObjectType::ForeignServer, "sv.oid"}, {ObjectType::UserMapping, "um.umid"}, {ObjectType::ForeignTable, "ft.oid"}
};

map<ObjectType, QString> Catalog::ext_oid_fields={
	{ObjectType::Constraint, "cs.conrelid"},
	{ObjectType::Index, "id.indexrelid"},
	{ObjectType::Trigger, "tg.tgrelid"},
	{ObjectType::Rule, "rl.ev_class"},
	{ObjectType::Policy, "pl.polrelid"}
};

map<ObjectType, QString> Catalog::parent_aliases={
	{ObjectType::Constraint, "tb"},
	{ObjectType::Index, "tb"},
	{ObjectType::Trigger, "tb"},
	{ObjectType::Rule, "cl"},
	{ObjectType::Policy, "tb"}
};

map<ObjectType, QString> Catalog::name_fields=
{ {ObjectType::Database, "datname"}, {ObjectType::Role, "rolname"}, {ObjectType::Schema,"nspname"},
	{ObjectType::Language, "lanname"}, {ObjectType::Tablespace, "spcname"}, {ObjectType::Extension, "extname"},
	{ObjectType::Function, "proname"}, {ObjectType::Aggregate, "proname"}, {ObjectType::Operator, "oprname"},
	{ObjectType::OpClass, "opcname"}, {ObjectType::OpFamily, "opfname"}, {ObjectType::Collation, "collname"},
	{ObjectType::Conversion, "conname"}, {ObjectType::Cast, ""}, {ObjectType::View, "relname"},
	{ObjectType::Sequence, "relname"}, {ObjectType::Domain, "typname"}, {ObjectType::Type, "typname"},
	{ObjectType::Table, "relname"}, {ObjectType::Column, "attname"}, {ObjectType::Constraint, "conname"},
	{ObjectType::Rule, "rulename"}, {ObjectType::Trigger, "tgname"}, {ObjectType::Index, "cl.relname"},
	{ObjectType::EventTrigger, "evtname"}, {ObjectType::Policy, "polname"}, {ObjectType::ForeignDataWrapper, "fdwname"},
	{ObjectType::ForeignServer, "srvname"}, {ObjectType::ForeignTable, "relname"}
};

Catalog::Catalog()
{
	last_sys_oid=0;
	setQueryFilter(ExclExtensionObjs | ExclSystemObjs);
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
		executeCatalogQuery(QueryList, ObjectType::Database, res, true,
		{{Attributes::Name, conn.getConnectionParam(Connection::ParamDbName)}});

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			attribs_map attribs=changeAttributeNames(res.getTupleValues());
			last_sys_oid=attribs[Attributes::LastSysOid].toUInt();
		}

		//Retrieving the list of objects created by extensions
		this->connection.executeDMLCommand(GetExtensionObjsSql, res);
		if(res.accessTuple(ResultSet::FirstTuple))
		{
			do
			{
				ext_obj.push_back(res.getColumnValue(QString("oid")));
			}
			while(res.accessTuple(ResultSet::NextTuple));

			ext_obj_oids=ext_obj.join(',');
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Catalog::closeConnection()
{
	connection.close();
}

void Catalog::setQueryFilter(unsigned filter)
{
	bool list_all=(ListAllObjects & filter) == ListAllObjects;

	this->filter=filter;
	list_only_sys_objs=false;
	exclude_array_types=(ExclBuiltinArrayTypes & filter) == ExclBuiltinArrayTypes;
	exclude_ext_objs=(ExclExtensionObjs & filter) == ExclExtensionObjs;
	exclude_sys_objs=(ExclSystemObjs & filter) == ExclSystemObjs;

	if(!list_all)
	{
		list_only_sys_objs=(ListOnlySystemObjs & filter) == ListOnlySystemObjs;

		if(list_only_sys_objs)
		{
			exclude_ext_objs=true;
			exclude_sys_objs=false;
		}
	}
}

void Catalog::setObjectFilters(QStringList filters, bool only_matching, QStringList tab_obj_types)
{
	ObjectType obj_type;
	QString pattern, mode, aux_filter, parent_alias_ref, tab_filter = "^(%1)(.)+";
	QStringList values,	relkinds, modes = { FilterExact, FilterLike, FilterRegExp };

	map<ObjectType, QStringList> tab_patterns;

	attribs_map fmt_filter, tmpl_filters = {{ FilterExact, "%1 = E'%2'" },
																					{ FilterLike, "%1 ILIKE E'%2'" },
																					{ FilterRegExp, "%1 ~* E'%2'" }};

	bool has_tab_filter = filters.indexOf(QRegExp(tab_filter.arg(BaseObject::getSchemaName(ObjectType::Table)))) >= 0,
			 has_view_filter = filters.indexOf(QRegExp(tab_filter.arg(BaseObject::getSchemaName(ObjectType::View)))) >= 0,
			 has_ftab_filter = filters.indexOf(QRegExp(tab_filter.arg(BaseObject::getSchemaName(ObjectType::ForeignTable)))) >= 0;

	obj_filters.clear();

	/* If we have at least one table (view or foreign table) filter
	 * and the forced object types list we configure filters to force the
	 * listing of table children objects, tied to the filters that list
	 * the parent tables */
	if(only_matching && (has_tab_filter || has_ftab_filter || has_view_filter))
	{
		/* Configuring the placeholder for the parent table name used in the construction of the creterias that filters
		 * table names in forced table children objects filters.
		 * This one comes in form of regexp_replace() because when calling oid::regclass::text the string comes schema qualified
		 * so in order to match the table/view/foreign tables filters correclty we need to remove the schema name for the oid::regclass cast */
		parent_alias_ref = QString("regexp_replace(%1,'^(.)+(\\.)', '')")
											 .arg(AliasPlaceholder + QString(".oid::regclass::text"));

		// Validating the provided table children objects types
		for(auto &type_name : tab_obj_types)
		{
			if(!TableObject::isTableObject(BaseObject::getObjectType(type_name)))
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::InvChildObjectTypeFilter).arg(type_name),
												ErrorCode::InvChildObjectTypeFilter,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		tab_obj_types.removeDuplicates();
	}

	// The only matching filter is generated if there're filters configured
	if(!filters.isEmpty() && only_matching)
	{
		for(auto &type : getFilterableObjectTypes())
		{
			/* We do not create exclusion filter for table objects if they were specified
			 * by the users forced objects to be filtered */
			if(tab_obj_types.contains(BaseObject::getSchemaName(type)))
				continue;

			if(filters.indexOf(QRegExp(QString("(%1)(.)+").arg(BaseObject::getSchemaName(type)))) < 0)
				obj_filters[type].append(tmpl_filters[Catalog::FilterExact].arg(name_fields[type]).arg(InvFilterPattern));
		}
	}

	for(auto &filter : filters)
	{
		values = filter.split(FilterSeparator);

		// Raises an error if the filter has an invalid field count
		if(values.size() != 3)
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvalidObjectFilter).arg(filter).arg(modes.join('|')),
											ErrorCode::InvalidObjectFilter,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		obj_type = BaseObject::getObjectType(values[0]);
		pattern = values[1];
		mode = values[2];

		// Raises an error if the filter has an invalid object type, pattern or mode
		if(obj_type == ObjectType::BaseObject || pattern.isEmpty() || !modes.contains(mode))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvalidObjectFilter).arg(filter).arg(modes.join('|')),
											ErrorCode::InvalidObjectFilter,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		obj_filters[obj_type].append(tmpl_filters[mode].arg(name_fields[obj_type]).arg(pattern));

		// Storing the table/view/foreign table patters if there're forced children objects to filter
		if(!tab_obj_types.isEmpty() && BaseTable::isBaseTable(obj_type))
			tab_patterns[obj_type].append(tmpl_filters[mode].arg(parent_alias_ref).arg(pattern));
	}

	if(!tab_obj_types.isEmpty())
	{
		map<ObjectType, QString> fmt_tab_patterns;
		QStringList fmt_conds;

		for(auto &itr : tab_patterns)
			fmt_tab_patterns[itr.first] = QString("(%1) AND %2.relkind ")
																		.arg(tab_patterns[itr.first].join(" OR "))
																		.arg(AliasPlaceholder);

		for(auto &type_name : tab_obj_types)
		{
			obj_type = BaseObject::getObjectType(type_name);

			/* Configuring the "relkind" criteria according to the
			 * flags indicating the presence of table/view/foreign table filters.
			 * This relkinds will be used to filter table children objects specifically for
			 * "relation kind" in pg_class in order to avoid bringing children object of
			 * table types not filtered */
			if(has_tab_filter && BaseObject::isChildObjectType(ObjectType::Table, obj_type))
				fmt_conds.append(QString("(%1)").arg(fmt_tab_patterns[ObjectType::Table] + QString("IN ('r','p')")));

			if(has_view_filter && BaseObject::isChildObjectType(ObjectType::View, obj_type))
				fmt_conds.append(QString("(%1)").arg(fmt_tab_patterns[ObjectType::View] + QString("IN ('v','m')")));

			if(has_ftab_filter && BaseObject::isChildObjectType(ObjectType::ForeignTable, obj_type))
				fmt_conds.append(QString("(%1)").arg(fmt_tab_patterns[ObjectType::ForeignTable] + QString("= 'f'")));

			obj_filters[obj_type].append(fmt_conds.join(" OR ").replace(AliasPlaceholder, parent_aliases[obj_type]));
			relkinds.clear();
		}
	}
}

unsigned Catalog::getLastSysObjectOID()
{
	return last_sys_oid;
}

bool Catalog::isSystemObject(unsigned oid)
{
	return (oid <= last_sys_oid);
}

bool Catalog::isExtensionObject(unsigned oid)
{
	return ext_obj_oids.contains(QString::number(oid));
}

void Catalog::loadCatalogQuery(const QString &qry_id)
{
	if(catalog_queries.count(qry_id)==0)
	{
		QFile input;
		input.setFileName(GlobalAttributes::getSchemaFilePath(GlobalAttributes::CatalogSchemasDir, qry_id));

		if(!input.open(QFile::ReadOnly))
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(input.fileName()),
											ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		if(attr.first != Attributes::CustomFilter && attr.second.contains(QChar('\'')))
			attr.second.replace(QChar('\''), QString("''"));
	}

	schparser.setPgSQLVersion(connection.getPgSQLVersion(true));
	attribs[qry_type]=Attributes::True;

	if(exclude_sys_objs || list_only_sys_objs)
		attribs[Attributes::LastSysOid]=QString::number(last_sys_oid);

	if(list_only_sys_objs)
		attribs[Attributes::OidFilterOp]=QString("<=");
	else
		attribs[Attributes::OidFilterOp]=QString(">");

	if(obj_type==ObjectType::Type && exclude_array_types)
		attribs[Attributes::ExcBuiltinArrays]=Attributes::True;

	// If there's a name filter configured for the object type
	if(obj_filters.count(obj_type))
		attribs[Attributes::NameFilter] = obj_filters[obj_type].join(" OR ");

	//Checking if the custom filter expression is present
	if(attribs.count(Attributes::CustomFilter))
	{
		custom_filter=attribs[Attributes::CustomFilter];
		attribs.erase(Attributes::CustomFilter);
	}

	if(exclude_ext_objs && obj_type!=ObjectType::Database &&	obj_type!=ObjectType::Role && obj_type!=ObjectType::Tablespace && obj_type!=ObjectType::Extension)
	{
		if(ext_oid_fields.count(obj_type)==0)
			attribs[Attributes::NotExtObject]=getNotExtObjectQuery(oid_fields[obj_type]);
		else
			attribs[Attributes::NotExtObject]=getNotExtObjectQuery(ext_oid_fields[obj_type]);
	}

	loadCatalogQuery(BaseObject::getSchemaName(obj_type));
	schparser.ignoreUnkownAttributes(true);
	schparser.ignoreEmptyAttributes(true);

	attribs[Attributes::PgSqlVersion]=schparser.getPgSQLVersion();
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

	return sql;
}

void Catalog::executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, bool single_result, attribs_map attribs)
{
	try
	{
		connection.executeDMLCommand(getCatalogQuery(qry_type, obj_type, single_result, attribs), result);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(BaseObject::getSchemaName(obj_type)));
	}
}

unsigned Catalog::getObjectCount(ObjectType obj_type, const QString &sch_name, const QString &tab_name, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;

		extra_attribs[Attributes::Schema]=sch_name;
		extra_attribs[Attributes::Table]=tab_name;

		executeCatalogQuery(QueryList, obj_type, res, false, extra_attribs);
		res.accessTuple(ResultSet::FirstTuple);
		return res.getTupleCount();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned Catalog::getQueryFilter()
{
	return filter;
}

map<ObjectType, QStringList> Catalog::getObjectFilters()
{
	return obj_filters;
}

vector<ObjectType> Catalog::getFilteredObjectTypes()
{
	vector<ObjectType> types;
	QRegExp regexp = QRegExp(QString("(.)*(%1)(.)*").arg(InvFilterPattern));

	for(auto &flt : obj_filters)
	{
		if(flt.second.indexOf(QRegExp(regexp)) < 0)
			types.push_back(flt.first);
	}

	return types;
}

void Catalog::getObjectsOIDs(map<ObjectType, vector<unsigned> > &obj_oids, map<unsigned, vector<unsigned> > &col_oids, attribs_map extra_attribs)
{
	try
	{
		vector<ObjectType> types=BaseObject::getObjectTypes(true, { ObjectType::Database, ObjectType::Relationship, ObjectType::BaseRelationship,
																																ObjectType::Textbox, ObjectType::Tag, ObjectType::Column, ObjectType::Permission,
																																ObjectType::GenericSql });
		attribs_map attribs, col_attribs, sch_names;
		vector<attribs_map> tab_attribs;
		unsigned tab_oid=0;

		for(ObjectType type : types)
		{
			attribs = getObjectsNames(type, QString(), QString(), extra_attribs);

			for(auto &attr : attribs)
			{
				obj_oids[type].push_back(attr.first.toUInt());

				//Store the schemas names in order to retrieve the tables' columns correctly
				if(type==ObjectType::Schema)
					sch_names[attr.first]=attr.second;
				else if(type==ObjectType::Table)
				{
					//Get the full set of attributes of the table
					tab_oid=attr.first.toUInt();
					tab_attribs=getObjectsAttributes(type, QString(), QString(), { tab_oid });

					//Retrieve the oid and names of the table's columns
					col_attribs=getObjectsNames(ObjectType::Column, sch_names[tab_attribs[0][Attributes::Schema]], attr.second);

					for(auto &col_attr : col_attribs)
						col_oids[tab_oid].push_back(col_attr.first.toUInt());
				}
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getObjectsNames(ObjectType obj_type, const QString &sch_name, const QString &tab_name, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map objects;

		extra_attribs[Attributes::Schema]=sch_name;
		extra_attribs[Attributes::Table]=tab_name;
		executeCatalogQuery(QueryList, obj_type, res, false, extra_attribs);

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			do
			{
				objects[res.getColumnValue(Attributes::Oid)]=res.getColumnValue(Attributes::Name);
			}
			while(res.accessTuple(ResultSet::NextTuple));
		}

		return objects;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

		extra_attribs[Attributes::Schema]=sch_name;
		extra_attribs[Attributes::Table]=tab_name;

		for(auto &obj_type : obj_types)
		{	
			//Build the catalog query for the specified object type
			sql=getCatalogQuery(QueryList, obj_type, false, extra_attribs);

			/* For certain objects the catalog query will be empty due to the
			absence of that kind of element in the version of the database.
			E.g.: Event triggers does not exists in PgSQL < 9.3 */
			if(!sql.isEmpty())
			{
				//Injecting the object type integer code in order to sort the final result
				sql.replace(sql.indexOf(select_kw), select_kw.size(),
										QString("%1 %2 AS object_type, ").arg(select_kw).arg(enum_cast(obj_type)));

				sql+=QChar('\n');
				queries.push_back(sql);
			}
		}

		//Joining the generated queries by using union in order to retrieve all results at once
		sql = QChar('(') +  queries.join(QString(") UNION (")) + QChar(')');

		if(sort_results)
			sql += QString(" ORDER BY oid, object_type");

		connection.executeDMLCommand(sql, res);

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			QString obj_type_attr = QString(Attributes::ObjectType).replace('-', '_'),
					parent_type_attr = QString(Attributes::ParentType).replace('-', '_');

			do
			{
				attribs[Attributes::Oid]=res.getColumnValue(Attributes::Oid);
				attribs[Attributes::Name]=res.getColumnValue(Attributes::Name);
				attribs[Attributes::ObjectType]=res.getColumnValue(obj_type_attr);
				attribs[Attributes::Parent]=res.getColumnValue(Attributes::Parent);
				attribs[Attributes::ParentType]=res.getColumnValue(parent_type_attr);
				objects.push_back(attribs);
				attribs.clear();
			}
			while(res.accessTuple(ResultSet::NextTuple));
		}

		return objects;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map obj_attribs;

		//Add the name of the object as extra attrib in order to retrieve the data only for it
		extra_attribs[Attributes::Name]=obj_name;
		executeCatalogQuery(QueryAttribs, obj_type, res, true, extra_attribs);

		if(res.accessTuple(ResultSet::FirstTuple))
			obj_attribs=changeAttributeNames(res.getTupleValues());

		/* Insert the object type as an attribute of the query result to facilitate the
		import process on the classes that uses the Catalog */
		obj_attribs[Attributes::ObjectType]=QString("%1").arg(enum_cast(obj_type));

		return obj_attribs;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<attribs_map> Catalog::getMultipleAttributes(ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map tuple;
		vector<attribs_map> obj_attribs;

		executeCatalogQuery(QueryAttribs, obj_type, res, false, extra_attribs);
		if(res.accessTuple(ResultSet::FirstTuple))
		{
			do
			{
				tuple=changeAttributeNames(res.getTupleValues());

				/* Insert the object type as an attribute of the query result to facilitate the
				import process on the classes that uses the Catalog */
				tuple[Attributes::ObjectType]=QString("%1").arg(enum_cast(obj_type));

				obj_attribs.push_back(tuple);
				tuple.clear();
			}
			while(res.accessTuple(ResultSet::NextTuple));
		}

		return obj_attribs;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

		attribs[Attributes::PgSqlVersion]=schparser.getPgSQLVersion();
		connection.executeDMLCommand(schparser.getCodeDefinition(attribs).simplified(), res);

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			do
			{
				tuple=changeAttributeNames(res.getTupleValues());
				obj_attribs.push_back(tuple);
				tuple.clear();
			}
			while(res.accessTuple(ResultSet::NextTuple));
		}

		return obj_attribs;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString Catalog::getCommentQuery(const QString &oid_field, bool is_shared_obj)
{
	QString query_id=Attributes::Comment;

	try
	{
		attribs_map attribs={{Attributes::Oid, oid_field},
												 {Attributes::SharedObj, (is_shared_obj ? Attributes::True : QString())}};

		loadCatalogQuery(query_id);
		return schparser.getCodeDefinition(attribs).simplified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(query_id));
	}
}

QString Catalog::getNotExtObjectQuery(const QString &oid_field)
{
	QString query_id=QString("notextobject");

	try
	{
		attribs_map attribs={{Attributes::Oid, oid_field},
							 {Attributes::ExtObjOids, ext_obj_oids}};


		loadCatalogQuery(query_id);
		return schparser.getCodeDefinition(attribs).simplified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
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

		if(attr_name.endsWith(BoolField))
		{
			attr_name.remove(BoolField);
			if(value==PgSqlFalse) value.clear();
			else value=Attributes::True;
		}

		attr_name.replace('_','-');
		new_attribs[attr_name]=value;
		itr++;
	}

	return new_attribs;
}

QString Catalog::createOidFilter(const vector<unsigned> &oids)
{
	QString filter;

	for(unsigned i=0; i < oids.size(); i++)
		filter+=QString("%1,").arg(oids.at(i));

	if(!filter.isEmpty())
		filter.remove(filter.size()-1,1);

	return filter;
}

vector<attribs_map> Catalog::getObjectsAttributes(ObjectType obj_type, const QString &schema, const QString &table, const vector<unsigned> &filter_oids, attribs_map extra_attribs)
{
	try
	{
		bool is_shared_obj=(obj_type==ObjectType::Database ||	obj_type==ObjectType::Role ||
												obj_type==ObjectType::Tablespace || obj_type==ObjectType::Language ||
												obj_type==ObjectType::Cast);

		extra_attribs[Attributes::Schema]=schema;
		extra_attribs[Attributes::Table]=table;

		if(!filter_oids.empty())
			extra_attribs[Attributes::FilterOids]=createOidFilter(filter_oids);

		//Retrieve the comment catalog query. Only columns need to retreive comments in their own catalog query file
		if(obj_type != ObjectType::Column)
			extra_attribs[Attributes::Comment]=getCommentQuery(oid_fields[obj_type], is_shared_obj);

		return getMultipleAttributes(obj_type, extra_attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QString("catalog: %1").arg(BaseObject::getSchemaName(obj_type)));
	}
}

attribs_map Catalog::getObjectAttributes(ObjectType obj_type, unsigned oid, const QString sch_name, const QString tab_name, attribs_map extra_attribs)
{
	try
	{
		vector<attribs_map> attribs_vect=getObjectsAttributes(obj_type, sch_name, tab_name, { oid }, extra_attribs);
		return (attribs_vect.empty() ? attribs_map() : attribs_vect[0]);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}

QString Catalog::getObjectOID(const QString &name, ObjectType obj_type, const QString &schema, const QString &table)
{
	try
	{
		attribs_map attribs;
		ResultSet res;

		attribs[Attributes::CustomFilter] = QString("%1 = E'%2'").arg(name_fields[obj_type]).arg(name);
		attribs[Attributes::Schema] = schema;
		attribs[Attributes::Table] = table;
		executeCatalogQuery(QueryList, obj_type, res, false, attribs);

		if(res.getTupleCount() > 1)
			throw Exception(QApplication::translate("Catalog","The catalog query returned more than one OID!","", -1),
											ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		else if(res.isEmpty())
			return "0";
		else
		{
			res.accessTuple(ResultSet::FirstTuple);
			return res.getColumnValue(Attributes::Oid);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}

attribs_map Catalog::getServerAttributes()
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

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			do
			{
				tuple=res.getTupleValues();
				attr_name = tuple[Attributes::Attribute];
				attr_name.replace('_','-');
				attribs[attr_name]=tuple[Attributes::Value];
			}
			while(res.accessTuple(ResultSet::NextTuple));

			attribs[Attributes::Connection] = connection.getConnectionId();
			attribs_aux = connection.getServerInfo();
			attribs.insert(attribs_aux.begin(), attribs_aux.end()) ;
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: server","", -1));
	}

	return attribs;
}

unsigned Catalog::getObjectCount(bool incl_sys_objs)
{
	unsigned count = 0;

	try
	{
		ResultSet res = ResultSet();
		QString sql, attr_name;
		attribs_map tuple, attribs;

		if(!incl_sys_objs)
			attribs[Attributes::LastSysOid]=QString::number(last_sys_oid);

		loadCatalogQuery(Attributes::ObjCount);
		schparser.ignoreUnkownAttributes(true);
		schparser.ignoreEmptyAttributes(true);
		sql = schparser.getCodeDefinition(attribs).simplified();
		connection.executeDMLCommand(sql, res);

		if(res.accessTuple(ResultSet::FirstTuple))
		{
			tuple = res.getTupleValues();
			count = tuple[Attributes::ObjCount].toUInt();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
						QApplication::translate("Catalog","Object type: server","", -1));
	}

	return count;
}

QStringList Catalog::parseArrayValues(const QString &array_val)
{
	QStringList list;

	if(QRegExp(ArrayPattern).exactMatch(array_val))
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

	return list;
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

	return values;
}

QStringList Catalog::parseRuleCommands(const QString &cmds)
{
	int start=-1, end=-1;
	QRegExp cmd_regexp(QString("(DO)( )*(INSTEAD)*( )+"));

	start=cmd_regexp.indexIn(cmds) + cmd_regexp.matchedLength();
	end=cmds.lastIndexOf(';');
	return (cmds.mid(start,(end - start) + 1).split(';', QString::SkipEmptyParts));
}

QStringList Catalog::parseIndexExpressions(const QString &expr)
{
	int open_paren = 0, close_paren = 0, pos = 0;
	QStringList expressions;
	QChar chr;
	QString word;
	bool open_apos = false;

	if(!expr.isEmpty())
	{
		while(pos < expr.length())
		{
			chr = expr[pos++];
			word += chr;

			if(chr == QChar('\''))
				open_apos = !open_apos;

			if(!open_apos && chr == QChar('('))
				open_paren++;
			else if(!open_apos && chr == QChar(')'))
				close_paren++;

			if(chr == QChar(',') || pos == expr.length())
			{
				if(open_paren == close_paren)
				{
					if(word.endsWith(QChar(',')))
						word.remove(word.length() - 1, 1);

					if(word.contains('(') && word.contains(')'))
						expressions.push_back(word.trimmed());
					else
						expressions.push_back(word);

					word.clear();
					open_paren = close_paren = 0;
				}
			}
		}
	}

	return expressions;
}

vector<ObjectType> Catalog::getFilterableObjectTypes()
{
	static vector<ObjectType> types = BaseObject::getObjectTypes(true, { ObjectType::Relationship,
																																			 ObjectType::BaseRelationship,																										ObjectType::Textbox,
																																			 ObjectType::GenericSql,
																																			 ObjectType::Permission,
																																			 ObjectType::Database,
																																			 ObjectType::Cast,
																																			 ObjectType::Column,
																																			 ObjectType::UserMapping,
																																			 ObjectType::Tag});

	return types;
}

QStringList Catalog::getFilterableObjectNames()
{
	static QStringList names;

	if(names.isEmpty())
	{
		for(auto &type : getFilterableObjectTypes())
			names.append(BaseObject::getSchemaName(type));

		names.sort();
	}

	return names;
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
		this->obj_filters=catalog.obj_filters;
		this->connection.connect();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
