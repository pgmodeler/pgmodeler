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

#include "operatorfamily.h"

OperatorFamily::OperatorFamily(void)
{
	obj_type=OBJ_OPFAMILY;
	attributes[ParsersAttributes::INDEX_TYPE]="";
}

void OperatorFamily::setIndexingType(IndexingType idx_type)
{
	indexing_type=idx_type;
}

IndexingType OperatorFamily::getIndexingType(void)
{
	return(indexing_type);
}

QString OperatorFamily::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString OperatorFamily::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
	return(BaseObject::getCodeDefinition(def_type,reduced_form));
}

