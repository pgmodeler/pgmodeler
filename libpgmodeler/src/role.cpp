/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

Role::Role(void)
{
	obj_type=OBJ_ROLE;
	object_id=Role::role_id++;

	for(unsigned i=0; i <= OP_REPLICATION; i++)
		options[i]=false;

	conn_limit=-1;

	attributes[ParsersAttributes::SUPERUSER]=QString();
	attributes[ParsersAttributes::CREATEDB]=QString();
	attributes[ParsersAttributes::CREATEROLE]=QString();
	attributes[ParsersAttributes::INHERIT]=QString();
	attributes[ParsersAttributes::LOGIN]=QString();
	attributes[ParsersAttributes::CONN_LIMIT]=QString();
	attributes[ParsersAttributes::PASSWORD]=QString();
	attributes[ParsersAttributes::ENCRYPTED]=QString();
	attributes[ParsersAttributes::VALIDITY]=QString();
	attributes[ParsersAttributes::REF_ROLES]=QString();
	attributes[ParsersAttributes::MEMBER_ROLES]=QString();
	attributes[ParsersAttributes::ADMIN_ROLES]=QString();
	attributes[ParsersAttributes::REPLICATION]=QString();
	attributes[ParsersAttributes::GROUP]=QString();
}

void Role::setOption(unsigned op_type, bool value)
{
	if(op_type > OP_REPLICATION)
		//Raises an error if the option type is invalid
		throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Uncheck all the other options when OP_SUPERUSER is true
	for(unsigned i=OP_CREATEDB; (op_type==OP_SUPERUSER && value) && i <= OP_REPLICATION; i++)
		options[i]=false;

	setCodeInvalidated(options[op_type] != value);

  if(op_type!=OP_ENCRYPTED)
    options[op_type]=(!options[OP_SUPERUSER] && value);
  else
    options[op_type]=value;
}

void Role::addRole(unsigned role_type, Role *role)
{
	//Raises an error if the role to be added is not allocated
	if(!role)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the role to be added is the 'this' role
	else if(role && this==role)
		throw Exception(Exception::getErrorMessage(ERR_ROLE_MEMBER_ITSELF)
                    .arg(role->getName()),
										ERR_ROLE_MEMBER_ITSELF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		bool role_ref, role_mem, role_adm,
				role_ref1, role_mem1, role_adm1;

		//Check if the role to be added already exists in one of the internal role list
		role_ref=this->isRoleExists(REF_ROLE, role);
		role_mem=this->isRoleExists(MEMBER_ROLE, role);
		role_adm=this->isRoleExists(ADMIN_ROLE, role);

		/* Check if the role 'this' is referenced in one of the internal role list
		 of the role to be added */
		role_ref1=role->isRoleExists(REF_ROLE, this);
		role_mem1=role->isRoleExists(MEMBER_ROLE, this);
		role_adm1=role->isRoleExists(ADMIN_ROLE, this);

		//Raises an error if the role already exists in one of the internal list
		if((role_type==REF_ROLE && role_ref) ||
			 (role_type==MEMBER_ROLE && (role_mem || role_adm)) ||
			 (role_type==ADMIN_ROLE && (role_adm || role_mem)))
			throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
                      .arg(role->getName())
                      .arg(this->getName()),
											ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		else if((role_type==REF_ROLE && ((role_mem || role_adm) || role_ref1)) ||
						(role_type==MEMBER_ROLE && ((role_mem1 || role_adm1) || role_ref)) ||
						(role_type==ADMIN_ROLE &&  ((role_mem1 || role_adm1) || role_ref)))
			throw Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
                      .arg(this->getName())
                      .arg(role->getName()),
											ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			switch(role_type)
			{
				case MEMBER_ROLE: member_roles.push_back(role); break;
				case ADMIN_ROLE:  admin_roles.push_back(role); break;
				case REF_ROLE:
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
		case MEMBER_ROLE:
			roles_vect=&member_roles;
			attrib=ParsersAttributes::MEMBER_ROLES;
		break;
		case ADMIN_ROLE:
			roles_vect=&admin_roles;
			attrib=ParsersAttributes::ADMIN_ROLES;
		break;
		case REF_ROLE:
		default:
			roles_vect=&ref_roles;
			attrib=ParsersAttributes::REF_ROLES;
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
		case REF_ROLE: list=&ref_roles; break;
		case MEMBER_ROLE: list=&member_roles; break;
		case ADMIN_ROLE: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	if(role_idx >= list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin() + role_idx;
	list->erase(itr);
	setCodeInvalidated(true);
}

void Role::removeRoles(unsigned role_type)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case REF_ROLE: list=&ref_roles; break;
		case MEMBER_ROLE: list=&member_roles; break;
		case ADMIN_ROLE: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		case REF_ROLE: list=&ref_roles; break;
		case MEMBER_ROLE: list=&member_roles; break;
		case ADMIN_ROLE: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	itr=list->begin();
	itr_end=list->end();
	while(!found && itr!=itr_end)
	{
		found=((*itr)==role);
		itr++;
	}

	return(found);
}

bool Role::getOption(unsigned op_type)
{
	if(op_type > OP_REPLICATION)
		throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(options[op_type]);
}

Role *Role::getRole(unsigned role_type, unsigned role_idx)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case REF_ROLE: list=&ref_roles; break;
		case MEMBER_ROLE: list=&member_roles; break;
		case ADMIN_ROLE: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	//Raises an error if the role index is invalid (out of bound)
	if(role_idx > list->size())
		throw Exception(ERR_REF_ROLE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(list->at(role_idx));
}

unsigned Role::getRoleCount(unsigned role_type)
{
	vector<Role *> *list=nullptr;

	switch(role_type)
	{
		case REF_ROLE: list=&ref_roles; break;
		case MEMBER_ROLE: list=&member_roles; break;
		case ADMIN_ROLE: list=&admin_roles; break;
		default:
			//Raises an error if the role type is invalid
			throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	return(list->size());
}

unsigned Role::getConnectionLimit(void)
{
	return(conn_limit);
}

QString Role::getValidity(void)
{
	return(validity);
}

QString Role::getPassword(void)
{
	return(password);
}

QString Role::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	unsigned i;
	QString op_attribs[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
												 ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
												 ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED,
												 ParsersAttributes::REPLICATION };

	setRoleAttribute(REF_ROLE);
	setRoleAttribute(MEMBER_ROLE);
	setRoleAttribute(ADMIN_ROLE);

	for(i=0; i <= OP_REPLICATION; i++)
		attributes[op_attribs[i]]=(options[i] ? ParsersAttributes::_TRUE_ : QString());

	attributes[ParsersAttributes::PASSWORD]=password;
	attributes[ParsersAttributes::VALIDITY]=validity;

	if(conn_limit >= 0)
		attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(conn_limit);

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Role::getAlterDefinition(BaseObject *object, bool ignore_name_diff)
{
  try
  {
    Role *role=dynamic_cast<Role *>(object);
    attribs_map attribs;
    QString op_attribs[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                           ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                           ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED,
                           ParsersAttributes::REPLICATION };

    attributes[ParsersAttributes::ALTER_CMDS]=BaseObject::getAlterDefinition(object, ignore_name_diff);

    if(this->password!=role->password)
      attribs[ParsersAttributes::PASSWORD]=role->password;

    if(this->validity!=role->validity)
      attribs[ParsersAttributes::VALIDITY]=role->validity;

    for(unsigned i=0; i <= OP_REPLICATION; i++)
    {
      if((attribs.count(ParsersAttributes::PASSWORD) && i==OP_ENCRYPTED) ||
         this->options[i]!=role->options[i])
        attribs[op_attribs[i]]=(role->options[i] ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);
    }

    copyAttributes(attribs);

    return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
