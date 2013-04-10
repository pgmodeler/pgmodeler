#include "excludeelement.h"

ExcludeElement::ExcludeElement(void) : Element()
{
 _operator=NULL;
}

void ExcludeElement::setOperator(Operator *oper)
{
	_operator=oper;
}

Operator *ExcludeElement::getOperator(void)
{
	return(_operator);
}

QString ExcludeElement::getCodeDefinition(unsigned def_type)
{
	map<QString, QString> attributes;

	attributes[ParsersAttributes::OPERATOR]="";

	configureAttributes(attributes, def_type);

	if(_operator)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::OPERATOR]=_operator->getName(true);
		else
			attributes[ParsersAttributes::OPERATOR]=_operator->getCodeDefinition(def_type, true);
	}

	return(SchemaParser::getCodeDefinition(ParsersAttributes::EXCLUDE_ELEMENT, attributes, def_type));
}
