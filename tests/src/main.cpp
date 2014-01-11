#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int, char **)
{
	try
	{
    QString dt=__DATE__;
    QString fmt=QDate::fromString(__DATE__, "MMM dd yyyy").toString("yyyyMMdd");

		return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
