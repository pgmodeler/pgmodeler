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

#include "permission.h"

const QString Permission::priv_codes("rawdDxtCcTXU");

Permission::Permission(BaseObject *obj)
{
	unsigned priv_id;

	//Initializes all the privileges as unchecked
	for(priv_id=PrivSelect; priv_id<=PrivUsage; priv_id++)
		privileges[priv_id]=grant_option[priv_id]=false;

	//Raises an error if the object associated to the permission is no allocated
	if(!obj)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Raises an error if the object type to be associated to the permission is
		invalid according to the rule (see class definition) */
	if(!acceptsPermission(obj->getObjectType()))
		throw Exception(ErrorCode::AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->object=obj;
	this->obj_type=ObjectType::Permission;
	revoke=cascade=false;

	attributes[Attributes::Object]=QString();
	attributes[Attributes::Type]=QString();
	attributes[Attributes::Parent]=QString();
	attributes[Attributes::GrantOp]=QString();
	attributes[Attributes::Roles]=QString();
	attributes[Attributes::Privileges]=QString();
	attributes[Attributes::Cascade]=QString();
	attributes[Attributes::PrivilegesGop]=QString();
}

bool Permission::acceptsPermission(ObjectType obj_type, int privilege)
{
	bool result=false;
	unsigned priv_id=static_cast<unsigned>(privilege);

	result=(obj_type==ObjectType::Table || obj_type==ObjectType::Column || obj_type==ObjectType::View ||
			obj_type==ObjectType::Sequence || obj_type==ObjectType::Database || obj_type==ObjectType::Function ||
			obj_type==ObjectType::Aggregate || obj_type==ObjectType::Language || obj_type==ObjectType::Schema ||
			obj_type==ObjectType::Tablespace || obj_type==ObjectType::Domain || obj_type==ObjectType::Type ||
			obj_type==ObjectType::ForeignDataWrapper || obj_type==ObjectType::ForeignServer ||
			obj_type==ObjectType::ForeignTable);


	//Validating privilege
	if(result && priv_id <= PrivUsage)
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
			View: SELECT | INSERT | UPDATE | DELETE | REFERENCES | TRIGGER
			Foreign Data Wrapper: USAGE
			Foreign Server: USAGE */
		result=result &&
				(((obj_type==ObjectType::Table || obj_type==ObjectType::View || obj_type==ObjectType::ForeignTable) &&
					(priv_id==PrivSelect || priv_id==PrivInsert ||
					 priv_id==PrivUpdate || priv_id==PrivDelete ||
					 priv_id==PrivReferences ||	priv_id==PrivTrigger)) ||

				((obj_type==ObjectType::Table || obj_type==ObjectType::View || obj_type==ObjectType::ForeignTable) && priv_id==PrivTruncate) ||

				(obj_type==ObjectType::Column &&
				 (priv_id==PrivSelect ||priv_id==PrivInsert ||
					priv_id==PrivUpdate || priv_id==PrivReferences)) ||

				(obj_type==ObjectType::Sequence &&
				 (priv_id==PrivUsage || priv_id==PrivSelect ||	priv_id==PrivUpdate)) ||

				(obj_type==ObjectType::Database &&
				 (priv_id==PrivCreate || priv_id==PrivConnect ||	priv_id==PrivTemporary)) ||

				((obj_type==ObjectType::Function || obj_type==ObjectType::Aggregate) && priv_id==PrivExecute) ||

				((obj_type==ObjectType::Language || obj_type==ObjectType::Type || obj_type==ObjectType::Domain) && priv_id==PrivUsage) ||

				(obj_type==ObjectType::Schema && (priv_id==PrivUsage || priv_id==PrivCreate)) ||

				(obj_type==ObjectType::Tablespace && priv_id==PrivCreate) ||

				((obj_type==ObjectType::ForeignDataWrapper ||  obj_type==ObjectType::ForeignServer) && priv_id==PrivUsage));
	}

	return result;
}

bool Permission::isRoleExists(Role *role)
{
	if(!role)	return false;
	return std::find(roles.begin(), roles.end(), role) != roles.end();
}

void Permission::addRole(Role *role)
{
	//Raises an error if the role is not allocated
	if(!role)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the role already exists in the permission
	if(isRoleExists(role))
		throw Exception(ErrorCode::InsDuplicatedRolePermission,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	roles.push_back(role);
	setCodeInvalidated(true);

	//Updates the permission Id
	generatePermissionId();
}

void Permission::setPrivilege(unsigned priv_id, bool value, bool grant_op)
{
	//Caso o tipo de privilégio sejá inválido dispara uma exceção
	if(priv_id > PrivUsage)
		throw Exception(ErrorCode::RefInvalidPrivilegeType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!acceptsPermission(object->getObjectType(), priv_id))
		//Raises an error if the privilege is invalid according to the object type
		throw Exception(ErrorCode::AsgInvalidPrivilegeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated((privileges[priv_id] != value) || grant_option[priv_id] != grant_op);
	privileges[priv_id]=value;
	this->grant_option[priv_id]=grant_op;
	generatePermissionId();
}

void Permission::setRevoke(bool value)
{
	setCodeInvalidated(revoke != value);
	revoke=value;
}

void Permission::setCascade(bool value)
{
	setCodeInvalidated(revoke && (cascade != value));
	cascade=(revoke && value);
}

bool Permission::isRevoke()
{
	return revoke;
}

bool Permission::isCascade()
{
	return cascade;
}

bool Permission::isSimilarTo(Permission *perm)
{
	if(!perm)
		return false;

	QStringList rol_names, fmt_rol_names;
	vector<vector<Role *>*> vect_roles={ &this->roles, &perm->roles };
	BaseObject *object=this->getObject(), *aux_object=perm->getObject();

	//Generating a list with role names of both permissions
	for(auto &roles : vect_roles)
	{
		for(auto &role : *roles)
			rol_names.append(role->getName());

		rol_names.sort();
		fmt_rol_names.append(rol_names.join(','));
		rol_names.clear();
	}

	return (((object==aux_object) ||
					 (object && aux_object && object->getSignature()==aux_object->getSignature())) &&
					this->getPermissionString()==perm->getPermissionString() &&
					this->revoke==perm->revoke &&
					fmt_rol_names[0]==fmt_rol_names[1]);

}

void Permission::removeRole(unsigned role_idx)
{
	if(role_idx > roles.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	roles.erase(roles.begin() + role_idx);
	generatePermissionId();
	setCodeInvalidated(true);
}

void Permission::removeRoles()
{
	roles.clear();
	generatePermissionId();
	setCodeInvalidated(true);
}

Role *Permission::getRole(unsigned role_idx)
{
	if(role_idx > roles.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return roles[role_idx];
}

vector<Role *> Permission::getRoles()
{
	return roles;
}

unsigned Permission::getRoleCount()
{
	return roles.size();
}

BaseObject *Permission::getObject()
{
	return object;
}

bool Permission::getPrivilege(unsigned priv_id)
{
	//Raises an error if the privilege is invalid
	if(priv_id > PrivUsage)
		throw Exception(ErrorCode::RefInvalidPrivilegeType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return privileges[priv_id];
}

bool Permission::getGrantOption(unsigned priv_id)
{
	//Raises an error if the privilege is invalid
	if(priv_id > PrivUsage)
		throw Exception(ErrorCode::RefInvalidPrivilegeType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return grant_option[priv_id];
}

QString Permission::getPermissionString()
{
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

	return str_priv;
}

QString Permission::parsePermissionString(QString perm_str, vector<unsigned> &privs, vector<unsigned> &gop_privs)
{
	QString role;
	QRegExp regexp(QString("(.)*(\\=)([%1*])+((\\/)(.)+)?").arg(priv_codes));

	privs.clear();
	gop_privs.clear();

	//Checking if the permission string is valid
	if(!perm_str.isEmpty() && regexp.exactMatch(perm_str))
	{
		QStringList list=perm_str.remove(perm_str.indexOf('/'), perm_str.size()).split('=');
		QChar chr;
		QString codes=list[1];
		int priv=-1, i=0;
		bool gop=false;

		role=list[0];

		while(i < codes.size())
		{
			chr=codes[i];

			//Get the privilege code
			if(chr!='*')
				priv=priv_codes.indexOf(chr);

			//Checking if the next char is the GRANT OPTION descriptor
			if(((i+1) < codes.size()) && (codes[i+1]=='*'))
			{
				i+=2;
				gop=true;
			}
			else
				i++;

			if(priv >= 0)
			{
				/* If the grant option flag is checked insert the privilege
				on the grant option list instead of ordinary privilete list */
				if(gop)
					gop_privs.push_back(priv);
				else
					privs.push_back(priv);

				priv=-1;
				gop=false;
			}
		}
	}

	return role;
}

void Permission::generatePermissionId()
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
			if(i < count-1) str_aux+='.';
		}

	}
	/* If no role is associated with permissions (public)
		generates an identifier with zeros indicating that permission
		is not linked directly to any role on the model */
	else
		str_aux+=QString("000000");

	//Generates an unique name for the permission through md5 hash
	hash.addData(QByteArray(str_aux.toStdString().c_str()));
	str_aux=hash.result().toHex();
	this->obj_name=(!revoke ? QString("grant_") : QString("revoke_")) + str_aux.mid(0,10);
}

QString Permission::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	unsigned i, count;
	ObjectType obj_type;
	QString priv_vect[12]={ Attributes::SelectPriv, Attributes::InsertPriv,
							Attributes::UpdatePriv, Attributes::DeletePriv,
							Attributes::TruncatePriv, Attributes::ReferencesPriv,
							Attributes::TriggerPriv, Attributes::CreatePriv,
							Attributes::ConnectPriv, Attributes::TemporaryPriv,
							Attributes::ExecutPriv, Attributes::UsagePriv };

	obj_type=object->getObjectType();

	attributes[Attributes::Revoke]=(revoke ? Attributes::True : QString());
	attributes[Attributes::Cascade]=(cascade ? Attributes::True : QString());

	if(def_type==SchemaParser::SqlDefinition)
	{
		if(obj_type == ObjectType::View || obj_type == ObjectType::ForeignTable)
			//Views, Tables and foreign tables use the same keyword when setting permission (TABLE)
			attributes[Attributes::Type] = BaseObject::getSQLName(ObjectType::Table);
		else if(obj_type == ObjectType::ForeignServer)
			attributes[Attributes::Type] = QString("FOREIGN ") + object->getSQLName();
		else
			attributes[Attributes::Type] = BaseObject::getSQLName(obj_type);
	}
	else
		attributes[Attributes::Type]=BaseObject::getSchemaName(obj_type);

	if(obj_type==ObjectType::Column)
	{
		attributes[Attributes::Object]=object->getName(true);
		attributes[Attributes::Parent]=dynamic_cast<Column *>(object)->getParentTable()->getName(true);
	}
	else
		attributes[Attributes::Object]=object->getSignature();

	if(def_type==SchemaParser::XmlDefinition)
	{
		for(i=0; i < 12; i++)
		{
			if(privileges[i] && grant_option[i])
				attributes[priv_vect[i]]=Attributes::GrantOp;
			else if(privileges[i])
				attributes[priv_vect[i]]=Attributes::True;
			else
				attributes[priv_vect[i]]=QString();
		}
	}
	else
	{
		QStringList priv_list, gop_priv_list;

		for(i=0; i < 12; i++)
		{
			if(privileges[i] && !grant_option[i])
				priv_list.push_back(object->getObjectType() == ObjectType::Column ? QString("%1(%2)").arg(priv_vect[i].toUpper()).arg(object->getName(true)) : priv_vect[i].toUpper());
			else if(grant_option[i])
				gop_priv_list.push_back(object->getObjectType() == ObjectType::Column ? QString("%1(%2)").arg(priv_vect[i].toUpper()).arg(object->getName(true)) : priv_vect[i].toUpper());
		}

		attributes[Attributes::Privileges] = priv_list.join(QChar(','));
		attributes[Attributes::PrivilegesGop] = gop_priv_list.join(QChar(','));
	}

	count=roles.size();

	for(i=0; i < count; i++)
		attributes[Attributes::Roles]+=roles[i]->getName(true) + QString(",");

	attributes[Attributes::Roles].remove(attributes[Attributes::Roles].size()-1,1);

	return BaseObject::__getCodeDefinition(def_type);
}

QString Permission::getSignature(bool format)
{
	QStringList rol_names, words;
	QString signature;

	for(Role *role : roles)
		rol_names.push_back(role->getName(format));

	rol_names.sort();
	signature=QString("=") + getPermissionString();

	if(roles.empty())
		signature=QString("PUBLIC") + signature;
	else
		signature=rol_names.join(',') + signature;

	words=this->obj_name.split("_");
	signature=words[0] + QChar(':') + signature + QString(" [id:%1]").arg(words[1]);
	return signature;
}

QString Permission::getDropDefinition(bool cascade)
{
	try
	{
		QString def;

		this->setRevoke(!revoke);
		this->setCascade(cascade);
		def=this->getCodeDefinition(SchemaParser::SqlDefinition);
		this->setRevoke(revoke);
		this->setCascade(this->cascade);

		return def;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
