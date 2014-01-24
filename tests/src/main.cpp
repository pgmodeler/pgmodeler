#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int, char **)
{
  try
  {
    PgSQLType tp;

    tp=PgSQLType::parseString("geometry(GEOMETRYZM)");
    cout << tp.getTypeId() << endl;
    cout << tp.getTypeName().toStdString() << endl;
    cout << tp.getSQLTypeName().toStdString() << endl;
    /*attribs_map attribs;
    SchemaParser::setIgnoreEmptyAttributes(true);
    SchemaParser::setIgnoreUnkownAttributes(true);
    SchemaParser::getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION);*/

    return(0);
  }
  catch(Exception &e)
  {
    cout << e.getExceptionsText().toStdString() << endl;
    return(e.getErrorType());
  }
}
