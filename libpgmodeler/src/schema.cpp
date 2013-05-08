/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "schema.h"

Schema::Schema(void)
{
	obj_type=OBJ_SCHEMA;
	fill_color=QColor(225,225,225, 80);
	rect_visible=false;
	attributes[ParsersAttributes::FILL_COLOR]="";
	attributes[ParsersAttributes::RECT_VISIBLE]="";
}

void Schema::setName(const QString &name)
{
	/* Schema names starting with pg_ is reserved to PostgreSQL if its the case
		raises an error */
	if(name.mid(0,3)=="pg_")
		throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_SCHEMA)),
										ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setName(name);
}

void Schema::setFillColor(const QColor &color)
{
	this->fill_color=color;
}

QColor Schema::getFillColor(void)
{
	return(fill_color);
}

void Schema::setRectVisible(bool value)
{
	rect_visible=value;
}

bool Schema::isRectVisible(void)
{
	return(rect_visible);
}

QString Schema::getCodeDefinition(unsigned def_type)
{
	attributes[ParsersAttributes::FILL_COLOR]=fill_color.name();
	attributes[ParsersAttributes::RECT_VISIBLE]=(rect_visible ? "1" : "");
	return(BaseObject::__getCodeDefinition(def_type));
}
