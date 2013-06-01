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

#include "permission.h"

Permission::Permission(BaseObject *obj)
{
	unsigned priv_id;

	//Initializes all the privileges as unchecked
	for(priv_id=PRIV_SELECT; priv_id<=PRIV_USAGE; priv_id++)
		privileges[priv_id]=grant_option[priv_id]=false;

	//Raises an error if the object associated to the permission is no allocated
	if(!obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Raises an error if the object type to be associated to the permission is
		invalid according to the rule (see class definition) */
	if(!objectAcceptsPermission(obj->getObjectType()))
		throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->object=obj;
	this->obj_type=OBJ_PERMISSION;
	revoke=cascade=false;

	attributes[ParsersAttributes::OBJECT]="";
	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::PARENT]="";
	attributes[ParsersAttributes::GRANT_OP]="";
	attributes[ParsersAttributes::ROLES]="";
	attributes[ParsersAttributes::PRIVILEGES]="";
	attributes[ParsersAttributes::CASCADE]="";
	attributes[ParsersAttributes::PRIVILEGES_GOP]="";
}

bool Permission::objectAcceptsPermission(ObjectType obj_type, int privilege)
{
	bool result=false;
	unsigned priv_id=static_cast<unsigned>(privilege);

	result=(obj_type==OBJ_TABLE || obj_type==OBJ_COLUMN || obj_type==OBJ_VIEW ||
					obj_type==OBJ_SEQUENCE || obj_type==OBJ_DATABASE || obj_type==OBJ_FUNCTION ||
					obj_type==OBJ_AGGREGATE || obj_type==OBJ_LANGUAGE || obj_type==OBJ_SCHEMA ||
					obj_type==OBJ_TABLESPACE);


	//Validating privilege
	if(result && priv_id <= PRIV_USAGE)
	{

		/* Some privileges are valid only for certain types
			of objects. If the user try to assign a privilege P
			for an object that does not accept this privilege the same
			is ignored. The schema below shows the privileges for
			each object:

			Table:   SELECT | INSERT | UPDATE | DELETE | TRUNCATE | REFERENCES | TRIGGER
			Column:   SELECT | INSERT | UPDATE | REFERENCES
			Sequence:  USAGE | SELECT | UPDATE
			Database: CREATE | CONNECT | TEMPORARY | TEMP
			Function: EXECUTE
			Aggregate: EXECUTE
			Linguage: USAGE
			Schema: CREATE | USAGE
			Tablespace: CREATE
			View: SELECT | INSERT | UPDATE | DELETE | REFERENCES | TRIGGER */
		result=result &&

		(((obj_type==OBJ_TABLE || obj_type==OBJ_VIEW) &&
			 (priv_id==PRIV_SELECT || priv_id==PRIV_INSERT ||
				priv_id==PRIV_UPDATE || priv_id==PRIV_DELETE ||
				priv_id==PRIV_REFERENCES ||	priv_id==PRIV_TRIGGER)) ||

		 (obj_type==OBJ_TABLE && priv_id==PRIV_TRUNCATE) ||

		 (obj_type==OBJ_COLUMN &&
			(priv_id==PRIV_SELECT ||priv_id==PRIV_INSERT ||
			 priv_id==PRIV_UPDATE || priv_id==PRIV_REFERENCES)) ||

		 (obj_type==OBJ_SEQUENCE &&
			(priv_id==PRIV_USAGE || priv_id==PRIV_SELECT ||	priv_id==PRIV_UPDATE)) ||

		 (obj_type==OBJ_DATABASE &&
			(priv_id==PRIV_CREATE || priv_id==PRIV_CONNECT ||	priv_id==PRIV_TEMPORARY)) ||

		 ((obj_type==OBJ_FUNCTION || obj_type==OBJ_AGGREGATE) && priv_id==PRIV_EXECUTE) ||

		 (obj_type==OBJ_LANGUAGE && priv_id==PRIV_USAGE) ||

		 (obj_type==OBJ_SCHEMA && (priv_id==PRIV_USAGE || priv_id==PRIV_CREATE)) ||

		 (obj_type==OBJ_TABLESPACE && priv_id==PRIV_CREATE));
	}

	return(result);
}

bool Permission::isRoleExists(Role *role)
{
	vector<Role *>::iterator itr, itr_end;
	bool found=false;

	itr=roles.begin();
	itr_end=roles.end();

	while(itr!=itr_end && !found)
	{
		found=(*itr)==role;
		itr++;
	}

	return(found);
}

void Permission::addRole(Role *role)
{
	//Raises an error if the role is not allocated
	if(!role)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the role already exists in the permission
	if(isRoleExists(role))
		throw Exception(ERR_INS_DUP_ROLE_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	roles.push_back(role);

	//Updates the permission Id
	generatePermissionId();
}

void Permission::setPrivilege(unsigned priv_id, bool value, bool grant_op)
{
	//Caso o tipo de privilégio sejá inválido dispara uma exceção
	if(priv_id > PRIV_USAGE)
		throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!objectAcceptsPermission(object->getObjectType(), priv_id))
		//Raises an error if the privilege is invalid according to the object type
		throw Exception(ERR_ASG_INCOMP_PRIV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	privileges[priv_id]=value;
	this->grant_option[priv_id]=grant_op;
	generatePermissionId();
}

void Permission::setRevoke(bool value)
{
	revoke=value;
}

void Permission::setCascade(bool value)
{
	cascade=value;
}

bool Permission::isRevoke(void)
{
	return(revoke);
}

bool Permission::isCascade(void)
{
	return(cascade);
}

void Permission::removeRole(unsigned role_idx)
{
	if(role_idx > roles.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	generatePermissionId();
}

void Permission::removeRoles(void)
{
	roles.clear();
	generatePermissionId();
}

Role *Permission::getRole(unsigned role_idx)
{
	if(role_idx > roles.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(roles[role_idx]);
}

unsigned Permission::getRoleCount(void)
{
	return(roles.size());
}

BaseObject *Permission::getObject(void)
{
	return(object);
}

bool Permission::getPrivilege(unsigned priv_id)
{
	//Raises an error if the privilege is invalid
	if(priv_id > PRIV_USAGE)
		throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(privileges[priv_id]);
}

bool Permission::getGrantOption(unsigned priv_id)
{
	//Raises an error if the privilege is invalid
	if(priv_id > PRIV_USAGE)
		throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(grant_option[priv_id]);
}

QString Permission::getPrivilegeString(void)
{
	unsigned char priv_codes[13]="rawdDxtCcTXU";
	QString str_priv;
	unsigned i;

	/* Schema to build the privilege string:

		rolename=xxxx -- privileges granted to a role
						=xxxx -- privileges granted to PUBLIC

						r -- SELECT ("read")
						a -- INSERT ("append")
						w -- UPDATE ("write")
						d -- DELETE
						D -- TRUNCATE
						x -- REFERENCES
						t -- TRIGGER
						C -- CREATE
						c -- CONNECT
						T -- TEMPORARY
						X -- EXECUTE
						U -- USAGE

			arwdDxt -- ALL PRIVILEGES (for tables, varies for other objects)
						* -- grant option for preceding privilege
				/yyyy -- role that granted this privilege  */
	for(i=0; i < 12; i++)
	{
		if(privileges[i])
			str_priv.append(priv_codes[i]);

		if(grant_option[i])
			str_priv.append(QChar('*'));
	}

	return(str_priv);
}

void Permission::generatePermissionId(void)
{
	vector<Role *>::iterator itr, itr_end;
	vector<QString> addr_vect;
	Role *role=nullptr;
	QString str_aux, addr;
	unsigned i, count;
	QTextStream stream(&addr);
	QCryptographicHash hash(QCryptographicHash::Md5);

	//Stores the permission address on a string
	stream << reinterpret_cast<unsigned *>(this);
	str_aux=addr.mid(2);
	addr.clear();

	//Generates the id only when there is associated roles to the permission
	if(roles.size() > 0)
	{
		itr=roles.begin();
		itr_end=roles.end();

		while(itr!=itr_end)
		{
			role=(*itr);
			//Convertes the role address to string and use it as the id for the permission
			stream << reinterpret_cast<unsigned *>(role);
			addr_vect.push_back(addr.mid(2));
			itr++;
		}

		sort(addr_vect.begin(), addr_vect.end());
		count=addr_vect.size();

		for(i=0; i < count; i++)
		{
			str_aux+=QString("%1").arg(addr_vect[i]);
			if(i < count-1) str_aux+=".";
		}

	}
	/* If no role is associated with permissions (public)
		generates an identifier with zeros indicating that permission
		is not linked directly to any role on the model */
	else
		str_aux+="000000";

	//Generates an unique name for the permission through md5 hash
	hash.addData(QByteArray(str_aux.toStdString().c_str()));
	str_aux=hash.result().toHex();
	this->obj_name=(!revoke ? QString("grant_") : QString("revoke_")) + str_aux.mid(0,10);
}

QString Permission::getCodeDefinition(unsigned def_type)
{
	unsigned i, count;
	ObjectType obj_type;
	QString priv_vect[12]={ ParsersAttributes::SELECT_PRIV, ParsersAttributes::INSERT_PRIV,
													ParsersAttributes::UPDATE_PRIV, ParsersAttributes::DELETE_PRIV,
													ParsersAttributes::TRUNCATE_PRIV, ParsersAttributes::REFERENCES_PRIV,
													ParsersAttributes::TRIGGER_PRIV, ParsersAttributes::CREATE_PRIV,
													ParsersAttributes::CONNECT_PRIV, ParsersAttributes::TEMPORARY_PRIV,
													ParsersAttributes::EXECUTE_PRIV, ParsersAttributes::USAGE_PRIV };

	obj_type=object->getObjectType();

	attributes[ParsersAttributes::REVOKE]=(revoke ? "1" : "");
	attributes[ParsersAttributes::CASCADE]=(cascade ? "1" : "");

	if(obj_type==OBJ_FUNCTION)
		attributes[ParsersAttributes::OBJECT]=dynamic_cast<Function *>(object)->getSignature();
	else
		attributes[ParsersAttributes::OBJECT]=object->getName(true);

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		//Views and Tables uses the same key word when setting permission (TABLE)
		attributes[ParsersAttributes::TYPE]=
		 (object->getObjectType()==OBJ_VIEW ? BaseObject::getSQLName(OBJ_TABLE): BaseObject::getSQLName(object->getObjectType()));
	}
	else
		attributes[ParsersAttributes::TYPE]=BaseObject::getSchemaName(object->getObjectType());

	if(obj_type==OBJ_COLUMN)
		attributes[ParsersAttributes::PARENT]=dynamic_cast<Column *>(object)->getParentTable()->getName(true);

	if(def_type==SchemaParser::XML_DEFINITION)
	{
		for(i=0; i < 12; i++)
		{
			if(privileges[i] && grant_option[i])
				attributes[priv_vect[i]]=ParsersAttributes::GRANT_OP;
			else if(privileges[i])
				attributes[priv_vect[i]]=ParsersAttributes::_TRUE_;
			else
				attributes[priv_vect[i]]="";
		}
	}
	else
	{
		for(i=0; i < 12; i++)
		{
			if(privileges[i] && !grant_option[i])
				attributes[ParsersAttributes::PRIVILEGES]+=priv_vect[i].toUpper() + ",";
			else if(grant_option[i])
				attributes[ParsersAttributes::PRIVILEGES_GOP]+=priv_vect[i].toUpper() + ",";
		}

		attributes[ParsersAttributes::PRIVILEGES].remove(attributes[ParsersAttributes::PRIVILEGES].size()-1,1);
		attributes[ParsersAttributes::PRIVILEGES_GOP].remove(attributes[ParsersAttributes::PRIVILEGES_GOP].size()-1,1);
	}

	count=roles.size();

	for(i=0; i < count; i++)
		attributes[ParsersAttributes::ROLES]+=roles[i]->getName(true) + ",";

	attributes[ParsersAttributes::ROLES].remove(attributes[ParsersAttributes::ROLES].size()-1,1);

	return(BaseObject::__getCodeDefinition(def_type));
}
