/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

IndexElement::IndexElement() : Element()
{
	collation=nullptr;
}

void IndexElement::setCollation(Collation *collation)
{
	this->collation=collation;
}

Collation *IndexElement::getCollation()
{
	return collation;
}

QString IndexElement::getCodeDefinition(unsigned def_type)
{
	attribs_map attributes;

	schparser.setPgSQLVersion(BaseObject::getPgSQLVersion());
	attributes[Attributes::Collation]=QString();
	configureAttributes(attributes, def_type);

	if(collation)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Collation]=collation->getName(true);
		else
			attributes[Attributes::Collation]=collation->getCodeDefinition(def_type, true);
	}

	return schparser.getCodeDefinition(Attributes::IndexElement,attributes, def_type);
}

