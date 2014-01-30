#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "tag.h"
#include <array>
using namespace std;

int main(int, char **)
{
  try
  {
    Tag *tag=new Tag;
    Tag *tag1=new Tag;
    Table *tab=new Table;
    DatabaseModel model;
    Schema *sch=new Schema;

    /*tag->setName("<teste_&tag>");
    tag->setComment("this tag is a test");
    tag->setElementColor(ParsersAttributes::TABLE_BODY, QColor(255,128,64), Tag::FILL_COLOR1);
    tag->setElementColor(ParsersAttributes::TABLE_BODY, QColor(64,64,32), Tag::FILL_COLOR2);
    tag->setElementColor(ParsersAttributes::TABLE_BODY, QColor(5,5,5), Tag::BORDER_COLOR);
    cout << tag->getCodeDefinition(SchemaParser::XML_DEFINITION).toStdString() << endl;

    (*tag1)=(*tag);
    tag1->setName("<teste_&tag>111");
    tag1->setElementColors(ParsersAttributes::TABLE_EXT_BODY, "#ff0cab,#fabcde,#bbccdd");
    cout << tag1->getCodeDefinition(SchemaParser::XML_DEFINITION, true).toStdString() << endl;
    tab->setTag(tag1);
    tab->setSchema(sch); */


    //cout << tab->getCodeDefinition(SchemaParser::XML_DEFINITION).toStdString() << endl;
    //model.addTag(tag);
    //model.addTag(tag1);
    model.createSystemObjects(true);
    model.loadModel("/root/tag_teste.dbm");


    //model.removeTag(tag);
    //model.removeTag(tag1);

    cout << model.getCodeDefinition(SchemaParser::XML_DEFINITION).toStdString() << endl;

    return(0);
  }
  catch(Exception &e)
  {
    cout << e.getExceptionsText().toStdString() << endl;
    return(e.getErrorType());
  }
}
