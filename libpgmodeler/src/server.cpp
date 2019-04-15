/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "server.h"

Server::Server(void) : ForeignObject()
{
	obj_type = ObjectType::Server;
	fdata_wrapper = nullptr;

	attributes[Attributes::Options] = QString();
	attributes[Attributes::Version] = QString();
	attributes[Attributes::Type] = QString();
	attributes[Attributes::Object] = QString();
}

void Server::setType(const QString &type)
{
	this->type = type;
}

void Server::setVersion(const QString &version)
{
	this->version = version;
}

void Server::setForeignDataWrapper(ForeignDataWrapper *fdw)
{
	fdata_wrapper = fdw;
}

QString Server::getType(void)
{
	return(type);
}

QString Server::getVersion(void)
{
	return(version);
}

ForeignDataWrapper *Server::getForeignDataWrapper(void)
{
	return(fdata_wrapper);
}

QString Server::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[Attributes::Version] = version;
	attributes[Attributes::Type] = type;
	attributes[Attributes::Fdw] = QString();

	if(fdata_wrapper)
	{
		attributes[Attributes::Fdw] = def_type == SchemaParser::SqlDefinition ?
																		fdata_wrapper->getName(true) :
																		fdata_wrapper->getCodeDefinition(def_type, true);
	}

	setOptionsAttribute(def_type);
	return(this->BaseObject::__getCodeDefinition(def_type));
}
