/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "indexelement.h"

IndexElement::IndexElement(void) : Element()
{
	collation=nullptr;
}

void IndexElement::setCollation(Collation *collation)
{
	this->collation=collation;
}

Collation *IndexElement::getCollation(void)
{
	return(collation);
}

QString IndexElement::getCodeDefinition(unsigned def_type)
{
	map<QString, QString> attributes;

	attributes[ParsersAttributes::COLLATION]="";

	configureAttributes(attributes, def_type);

	if(collation)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::COLLATION]=collation->getName(true);
		else
			attributes[ParsersAttributes::COLLATION]=collation->getCodeDefinition(def_type, true);
	}

	return(SchemaParser::getCodeDefinition(ParsersAttributes::INDEX_ELEMENT,attributes, def_type));
}

