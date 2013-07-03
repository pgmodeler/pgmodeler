#include "catalog.h"

const QString Catalog::QUERY_LIST="list";
const QString Catalog::QUERY_ATTRIBS="attribs";
const QString Catalog::QUERY_GETDEPOBJ="get";
const QString Catalog::QUERY_GETCOMMENT="getcomment";
const QString Catalog::CATALOG_SCH_DIR="catalog";
const QString Catalog::PGSQL_TRUE="t";
const QString Catalog::PGSQL_FALSE="f";
const QString Catalog::BOOL_FIELD="_bool";

Catalog::Catalog(Connection &conn)
{
	setConnection(conn);
}

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

		if(qry_type!=QUERY_GETDEPOBJ && qry_type!=QUERY_GETCOMMENT)
		{
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
		}
		else if(qry_type==QUERY_GETDEPOBJ)
		{
			sql=SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																					CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																					QUERY_GETDEPOBJ + BaseObject::getSchemaName(obj_type) + GlobalAttributes::SCHEMA_EXT,
																					attribs).simplified();
		}
		else if(qry_type==QUERY_GETCOMMENT)
		{
			sql=SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																					CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																					QUERY_GETCOMMENT + GlobalAttributes::SCHEMA_EXT,
																					attribs).simplified();
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

vector<QString> Catalog::getObjects(ObjectType obj_type, const QString &sch_name)
{
	try
	{
		ResultSet res;
		vector<QString> names;

		executeCatalogQuery(QUERY_LIST, obj_type, res, false, {{ParsersAttributes::SCHEMA, sch_name}});

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				names.push_back(res.getColumnValue(0));
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		return(names);
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

vector<attribs_map> Catalog::getMultipleAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map tuple;
		vector<attribs_map> obj_attribs;

		//Add the name of the object as extra attrib in order to retrieve the data only for it
		extra_attribs[ParsersAttributes::NAME]=obj_name;
		executeCatalogQuery(QUERY_ATTRIBS, obj_type, res, true, extra_attribs);

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

QString Catalog::getDependencyObject(const QString &oid, ObjectType obj_type)
{
	try
	{
		ResultSet res;

		executeCatalogQuery(QUERY_GETDEPOBJ, obj_type, res, true, {{ParsersAttributes::OID, oid}});

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
			return(res.getColumnValue(ParsersAttributes::NAME));
		else
			return(QString());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString Catalog::getObjectComment(const QString &obj_oid, bool is_shared_obj)
{
	try
	{
		ResultSet res;

		executeCatalogQuery(QUERY_GETCOMMENT, BASE_OBJECT, res, true,
												{{ParsersAttributes::OID, obj_oid},
												 {ParsersAttributes::SHARED_OBJ, (is_shared_obj ? "1" : "") }});

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
			return(res.getColumnValue(ParsersAttributes::COMMENT));
		else
			return(QString());
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

attribs_map Catalog::getDatabaseAttributes(const QString &db_name)
{
	try
	{
		attribs_map database=getAttributes(db_name, OBJ_DATABASE);

		if(!database.empty())
		{
			database[ParsersAttributes::TABLESPACE]=getDependencyObject(database[ParsersAttributes::TABLESPACE], OBJ_TABLESPACE);
			database[ParsersAttributes::OWNER]=getDependencyObject(database[ParsersAttributes::OWNER], OBJ_ROLE);
			database[ParsersAttributes::COMMENT]=getObjectComment(database[ParsersAttributes::OID], true);
		}

		return(database);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getRoleAttributes(const QString &rol_name)
{
	try
	{
		attribs_map role=getAttributes(rol_name, OBJ_ROLE);

		//Retrieving the member roles
		vector<attribs_map> member_roles=getMultipleAttributes(rol_name, OBJ_ROLE, {{ParsersAttributes::MEMBER_ROLES, "1"}});

		//If a role was retrieved as well its members
		if(!role.empty() && !member_roles.empty())
		{
			QString members, admins;

			/* Appending the member names in separeted strings:
			1) members: store the ordinary members
			2) admins: store the members with admin option set */
			while(!member_roles.empty())
			{
				if(member_roles.back()[ParsersAttributes::ADMIN_OPTION]==PGSQL_TRUE)
					admins+=member_roles.back()[ParsersAttributes::NAME] + ",";
				else
					members+=member_roles.back()[ParsersAttributes::NAME] + ",";

				member_roles.pop_back();
			}

			members.remove(members.size()-1, 1);
			admins.remove(admins.size()-1, 1);

			//Insert the members/admins as attributes of the retrieved role
			role[ParsersAttributes::MEMBER_ROLES]=members;
			role[ParsersAttributes::ADMIN_ROLES]=admins;
		}

		if(!role.empty())
		 role[ParsersAttributes::COMMENT]=getObjectComment(role[ParsersAttributes::OID], true);

		return(role);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getSchemaAttributes(const QString &sch_name)
{
	try
	{
		attribs_map schema=getAttributes(sch_name, OBJ_SCHEMA);

		if(!schema.empty())
		{
			schema[ParsersAttributes::OWNER]=getDependencyObject(schema[ParsersAttributes::OWNER], OBJ_ROLE);
			schema[ParsersAttributes::COMMENT]=getObjectComment(schema[ParsersAttributes::OID]);
		}

		return(schema);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getTablespaceAttributes(const QString &spc_name)
{
	try
	{
		attribs_map tablespace=getAttributes(spc_name, OBJ_TABLESPACE);

		if(!tablespace.empty())
		{
			tablespace[ParsersAttributes::OWNER]=getDependencyObject(tablespace[ParsersAttributes::OWNER], OBJ_ROLE);
			tablespace[ParsersAttributes::COMMENT]=getObjectComment(tablespace[ParsersAttributes::OID]);
		}

		return(tablespace);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getExtensionAttributes(const QString &ext_name, const QString &sch_name)
{
	try
	{
		attribs_map extension=getAttributes(ext_name, OBJ_EXTENSION, {{ParsersAttributes::SCHEMA, sch_name}});
		attribs_map types=getAttributes(ext_name, OBJ_EXTENSION, {{ParsersAttributes::SCHEMA, sch_name},
																															{ParsersAttributes::HANDLES_TYPE, "1"}});

		extension[ParsersAttributes::HANDLES_TYPE]=(!types.empty() ? "1" : "");
		extension[ParsersAttributes::COMMENT]=getObjectComment(extension[ParsersAttributes::OID]);

		return(extension);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map Catalog::getFunctionAttributes(const QString &func_name, const QString &sch_name)
{
	try
	{
		attribs_map func=getAttributes(func_name, OBJ_FUNCTION, {{ParsersAttributes::SCHEMA, sch_name}});

		if(!func.empty())
		{
			func[ParsersAttributes::OWNER]=getDependencyObject(func[ParsersAttributes::OWNER], OBJ_ROLE);
			func[ParsersAttributes::COMMENT]=getObjectComment(func[ParsersAttributes::OID]);
		}

		return(func);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
