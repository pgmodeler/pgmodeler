#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int, char **)
{
	try
	{
		/*QTextStream ts(stdout);
		vector<unsigned> v1,v2;
		Permission::parsePermissionString("postgres=ar*wdD*xt/postgres",v1,v2);*/

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
