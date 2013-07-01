#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int argc, char **argv)
{
	try
	{
		Connection conn;
		conn.setConnectionParam(Connection::PARAM_DB_NAME, "postgres");
		conn.setConnectionParam(Connection::PARAM_USER, "postgres");
		conn.setConnectionParam(Connection::PARAM_PASSWORD, "postgres");
		conn.setConnectionParam(Connection::PARAM_SERVER_FQDN, "localhost");

		Catalog catalog;
		catalog.setConnection(conn);

		cout << "[object]: " << BaseObject::getTypeName(OBJ_TABLESPACE).toStdString() << endl;
		cout << "[cout]: " << catalog.getObjectCount(OBJ_TABLESPACE) << endl;

		cout << "[list]: ";
		vector<QString> v1=catalog.getObjects(OBJ_TABLESPACE);
		while(!v1.empty())
		{
			cout << v1.back().toStdString() << " ";
			v1.pop_back();
		}

		cout << endl;

		vector<map<QString, QString>> v=catalog.getObjectAttributes("pg_default", OBJ_TABLESPACE);
		map<QString, QString>::iterator itr;

		cout << "[attribs]: ";
		for(int i=0; i < v.size(); i++)
		{
			for(itr=v[i].begin(); itr!=v[i].end(); itr++)
				cout << itr->first.toStdString() << "=" << itr->second.toStdString() << " ";
		}

		cout << endl;

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
