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

#include "operatorfamily.h"

OperatorFamily::OperatorFamily()
{
	obj_type=ObjectType::OpFamily;
	attributes[Attributes::IndexType]=QString();
}

void OperatorFamily::setIndexingType(IndexingType idx_type)
{
	setCodeInvalidated(indexing_type != idx_type);
	indexing_type=idx_type;
}

IndexingType OperatorFamily::getIndexingType()
{
	return indexing_type;
}

QString OperatorFamily::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, false);
}

QString OperatorFamily::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Signature]=getSignature();
	attributes[Attributes::IndexType]=(~indexing_type);
	return BaseObject::getCodeDefinition(def_type,reduced_form);
}

QString OperatorFamily::getSignature(bool format)
{
	return BaseObject::getSignature(format) + QString(" USING %1").arg(~indexing_type);
}
