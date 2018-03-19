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

	for(unsigned rol_id = ROLE_CURRENT_USER; rol_id <= ROLE_PUBLIC; rol_id++)
		special_roles[rol_id] = false;

	attributes[ParsersAttributes::RESTRICTIVE] = QString();
	attributes[ParsersAttributes::SELECT_PRIV] = QString();
	attributes[ParsersAttributes::INSERT_PRIV] = QString();
	attributes[ParsersAttributes::DELETE_PRIV] = QString();
	attributes[ParsersAttributes::UPDATE_PRIV] = QString();
	attributes[ParsersAttributes::USING_EXP] = QString();
	attributes[ParsersAttributes::CHECK_EXP] = QString();
	attributes[ParsersAttributes::ROLES] = QString();
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

void Policy::setSpecialRole(unsigned rol_id, bool value)
{
	if(rol_id > ROLE_PUBLIC)
		throw Exception(Exception::getErrorMessage(ERR_REF_INV_SPECIAL_ROLE).arg(this->obj_name),
										ERR_REF_INV_SPECIAL_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(special_roles[rol_id] != value);
	special_roles[rol_id] = value;
}

bool Policy::isSpecialRoleSet(unsigned rol_id)
{
	if(rol_id > ROLE_PUBLIC)
		throw Exception(Exception::getErrorMessage(ERR_REF_INV_SPECIAL_ROLE).arg(this->obj_name),
										ERR_REF_INV_SPECIAL_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(special_roles[rol_id]);
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
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	QStringList cmd_str = { ParsersAttributes::SELECT_PRIV, ParsersAttributes::INSERT_PRIV,
													ParsersAttributes::UPDATE_PRIV, ParsersAttributes::DELETE_PRIV },
			sp_roles = { ParsersAttributes::ROLE_CURRENT_USER, ParsersAttributes::ROLE_SESSION_USER,
									 ParsersAttributes::ROLE_PUBLIC },
			rol_names;

	if(getParentTable())
		attributes[ParsersAttributes::TABLE]=getParentTable()->getName(true);

	for(unsigned cmd = CMD_SELECT; cmd <= CMD_DELETE; cmd++)
		attributes[cmd_str[cmd]] = (affected_cms[cmd] ? ParsersAttributes::_TRUE_ : QString());

	for(unsigned rol_id = ROLE_CURRENT_USER; rol_id <= ROLE_PUBLIC; rol_id++)
	{
		if(special_roles[rol_id])
			rol_names.append(sp_roles[rol_id]);
	}

	for(auto role : roles)
		rol_names.append(role->getName(true));

	attributes[ParsersAttributes::RESTRICTIVE] = (restrictive ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::USING_EXP] = using_expr;
	attributes[ParsersAttributes::CHECK_EXP] = check_expr;
	attributes[ParsersAttributes::ROLES] = rol_names.join(QString(", "));

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Policy::getSignature(bool format)
{
	if(!getParentTable())
		return(BaseObject::getSignature(format));

	return(QString("%1 ON %2").arg(this->getName(format)).arg(getParentTable()->getSignature(true)));
}

QString Policy::getAlterDefinition(BaseObject *object)
{
	return("");
}
