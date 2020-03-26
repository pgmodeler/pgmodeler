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

#include "foreignserver.h"

ForeignServer::ForeignServer() : BaseObject()
{
	obj_type = ObjectType::ForeignServer;
	fdata_wrapper = nullptr;

	attributes[Attributes::Options] = QString();
	attributes[Attributes::Version] = QString();
	attributes[Attributes::Type] = QString();
	attributes[Attributes::Object] = QString();
}

void ForeignServer::setType(const QString &type)
{
	this->type = type;
}

void ForeignServer::setVersion(const QString &version)
{
	this->version = version;
}

void ForeignServer::setForeignDataWrapper(ForeignDataWrapper *fdw)
{
	fdata_wrapper = fdw;
}

QString ForeignServer::getType()
{
	return type;
}

QString ForeignServer::getVersion()
{
	return version;
}

ForeignDataWrapper *ForeignServer::getForeignDataWrapper()
{
	return fdata_wrapper;
}

QString ForeignServer::getCodeDefinition(unsigned def_type)
{
	return getCodeDefinition(def_type, false);
}

QString ForeignServer::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Version] = version;
	attributes[Attributes::Type] = type;
	attributes[Attributes::Fdw] = QString();

	if(fdata_wrapper)
	{
		if(def_type == SchemaParser::SqlDefinition)
			attributes[Attributes::Fdw] = fdata_wrapper->getName(true);
		else
			attributes[Attributes::Fdw] = fdata_wrapper->getCodeDefinition(def_type, true);
	}

	attributes[Attributes::Options] = getOptionsAttribute(def_type);

	return this->BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString ForeignServer::getAlterDefinition(BaseObject *object)
{
	try
	{
		ForeignServer *server=dynamic_cast<ForeignServer *>(object);
		attribs_map attribs;

		attributes[Attributes::AlterCmds] = BaseObject::getAlterDefinition(server);
		getAlteredAttributes(server, attribs);

		if(this->version != server->version)
			attribs[Attributes::Version] = server->version;

		copyAttributes(attribs);
		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
