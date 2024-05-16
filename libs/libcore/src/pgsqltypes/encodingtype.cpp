/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "encodingtype.h"

EncodingType::EncodingType()
{
	type_idx = 1;
}

EncodingType::EncodingType(const QString &type_name)
{
	setType(type_name);
}

EncodingType::EncodingType(unsigned type_id)
{
	setType(type_id);
}

QStringList EncodingType::getTypes()
{
	return TemplateType<EncodingType>::getTypes(type_names);
}

bool EncodingType::operator == (const char *type_name)
{
	return ((*this) == QString(type_name));
}

bool EncodingType::operator == (const QString &type_name)
{
	return (type_idx == static_cast<unsigned>(type_names.indexOf(type_name)));
}

bool EncodingType::operator != (const QString &type_name)
{
	return !((*this) == type_name);
}

bool EncodingType::operator != (EncodingType type)
{
	return (this->type_idx != type.type_idx);
}

bool EncodingType::operator != (unsigned type_id)
{
	return (this->type_idx != type_id);
}

unsigned EncodingType::setType(unsigned type_id)
{
	return TemplateType<EncodingType>::setType(type_id, type_names);
}

unsigned EncodingType::setType(const QString &type_name)
{
	return TemplateType<EncodingType>::setType(type_name, type_names);
}

QString EncodingType::getTypeName(unsigned type_id)
{
	return TemplateType<EncodingType>::getTypeName(type_id, type_names);
}
