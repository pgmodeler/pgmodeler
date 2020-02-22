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

#include "basetype.h"

BaseType::BaseType()
{
	type_idx = BaseType::Null;
}

BaseType::~BaseType()
{

}

unsigned BaseType::setType(unsigned type_id, const QStringList &type_list)
{
	//Raises an error if the type count is invalid
	if(type_list.isEmpty())
		throw Exception(ErrorCode::ObtTypesInvalidQuantity,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the type id is invalid
	else if(!isTypeValid(type_id, type_list))
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_idx = type_id;
	return type_idx;
}

unsigned BaseType::setType(const QString &type_name, const QStringList &type_list)
{
	return setType(static_cast<unsigned>(type_list.indexOf(type_name)), type_list);
}

bool BaseType::isTypeValid(unsigned type_id, const QStringList &type_list)
{
	//Returns if the type id is valid according to the specified interval (offset-count)
	return (type_id < static_cast<unsigned>(type_list.size()) || type_id==BaseType::Null);
}

QStringList BaseType::getTypes(const QStringList &type_list)
{
	QStringList types = type_list;
	types.removeAll("");
	return types;
}

unsigned BaseType::getType(const QString &type_name, const QStringList &type_list)
{
	if(type_name.isEmpty())
		return BaseType::Null;
	else
	{
		int idx = type_list.indexOf(type_name);

		if(idx >= 0)
			return static_cast<unsigned>(idx);

		return BaseType::Null;
	}
}

QString BaseType::getTypeName(unsigned type_id, const QStringList &type_list)
{
	if(type_id > static_cast<unsigned>(type_list.size()))
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return type_list[static_cast<int>(type_id)];
}

unsigned BaseType::operator ! ()
{
	return type_idx;
}

unsigned BaseType::getTypeId()
{
	return type_idx;
}

bool BaseType::operator == (BaseType &type)
{
	return (type.type_idx == type_idx);
}

bool BaseType::operator == (unsigned type_id)
{
	return (type_idx==type_id);
}

bool BaseType::operator != (BaseType &type)
{
	return (type.type_idx != type_idx);
}

bool BaseType::operator != (unsigned type_id)
{
	return (type_idx!=type_id);
}
