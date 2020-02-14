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

#include "constrainttype.h"

QStringList ConstraintType::type_names =
{
	"", // Reserved for BaseType::null
	"PRIMARY KEY",
	"FOREIGN KEY",
	"CHECK",
	"UNIQUE",
	"EXCLUDE",
};

ConstraintType::ConstraintType(unsigned type_id)
{
	(*this) = type_id;
}

ConstraintType::ConstraintType(const QString &type_name)
{
	(*this) = type_name;
}

ConstraintType::ConstraintType()
{
	type_idx = PrimaryKey;
}

QStringList ConstraintType::getTypes()
{
	return BaseType::getTypes(type_names);
}

unsigned ConstraintType::operator = (unsigned type_id)
{
	BaseType::setType(type_id, type_names);
	return type_idx;
}

unsigned ConstraintType::operator = (const QString &type_name)
{
	unsigned type_id = BaseType::getType(type_name, type_names);
	BaseType::setType(type_id, type_names);
	return type_id;
}

QString ConstraintType::operator ~ ()
{
	return type_names[type_idx];
}
