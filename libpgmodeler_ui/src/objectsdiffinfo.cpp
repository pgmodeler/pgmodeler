/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "objectsdiffinfo.h"

const unsigned ObjectsDiffInfo::CREATE_OBJECT=0;
const unsigned ObjectsDiffInfo::DROP_OBJECT=1;
const unsigned ObjectsDiffInfo::ALTER_OBJECT=2;
const unsigned ObjectsDiffInfo::NO_DIFFERENCE=3;

ObjectsDiffInfo::ObjectsDiffInfo(void)
{
	src_object=comp_object=nullptr;
	diff_type=NO_DIFFERENCE;
}

ObjectsDiffInfo::ObjectsDiffInfo(unsigned diff_type, BaseObject *src_object, BaseObject *comp_object)
{
	this->diff_type=diff_type;
	this->src_object=src_object;
	this->comp_object=comp_object;
}

unsigned ObjectsDiffInfo::getDiffType(void)
{
	return(diff_type);
}

QString ObjectsDiffInfo::getInfoMessage(void)
{

	if(diff_type==DROP_OBJECT)
	{
		QString msg=QT_TR_NOOP("Object `%1' `(%2)' will be dropped.");
		return(msg.arg(src_object->getName()).arg(src_object->getTypeName()));
	}

	return("");
}
