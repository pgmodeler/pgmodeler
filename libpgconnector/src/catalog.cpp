#include "catalog.h"

const QString Catalog::QUERY_LIST="list";
const QString Catalog::QUERY_COUNT="count";
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

void Catalog::executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result, map<QString, QString> attribs)
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

		executeCatalogQuery(QUERY_COUNT, obj_type, res);
		res.accessTuple(ResultSet::FIRST_TUPLE);

		return(QString(res.getColumnValue(0)).toUInt());
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

vector<map<QString, QString> > Catalog::getObjectAttributes(const QString &obj_name, ObjectType obj_type, map<QString, QString> extra_attribs)
{
	try
	{
		ResultSet res;
		map<QString, QString> tuple;
		vector<map<QString, QString> > obj_attribs;

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

/*unsigned Catalog::getDatabaseCount(void)
{
	try
	{
		return(getObjectCount(OBJ_DATABASE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QString> Catalog::getDatabases(void)
{
	try
	{
		return(getObjectNames(OBJ_DATABASE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

map<QString, QString> Catalog::getDatabaseAttributes(const QString &db_name)
{
	try
	{
		return(getObjectAttributes(db_name, OBJ_DATABASE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned Catalog::getRoleCount(void)
{
	try
	{
		return(getObjectCount(OBJ_ROLE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QString> Catalog::getRoles(void)
{
	try
	{
		return(getObjectNames(OBJ_ROLE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

map<QString, QString> Catalog::getRoleAttributes(const QString &rol_name)
{
	try
	{
		return(getObjectAttributes(rol_name, OBJ_ROLE));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
} */

