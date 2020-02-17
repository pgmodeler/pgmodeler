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

#include "storagetype.h"

template<>
QStringList StorageType::TemplateType<StorageType>::type_names =
{
	"", // Reserved for BaseType::null

	"plain",
	"external",
	"extended",
	"main"
};

StorageType::StorageType()
{
	type_idx = Plain;
}

StorageType::StorageType(unsigned type_id)
{
	setType(type_id);
}

StorageType::StorageType(const QString &type_name)
{
	setType(type_name);
}

bool StorageType::operator == (const char *type_name)
{
	return ((*this)==QString(type_name));
}

bool StorageType::operator == (const QString &type_name)
{
	return (type_idx == static_cast<unsigned>(type_names.indexOf(type_name)));
}

bool StorageType::operator != (const QString &type_name)
{
	return (!((*this)==type_name));
}

bool StorageType::operator != (StorageType type)
{
	return (this->type_idx!=type.type_idx);
}
