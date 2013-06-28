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

		return(QString(res.getColumnValue(0)).toUInt());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QString> Catalog::getObjectNames(ObjectType obj_type)
{
	try
	{
		ResultSet res;
		vector<QString> names;

		executeCatalogQuery(QUERY_LIST, obj_type, res);

		if(res.getTupleCount() > 0)
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
