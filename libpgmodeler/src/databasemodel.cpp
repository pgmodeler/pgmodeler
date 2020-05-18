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

#include "databasemodel.h"
#include "pgmodelerns.h"
#include "defaultlanguages.h"
#include <QtDebug>

unsigned DatabaseModel::dbmodel_id=2000;

DatabaseModel::DatabaseModel()
{
	this->model_wgt=nullptr;
	object_id=DatabaseModel::dbmodel_id++;
	obj_type=ObjectType::Database;

	is_template = false;
	allow_conns = true;

	encoding=BaseType::Null;
	BaseObject::setName(QObject::tr("new_database"));

	default_objs[ObjectType::Schema]=nullptr;
	default_objs[ObjectType::Role]=nullptr;
	default_objs[ObjectType::Tablespace]=nullptr;
	default_objs[ObjectType::Collation]=nullptr;

	conn_limit=-1;
	last_zoom=1;
	loading_model=invalidated=append_at_eod=prepend_at_bod=false;
	attributes[Attributes::Encoding]=QString();
	attributes[Attributes::TemplateDb]=QString();
	attributes[Attributes::ConnLimit]=QString();
	attributes[Attributes::LcCollate]=QString();
	attributes[Attributes::LcCtype]=QString();
	attributes[Attributes::AppendAtEod]=QString();
	attributes[Attributes::PrependAtBod]=QString();
	attributes[Attributes::AllowConns]=QString();
	attributes[Attributes::IsTemplate]=QString();

	obj_lists = {
		{ ObjectType::Textbox, &textboxes },
		{ ObjectType::Table, &tables },
		{ ObjectType::Function, &functions },
		{ ObjectType::Aggregate, &aggregates },
		{ ObjectType::Schema, &schemas },
		{ ObjectType::View, &views },
		{ ObjectType::Type, &types },
		{ ObjectType::Role, &roles },
		{ ObjectType::Tablespace, &tablespaces },
		{ ObjectType::Language, &languages },
		{ ObjectType::Cast, &casts },
		{ ObjectType::Conversion, &conversions },
		{ ObjectType::Operator, &operators },
		{ ObjectType::OpClass, &op_classes },
		{ ObjectType::OpFamily, &op_families },
		{ ObjectType::Domain, &domains },
		{ ObjectType::Sequence, &sequences },
		{ ObjectType::BaseRelationship, &base_relationships },
		{ ObjectType::Relationship, &relationships },
		{ ObjectType::Permission, &permissions },
		{ ObjectType::Collation, &collations },
		{ ObjectType::Extension, &extensions },
		{ ObjectType::Tag, &tags },
		{ ObjectType::EventTrigger, &eventtriggers },
		{ ObjectType::GenericSql, &genericsqls },
		{ ObjectType::ForeignDataWrapper, &fdata_wrappers },
		{ ObjectType::ForeignServer, &foreign_servers },
		{ ObjectType::UserMapping, &usermappings },
		{ ObjectType::ForeignTable, &foreign_tables }
	};
}

DatabaseModel::DatabaseModel(ModelWidget *model_wgt):DatabaseModel()
{
	this->model_wgt=model_wgt;
}

DatabaseModel::~DatabaseModel()
{
	this->blockSignals(true);
	destroyObjects();
}

ModelWidget *DatabaseModel::getModelWidget()
{
	return model_wgt;
}

void DatabaseModel::setEncoding(EncodingType encod)
{
	encoding=encod;
}

void DatabaseModel::setLocalization(unsigned localiz_id, const QString &value)
{
	if(localiz_id > Collation::LcCollate)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	localizations[localiz_id]=value;
}

void DatabaseModel::setConnectionLimit(int conn_lim)
{
	if(conn_lim < -1) conn_lim=-1;
	this->conn_limit=conn_lim;
}

void DatabaseModel::setTemplateDB(const QString &temp_db)
{
	if(!temp_db.isEmpty() && !BaseObject::isValidName(temp_db))
		throw Exception(ErrorCode::AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->template_db=temp_db;
}

void DatabaseModel::setAuthor(const QString &author)
{
	this->author=author;
}

vector<BaseObject *> *DatabaseModel::getObjectList(ObjectType obj_type)
{
	if(obj_lists.count(obj_type) == 0)
		return nullptr;

	return obj_lists[obj_type];
}

void DatabaseModel::addObject(BaseObject *object, int obj_idx)
{
	ObjectType obj_type;

	if(!object)
		return;

	try
	{
		obj_type=object->getObjectType();

		if(obj_type==ObjectType::Relationship || obj_type==ObjectType::BaseRelationship)
			addRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
		else if(obj_type==ObjectType::Textbox)
			addTextbox(dynamic_cast<Textbox *>(object), obj_idx);
		else if(obj_type==ObjectType::Table)
			addTable(dynamic_cast<Table *>(object), obj_idx);
		else if(obj_type==ObjectType::Function)
			addFunction(dynamic_cast<Function *>(object), obj_idx);
		else if(obj_type==ObjectType::Aggregate)
			addAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
		else if(obj_type==ObjectType::Schema)
			addSchema(dynamic_cast<Schema *>(object), obj_idx);
		else if(obj_type==ObjectType::View)
			addView(dynamic_cast<View *>(object), obj_idx);
		else if(obj_type==ObjectType::Type)
			addType(dynamic_cast<Type *>(object), obj_idx);
		else if(obj_type==ObjectType::Role)
			addRole(dynamic_cast<Role *>(object), obj_idx);
		else if(obj_type==ObjectType::Tablespace)
			addTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
		else if(obj_type==ObjectType::Language)
			addLanguage(dynamic_cast<Language *>(object), obj_idx);
		else if(obj_type==ObjectType::Cast)
			addCast(dynamic_cast<Cast *>(object), obj_idx);
		else if(obj_type==ObjectType::Conversion)
			addConversion(dynamic_cast<Conversion *>(object), obj_idx);
		else if(obj_type==ObjectType::Operator)
			addOperator(dynamic_cast<Operator *>(object), obj_idx);
		else if(obj_type==ObjectType::OpClass)
			addOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
		else if(obj_type==ObjectType::OpFamily)
			addOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
		else if(obj_type==ObjectType::Domain)
			addDomain(dynamic_cast<Domain *>(object), obj_idx);
		else if(obj_type==ObjectType::Sequence)
			addSequence(dynamic_cast<Sequence *>(object), obj_idx);
		else if(obj_type==ObjectType::Collation)
			addCollation(dynamic_cast<Collation *>(object), obj_idx);
		else if(obj_type==ObjectType::Extension)
			addExtension(dynamic_cast<Extension *>(object), obj_idx);
		else if(obj_type==ObjectType::Tag)
			addTag(dynamic_cast<Tag *>(object), obj_idx);
		else if(obj_type==ObjectType::Permission)
			addPermission(dynamic_cast<Permission *>(object));
		else if(obj_type==ObjectType::EventTrigger)
			addEventTrigger(dynamic_cast<EventTrigger *>(object));
		else if(obj_type==ObjectType::GenericSql)
			addGenericSQL(dynamic_cast<GenericSQL *>(object));
		else if(obj_type==ObjectType::ForeignDataWrapper)
			addForeignDataWrapper(dynamic_cast<ForeignDataWrapper *>(object));
		else if(obj_type==ObjectType::ForeignServer)
			addForeignServer(dynamic_cast<ForeignServer *>(object));
		else if(obj_type==ObjectType::UserMapping)
			addUserMapping(dynamic_cast<UserMapping *>(object));
		else if(obj_type==ObjectType::ForeignTable)
			addForeignTable(dynamic_cast<ForeignTable *>(object));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeObject(BaseObject *object, int obj_idx)
{
	ObjectType obj_type;

	if(!object)
		return;

	try
	{
		obj_type=object->getObjectType();

		if(obj_type==ObjectType::Relationship || obj_type==ObjectType::BaseRelationship)
			removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
		else if(obj_type==ObjectType::Textbox)
			removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
		else if(obj_type==ObjectType::Table)
			removeTable(dynamic_cast<Table *>(object), obj_idx);
		else if(obj_type==ObjectType::Function)
			removeFunction(dynamic_cast<Function *>(object), obj_idx);
		else if(obj_type==ObjectType::Aggregate)
			removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
		else if(obj_type==ObjectType::Schema)
			removeSchema(dynamic_cast<Schema *>(object), obj_idx);
		else if(obj_type==ObjectType::View)
			removeView(dynamic_cast<View *>(object), obj_idx);
		else if(obj_type==ObjectType::Type)
			removeType(dynamic_cast<Type *>(object), obj_idx);
		else if(obj_type==ObjectType::Role)
			removeRole(dynamic_cast<Role *>(object), obj_idx);
		else if(obj_type==ObjectType::Tablespace)
			removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
		else if(obj_type==ObjectType::Language)
			removeLanguage(dynamic_cast<Language *>(object), obj_idx);
		else if(obj_type==ObjectType::Cast)
			removeCast(dynamic_cast<Cast *>(object), obj_idx);
		else if(obj_type==ObjectType::Conversion)
			removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
		else if(obj_type==ObjectType::Operator)
			removeOperator(dynamic_cast<Operator *>(object), obj_idx);
		else if(obj_type==ObjectType::OpClass)
			removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
		else if(obj_type==ObjectType::OpFamily)
			removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
		else if(obj_type==ObjectType::Domain)
			removeDomain(dynamic_cast<Domain *>(object), obj_idx);
		else if(obj_type==ObjectType::Sequence)
			removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
		else if(obj_type==ObjectType::Collation)
			removeCollation(dynamic_cast<Collation *>(object), obj_idx);
		else if(obj_type==ObjectType::Extension)
			removeExtension(dynamic_cast<Extension *>(object), obj_idx);
		else if(obj_type==ObjectType::Tag)
			removeTag(dynamic_cast<Tag *>(object), obj_idx);
		else if(obj_type==ObjectType::Permission)
			removePermission(dynamic_cast<Permission *>(object));
		else if(obj_type==ObjectType::EventTrigger)
			removeEventTrigger(dynamic_cast<EventTrigger *>(object));
		else if(obj_type==ObjectType::GenericSql)
			removeGenericSQL(dynamic_cast<GenericSQL *>(object));
		else if(obj_type==ObjectType::ForeignDataWrapper)
			removeForeignDataWrapper(dynamic_cast<ForeignDataWrapper *>(object));
		else if(obj_type==ObjectType::ForeignServer)
			removeForeignServer(dynamic_cast<ForeignServer *>(object));
		else if(obj_type==ObjectType::UserMapping)
			removeUserMapping(dynamic_cast<UserMapping *>(object));
		else if(obj_type==ObjectType::ForeignTable)
			removeForeignTable(dynamic_cast<ForeignTable *>(object));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

}

void DatabaseModel::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	if(TableObject::isTableObject(obj_type) ||
			obj_type==ObjectType::BaseObject || obj_type==ObjectType::BaseRelationship ||
			obj_type==ObjectType::Database)
		throw Exception(ErrorCode::RemObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<BaseObject *> *obj_list=nullptr;
	BaseObject *object=nullptr;

	obj_list=getObjectList(obj_type);
	if(obj_idx >= obj_list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	object=(*obj_list)[obj_idx];
	if(obj_type==ObjectType::Textbox)
		removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
	else if(obj_type==ObjectType::Table)
		removeTable(dynamic_cast<Table *>(object), obj_idx);
	else if(obj_type==ObjectType::Function)
		removeFunction(dynamic_cast<Function *>(object), obj_idx);
	else if(obj_type==ObjectType::Aggregate)
		removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
	else if(obj_type==ObjectType::Schema)
		removeSchema(dynamic_cast<Schema *>(object), obj_idx);
	else if(obj_type==ObjectType::View)
		removeView(dynamic_cast<View *>(object), obj_idx);
	else if(obj_type==ObjectType::Type)
		removeType(dynamic_cast<Type *>(object), obj_idx);
	else if(obj_type==ObjectType::Role)
		removeRole(dynamic_cast<Role *>(object), obj_idx);
	else if(obj_type==ObjectType::Tablespace)
		removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
	else if(obj_type==ObjectType::Language)
		removeLanguage(dynamic_cast<Language *>(object), obj_idx);
	else if(obj_type==ObjectType::Cast)
		removeCast(dynamic_cast<Cast *>(object), obj_idx);
	else if(obj_type==ObjectType::Conversion)
		removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
	else if(obj_type==ObjectType::Operator)
		removeOperator(dynamic_cast<Operator *>(object), obj_idx);
	else if(obj_type==ObjectType::OpClass)
		removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
	else if(obj_type==ObjectType::OpFamily)
		removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
	else if(obj_type==ObjectType::Domain)
		removeDomain(dynamic_cast<Domain *>(object), obj_idx);
	else if(obj_type==ObjectType::Sequence)
		removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
	else if(obj_type==ObjectType::Collation)
		removeCollation(dynamic_cast<Collation *>(object), obj_idx);
	else if(obj_type==ObjectType::Relationship || obj_type==ObjectType::BaseRelationship)
		removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
	else if(obj_type==ObjectType::Permission)
		removePermission(dynamic_cast<Permission *>(object));
	else if(obj_type==ObjectType::EventTrigger)
		removeEventTrigger(dynamic_cast<EventTrigger *>(object), obj_idx);
	else if(obj_type==ObjectType::GenericSql)
		removeGenericSQL(dynamic_cast<GenericSQL *>(object), obj_idx);
	else if(obj_type==ObjectType::ForeignDataWrapper)
		removeForeignDataWrapper(dynamic_cast<ForeignDataWrapper *>(object), obj_idx);
	else if(obj_type==ObjectType::UserMapping)
		removeUserMapping(dynamic_cast<UserMapping *>(object), obj_idx);
	else if(obj_type==ObjectType::ForeignTable)
		removeForeignTable(dynamic_cast<ForeignTable *>(object), obj_idx);
}

void DatabaseModel::__addObject(BaseObject *object, int obj_idx)
{
	int idx;
	ObjectType obj_type;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!object)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	obj_type=object->getObjectType();

#ifdef DEMO_VERSION
#warning "DEMO VERSION: database model object creation limit."
	obj_list=getObjectList(obj_type);
	if(obj_list && obj_list->size() >= GlobalAttributes::MaxObjectCount)
		throw Exception(tr("The demonstration version can create only `%1' instances of each object type! You've reach this limit for the type: `%2'")
						.arg(GlobalAttributes::MaxObjectCount)
						.arg(BaseObject::getTypeName(obj_type)),
						ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

#endif

	if(obj_type==ObjectType::Tablespace)
	{
		Tablespace *tabspc=nullptr, *aux_tabspc=nullptr;

		obj_list=getObjectList(obj_type);
		itr=obj_list->begin();
		itr_end=obj_list->end();
		tabspc=dynamic_cast<Tablespace *>(object);

		while(itr!=itr_end)
		{
			aux_tabspc=dynamic_cast<Tablespace *>(*itr);

			/* Raises an error if the object to be added is a tablespace and
			there is some other tablespace pointing to the same directory */
			if(tabspc->getDirectory()==aux_tabspc->getDirectory())
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgTablespaceDuplicatedDirectory)
								.arg(tabspc->getName())
								.arg(aux_tabspc->getName()),
								ErrorCode::AsgTablespaceDuplicatedDirectory,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			itr++;
		}
	}

	/* Raises an error if there is an object with the same name.
		 Special cases are for: functions/operator that are search by signature and views
		 that are search on tables and views list */
	if(((obj_type==ObjectType::View ||
			 obj_type==ObjectType::Table ||
			 obj_type==ObjectType::ForeignTable) &&
			(getObject(object->getName(true), ObjectType::View, idx) ||
			 getObject(object->getName(true), ObjectType::Table, idx) ||
			 getObject(object->getName(true), ObjectType::ForeignTable, idx))) ||
			(obj_type==ObjectType::Extension &&	(getObject(object->getName(false), obj_type, idx))) ||
			(getObject(object->getSignature(), obj_type, idx)))
	{
		QString str_aux;

		str_aux=Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
				.arg(object->getName(obj_type != ObjectType::Extension))
				.arg(object->getTypeName())
				.arg(this->getName(true))
				.arg(this->getTypeName());

		throw Exception(str_aux,ErrorCode::AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	try
	{
		if(obj_type==ObjectType::Textbox || obj_type==ObjectType::BaseRelationship)
			object->getCodeDefinition(SchemaParser::XmlDefinition);
		else
			object->getCodeDefinition(SchemaParser::SqlDefinition);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

	obj_list=getObjectList(object->getObjectType());

	if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
		obj_list->push_back(object);
	else
	{
		if(obj_idx >=0 && idx < 0)
			idx=obj_idx;
		else if(obj_idx < 0 && idx < 0)
			idx=0;

		if(obj_list->size() > 0)
			obj_list->insert((obj_list->begin() + idx), object);
		else
			obj_list->push_back(object);
	}

	object->setDatabase(this);
	emit s_objectAdded(object);
	this->setInvalidated(true);
}

void DatabaseModel::__removeObject(BaseObject *object, int obj_idx, bool check_refs)
{
	if(!object)
		throw Exception(ErrorCode::RemNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		vector<BaseObject *> *obj_list=nullptr;
		ObjectType obj_type;

		obj_type=object->getObjectType();
		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			vector<BaseObject *> refs;

			//Get the table references
			if(check_refs)
				getObjectReferences(object, refs, true, true);

			//If there are objects referencing the table
			if(!refs.empty())
			{
				ErrorCode err_code;

				//Raises an error indicating the object that is referencing the table
				if(!dynamic_cast<TableObject *>(refs[0]))
				{
					err_code=ErrorCode::RemDirectReference;
					throw Exception(Exception::getErrorMessage(err_code)
									.arg(object->getName(true))
									.arg(object->getTypeName())
									.arg(refs[0]->getName(true))
							.arg(refs[0]->getTypeName()),
							err_code,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
				else
				{
					BaseObject *ref_obj_parent=dynamic_cast<TableObject *>(refs[0])->getParentTable();

					err_code=ErrorCode::RemInderectReference;
					throw Exception(Exception::getErrorMessage(err_code)
									.arg(object->getName(true))
									.arg(object->getTypeName())
									.arg(refs[0]->getName(true))
							.arg(refs[0]->getTypeName())
							.arg(ref_obj_parent->getName(true))
							.arg(ref_obj_parent->getTypeName()),
							err_code,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
				getObject(object->getSignature(), obj_type, obj_idx);

			if(obj_idx >= 0)
			{
				if(Permission::acceptsPermission(obj_type))
					removePermissions(object);

				obj_list->erase(obj_list->begin() + obj_idx);
			}
		}

		object->setDatabase(nullptr);
		emit s_objectRemoved(object);
	}
}

vector<BaseObject *> DatabaseModel::getObjects(ObjectType obj_type, BaseObject *schema)
{
	vector<BaseObject *> *obj_list=nullptr, sel_list;
	vector<BaseObject *>::iterator itr, itr_end;
	BaseRelationship *rel=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=obj_list->begin();
	itr_end=obj_list->end();

	while(itr!=itr_end)
	{
		rel=dynamic_cast<BaseRelationship *>(*itr);

		if((!rel && (*itr)->getSchema()==schema) ||
				(rel && (rel->getTable(BaseRelationship::SrcTable)->getSchema()==schema ||
						 rel->getTable(BaseRelationship::DstTable)->getSchema()==schema)))
			sel_list.push_back(*itr);

		itr++;
	}

	return sel_list;
}

vector<BaseObject *> DatabaseModel::getObjects(BaseObject *schema)
{
	vector<BaseObject *> *obj_list=nullptr, sel_list;
	vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::Schema);

	for(auto &type : types)
	{
		obj_list = getObjectList(type);

		for(auto &obj : *obj_list)
		{
			if(obj->getSchema() == schema)
				sel_list.push_back(obj);
		}
	}

	return sel_list;
}

BaseObject *DatabaseModel::getObject(const QString &name, ObjectType obj_type, int &obj_idx)
{
	BaseObject *object=nullptr;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	bool found=false;
	QString aux_name1;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		QString signature;

		itr=obj_list->begin();
		itr_end=obj_list->end();
		obj_idx=-1;
		aux_name1=QString(name).remove('"');

		while(itr!=itr_end && !found)
		{
			signature=(*itr)->getSignature().remove("\"");
			found=(signature==aux_name1);
			if(!found) itr++;
		}

		if(found)
		{
			object=(*itr);
			obj_idx=(itr - obj_list->begin());
		}
		else obj_idx=-1;
	}

	return object;
}

BaseObject *DatabaseModel::getObject(const QString &name, const vector<ObjectType> &types)
{
	BaseObject *object = nullptr;

	for(auto &type : types)
	{
		object = getObject(name, type);
		if(object) break;
	}

	return object;
}

BaseObject *DatabaseModel::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<BaseObject *> *obj_list=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(obj_idx >= obj_list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return obj_list->at(obj_idx);
}

unsigned DatabaseModel::getObjectCount(ObjectType obj_type)
{
	vector<BaseObject *> *obj_list=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return obj_list->size();
}

unsigned DatabaseModel::getMaxObjectCount()
{
	vector<ObjectType> types = getObjectTypes(false, {ObjectType::Database});
  unsigned count = 0, max = 0;

  for(auto &type : types)
  {
	 count = getObjectList(type)->size();
	 if(count > max) max = count;
  }

	return max;
}

unsigned DatabaseModel::getObjectCount()
{
	vector<ObjectType> types= getObjectTypes(false, {ObjectType::Database});
	unsigned count=0;

	for(auto &type : types)
		count+=getObjectList(type)->size();

	return count;
}

QString DatabaseModel::getLocalization(unsigned localiz_id)
{
	if(localiz_id > Collation::LcCollate)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return localizations[localiz_id];
}

int DatabaseModel::getConnectionLimit()
{
	return conn_limit;
}

QString DatabaseModel::getTemplateDB()
{
	return template_db;
}

EncodingType DatabaseModel::getEncoding()
{
	return encoding;
}

BaseObject *DatabaseModel::getDefaultObject(ObjectType obj_type)
{
	if(default_objs.count(obj_type)==0)
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	return default_objs[obj_type];
}

QString DatabaseModel::getAuthor()
{
	return author;
}

void DatabaseModel::setProtected(bool value)
{
	for(auto &itr : obj_lists)
	{
		for(auto &object :  *itr.second)
		{
			object->setProtected(value);
		}
	}

	BaseObject::setProtected(value);
}

void DatabaseModel::destroyObjects()
{
	ObjectType graph_types[]={ ObjectType::Schema, ObjectType::BaseRelationship, ObjectType::Relationship,
														 ObjectType::Table, ObjectType::ForeignTable, ObjectType::View };
	BaseObject *object=nullptr;
	map<unsigned, BaseObject *> objects;
	map<unsigned, BaseObject *>::reverse_iterator ritr, ritr_end;
	vector<ObjectType> rem_obj_types;
	BaseGraphicObject *graph_obj = nullptr;

	//Blocking signals of all graphical objects to avoid uneeded updates in the destruction
	this->blockSignals(true);

	for(unsigned i=0; i < 5; i++)
	{
		for(auto &object : *this->getObjectList(graph_types[i]))
		{
			graph_obj = dynamic_cast<BaseGraphicObject *>(object);

			if(graph_obj)
				dynamic_cast<BaseGraphicObject *>(object)->blockSignals(true);
		}
	}

	try
	{
		//Removing the special objects first
		storeSpecialObjectsXML();
		disconnectRelationships();
	}
	catch(Exception &e)
	{
		/* DEBUG: An exception at this point shouldn't never occur but if
		 * it is raised, we spit out the error to the stdout in order to try to
		 * find out the problem! */
		qDebug() << "** FAIL TO DESTROY ALL OBJECTS **" << endl;
		qDebug() << e.getExceptionsText().toStdString().c_str() << endl;
	}

	objects = getCreationOrder(SchemaParser::XmlDefinition, true);
	ritr = objects.rbegin();
	ritr_end = objects.rend();

	while(ritr != ritr_end)
	{
		object = ritr->second;
		ritr++;

		// We ignore the database itself, permission objects (destroyed separetely) and table children objects
		if(object->getObjectType() == ObjectType::Database ||
			 object->getObjectType() == ObjectType::Permission ||
			 TableObject::isTableObject(object->getObjectType()))
			continue;

		// Register the type of the object being removed so the respective list can be cleaned in the end
		rem_obj_types.push_back(object->getObjectType());

		/* If the object is graphical destroy using the __removeObject in order
		emit the signal to object scene to remove the graphical representation
		of the to-be-destroyed object */
		if(dynamic_cast<BaseGraphicObject *>(object))
		{
			__removeObject(object,-1,false);

			if(object->getObjectType()==ObjectType::Relationship)
				dynamic_cast<Relationship *>(object)->destroyObjects();
		}

		delete object;
	}

	PgSqlType::removeUserTypes(this);

	for(auto &perm : 	permissions)
		delete perm;

	permissions.clear();

	for(auto &inv_obj : invalid_special_objs)
		delete inv_obj;

	invalid_special_objs.clear();

	//Cleaning out the list of removed objects to avoid segfaults while calling this method again
	if(!rem_obj_types.empty())
	{
		std::sort(rem_obj_types.begin(), rem_obj_types.end());
		vector<ObjectType>::iterator end = std::unique(rem_obj_types.begin(), rem_obj_types.end());
		rem_obj_types.erase(end, rem_obj_types.end());

		for(auto type : rem_obj_types)
			getObjectList(type)->clear();
	}
}

void DatabaseModel::addTable(Table *table, int obj_idx)
{
	try
	{
		__addObject(table, obj_idx);

		PgSqlType::addUserType(table->getName(true), table, this, UserTypeConfig::TableType);

		updateTableFKRelationships(table);

		dynamic_cast<Schema *>(table->getSchema())->setModified(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Table *DatabaseModel::getTable(unsigned obj_idx)
{
	return dynamic_cast<Table *>(getObject(obj_idx, ObjectType::Table));
}

Table *DatabaseModel::getTable(const QString &name)
{
	return dynamic_cast<Table *>(getObject(name, ObjectType::Table));
}

void DatabaseModel::removeTable(Table *table, int obj_idx)
{
	try
	{
		__removeObject(table, obj_idx);
		PgSqlType::removeUserType(table->getName(true), table);
		updateTableFKRelationships(table);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addSequence(Sequence *sequence, int obj_idx)
{
	try
	{
		__addObject(sequence, obj_idx);
		PgSqlType::addUserType(sequence->getName(true), sequence, this, UserTypeConfig::SequenceType);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Sequence *DatabaseModel::getSequence(unsigned obj_idx)
{
	return dynamic_cast<Sequence *>(getObject(obj_idx, ObjectType::Sequence));
}

Sequence *DatabaseModel::getSequence(const QString &name)
{
	return dynamic_cast<Sequence *>(getObject(name, ObjectType::Sequence));
}

void DatabaseModel::removeSequence(Sequence *sequence, int obj_idx)
{
	try
	{
		removeUserType(sequence, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addCollation(Collation *collation, int obj_idx)
{
	try
	{
		__addObject(collation, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Collation *DatabaseModel::getCollation(unsigned obj_idx)
{
	return dynamic_cast<Collation *>(getObject(obj_idx, ObjectType::Collation));
}

Collation *DatabaseModel::getCollation(const QString &name)
{
	return dynamic_cast<Collation *>(getObject(name, ObjectType::Collation));
}

void DatabaseModel::removeCollation(Collation *collation, int obj_idx)
{
	try
	{
		__removeObject(collation, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::addExtension(Extension *extension, int obj_idx)
{
	try
	{
		__addObject(extension, obj_idx);

		if(extension->handlesType())
			PgSqlType::addUserType(extension->getName(true), extension, this, UserTypeConfig::ExtensionType);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Extension *DatabaseModel::getExtension(unsigned obj_idx)
{
	return dynamic_cast<Extension *>(getObject(obj_idx, ObjectType::Collation));
}

Extension *DatabaseModel::getExtension(const QString &name)
{
	return dynamic_cast<Extension *>(getObject(name, ObjectType::Extension));
}

void DatabaseModel::addTag(Tag *tag, int obj_idx)
{
	try
	{
		__addObject(tag, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeTag(Tag *tag, int obj_idx)
{
	try
	{
		__removeObject(tag, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Tag *DatabaseModel::getTag(unsigned obj_idx)
{
	return dynamic_cast<Tag *>(getObject(obj_idx, ObjectType::Tag));
}

Tag *DatabaseModel::getTag(const QString &name)
{
	return dynamic_cast<Tag *>(getObject(name, ObjectType::Tag));
}

void DatabaseModel::addEventTrigger(EventTrigger *evnttrig, int obj_idx)
{
	try
	{
		__addObject(evnttrig, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeEventTrigger(EventTrigger *evnttrig, int obj_idx)
{
	try
	{
		__removeObject(evnttrig, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

EventTrigger *DatabaseModel::getEventTrigger(unsigned obj_idx)
{
	return dynamic_cast<EventTrigger *>(getObject(obj_idx, ObjectType::EventTrigger));
}

EventTrigger *DatabaseModel::getEventTrigger(const QString &name)
{
	return dynamic_cast<EventTrigger *>(getObject(name, ObjectType::EventTrigger));
}

void DatabaseModel::addGenericSQL(GenericSQL *genericsql, int obj_idx)
{
	try
	{
		__addObject(genericsql, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeGenericSQL(GenericSQL *genericsql, int obj_idx)
{
	try
	{
		__removeObject(genericsql, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

GenericSQL *DatabaseModel::getGenericSQL(unsigned obj_idx)
{
	return dynamic_cast<GenericSQL *>(getObject(obj_idx, ObjectType::GenericSql));
}

GenericSQL *DatabaseModel::getGenericSQL(const QString &name)
{
	return dynamic_cast<GenericSQL *>(getObject(name, ObjectType::GenericSql));
}

void DatabaseModel::addForeignDataWrapper(ForeignDataWrapper *fdata_wrapper, int obj_idx)
{
	try
	{
		__addObject(fdata_wrapper, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeForeignDataWrapper(ForeignDataWrapper *fdata_wrapper, int obj_idx)
{
	try
	{
		__removeObject(fdata_wrapper, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

ForeignDataWrapper *DatabaseModel::getForeignDataWrapper(unsigned obj_idx)
{
	return dynamic_cast<ForeignDataWrapper *>(getObject(obj_idx, ObjectType::ForeignDataWrapper));
}

ForeignDataWrapper *DatabaseModel::getForeignDataWrapper(const QString &name)
{
	return dynamic_cast<ForeignDataWrapper *>(getObject(name, ObjectType::ForeignDataWrapper));
}

void DatabaseModel::addForeignServer(ForeignServer *server, int obj_idx)
{
	try
	{
		__addObject(server, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeForeignServer(ForeignServer *server, int obj_idx)
{
	try
	{
		__removeObject(server, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

ForeignServer *DatabaseModel::getForeignServer(unsigned obj_idx)
{
	return dynamic_cast<ForeignServer *>(getObject(obj_idx, ObjectType::ForeignServer));
}

ForeignServer *DatabaseModel::getForeignServer(const QString &name)
{
	return dynamic_cast<ForeignServer *>(getObject(name, ObjectType::ForeignServer));
}

void DatabaseModel::addUserMapping(UserMapping *usrmap, int obj_idx)
{
	try
	{
		__addObject(usrmap, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeUserMapping(UserMapping *usrmap, int obj_idx)
{
	try
	{
		__removeObject(usrmap, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

UserMapping *DatabaseModel::getUserMapping(unsigned obj_idx)
{
	return dynamic_cast<UserMapping *>(getObject(obj_idx, ObjectType::UserMapping));
}

UserMapping *DatabaseModel::getUserMapping(const QString &name)
{
	return dynamic_cast<UserMapping *>(getObject(name, ObjectType::UserMapping));
}

void DatabaseModel::addForeignTable(ForeignTable *table, int obj_idx)
{
	try
	{
		__addObject(table, obj_idx);

		PgSqlType::addUserType(table->getName(true), table, this, UserTypeConfig::ForeignTableType);
		dynamic_cast<Schema *>(table->getSchema())->setModified(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

ForeignTable *DatabaseModel::getForeignTable(unsigned obj_idx)
{
	return dynamic_cast<ForeignTable *>(getObject(obj_idx, ObjectType::ForeignTable));
}

ForeignTable *DatabaseModel::getForeignTable(const QString &name)
{
	return dynamic_cast<ForeignTable *>(getObject(name, ObjectType::ForeignTable));
}

void DatabaseModel::removeForeignTable(ForeignTable *table, int obj_idx)
{
	try
	{
		__removeObject(table, obj_idx);
		PgSqlType::removeUserType(table->getName(true), table);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removeExtension(Extension *extension, int obj_idx)
{
	try
	{
		if(extension->handlesType())
			removeUserType(extension, obj_idx);
		else
			__removeObject(extension, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::addView(View *view, int obj_idx)
{
	try
	{
		__addObject(view, obj_idx);
		PgSqlType::addUserType(view->getName(true), view, this, UserTypeConfig::ViewType);

		updateViewRelationships(view);
		dynamic_cast<Schema *>(view->getSchema())->setModified(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

View *DatabaseModel::getView(unsigned obj_idx)
{
	return dynamic_cast<View *>(getObject(obj_idx, ObjectType::View));
}

View *DatabaseModel::getView(const QString &name)
{
	return dynamic_cast<View *>(getObject(name, ObjectType::View));
}

void DatabaseModel::removeView(View *view, int obj_idx)
{
	try
	{
		//The relationships that links tables to the view must be removed before erase the view itself
		updateViewRelationships(view, true);

		__removeObject(view, obj_idx);
		PgSqlType::removeUserType(view->getName(true), view);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::updateTableFKRelationships(Table *table)
{
	if(!table)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(table->getDatabase()==this)
	{
		Table *ref_tab=nullptr;
		BaseRelationship *rel=nullptr;
		unsigned idx;
		vector<Constraint *> fks;
		vector<BaseObject *>::iterator itr1, itr1_end;

		table->getForeignKeys(fks);

		/* First remove the invalid relationships (the foreign key that generates the
			relationship no longer exists) */
		itr1=base_relationships.begin();
		itr1_end=base_relationships.end();

		idx=0;
		while(itr1!=itr1_end)
		{
			rel=dynamic_cast<BaseRelationship *>(*itr1);

			if(rel->getRelationshipType()==BaseRelationship::RelationshipFk &&
					(rel->getTable(BaseRelationship::SrcTable)==table ||
					 rel->getTable(BaseRelationship::DstTable)==table))
			{
				Constraint *fk = rel->getReferenceForeignKey();

				if(rel->getTable(BaseRelationship::SrcTable)==table)
					ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DstTable));
				else
					ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::SrcTable));

				/* Removes the relationship if the following cases happen:
				 * 1) The foreign key references a table different from ref_tab, which means, the user
				 *		have changed the fk manually by setting a new referenced table but the relationship tied to the fk
				 *		does not reflect the new reference.
				 *
				 * 2) The fk references the correct table but the source table does not own the fk anymore, which means,
				 *		the fk as removed manually by the user. */
				if((table->getObjectIndex(fk) >= 0 && fk->getReferencedTable() != ref_tab) ||
					 (table->getObjectIndex(fk) < 0 && fk->getReferencedTable() == ref_tab))
				{
					removeRelationship(rel);
					itr1=base_relationships.begin() + idx;
					itr1_end=base_relationships.end();
				}
				else
				{
					rel->setModified(!loading_model);
					itr1++; idx++;
				}
			}
			else
			{
				itr1++; idx++;
			}
		}

		//Creating the relationships from the foreign keys
		for(auto &fk : fks)
		{
			ref_tab=dynamic_cast<Table *>(fk->getReferencedTable());

			//Only creates the relationship if does'nt exist one between the tables
			rel=getRelationship(table, ref_tab, fk);

			if(!rel && ref_tab->getDatabase()==this)
			{
				bool ref_mandatory = false;

				for(auto &col : fk->getColumns(Constraint::SourceCols))
				{
					if(col->isNotNull())
					{
						ref_mandatory = true;
						break;
					}
				}

				rel = new BaseRelationship(BaseRelationship::RelationshipFk, table, ref_tab, false, ref_mandatory);
				rel->setReferenceForeignKey(fk);
				rel->setCustomColor(Qt::transparent);

				/* Workaround: In some cases the combination of the two tablenames can generate a duplicated relationship
					 name so it`s necessary to check if a relationship with the same name already exists. If exists changes
					 the name of the new one */
				if(getObjectIndex(rel->getName(), ObjectType::BaseRelationship) >= 0)
					rel->setName(PgModelerNs::generateUniqueName(rel, base_relationships));

				addRelationship(rel);
			}
		}
	}
}

void DatabaseModel::updateTablesFKRelationships()
{
	vector<BaseObject *>::iterator itr=tables.begin();

	while(itr!=tables.end())
	{
		updateTableFKRelationships(dynamic_cast<Table *>(*itr));
		itr++;
	}
}

void DatabaseModel::updateViewRelationships(View *view, bool force_rel_removal)
{
	PhysicalTable *table=nullptr;
	BaseRelationship *rel=nullptr;
	Reference ref;
	unsigned i, ref_count, idx;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<PhysicalTable *> tables;

	if(!view)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(getObjectIndex(view) < 0 || force_rel_removal)
	{
		//Remove all the relationship related to the view when this latter no longer exists
		itr=base_relationships.begin();
		itr_end=base_relationships.end();

		idx=0;
		while(itr!=itr_end)
		{
			rel=dynamic_cast<BaseRelationship *>(*itr);

			if(rel->getTable(BaseRelationship::SrcTable)==view ||
					rel->getTable(BaseRelationship::DstTable)==view)
			{
				removeRelationship(rel);
				itr=base_relationships.begin() + idx;
				itr_end=base_relationships.end();
			}
			else
			{
				itr++; idx++;
			}
		}
	}
	else
	{
		/* Remove the relationships between tables and the view
		 when this latter doesn't reference the first */
		itr=base_relationships.begin();
		itr_end=base_relationships.end();

		idx=0;
		while(itr!=itr_end)
		{
			rel=dynamic_cast<BaseRelationship *>(*itr);

			if(rel->getTable(BaseRelationship::SrcTable)==view ||
					rel->getTable(BaseRelationship::DstTable)==view)
			{
				if(rel->getTable(BaseRelationship::SrcTable)->getObjectType()==ObjectType::Table)
					table=dynamic_cast<PhysicalTable *>(rel->getTable(BaseRelationship::SrcTable));
				else
					table=dynamic_cast<PhysicalTable *>(rel->getTable(BaseRelationship::DstTable));

				if(!view->isReferencingTable(table))
				{
					removeRelationship(rel);
					itr=base_relationships.begin() + idx;
					itr_end=base_relationships.end();
				}
				else
				{
					itr++; idx++;
				}
			}
			else
			{
				itr++; idx++;
			}
		}

		/* Creates the relationships from the view references
		 * First we try to create relationship from referecences in SELECT portion of view's definition */
		ref_count=view->getReferenceCount(Reference::SqlReferSelect);
		for(i=0; i < ref_count; i++)
		{
			table = view->getReference(i, Reference::SqlReferSelect).getTable();
			if(table) tables.push_back(table);
		}

		/* If the view does have tables referenced from SELECT portion we check if
		 * the table was constructed from a single reference (Reference::SqlViewDefinition). In
		 * that case we use the list of referenced tables configured in that view reference object */
		if(tables.empty() && view->getReferenceCount(Reference::SqlViewDefinition) > 0)
			tables = view->getReference(0, Reference::SqlViewDefinition).getReferencedTables();

		// Effectively creating the relationships
		for(auto &tab : tables)
		{
			rel = getRelationship(view, tab);

			if(!rel)
			{
				rel=new BaseRelationship(BaseRelationship::RelationshipDep, view, tab, false, false);
				rel->setName(PgModelerNs::generateUniqueName(rel, base_relationships));
				addRelationship(rel);
			}
		}
	}
}

void DatabaseModel::disconnectRelationships()
{
	try
	{
		BaseRelationship *base_rel=nullptr;
		Relationship *rel=nullptr;
		vector<BaseObject *>::reverse_iterator ritr_rel, ritr_rel_end;

		//The relationships must be disconnected from the last to the first
		ritr_rel=relationships.rbegin();
		ritr_rel_end=relationships.rend();

		while(ritr_rel!=ritr_rel_end)
		{
			base_rel=dynamic_cast<BaseRelationship *>(*ritr_rel);
			ritr_rel++;
			base_rel->blockSignals(loading_model);

			if(base_rel->getObjectType()==ObjectType::Relationship)
			{
				rel=dynamic_cast<Relationship *>(base_rel);
				rel->disconnectRelationship();
			}
			else
				base_rel->disconnectRelationship();

			base_rel->blockSignals(false);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::validateRelationships()
{
	vector<BaseObject *>::iterator itr, itr_end, itr_ant;
	Relationship *rel=nullptr;
	BaseRelationship *base_rel=nullptr;
	vector<BaseObject *> vet_rel, vet_rel_inv, rels, fail_rels;
	bool found_inval_rel, valid_fail_rels=false;
	vector<Exception> errors;
	map<unsigned, QString>::iterator itr1, itr1_end;
	map<unsigned, Exception> error_map;
	map<unsigned, Exception>::iterator itr2, itr2_end;
	map<BaseObject *, unsigned> conn_tries;
	unsigned idx, rels_gen_pk=0;
	vector<Schema *> schemas;
	BaseTable *tab1=nullptr, *tab2=nullptr;

	itr=relationships.begin();
	itr_end=relationships.end();

	/* Calculates the quantity of referece tables which has primary keys added by relationship.
	This type of primary key may cause unexpected relationship invalidation during the validation process
	because all relationship are disconnected and when reconnecting them the primary key sometimes is not yet
	created causing other relationships to be broken. This counter is used to try revalidate any relationship
	that emits ERR_LINK_TABLES_NO_PK exception during its connection */
	while(itr!=itr_end)
	{
		rel=dynamic_cast<Relationship *>(*itr);
		itr++;

		if(rel &&
				(rel->getRelationshipType()==Relationship::Relationship11 ||
				 rel->getRelationshipType()==Relationship::Relationship1n) &&
				rel->getReferenceTable()->getPrimaryKey() &&
				rel->getReferenceTable()->getPrimaryKey()->isAddedByRelationship())
			rels_gen_pk++;
	}

	itr=relationships.begin();
	itr_end=relationships.end();

	do
	{
		found_inval_rel=false;

		while(itr!=itr_end)
		{
			base_rel=dynamic_cast<BaseRelationship *>(*itr);
			itr++;

			//Validates only table-table relationships
			if(base_rel->getObjectType()==ObjectType::Relationship)
			{
				//Makes a cast to the correct object class
				rel=dynamic_cast<Relationship *>(base_rel);

				//If the relationships is invalid
				if(rel->isInvalidated())
				{
					//Inserts it to the invalid relationship vector
					vet_rel_inv.push_back(base_rel);

					//Marks the flag indicating the at least one relationship was found invalidated
					found_inval_rel=true;
				}
				else
					//Otherwise inserts the relationship on the valid relationships
					vet_rel.push_back(base_rel);
			}
		}

		//If there is some invalidated relationship or special objects to be recreated
		if(found_inval_rel || !xml_special_objs.empty())
		{
			//Stores the special objects definition if there is some invalidated relationships
			if(!loading_model && xml_special_objs.empty())
				storeSpecialObjectsXML();

			if(found_inval_rel)
			{
				//Disconnects all the relationship
				disconnectRelationships();

				/* Merges the two lists (valid and invalid relationships),
					 taking care to insert the invalid ones at the end of the list */
				rels=vet_rel;
				rels.insert(rels.end(), vet_rel_inv.begin(), vet_rel_inv.end());
				vet_rel.clear();
				vet_rel_inv.clear();

				//Walking through the created list connecting the relationships
				itr=rels.begin();
				itr_end=rels.end();
				idx=0;

				while(itr!=itr_end)
				{
					rel=dynamic_cast<Relationship *>(*itr);

					//Stores the current iterator in a auxiliary one to remove from list in case of error
					itr_ant=itr;
					itr++;

					try
					{
						//Try to connect the relationship
						rel->connectRelationship();

						//Storing the schemas on a auxiliary vector to update them later
						tab1=rel->getTable(BaseRelationship::SrcTable);
						tab2=rel->getTable(BaseRelationship::DstTable);
						schemas.push_back(dynamic_cast<Schema *>(tab1->getSchema()));

						if(tab2 != tab1)
							schemas.push_back(dynamic_cast<Schema *>(tab2->getSchema()));

						idx++;

						/* Removes the relationship from the current position and inserts it
							 into the next position after the next relationship to try the reconnection */
						rels.erase(itr_ant);
						idx=0;

						/* If the list was emptied and there is relationship that fails to validate,
							 the method will try to validate them one last time */
						if(rels.size()==0 && !fail_rels.empty() && !valid_fail_rels)
						{
							rels.insert(rels.end(), fail_rels.begin(), fail_rels.end());
							//Check this flag indicates that the fail_rels list must be copied only one time
							valid_fail_rels=true;
						}

						itr=rels.begin();
						itr_end=rels.end();
					}
					/* Case some error is raised during the connection the relationship is
						 permanently invalidated and need to be removed from the model */
					catch(Exception &e)
					{
						/* If the relationship connection failed after 'rels_gen_pk' times at the
						different errors or exists on the fail_rels vector (already tried to be validated)
						it will be deleted from model */
						if((e.getErrorCode() != ErrorCode::InvLinkTablesNoPrimaryKey && conn_tries[rel] > rels_gen_pk) ||
								(std::find(fail_rels.begin(), fail_rels.end(), rel)!=fail_rels.end()))
						{
							//Removes the relationship
							__removeObject(rel);

							//Removes the iterator that stores the relationship from the list
							rels.erase(itr_ant);

							//Stores the error raised in a list
							errors.push_back(e);
						}
						/* If the relationship connection fails with the ERR_LINK_TABLES_NO_PK error and
								the connection tries exceed the size of the relationship the relationship is isolated
								on a "failed to validate" list. This list will be appended to the main rel list when
								there is only one relationship to be validated */
						else if(e.getErrorCode()==ErrorCode::InvLinkTablesNoPrimaryKey &&
								(conn_tries[rel] > rels.size() ||
								 rel->getRelationshipType()==BaseRelationship::RelationshipNn))
						{
							fail_rels.push_back(rel);
							rels.erase(itr_ant);
							conn_tries[rel]=0;

							/* If the list was emptied and there is relationship that fails to validate,
								 the method will try to validate them one last time */
							if(rels.size()==0 && !valid_fail_rels)
							{
								rels.insert(rels.end(), fail_rels.begin(), fail_rels.end());
								valid_fail_rels=true;
							}
						}
						else
						{
							//Increments the connection tries
							conn_tries[rel]++;

							/* Removes the relationship from the current position and inserts it
								 into the next position after the next relationship to try the reconnection */
							rels.erase(itr_ant);

							//If the next index doesn't extrapolates the list size insert it on the next position
							if(idx+1 < rels.size())
								rels.insert(rels.begin() + idx + 1,rel);
							else
								rels.push_back(rel);
						}

						/* Points the searching to the iterator immediately after the removed iterator
						evicting to walk on the list from the first item */
						itr_end=rels.end();
						itr=rels.begin() + idx;
					}
				}

				itr=rels.begin();
			}

			//Recreating the special objects
			itr1=xml_special_objs.begin();
			itr1_end=xml_special_objs.end();

			//The special objects are created only when the model is not being loaded
			if(!loading_model && itr1!=itr1_end)
			{
				do
				{
					try
					{
						//Try to create the special object
						createSpecialObject(itr1->second, itr1->first);

						/* If the special object is successfully created, remove the errors
							 related to a previous attempt to create it */
						if(error_map.count(itr1->first))
							error_map.erase(error_map.find(itr1->first));

						//Removes the definition of the special object when it is created successfully
						xml_special_objs.erase(itr1);

						//Restart the special object creation
						itr1=xml_special_objs.begin();
						itr1_end=xml_special_objs.end();
					}
					catch(Exception &e)
					{
						//If some error related to the special object is raised, stores it for latter creation attempts
						error_map[itr1->first]=e;
						itr1++; idx++;
					}
				}
				while(itr1!=itr1_end);
			}
		}
	}
	//The validation continues until there is some invalid relationship
	while(found_inval_rel);

	if(!loading_model && !schemas.empty())
	{
		std::sort(schemas.begin(), schemas.end());
		vector<Schema *>::iterator end = std::unique(schemas.begin(), schemas.end());
		schemas.erase(end, schemas.end());

		//Updates the schemas to ajdust its sizes due to the tables resizings
		for(auto &sch : schemas)
			sch->setModified(true);
	}

	//Stores the errors related to creation of special objects on the general error vector
	itr2=error_map.begin();
	itr2_end=error_map.end();
	while(itr2!=itr2_end)
	{
		errors.push_back(itr2->second);
		itr2++;
	}

	//If errors were caught on the above executions they will be redirected to the user
	if(!errors.empty())
	{
		if(!loading_model)
			xml_special_objs.clear();

		/* Revalidates the fk relationships at this points because some fks must be removed due
		 to special object invalidation */
		itr=base_relationships.begin();
		itr_end=base_relationships.end();

		while(itr!=itr_end)
		{
			base_rel=dynamic_cast<BaseRelationship *>(*itr);

			if(base_rel->getRelationshipType()==BaseRelationship::RelationshipFk)
				this->updateTableFKRelationships(dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SrcTable)));

			itr++;
		}

		//Set all the model objects as modified to force the redraw of the entire model
		this->setObjectsModified();

		//Redirects all the errors captured on the revalidation
		throw Exception(ErrorCode::RemInvalidatedObjects,__PRETTY_FUNCTION__,__FILE__,__LINE__,errors);
	}

	if(!loading_model)
	{
		for(auto &tab : tables)
			dynamic_cast<PhysicalTable *>(tab)->restoreRelObjectsIndexes();

		for(auto &tab : foreign_tables)
			dynamic_cast<PhysicalTable *>(tab)->restoreRelObjectsIndexes();

		xml_special_objs.clear();
	}
}

void DatabaseModel::checkRelationshipRedundancy(Relationship *rel)
{
	try
	{
		unsigned rel_type;

		//Raises an error if the user try to check the redundancy starting from a unnallocated relationship
		if(!rel)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		rel_type=rel->getRelationshipType();

		/* Only identifier relationships or relationship that has identifier
		 attributes (primary keys) are checked */
		if((!rel->isSelfRelationship() &&
				(rel->isIdentifier() ||
				 rel->hasIndentifierAttribute())) ||
				(rel_type==Relationship::RelationshipGen ||
				 rel_type==Relationship::RelationshipDep ||
				 rel_type==Relationship::RelationshipPart))
		{
			BaseTable *ref_table=nullptr, *src_table=nullptr;
			PhysicalTable *recv_table=nullptr;
			Relationship *rel_aux=nullptr;
			BaseRelationship *base_rel=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			bool found_cycle=false;
			unsigned aux_rel_type;
			QString str_aux, msg;

			//Gets the tables from the relationship
			recv_table=rel->getReceiverTable();
			ref_table=rel->getReferenceTable();

			itr=relationships.begin();
			itr_end=relationships.end();

			/* Based on the obtained tables, scans the list of relationships in search of the cycle.
			A cycle is detected when a reference table from a relationship is the receiver table of
			the relationship used in the validation. */
			while(itr!=itr_end && !found_cycle)
			{
				base_rel=dynamic_cast<BaseRelationship *>(*itr);
				itr++;

				if(base_rel->getObjectType()==ObjectType::Relationship)
				{
					rel_aux=dynamic_cast<Relationship *>(base_rel);
					aux_rel_type=rel_aux->getRelationshipType();
					src_table=rel_aux->getReferenceTable();

					/* Case the reference table is equal to the receiver table of the relationship used
				as the start of validation and the current relationship type is the same as the
				latter relationship, this can indicate a principle of closing cycle, in this way
				the validation will proceed with the receiver table from the current relationship
				until que receiver table is the reference table of the initial relationship */
					if(recv_table==src_table &&  aux_rel_type==rel_type &&
							((!rel_aux->isSelfRelationship() &&
							  (rel_aux->isIdentifier() ||
							   rel_aux->hasIndentifierAttribute())) ||
							 (aux_rel_type==Relationship::RelationshipGen ||
								aux_rel_type==Relationship::RelationshipDep ||
								aux_rel_type==Relationship::RelationshipPart)))

					{
						//The receiver table will be the receiver from the current relationship
						recv_table=rel_aux->getReceiverTable();

						//Stores the relationship name to raise an error in case of closing cycle
						str_aux+=rel_aux->getName() + QString(", ");

						//Checking the closing cycle
						found_cycle=(recv_table==ref_table);

						//Restart the validation
						itr=relationships.begin();
					}
				}
			}

			//Raises an error indicating the relationship names that close the cycle
			if(found_cycle)
			{
				str_aux+=rel->getName();
				msg=Exception::getErrorMessage(ErrorCode::InsRelationshipRedundancy)
					.arg(rel->getName())
					.arg(str_aux);
				throw Exception(msg,ErrorCode::InsRelationshipRedundancy,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::storeSpecialObjectsXML()
{
	unsigned count=0, i=0, type_id=0;
	vector<BaseObject *>::iterator itr, itr_end;
	Sequence *sequence=nullptr;
	Permission *permission=nullptr;
	PhysicalTable *table=nullptr;
	TableObject *tab_obj=nullptr;
	Constraint *constr=nullptr;
	Index *index=nullptr;
	Trigger *trigger=nullptr;
	View *view=nullptr;
	BaseRelationship *rel=nullptr;
	GenericSQL *generic_sql=nullptr;
	Reference ref;
	ObjectType tab_obj_type[3]={ ObjectType::Constraint, ObjectType::Trigger, ObjectType::Index };
	bool found=false;
	vector<BaseObject *> objects, rem_objects, upd_tables_rels, aux_tables;

	try
	{
		aux_tables = tables;
		aux_tables.insert(aux_tables.end(), foreign_tables.begin(), foreign_tables.end());
		itr=aux_tables.begin();
		itr_end=aux_tables.end();

		/* Check on tables if there is some constraint/index/trigger that is referencing
		 some column added by relationship */
		while(itr!=itr_end)
		{
			table=dynamic_cast<PhysicalTable *>(*itr);
			itr++;

			for(type_id=0; type_id < 3; type_id++)
			{
				// Ignoring index type if we are dealing with foreign table
				if(table->getObjectType() == ObjectType::ForeignTable &&
					 tab_obj_type[type_id] == ObjectType::Index)
					continue;

				//Gets the table object count for the curret object type
				count=table->getObjectCount(tab_obj_type[type_id]);

				for(i=0; i < count; i++)
				{
					tab_obj=dynamic_cast<TableObject *>(table->getObject(i, tab_obj_type[type_id]));
					found=false;

					if(tab_obj_type[type_id]==ObjectType::Constraint)
					{
						constr=dynamic_cast<Constraint *>(tab_obj);

						/* A constraint is considered special in this case when it is referencing
							 relationship added column and the constraint itself was not added by
							 relationship (created manually by the user) */
						found=(!constr->isAddedByRelationship() &&
									 constr->isReferRelationshipAddedColumn() &&
									 constr->getConstraintType()!=ConstraintType::PrimaryKey);

						//When found some special object, stores is xml definition
						if(found)
							xml_special_objs[constr->getObjectId()]=constr->getCodeDefinition(SchemaParser::XmlDefinition, true);
					}
					else if(tab_obj_type[type_id]==ObjectType::Trigger)
					{
						trigger=dynamic_cast<Trigger *>(tab_obj);
						found=trigger->isReferRelationshipAddedColumn();

						if(found)
							xml_special_objs[trigger->getObjectId()]=trigger->getCodeDefinition(SchemaParser::XmlDefinition);
					}
					else
					{
						index=dynamic_cast<Index *>(tab_obj);
						found=index->isReferRelationshipAddedColumn();

						if(found)
							xml_special_objs[index->getObjectId()]=index->getCodeDefinition(SchemaParser::XmlDefinition);
					}

					if(found)
					{
						constr = dynamic_cast<Constraint *>(tab_obj);

						//When found the special object must be removed from the parent table
						table->removeObject(tab_obj->getName(), tab_obj->getObjectType());

						//We need to store the table which fk was referencing relationship added columns in order to update the fk relationships of that table
						if(constr && constr->getConstraintType() == ConstraintType::ForeignKey)
							upd_tables_rels.push_back(table);

						//Removes the permission from the table object
						removePermissions(tab_obj);

						i--; count--;
					}
				}
			}
		}

		//Updating fk rels of tables which had fks referencing relationship added columns
		if(!upd_tables_rels.empty())
		{
			vector<BaseObject *>::iterator end;

			if(upd_tables_rels.size() > 1)
			{
				std::sort(upd_tables_rels.begin(), upd_tables_rels.end());
				end = std::unique(upd_tables_rels.begin(), upd_tables_rels.end());
				upd_tables_rels.erase(end, upd_tables_rels.end());
			}

			for(auto &tab : upd_tables_rels)
				updateTableFKRelationships(dynamic_cast<Table *>(tab));
		}

		//Making a copy of the sequences list to avoid iterator invalidation when removing an object
		rem_objects.assign(sequences.begin(), sequences.end());
		itr=rem_objects.begin();
		itr_end=rem_objects.end();

		while(itr!=itr_end)
		{
			sequence=dynamic_cast<Sequence *>(*itr);
			itr++;

			if(sequence->isReferRelationshipAddedColumn())
			{
				xml_special_objs[sequence->getObjectId()]=sequence->getCodeDefinition(SchemaParser::XmlDefinition);
				removeSequence(sequence);
				invalid_special_objs.push_back(sequence);
				//delete sequence;
			}
		}

		//Making a copy of the view list to avoid iterator invalidation when removing an object
		rem_objects.assign(views.begin(), views.end());
		itr=rem_objects.begin();
		itr_end=rem_objects.end();

		while(itr!=itr_end)
		{
			view=dynamic_cast<View *>(*itr);
			itr++;

			if(view->isReferRelationshipAddedColumn())
			{
				xml_special_objs[view->getObjectId()]=view->getCodeDefinition(SchemaParser::XmlDefinition);

				/* Relationships linking the view and the referenced tables are considered as
			 special objects in this case only to be recreated more easely latter */

				count=view->getReferenceCount(Reference::SqlReferSelect);

				for(i=0; i < count; i++)
				{
					ref=view->getReference(i, Reference::SqlReferSelect);
					table=ref.getTable();

					if(table)
					{
						//Get the relationship between the view and the referenced table
						rel=getRelationship(view, table);

						if(rel)
						{
							xml_special_objs[rel->getObjectId()]=rel->getCodeDefinition(SchemaParser::XmlDefinition);
							removeRelationship(rel);
							invalid_special_objs.push_back(rel);
							//delete rel;
						}
					}
				}

				/* Removing child objects from view and including them in the list of objects to be recreated,
		   this will avoid errors when removing the view from model */
				objects=view->getObjects();
				for(auto &obj : objects)
				{
					xml_special_objs[obj->getObjectId()]=obj->getCodeDefinition(SchemaParser::XmlDefinition);
					view->removeObject(obj);
					invalid_special_objs.push_back(obj);
					//delete obj;
				}

				removeView(view);
				invalid_special_objs.push_back(view);
				//delete view;
			}
		}

		//Making a copy of the permissions list to avoid iterator invalidation when removing an object
		rem_objects.assign(permissions.begin(), permissions.end());
		itr=rem_objects.begin();
		itr_end=rem_objects.end();

		while(itr!=itr_end)
		{
			permission=dynamic_cast<Permission *>(*itr);
			tab_obj=dynamic_cast<TableObject *>(permission->getObject());
			itr++;

			if(tab_obj)
			{
				xml_special_objs[permission->getObjectId()]=permission->getCodeDefinition(SchemaParser::XmlDefinition);
				removePermission(permission);
				invalid_special_objs.push_back(permission);
				//delete permission;
			}
		}

		//Making a copy of the generic SQL objects list to avoid iterator invalidation when removing an object
		rem_objects.assign(genericsqls.begin(), genericsqls.end());
		itr=rem_objects.begin();
		itr_end=rem_objects.end();

		while(itr!=itr_end)
		{
			generic_sql = dynamic_cast<GenericSQL *>(*itr);
			itr++;

			if(generic_sql->isReferRelationshipAddedObject())
			{
				xml_special_objs[generic_sql->getObjectId()] = generic_sql->getCodeDefinition(SchemaParser::XmlDefinition);
				removeGenericSQL(generic_sql);
				invalid_special_objs.push_back(generic_sql);
				//delete generic_sql;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::createSpecialObject(const QString &xml_def, unsigned obj_id)
{
	ObjectType obj_type;
	BaseObject *object=nullptr;

	try
	{
		//Restart the XML parser to read the passed xml buffer
		xmlparser.restartParser();
		xmlparser.loadXMLBuffer(xml_def);

		//Identifies the object type through the start element on xml buffer
		obj_type=BaseObject::getObjectType(xmlparser.getElementName());

		if(obj_type==ObjectType::Sequence)
			object=createSequence(true);
		else
			object=createObject(obj_type);

		if(obj_type==ObjectType::Sequence)
			addSequence(dynamic_cast<Sequence *>(object));
		else if(obj_type==ObjectType::View)
			addView(dynamic_cast<View *>(object));
		else if(obj_type==ObjectType::Permission)
			addPermission(createPermission());

		/* When the special object is recreated it receive a new id but to maintain
		 the correct creation order, the object has its id restored with the passed
		 id (obj_id) if it is specified */
		if(object && obj_id!=0)
			object->object_id=obj_id;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, xml_def);
	}
}

void DatabaseModel::addRelationship(BaseRelationship *rel, int obj_idx)
{
	try
	{
		BaseTable *tab1=nullptr, *tab2=nullptr;
		QString msg;

		if(rel)
		{
			tab1=rel->getTable(BaseRelationship::SrcTable);
			tab2=rel->getTable(BaseRelationship::DstTable);

			//Raises an error if already exists an relationship between the tables
			if(rel->getRelationshipType() != Relationship::Relationship1n &&
				 rel->getRelationshipType() != Relationship::RelationshipNn &&
				 rel->getRelationshipType() != Relationship::RelationshipFk &&
				 getRelationship(tab1,tab2))
			{
				msg=Exception::getErrorMessage(ErrorCode::InsDuplicatedRelationship)
					.arg(tab1->getName(true))
					.arg(tab1->getTypeName())
					.arg(tab2->getName(true))
					.arg(tab2->getTypeName());
				throw Exception(msg,ErrorCode::InsDuplicatedRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		//Before add the relationship, checks if a redundancy can occur case the relationship is added
		if(rel->getObjectType()==ObjectType::Relationship)
			checkRelationshipRedundancy(dynamic_cast<Relationship *>(rel));

		rel->blockSignals(loading_model);
		__addObject(rel, obj_idx);

		if(rel->getObjectType()==ObjectType::Relationship)
		{
			dynamic_cast<Relationship *>(rel)->connectRelationship();
			validateRelationships();
		}
		else
			rel->connectRelationship();

		rel->blockSignals(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeRelationship(BaseRelationship *rel, int obj_idx)
{
	try
	{
		if(getObjectIndex(rel) >= 0)
		{
			PhysicalTable *recv_tab=nullptr;

			if(rel->getObjectType()==ObjectType::Relationship)
			{
				/* If the relationship is not a many-to-many we store the receiver table in order to
				 * update the fk relationships (if there are any) */
				if(rel->getRelationshipType()!=Relationship::RelationshipNn)
					recv_tab=dynamic_cast<Relationship *>(rel)->getReceiverTable();

				storeSpecialObjectsXML();
				disconnectRelationships();
			}
			else if(rel->getObjectType()==ObjectType::BaseRelationship)
			{
				rel->blockSignals(loading_model);
				rel->disconnectRelationship();
				rel->blockSignals(false);
			}

			__removeObject(rel, obj_idx);

			if(rel->getObjectType()==ObjectType::Relationship)
				validateRelationships();

			//Updating the fk relationships for the receiver table after removing the old relationship
			if(recv_tab && recv_tab->getObjectType() == ObjectType::Table)
				updateTableFKRelationships(dynamic_cast<Table *>(recv_tab));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

BaseRelationship *DatabaseModel::getRelationship(unsigned obj_idx, ObjectType rel_type)
{
	//Raises an error if the object type used to get a relationship is invalid
	if(rel_type!=ObjectType::Relationship && rel_type!=ObjectType::BaseRelationship)
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return dynamic_cast<BaseRelationship *>(getObject(obj_idx, rel_type));
}

BaseRelationship *DatabaseModel::getRelationship(const QString &name)
{
	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(getObject(name, ObjectType::BaseRelationship));

	if(!rel)
		rel=dynamic_cast<BaseRelationship *>(getObject(name, ObjectType::Relationship));

	return rel;
}

BaseRelationship *DatabaseModel::getRelationship(BaseTable *src_tab, BaseTable *dst_tab, Constraint *ref_fk)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> rel_list;
	BaseRelationship *rel=nullptr;
	bool found=false, search_uniq_tab=false;
	BaseTable *tab1=nullptr, *tab2=nullptr;

	if(src_tab)
	{
		if(!dst_tab)
		{
			dst_tab=src_tab;
			search_uniq_tab=true;
		}

		if(ref_fk || src_tab->getObjectType()==ObjectType::View || dst_tab->getObjectType()==ObjectType::View)
		{
			itr=base_relationships.begin();
			itr_end=base_relationships.end();
		}
		else
		{
			rel_list.assign(base_relationships.begin(), base_relationships.end());
			rel_list.insert(rel_list.end(), relationships.begin(), relationships.end());
			itr=rel_list.begin();
			itr_end=rel_list.end();
		}

		while(itr!=itr_end && !found)
		{
			rel=dynamic_cast<BaseRelationship *>(*itr);
			tab1=rel->getTable(BaseRelationship::SrcTable);
			tab2=rel->getTable(BaseRelationship::DstTable);

			found=((!ref_fk || (ref_fk && rel->getReferenceForeignKey() == ref_fk)) &&
						 ((tab1==src_tab && tab2==dst_tab) ||
							(tab2==src_tab && tab1==dst_tab) ||
							(search_uniq_tab && (tab1==src_tab || tab2==src_tab))));

			if(!found)
			{ rel=nullptr; itr++; }
		}
	}

	return rel;
}

vector<BaseRelationship *> DatabaseModel::getRelationships(BaseTable *tab)
{
	vector<BaseRelationship *> aux_rels;
	vector<BaseObject *> rels;
	BaseRelationship *base_rel=nullptr;

	rels=base_relationships;
	rels.insert(rels.end(), relationships.begin(), relationships.end());

	for(auto &obj : rels)
	{
		base_rel=dynamic_cast<BaseRelationship *>(obj);

		if(base_rel->getTable(BaseRelationship::SrcTable)==tab ||
			 base_rel->getTable(BaseRelationship::DstTable)==tab)
			aux_rels.push_back(base_rel);
	}

	return aux_rels;
}

void DatabaseModel::addTextbox(Textbox *txtbox, int obj_idx)
{
	try
	{
		__addObject(txtbox, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeTextbox(Textbox *txtbox, int obj_idx)
{
	try
	{
		__removeObject(txtbox, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Textbox *DatabaseModel::getTextbox(unsigned obj_idx)
{
	return dynamic_cast<Textbox *>(getObject(obj_idx, ObjectType::Textbox));
}

Textbox *DatabaseModel::getTextbox(const QString &name)
{
	return dynamic_cast<Textbox *>(getObject(name, ObjectType::Textbox));
}

void DatabaseModel::addSchema(Schema *schema, int obj_idx)
{
	try
	{
		__addObject(schema, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Schema *DatabaseModel::getSchema(unsigned obj_idx)
{
	return dynamic_cast<Schema *>(getObject(obj_idx, ObjectType::Schema));
}

Schema *DatabaseModel::getSchema(const QString &name)
{
	return dynamic_cast<Schema *>(getObject(name, ObjectType::Schema));
}

void DatabaseModel::removeSchema(Schema *schema, int obj_idx)
{
	try
	{
		__removeObject(schema, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addRole(Role *role, int obj_idx)
{
	try
	{
		__addObject(role, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Role *DatabaseModel::getRole(unsigned obj_idx)
{
	return dynamic_cast<Role *>(getObject(obj_idx, ObjectType::Role));
}

Role *DatabaseModel::getRole(const QString &name)
{
	return dynamic_cast<Role *>(getObject(name, ObjectType::Role));
}

void DatabaseModel::removeRole(Role *role, int obj_idx)
{
	try
	{
		__removeObject(role, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addTablespace(Tablespace *tabspc, int obj_idx)
{
	try
	{
		__addObject(tabspc, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Tablespace *DatabaseModel::getTablespace(unsigned obj_idx)
{
	return dynamic_cast<Tablespace *>(getObject(obj_idx, ObjectType::Tablespace));
}

Tablespace *DatabaseModel::getTablespace(const QString &name)
{
	return dynamic_cast<Tablespace *>(getObject(name, ObjectType::Tablespace));
}

void DatabaseModel::removeTablespace(Tablespace *tabspc, int obj_idx)
{
	try
	{
		__removeObject(tabspc, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addCast(Cast *cast, int obj_idx)
{
	try
	{
		__addObject(cast, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeCast(Cast *cast, int obj_idx)
{
	try
	{
		__removeObject(cast, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Cast *DatabaseModel::getCast(unsigned obj_idx)
{
	return dynamic_cast<Cast *>(getObject(obj_idx, ObjectType::Cast));
}

Cast *DatabaseModel::getCast(const QString &name)
{
	return dynamic_cast<Cast *>(getObject(name, ObjectType::Cast));
}

void DatabaseModel::addConversion(Conversion *conv, int obj_idx)
{
	try
	{
		__addObject(conv, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeConversion(Conversion *conv, int obj_idx)
{
	try
	{
		__removeObject(conv, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Conversion *DatabaseModel::getConversion(unsigned obj_idx)
{
	return dynamic_cast<Conversion *>(getObject(obj_idx, ObjectType::Conversion));
}

Conversion *DatabaseModel::getConversion(const QString &name)
{
	return dynamic_cast<Conversion *>(getObject(name, ObjectType::Conversion));
}

void DatabaseModel::addLanguage(Language *lang, int obj_idx)
{
	try
	{
		__addObject(lang, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Language *DatabaseModel::getLanguage(unsigned obj_idx)
{
	return dynamic_cast<Language *>(getObject(obj_idx, ObjectType::Language));
}

Language *DatabaseModel::getLanguage(const QString &name)
{
	return dynamic_cast<Language *>(getObject(name, ObjectType::Language));
}

void DatabaseModel::removeLanguage(Language *lang, int obj_idx)
{
	try
	{
		__removeObject(lang, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addFunction(Function *func, int obj_idx)
{
	try
	{
		__addObject(func, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Function *DatabaseModel::getFunction(unsigned obj_idx)
{
	return dynamic_cast<Function *>(getObject(obj_idx, ObjectType::Function));
}

Function *DatabaseModel::getFunction(const QString &signature)
{
	return dynamic_cast<Function *>(getObject(signature, ObjectType::Function));
}

void DatabaseModel::removeFunction(Function *func, int obj_idx)
{
	try
	{
		__removeObject(func, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addAggregate(Aggregate *aggreg, int obj_idx)
{
	try
	{
		__addObject(aggreg, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Aggregate *DatabaseModel::getAggregate(unsigned obj_idx)
{
	return dynamic_cast<Aggregate *>(getObject(obj_idx, ObjectType::Aggregate));
}

Aggregate *DatabaseModel::getAggregate(const QString &name)
{
	return dynamic_cast<Aggregate *>(getObject(name, ObjectType::Aggregate));
}

void DatabaseModel::removeAggregate(Aggregate *aggreg, int obj_idx)
{
	try
	{
		__removeObject(aggreg, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addDomain(Domain *domain, int obj_idx)
{
	if(domain)
	{
		vector<BaseObject *>::iterator itr, itr_end;
		bool found=false;
		QString str_aux;

		/* Before insert the domain checks if there is some user defined type
		 with the same name as the domain. */
		itr=types.begin();
		itr_end=types.end();
		while(itr!=itr_end && !found)
		{
			found=((*itr)->getName(true)==domain->getName(true));
			itr++;
		}

		//Raises an error if found a type with the same name as the domain
		if(found)
		{
			str_aux=Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
					.arg(domain->getName(true))
					.arg(domain->getTypeName())
					.arg(this->getName(true))
					.arg(this->getTypeName());
			throw Exception(str_aux, ErrorCode::AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		try
		{
			__addObject(domain, obj_idx);

			//When added to the model the domain is inserted on the pgsql base type list to be used as a column type
			PgSqlType::addUserType(domain->getName(true), domain, this, UserTypeConfig::DomainType);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}
}

void DatabaseModel::removeDomain(Domain *domain, int obj_idx)
{
	try
	{
		removeUserType(domain, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Domain *DatabaseModel::getDomain(unsigned obj_idx)
{
	return dynamic_cast<Domain *>(getObject(obj_idx, ObjectType::Domain));
}

Domain *DatabaseModel::getDomain(const QString &name)
{
	return dynamic_cast<Domain *>(getObject(name, ObjectType::Domain));
}

void DatabaseModel::addOperatorFamily(OperatorFamily *op_family, int obj_idx)
{
	try
	{
		__addObject(op_family, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

OperatorFamily *DatabaseModel::getOperatorFamily(unsigned obj_idx)
{
	return dynamic_cast<OperatorFamily *>(getObject(obj_idx, ObjectType::OpFamily));
}

OperatorFamily *DatabaseModel::getOperatorFamily(const QString &name)
{
	return dynamic_cast<OperatorFamily *>(getObject(name, ObjectType::OpFamily));
}

void DatabaseModel::removeOperatorFamily(OperatorFamily *op_family, int obj_idx)
{
	try
	{
		__removeObject(op_family, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addOperatorClass(OperatorClass *op_class, int obj_idx)
{
	try
	{
		__addObject(op_class, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeOperatorClass(OperatorClass *op_class, int obj_idx)
{
	try
	{
		__removeObject(op_class, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

OperatorClass *DatabaseModel::getOperatorClass(unsigned obj_idx)
{
	return dynamic_cast<OperatorClass *>(getObject(obj_idx, ObjectType::OpClass));
}

OperatorClass *DatabaseModel::getOperatorClass(const QString &name)
{
	return dynamic_cast<OperatorClass *>(getObject(name, ObjectType::OpClass));
}

void DatabaseModel::addOperator(Operator *oper, int obj_idx)
{
	try
	{
		__addObject(oper, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeOperator(Operator *oper, int obj_idx)
{
	try
	{
		__removeObject(oper, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Operator *DatabaseModel::getOperator(unsigned obj_idx)
{
	return dynamic_cast<Operator *>(getObject(obj_idx, ObjectType::Operator));
}

Operator *DatabaseModel::getOperator(const QString &signature)
{
	return dynamic_cast<Operator *>(getObject(signature, ObjectType::Operator));
}

void DatabaseModel::addType(Type *type, int obj_idx)
{
	if(type)
	{
		vector<BaseObject *>::iterator itr, itr_end;
		bool found=false;
		QString str_aux;

		/* Before insert the type checks if there is some domain
		 with the same name as the type. */
		itr=domains.begin();
		itr_end=domains.end();
		while(itr!=itr_end && !found)
		{
			found=((*itr)->getName(true)==type->getName(true));
			itr++;
		}

		if(found)
		{
			str_aux=Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
					.arg(type->getName(true))
					.arg(type->getTypeName())
					.arg(this->getName(true))
					.arg(this->getTypeName());
			throw Exception(str_aux, ErrorCode::AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		try
		{
			__addObject(type, obj_idx);

			//When added to the model the user type is inserted on the pgsql base type list to be used as a column type
			PgSqlType::addUserType(type->getName(true), type, this, UserTypeConfig::BaseType);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}
}

void DatabaseModel::removeType(Type *type, int obj_idx)
{
	try
	{
		removeUserType(type, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Type *DatabaseModel::getType(unsigned obj_idx)
{
	return dynamic_cast<Type *>(getObject(obj_idx, ObjectType::Type));
}

Type *DatabaseModel::getType(const QString &name)
{
	return dynamic_cast<Type *>(getObject(name, ObjectType::Type));
}

void DatabaseModel::removeUserType(BaseObject *object, int obj_idx)
{
	try
	{
		__removeObject(object, obj_idx);

		//Removes the user type from the list of base types of pgsql
		PgSqlType::removeUserType(object->getName(true), object);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addPermissions(const vector<Permission *> &perms)
{
	vector<Permission *>::const_iterator itr=perms.cbegin(), itr_end=perms.cend();

	try
	{
		while(itr!=itr_end)
		{
			addPermission(*itr);
			itr++;
		}
	}
	catch(Exception &e)
	{
		//In case of errors removes the added permissions
		itr=perms.begin();
		itr_end=perms.end();

		while(itr!=itr_end)
		{
			removePermission(*itr);
			itr++;
		}

		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addPermission(Permission *perm)
{
	try
	{
		if(!perm)
			throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		TableObject *tab_obj=dynamic_cast<TableObject *>(perm->getObject());

		if(getPermissionIndex(perm, false) >=0)
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedPermission)
							.arg(perm->getObject()->getName())
							.arg(perm->getObject()->getTypeName()),
							ErrorCode::AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Raises an error if the permission is referencing an object that does not exists on model
		else if(perm->getObject()!=this &&
				((tab_obj && (getObjectIndex(tab_obj->getParentTable()) < 0)) ||
				 (!tab_obj && (getObjectIndex(perm->getObject()) < 0))))
			throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
							.arg(perm->getName())
							.arg(perm->getObject()->getTypeName())
							.arg(perm->getObject()->getName())
							.arg(perm->getObject()->getTypeName()),
							ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		permissions.push_back(perm);
		perm->setDatabase(this);
	}
	catch(Exception &e)
	{
		if(e.getErrorCode()==ErrorCode::AsgDuplicatedObject)
			throw
			Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedPermission)
					  .arg(perm->getObject()->getName())
					  .arg(perm->getObject()->getTypeName()),
						ErrorCode::AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

		else
			throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removePermission(Permission *perm)
{
	try
	{
		__removeObject(perm);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removePermissions(BaseObject *object)
{
	vector<BaseObject *>::iterator itr, itr_end;
	Permission *perm=nullptr;
	unsigned idx=0;

	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=permissions.begin();
	itr_end=permissions.end();

	while(itr!=itr_end)
	{
		perm=dynamic_cast<Permission *>(*itr);

		if(perm->getObject()==object)
		{
			permissions.erase(itr);
			itr=itr_end=permissions.end();

			if(!permissions.empty())
				itr=permissions.begin() + idx;
		}
		else
		{ itr++; idx++; }
	}
}

void DatabaseModel::getPermissions(BaseObject *object, vector<Permission *> &perms)
{
	vector<BaseObject *>::iterator itr, itr_end;
	Permission *perm=nullptr;

	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=permissions.begin();
	itr_end=permissions.end();

	perms.clear();

	while(itr!=itr_end)
	{
		perm=dynamic_cast<Permission *>(*itr);

		if(perm->getObject()==object)
			perms.push_back(perm);

		itr++;
	}
}

int DatabaseModel::getPermissionIndex(Permission *perm, bool exact_match)
{
	int perm_idx=-1;

	if(perm)
	{
		Permission *perm_aux=nullptr;
		vector<BaseObject *>::iterator itr, itr_end;

		itr=permissions.begin();
		itr_end=permissions.end();

		if(exact_match)
		{
			while(itr!=itr_end)
			{
				perm_aux=dynamic_cast<Permission *>(*itr);

				if(perm->isSimilarTo(perm_aux))
				{
					perm_idx=itr-permissions.begin();
					break;
				}

				itr++;
			}
		}
		else
		{
			BaseObject *object=nullptr;
			Role *role=nullptr;
			unsigned count, i;
			bool ref_role=false;

			object=perm->getObject();

			while(itr!=itr_end)
			{
				perm_aux=dynamic_cast<Permission *>(*itr);

				/* When the object of the auxiliary permission is the same as the
		   specified permission it will be check if the existant roles are
		   the same on both permissions */
				if(object==perm_aux->getObject())
				{
					count=perm->getRoleCount();

					for(i=0; i < count && !ref_role; i++)
					{
						role=perm->getRole(i);
						ref_role=perm_aux->isRoleExists(role);
					}
				}

				//If the permissions references the same roles but one is a REVOKE and other GRANT they a considered different
				if(perm==perm_aux || (ref_role && perm->isRevoke()==perm_aux->isRevoke()))
				{
					perm_idx=itr-permissions.begin();
					break;
				}

				itr++;
			}
		}
	}

	return perm_idx;
}

BaseObject *DatabaseModel::getObject(const QString &name, ObjectType obj_type)
{
	int idx;
	return getObject(name, obj_type, idx);
}

int DatabaseModel::getObjectIndex(const QString &name, ObjectType obj_type)
{
	int idx;
	getObject(name, obj_type, idx);
	return idx;
}

int DatabaseModel::getObjectIndex(BaseObject *object)
{
	if(!object)
		return -1;
	else
	{
		ObjectType obj_type=object->getObjectType();
		vector<BaseObject *> *obj_list=nullptr;
		vector<BaseObject *>::iterator itr, itr_end;
		bool found=false;

		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end && !found)
			{
				found=((*itr)==object);
				if(!found) itr++;
			}

			if(found)
				return itr - obj_list->begin();
			else
				return -1;
		}
	}
}

void DatabaseModel::configureDatabase(attribs_map &attribs)
{
	encoding=attribs[Attributes::Encoding];
	template_db=attribs[Attributes::TemplateDb];
	localizations[0]=attribs[Attributes::LcCtype];
	localizations[1]=attribs[Attributes::LcCollate];
	append_at_eod=attribs[Attributes::AppendAtEod]==Attributes::True;
	prepend_at_bod=attribs[Attributes::PrependAtBod]==Attributes::True;
	is_template=attribs[Attributes::IsTemplate]==Attributes::True;
	allow_conns=attribs[Attributes::AllowConns] != Attributes::False;

	if(!attribs[Attributes::ConnLimit].isEmpty())
		conn_limit=attribs[Attributes::ConnLimit].toInt();

	setBasicAttributes(this);
}

void DatabaseModel::loadModel(const QString &filename)
{
	if(!filename.isEmpty())
	{
		QString dtd_file, str_aux, elem_name;
		ObjectType obj_type;
		attribs_map attribs;
		BaseObject *object=nullptr;
		bool protected_model=false, found_inh_rel = false;
		QStringList pos_str;
		map<ObjectType, QString> def_objs;

		//Configuring the path to the base path for objects DTD
		dtd_file=GlobalAttributes::getSchemasRootDir() +
						 GlobalAttributes::DirSeparator +
						 GlobalAttributes::XMLSchemaDir +
						 GlobalAttributes::DirSeparator +
						 GlobalAttributes::ObjectDTDDir +
						 GlobalAttributes::DirSeparator;

		try
		{
			loading_model=true;
			xmlparser.restartParser();

			//Loads the root DTD
			xmlparser.setDTDFile(dtd_file + GlobalAttributes::RootDTD +
								 GlobalAttributes::ObjectDTDExt,
								 GlobalAttributes::RootDTD);

			//Loads the file validating it against the root DTD
			xmlparser.loadXMLFile(filename);

			//Gets the basic model information
			xmlparser.getElementAttributes(attribs);

			setObjectListsCapacity(attribs[Attributes::MaxObjCount].toUInt());

			this->author=attribs[Attributes::ModelAuthor];

			pos_str=attribs[Attributes::LastPosition].split(',');

			if(pos_str.size()>=2)
				this->last_pos=QPoint(pos_str[0].toUInt(),pos_str[1].toUInt());

			this->last_zoom=attribs[Attributes::LastZoom].toDouble();
			if(this->last_zoom <= 0) this->last_zoom=1;

			this->is_template = attribs[Attributes::IsTemplate] == Attributes::True;
			this->allow_conns = (attribs[Attributes::AllowConns].isEmpty() ||
													 attribs[Attributes::AllowConns] == Attributes::True);

			layers = attribs[Attributes::Layers].split(';', QString::SkipEmptyParts);

			active_layers.clear();

			/* Compatibility with models created prior the layers features:
			 * If the "active-layers" is absent we make the default layer always visible */
			if(!attribs.count(Attributes::ActiveLayers))
				active_layers.push_back(0);
			else
			{
				for(auto &layer_id : attribs[Attributes::ActiveLayers].split(';', QString::SkipEmptyParts))
					active_layers.push_back(layer_id.toInt());
			}

			protected_model=(attribs[Attributes::Protected]==Attributes::True);

			def_objs[ObjectType::Schema]=attribs[Attributes::DefaultSchema];
			def_objs[ObjectType::Role]=attribs[Attributes::DefaultOwner];
			def_objs[ObjectType::Collation]=attribs[Attributes::DefaultCollation];
			def_objs[ObjectType::Tablespace]=attribs[Attributes::DefaultTablespace];

			if(xmlparser.accessElement(XmlParser::ChildElement))
			{
				do
				{
					if(xmlparser.getElementType()==XML_ELEMENT_NODE)
					{
						elem_name=xmlparser.getElementName();

						//Indentifies the object type to be load according to the current element on the parser
						obj_type=getObjectType(elem_name);

						if(obj_type==ObjectType::Database)
						{
							xmlparser.getElementAttributes(attribs);
							configureDatabase(attribs);
						}
						else
						{
							try
							{
								//Saves the current position of the parser before create any object
								xmlparser.savePosition();
								object=createObject(obj_type);

								if(object)
								{
									if(!dynamic_cast<TableObject *>(object) && obj_type!=ObjectType::Relationship && obj_type!=ObjectType::BaseRelationship)
										addObject(object);

									/* If there is at least one inheritance relationship we need to flag this situation
									 in order to do an addtional rel. validation in the end of loading */
									if(!found_inh_rel && object->getObjectType()==ObjectType::Relationship &&
											dynamic_cast<Relationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipGen)
										found_inh_rel=true;

									emit s_objectLoaded((xmlparser.getCurrentBufferLine()/static_cast<double>(xmlparser.getBufferLineCount()))*100,
														tr("Loading: `%1' (%2)")
														.arg(object->getName())
														.arg(object->getTypeName()),
														enum_cast(obj_type));
								}

								xmlparser.restorePosition();
							}
							catch(Exception &e)
							{
								QString info_adicional=QString(QObject::tr("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);
								throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
							}
						}
					}
				}
				while(xmlparser.accessElement(XmlParser::NextElement));
			}

			this->BaseObject::setProtected(protected_model);

			//Validating default objects
			for(auto &itr : def_objs)
			{
				if(!itr.second.isEmpty())
				{
					object=this->getObject(itr.second, itr.first);

					if(!object)
						throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
										.arg(this->getName())
										.arg(this->getTypeName())
										.arg(itr.second)
										.arg(BaseObject::getTypeName(itr.first)),
										ErrorCode::AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					this->setDefaultObject(object);
				}
				else
					this->setDefaultObject(nullptr, itr.first);
			}

			loading_model=false;

			//If there are relationship make a relationship validation to recreate any special object left behind
			if(!relationships.empty())
			{
				emit s_objectLoaded(100, tr("Validating relationships..."), enum_cast(ObjectType::Relationship));
				storeSpecialObjectsXML();
				disconnectRelationships();
				validateRelationships();
			}

			this->setInvalidated(false);
			emit s_objectLoaded(100, tr("Validating relationships..."), enum_cast(ObjectType::Relationship));

			//Doing another relationship validation when there are inheritances to avoid incomplete tables
			if(found_inh_rel)
			{
				emit s_objectLoaded(100, tr("Validating relationships..."), enum_cast(ObjectType::Relationship));
				validateRelationships();
			}

			updateTablesFKRelationships();
			emit s_objectLoaded(100, tr("Rendering database model..."), enum_cast(ObjectType::BaseObject));
			this->setObjectsModified();
		}
		catch(Exception &e)
		{
			QString extra_info;
			loading_model=false;

			if(xmlparser.getCurrentElement())
				extra_info=QString(QObject::tr("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);

			if(e.getErrorCode()>=ErrorCode::InvalidSyntax)
			{
				str_aux=Exception::getErrorMessage(ErrorCode::InvModelFileNotLoaded).arg(filename);
				throw Exception(str_aux,ErrorCode::InvModelFileNotLoaded,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
			}
			else
				throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
		}
	}
}

BaseObject *DatabaseModel::createObject(ObjectType obj_type)
{
	BaseObject *object=nullptr;

	if(obj_type!=ObjectType::BaseObject)
	{
		if(obj_type==ObjectType::Role)
			object=createRole();
		else if(obj_type==ObjectType::Tablespace)
			object=createTablespace();
		else if(obj_type==ObjectType::Schema)
			object=createSchema();
		else if(obj_type==ObjectType::Language)
			object=createLanguage();
		else if(obj_type==ObjectType::Function)
			object=createFunction();
		else if(obj_type==ObjectType::Type)
			object=createType();
		else if(obj_type==ObjectType::Domain)
			object=createDomain();
		else if(obj_type==ObjectType::Cast)
			object=createCast();
		else if(obj_type==ObjectType::Conversion)
			object=createConversion();
		else if(obj_type==ObjectType::Operator)
			object=createOperator();
		else if(obj_type==ObjectType::OpFamily)
			object=createOperatorFamily();
		else if(obj_type==ObjectType::OpClass)
			object=createOperatorClass();
		else if(obj_type==ObjectType::Aggregate)
			object=createAggregate();
		else if(obj_type==ObjectType::Table)
			object=createTable();
		else if(obj_type==ObjectType::Sequence)
			object=createSequence();
		else if(obj_type==ObjectType::View)
			object=createView();
		else if(obj_type==ObjectType::Textbox)
			object=createTextbox();
		else if(obj_type==ObjectType::Constraint)
			object=createConstraint(nullptr);
		else if(obj_type==ObjectType::Trigger)
			object=createTrigger();
		else if(obj_type==ObjectType::Index)
			object=createIndex();
		else if(obj_type==ObjectType::Column)
			object=createColumn();
		else if(obj_type==ObjectType::Rule)
			object=createRule();
		else if(obj_type==ObjectType::Relationship ||
				obj_type==ObjectType::BaseRelationship)
			object=createRelationship();
		else if(obj_type==ObjectType::Collation)
			object=createCollation();
		else if(obj_type==ObjectType::Extension)
			object=createExtension();
		else if(obj_type==ObjectType::Tag)
			object=createTag();
		else if(obj_type==ObjectType::Permission)
			object=createPermission();
		else if(obj_type==ObjectType::EventTrigger)
			object=createEventTrigger();
		else if(obj_type==ObjectType::GenericSql)
			object=createGenericSQL();
		else if(obj_type==ObjectType::Policy)
			object=createPolicy();
		else if(obj_type==ObjectType::ForeignDataWrapper)
			object=createForeignDataWrapper();
		else if(obj_type==ObjectType::ForeignServer)
			object=createForeignServer();
		else if(obj_type==ObjectType::UserMapping)
			object=createUserMapping();
		else if(obj_type==ObjectType::ForeignTable)
			object=createForeignTable();
	}

	return object;
}

void DatabaseModel::setBasicAttributes(BaseObject *object)
{
	attribs_map attribs, attribs_aux;
	QString elem_name;
	BaseObject *tabspc=nullptr, *owner=nullptr, *collation=nullptr;
	Schema *schema=nullptr;
	ObjectType obj_type=ObjectType::BaseObject, obj_type_aux;
	bool has_error=false, protected_obj=false, sql_disabled=false;
	ForeignObject *frn_object = dynamic_cast<ForeignObject *>(object);

	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	xmlparser.getElementAttributes(attribs);
	obj_type_aux=object->getObjectType();

	if(obj_type_aux!=ObjectType::Cast && obj_type_aux != ObjectType::UserMapping)
		object->setName(attribs[Attributes::Name]);

	if(BaseGraphicObject::isGraphicObject(obj_type_aux) && !attribs[Attributes::ZValue].isEmpty())
		dynamic_cast<BaseGraphicObject *>(object)->setZValue(attribs[Attributes::ZValue].toInt());

	if(BaseObject::acceptsAlias(obj_type_aux))
		object->setAlias(attribs[Attributes::Alias]);

	protected_obj=attribs[Attributes::Protected]==Attributes::True;
	sql_disabled=attribs[Attributes::SqlDisabled]==Attributes::True;

	if(frn_object)
	{
		QStringList opt_val;

		for(auto &option : attribs[Attributes::Options].split(ForeignObject::OptionsSeparator))
		{
			opt_val = option.split(UserMapping::OptionValueSeparator);

			if(opt_val.size() < 2)
				continue;

			frn_object->setOption(opt_val[0], opt_val[1]);
		}
	}

	xmlparser.savePosition();

	if(xmlparser.accessElement(XmlParser::ChildElement))
	{
		do
		{
			if(xmlparser.getElementType()==XML_ELEMENT_NODE)
			{
				elem_name=xmlparser.getElementName();

				//Defines the object's comment
				if(elem_name==Attributes::Comment)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setComment(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				//Defines the object's schema
				else if(elem_name==Attributes::Schema)
				{
					obj_type=ObjectType::Schema;
					xmlparser.getElementAttributes(attribs_aux);
					schema=dynamic_cast<Schema *>(getObject(attribs_aux[Attributes::Name], obj_type));
					object->setSchema(schema);
					has_error=(!schema && !attribs_aux[Attributes::Name].isEmpty());
				}
				//Defines the object's tablespace
				else if(elem_name==Attributes::Tablespace)
				{
					obj_type=ObjectType::Tablespace;
					xmlparser.getElementAttributes(attribs_aux);
					tabspc=getObject(attribs_aux[Attributes::Name], obj_type);
					object->setTablespace(tabspc);
					has_error=(!tabspc && !attribs_aux[Attributes::Name].isEmpty());
				}
				//Defines the object's owner
				else if(elem_name==Attributes::Role)
				{
					obj_type=ObjectType::Role;
					xmlparser.getElementAttributes(attribs_aux);
					owner=getObject(attribs_aux[Attributes::Name], obj_type);
					object->setOwner(owner);
					has_error=(!owner && !attribs_aux[Attributes::Name].isEmpty());
				}
				//Defines the object's schema
				else if(elem_name==Attributes::Collation)
				{
					obj_type=ObjectType::Collation;
					xmlparser.getElementAttributes(attribs_aux);
					collation=getObject(attribs_aux[Attributes::Name], obj_type);
					object->setCollation(collation);
					has_error=(!collation && !attribs_aux[Attributes::Name].isEmpty());
				}
				else if(elem_name==Attributes::AppendedSql)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setAppendedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				else if(elem_name==Attributes::PrependedSql)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setPrependedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				//Defines the object's position (only for graphical objects)
				else if(elem_name==Attributes::Position)
				{
					xmlparser.getElementAttributes(attribs);

					if(elem_name==Attributes::Position &&
							(obj_type_aux!=ObjectType::Relationship &&
							 obj_type_aux!=ObjectType::BaseRelationship))
					{
						dynamic_cast<BaseGraphicObject *>(object)->setPosition(QPointF(attribs[Attributes::XPos].toDouble(),
																				 attribs[Attributes::YPos].toDouble()));


					}
				}
			}
		}
		while(!has_error && xmlparser.accessElement(XmlParser::NextElement));
	}

	xmlparser.restorePosition();
	object->setProtected(protected_obj);
	object->setSQLDisabled(sql_disabled);

	if(has_error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
						.arg(object->getName())
						.arg(object->getTypeName())
						.arg(attribs_aux[Attributes::Name])
				.arg(BaseObject::getTypeName(obj_type)),
				ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//Schema on extensions are optional
	else if(!object->getSchema() && (BaseObject::acceptsSchema(obj_type_aux) && obj_type_aux != ObjectType::Extension))
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvObjectAllocationNoSchema)
						.arg(object->getName())
						.arg(object->getTypeName()),
						ErrorCode::InvObjectAllocationNoSchema,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

QString DatabaseModel::getErrorExtraInfo()
{
	QString extra_info;

	if(!xmlparser.getLoadedFilename().isEmpty())
		extra_info=QString(QObject::tr("%1 (line: %2)")).arg(xmlparser.getLoadedFilename())
				   .arg(xmlparser.getCurrentElement()->line);
	else
		extra_info=xmlparser.getXMLBuffer();

	return extra_info;
}

void DatabaseModel::setLoadingModel(bool value)
{
  loading_model = value;
}

void DatabaseModel::setObjectListsCapacity(unsigned capacity)
{
  if(capacity < BaseObject::DefMaxObjectCount ||
	 capacity > (BaseObject::DefMaxObjectCount * 1000))
	capacity = BaseObject::DefMaxObjectCount;

  unsigned half_cap = capacity/2, one_fourth_cap = capacity/4;

  views.reserve(capacity);
  tables.reserve(capacity);
  relationships.reserve(capacity);
  base_relationships.reserve(capacity);
  sequences.reserve(capacity);
  permissions.reserve(capacity);
  schemas.reserve(half_cap);
  roles.reserve(half_cap);
  functions.reserve(half_cap);
  types.reserve(half_cap);
  textboxes.reserve(half_cap);
  aggregates.reserve(half_cap);
  operators.reserve(half_cap);
  op_classes.reserve(half_cap);
  op_families.reserve(half_cap);
  domains.reserve(half_cap);
  collations.reserve(half_cap);
  extensions.reserve(half_cap);
  tags.reserve(half_cap);
  genericsqls.reserve(half_cap);
  tablespaces.reserve(one_fourth_cap);
  languages.reserve(one_fourth_cap);
  casts.reserve(one_fourth_cap);
  conversions.reserve(one_fourth_cap);
  eventtriggers.reserve(one_fourth_cap);
}

void DatabaseModel::setLastPosition(const QPoint &pnt)
{
	last_pos=pnt;
}

QPoint DatabaseModel::getLastPosition()
{
	return last_pos;
}

void DatabaseModel::setLastZoomFactor(double zoom)
{
	last_zoom=zoom;
}

double DatabaseModel::getLastZoomFactor()
{
	return last_zoom;
}

Role *DatabaseModel::createRole()
{
	attribs_map attribs, attribs_aux;
	Role *role=nullptr, *ref_role=nullptr;
	int i, len;
	bool marked;
	QStringList list;
	QString elem_name;
	unsigned role_type;

	QString op_attribs[]={ Attributes::Superuser, Attributes::CreateDb,
							 Attributes::CreateRole, Attributes::Inherit,
							 Attributes::Login, Attributes::Encrypted,
							 Attributes::Replication, Attributes::BypassRls };

	unsigned op_vect[]={ Role::OpSuperuser, Role::OpCreateDb,
						 Role::OpCreateRole, Role::OpInherit,
						 Role::OpLogin, Role::OpEncrypted,
						 Role::OpReplication, Role::OpBypassRls };

	try
	{
		role=new Role;
		setBasicAttributes(role);

		//Gets all the attributes values from the XML
		xmlparser.getElementAttributes(attribs);

		role->setPassword(attribs[Attributes::Password]);
		role->setValidity(attribs[Attributes::Validity]);

		if(!attribs[Attributes::ConnLimit].isEmpty())
			role->setConnectionLimit(attribs[Attributes::ConnLimit].toInt());

		//Setting up the role options according to the configured on the XML
		for(i=0; i < 8; i++)
		{
			marked=attribs[op_attribs[i]]==Attributes::True;
			role->setOption(op_vect[i], marked);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem_name=xmlparser.getElementName();

					//Getting the member roles
					if(elem_name==Attributes::Roles)
					{
						//Gets the member roles attributes
						xmlparser.getElementAttributes(attribs_aux);

						//The member roles names are separated by comma, so it is needed to split them
						list=attribs_aux[Attributes::Names].split(',');
						len=list.size();

						//Identifying the member role type
						if(attribs_aux[Attributes::RoleType]==Attributes::Refer)
							role_type=Role::RefRole;
						else if(attribs_aux[Attributes::RoleType]==Attributes::Member)
							role_type=Role::MemberRole;
						else
							role_type=Role::AdminRole;

						for(i=0; i < len; i++)
						{
							//Gets the role using the name from the model using the name from the list
							ref_role=dynamic_cast<Role *>(getObject(list[i].trimmed(),ObjectType::Role));

							//Raises an error if the roles doesn't exists
							if(!ref_role)
							{
								throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
												.arg(role->getName())
												.arg(BaseObject::getTypeName(ObjectType::Role))
												.arg(list[i])
												.arg(BaseObject::getTypeName(ObjectType::Role)),
												ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							role->addRole(role_type, ref_role);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(role) delete role;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return role;
}

Tablespace *DatabaseModel::createTablespace()
{
	attribs_map attribs;
	Tablespace *tabspc=nullptr;

	try
	{
		tabspc=new Tablespace;
		setBasicAttributes(tabspc);
		xmlparser.getElementAttributes(attribs);
		tabspc->setDirectory(attribs[Attributes::Directory]);
	}
	catch(Exception &e)
	{
		if(tabspc) delete tabspc;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return tabspc;
}

Schema *DatabaseModel::createSchema()
{
	Schema *schema=nullptr;
	attribs_map attribs;

	try
	{
		schema=new Schema;
		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(schema);
		schema->setFillColor(QColor(attribs[Attributes::FillColor]));
		schema->setRectVisible(attribs[Attributes::RectVisible]==Attributes::True);
		schema->setFadedOut(attribs[Attributes::FadedOut]==Attributes::True);
		schema->setLayer(attribs[Attributes::Layer].toUInt());
	}
	catch(Exception &e)
	{
		if(schema) delete schema;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return schema;
}

Language *DatabaseModel::createLanguage()
{
	attribs_map attribs;
	Language *lang=nullptr;
	BaseObject *func=nullptr;
	QString signature, ref_type;
	ObjectType obj_type;

	try
	{
		lang=new Language;
		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(lang);

		lang->setTrusted(attribs[Attributes::Trusted]==Attributes::True);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					obj_type=BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type==ObjectType::Function)
					{
						xmlparser.getElementAttributes(attribs);

						//Gets the function reference type
						ref_type=attribs[Attributes::RefType];

						//Only VALIDATOR, HANDLER and INLINE functions are accepted for the language
						if(ref_type==Attributes::ValidatorFunc ||
								ref_type==Attributes::HandlerFunc ||
								ref_type==Attributes::InlineFunc)
						{
							//Gets the function signature and tries to retrieve it from the model
							signature=attribs[Attributes::Signature];
							func=getObject(signature, ObjectType::Function);

							//Raises an error if the function doesn't exists
							if(!func)
								throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
												.arg(lang->getName())
												.arg(lang->getTypeName())
												.arg(signature)
												.arg(BaseObject::getTypeName(ObjectType::Function)),
												ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

							if(ref_type==Attributes::ValidatorFunc)
								lang->setFunction(dynamic_cast<Function *>(func), Language::ValidatorFunc);
							else if(ref_type==Attributes::HandlerFunc)
								lang->setFunction(dynamic_cast<Function *>(func), Language::HandlerFunc);
							else
								lang->setFunction(dynamic_cast<Function *>(func), Language::InlineFunc);

						}
						else
							//Raises an error if the function type is invalid
							throw Exception(ErrorCode::RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

	}
	catch(Exception &e)
	{
		if(lang) delete lang;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return lang;
}

Function *DatabaseModel::createFunction()
{
	attribs_map attribs, attribs_aux;
	Function *func=nullptr;
	ObjectType obj_type;
	BaseObject *object=nullptr;
	PgSqlType type;
	Parameter param;
	QString str_aux, elem;

	try
	{
		func=new Function;
		setBasicAttributes(func);
		xmlparser.getElementAttributes(attribs);

		if(!attribs[Attributes::ReturnsSetOf].isEmpty())
			func->setReturnSetOf(attribs[Attributes::ReturnsSetOf]==
					Attributes::True);

		if(!attribs[Attributes::WindowFunc].isEmpty())
			func->setWindowFunction(attribs[Attributes::WindowFunc]==
					Attributes::True);

		if(!attribs[Attributes::LeakProof].isEmpty())
			func->setLeakProof(attribs[Attributes::LeakProof]==
					Attributes::True);

		if(!attribs[Attributes::BehaviorType].isEmpty())
			func->setBehaviorType(BehaviorType(attribs[Attributes::BehaviorType]));

		if(!attribs[Attributes::FunctionType].isEmpty())
			func->setFunctionType(FunctionType(attribs[Attributes::FunctionType]));

		if(!attribs[Attributes::SecurityType].isEmpty())
			func->setSecurityType(SecurityType(attribs[Attributes::SecurityType]));

		if(!attribs[Attributes::ExecutionCost].isEmpty())
			func->setExecutionCost(attribs[Attributes::ExecutionCost].toInt());

		if(!attribs[Attributes::RowAmount].isEmpty())
			func->setRowAmount(attribs[Attributes::RowAmount].toInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();
					obj_type=BaseObject::getObjectType(elem);

					//Gets the function return type from the XML
					if(elem==Attributes::ReturnType)
					{
						xmlparser.savePosition();

						try
						{
							xmlparser.accessElement(XmlParser::ChildElement);

							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE)
								{
									//when the element found is a TYPE indicates that the function return type is a single one
									if(xmlparser.getElementName()==Attributes::Type)
									{
										type=createPgSQLType();
										func->setReturnType(type);
									}
									//when the element found is a PARAMETER indicates that the function return type is a table
									else if(xmlparser.getElementName()==Attributes::Parameter)
									{
										param=createParameter();
										func->addReturnedTableColumn(param.getName(), param.getType());
									}
								}
							}
							while(xmlparser.accessElement(XmlParser::NextElement));

							xmlparser.restorePosition();
						}
						catch(Exception &e)
						{
							xmlparser.restorePosition();
							throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
						}
					}
					//Gets the function language
					else if(obj_type==ObjectType::Language)
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[Attributes::Name], obj_type);

						//Raises an error if the function doesn't exisits
						if(!object)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(func->getName())
											.arg(func->getTypeName())
											.arg(attribs[Attributes::Name])
								.arg(BaseObject::getTypeName(ObjectType::Language)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						func->setLanguage(dynamic_cast<Language *>(object));
					}
					//Gets a function parameter
					else if(xmlparser.getElementName()==Attributes::Parameter)
					{
						param=createParameter();
						func->addParameter(param);
					}
					//Gets the function code definition
					else if(xmlparser.getElementName()==Attributes::Definition)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs_aux);

						if(!attribs_aux[Attributes::Library].isEmpty())
						{
							func->setLibrary(attribs_aux[Attributes::Library]);
							func->setSymbol(attribs_aux[Attributes::Symbol]);
						}
						else if(xmlparser.accessElement(XmlParser::ChildElement))
							func->setSourceCode(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(func)
		{
			str_aux=func->getName(true);
			delete func;
		}

		if(e.getErrorCode()==ErrorCode::RefUserTypeInexistsModel)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgObjectInvalidDefinition)
							.arg(str_aux)
							.arg(BaseObject::getTypeName(ObjectType::Function)),
							ErrorCode::AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
		else
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return func;
}

Parameter DatabaseModel::createParameter()
{
	Parameter param;
	attribs_map attribs;
	QString elem;

	try
	{
		xmlparser.savePosition();
		xmlparser.getElementAttributes(attribs);

		param.setName(attribs[Attributes::Name]);
		param.setDefaultValue(attribs[Attributes::DefaultValue]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Type)
					{
						param.setType(createPgSQLType());
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		param.setIn(attribs[Attributes::ParamIn]==Attributes::True);
		param.setOut(attribs[Attributes::ParamOut]==Attributes::True);
		param.setVariadic(attribs[Attributes::ParamVariadic]==Attributes::True);

		xmlparser.restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info=getErrorExtraInfo();
		xmlparser.restorePosition();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return param;
}

TypeAttribute DatabaseModel::createTypeAttribute()
{
	TypeAttribute tpattrib;
	attribs_map attribs;
	QString elem;
	BaseObject *collation=nullptr;

	try
	{
		xmlparser.savePosition();
		xmlparser.getElementAttributes(attribs);

		tpattrib.setName(attribs[Attributes::Name]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Type)
					{
						tpattrib.setType(createPgSQLType());
					}
					else if(elem==Attributes::Collation)
					{
						xmlparser.getElementAttributes(attribs);

						collation=getObject(attribs[Attributes::Name], ObjectType::Collation);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(tpattrib.getName())
											.arg(tpattrib.getTypeName())
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Collation)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						tpattrib.setCollation(collation);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		xmlparser.restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info=getErrorExtraInfo();
		xmlparser.restorePosition();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return tpattrib;
}

PgSqlType DatabaseModel::createPgSQLType()
{
	attribs_map attribs;
	unsigned length=1, dimension=0, type_idx=0;
	int precision=-1;
	QString name;
	void *ptype=nullptr;
	bool with_timezone;
	IntervalType interv_type;
	SpatialType spatial_type;

	xmlparser.getElementAttributes(attribs);

	if(!attribs[Attributes::Length].isEmpty())
		length=attribs[Attributes::Length].toUInt();

	if(!attribs[Attributes::Dimension].isEmpty())
		dimension=attribs[Attributes::Dimension].toUInt();

	if(!attribs[Attributes::Precision].isEmpty())
		precision=attribs[Attributes::Precision].toInt();

	with_timezone=(attribs[Attributes::WithTimezone]==Attributes::True);
	interv_type=attribs[Attributes::IntervalType];

	if(!attribs[Attributes::SpatialType].isEmpty())
		spatial_type=SpatialType(attribs[Attributes::SpatialType],
				attribs[Attributes::Srid].toUInt(),
				attribs[Attributes::Variation].toUInt());

	name=attribs[Attributes::Name];

	/* A small tweak to detect a timestamp/date type which name contains the time zone modifier.
		 This situation can occur mainly on reverse engineering operation where the data type of objects
		 in most of times came as string form and need to be parsed */
	if(!with_timezone && attribs[Attributes::Name].contains(QString("with time zone"), Qt::CaseInsensitive))
	{
		with_timezone=true;
		name.remove(QString(" with time zone"), Qt::CaseInsensitive);
	}

	type_idx=PgSqlType::getBaseTypeIndex(name);
	if(type_idx!=PgSqlType::Null)
	{
		return PgSqlType(name, dimension, length, precision, with_timezone, interv_type, spatial_type);
	}
	else
	{
		//Raises an error if the referenced type name doesn't exists
		if(PgSqlType::getUserTypeIndex(name,nullptr,this) == BaseType::Null)
			throw Exception(ErrorCode::RefUserTypeInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		type_idx=PgSqlType::getUserTypeIndex(name, ptype);
		return PgSqlType(type_idx, dimension, length, precision, with_timezone, interv_type, spatial_type);
	}
}

Type *DatabaseModel::createType()
{
	attribs_map attribs;
	map<QString, unsigned> func_types;
	Type *type=nullptr;
	int count, i;
	QStringList enums;
	QString elem, str_aux;
	BaseObject *func=nullptr, *collation=nullptr;
	OperatorClass *op_class=nullptr;
	PgSqlType aux_type;

	try
	{
		type=new Type;
		setBasicAttributes(type);
		xmlparser.getElementAttributes(attribs);

		if(attribs[Attributes::Configuration]==Attributes::BaseType)
		{
			type->setConfiguration(Type::BaseType);
			type->setByValue(attribs[Attributes::ByValue]==Attributes::True);

			if(!attribs[Attributes::InternalLength].isEmpty())
				type->setInternalLength(attribs[Attributes::InternalLength].toUInt());

			if(!attribs[Attributes::Alignment].isEmpty())
				type->setAlignment(attribs[Attributes::Alignment]);

			if(!attribs[Attributes::Storage].isEmpty())
				type->setStorage(attribs[Attributes::Storage]);

			if(!attribs[Attributes::Element].isEmpty())
				type->setElement(attribs[Attributes::Element]);

			if(!attribs[Attributes::Delimiter].isEmpty())
				type->setDelimiter(attribs[Attributes::Delimiter][0].toLatin1());

			if(!attribs[Attributes::DefaultValue].isEmpty())
				type->setDefaultValue(attribs[Attributes::DefaultValue]);

			if(!attribs[Attributes::Category].isEmpty())
				type->setCategory(attribs[Attributes::Category]);

			if(!attribs[Attributes::Preferred].isEmpty())
				type->setPreferred(attribs[Attributes::Preferred]==Attributes::True);

			//Configuring an auxiliary map used to reference the functions used by base type
			func_types[Attributes::InputFunc]=Type::InputFunc;
			func_types[Attributes::OutputFunc]=Type::OutputFunc;
			func_types[Attributes::SendFunc]=Type::SendFunc;
			func_types[Attributes::RecvFunc]=Type::RecvFunc;
			func_types[Attributes::TpmodInFunc]=Type::TpmodInFunc;
			func_types[Attributes::TpmodOutFunc]=Type::TpmodOutFunc;
			func_types[Attributes::AnalyzeFunc]=Type::AnalyzeFunc;
		}
		else if(attribs[Attributes::Configuration]==Attributes::CompositeType)
			type->setConfiguration(Type::CompositeType);
		else if(attribs[Attributes::Configuration]==Attributes::EnumType)
			type->setConfiguration(Type::EnumerationType);
		else
		{
			type->setConfiguration(Type::RangeType);
			func_types[Attributes::CanonicalFunc]=Type::CanonicalFunc;
			func_types[Attributes::SubtypeDiffFunc]=Type::SubtypeDiffFunc;
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//Specific operations for ENUM type
					if(elem==Attributes::EnumType)
					{
						xmlparser.getElementAttributes(attribs);
						enums=attribs[Attributes::Values].split(',');

						count=enums.size();
						for(i=0; i < count; i++)
							type->addEnumeration(enums[i]);
					}
					//Specific operations for COMPOSITE types
					else if(elem==Attributes::TypeAttribute)
					{
						type->addAttribute(createTypeAttribute());
					}
					//Specific operations for BASE / RANGE type
					else if(elem==Attributes::Type)
					{
						aux_type=createPgSQLType();

						if(type->getConfiguration()==Type::RangeType)
							type->setSubtype(aux_type);
						else
							type->setLikeType(aux_type);
					}
					else if(elem==Attributes::Collation)
					{
						xmlparser.getElementAttributes(attribs);
						collation=getObject(attribs[Attributes::Name], ObjectType::Collation);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Collation)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setCollation(collation);
					}
					if(elem==Attributes::OpClass)
					{
						xmlparser.getElementAttributes(attribs);
						op_class=dynamic_cast<OperatorClass *>(getObject(attribs[Attributes::Name], ObjectType::OpClass));

						//Raises an error if the operator class doesn't exists
						if(!op_class)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::OpClass)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setSubtypeOpClass(op_class);
					}
					//Configuring the functions used by the type (only for BASE type)
					else if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);

						//Tries to get the function from the model
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Function)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						//Raises an error if the function type is invalid
						else if(func_types.count(attribs[Attributes::RefType])==0)
							throw Exception(ErrorCode::RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						type->setFunction(func_types[attribs[Attributes::RefType]],	dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(type)
		{
			str_aux=type->getName(true);
			delete type;
		}

		if(e.getErrorCode()==ErrorCode::RefUserTypeInexistsModel)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgObjectInvalidDefinition)
							.arg(str_aux)
							.arg(type->getTypeName()),
							ErrorCode::AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
		else
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return type;
}

Domain *DatabaseModel::createDomain()
{
	attribs_map attribs;
	Domain *domain=nullptr;
	QString elem;

	try
	{
		domain=new Domain;
		setBasicAttributes(domain);
		xmlparser.getElementAttributes(attribs);

		if(!attribs[Attributes::DefaultValue].isEmpty())
			domain->setDefaultValue(attribs[Attributes::DefaultValue]);

		domain->setNotNull(attribs[Attributes::NotNull]==Attributes::True);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//If a type element is found it'll be extracted an type which the domain is applied
					if(elem==Attributes::Type)
					{
						domain->setType(createPgSQLType());
					}
					else if(elem==Attributes::Constraint)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						xmlparser.accessElement(XmlParser::ChildElement);
						xmlparser.accessElement(XmlParser::ChildElement);
						domain->addCheckConstraint(attribs[Attributes::Name], xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(xmlparser.NextElement));
		}
	}
	catch(Exception &e)
	{
		if(domain) delete domain;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return domain;
}

Cast *DatabaseModel::createCast()
{
	attribs_map attribs;
	Cast *cast=nullptr;
	QString elem;
	unsigned type_idx=0;
	PgSqlType type;
	BaseObject *func=nullptr;

	try
	{
		cast=new Cast;
		setBasicAttributes(cast);
		xmlparser.getElementAttributes(attribs);

		if(attribs[Attributes::CastType]==Attributes::Implicit)
			cast->setCastType(Cast::Implicit);
		else if(attribs[Attributes::CastType]==Attributes::Assignment)
			cast->setCastType(Cast::Assignment);
		else
			cast->setCastType(Cast::Explicit);

		cast->setInOut(attribs[Attributes::IoCast]==Attributes::True);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//Extract one argument type from the XML
					if(elem==Attributes::Type)
					{
						type=createPgSQLType();
						if(type_idx==0)
							cast->setDataType(Cast::SrcType, type);
						else
							cast->setDataType(Cast::DstType, type);
						type_idx++;
					}
					//Extracts the conversion function
					else if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(cast->getName())
											.arg(cast->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Function)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						cast->setCastFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(cast) delete cast;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return cast;
}

Conversion *DatabaseModel::createConversion()
{
	attribs_map attribs;
	Conversion *conv=nullptr;
	QString elem;
	BaseObject *func=nullptr;

	try
	{
		conv=new Conversion;
		setBasicAttributes(conv);
		xmlparser.getElementAttributes(attribs);

		conv->setEncoding(Conversion::SrcEncoding,
							EncodingType(attribs[Attributes::SrcEncoding]));

		conv->setEncoding(Conversion::DstEncoding,
							EncodingType(attribs[Attributes::DstEncoding]));

		conv->setDefault(attribs[Attributes::Default]==Attributes::True);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(conv->getName())
											.arg(conv->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Function)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						conv->setConversionFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(conv) delete conv;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return conv;
}

Operator *DatabaseModel::createOperator()
{
	attribs_map attribs;
	map<QString, unsigned> func_types;
	map<QString, unsigned> oper_types;
	Operator *oper=nullptr;
	QString elem;
	BaseObject *func=nullptr,*oper_aux=nullptr;
	unsigned arg_type;
	PgSqlType type;

	try
	{
		oper=new Operator;
		setBasicAttributes(oper);
		xmlparser.getElementAttributes(attribs);

		oper->setMerges(attribs[Attributes::Merges]==Attributes::True);
		oper->setHashes(attribs[Attributes::Hashes]==Attributes::True);

		func_types[Attributes::OperatorFunc]=Operator::FuncOperator;
		func_types[Attributes::JoinFunc]=Operator::FuncJoin;
		func_types[Attributes::RestrictionFunc]=Operator::FuncRestrict;

		oper_types[Attributes::CommutatorOp]=Operator::OperCommutator;
		oper_types[Attributes::NegatorOp]=Operator::OperNegator;

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[enum_cast(ObjectType::Operator)])
					{
						xmlparser.getElementAttributes(attribs);
						oper_aux=getObject(attribs[Attributes::Signature], ObjectType::Operator);

						//Raises an error if the auxiliary operator doesn't exists
						if(!oper_aux && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(oper->getSignature(true))
											.arg(oper->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Operator)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setOperator(dynamic_cast<Operator *>(oper_aux),
											oper_types[attribs[Attributes::RefType]]);
					}
					else if(elem==Attributes::Type)
					{
						xmlparser.getElementAttributes(attribs);

						if(attribs[Attributes::RefType]!=Attributes::RightType)
							arg_type=Operator::LeftArg;
						else
							arg_type=Operator::RightArg;

						type=createPgSQLType();
						oper->setArgumentType(type, arg_type);
					}
					else if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(oper->getName())
											.arg(oper->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Function)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setFunction(dynamic_cast<Function *>(func),
											func_types[attribs[Attributes::RefType]]);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(oper) delete oper;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return oper;
}

OperatorClass *DatabaseModel::createOperatorClass()
{
	attribs_map attribs, attribs_aux;
	map<QString, unsigned> elem_types;
	BaseObject *object=nullptr;
	QString elem;
	PgSqlType type;
	OperatorClass *op_class=nullptr;
	OperatorClassElement class_elem;
	unsigned stg_number, elem_type;

	try
	{
		op_class=new OperatorClass;
		setBasicAttributes(op_class);
		xmlparser.getElementAttributes(attribs);

		op_class->setIndexingType(IndexingType(attribs[Attributes::IndexType]));
		op_class->setDefault(attribs[Attributes::Default]==Attributes::True);

		elem_types[Attributes::Function]=OperatorClassElement::FunctionElem;
		elem_types[Attributes::Operator]=OperatorClassElement::OperatorElem;
		elem_types[Attributes::Storage]=OperatorClassElement::StorageElem;

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[enum_cast(ObjectType::OpFamily)])
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[Attributes::Signature], ObjectType::OpFamily);

						//Raises an error if the operator family doesn't exists
						if(!object)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(op_class->getName())
											.arg(op_class->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::OpFamily)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						op_class->setFamily(dynamic_cast<OperatorFamily *>(object));
					}
					else if(elem==Attributes::Type)
					{
						xmlparser.getElementAttributes(attribs);
						type=createPgSQLType();
						op_class->setDataType(type);
					}
					else if(elem==Attributes::Element)
					{
						xmlparser.getElementAttributes(attribs);

						stg_number=attribs[Attributes::StrategyNum].toUInt();
						elem_type=elem_types[attribs[Attributes::Type]];

						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						xmlparser.getElementAttributes(attribs);

						if(elem_type==OperatorClassElement::StorageElem)
						{
							type=createPgSQLType();
							class_elem.setStorage(type);
						}
						else if(elem_type==OperatorClassElement::FunctionElem)
						{
							object=getObject(attribs[Attributes::Signature],ObjectType::Function);
							class_elem.setFunction(dynamic_cast<Function *>(object),stg_number);
						}
						else if(elem_type==OperatorClassElement::OperatorElem)
						{
							object=getObject(attribs[Attributes::Signature],ObjectType::Operator);
							class_elem.setOperator(dynamic_cast<Operator *>(object),stg_number);

							if(xmlparser.hasElement(XmlParser::NextElement))
							{
								xmlparser.savePosition();
								xmlparser.accessElement(XmlParser::NextElement);
								xmlparser.getElementAttributes(attribs_aux);

								object=getObject(attribs_aux[Attributes::Signature],ObjectType::OpFamily);

								if(!object && !attribs_aux[Attributes::Signature].isEmpty())
									throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
													.arg(op_class->getName())
													.arg(op_class->getTypeName())
													.arg(attribs_aux[Attributes::Signature])
										.arg(BaseObject::getTypeName(ObjectType::OpFamily)),
										ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

								class_elem.setOperatorFamily(dynamic_cast<OperatorFamily *>(object));
								xmlparser.restorePosition();
							}
						}

						op_class->addElement(class_elem);
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(op_class) delete op_class;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return op_class;
}

OperatorFamily *DatabaseModel::createOperatorFamily()
{
	attribs_map attribs;
	OperatorFamily *op_family=nullptr;

	try
	{
		op_family=new OperatorFamily;
		setBasicAttributes(op_family);
		xmlparser.getElementAttributes(attribs);
		op_family->setIndexingType(IndexingType(attribs[Attributes::IndexType]));
	}
	catch(Exception &e)
	{
		if(op_family) delete op_family;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return op_family;
}

Aggregate *DatabaseModel::createAggregate()
{
	attribs_map attribs;
	BaseObject *func=nullptr;
	QString elem;
	PgSqlType type;
	Aggregate *aggreg=nullptr;

	try
	{
		aggreg=new Aggregate;
		setBasicAttributes(aggreg);
		xmlparser.getElementAttributes(attribs);

		aggreg->setInitialCondition(attribs[Attributes::InitialCond]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Type)
					{
						xmlparser.getElementAttributes(attribs);
						type=createPgSQLType();

						if(attribs[Attributes::RefType]==Attributes::StateType)
							aggreg->setStateType(type);
						else
							aggreg->addDataType(type);
					}
					else if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[Attributes::Signature].isEmpty())
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(aggreg->getName())
											.arg(aggreg->getTypeName())
											.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Function)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						if(attribs[Attributes::RefType]==Attributes::TransitionFunc)
							aggreg->setFunction(Aggregate::TransitionFunc,
												dynamic_cast<Function *>(func));
						else
							aggreg->setFunction(Aggregate::FinalFunc,
												dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(aggreg) delete aggreg;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return aggreg;
}

Table *DatabaseModel::createTable()
{
	try
	{
		Table *table = nullptr;
		attribs_map attribs;

		xmlparser.getElementAttributes(attribs);
		table = createPhysicalTable<Table>();
		table->setUnlogged(attribs[Attributes::Unlogged]==Attributes::True);
		table->setRLSEnabled(attribs[Attributes::RlsEnabled]==Attributes::True);
		table->setRLSForced(attribs[Attributes::RlsForced]==Attributes::True);
		table->setWithOIDs(attribs[Attributes::Oids]==Attributes::True);

		return table;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Column *DatabaseModel::createColumn()
{
	attribs_map attribs;
	Column *column=nullptr;
	BaseObject *seq=nullptr;
	QString elem;

	try
	{
		column=new Column;
		setBasicAttributes(column);

		xmlparser.getElementAttributes(attribs);
		column->setNotNull(attribs[Attributes::NotNull]==Attributes::True);
		column->setGenerated(attribs[Attributes::Generated]==Attributes::True);
		column->setDefaultValue(attribs[Attributes::DefaultValue]);
		column->setIdSeqAttributes(attribs[Attributes::MinValue], attribs[Attributes::MaxValue], attribs[Attributes::Increment],
																attribs[Attributes::Start], attribs[Attributes::Cache], attribs[Attributes::Cycle] == Attributes::True);

		if(!attribs[Attributes::IdentityType].isEmpty())
			column->setIdentityType(IdentityType(attribs[Attributes::IdentityType]));

		if(!attribs[Attributes::Sequence].isEmpty())
		{
			seq=getObject(attribs[Attributes::Sequence], ObjectType::Sequence);

			if(!seq)
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
								.arg(attribs[Attributes::Name])
					.arg(BaseObject::getTypeName(ObjectType::Column))
					.arg(attribs[Attributes::Sequence])
					.arg(BaseObject::getTypeName(ObjectType::Sequence)),
					ErrorCode::PermissionRefInexistObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);


			column->setSequence(seq);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Type)
					{
						column->setType(createPgSQLType());
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(column) delete column;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return column;
}

Constraint *DatabaseModel::createConstraint(BaseObject *parent_obj)
{
	attribs_map attribs;
	Constraint *constr=nullptr;
	BaseObject *ref_table=nullptr;
	PhysicalTable *table=nullptr,*table_aux=nullptr;
	Column *column=nullptr;
	Relationship *rel=nullptr;
	QString elem, str_aux;
	bool deferrable, ins_constr_table=false;
	ConstraintType constr_type;
	QStringList col_list;
	int count, i;
	unsigned col_type;
	ObjectType obj_type;
	ExcludeElement exc_elem;

	try
	{
		xmlparser.getElementAttributes(attribs);

		//If the constraint parent is allocated
		if(parent_obj)
		{
			obj_type=parent_obj->getObjectType();

			//Identifies the correct parent type
			if(PhysicalTable::isPhysicalTable(obj_type))
				table=dynamic_cast<PhysicalTable *>(parent_obj);
			else if(obj_type==ObjectType::Relationship)
				rel=dynamic_cast<Relationship *>(parent_obj);
			else
				//Raises an error if the user tries to create a constraint in a invalid parent
				throw Exception(ErrorCode::OprObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		else
		{
			obj_type = ObjectType::Table;
			table = dynamic_cast<PhysicalTable *>(getObject(attribs[Attributes::Table], {ObjectType::Table, ObjectType::ForeignTable}));
			parent_obj=table;
			ins_constr_table=true;

			//Raises an error if the parent table doesn't exists
			if(!table)
			{
				str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
						.arg(attribs[Attributes::Name])
						.arg(BaseObject::getTypeName(ObjectType::Constraint))
						.arg(attribs[Attributes::Table])
						.arg(BaseObject::getTypeName(obj_type));

				throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		constr=new Constraint;
		constr->setParentTable(table);

		//Configuring the constraint type
		if(attribs[Attributes::Type]==Attributes::CkConstr)
			constr_type=ConstraintType::Check;
		else if(attribs[Attributes::Type]==Attributes::PkConstr)
			constr_type=ConstraintType::PrimaryKey;
		else if(attribs[Attributes::Type]==Attributes::FkConstr)
			constr_type=ConstraintType::ForeignKey;
		else if(attribs[Attributes::Type]==Attributes::UqConstr)
			constr_type=ConstraintType::Unique;
		else
			constr_type=ConstraintType::Exclude;

		constr->setConstraintType(constr_type);

		if(!attribs[Attributes::Factor].isEmpty())
			constr->setFillFactor(attribs[Attributes::Factor].toUInt());

		setBasicAttributes(constr);

		//Raises an error if the constraint is a primary key and no parent object is specified
		if(!parent_obj && constr_type==ConstraintType::PrimaryKey)
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPrimaryKeyAllocation)
							.arg(constr->getName()),
							ErrorCode::InvPrimaryKeyAllocation,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		deferrable=(attribs[Attributes::Deferrable]==Attributes::True);
		constr->setDeferrable(deferrable);

		if(deferrable && !attribs[Attributes::DeferType].isEmpty())
			constr->setDeferralType(attribs[Attributes::DeferType]);

		if(constr_type==ConstraintType::ForeignKey)
		{
			if(!attribs[Attributes::ComparisonType].isEmpty())
				constr->setMatchType(attribs[Attributes::ComparisonType]);

			if(!attribs[Attributes::DelAction].isEmpty())
				constr->setActionType(attribs[Attributes::DelAction], Constraint::DeleteAction);

			if(!attribs[Attributes::UpdAction].isEmpty())
				constr->setActionType(attribs[Attributes::UpdAction], Constraint::UpdateAction);

			ref_table=getObject(attribs[Attributes::RefTable], ObjectType::Table);

			if(!ref_table && table->getName(true)==attribs[Attributes::RefTable])
				ref_table=table;

			//Raises an error if the referenced table doesn't exists
			if(!ref_table)
			{
				str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
						.arg(constr->getName())
						.arg(constr->getTypeName())
						.arg(attribs[Attributes::RefTable])
						.arg(BaseObject::getTypeName(ObjectType::Table));

				throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			constr->setReferencedTable(dynamic_cast<BaseTable *>(ref_table));
		}
		else if(constr_type==ConstraintType::Check)
		{
			constr->setNoInherit(attribs[Attributes::NoInherit]==Attributes::True);
		}
		else if(constr_type==ConstraintType::Exclude &&	!attribs[Attributes::IndexType].isEmpty())
		{
			constr->setIndexType(attribs[Attributes::IndexType]);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::ExcludeElement)
					{
						createElement(exc_elem, constr, parent_obj);
						constr->addExcludeElement(exc_elem);
					}
					else if(elem==Attributes::Expression)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						constr->setExpression(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Columns)
					{
						xmlparser.getElementAttributes(attribs);

						col_list=attribs[Attributes::Names].split(',');
						count=col_list.count();

						if(attribs[Attributes::RefType]==Attributes::SrcColumns)
							col_type=Constraint::SourceCols;
						else
							col_type=Constraint::ReferencedCols;

						for(i=0; i < count; i++)
						{
							if(col_type==Constraint::SourceCols)
							{
								if(PhysicalTable::isPhysicalTable(obj_type))
								{
									column=table->getColumn(col_list[i]);

									//If the column doesn't exists tries to get it searching by the old name
									if(!column)
										column=table->getColumn(col_list[i], true);
								}
								else
									column=dynamic_cast<Column *>(rel->getObject(col_list[i], ObjectType::Column));
							}
							else
							{
								table_aux=dynamic_cast<PhysicalTable *>(ref_table);
								column=table_aux->getColumn(col_list[i]);

								//If the column doesn't exists tries to get it searching by the old name
								if(!column)
									column=table_aux->getColumn(col_list[i], true);
							}

							constr->addColumn(column, col_type);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		if(ins_constr_table)
		{
			if(constr->getConstraintType()!=ConstraintType::PrimaryKey)
			{
				table->addConstraint(constr);

				if(this->getObjectIndex(table) >= 0)
					table->setModified(!loading_model);
			}
		}
	}
	catch(Exception &e)
	{
		if(constr) delete constr;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return constr;
}

void DatabaseModel::createElement(Element &elem, TableObject *tab_obj, BaseObject *parent_obj)
{
	attribs_map attribs;
	Column *column=nullptr;
	OperatorClass *op_class=nullptr;
	Operator *oper=nullptr;
	Collation *collation=nullptr;
	QString xml_elem;
	bool is_part_key = false;

	xml_elem=xmlparser.getElementName();
	is_part_key = xml_elem == Attributes::PartitionKey;

	if(xml_elem==Attributes::IndexElement || xml_elem==Attributes::ExcludeElement || is_part_key)
	{
		xmlparser.getElementAttributes(attribs);

		elem.setColumn(nullptr);
		elem.setCollation(nullptr);
		elem.setOperator(nullptr);
		elem.setOperatorClass(nullptr);

		elem.setSortingAttribute(Element::AscOrder, attribs[Attributes::AscOrder]==Attributes::True);
		elem.setSortingAttribute(Element::NullsFirst, attribs[Attributes::NullsFirst]==Attributes::True);
		elem.setSortingEnabled(attribs[Attributes::UseSorting]!=Attributes::False);

		xmlparser.savePosition();
		xmlparser.accessElement(XmlParser::ChildElement);

		do
		{
			xml_elem=xmlparser.getElementName();

			if(xmlparser.getElementType()==XML_ELEMENT_NODE)
			{
				if(xml_elem==Attributes::OpClass)
				{
					xmlparser.getElementAttributes(attribs);
					op_class=dynamic_cast<OperatorClass *>(getObject(attribs[Attributes::Signature], ObjectType::OpClass));

					//Raises an error if the operator class doesn't exists
					if(!op_class)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(tab_obj->getName())
											.arg(tab_obj->getTypeName())
											.arg(attribs[Attributes::Signature])
									.arg(BaseObject::getTypeName(ObjectType::OpClass)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::PartKeyObjectInexistsModel)
											.arg(parent_obj->getName())
											.arg(attribs[Attributes::Signature])
									.arg(BaseObject::getTypeName(ObjectType::OpClass)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setOperatorClass(op_class);
				}
				//Checking if elem is a ExcludeElement to be able to assign an operator to it
				else if(xml_elem==Attributes::Operator)
				{
					xmlparser.getElementAttributes(attribs);
					oper=dynamic_cast<Operator *>(getObject(attribs[Attributes::Signature], ObjectType::Operator));

					//Raises an error if the operator doesn't exists
					if(!oper)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
										.arg(tab_obj->getName())
										.arg(tab_obj->getTypeName())
										.arg(attribs[Attributes::Signature])
								.arg(BaseObject::getTypeName(ObjectType::Operator)),
								ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setOperator(oper);
				}
				else if(xml_elem==Attributes::Collation)
				{
					xmlparser.getElementAttributes(attribs);
					collation=dynamic_cast<Collation *>(getObject(attribs[Attributes::Name], ObjectType::Collation));

					//Raises an error if the operator class doesn't exists
					if(!collation)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(tab_obj->getName())
															.arg(tab_obj->getTypeName())
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(ObjectType::Collation)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::PartKeyObjectInexistsModel)
															.arg(parent_obj->getName())
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(ObjectType::Collation)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setCollation(collation);
				}
				else if(xml_elem==Attributes::Column)
				{
					xmlparser.getElementAttributes(attribs);

					if(parent_obj->getObjectType()==ObjectType::Table)
					{
						column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[Attributes::Name]);

						if(!column)
							column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[Attributes::Name], true);
					}
					else
					{
						column=dynamic_cast<Column *>(dynamic_cast<Relationship *>(parent_obj)->getObject(attribs[Attributes::Name], ObjectType::Column));
					}

					//Raises an error if the column doesn't exists
					if(!column)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(tab_obj->getName())
											.arg(tab_obj->getTypeName())
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Column)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::PartKeyObjectInexistsModel)
											.arg(parent_obj->getName())
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Column)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setColumn(column);
				}
				else if(xml_elem==Attributes::Expression)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					elem.setExpression(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
			}
		}
		while(xmlparser.accessElement(XmlParser::NextElement));

		xmlparser.restorePosition();
	}
}

XmlParser *DatabaseModel::getXMLParser()
{
	return &xmlparser;
}

QString DatabaseModel::getAlterDefinition(BaseObject *object)
{
	DatabaseModel *db_aux=dynamic_cast<DatabaseModel *>(object);

	if(!db_aux)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QString alter_def;
		attribs_map aux_attribs;

		aux_attribs[Attributes::Signature] = this->getSignature();
		aux_attribs[Attributes::SqlObject] = this->getSQLName();

		if(this->conn_limit!=db_aux->conn_limit)
			aux_attribs[Attributes::ConnLimit]=QString::number(db_aux->conn_limit);

		if(this->is_template != db_aux->is_template)
			aux_attribs[Attributes::IsTemplate] = (db_aux->is_template ? Attributes::True : Attributes::False);

		if(this->allow_conns != db_aux->allow_conns)
			aux_attribs[Attributes::AllowConns] = (db_aux->allow_conns ? Attributes::True : Attributes::False);

		alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), aux_attribs, true, false);
		alter_def+=BaseObject::getAlterDefinition(object);

		return alter_def;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Index *DatabaseModel::createIndex()
{
	attribs_map attribs;
	Index *index=nullptr;
	QString elem, str_aux;
	IndexElement idx_elem;
	BaseTable *table=nullptr;

	try
	{
		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], ObjectType::Table));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], ObjectType::View));

		//Raises an error if the parent table doesn't exists
		if(!table)
		{
			str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
					.arg(attribs[Attributes::Name])
					.arg(BaseObject::getTypeName(ObjectType::Index))
					.arg(attribs[Attributes::Table])
					.arg(BaseObject::getTypeName(ObjectType::Table));

			throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		index=new Index;
		setBasicAttributes(index);
		index->setParentTable(table);
		index->setIndexAttribute(Index::Concurrent, attribs[Attributes::Concurrent]==Attributes::True);
		index->setIndexAttribute(Index::Unique, attribs[Attributes::Unique]==Attributes::True);
		index->setIndexAttribute(Index::FastUpdate, attribs[Attributes::FastUpdate]==Attributes::True);
		index->setIndexAttribute(Index::Buffering, attribs[Attributes::Buffering]==Attributes::True);
		index->setIndexingType(attribs[Attributes::IndexType]);
		index->setFillFactor(attribs[Attributes::Factor].toUInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::IndexElement)
					{
						createElement(idx_elem, index, table);
						index->addIndexElement(idx_elem);
					}
					else if(elem==Attributes::Predicate)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();
						index->setPredicate(str_aux);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(index);
		table->setModified(!loading_model);
	}
	catch(Exception &e)
	{
		if(index) delete index;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return index;
}

Rule *DatabaseModel::createRule()
{
	attribs_map attribs;
	QStringList cmd_list;
	Rule *rule=nullptr;
	QString elem, str_aux;
	int count, i;
	BaseTable *table=nullptr;

	try
	{
		rule=new Rule;
		setBasicAttributes(rule);

		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], ObjectType::Table));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], ObjectType::View));

		if(!table)
			throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
							.arg(attribs[Attributes::Name])
				.arg(BaseObject::getTypeName(ObjectType::Rule))
				.arg(attribs[Attributes::Table])
				.arg(BaseObject::getTypeName(ObjectType::Table)),
				ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		rule->setExecutionType(attribs[Attributes::ExecType]);
		rule->setEventType(attribs[Attributes::EventType]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Commands ||
							elem==Attributes::Condition)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();

						if(elem==Attributes::Commands)
						{
							cmd_list=str_aux.split(';');
							count=cmd_list.count();
							for(i=0; i < count; i++)
							{
								if(!cmd_list[i].isEmpty())
									rule->addCommand(cmd_list[i]);
							}
						}
						else
							rule->setConditionalExpression(str_aux);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(rule);
		table->setModified(!loading_model);
	}
	catch(Exception &e)
	{
		if(rule) delete rule;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return rule;
}

Trigger *DatabaseModel::createTrigger()
{
	attribs_map attribs;
	Trigger *trigger=nullptr;
	QString elem, str_aux;
	QStringList list_aux;
	int count, i;
	BaseObject *ref_table=nullptr, *func=nullptr;
	Column *column=nullptr;
	BaseTable *table=nullptr;
	PhysicalTable *aux_table = nullptr;
	vector<ObjectType> table_types = { ObjectType::Table, ObjectType::ForeignTable, ObjectType::View };

	try
	{
		xmlparser.getElementAttributes(attribs);

		for(auto &type : table_types)
		{
			table = dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], type));
			if(table) break;
		}

		if(!table)
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
							.arg(attribs[Attributes::Name])
				.arg(BaseObject::getTypeName(ObjectType::Trigger))
				.arg(attribs[Attributes::Table])
				.arg(BaseObject::getTypeName(ObjectType::Table)),
				ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		trigger=new Trigger;
		trigger->setParentTable(table);

		setBasicAttributes(trigger);

		trigger->setConstraint(attribs[Attributes::Constraint]==Attributes::True);

		trigger->setEvent(EventType::OnInsert,
							(attribs[Attributes::InsEvent]==Attributes::True));

		trigger->setEvent(EventType::OnDelete,
							(attribs[Attributes::DelEvent]==Attributes::True));

		trigger->setEvent(EventType::OnUpdate,
							(attribs[Attributes::UpdEvent]==Attributes::True));

		trigger->setEvent(EventType::OnTruncate,
							(attribs[Attributes::TruncEvent]==Attributes::True));

		trigger->setExecutePerRow(attribs[Attributes::PerRow]==Attributes::True);

		trigger->setFiringType(FiringType(attribs[Attributes::FiringType]));

		trigger->setTransitionTableName(Trigger::OldTableName, attribs[Attributes::OldTableName]);
		trigger->setTransitionTableName(Trigger::NewTableName, attribs[Attributes::NewTableName]);

		list_aux=attribs[Attributes::Arguments].split(',');
		count=list_aux.count();
		for(i=0; i < count; i++)
		{
			if(!list_aux[i].isEmpty())
				trigger->addArgument(list_aux[i]);
		}

		trigger->setDeferrable(attribs[Attributes::Deferrable]==Attributes::True);

		if(trigger->isDeferrable())
			trigger->setDeferralType(attribs[Attributes::DeferType]);

		if(!attribs[Attributes::RefTable].isEmpty())
		{
			ref_table=getObject(attribs[Attributes::RefTable], ObjectType::Table);

			if(!ref_table)
				ref_table=getObject(attribs[Attributes::RefTable], ObjectType::View);

			//Raises an error if the trigger is referencing a inexistent table
			if(!ref_table)
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
								.arg(trigger->getName())
								.arg(trigger->getTypeName())
								.arg(attribs[Attributes::RefTable])
						.arg(BaseObject::getTypeName(ObjectType::Table)),
						ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			trigger->setReferecendTable(dynamic_cast<BaseTable *>(ref_table));
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[Attributes::Signature].isEmpty())
						{
							str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
									.arg(trigger->getName())
									.arg(trigger->getTypeName())
									.arg(attribs[Attributes::Signature])
									.arg(BaseObject::getTypeName(ObjectType::Function));

							throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						trigger->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==Attributes::Condition)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();
						trigger->setCondition(str_aux);
					}
					else if(elem==Attributes::Columns)
					{
						xmlparser.getElementAttributes(attribs);

						list_aux=attribs[Attributes::Names].split(',');
						count=list_aux.count();

						for(i=0; i < count; i++)
						{
							column=dynamic_cast<Column *>(table->getObject(list_aux[i].trimmed(), ObjectType::Column));
							aux_table=dynamic_cast<PhysicalTable *>(table);

							if(!column && aux_table)
								column=aux_table->getColumn(list_aux[i].trimmed(), true);

							trigger->addColumn(column);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(trigger);
		table->setModified(!loading_model);
	}
	catch(Exception &e)
	{
		if(trigger) delete trigger;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return trigger;
}

Policy *DatabaseModel::createPolicy()
{
	attribs_map attribs;
	Policy *policy=nullptr;
	QString elem;
	BaseTable *table=nullptr;

	try
	{
		policy=new Policy;
		setBasicAttributes(policy);

		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::Table], ObjectType::Table));

		if(!table)
			throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(attribs[Attributes::Name])
											.arg(BaseObject::getTypeName(ObjectType::Policy))
											.arg(attribs[Attributes::Table])
											.arg(BaseObject::getTypeName(ObjectType::Table)),
				ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		policy->setPermissive(attribs[Attributes::Permissive] == Attributes::True);
		policy->setPolicyCommand(PolicyCmdType(attribs[Attributes::Command]));

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Expression)
					{
						xmlparser.getElementAttributes(attribs);
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						if(attribs[Attributes::Type] == Attributes::UsingExp)
							policy->setUsingExpression(xmlparser.getElementContent());
						else if(attribs[Attributes::Type] == Attributes::CheckExp)
							policy->setCheckExpression(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
					else if(xmlparser.getElementName()==Attributes::Roles)
					{
						QStringList rol_names;
						Role *role = nullptr;

						xmlparser.getElementAttributes(attribs);

						rol_names = attribs[Attributes::Names].split(',');

						for(auto &name : rol_names)
						{
							role=dynamic_cast<Role *>(getObject(name.trimmed(), ObjectType::Role));

							//Raises an error if the referenced role doesn't exists
							if(!role)
							{
								throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
																.arg(policy->getName())
																.arg(policy->getTypeName())
																.arg(name)
																.arg(BaseObject::getTypeName(ObjectType::Role)),
																ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							policy->addRole(role);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(policy);
		table->setModified(!loading_model);
	}
	catch(Exception &e)
	{
		if(policy) delete policy;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return policy;
}

EventTrigger *DatabaseModel::createEventTrigger()
{
	attribs_map attribs;
	EventTrigger *event_trig=nullptr;
	BaseObject *func=nullptr;
	QString elem;

	try
	{
		event_trig=new EventTrigger;
		setBasicAttributes(event_trig);
		xmlparser.getElementAttributes(attribs);
		event_trig->setEvent(EventTriggerType(attribs[Attributes::Event]));

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Function)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[Attributes::Signature], ObjectType::Function);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[Attributes::Signature].isEmpty())
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(event_trig->getName())
											.arg(event_trig->getTypeName())
											.arg(attribs[Attributes::Signature])
									.arg(BaseObject::getTypeName(ObjectType::Function)),
									ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						event_trig->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==Attributes::Filter)
					{
						xmlparser.getElementAttributes(attribs);
						event_trig->setFilter(attribs[Attributes::Variable], attribs[Attributes::Values].split(','));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

	}
	catch(Exception &e)
	{
		if(event_trig) delete event_trig;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return event_trig;
}

GenericSQL *DatabaseModel::createGenericSQL()
{
	GenericSQL *genericsql=nullptr;
	attribs_map attribs;
	QString elem, parent_name, obj_name;
	ObjectType obj_type;
	PhysicalTable *parent_table = nullptr;
	BaseObject *object = nullptr;

	try
	{
		genericsql = new GenericSQL;
		setBasicAttributes(genericsql);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem = xmlparser.getElementName();

					if(elem == Attributes::Definition)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						genericsql->setDefinition(xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
					else if(elem == Attributes::Object)
					{
						xmlparser.getElementAttributes(attribs);

						obj_type = BaseObject::getObjectType(attribs[Attributes::Type]);
						obj_name = attribs[Attributes::Name];

						//If the object is a column its needed to get the parent table
						if(obj_type == ObjectType::Column)
						{
							QStringList names = obj_name.split('.');

							if(names.size() > 2)
							{
								parent_name = QString("%1.%2").arg(names[0]).arg(names[1]);
								obj_name = names[2];
							}

							parent_table = dynamic_cast<PhysicalTable *>(getObject(parent_name, {ObjectType::Table, ObjectType::ForeignTable}));

							if(parent_table)
								object = parent_table->getColumn(obj_name);
						}
						else
							object = getObject(obj_name, obj_type);

						//Raises an error if the generic object references an object that does not exist
						if(!object)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(genericsql->getName())
															.arg(genericsql->getTypeName())
															.arg(obj_name)
															.arg(BaseObject::getTypeName(obj_type)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						genericsql->addObjectReference(object, attribs[Attributes::RefName],
																					 attribs[Attributes::UseSignature] == Attributes::True,
																					 attribs[Attributes::FormatName] == Attributes::True);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(genericsql) delete genericsql;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return genericsql;
}

ForeignDataWrapper *DatabaseModel::createForeignDataWrapper()
{
	attribs_map attribs;
	ForeignDataWrapper *fdw=nullptr;
	BaseObject *func=nullptr;
	QString signature, ref_type;
	ObjectType obj_type;

	try
	{
		fdw = new ForeignDataWrapper;

		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(fdw);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType() == XML_ELEMENT_NODE)
				{
					obj_type = BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type == ObjectType::Function)
					{
						xmlparser.getElementAttributes(attribs);

						//Gets the function reference type
						ref_type = attribs[Attributes::RefType];

						//Try to retrieve one the functions of the fdw
						if(ref_type == Attributes::ValidatorFunc || ref_type == Attributes::HandlerFunc)
						{
							//Gets the function signature and tries to retrieve it from the model
							signature = attribs[Attributes::Signature];
							func = getObject(signature, ObjectType::Function);

							//Raises an error if the function doesn't exists
							if(!func)
								throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
												.arg(fdw->getName())
												.arg(fdw->getTypeName())
												.arg(signature)
												.arg(BaseObject::getTypeName(ObjectType::Function)),
												ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

							if(ref_type == Attributes::ValidatorFunc)
								fdw->setValidatorFunction(dynamic_cast<Function *>(func));
							else if(ref_type == Attributes::HandlerFunc)
								fdw->setHandlerFunction(dynamic_cast<Function *>(func));
						}
						else
							//Raises an error if the function type is invalid
							throw Exception(ErrorCode::RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(fdw)
			delete fdw;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return fdw;
}

ForeignServer *DatabaseModel::createForeignServer()
{
	attribs_map attribs;
	ForeignServer *server = nullptr;
	BaseObject *fdw = nullptr;
	ObjectType obj_type;

	try
	{
		server = new ForeignServer;

		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(server);
		server->setType(attribs[Attributes::Type]);
		server->setVersion(attribs[Attributes::Version]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType() == XML_ELEMENT_NODE)
				{
					obj_type = BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type == ObjectType::ForeignDataWrapper)
					{
						xmlparser.getElementAttributes(attribs);
						fdw = getObject(attribs[Attributes::Name], ObjectType::ForeignDataWrapper);

						//Raises an error if the fdw doesn't exists
						if(!fdw)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(server->getName())
															.arg(server->getTypeName())
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(ObjectType::ForeignDataWrapper)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						server->setForeignDataWrapper(dynamic_cast<ForeignDataWrapper *>(fdw));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(server)
			delete server;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return server;
}

UserMapping *DatabaseModel::createUserMapping()
{
	attribs_map attribs;
	UserMapping *user_map = nullptr;
	ForeignServer *server = nullptr;
	ObjectType obj_type;

	try
	{
		user_map = new UserMapping;

		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(user_map);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType() == XML_ELEMENT_NODE)
				{
					obj_type = BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type == ObjectType::ForeignServer)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						server = dynamic_cast<ForeignServer *>(getObject(attribs[Attributes::Name], ObjectType::ForeignServer));

						//Raises an error if the server doesn't exists
						if(!server)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(user_map->getName())
															.arg(user_map->getTypeName())
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(ObjectType::ForeignServer)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						user_map->setForeignServer(server);
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(user_map)
			delete user_map;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return user_map;
}

ForeignTable *DatabaseModel::createForeignTable()
{
	ForeignTable *ftable = nullptr;

	try
	{
		ForeignServer *fserver = nullptr;
		attribs_map attribs;
		ObjectType obj_type;

		xmlparser.savePosition();
		ftable = createPhysicalTable<ForeignTable>();
		xmlparser.restorePosition();

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType() == XML_ELEMENT_NODE)
				{
					obj_type = BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type == ObjectType::ForeignServer)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						fserver = dynamic_cast<ForeignServer *>(getObject(attribs[Attributes::Name], ObjectType::ForeignServer));

						//Raises an error if the server doesn't exists
						if(!fserver)
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(ftable->getName())
															.arg(ftable->getTypeName())
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(ObjectType::ForeignServer)),
															ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						ftable->setForeignServer(fserver);
						xmlparser.restorePosition();
						break;
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		return ftable;
	}
	catch(Exception &e)
	{
		if(ftable) delete ftable;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::updateViewsReferencingTable(PhysicalTable *table)
{
	BaseRelationship *rel = nullptr;
	View *view = nullptr;
	PhysicalTable *tab = nullptr;

	if(!table) return;

	for(auto obj : base_relationships)
	{
		rel = dynamic_cast<BaseRelationship *>(obj);

		if(rel->getRelationshipType() != BaseRelationship::RelationshipDep)
			continue;

		view = dynamic_cast<View *>(rel->getTable(BaseRelationship::SrcTable));
		tab = dynamic_cast<PhysicalTable *>(rel->getTable(BaseRelationship::DstTable));

		if(view && tab == table)
		{
			view->generateColumns();
			view->setCodeInvalidated(true);
			view->setModified(true);
			dynamic_cast<Schema *>(view->getSchema())->setModified(true);
		}
	}
}

Sequence *DatabaseModel::createSequence(bool ignore_onwer)
{
	attribs_map attribs;
	Sequence *sequence=nullptr;
	BaseObject *table=nullptr;
	Column *column=nullptr;
	QString str_aux, tab_name, col_name;
	QStringList elem_list;
	int count;

	try
	{
		sequence=new Sequence;
		setBasicAttributes(sequence);
		xmlparser.getElementAttributes(attribs);

		sequence->setValues(attribs[Attributes::MinValue],
				attribs[Attributes::MaxValue],
				attribs[Attributes::Increment],
				attribs[Attributes::Start],
				attribs[Attributes::Cache]);

		sequence->setCycle(attribs[Attributes::Cycle]==Attributes::True);

		//Getting the sequence's owner column
		if(!attribs[Attributes::OwnerColumn].isEmpty())
		{
			elem_list=attribs[Attributes::OwnerColumn].split('.');
			count=elem_list.count();

			if(count==3)
			{
				tab_name=elem_list[0] + QString(".") + elem_list[1];
				col_name=elem_list[2];
			}
			else if(count==2)
			{
				tab_name=elem_list[0];
				col_name=elem_list[1];
			}

			table=getObject(tab_name, {ObjectType::Table, ObjectType::ForeignTable});

			//Raises an error if the column parent table doesn't exists
			if(!table)
			{
				str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
						.arg(sequence->getName())
						.arg(BaseObject::getTypeName(ObjectType::Sequence))
						.arg(tab_name)
						.arg(BaseObject::getTypeName(ObjectType::Table));

				throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column=dynamic_cast<PhysicalTable *>(table)->getColumn(col_name);

			if(!column)
				column=dynamic_cast<PhysicalTable *>(table)->getColumn(col_name, true);

			//Raises an error if the column doesn't exists
			if(!column && !ignore_onwer)
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgInexistentSeqOwnerColumn)
								.arg(sequence->getName(true)),
								ErrorCode::AsgInexistentSeqOwnerColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			sequence->setOwnerColumn(column);
		}
	}
	catch(Exception &e)
	{
		if(sequence) delete sequence;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return sequence;
}

View *DatabaseModel::createView()
{
	attribs_map attribs, aux_attribs;
	View *view=nullptr;
	Column *column=nullptr;
	PhysicalTable *table=nullptr;
	QString elem, str_aux;
	QStringList list_aux;
	vector<Reference> refs;
	BaseObject *tag=nullptr;
	unsigned type;
	int ref_idx, i, count;
	bool refs_in_expr=false;
	Reference reference;

	try
	{
		view=new View;
		setBasicAttributes(view);

		xmlparser.getElementAttributes(attribs);
		view->setObjectListsCapacity(attribs[Attributes::MaxObjCount].toUInt());
		view->setMaterialized(attribs[Attributes::Materialized]==Attributes::True);
		view->setRecursive(attribs[Attributes::Recursive]==Attributes::True);
		view->setWithNoData(attribs[Attributes::WithNoData]==Attributes::True);
		view->setCollapseMode(attribs[Attributes::CollapseMode].isEmpty() ? CollapseMode::NotCollapsed : static_cast<CollapseMode>(attribs[Attributes::CollapseMode].toUInt()));
		view->setPaginationEnabled(attribs[Attributes::Pagination]==Attributes::True);
		view->setCurrentPage(BaseTable::AttribsSection, attribs[Attributes::AttribsPage].toUInt());
		view->setCurrentPage(BaseTable::ExtAttribsSection, attribs[Attributes::ExtAttribsPage].toUInt());
		view->setFadedOut(attribs[Attributes::FadedOut]==Attributes::True);
		view->setLayer(attribs[Attributes::Layer].toUInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Reference)
					{
						xmlparser.getElementAttributes(attribs);

						//If the table name is specified tries to create a reference to a table/column
						if(!attribs[Attributes::Table].isEmpty())
						{
							column=nullptr;
							table=dynamic_cast<PhysicalTable *>(getObject(attribs[Attributes::Table], {ObjectType::Table, ObjectType::ForeignTable}));

							//Raises an error if the table doesn't exists
							if(!table)
							{
								str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
										.arg(view->getName())
										.arg(BaseObject::getTypeName(ObjectType::View))
										.arg(attribs[Attributes::Table])
										.arg(BaseObject::getTypeName(ObjectType::Table));

								throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							if(!attribs[Attributes::Column].isEmpty())
							{
								column=table->getColumn(attribs[Attributes::Column]);

								if(!column)
									column=table->getColumn(attribs[Attributes::Column], true);

								//Raises an error if the view references an inexistant column
								if(!column)
								{
									str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(view->getName())
											.arg(BaseObject::getTypeName(ObjectType::View))
											.arg(attribs[Attributes::Table] + QString(".") +
											attribs[Attributes::Column])
											.arg(BaseObject::getTypeName(ObjectType::Column));

									throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
							}

							//Adds the configured reference to a temporarily list
							reference = Reference(table, column,
																		attribs[Attributes::Alias],
																		attribs[Attributes::ColumnAlias]);
							reference.setReferenceAlias(attribs[Attributes::RefAlias]);
							refs.push_back(reference);
						}
						else
						{
							xmlparser.savePosition();
							str_aux=attribs[Attributes::Alias];

							// Retrieving the reference expression
							xmlparser.accessElement(XmlParser::ChildElement);
							xmlparser.savePosition();
							xmlparser.accessElement(XmlParser::ChildElement);
							reference = Reference(xmlparser.getElementContent(), str_aux);
							reference.setReferenceAlias(attribs[Attributes::RefAlias]);
							xmlparser.restorePosition();

							// Creating the columns related to the expression
							if(xmlparser.accessElement(XmlParser::NextElement))
							{
								do
								{
									elem = xmlparser.getElementName();
									xmlparser.savePosition();

									if(elem == Attributes::Column)
									{
										column = createColumn();
										reference.addColumn(column);
										delete column;
									}
									else if(elem == Attributes::RefTableTag)
									{
										xmlparser.getElementAttributes(aux_attribs);
										table = getTable(aux_attribs[Attributes::Name]);

										if(!table)
											table = getForeignTable(aux_attribs[Attributes::Name]);

										reference.addReferencedTable(table);
									}

									xmlparser.restorePosition();
								}
								while(xmlparser.accessElement(XmlParser::NextElement));
							}

							refs.push_back(reference);
							xmlparser.restorePosition();
						}
					}
					else if(elem==Attributes::Expression)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						xmlparser.accessElement(XmlParser::ChildElement);

						if(attribs[Attributes::Type]==Attributes::CteExpression)
							view->setCommomTableExpression(xmlparser.getElementContent());
						else
						{
							if(attribs[Attributes::Type]==Attributes::SelectExp)
								type=Reference::SqlReferSelect;
							else if(attribs[Attributes::Type]==Attributes::FromExp)
								type=Reference::SqlReferFrom;
							else if(attribs[Attributes::Type]==Attributes::SimpleExp)
								type=Reference::SqlReferWhere;
							else
								type=Reference::SqlReferEndExpr;

							list_aux=xmlparser.getElementContent().split(',');
							count=list_aux.size();

							//Indicates that some of the references were used in the expressions
							if(count > 0 && !refs_in_expr)
								refs_in_expr=true;

							for(i=0; i < count; i++)
							{
								ref_idx=list_aux[i].toInt();
								view->addReference(refs[ref_idx],type);
							}
						}

						xmlparser.restorePosition();
					}
					else if(elem==BaseObject::getSchemaName(ObjectType::Tag))
					{
						xmlparser.getElementAttributes(aux_attribs);
						tag=getObject(aux_attribs[Attributes::Name] ,ObjectType::Tag);

						if(!tag)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
											.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Table))
									.arg(aux_attribs[Attributes::Table])
									.arg(BaseObject::getTypeName(ObjectType::Tag))
									, ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						view->setTag(dynamic_cast<Tag *>(tag));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		/** Special case for refereces used as view definition **

			If the flag 'refs_in_expr' isn't set indicates that the user defined a reference
	  but has no used to define the view declaration, this way pgModeler will consider these
	  references as View definition expressions and will force the insertion of them as
			Reference::SQL_VIEW_DEFINITION.

		This process can raise errors because if the user defined more than one reference the view
		cannot accept the two as it's SQL definition, also the defined references MUST be expressions in
		order to be used as view definition */
		if(!refs.empty() && !refs_in_expr)
		{
			vector<Reference>::iterator itr;

			itr=refs.begin();
			while(itr!=refs.end())
			{
				view->addReference(*itr, Reference::SqlViewDefinition);
				itr++;
			}
		}
	}
	catch(Exception &e)
	{
		if(view) delete view;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return view;
}

Collation *DatabaseModel::createCollation()
{
	Collation *collation=nullptr;
	BaseObject *copy_coll=nullptr;
	EncodingType encoding;
	attribs_map attribs;

	try
	{
		collation=new Collation;
		setBasicAttributes(collation);

		xmlparser.getElementAttributes(attribs);

		encoding=EncodingType(attribs[Attributes::Encoding]);
		collation->setEncoding(encoding);

		//Creating a collation from a base locale
		if(!attribs[Attributes::Locale].isEmpty())
			collation->setLocale(attribs[Attributes::Locale]);
		//Creating a collation from another collation
		else if(!attribs[Attributes::Collation].isEmpty())
		{
			copy_coll=this->getObject(attribs[Attributes::Collation], ObjectType::Collation);

			//Raises an error if the copy collation doesn't exists
			if(!copy_coll)
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
								.arg(collation->getName())
								.arg(BaseObject::getTypeName(ObjectType::Collation))
								.arg(attribs[Attributes::Collation])
						.arg(BaseObject::getTypeName(ObjectType::Collation)),
						ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			collation->setCollation(dynamic_cast<Collation *>(copy_coll));
		}
		//Creating a collation using LC_COLLATE and LC_CTYPE params
		else
		{
			collation->setLocalization(Collation::LcCollate, attribs[Attributes::LcCollate]);
			collation->setLocalization(Collation::LcCtype, attribs[Attributes::LcCtype]);
		}
	}
	catch(Exception &e)
	{
		if(collation) delete collation;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return collation;
}

Extension *DatabaseModel::createExtension()
{
	Extension *extension=nullptr;
	attribs_map attribs;

	try
	{
		extension=new Extension;
		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(extension);

		extension->setHandlesType(attribs[Attributes::HandlesType]==Attributes::True);
		extension->setVersion(Extension::CurVersion, attribs[Attributes::CurVersion]);
		extension->setVersion(Extension::OldVersion, attribs[Attributes::OldVersion]);
	}
	catch(Exception &e)
	{
		if(extension) delete extension;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return extension;
}

Tag *DatabaseModel::createTag()
{
	Tag *tag=nullptr;
	attribs_map attribs;
	QString elem;

	try
	{
		tag=new Tag;
		setBasicAttributes(tag);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==Attributes::Style)
					{
						xmlparser.getElementAttributes(attribs);
						tag->setElementColors(attribs[Attributes::Id],attribs[Attributes::Colors]);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		return tag;
	}
	catch(Exception &e)
	{
		if(tag) delete tag;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}
}

Textbox *DatabaseModel::createTextbox()
{
	Textbox *txtbox=nullptr;
	attribs_map attribs;

	try
	{
		txtbox=new Textbox;
		setBasicAttributes(txtbox);

		xmlparser.getElementAttributes(attribs);

		txtbox->setFadedOut(attribs[Attributes::FadedOut]==Attributes::True);
		txtbox->setLayer(attribs[Attributes::Layer].toUInt());
		txtbox->setTextAttribute(Textbox::ItalicText, attribs[Attributes::Italic]==Attributes::True);
		txtbox->setTextAttribute(Textbox::BoldText, attribs[Attributes::Bold]==Attributes::True);
		txtbox->setTextAttribute(Textbox::UnderlineText, attribs[Attributes::Underline]==Attributes::True);

		if(!attribs[Attributes::Color].isEmpty())
			txtbox->setTextColor(QColor(attribs[Attributes::Color]));

		if(!attribs[Attributes::FontSize].isEmpty())
			txtbox->setFontSize(attribs[Attributes::FontSize].toDouble());
	}
	catch(Exception &e)
	{
		if(txtbox) delete txtbox;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return txtbox;
}

BaseRelationship *DatabaseModel::createRelationship()
{
	vector<unsigned> cols_special_pk;
	attribs_map attribs, constr_attribs;
	map<QString, unsigned> labels_id;
	BaseRelationship *base_rel=nullptr;
	Relationship *rel=nullptr;
	BaseTable *tables[2]={nullptr, nullptr};
	bool src_mand, dst_mand, identifier, protect, deferrable, sql_disabled, single_pk_col, faded_out;
	DeferralType defer_type;
	ActionType del_action, upd_action;
	unsigned rel_type=0, i = 0, layer = 0;
	ObjectType table_types[2]={ ObjectType::View, ObjectType::Table }, obj_rel_type;
	QString str_aux, elem, tab_attribs[2]={ Attributes::SrcTable, Attributes::DstTable };
	QColor custom_color=Qt::transparent;
	Table *table = nullptr;

	try
	{
		labels_id[Attributes::NameLabel]=BaseRelationship::RelNameLabel;
		labels_id[Attributes::SrcLabel]=BaseRelationship::SrcCardLabel;
		labels_id[Attributes::DstLabel]=BaseRelationship::DstCardLabel;

		xmlparser.getElementAttributes(attribs);

		src_mand=attribs[Attributes::SrcRequired]==Attributes::True;
		dst_mand=attribs[Attributes::DstRequired]==Attributes::True;
		protect=(attribs[Attributes::Protected]==Attributes::True);
		faded_out=(attribs[Attributes::FadedOut]==Attributes::True);
		layer = attribs[Attributes::Layer].toUInt();

		if(!attribs[Attributes::CustomColor].isEmpty())
			custom_color=QColor(attribs[Attributes::CustomColor]);

		if(attribs[Attributes::Type]!=Attributes::RelationshipTabView &&
				attribs[Attributes::Type]!=Attributes::RelationshipFk)
		{
			table_types[0]=ObjectType::Table;
			obj_rel_type=ObjectType::Relationship;
		}
		else
		{
			if(attribs[Attributes::Type]==Attributes::RelationshipFk)
				table_types[0]=ObjectType::Table;

			obj_rel_type=ObjectType::BaseRelationship;
		}

		//Gets the participant tables
		for(i=0; i < 2; i++)
		{
			tables[i]=dynamic_cast<BaseTable *>(getObject(attribs[tab_attribs[i]], table_types[i]));

			/* If the relationship is between a view and a table and the table is not found
			 * we try to find a foreign table instead */
			if(table_types[i] == ObjectType::Table && !tables[i])
				tables[i]=dynamic_cast<BaseTable *>(getObject(attribs[tab_attribs[i]], ObjectType::ForeignTable));

			//Raises an error if some table doesn't exists
			if(!tables[i])
			{
				str_aux=Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
						.arg(attribs[Attributes::Name])
						.arg(BaseObject::getTypeName(obj_rel_type))
						.arg(attribs[tab_attribs[i]])
						.arg(BaseObject::getTypeName(table_types[i]));

				throw Exception(str_aux,ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		if(obj_rel_type==ObjectType::BaseRelationship)
		{
			base_rel=getRelationship(tables[0], tables[1]);

			/* Creates the fk relationship if it not exists. This generally happens when a foreign key is
			added to the table after its creation. */
			if(attribs[Attributes::Type]==Attributes::RelationshipFk)
			{
				base_rel=new BaseRelationship(BaseRelationship::RelationshipFk, tables[0], tables[1], src_mand, dst_mand);
				base_rel->setName(attribs[Attributes::Name]);
				base_rel->setAlias(attribs[Attributes::Alias]);
				addRelationship(base_rel);

				/* If the source table doesn't have any fk that references the destination table indicates that the relationship
				is being created before the fk that represents it or the fk is invalid (inconsistence!). In this case an error is raised. */
				if(base_rel->getRelationshipType() == BaseRelationship::RelationshipFk &&
				   !base_rel->getReferenceForeignKey())
				{
					Table *src_tab = dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SrcTable)),
							*dst_tab = dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::DstTable));
					vector<Constraint *> fks;

					src_tab->getForeignKeys(fks, false, dst_tab);

					for(auto fk : fks)
					{
						if(!getRelationship(src_tab, dst_tab, fk))
						{
							base_rel->setReferenceForeignKey(fk);
							break;
						}
					}

					//Throws an error if the relationship was created without a valid foreign key attached to it
					if(!base_rel->getReferenceForeignKey())
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvAllocationFKRelationship)
										.arg(attribs[Attributes::Name])
									  .arg(src_tab->getName(true)),
										ErrorCode::InvAllocationFKRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			else if(base_rel)
				base_rel->setName(attribs[Attributes::Name]);

			if(!base_rel)
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
								.arg(this->getName())
								.arg(this->getTypeName())
								.arg(attribs[Attributes::Name])
					.arg(BaseObject::getTypeName(ObjectType::BaseRelationship)),
					ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			base_rel->blockSignals(loading_model);
			base_rel->disconnectRelationship();
			base_rel->blockSignals(false);
		}
		else
		{
			QString pat_attrib[]= { Attributes::SrcColPattern, Attributes::DstColPattern,
									Attributes::SrcFkPattern, Attributes::DstFkPattern,
									Attributes::PkPattern, Attributes::UqPattern,
									Attributes::PkColPattern };

			unsigned 	pattern_id[]= { Relationship::SrcColPattern, Relationship::DstColPattern,
										Relationship::SrcFkPattern, Relationship::DstFkPattern,
										Relationship::PkPattern, Relationship::UqPattern,
										Relationship::PkColPattern },
					pat_count=sizeof(pattern_id)/sizeof(unsigned);

			sql_disabled=attribs[Attributes::SqlDisabled]==Attributes::True;
			identifier=attribs[Attributes::Identifier]==Attributes::True;
			deferrable=attribs[Attributes::Deferrable]==Attributes::True;
			defer_type=DeferralType(attribs[Attributes::DeferType]);
			del_action=ActionType(attribs[Attributes::DelAction]);
			upd_action=ActionType(attribs[Attributes::UpdAction]);
			single_pk_col=(attribs[Attributes::SinglePkColumn]==Attributes::True);

			if(attribs[Attributes::Type]==Attributes::Relationship11)
				rel_type=BaseRelationship::Relationship11;
			else if(attribs[Attributes::Type]==Attributes::Relationship1n)
				rel_type=BaseRelationship::Relationship1n;
			else if(attribs[Attributes::Type]==Attributes::RelationshipNn)
				rel_type=BaseRelationship::RelationshipNn;
			else if(attribs[Attributes::Type]==Attributes::RelationshipGen)
				rel_type=BaseRelationship::RelationshipGen;
			else if(attribs[Attributes::Type]==Attributes::RelationshipDep)
				rel_type=BaseRelationship::RelationshipDep;
			else if(attribs[Attributes::Type]==Attributes::RelationshipPart)
				rel_type=BaseRelationship::RelationshipPart;

			rel=new Relationship(rel_type,
					dynamic_cast<PhysicalTable *>(tables[0]),
					dynamic_cast<PhysicalTable *>(tables[1]),
					src_mand, dst_mand,
					identifier, deferrable, defer_type, del_action, upd_action,
					CopyOptions(attribs[Attributes::CopyMode].toUInt(),
					attribs[Attributes::CopyOptions].toUInt()));

			rel->setSQLDisabled(sql_disabled);
			rel->setSiglePKColumn(single_pk_col);

			if(!attribs[Attributes::TableName].isEmpty())
				rel->setTableNameRelNN(attribs[Attributes::TableName]);

			rel->setName(attribs[Attributes::Name]);
			rel->setAlias(attribs[Attributes::Alias]);
			base_rel=rel;

			//Configuring the name patterns
			for(i=0; i < pat_count; i++)
				rel->setNamePattern(pattern_id[i], attribs[pat_attrib[i]]);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem == Attributes::Expression && rel)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						rel->setPartitionBoundingExpr(xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Column && rel)
					{
						xmlparser.savePosition();
						rel->addObject(createColumn());
						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Constraint && rel)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(constr_attribs);

						/* If we find a primary key constraint at this point means that we're handling the original primary key stored by the relationship.
						 * Since relationships can't have primary keys created manually by the users we assume that
						 * the relationship contains a special primary key (created during relationship connection)
						 * and the current constraint is the original one owned by one of the tables prior the connection
						 * of the relationship. */
						if(constr_attribs[Attributes::Type] == Attributes::PkConstr)
						{
							table = getTable(constr_attribs[Attributes::Table]);
							rel->setOriginalPrimaryKey(createConstraint(table));
						}
						else
							rel->addObject(createConstraint(rel));

						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Line)
					{
						vector<QPointF> points;
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						do
						{
							xmlparser.getElementAttributes(attribs);
							points.push_back(QPointF(attribs[Attributes::XPos].toDouble(),
											 attribs[Attributes::YPos].toDouble()));
						}
						while(xmlparser.accessElement(XmlParser::NextElement));

						base_rel->setPoints(points);
						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Label)
					{
						xmlparser.getElementAttributes(attribs);
						str_aux=attribs[Attributes::RefType];

						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						xmlparser.getElementAttributes(attribs);
						xmlparser.restorePosition();

						base_rel->setLabelDistance(labels_id[str_aux],
													 QPointF(attribs[Attributes::XPos].toDouble(),
													 attribs[Attributes::YPos].toDouble()));
					}
					else if(elem==Attributes::SpecialPkCols && rel)
					{
						QList<QString> col_list;

						xmlparser.getElementAttributes(attribs);
						col_list=attribs[Attributes::Indexes].split(',');

						while(!col_list.isEmpty())
						{
							cols_special_pk.push_back(col_list.front().toUInt());
							col_list.pop_front();
						}

						rel->setSpecialPrimaryKeyCols(cols_special_pk);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(base_rel && base_rel->getObjectType()==ObjectType::Relationship)
			delete base_rel;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	if(rel)
	{
		storeSpecialObjectsXML();
		addRelationship(rel);
	}

	base_rel->setFadedOut(faded_out);
	base_rel->setProtected(protect);
	base_rel->setCustomColor(custom_color);
	base_rel->setLayer(layer);

	/* If the FK relationship does not reference a foreign key (models generated in older versions)
	 * we need to assign them to the respective relationships */
	if(base_rel && base_rel->getObjectType()==ObjectType::BaseRelationship)
	{
		base_rel->blockSignals(loading_model);
		base_rel->connectRelationship();
		base_rel->blockSignals(false);
	}

	return base_rel;
}

Permission *DatabaseModel::createPermission()
{
	Permission *perm=nullptr;
	BaseObject *object=nullptr;
	PhysicalTable *parent_table=nullptr;
	Role *role=nullptr;
	attribs_map priv_attribs, attribs;
	attribs_map::iterator itr, itr_end;
	ObjectType obj_type;
	QString parent_name, obj_name;
	QStringList list;
	unsigned priv_type=Permission::PrivSelect;
	bool priv_value, grant_op, revoke, cascade;

	try
	{
		xmlparser.getElementAttributes(priv_attribs);
		revoke=priv_attribs[Attributes::Revoke]==Attributes::True;
		cascade=priv_attribs[Attributes::Cascade]==Attributes::True;

		xmlparser.savePosition();
		xmlparser.accessElement(XmlParser::ChildElement);
		xmlparser.getElementAttributes(attribs);

		obj_type=BaseObject::getObjectType(attribs[Attributes::Type]);
		obj_name=attribs[Attributes::Name];
		parent_name=attribs[Attributes::Parent];

		//If the object is a column its needed to get the parent table
		if(obj_type==ObjectType::Column)
		{
			parent_table=dynamic_cast<PhysicalTable *>(getObject(parent_name, {ObjectType::Table, ObjectType::ForeignTable}));

			if(parent_table)
				object=parent_table->getColumn(obj_name);
		}
		else if(obj_type==ObjectType::Database)
		{
			object=this;
		}
		else
			object=getObject(obj_name, obj_type);

		//Raises an error if the permission references an object that does not exists
		if(!object)
			throw Exception(Exception::getErrorMessage(ErrorCode::PermissionRefInexistObject)
							.arg(obj_name)
							.arg(BaseObject::getTypeName(obj_type)),
							ErrorCode::PermissionRefInexistObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		perm=new Permission(object);
		perm->setRevoke(revoke);
		perm->setCascade(cascade);

		do
		{
			if(xmlparser.getElementName()==Attributes::Roles)
			{
				xmlparser.getElementAttributes(attribs);
				list = attribs[Attributes::Names].split(',');

				for(auto &name : list)
				{
					role=dynamic_cast<Role *>(getObject(name.trimmed(), ObjectType::Role));

					//Raises an error if the referenced role doesn't exists
					if(!role)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::PermissionRefInexistObject)
										.arg(name)
										.arg(BaseObject::getTypeName(ObjectType::Role)),
										ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					}

					perm->addRole(role);
				}
			}
			else if(xmlparser.getElementName()==Attributes::Privileges)
			{
				xmlparser.getElementAttributes(priv_attribs);

				itr=priv_attribs.begin();
				itr_end=priv_attribs.end();

				while(itr!=itr_end)
				{
					if(itr->first!=Attributes::GrantOp)
					{
						priv_value=(itr->second==Attributes::True);
						grant_op=(itr->second==Attributes::GrantOp);

						if(itr->first==Attributes::ConnectPriv)
							priv_type=Permission::PrivConnect;
						else if(itr->first==Attributes::CreatePriv)
							priv_type=Permission::PrivCreate;
						else if(itr->first==Attributes::DeletePriv)
							priv_type=Permission::PrivDelete;
						else if(itr->first==Attributes::ExecutPriv)
							priv_type=Permission::PrivExecute;
						else if(itr->first==Attributes::InsertPriv)
							priv_type=Permission::PrivInsert;
						else if(itr->first==Attributes::ReferencesPriv)
							priv_type=Permission::PrivReferences;
						else if(itr->first==Attributes::SelectPriv)
							priv_type=Permission::PrivSelect;
						else if(itr->first==Attributes::TemporaryPriv)
							priv_type=Permission::PrivTemporary;
						else if(itr->first==Attributes::TriggerPriv)
							priv_type=Permission::PrivTrigger;
						else if(itr->first==Attributes::TruncatePriv)
							priv_type=Permission::PrivTruncate;
						else if(itr->first==Attributes::UpdatePriv)
							priv_type=Permission::PrivUpdate;
						else if(itr->first==Attributes::UsagePriv)
							priv_type=Permission::PrivUsage;

						perm->setPrivilege(priv_type, (priv_value || grant_op), grant_op);
					}
					itr++;
				}
			}
		}
		while(xmlparser.accessElement(XmlParser::NextElement));

		xmlparser.restorePosition();
	}
	catch(Exception &e)
	{
		if(perm) delete perm;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return perm;
}

void DatabaseModel::validateColumnRemoval(Column *column)
{
	if(column && column->getParentTable())
	{
		vector<BaseObject *> refs;
		getObjectReferences(column, refs);

		//Raises an error if there are objects referencing the column
		if(!refs.empty())
			throw Exception(Exception::getErrorMessage(ErrorCode::RemDirectReference)
							.arg(column->getParentTable()->getName(true) + QString(".") + column->getName(true))
							.arg(column->getTypeName())
							.arg(refs[0]->getName(true))
				.arg(refs[0]->getTypeName()),
				ErrorCode::RemDirectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

void DatabaseModel::validateRelationships(TableObject *object, Table *parent_tab)
{
	try
	{
		bool revalidate_rels=false, ref_tab_inheritance=false;
		Relationship *rel=nullptr;
		vector<BaseObject *>::iterator itr, itr_end;
		ObjectType obj_type;

		if(object && parent_tab)
		{
			obj_type=object->getObjectType();

			/* Relationship validation condition:
			> Case the object is a column and its reference by the parent table primary key
			> Case the parent table is a partition and a column is being removed
			> Case the object is a constraint and its a table primary key */
			revalidate_rels=((obj_type==ObjectType::Column &&
												(parent_tab->isConstraintRefColumn(dynamic_cast<Column *>(object), ConstraintType::PrimaryKey) ||
												 parent_tab->isPartition() || parent_tab->isPartitioned())) ||
											 (obj_type==ObjectType::Constraint &&
												dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::PrimaryKey));

			/* Additional validation for columns: checks if the parent table participates on a
			generalization/copy as destination table */
			if(obj_type==ObjectType::Column)
			{
				itr=relationships.begin();
				itr_end=relationships.end();

				while(itr!=itr_end && !ref_tab_inheritance)
				{
					rel=dynamic_cast<Relationship *>(*itr);
					itr++;
					ref_tab_inheritance=(rel->getRelationshipType()==Relationship::RelationshipGen &&
															 rel->getReferenceTable()==parent_tab);
				}
			}

			if(revalidate_rels || ref_tab_inheritance)
			{
				storeSpecialObjectsXML();
				disconnectRelationships();
				validateRelationships();
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseModel::__getCodeDefinition(unsigned def_type)
{
	QString def, bkp_appended_sql, bkp_prepended_sql;

	//Forcing the name/signature cleanup due to the validation temp. names feature
	attributes[Attributes::Name]=QString();
	attributes[Attributes::Signature]=QString();

	if(conn_limit >= 0)
		attributes[Attributes::ConnLimit]=QString("%1").arg(conn_limit);

	if(def_type==SchemaParser::SqlDefinition)
	{
		QString loc_attribs[]={ Attributes::LcCtype,  Attributes::LcCollate };

		if(encoding!=BaseType::Null)
			attributes[Attributes::Encoding]=QString("'%1'").arg(~encoding);

		for(unsigned i=0; i < 2; i++)
		{
			if(!localizations[i].isEmpty())
			{
				attributes[loc_attribs[i]]=QString("'%1'").arg(localizations[i]);
			}
		}
	}
	else
	{
		attributes[Attributes::Encoding]=(~encoding);
		attributes[Attributes::LcCollate]=localizations[1];
		attributes[Attributes::LcCtype]=localizations[0];
		attributes[Attributes::AppendAtEod]=(append_at_eod ? Attributes::True : QString());
		attributes[Attributes::PrependAtBod]=(prepend_at_bod ? Attributes::True : QString());
	}

	attributes[Attributes::IsTemplate]=(is_template ? Attributes::True : Attributes::False);
	attributes[Attributes::AllowConns]=(allow_conns ? Attributes::True : Attributes::False);
	attributes[Attributes::TemplateDb]=template_db;

	if(def_type==SchemaParser::SqlDefinition && append_at_eod)
	{
		bkp_appended_sql=this->appended_sql;
		this->appended_sql.clear();
	}

	if(def_type==SchemaParser::SqlDefinition && prepend_at_bod)
	{
		bkp_prepended_sql=this->prepended_sql;
		this->prepended_sql.clear();
	}

	try
	{
		def=this->BaseObject::__getCodeDefinition(def_type);

		if(def_type==SchemaParser::SqlDefinition && append_at_eod)
			this->appended_sql=bkp_appended_sql;

		if(def_type==SchemaParser::SqlDefinition && prepend_at_bod)
			this->prepended_sql=bkp_prepended_sql;

		return def;
	}
	catch(Exception &e)
	{
		this->appended_sql=bkp_appended_sql;
		this->prepended_sql=bkp_prepended_sql;
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseModel::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, true);
}

QString DatabaseModel::getCodeDefinition(unsigned def_type, bool export_file)
{
	attribs_map attribs_aux;
	unsigned general_obj_cnt, gen_defs_count;
	bool sql_disabled=false;
	BaseObject *object=nullptr;
	QString def, search_path=QString("pg_catalog,public"),
			msg=tr("Generating %1 code: `%2' (%3)"),
			attrib=Attributes::Objects, attrib_aux,
			def_type_str=(def_type==SchemaParser::SqlDefinition ? QString("SQL") : QString("XML"));
	Type *usr_type=nullptr;
	map<unsigned, BaseObject *> objects_map;
	ObjectType obj_type;

	try
	{
		objects_map=getCreationOrder(def_type);
		general_obj_cnt=objects_map.size();
		gen_defs_count=0;

		attribs_aux[Attributes::ShellTypes]=QString();
		attribs_aux[Attributes::Permission]=QString();
		attribs_aux[Attributes::Schema]=QString();
		attribs_aux[Attributes::Tablespace]=QString();
		attribs_aux[Attributes::Role]=QString();

		if(def_type==SchemaParser::SqlDefinition)
		{
			attribs_aux[Attributes::Function]=(!functions.empty() ? Attributes::True : QString());

			for(auto &type : types)
			{
				usr_type=dynamic_cast<Type *>(type);

				if(usr_type->getConfiguration()==Type::BaseType)
				{
					usr_type->convertFunctionParameters();

					//Generating the shell type declaration (only for base types)
					attribs_aux[Attributes::ShellTypes] += usr_type->getCodeDefinition(def_type, true);

					/* Forcing the code invalidation for the type so the complete definition can be
					 * generated in the below iteration */
					usr_type->setCodeInvalidated(true);
				}
			}
		}

		for(auto &obj_itr : objects_map)
		{
			object=obj_itr.second;
			obj_type=object->getObjectType();

			if(obj_type==ObjectType::Type && def_type==SchemaParser::SqlDefinition)
			{
				usr_type=dynamic_cast<Type *>(object);
				attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
			}
			else if(obj_type==ObjectType::Database)
			{
				if(def_type==SchemaParser::SqlDefinition)
				{
					/* The Database has the SQL code definition disabled when generating the
		  code of the entire model because this object cannot be created from a multiline sql command */

					//Saving the sql disabled state
					sql_disabled=this->isSQLDisabled();

					//Disables the sql to generate a commented code
					this->setSQLDisabled(true);
					attribs_aux[this->getSchemaName()]+=this->__getCodeDefinition(def_type);

					//Restore the original sql disabled state
					this->setSQLDisabled(sql_disabled);
				}
				else
					attribs_aux[attrib]+=this->__getCodeDefinition(def_type);
			}
			else if(obj_type==ObjectType::Permission)
			{
				attribs_aux[Attributes::Permission]+=dynamic_cast<Permission *>(object)->getCodeDefinition(def_type);
			}
			else if(obj_type==ObjectType::Constraint)
			{
				attribs_aux[attrib]+=dynamic_cast<Constraint *>(object)->getCodeDefinition(def_type, true);
			}
			else if(obj_type==ObjectType::Role || obj_type==ObjectType::Tablespace ||  obj_type==ObjectType::Schema)
			{
				//The "public" schema does not have the SQL code definition generated
				if(def_type==SchemaParser::SqlDefinition)
					attrib_aux=BaseObject::getSchemaName(obj_type);
				else
					attrib_aux=attrib;

				/* The Tablespace has the SQL code definition disabled when generating the
		  code of the entire model because this object cannot be created from a multiline sql command */
				if(obj_type==ObjectType::Tablespace && !object->isSystemObject() && def_type==SchemaParser::SqlDefinition)
				{
					//Saving the sql disabled state
					sql_disabled=object->isSQLDisabled();

					//Disables the sql to generate a commented code
					object->setSQLDisabled(true);
					attribs_aux[attrib_aux]+=object->getCodeDefinition(def_type);

					//Restore the original sql disabled state
					object->setSQLDisabled(sql_disabled);
				}
				//System object doesn't has the XML generated (the only exception is for public schema)
				else if((obj_type!=ObjectType::Schema && !object->isSystemObject()) ||
						(obj_type==ObjectType::Schema &&
						 ((object->getName()==QString("public") && def_type==SchemaParser::XmlDefinition) ||
						  (object->getName()!=QString("public") && object->getName()!=QString("pg_catalog")))))
				{
					if(object->getObjectType()==ObjectType::Schema)
						search_path+=QString(",") + object->getName(true);

					//Generates the code definition and concatenates to the others
					attribs_aux[attrib_aux]+=object->getCodeDefinition(def_type);
				}
			}
			else
			{
				if(object->isSystemObject())
					attribs_aux[attrib]+=QString();
				else
					attribs_aux[attrib]+=object->getCodeDefinition(def_type);
			}

			gen_defs_count++;

			if((def_type==SchemaParser::SqlDefinition && !object->isSQLDisabled()) ||
					(def_type==SchemaParser::XmlDefinition && !object->isSystemObject()))
			{
				emit s_objectLoaded((gen_defs_count/static_cast<double>(general_obj_cnt)) * 100,
									msg.arg(def_type_str)
									.arg(object->getName())
									.arg(object->getTypeName()),
									enum_cast(object->getObjectType()));
			}
		}

		attribs_aux[Attributes::SearchPath]=search_path;
		attribs_aux[Attributes::ModelAuthor]=author;
		attribs_aux[Attributes::PgModelerVersion]=GlobalAttributes::PgModelerVersion;

		if(def_type==SchemaParser::XmlDefinition)
		{
			QStringList act_layers;

			for(auto &layer_id : active_layers)
				act_layers.push_back(QString::number(layer_id));

			attribs_aux[Attributes::Layers]=layers.join(';');
			attribs_aux[Attributes::ActiveLayers]=act_layers.join(';');
			attribs_aux[Attributes::MaxObjCount]=QString::number(static_cast<unsigned>(getMaxObjectCount() * 1.20));
			attribs_aux[Attributes::Protected]=(this->is_protected ? Attributes::True : QString());
			attribs_aux[Attributes::LastPosition]=QString("%1,%2").arg(last_pos.x()).arg(last_pos.y());
			attribs_aux[Attributes::LastZoom]=QString::number(last_zoom);
			attribs_aux[Attributes::DefaultSchema]=(default_objs[ObjectType::Schema] ? default_objs[ObjectType::Schema]->getName(true) : QString());
			attribs_aux[Attributes::DefaultOwner]=(default_objs[ObjectType::Role] ? default_objs[ObjectType::Role]->getName(true) : QString());
			attribs_aux[Attributes::DefaultTablespace]=(default_objs[ObjectType::Tablespace] ? default_objs[ObjectType::Tablespace]->getName(true) : QString());
			attribs_aux[Attributes::DefaultCollation]=(default_objs[ObjectType::Collation] ? default_objs[ObjectType::Collation]->getName(true) : QString());
		}
		else
		{
			for(auto &type : types)
			{
				usr_type=dynamic_cast<Type *>(type);
				if(usr_type->getConfiguration()==Type::BaseType)
				{
					attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
					usr_type->convertFunctionParameters(true);
				}
			}
		}
	}
	catch(Exception &e)
	{
		if(def_type==SchemaParser::SqlDefinition)
		{
			for(auto &type : types)
			{
				usr_type=dynamic_cast<Type *>(type);
				if(usr_type->getConfiguration()==Type::BaseType)
				{
					attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
					usr_type->convertFunctionParameters(true);
				}
			}
		}
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	attribs_aux[Attributes::ExportToFile]=(export_file ? Attributes::True : QString());
	def=schparser.getCodeDefinition(Attributes::DbModel, attribs_aux, def_type);

	if(prepend_at_bod && def_type==SchemaParser::SqlDefinition)
		def=QString("-- Prepended SQL commands --\n") +	this->prepended_sql + Attributes::DdlEndToken + def;

	if(append_at_eod && def_type==SchemaParser::SqlDefinition)
		def+=QString("-- Appended SQL commands --\n") +	this->appended_sql + QChar('\n') + Attributes::DdlEndToken;

	return def;
}

map<unsigned, BaseObject *> DatabaseModel::getCreationOrder(unsigned def_type, bool incl_relnn_objs, bool incl_rel1n_constrs)
{
	BaseObject *object=nullptr;
	vector<BaseObject *> fkeys, fk_rels, aux_tables;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	map<unsigned, BaseObject *> objects_map;
	PhysicalTable *table=nullptr;
	Constraint *constr=nullptr;
	View *view=nullptr;
	Relationship *rel=nullptr;
	ObjectType aux_obj_types[]={ ObjectType::Role, ObjectType::Tablespace, ObjectType::Schema, ObjectType::Tag };

	vector<ObjectType> obj_types_vect = getObjectTypes(false, { ObjectType::Role, ObjectType::Tablespace, ObjectType::Schema,
																															ObjectType::Tag, ObjectType::Database, ObjectType::Permission });

	unsigned i=0, aux_obj_cnt=sizeof(aux_obj_types)/sizeof(ObjectType);

	//The first objects on the map will be roles, tablespaces, schemas and tags
	for(i=0; i < aux_obj_cnt; i++)
	{
		if(aux_obj_types[i]!=ObjectType::Tag || def_type==SchemaParser::XmlDefinition)
		{
			obj_list=getObjectList(aux_obj_types[i]);

			for(auto &object : (*obj_list))
				objects_map[object->getObjectId()]=object;
		}
	}

	//Includes the database model on the objects map permitting to create the code in a correct order
	objects_map[this->getObjectId()]=this;

	for(auto &obj_type : obj_types_vect)
	{
		//For SQL definition, only the textbox and base relationship does not enters to the code generation list
		if(def_type==SchemaParser::SqlDefinition &&
			 (obj_type==ObjectType::Textbox || obj_type==ObjectType::BaseRelationship))
			obj_list=nullptr;
		else
			obj_list=getObjectList(obj_type);

		if(obj_list)
		{
			for(auto &object : (*obj_list))
			{
				/* If the object is a FK relationship it's stored in a separeted list in order to have the
			 code generated at end of whole definition (after foreign keys definition) */
				if(object->getObjectType()==ObjectType::BaseRelationship &&
						dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipFk)
				{
					fk_rels.push_back(object);
				}
				else
				{
					if(def_type==SchemaParser::XmlDefinition || !incl_relnn_objs)
						objects_map[object->getObjectId()]=object;
					else
					{
						rel=dynamic_cast<Relationship *>(object);

						/* Avoiding many-to-many relationships to be included in the map.
			 They are treated in a separated way below, because on the diff process (ModelsDiffHelper) the generated table
			 need to be compared to other tables not the relationship itself */
						if(!incl_relnn_objs || !rel || (rel && rel->getRelationshipType()!=BaseRelationship::RelationshipNn))
							objects_map[object->getObjectId()]=object;
					}
				}
			}
		}
	}

	/* Getting and storing the special objects (which reference columns of tables added for relationships)
	  on the map of objects. */
	aux_tables = tables;
	aux_tables.insert(aux_tables.end(), foreign_tables.begin(), foreign_tables.end());

	for(auto &obj : aux_tables)
	{
		table = dynamic_cast<PhysicalTable *>(obj);

		for(auto &obj : *table->getObjectList(ObjectType::Constraint))
		{
			//table->getConstraint(i);
			constr = dynamic_cast<Constraint *>(obj);

			/* Case the constraint is a special object stores it on the objects map. Independently to the
		configuration, foreign keys are discarded in this iteration because on the end of the method
		they have the definition generated */
			if(constr->getConstraintType()!=ConstraintType::ForeignKey &&  !constr->isAddedByLinking() &&
					((constr->getConstraintType()!=ConstraintType::PrimaryKey && constr->isReferRelationshipAddedColumn())))
				objects_map[constr->getObjectId()]=constr;
			else if(constr->getConstraintType()==ConstraintType::ForeignKey && !constr->isAddedByLinking())
				fkeys.push_back(constr);
		}

		for(auto obj : table->getObjects({ ObjectType::Column, ObjectType::Constraint }))
			objects_map[obj->getObjectId()]=obj;
	}

	/* Getting and storing the special objects (which reference columns of tables added for relationships)
			on the map of objects. */
	for(auto &obj : views)
	{
		view=dynamic_cast<View *>(obj);

		for(auto obj : view->getObjects())
			objects_map[obj->getObjectId()]=obj;
	}

	/* SPECIAL CASE: Generating the correct order for tables, views, relationships and sequences

	 This generation is made in the following way:
	 1) Based on the relationship list, participant tables comes before the relationship itself.
	 2) Other tables came after the objects on the step 1.
	 3) The sequences must have their code generated after the tables
	 4) View are the last objects in the list avoiding table/column reference breaking */
	if(def_type==SchemaParser::SqlDefinition)
	{
		BaseObject *objs[3]={nullptr, nullptr, nullptr};
		vector<BaseObject *> vet_aux, rel_constrs;

		vet_aux=relationships;
		vet_aux.insert(vet_aux.end(), tables.begin(),tables.end());
		vet_aux.insert(vet_aux.end(), foreign_tables.begin(),foreign_tables.end());
		vet_aux.insert(vet_aux.end(), sequences.begin(),sequences.end());
		vet_aux.insert(vet_aux.end(), views.begin(),views.end());;
		itr=vet_aux.begin();
		itr_end=vet_aux.end();

		while(itr!=itr_end)
		{
			object=(*itr);
			itr++;

			if(object->getObjectType()==ObjectType::Relationship)
			{
				rel=dynamic_cast<Relationship *>(object);
				objs[0]=rel->getTable(Relationship::SrcTable);
				objs[1]=rel->getTable(Relationship::DstTable);

				/* For many-to-many relationship, the generated table and the foreign keys that represents
		   the link are included on the creation order map instead of the relationship itself. This is
		   done to permit the table to be accessed and compared on the diff process */
				if(incl_relnn_objs &&
						rel->getRelationshipType()==BaseRelationship::RelationshipNn &&
						rel->getGeneratedTable())
				{
					table=rel->getGeneratedTable();
					objs[2]=table;

					for(BaseObject *tab_obj : *table->getObjectList(ObjectType::Constraint))
					{
						constr=dynamic_cast<Constraint *>(tab_obj);

						if(constr->getConstraintType()==ConstraintType::ForeignKey)
							fkeys.push_back(constr);
					}
				}
				else if(incl_rel1n_constrs)
				{
					vector<Constraint *> constrs=rel->getGeneratedConstraints();

					for(auto &constr : constrs)
					{
						if(constr->getConstraintType()!=ConstraintType::PrimaryKey)
							rel_constrs.push_back(constr);
					}
				}
				else
					objs[2]=rel;

				for(i=0; i < 3; i++)
				{
					if(objs[i] && objects_map.count(objs[i]->getObjectId())==0)
						objects_map[objs[i]->getObjectId()]=objs[i];
				}
			}
			else
			{
				if(objects_map.count(object->getObjectId())==0)
					objects_map[object->getObjectId()]=object;
			}
		}

		fkeys.insert(fkeys.end(), rel_constrs.begin(), rel_constrs.end());
	}

	//Adding fk relationships and foreign keys at end of objects map
	i=BaseObject::getGlobalId() + 1;
	fkeys.insert(fkeys.end(), fk_rels.begin(), fk_rels.end());

	for(auto &obj : fkeys)
	{
		objects_map[i]=obj;
		i++;
	}

	//Adding permissions at the very end of object map
	i=BaseObject::getGlobalId() + fkeys.size() + 1;

	for(auto &obj : permissions)
	{
		objects_map[i]=obj;
		i++;
	}

	return objects_map;
}


void DatabaseModel::__getObjectDependencies(BaseObject *object, vector<BaseObject *> &objs)
{
	vector<BaseObject *> dep_objs, chld_objs;
	PhysicalTable *table=dynamic_cast<PhysicalTable *>(object);
	ObjectType obj_type=ObjectType::BaseObject;

	if(!object) return;

	getObjectDependecies(object, objs, true);
	obj_type=object->getObjectType();

	//If the object is a table include as dependency the copy table as well any ancestor table
	if(table)
	{
		//Including copy table and its dependencies
		if(table->getCopyTable())
		{
			__getObjectDependencies(table->getCopyTable(), dep_objs);
			objs.insert(objs.end(), dep_objs.begin(), dep_objs.end());
		}

		//Including ancestor tables and their dependencies
		dep_objs.clear();
		for(unsigned i=0; i < table->getAncestorTableCount(); i++)
		{
			__getObjectDependencies(table->getAncestorTable(i), dep_objs);
			objs.insert(objs.end(), dep_objs.begin(), dep_objs.end());
		}
	}

	//If there is the need to include the children objects
	if(BaseTable::isBaseTable(obj_type) || obj_type==ObjectType::Schema)
	{
		vector<BaseObject *>::iterator end;

		if(obj_type==ObjectType::Schema)
		{
			//Retrieve all objects that belongs to the schema
			chld_objs=getObjects(object);
			objs.insert(objs.end(), chld_objs.begin(), chld_objs.end());

			for(BaseObject *aux_obj : chld_objs)
			{
				__getObjectDependencies(aux_obj, dep_objs);
				objs.insert(objs.end(), dep_objs.begin(), dep_objs.end());
			}
		}
		else
		{
			BaseTable *tab=dynamic_cast<BaseTable *>(object);
			Constraint *constr=nullptr;

			chld_objs=tab->getObjects();

			for(BaseObject *child : chld_objs)
			{
				constr=dynamic_cast<Constraint *>(child);

				/* Columns are discarded but constraint included only if they are included by relationship
		   or foreign keys in which referenced table resides in the same schema as their parent tables */
				if((!constr && child->getObjectType()!=ObjectType::Column) ||
						(constr &&
						 ((constr->getConstraintType()==ConstraintType::ForeignKey) ||
							(constr->getConstraintType()!=ConstraintType::ForeignKey &&
							 constr->getConstraintType()!=ConstraintType::PrimaryKey &&
						   constr->isReferRelationshipAddedColumn()))))
				{
					__getObjectDependencies(child, objs);

					if(constr && constr->getReferencedTable() && std::find(objs.begin(), objs.end(), constr->getReferencedTable())==objs.end())
						__getObjectDependencies(constr->getReferencedTable(), objs);
				}
			}
		}

		//Cleaning up the resulting list removing duplicate elements
		std::sort(objs.begin(), objs.end());
		end=std::unique(objs.begin(), objs.end());
		objs.erase(end, objs.end());
	}
}

vector<BaseObject *> DatabaseModel::getCreationOrder(BaseObject *object, bool only_children)
{
	if(!object)
		return vector<BaseObject *>();

	map<unsigned, BaseObject *> objs_map;
	vector<BaseObject *> objs, children;
	vector<Permission *> perms_aux, perms;
	vector<Role *> roles;
	PhysicalTable *table=nullptr;
	Relationship *rel=nullptr;
	ObjectType obj_type=object->getObjectType();

	if(only_children)
		objs.push_back(object);
	else
		__getObjectDependencies(object, objs);

	/* Include tables generated by many-to-many relationships if their schemas are the same
	 as the 'object' when this one is a schema too */
	if(obj_type==ObjectType::Schema)
	{
		if(only_children)
		{
			children=getObjects(object);
			objs.insert(objs.end(), children.begin(), children.end());
		}

		for(BaseObject *obj : relationships)
		{
			rel=dynamic_cast<Relationship *>(obj);

			if(rel->getRelationshipType()==Relationship::RelationshipNn &&
					rel->getGeneratedTable() &&
					rel->getGeneratedTable()->getSchema()==object)
			{
				if(only_children)
					objs.push_back(rel->getGeneratedTable());
				else
					__getObjectDependencies(rel->getGeneratedTable(), objs);
			}
		}
	}

	if(only_children)
	{
		BaseTable *table=nullptr;
		Constraint *constr=nullptr;
		vector<BaseObject *> objs_aux;

		for(BaseObject *obj : objs)
		{
			table=dynamic_cast<BaseTable *>(obj);

			if(table)
			{
				children=table->getObjects();

				for(BaseObject *child : children)
				{
					constr=dynamic_cast<Constraint *>(child);

					if((!constr && child->getObjectType()!=ObjectType::Column) ||
							(constr &&
							 ((constr->getConstraintType()==ConstraintType::ForeignKey) ||
								(constr->getConstraintType()!=ConstraintType::ForeignKey &&
								 constr->getConstraintType()!=ConstraintType::PrimaryKey &&
							   constr->isReferRelationshipAddedColumn()))))
					{
						objs_aux.push_back(child);
					}
				}
			}
		}

		objs.insert(objs.end(), objs_aux.begin(), objs_aux.end());
	}
	else
	{
		//Retrieving all permission related to the gathered objects
		for(BaseObject *obj : objs)
		{
			getPermissions(obj, perms_aux);
			perms.insert(perms.end(), perms_aux.begin(), perms_aux.end());
			perms_aux.clear();

			table=dynamic_cast<PhysicalTable *>(obj);

			if(table)
			{
				for(BaseObject *col : *table->getObjectList(ObjectType::Column))
				{
					getPermissions(col, perms_aux);
					perms.insert(perms.end(), perms_aux.begin(), perms_aux.end());
				}
			}
		}

		/* Retrieving all additional roles (reference by permissions) that are not in the
	 main object list (used as creation order) */
		for(Permission *perm : perms)
		{
			roles=perm->getRoles();

			for(Role *role : roles)
			{
				if(std::find(objs.begin(), objs.end(), role)==objs.end())
					getObjectDependecies(role, objs, true);
			}
		}
	}

	if(objs.size() > 1 || !perms.empty())
	{
		//Putting all objects in a map ordering them by ID
		for(BaseObject *obj : objs)
			objs_map[obj->getObjectId()]=obj;

		//Recreationg the object list now with objects ordered properly
		objs.clear();
		for(auto &itr : objs_map)
			objs.push_back(itr.second);

		//Appending permissions at the end of the creation order list
		objs.insert(objs.end(), perms.begin(), perms.end());
	}

	return objs;
}

void DatabaseModel::saveModel(const QString &filename, unsigned def_type)
{
	QFile output(filename);
	QByteArray buf;

	output.open(QFile::WriteOnly);

	if(!output.isOpen())
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(filename),
										ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		buf.append(this->getCodeDefinition(def_type));
		output.write(buf.data(),buf.size());
		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(ErrorCode::FileNotWrittenInvalidDefinition).arg(filename),
										ErrorCode::FileNotWrittenInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::getOpClassDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	OperatorClass *opclass=dynamic_cast<OperatorClass *>(object);
	BaseObject *usr_type=getObjectPgSQLType(opclass->getDataType());
	unsigned i, cnt;
	OperatorClassElement elem;

	if(usr_type)
		getObjectDependecies(usr_type, deps, inc_indirect_deps);

	if(opclass->getFamily())
		getObjectDependecies(opclass->getFamily(), deps, inc_indirect_deps);

	cnt=opclass->getElementCount();

	for(i=0; i < cnt; i++)
	{
		elem=opclass->getElement(i);

		if(elem.getFunction())
			getObjectDependecies(elem.getFunction(), deps, inc_indirect_deps);

		if(elem.getOperator())
			getObjectDependecies(elem.getOperator(), deps, inc_indirect_deps);

		if(elem.getOperatorFamily())
			getObjectDependecies(elem.getOperatorFamily(), deps, inc_indirect_deps);

		if(elem.getStorage().isUserType())
		{
			usr_type=getObjectPgSQLType(elem.getStorage());
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
		}
	}
}

void DatabaseModel::getDomainDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	BaseObject *usr_type=getObjectPgSQLType(dynamic_cast<Domain *>(object)->getType());

	if(usr_type)
		getObjectDependecies(usr_type, deps, inc_indirect_deps);
}

void DatabaseModel::getCastDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Cast *cast=dynamic_cast<Cast *>(object);
	BaseObject *usr_type=nullptr;

	for(unsigned i=Cast::SrcType; i <= Cast::DstType; i++)
	{
		usr_type=getObjectPgSQLType(cast->getDataType(i));

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}

	getObjectDependecies(cast->getCastFunction(), deps, inc_indirect_deps);
}

void DatabaseModel::getFunctionDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Function *func=dynamic_cast<Function *>(object);
	BaseObject *usr_type=getObjectPgSQLType(func->getReturnType());
	unsigned count, i;

	if(!func->isSystemObject())
		getObjectDependecies(func->getLanguage(), deps, inc_indirect_deps);

	if(usr_type)
		getObjectDependecies(usr_type, deps, inc_indirect_deps);

	count=func->getParameterCount();
	for(i=0; i < count; i++)
	{
		usr_type=getObjectPgSQLType(func->getParameter(i).getType());

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}

	count=func->getReturnedTableColumnCount();
	for(i=0; i < count; i++)
	{
		usr_type=getObjectPgSQLType(func->getReturnedTableColumn(i).getType());

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}
}

void DatabaseModel::getAggregateDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Aggregate *aggreg=dynamic_cast<Aggregate *>(object);
	BaseObject *usr_type=nullptr;
	unsigned count, i;

	for(i=Aggregate::FinalFunc; i <= Aggregate::TransitionFunc; i++)
		getObjectDependecies(aggreg->getFunction(i), deps, inc_indirect_deps);

	usr_type=getObjectPgSQLType(aggreg->getStateType());

	if(usr_type)
		getObjectDependecies(usr_type, deps, inc_indirect_deps);

	if(aggreg->getSortOperator())
		getObjectDependecies(aggreg->getSortOperator(), deps, inc_indirect_deps);

	count=aggreg->getDataTypeCount();
	for(i=0; i < count; i++)
	{
		usr_type=getObjectPgSQLType(aggreg->getDataType(i));

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}
}

void DatabaseModel::getLanguageDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Language *lang=dynamic_cast<Language *>(object);

	for(unsigned i=Language::ValidatorFunc; i <= Language::InlineFunc; i++)
	{
		if(lang->getFunction(i))
			getObjectDependecies(lang->getFunction(i), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getOperatorDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Operator *oper=dynamic_cast<Operator *>(object);
	BaseObject *usr_type=nullptr;
	unsigned i;

	for(i=Operator::FuncOperator; i <= Operator::FuncRestrict; i++)
	{
		if(oper->getFunction(i))
			getObjectDependecies(oper->getFunction(i), deps, inc_indirect_deps);
	}

	for(i=Operator::LeftArg; i <= Operator::RightArg; i++)
	{
		usr_type=getObjectPgSQLType(oper->getArgumentType(i));

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}

	for(i=Operator::OperCommutator; i <= Operator::OperNegator; i++)
	{
		if(oper->getOperator(i))
			getObjectDependecies(oper->getOperator(i), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getRoleDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Role *role=dynamic_cast<Role *>(object);
	unsigned i, i1, count,
			role_types[3]={ Role::RefRole, Role::MemberRole, Role::AdminRole };

	for(i=0; i < 3; i++)
	{
		count=role->getRoleCount(role_types[i]);
		for(i1=0; i1 < count; i1++)
			getObjectDependecies(role->getRole(role_types[i], i1), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getRelationshipDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Relationship *rel=dynamic_cast<Relationship *>(object);
	BaseObject *usr_type=nullptr;
	Constraint *constr=nullptr;
	unsigned i, count;

	getObjectDependecies(rel->getTable(Relationship::SrcTable), deps, inc_indirect_deps);
	getObjectDependecies(rel->getTable(Relationship::DstTable), deps, inc_indirect_deps);

	count=rel->getAttributeCount();
	for(i=0; i < count; i++)
	{
		usr_type=getObjectPgSQLType(rel->getAttribute(i)->getType());

		if(usr_type)
			getObjectDependecies(usr_type, deps, inc_indirect_deps);
	}

	count=rel->getConstraintCount();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(rel->getConstraint(i));

		if(constr->getTablespace())
			getObjectDependecies(constr->getTablespace(), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getSequenceDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Sequence *seq=dynamic_cast<Sequence *>(object);
	if(seq->getOwnerColumn())
		getObjectDependecies(seq->getOwnerColumn()->getParentTable(), deps, inc_indirect_deps);
}

void DatabaseModel::getColumnDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Column *col=dynamic_cast<Column *>(object);
	BaseObject *usr_type=getObjectPgSQLType(col->getType()),
			*sequence=col->getSequence();

	if(usr_type)
		getObjectDependecies(usr_type, deps, inc_indirect_deps);

	if(sequence)
		getObjectDependecies(sequence, deps, inc_indirect_deps);
}

void DatabaseModel::getTriggerDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Trigger *trig=dynamic_cast<Trigger *>(object);

	if(trig->getReferencedTable())
		getObjectDependecies(trig->getReferencedTable(), deps, inc_indirect_deps);

	if(trig->getFunction())
		getObjectDependecies(trig->getFunction(), deps, inc_indirect_deps);
}

void DatabaseModel::getIndexDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Index *index=dynamic_cast<Index *>(object);
	BaseObject *usr_type=nullptr;
	unsigned i, count=index->getIndexElementCount();

	for(i=0; i < count; i++)
	{
		if(index->getIndexElement(i).getOperatorClass())
			getObjectDependecies(index->getIndexElement(i).getOperatorClass(), deps, inc_indirect_deps);

		if(index->getIndexElement(i).getColumn())
		{
			usr_type=getObjectPgSQLType(index->getIndexElement(i).getColumn()->getType());

			if(usr_type)
				getObjectDependecies(usr_type, deps, inc_indirect_deps);
		}

		if(index->getIndexElement(i).getCollation())
			getObjectDependecies(index->getIndexElement(i).getCollation(), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getPolicyDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Policy *pol=dynamic_cast<Policy *>(object);

	for(auto role : pol->getRoles())
		getObjectDependecies(role, deps, inc_indirect_deps);
}

void DatabaseModel::getPhysicalTableDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	PhysicalTable *tab=dynamic_cast<PhysicalTable *>(object);
	Table *aux_tab = dynamic_cast<Table *>(object);
	ForeignTable *ftable = dynamic_cast<ForeignTable *>(tab);
	BaseObject *usr_type=nullptr,  *seq=nullptr;
	Constraint *constr=nullptr;
	Trigger *trig=nullptr;
	Index *index=nullptr;
	Column *col=nullptr;
	Policy *pol=nullptr;
	unsigned count, i, count1, i1;

	count=tab->getColumnCount();
	for(i=0; i < count; i++)
	{
		col=tab->getColumn(i);
		usr_type=getObjectPgSQLType(col->getType());
		seq=col->getSequence();

		if(!col->isAddedByLinking())
		{
			if(usr_type)
				getObjectDependecies(usr_type, deps, inc_indirect_deps);

			if(seq)
				getObjectDependecies(seq, deps, inc_indirect_deps);
		}
	}

	count=tab->getConstraintCount();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(tab->getConstraint(i));
		count1=constr->getExcludeElementCount();

		for(i1=0; i1 < count1; i1++)
		{
			if(constr->getExcludeElement(i1).getOperator())
				getObjectDependecies(constr->getExcludeElement(i1).getOperator(), deps, inc_indirect_deps);

			if(constr->getExcludeElement(i1).getOperatorClass())
				getObjectDependecies(constr->getExcludeElement(i1).getOperatorClass(), deps, inc_indirect_deps);
		}

		if(inc_indirect_deps &&
				!constr->isAddedByLinking() &&
				constr->getConstraintType()==ConstraintType::ForeignKey)
			getObjectDependecies(constr->getReferencedTable(), deps, inc_indirect_deps);

		if(!constr->isAddedByLinking() && constr->getTablespace())
			getObjectDependecies(constr->getTablespace(), deps, inc_indirect_deps);
	}

	count=tab->getTriggerCount();
	for(i=0; i < count; i++)
	{
		trig=dynamic_cast<Trigger *>(tab->getTrigger(i));
		if(trig->getReferencedTable())
			getObjectDependecies(trig->getReferencedTable(), deps, inc_indirect_deps);

		if(trig->getFunction())
			getObjectDependecies(trig->getFunction(), deps, inc_indirect_deps);
	}

	if(ftable)
	{
		getObjectDependecies(ftable->getForeignServer(), deps, inc_indirect_deps);
	}

	if(aux_tab)
	{
		count=aux_tab->getIndexCount();
		for(i=0; i < count; i++)
		{
			index=dynamic_cast<Index *>(aux_tab->getIndex(i));
			count1=index->getIndexElementCount();

			for(i1=0; i1 < count1; i1++)
			{
				if(index->getIndexElement(i1).getOperatorClass())
					getObjectDependecies(index->getIndexElement(i1).getOperatorClass(), deps, inc_indirect_deps);

				if(index->getIndexElement(i1).getColumn())
				{
					usr_type=getObjectPgSQLType(index->getIndexElement(i1).getColumn()->getType());

					if(usr_type)
						getObjectDependecies(usr_type, deps, inc_indirect_deps);
				}

				if(index->getIndexElement(i1).getCollation())
					getObjectDependecies(index->getIndexElement(i1).getCollation(), deps, inc_indirect_deps);
			}
		}

		count=aux_tab->getPolicyCount();
		for(i=0; i < count; i++)
		{
			pol=dynamic_cast<Policy *>(aux_tab->getPolicy(i));

			for(auto role : pol->getRoles())
				getObjectDependecies(role, deps, inc_indirect_deps);
		}
	}
}

void DatabaseModel::getTypeDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	Type *usr_type=dynamic_cast<Type *>(object);
	BaseObject *aux_type=nullptr;
	unsigned count, i;

	if(usr_type->getConfiguration()==Type::BaseType)
	{
		aux_type=getObjectPgSQLType(usr_type->getLikeType());

		if(aux_type)
			getObjectDependecies(aux_type, deps, inc_indirect_deps);

		for(i=Type::InputFunc; i <= Type::AnalyzeFunc; i++)
			getObjectDependecies(usr_type->getFunction(i), deps, inc_indirect_deps);
	}
	else if(usr_type->getConfiguration()==Type::CompositeType)
	{
		count=usr_type->getAttributeCount();
		for(i=0; i < count; i++)
		{
			aux_type=getObjectPgSQLType(usr_type->getAttribute(i).getType());

			if(aux_type)
				getObjectDependecies(aux_type, deps, inc_indirect_deps);
		}
	}
}

void DatabaseModel::getViewDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	View *view=dynamic_cast<View *>(object);
	unsigned i, count;

	count=view->getReferenceCount();
	for(i=0; i < count; i++)
	{
		if(view->getReference(i).getTable())
			getObjectDependecies(view->getReference(i).getTable(), deps, inc_indirect_deps);
	}

	for(i=0; i < view->getTriggerCount(); i++)
		getObjectDependecies(view->getTrigger(i), deps, inc_indirect_deps);

	for(i=0; i < view->getTriggerCount(); i++)
	{
		if(view->getTrigger(i)->getReferencedTable())
			getObjectDependecies(view->getTrigger(i)->getReferencedTable(), deps, inc_indirect_deps);
	}
}

void DatabaseModel::getGenericSQLDependencies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	GenericSQL *generic_sql = dynamic_cast<GenericSQL *>(object);
	vector<BaseObject *> ref_objs = generic_sql->getReferencedObjects();
	for(auto &obj : ref_objs)
		getObjectDependecies(obj, deps, inc_indirect_deps);
}

void DatabaseModel::getObjectDependecies(BaseObject *object, vector<BaseObject *> &deps, bool inc_indirect_deps)
{
	//Case the object is allocated and is not included in the dependecies list
	if(object && std::find(deps.begin(), deps.end(), object)==deps.end())
	{
		deps.push_back(object);

		if((deps.size()==1 && !inc_indirect_deps) || inc_indirect_deps)
		{
			ObjectType obj_type=object->getObjectType();

			if(object->getSchema())
				getObjectDependecies(object->getSchema(), deps, inc_indirect_deps);

			if(object->getTablespace())
				getObjectDependecies(object->getTablespace(), deps, inc_indirect_deps);

			if(object->getOwner())
				getObjectDependecies(object->getOwner(), deps, inc_indirect_deps);

			if(object->getCollation())
				getObjectDependecies(object->getCollation(), deps, inc_indirect_deps);

			if(obj_type==ObjectType::OpClass)
				getOpClassDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Domain)
				getDomainDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Conversion)
				getObjectDependecies(dynamic_cast<Conversion *>(object)->getConversionFunction(), deps, inc_indirect_deps);

			if(obj_type==ObjectType::Cast)
				getCastDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::EventTrigger)
				getObjectDependecies(dynamic_cast<EventTrigger *>(object)->getFunction(), deps, inc_indirect_deps);

			if(obj_type==ObjectType::Function)
				getFunctionDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Aggregate)
				getAggregateDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Language)
				getLanguageDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Operator)
				getOperatorDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Role)
				getRoleDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Relationship)
				getRelationshipDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Sequence)
				getSequenceDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Column)
				getColumnDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Trigger)
				getTriggerDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Index)
				getIndexDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Policy)
				getPolicyDependencies(object, deps, inc_indirect_deps);

			if(PhysicalTable::isPhysicalTable(obj_type))
				getPhysicalTableDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::Type)
				getTypeDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::View)
			 getViewDependencies(object, deps, inc_indirect_deps);

			if(obj_type == ObjectType::ForeignDataWrapper)
			{
				ForeignDataWrapper *fdw = dynamic_cast<ForeignDataWrapper *>(object);
				getObjectDependecies(fdw->getHandlerFunction(), deps, inc_indirect_deps);
				getObjectDependecies(fdw->getValidatorFunction(), deps, inc_indirect_deps);
			}

			if(obj_type == ObjectType::ForeignServer)
				getObjectDependecies(dynamic_cast<ForeignServer *>(object)->getForeignDataWrapper(), deps, inc_indirect_deps);

			if(obj_type==ObjectType::GenericSql)
				getGenericSQLDependencies(object, deps, inc_indirect_deps);

			if(obj_type==ObjectType::UserMapping)
				getObjectDependecies(dynamic_cast<UserMapping *>(object)->getForeignServer(), deps, inc_indirect_deps);

			if(BaseTable::isBaseTable(obj_type))
			{
				BaseTable *tab = dynamic_cast<BaseTable *>(object);

				if(tab->getTag())
					deps.push_back(tab->getTag());
			}
		}
	}
}

void DatabaseModel::getViewReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode)
{
	View *view=dynamic_cast<View *>(object);
	vector<BaseObject *> tab_objs=view->getObjects();
	refs.insert(refs.end(), tab_objs.begin(), tab_objs.end());

	if(!exclusion_mode)
	{
		vector<BaseRelationship *> base_rels=getRelationships(view);
		while(!base_rels.empty())
		{
			refs.push_back(base_rels.back());
			base_rels.pop_back();
		}
	}
}

void DatabaseModel::getPhysicalTableReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	PhysicalTable *table=dynamic_cast<PhysicalTable *>(object);
	ObjectType obj_type = object->getObjectType();
	Sequence *seq=nullptr;
	Constraint *constr=nullptr;
	PhysicalTable *tab=nullptr;
	Trigger *gat=nullptr;
	BaseRelationship *base_rel=nullptr;
	View *view=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<TableObject *> *tab_objs;
	unsigned i, count;
	vector<ObjectType> tab_obj_types={ ObjectType::Trigger, ObjectType::Rule, ObjectType::Index, ObjectType::Policy };
	vector<BaseObject *> tabs;

	for(auto &type : tab_obj_types)
	{
		if(obj_type == ObjectType::ForeignTable && type != ObjectType::Trigger)
			continue;

		tab_objs=table->getObjectList(type);
		refs.insert(refs.end(), tab_objs->begin(), tab_objs->end());
	}

	itr=relationships.begin();
	itr_end=relationships.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		base_rel=dynamic_cast<BaseRelationship *>(*itr);
		if(base_rel->getTable(BaseRelationship::SrcTable)==table ||
				base_rel->getTable(BaseRelationship::DstTable)==table)
		{
			refer=true;
			refs.push_back(base_rel);
		}
		itr++;
	}

	itr=base_relationships.begin();
	itr_end=base_relationships.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		base_rel=dynamic_cast<BaseRelationship *>(*itr);
		if(base_rel->getRelationshipType()==BaseRelationship::RelationshipFk &&
				(base_rel->getTable(BaseRelationship::SrcTable)==table ||
				 base_rel->getTable(BaseRelationship::DstTable)==table))
		{
			refer=true;
			refs.push_back(base_rel);
		}
		itr++;
	}

	itr=sequences.begin();
	itr_end=sequences.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		seq=dynamic_cast<Sequence *>(*itr);
		if(seq->getOwnerColumn() &&
				seq->getOwnerColumn()->getParentTable()==table)
		{
			refer=true;
			refs.push_back(seq);
		}

		itr++;
	}

	tabs = tables;
	tabs.insert(tabs.end(), foreign_tables.begin(), foreign_tables.end());
	itr= tabs.begin();
	itr_end = tabs.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		tab = dynamic_cast<PhysicalTable *>(*itr);

		count=tab->getConstraintCount();
		for(i=0; i < count&& (!exclusion_mode || (exclusion_mode && !refer)); i++)
		{
			constr=tab->getConstraint(i);
			//If a constraint references its own parent table it'll not be included on the references list
			if(constr->getConstraintType()==ConstraintType::ForeignKey &&
					constr->getParentTable()!=constr->getReferencedTable() &&
					constr->getReferencedTable()==table)
			{
				refer=true;
				refs.push_back(constr);
			}
		}

		count=tab->getTriggerCount();
		for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
		{
			gat=tab->getTrigger(i);
			if(gat->getReferencedTable()==table)
			{
				refer=true;
				refs.push_back(gat);
			}
		}

		itr++;
	}

	itr=views.begin();
	itr_end=views.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		view=dynamic_cast<View *>(*itr);

		if(view->isReferencingTable(table))
		{
			refer=true;
			refs.push_back(view);
		}

		itr++;
	}

	/* As base relationship are created automatically by the model they aren't considered
	as a reference to the table in exclusion mode */
	itr=base_relationships.begin();
	itr_end=base_relationships.end();

	while(itr!=itr_end && !exclusion_mode)// || (exclusion_mode && !refer)))
	{
		base_rel=dynamic_cast<BaseRelationship *>(*itr);

		if(base_rel->getTable(BaseRelationship::SrcTable)==table ||
				base_rel->getTable(BaseRelationship::DstTable)==table)
		{
			refer=true;
			refs.push_back(base_rel);
		}
		itr++;
	}
}

void DatabaseModel::getFunctionReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	Function *func=dynamic_cast<Function *>(object);
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType obj_types[]={ObjectType::Cast, ObjectType::EventTrigger, ObjectType::Conversion,
													ObjectType::Aggregate, ObjectType::Operator, ObjectType::OpClass,
													ObjectType::Table, ObjectType::Type, ObjectType::Language,
													ObjectType::ForeignDataWrapper, ObjectType::ForeignTable };
	unsigned i, i1, count, cnt=sizeof(obj_types)/sizeof(ObjectType);
	PhysicalTable *tab=nullptr;
	Aggregate *aggreg=nullptr;
	Operator *oper=nullptr;
	Trigger *trig=nullptr;
	Type *type=nullptr;
	Language *lang=nullptr;
	OperatorClass *opclass=nullptr;
	ForeignDataWrapper *fdw=nullptr;

	for(i=0; i < cnt && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		if(obj_types[i]==ObjectType::Cast)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				if(dynamic_cast<Cast *>(*itr)->getCastFunction()==func)
				{
					refer=true;
					refs.push_back(*itr);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::EventTrigger)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				if(dynamic_cast<EventTrigger *>(*itr)->getFunction()==func)
				{
					refer=true;
					refs.push_back(*itr);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::Conversion)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				if(dynamic_cast<Conversion *>(*itr)->getConversionFunction()==func)
				{
					refer=true;
					refs.push_back(*itr);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::Aggregate)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				aggreg=dynamic_cast<Aggregate *>(*itr);

				if(aggreg->getFunction(Aggregate::FinalFunc)==func ||
						aggreg->getFunction(Aggregate::TransitionFunc)==func)
				{
					refer=true;
					refs.push_back(aggreg);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::Operator)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				oper=dynamic_cast<Operator *>(*itr);

				if(oper->getFunction(Operator::FuncOperator)==func ||
						oper->getFunction(Operator::FuncJoin)==func  ||
						oper->getFunction(Operator::FuncRestrict)==func)
				{
					refer=true;
					refs.push_back(oper);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::OpClass)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				opclass=dynamic_cast<OperatorClass *>(*itr);
				count=opclass->getElementCount();

				for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					if(opclass->getElement(i1).getFunction()==func)
					{
						refer=true;
						refs.push_back(opclass);
					}
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::Table ||
						obj_types[i]==ObjectType::ForeignTable)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				tab=dynamic_cast<PhysicalTable *>(*itr);
				itr++;
				count=tab->getTriggerCount();

				for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					trig=tab->getTrigger(i1);

					if(trig->getFunction()==func)
					{
						refer=true;
						refs.push_back(trig);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Type)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				type=dynamic_cast<Type *>(*itr);
				itr++;

				for(i1=Type::InputFunc; i1 <= Type::AnalyzeFunc && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					if(type->getFunction(i1)==func)
					{
						refer=true;
						refs.push_back(type);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Language)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				lang=dynamic_cast<Language *>(*itr);
				itr++;

				if(lang->getFunction(Language::HandlerFunc)==func ||
						lang->getFunction(Language::ValidatorFunc)==func ||
						lang->getFunction(Language::InlineFunc)==func)
				{
					refer=true;
					refs.push_back(lang);
				}
			}
		}
		else if(obj_types[i]==ObjectType::ForeignDataWrapper)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				fdw=dynamic_cast<ForeignDataWrapper *>(*itr);
				itr++;

				if(fdw->getHandlerFunction() == func || fdw->getValidatorFunction() == func)
				{
					refer=true;
					refs.push_back(fdw);
				}
			}
		}
	}
}

void DatabaseModel::getSchemaReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType obj_types[12]={ObjectType::Function, ObjectType::Table, ObjectType::ForeignTable, ObjectType::View,
														ObjectType::Domain, ObjectType::Aggregate, ObjectType::Operator,
														ObjectType::Sequence, ObjectType::Conversion,
														ObjectType::Type, ObjectType::OpFamily, ObjectType::OpClass};
	unsigned i;

	for(i=0; i < 12 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			if((*itr)->getSchema()==object)
			{
				refer=true;
				refs.push_back(*itr);
			}
			itr++;
		}
	}
}

void DatabaseModel::getUserDefTypesReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType obj_types[]={ObjectType::Table, ObjectType::ForeignTable, ObjectType::OpClass,
													ObjectType::Cast,	ObjectType::Domain, ObjectType::Function, ObjectType::Aggregate,
													ObjectType::Operator, ObjectType::Type, ObjectType::Relationship };
	unsigned i, i1, count, tp_count = sizeof(obj_types)/sizeof(ObjectType);
	OperatorClass *op_class=nullptr;
	OperatorClassElement elem;
	PhysicalTable *tab=nullptr;
	Column *col=nullptr;
	Cast *cast=nullptr;
	Domain *dom=nullptr;
	Function *func=nullptr;
	Aggregate *aggreg=nullptr;
	Operator *oper=nullptr;
	Type *type=nullptr;
	Relationship *rel=nullptr;
	void *ptr_pgsqltype=nullptr;
	ObjectType obj_type = object->getObjectType();

	switch(obj_type)
	{
		case ObjectType::Type:
			ptr_pgsqltype=dynamic_cast<Type*>(object);
		break;
		case ObjectType::Domain:
			ptr_pgsqltype=dynamic_cast<Domain*>(object);
		break;
		case ObjectType::Sequence:
			ptr_pgsqltype=dynamic_cast<Sequence*>(object);
		break;
		case ObjectType::Extension:
			ptr_pgsqltype=dynamic_cast<Extension*>(object);
		break;
		case ObjectType::View:
			ptr_pgsqltype=dynamic_cast<View*>(object);
		break;
		case ObjectType::ForeignTable:
			ptr_pgsqltype=dynamic_cast<ForeignTable*>(object);
		break;
		default:
			ptr_pgsqltype=dynamic_cast<Table*>(object);
		break;
	}

	for(i=0; i < tp_count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		if(obj_types[i]==ObjectType::Relationship)
		{
			bool added;

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				added=false;
				rel=dynamic_cast<Relationship *>(*itr);
				itr++;

				count=rel->getAttributeCount();
				for(i1=0; i1 < count && !added; i1++)
				{
					col=rel->getAttribute(i1);

					if(col->getType()==ptr_pgsqltype)
					{
						added=refer=true;
						refs.push_back(rel);
					}
				}
			}
		}
		else if(PhysicalTable::isPhysicalTable(obj_types[i]))
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				tab=dynamic_cast<PhysicalTable *>(*itr);
				itr++;

				count=tab->getColumnCount();
				for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					col=tab->getColumn(i1);

					if(!col->isAddedByRelationship() &&
						 (col->getType()==ptr_pgsqltype ||
							//Special case for postgis extension
							(obj_type == ObjectType::Extension && object->getName() == QString("postgis") && col->getType().isGiSType())))
					{
						refer=true;
						refs.push_back(col);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::OpClass)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				op_class=dynamic_cast<OperatorClass *>(*itr);
				itr++;

				if(op_class->getDataType()==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(op_class);
				}

				for(i1=0; i1 < op_class->getElementCount() && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					elem=op_class->getElement(i1);
					if(elem.getStorage()==ptr_pgsqltype)
					{
						refer=true;
						refs.push_back(op_class);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Domain)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				dom=dynamic_cast<Domain *>(*itr);
				itr++;

				if(dom->getType()==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(dom);
				}
			}
		}
		else if(obj_types[i]==ObjectType::Type)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				type=dynamic_cast<Type *>(*itr);
				itr++;

				if(type->getAlignment()==ptr_pgsqltype ||
						type->getElement()==ptr_pgsqltype ||
						type->getLikeType()==ptr_pgsqltype ||
						type->getSubtype()==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(type);
				}
			}
		}
		else if(obj_types[i]==ObjectType::Aggregate)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				aggreg=dynamic_cast<Aggregate *>(*itr);
				itr++;

				count=aggreg->getDataTypeCount();
				for(i1=0; i1 < count  && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					if(aggreg->getDataType(i1)==ptr_pgsqltype)
					{
						refer=true;
						refs.push_back(aggreg);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Function)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				func=dynamic_cast<Function *>(*itr);
				itr++;

				if(func->getReturnType()==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(func);
				}
				else
				{
					count=func->getParameterCount();
					for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
					{
						if(func->getParameter(i1).getType()==ptr_pgsqltype)
						{
							refer=true;
							refs.push_back(func);
						}
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Operator)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				oper=dynamic_cast<Operator *>(*itr);
				itr++;

				if(oper->getArgumentType(Operator::LeftArg)==ptr_pgsqltype ||
						oper->getArgumentType(Operator::RightArg)==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(oper);
				}
			}
		}
		else if(obj_types[i]==ObjectType::Cast)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				cast=dynamic_cast<Cast *>(*itr);
				itr++;

				if(cast->getDataType(Cast::SrcType)==ptr_pgsqltype ||
						cast->getDataType(Cast::DstType)==ptr_pgsqltype)
				{
					refer=true;
					refs.push_back(cast);
				}
			}
		}
	}
}

void DatabaseModel::getRoleReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<ObjectType> obj_types = {
								ObjectType::Function, ObjectType::Table, ObjectType::ForeignTable, ObjectType::Domain,
								ObjectType::Aggregate, ObjectType::Schema, ObjectType::Operator,
								ObjectType::Sequence, ObjectType::Conversion,
								ObjectType::Language, ObjectType::Tablespace,
								ObjectType::Type, ObjectType::OpFamily, ObjectType::OpClass,
								ObjectType::UserMapping };
	vector<ObjectType>::iterator itr_tp, itr_tp_end;
	unsigned i,i1, count;
	Role *role_aux=nullptr;
	Role *role=dynamic_cast<Role *>(object);
	unsigned role_types[3]={Role::RefRole, Role::MemberRole, Role::AdminRole};
	Permission *perm=nullptr;

	//Check if the role is being referencend by permissions
	itr=permissions.begin();
	itr_end=permissions.end();
	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		perm=dynamic_cast<Permission *>(*itr);
		itr++;

		if(perm->isRoleExists(role))
		{
			refer=true;
			refs.push_back(perm);
		}
	}

	//Check if the role is being referenced in other roles
	itr=roles.begin();
	itr_end=roles.end();
	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		role_aux=dynamic_cast<Role *>(*itr);
		itr++;

		for(i1=0; i1 < 3 && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
		{
			count=role_aux->getRoleCount(role_types[i1]);
			for(i=0; i < count && !refer; i++)
			{
				if(role_aux->getRole(role_types[i1], i)==role)
				{
					refer=true;
					refs.push_back(role_aux);
				}
			}
		}
	}

	itr_tp = obj_types.begin();
	itr_tp_end = obj_types.end();

	while(itr_tp != itr_tp_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		obj_list=getObjectList(*itr_tp);
		itr_tp++;

		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			if((*itr)->getOwner()==role)
			{
				refer=true;
				refs.push_back(*itr);
			}

			if((*itr)->getObjectType() == ObjectType::Table)
			{
				for(auto obj : *(dynamic_cast<Table *>(*itr))->getObjectList(ObjectType::Policy))
				{
					if(dynamic_cast<Policy *>(obj)->isRoleExists(role))
					{
						refer=true;
						refs.push_back(obj);
					}
				}
			}

			itr++;
		}
	}

	//Special case: check if the role to be removed is the owner of the database
	if((!exclusion_mode || (exclusion_mode && !refer)) && this->getOwner()==role)
	{
		refer=true;
		refs.push_back(this);
	}
}

void DatabaseModel::getTablespaceReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	unsigned i, count;
	Table *tab=nullptr;
	Index *ind=nullptr;
	Constraint *rest=nullptr;

	itr=tables.begin();
	itr_end=tables.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		tab=dynamic_cast<Table *>(*itr);

		if(tab->getTablespace()==object)
		{
			refer=true;
			refs.push_back(tab);
		}

		count=tab->getIndexCount();
		for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
		{
			ind=tab->getIndex(i);
			if(ind->getTablespace()==object)
			{
				refer=true;
				refs.push_back(ind);
			}
		}

		count=tab->getConstraintCount();
		for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
		{
			rest=tab->getConstraint(i);
			if(rest->getTablespace()==object)
			{
				refer=true;
				refs.push_back(rest);
			}
		}

		itr++;
	}

	if((!exclusion_mode || (exclusion_mode && !refer)) && this->BaseObject::getTablespace()==object)
	{
		refer=true;
		refs.push_back(this);
	}
}

void DatabaseModel::getLanguageReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	Function *func=nullptr;

	itr=functions.begin();
	itr_end=functions.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		func=dynamic_cast<Function *>(*itr);
		if(func->getLanguage()==object)
		{
			refer=true;
			refs.push_back(func);
		}
		itr++;
	}
}

void DatabaseModel::getOpClassReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	Type *usertype=nullptr;
	Index *ind=nullptr;
	Constraint *constr=nullptr;
	Table *table=nullptr;
	ForeignTable *ftable=nullptr;

	itr=types.begin();
	itr_end=types.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		usertype=dynamic_cast<Type *>(*itr);

		if(usertype->getSubtypeOpClass()==object)
		{
			refer=true;
			refs.push_back(usertype);
		}
		itr++;
	}

	itr=tables.begin();
	itr_end=tables.end();
	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		table=dynamic_cast<Table *>(*itr);

		//Checking if the indexes are referencing the operator class
		for(unsigned idx=0; idx < table->getIndexCount() &&
			(!exclusion_mode || (exclusion_mode && !refer)); idx++)
		{
			ind=table->getIndex(idx);

			for(unsigned id_elem=0; id_elem < ind->getIndexElementCount() &&
				(!exclusion_mode || (exclusion_mode && !refer)); id_elem++)
			{
				if(ind->getIndexElement(id_elem).getOperatorClass()==object)
				{
					refer=true;
					refs.push_back(ind);
				}
			}
		}

		//Checking if the constraints are referencing the operator class
		for(unsigned idx=0; idx < table->getConstraintCount() &&
			(!exclusion_mode || (exclusion_mode && !refer)); idx++)
		{
			constr=table->getConstraint(idx);

			for(unsigned id_elem=0; id_elem < constr->getExcludeElementCount() &&
				(!exclusion_mode || (exclusion_mode && !refer)); id_elem++)
			{
				if(constr->getExcludeElement(id_elem).getOperatorClass()==object)
				{
					refer=true;
					refs.push_back(constr);
				}
			}
		}

		//Checking if the partition keys are referencing the operator class
		for(auto &part_key : table->getPartitionKeys())
		{
			if(part_key.getOperatorClass() == object)
			{
				refer = true;
				refs.push_back(table);
				break;
			}
		}

		itr++;
	}


	itr=foreign_tables.begin();
	itr_end=foreign_tables.end();
	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		ftable=dynamic_cast<ForeignTable *>(*itr);

		//Checking if the partition keys are referencing the operator class
		for(auto &part_key : ftable->getPartitionKeys())
		{
			if(part_key.getOperatorClass() == object)
			{
				refer = true;
				refs.push_back(ftable);
				break;
			}
		}

		itr++;
	}
}

void DatabaseModel::getOperatorReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType obj_types[]={ObjectType::OpClass,
													ObjectType::Aggregate,
													ObjectType::Operator,
													ObjectType::Table };
	unsigned i, i1, count;
	OperatorClass *op_class=nullptr;
	Operator *oper_aux=nullptr, *oper=dynamic_cast<Operator *>(object);
	Table *table=nullptr;
	Constraint *constr=nullptr;

	for(i=0; i < 4 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		if(obj_types[i]==ObjectType::OpClass)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				op_class=dynamic_cast<OperatorClass *>(*itr);
				itr++;

				count=op_class->getElementCount();
				for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					if(op_class->getElement(i1).getOperator()==oper)
					{
						refer=true;
						refs.push_back(op_class);
					}
				}
			}
		}
		else if(obj_types[i]==ObjectType::Aggregate)
		{
			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				if(dynamic_cast<Aggregate *>(*itr)->getSortOperator()==oper)
				{
					refer=true;
					refs.push_back(*itr);
				}
				itr++;
			}
		}
		else if(obj_types[i]==ObjectType::Operator)
		{
			while(itr!=itr_end && !refer)
			{
				oper_aux=dynamic_cast<Operator *>(*itr);
				itr++;

				for(i1=Operator::OperCommutator; i1 <= Operator::OperNegator &&
					(!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					if(oper_aux->getOperator(i1)==oper)
					{
						refer=true;
						refs.push_back(oper_aux);
					}
				}
			}
		}
		else
		{
			while(itr!=itr_end && !refer)
			{
				table=dynamic_cast<Table *>(*itr);
				itr++;

				count=table->getConstraintCount();
				for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
				{
					constr=table->getConstraint(i1);

					if(constr->getConstraintType()==ConstraintType::Exclude)
					{
						for(auto &elem : constr->getExcludeElements())
						{
							if(elem.getOperator()==oper)
							{
								refer=true;
								refs.push_back(constr);
								if(exclusion_mode) break;
							}
						}
					}
				}
			}
		}
	}
}

void DatabaseModel::getCollationReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	ObjectType  obj_types[]={ ObjectType::Domain, ObjectType::Collation, ObjectType::Type },
			tab_obj_types[]={ ObjectType::Column, ObjectType::Index };
	unsigned i, count;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<TableObject *> *tab_obj_list=nullptr;
	vector<TableObject *>::iterator tab_itr, tab_itr_end;
	TableObject *tab_obj=nullptr;
	PhysicalTable *table = nullptr;

	count=sizeof(obj_types)/sizeof(ObjectType);
	for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			if((*itr)->getCollation()==object)
			{
				refer=true;
				refs.push_back(*itr);
			}

			itr++;
		}
	}

	count=sizeof(tab_obj_types)/sizeof(ObjectType);
	vector<BaseObject *> tabs;

	tabs.insert(tabs.end(), tables.begin(), tables.end());
	tabs.insert(tabs.end(), foreign_tables.begin(), foreign_tables.end());
	itr=tabs.begin();
	itr_end=tabs.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		table = dynamic_cast<PhysicalTable *>(*itr);

		for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
		{
			tab_obj_list = table->getObjectList(tab_obj_types[i]);
			if(!tab_obj_list) continue;

			tab_itr=tab_obj_list->begin();
			tab_itr_end=tab_obj_list->end();

			while(tab_itr!=tab_itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				tab_obj=(*tab_itr);
				if((tab_obj->getObjectType()==ObjectType::Column && tab_obj->getCollation()==object) ||
						(tab_obj->getObjectType()==ObjectType::Index &&
						 dynamic_cast<Index *>(tab_obj)->isReferCollation(dynamic_cast<Collation *>(object))))
				{
					refer=true;
					refs.push_back(*tab_itr);
				}
				tab_itr++;
			}
		}

		//Checking if the partition keys are referencing the operator class
		for(auto &part_key : table->getPartitionKeys())
		{
			if(part_key.getCollation() == object)
			{
				refer = true;
				refs.push_back(table);
				break;
			}
		}

		itr++;
	}
}

void DatabaseModel::getOpFamilyReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	OperatorFamily *op_family=dynamic_cast<OperatorFamily *>(object);

	itr=op_classes.begin();
	itr_end=op_classes.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<OperatorClass *>(*itr)->getFamily()==op_family)
		{
			refer=true;
			refs.push_back(*itr);
		}
		itr++;
	}
}

void DatabaseModel::getColumnReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	Column *column=dynamic_cast<Column *>(object);
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType  obj_types[]={ ObjectType::Sequence, ObjectType::View, ObjectType::Table,
														ObjectType::ForeignTable, ObjectType::Relationship };
	unsigned i, count=sizeof(obj_types)/sizeof(ObjectType);

	for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		obj_list=getObjectList(obj_types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			if((obj_types[i]==ObjectType::Sequence && dynamic_cast<Sequence *>(*itr)->getOwnerColumn()==column) ||
					(obj_types[i]==ObjectType::View && dynamic_cast<View *>(*itr)->isReferencingColumn(column)))
			{
				refer=true;
				refs.push_back(*itr);
			}
			else if(obj_types[i]==ObjectType::Table || obj_types[i]==ObjectType::ForeignTable)
			{
				PhysicalTable *tab=dynamic_cast<PhysicalTable *>(*itr);
				Table *aux_tab = dynamic_cast<Table *>(tab);
				unsigned count, idx, count1, i1;
				Trigger *trig=nullptr;
				Index *index=nullptr;
				Constraint *constr=nullptr;
				vector<PartitionKey> part_keys;

				count=tab->getConstraintCount();
				for(idx=0; idx < count && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
				{
					constr=tab->getConstraint(idx);
					if(constr->isColumnReferenced(column))
					{
						refer=true;
						refs.push_back(constr);
					}
				}

				if(aux_tab)
				{
					count=aux_tab->getIndexCount();
					for(idx=0; idx < count && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
					{
						index=aux_tab->getIndex(idx);
						if(index->isReferColumn(column))
						{
							refer=true;
							refs.push_back(index);
						}
					}
				}

				count=tab->getTriggerCount();
				for(idx=0; idx < count && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
				{
					trig=tab->getTrigger(idx);
					count1=trig->getColumnCount();

					for(i1=0; i1 < count1 && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
					{
						if(trig->getColumn(i1)==column)
						{
							refer=true;
							refs.push_back(trig);
						}
					}
				}

				part_keys = tab->getPartitionKeys();
				for(auto &part_key : part_keys)
				{
					if(part_key.getColumn() == column)
					{
						refer = true;
						refs.push_back(tab);
						break;
					}
				}
			}
			else if(obj_types[i]==ObjectType::Relationship)
			{
				Relationship *rel=dynamic_cast<Relationship *>(*itr);
				unsigned constr_cnt, idx;

				constr_cnt=rel->getConstraintCount();
				for(idx=0; idx < constr_cnt && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
				{
					if(rel->getConstraint(idx)->isColumnReferenced(column))
					{
						refer=true;
						refs.push_back(rel);
					}
				}
			}

			itr++;
		}
	}
}

void DatabaseModel::getTagReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> list;
	Tag *tag=dynamic_cast<Tag *>(object);

	list.assign(tables.begin(), tables.end());
	list.insert(list.end(), foreign_tables.begin(), foreign_tables.end());
	list.insert(list.end(), views.begin(), views.end());

	itr=list.begin();
	itr_end=list.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<BaseTable *>(*itr)->getTag()==tag)
		{
			refer=true;
			refs.push_back(*itr);
		}
		itr++;
	}
}

void DatabaseModel::getSequenceReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	PhysicalTable *table=nullptr;
	vector<TableObject *> *cols=nullptr;
	vector<TableObject *>::iterator itr, itr_end;
	vector<BaseObject *> tabs;
	unsigned i = 0, cnt = 0;

	tabs = tables;
	tabs.insert(tabs.end(), foreign_tables.begin(), foreign_tables.end());
	cnt = tabs.size();

	for(i=0; i < cnt && (!exclusion_mode || (exclusion_mode && !refer)); i++)
	{
		table=dynamic_cast<PhysicalTable *>(tabs[i]);
		cols=table->getObjectList(ObjectType::Column);
		itr=cols->begin();
		itr_end=cols->end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			if(dynamic_cast<Column *>(*itr)->getSequence()==object)
			{
				refer=true;
				refs.push_back(*itr);
			}
			itr++;
		}
	}
}

void DatabaseModel::getFdwReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> list;
	ForeignDataWrapper *fdw=dynamic_cast<ForeignDataWrapper *>(object);

	itr=foreign_servers.begin();
	itr_end=foreign_servers.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<ForeignServer *>(*itr)->getForeignDataWrapper() == fdw)
		{
			refer=true;
			refs.push_back(*itr);
		}
		itr++;
	}
}

void DatabaseModel::getServerReferences(BaseObject *object, vector<BaseObject *> &refs, bool &refer, bool exclusion_mode)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> list;
	ForeignServer *srv=dynamic_cast<ForeignServer *>(object);

	itr=usermappings.begin();
	itr_end=usermappings.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<UserMapping *>(*itr)->getForeignServer() == srv)
		{
			refer=true;
			refs.push_back(*itr);
		}
		itr++;
	}

	itr=foreign_tables.begin();
	itr_end=foreign_tables.end();

	while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<ForeignTable *>(*itr)->getForeignServer() == srv)
		{
			refer=true;
			refs.push_back(*itr);
		}
		itr++;
	}
}

void DatabaseModel::getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode, bool exclude_perms)
{
	refs.clear();

	if(!object)
		return;

	vector<BaseObject *>::iterator itr_perm, itr_perm_end;
	ObjectType obj_type=object->getObjectType();
	bool refer=false;
	Permission *perm=nullptr;

	if(!exclude_perms)
	{
		//Get the permissions thata references the object
		itr_perm=permissions.begin();
		itr_perm_end=permissions.end();

		while(itr_perm!=itr_perm_end && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			perm=dynamic_cast<Permission *>(*itr_perm);
			if(perm->getObject()==object)
			{
				refer=true;
				refs.push_back(perm);
			}
			itr_perm++;
		}
	}

	if(exclusion_mode && !refer && default_objs.count(obj_type) && default_objs[obj_type]==object)
	{
		refer=true;
		refs.push_back(this);
	}

	if(obj_type==ObjectType::View && (!exclusion_mode || (exclusion_mode && !refer)))
		getViewReferences(object, refs, exclusion_mode);

	if(PhysicalTable::isPhysicalTable(obj_type) && (!exclusion_mode || (exclusion_mode && !refer)))
		getPhysicalTableReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Function && (!exclusion_mode || (exclusion_mode && !refer)))
		getFunctionReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Schema && (!exclusion_mode || (exclusion_mode && !refer)))
		getSchemaReferences(object, refs, refer, exclusion_mode);

	if((obj_type==ObjectType::Type || obj_type==ObjectType::Domain || obj_type==ObjectType::Sequence ||
			obj_type==ObjectType::Extension || BaseTable::isBaseTable(obj_type))
			&& (!exclusion_mode || (exclusion_mode && !refer)))
		getUserDefTypesReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Role && (!exclusion_mode || (exclusion_mode && !refer)))
		getRoleReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Tablespace && (!exclusion_mode || (exclusion_mode && !refer)))
		getTablespaceReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Language && (!exclusion_mode || (exclusion_mode && !refer)))
		getLanguageReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::OpClass && (!exclusion_mode || (exclusion_mode && !refer)))
		getOpClassReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Operator && (!exclusion_mode || (exclusion_mode && !refer)))
		getOperatorReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::OpFamily && (!exclusion_mode || (exclusion_mode && !refer)))
		getOpFamilyReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Collation && (!exclusion_mode || (exclusion_mode && !refer)))
		getCollationReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Column && (!exclusion_mode || (exclusion_mode && !refer)))
		getColumnReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Tag && (!exclusion_mode || (exclusion_mode && !refer)))
		getTagReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::Sequence && (!exclusion_mode || (exclusion_mode && !refer)))
		getSequenceReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::ForeignDataWrapper && (!exclusion_mode || (exclusion_mode && !refer)))
		getFdwReferences(object, refs, refer, exclusion_mode);

	if(obj_type==ObjectType::ForeignServer && (!exclusion_mode || (exclusion_mode && !refer)))
		getServerReferences(object, refs, refer, exclusion_mode);

	// Checking if any generic SQL object is referencing the object passed
	vector<BaseObject *>::iterator itr = genericsqls.begin(),
			itr_end = genericsqls.end();

	while(itr != itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
	{
		if(dynamic_cast<GenericSQL *>(*itr)->isObjectReferenced(object))
		{
			refer = true;
			refs.push_back(*itr);
		}

		itr++;
	}
}

void DatabaseModel::__getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclude_perms)
{
	vector<BaseObject *> refs_aux;
	vector<BaseObject *>::iterator end;

	getObjectReferences(object, refs_aux, exclude_perms);

	if(!refs_aux.empty())
	{
		refs.insert(refs.end(), refs_aux.begin(), refs_aux.end());
		std::sort(refs.begin(), refs.end());
		end=std::unique(refs.begin(), refs.end());
		refs.erase(end, refs.end());

		for(BaseObject *obj : refs_aux)
			__getObjectReferences(obj, refs, exclude_perms);
	}
}

void DatabaseModel::setObjectsModified(vector<BaseObject *> &objects)
{
	for(auto &obj : objects)
	{
		if(BaseGraphicObject::isGraphicObject(obj->getObjectType()))
			dynamic_cast<BaseGraphicObject *>(obj)->setModified(true);
	}
}

void DatabaseModel::setObjectsModified(vector<ObjectType> types)
{
	ObjectType obj_types[]={ObjectType::Table, ObjectType::View, ObjectType::ForeignTable,
							ObjectType::Relationship, ObjectType::BaseRelationship,
							ObjectType::Textbox, ObjectType::Schema };
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> *obj_list=nullptr;
	Textbox *label=nullptr;
	BaseRelationship *rel=nullptr;
	unsigned i, i1, count=sizeof(obj_types)/sizeof(ObjectType);

	for(i=0; i < count; i++)
	{
		if(types.empty() || find(types.begin(), types.end(), obj_types[i])!=types.end())
		{
			obj_list=getObjectList(obj_types[i]);
			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end)
			{
				dynamic_cast<BaseGraphicObject *>(*itr)->setModified(true);

				//For relationships is needed to set the labels as modified too
				if(obj_types[i]==ObjectType::Relationship || obj_types[i]==ObjectType::BaseRelationship)
				{
					rel=dynamic_cast<BaseRelationship *>(*itr);
					for(i1=0; i1 < 3; i1++)
					{
						label=rel->getLabel(i1);
						if(label) label->setModified(true);
					}
				}

				itr++;
			}
		}
	}
}

void DatabaseModel::setCodesInvalidated(vector<ObjectType> types)
{
	vector<ObjectType> sel_types;
	vector<BaseObject *> *list=nullptr;

	if(types.empty())
		sel_types=BaseObject::getObjectTypes(false);
	else
	{
		ObjectType tab_obj_types[]={ObjectType::Column, ObjectType::Constraint,
									ObjectType::Trigger, ObjectType::Rule, ObjectType::Index, ObjectType::Policy};
		for(unsigned i=0; i < 6; i++)
			sel_types.erase(std::find(sel_types.begin(), sel_types.end(), tab_obj_types[i]));

		sel_types=types;
	}

	while(!sel_types.empty())
	{
		list=getObjectList(sel_types.back());
		sel_types.pop_back();

		if(list)
		{
			for(auto &obj : *list)
				obj->setCodeInvalidated(true);
		}
	}
}

BaseObject *DatabaseModel::getObjectPgSQLType(PgSqlType type)
{
	switch(type.getUserTypeConfig())
	{
		case UserTypeConfig::BaseType:
		return this->getObject(*type, ObjectType::Type);

		case UserTypeConfig::DomainType:
		return this->getObject(*type, ObjectType::Domain);

		case UserTypeConfig::TableType:
		return this->getObject(*type, ObjectType::Table);

		case UserTypeConfig::ViewType:
		return this->getObject(*type, ObjectType::View);

		case UserTypeConfig::SequenceType:
		return this->getObject(*type, ObjectType::Sequence);

		case UserTypeConfig::ExtensionType:
		return this->getObject(*type, ObjectType::Extension);

		default:
		return nullptr;
	}
}

void DatabaseModel::validateSchemaRenaming(Schema *schema, const QString &prev_sch_name)
{
	vector<ObjectType> types = { ObjectType::Table, ObjectType::ForeignTable, ObjectType::View,
															 ObjectType::Domain, ObjectType::Type, ObjectType::Sequence };
	vector<BaseObject *> list, sch_objs, refs;
	QString prev_name;

	//Raise an error if the schema is not allocated
	if(!schema)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Get all the objects on the informed schema
	for(auto &type : types)
	{
		list = getObjects(type, schema);
		sch_objs.insert(sch_objs.end(), list.begin(), list.end());
	}

	for(auto &obj : sch_objs)
	{
		//Configures the previous type name
		prev_name=BaseObject::formatName(prev_sch_name) + QString(".") +
							BaseObject::formatName(obj->getName(), false);

		/* Special case for tables. Need to make a dynamic_cast before the reinterpret_cast to get
		the correct reference to table */
		if(obj->getObjectType() == ObjectType::Table)
			PgSqlType::renameUserType(prev_name, reinterpret_cast<void *>(dynamic_cast<Table *>(obj)), obj->getName(true));
		else if(obj->getObjectType() == ObjectType::View)
			PgSqlType::renameUserType(prev_name, reinterpret_cast<void *>(dynamic_cast<View *>(obj)), obj->getName(true));
		else if(obj->getObjectType() == ObjectType::ForeignTable)
			PgSqlType::renameUserType(prev_name, reinterpret_cast<void *>(dynamic_cast<ForeignTable *>(obj)), obj->getName(true));
		else
			PgSqlType::renameUserType(prev_name, reinterpret_cast<void *>(obj), obj->getName(true));

		getObjectReferences(obj, refs);

		//For graphical objects set them as modified to redraw them
		if(BaseTable::isBaseTable(obj->getObjectType()))
			dynamic_cast<BaseGraphicObject *>(obj)->setModified(true);

		for(auto &ref_obj : refs)
		{
			if(BaseTable::isBaseTable(ref_obj->getObjectType()))
				dynamic_cast<BaseGraphicObject *>(ref_obj)->setModified(true);
			else if(TableObject::isTableObject(ref_obj->getObjectType()))
			{
				BaseTable *tab = dynamic_cast<TableObject *>(ref_obj)->getParentTable();
				tab->setModified(true);
				tab->setCodeInvalidated(true);
			}

			ref_obj->setCodeInvalidated(true);
		}

		refs.clear();
	}
}

void DatabaseModel::createSystemObjects(bool create_public)
{
	Schema *public_sch=nullptr, *pg_catalog=nullptr;
	Language *lang=nullptr;
	Tablespace *tbspace=nullptr;
	QStringList langs={ DefaultLanguages::C, DefaultLanguages::Sql,
											DefaultLanguages::PlPgsql, DefaultLanguages::Internal };
	Role *postgres=nullptr;
	Collation *collation=nullptr;
	QString collnames[]={ "default", "C", "POSIX" };

	/* The particular case is for public schema that is created only when the flag
	is set. This because the public schema is written on model file even being
	a system object. This strategy permits the user controls the schema rectangle behavior */
	if(create_public && getObjectIndex(QString("public"), ObjectType::Schema) < 0)
	{
		public_sch=new Schema;
		public_sch->setName(QString("public"));
		public_sch->setSystemObject(true);
		addSchema(public_sch);
	}

	//Create the pg_catalog schema in order to insert default collations in
	pg_catalog=new Schema;
	pg_catalog->BaseObject::setName(QString("pg_catalog"));
	pg_catalog->setSystemObject(true);
	addSchema(pg_catalog);

	//Creating default collations
	for(unsigned i=0; i < 3; i++)
	{
		collation=new Collation;
		collation->setName(collnames[i]);
		collation->setSchema(pg_catalog);
		collation->setEncoding(EncodingType(QString("UTF8")));
		collation->setLocale(QString("C"));
		collation->setSystemObject(true);
		addCollation(collation);
	}

	for(auto &lang_name : langs)
	{
		if(getObjectIndex(lang_name, ObjectType::Language) < 0)
		{
			lang = new Language;
			lang->BaseObject::setName(lang_name);
			lang->setSystemObject(true);
			addLanguage(lang);
		}
	}

	tbspace=new Tablespace;
	tbspace->BaseObject::setName(QString("pg_global"));
	tbspace->setDirectory(QString("_pg_global_dir_"));
	tbspace->setSystemObject(true);
	addTablespace(tbspace);

	tbspace=new Tablespace;
	tbspace->BaseObject::setName(QString("pg_default"));
	tbspace->setDirectory(QString("_pg_default_dir_"));
	tbspace->setSystemObject(true);
	addTablespace(tbspace);

	postgres=new Role;
	postgres->setName(QString("postgres"));
	postgres->setOption(Role::OpSuperuser, true);
	postgres->setSystemObject(true);
	addRole(postgres);

	setDefaultObject(postgres);
	setDefaultObject(getObject(QString("public"), ObjectType::Schema), ObjectType::Schema);
}

vector<BaseObject *> DatabaseModel::findObjects(const QString &pattern, vector<ObjectType> types, bool case_sensitive, bool is_regexp, bool exact_match, const QString &search_attr)
{
	vector<BaseObject *> list, objs;
	vector<BaseObject *>::iterator end;
	vector<ObjectType>::iterator itr_tp=types.begin();
	vector<BaseObject *> tables;
	bool inc_tabs=false, inc_views=false;
	ObjectType obj_type;
	QRegExp regexp;
	BaseObject *object = nullptr;
	attribs_map srch_attribs;

	//Configuring the regex style
	regexp.setPattern(pattern);
	regexp.setCaseSensitivity(case_sensitive ?  Qt::CaseSensitive :  Qt::CaseInsensitive);

	if(is_regexp)
		regexp.setPatternSyntax(QRegExp::RegExp2);
	else if(exact_match)
		regexp.setPatternSyntax(QRegExp::FixedString);
	else
		regexp.setPatternSyntax(QRegExp::Wildcard);

	//If there is some table object types on the type list, gather tables and views
	while(itr_tp!=types.end() && (!inc_views || !inc_tabs))
	{
		if(!inc_tabs && TableObject::isTableObject(*itr_tp))
		{
			tables.insert(tables.end(), getObjectList(ObjectType::Table)->begin(), getObjectList(ObjectType::Table)->end());
			tables.insert(tables.end(), getObjectList(ObjectType::ForeignTable)->begin(), getObjectList(ObjectType::ForeignTable)->end());
			inc_tabs=true;
		}

		if(!inc_views && ((*itr_tp)==ObjectType::Rule || (*itr_tp)==ObjectType::Trigger))
		{
			tables.insert(tables.end(), getObjectList(ObjectType::View)->begin(), getObjectList(ObjectType::View)->end());
			inc_views=true;
		}

		itr_tp++;
	}

	//Gathering all other objects
	itr_tp=types.begin();
	while(itr_tp!=types.end())
	{
		obj_type=(*itr_tp);
		itr_tp++;

		if(obj_type==ObjectType::Database)
			objs.push_back(this);
		else if(!TableObject::isTableObject(obj_type))
			objs.insert(objs.end(), getObjectList(obj_type)->begin(), getObjectList(obj_type)->end());
		else
		{
			//Including table object on the object list
			vector<TableObject *> *tab_objs=nullptr;
			vector<BaseObject *>::iterator itr=tables.begin();
			BaseObject *tab=nullptr;

			while(itr!=tables.end())
			{
				tab=(*itr);
				itr++;

				if(PhysicalTable::isPhysicalTable(tab->getObjectType()))
					tab_objs=dynamic_cast<PhysicalTable *>(tab)->getObjectList(obj_type);
				else if(tab->getObjectType()==ObjectType::View &&	(obj_type==ObjectType::Trigger || obj_type==ObjectType::Rule))
					tab_objs=dynamic_cast<View *>(tab)->getObjectList(obj_type);

				if(tab_objs)
					objs.insert(objs.end(), tab_objs->begin(), tab_objs->end());
			}
		}
	}

	//Try to find  the objects on the configured list
	while(!objs.empty())
	{
		object = objs.back();
		object->configureSearchAttributes();
		srch_attribs = object->getSearchAttributes();

		if((exact_match && pattern == srch_attribs[search_attr]) ||
			 (exact_match && regexp.exactMatch(srch_attribs[search_attr])) ||
			 (!exact_match && regexp.indexIn(srch_attribs[search_attr]) >= 0))
			list.push_back(object);

		objs.pop_back();
	}

	//Removing the duplicate items on the list
	std::sort(list.begin(), list.end());
	end=std::unique(list.begin(), list.end());
	list.erase(end, list.end());

	return list;
}

void DatabaseModel::setInvalidated(bool value)
{
	this->invalidated=value;
}

bool DatabaseModel::isInvalidated()
{
	return invalidated;
}

void  DatabaseModel::setAppendAtEOD(bool value)
{
	append_at_eod=value;
}

void DatabaseModel::setPrependAtBOD(bool value)
{
	prepend_at_bod=value;
}

void DatabaseModel::setDefaultObject(BaseObject *object, ObjectType obj_type)
{
	if((!object && default_objs.count(obj_type)==0) ||
			(object && default_objs.count(object->getObjectType())==0))
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!object)
		default_objs[obj_type]=nullptr;
	else
		default_objs[object->getObjectType()]=object;
}

void DatabaseModel::setIsTemplate(bool value)
{
	is_template = value;
}

void DatabaseModel::setAllowConnections(bool value)
{
	allow_conns = value;
}

bool  DatabaseModel::isAppendAtEOD()
{
	return append_at_eod;
}

bool DatabaseModel::isPrependedAtBOD()
{
	return prepend_at_bod;
}

bool DatabaseModel::isTemplate()
{
	return is_template;
}

bool DatabaseModel::isAllowConnections()
{
	return allow_conns;
}

void DatabaseModel::saveObjectsMetadata(const QString &filename, unsigned options)
{
	QFile output(filename);
	QByteArray buf;
	QString objs_def;
	vector<BaseObject *> objects, tab_objs;
	attribs_map attribs;
	BaseGraphicObject *graph_obj=nullptr;
	Relationship *rel=nullptr;
	PhysicalTable *tab_nn=nullptr;
	BaseTable *src_tab=nullptr, *dst_tab=nullptr, *base_tab=nullptr;
	Schema *schema=nullptr;
	QPointF pnt;
	ObjectType obj_type;
	int idx=0;
	bool save_db_attribs=false, save_objs_pos=false, save_objs_prot=false,
			save_objs_sqldis=false, save_textboxes=false, save_tags=false,
			save_custom_sql=false, save_custom_colors=false, save_fadeout=false,
			save_collapsemode=false, save_genericsqls=false, save_objs_aliases=false,
			save_objs_z_value=false;
	QStringList labels_attrs={ Attributes::SrcLabel,
														 Attributes::DstLabel,
														 Attributes::NameLabel };

	save_db_attribs=(MetaDbAttributes & options) == MetaDbAttributes;
	save_objs_pos=(MetaObjsPositioning & options) == MetaObjsPositioning;
	save_objs_prot=(MetaObjsProtection & options) == MetaObjsProtection;
	save_objs_sqldis=(MetaObjsSqlDisabled & options) == MetaObjsSqlDisabled;
	save_textboxes=(MetaTextboxObjs & options) == MetaTextboxObjs;
	save_tags=(MetaTagObjs & options) == MetaTagObjs;
	save_custom_sql=(MetaObjsCustomSql & options) == MetaObjsCustomSql;
	save_custom_colors=(MetaObjsCustomColors & options) == MetaObjsCustomColors;
	save_fadeout=(MetaObjsFadeOut & options) == MetaObjsFadeOut;
	save_collapsemode=(MetaObjsCollapseMode & options) == MetaObjsCollapseMode;
	save_genericsqls=(MetaGenericSqlObjs & options) == MetaGenericSqlObjs;
	save_objs_aliases=(MetaObjsAliases & options) == MetaObjsAliases;
	save_objs_z_value=(MetaObjsZStackValue & options) == MetaObjsZStackValue;

	output.open(QFile::WriteOnly);

	if(!output.isOpen())
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(filename),
										ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		if(save_textboxes || save_tags || save_genericsqls)
		{
			if(save_textboxes)
				objects.insert(objects.end(), textboxes.begin(), textboxes.end());

			if(save_tags)
				objects.insert(objects.end(), tags.begin(), tags.end());

			if(save_genericsqls)
				objects.insert(objects.end(), genericsqls.begin(), genericsqls.end());
		}

		if(save_db_attribs)
			objects.push_back(this);

		if(save_objs_pos || save_objs_aliases)
		{
			objects.insert(objects.end(), schemas.begin(), schemas.end());
			objects.insert(objects.end(), tables.begin(), tables.end());
			objects.insert(objects.end(), views.begin(), views.end());
			objects.insert(objects.end(), relationships.begin(), relationships.end());
			objects.insert(objects.end(), base_relationships.begin(), base_relationships.end());

			//Adding rel. n-n generated tables to the list of tables too
			for(BaseObject *object : relationships)
			{
				rel=dynamic_cast<Relationship *>(object);

				if(save_objs_pos && rel->getRelationshipType()==BaseRelationship::RelationshipNn && rel->getReceiverTable())
				{
					tab_nn=rel->getReceiverTable();
					src_tab=rel->getTable(BaseRelationship::SrcTable);
					dst_tab=rel->getTable(BaseRelationship::DstTable);

					//Since the generated table does not have a position we create one based upon the source tables  positions
					pnt.setX((src_tab->getPosition().x() + dst_tab->getPosition().x())/2.0);
					pnt.setY((src_tab->getPosition().y() + dst_tab->getPosition().y())/2.0);
					tab_nn->setPosition(pnt);

					objects.push_back(tab_nn);
				}
			}

			//Saving aliases the children of tables and views
			if(save_objs_aliases)
			{
				for(auto &tab : tables)
				{
					tab_objs = dynamic_cast<Table *>(tab)->getObjects();
					objects.insert(objects.end(), tab_objs.begin(), tab_objs.end());
				}

				for(auto &tab : foreign_tables)
				{
					tab_objs = dynamic_cast<ForeignTable *>(tab)->getObjects();
					objects.insert(objects.end(), tab_objs.begin(), tab_objs.end());
				}

				for(auto &vw : views)
				{
					tab_objs = dynamic_cast<View *>(vw)->getObjects();
					objects.insert(objects.end(), tab_objs.begin(), tab_objs.end());
				}
			}
		}

		if(save_objs_prot || save_objs_sqldis)
		{
			vector<ObjectType> types=getChildObjectTypes(ObjectType::Database), sch_types=getChildObjectTypes(ObjectType::Schema);
			types.insert(types.end(), sch_types.begin(), sch_types.end());

			//Removing the types for schema, table and view to avoid retrieving the objects twice
			if(save_objs_pos)
			{
				types.erase(std::find(types.begin(), types.end(), ObjectType::Schema));
				types.erase(std::find(types.begin(), types.end(), ObjectType::Table));
				types.erase(std::find(types.begin(), types.end(), ObjectType::View));
			}

			//Append the other objects to the list of selected entities
			for(ObjectType type : types)
				objects.insert(objects.end(), getObjectList(type)->begin(), getObjectList(type)->end());
		}

		for(BaseObject *object : objects)
		{
			obj_type=object->getObjectType();

			//When handling a tag , textbox or generic sql we just extract their XML code
			if(obj_type==ObjectType::Textbox || obj_type==ObjectType::Tag || obj_type == ObjectType::GenericSql)
			{
				emit s_objectLoaded(((idx++)/static_cast<double>(objects.size()))*100,
														tr("Saving object `%1' (%2)")
														.arg(object->getName()).arg(object->getTypeName()), enum_cast(obj_type));

				objs_def+=object->getCodeDefinition(SchemaParser::XmlDefinition);
				continue;
			}
			//Discarding the relationship added table objects (when extracting aliases)
			else if(TableObject::isTableObject(obj_type) && dynamic_cast<TableObject *>(object)->isAddedByRelationship())
				continue;

			graph_obj=dynamic_cast<BaseGraphicObject *>(object);
			base_tab=dynamic_cast<BaseTable *>(object);

			attribs[Attributes::Table]=QString();
			attribs[Attributes::Name]=(TableObject::isTableObject(obj_type) ? object->getName() : object->getSignature());
			attribs[Attributes::Alias]=(save_objs_aliases ? object->getAlias() : QString());
			attribs[Attributes::Type]=object->getSchemaName();
			attribs[Attributes::Protected]=(save_objs_prot && object->isProtected() && !object->isSystemObject() ? Attributes::True : QString());
			attribs[Attributes::SqlDisabled]=(save_objs_sqldis && object->isSQLDisabled() && !object->isSystemObject()  ? Attributes::True : QString());
			attribs[Attributes::Tag]=(save_tags && base_tab && base_tab->getTag() ? base_tab->getTag()->getName() : QString());
			attribs[Attributes::AppendedSql]=object->getAppendedSQL();
			attribs[Attributes::PrependedSql]=object->getPrependedSQL();			
			attribs[Attributes::FadedOut]=(save_fadeout && graph_obj && graph_obj->isFadedOut() ? Attributes::True : QString());
			attribs[Attributes::CollapseMode]=(save_collapsemode && base_tab ? QString::number(enum_cast(base_tab->getCollapseMode())) : QString());

			if(TableObject::isTableObject(obj_type))
			{
				base_tab = dynamic_cast<TableObject *>(object)->getParentTable();
				attribs[Attributes::Table]=base_tab->getSignature();
			}

			if(save_custom_sql && obj_type==ObjectType::Database)
			{
				attribs[Attributes::AppendAtEod]=(this->isAppendAtEOD() ? Attributes::True : Attributes::False);
				attribs[Attributes::PrependAtBod]=(this->isPrependedAtBOD() ? Attributes::True : Attributes::False);
			}

			//Configuring database model attributes
			if(save_db_attribs && object==this)
			{			
				attribs[Attributes::ModelAuthor]=this->getAuthor();
				attribs[Attributes::LastPosition]=QString("%1,%2").arg(last_pos.x()).arg(last_pos.y());
				attribs[Attributes::LastZoom]=QString::number(last_zoom);
				attribs[Attributes::DefaultCollation]=(default_objs[ObjectType::Collation] ? default_objs[ObjectType::Collation]->getSignature() : QString());
				attribs[Attributes::DefaultSchema]=(default_objs[ObjectType::Schema] ? default_objs[ObjectType::Schema]->getSignature() : QString());
				attribs[Attributes::DefaultTablespace]=(default_objs[ObjectType::Tablespace] ? default_objs[ObjectType::Tablespace]->getSignature() : QString());
				attribs[Attributes::DefaultOwner]=(default_objs[ObjectType::Role] ? default_objs[ObjectType::Role]->getSignature() : QString());
			}

			/* If the object is a graphic one and has Z stack value (currently only for tables/view/foreign tables,
			 * textboxes are already save completely in the metadata, so there's no need to separate their z values) */
			if(save_objs_z_value && graph_obj && BaseTable::isBaseTable(graph_obj->getObjectType()))
			{
				attribs[Attributes::ZValue]=QString::number(graph_obj->getZValue());
			}

			//If the object is a graphic one and we need to save positions and colors
			if((save_objs_pos || save_custom_colors) && graph_obj)
			{
				if(obj_type!=ObjectType::BaseRelationship && obj_type!=ObjectType::Relationship)
				{
					pnt=graph_obj->getPosition();

					if(obj_type==ObjectType::Schema)
					{
						schema=dynamic_cast<Schema *>(object);

						attribs[Attributes::CustomColor]=(save_custom_colors ? schema->getFillColor().name() : QString());
						attribs[Attributes::RectVisible]=(schema->isRectVisible() ? Attributes::True : Attributes::False);

						if(schema->isRectVisible())
						{
							attribs[Attributes::XPos]=QString::number(pnt.x());
							attribs[Attributes::YPos]=QString::number(pnt.y());
						}
					}
					else
					{
						attribs[Attributes::XPos]=QString::number(pnt.x());
						attribs[Attributes::YPos]=QString::number(pnt.y());
					}

					if(obj_type!=ObjectType::Schema || !attribs[Attributes::XPos].isEmpty())
					{
						schparser.ignoreUnkownAttributes(true);
						attribs[Attributes::Position]=
								schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, Attributes::Position),
																						attribs);
					}
				}
				else
				{
					BaseRelationship *rel=dynamic_cast<BaseRelationship *>(object);
					vector<QPointF> points=rel->getPoints();

					attribs_map aux_attribs;

					attribs[Attributes::CustomColor]=(save_custom_colors && rel->getCustomColor()!=Qt::transparent ? rel->getCustomColor().name() : Attributes::None);

					attribs[Attributes::SrcTable]=rel->getTable(BaseRelationship::SrcTable)->getSignature();
					attribs[Attributes::SrcType]=rel->getTable(BaseRelationship::SrcTable)->getSchemaName();

					attribs[Attributes::DstTable]=rel->getTable(BaseRelationship::DstTable)->getSignature();
					attribs[Attributes::DstType]=rel->getTable(BaseRelationship::DstTable)->getSchemaName();

					for(QPointF pnt : points)
					{
						attribs[Attributes::XPos]=QString::number(pnt.x());
						attribs[Attributes::YPos]=QString::number(pnt.y());

						schparser.ignoreUnkownAttributes(true);
						attribs[Attributes::Position]+=
								schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, Attributes::Position),
																						attribs);
					}

					//Saving the labels' custom positions
					for(unsigned id=BaseRelationship::SrcCardLabel; id <= BaseRelationship::RelNameLabel; id++)
					{
						pnt=rel->getLabelDistance(id);
						if(!std::isnan(pnt.x()) && !std::isnan(pnt.y()))
						{
							aux_attribs[Attributes::XPos]=QString::number(pnt.x());
							aux_attribs[Attributes::YPos]=QString::number(pnt.y());
							aux_attribs[Attributes::RefType]=labels_attrs[id];

							aux_attribs[Attributes::Position]=schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, Attributes::Position),
																																						aux_attribs);

							attribs[Attributes::Position]+=schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath( GlobalAttributes::XMLSchemaDir, Attributes::Label),
																																				 aux_attribs);

						}
					}
				}
			}

			//Storing the custom SQLs if the object has them configured
			if(save_custom_sql)
			{
				if(!object->getAppendedSQL().isEmpty())
					attribs[Attributes::AppendedSql]=schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, QString(Attributes::AppendedSql).remove(QChar('-'))),
																																			 attribs);


				if(!object->getPrependedSQL().isEmpty())
					attribs[Attributes::PrependedSql]=schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir,	QString(Attributes::PrependedSql).remove(QChar('-'))),
																																				attribs);

			}

			/* The object's metadata code will be generated only if one of the key attributes
			 for each option were filled previously. */
			if((save_db_attribs && obj_type==ObjectType::Database) ||
				 (save_custom_colors &&
					((obj_type==ObjectType::Relationship || obj_type==ObjectType::BaseRelationship) ||
					 (!attribs[Attributes::CustomColor].isEmpty()))) ||
				 (save_objs_pos &&
					(!attribs[Attributes::Position].isEmpty() ||
					 !attribs[Attributes::RectVisible].isEmpty())) ||
				 (save_tags && !attribs[Attributes::Tag].isEmpty()) ||
				 (save_objs_prot && !attribs[Attributes::Protected].isEmpty()) ||
				 (save_objs_sqldis && !attribs[Attributes::SqlDisabled].isEmpty()) ||
				 (save_custom_sql && (!attribs[Attributes::AppendedSql].isEmpty() ||
															!attribs[Attributes::PrependedSql].isEmpty())) ||
				 (save_fadeout && !attribs[Attributes::FadedOut].isEmpty()) ||
				 (save_collapsemode && !attribs[Attributes::CollapseMode].isEmpty()) ||
				 (save_objs_aliases && !attribs[Attributes::Alias].isEmpty()) ||
				 (save_objs_z_value && !attribs[Attributes::ZValue].isEmpty()))
			{
				emit s_objectLoaded(((idx++)/static_cast<double>(objects.size()))*100,
														tr("Saving metadata of the object `%1' (%2)")
														.arg(object->getSignature()).arg(object->getTypeName()), enum_cast(obj_type));

				schparser.ignoreUnkownAttributes(true);
				objs_def+=XmlParser::convertCharsToXMLEntities(
										schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, Attributes::Info),
																								attribs));
			}
			else
				idx++;

			attribs.clear();
		}

		if(!objs_def.isEmpty())
		{
			//Generates the metadata XML buffer
			attribs[Attributes::Info]=objs_def;
			buf.append(schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, Attributes::Metadata),
																						 attribs));

			output.write(buf.data(),buf.size());

			emit s_objectLoaded(100, tr("Metadata file successfully saved!"), enum_cast(ObjectType::BaseObject));
		}
		else
			emit s_objectLoaded(100, tr("Process successfully ended but no metadata was saved!"), enum_cast(ObjectType::BaseObject));

		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(ErrorCode::FileNotWrittenInvalidDefinition).arg(filename),
										ErrorCode::FileNotWrittenInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::loadObjectsMetadata(const QString &filename, unsigned options)
{
	QString elem_name, aux_elem, obj_name, ref_type,
			dtd_file=GlobalAttributes::getSchemasRootDir() +
							 GlobalAttributes::DirSeparator +
							 GlobalAttributes::XMLSchemaDir +
							 GlobalAttributes::DirSeparator +
							 GlobalAttributes::ObjectDTDDir +
							 GlobalAttributes::DirSeparator;
	attribs_map attribs, aux_attrib;
	ObjectType obj_type;
	BaseObject *object=nullptr, *new_object=nullptr;
	BaseTable *src_tab=nullptr, *dst_tab=nullptr, *base_tab=nullptr;
	vector<QPointF> points;
	map<QString, unsigned> labels_attrs;
	vector<QPointF> labels_pos={ QPointF(DNaN,DNaN), QPointF(DNaN,DNaN), QPointF(DNaN,DNaN) };
	BaseRelationship *rel=nullptr;
	Schema *schema=nullptr;
	Tag *tag=nullptr;
	int progress=0;
	bool load_db_attribs=false, load_objs_pos=false, load_objs_prot=false,
			load_objs_sqldis=false, load_textboxes=false, load_tags=false,
			load_custom_sql=false, load_custom_colors=false, load_fadeout=false,
			load_collapse_mode=false, load_genericsqls=false, load_objs_aliases=false,
			load_objs_z_value=false;

	load_db_attribs=(MetaDbAttributes & options) == MetaDbAttributes;
	load_objs_pos=(MetaObjsPositioning & options) == MetaObjsPositioning;
	load_objs_prot=(MetaObjsProtection & options) == MetaObjsProtection;
	load_objs_sqldis=(MetaObjsSqlDisabled & options) == MetaObjsSqlDisabled;
	load_textboxes=(MetaTextboxObjs & options) == MetaTextboxObjs;
	load_tags=(MetaTagObjs & options) == MetaTagObjs;
	load_custom_sql=(MetaObjsCustomSql & options) == MetaObjsCustomSql;
	load_custom_colors=(MetaObjsCustomColors & options) == MetaObjsCustomColors;
	load_fadeout=(MetaObjsFadeOut & options) == MetaObjsFadeOut;
	load_collapse_mode=(MetaObjsCollapseMode & options) == MetaObjsCollapseMode;
	load_genericsqls=(MetaGenericSqlObjs & options) == MetaGenericSqlObjs;
	load_objs_aliases=(MetaObjsAliases & options) == MetaObjsAliases;
	load_objs_z_value=(MetaObjsZStackValue & options) == MetaObjsZStackValue;

	try
	{
		labels_attrs[Attributes::SrcLabel]=BaseRelationship::SrcCardLabel;
		labels_attrs[Attributes::DstLabel]=BaseRelationship::DstCardLabel;
		labels_attrs[Attributes::NameLabel]=BaseRelationship::RelNameLabel;

		xmlparser.restartParser();

		xmlparser.setDTDFile(dtd_file + GlobalAttributes::MetadataDTD +
												 GlobalAttributes::ObjectDTDExt,
												 GlobalAttributes::MetadataDTD);

		xmlparser.loadXMLFile(filename);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem_name=xmlparser.getElementName();

					if((elem_name==BaseObject::getSchemaName(ObjectType::Tag) && load_tags) ||
						 (elem_name==BaseObject::getSchemaName(ObjectType::Textbox) && load_textboxes) ||
						 (elem_name==BaseObject::getSchemaName(ObjectType::GenericSql) && load_genericsqls))
					{
						xmlparser.savePosition();
						obj_type=BaseObject::getObjectType(elem_name);
						new_object=createObject(obj_type);

						if(getObjectIndex(new_object->getName(), obj_type) < 0)
						{
							emit s_objectLoaded(progress, tr("Creating object `%1' (%2)")
																	.arg(new_object->getName()).arg(new_object->getTypeName()), enum_cast(obj_type));
							addObject(new_object);
						}
						else
						{
							emit s_objectLoaded(progress, tr("Object `%1' (%2) already exists. Ignoring.")
																	.arg(new_object->getName()).arg(new_object->getTypeName()), enum_cast(ObjectType::BaseObject));
							delete new_object;
						}

						new_object=nullptr;
						xmlparser.restorePosition();
					}
					else if(elem_name==Attributes::Info)
					{
						xmlparser.getElementAttributes(attribs);
						obj_name=attribs[Attributes::Object];
						xmlparser.savePosition();

						obj_type=BaseObject::getObjectType(attribs[Attributes::Type]);
						progress=xmlparser.getCurrentBufferLine()/static_cast<double>(xmlparser.getBufferLineCount()) * 100;

						if(obj_type==ObjectType::Database)
						{
							if(load_db_attribs)
							{
								QStringList pos=attribs[Attributes::LastPosition].split(',');

								default_objs[ObjectType::Schema]=getSchema(attribs[Attributes::DefaultSchema]);
								default_objs[ObjectType::Role]=getRole(attribs[Attributes::DefaultOwner]);
								default_objs[ObjectType::Collation]=getCollation(attribs[Attributes::DefaultCollation]);
								default_objs[ObjectType::Tablespace]=getTablespace(attribs[Attributes::DefaultTablespace]);
								author=attribs[Attributes::ModelAuthor];
								last_zoom=attribs[Attributes::LastZoom].toDouble();

								if(pos.size()>=2)
									last_pos=QPoint(pos[0].toInt(), pos[1].toInt());
							}

							object=this;
						}
						else if(TableObject::isTableObject(obj_type))
						{
							base_tab = getTable(attribs[Attributes::Table]);

							if(!base_tab && (obj_type == ObjectType::Rule || obj_type == ObjectType::Index || obj_type == ObjectType::Trigger))
								base_tab = getView(attribs[Attributes::Table]);

							if(base_tab)
								object = base_tab->getObject(attribs[Attributes::Object], obj_type);

							//Discarding the object if it was added by relationship
							if(object && dynamic_cast<TableObject *>(object)->isAddedByRelationship())
								object = nullptr;
						}
						else
							object=getObject(obj_name, obj_type);

						/* If the object does not exists but it is a relationship, we try to get the relationship
						 involving the tables in paramenters src-table and dst-table */
						if(!object && obj_type==ObjectType::Relationship)
						{
							src_tab=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::SrcTable],
																								BaseObject::getObjectType(attribs[Attributes::SrcType])));
							dst_tab=dynamic_cast<BaseTable *>(getObject(attribs[Attributes::DstTable],
																								BaseObject::getObjectType(attribs[Attributes::DstType])));
							object=getRelationship(src_tab, dst_tab);
						}

						if(object)
						{
							emit s_objectLoaded(progress, tr("Loading metadata for object `%1' (%2)")
																	.arg(object->getName()).arg(object->getTypeName()), enum_cast(obj_type));

							if(!object->isSystemObject() &&
								 ((!attribs[Attributes::Protected].isEmpty() && load_objs_prot) ||
									(!attribs[Attributes::SqlDisabled].isEmpty() && load_objs_sqldis)))
							{
								if(!attribs[Attributes::Protected].isEmpty())
									object->setProtected(attribs[Attributes::Protected]==Attributes::True);

								if(!attribs[Attributes::SqlDisabled].isEmpty())
									object->setSQLDisabled(attribs[Attributes::SqlDisabled]==Attributes::True);
							}
							else if(BaseTable::isBaseTable(obj_type) && load_tags && !attribs[Attributes::Tag].isEmpty())
							{
								tag=getTag(attribs[Attributes::Tag]);

								if(tag)
									dynamic_cast<BaseTable *>(object)->setTag(tag);
							}
							else if(BaseTable::isBaseTable(obj_type) && load_objs_z_value && !attribs[Attributes::ZValue].isEmpty())
							{
								dynamic_cast<BaseTable *>(object)->setZValue(attribs[Attributes::ZValue].toInt());
							}
							else if(obj_type==ObjectType::Database && load_custom_sql)
							{
								if(!attribs[Attributes::AppendAtEod].isEmpty())
									this->setAppendAtEOD(attribs[Attributes::AppendAtEod]==Attributes::True);

								if(!attribs[Attributes::PrependAtBod].isEmpty())
									this->setPrependAtBOD(attribs[Attributes::PrependAtBod]==Attributes::True);
							}

							if(load_objs_aliases && !attribs[Attributes::Alias].isEmpty())
								object->setAlias(attribs[Attributes::Alias]);

							if(xmlparser.accessElement(XmlParser::ChildElement))
							{
								do
								{
									aux_elem=xmlparser.getElementName();
									xmlparser.getElementAttributes(aux_attrib);

									//Retrieving and storing the points
									if(aux_elem==Attributes::Position)
									{
										points.push_back(QPointF(aux_attrib[Attributes::XPos].toDouble(),
																						 aux_attrib[Attributes::YPos].toDouble()));
									}
									//Retrieving and storing the labels' custom positions
									else if(aux_elem==Attributes::Label)
									{
										ref_type=aux_attrib[Attributes::RefType];
										xmlparser.savePosition();

										if(xmlparser.accessElement(XmlParser::ChildElement))
										{
											xmlparser.getElementAttributes(aux_attrib);
											labels_pos[labels_attrs[ref_type]]=QPointF(aux_attrib[Attributes::XPos].toDouble(),
																																 aux_attrib[Attributes::YPos].toDouble());
										}

										xmlparser.restorePosition();
									}
									else if(load_custom_sql && aux_elem==Attributes::AppendedSql &&
													attribs[Attributes::AppendedSql].isEmpty())
									{
										xmlparser.savePosition();
										xmlparser.accessElement(XmlParser::ChildElement);
										object->setAppendedSQL(xmlparser.getElementContent());
										xmlparser.restorePosition();
									}
									else if(load_custom_sql && aux_elem==Attributes::PrependedSql &&
													attribs[Attributes::PrependedSql].isEmpty())
									{
										xmlparser.savePosition();
										xmlparser.accessElement(XmlParser::ChildElement);
										object->setPrependedSQL(xmlparser.getElementContent());
										xmlparser.restorePosition();
									}
								}
								while(xmlparser.accessElement(XmlParser::NextElement));
							}

							if(BaseGraphicObject::isGraphicObject(obj_type))
							{
								base_tab=dynamic_cast<BaseTable *>(object);
								rel=dynamic_cast<BaseRelationship *>(object);
								schema=dynamic_cast<Schema *>(object);

								if(!rel && !schema && !points.empty() && load_objs_pos)
									dynamic_cast<BaseGraphicObject *>(object)->setPosition(points[0]);
								else if(rel)
								{
									if(load_custom_colors)
									{
										if(attribs[Attributes::CustomColor]==Attributes::None)
											rel->setCustomColor(Qt::transparent);
										else
											rel->setCustomColor(QColor(attribs[Attributes::CustomColor]));
									}

									if(load_objs_pos)
									{
										rel->setPoints(points);

										for(unsigned id=BaseRelationship::SrcCardLabel; id <= BaseRelationship::RelNameLabel; id++)
										{
											rel->setLabelDistance(id, labels_pos[id]);
											labels_pos[id]=QPointF(DNaN, DNaN);
										}
									}
								}
								else if(schema)
								{
									if(load_custom_colors)
										schema->setFillColor(QColor(attribs[Attributes::CustomColor]));

									schema->setRectVisible(attribs[Attributes::RectVisible]==Attributes::True);
								}

								if(load_fadeout)
									dynamic_cast<BaseGraphicObject *>(object)->setFadedOut(attribs[Attributes::FadedOut]==Attributes::True);

								if(load_collapse_mode && base_tab)
									base_tab->setCollapseMode(static_cast<CollapseMode>(attribs[Attributes::CollapseMode].toUInt()));
							}

							points.clear();

						}
						else if(!object)
						{
							emit s_objectLoaded(progress, tr("Object `%1' (%2) not found. Ignoring metadata.")
																	.arg(obj_name).arg(BaseObject::getTypeName(obj_type)), enum_cast(ObjectType::BaseObject));
						}

						xmlparser.restorePosition();

					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		emit s_objectLoaded(100, tr("Metadata file successfully loaded!"), enum_cast(ObjectType::BaseObject));
		setObjectsModified();
	}
	catch(Exception &e)
	{
		QString extra_info;

		if(xmlparser.getCurrentElement())
			extra_info=QString(QObject::tr("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}
}

void DatabaseModel::setLayers(const QStringList &layers)
{
	this->layers = layers;
}

QStringList DatabaseModel::getLayers()
{
	return layers;
}

void DatabaseModel::setActiveLayers(const QList<unsigned> &layers)
{
	active_layers = layers;
}

QList<unsigned> DatabaseModel::getActiveLayers()
{
	return active_layers;
}

template<class TableClass>
TableClass *DatabaseModel::createPhysicalTable()
{
	attribs_map attribs, aux_attribs;
	QString elem;
	TableClass *table=nullptr;
	TableObject *object=nullptr;
	BaseObject *tag=nullptr;
	ObjectType obj_type;
	vector<unsigned> idxs;
	vector<QString> names;
	PartitionKey part_key;
	vector<PartitionKey> partition_keys;

	try
	{
		table = new TableClass;
		setBasicAttributes(table);
		xmlparser.getElementAttributes(attribs);

		table->setObjectListsCapacity(attribs[Attributes::MaxObjCount].toUInt());
		table->setGenerateAlterCmds(attribs[Attributes::GenAlterCmds]==Attributes::True);
		table->setCollapseMode(attribs[Attributes::CollapseMode].isEmpty() ? CollapseMode::NotCollapsed : static_cast<CollapseMode>(attribs[Attributes::CollapseMode].toUInt()));
		table->setPaginationEnabled(attribs[Attributes::Pagination]==Attributes::True);
		table->setCurrentPage(BaseTable::AttribsSection, attribs[Attributes::AttribsPage].toUInt());
		table->setCurrentPage(BaseTable::ExtAttribsSection, attribs[Attributes::ExtAttribsPage].toUInt());
		table->setFadedOut(attribs[Attributes::FadedOut]==Attributes::True);
		table->setLayer(attribs[Attributes::Layer].toUInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();
					xmlparser.savePosition();
					object=nullptr;

					if(elem==BaseObject::objs_schemas[enum_cast(ObjectType::Column)])
						object=createColumn();
					else if(elem==BaseObject::objs_schemas[enum_cast(ObjectType::Constraint)])
						object=createConstraint(table);
					else if(elem==BaseObject::objs_schemas[enum_cast(ObjectType::Tag)])
					{
						xmlparser.getElementAttributes(aux_attribs);
						tag=getObject(aux_attribs[Attributes::Name], ObjectType::Tag);

						if(!tag)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
															.arg(attribs[Attributes::Name])
									.arg(BaseObject::getTypeName(ObjectType::Table))
									.arg(aux_attribs[Attributes::Table])
									.arg(BaseObject::getTypeName(ObjectType::Tag))
									, ErrorCode::RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						table->setTag(dynamic_cast<Tag *>(tag));
					}
					//Retrieving custom columns / constraint indexes
					else if(elem==Attributes::CustomIdxs)
					{
						xmlparser.getElementAttributes(aux_attribs);
						obj_type=BaseObject::getObjectType(aux_attribs[Attributes::ObjectType]);

						xmlparser.savePosition();

						if(xmlparser.accessElement(XmlParser::ChildElement))
						{
							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE)
								{
									elem=xmlparser.getElementName();

									//The element <object> stores the index for each object in the current group
									if(elem==Attributes::Object)
									{
										xmlparser.getElementAttributes(aux_attribs);
										names.push_back(aux_attribs[Attributes::Name]);
										idxs.push_back(aux_attribs[Attributes::Index].toUInt());
									}
								}
							}
							while(xmlparser.accessElement(XmlParser::NextElement));

							table->setRelObjectsIndexes(names, idxs, obj_type);
							names.clear();
							idxs.clear();
						}

						xmlparser.restorePosition();
					}
					else if(elem==Attributes::Partitioning)
					{
						xmlparser.getElementAttributes(aux_attribs);
						table->setPartitioningType(aux_attribs[Attributes::Type]);
						xmlparser.savePosition();

						if(xmlparser.accessElement(XmlParser::ChildElement))
						{
							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE &&
									 xmlparser.getElementName()==Attributes::PartitionKey)
								{
									createElement(part_key, nullptr, table);
									partition_keys.push_back(part_key);
								}
							}
							while(xmlparser.accessElement(XmlParser::NextElement));

							table->addPartitionKeys(partition_keys);
						}

						xmlparser.restorePosition();
					}
					//Retrieving initial data
					else if(elem==Attributes::InitialData)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						table->setInitialData(xmlparser.getElementContent());
						xmlparser.restorePosition();
					}

					if(object)
						table->addObject(object);

					xmlparser.restorePosition();
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->setProtected(table->isProtected());
	}
	catch(Exception &e)
	{
		QString extra_info=getErrorExtraInfo();
		xmlparser.restorePosition();

		if(table) delete table;
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return table;
}

void DatabaseModel::getDataDictionary(attribs_map &datadict, bool browsable, bool splitted)
{
	int idx = 0;
	BaseObject *object = nullptr;
	vector<BaseObject *> objects;
	map<QString, BaseObject *> objs_map;
	QString styles, id, index, items, buffer;
	attribs_map attribs, aux_attribs;
	QStringList index_list;
	QString dict_sch_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, GlobalAttributes::DataDictSchemaDir),
			style_sch_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, Attributes::Styles),
			item_sch_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, Attributes::Item),
			index_sch_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, Attributes::Index);

	objects.assign(tables.begin(), tables.end());
	objects.insert(objects.end(), foreign_tables.begin(), foreign_tables.end());
	objects.insert(objects.end(), views.begin(), views.end());
	objects.insert(objects.end(), relationships.begin(), relationships.end());

	// Placing the objects in alphabectical order
	for(auto &obj : objects)
	{
		// Retrieving the generated table if the current object is a relationship (n-n)
		if(obj->getObjectType() == ObjectType::Relationship)
		{
			Relationship *rel = dynamic_cast<Relationship *>(obj);

			if(!rel->getGeneratedTable())
				continue;

			obj = rel->getGeneratedTable();
		}

		id = obj->getSignature().remove(QChar('"'));
		objs_map[id] = obj;
		index_list.push_back(id);
	}

	index_list.sort();
	datadict.clear();

	// Generates the the stylesheet
	styles = schparser.getCodeDefinition(style_sch_file, attribs);
	attribs[Attributes::Styles] = QString();
	attribs[Attributes::Index] = QString();
	attribs[Attributes::Splitted] = splitted ? Attributes::True : QString();

	// If the generation is a standalone HTML the css is embedded
	if(!splitted)
		attribs[Attributes::Styles] = styles;
	else
		// Otherwise we create a separated stylesheet file
		datadict[Attributes::Styles + QString(".css")] = styles;

	// Generating individual data dictionaries
	for(auto &itr : objs_map)
	{
		object = itr.second;

		// Generate the individual data dictionaries
		aux_attribs[Attributes::Index] = browsable ? Attributes::True : QString();
		aux_attribs[Attributes::Previous] = idx - 1 >= 0 ? index_list.at(idx - 1) : QString();
		aux_attribs[Attributes::Next] = (++idx <= index_list.size() - 1) ? index_list.at(idx) : QString();
		attribs[Attributes::Objects] += dynamic_cast<BaseTable *>(object)->getDataDictionary(splitted, aux_attribs);

		// If the generation is configured to be splitted we generate a complete HTML file for the current table
		if(splitted && !attribs[Attributes::Objects].isEmpty())
		{
			id = itr.first + QString(".html");
			schparser.ignoreEmptyAttributes(true);
			datadict[id] = schparser.getCodeDefinition(dict_sch_file, attribs);
			attribs[Attributes::Objects].clear();
		}
	}

	// If the data dictionary is browsable we proceed with the index generation
	if(browsable)
	{
		attribs_map idx_attribs;

		idx_attribs[BaseObject::getSchemaName(ObjectType::Table)] = QString();
		idx_attribs[BaseObject::getSchemaName(ObjectType::View)] = QString();
		idx_attribs[BaseObject::getSchemaName(ObjectType::ForeignTable)] = QString();

		// Generating the index items
		for(auto &item : index_list)
		{
			aux_attribs[Attributes::Splitted] = attribs[Attributes::Splitted];
			aux_attribs[Attributes::Item] = item;
			idx_attribs[objs_map[item]->getSchemaName()] += schparser.getCodeDefinition(item_sch_file, aux_attribs);
		}

		idx_attribs[Attributes::Name] = this->obj_name;
		idx_attribs[Attributes::Splitted] = attribs[Attributes::Splitted];

		schparser.ignoreEmptyAttributes(true);
		index = schparser.getCodeDefinition(index_sch_file, idx_attribs);
	}

	// If the data dictionary is browsable and splitted the index goes into a separated file
	if(splitted && browsable)
		datadict[Attributes::Index + QString(".html")] = index;
	else if(!splitted)
	{
		attribs[Attributes::Index] = index;
		schparser.ignoreEmptyAttributes(true);
		datadict[Attributes::Database] = schparser.getCodeDefinition(dict_sch_file, attribs);
	}
}

void DatabaseModel::saveDataDictionary(const QString &path, bool browsable, bool splitted)
{
	try
	{
		attribs_map datadict;
		QFile output;
		QByteArray buffer;
		QFileInfo finfo(path);
		QDir dir;

		if(splitted)
		{
			if(finfo.exists() && !finfo.isDir())
				throw Exception(Exception::getErrorMessage(ErrorCode::InvDataDictDirectory).arg(path),
												ErrorCode::InvDataDictDirectory,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			else if(!finfo.exists())
				dir.mkpath(path);
		}

		getDataDictionary(datadict, browsable, splitted);
		output.setFileName(path);

		for(auto &itr : datadict)
		{
			if(splitted)
				output.setFileName(path + GlobalAttributes::DirSeparator + itr.first);

			output.open(QFile::WriteOnly);

			if(!output.isOpen())
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(output.fileName()),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			buffer.append(itr.second);
			output.write(buffer);
			output.close();
			buffer.clear();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
