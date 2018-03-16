/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "policy.h"

Policy::Policy(void) : TableObject()
{
	obj_type=OBJ_POLICY;
	restrictive = false;

	for(unsigned cmd = CMD_SELECT; cmd <= CMD_DELETE; cmd++)
		affected_cms[cmd] = false;
}

void Policy::setParentTable(BaseTable *table)
{
	if(table && table->getObjectType() != OBJ_TABLE)
	{
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_OBJECT_TYPE)
						.arg(this->obj_name)
						.arg(this->getTypeName())
						.arg(BaseObject::getTypeName(OBJ_TABLE)),
						ERR_ASG_INV_OBJECT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	TableObject::setParentTable(table);
}

void Policy::setRestrictive(bool value)
{
	setCodeInvalidated(restrictive != value);
	restrictive = value;
}

bool Policy::isRestrictive(void)
{
	return(restrictive);
}

void Policy::setAffectedCommand(unsigned cmd, bool value)
{
	if(cmd > CMD_DELETE)
		throw Exception(Exception::getErrorMessage(ERR_REF_INV_AFFECTED_CMD).arg(this->obj_name),
										ERR_REF_INV_AFFECTED_CMD,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(affected_cms[cmd] != value);
	affected_cms[cmd] = value;
}

bool Policy::isCommandAffected(unsigned cmd)
{
	if(cmd > CMD_DELETE)
		throw Exception(Exception::getErrorMessage(ERR_REF_INV_AFFECTED_CMD).arg(this->obj_name),
										ERR_REF_INV_AFFECTED_CMD,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(affected_cms[cmd]);
}

void Policy::setUsingExpression(const QString &expr)
{
	setCodeInvalidated(using_expr != expr);
	using_expr = expr;
}

QString Policy::getUsingExpression(void)
{
	return(using_expr);
}

void Policy::setCheckExpression(const QString &expr)
{
	setCodeInvalidated(check_expr != expr);
	check_expr = expr;
}

QString Policy::getCheckExpression(void)
{
	return(check_expr);
}

void Policy::addRole(Role *role)
{
	if(!role)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(std::find(roles.begin(), roles.end(), role) == roles.end())
		roles.push_back(role);
}

void Policy::removeRoles(void)
{
	roles.clear();
	setCodeInvalidated(true);
}

QString Policy::getCodeDefinition(unsigned def_type)
{

}

QString Policy::getSignature(bool format)
{
	if(!getParentTable())
		return(BaseObject::getSignature(format));

	return(QString("%1 ON %2").arg(this->getName(format)).arg(getParentTable()->getSignature(true)));
}

QString Policy::getDropDefinition(bool cascade)
{

}

QString Policy::getAlterDefinition(BaseObject *object)
{

}
