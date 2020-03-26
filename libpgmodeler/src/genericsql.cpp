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

#include "genericsql.h"
#include "tableobject.h"

GenericSQL::GenericSQL()
{
	obj_type = ObjectType::GenericSql;
	attributes[Attributes::Definition] = QString();
	attributes[Attributes::Objects] = QString();
}

void GenericSQL::setDefinition(const QString &def)
{
	setCodeInvalidated(definition != def);
	definition = def;
}

QString GenericSQL::getDefinition()
{
	return definition;
}

vector<GenericSQL::ObjectRefConfig> GenericSQL::getObjectsReferences()
{
	return objects_refs;
}

int GenericSQL::getObjectRefNameIndex(const QString &ref_name)
{
	int idx = -1;
	vector<ObjectRefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.end();

	if(ref_name.isEmpty())
		return -1;

	while(itr != itr_end)
	{
		if((*itr).ref_name == ref_name)
		{
			idx = itr - objects_refs.begin();
			break;
		}

		itr++;
	}

	return idx;
}

bool GenericSQL::isObjectReferenced(BaseObject *object)
{
	bool found = false;
	BaseObject *ref_obj = nullptr;
	vector<ObjectRefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.end();

	if(!object)
		return false;

	while(itr != itr_end && !found)
	{
		ref_obj = (*itr).object;
		found = (ref_obj == object);

		if(!found && TableObject::isTableObject(ref_obj->getObjectType()))
			found = (dynamic_cast<TableObject *>(ref_obj)->getParentTable() == object);

		itr++;
	}

	return found;
}

bool GenericSQL::isReferRelationshipAddedObject()
{
	bool found = false;
	vector<ObjectRefConfig>::iterator itr = objects_refs.begin(),
			itr_end = objects_refs.end();
	TableObject *tab_obj = nullptr;

	while(itr != itr_end && !found)
	{
		tab_obj = dynamic_cast<TableObject *>(itr->object);
		found = (tab_obj && tab_obj->isAddedByRelationship());
		itr++;
	}

	return found;
}

vector<BaseObject *> GenericSQL::getReferencedObjects()
{
	vector<BaseObject *> ref_objs;

	for(auto &ref : objects_refs)
		ref_objs.push_back(ref.object);

	return ref_objs;
}

void GenericSQL::validateObjectReference(ObjectRefConfig ref, bool ignore_duplic)
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
		ObjectRefConfig ref = ObjectRefConfig(ref_name, object, use_signature, format_name);
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
		ObjectRefConfig ref = ObjectRefConfig(new_ref_name, object, use_signature, format_name);
		vector<ObjectRefConfig>::iterator itr = objects_refs.begin() + idx;
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

void GenericSQL::removeObjectReferences()
{
	objects_refs.clear();
	setCodeInvalidated(true);
}

QString GenericSQL::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	QString fmt_definition = definition,
			name_attr = QString("%1%2%3").arg(SchemaParser::CharIniAttribute)
																	 .arg(Attributes::Name)
																	 .arg(SchemaParser::CharEndAttribute);

	if(!objects_refs.empty())
	{
		QString ref_name, ref_value;
		attribs_map obj_attrs;

		for(auto &ref : objects_refs)
		{
			if(def_type == SchemaParser::XmlDefinition)
			{
				obj_attrs[Attributes::Name] = ref.object->getSignature();
				obj_attrs[Attributes::Type] = ref.object->getSchemaName();
				obj_attrs[Attributes::RefName] = ref.ref_name;
				obj_attrs[Attributes::FormatName] = ref.format_name ? Attributes::True : QString();
				obj_attrs[Attributes::UseSignature] = ref.use_signature ? Attributes::True : QString();

				schparser.ignoreUnkownAttributes(true);
				attributes[Attributes::Objects] += schparser.getCodeDefinition(Attributes::Object, obj_attrs, SchemaParser::XmlDefinition);
			}
			else
			{
				/* In order to use a reference name in the object's SQL code, the reference should be writter in the for
				 * {ref_name} so it can be replaced by the corresponding value in the SQL code */
				ref_name = QString("%1%2%3").arg(SchemaParser::CharIniAttribute)
									 .arg(ref.ref_name)
									 .arg(SchemaParser::CharEndAttribute);

				// Configuring the value of the reference
				ref_value = ref.use_signature ?
										ref.object->getSignature(ref.format_name) :
										ref.object->getName(ref.format_name);

				fmt_definition = fmt_definition.replace(ref_name, ref_value);
			}
		}
	}

	// Special case for the {name} attribute which is created automatically when there's no one defined by the user
	if(def_type == SchemaParser::SqlDefinition &&
		 fmt_definition.contains(name_attr) && getObjectRefNameIndex(Attributes::Name) < 0)
		fmt_definition = fmt_definition.replace(name_attr, this->getName(true));

	attributes[Attributes::Definition] = fmt_definition;

	return this->BaseObject::__getCodeDefinition(def_type);
}
