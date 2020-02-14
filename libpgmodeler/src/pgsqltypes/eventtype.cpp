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

#include "eventtype.h"

EventType::EventType()
{
	type_idx=Offset;
}

EventType::EventType(const QString &type_name)
{
	(*this)=type_name;
}

EventType::EventType(unsigned type_id)
{
	(*this)=type_id;
}

void EventType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,Offset,TypesCount);
}

unsigned EventType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,Offset,TypesCount);
	return type_idx;
}

unsigned EventType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, Offset, TypesCount);
	BaseType::setType(type_id,Offset,TypesCount);
	return type_id;
}

bool EventType::operator < (EventType type) const
{
	return (type_idx < type.type_idx);
}

bool EventType::operator < (unsigned type_id) const
{
	return (type_idx < type_id);
}

QString EventType::operator ~ ()
{
	return type_list[type_idx];
}
