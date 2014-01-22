#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int, char **)
{
	try
	{
    PgSQLType tp;

    tp=PgSQLType::parseString("varchar(250)");
    cout << tp.getTypeId() << endl;
    cout << tp.getTypeName().toStdString() << endl;
    cout << tp.getSQLTypeName().toStdString() << endl;

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
