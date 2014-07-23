#include "excludeelement.h"

ExcludeElement::ExcludeElement(void) : Element()
{
 _operator=nullptr;
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
	attribs_map attributes;

	schparser.setPgSQLVersion(BaseObject::getPgSQLVersion());
	attributes[ParsersAttributes::OPERATOR]="";
	configureAttributes(attributes, def_type);

	if(_operator)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::OPERATOR]=_operator->getName(true);
		else
			attributes[ParsersAttributes::OPERATOR]=_operator->getCodeDefinition(def_type, true);
	}

	return(schparser.getCodeDefinition(ParsersAttributes::EXCLUDE_ELEMENT, attributes, def_type));
}

bool ExcludeElement::operator == (ExcludeElement &elem)
{
	return(this->_operator==elem._operator &&
				 *(dynamic_cast<Element *>(this))==dynamic_cast<Element &>(elem));
}
