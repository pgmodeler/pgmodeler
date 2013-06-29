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

		cout << catalog.getObjectCount(OBJ_ROLE) << endl;

		vector<QString> v1=catalog.getObjects(OBJ_ROLE);
		while(!v1.empty())
		{
			cout << v1.back().toStdString() << endl;
			v1.pop_back();
		}

		vector<map<QString, QString>> v=catalog.getObjectAttributes("mediawiki", OBJ_ROLE);
		map<QString, QString>::iterator itr;

		for(int i=0; i < v.size(); i++)
		{
			for(itr=v[i].begin(); itr!=v[i].end(); itr++)
				cout << itr->first.toStdString() << " = " << itr->second.toStdString() << endl;
		}

		cout << "---" << endl;

		v=catalog.getObjectAttributes("mediawiki", OBJ_ROLE, {{"member-roles","1"}});
		for(int i=0; i < v.size(); i++)
		{
			for(itr=v[i].begin(); itr!=v[i].end(); itr++)
				cout << itr->first.toStdString() << " = " << itr->second.toStdString() << endl;
		}

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
