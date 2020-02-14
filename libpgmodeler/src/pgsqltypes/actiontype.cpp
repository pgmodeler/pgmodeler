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

#include "actiontype.h"

QStringList ActionType::type_names =
{
	"NO ACTION",
	"RESTRICT",
	"CASCADE",
	"SET NULL",
	"SET DEFAULT"
};

ActionType::ActionType(unsigned type_id)
{
	(*this) = type_id;
}

ActionType::ActionType(const QString &type_name)
{
	(*this) = type_name;
}

ActionType::ActionType()
{
	type_idx = NoAction;
}

QStringList ActionType::getTypes()
{
	return type_names;
}

unsigned ActionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id, type_names);
	return type_idx;
}

unsigned ActionType::operator = (const QString &type_name)
{
	unsigned type_idx = BaseType::getType(type_name, type_names);
	BaseType::setType(type_idx, type_names);
	return type_idx;
}

QString ActionType::operator ~ ()
{
	return type_names[type_idx];
}
