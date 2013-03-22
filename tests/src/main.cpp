#include <iostream>
#include "exception.h"
#include "mainwindow.h"

int main(int argc, char **argv)
{
	try
	{
		QString buf;
		map<QString, QString> attribs;

		buf+="# comment tst \n";
		buf+="\n";
		buf+="  %if %not @{a3} %and @{a3} %then \n";
		buf+=" true \n";
		buf+=" %else \n";
		buf+=" false \n";
		buf+=" %end\n";

		attribs["a1"]="attrib 1";
		attribs["a2"]="attrib 2";
		attribs["a3"]="";
		//SchemaParser::loadBuffer(buf);
		SchemaParser::loadFile("/root/buf.sch");
		cout << SchemaParser::getCodeDefinition(attribs).toStdString() << endl;

	 return(0);
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
