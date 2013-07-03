#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int argc, char **argv)
{
	try
	{
		Connection conn;
		conn.setConnectionParam(Connection::PARAM_DB_NAME, "rmfa");
		conn.setConnectionParam(Connection::PARAM_USER, "postgres");
		conn.setConnectionParam(Connection::PARAM_PASSWORD, "postgres");
		conn.setConnectionParam(Connection::PARAM_SERVER_FQDN, "localhost");

		Catalog catalog;
		catalog.setConnection(conn);

		cout << "[object]: " << BaseObject::getTypeName(OBJ_FUNCTION).toStdString() << endl;
		cout << "[cout]: " << catalog.getObjectCount(OBJ_FUNCTION, "public") << endl;

		cout << "[list]: ";
		vector<QString> v1=catalog.getObjects(OBJ_FUNCTION, "public");
		while(!v1.empty())
		{
			cout << v1.back().toStdString() << " ";
			v1.pop_back();
		}

		cout << endl;

		attribs_map v=catalog.getFunctionAttributes("funct_teste_abc", "public");
		attribs_map::iterator itr;

		cout << "[attribs]: ";
		for(itr=v.begin(); itr!=v.end(); itr++)
			cout << itr->first.toStdString() << "=" << itr->second.toStdString() << " ";

		cout << endl;

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
