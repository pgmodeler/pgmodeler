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

#include "encodingtype.h"

EncodingType::EncodingType()
{
	type_idx=Offset;
}

EncodingType::EncodingType(const QString &type)
{
	(*this)=type;
}

EncodingType::EncodingType(const unsigned type_id)
{
	(*this)=type_id;
}

void EncodingType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,Offset,TypesCount);
}

unsigned EncodingType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,Offset,TypesCount);
	return type_idx;
}

unsigned EncodingType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, Offset, TypesCount);
	BaseType::setType(type_id,Offset,TypesCount);
	return type_id;
}

bool EncodingType::operator == (const char *type_name)
{
	return ((*this)==QString(type_name));
}

bool EncodingType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=Offset + TypesCount;

	for(idx=Offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return (type_idx==idx);
}

bool EncodingType::operator != (const QString &type_name)
{
	return (!((*this)==type_name));
}

bool EncodingType::operator != (EncodingType type)
{
	return (this->type_idx!=type.type_idx);
}

bool EncodingType::operator != (unsigned type_id)
{
	return (this->type_idx!=type_id);
}

QString EncodingType::operator ~ ()
{
	return type_list[type_idx];
}
