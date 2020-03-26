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

#include "foreigntable.h"

ForeignTable::ForeignTable() : PhysicalTable()
{
	obj_type = ObjectType::ForeignTable;
	attributes[Attributes::Server] = QString();
	attributes[Attributes::Options] = QString();
	foreign_server = nullptr;

	setName(tr("new_foreign_table"));
}

ForeignTable::~ForeignTable()
{
	destroyObjects();
}

void ForeignTable::setForeignServer(ForeignServer *server)
{
	setCodeInvalidated(foreign_server != server);
	foreign_server = server;
}

ForeignServer *ForeignTable::getForeignServer()
{
	return foreign_server;
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

void ForeignTable::setPartitioningType(PartitioningType)
{
	PhysicalTable::setPartitioningType(PartitioningType::Null);
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

	attributes[Attributes::Options] = getOptionsAttribute(def_type);
	return PhysicalTable::__getCodeDefinition(def_type);
}

QString ForeignTable::getCodeDefinition(unsigned def_type)
{
	QString code_def = PhysicalTable::getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	return __getCodeDefinition(def_type, false);
}

void ForeignTable::operator = (ForeignTable &tab)
{
	(*dynamic_cast<PhysicalTable *>(this))=dynamic_cast<PhysicalTable &>(tab);
}

QString ForeignTable::getAlterDefinition(BaseObject *object)
{
	try
	{
		attribs_map attribs;
		attributes[Attributes::AlterCmds] = BaseObject::getAlterDefinition(object);
		getAlteredAttributes(dynamic_cast<ForeignObject *>(object), attribs);
		copyAttributes(attribs);
		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
