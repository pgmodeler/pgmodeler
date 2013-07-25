#include "catalog.h"

const QString Catalog::QUERY_LIST="list";
const QString Catalog::QUERY_ATTRIBS="attribs";
const QString Catalog::CATALOG_SCH_DIR="catalog";
const QString Catalog::PGSQL_TRUE="t";
const QString Catalog::PGSQL_FALSE="f";
const QString Catalog::BOOL_FIELD="_bool";

map<ObjectType, QString> Catalog::oid_fields=
{ {OBJ_DATABASE, "oid"}, {OBJ_ROLE, "oid"}, {OBJ_SCHEMA,"oid"},
	{OBJ_LANGUAGE, "oid"}, {OBJ_TABLESPACE, "oid"}, {OBJ_EXTENSION, "ex.oid"},
	{OBJ_FUNCTION, "pr.oid"}, {OBJ_AGGREGATE, "pr.oid"}, {OBJ_OPERATOR, "op.oid"},
	{OBJ_OPCLASS, "op.oid"}, {OBJ_OPFAMILY, "op.oid"}, {OBJ_COLLATION, "cl.oid"},
	{OBJ_CONVERSION, "cn.oid"}
};

void Catalog::setConnection(Connection &conn)
{
	try
	{
		this->connection=conn;
		this->connection.connect();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Catalog::executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, bool single_result, attribs_map attribs)
{
	try
	{
		QString sql;

		SchemaParser::setPgSQLVersion(connection.getPgSQLVersion().mid(0,3));

		attribs[qry_type]="1";
		SchemaParser::setIgnoreUnkownAttributes(true);
		SchemaParser::setIgnoreEmptyAttributes(true);

		sql=SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																				CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																				BaseObject::getSchemaName(obj_type) + GlobalAttributes::SCHEMA_EXT,
																				attribs).simplified();

		//Append a LIMIT clause when the single_result is set
		if(single_result)
		{
			if(sql.endsWith(';'))	sql.remove(sql.size()-1, 1);
			sql+=" LIMIT 1";
		}

		connection.executeDMLCommand(sql, result);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned Catalog::getObjectCount(ObjectType obj_type, const QString &sch_name)
{
	try
	{
		ResultSet res;

		executeCatalogQuery(QUERY_LIST, obj_type, res, false, {{ParsersAttributes::SCHEMA, sch_name}});
		res.accessTuple(ResultSet::FIRST_TUPLE);
		return(res.getTupleCount());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getObjectsNames(ObjectType obj_type, const QString &sch_name)
{
	try
	{
		ResultSet res;
		attribs_map objects;

		executeCatalogQuery(QUERY_LIST, obj_type, res, false, {{ParsersAttributes::SCHEMA, sch_name}});

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
				obj_attribs.push_back(changeAttributeNames(res.getTupleValues()));
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
	try
	{
		attribs_map attribs={{ParsersAttributes::OID, oid_field},
												 {ParsersAttributes::SHARED_OBJ, (is_shared_obj ? "1" : "")}};

		return(SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																					 CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																					 "get" + ParsersAttributes::COMMENT + GlobalAttributes::SCHEMA_EXT,
																					 attribs).simplified());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString Catalog::getFromExtensionQuery(const QString &oid_field)
{
	try
	{
		attribs_map attribs={{ParsersAttributes::OID, oid_field}};

		return(SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																					 CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																					 "fromextension" + GlobalAttributes::SCHEMA_EXT,
																					 attribs).simplified());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			else value="1";
		}

		attr_name.replace("_","-");
		new_attribs[attr_name]=value;
		itr++;
	}

	return(new_attribs);
}

QString Catalog::createOidFilter(const vector<QString> &oids)
{
	QString filter;

	for(unsigned i=0; i < oids.size(); i++)
		filter+=oids.at(i) + ",";

	if(!filter.isEmpty())
		filter.remove(filter.size()-1,1);

	return(filter);
}

vector<attribs_map> Catalog::getObjectsAttributes(ObjectType obj_type, const QString &schema, const vector<QString> &filter_oids)
{
	try
	{
		attribs_map extra_attribs;
		bool is_shared_obj=(obj_type==OBJ_DATABASE ||	obj_type==OBJ_ROLE ||	obj_type==OBJ_TABLESPACE || obj_type==OBJ_LANGUAGE);

		extra_attribs[ParsersAttributes::COMMENT]=getCommentQuery(oid_fields[obj_type], is_shared_obj);
		extra_attribs[ParsersAttributes::SCHEMA]=schema;

		if(!filter_oids.empty())
			extra_attribs[ParsersAttributes::FILTER_OIDS]=createOidFilter(filter_oids);

		if(!obj_type!=OBJ_DATABASE &&	obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE && obj_type!=OBJ_EXTENSION)
			extra_attribs[ParsersAttributes::FROM_EXTENSION]=getFromExtensionQuery(oid_fields[obj_type]);

		return(getMultipleAttributes(obj_type, extra_attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e,
										QApplication::translate("Catalog","Object type: %1","", -1).arg(BaseObject::getSchemaName(obj_type)));
	}
}
