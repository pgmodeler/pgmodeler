#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int argc, char **argv)
{
	try
	{
		Connection conn;
		Connection::setPrintSQL(true);
		conn.setConnectionParam(Connection::PARAM_DB_NAME, "rmfa");
		conn.setConnectionParam(Connection::PARAM_USER, "postgres");
		conn.setConnectionParam(Connection::PARAM_PASSWORD, "postgres");
		conn.setConnectionParam(Connection::PARAM_SERVER_FQDN, "localhost");

		Catalog catalog;
		catalog.setConnection(conn);

		ObjectType types[]={ /*OBJ_DATABASE, OBJ_TABLESPACE, OBJ_ROLE, OBJ_SCHEMA,
												 OBJ_LANGUAGE, OBJ_EXTENSION, OBJ_FUNCTION, OBJ_AGGREGATE,
												 OBJ_OPERATOR, OBJ_OPCLASS, OBJ_OPFAMILY, OBJ_COLLATION,
												 OBJ_CONVERSION,  OBJ_CAST, OBJ_VIEW, OBJ_SEQUENCE ,
												 OBJ_DOMAIN, OBJ_TYPE , OBJ_TABLE */ OBJ_COLUMN };

		unsigned i, cnt=sizeof(types)/sizeof(ObjectType);

		for(i=0; i < cnt; i++)
		{
			long time1=QDateTime::currentMSecsSinceEpoch();
			cout << "[object]: " << BaseObject::getTypeName(types[i]).toStdString() << endl;
			cout << "[count]: " << catalog.getObjectCount(types[i],"public","filo") << endl;

			cout << "[list]: ";
			attribs_map v1=catalog.getObjectsNames(types[i],"public", "filo");
			attribs_map::iterator itr1=v1.begin();

			while(itr1!=v1.end())
			{
				cout << itr1->first.toStdString() <<  ":" <<itr1->second.toStdString() << " ";
				itr1++;
			}

			cout << endl;

			vector<attribs_map> v=catalog.getObjectsAttributes(types[i],"public", "filo");
			attribs_map::iterator itr;

			while(!v.empty())
			{
				cout << "[attribs]: ";
				for(itr=v.back().begin(); itr!=v.back().end(); itr++)
					cout << itr->first.toStdString() << "=" << itr->second.toStdString() << " ";

				cout << endl;
				v.pop_back();
			}
			long time2=QDateTime::currentMSecsSinceEpoch();
			cout << "[Execution]: " << time2 - time1 << " ms" << endl;

			//cin.get();
		}

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
