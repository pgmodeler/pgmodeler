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

#include "role.h"

unsigned Role::role_id=0;

Role::Role()
{
	obj_type=ObjectType::Role;
	object_id=Role::role_id++;

	for(unsigned i=0; i <= OpBypassRls; i++)
		options[i]=false;

	conn_limit=-1;

	attributes[Attributes::Superuser]=QString();
	attributes[Attributes::CreateDb]=QString();
	attributes[Attributes::CreateRole]=QString();
	attributes[Attributes::Inherit]=QString();
	attributes[Attributes::Login]=QString();
	attributes[Attributes::ConnLimit]=QString();
	attributes[Attributes::Password]=QString();
	attributes[Attributes::Encrypted]=QString();
	attributes[Attributes::Validity]=QString();
	attributes[Attributes::RefRoles]=QString();
	attributes[Attributes::MemberRoles]=QString();
	attributes[Attributes::AdminRoles]=QString();
	attributes[Attributes::Replication]=QString();
	attributes[Attributes::Group]=QString();
	attributes[Attributes::BypassRls]=QString();
	attributes[Attributes::EmptyPassword]=QString();
}

void Role::setOption(unsigned op_type, bool value)
{
	if(op_type > OpBypassRls)
		//Raises an error if the option type is invalid
		throw Exception(ErrorCode::AsgValueInvalidRoleOptionType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(options[op_type] != value);
	options[op_type]=value;
}

void Role::addRole(unsigned role_type, Role *role)
{
	//Raises an error if the role to be added is not allocated
	if(!role)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the role to be added is the 'this' role
	else if(role && this==role)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgRoleMemberItself)
						.arg(role->getName()),
						ErrorCode::AsgRoleMemberItself,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		bool role_ref, role_mem, role_adm,
				role_ref1, role_mem1, role_adm1;

		//Check if the role to be added already exists in one of the internal role list
		role_ref=this->isRoleExists(RefRole, role);
		role_mem=this->isRoleExists(MemberRole, role);
		role_adm=this->isRoleExists(AdminRole, role);

		/* Check if the role 'this' is referenced in one of the internal role list
		 of the role to be added */
		role_ref1=role->isRoleExists(RefRole, this);
		role_mem1=role->isRoleExists(MemberRole, this);
		role_adm1=role->isRoleExists(AdminRole, this);

		//Raises an error if the role already exists in one of the internal list
		if((role_type==RefRole && role_ref) ||
				(role_type==MemberRole && (role_mem || role_adm)) ||
				(role_type==AdminRole && (role_adm || role_mem)))
			throw Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedRole)
							.arg(role->getName())
							.arg(this->getName()),
							ErrorCode::InsDuplicatedRole,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Checking for redundant reference between roles.
			A redundant reference can happen when:

			1) The role 'this' is already part of the 'ref_roles' list of 'role' object and
				 the user try to add the object 'role' (from parameter) as an element
				 of 'ref_roles' list of role 'this'

			2) The role 'this' is already part of the 'member_roles' list of 'role' object
				 and the user try to add the object 'role' (from parameter) as an element
				 of the 'member_roles' of role 'this'

			3) The role 'this' is already part of the 'admin_roles' list of 'role' object and
				 the user try to add the 'role' object (from parameter) as an element
				 of 'admin_roles' list of role 'this'

			4) The role 'role' (from parameter) is already part of the 'member_roles' or 'admin_roles' list of
				 the 'this' role and the user try to add the object 'role' as an element of
				 the 'ref_roles' of the role 'this'

			5) The role 'role' (from parameter) is already part of the 'ref_roles' list of role 'this'
				 and the user try to add the object 'role' as an element of the 'member_roles' list
				 of the role 'this' */
		else if((role_type==RefRole && ((role_mem || role_adm) || role_ref1)) ||
				(role_type==MemberRole && ((role_mem1 || role_adm1) || role_ref)) ||
				(role_type==AdminRole &&  ((role_mem1 || role_adm1) || role_ref)))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgRoleReferenceRedundancy)
							.arg(this->getName())
							.arg(role->getName()),
							ErrorCode::AsgRoleReferenceRedundancy,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			switch(role_type)
			{
				case MemberRole: member_roles.push_back(role); break;
				case AdminRole:  admin_roles.push_back(role); break;
				case RefRole:
				default:
					ref_roles.push_back(role);
				break;
			}

			setCodeInvalidated(true);
		}
	}
}

void Role::setConnectionLimit(int limit)
{
	setCodeInvalidated(conn_limit != limit);
	conn_limit=limit;
}

void Role::setValidity(const QString &date)
{
	setCodeInvalidated(validity != date);
	validity=date.mid(0,19);
}

void Role::setPassword(const QString &passwd)
{
	setCodeInvalidated(password != passwd);
	this->password=passwd;
}

void Role::setRoleAttribute(unsigned role_type)
{
	QString str_roles, attrib;
	unsigned i, count;
	vector<Role *>  *roles_vect=nullptr;

	switch(role_type)
	{
		case MemberRole:
			roles_vect=&member_roles;
			attrib=Attributes::MemberRoles;
		break;
		case AdminRole:
			roles_vect=&admin_roles;
			attrib=Attributes::AdminRoles;
		break;
		case RefRole:
		default:
			roles_vect=&ref_roles;
			attrib=Attributes::RefRoles;
		break;
	}

	count=roles_vect->size();
	for(i=0; i < count; i++)
	{
		str_roles+=roles_vect->at(i)->getName(true);
		if(i < (count-1)) str_roles+=QString(",");
	}

	attributes[attrib]=str_roles;
}

void Role::removeRole(unsigned role_type, unsigned role_idx)
{
	vector<Role *> *list=nullptr;
	vector<Role *>::iterator itr;

	switch(role_type)
	{
		case RefRole: list=&ref_roles; break;
		case MemberRole: list=&member_roles; break;
		case AdminRole: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ErrorCode::RefInvalidRoleType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	if(role_idx >= list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin() + role_idx;
	list->erase(itr);
	setCodeInvalidated(true);
}

void Role::removeRoles(unsigned role_type)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case RefRole: list=&ref_roles; break;
		case MemberRole: list=&member_roles; break;
		case AdminRole: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ErrorCode::RefInvalidRoleType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	list->clear();
	setCodeInvalidated(true);
}

bool Role::isRoleExists(unsigned role_type, Role *role)
{
	vector<Role *> *list=nullptr;
	vector<Role *>::iterator itr, itr_end;
	bool found=false;

	switch(role_type)
	{
		case RefRole: list=&ref_roles; break;
		case MemberRole: list=&member_roles; break;
		case AdminRole: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ErrorCode::RefInvalidRoleType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	itr=list->begin();
	itr_end=list->end();
	while(!found && itr!=itr_end)
	{
		found=((*itr)==role);
		itr++;
	}

	return found;
}

bool Role::getOption(unsigned op_type)
{
	if(op_type > OpBypassRls)
		throw Exception(ErrorCode::AsgValueInvalidRoleOptionType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return options[op_type];
}

Role *Role::getRole(unsigned role_type, unsigned role_idx)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case RefRole: list=&ref_roles; break;
		case MemberRole: list=&member_roles; break;
		case AdminRole: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ErrorCode::RefInvalidRoleType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	//Raises an error if the role index is invalid (out of bound)
	if(role_idx > list->size())
		throw Exception(ErrorCode::RefRoleInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return list->at(role_idx);
}

unsigned Role::getRoleCount(unsigned role_type)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case RefRole: list=&ref_roles; break;
		case MemberRole: list=&member_roles; break;
		case AdminRole: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ErrorCode::RefInvalidRoleType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return list->size();
}

unsigned Role::getConnectionLimit()
{
	return conn_limit;
}

QString Role::getValidity()
{
	return validity;
}

QString Role::getPassword()
{
	return password;
}

QString Role::getCodeDefinition(unsigned def_type)
{
	return (getCodeDefinition(def_type, false));
}

QString Role::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	unsigned i;
	QString op_attribs[]={ Attributes::Superuser, Attributes::CreateDb,
						   Attributes::CreateRole, Attributes::Inherit,
						   Attributes::Login, Attributes::Encrypted,
							 Attributes::Replication, Attributes::BypassRls };

	setRoleAttribute(RefRole);
	setRoleAttribute(MemberRole);
	setRoleAttribute(AdminRole);

	for(i=0; i <= OpBypassRls; i++)
		attributes[op_attribs[i]]=(options[i] ? Attributes::True : QString());

	attributes[Attributes::Password]=password;
	attributes[Attributes::Validity]=validity;

	if(conn_limit >= 0)
		attributes[Attributes::ConnLimit]=QString("%1").arg(conn_limit);

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Role::getAlterDefinition(BaseObject *object, bool ignore_name_diff)
{
	Role *role=dynamic_cast<Role *>(object);

	if(!role)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		QString op_attribs[]={ Attributes::Superuser, Attributes::CreateDb,
							   Attributes::CreateRole, Attributes::Inherit,
							   Attributes::Login, Attributes::Encrypted,
								 Attributes::Replication, Attributes::BypassRls };

		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object, ignore_name_diff);

		if(this->password!=role->password)
		{
			attribs[Attributes::EmptyPassword]=role->password.isEmpty() ? Attributes::True : QString();
			attribs[Attributes::Password]=role->password;
		}

		if(this->validity!=role->validity)
			attribs[Attributes::Validity]=role->validity;

		for(unsigned i=0; i <= OpBypassRls; i++)
		{
			if((attribs.count(Attributes::Password) && i==OpEncrypted) ||	this->options[i]!=role->options[i])
				attribs[op_attribs[i]]=(role->options[i] ? Attributes::True : Attributes::Unset);
		}

		copyAttributes(attribs);

		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
