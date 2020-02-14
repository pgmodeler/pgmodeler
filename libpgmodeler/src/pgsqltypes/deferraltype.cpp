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

#include "deferraltype.h"

QStringList DeferralType::type_names =
{
	"", // Reserved for BaseType::null
	"INITIALLY IMMEDIATE",
	"INITIALLY DEFERRED"
};

DeferralType::DeferralType()
{
	type_idx = Immediate;
}

DeferralType::DeferralType(const QString &type_name)
{
	(*this)=type_name;
}

DeferralType::DeferralType(unsigned type_id)
{
	(*this)=type_id;
}

QStringList DeferralType::getTypes()
{
	return BaseType::getTypes(type_names);
}

unsigned DeferralType::operator = (unsigned type_id)
{
	BaseType::setType(type_id, type_names);
	return type_idx;
}

unsigned DeferralType::operator = (const QString &type_name)
{
	unsigned type_id = BaseType::getType(type_name, type_names);
	BaseType::setType(type_id, type_names);
	return type_id;
}

QString DeferralType::operator ~ ()
{
	return type_names[type_idx];
}
