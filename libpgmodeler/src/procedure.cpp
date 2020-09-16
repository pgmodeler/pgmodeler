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

#include "procedure.h"

Procedure::Procedure() : BaseFunction()
{
	obj_type = ObjectType::Procedure;
}

void Procedure::addParameter(Parameter param)
{
	if(param.isOut() && !param.isIn())
		throw Exception(ErrorCode::InvProcedureParamOutMode, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	BaseFunction::addParameter(param);
}

void Procedure::configureSearchAttributes()
{

}

QString Procedure::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def = getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	setBasicFunctionAttributes(def_type);

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Procedure::getCodeDefinition(unsigned def_type)
{
	return getCodeDefinition(def_type, false);
}

QString Procedure::getAlterDefinition(BaseObject *)
{
	return "";
}
