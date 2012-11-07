#include "elementoindice.h"

IndexElement::IndexElement(void)
{
 column=NULL;
 operator_class=NULL;
 attributes[NULLS_FIRST]=false;
 attributes[ASC_ORDER]=true;
}

void IndexElement::setColumn(Column *column)
{
 if(column)
 {
  this->column=column;
  this->expression="";
 }
}

void IndexElement::setExpression(const QString &expression)
{
 if(!expression.isEmpty())
 {
  this->expression=expression;
  this->column=NULL;
 }
}

void IndexElement::setOperatorClass(OperatorClass *oper_class)
{
 this->operator_class=oper_class;
}

void IndexElement::setAttribute(unsigned attrib, bool value)
{
 if(attrib > NULLS_FIRST)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 attributes[attrib]=value;
}

bool IndexElement::getAttribute(unsigned attrib)
{
 if(attrib > NULLS_FIRST)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 return(attributes[attrib]);
}

Column *IndexElement::getColumn(void)
{
 return(column);
}

QString IndexElement::getExpression(void)
{
 return(expression);
}

OperatorClass *IndexElement::getOperatorClass(void)
{
 return(operator_class);
}

QString IndexElement::getCodeDefinition(unsigned def_type)
{
 map<QString, QString> attributes;

 attributes[ParsersAttributes::COLUMN]="";
 attributes[ParsersAttributes::EXPRESSION]="";
 attributes[ParsersAttributes::OP_CLASS]="";
 
 attributes[ParsersAttributes::NULLS_FIRST]=(attributes[NULLS_FIRST] ? "1" : "");
 attributes[ParsersAttributes::ASC_ORDER]=(attributes[ASC_ORDER] ? "1" : "");
 
 if(column)
  attributes[ParsersAttributes::COLUMN]=column->getName(true);
 else
  attributes[ParsersAttributes::EXPRESSION]=expression;
 
 if(operator_class)
 {
  if(def_type==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::OP_CLASS]=operator_class->getName(true);
  else
   attributes[ParsersAttributes::OP_CLASS]=operator_class->getCodeDefinition(def_type, true);
 }
 
 return(SchemaParser::getObjectDefinition(ParsersAttributes::INDEX_ELEMENT,attributes, def_type));
}

