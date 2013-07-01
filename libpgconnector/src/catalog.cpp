#include "catalog.h"

const QString Catalog::QUERY_LIST="list";
const QString Catalog::QUERY_ATTRIBS="attribs";
const QString Catalog::CATALOG_SCH_DIR="catalog";

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

void Catalog::executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, attribs_map attribs)
{
	try
	{
		QString sql;

		attribs[qry_type]="1";
		SchemaParser::setIgnoreUnkownAttributes(true);
		SchemaParser::setPgSQLVersion(connection.getPgSQLVersion().mid(0,3));
		sql=SchemaParser::getCodeDefinition(GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR +
																				CATALOG_SCH_DIR + GlobalAttributes::DIR_SEPARATOR +
																				BaseObject::getSchemaName(obj_type) + GlobalAttributes::SCHEMA_EXT,
																				attribs);

		connection.executeDMLCommand(sql, result);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned Catalog::getObjectCount(ObjectType obj_type)
{
	try
	{
		ResultSet res;

		executeCatalogQuery(QUERY_LIST, obj_type, res);
		res.accessTuple(ResultSet::FIRST_TUPLE);

		return(res.getTupleCount());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QString> Catalog::getObjects(ObjectType obj_type)
{
	try
	{
		ResultSet res;
		vector<QString> names;

		executeCatalogQuery(QUERY_LIST, obj_type, res);

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

vector<attribs_map> Catalog::getObjectAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs)
{
	try
	{
		ResultSet res;
		attribs_map tuple;
		vector<attribs_map> obj_attribs;

		extra_attribs[ParsersAttributes::NAME]=obj_name;
		executeCatalogQuery(QUERY_ATTRIBS, obj_type, res, extra_attribs);

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				for(int col=0; col < res.getColumnCount(); col++)
					tuple[res.getColumnName(col)]=res.getColumnValue(col);

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

attribs_map Catalog::changeAttributeNames(const attribs_map &attribs)
{
	map<QString,QString>::const_iterator itr=attribs.begin();
	map<QString,QString> new_attribs;
	QString attr_name;

	while(itr!=attribs.end())
	{
		attr_name=QString(itr->first).replace("_","-");
		new_attribs[attr_name]=itr->second;
		itr++;
	}
}

attribs_map Catalog::getDatabaseAttributes(const QString &db_name)
{
	try
	{
		vector<attribs_map> dbs=getObjectAttributes(db_name, OBJ_DATABASE);
		return(!dbs.empty() ? changeAttributeNames(dbs.at(0)) : attribs_map());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

/* map<QString, QString> Catalog::getRoleAttributes(const QString &rol_name)
{
	try
	{
		vector<map<QString, QString>> roles=getObjectAttributes(rol_name, OBJ_ROLE);
		vector<map<QString, QString>> members=getObjectAttributes(rol_name, OBJ_ROLE, {"member-roles", "1"});

		if(roles.empty() && members.empty())
		{
			QString str_member;

			while(!members.empty())
			{
				str_member+=members.back() + ",";
				members.pop_back();
			}

			str_member.remove(str_member.size()-1, 1);
			roles[ParsersAttributes::MEMBER_ROLES]
		}


		return(roles);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
} */

/* map<QString, QString> Catalog::getSchemaAttributes(const QString &sch_name)
{
	try
	{
		return(getObjectAttributes(sch_name, OBJ_SCHEMA));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

map<QString, QString> Catalog::getFunctionAttributes(const QString &func_name)
{
	try
	{
		return(getObjectAttributes(func_name, OBJ_FUNCTION));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
} */
