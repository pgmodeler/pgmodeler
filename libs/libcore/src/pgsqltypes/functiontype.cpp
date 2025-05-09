/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "functiontype.h"

const QStringList FunctionType::type_names {
	"", // Reserved for Class::Null
	"VOLATILE",
	"STABLE",
	"IMMUTABLE"
};

FunctionType::FunctionType(unsigned type_id)
{
	setType(type_id);
}

FunctionType::FunctionType()
{
	type_idx = Volatile;
}

QStringList FunctionType::getTypes()
{
	return TemplateType<FunctionType>::getTypes(type_names);
}

FunctionType::FunctionType(const QString &type_name)
{
	setType(type_name);
}

unsigned FunctionType::setType(unsigned type_id)
{
	return TemplateType<FunctionType>::setType(type_id, type_names);
}

unsigned FunctionType::setType(const QString &type_name)
{
	return TemplateType<FunctionType>::setType(type_name, type_names);
}

QString FunctionType::getTypeName(unsigned type_id)
{
	return TemplateType<FunctionType>::getTypeName(type_id, type_names);
}
