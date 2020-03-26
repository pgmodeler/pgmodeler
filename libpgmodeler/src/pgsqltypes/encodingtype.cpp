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

#include "encodingtype.h"

template<>
QStringList EncodingType::TemplateType<EncodingType>::type_names = {
	"", // Reserved for BaseType::null

	"UTF8", "BIG5", "EUC_CN", "EUC_JP", "EUC_JIS_2004",
	"EUC_KR", "EUC_TW", "GB18030", "GBK",
	"ISO_8859_5", "ISO_8859_6", "ISO_8859_7", "ISO_8859_8",
	"JOHAB", "KOI8R", "KOI8U", "LATIN1",
	"LATIN2", "LATIN3", "LATIN4", "LATIN5",
	"LATIN6", "LATIN7", "LATIN8", "LATIN9",
	"LATIN10", "MULE_INTERNAL", "SJIS", "SHIFT_JIS_2004",
	"SQL_ASCII", "UHC", "WIN866", "WIN874",
	"WIN1250", "WIN1251", "WIN1252", "WIN1253",
	"WIN1254", "WIN1255", "WIN1256", "WIN1257", "WIN1258"
};

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
