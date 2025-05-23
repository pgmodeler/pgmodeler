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

#include "behaviortype.h"

const QStringList BehaviorType::type_names
{
	"", // Reserved for Class::Null

	"CALLED ON NULL INPUT",
	"RETURNS NULL ON NULL INPUT"//,
	//"STRICT"
};

BehaviorType::BehaviorType(unsigned type_id)
{
	setType(type_id);
}

BehaviorType::BehaviorType(const QString &type_name)
{
	setType(type_name);
}

BehaviorType::BehaviorType()
{
	type_idx = CalledOnNullInput;
}

QStringList BehaviorType::getTypes()
{
	return TemplateType<BehaviorType>::getTypes(type_names);
}

unsigned BehaviorType::setType(unsigned type_id)
{
	return TemplateType<BehaviorType>::setType(type_id, type_names);
}

unsigned BehaviorType::setType(const QString &type_name)
{
	return TemplateType<BehaviorType>::setType(type_name, type_names);
}

QString BehaviorType::getTypeName(unsigned type_id)
{
	return TemplateType<BehaviorType>::getTypeName(type_id, type_names);
}
