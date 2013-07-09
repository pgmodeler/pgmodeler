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

		long time1=QDateTime::currentMSecsSinceEpoch();
		cout << "[object]: " << BaseObject::getTypeName(OBJ_OPERATOR).toStdString() << endl;
		cout << "[count]: " << catalog.getObjectCount(OBJ_OPERATOR,"public") << endl;

		cout << "[list]: ";
		attribs_map v1=catalog.getObjects(OBJ_OPERATOR,"public");
		attribs_map::iterator itr1=v1.begin();

		while(itr1!=v1.end())
		{
			cout << itr1->first.toStdString() <<  ":" <<itr1->second.toStdString() << " ";
			itr1++;
		}

		cout << endl;

		vector<attribs_map> v=catalog.getOperators("public", {"75651", "75652"});
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

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
