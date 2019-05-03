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

#include "genericsql.h"

GenericSQL::GenericSQL(void)
{
	obj_type = ObjectType::GenericSql;
	attributes[Attributes::Definition] = QString();
}

void GenericSQL::setDefinition(const QString &def)
{
	setCodeInvalidated(definition != def);
	definition = def;
}

QString GenericSQL::getDefinition(void)
{
	return(definition);
}

int GenericSQL::getObjectRefNameIndex(const QString &ref_name)
{
	int idx = -1;
	vector<RefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.end();

	if(ref_name.isEmpty())
		return(-1);

	while(itr != itr_end)
	{
		if((*itr).ref_name == ref_name)
		{
			idx = itr - objects_refs.begin();
			break;
		}

		itr++;
	}

	return(idx);
}

bool GenericSQL::isObjectReferenced(BaseObject *object)
{
	bool found = false;
	vector<RefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.end();

	if(!object)
		return(false);

	while(itr != itr_end && !found)
	{
		found = ((*itr).object == object);
		itr++;
	}

	return(found);
}

void GenericSQL::validateObjectReference(GenericSQL::RefConfig ref, bool ignore_duplic)
{
	if(!ref.object)
		throw Exception(ErrorCode::AsgNotAllocatedObjectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!BaseObject::isValidName(ref.ref_name))
		throw Exception(ErrorCode::AsgInvalidNameObjReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!ignore_duplic && getObjectRefNameIndex(ref.ref_name) >= 0)
		throw Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedObjectReference).arg(ref.ref_name),
										ErrorCode::InsDuplicatedObjectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void GenericSQL::addObjectReference(BaseObject *object, const QString &ref_name, bool use_signature, bool format_name)
{
	try
	{
		RefConfig ref = RefConfig(ref_name, object, use_signature, format_name);
		validateObjectReference(ref, false);
		objects_refs.push_back(ref);
		setCodeInvalidated(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void GenericSQL::updateObjectReference(const QString &ref_name, BaseObject *object, const QString &new_ref_name, bool use_signature, bool format_name)
{
	int idx = getObjectRefNameIndex(ref_name);

	if(idx < 0)
		return;

	try
	{
		RefConfig ref = RefConfig(new_ref_name, object, use_signature, format_name);
		vector<RefConfig>::iterator itr = objects_refs.begin() + idx;
		int idx_aux = getObjectRefNameIndex(new_ref_name);

		if(idx_aux != idx)
			throw Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedObjectReference).arg(new_ref_name),
											ErrorCode::InsDuplicatedObjectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		validateObjectReference(ref, true);
		(*itr) = ref;
		setCodeInvalidated(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void GenericSQL::removeObjectReference(const QString &ref_name)
{
	int idx = getObjectRefNameIndex(ref_name);

	if(idx >= 0)
		objects_refs.erase(objects_refs.begin() + idx);

	setCodeInvalidated(true);
}

void GenericSQL::removeObjectReferences(void)
{
	objects_refs.clear();
	setCodeInvalidated(true);
}

QString GenericSQL::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	if(objects_refs.empty())
		attributes[Attributes::Definition] = definition;
	else
	{
		QString fmt_definition = definition, ref_name, ref_value;

		for(auto &ref : objects_refs)
		{
			ref_name = QString("{%1}").arg(ref.ref_name);
			ref_value = ref.use_signature ?
										ref.object->getSignature(ref.format_name) :
										ref.object->getName(ref.format_name);

			fmt_definition = fmt_definition.replace(ref_name, ref_value);
		}

		attributes[Attributes::Definition] = fmt_definition;
	}

	return(this->BaseObject::__getCodeDefinition(def_type));
}
