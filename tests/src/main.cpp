#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "tag.h"

int main(int, char **)
{
  try
  {
    Tag *tag=new Tag;

    tag->setName("<teste_&tag>");
    tag->setComment("this tag is a test");
    cout << tag->getCodeDefinition(SchemaParser::XML_DEFINITION).toStdString() << endl;

    return(0);
  }
  catch(Exception &e)
  {
    cout << e.getExceptionsText().toStdString() << endl;
    return(e.getErrorType());
  }
}
