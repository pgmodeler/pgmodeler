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

#include "foreigntable.h"

ForeignTable::ForeignTable(void) : PhysicalTable()
{
	setName(trUtf8("new_foreign_table"));
	attributes[Attributes::Server] = QString();
	obj_type = ObjectType::ForeignTable;
	foreign_server = nullptr;
}

ForeignTable::~ForeignTable(void)
{
	destroyObjects();
}

void ForeignTable::setForeignServer(ForeignServer *server)
{
	setCodeInvalidated(foreign_server != server);
	foreign_server = server;
}

ForeignServer *ForeignTable::getForeignServer(void)
{
	return(foreign_server);
}

void ForeignTable::addObject(BaseObject *object, int obj_idx)
{
	if(object)
	{
		ObjectType obj_type = object->getObjectType();

		// If the child object is of an invalid type we need to reject that object
		if(obj_type == ObjectType::Index || obj_type == ObjectType::Rule || obj_type == ObjectType::Policy ||
			 (obj_type == ObjectType::Constraint &&
				dynamic_cast<Constraint *>(object)->getConstraintType() != ConstraintType::Check))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidObjectForeignTable)
											.arg(object->getName(true))
											.arg(object->getTypeName())
											.arg(this->getName(true)),
											ErrorCode::AsgInvalidObjectForeignTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}

	PhysicalTable::addObject(object, obj_idx);
}

QString ForeignTable::__getCodeDefinition(unsigned def_type, bool incl_rel_added_objs)
{
	setTableAttributes(def_type, incl_rel_added_objs);

	if(foreign_server)
	{
		attributes[Attributes::Server] = (def_type == SchemaParser::SqlDefinition ?
																			foreign_server->getSignature() :
																			foreign_server->getCodeDefinition(SchemaParser::XmlDefinition, true));
	}

	return(PhysicalTable::__getCodeDefinition(def_type));
}

QString ForeignTable::getCodeDefinition(unsigned def_type)
{
	QString code_def = PhysicalTable::getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	return(__getCodeDefinition(def_type, false));
}

void ForeignTable::operator = (ForeignTable &tab)
{
	(*dynamic_cast<PhysicalTable *>(this))=dynamic_cast<PhysicalTable &>(tab);
}

QString ForeignTable::getAlterDefinition(BaseObject *object)
{

}
