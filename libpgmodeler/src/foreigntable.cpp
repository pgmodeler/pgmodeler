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

#include "foreigntable.h"

ForeignTable::ForeignTable(void) : PhysicalTable(ObjectType::ForeignTable)
{
	PhysicalTable::setName(trUtf8("new_foreign_table"));
}

ForeignTable::~ForeignTable(void)
{
	destroyObjects();
}

QString ForeignTable::__getCodeDefinition(unsigned def_type, bool incl_rel_added_objs)
{
	setTableAttributes(def_type, incl_rel_added_objs);
	return(PhysicalTable::__getCodeDefinition(def_type));
}

QString ForeignTable::getCodeDefinition(unsigned def_type)
{
	QString code_def = PhysicalTable::getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	return(__getCodeDefinition(def_type, false));
}

void ForeignTable::operator = (ForeignTable &tab)
{
	(*dynamic_cast<PhysicalTable *>(this))=dynamic_cast<PhysicalTable &>(tab);
}

QString ForeignTable::getAlterDefinition(BaseObject *object)
{

}
