/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "partitionkey.h"

PartitionKey::PartitionKey(void)
{
	collation = nullptr;
}

void PartitionKey::setCollation(Collation *collation)
{
	this->collation=collation;
}

Collation *PartitionKey::getCollation(void)
{
	return(collation);
}

QString PartitionKey::getCodeDefinition(unsigned def_type)
{
	attribs_map attribs;
	schparser.setPgSQLVersion(BaseObject::getPgSQLVersion());

	attribs[ParsersAttributes::COLLATION]=QString();
	configureAttributes(attribs, def_type);

	if(collation)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attribs[ParsersAttributes::COLLATION]=collation->getName(true);
		else
			attribs[ParsersAttributes::COLLATION]=collation->getCodeDefinition(def_type, true);
	}

	return(schparser.getCodeDefinition(ParsersAttributes::PARTITION_KEY, attribs, def_type));
}
