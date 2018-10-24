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

#include "databasemodel.h"
#include "pgmodelerns.h"

unsigned DatabaseModel::dbmodel_id=2000;

DatabaseModel::DatabaseModel(void)
{
	this->model_wgt=nullptr;
	object_id=DatabaseModel::dbmodel_id++;
	obj_type=ObjectType::ObjDatabase;

	is_template = false;
	allow_conns = true;

	encoding=BaseType::null;
	BaseObject::setName(QObject::trUtf8("new_database").toUtf8());

	default_objs[ObjectType::ObjSchema]=nullptr;
	default_objs[ObjectType::ObjRole]=nullptr;
	default_objs[ObjectType::ObjTablespace]=nullptr;
	default_objs[ObjectType::ObjCollation]=nullptr;

	conn_limit=-1;
	last_zoom=1;
	loading_model=invalidated=append_at_eod=prepend_at_bod=false;
	attributes[ParsersAttributes::ENCODING]=QString();
	attributes[ParsersAttributes::TEMPLATE_DB]=QString();
	attributes[ParsersAttributes::CONN_LIMIT]=QString();
	attributes[ParsersAttributes::_LC_COLLATE_]=QString();
	attributes[ParsersAttributes::_LC_CTYPE_]=QString();
	attributes[ParsersAttributes::APPEND_AT_EOD]=QString();
	attributes[ParsersAttributes::PREPEND_AT_BOD]=QString();
	attributes[ParsersAttributes::ALLOW_CONNS]=QString();
	attributes[ParsersAttributes::IS_TEMPLATE]=QString();
}

DatabaseModel::DatabaseModel(ModelWidget *model_wgt):DatabaseModel()
{
	this->model_wgt=model_wgt;
}

DatabaseModel::~DatabaseModel(void)
{
	this->blockSignals(true);
	destroyObjects();
}

ModelWidget *DatabaseModel::getModelWidget(void)
{
	return(model_wgt);
}

void DatabaseModel::setEncoding(EncodingType encod)
{
	encoding=encod;
}

void DatabaseModel::setLocalization(unsigned localiz_id, const QString &value)
{
	if(localiz_id > Collation::LcCollate)
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(AsgInvalidNameObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->template_db=temp_db;
}

void DatabaseModel::setAuthor(const QString &author)
{
	this->author=author;
}

vector<BaseObject *> *DatabaseModel::getObjectList(ObjectType obj_type)
{
	if(obj_type==ObjectType::ObjTextbox)
		return(&textboxes);
	else if(obj_type==ObjectType::ObjTable)
		return(&tables);
	else if(obj_type==ObjectType::ObjFunction)
		return(&functions);
	else if(obj_type==ObjectType::ObjAggregate)
		return(&aggregates);
	else if(obj_type==ObjectType::ObjSchema)
		return(&schemas);
	else if(obj_type==ObjectType::ObjView)
		return(&views);
	else if(obj_type==ObjectType::ObjType)
		return(&types);
	else if(obj_type==ObjectType::ObjRole)
		return(&roles);
	else if(obj_type==ObjectType::ObjTablespace)
		return(&tablespaces);
	else if(obj_type==ObjectType::ObjLanguage)
		return(&languages);
	else if(obj_type==ObjectType::ObjCast)
		return(&casts);
	else if(obj_type==ObjectType::ObjConversion)
		return(&conversions);
	else if(obj_type==ObjectType::ObjOperator)
		return(&operators);
	else if(obj_type==ObjectType::ObjOpClass)
		return(&op_classes);
	else if(obj_type==ObjectType::ObjOpFamily)
		return(&op_families);
	else if(obj_type==ObjectType::ObjDomain)
		return(&domains);
	else if(obj_type==ObjectType::ObjSequence)
		return(&sequences);
	else if(obj_type==ObjectType::ObjBaseRelationship)
		return(&base_relationships);
	else if(obj_type==ObjectType::ObjRelationship)
		return(&relationships);
	else if(obj_type==ObjectType::ObjPermission)
		return(&permissions);
	else if(obj_type==ObjectType::ObjCollation)
		return(&collations);
	else if(obj_type==ObjectType::ObjExtension)
		return(&extensions);
	else if(obj_type==ObjectType::ObjTag)
		return(&tags);
	else if(obj_type==ObjectType::ObjEventTrigger)
		return(&eventtriggers);
	else if(obj_type==ObjectType::ObjGenericSQL)
		return(&genericsqls);
	else
		return(nullptr);
}

void DatabaseModel::addObject(BaseObject *object, int obj_idx)
{
	ObjectType obj_type;

	if(object)
	{
		try
		{
			obj_type=object->getObjectType();

			if(obj_type==ObjectType::ObjRelationship ||
					obj_type==ObjectType::ObjBaseRelationship)
				addRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTextbox)
				addTextbox(dynamic_cast<Textbox *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTable)
				addTable(dynamic_cast<Table *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjFunction)
				addFunction(dynamic_cast<Function *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjAggregate)
				addAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjSchema)
				addSchema(dynamic_cast<Schema *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjView)
				addView(dynamic_cast<View *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjType)
				addType(dynamic_cast<Type *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjRole)
				addRole(dynamic_cast<Role *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTablespace)
				addTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjLanguage)
				addLanguage(dynamic_cast<Language *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjCast)
				addCast(dynamic_cast<Cast *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjConversion)
				addConversion(dynamic_cast<Conversion *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOperator)
				addOperator(dynamic_cast<Operator *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOpClass)
				addOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOpFamily)
				addOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjDomain)
				addDomain(dynamic_cast<Domain *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjSequence)
				addSequence(dynamic_cast<Sequence *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjCollation)
				addCollation(dynamic_cast<Collation *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjExtension)
				addExtension(dynamic_cast<Extension *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTag)
				addTag(dynamic_cast<Tag *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjPermission)
				addPermission(dynamic_cast<Permission *>(object));
			else if(obj_type==ObjectType::ObjEventTrigger)
				addEventTrigger(dynamic_cast<EventTrigger *>(object));
			else if(obj_type==ObjectType::ObjGenericSQL)
				addGenericSQL(dynamic_cast<GenericSQL *>(object));
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}
}

void DatabaseModel::removeObject(BaseObject *object, int obj_idx)
{
	ObjectType obj_type;


	if(object)
	{
		try
		{
			obj_type=object->getObjectType();

			if(obj_type==ObjectType::ObjRelationship ||
					obj_type==ObjectType::ObjBaseRelationship)
				removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTextbox)
				removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTable)
				removeTable(dynamic_cast<Table *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjFunction)
				removeFunction(dynamic_cast<Function *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjAggregate)
				removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjSchema)
				removeSchema(dynamic_cast<Schema *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjView)
				removeView(dynamic_cast<View *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjType)
				removeType(dynamic_cast<Type *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjRole)
				removeRole(dynamic_cast<Role *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTablespace)
				removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjLanguage)
				removeLanguage(dynamic_cast<Language *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjCast)
				removeCast(dynamic_cast<Cast *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjConversion)
				removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOperator)
				removeOperator(dynamic_cast<Operator *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOpClass)
				removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjOpFamily)
				removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjDomain)
				removeDomain(dynamic_cast<Domain *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjSequence)
				removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjCollation)
				removeCollation(dynamic_cast<Collation *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjExtension)
				removeExtension(dynamic_cast<Extension *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjTag)
				removeTag(dynamic_cast<Tag *>(object), obj_idx);
			else if(obj_type==ObjectType::ObjPermission)
				removePermission(dynamic_cast<Permission *>(object));
			else if(obj_type==ObjectType::ObjEventTrigger)
				removeEventTrigger(dynamic_cast<EventTrigger *>(object));
			else if(obj_type==ObjectType::ObjGenericSQL)
				removeGenericSQL(dynamic_cast<GenericSQL *>(object));
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}
}

void DatabaseModel::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	if(TableObject::isTableObject(obj_type) ||
			obj_type==ObjectType::ObjBaseObject || obj_type==ObjectType::ObjBaseRelationship ||
			obj_type==ObjectType::ObjDatabase)
		throw Exception(RemObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else
	{
		vector<BaseObject *> *obj_list=nullptr;
		BaseObject *object=nullptr;

		obj_list=getObjectList(obj_type);
		if(obj_idx >= obj_list->size())
			throw Exception(RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		object=(*obj_list)[obj_idx];
		if(obj_type==ObjectType::ObjTextbox)
			removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjTable)
			removeTable(dynamic_cast<Table *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjFunction)
			removeFunction(dynamic_cast<Function *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjAggregate)
			removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjSchema)
			removeSchema(dynamic_cast<Schema *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjView)
			removeView(dynamic_cast<View *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjType)
			removeType(dynamic_cast<Type *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjRole)
			removeRole(dynamic_cast<Role *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjTablespace)
			removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjLanguage)
			removeLanguage(dynamic_cast<Language *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjCast)
			removeCast(dynamic_cast<Cast *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjConversion)
			removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjOperator)
			removeOperator(dynamic_cast<Operator *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjOpClass)
			removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjOpFamily)
			removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjDomain)
			removeDomain(dynamic_cast<Domain *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjSequence)
			removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjCollation)
			removeCollation(dynamic_cast<Collation *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjRelationship || obj_type==ObjectType::ObjBaseRelationship)
			removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjPermission)
			removePermission(dynamic_cast<Permission *>(object));
		else if(obj_type==ObjectType::ObjEventTrigger)
			removeEventTrigger(dynamic_cast<EventTrigger *>(object), obj_idx);
		else if(obj_type==ObjectType::ObjGenericSQL)
			removeGenericSQL(dynamic_cast<GenericSQL *>(object), obj_idx);
	}
}

void DatabaseModel::__addObject(BaseObject *object, int obj_idx)
{
	int idx;
	ObjectType obj_type;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!object)
		throw Exception(AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);


	obj_type=object->getObjectType();

#ifdef DEMO_VERSION
#warning "DEMO VERSION: database model object creation limit."
	obj_list=getObjectList(obj_type);
	if(obj_list && obj_list->size() >= GlobalAttributes::MaxObjectCount)
		throw Exception(trUtf8("The demonstration version can create only `%1' instances of each object type! You've reach this limit for the type: `%2'")
						.arg(GlobalAttributes::MaxObjectCount)
						.arg(BaseObject::getTypeName(obj_type)),
						ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

#endif

	if(obj_type==ObjectType::ObjTablespace)
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
				throw Exception(Exception::getErrorMessage(AsgTablespaceDuplicatedDirectory)
								.arg(tabspc->getName())
								.arg(aux_tabspc->getName()),
								AsgTablespaceDuplicatedDirectory,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			itr++;
		}
	}

	/* Raises an error if there is an object with the same name.
		 Special cases are for: functions/operator that are search by signature and views
		 that are search on tables and views list */
	if((obj_type==ObjectType::ObjView &&	(getObject(object->getName(true), obj_type, idx) ||
														 getObject(object->getName(true), ObjectType::ObjTable, idx))) ||
			(obj_type==ObjectType::ObjTable && (getObject(object->getName(true), obj_type, idx) ||
															 getObject(object->getName(true), ObjectType::ObjView, idx))) ||
			(obj_type==ObjectType::ObjExtension &&	(getObject(object->getName(false), obj_type, idx))) ||
			(getObject(object->getSignature(), obj_type, idx)))
	{
		QString str_aux;

		str_aux=Exception::getErrorMessage(AsgDuplicatedObject)
				.arg(object->getName(obj_type != ObjectType::ObjExtension))
				.arg(object->getTypeName())
				.arg(this->getName(true))
				.arg(this->getTypeName());

		throw Exception(str_aux,AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	try
	{
		if(obj_type==ObjectType::ObjTextbox || obj_type==ObjectType::ObjBaseRelationship)
			object->getCodeDefinition(SchemaParser::XmlDefinition);
		else
			object->getCodeDefinition(SchemaParser::SqlDefinition);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(RemNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		vector<BaseObject *> *obj_list=nullptr;
		ObjectType obj_type;

		obj_type=object->getObjectType();
		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
					err_code=RemDirectReference;
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

					err_code=RemInderectReference;
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
		throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

	return(sel_list);
}

vector<BaseObject *> DatabaseModel::getObjects(BaseObject *schema)
{
	vector<BaseObject *> *obj_list=nullptr, sel_list;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType types[]={	ObjectType::ObjFunction, ObjectType::ObjTable, ObjectType::ObjView, ObjectType::ObjDomain,
							ObjectType::ObjAggregate, ObjectType::ObjOperator, ObjectType::ObjSequence, ObjectType::ObjConversion,
							ObjectType::ObjType, ObjectType::ObjOpClass, ObjectType::ObjOpFamily, ObjectType::ObjCollation };
	unsigned i, count=sizeof(types)/sizeof(ObjectType);

	for(i=0; i < count; i++)
	{
		obj_list=getObjectList(types[i]);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end)
		{
			if((*itr)->getSchema()==schema)
				sel_list.push_back(*itr);
			itr++;
		}
	}

	return(sel_list);
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
		throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			obj_idx=(itr-obj_list->begin());
		}
		else obj_idx=-1;
	}

	return(object);
}

BaseObject *DatabaseModel::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<BaseObject *> *obj_list=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(obj_idx >= obj_list->size())
		throw Exception(RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return(obj_list->at(obj_idx));
}

unsigned DatabaseModel::getObjectCount(ObjectType obj_type)
{
	vector<BaseObject *> *obj_list=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	  return(obj_list->size());
}

unsigned DatabaseModel::getMaxObjectCount(void)
{
  vector<ObjectType> types = getObjectTypes(false, {ObjectType::ObjDatabase});
  unsigned count = 0, max = 0;

  for(auto &type : types)
  {
	 count = getObjectList(type)->size();
	 if(count > max) max = count;
  }

  return(max);
}

unsigned DatabaseModel::getObjectCount(void)
{
  vector<ObjectType> types= getObjectTypes(false, {ObjectType::ObjDatabase});
	unsigned count=0;

	for(auto &type : types)
		count+=getObjectList(type)->size();

	return(count);
}

QString DatabaseModel::getLocalization(unsigned localiz_id)
{
	if(localiz_id > Collation::LcCollate)
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(localizations[localiz_id]);
}

int DatabaseModel::getConnectionLimit(void)
{
	return(conn_limit);
}

QString DatabaseModel::getTemplateDB(void)
{
	return(template_db);
}

EncodingType DatabaseModel::getEncoding(void)
{
	return(encoding);
}

BaseObject *DatabaseModel::getDefaultObject(ObjectType obj_type)
{
	if(default_objs.count(obj_type)==0)
		throw Exception(RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(default_objs[obj_type]);
}

QString DatabaseModel::getAuthor(void)
{
	return(author);
}

void DatabaseModel::setProtected(bool value)
{
	vector<ObjectType> types = {
		ObjectType::ObjBaseRelationship,ObjectType::ObjRelationship, ObjectType::ObjTable, ObjectType::ObjView,
		ObjectType::ObjAggregate, ObjectType::ObjOperator,
		ObjectType::ObjSequence, ObjectType::ObjConversion,
		ObjectType::ObjCast, ObjectType::ObjOpFamily, ObjectType::ObjOpClass,
		ObjectType::ObjBaseRelationship, ObjectType::ObjTextbox,
		ObjectType::ObjDomain, ObjectType::ObjType, ObjectType::ObjFunction,
		ObjectType::ObjLanguage, ObjectType::ObjTablespace, ObjectType::ObjRole, ObjectType::ObjCollation,
		ObjectType::ObjExtension, ObjectType::ObjSchema, ObjectType::ObjPermission, ObjectType::ObjTag, ObjectType::ObjGenericSQL
	};

	for(auto &type : types)
	{
		for(auto &object :  *getObjectList(type))
		{
			object->setProtected(value);
		}
	}

	BaseObject::setProtected(value);
}

void DatabaseModel::destroyObjects(void)
{
	ObjectType graph_types[]={ ObjectType::ObjSchema, ObjectType::ObjBaseRelationship, ObjectType::ObjRelationship, ObjectType::ObjTable, ObjectType::ObjView };
	BaseObject *object=nullptr;
	map<unsigned, BaseObject *> objects;
	map<unsigned, BaseObject *>::reverse_iterator ritr, ritr_end;
	vector<ObjectType> rem_obj_types;

	//Blocking signals of all graphical objects to avoid uneeded updates in the destruction
	this->blockSignals(true);

	for(unsigned i=0; i < 5; i++)
	{
		for(auto &object : *this->getObjectList(graph_types[i]))
			dynamic_cast<BaseGraphicObject *>(object)->blockSignals(true);
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
		QTextStream out(stdout);
		out << trUtf8("** FAIL TO DESTROY ALL OBJECTS **") << endl;
		out << e.getExceptionsText() << endl;
	}

	objects = getCreationOrder(SchemaParser::XmlDefinition, true);
	ritr = objects.rbegin(),
	ritr_end = objects.rend();

	while(ritr != ritr_end)
	{
		object = ritr->second;
		ritr++;

		// We ignore the database itself as well table children objects
		if(object->getObjectType() == ObjectType::ObjDatabase ||
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

			if(object->getObjectType()==ObjectType::ObjRelationship)
				dynamic_cast<Relationship *>(object)->destroyObjects();
		}

		delete(object);
	}

	PgSQLType::removeUserTypes(this);

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

		PgSQLType::addUserType(table->getName(true), table, this, UserTypeConfig::TABLE_TYPE);

		updateTableFKRelationships(table);

		dynamic_cast<Schema *>(table->getSchema())->setModified(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Table *DatabaseModel::getTable(unsigned obj_idx)
{
	return(dynamic_cast<Table *>(getObject(obj_idx, ObjectType::ObjTable)));
}

Table *DatabaseModel::getTable(const QString &name)
{
	return(dynamic_cast<Table *>(getObject(name, ObjectType::ObjTable)));
}

void DatabaseModel::removeTable(Table *table, int obj_idx)
{
	try
	{
		__removeObject(table, obj_idx);
		PgSQLType::removeUserType(table->getName(true), table);
		updateTableFKRelationships(table);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addSequence(Sequence *sequence, int obj_idx)
{
	try
	{
		__addObject(sequence, obj_idx);
		PgSQLType::addUserType(sequence->getName(true), sequence, this, UserTypeConfig::SEQUENCE_TYPE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Sequence *DatabaseModel::getSequence(unsigned obj_idx)
{
	return(dynamic_cast<Sequence *>(getObject(obj_idx, ObjectType::ObjSequence)));
}

Sequence *DatabaseModel::getSequence(const QString &name)
{
	return(dynamic_cast<Sequence *>(getObject(name, ObjectType::ObjSequence)));
}

void DatabaseModel::removeSequence(Sequence *sequence, int obj_idx)
{
	try
	{
		removeUserType(sequence, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Collation *DatabaseModel::getCollation(unsigned obj_idx)
{
	return(dynamic_cast<Collation *>(getObject(obj_idx, ObjectType::ObjCollation)));
}

Collation *DatabaseModel::getCollation(const QString &name)
{
	return(dynamic_cast<Collation *>(getObject(name, ObjectType::ObjCollation)));
}

void DatabaseModel::removeCollation(Collation *collation, int obj_idx)
{
	try
	{
		__removeObject(collation, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::addExtension(Extension *extension, int obj_idx)
{
	try
	{
		__addObject(extension, obj_idx);

		if(extension->handlesType())
			PgSQLType::addUserType(extension->getName(true), extension, this, UserTypeConfig::EXTENSION_TYPE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Extension *DatabaseModel::getExtension(unsigned obj_idx)
{
	return(dynamic_cast<Extension *>(getObject(obj_idx, ObjectType::ObjCollation)));
}

Extension *DatabaseModel::getExtension(const QString &name)
{
	return(dynamic_cast<Extension *>(getObject(name, ObjectType::ObjExtension)));
}

void DatabaseModel::addTag(Tag *tag, int obj_idx)
{
	try
	{
		__addObject(tag, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Tag *DatabaseModel::getTag(unsigned obj_idx)
{
	return(dynamic_cast<Tag *>(getObject(obj_idx, ObjectType::ObjTag)));
}

Tag *DatabaseModel::getTag(const QString &name)
{
	return(dynamic_cast<Tag *>(getObject(name, ObjectType::ObjTag)));
}

void DatabaseModel::addEventTrigger(EventTrigger *evnttrig, int obj_idx)
{
	try
	{
		__addObject(evnttrig, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

EventTrigger *DatabaseModel::getEventTrigger(unsigned obj_idx)
{
	return(dynamic_cast<EventTrigger *>(getObject(obj_idx, ObjectType::ObjEventTrigger)));
}

EventTrigger *DatabaseModel::getEventTrigger(const QString &name)
{
	return(dynamic_cast<EventTrigger *>(getObject(name, ObjectType::ObjEventTrigger)));
}

void DatabaseModel::addGenericSQL(GenericSQL *genericsql, int obj_idx)
{
	try
	{
		__addObject(genericsql, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

GenericSQL *DatabaseModel::getGenericSQL(unsigned obj_idx)
{
	return(dynamic_cast<GenericSQL *>(getObject(obj_idx, ObjectType::ObjGenericSQL)));
}

GenericSQL *DatabaseModel::getGenericSQL(const QString &name)
{
	return(dynamic_cast<GenericSQL *>(getObject(name, ObjectType::ObjGenericSQL)));
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::addView(View *view, int obj_idx)
{
	try
	{
		__addObject(view, obj_idx);
		PgSQLType::addUserType(view->getName(true), view, this, UserTypeConfig::VIEW_TYPE);

		updateViewRelationships(view);
		dynamic_cast<Schema *>(view->getSchema())->setModified(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

View *DatabaseModel::getView(unsigned obj_idx)
{
	return(dynamic_cast<View *>(getObject(obj_idx, ObjectType::ObjView)));
}

View *DatabaseModel::getView(const QString &name)
{
	return(dynamic_cast<View *>(getObject(name, ObjectType::ObjView)));
}

void DatabaseModel::removeView(View *view, int obj_idx)
{
	try
	{
		//The relationships that links tables to the view must be removed before erase the view itself
		updateViewRelationships(view, true);

		__removeObject(view, obj_idx);
		PgSQLType::removeUserType(view->getName(true), view);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::updateTableFKRelationships(Table *table)
{
	if(!table)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(table->getDatabase()==this)
	{
		Table *ref_tab=nullptr;
		BaseRelationship *rel=nullptr;
		Constraint *fk=nullptr;
		unsigned idx;
		vector<Constraint *> fks;
		vector<Constraint *>::iterator itr, itr_end;
		vector<BaseObject *>::iterator itr1, itr1_end;

		table->getForeignKeys(fks);
		itr=fks.begin();
		itr_end=fks.end();

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

				//Removes the relationship if the table does'nt references the 'ref_tab'
				if(fk->getReferencedTable() == ref_tab && table->getObjectIndex(fk) < 0)
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
		while(itr!=itr_end)
		{
			fk=(*itr);
			ref_tab=dynamic_cast<Table *>(fk->getReferencedTable());
			itr++;

			//Only creates the relationship if does'nt exist one between the tables
			rel=getRelationship(table, ref_tab, fk);

			if(!rel && ref_tab->getDatabase()==this)
			{
				rel=new BaseRelationship(BaseRelationship::RelationshipFk, table, ref_tab, false, false);
				rel->setReferenceForeignKey(fk);
				rel->setCustomColor(Qt::transparent);

				/* Workaround: In some cases the combination of the two tablenames can generate a duplicated relationship
					 name so it`s necessary to check if a relationship with the same name already exists. If exists changes
					 the name of the new one */
				if(getObjectIndex(rel->getName(), ObjectType::ObjBaseRelationship) >= 0)
					rel->setName(PgModelerNs::generateUniqueName(rel, base_relationships));

				addRelationship(rel);
			}
		}
	}
}

void DatabaseModel::updateTablesFKRelationships(void)
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
	Table *tab=nullptr;
	BaseRelationship *rel=nullptr;
	Reference ref;
	unsigned i, ref_count, idx;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!view)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(getObjectIndex(view) < 0 || force_rel_removal)
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
				if(rel->getTable(BaseRelationship::SrcTable)->getObjectType()==ObjectType::ObjTable)
					tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::SrcTable));
				else
					tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DstTable));

				if(!view->isReferencingTable(tab))
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

		//Creates the relationships from the view references
		ref_count=view->getReferenceCount(Reference::SqlReferSelect);

		for(i=0; i < ref_count; i++)
		{
			ref = view->getReference(i, Reference::SqlReferSelect);
			tab = ref.getTable();

			rel=getRelationship(view,tab);
			if(tab && !rel)
			{
				rel=new BaseRelationship(BaseRelationship::RelationshipDep, view, tab, false, false);
				rel->setName(PgModelerNs::generateUniqueName(rel, base_relationships));
				addRelationship(rel);
			}
		}
	}
}

void DatabaseModel::disconnectRelationships(void)
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

			if(base_rel->getObjectType()==ObjectType::ObjRelationship)
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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::validateRelationships(void)
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
		//Initializes the flag that indicates that some invalid relatioship was found.
		found_inval_rel=false;

		while(itr!=itr_end)
		{
			base_rel=dynamic_cast<BaseRelationship *>(*itr);
			itr++;

			//Validates only table-table relationships
			if(base_rel->getObjectType()==ObjectType::ObjRelationship)
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

						if(std::find(schemas.begin(), schemas.end(), tab1->getSchema())==schemas.end())
							schemas.push_back(dynamic_cast<Schema *>(tab1->getSchema()));
						else if(tab2!=tab1 &&
								std::find(schemas.begin(), schemas.end(), tab1->getSchema())==schemas.end())
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
						if((e.getErrorType()!=InvLinkTablesNoPrimaryKey && conn_tries[rel] > rels_gen_pk) ||
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
						else if(e.getErrorType()==InvLinkTablesNoPrimaryKey &&
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

	if(!loading_model)
	{
		//Updates the schemas to ajdust its sizes due to the tables resizings
		while(!schemas.empty())
		{
			schemas.back()->setModified(true);
			schemas.pop_back();
		}
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
		throw Exception(RemInvalidatedObjects,__PRETTY_FUNCTION__,__FILE__,__LINE__,errors);
	}

	if(!loading_model)
	{
		for(auto &tab : tables)
			dynamic_cast<Table *>(tab)->restoreRelObjectsIndexes();

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
			throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			Table *recv_table=nullptr;
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

				if(base_rel->getObjectType()==ObjectType::ObjRelationship)
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
				msg=Exception::getErrorMessage(InsRelationshipRedundancy)
					.arg(rel->getName())
					.arg(str_aux);
				throw Exception(msg,InsRelationshipRedundancy,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::storeSpecialObjectsXML(void)
{
	unsigned count=0, i=0, type_id=0;
	vector<BaseObject *>::iterator itr, itr_end;
	Sequence *sequence=nullptr;
	Permission *permission=nullptr;
	Table *table=nullptr;
	TableObject *tab_obj=nullptr;
	Constraint *constr=nullptr;
	Index *index=nullptr;
	Trigger *trigger=nullptr;
	View *view=nullptr;
	BaseRelationship *rel=nullptr;
	Reference ref;
	ObjectType tab_obj_type[3]={ ObjectType::ObjConstraint, ObjectType::ObjTrigger, ObjectType::ObjIndex };
	bool found=false;
	vector<BaseObject *> objects, rem_objects, upd_tables_rels;

	try
	{
		itr=tables.begin();
		itr_end=tables.end();

		/* Check on tables if there is some constraint/index/trigger that is referencing
		 some column added by relationship */
		while(itr!=itr_end)
		{
			table=dynamic_cast<Table *>(*itr);
			itr++;

			for(type_id=0; type_id < 3; type_id++)
			{
				//Gets the table object count for the curret object type
				count=table->getObjectCount(tab_obj_type[type_id]);

				for(i=0; i < count; i++)
				{
					tab_obj=dynamic_cast<TableObject *>(table->getObject(i, tab_obj_type[type_id]));
					found=false;

					if(tab_obj_type[type_id]==ObjectType::ObjConstraint)
					{
						constr=dynamic_cast<Constraint *>(tab_obj);

						/* A constraint is considered special in this case when it is referencing
							 relationship added column and the constraint itself was not added by
							 relationship (created manually by the user) */
						found=(!constr->isAddedByRelationship() &&
									 constr->isReferRelationshipAddedColumn() &&
									 constr->getConstraintType()!=ConstraintType::primary_key);

						//When found some special object, stores is xml definition
						if(found)
							xml_special_objs[constr->getObjectId()]=constr->getCodeDefinition(SchemaParser::XmlDefinition, true);
					}
					else if(tab_obj_type[type_id]==ObjectType::ObjTrigger)
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
						if(constr && constr->getConstraintType() == ConstraintType::foreign_key)
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
				delete(sequence);
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
							delete(rel);
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
					delete(obj);
				}

				removeView(view);
				delete(view);
			}
		}

		//Making a copy of the permissions list to avoid iterator invalidation when removing an object
		rem_objects.assign(permissions.begin(), permissions.end());
		itr=permissions.begin();
		itr_end=permissions.end();

		while(itr!=itr_end)
		{
			permission=dynamic_cast<Permission *>(*itr);
			tab_obj=dynamic_cast<TableObject *>(permission->getObject());
			itr++;

			if(tab_obj)
			{
				xml_special_objs[permission->getObjectId()]=permission->getCodeDefinition(SchemaParser::XmlDefinition);
				removePermission(permission);
				delete(permission);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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

		if(obj_type==ObjectType::ObjSequence)
			object=createSequence(true);
		else
			object=createObject(obj_type);

		if(obj_type==ObjectType::ObjSequence)
			addSequence(dynamic_cast<Sequence *>(object));
		else if(obj_type==ObjectType::ObjView)
			addView(dynamic_cast<View *>(object));
		else if(obj_type==ObjectType::ObjPermission)
			addPermission(createPermission());

		/* When the special object is recreated it receive a new id but to maintain
		 the correct creation order, the object has its id restored with the passed
		 id (obj_id) if it is specified */
		if(object && obj_id!=0)
			object->object_id=obj_id;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, xml_def);
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
				msg=Exception::getErrorMessage(InsDuplicatedRelationship)
					.arg(tab1->getName(true))
					.arg(tab1->getTypeName())
					.arg(tab2->getName(true))
					.arg(tab2->getTypeName());
				throw Exception(msg,InsDuplicatedRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		//Before add the relationship, checks if a redundancy can occur case the relationship is added
		if(rel->getObjectType()==ObjectType::ObjRelationship)
			checkRelationshipRedundancy(dynamic_cast<Relationship *>(rel));

		rel->blockSignals(loading_model);
		__addObject(rel, obj_idx);

		if(rel->getObjectType()==ObjectType::ObjRelationship)
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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseModel::removeRelationship(BaseRelationship *rel, int obj_idx)
{
	try
	{
		if(getObjectIndex(rel) >= 0)
		{
			Table *recv_tab=nullptr;

			if(rel->getObjectType()==ObjectType::ObjRelationship)
			{
				/* If the relationship is not a many-to-many we store the receiver table in order to
		   update the fk relationships (if there are any) */
				if(rel->getRelationshipType()!=Relationship::RelationshipNn)
					recv_tab=dynamic_cast<Relationship *>(rel)->getReceiverTable();

				storeSpecialObjectsXML();
				disconnectRelationships();
			}
			else if(rel->getObjectType()==ObjectType::ObjBaseRelationship)
			{
				rel->blockSignals(loading_model);
				rel->disconnectRelationship();
				rel->blockSignals(false);
			}

			__removeObject(rel, obj_idx);

			if(rel->getObjectType()==ObjectType::ObjRelationship)
			{
				validateRelationships();
			}

			//Updating the fk relationships for the receiver table after removing the old relationship
			if(recv_tab)
				updateTableFKRelationships(recv_tab);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

BaseRelationship *DatabaseModel::getRelationship(unsigned obj_idx, ObjectType rel_type)
{
	//Raises an error if the object type used to get a relationship is invalid
	if(rel_type!=ObjectType::ObjRelationship && rel_type!=ObjectType::ObjBaseRelationship)
		throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<BaseRelationship *>(getObject(obj_idx, rel_type)));
}

BaseRelationship *DatabaseModel::getRelationship(const QString &name)
{
	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(getObject(name, ObjectType::ObjBaseRelationship));

	if(!rel)
		rel=dynamic_cast<BaseRelationship *>(getObject(name, ObjectType::ObjRelationship));

	return(rel);
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

		if(ref_fk || src_tab->getObjectType()==ObjectType::ObjView || dst_tab->getObjectType()==ObjectType::ObjView)
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

	return(rel);
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

	return(aux_rels);
}

void DatabaseModel::addTextbox(Textbox *txtbox, int obj_idx)
{
	try
	{
		__addObject(txtbox, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Textbox *DatabaseModel::getTextbox(unsigned obj_idx)
{
	return(dynamic_cast<Textbox *>(getObject(obj_idx, ObjectType::ObjTextbox)));
}

Textbox *DatabaseModel::getTextbox(const QString &name)
{
	return(dynamic_cast<Textbox *>(getObject(name, ObjectType::ObjTextbox)));
}

void DatabaseModel::addSchema(Schema *schema, int obj_idx)
{
	try
	{
		__addObject(schema, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Schema *DatabaseModel::getSchema(unsigned obj_idx)
{
	return(dynamic_cast<Schema *>(getObject(obj_idx, ObjectType::ObjSchema)));
}

Schema *DatabaseModel::getSchema(const QString &name)
{
	return(dynamic_cast<Schema *>(getObject(name, ObjectType::ObjSchema)));
}

void DatabaseModel::removeSchema(Schema *schema, int obj_idx)
{
	try
	{
		__removeObject(schema, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Role *DatabaseModel::getRole(unsigned obj_idx)
{
	return(dynamic_cast<Role *>(getObject(obj_idx, ObjectType::ObjRole)));
}

Role *DatabaseModel::getRole(const QString &name)
{
	return(dynamic_cast<Role *>(getObject(name, ObjectType::ObjRole)));
}

void DatabaseModel::removeRole(Role *role, int obj_idx)
{
	try
	{
		__removeObject(role, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Tablespace *DatabaseModel::getTablespace(unsigned obj_idx)
{
	return(dynamic_cast<Tablespace *>(getObject(obj_idx, ObjectType::ObjTablespace)));
}

Tablespace *DatabaseModel::getTablespace(const QString &name)
{
	return(dynamic_cast<Tablespace *>(getObject(name, ObjectType::ObjTablespace)));
}

void DatabaseModel::removeTablespace(Tablespace *tabspc, int obj_idx)
{
	try
	{
		__removeObject(tabspc, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Cast *DatabaseModel::getCast(unsigned obj_idx)
{
	return(dynamic_cast<Cast *>(getObject(obj_idx, ObjectType::ObjCast)));
}

Cast *DatabaseModel::getCast(const QString &name)
{
	return(dynamic_cast<Cast *>(getObject(name, ObjectType::ObjCast)));
}

void DatabaseModel::addConversion(Conversion *conv, int obj_idx)
{
	try
	{
		__addObject(conv, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Conversion *DatabaseModel::getConversion(unsigned obj_idx)
{
	return(dynamic_cast<Conversion *>(getObject(obj_idx,
												ObjectType::ObjConversion)));
}

Conversion *DatabaseModel::getConversion(const QString &name)
{
	return(dynamic_cast<Conversion *>(getObject(name, ObjectType::ObjConversion)));
}

void DatabaseModel::addLanguage(Language *lang, int obj_idx)
{
	try
	{
		__addObject(lang, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Language *DatabaseModel::getLanguage(unsigned obj_idx)
{
	return(dynamic_cast<Language *>(getObject(obj_idx, ObjectType::ObjLanguage)));
}

Language *DatabaseModel::getLanguage(const QString &name)
{
	return(dynamic_cast<Language *>(getObject(name, ObjectType::ObjLanguage)));
}

void DatabaseModel::removeLanguage(Language *lang, int obj_idx)
{
	try
	{
		__removeObject(lang, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Function *DatabaseModel::getFunction(unsigned obj_idx)
{
	return(dynamic_cast<Function *>(getObject(obj_idx, ObjectType::ObjFunction)));
}

Function *DatabaseModel::getFunction(const QString &signature)
{
	return(dynamic_cast<Function *>(getObject(signature, ObjectType::ObjFunction)));
}

void DatabaseModel::removeFunction(Function *func, int obj_idx)
{
	try
	{
		__removeObject(func, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Aggregate *DatabaseModel::getAggregate(unsigned obj_idx)
{
	return(dynamic_cast<Aggregate *>(getObject(obj_idx, ObjectType::ObjAggregate)));
}

Aggregate *DatabaseModel::getAggregate(const QString &name)
{
	return(dynamic_cast<Aggregate *>(getObject(name, ObjectType::ObjAggregate)));
}

void DatabaseModel::removeAggregate(Aggregate *aggreg, int obj_idx)
{
	try
	{
		__removeObject(aggreg, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			str_aux=Exception::getErrorMessage(AsgDuplicatedObject)
					.arg(domain->getName(true))
					.arg(domain->getTypeName())
					.arg(this->getName(true))
					.arg(this->getTypeName());
			throw Exception(str_aux, AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		try
		{
			__addObject(domain, obj_idx);

			//When added to the model the domain is inserted on the pgsql base type list to be used as a column type
			PgSQLType::addUserType(domain->getName(true), domain, this, UserTypeConfig::DOMAIN_TYPE);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Domain *DatabaseModel::getDomain(unsigned obj_idx)
{
	return(dynamic_cast<Domain *>(getObject(obj_idx, ObjectType::ObjDomain)));
}

Domain *DatabaseModel::getDomain(const QString &name)
{
	return(dynamic_cast<Domain *>(getObject(name, ObjectType::ObjDomain)));
}

void DatabaseModel::addOperatorFamily(OperatorFamily *op_family, int obj_idx)
{
	try
	{
		__addObject(op_family, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

OperatorFamily *DatabaseModel::getOperatorFamily(unsigned obj_idx)
{
	return(dynamic_cast<OperatorFamily *>(getObject(obj_idx, ObjectType::ObjOpFamily)));
}

OperatorFamily *DatabaseModel::getOperatorFamily(const QString &name)
{
	return(dynamic_cast<OperatorFamily *>(getObject(name, ObjectType::ObjOpFamily)));
}

void DatabaseModel::removeOperatorFamily(OperatorFamily *op_family, int obj_idx)
{
	try
	{
		__removeObject(op_family, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

OperatorClass *DatabaseModel::getOperatorClass(unsigned obj_idx)
{
	return(dynamic_cast<OperatorClass *>(getObject(obj_idx, ObjectType::ObjOpClass)));
}

OperatorClass *DatabaseModel::getOperatorClass(const QString &name)
{
	return(dynamic_cast<OperatorClass *>(getObject(name, ObjectType::ObjOpClass)));
}

void DatabaseModel::addOperator(Operator *oper, int obj_idx)
{
	try
	{
		__addObject(oper, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Operator *DatabaseModel::getOperator(unsigned obj_idx)
{
	return(dynamic_cast<Operator *>(getObject(obj_idx, ObjectType::ObjOperator)));
}

Operator *DatabaseModel::getOperator(const QString &signature)
{
	return(dynamic_cast<Operator *>(getObject(signature, ObjectType::ObjOperator)));
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
			str_aux=Exception::getErrorMessage(AsgDuplicatedObject)
					.arg(type->getName(true))
					.arg(type->getTypeName())
					.arg(this->getName(true))
					.arg(this->getTypeName());
			throw Exception(str_aux, AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		try
		{
			__addObject(type, obj_idx);

			//When added to the model the user type is inserted on the pgsql base type list to be used as a column type
			PgSQLType::addUserType(type->getName(true), type, this, UserTypeConfig::BASE_TYPE);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Type *DatabaseModel::getType(unsigned obj_idx)
{
	return(dynamic_cast<Type *>(getObject(obj_idx, ObjectType::ObjType)));
}

Type *DatabaseModel::getType(const QString &name)
{
	return(dynamic_cast<Type *>(getObject(name, ObjectType::ObjType)));
}

void DatabaseModel::removeUserType(BaseObject *object, int obj_idx)
{
	try
	{
		__removeObject(object, obj_idx);

		//Removes the user type from the list of base types of pgsql
		PgSQLType::removeUserType(object->getName(true), object);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::addPermission(Permission *perm)
{
	try
	{
		if(!perm)
			throw Exception(AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		TableObject *tab_obj=dynamic_cast<TableObject *>(perm->getObject());

		if(getPermissionIndex(perm, false) >=0)
		{
			throw Exception(Exception::getErrorMessage(AsgDuplicatedPermission)
							.arg(perm->getObject()->getName())
							.arg(perm->getObject()->getTypeName()),
							AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Raises an error if the permission is referencing an object that does not exists on model
		else if(perm->getObject()!=this &&
				((tab_obj && (getObjectIndex(tab_obj->getParentTable()) < 0)) ||
				 (!tab_obj && (getObjectIndex(perm->getObject()) < 0))))
			throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
							.arg(perm->getName())
							.arg(perm->getObject()->getTypeName())
							.arg(perm->getObject()->getName())
							.arg(perm->getObject()->getTypeName()),
							RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		permissions.push_back(perm);
		perm->setDatabase(this);
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==AsgDuplicatedObject)
			throw
			Exception(Exception::getErrorMessage(AsgDuplicatedPermission)
					  .arg(perm->getObject()->getName())
					  .arg(perm->getObject()->getTypeName()),
						AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

		else
			throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::removePermissions(BaseObject *object)
{
	vector<BaseObject *>::iterator itr, itr_end;
	Permission *perm=nullptr;
	unsigned idx=0;

	if(!object)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

	return(perm_idx);
}

BaseObject *DatabaseModel::getObject(const QString &name, ObjectType obj_type)
{
	int idx;
	return(getObject(name, obj_type, idx));
}

int DatabaseModel::getObjectIndex(const QString &name, ObjectType obj_type)
{
	int idx;
	getObject(name, obj_type, idx);
	return(idx);
}

int DatabaseModel::getObjectIndex(BaseObject *object)
{
	if(!object)
		return(-1);
	else
	{
		ObjectType obj_type=object->getObjectType();
		vector<BaseObject *> *obj_list=nullptr;
		vector<BaseObject *>::iterator itr, itr_end;
		bool found=false;

		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
				return(itr-obj_list->begin());
			else
				return(-1);
		}
	}
}

void DatabaseModel::configureDatabase(attribs_map &attribs)
{
	encoding=attribs[ParsersAttributes::ENCODING];
	template_db=attribs[ParsersAttributes::TEMPLATE_DB];
	localizations[0]=attribs[ParsersAttributes::_LC_CTYPE_];
	localizations[1]=attribs[ParsersAttributes::_LC_COLLATE_];
	append_at_eod=attribs[ParsersAttributes::APPEND_AT_EOD]==ParsersAttributes::_TRUE_;
	prepend_at_bod=attribs[ParsersAttributes::PREPEND_AT_BOD]==ParsersAttributes::_TRUE_;
	is_template=attribs[ParsersAttributes::IS_TEMPLATE]==ParsersAttributes::_TRUE_;
	allow_conns=attribs[ParsersAttributes::ALLOW_CONNS] != ParsersAttributes::_FALSE_;

	if(!attribs[ParsersAttributes::CONN_LIMIT].isEmpty())
		conn_limit=attribs[ParsersAttributes::CONN_LIMIT].toInt();

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
		dtd_file=GlobalAttributes::SchemasRootDir +
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

			setObjectListsCapacity(attribs[ParsersAttributes::MAX_OBJ_COUNT].toUInt());

			this->author=attribs[ParsersAttributes::MODEL_AUTHOR];

			pos_str=attribs[ParsersAttributes::LAST_POSITION].split(',');

			if(pos_str.size()>=2)
				this->last_pos=QPoint(pos_str[0].toUInt(),pos_str[1].toUInt());

			this->last_zoom=attribs[ParsersAttributes::LAST_ZOOM].toDouble();
			if(this->last_zoom <= 0) this->last_zoom=1;

			this->is_template = attribs[ParsersAttributes::IS_TEMPLATE] == ParsersAttributes::_TRUE_;
			this->allow_conns = (attribs[ParsersAttributes::ALLOW_CONNS].isEmpty() ||
													 attribs[ParsersAttributes::ALLOW_CONNS] == ParsersAttributes::_TRUE_);

			protected_model=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

			def_objs[ObjectType::ObjSchema]=attribs[ParsersAttributes::DEFAULT_SCHEMA];
			def_objs[ObjectType::ObjRole]=attribs[ParsersAttributes::DEFAULT_OWNER];
			def_objs[ObjectType::ObjCollation]=attribs[ParsersAttributes::DEFAULT_COLLATION];
			def_objs[ObjectType::ObjTablespace]=attribs[ParsersAttributes::DEFAULT_TABLESPACE];

			if(xmlparser.accessElement(XmlParser::ChildElement))
			{
				do
				{
					if(xmlparser.getElementType()==XML_ELEMENT_NODE)
					{
						elem_name=xmlparser.getElementName();

						//Indentifies the object type to be load according to the current element on the parser
						obj_type=getObjectType(elem_name);

						if(obj_type==ObjectType::ObjDatabase)
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
									if(!dynamic_cast<TableObject *>(object) && obj_type!=ObjectType::ObjRelationship && obj_type!=ObjectType::ObjBaseRelationship)
										addObject(object);

									/* If there is at least one inheritance relationship we need to flag this situation
									 in order to do an addtional rel. validation in the end of loading */
									if(!found_inh_rel && object->getObjectType()==ObjectType::ObjRelationship &&
											dynamic_cast<Relationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipGen)
										found_inh_rel=true;

									emit s_objectLoaded((xmlparser.getCurrentBufferLine()/static_cast<float>(xmlparser.getBufferLineCount()))*100,
														trUtf8("Loading: `%1' (%2)")
														.arg(object->getName())
														.arg(object->getTypeName()),
														obj_type);
								}

								xmlparser.restorePosition();
							}
							catch(Exception &e)
							{
								QString info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);
								throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
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
						throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
										.arg(this->getName())
										.arg(this->getTypeName())
										.arg(itr.second)
										.arg(BaseObject::getTypeName(itr.first)),
										AsgDuplicatedPermission,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					this->setDefaultObject(object);
				}
				else
					this->setDefaultObject(nullptr, itr.first);
			}

			loading_model=false;

			//If there are relationship make a relationship validation to recreate any special object left behind
			if(!relationships.empty())
			{
				emit s_objectLoaded(100, trUtf8("Validating relationships..."), ObjectType::ObjRelationship);
				storeSpecialObjectsXML();
				disconnectRelationships();
				validateRelationships();
			}

			this->setInvalidated(false);

			emit s_objectLoaded(100, trUtf8("Validating relationships..."), ObjectType::ObjRelationship);
			this->setObjectsModified({ObjectType::ObjRelationship, ObjectType::ObjBaseRelationship});

			//Doing another relationship validation when there are inheritances to avoid incomplete tables
			if(found_inh_rel)
			{
				emit s_objectLoaded(100, trUtf8("Validating relationships..."), ObjectType::ObjRelationship);
				validateRelationships();
				updateTablesFKRelationships();
			}
		}
		catch(Exception &e)
		{
			QString extra_info;
			loading_model=false;

			if(xmlparser.getCurrentElement())
				extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);

			if(e.getErrorType()>=InvalidSyntax)
			{
				str_aux=Exception::getErrorMessage(InvModelFileNotLoaded).arg(filename);
				throw Exception(str_aux,InvModelFileNotLoaded,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
			}
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
		}
	}
}

BaseObject *DatabaseModel::createObject(ObjectType obj_type)
{
	BaseObject *object=nullptr;

	if(obj_type!=ObjectType::ObjBaseObject)
	{
		if(obj_type==ObjectType::ObjRole)
			object=createRole();
		else if(obj_type==ObjectType::ObjTablespace)
			object=createTablespace();
		else if(obj_type==ObjectType::ObjSchema)
			object=createSchema();
		else if(obj_type==ObjectType::ObjLanguage)
			object=createLanguage();
		else if(obj_type==ObjectType::ObjFunction)
			object=createFunction();
		else if(obj_type==ObjectType::ObjType)
			object=createType();
		else if(obj_type==ObjectType::ObjDomain)
			object=createDomain();
		else if(obj_type==ObjectType::ObjCast)
			object=createCast();
		else if(obj_type==ObjectType::ObjConversion)
			object=createConversion();
		else if(obj_type==ObjectType::ObjOperator)
			object=createOperator();
		else if(obj_type==ObjectType::ObjOpFamily)
			object=createOperatorFamily();
		else if(obj_type==ObjectType::ObjOpClass)
			object=createOperatorClass();
		else if(obj_type==ObjectType::ObjAggregate)
			object=createAggregate();
		else if(obj_type==ObjectType::ObjTable)
			object=createTable();
		else if(obj_type==ObjectType::ObjSequence)
			object=createSequence();
		else if(obj_type==ObjectType::ObjView)
			object=createView();
		else if(obj_type==ObjectType::ObjTextbox)
			object=createTextbox();
		else if(obj_type==ObjectType::ObjConstraint)
			object=createConstraint(nullptr);
		else if(obj_type==ObjectType::ObjTrigger)
			object=createTrigger();
		else if(obj_type==ObjectType::ObjIndex)
			object=createIndex();
		else if(obj_type==ObjectType::ObjColumn)
			object=createColumn();
		else if(obj_type==ObjectType::ObjRule)
			object=createRule();
		else if(obj_type==ObjectType::ObjRelationship ||
				obj_type==ObjectType::ObjBaseRelationship)
			object=createRelationship();
		else if(obj_type==ObjectType::ObjCollation)
			object=createCollation();
		else if(obj_type==ObjectType::ObjExtension)
			object=createExtension();
		else if(obj_type==ObjectType::ObjTag)
			object=createTag();
		else if(obj_type==ObjectType::ObjPermission)
			object=createPermission();
		else if(obj_type==ObjectType::ObjEventTrigger)
			object=createEventTrigger();
		else if(obj_type==ObjectType::ObjGenericSQL)
			object=createGenericSQL();
		else if(obj_type==ObjectType::ObjPolicy)
			object=createPolicy();
	}

	return(object);
}

void DatabaseModel::setBasicAttributes(BaseObject *object)
{
	attribs_map attribs, attribs_aux;
	QString elem_name;
	BaseObject *tabspc=nullptr, *owner=nullptr, *collation=nullptr;
	Schema *schema=nullptr;
	ObjectType obj_type=ObjectType::ObjBaseObject, obj_type_aux;
	bool has_error=false, protected_obj=false, sql_disabled=false;

	if(!object)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	xmlparser.getElementAttributes(attribs);

	obj_type_aux=object->getObjectType();

	if(obj_type_aux!=ObjectType::ObjCast)
		object->setName(attribs[ParsersAttributes::NAME]);

	if(BaseObject::acceptsAlias(obj_type_aux))
		object->setAlias(attribs[ParsersAttributes::ALIAS]);

	protected_obj=attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;
	sql_disabled=attribs[ParsersAttributes::SQL_DISABLED]==ParsersAttributes::_TRUE_;

	xmlparser.savePosition();

	if(xmlparser.accessElement(XmlParser::ChildElement))
	{
		do
		{
			if(xmlparser.getElementType()==XML_ELEMENT_NODE)
			{
				elem_name=xmlparser.getElementName();

				//Defines the object's comment
				if(elem_name==ParsersAttributes::COMMENT)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setComment(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::SCHEMA)
				{
					obj_type=ObjectType::ObjSchema;
					xmlparser.getElementAttributes(attribs_aux);
					schema=dynamic_cast<Schema *>(getObject(attribs_aux[ParsersAttributes::NAME], obj_type));
					object->setSchema(schema);
					has_error=(!schema && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's tablespace
				else if(elem_name==ParsersAttributes::TABLESPACE)
				{
					obj_type=ObjectType::ObjTablespace;
					xmlparser.getElementAttributes(attribs_aux);
					tabspc=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setTablespace(tabspc);
					has_error=(!tabspc && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's owner
				else if(elem_name==ParsersAttributes::ROLE)
				{
					obj_type=ObjectType::ObjRole;
					xmlparser.getElementAttributes(attribs_aux);
					owner=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setOwner(owner);
					has_error=(!owner && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::COLLATION)
				{
					obj_type=ObjectType::ObjCollation;
					xmlparser.getElementAttributes(attribs_aux);
					collation=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setCollation(collation);
					has_error=(!collation && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				else if(elem_name==ParsersAttributes::APPENDED_SQL)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setAppendedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				else if(elem_name==ParsersAttributes::PREPENDED_SQL)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XmlParser::ChildElement);
					object->setPrependedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				//Defines the object's position (only for graphical objects)
				else if(elem_name==ParsersAttributes::POSITION)
				{
					xmlparser.getElementAttributes(attribs);

					if(elem_name==ParsersAttributes::POSITION &&
							(obj_type_aux!=ObjectType::ObjRelationship &&
							 obj_type_aux!=ObjectType::ObjBaseRelationship))
					{
						dynamic_cast<BaseGraphicObject *>(object)->setPosition(QPointF(attribs[ParsersAttributes::X_POS].toDouble(),
																			   attribs[ParsersAttributes::Y_POS].toDouble()));


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
		throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
						.arg(object->getName())
						.arg(object->getTypeName())
						.arg(attribs_aux[ParsersAttributes::NAME])
				.arg(BaseObject::getTypeName(obj_type)),
				RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(!object->getSchema() &&
			(obj_type_aux==ObjectType::ObjFunction || obj_type_aux==ObjectType::ObjTable ||
			 obj_type_aux==ObjectType::ObjView  || obj_type_aux==ObjectType::ObjDomain ||
			 obj_type_aux==ObjectType::ObjAggregate || obj_type_aux==ObjectType::ObjOperator ||
			 obj_type_aux==ObjectType::ObjSequence || obj_type_aux==ObjectType::ObjConversion ||
			 obj_type_aux==ObjectType::ObjType || obj_type_aux==ObjectType::ObjOpFamily ||
			 obj_type_aux==ObjectType::ObjOpClass))
	{
		throw Exception(Exception::getErrorMessage(InvObjectAllocationNoSchema)
						.arg(object->getName())
						.arg(object->getTypeName()),
						InvObjectAllocationNoSchema,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

QString DatabaseModel::getErrorExtraInfo(void)
{
	QString extra_info;

	if(!xmlparser.getLoadedFilename().isEmpty())
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(xmlparser.getLoadedFilename())
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

QPoint DatabaseModel::getLastPosition(void)
{
	return(last_pos);
}

void DatabaseModel::setLastZoomFactor(double zoom)
{
	last_zoom=zoom;
}

double DatabaseModel::getLastZoomFactor(void)
{
	return(last_zoom);
}

Role *DatabaseModel::createRole(void)
{
	attribs_map attribs, attribs_aux;
	Role *role=nullptr, *ref_role=nullptr;
	int i, len;
	bool marked;
	QStringList list;
	QString elem_name;
	unsigned role_type;

	QString op_attribs[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
						   ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
						   ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED,
							 ParsersAttributes::REPLICATION, ParsersAttributes::BYPASSRLS };

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

		role->setPassword(attribs[ParsersAttributes::PASSWORD]);
		role->setValidity(attribs[ParsersAttributes::VALIDITY]);

		if(!attribs[ParsersAttributes::CONN_LIMIT].isEmpty())
			role->setConnectionLimit(attribs[ParsersAttributes::CONN_LIMIT].toInt());

		//Setting up the role options according to the configured on the XML
		for(i=0; i < 8; i++)
		{
			marked=attribs[op_attribs[i]]==ParsersAttributes::_TRUE_;
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
					if(elem_name==ParsersAttributes::ROLES)
					{
						//Gets the member roles attributes
						xmlparser.getElementAttributes(attribs_aux);

						//The member roles names are separated by comma, so it is needed to split them
						list=attribs_aux[ParsersAttributes::NAMES].split(',');
						len=list.size();

						//Identifying the member role type
						if(attribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::REFER)
							role_type=Role::RefRole;
						else if(attribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::MEMBER)
							role_type=Role::MemberRole;
						else
							role_type=Role::AdminRole;

						for(i=0; i < len; i++)
						{
							//Gets the role using the name from the model using the name from the list
							ref_role=dynamic_cast<Role *>(getObject(list[i].trimmed(),ObjectType::ObjRole));

							//Raises an error if the roles doesn't exists
							if(!ref_role)
							{
								throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
												.arg(role->getName())
												.arg(BaseObject::getTypeName(ObjectType::ObjRole))
												.arg(list[i])
												.arg(BaseObject::getTypeName(ObjectType::ObjRole)),
												RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		if(role) delete(role);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(role);
}

Tablespace *DatabaseModel::createTablespace(void)
{
	attribs_map attribs;
	Tablespace *tabspc=nullptr;

	try
	{
		tabspc=new Tablespace;
		setBasicAttributes(tabspc);
		xmlparser.getElementAttributes(attribs);
		tabspc->setDirectory(attribs[ParsersAttributes::DIRECTORY]);
	}
	catch(Exception &e)
	{
		if(tabspc) delete(tabspc);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(tabspc);
}

Schema *DatabaseModel::createSchema(void)
{
	Schema *schema=nullptr;
	attribs_map attribs;

	try
	{
		schema=new Schema;
		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(schema);
		schema->setFillColor(QColor(attribs[ParsersAttributes::FILL_COLOR]));
		schema->setRectVisible(attribs[ParsersAttributes::RECT_VISIBLE]==ParsersAttributes::_TRUE_);
		schema->setFadedOut(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);
	}
	catch(Exception &e)
	{
		if(schema) delete(schema);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(schema);
}

Language *DatabaseModel::createLanguage(void)
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

		lang->setTrusted(attribs[ParsersAttributes::TRUSTED]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					obj_type=BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type==ObjectType::ObjFunction)
					{
						xmlparser.getElementAttributes(attribs);

						//Gets the function reference type
						ref_type=attribs[ParsersAttributes::REF_TYPE];

						//Only VALIDATOR, HANDLER and INLINE functions are accepted for the language
						if(ref_type==ParsersAttributes::VALIDATOR_FUNC ||
								ref_type==ParsersAttributes::HANDLER_FUNC ||
								ref_type==ParsersAttributes::INLINE_FUNC)
						{
							//Gets the function signature and tries to retrieve it from the model
							signature=attribs[ParsersAttributes::SIGNATURE];
							func=getObject(signature, ObjectType::ObjFunction);

							//Raises an error if the function doesn't exists
							if(!func)
								throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
												.arg(lang->getName())
												.arg(lang->getTypeName())
												.arg(signature)
												.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
												RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

							if(ref_type==ParsersAttributes::VALIDATOR_FUNC)
								lang->setFunction(dynamic_cast<Function *>(func), Language::ValidatorFunc);
							else if(ref_type==ParsersAttributes::HANDLER_FUNC)
								lang->setFunction(dynamic_cast<Function *>(func), Language::HandlerFunc);
							else
								lang->setFunction(dynamic_cast<Function *>(func), Language::InlineFunc);

						}
						else
							//Raises an error if the function type is invalid
							throw Exception(RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

	}
	catch(Exception &e)
	{
		if(lang) delete(lang);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(lang);
}

Function *DatabaseModel::createFunction(void)
{
	attribs_map attribs, attribs_aux;
	Function *func=nullptr;
	ObjectType obj_type;
	BaseObject *object=nullptr;
	PgSQLType type;
	Parameter param;
	QString str_aux, elem;

	try
	{
		func=new Function;
		setBasicAttributes(func);
		xmlparser.getElementAttributes(attribs);

		if(!attribs[ParsersAttributes::RETURNS_SETOF].isEmpty())
			func->setReturnSetOf(attribs[ParsersAttributes::RETURNS_SETOF]==
					ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::WINDOW_FUNC].isEmpty())
			func->setWindowFunction(attribs[ParsersAttributes::WINDOW_FUNC]==
					ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::LEAKPROOF].isEmpty())
			func->setLeakProof(attribs[ParsersAttributes::LEAKPROOF]==
					ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::BEHAVIOR_TYPE].isEmpty())
			func->setBehaviorType(BehaviorType(attribs[ParsersAttributes::BEHAVIOR_TYPE]));

		if(!attribs[ParsersAttributes::FUNCTION_TYPE].isEmpty())
			func->setFunctionType(FunctionType(attribs[ParsersAttributes::FUNCTION_TYPE]));

		if(!attribs[ParsersAttributes::SECURITY_TYPE].isEmpty())
			func->setSecurityType(SecurityType(attribs[ParsersAttributes::SECURITY_TYPE]));

		if(!attribs[ParsersAttributes::EXECUTION_COST].isEmpty())
			func->setExecutionCost(attribs[ParsersAttributes::EXECUTION_COST].toInt());

		if(!attribs[ParsersAttributes::ROW_AMOUNT].isEmpty())
			func->setRowAmount(attribs[ParsersAttributes::ROW_AMOUNT].toInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();
					obj_type=BaseObject::getObjectType(elem);

					//Gets the function return type from the XML
					if(elem==ParsersAttributes::RETURN_TYPE)
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
									if(xmlparser.getElementName()==ParsersAttributes::TYPE)
									{
										type=createPgSQLType();
										func->setReturnType(type);
									}
									//when the element found is a PARAMETER indicates that the function return type is a table
									else if(xmlparser.getElementName()==ParsersAttributes::PARAMETER)
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
							throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
						}
					}
					//Gets the function language
					else if(obj_type==ObjectType::ObjLanguage)
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::NAME], obj_type);

						//Raises an error if the function doesn't exisits
						if(!object)
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(func->getName())
											.arg(func->getTypeName())
											.arg(attribs[ParsersAttributes::NAME])
								.arg(BaseObject::getTypeName(ObjectType::ObjLanguage)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						func->setLanguage(dynamic_cast<Language *>(object));
					}
					//Gets a function parameter
					else if(xmlparser.getElementName()==ParsersAttributes::PARAMETER)
					{
						param=createParameter();
						func->addParameter(param);
					}
					//Gets the function code definition
					else if(xmlparser.getElementName()==ParsersAttributes::DEFINITION)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs_aux);

						if(!attribs_aux[ParsersAttributes::LIBRARY].isEmpty())
						{
							func->setLibrary(attribs_aux[ParsersAttributes::LIBRARY]);
							func->setSymbol(attribs_aux[ParsersAttributes::SYMBOL]);
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
			delete(func);
		}

		if(e.getErrorType()==RefUserTypeInexistsModel)
			throw Exception(Exception::getErrorMessage(AsgObjectInvalidDefinition)
							.arg(str_aux)
							.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
							AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(func);
}

Parameter DatabaseModel::createParameter(void)
{
	Parameter param;
	attribs_map attribs;
	QString elem;

	try
	{
		xmlparser.savePosition();
		xmlparser.getElementAttributes(attribs);

		param.setName(attribs[ParsersAttributes::NAME]);
		param.setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						param.setType(createPgSQLType());
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		param.setIn(attribs[ParsersAttributes::PARAM_IN]==ParsersAttributes::_TRUE_);
		param.setOut(attribs[ParsersAttributes::PARAM_OUT]==ParsersAttributes::_TRUE_);
		param.setVariadic(attribs[ParsersAttributes::PARAM_VARIADIC]==ParsersAttributes::_TRUE_);

		xmlparser.restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info=getErrorExtraInfo();
		xmlparser.restorePosition();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(param);
}

TypeAttribute DatabaseModel::createTypeAttribute(void)
{
	TypeAttribute tpattrib;
	attribs_map attribs;
	QString elem;
	BaseObject *collation=nullptr;

	try
	{
		xmlparser.savePosition();
		xmlparser.getElementAttributes(attribs);

		tpattrib.setName(attribs[ParsersAttributes::NAME]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						tpattrib.setType(createPgSQLType());
					}
					else if(elem==ParsersAttributes::COLLATION)
					{
						xmlparser.getElementAttributes(attribs);

						collation=getObject(attribs[ParsersAttributes::NAME], ObjectType::ObjCollation);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(tpattrib.getName())
											.arg(tpattrib.getTypeName())
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjCollation)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(tpattrib);
}

PgSQLType DatabaseModel::createPgSQLType(void)
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

	if(!attribs[ParsersAttributes::LENGTH].isEmpty())
		length=attribs[ParsersAttributes::LENGTH].toUInt();

	if(!attribs[ParsersAttributes::DIMENSION].isEmpty())
		dimension=attribs[ParsersAttributes::DIMENSION].toUInt();

	if(!attribs[ParsersAttributes::PRECISION].isEmpty())
		precision=attribs[ParsersAttributes::PRECISION].toInt();

	with_timezone=(attribs[ParsersAttributes::WITH_TIMEZONE]==ParsersAttributes::_TRUE_);
	interv_type=attribs[ParsersAttributes::INTERVAL_TYPE];

	if(!attribs[ParsersAttributes::SPATIAL_TYPE].isEmpty())
		spatial_type=SpatialType(attribs[ParsersAttributes::SPATIAL_TYPE],
				attribs[ParsersAttributes::SRID].toUInt(),
				attribs[ParsersAttributes::VARIATION].toUInt());

	name=attribs[ParsersAttributes::NAME];

	/* A small tweak to detect a timestamp/date type which name contains the time zone modifier.
		 This situation can occur mainly on reverse engineering operation where the data type of objects
		 in most of times came as string form and need to be parsed */
	if(!with_timezone && attribs[ParsersAttributes::NAME].contains(QString("with time zone"), Qt::CaseInsensitive))
	{
		with_timezone=true;
		name.remove(QString(" with time zone"), Qt::CaseInsensitive);
	}

	type_idx=PgSQLType::getBaseTypeIndex(name);
	if(type_idx!=PgSQLType::null)
	{
		return(PgSQLType(name,length,dimension,precision,with_timezone,interv_type, spatial_type));
	}
	else
	{
		//Raises an error if the referenced type name doesn't exists
		if(PgSQLType::getUserTypeIndex(name,nullptr,this) == BaseType::null)
			throw Exception(RefUserTypeInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		type_idx=PgSQLType::getUserTypeIndex(name, ptype);
		return(PgSQLType(type_idx,length,dimension,precision,with_timezone,interv_type,spatial_type));
	}
}

Type *DatabaseModel::createType(void)
{
	attribs_map attribs;
	map<QString, unsigned> func_types;
	Type *type=nullptr;
	int count, i;
	QStringList enums;
	QString elem, str_aux;
	BaseObject *func=nullptr, *collation=nullptr;
	OperatorClass *op_class=nullptr;
	PgSQLType aux_type;

	try
	{
		type=new Type;
		setBasicAttributes(type);
		xmlparser.getElementAttributes(attribs);

		if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE)
		{
			type->setConfiguration(Type::BaseType);
			type->setByValue(attribs[ParsersAttributes::BY_VALUE]==ParsersAttributes::_TRUE_);

			if(!attribs[ParsersAttributes::INTERNAL_LENGTH].isEmpty())
				type->setInternalLength(attribs[ParsersAttributes::INTERNAL_LENGTH].toUInt());

			if(!attribs[ParsersAttributes::ALIGNMENT].isEmpty())
				type->setAlignment(attribs[ParsersAttributes::ALIGNMENT]);

			if(!attribs[ParsersAttributes::STORAGE].isEmpty())
				type->setStorage(attribs[ParsersAttributes::STORAGE]);

			if(!attribs[ParsersAttributes::ELEMENT].isEmpty())
				type->setElement(attribs[ParsersAttributes::ELEMENT]);

			if(!attribs[ParsersAttributes::DELIMITER].isEmpty())
				type->setDelimiter(attribs[ParsersAttributes::DELIMITER][0].toLatin1());

			if(!attribs[ParsersAttributes::DEFAULT_VALUE].isEmpty())
				type->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

			if(!attribs[ParsersAttributes::CATEGORY].isEmpty())
				type->setCategory(attribs[ParsersAttributes::CATEGORY]);

			if(!attribs[ParsersAttributes::PREFERRED].isEmpty())
				type->setPreferred(attribs[ParsersAttributes::PREFERRED]==ParsersAttributes::_TRUE_);

			//Configuring an auxiliary map used to reference the functions used by base type
			func_types[ParsersAttributes::INPUT_FUNC]=Type::InputFunc;
			func_types[ParsersAttributes::OUTPUT_FUNC]=Type::OutputFunc;
			func_types[ParsersAttributes::SEND_FUNC]=Type::SendFunc;
			func_types[ParsersAttributes::RECV_FUNC]=Type::RecvFunc;
			func_types[ParsersAttributes::TPMOD_IN_FUNC]=Type::TpmodInFunc;
			func_types[ParsersAttributes::TPMOD_OUT_FUNC]=Type::TpmodOutFunc;
			func_types[ParsersAttributes::ANALYZE_FUNC]=Type::AnalyzeFunc;
		}
		else if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::COMPOSITE_TYPE)
			type->setConfiguration(Type::CompositeType);
		else if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::ENUM_TYPE)
			type->setConfiguration(Type::EnumerationType);
		else
		{
			type->setConfiguration(Type::RangeType);
			func_types[ParsersAttributes::CANONICAL_FUNC]=Type::CanonicalFunc;
			func_types[ParsersAttributes::SUBTYPE_DIFF_FUNC]=Type::SubtypeDiffFunc;
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//Specific operations for ENUM type
					if(elem==ParsersAttributes::ENUM_TYPE)
					{
						xmlparser.getElementAttributes(attribs);
						enums=attribs[ParsersAttributes::VALUES].split(',');

						count=enums.size();
						for(i=0; i < count; i++)
							type->addEnumeration(enums[i]);
					}
					//Specific operations for COMPOSITE types
					else if(elem==ParsersAttributes::TYPE_ATTRIBUTE)
					{
						type->addAttribute(createTypeAttribute());
					}
					//Specific operations for BASE / RANGE type
					else if(elem==ParsersAttributes::TYPE)
					{
						aux_type=createPgSQLType();

						if(type->getConfiguration()==Type::RangeType)
							type->setSubtype(aux_type);
						else
							type->setLikeType(aux_type);
					}
					else if(elem==ParsersAttributes::COLLATION)
					{
						xmlparser.getElementAttributes(attribs);
						collation=getObject(attribs[ParsersAttributes::NAME], ObjectType::ObjCollation);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjCollation)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setCollation(collation);
					}
					if(elem==ParsersAttributes::OP_CLASS)
					{
						xmlparser.getElementAttributes(attribs);
						op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], ObjectType::ObjOpClass));

						//Raises an error if the operator class doesn't exists
						if(!op_class)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjOpClass)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setSubtypeOpClass(op_class);
					}
					//Configuring the functions used by the type (only for BASE type)
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);

						//Tries to get the function from the model
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(type->getName())
											.arg(type->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						//Raises an error if the function type is invalid
						else if(func_types.count(attribs[ParsersAttributes::REF_TYPE])==0)
							throw Exception(RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						type->setFunction(func_types[attribs[ParsersAttributes::REF_TYPE]],	dynamic_cast<Function *>(func));
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
			delete(type);
		}

		if(e.getErrorType()==RefUserTypeInexistsModel)
			throw Exception(Exception::getErrorMessage(AsgObjectInvalidDefinition)
							.arg(str_aux)
							.arg(type->getTypeName()),
							AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(type);
}

Domain *DatabaseModel::createDomain(void)
{
	attribs_map attribs;
	Domain *domain=nullptr;
	QString elem;

	try
	{
		domain=new Domain;
		setBasicAttributes(domain);
		xmlparser.getElementAttributes(attribs);

		if(!attribs[ParsersAttributes::DEFAULT_VALUE].isEmpty())
			domain->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		domain->setNotNull(attribs[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//If a type element is found it'll be extracted an type which the domain is applied
					if(elem==ParsersAttributes::TYPE)
					{
						domain->setType(createPgSQLType());
					}
					else if(elem==ParsersAttributes::CONSTRAINT)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						xmlparser.accessElement(XmlParser::ChildElement);
						xmlparser.accessElement(XmlParser::ChildElement);
						domain->addCheckConstraint(attribs[ParsersAttributes::NAME], xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(xmlparser.NextElement));
		}
	}
	catch(Exception &e)
	{
		if(domain) delete(domain);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(domain);
}

Cast *DatabaseModel::createCast(void)
{
	attribs_map attribs;
	Cast *cast=nullptr;
	QString elem;
	unsigned type_idx=0;
	PgSQLType type;
	BaseObject *func=nullptr;

	try
	{
		cast=new Cast;
		setBasicAttributes(cast);
		xmlparser.getElementAttributes(attribs);

		if(attribs[ParsersAttributes::CAST_TYPE]==ParsersAttributes::IMPLICIT)
			cast->setCastType(Cast::Implicit);
		else if(attribs[ParsersAttributes::CAST_TYPE]==ParsersAttributes::ASSIGNMENT)
			cast->setCastType(Cast::Assignment);
		else
			cast->setCastType(Cast::Explicit);

		cast->setInOut(attribs[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					//Extract one argument type from the XML
					if(elem==ParsersAttributes::TYPE)
					{
						type=createPgSQLType();
						if(type_idx==0)
							cast->setDataType(Cast::SrcType, type);
						else
							cast->setDataType(Cast::DstType, type);
						type_idx++;
					}
					//Extracts the conversion function
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(cast->getName())
											.arg(cast->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						cast->setCastFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(cast) delete(cast);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(cast);
}

Conversion *DatabaseModel::createConversion(void)
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
						  EncodingType(attribs[ParsersAttributes::SRC_ENCODING]));

		conv->setEncoding(Conversion::DstEncoding,
						  EncodingType(attribs[ParsersAttributes::DST_ENCODING]));

		conv->setDefault(attribs[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(conv->getName())
											.arg(conv->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						conv->setConversionFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(conv) delete(conv);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(conv);
}

Operator *DatabaseModel::createOperator(void)
{
	attribs_map attribs;
	map<QString, unsigned> func_types;
	map<QString, unsigned> oper_types;
	Operator *oper=nullptr;
	QString elem;
	BaseObject *func=nullptr,*oper_aux=nullptr;
	unsigned arg_type;
	PgSQLType type;

	try
	{
		oper=new Operator;
		setBasicAttributes(oper);
		xmlparser.getElementAttributes(attribs);

		oper->setMerges(attribs[ParsersAttributes::MERGES]==ParsersAttributes::_TRUE_);
		oper->setHashes(attribs[ParsersAttributes::HASHES]==ParsersAttributes::_TRUE_);

		func_types[ParsersAttributes::OPERATOR_FUNC]=Operator::FUNC_OPERATOR;
		func_types[ParsersAttributes::JOIN_FUNC]=Operator::FUNC_JOIN;
		func_types[ParsersAttributes::RESTRICTION_FUNC]=Operator::FUNC_RESTRICT;

		oper_types[ParsersAttributes::COMMUTATOR_OP]=Operator::OPER_COMMUTATOR;
		oper_types[ParsersAttributes::NEGATOR_OP]=Operator::OPER_NEGATOR;

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[ObjectType::ObjOperator])
					{
						xmlparser.getElementAttributes(attribs);
						oper_aux=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjOperator);

						//Raises an error if the auxiliary operator doesn't exists
						if(!oper_aux && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(oper->getSignature(true))
											.arg(oper->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjOperator)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setOperator(dynamic_cast<Operator *>(oper_aux),
										  oper_types[attribs[ParsersAttributes::REF_TYPE]]);
					}
					else if(elem==ParsersAttributes::TYPE)
					{
						xmlparser.getElementAttributes(attribs);

						if(attribs[ParsersAttributes::REF_TYPE]!=ParsersAttributes::RIGHT_TYPE)
							arg_type=Operator::LEFT_ARG;
						else
							arg_type=Operator::RIGHT_ARG;

						type=createPgSQLType();
						oper->setArgumentType(type, arg_type);
					}
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(oper->getName())
											.arg(oper->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setFunction(dynamic_cast<Function *>(func),
										  func_types[attribs[ParsersAttributes::REF_TYPE]]);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		if(oper) delete(oper);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(oper);
}

OperatorClass *DatabaseModel::createOperatorClass(void)
{
	attribs_map attribs, attribs_aux;
	map<QString, unsigned> elem_types;
	BaseObject *object=nullptr;
	QString elem;
	PgSQLType type;
	OperatorClass *op_class=nullptr;
	OperatorClassElement class_elem;
	unsigned stg_number, elem_type;

	try
	{
		op_class=new OperatorClass;
		setBasicAttributes(op_class);
		xmlparser.getElementAttributes(attribs);

		op_class->setIndexingType(IndexingType(attribs[ParsersAttributes::INDEX_TYPE]));
		op_class->setDefault(attribs[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

		elem_types[ParsersAttributes::FUNCTION]=OperatorClassElement::FunctionElem;
		elem_types[ParsersAttributes::OPERATOR]=OperatorClassElement::OperatorElem;
		elem_types[ParsersAttributes::STORAGE]=OperatorClassElement::StorageElem;

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[ObjectType::ObjOpFamily])
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjOpFamily);

						//Raises an error if the operator family doesn't exists
						if(!object)
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(op_class->getName())
											.arg(op_class->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjOpFamily)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						op_class->setFamily(dynamic_cast<OperatorFamily *>(object));
					}
					else if(elem==ParsersAttributes::TYPE)
					{
						xmlparser.getElementAttributes(attribs);
						type=createPgSQLType();
						op_class->setDataType(type);
					}
					else if(elem==ParsersAttributes::ELEMENT)
					{
						xmlparser.getElementAttributes(attribs);

						stg_number=attribs[ParsersAttributes::STRATEGY_NUM].toUInt();
						elem_type=elem_types[attribs[ParsersAttributes::TYPE]];

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
							object=getObject(attribs[ParsersAttributes::SIGNATURE],ObjectType::ObjFunction);
							class_elem.setFunction(dynamic_cast<Function *>(object),stg_number);
						}
						else if(elem_type==OperatorClassElement::OperatorElem)
						{
							object=getObject(attribs[ParsersAttributes::SIGNATURE],ObjectType::ObjOperator);
							class_elem.setOperator(dynamic_cast<Operator *>(object),stg_number);

							if(xmlparser.hasElement(XmlParser::NextElement))
							{
								xmlparser.savePosition();
								xmlparser.accessElement(XmlParser::NextElement);
								xmlparser.getElementAttributes(attribs_aux);

								object=getObject(attribs_aux[ParsersAttributes::SIGNATURE],ObjectType::ObjOpFamily);

								if(!object && !attribs_aux[ParsersAttributes::SIGNATURE].isEmpty())
									throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
													.arg(op_class->getName())
													.arg(op_class->getTypeName())
													.arg(attribs_aux[ParsersAttributes::SIGNATURE])
										.arg(BaseObject::getTypeName(ObjectType::ObjOpFamily)),
										RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		if(op_class) delete(op_class);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(op_class);
}

OperatorFamily *DatabaseModel::createOperatorFamily(void)
{
	attribs_map attribs;
	OperatorFamily *op_family=nullptr;

	try
	{
		op_family=new OperatorFamily;
		setBasicAttributes(op_family);
		xmlparser.getElementAttributes(attribs);
		op_family->setIndexingType(IndexingType(attribs[ParsersAttributes::INDEX_TYPE]));
	}
	catch(Exception &e)
	{
		if(op_family) delete(op_family);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(op_family);
}

Aggregate *DatabaseModel::createAggregate(void)
{
	attribs_map attribs;
	BaseObject *func=nullptr;
	QString elem;
	PgSQLType type;
	Aggregate *aggreg=nullptr;

	try
	{
		aggreg=new Aggregate;
		setBasicAttributes(aggreg);
		xmlparser.getElementAttributes(attribs);

		aggreg->setInitialCondition(attribs[ParsersAttributes::INITIAL_COND]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						xmlparser.getElementAttributes(attribs);
						type=createPgSQLType();

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::STATE_TYPE)
							aggreg->setStateType(type);
						else
							aggreg->addDataType(type);
					}
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(aggreg->getName())
											.arg(aggreg->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::TRANSITION_FUNC)
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
		if(aggreg) delete(aggreg);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(aggreg);
}

Table *DatabaseModel::createTable(void)
{
	attribs_map attribs, aux_attribs;
	QString elem;
	Table *table=nullptr;
	TableObject *object=nullptr;
	BaseObject *tag=nullptr;
	ObjectType obj_type;
	vector<unsigned> idxs;
	vector<QString> names;
	PartitionKey part_key;
	vector<PartitionKey> partition_keys;

	try
	{
		table=new Table;
		setBasicAttributes(table);
		xmlparser.getElementAttributes(attribs);

		table->setObjectListsCapacity(attribs[ParsersAttributes::MAX_OBJ_COUNT].toUInt());
		table->setWithOIDs(attribs[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);
		table->setUnlogged(attribs[ParsersAttributes::UNLOGGED]==ParsersAttributes::_TRUE_);
		table->setRLSEnabled(attribs[ParsersAttributes::RLS_ENABLED]==ParsersAttributes::_TRUE_);
		table->setRLSForced(attribs[ParsersAttributes::RLS_FORCED]==ParsersAttributes::_TRUE_);
		table->setGenerateAlterCmds(attribs[ParsersAttributes::GEN_ALTER_CMDS]==ParsersAttributes::_TRUE_);
		table->setExtAttribsHidden(attribs[ParsersAttributes::HIDE_EXT_ATTRIBS]==ParsersAttributes::_TRUE_);
		table->setFadedOut(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();
					xmlparser.savePosition();
					object=nullptr;

					if(elem==BaseObject::objs_schemas[ObjectType::ObjColumn])
						object=createColumn();
					else if(elem==BaseObject::objs_schemas[ObjectType::ObjConstraint])
						object=createConstraint(table);
					else if(elem==BaseObject::objs_schemas[ObjectType::ObjTag])
					{
						xmlparser.getElementAttributes(aux_attribs);
						tag=getObject(aux_attribs[ParsersAttributes::NAME] ,ObjectType::ObjTag);

						if(!tag)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjTable))
									.arg(aux_attribs[ParsersAttributes::TABLE])
									.arg(BaseObject::getTypeName(ObjectType::ObjTag))
									, RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						table->setTag(dynamic_cast<Tag *>(tag));
					}
					//Retrieving custom columns / constraint indexes
					else if(elem==ParsersAttributes::CUSTOMIDXS)
					{
						xmlparser.getElementAttributes(aux_attribs);
						obj_type=BaseObject::getObjectType(aux_attribs[ParsersAttributes::OBJECT_TYPE]);

						xmlparser.savePosition();

						if(xmlparser.accessElement(XmlParser::ChildElement))
						{
							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE)
								{
									elem=xmlparser.getElementName();

									//The element <object> stores the index for each object in the current group
									if(elem==ParsersAttributes::OBJECT)
									{
										xmlparser.getElementAttributes(aux_attribs);
										names.push_back(aux_attribs[ParsersAttributes::NAME]);
										idxs.push_back(aux_attribs[ParsersAttributes::INDEX].toUInt());
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
					else if(elem==ParsersAttributes::PARTITIONING)
					{
						xmlparser.getElementAttributes(aux_attribs);
						table->setPartitioningType(aux_attribs[ParsersAttributes::TYPE]);
						xmlparser.savePosition();

						if(xmlparser.accessElement(XmlParser::ChildElement))
						{
							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE &&
									 xmlparser.getElementName()==ParsersAttributes::PARTITION_KEY)
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
					else if(elem==ParsersAttributes::INITIAL_DATA)
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

		if(table) delete(table);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(table);
}

Column *DatabaseModel::createColumn(void)
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
		column->setNotNull(attribs[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);
		column->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);
		column->setIdSeqAttributes(attribs[ParsersAttributes::MIN_VALUE], attribs[ParsersAttributes::MAX_VALUE], attribs[ParsersAttributes::INCREMENT],
																attribs[ParsersAttributes::START], attribs[ParsersAttributes::CACHE], attribs[ParsersAttributes::CYCLE] == ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::IDENTITY_TYPE].isEmpty())
			column->setIdentityType(IdentityType(attribs[ParsersAttributes::IDENTITY_TYPE]));

		if(!attribs[ParsersAttributes::SEQUENCE].isEmpty())
		{
			seq=getObject(attribs[ParsersAttributes::SEQUENCE], ObjectType::ObjSequence);

			if(!seq)
				throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
								.arg(attribs[ParsersAttributes::NAME])
					.arg(BaseObject::getTypeName(ObjectType::ObjColumn))
					.arg(attribs[ParsersAttributes::SEQUENCE])
					.arg(BaseObject::getTypeName(ObjectType::ObjSequence)),
					PermissionRefInexistObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);


			column->setSequence(seq);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::TYPE)
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
		if(column) delete(column);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(column);
}

Constraint *DatabaseModel::createConstraint(BaseObject *parent_obj)
{
	attribs_map attribs;
	Constraint *constr=nullptr;
	BaseObject *ref_table=nullptr;
	Table *table=nullptr,*table_aux=nullptr;
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
			if(obj_type==ObjectType::ObjTable)
				table=dynamic_cast<Table *>(parent_obj);
			else if(obj_type==ObjectType::ObjRelationship)
				rel=dynamic_cast<Relationship *>(parent_obj);
			else
				//Raises an error if the user tries to create a constraint in a invalid parent
				throw Exception(OprObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		else
		{
			obj_type=ObjectType::ObjTable;
			table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));
			parent_obj=table;
			ins_constr_table=true;

			//Raises an error if the parent table doesn't exists
			if(!table)
			{
				str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
						.arg(attribs[ParsersAttributes::NAME])
						.arg(BaseObject::getTypeName(ObjectType::ObjConstraint))
						.arg(attribs[ParsersAttributes::TABLE])
						.arg(BaseObject::getTypeName(ObjectType::ObjTable));

				throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		constr=new Constraint;
		constr->setParentTable(table);

		//Configuring the constraint type
		if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CK_CONSTR)
			constr_type=ConstraintType::check;
		else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::PK_CONSTR)
			constr_type=ConstraintType::primary_key;
		else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FK_CONSTR)
			constr_type=ConstraintType::foreign_key;
		else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::UQ_CONSTR)
			constr_type=ConstraintType::unique;
		else
			constr_type=ConstraintType::exclude;

		constr->setConstraintType(constr_type);

		if(!attribs[ParsersAttributes::FACTOR].isEmpty())
			constr->setFillFactor(attribs[ParsersAttributes::FACTOR].toUInt());

		setBasicAttributes(constr);

		//Raises an error if the constraint is a primary key and no parent object is specified
		if(!parent_obj && constr_type==ConstraintType::primary_key)
			throw Exception(Exception::getErrorMessage(InvPrimaryKeyAllocation)
							.arg(constr->getName()),
							InvPrimaryKeyAllocation,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		deferrable=(attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
		constr->setDeferrable(deferrable);

		if(deferrable && !attribs[ParsersAttributes::DEFER_TYPE].isEmpty())
			constr->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

		if(constr_type==ConstraintType::foreign_key)
		{
			if(!attribs[ParsersAttributes::COMPARISON_TYPE].isEmpty())
				constr->setMatchType(attribs[ParsersAttributes::COMPARISON_TYPE]);

			if(!attribs[ParsersAttributes::DEL_ACTION].isEmpty())
				constr->setActionType(attribs[ParsersAttributes::DEL_ACTION], Constraint::DeleteAction);

			if(!attribs[ParsersAttributes::UPD_ACTION].isEmpty())
				constr->setActionType(attribs[ParsersAttributes::UPD_ACTION], Constraint::UpdateAction);

			ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], ObjectType::ObjTable);

			if(!ref_table && table->getName(true)==attribs[ParsersAttributes::REF_TABLE])
				ref_table=table;

			//Raises an error if the referenced table doesn't exists
			if(!ref_table)
			{
				str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
						.arg(constr->getName())
						.arg(constr->getTypeName())
						.arg(attribs[ParsersAttributes::REF_TABLE])
						.arg(BaseObject::getTypeName(ObjectType::ObjTable));

				throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			constr->setReferencedTable(dynamic_cast<BaseTable *>(ref_table));
		}
		else if(constr_type==ConstraintType::check)
		{
			constr->setNoInherit(attribs[ParsersAttributes::NO_INHERIT]==ParsersAttributes::_TRUE_);
		}
		else if(constr_type==ConstraintType::exclude &&	!attribs[ParsersAttributes::INDEX_TYPE].isEmpty())
		{
			constr->setIndexType(attribs[ParsersAttributes::INDEX_TYPE]);
		}

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::EXCLUDE_ELEMENT)
					{
						createElement(exc_elem, constr, parent_obj);
						constr->addExcludeElement(exc_elem);
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						constr->setExpression(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::COLUMNS)
					{
						xmlparser.getElementAttributes(attribs);

						col_list=attribs[ParsersAttributes::NAMES].split(',');
						count=col_list.count();

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::SRC_COLUMNS)
							col_type=Constraint::SourceCols;
						else
							col_type=Constraint::ReferencedCols;

						for(i=0; i < count; i++)
						{
							if(col_type==Constraint::SourceCols)
							{
								if(obj_type==ObjectType::ObjTable)
								{
									column=table->getColumn(col_list[i]);

									//If the column doesn't exists tries to get it searching by the old name
									if(!column)
										column=table->getColumn(col_list[i], true);
								}
								else
									column=dynamic_cast<Column *>(rel->getObject(col_list[i], ObjectType::ObjColumn));
							}
							else
							{
								table_aux=dynamic_cast<Table *>(ref_table);
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
			if(constr->getConstraintType()!=ConstraintType::primary_key)
			{
				table->addConstraint(constr);
				if(this->getObjectIndex(table) >= 0)
					table->setModified(true);
			}
		}
	}
	catch(Exception &e)
	{
		if(constr) delete(constr);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(constr);
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
	is_part_key = xml_elem == ParsersAttributes::PARTITION_KEY;

	if(xml_elem==ParsersAttributes::INDEX_ELEMENT || xml_elem==ParsersAttributes::EXCLUDE_ELEMENT || is_part_key)
	{
		xmlparser.getElementAttributes(attribs);

		elem.setColumn(nullptr);
		elem.setCollation(nullptr);
		elem.setOperator(nullptr);
		elem.setOperatorClass(nullptr);

		elem.setSortingAttribute(Element::AscOrder, attribs[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);
		elem.setSortingAttribute(Element::NullsFirst, attribs[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
		elem.setSortingEnabled(attribs[ParsersAttributes::USE_SORTING]!=ParsersAttributes::_FALSE_);

		xmlparser.savePosition();
		xmlparser.accessElement(XmlParser::ChildElement);

		do
		{
			xml_elem=xmlparser.getElementName();

			if(xmlparser.getElementType()==XML_ELEMENT_NODE)
			{
				if(xml_elem==ParsersAttributes::OP_CLASS)
				{
					xmlparser.getElementAttributes(attribs);
					op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjOpClass));

					//Raises an error if the operator class doesn't exists
					if(!op_class)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(tab_obj->getName())
											.arg(tab_obj->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
									.arg(BaseObject::getTypeName(ObjectType::ObjOpClass)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(PartKeyObjectInexistsModel)
											.arg(parent_obj->getName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
									.arg(BaseObject::getTypeName(ObjectType::ObjOpClass)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setOperatorClass(op_class);
				}
				//Checking if elem is a ExcludeElement to be able to assign an operator to it
				else if(xml_elem==ParsersAttributes::OPERATOR)
				{
					xmlparser.getElementAttributes(attribs);
					oper=dynamic_cast<Operator *>(getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjOperator));

					//Raises an error if the operator doesn't exists
					if(!oper)
					{
						throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
										.arg(tab_obj->getName())
										.arg(tab_obj->getTypeName())
										.arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(ObjectType::ObjOperator)),
								RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setOperator(oper);
				}
				else if(xml_elem==ParsersAttributes::COLLATION)
				{
					xmlparser.getElementAttributes(attribs);
					collation=dynamic_cast<Collation *>(getObject(attribs[ParsersAttributes::NAME], ObjectType::ObjCollation));

					//Raises an error if the operator class doesn't exists
					if(!collation)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
															.arg(tab_obj->getName())
															.arg(tab_obj->getTypeName())
															.arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(ObjectType::ObjCollation)),
															RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(PartKeyObjectInexistsModel)
															.arg(parent_obj->getName())
															.arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(ObjectType::ObjCollation)),
															RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setCollation(collation);
				}
				else if(xml_elem==ParsersAttributes::COLUMN)
				{
					xmlparser.getElementAttributes(attribs);

					if(parent_obj->getObjectType()==ObjectType::ObjTable)
					{
						column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[ParsersAttributes::NAME]);

						if(!column)
							column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[ParsersAttributes::NAME], true);
					}
					else
					{
						column=dynamic_cast<Column *>(dynamic_cast<Relationship *>(parent_obj)->getObject(attribs[ParsersAttributes::NAME], ObjectType::ObjColumn));
					}

					//Raises an error if the column doesn't exists
					if(!column)
					{
						if(!is_part_key)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(tab_obj->getName())
											.arg(tab_obj->getTypeName())
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjColumn)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
						{
							throw Exception(Exception::getErrorMessage(PartKeyObjectInexistsModel)
											.arg(parent_obj->getName())
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjColumn)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}

					elem.setColumn(column);
				}
				else if(xml_elem==ParsersAttributes::EXPRESSION)
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

XmlParser *DatabaseModel::getXMLParser(void)
{
	return(&xmlparser);
}

QString DatabaseModel::getAlterDefinition(BaseObject *object)
{
	DatabaseModel *db_aux=dynamic_cast<DatabaseModel *>(object);

	if(!db_aux)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QString alter_def;
		attribs_map aux_attribs;

		aux_attribs[ParsersAttributes::SIGNATURE] = this->getSignature();
		aux_attribs[ParsersAttributes::SQL_OBJECT] = this->getSQLName();

		if(this->conn_limit!=db_aux->conn_limit)
			aux_attribs[ParsersAttributes::CONN_LIMIT]=QString::number(db_aux->conn_limit);

		if(this->is_template != db_aux->is_template)
			aux_attribs[ParsersAttributes::IS_TEMPLATE] = (db_aux->is_template ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

		if(this->allow_conns != db_aux->allow_conns)
			aux_attribs[ParsersAttributes::ALLOW_CONNS] = (db_aux->allow_conns ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

		alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), aux_attribs, true, false);
		alter_def+=BaseObject::getAlterDefinition(object);

		return(alter_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Index *DatabaseModel::createIndex(void)
{
	attribs_map attribs;
	Index *index=nullptr;
	QString elem, str_aux;
	IndexElement idx_elem;
	BaseTable *table=nullptr;

	try
	{
		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjView));

		//Raises an error if the parent table doesn't exists
		if(!table)
		{
			str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
					.arg(attribs[ParsersAttributes::NAME])
					.arg(BaseObject::getTypeName(ObjectType::ObjIndex))
					.arg(attribs[ParsersAttributes::TABLE])
					.arg(BaseObject::getTypeName(ObjectType::ObjTable));

			throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		index=new Index;
		setBasicAttributes(index);
		index->setParentTable(table);
		index->setIndexAttribute(Index::Concurrent, attribs[ParsersAttributes::CONCURRENT]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::Unique, attribs[ParsersAttributes::UNIQUE]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::FastUpdate, attribs[ParsersAttributes::FAST_UPDATE]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::Buffering, attribs[ParsersAttributes::BUFFERING]==ParsersAttributes::_TRUE_);
		index->setIndexingType(attribs[ParsersAttributes::INDEX_TYPE]);
		index->setFillFactor(attribs[ParsersAttributes::FACTOR].toUInt());

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::INDEX_ELEMENT)
					{
						createElement(idx_elem, index, table);
						index->addIndexElement(idx_elem);
					}
					else if(elem==ParsersAttributes::PREDICATE)
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
		table->setModified(true);
	}
	catch(Exception &e)
	{
		if(index) delete(index);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(index);
}

Rule *DatabaseModel::createRule(void)
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

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjView));

		if(!table)
			throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
							.arg(attribs[ParsersAttributes::NAME])
				.arg(BaseObject::getTypeName(ObjectType::ObjRule))
				.arg(attribs[ParsersAttributes::TABLE])
				.arg(BaseObject::getTypeName(ObjectType::ObjTable)),
				RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		rule->setExecutionType(attribs[ParsersAttributes::EXEC_TYPE]);
		rule->setEventType(attribs[ParsersAttributes::EVENT_TYPE]);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::COMMANDS ||
							elem==ParsersAttributes::CONDITION)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();

						if(elem==ParsersAttributes::COMMANDS)
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
		table->setModified(true);
	}
	catch(Exception &e)
	{
		if(rule) delete(rule);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(rule);
}

Trigger *DatabaseModel::createTrigger(void)
{
	attribs_map attribs;
	Trigger *trigger=nullptr;
	QString elem, str_aux;
	QStringList list_aux;
	int count, i;
	BaseObject *ref_table=nullptr, *func=nullptr;
	Column *column=nullptr;
	BaseTable *table=nullptr;

	try
	{
		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjView));

		if(!table)
			throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
							.arg(attribs[ParsersAttributes::NAME])
				.arg(BaseObject::getTypeName(ObjectType::ObjTrigger))
				.arg(attribs[ParsersAttributes::TABLE])
				.arg(BaseObject::getTypeName(ObjectType::ObjTable)),
				RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		trigger=new Trigger;
		trigger->setParentTable(table);

		setBasicAttributes(trigger);

		trigger->setConstraint(attribs[ParsersAttributes::CONSTRAINT]==ParsersAttributes::_TRUE_);

		trigger->setEvent(EventType::on_insert,
						  (attribs[ParsersAttributes::INS_EVENT]==ParsersAttributes::_TRUE_));

		trigger->setEvent(EventType::on_delete,
						  (attribs[ParsersAttributes::DEL_EVENT]==ParsersAttributes::_TRUE_));

		trigger->setEvent(EventType::on_update,
						  (attribs[ParsersAttributes::UPD_EVENT]==ParsersAttributes::_TRUE_));

		trigger->setEvent(EventType::on_truncate,
						  (attribs[ParsersAttributes::TRUNC_EVENT]==ParsersAttributes::_TRUE_));

		trigger->setExecutePerRow(attribs[ParsersAttributes::PER_ROW]==ParsersAttributes::_TRUE_);

		trigger->setFiringType(FiringType(attribs[ParsersAttributes::FIRING_TYPE]));

		trigger->setTransitionTableName(Trigger::OldTableName, attribs[ParsersAttributes::OLD_TABLE_NAME]);
		trigger->setTransitionTableName(Trigger::NewTableName, attribs[ParsersAttributes::NEW_TABLE_NAME]);

		list_aux=attribs[ParsersAttributes::ARGUMENTS].split(',');
		count=list_aux.count();
		for(i=0; i < count; i++)
		{
			if(!list_aux[i].isEmpty())
				trigger->addArgument(list_aux[i]);
		}

		trigger->setDeferrable(attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);

		if(trigger->isDeferrable())
			trigger->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

		if(!attribs[ParsersAttributes::REF_TABLE].isEmpty())
		{
			ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], ObjectType::ObjTable);

			if(!ref_table)
				ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], ObjectType::ObjView);

			//Raises an error if the trigger is referencing a inexistent table
			if(!ref_table)
			{
				throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
								.arg(trigger->getName())
								.arg(trigger->getTypeName())
								.arg(attribs[ParsersAttributes::REF_TABLE])
						.arg(BaseObject::getTypeName(ObjectType::ObjTable)),
						RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
						{
							str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
									.arg(trigger->getName())
									.arg(trigger->getTypeName())
									.arg(attribs[ParsersAttributes::SIGNATURE])
									.arg(BaseObject::getTypeName(ObjectType::ObjFunction));

							throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						trigger->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==ParsersAttributes::CONDITION)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();
						trigger->setCondition(str_aux);
					}
					else if(elem==ParsersAttributes::COLUMNS)
					{
						xmlparser.getElementAttributes(attribs);

						list_aux=attribs[ParsersAttributes::NAMES].split(',');
						count=list_aux.count();

						for(i=0; i < count; i++)
						{
							column=dynamic_cast<Column *>(table->getObject(list_aux[i].trimmed(), ObjectType::ObjColumn));

							if(!column && dynamic_cast<Table *>(table))
								column=dynamic_cast<Table *>(table)->getColumn(list_aux[i].trimmed(), true);

							trigger->addColumn(column);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(trigger);
		table->setModified(true);
	}
	catch(Exception &e)
	{
		if(trigger) delete(trigger);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(trigger);
}

Policy *DatabaseModel::createPolicy(void)
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

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));

		if(!table)
			throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(attribs[ParsersAttributes::NAME])
											.arg(BaseObject::getTypeName(ObjectType::ObjPolicy))
											.arg(attribs[ParsersAttributes::TABLE])
											.arg(BaseObject::getTypeName(ObjectType::ObjTable)),
				RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		policy->setPermissive(attribs[ParsersAttributes::PERMISSIVE] == ParsersAttributes::_TRUE_);
		policy->setPolicyCommand(PolicyCmdType(attribs[ParsersAttributes::COMMAND]));

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::EXPRESSION)
					{
						xmlparser.getElementAttributes(attribs);
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						if(attribs[ParsersAttributes::TYPE] == ParsersAttributes::USING_EXP)
							policy->setUsingExpression(xmlparser.getElementContent());
						else if(attribs[ParsersAttributes::TYPE] == ParsersAttributes::CHECK_EXP)
							policy->setCheckExpression(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
					else if(xmlparser.getElementName()==ParsersAttributes::ROLES)
					{
						QStringList rol_names;
						Role *role = nullptr;

						xmlparser.getElementAttributes(attribs);

						rol_names = attribs[ParsersAttributes::NAMES].split(',');

						for(auto &name : rol_names)
						{
							role=dynamic_cast<Role *>(getObject(name.trimmed(), ObjectType::ObjRole));

							//Raises an error if the referenced role doesn't exists
							if(!role)
							{
								throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
																.arg(policy->getName())
																.arg(policy->getTypeName())
																.arg(name)
																.arg(BaseObject::getTypeName(ObjectType::ObjRole)),
																RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							policy->addRole(role);
						}
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		table->addObject(policy);
		table->setModified(true);
	}
	catch(Exception &e)
	{
		if(policy) delete(policy);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(policy);
}

EventTrigger *DatabaseModel::createEventTrigger(void)
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
		event_trig->setEvent(EventTriggerType(attribs[ParsersAttributes::EVENT]));

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], ObjectType::ObjFunction);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(event_trig->getName())
											.arg(event_trig->getTypeName())
											.arg(attribs[ParsersAttributes::SIGNATURE])
									.arg(BaseObject::getTypeName(ObjectType::ObjFunction)),
									RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						event_trig->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==ParsersAttributes::FILTER)
					{
						xmlparser.getElementAttributes(attribs);
						event_trig->setFilter(attribs[ParsersAttributes::VARIABLE], attribs[ParsersAttributes::VALUES].split(','));
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

	}
	catch(Exception &e)
	{
		if(event_trig) delete(event_trig);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(event_trig);
}

GenericSQL *DatabaseModel::createGenericSQL(void)
{
	GenericSQL *genericsql=nullptr;
	attribs_map attribs;

	try
	{
		genericsql=new GenericSQL;
		setBasicAttributes(genericsql);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			if(xmlparser.getElementType()==XML_ELEMENT_NODE && xmlparser.getElementName() == ParsersAttributes::DEFINITION)
			{
				xmlparser.accessElement(XmlParser::ChildElement);
				genericsql->setDefinition(xmlparser.getElementContent());
			}
		}
	}
	catch(Exception &e)
	{
		if(genericsql) delete(genericsql);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(genericsql);
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

		sequence->setValues(attribs[ParsersAttributes::MIN_VALUE],
				attribs[ParsersAttributes::MAX_VALUE],
				attribs[ParsersAttributes::INCREMENT],
				attribs[ParsersAttributes::START],
				attribs[ParsersAttributes::CACHE]);

		sequence->setCycle(attribs[ParsersAttributes::CYCLE]==ParsersAttributes::_TRUE_);

		//Getting the sequence's owner column
		if(!attribs[ParsersAttributes::OWNER_COLUMN].isEmpty())
		{
			elem_list=attribs[ParsersAttributes::OWNER_COLUMN].split('.');
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

			table=getObject(tab_name, ObjectType::ObjTable);

			//Raises an error if the column parent table doesn't exists
			if(!table)
			{
				str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
						.arg(sequence->getName())
						.arg(BaseObject::getTypeName(ObjectType::ObjSequence))
						.arg(tab_name)
						.arg(BaseObject::getTypeName(ObjectType::ObjTable));

				throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column=dynamic_cast<Table *>(table)->getColumn(col_name);

			if(!column)
				column=dynamic_cast<Table *>(table)->getColumn(col_name, true);

			//Raises an error if the column doesn't exists
			if(!column && !ignore_onwer)
				throw Exception(Exception::getErrorMessage(AsgInexistentSeqOwnerColumn)
								.arg(sequence->getName(true)),
								AsgInexistentSeqOwnerColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			sequence->setOwnerColumn(column);
		}
	}
	catch(Exception &e)
	{
		if(sequence) delete(sequence);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(sequence);
}

View *DatabaseModel::createView(void)
{
	attribs_map attribs, aux_attribs;
	View *view=nullptr;
	Column *column=nullptr;
	Table *table=nullptr;
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
		view->setObjectListsCapacity(attribs[ParsersAttributes::MAX_OBJ_COUNT].toUInt());
		view->setMaterialized(attribs[ParsersAttributes::MATERIALIZED]==ParsersAttributes::_TRUE_);
		view->setRecursive(attribs[ParsersAttributes::RECURSIVE]==ParsersAttributes::_TRUE_);
		view->setWithNoData(attribs[ParsersAttributes::WITH_NO_DATA]==ParsersAttributes::_TRUE_);
		view->setExtAttribsHidden(attribs[ParsersAttributes::HIDE_EXT_ATTRIBS]==ParsersAttributes::_TRUE_);
		view->setFadedOut(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::REFERENCE)
					{
						xmlparser.getElementAttributes(attribs);

						//If the table name is specified tries to create a reference to a table/column
						if(!attribs[ParsersAttributes::TABLE].isEmpty())
						{
							column=nullptr;
							table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], ObjectType::ObjTable));

							//Raises an error if the table doesn't exists
							if(!table)
							{
								str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
										.arg(view->getName())
										.arg(BaseObject::getTypeName(ObjectType::ObjView))
										.arg(attribs[ParsersAttributes::TABLE])
										.arg(BaseObject::getTypeName(ObjectType::ObjTable));

								throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							if(!attribs[ParsersAttributes::COLUMN].isEmpty())
							{
								column=table->getColumn(attribs[ParsersAttributes::COLUMN]);

								if(!column)
									column=table->getColumn(attribs[ParsersAttributes::COLUMN], true);

								//Raises an error if the view references an inexistant column
								if(!column)
								{
									str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(view->getName())
											.arg(BaseObject::getTypeName(ObjectType::ObjView))
											.arg(attribs[ParsersAttributes::TABLE] + QString(".") +
											attribs[ParsersAttributes::COLUMN])
											.arg(BaseObject::getTypeName(ObjectType::ObjColumn));

									throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
							}

							//Adds the configured reference to a temporarily list
							reference = Reference(table, column,
																		attribs[ParsersAttributes::ALIAS],
																		attribs[ParsersAttributes::COLUMN_ALIAS]);
							reference.setReferenceAlias(attribs[ParsersAttributes::REF_ALIAS]);
							refs.push_back(reference);
						}
						else
						{
							xmlparser.savePosition();
							str_aux=attribs[ParsersAttributes::ALIAS];

							xmlparser.accessElement(XmlParser::ChildElement);
							xmlparser.accessElement(XmlParser::ChildElement);

							reference = Reference(xmlparser.getElementContent(),str_aux);
							reference.setReferenceAlias(attribs[ParsersAttributes::REF_ALIAS]);
							refs.push_back(reference);

							xmlparser.restorePosition();
						}
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						xmlparser.accessElement(XmlParser::ChildElement);

						if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CTE_EXPRESSION)
							view->setCommomTableExpression(xmlparser.getElementContent());
						else
						{
							if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
								type=Reference::SqlReferSelect;
							else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
								type=Reference::SqlReferFrom;
							else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::SIMPLE_EXP)
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
					else if(elem==BaseObject::getSchemaName(ObjectType::ObjTag))
					{
						xmlparser.getElementAttributes(aux_attribs);
						tag=getObject(aux_attribs[ParsersAttributes::NAME] ,ObjectType::ObjTag);

						if(!tag)
						{
							throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
											.arg(attribs[ParsersAttributes::NAME])
									.arg(BaseObject::getTypeName(ObjectType::ObjTable))
									.arg(aux_attribs[ParsersAttributes::TABLE])
									.arg(BaseObject::getTypeName(ObjectType::ObjTag))
									, RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		if(view) delete(view);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(view);
}

Collation *DatabaseModel::createCollation(void)
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

		encoding=EncodingType(attribs[ParsersAttributes::ENCODING]);
		collation->setEncoding(encoding);

		//Creating a collation from a base locale
		if(!attribs[ParsersAttributes::LOCALE].isEmpty())
			collation->setLocale(attribs[ParsersAttributes::LOCALE]);
		//Creating a collation from another collation
		else if(!attribs[ParsersAttributes::COLLATION].isEmpty())
		{
			copy_coll=this->getObject(attribs[ParsersAttributes::COLLATION], ObjectType::ObjCollation);

			//Raises an error if the copy collation doesn't exists
			if(!copy_coll)
			{
				throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
								.arg(collation->getName())
								.arg(BaseObject::getTypeName(ObjectType::ObjCollation))
								.arg(attribs[ParsersAttributes::COLLATION])
						.arg(BaseObject::getTypeName(ObjectType::ObjCollation)),
						RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			collation->setCollation(dynamic_cast<Collation *>(copy_coll));
		}
		//Creating a collation using LC_COLLATE and LC_CTYPE params
		else
		{
			collation->setLocalization(Collation::LcCollate, attribs[ParsersAttributes::_LC_COLLATE_]);
			collation->setLocalization(Collation::LcCtype, attribs[ParsersAttributes::_LC_CTYPE_]);
		}
	}
	catch(Exception &e)
	{
		if(collation) delete(collation);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(collation);
}

Extension *DatabaseModel::createExtension(void)
{
	Extension *extension=nullptr;
	attribs_map attribs;

	try
	{
		extension=new Extension;
		xmlparser.getElementAttributes(attribs);
		setBasicAttributes(extension);

		extension->setHandlesType(attribs[ParsersAttributes::HANDLES_TYPE]==ParsersAttributes::_TRUE_);
		extension->setVersion(Extension::CurVersion, attribs[ParsersAttributes::CUR_VERSION]);
		extension->setVersion(Extension::OldVersion, attribs[ParsersAttributes::OLD_VERSION]);
	}
	catch(Exception &e)
	{
		if(extension) delete(extension);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(extension);
}

Tag *DatabaseModel::createTag(void)
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

					if(elem==ParsersAttributes::STYLE)
					{
						xmlparser.getElementAttributes(attribs);
						tag->setElementColors(attribs[ParsersAttributes::ID],attribs[ParsersAttributes::COLORS]);
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		return(tag);
	}
	catch(Exception &e)
	{
		if(tag) delete(tag);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}
}

Textbox *DatabaseModel::createTextbox(void)
{
	Textbox *txtbox=nullptr;
	attribs_map attribs;

	try
	{
		txtbox=new Textbox;
		setBasicAttributes(txtbox);

		xmlparser.getElementAttributes(attribs);

		txtbox->setFadedOut(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);
		txtbox->setTextAttribute(Textbox::ItalicText, attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_);
		txtbox->setTextAttribute(Textbox::BoldText, attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_);
		txtbox->setTextAttribute(Textbox::UnderlineText, attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::COLOR].isEmpty())
			txtbox->setTextColor(QColor(attribs[ParsersAttributes::COLOR]));

		if(!attribs[ParsersAttributes::FONT_SIZE].isEmpty())
			txtbox->setFontSize(attribs[ParsersAttributes::FONT_SIZE].toDouble());
	}
	catch(Exception &e)
	{
		if(txtbox) delete(txtbox);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(txtbox);
}

BaseRelationship *DatabaseModel::createRelationship(void)
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
	unsigned rel_type=0, i;
	ObjectType table_types[2]={ObjectType::ObjView, ObjectType::ObjTable}, obj_rel_type;
	QString str_aux, elem,
			tab_attribs[2]={ ParsersAttributes::SRC_TABLE,
							 ParsersAttributes::DST_TABLE };
	QColor custom_color=Qt::transparent;
	Table *table = nullptr;

	try
	{
		labels_id[ParsersAttributes::NAME_LABEL]=BaseRelationship::RelNameLabel;
		labels_id[ParsersAttributes::SRC_LABEL]=BaseRelationship::SrcCardLabel;
		labels_id[ParsersAttributes::DST_LABEL]=BaseRelationship::DstCardLabel;

		xmlparser.getElementAttributes(attribs);
		protect=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);
		faded_out=(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::CUSTOM_COLOR].isEmpty())
			custom_color=QColor(attribs[ParsersAttributes::CUSTOM_COLOR]);

		if(attribs[ParsersAttributes::TYPE]!=ParsersAttributes::RELATION_TAB_VIEW &&
				attribs[ParsersAttributes::TYPE]!=ParsersAttributes::RELATIONSHIP_FK)
		{
			table_types[0]=ObjectType::ObjTable;
			obj_rel_type=ObjectType::ObjRelationship;
		}
		else
		{
			if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
				table_types[0]=ObjectType::ObjTable;

			obj_rel_type=ObjectType::ObjBaseRelationship;
		}

		//Gets the participant tables
		for(i=0; i < 2; i++)
		{
			tables[i]=dynamic_cast<BaseTable *>(getObject(attribs[tab_attribs[i]], table_types[i]));

			//Raises an error if some table doesn't exists
			if(!tables[i])
			{
				str_aux=Exception::getErrorMessage(RefObjectInexistsModel)
						.arg(attribs[ParsersAttributes::NAME])
						.arg(BaseObject::getTypeName(obj_rel_type))
						.arg(attribs[tab_attribs[i]])
						.arg(BaseObject::getTypeName(table_types[i]));

				throw Exception(str_aux,RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		if(obj_rel_type==ObjectType::ObjBaseRelationship)
		{
			base_rel=getRelationship(tables[0], tables[1]);

			/* Creates the fk relationship if it not exists. This generally happens when a foreign key is
			added to the table after its creation. */
			if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
			{
				base_rel=new BaseRelationship(BaseRelationship::RelationshipFk, tables[0], tables[1], false, false);
				base_rel->setName(attribs[ParsersAttributes::NAME]);
				base_rel->setAlias(attribs[ParsersAttributes::ALIAS]);
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
						throw Exception(Exception::getErrorMessage(InvAllocationFKRelationship)
									  .arg(attribs[ParsersAttributes::NAME])
									  .arg(src_tab->getName(true)),
										InvAllocationFKRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			else if(base_rel)
				base_rel->setName(attribs[ParsersAttributes::NAME]);

			if(!base_rel)
				throw Exception(Exception::getErrorMessage(RefObjectInexistsModel)
								.arg(this->getName())
								.arg(this->getTypeName())
								.arg(attribs[ParsersAttributes::NAME])
					.arg(BaseObject::getTypeName(ObjectType::ObjBaseRelationship)),
					RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			base_rel->blockSignals(loading_model);
			base_rel->disconnectRelationship();
			base_rel->blockSignals(false);
		}
		else
		{
			QString pat_attrib[]= { ParsersAttributes::SRC_COL_PATTERN, ParsersAttributes::DST_COL_PATTERN,
									ParsersAttributes::SRC_FK_PATTERN, ParsersAttributes::DST_FK_PATTERN,
									ParsersAttributes::PK_PATTERN, ParsersAttributes::UQ_PATTERN,
									ParsersAttributes::PK_COL_PATTERN };

			unsigned 	pattern_id[]= { Relationship::SrcColPattern, Relationship::DstColPattern,
										Relationship::SrcFkPattern, Relationship::DstFkPattern,
										Relationship::PkPattern, Relationship::UqPattern,
										Relationship::PkColPattern },
					pat_count=sizeof(pattern_id)/sizeof(unsigned);

			sql_disabled=attribs[ParsersAttributes::SQL_DISABLED]==ParsersAttributes::_TRUE_;
			src_mand=attribs[ParsersAttributes::SRC_REQUIRED]==ParsersAttributes::_TRUE_;
			dst_mand=attribs[ParsersAttributes::DST_REQUIRED]==ParsersAttributes::_TRUE_;
			identifier=attribs[ParsersAttributes::IDENTIFIER]==ParsersAttributes::_TRUE_;
			deferrable=attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_;
			defer_type=DeferralType(attribs[ParsersAttributes::DEFER_TYPE]);
			del_action=ActionType(attribs[ParsersAttributes::DEL_ACTION]);
			upd_action=ActionType(attribs[ParsersAttributes::UPD_ACTION]);
			single_pk_col=(attribs[ParsersAttributes::SINGLE_PK_COLUMN]==ParsersAttributes::_TRUE_);

			if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_11)
				rel_type=BaseRelationship::Relationship11;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_1N)
				rel_type=BaseRelationship::Relationship1n;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_NN)
				rel_type=BaseRelationship::RelationshipNn;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_GEN)
				rel_type=BaseRelationship::RelationshipGen;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_DEP)
				rel_type=BaseRelationship::RelationshipDep;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_PART)
				rel_type=BaseRelationship::RelationshipPart;

			rel=new Relationship(rel_type,
								 dynamic_cast<Table *>(tables[0]),
					dynamic_cast<Table *>(tables[1]),
					src_mand, dst_mand,
					identifier, deferrable, defer_type, del_action, upd_action,
					CopyOptions(attribs[ParsersAttributes::COPY_MODE].toUInt(),
					attribs[ParsersAttributes::COPY_OPTIONS].toUInt()));

			rel->setSQLDisabled(sql_disabled);
			rel->setSiglePKColumn(single_pk_col);

			if(!attribs[ParsersAttributes::TABLE_NAME].isEmpty())
				rel->setTableNameRelNN(attribs[ParsersAttributes::TABLE_NAME]);

			rel->setName(attribs[ParsersAttributes::NAME]);
			rel->setAlias(attribs[ParsersAttributes::ALIAS]);
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

					if(elem == ParsersAttributes::EXPRESSION && rel)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						rel->setPartitionBoundingExpr(xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::COLUMN && rel)
					{
						xmlparser.savePosition();
						rel->addObject(createColumn());
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::CONSTRAINT && rel)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(constr_attribs);

						/* If we find a primary key constraint at this point means that we're handling the original primary key stored by the relationship.
						 * Since relationships can't have primary keys created manually by the users we assume that
						 * the relationship contains a special primary key (created during relationship connection)
						 * and the current constraint is the original one owned by one of the tables prior the connection
						 * of the relationship. */
						if(constr_attribs[ParsersAttributes::TYPE] == ParsersAttributes::PK_CONSTR)
						{
							table = getTable(constr_attribs[ParsersAttributes::TABLE]);
							rel->setOriginalPrimaryKey(createConstraint(table));
						}
						else
							rel->addObject(createConstraint(rel));

						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::LINE)
					{
						vector<QPointF> points;
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						do
						{
							xmlparser.getElementAttributes(attribs);
							points.push_back(QPointF(attribs[ParsersAttributes::X_POS].toDouble(),
											 attribs[ParsersAttributes::Y_POS].toDouble()));
						}
						while(xmlparser.accessElement(XmlParser::NextElement));

						base_rel->setPoints(points);
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::LABEL)
					{
						xmlparser.getElementAttributes(attribs);
						str_aux=attribs[ParsersAttributes::REF_TYPE];

						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);
						xmlparser.getElementAttributes(attribs);
						xmlparser.restorePosition();

						base_rel->setLabelDistance(labels_id[str_aux],
												   QPointF(attribs[ParsersAttributes::X_POS].toFloat(),
												   attribs[ParsersAttributes::Y_POS].toFloat()));
					}
					else if(elem==ParsersAttributes::SPECIAL_PK_COLS && rel)
					{
						QList<QString> col_list;

						xmlparser.getElementAttributes(attribs);
						col_list=attribs[ParsersAttributes::INDEXES].split(',');

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
		if(base_rel && base_rel->getObjectType()==ObjectType::ObjRelationship)
			delete(base_rel);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	if(rel)
	{
		storeSpecialObjectsXML();
		addRelationship(rel);
	}

	base_rel->setFadedOut(faded_out);
	base_rel->setProtected(protect);
	base_rel->setCustomColor(custom_color);

	/* If the FK relationship does not reference a foreign key (models generated in older versions)
	 * we need to assign them to the respective relationships */
	if(base_rel && base_rel->getObjectType()==ObjectType::ObjBaseRelationship)
	{
		base_rel->blockSignals(loading_model);
		base_rel->connectRelationship();
		base_rel->blockSignals(false);
	}

	return(base_rel);
}

Permission *DatabaseModel::createPermission(void)
{
	Permission *perm=nullptr;
	BaseObject *object=nullptr;
	Table *parent_table=nullptr;
	Role *role=nullptr;
	attribs_map priv_attribs, attribs;
	attribs_map::iterator itr, itr_end;
	ObjectType obj_type;
	QString parent_name, obj_name;
	QStringList list;
	unsigned i, len, priv_type=Permission::PrivSelect;
	bool priv_value, grant_op, revoke, cascade;

	try
	{
		xmlparser.getElementAttributes(priv_attribs);
		revoke=priv_attribs[ParsersAttributes::REVOKE]==ParsersAttributes::_TRUE_;
		cascade=priv_attribs[ParsersAttributes::CASCADE]==ParsersAttributes::_TRUE_;

		xmlparser.savePosition();
		xmlparser.accessElement(XmlParser::ChildElement);
		xmlparser.getElementAttributes(attribs);

		obj_type=BaseObject::getObjectType(attribs[ParsersAttributes::TYPE]);
		obj_name=attribs[ParsersAttributes::NAME];
		parent_name=attribs[ParsersAttributes::PARENT];

		//If the object is a column its needed to get the parent table
		if(obj_type==ObjectType::ObjColumn)
		{
			parent_table=dynamic_cast<Table *>(getObject(parent_name, ObjectType::ObjTable));

			if(parent_table)
				object=parent_table->getColumn(obj_name);
		}
		else if(obj_type==ObjectType::ObjDatabase)
		{
			object=this;
		}
		else
			object=getObject(obj_name, obj_type);

		//Raises an error if the permission references an object that does not exists
		if(!object)
			throw Exception(Exception::getErrorMessage(PermissionRefInexistObject)
							.arg(obj_name)
							.arg(BaseObject::getTypeName(obj_type)),
							PermissionRefInexistObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		perm=new Permission(object);
		perm->setRevoke(revoke);
		perm->setCascade(cascade);

		do
		{
			if(xmlparser.getElementName()==ParsersAttributes::ROLES)
			{
				xmlparser.getElementAttributes(attribs);

				list=attribs[ParsersAttributes::NAMES].split(',');
				len=list.size();

				for(i=0; i < len; i++)
				{
					role=dynamic_cast<Role *>(getObject(list[i].trimmed(),ObjectType::ObjRole));

					//Raises an error if the referenced role doesn't exists
					if(!role)
					{
						throw Exception(Exception::getErrorMessage(PermissionRefInexistObject)
										.arg(list[i])
										.arg(BaseObject::getTypeName(ObjectType::ObjRole)),
										RefObjectInexistsModel,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					}

					perm->addRole(role);
				}
			}
			else if(xmlparser.getElementName()==ParsersAttributes::PRIVILEGES)
			{
				xmlparser.getElementAttributes(priv_attribs);

				itr=priv_attribs.begin();
				itr_end=priv_attribs.end();

				while(itr!=itr_end)
				{
					if(itr->first!=ParsersAttributes::GRANT_OP)
					{
						priv_value=(itr->second==ParsersAttributes::_TRUE_);
						grant_op=(itr->second==ParsersAttributes::GRANT_OP);

						if(itr->first==ParsersAttributes::CONNECT_PRIV)
							priv_type=Permission::PrivConnect;
						else if(itr->first==ParsersAttributes::CREATE_PRIV)
							priv_type=Permission::PrivCreate;
						else if(itr->first==ParsersAttributes::DELETE_PRIV)
							priv_type=Permission::PrivDelete;
						else if(itr->first==ParsersAttributes::EXECUTE_PRIV)
							priv_type=Permission::PrivExecute;
						else if(itr->first==ParsersAttributes::INSERT_PRIV)
							priv_type=Permission::PrivInsert;
						else if(itr->first==ParsersAttributes::REFERENCES_PRIV)
							priv_type=Permission::PrivReferences;
						else if(itr->first==ParsersAttributes::SELECT_PRIV)
							priv_type=Permission::PrivSelect;
						else if(itr->first==ParsersAttributes::TEMPORARY_PRIV)
							priv_type=Permission::PrivTemporary;
						else if(itr->first==ParsersAttributes::TRIGGER_PRIV)
							priv_type=Permission::PrivTrigger;
						else if(itr->first==ParsersAttributes::TRUNCATE_PRIV)
							priv_type=Permission::PrivTruncate;
						else if(itr->first==ParsersAttributes::UPDATE_PRIV)
							priv_type=Permission::PrivUpdate;
						else if(itr->first==ParsersAttributes::USAGE_PRIV)
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
		if(perm) delete(perm);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(perm);
}

void DatabaseModel::validateColumnRemoval(Column *column)
{
	if(column && column->getParentTable())
	{
		vector<BaseObject *> refs;
		getObjectReferences(column, refs);

		//Raises an error if there are objects referencing the column
		if(!refs.empty())
			throw Exception(Exception::getErrorMessage(RemDirectReference)
							.arg(column->getParentTable()->getName(true) + QString(".") + column->getName(true))
							.arg(column->getTypeName())
							.arg(refs[0]->getName(true))
				.arg(refs[0]->getTypeName()),
				RemDirectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			revalidate_rels=((obj_type==ObjectType::ObjColumn &&
												(parent_tab->isConstraintRefColumn(dynamic_cast<Column *>(object), ConstraintType::primary_key) ||
												 parent_tab->isPartition() || parent_tab->isPartitioned())) ||
											 (obj_type==ObjectType::ObjConstraint &&
												dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::primary_key));

			/* Additional validation for columns: checks if the parent table participates on a
			generalization/copy as destination table */
			if(obj_type==ObjectType::ObjColumn)
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseModel::__getCodeDefinition(unsigned def_type)
{
	QString def, bkp_appended_sql, bkp_prepended_sql;

	//Forcing the name/signature cleanup due to the validation temp. names feature
	attributes[ParsersAttributes::NAME]=QString();
	attributes[ParsersAttributes::SIGNATURE]=QString();

	if(conn_limit >= 0)
		attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(conn_limit);

	if(def_type==SchemaParser::SqlDefinition)
	{
		QString loc_attribs[]={ ParsersAttributes::_LC_CTYPE_,  ParsersAttributes::_LC_COLLATE_ };

		if(encoding!=BaseType::null)
			attributes[ParsersAttributes::ENCODING]=QString("'%1'").arg(~encoding);

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
		attributes[ParsersAttributes::ENCODING]=(~encoding);
		attributes[ParsersAttributes::_LC_COLLATE_]=localizations[1];
		attributes[ParsersAttributes::_LC_CTYPE_]=localizations[0];
		attributes[ParsersAttributes::APPEND_AT_EOD]=(append_at_eod ? ParsersAttributes::_TRUE_ : QString());
		attributes[ParsersAttributes::PREPEND_AT_BOD]=(prepend_at_bod ? ParsersAttributes::_TRUE_ : QString());
	}

	attributes[ParsersAttributes::IS_TEMPLATE]=(is_template ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
	attributes[ParsersAttributes::ALLOW_CONNS]=(allow_conns ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
	attributes[ParsersAttributes::TEMPLATE_DB]=template_db;

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

		return(def);
	}
	catch(Exception &e)
	{
		this->appended_sql=bkp_appended_sql;
		this->prepended_sql=bkp_prepended_sql;
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseModel::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, true));
}

QString DatabaseModel::getCodeDefinition(unsigned def_type, bool export_file)
{
	attribs_map attribs_aux;
	unsigned general_obj_cnt, gen_defs_count;
	bool sql_disabled=false;
	BaseObject *object=nullptr;
	QString def, search_path=QString("pg_catalog,public"),
			msg=trUtf8("Generating %1 code: `%2' (%3)"),
			attrib=ParsersAttributes::OBJECTS, attrib_aux,
			def_type_str=(def_type==SchemaParser::SqlDefinition ? QString("SQL") : QString("XML"));
	Type *usr_type=nullptr;
	map<unsigned, BaseObject *> objects_map;
	ObjectType obj_type;

	try
	{
		objects_map=getCreationOrder(def_type);
		general_obj_cnt=objects_map.size();
		gen_defs_count=0;

		attribs_aux[ParsersAttributes::SHELL_TYPES]=QString();
		attribs_aux[ParsersAttributes::PERMISSION]=QString();
		attribs_aux[ParsersAttributes::SCHEMA]=QString();
		attribs_aux[ParsersAttributes::TABLESPACE]=QString();
		attribs_aux[ParsersAttributes::ROLE]=QString();

		if(def_type==SchemaParser::SqlDefinition)
		{
			attribs_aux[ParsersAttributes::FUNCTION]=(!functions.empty() ? ParsersAttributes::_TRUE_ : QString());

			for(auto &type : types)
			{
				usr_type=dynamic_cast<Type *>(type);

				if(usr_type->getConfiguration()==Type::BaseType)
					usr_type->convertFunctionParameters();
			}
		}

		for(auto &obj_itr : objects_map)
		{
			object=obj_itr.second;
			obj_type=object->getObjectType();

			if(obj_type==ObjectType::ObjType && def_type==SchemaParser::SqlDefinition)
			{
				usr_type=dynamic_cast<Type *>(object);

				//Generating the shell type declaration (only for base types)
				if(usr_type->getConfiguration()==Type::BaseType)
					attribs_aux[ParsersAttributes::SHELL_TYPES]+=usr_type->getCodeDefinition(def_type, true);
				else
					attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
			}
			else if(obj_type==ObjectType::ObjDatabase)
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
			else if(obj_type==ObjectType::ObjPermission)
			{
				attribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permission *>(object)->getCodeDefinition(def_type);
			}
			else if(obj_type==ObjectType::ObjConstraint)
			{
				attribs_aux[attrib]+=dynamic_cast<Constraint *>(object)->getCodeDefinition(def_type, true);
			}
			else if(obj_type==ObjectType::ObjRole || obj_type==ObjectType::ObjTablespace ||  obj_type==ObjectType::ObjSchema)
			{
				//The "public" schema does not have the SQL code definition generated
				if(def_type==SchemaParser::SqlDefinition)
					attrib_aux=BaseObject::getSchemaName(obj_type);
				else
					attrib_aux=attrib;

				/* The Tablespace has the SQL code definition disabled when generating the
		  code of the entire model because this object cannot be created from a multiline sql command */
				if(obj_type==ObjectType::ObjTablespace && !object->isSystemObject() && def_type==SchemaParser::SqlDefinition)
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
				else if((obj_type!=ObjectType::ObjSchema && !object->isSystemObject()) ||
						(obj_type==ObjectType::ObjSchema &&
						 ((object->getName()==QString("public") && def_type==SchemaParser::XmlDefinition) ||
						  (object->getName()!=QString("public") && object->getName()!=QString("pg_catalog")))))
				{
					if(object->getObjectType()==ObjectType::ObjSchema)
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
									object->getObjectType());
			}
		}

		attribs_aux[ParsersAttributes::SEARCH_PATH]=search_path;
		attribs_aux[ParsersAttributes::MODEL_AUTHOR]=author;
		attribs_aux[ParsersAttributes::PGMODELER_VERSION]=GlobalAttributes::PgModelerVersion;

		if(def_type==SchemaParser::XmlDefinition)
		{
			attribs_aux[ParsersAttributes::MAX_OBJ_COUNT]=QString::number(static_cast<unsigned>(getMaxObjectCount() * 1.20));
			attribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? ParsersAttributes::_TRUE_ : QString());
			attribs_aux[ParsersAttributes::LAST_POSITION]=QString("%1,%2").arg(last_pos.x()).arg(last_pos.y());
			attribs_aux[ParsersAttributes::LAST_ZOOM]=QString::number(last_zoom);
			attribs_aux[ParsersAttributes::DEFAULT_SCHEMA]=(default_objs[ObjectType::ObjSchema] ? default_objs[ObjectType::ObjSchema]->getName(true) : QString());
			attribs_aux[ParsersAttributes::DEFAULT_OWNER]=(default_objs[ObjectType::ObjRole] ? default_objs[ObjectType::ObjRole]->getName(true) : QString());
			attribs_aux[ParsersAttributes::DEFAULT_TABLESPACE]=(default_objs[ObjectType::ObjTablespace] ? default_objs[ObjectType::ObjTablespace]->getName(true) : QString());
			attribs_aux[ParsersAttributes::DEFAULT_COLLATION]=(default_objs[ObjectType::ObjCollation] ? default_objs[ObjectType::ObjCollation]->getName(true) : QString());
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	attribs_aux[ParsersAttributes::EXPORT_TO_FILE]=(export_file ? ParsersAttributes::_TRUE_ : QString());
	def=schparser.getCodeDefinition(ParsersAttributes::DB_MODEL, attribs_aux, def_type);

	if(prepend_at_bod && def_type==SchemaParser::SqlDefinition)
		def=QString("-- Prepended SQL commands --\n") +	this->prepended_sql + QString("\n---\n\n") + def;

	if(append_at_eod && def_type==SchemaParser::SqlDefinition)
		def+=QString("-- Appended SQL commands --\n") +	this->appended_sql + QString("\n---\n");

	return(def);
}

map<unsigned, BaseObject *> DatabaseModel::getCreationOrder(unsigned def_type, bool incl_relnn_objs, bool incl_rel1n_constrs)
{
	BaseObject *object=nullptr;
	vector<BaseObject *> fkeys;
	vector<BaseObject *> fk_rels;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	map<unsigned, BaseObject *> objects_map;
	Table *table=nullptr;
	Constraint *constr=nullptr;
	View *view=nullptr;
	Relationship *rel=nullptr;
	ObjectType aux_obj_types[]={ ObjectType::ObjRole, ObjectType::ObjTablespace, ObjectType::ObjSchema, ObjectType::ObjTag },
			obj_types[]={ ObjectType::ObjEventTrigger, ObjectType::ObjCollation, ObjectType::ObjLanguage, ObjectType::ObjFunction, ObjectType::ObjType,
						  ObjectType::ObjCast, ObjectType::ObjConversion, ObjectType::ObjExtension,
						  ObjectType::ObjOperator, ObjectType::ObjOpFamily, ObjectType::ObjOpClass,
						  ObjectType::ObjAggregate, ObjectType::ObjDomain, ObjectType::ObjTextbox, ObjectType::ObjBaseRelationship,
							ObjectType::ObjRelationship, ObjectType::ObjTable, ObjectType::ObjView, ObjectType::ObjSequence, ObjectType::ObjGenericSQL };
	unsigned i=0, aux_obj_cnt=sizeof(aux_obj_types)/sizeof(ObjectType), count=sizeof(obj_types)/sizeof(ObjectType);

	//The first objects on the map will be roles, tablespaces, schemas and tags
	for(i=0; i < aux_obj_cnt; i++)
	{
		if(aux_obj_types[i]!=ObjectType::ObjTag || def_type==SchemaParser::XmlDefinition)
		{
			obj_list=getObjectList(aux_obj_types[i]);

			for(auto &object : (*obj_list))
				objects_map[object->getObjectId()]=object;
		}
	}

	//Includes the database model on the objects map permitting to create the code in a correct order
	objects_map[this->getObjectId()]=this;

	for(i=0; i < count; i++)
	{
		//For SQL definition, only the textbox and base relationship does not enters to the code generation list
		if(def_type==SchemaParser::SqlDefinition &&
				(obj_types[i]==ObjectType::ObjTextbox || obj_types[i]==ObjectType::ObjBaseRelationship))
			obj_list=nullptr;
		else
			obj_list=getObjectList(obj_types[i]);

		if(obj_list)
		{
			for(auto &object : (*obj_list))
			{
				/* If the object is a FK relationship it's stored in a separeted list in order to have the
			 code generated at end of whole definition (after foreign keys definition) */
				if(object->getObjectType()==ObjectType::ObjBaseRelationship &&
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
	for(auto &obj : tables)
	{
		table=dynamic_cast<Table *>(obj);
		count=table->getConstraintCount();
		for(i=0; i < count; i++)
		{
			constr=table->getConstraint(i);

			/* Case the constraint is a special object stores it on the objects map. Independently to the
		configuration, foreign keys are discarded in this iteration because on the end of the method
		they have the definition generated */
			if(constr->getConstraintType()!=ConstraintType::foreign_key &&  !constr->isAddedByLinking() &&
					((constr->getConstraintType()!=ConstraintType::primary_key && constr->isReferRelationshipAddedColumn())))
				objects_map[constr->getObjectId()]=constr;
			else if(constr->getConstraintType()==ConstraintType::foreign_key && !constr->isAddedByLinking())
				fkeys.push_back(constr);
		}

		for(auto obj : table->getObjects(true))
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
		vet_aux.insert(vet_aux.end(), sequences.begin(),sequences.end());
		vet_aux.insert(vet_aux.end(), views.begin(),views.end());;
		itr=vet_aux.begin();
		itr_end=vet_aux.end();

		while(itr!=itr_end)
		{
			object=(*itr);
			itr++;

			if(object->getObjectType()==ObjectType::ObjRelationship)
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

					for(BaseObject *tab_obj : *table->getObjectList(ObjectType::ObjConstraint))
					{
						constr=dynamic_cast<Constraint *>(tab_obj);

						if(constr->getConstraintType()==ConstraintType::foreign_key)
							fkeys.push_back(constr);
					}
				}
				else if(incl_rel1n_constrs)
				{
					vector<Constraint *> constrs=rel->getGeneratedConstraints();

					for(auto &constr : constrs)
					{
						if(constr->getConstraintType()!=ConstraintType::primary_key)
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

	return(objects_map);
}


void DatabaseModel::__getObjectDependencies(BaseObject *object, vector<BaseObject *> &objs)//, bool inc_children)
{
	vector<BaseObject *> dep_objs, chld_objs;
	Table *table=dynamic_cast<Table *>(object);
	ObjectType obj_type=ObjectType::ObjBaseObject;

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
	if(obj_type==ObjectType::ObjTable || obj_type==ObjectType::ObjView || obj_type==ObjectType::ObjSchema)
	{
		vector<BaseObject *>::iterator end;

		if(obj_type==ObjectType::ObjSchema)
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
				if((!constr && child->getObjectType()!=ObjectType::ObjColumn) ||
						(constr &&
						 ((constr->getConstraintType()==ConstraintType::foreign_key) ||
						  (constr->getConstraintType()!=ConstraintType::foreign_key &&
						   constr->getConstraintType()!=ConstraintType::primary_key &&
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
		return(vector<BaseObject *>());

	map<unsigned, BaseObject *> objs_map;
	vector<BaseObject *> objs, children;
	vector<Permission *> perms_aux, perms;
	vector<Role *> roles;
	Table *table=nullptr;
	Relationship *rel=nullptr;
	ObjectType obj_type=object->getObjectType();

	if(only_children)
		objs.push_back(object);
	else
		__getObjectDependencies(object, objs);

	/* Include tables generated by many-to-many relationships if their schemas are the same
	 as the 'object' when this one is a schema too */
	if(obj_type==ObjectType::ObjSchema)
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

					if((!constr && child->getObjectType()!=ObjectType::ObjColumn) ||
							(constr &&
							 ((constr->getConstraintType()==ConstraintType::foreign_key) ||
							  (constr->getConstraintType()!=ConstraintType::foreign_key &&
							   constr->getConstraintType()!=ConstraintType::primary_key &&
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

			table=dynamic_cast<Table *>(obj);

			if(table)
			{
				for(BaseObject *col : *table->getObjectList(ObjectType::ObjColumn))
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

	return(objs);
}

void DatabaseModel::saveModel(const QString &filename, unsigned def_type)
{
	QFile output(filename);
	QByteArray buf;

	output.open(QFile::WriteOnly);

	if(!output.isOpen())
		throw Exception(Exception::getErrorMessage(FileDirectoryNotWritten).arg(filename),
						FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		buf.append(this->getCodeDefinition(def_type));
		output.write(buf.data(),buf.size());
		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(FileNotWrittenInvalidDefinition).arg(filename),
						FileNotWrittenInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
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

			/* if the object has a schema, tablespace and owner applies the
		 dependecy search in these objects */
			if(object->getSchema())
				getObjectDependecies(object->getSchema(), deps, inc_indirect_deps);

			if(object->getTablespace())
				getObjectDependecies(object->getTablespace(), deps, inc_indirect_deps);

			if(object->getOwner())
				getObjectDependecies(object->getOwner(), deps, inc_indirect_deps);

			if(object->getCollation())
				getObjectDependecies(object->getCollation(), deps, inc_indirect_deps);

			//** Getting the dependecies for operator class **
			if(obj_type==ObjectType::ObjOpClass)
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
			//** Getting the dependecies for domain **
			else if(obj_type==ObjectType::ObjDomain)
			{
				BaseObject *usr_type=getObjectPgSQLType(dynamic_cast<Domain *>(object)->getType());

				if(usr_type)
					getObjectDependecies(usr_type, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for conversion **
			else if(obj_type==ObjectType::ObjConversion)
			{
				Function *func=dynamic_cast<Conversion *>(object)->getConversionFunction();
				getObjectDependecies(func, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for cast **
			else if(obj_type==ObjectType::ObjCast)
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
			//** Getting the dependecies for event trigger **
			else if(obj_type==ObjectType::ObjEventTrigger)
			{
				getObjectDependecies(dynamic_cast<EventTrigger *>(object)->getFunction(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for function **
			else if(obj_type==ObjectType::ObjFunction)
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
			//** Getting the dependecies for aggregate **
			else if(obj_type==ObjectType::ObjAggregate)
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
			//** Getting the dependecies for language **
			else if(obj_type==ObjectType::ObjLanguage)
			{
				Language *lang=dynamic_cast<Language *>(object);

				for(unsigned i=Language::ValidatorFunc; i <= Language::InlineFunc; i++)
				{
					if(lang->getFunction(i))
						getObjectDependecies(lang->getFunction(i), deps, inc_indirect_deps);
				}
			}
			//** Getting the dependecies for operator **
			else if(obj_type==ObjectType::ObjOperator)
			{
				Operator *oper=dynamic_cast<Operator *>(object);
				BaseObject *usr_type=nullptr;
				unsigned i;

				for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
				{
					if(oper->getFunction(i))
						getObjectDependecies(oper->getFunction(i), deps, inc_indirect_deps);
				}

				for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
				{
					usr_type=getObjectPgSQLType(oper->getArgumentType(i));

					if(usr_type)
						getObjectDependecies(usr_type, deps, inc_indirect_deps);
				}

				for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
				{
					if(oper->getOperator(i))
						getObjectDependecies(oper->getOperator(i), deps, inc_indirect_deps);
				}
			}
			//** Getting the dependecies for role **
			else if(obj_type==ObjectType::ObjRole)
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
			//** Getting the dependecies for relationships **
			else if(obj_type==ObjectType::ObjRelationship)
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
			//** Getting the dependecies for sequence **
			else if(obj_type==ObjectType::ObjSequence)
			{
				Sequence *seq=dynamic_cast<Sequence *>(object);
				if(seq->getOwnerColumn())
					getObjectDependecies(seq->getOwnerColumn()->getParentTable(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for column **
			else if(obj_type==ObjectType::ObjColumn)
			{
				Column *col=dynamic_cast<Column *>(object);
				BaseObject *usr_type=getObjectPgSQLType(col->getType()),
						*sequence=col->getSequence();

				if(usr_type)
					getObjectDependecies(usr_type, deps, inc_indirect_deps);

				if(sequence)
					getObjectDependecies(sequence, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for trigger **
			else if(obj_type==ObjectType::ObjTrigger)
			{
				Trigger *trig=dynamic_cast<Trigger *>(object);

				if(trig->getReferencedTable())
					getObjectDependecies(trig->getReferencedTable(), deps, inc_indirect_deps);

				if(trig->getFunction())
					getObjectDependecies(trig->getFunction(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for index **
			else if(obj_type==ObjectType::ObjIndex)
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
			else if(obj_type==ObjectType::ObjPolicy)
			{
				Policy *pol=dynamic_cast<Policy *>(object);

				for(auto role : pol->getRoles())
					getObjectDependecies(role, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for table **
			else if(obj_type==ObjectType::ObjTable)
			{
				Table *tab=dynamic_cast<Table *>(object);
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
							constr->getConstraintType()==ConstraintType::foreign_key)
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

				count=tab->getIndexCount();
				for(i=0; i < count; i++)
				{
					index=dynamic_cast<Index *>(tab->getIndex(i));
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

				count=tab->getPolicyCount();
				for(i=0; i < count; i++)
				{
					pol=dynamic_cast<Policy *>(tab->getPolicy(i));

					for(auto role : pol->getRoles())
						getObjectDependecies(role, deps, inc_indirect_deps);
				}
			}
			//** Getting the dependecies for user defined type **
			else if(obj_type==ObjectType::ObjType)
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
			//** Getting the dependecies for view **
			else if(obj_type==ObjectType::ObjView)
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

			if(obj_type == ObjectType::ObjTable || obj_type == ObjectType::ObjView)
			{
				BaseTable *tab = dynamic_cast<BaseTable *>(object);

				if(tab->getTag())
					deps.push_back(tab->getTag());
			}
		}
	}
}

void DatabaseModel::getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode, bool exclude_perms)
{
	refs.clear();

	if(object)
	{
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

		if(obj_type==ObjectType::ObjView && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			View *view=dynamic_cast<View *>(object);
			vector<BaseObject *> tab_objs=view->getObjects();
			refs.insert(refs.end(), tab_objs.begin(), tab_objs.end());
		}

		if(obj_type==ObjectType::ObjTable && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Table *table=dynamic_cast<Table *>(object);
			Sequence *seq=nullptr;
			Constraint *constr=nullptr;
			Table *tab=nullptr;
			Trigger *gat=nullptr;
			BaseRelationship *base_rel=nullptr;
			View *view=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			vector<TableObject *> *tab_objs;
			unsigned i, count;
			ObjectType tab_obj_types[4]={ ObjectType::ObjTrigger, ObjectType::ObjRule, ObjectType::ObjIndex, ObjectType::ObjPolicy };

			for(i=0; i < 4; i++)
			{
				tab_objs=table->getObjectList(tab_obj_types[i]);
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

			itr=tables.begin();
			itr_end=tables.end();

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				tab=dynamic_cast<Table *>(*itr);

				count=tab->getConstraintCount();
				for(i=0; i < count&& (!exclusion_mode || (exclusion_mode && !refer)); i++)
				{
					constr=tab->getConstraint(i);
					//If a constraint references its own parent table it'll not be included on the references list
					if(constr->getConstraintType()==ConstraintType::foreign_key &&
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

		if(obj_type==ObjectType::ObjFunction && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Function *func=dynamic_cast<Function *>(object);
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={ObjectType::ObjCast, ObjectType::ObjEventTrigger, ObjectType::ObjConversion,
									ObjectType::ObjAggregate, ObjectType::ObjOperator, ObjectType::ObjOpClass,
									ObjectType::ObjTable, ObjectType::ObjType, ObjectType::ObjLanguage };
			unsigned i, i1, count, cnt=sizeof(obj_types)/sizeof(ObjectType);
			Table *tab=nullptr;
			Aggregate *aggreg=nullptr;
			Operator *oper=nullptr;
			Trigger *trig=nullptr;
			Type *type=nullptr;
			Language *lang=nullptr;
			OperatorClass *opclass=nullptr;

			for(i=0; i < cnt && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				if(obj_types[i]==ObjectType::ObjCast)
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
				else if(obj_types[i]==ObjectType::ObjEventTrigger)
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
				else if(obj_types[i]==ObjectType::ObjConversion)
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
				else if(obj_types[i]==ObjectType::ObjAggregate)
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
				else if(obj_types[i]==ObjectType::ObjOperator)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						oper=dynamic_cast<Operator *>(*itr);

						if(oper->getFunction(Operator::FUNC_OPERATOR)==func ||
								oper->getFunction(Operator::FUNC_JOIN)==func  ||
								oper->getFunction(Operator::FUNC_RESTRICT)==func)
						{
							refer=true;
							refs.push_back(oper);
						}
						itr++;
					}
				}
				else if(obj_types[i]==ObjectType::ObjOpClass)
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
				else if(obj_types[i]==ObjectType::ObjTable)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						tab=dynamic_cast<Table *>(*itr);
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
				else if(obj_types[i]==ObjectType::ObjType)
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
				else if(obj_types[i]==ObjectType::ObjLanguage)
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
			}
		}

		if(obj_type==ObjectType::ObjSchema && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[11]={ObjectType::ObjFunction, ObjectType::ObjTable, ObjectType::ObjView,
									  ObjectType::ObjDomain, ObjectType::ObjAggregate, ObjectType::ObjOperator,
									  ObjectType::ObjSequence, ObjectType::ObjConversion,
									  ObjectType::ObjType, ObjectType::ObjOpFamily, ObjectType::ObjOpClass};
			unsigned i;

			for(i=0; i < 11 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
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

		if((obj_type==ObjectType::ObjType || obj_type==ObjectType::ObjDomain || obj_type==ObjectType::ObjSequence ||
			obj_type==ObjectType::ObjTable || obj_type==ObjectType::ObjExtension || obj_type==ObjectType::ObjView)
				&& (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={ObjectType::ObjTable, ObjectType::ObjOpClass, ObjectType::ObjCast,
									ObjectType::ObjDomain, ObjectType::ObjFunction, ObjectType::ObjAggregate,
									ObjectType::ObjOperator, ObjectType::ObjType, ObjectType::ObjRelationship };
			unsigned i, i1, count, tp_count = sizeof(obj_types)/sizeof(ObjectType);
			OperatorClass *op_class=nullptr;
			OperatorClassElement elem;
			Table *tab=nullptr;
			Column *col=nullptr;
			Cast *cast=nullptr;
			Domain *dom=nullptr;
			Function *func=nullptr;
			Aggregate *aggreg=nullptr;
			Operator *oper=nullptr;
			Type *type=nullptr;
			Relationship *rel=nullptr;
			void *ptr_pgsqltype=nullptr;

			switch(obj_type)
			{
				case ObjectType::ObjType: ptr_pgsqltype=dynamic_cast<Type*>(object); break;
				case ObjectType::ObjDomain:
			  ptr_pgsqltype=dynamic_cast<Domain*>(object);
			  break;
				case ObjectType::ObjSequence: ptr_pgsqltype=dynamic_cast<Sequence*>(object); break;
				case ObjectType::ObjExtension: ptr_pgsqltype=dynamic_cast<Extension*>(object); break;
				case ObjectType::ObjView: ptr_pgsqltype=dynamic_cast<View*>(object); break;
				default: ptr_pgsqltype=dynamic_cast<Table*>(object); break;
			}

			for(i=0; i < tp_count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				if(obj_types[i]==ObjectType::ObjRelationship)
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
				else if(obj_types[i]==ObjectType::ObjTable)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						tab=dynamic_cast<Table *>(*itr);
						itr++;

						count=tab->getColumnCount();
						for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
						{
							col=tab->getColumn(i1);

							if(!col->isAddedByRelationship() &&
								 (col->getType()==ptr_pgsqltype ||
									//Special case for postgis extension
									(obj_type == ObjectType::ObjExtension && object->getName() == QString("postgis") && col->getType().isGiSType())))
							{
								refer=true;
								refs.push_back(col);
							}
						}
					}
				}
				else if(obj_types[i]==ObjectType::ObjOpClass)
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
				else if(obj_types[i]==ObjectType::ObjDomain)
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
				else if(obj_types[i]==ObjectType::ObjType)
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
				else if(obj_types[i]==ObjectType::ObjAggregate)
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
				else if(obj_types[i]==ObjectType::ObjFunction)
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
				else if(obj_types[i]==ObjectType::ObjOperator)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						oper=dynamic_cast<Operator *>(*itr);
						itr++;

						if(oper->getArgumentType(Operator::LEFT_ARG)==ptr_pgsqltype ||
								oper->getArgumentType(Operator::RIGHT_ARG)==ptr_pgsqltype)
						{
							refer=true;
							refs.push_back(oper);
						}
					}
				}
				else if(obj_types[i]==ObjectType::ObjCast)
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

		if(obj_type==ObjectType::ObjRole && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[13]={ObjectType::ObjFunction, ObjectType::ObjTable, ObjectType::ObjDomain,
									  ObjectType::ObjAggregate, ObjectType::ObjSchema, ObjectType::ObjOperator,
									  ObjectType::ObjSequence, ObjectType::ObjConversion,
									  ObjectType::ObjLanguage, ObjectType::ObjTablespace,
									  ObjectType::ObjType, ObjectType::ObjOpFamily, ObjectType::ObjOpClass};
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

			for(i=0; i < 13 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
				{
					if((*itr)->getOwner()==role)
					{
						refer=true;
						refs.push_back(*itr);
					}

					if((*itr)->getObjectType() == ObjectType::ObjTable)
					{
						for(auto obj : *(dynamic_cast<Table *>(*itr))->getObjectList(ObjectType::ObjPolicy))
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

		if(obj_type==ObjectType::ObjTablespace && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==ObjectType::ObjLanguage && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==ObjectType::ObjOpClass && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *>::iterator itr, itr_end;
			Type *usertype=nullptr;
			Index *ind=nullptr;
			Constraint *constr=nullptr;
			Table *table=nullptr;

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
		}

		if(obj_type==ObjectType::ObjOperator && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={ObjectType::ObjOpClass,
									ObjectType::ObjAggregate,
									ObjectType::ObjOperator,
									ObjectType::ObjTable };
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

				if(obj_types[i]==ObjectType::ObjOpClass)
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
				else if(obj_types[i]==ObjectType::ObjAggregate)
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
				else if(obj_types[i]==ObjectType::ObjOperator)
				{
					while(itr!=itr_end && !refer)
					{
						oper_aux=dynamic_cast<Operator *>(*itr);
						itr++;

						for(i1=Operator::OPER_COMMUTATOR; i1 <= Operator::OPER_NEGATOR &&
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

							if(constr->getConstraintType()==ConstraintType::exclude)
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

		if(obj_type==ObjectType::ObjOpFamily && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==ObjectType::ObjCollation && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			ObjectType  obj_types[]={ ObjectType::ObjDomain, ObjectType::ObjCollation, ObjectType::ObjType },
					tab_obj_types[]={ ObjectType::ObjColumn, ObjectType::ObjIndex };
			unsigned i, count;
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			vector<TableObject *> *tab_obj_list=nullptr;
			vector<TableObject *>::iterator tab_itr, tab_itr_end;
			TableObject *tab_obj=nullptr;
			Table *table = nullptr;

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
			obj_list=getObjectList(ObjectType::ObjTable);
			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				table = dynamic_cast<Table *>(*itr);

				for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
				{
					tab_obj_list=table->getObjectList(tab_obj_types[i]);
					tab_itr=tab_obj_list->begin();
					tab_itr_end=tab_obj_list->end();

					while(tab_itr!=tab_itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						tab_obj=(*tab_itr);
						if((tab_obj->getObjectType()==ObjectType::ObjColumn && tab_obj->getCollation()==object) ||
								(tab_obj->getObjectType()==ObjectType::ObjIndex &&
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

		if(obj_type==ObjectType::ObjColumn && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Column *column=dynamic_cast<Column *>(object);
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType  obj_types[]={ ObjectType::ObjSequence, ObjectType::ObjView, ObjectType::ObjTable, ObjectType::ObjRelationship };
			unsigned i, count=sizeof(obj_types)/sizeof(ObjectType);

			for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
				{
					if((obj_types[i]==ObjectType::ObjSequence && dynamic_cast<Sequence *>(*itr)->getOwnerColumn()==column) ||
							(obj_types[i]==ObjectType::ObjView && dynamic_cast<View *>(*itr)->isReferencingColumn(column)))
					{
						refer=true;
						refs.push_back(*itr);
					}
					else if(obj_types[i]==ObjectType::ObjTable)
					{
						Table *tab=dynamic_cast<Table *>(*itr);
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

						count=tab->getIndexCount();
						for(idx=0; idx < count && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
						{
							index=tab->getIndex(idx);
							if(index->isReferColumn(column))
							{
								refer=true;
								refs.push_back(index);
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
					else if(obj_types[i]==ObjectType::ObjRelationship)
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


		if(obj_type==ObjectType::ObjTag && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *>::iterator itr, itr_end;
			vector<BaseObject *> list;
			Tag *tag=dynamic_cast<Tag *>(object);

			list.assign(tables.begin(), tables.end());
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

		if(obj_type==ObjectType::ObjSequence && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Table *table=nullptr;
			vector<TableObject *> *cols=nullptr;
			vector<TableObject *>::iterator itr, itr_end;
			unsigned i, cnt=tables.size();

			for(i=0; i < cnt && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				table=dynamic_cast<Table *>(tables[i]);
				cols=table->getObjectList(ObjectType::ObjColumn);
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
	ObjectType obj_types[]={ObjectType::ObjTable, ObjectType::ObjView,
							ObjectType::ObjRelationship, ObjectType::ObjBaseRelationship,
							ObjectType::ObjTextbox, ObjectType::ObjSchema };
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
				if(obj_types[i]==ObjectType::ObjRelationship || obj_types[i]==ObjectType::ObjBaseRelationship)
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
		ObjectType tab_obj_types[]={ObjectType::ObjColumn, ObjectType::ObjConstraint,
									ObjectType::ObjTrigger, ObjectType::ObjRule, ObjectType::ObjIndex, ObjectType::ObjPolicy};
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

BaseObject *DatabaseModel::getObjectPgSQLType(PgSQLType type)
{
	switch(type.getUserTypeConfig())
	{
		case UserTypeConfig::BASE_TYPE:
		return(this->getObject(*type, ObjectType::ObjType));
		break;

		case UserTypeConfig::DOMAIN_TYPE:
		return(this->getObject(*type, ObjectType::ObjDomain));
		break;

		case UserTypeConfig::TABLE_TYPE:
		return(this->getObject(*type, ObjectType::ObjTable));
		break;

		case UserTypeConfig::VIEW_TYPE:
		return(this->getObject(*type, ObjectType::ObjView));
		break;

		case UserTypeConfig::SEQUENCE_TYPE:
		return(this->getObject(*type, ObjectType::ObjSequence));
		break;

		case UserTypeConfig::EXTENSION_TYPE:
		return(this->getObject(*type, ObjectType::ObjExtension));
		break;

		default:
		return(nullptr);
		break;
	}
}

void DatabaseModel::validateSchemaRenaming(Schema *schema, const QString &prev_sch_name)
{
	ObjectType types[]={ ObjectType::ObjTable, ObjectType::ObjView, ObjectType::ObjDomain, ObjectType::ObjType, ObjectType::ObjSequence };
	vector<BaseObject *> list, vet;
	BaseObject *obj=nullptr;
	QString prev_name;

	//Raise an error if the schema is not allocated
	if(!schema)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Get all the objects on the informed schema
	for(unsigned i=0; i < 5; i++)
	{
		vet=getObjects(types[i], schema);
		list.insert(list.end(), vet.begin(), vet.end());
	}

	while(!list.empty())
	{
		obj=list.back();

		//For objects that is not a Views is needed to rename the pgsql type represented by the object
		if(obj->getObjectType()!=ObjectType::ObjView)
		{
			//Configures the previous type name
			prev_name=BaseObject::formatName(prev_sch_name) + QString(".") +
					  BaseObject::formatName(obj->getName(), false);

			/* Special case for tables. Need to make a dynamic_cast before the reinterpret_cast to get
			the correct reference to table */
			if(obj->getObjectType()==ObjectType::ObjTable)
				PgSQLType::renameUserType(prev_name, reinterpret_cast<void *>(dynamic_cast<Table *>(obj)), obj->getName(true));
			else
				PgSQLType::renameUserType(prev_name, reinterpret_cast<void *>(obj), obj->getName(true));
		}

		//For graphical objects set them as modified to redraw them
		if(obj->getObjectType()==ObjectType::ObjTable || obj->getObjectType()==ObjectType::ObjView)
			dynamic_cast<BaseGraphicObject *>(obj)->setModified(true);

		list.pop_back();
	}
}

void DatabaseModel::createSystemObjects(bool create_public)
{
	Schema *public_sch=nullptr, *pg_catalog=nullptr;
	Language *lang=nullptr;
	Tablespace *tbspace=nullptr;
	LanguageType lang_types[]={ LanguageType::c, LanguageType::sql, LanguageType::plpgsql, LanguageType::internal };
	Role *postgres=nullptr;
	Collation *collation=nullptr;
	QString collnames[]={ "default", "C", "POSIX" };

	/* The particular case is for public schema that is created only when the flag
	is set. This because the public schema is written on model file even being
	a system object. This strategy permits the user controls the schema rectangle behavior */
	if(create_public && getObjectIndex(QString("public"), ObjectType::ObjSchema) < 0)
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

	for(unsigned i=0; i < sizeof(lang_types)/sizeof(LanguageType); i++)
	{
		if(getObjectIndex(~LanguageType(lang_types[i]), ObjectType::ObjLanguage) < 0)
		{
			lang=new Language;
			lang->BaseObject::setName(~LanguageType(lang_types[i]));
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
	setDefaultObject(getObject(QString("public"), ObjectType::ObjSchema), ObjectType::ObjSchema);
}

vector<BaseObject *> DatabaseModel::findObjects(const QString &pattern, vector<ObjectType> types, bool format_obj_names, bool case_sensitive, bool is_regexp, bool exact_match)
{
	vector<BaseObject *> list, objs;
	vector<BaseObject *>::iterator end;
	vector<ObjectType>::iterator itr_tp=types.begin();
	vector<BaseObject *> tables;
	bool inc_tabs=false, inc_views=false;
	ObjectType obj_type;
	QRegExp regexp;
	QString obj_name;

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
			tables.insert(tables.end(), getObjectList(ObjectType::ObjTable)->begin(), getObjectList(ObjectType::ObjTable)->end());
			inc_tabs=true;
		}

		if(!inc_views && ((*itr_tp)==ObjectType::ObjRule || (*itr_tp)==ObjectType::ObjTrigger))
		{
			tables.insert(tables.end(), getObjectList(ObjectType::ObjView)->begin(), getObjectList(ObjectType::ObjView)->end());
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

		if(obj_type==ObjectType::ObjDatabase)
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

				if(tab->getObjectType()==ObjectType::ObjTable)
					tab_objs=dynamic_cast<Table *>(tab)->getObjectList(obj_type);
				else if(tab->getObjectType()==ObjectType::ObjView &&
						(obj_type==ObjectType::ObjTrigger || obj_type==ObjectType::ObjRule))
					tab_objs=dynamic_cast<View *>(tab)->getObjectList(obj_type);

				if(tab_objs)
					objs.insert(objs.end(), tab_objs->begin(), tab_objs->end());
			}
		}
	}

	//Try to find  the objects on the configured list
	while(!objs.empty())
	{
		//Quotes are removed from the name by default
		if(format_obj_names && !exact_match)
		{
			if(TableObject::isTableObject(objs.back()->getObjectType()))
			{
				TableObject *tab_obj=dynamic_cast<TableObject *>(objs.back());

				if(tab_obj->getParentTable())
					obj_name=tab_obj->getParentTable()->getName(true);
			}

			obj_name+=objs.back()->getName(true, true);
			obj_name.remove('"');
		}
		else
			obj_name=objs.back()->getName();

		//Try to match the name on the configured regexp
		if((exact_match && pattern==obj_name) ||
				(exact_match && regexp.exactMatch(obj_name)) ||
				(!exact_match && regexp.indexIn(obj_name) >= 0))
			list.push_back(objs.back());

		objs.pop_back();
		obj_name.clear();
	}

	//Removing the duplicate items on the list
	std::sort(list.begin(), list.end());
	end=std::unique(list.begin(), list.end());
	list.erase(end, list.end());

	return(list);
}

void DatabaseModel::setInvalidated(bool value)
{
	this->invalidated=value;
}

bool DatabaseModel::isInvalidated(void)
{
	return(invalidated);
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
		throw Exception(RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

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

bool  DatabaseModel::isAppendAtEOD(void)
{
	return(append_at_eod);
}

bool DatabaseModel::isPrependedAtBOD(void)
{
	return(prepend_at_bod);
}

bool DatabaseModel::isTemplate(void)
{
	return(is_template);
}

bool DatabaseModel::isAllowConnections(void)
{
	return(allow_conns);
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
	Table *tab_nn=nullptr;
	BaseTable *src_tab=nullptr, *dst_tab=nullptr, *base_tab=nullptr;
	Schema *schema=nullptr;
	QPointF pnt;
	ObjectType obj_type;
	int idx=0;
	bool save_db_attribs=false, save_objs_pos=false, save_objs_prot=false,
			save_objs_sqldis=false, save_textboxes=false, save_tags=false,
			save_custom_sql=false, save_custom_colors=false, save_fadeout=false,
			save_extattribs=false, save_genericsqls=false, save_objs_aliases=false;
	QStringList labels_attrs={ ParsersAttributes::SRC_LABEL,
														 ParsersAttributes::DST_LABEL,
														 ParsersAttributes::NAME_LABEL };

	save_db_attribs=(MetaDbAttributes & options) == MetaDbAttributes;
	save_objs_pos=(MetaObjsPositioning & options) == MetaObjsPositioning;
	save_objs_prot=(MetaObjsProtection & options) == MetaObjsProtection;
	save_objs_sqldis=(MetaObjsSqlDisabled & options) == MetaObjsSqlDisabled;
	save_textboxes=(MetaTextboxObjs & options) == MetaTextboxObjs;
	save_tags=(MetaTagObjs & options) == MetaTagObjs;
	save_custom_sql=(MetaObjsCustomSql & options) == MetaObjsCustomSql;
	save_custom_colors=(MetaObjsCustomColors & options) == MetaObjsCustomColors;
	save_fadeout=(MetaObjsFadeOut & options) == MetaObjsFadeOut;
	save_extattribs=(MetaObjsExtAttribs & options) == MetaObjsExtAttribs;
	save_genericsqls=(MetaGenericSqlObjs & options) == MetaGenericSqlObjs;
	save_objs_aliases=(MetaObjsAliases & options) == MetaObjsAliases;

	output.open(QFile::WriteOnly);

	if(!output.isOpen())
		throw Exception(Exception::getErrorMessage(FileDirectoryNotWritten).arg(filename),
										FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
					pnt.setX((src_tab->getPosition().x() + dst_tab->getPosition().x())/2.0f);
					pnt.setY((src_tab->getPosition().y() + dst_tab->getPosition().y())/2.0f);
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

				for(auto &vw : views)
				{
					tab_objs = dynamic_cast<View *>(vw)->getObjects();
					objects.insert(objects.end(), tab_objs.begin(), tab_objs.end());
				}
			}
		}

		if(save_objs_prot || save_objs_sqldis)
		{
			vector<ObjectType> types=getChildObjectTypes(ObjectType::ObjDatabase), sch_types=getChildObjectTypes(ObjectType::ObjSchema);
			types.insert(types.end(), sch_types.begin(), sch_types.end());

			//Removing the types for schema, table and view to avoid retrieving the objects twice
			if(save_objs_pos)
			{
				types.erase(std::find(types.begin(), types.end(), ObjectType::ObjSchema));
				types.erase(std::find(types.begin(), types.end(), ObjectType::ObjTable));
				types.erase(std::find(types.begin(), types.end(), ObjectType::ObjView));
			}

			//Append the other objects to the list of selected entities
			for(ObjectType type : types)
				objects.insert(objects.end(), getObjectList(type)->begin(), getObjectList(type)->end());
		}

		for(BaseObject *object : objects)
		{
			obj_type=object->getObjectType();

			//When handling a tag , textbox or generic sql we just extract their XML code
			if(obj_type==ObjectType::ObjTextbox || obj_type==ObjectType::ObjTag || obj_type == ObjectType::ObjGenericSQL)
			{
				emit s_objectLoaded(((idx++)/static_cast<float>(objects.size()))*100,
														trUtf8("Saving object `%1' (%2)")
														.arg(object->getName()).arg(object->getTypeName()), obj_type);

				objs_def+=object->getCodeDefinition(SchemaParser::XmlDefinition);
				continue;
			}
			//Discarding the relationship added table objects (when extracting aliases)
			else if(TableObject::isTableObject(obj_type) && dynamic_cast<TableObject *>(object)->isAddedByRelationship())
				continue;

			graph_obj=dynamic_cast<BaseGraphicObject *>(object);
			base_tab=dynamic_cast<BaseTable *>(object);

			attribs[ParsersAttributes::TABLE]=QString();
			attribs[ParsersAttributes::NAME]=(TableObject::isTableObject(obj_type) ? object->getName() : object->getSignature());
			attribs[ParsersAttributes::ALIAS]=(save_objs_aliases ? object->getAlias() : QString());
			attribs[ParsersAttributes::TYPE]=object->getSchemaName();
			attribs[ParsersAttributes::PROTECTED]=(save_objs_prot && object->isProtected() && !object->isSystemObject() ? ParsersAttributes::_TRUE_ : QString());
			attribs[ParsersAttributes::SQL_DISABLED]=(save_objs_sqldis && object->isSQLDisabled() && !object->isSystemObject()  ? ParsersAttributes::_TRUE_ : QString());
			attribs[ParsersAttributes::TAG]=(save_tags && base_tab && base_tab->getTag() ? base_tab->getTag()->getName() : QString());
			attribs[ParsersAttributes::APPENDED_SQL]=object->getAppendedSQL();
			attribs[ParsersAttributes::PREPENDED_SQL]=object->getPrependedSQL();
			attribs[ParsersAttributes::HIDE_EXT_ATTRIBS]=(save_extattribs && base_tab && base_tab->isExtAttribsHidden() ? ParsersAttributes::_TRUE_ : QString());
			attribs[ParsersAttributes::FADED_OUT]=(save_fadeout && graph_obj && graph_obj->isFadedOut() ? ParsersAttributes::_TRUE_ : QString());

			if(TableObject::isTableObject(obj_type))
			{
				base_tab = dynamic_cast<TableObject *>(object)->getParentTable();
				attribs[ParsersAttributes::TABLE]=base_tab->getSignature();
			}

			if(save_custom_sql && obj_type==ObjectType::ObjDatabase)
			{
				attribs[ParsersAttributes::APPEND_AT_EOD]=(this->isAppendAtEOD() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
				attribs[ParsersAttributes::PREPEND_AT_BOD]=(this->isPrependedAtBOD() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
			}

			//Configuring database model attributes
			if(save_db_attribs && object==this)
			{			
				attribs[ParsersAttributes::MODEL_AUTHOR]=this->getAuthor();
				attribs[ParsersAttributes::LAST_POSITION]=QString("%1,%2").arg(last_pos.x()).arg(last_pos.y());
				attribs[ParsersAttributes::LAST_ZOOM]=QString::number(last_zoom);
				attribs[ParsersAttributes::DEFAULT_COLLATION]=(default_objs[ObjectType::ObjCollation] ? default_objs[ObjectType::ObjCollation]->getSignature() : QString());
				attribs[ParsersAttributes::DEFAULT_SCHEMA]=(default_objs[ObjectType::ObjSchema] ? default_objs[ObjectType::ObjSchema]->getSignature() : QString());
				attribs[ParsersAttributes::DEFAULT_TABLESPACE]=(default_objs[ObjectType::ObjTablespace] ? default_objs[ObjectType::ObjTablespace]->getSignature() : QString());
				attribs[ParsersAttributes::DEFAULT_OWNER]=(default_objs[ObjectType::ObjRole] ? default_objs[ObjectType::ObjRole]->getSignature() : QString());
			}

			//If the object is a graphic one and we need to save positions and colors
			if((save_objs_pos || save_custom_colors) && graph_obj)
			{
				if(obj_type!=ObjectType::ObjBaseRelationship && obj_type!=ObjectType::ObjRelationship)
				{
					pnt=graph_obj->getPosition();

					if(obj_type==ObjectType::ObjSchema)
					{
						schema=dynamic_cast<Schema *>(object);

						attribs[ParsersAttributes::CUSTOM_COLOR]=(save_custom_colors ? schema->getFillColor().name() : QString());
						attribs[ParsersAttributes::RECT_VISIBLE]=(schema->isRectVisible() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

						if(schema->isRectVisible())
						{
							attribs[ParsersAttributes::X_POS]=QString::number(pnt.x());
							attribs[ParsersAttributes::Y_POS]=QString::number(pnt.y());
						}
					}
					else
					{
						attribs[ParsersAttributes::X_POS]=QString::number(pnt.x());
						attribs[ParsersAttributes::Y_POS]=QString::number(pnt.y());
					}

					if(obj_type!=ObjectType::ObjSchema || !attribs[ParsersAttributes::X_POS].isEmpty())
					{
						schparser.ignoreUnkownAttributes(true);
						attribs[ParsersAttributes::POSITION]=
								schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																						GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																						ParsersAttributes::POSITION + GlobalAttributes::SchemaExt, attribs);
					}
				}
				else
				{
					BaseRelationship *rel=dynamic_cast<BaseRelationship *>(object);
					vector<QPointF> points=rel->getPoints();

					attribs_map aux_attribs;

					attribs[ParsersAttributes::CUSTOM_COLOR]=(save_custom_colors && rel->getCustomColor()!=Qt::transparent ? rel->getCustomColor().name() : ParsersAttributes::NONE);

					attribs[ParsersAttributes::SRC_TABLE]=rel->getTable(BaseRelationship::SrcTable)->getSignature();
					attribs[ParsersAttributes::SRC_TYPE]=rel->getTable(BaseRelationship::SrcTable)->getSchemaName();

					attribs[ParsersAttributes::DST_TABLE]=rel->getTable(BaseRelationship::DstTable)->getSignature();
					attribs[ParsersAttributes::DST_TYPE]=rel->getTable(BaseRelationship::DstTable)->getSchemaName();

					for(QPointF pnt : points)
					{
						attribs[ParsersAttributes::X_POS]=QString::number(pnt.x());
						attribs[ParsersAttributes::Y_POS]=QString::number(pnt.y());

						schparser.ignoreUnkownAttributes(true);
						attribs[ParsersAttributes::POSITION]+=
								schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																						GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																						ParsersAttributes::POSITION + GlobalAttributes::SchemaExt, attribs);
					}

					//Saving the labels' custom positions
					for(unsigned id=BaseRelationship::SrcCardLabel; id <= BaseRelationship::RelNameLabel; id++)
					{
						pnt=rel->getLabelDistance(id);
						if(!std::isnan(pnt.x()) && !std::isnan(pnt.y()))
						{
							aux_attribs[ParsersAttributes::X_POS]=QString::number(pnt.x());
							aux_attribs[ParsersAttributes::Y_POS]=QString::number(pnt.y());
							aux_attribs[ParsersAttributes::REF_TYPE]=labels_attrs[id];

							aux_attribs[ParsersAttributes::POSITION]=schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																																									 GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																																									 ParsersAttributes::POSITION + GlobalAttributes::SchemaExt, aux_attribs);

							attribs[ParsersAttributes::POSITION]+=schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																																								GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																																								ParsersAttributes::LABEL + GlobalAttributes::SchemaExt, aux_attribs);
						}
					}
				}
			}

			//Storing the custom SQLs if the object has them configured
			if(save_custom_sql)
			{
				if(!object->getAppendedSQL().isEmpty())
					attribs[ParsersAttributes::APPENDED_SQL]=schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																																							 GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																																							 QString(ParsersAttributes::APPENDED_SQL).remove(QChar('-')) + GlobalAttributes::SchemaExt, attribs);

				if(!object->getPrependedSQL().isEmpty())
					attribs[ParsersAttributes::PREPENDED_SQL]=schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																																								GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																																								QString(ParsersAttributes::PREPENDED_SQL).remove(QChar('-')) + GlobalAttributes::SchemaExt, attribs);
			}

			/* The object's metadata code will be generated only if one of the key attributes
			 for each option were filled previously. */
			if((save_db_attribs && obj_type==ObjectType::ObjDatabase) ||
				 (save_custom_colors &&
					((obj_type==ObjectType::ObjRelationship || obj_type==ObjectType::ObjBaseRelationship) ||
					 (!attribs[ParsersAttributes::CUSTOM_COLOR].isEmpty()))) ||
				 (save_objs_pos &&
					(!attribs[ParsersAttributes::POSITION].isEmpty() ||
					 !attribs[ParsersAttributes::RECT_VISIBLE].isEmpty())) ||
				 (save_tags && !attribs[ParsersAttributes::TAG].isEmpty()) ||
				 (save_objs_prot && !attribs[ParsersAttributes::PROTECTED].isEmpty()) ||
				 (save_objs_sqldis && !attribs[ParsersAttributes::SQL_DISABLED].isEmpty()) ||
				 (save_custom_sql && (!attribs[ParsersAttributes::APPENDED_SQL].isEmpty() ||
															!attribs[ParsersAttributes::PREPENDED_SQL].isEmpty())) ||
				 (save_fadeout && !attribs[ParsersAttributes::FADED_OUT].isEmpty()) ||
				 (save_extattribs && !attribs[ParsersAttributes::HIDE_EXT_ATTRIBS].isEmpty()) ||
				 (save_objs_aliases && !attribs[ParsersAttributes::ALIAS].isEmpty()))
			{
				emit s_objectLoaded(((idx++)/static_cast<float>(objects.size()))*100,
														trUtf8("Saving metadata of the object `%1' (%2)")
														.arg(object->getSignature()).arg(object->getTypeName()), obj_type);

				schparser.ignoreUnkownAttributes(true);
				objs_def+=schparser.convertCharsToXMLEntities(
										schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																							GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																							ParsersAttributes::INFO + GlobalAttributes::SchemaExt, attribs));
			}
			else
				idx++;

			attribs.clear();
		}

		if(!objs_def.isEmpty())
		{
			//Generates the metadata XML buffer
			attribs[ParsersAttributes::INFO]=objs_def;
			buf.append(schparser.getCodeDefinition(GlobalAttributes::SchemasRootDir + GlobalAttributes::DirSeparator +
																						 GlobalAttributes::XMLSchemaDir + GlobalAttributes::DirSeparator +
																						 ParsersAttributes::METADATA + GlobalAttributes::SchemaExt, attribs));
			output.write(buf.data(),buf.size());

			emit s_objectLoaded(100, trUtf8("Metadata file successfully saved!"), ObjectType::ObjBaseObject);
		}
		else
			emit s_objectLoaded(100, trUtf8("Process successfully ended but no metadata was saved!"), ObjectType::ObjBaseObject);

		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(FileNotWrittenInvalidDefinition).arg(filename),
										FileNotWrittenInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseModel::loadObjectsMetadata(const QString &filename, unsigned options)
{
	QString elem_name, aux_elem, obj_name, ref_type,
			dtd_file=GlobalAttributes::SchemasRootDir +
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
	vector<QPointF> labels_pos={ QPointF(NAN,NAN), QPointF(NAN,NAN), QPointF(NAN,NAN) };
	BaseRelationship *rel=nullptr;
	Schema *schema=nullptr;
	Tag *tag=nullptr;
	int progress=0;
	bool load_db_attribs=false, load_objs_pos=false, load_objs_prot=false,
			load_objs_sqldis=false, load_textboxes=false, load_tags=false,
			load_custom_sql=false, load_custom_colors=false, load_fadeout=false,
			load_extattribs=false, load_genericsqls=false, load_objs_aliases=false;

	load_db_attribs=(MetaDbAttributes & options) == MetaDbAttributes;
	load_objs_pos=(MetaObjsPositioning & options) == MetaObjsPositioning;
	load_objs_prot=(MetaObjsProtection & options) == MetaObjsProtection;
	load_objs_sqldis=(MetaObjsSqlDisabled & options) == MetaObjsSqlDisabled;
	load_textboxes=(MetaTextboxObjs & options) == MetaTextboxObjs;
	load_tags=(MetaTagObjs & options) == MetaTagObjs;
	load_custom_sql=(MetaObjsCustomSql & options) == MetaObjsCustomSql;
	load_custom_colors=(MetaObjsCustomColors & options) == MetaObjsCustomColors;
	load_fadeout=(MetaObjsFadeOut & options) == MetaObjsFadeOut;
	load_extattribs=(MetaObjsExtAttribs & options) == MetaObjsExtAttribs;
	load_genericsqls=(MetaGenericSqlObjs & options) == MetaGenericSqlObjs;
	load_objs_aliases=(MetaObjsAliases & options) == MetaObjsAliases;

	try
	{
		labels_attrs[ParsersAttributes::SRC_LABEL]=BaseRelationship::SrcCardLabel;
		labels_attrs[ParsersAttributes::DST_LABEL]=BaseRelationship::DstCardLabel;
		labels_attrs[ParsersAttributes::NAME_LABEL]=BaseRelationship::RelNameLabel;

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

					if((elem_name==BaseObject::getSchemaName(ObjectType::ObjTag) && load_tags) ||
						 (elem_name==BaseObject::getSchemaName(ObjectType::ObjTextbox) && load_textboxes) ||
						 (elem_name==BaseObject::getSchemaName(ObjectType::ObjGenericSQL) && load_genericsqls))
					{
						xmlparser.savePosition();
						obj_type=BaseObject::getObjectType(elem_name);
						new_object=createObject(obj_type);

						if(getObjectIndex(new_object->getName(), obj_type) < 0)
						{
							emit s_objectLoaded(progress, trUtf8("Creating object `%1' (%2)")
																	.arg(new_object->getName()).arg(new_object->getTypeName()), obj_type);
							addObject(new_object);
						}
						else
						{
							emit s_objectLoaded(progress, trUtf8("Object `%1' (%2) already exists. Ignoring.")
																	.arg(new_object->getName()).arg(new_object->getTypeName()), ObjectType::ObjBaseObject);
							delete(new_object);
						}

						new_object=nullptr;
						xmlparser.restorePosition();
					}
					else if(elem_name==ParsersAttributes::INFO)
					{
						xmlparser.getElementAttributes(attribs);
						obj_name=attribs[ParsersAttributes::OBJECT];
						xmlparser.savePosition();

						obj_type=BaseObject::getObjectType(attribs[ParsersAttributes::TYPE]);
						progress=xmlparser.getCurrentBufferLine()/static_cast<float>(xmlparser.getBufferLineCount()) * 100;

						if(obj_type==ObjectType::ObjDatabase)
						{
							if(load_db_attribs)
							{
								QStringList pos=attribs[ParsersAttributes::LAST_POSITION].split(',');

								default_objs[ObjectType::ObjSchema]=getSchema(attribs[ParsersAttributes::DEFAULT_SCHEMA]);
								default_objs[ObjectType::ObjRole]=getRole(attribs[ParsersAttributes::DEFAULT_OWNER]);
								default_objs[ObjectType::ObjCollation]=getCollation(attribs[ParsersAttributes::DEFAULT_COLLATION]);
								default_objs[ObjectType::ObjTablespace]=getTablespace(attribs[ParsersAttributes::DEFAULT_TABLESPACE]);
								author=attribs[ParsersAttributes::MODEL_AUTHOR];
								last_zoom=attribs[ParsersAttributes::LAST_ZOOM].toFloat();

								if(pos.size()>=2)
									last_pos=QPoint(pos[0].toFloat(), pos[1].toFloat());
							}

							object=this;
						}
						else if(TableObject::isTableObject(obj_type))
						{
							base_tab = getTable(attribs[ParsersAttributes::TABLE]);

							if(!base_tab && (obj_type == ObjectType::ObjRule || obj_type == ObjectType::ObjIndex || obj_type == ObjectType::ObjTrigger))
								base_tab = getView(attribs[ParsersAttributes::TABLE]);

							if(base_tab)
								object = base_tab->getObject(attribs[ParsersAttributes::OBJECT], obj_type);

							//Discarding the object if it was added by relationship
							if(object && dynamic_cast<TableObject *>(object)->isAddedByRelationship())
								object = nullptr;
						}
						else
							object=getObject(obj_name, obj_type);

						/* If the object does not exists but it is a relationship, we try to get the relationship
						 involving the tables in paramenters src-table and dst-table */
						if(!object && obj_type==ObjectType::ObjRelationship)
						{
							src_tab=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::SRC_TABLE],
																								BaseObject::getObjectType(attribs[ParsersAttributes::SRC_TYPE])));
							dst_tab=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::DST_TABLE],
																								BaseObject::getObjectType(attribs[ParsersAttributes::DST_TYPE])));
							object=getRelationship(src_tab, dst_tab);
						}

						if(object)
						{
							emit s_objectLoaded(progress, trUtf8("Loading metadata for object `%1' (%2)")
																	.arg(object->getName()).arg(object->getTypeName()), obj_type);

							if(!object->isSystemObject() &&
								 ((!attribs[ParsersAttributes::PROTECTED].isEmpty() && load_objs_prot) ||
									(!attribs[ParsersAttributes::SQL_DISABLED].isEmpty() && load_objs_sqldis)))
							{
								if(!attribs[ParsersAttributes::PROTECTED].isEmpty())
									object->setProtected(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

								if(!attribs[ParsersAttributes::SQL_DISABLED].isEmpty())
									object->setSQLDisabled(attribs[ParsersAttributes::SQL_DISABLED]==ParsersAttributes::_TRUE_);
							}
							else if((obj_type==ObjectType::ObjTable || obj_type==ObjectType::ObjView) && load_tags && !attribs[ParsersAttributes::TAG].isEmpty())
							{
								tag=getTag(attribs[ParsersAttributes::TAG]);

								if(tag)
									dynamic_cast<BaseTable *>(object)->setTag(tag);
							}
							else if(obj_type==ObjectType::ObjDatabase && load_custom_sql)
							{
								if(!attribs[ParsersAttributes::APPEND_AT_EOD].isEmpty())
									this->setAppendAtEOD(attribs[ParsersAttributes::APPEND_AT_EOD]==ParsersAttributes::_TRUE_);

								if(!attribs[ParsersAttributes::PREPEND_AT_BOD].isEmpty())
									this->setPrependAtBOD(attribs[ParsersAttributes::PREPEND_AT_BOD]==ParsersAttributes::_TRUE_);
							}

							if(load_objs_aliases && !attribs[ParsersAttributes::ALIAS].isEmpty())
								object->setAlias(attribs[ParsersAttributes::ALIAS]);

							if(xmlparser.accessElement(XmlParser::ChildElement))
							{
								do
								{
									aux_elem=xmlparser.getElementName();
									xmlparser.getElementAttributes(aux_attrib);

									//Retrieving and storing the points
									if(aux_elem==ParsersAttributes::POSITION)
									{
										points.push_back(QPointF(aux_attrib[ParsersAttributes::X_POS].toFloat(),
																						 aux_attrib[ParsersAttributes::Y_POS].toFloat()));
									}
									//Retrieving and storing the labels' custom positions
									else if(aux_elem==ParsersAttributes::LABEL)
									{
										ref_type=aux_attrib[ParsersAttributes::REF_TYPE];
										xmlparser.savePosition();

										if(xmlparser.accessElement(XmlParser::ChildElement))
										{
											xmlparser.getElementAttributes(aux_attrib);
											labels_pos[labels_attrs[ref_type]]=QPointF(aux_attrib[ParsersAttributes::X_POS].toFloat(),
																																 aux_attrib[ParsersAttributes::Y_POS].toFloat());
										}

										xmlparser.restorePosition();
									}
									else if(load_custom_sql && aux_elem==ParsersAttributes::APPENDED_SQL &&
													attribs[ParsersAttributes::APPENDED_SQL].isEmpty())
									{
										xmlparser.savePosition();
										xmlparser.accessElement(XmlParser::ChildElement);
										object->setAppendedSQL(xmlparser.getElementContent());
										xmlparser.restorePosition();
									}
									else if(load_custom_sql && aux_elem==ParsersAttributes::PREPENDED_SQL &&
													attribs[ParsersAttributes::PREPENDED_SQL].isEmpty())
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
										if(attribs[ParsersAttributes::CUSTOM_COLOR]==ParsersAttributes::NONE)
											rel->setCustomColor(Qt::transparent);
										else
											rel->setCustomColor(QColor(attribs[ParsersAttributes::CUSTOM_COLOR]));
									}

									if(load_objs_pos)
									{
										rel->setPoints(points);

										for(unsigned id=BaseRelationship::SrcCardLabel; id <= BaseRelationship::RelNameLabel; id++)
										{
											rel->setLabelDistance(id, labels_pos[id]);
											labels_pos[id]=QPointF(NAN,NAN);
										}
									}
								}
								else if(schema)
								{
									if(load_custom_colors)
										schema->setFillColor(QColor(attribs[ParsersAttributes::CUSTOM_COLOR]));

									schema->setRectVisible(attribs[ParsersAttributes::RECT_VISIBLE]==ParsersAttributes::_TRUE_);
								}

								if(load_fadeout)
									dynamic_cast<BaseGraphicObject *>(object)->setFadedOut(attribs[ParsersAttributes::FADED_OUT]==ParsersAttributes::_TRUE_);

								if(load_extattribs && base_tab)
									base_tab->setExtAttribsHidden(attribs[ParsersAttributes::HIDE_EXT_ATTRIBS]==ParsersAttributes::_TRUE_);
							}

							points.clear();

						}
						else if(!object)
						{
							emit s_objectLoaded(progress, trUtf8("Object `%1' (%2) not found. Ignoring metadata.")
																	.arg(obj_name).arg(BaseObject::getTypeName(obj_type)), ObjectType::ObjBaseObject);
						}

						xmlparser.restorePosition();

					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}

		emit s_objectLoaded(100, trUtf8("Metadata file successfully loaded!"), ObjectType::ObjBaseObject);
		setObjectsModified();
	}
	catch(Exception &e)
	{
		QString extra_info;

		if(xmlparser.getCurrentElement())
			extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}
}
