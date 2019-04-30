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

bool GenericSQL::isObjectRefNameExists(const QString &ref_name)
{
	bool found = false;
	vector<RefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.begin();

	if(!ref_name.isEmpty())
		return(false);

	while(itr != itr_end && !found)
	{
		found = ((*itr).ref_name == ref_name);
		itr++;
	}

	return(found);
}

bool GenericSQL::isObjectReferenced(BaseObject *object)
{
	bool found = false;
	vector<RefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.begin();

	if(!object)
		return(false);

	while(itr != itr_end && !found)
	{
		found = ((*itr).object == object);
		itr++;
	}

	return(found);
}

void GenericSQL::addObjectReference(const QString &ref_name, BaseObject *object, bool use_signature, bool format_name)
{
	if(BaseObject::isValidName(ref_name))
		throw Exception(ErrorCode::AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!object)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(isObjectRefNameExists(ref_name))
		throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	objects_refs.push_back(RefConfig(ref_name, object, use_signature, format_name));
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
		QString fmt_definition, ref_name, ref_value;

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
