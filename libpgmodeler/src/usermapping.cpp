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

#include "usermapping.h"

UserMapping::UserMapping(void) : ForeignObject()
{
	obj_type = ObjectType::UserMapping;
	foreign_server = nullptr;

	attributes[Attributes::Version] = QString();
	attributes[Attributes::Object] = QString();
}

void UserMapping::setForeignServer(ForeignServer *server)
{
	foreign_server = server;
}

ForeignServer *UserMapping::getForeignServer(void)
{
	return(foreign_server);
}

QString UserMapping::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	/*attributes[Attributes::Version] = version;
	attributes[Attributes::Type] = type;
	attributes[Attributes::Fdw] = QString();

	if(fdata_wrapper)
	{
		if(def_type == SchemaParser::SqlDefinition)
			attributes[Attributes::Fdw] = fdata_wrapper->getName(true);
		else
			attributes[Attributes::Fdw] = fdata_wrapper->getCodeDefinition(def_type, true);
	}

	setOptionsAttribute(def_type);
	return(this->BaseObject::__getCodeDefinition(def_type));*/
}

QString UserMapping::getAlterDefinition(BaseObject *object)
{
	/*try
	{
		ForeignServer *server=dynamic_cast<ForeignServer *>(object);
		attribs_map attribs;

		attributes[Attributes::AlterCmds] = ForeignObject::getAlterDefinition(server);

		if(this->version != server->version)
			attribs[Attributes::Version] = server->version;

		copyAttributes(attribs);
		return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}*/
}
