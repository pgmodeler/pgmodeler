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

#include "databasemodel.h"
#include "pgmodelerns.h"

unsigned DatabaseModel::dbmodel_id=2000;

DatabaseModel::DatabaseModel(void)
{
  this->model_wgt=nullptr;
	object_id=DatabaseModel::dbmodel_id++;
	obj_type=OBJ_DATABASE;

	encoding=BaseType::null;
	BaseObject::setName(QObject::trUtf8("new_database").toUtf8());

  default_objs[OBJ_SCHEMA]=nullptr;
  default_objs[OBJ_ROLE]=nullptr;
  default_objs[OBJ_TABLESPACE]=nullptr;
  default_objs[OBJ_COLLATION]=nullptr;

	conn_limit=-1;
  last_zoom=1;
  loading_model=invalidated=append_at_eod=prepend_at_bod=false;
	attributes[ParsersAttributes::ENCODING]=QString();
	attributes[ParsersAttributes::TEMPLATE_DB]=QString();
	attributes[ParsersAttributes::CONN_LIMIT]=QString();
	attributes[ParsersAttributes::_LC_COLLATE_]=QString();
	attributes[ParsersAttributes::_LC_CTYPE_]=QString();
	attributes[ParsersAttributes::APPEND_AT_EOD]=QString();
  attributes[ParsersAttributes::PREPEND_AT_EOD]=QString();
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
	if(localiz_id > Collation::_LC_COLLATE)
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->template_db=temp_db;
}

void DatabaseModel::setAuthor(const QString &author)
{
	this->author=author;
}

vector<BaseObject *> *DatabaseModel::getObjectList(ObjectType obj_type)
{
	if(obj_type==OBJ_TEXTBOX)
		return(&textboxes);
	else if(obj_type==OBJ_TABLE)
		return(&tables);
	else if(obj_type==OBJ_FUNCTION)
		return(&functions);
	else if(obj_type==OBJ_AGGREGATE)
		return(&aggregates);
	else if(obj_type==OBJ_SCHEMA)
		return(&schemas);
	else if(obj_type==OBJ_VIEW)
		return(&views);
	else if(obj_type==OBJ_TYPE)
		return(&types);
	else if(obj_type==OBJ_ROLE)
		return(&roles);
	else if(obj_type==OBJ_TABLESPACE)
		return(&tablespaces);
	else if(obj_type==OBJ_LANGUAGE)
		return(&languages);
	else if(obj_type==OBJ_CAST)
		return(&casts);
	else if(obj_type==OBJ_CONVERSION)
		return(&conversions);
	else if(obj_type==OBJ_OPERATOR)
		return(&operators);
	else if(obj_type==OBJ_OPCLASS)
		return(&op_classes);
	else if(obj_type==OBJ_OPFAMILY)
		return(&op_families);
	else if(obj_type==OBJ_DOMAIN)
		return(&domains);
	else if(obj_type==OBJ_SEQUENCE)
		return(&sequences);
	else if(obj_type==BASE_RELATIONSHIP)
		return(&base_relationships);
	else if(obj_type==OBJ_RELATIONSHIP)
		return(&relationships);
	else if(obj_type==OBJ_PERMISSION)
		return(&permissions);
	else if(obj_type==OBJ_COLLATION)
		return(&collations);
	else if(obj_type==OBJ_EXTENSION)
		return(&extensions);
  else if(obj_type==OBJ_TAG)
    return(&tags);
	else if(obj_type==OBJ_EVENT_TRIGGER)
		return(&eventtriggers);
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

			if(obj_type==OBJ_RELATIONSHIP ||
				 obj_type==BASE_RELATIONSHIP)
				addRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
			else if(obj_type==OBJ_TEXTBOX)
				addTextbox(dynamic_cast<Textbox *>(object), obj_idx);
			else if(obj_type==OBJ_TABLE)
				addTable(dynamic_cast<Table *>(object), obj_idx);
			else if(obj_type==OBJ_FUNCTION)
				addFunction(dynamic_cast<Function *>(object), obj_idx);
			else if(obj_type==OBJ_AGGREGATE)
				addAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
			else if(obj_type==OBJ_SCHEMA)
				addSchema(dynamic_cast<Schema *>(object), obj_idx);
			else if(obj_type==OBJ_VIEW)
				addView(dynamic_cast<View *>(object), obj_idx);
			else if(obj_type==OBJ_TYPE)
				addType(dynamic_cast<Type *>(object), obj_idx);
			else if(obj_type==OBJ_ROLE)
				addRole(dynamic_cast<Role *>(object), obj_idx);
			else if(obj_type==OBJ_TABLESPACE)
				addTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
			else if(obj_type==OBJ_LANGUAGE)
				addLanguage(dynamic_cast<Language *>(object), obj_idx);
			else if(obj_type==OBJ_CAST)
				addCast(dynamic_cast<Cast *>(object), obj_idx);
			else if(obj_type==OBJ_CONVERSION)
				addConversion(dynamic_cast<Conversion *>(object), obj_idx);
			else if(obj_type==OBJ_OPERATOR)
				addOperator(dynamic_cast<Operator *>(object), obj_idx);
			else if(obj_type==OBJ_OPCLASS)
				addOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
			else if(obj_type==OBJ_OPFAMILY)
				addOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
			else if(obj_type==OBJ_DOMAIN)
				addDomain(dynamic_cast<Domain *>(object), obj_idx);
			else if(obj_type==OBJ_SEQUENCE)
				addSequence(dynamic_cast<Sequence *>(object), obj_idx);
			else if(obj_type==OBJ_COLLATION)
				addCollation(dynamic_cast<Collation *>(object), obj_idx);
			else if(obj_type==OBJ_EXTENSION)
				addExtension(dynamic_cast<Extension *>(object), obj_idx);
      else if(obj_type==OBJ_TAG)
        addTag(dynamic_cast<Tag *>(object), obj_idx);
			else if(obj_type==OBJ_PERMISSION)
				addPermission(dynamic_cast<Permission *>(object));
			else if(obj_type==OBJ_EVENT_TRIGGER)
				addEventTrigger(dynamic_cast<EventTrigger *>(object));
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

			if(obj_type==OBJ_RELATIONSHIP ||
				 obj_type==BASE_RELATIONSHIP)
				removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
			else if(obj_type==OBJ_TEXTBOX)
				removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
			else if(obj_type==OBJ_TABLE)
				removeTable(dynamic_cast<Table *>(object), obj_idx);
			else if(obj_type==OBJ_FUNCTION)
				removeFunction(dynamic_cast<Function *>(object), obj_idx);
			else if(obj_type==OBJ_AGGREGATE)
				removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
			else if(obj_type==OBJ_SCHEMA)
				removeSchema(dynamic_cast<Schema *>(object), obj_idx);
			else if(obj_type==OBJ_VIEW)
				removeView(dynamic_cast<View *>(object), obj_idx);
			else if(obj_type==OBJ_TYPE)
				removeType(dynamic_cast<Type *>(object), obj_idx);
			else if(obj_type==OBJ_ROLE)
				removeRole(dynamic_cast<Role *>(object), obj_idx);
			else if(obj_type==OBJ_TABLESPACE)
				removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
			else if(obj_type==OBJ_LANGUAGE)
				removeLanguage(dynamic_cast<Language *>(object), obj_idx);
			else if(obj_type==OBJ_CAST)
				removeCast(dynamic_cast<Cast *>(object), obj_idx);
			else if(obj_type==OBJ_CONVERSION)
				removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
			else if(obj_type==OBJ_OPERATOR)
				removeOperator(dynamic_cast<Operator *>(object), obj_idx);
			else if(obj_type==OBJ_OPCLASS)
				removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
			else if(obj_type==OBJ_OPFAMILY)
				removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
			else if(obj_type==OBJ_DOMAIN)
				removeDomain(dynamic_cast<Domain *>(object), obj_idx);
			else if(obj_type==OBJ_SEQUENCE)
				removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
			else if(obj_type==OBJ_COLLATION)
				removeCollation(dynamic_cast<Collation *>(object), obj_idx);
			else if(obj_type==OBJ_EXTENSION)
				removeExtension(dynamic_cast<Extension *>(object), obj_idx);
      else if(obj_type==OBJ_TAG)
        removeTag(dynamic_cast<Tag *>(object), obj_idx);
			else if(obj_type==OBJ_PERMISSION)
				removePermission(dynamic_cast<Permission *>(object));
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
		 obj_type==BASE_OBJECT || obj_type==BASE_RELATIONSHIP ||
		 obj_type==OBJ_DATABASE)
		throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else
	{
		vector<BaseObject *> *obj_list=nullptr;
		BaseObject *object=nullptr;

		obj_list=getObjectList(obj_type);
		if(obj_idx >= obj_list->size())
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		object=(*obj_list)[obj_idx];
		if(obj_type==OBJ_TEXTBOX)
			removeTextbox(dynamic_cast<Textbox *>(object), obj_idx);
		else if(obj_type==OBJ_TABLE)
			removeTable(dynamic_cast<Table *>(object), obj_idx);
		else if(obj_type==OBJ_FUNCTION)
			removeFunction(dynamic_cast<Function *>(object), obj_idx);
		else if(obj_type==OBJ_AGGREGATE)
			removeAggregate(dynamic_cast<Aggregate *>(object), obj_idx);
		else if(obj_type==OBJ_SCHEMA)
			removeSchema(dynamic_cast<Schema *>(object), obj_idx);
		else if(obj_type==OBJ_VIEW)
			removeView(dynamic_cast<View *>(object), obj_idx);
		else if(obj_type==OBJ_TYPE)
			removeType(dynamic_cast<Type *>(object), obj_idx);
		else if(obj_type==OBJ_ROLE)
			removeRole(dynamic_cast<Role *>(object), obj_idx);
		else if(obj_type==OBJ_TABLESPACE)
			removeTablespace(dynamic_cast<Tablespace *>(object), obj_idx);
		else if(obj_type==OBJ_LANGUAGE)
			removeLanguage(dynamic_cast<Language *>(object), obj_idx);
		else if(obj_type==OBJ_CAST)
			removeCast(dynamic_cast<Cast *>(object), obj_idx);
		else if(obj_type==OBJ_CONVERSION)
			removeConversion(dynamic_cast<Conversion *>(object), obj_idx);
		else if(obj_type==OBJ_OPERATOR)
			removeOperator(dynamic_cast<Operator *>(object), obj_idx);
		else if(obj_type==OBJ_OPCLASS)
			removeOperatorClass(dynamic_cast<OperatorClass *>(object), obj_idx);
		else if(obj_type==OBJ_OPFAMILY)
			removeOperatorFamily(dynamic_cast<OperatorFamily *>(object), obj_idx);
		else if(obj_type==OBJ_DOMAIN)
			removeDomain(dynamic_cast<Domain *>(object), obj_idx);
		else if(obj_type==OBJ_SEQUENCE)
			removeSequence(dynamic_cast<Sequence *>(object), obj_idx);
		else if(obj_type==OBJ_COLLATION)
			removeCollation(dynamic_cast<Collation *>(object), obj_idx);
		else if(obj_type==OBJ_RELATIONSHIP || obj_type==BASE_RELATIONSHIP)
			removeRelationship(dynamic_cast<BaseRelationship *>(object), obj_idx);
		else if(obj_type==OBJ_PERMISSION)
			removePermission(dynamic_cast<Permission *>(object));
	}
}

void DatabaseModel::__addObject(BaseObject *object, int obj_idx)
{
	int idx;
	ObjectType obj_type;
	vector<BaseObject *> *obj_list=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!object)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);


	obj_type=object->getObjectType();

	#ifdef DEMO_VERSION
	 #warning "DEMO VERSION: database model object creation limit."
	 obj_list=getObjectList(obj_type);
	 if(obj_list && obj_list->size() >= GlobalAttributes::MAX_OBJECT_COUNT)
		 throw Exception(trUtf8("The demonstration version can create only `%1' instances of each object type! You've reach this limit for the type: `%2'")
										 .arg(GlobalAttributes::MAX_OBJECT_COUNT)
										 .arg(BaseObject::getTypeName(obj_type)),
										 ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	#endif

	if(obj_type==OBJ_TABLESPACE)
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
				throw Exception(Exception::getErrorMessage(ERR_ASG_DUP_TABLESPACE_DIR)
                        .arg(tabspc->getName())
												.arg(aux_tabspc->getName()),
												ERR_ASG_DUP_TABLESPACE_DIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			itr++;
		}
	}

	/* Raises an error if there is an object with the same name.
		 Special cases are for: functions/operator that are search by signature and views
		 that are search on tables and views list */
	if((obj_type==OBJ_VIEW &&	(getObject(object->getName(true), obj_type, idx) ||
														 getObject(object->getName(true), OBJ_TABLE, idx))) ||
		 (obj_type==OBJ_TABLE && (getObject(object->getName(true), obj_type, idx) ||
                              getObject(object->getName(true), OBJ_VIEW, idx))) ||
     (getObject(object->getSignature(), obj_type, idx)))
	{
		QString str_aux;

		str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
						.arg(object->getName(true))
						.arg(object->getTypeName())
						.arg(this->getName(true))
						.arg(this->getTypeName());

		throw Exception(str_aux,ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	try
	{
		if(obj_type==OBJ_TEXTBOX || obj_type==BASE_RELATIONSHIP)
			object->getCodeDefinition(SchemaParser::XML_DEFINITION);
		else
			object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
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
		throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		vector<BaseObject *> *obj_list=nullptr;
		ObjectType obj_type;

		obj_type=object->getObjectType();
		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			vector<BaseObject *> refs;

			//Get the table references
			if(check_refs)
				getObjectReferences(object, refs, true, true);

			//If there are objects referencing the table
			if(!refs.empty())
			{
				ErrorType err_type;

				//Raises an error indicating the object that is referencing the table
				if(!dynamic_cast<TableObject *>(refs[0]))
				{
					err_type=ERR_REM_DIRECT_REFERENCE;
					throw Exception(QString(Exception::getErrorMessage(err_type))
                          .arg(object->getName(true))
                          .arg(object->getTypeName())
                          .arg(refs[0]->getName(true))
              .arg(refs[0]->getTypeName()),
							err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
				else
				{
					BaseObject *ref_obj_parent=dynamic_cast<TableObject *>(refs[0])->getParentTable();

					err_type=ERR_REM_INDIRECT_REFERENCE;
					throw Exception(QString(Exception::getErrorMessage(err_type))
                          .arg(object->getName(true))
                          .arg(object->getTypeName())
                          .arg(refs[0]->getName(true))
              .arg(refs[0]->getTypeName())
              .arg(ref_obj_parent->getName(true))
              .arg(ref_obj_parent->getTypeName()),
							err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

      if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
         getObject(object->getSignature(), obj_type, obj_idx);

      if(obj_idx >= 0)
			{
        if(Permission::objectAcceptsPermission(obj_type))
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
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=obj_list->begin();
	itr_end=obj_list->end();

	while(itr!=itr_end)
	{
    rel=dynamic_cast<BaseRelationship *>(*itr);

    if((!rel && (*itr)->getSchema()==schema) ||
       (rel && (rel->getTable(BaseRelationship::SRC_TABLE)->getSchema()==schema ||
                rel->getTable(BaseRelationship::DST_TABLE)->getSchema()==schema)))
			sel_list.push_back(*itr);

		itr++;
	}

	return(sel_list);
}

vector<BaseObject *> DatabaseModel::getObjects(BaseObject *schema)
{
	vector<BaseObject *> *obj_list=nullptr, sel_list;
	vector<BaseObject *>::iterator itr, itr_end;
	ObjectType types[]={	OBJ_FUNCTION, OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN,
												OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_SEQUENCE, OBJ_CONVERSION,
												OBJ_TYPE, OBJ_OPCLASS, OBJ_OPFAMILY, OBJ_COLLATION,	OBJ_EXTENSION };
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
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
    /* This regexp is used for operator class and operator family.
       Their signature comes with a "USING index_mode" string
       that must be removed before any comparison is done */
    QRegExp aux_regexp(QString("( )+(USING)(.)+"));
    QString signature;

    itr=obj_list->begin();
		itr_end=obj_list->end();
		obj_idx=-1;		
    aux_name1=QString(name).remove('"').remove(aux_regexp);

    while(itr!=itr_end && !found)
    {
      signature=(*itr)->getSignature().remove("\"").remove(aux_regexp);
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
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(obj_idx >= obj_list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return(obj_list->at(obj_idx));
}

unsigned DatabaseModel::getObjectCount(ObjectType obj_type)
{
	vector<BaseObject *> *obj_list=nullptr;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return(obj_list->size());
}

unsigned DatabaseModel::getObjectCount(void)
{
	ObjectType types[20]={
		BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
		OBJ_AGGREGATE, OBJ_OPERATOR,
		OBJ_SEQUENCE, OBJ_CONVERSION,
		OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
		BASE_RELATIONSHIP, OBJ_TEXTBOX,
		OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
		OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
	unsigned count=0, i;

	for(i=0; i < 20; i++)
		count+=getObjectList(types[i])->size();

	return(count);
}

QString DatabaseModel::getLocalization(unsigned localiz_id)
{
	if(localiz_id > Collation::_LC_COLLATE)
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
    throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

  return(default_objs[obj_type]);
}

QString DatabaseModel::getAuthor(void)
{
	return(author);
}

void DatabaseModel::setProtected(bool value)
{
	ObjectType types[19]={
		OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
		OBJ_AGGREGATE, OBJ_OPERATOR,
		OBJ_SEQUENCE, OBJ_CONVERSION,
		OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
		BASE_RELATIONSHIP, OBJ_TEXTBOX,
		OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
		OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
	vector<BaseObject *> *lista=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	BaseObject *objeto=nullptr;
	unsigned i;

	for(i=0; i < 19; i++)
	{
		lista=getObjectList(types[i]);
		itr=lista->begin();
		itr_end=lista->end();

		while(itr!=itr_end)
		{
			objeto=(*itr);
			objeto->setProtected(value);
			itr++;
		}
	}

	BaseObject::setProtected(value);
}

void DatabaseModel::destroyObjects(void)
{
	ObjectType types[]={
		BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
		OBJ_AGGREGATE, OBJ_OPERATOR,
		OBJ_SEQUENCE, OBJ_CONVERSION,
		OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
		BASE_RELATIONSHIP, OBJ_TEXTBOX,
		OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION,
		OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE, OBJ_COLLATION,
		OBJ_EXTENSION, OBJ_SCHEMA, OBJ_PERMISSION };
	vector<BaseObject *> *list=nullptr;
	BaseObject *object=nullptr;
	unsigned i, cnt=sizeof(types)/sizeof(ObjectType);

  //Removing the special objects first
  storeSpecialObjectsXML();
  disconnectRelationships();

	for(i=0; i < cnt; i++)
	{
		list=getObjectList(types[i]);

		while(!list->empty())
		{
			object=list->back();

			/* If the object is graphical destroy using the __removeObject in order
			emit the signal to object scene to remove the graphical representation
			of the to-be-destroyed object */
			if(dynamic_cast<BaseGraphicObject *>(object))
			{
				__removeObject(object,-1,false);

				if(object->getObjectType()==OBJ_RELATIONSHIP)
					dynamic_cast<Relationship *>(object)->destroyObjects();
			}
			else
				list->pop_back();

      delete(object);
		}
	}

	PgSQLType::removeUserTypes(this);
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
	return(dynamic_cast<Table *>(getObject(obj_idx, OBJ_TABLE)));
}

Table *DatabaseModel::getTable(const QString &name)
{
	return(dynamic_cast<Table *>(getObject(name, OBJ_TABLE)));
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
	return(dynamic_cast<Sequence *>(getObject(obj_idx, OBJ_SEQUENCE)));
}

Sequence *DatabaseModel::getSequence(const QString &name)
{
	return(dynamic_cast<Sequence *>(getObject(name, OBJ_SEQUENCE)));
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
	return(dynamic_cast<Collation *>(getObject(obj_idx, OBJ_COLLATION)));
}

Collation *DatabaseModel::getCollation(const QString &name)
{
	return(dynamic_cast<Collation *>(getObject(name, OBJ_COLLATION)));
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
	return(dynamic_cast<Extension *>(getObject(obj_idx, OBJ_COLLATION)));
}

Extension *DatabaseModel::getExtension(const QString &name)
{
	return(dynamic_cast<Extension *>(getObject(name, OBJ_EXTENSION)));
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
	return(dynamic_cast<Tag *>(getObject(obj_idx, OBJ_TAG)));
}

Tag *DatabaseModel::getTag(const QString &name)
{
	return(dynamic_cast<Tag *>(getObject(name, OBJ_TAG)));
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
	return(dynamic_cast<EventTrigger *>(getObject(obj_idx, OBJ_EVENT_TRIGGER)));
}

EventTrigger *DatabaseModel::getEventTrigger(const QString &name)
{
	return(dynamic_cast<EventTrigger *>(getObject(name, OBJ_EVENT_TRIGGER)));
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
	return(dynamic_cast<View *>(getObject(obj_idx, OBJ_VIEW)));
}

View *DatabaseModel::getView(const QString &name)
{
	return(dynamic_cast<View *>(getObject(name, OBJ_VIEW)));
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
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

			if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK &&
				 (rel->getTable(BaseRelationship::SRC_TABLE)==table ||
					rel->getTable(BaseRelationship::DST_TABLE)==table))
			{
				if(rel->getTable(BaseRelationship::SRC_TABLE)==table)
					ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));
				else
					ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::SRC_TABLE));

				//Removes the relationship if the table does'nt references the 'ref_tab'
				if(!table->isReferTableOnForeignKey(ref_tab) &&
					 (rel->isSelfRelationship() ||
						(!rel->isSelfRelationship() && !ref_tab->isReferTableOnForeignKey(table))))
				{
					removeRelationship(rel);
					itr1=base_relationships.begin() + idx;
					itr1_end=base_relationships.end();
				}
				else
				{
					if(!rel->isSelfRelationship() && ref_tab->isReferTableOnForeignKey(table))
						rel->setModified(true);

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
			rel=getRelationship(table, ref_tab);

			if(!rel && ref_tab->getDatabase()==this)
			{
				rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK, table, ref_tab, false, false);
				rel->setCustomColor(Qt::transparent);

				/* Workaround: In some cases the combination of the two tablenames can generate a duplicated relationship
					 name so it`s necessary to check if a relationship with the same name already exists. If exists changes
					 the name of the new one */
        if(getObjectIndex(rel->getName(), BASE_RELATIONSHIP) >= 0)
          rel->setName(PgModelerNS::generateUniqueName(rel, base_relationships));

        addRelationship(rel);
			}
			else if(rel && rel->isBidirectional())
				rel->setModified(true);
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
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(getObjectIndex(view) < 0 || force_rel_removal)
	{
		//Remove all the relationship related to the view when this latter no longer exists
		itr=base_relationships.begin();
		itr_end=base_relationships.end();

		idx=0;
		while(itr!=itr_end)
		{
			rel=dynamic_cast<BaseRelationship *>(*itr);

			if(rel->getTable(BaseRelationship::SRC_TABLE)==view ||
				 rel->getTable(BaseRelationship::DST_TABLE)==view)
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

			if(rel->getTable(BaseRelationship::SRC_TABLE)==view ||
				 rel->getTable(BaseRelationship::DST_TABLE)==view)
			{
				if(rel->getTable(BaseRelationship::SRC_TABLE)->getObjectType()==OBJ_TABLE)
					tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::SRC_TABLE));
				else
					tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));

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
		ref_count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

		for(i=0; i < ref_count; i++)
		{
			ref=view->getReference(i, Reference::SQL_REFER_SELECT);
			tab=ref.getTable();

			rel=getRelationship(view,tab);
			if(tab && !rel)
			{
				rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_DEP,view,tab,false,false);
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

			if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
			{
				rel=dynamic_cast<Relationship *>(base_rel);
				rel->disconnectRelationship();
			}
			else
				base_rel->disconnectRelationship();
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
			 (rel->getRelationshipType()==Relationship::RELATIONSHIP_11 ||
				rel->getRelationshipType()==Relationship::RELATIONSHIP_1N) &&
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
			if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
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
						tab1=rel->getTable(BaseRelationship::SRC_TABLE);
						tab2=rel->getTable(BaseRelationship::DST_TABLE);

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
						if((e.getErrorType()!=ERR_LINK_TABLES_NO_PK && conn_tries[rel] > rels_gen_pk) ||
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
						else if(e.getErrorType()==ERR_LINK_TABLES_NO_PK &&
										(conn_tries[rel] > rels.size() ||
										 rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN))
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

	//Updates the schemas to ajdust its sizes due to the tables resizings
	while(!schemas.empty())
	{
		schemas.back()->setModified(true);
		schemas.pop_back();
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

			if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
				this->updateTableFKRelationships(dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SRC_TABLE)));

			itr++;
		}

		//Set all the model objects as modified to force the redraw of the entire model
		this->setObjectsModified();

		//Redirects all the errors captured on the revalidation
		throw Exception(ERR_INVALIDATED_OBJECTS,__PRETTY_FUNCTION__,__FILE__,__LINE__,errors);
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
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		rel_type=rel->getRelationshipType();

		/* Only identifier relationships or relationship that has identifier
		 attributes (primary keys) are checked */
		if((!rel->isSelfRelationship() &&
				(rel->isIdentifier() ||
				 rel->hasIndentifierAttribute())) ||

			 (rel_type==Relationship::RELATIONSHIP_GEN ||
				rel_type==Relationship::RELATIONSHIP_DEP))
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

				if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
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
							(aux_rel_type==Relationship::RELATIONSHIP_GEN ||
							 aux_rel_type==Relationship::RELATIONSHIP_DEP)))

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
				msg=Exception::getErrorMessage(ERR_INS_REL_GENS_REDUNDACY)
						.arg(rel->getName())
						.arg(str_aux);
				throw Exception(msg,ERR_INS_REL_GENS_REDUNDACY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
	ObjectType tab_obj_type[3]={ OBJ_CONSTRAINT, OBJ_TRIGGER, OBJ_INDEX };
	bool found=false;
  vector<BaseObject *> objects, rem_objects;

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

					if(tab_obj_type[type_id]==OBJ_CONSTRAINT)
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
							xml_special_objs[constr->getObjectId()]=constr->getCodeDefinition(SchemaParser::XML_DEFINITION, true);
					}
					else if(tab_obj_type[type_id]==OBJ_TRIGGER)
					{
						trigger=dynamic_cast<Trigger *>(tab_obj);
						found=trigger->isReferRelationshipAddedColumn();

						if(found)
							xml_special_objs[trigger->getObjectId()]=trigger->getCodeDefinition(SchemaParser::XML_DEFINITION);
					}
					else
					{
						index=dynamic_cast<Index *>(tab_obj);
						found=index->isReferRelationshipAddedColumn();

						if(found)
							xml_special_objs[index->getObjectId()]=index->getCodeDefinition(SchemaParser::XML_DEFINITION);
					}

					if(found)
					{
						//When found the special object must be removed from the parent table
						table->removeObject(tab_obj->getName(), tab_obj->getObjectType());

						//Removes the permission from the table object
						removePermissions(tab_obj);

						i--; count--;
					}
				}
			}
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
				xml_special_objs[sequence->getObjectId()]=sequence->getCodeDefinition(SchemaParser::XML_DEFINITION);
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
				xml_special_objs[view->getObjectId()]=view->getCodeDefinition(SchemaParser::XML_DEFINITION);

				/* Relationships linking the view and the referenced tables are considered as
			 special objects in this case only to be recreated more easely latter */

				count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

				for(i=0; i < count; i++)
				{
					ref=view->getReference(i, Reference::SQL_REFER_SELECT);
					table=ref.getTable();

					if(table)
					{
						//Get the relationship between the view and the referenced table
						rel=getRelationship(view, table);

						if(rel)
						{
							xml_special_objs[rel->getObjectId()]=rel->getCodeDefinition(SchemaParser::XML_DEFINITION);
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
          xml_special_objs[obj->getObjectId()]=obj->getCodeDefinition(SchemaParser::XML_DEFINITION);
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
				xml_special_objs[permission->getObjectId()]=permission->getCodeDefinition(SchemaParser::XML_DEFINITION);
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

		if(obj_type==OBJ_SEQUENCE)
			object=createSequence(true);
		else
			object=createObject(obj_type);

		if(obj_type==OBJ_SEQUENCE)
			addSequence(dynamic_cast<Sequence *>(object));
		else if(obj_type==OBJ_VIEW)
			addView(dynamic_cast<View *>(object));
		else if(obj_type==OBJ_PERMISSION)
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
			tab1=rel->getTable(BaseRelationship::SRC_TABLE);
			tab2=rel->getTable(BaseRelationship::DST_TABLE);

			//Raises an error if already exists an relationship between the tables
			if(getRelationship(tab1,tab2))
			{
				msg=Exception::getErrorMessage(ERR_DUPLIC_RELATIONSHIP)
            .arg(tab1->getName(true))
            .arg(tab1->getTypeName())
            .arg(tab2->getName(true))
            .arg(tab2->getTypeName());
				throw Exception(msg,ERR_DUPLIC_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		//Before add the relationship, checks if a redundancy can occur case the relationship is added
		if(rel->getObjectType()==OBJ_RELATIONSHIP)
			checkRelationshipRedundancy(dynamic_cast<Relationship *>(rel));

		__addObject(rel, obj_idx);

		if(rel->getObjectType()==OBJ_RELATIONSHIP)
		{
			dynamic_cast<Relationship *>(rel)->connectRelationship();
			validateRelationships();
		}
		else
			rel->connectRelationship();
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

			if(rel->getObjectType()==OBJ_RELATIONSHIP)
			{
        /* If the relationship is not a many-to-many we store the receiver table in order to
           update the fk relationships (if there are any) */
        if(rel->getRelationshipType()!=Relationship::RELATIONSHIP_NN)
          recv_tab=dynamic_cast<Relationship *>(rel)->getReceiverTable();

				storeSpecialObjectsXML();
				disconnectRelationships();
			}
			else if(rel->getObjectType()==BASE_RELATIONSHIP)
			{
				rel->disconnectRelationship();
			}

			__removeObject(rel, obj_idx);

			if(rel->getObjectType()==OBJ_RELATIONSHIP)
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
	if(rel_type!=OBJ_RELATIONSHIP && rel_type!=BASE_RELATIONSHIP)
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<BaseRelationship *>(getObject(obj_idx, rel_type)));
}

BaseRelationship *DatabaseModel::getRelationship(const QString &name)
{
	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(getObject(name, BASE_RELATIONSHIP));

	if(!rel)
		rel=dynamic_cast<BaseRelationship *>(getObject(name, OBJ_RELATIONSHIP));

	return(rel);
}

BaseRelationship *DatabaseModel::getRelationship(BaseTable *src_tab, BaseTable *dst_tab)
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


		if(src_tab->getObjectType()==OBJ_VIEW ||
			 dst_tab->getObjectType()==OBJ_VIEW)
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
			tab1=rel->getTable(BaseRelationship::SRC_TABLE);
			tab2=rel->getTable(BaseRelationship::DST_TABLE);

			found=((tab1==src_tab && tab2==dst_tab) ||
						 (tab2==src_tab && tab1==dst_tab) ||
						 (search_uniq_tab && (tab1==src_tab || tab2==src_tab)));

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

    if(base_rel->getTable(BaseRelationship::SRC_TABLE)==tab ||
       base_rel->getTable(BaseRelationship::DST_TABLE)==tab)
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
	return(dynamic_cast<Textbox *>(getObject(obj_idx, OBJ_TEXTBOX)));
}

Textbox *DatabaseModel::getTextbox(const QString &name)
{
	return(dynamic_cast<Textbox *>(getObject(name, OBJ_TEXTBOX)));
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
	return(dynamic_cast<Schema *>(getObject(obj_idx, OBJ_SCHEMA)));
}

Schema *DatabaseModel::getSchema(const QString &name)
{
	return(dynamic_cast<Schema *>(getObject(name, OBJ_SCHEMA)));
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
	return(dynamic_cast<Role *>(getObject(obj_idx, OBJ_ROLE)));
}

Role *DatabaseModel::getRole(const QString &name)
{
	return(dynamic_cast<Role *>(getObject(name, OBJ_ROLE)));
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
	return(dynamic_cast<Tablespace *>(getObject(obj_idx, OBJ_TABLESPACE)));
}

Tablespace *DatabaseModel::getTablespace(const QString &name)
{
	return(dynamic_cast<Tablespace *>(getObject(name, OBJ_TABLESPACE)));
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
	return(dynamic_cast<Cast *>(getObject(obj_idx, OBJ_CAST)));
}

Cast *DatabaseModel::getCast(const QString &name)
{
	return(dynamic_cast<Cast *>(getObject(name, OBJ_CAST)));
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
																							OBJ_CONVERSION)));
}

Conversion *DatabaseModel::getConversion(const QString &name)
{
	return(dynamic_cast<Conversion *>(getObject(name, OBJ_CONVERSION)));
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
	return(dynamic_cast<Language *>(getObject(obj_idx, OBJ_LANGUAGE)));
}

Language *DatabaseModel::getLanguage(const QString &name)
{
	return(dynamic_cast<Language *>(getObject(name, OBJ_LANGUAGE)));
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
	return(dynamic_cast<Function *>(getObject(obj_idx, OBJ_FUNCTION)));
}

Function *DatabaseModel::getFunction(const QString &signature)
{
	return(dynamic_cast<Function *>(getObject(signature, OBJ_FUNCTION)));
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
	return(dynamic_cast<Aggregate *>(getObject(obj_idx, OBJ_AGGREGATE)));
}

Aggregate *DatabaseModel::getAggregate(const QString &name)
{
	return(dynamic_cast<Aggregate *>(getObject(name, OBJ_AGGREGATE)));
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
			str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
							.arg(domain->getName(true))
							.arg(domain->getTypeName())
							.arg(this->getName(true))
							.arg(this->getTypeName());
			throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
	return(dynamic_cast<Domain *>(getObject(obj_idx, OBJ_DOMAIN)));
}

Domain *DatabaseModel::getDomain(const QString &name)
{
	return(dynamic_cast<Domain *>(getObject(name, OBJ_DOMAIN)));
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
	return(dynamic_cast<OperatorFamily *>(getObject(obj_idx, OBJ_OPFAMILY)));
}

OperatorFamily *DatabaseModel::getOperatorFamily(const QString &name)
{
	return(dynamic_cast<OperatorFamily *>(getObject(name, OBJ_OPFAMILY)));
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
	return(dynamic_cast<OperatorClass *>(getObject(obj_idx, OBJ_OPCLASS)));
}

OperatorClass *DatabaseModel::getOperatorClass(const QString &name)
{
	return(dynamic_cast<OperatorClass *>(getObject(name, OBJ_OPCLASS)));
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
	return(dynamic_cast<Operator *>(getObject(obj_idx, OBJ_OPERATOR)));
}

Operator *DatabaseModel::getOperator(const QString &signature)
{
	return(dynamic_cast<Operator *>(getObject(signature, OBJ_OPERATOR)));
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
			str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
              .arg(type->getName(true))
							.arg(type->getTypeName())
              .arg(this->getName(true))
							.arg(this->getTypeName());
			throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
	return(dynamic_cast<Type *>(getObject(obj_idx, OBJ_TYPE)));
}

Type *DatabaseModel::getType(const QString &name)
{
	return(dynamic_cast<Type *>(getObject(name, OBJ_TYPE)));
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
			throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		TableObject *tab_obj=dynamic_cast<TableObject *>(perm->getObject());

    if(getPermissionIndex(perm, false) >=0)
		{
			throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
                      .arg(perm->getObject()->getName())
											.arg(perm->getObject()->getTypeName()),
											ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Raises an error if the permission is referencing an object that does not exists on model
		else if(perm->getObject()!=this &&
						((tab_obj && (getObjectIndex(tab_obj->getParentTable()) < 0)) ||
						 (!tab_obj && (getObjectIndex(perm->getObject()) < 0))))
			throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                      .arg(perm->getName())
                      .arg(perm->getObject()->getTypeName())
                      .arg(perm->getObject()->getName())
                      .arg(perm->getObject()->getTypeName()),
											ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		permissions.push_back(perm);
		perm->setDatabase(this);
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==ERR_ASG_DUPLIC_OBJECT)
			throw
			Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
                .arg(perm->getObject()->getName())
								.arg(perm->getObject()->getTypeName()),
								ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

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
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

int DatabaseModel::getPermissionIndex(Permission *perm, bool check_contents)
{
	int perm_idx=-1;

	if(perm)
	{
    Permission *perm_aux=nullptr;
    vector<BaseObject *>::iterator itr, itr_end;

    itr=permissions.begin();
    itr_end=permissions.end();

    if(check_contents)
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
			throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
  prepend_at_bod=attribs[ParsersAttributes::PREPEND_AT_EOD]==ParsersAttributes::_TRUE_;


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
    bool protected_model=false;
    QStringList pos_str;
    map<ObjectType, QString> def_objs;

    //Configuring the path to the base path for objects DTD
    dtd_file=GlobalAttributes::SCHEMAS_ROOT_DIR +
        GlobalAttributes::DIR_SEPARATOR +
        GlobalAttributes::XML_SCHEMA_DIR +
        GlobalAttributes::DIR_SEPARATOR +
        GlobalAttributes::OBJECT_DTD_DIR +
        GlobalAttributes::DIR_SEPARATOR;

    try
    {
      loading_model=true;
			xmlparser.restartParser();

      //Loads the root DTD
			xmlparser.setDTDFile(dtd_file + GlobalAttributes::ROOT_DTD +
                            GlobalAttributes::OBJECT_DTD_EXT,
                            GlobalAttributes::ROOT_DTD);

      //Loads the file validating it against the root DTD
			xmlparser.loadXMLFile(filename);

      //Gets the basic model information
			xmlparser.getElementAttributes(attribs);

      this->author=attribs[ParsersAttributes::MODEL_AUTHOR];

      pos_str=attribs[ParsersAttributes::LAST_POSITION].split(',');

      if(pos_str.size()>=2)
        this->last_pos=QPoint(pos_str[0].toUInt(),pos_str[1].toUInt());

      this->last_zoom=attribs[ParsersAttributes::LAST_ZOOM].toDouble();
      if(this->last_zoom <= 0) this->last_zoom=1;

      protected_model=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

      def_objs[OBJ_SCHEMA]=attribs[ParsersAttributes::DEFAULT_SCHEMA];
      def_objs[OBJ_ROLE]=attribs[ParsersAttributes::DEFAULT_OWNER];
      def_objs[OBJ_COLLATION]=attribs[ParsersAttributes::DEFAULT_COLLATION];
      def_objs[OBJ_TABLESPACE]=attribs[ParsersAttributes::DEFAULT_TABLESPACE];

			if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
      {
        do
        {
					if(xmlparser.getElementType()==XML_ELEMENT_NODE)
          {
						elem_name=xmlparser.getElementName();

            //Indentifies the object type to be load according to the current element on the parser
            obj_type=getObjectType(elem_name);

            if(obj_type==OBJ_DATABASE)
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
                  if(!dynamic_cast<TableObject *>(object) && obj_type!=OBJ_RELATIONSHIP && obj_type!=BASE_RELATIONSHIP)
                    addObject(object);

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
				while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
      }

      this->BaseObject::setProtected(protected_model);

      //Validating default objects
      for(auto &itr : def_objs)
      {
        if(!itr.second.isEmpty())
        {
          object=this->getObject(itr.second, itr.first);

          if(!object)
            throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                      .arg(this->getName())
                      .arg(this->getTypeName())
                      .arg(itr.second)
                      .arg(BaseObject::getTypeName(itr.first)),
                      ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

          this->setDefaultObject(object);
        }
        else
          this->setDefaultObject(nullptr, itr.first);
      }

      loading_model=false;

      /* If there are relationship make an last relationship validation to
      recreate any special object left behind */
      if(!relationships.empty())
      {
        storeSpecialObjectsXML();
        disconnectRelationships();
        validateRelationships();
      }

      this->setInvalidated(false);
      this->setObjectsModified({OBJ_RELATIONSHIP, BASE_RELATIONSHIP});
    }
    catch(Exception &e)
    {
      QString extra_info;
      loading_model=false;
      destroyObjects();

			if(xmlparser.getCurrentElement())
				extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(xmlparser.getLoadedFilename()).arg(xmlparser.getCurrentElement()->line);

      if(e.getErrorType()>=ERR_INV_SYNTAX)
      {
        str_aux=QString(Exception::getErrorMessage(ERR_LOAD_INV_MODEL_FILE)).arg(filename);
        throw Exception(str_aux,ERR_LOAD_INV_MODEL_FILE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
      }
      else
        throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
    }
  }
}

BaseObject *DatabaseModel::createObject(ObjectType obj_type)
{
	BaseObject *object=nullptr;

	if(obj_type!=BASE_OBJECT)
	{
		if(obj_type==OBJ_ROLE)
			object=createRole();
		else if(obj_type==OBJ_TABLESPACE)
			object=createTablespace();
		else if(obj_type==OBJ_SCHEMA)
			object=createSchema();
		else if(obj_type==OBJ_LANGUAGE)
			object=createLanguage();
		else if(obj_type==OBJ_FUNCTION)
			object=createFunction();
		else if(obj_type==OBJ_TYPE)
			object=createType();
		else if(obj_type==OBJ_DOMAIN)
			object=createDomain();
		else if(obj_type==OBJ_CAST)
			object=createCast();
		else if(obj_type==OBJ_CONVERSION)
			object=createConversion();
		else if(obj_type==OBJ_OPERATOR)
			object=createOperator();
		else if(obj_type==OBJ_OPFAMILY)
			object=createOperatorFamily();
		else if(obj_type==OBJ_OPCLASS)
			object=createOperatorClass();
		else if(obj_type==OBJ_AGGREGATE)
			object=createAggregate();
		else if(obj_type==OBJ_TABLE)
			object=createTable();
		else if(obj_type==OBJ_SEQUENCE)
			object=createSequence();
		else if(obj_type==OBJ_VIEW)
			object=createView();
		else if(obj_type==OBJ_TEXTBOX)
			object=createTextbox();
		else if(obj_type==OBJ_CONSTRAINT)
			object=createConstraint(nullptr);
		else if(obj_type==OBJ_TRIGGER)
			object=createTrigger();//nullptr);
		else if(obj_type==OBJ_INDEX)
			object=createIndex();//nullptr);
		else if(obj_type==OBJ_COLUMN)
			object=createColumn();
		else if(obj_type==OBJ_RULE)
			object=createRule();//nullptr);
		else if(obj_type==OBJ_RELATIONSHIP ||
						obj_type==BASE_RELATIONSHIP)
			object=createRelationship();
		else if(obj_type==OBJ_COLLATION)
			object=createCollation();
		else if(obj_type==OBJ_EXTENSION)
			object=createExtension();
    else if(obj_type==OBJ_TAG)
      object=createTag();
		else if(obj_type==OBJ_PERMISSION)
			object=createPermission();
		else if(obj_type==OBJ_EVENT_TRIGGER)
			object=createEventTrigger();
	}

	return(object);
}

void DatabaseModel::setBasicAttributes(BaseObject *object)
{
	attribs_map attribs, attribs_aux;
	QString elem_name;
	BaseObject *tabspc=nullptr, *owner=nullptr, *collation=nullptr;
	Schema *schema=nullptr;
	ObjectType obj_type=BASE_OBJECT, obj_type_aux;
	bool has_error=false, protected_obj=false, sql_disabled=false;

	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	xmlparser.getElementAttributes(attribs);

	obj_type_aux=object->getObjectType();
	if(obj_type_aux!=OBJ_CAST)
		object->setName(attribs[ParsersAttributes::NAME]);

	protected_obj=attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;
	sql_disabled=attribs[ParsersAttributes::SQL_DISABLED]==ParsersAttributes::_TRUE_;

	xmlparser.savePosition();

	if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
					xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
					object->setComment(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::SCHEMA)
				{
					obj_type=OBJ_SCHEMA;
					xmlparser.getElementAttributes(attribs_aux);
					schema=dynamic_cast<Schema *>(getObject(attribs_aux[ParsersAttributes::NAME], obj_type));
					object->setSchema(schema);
					has_error=(!schema && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's tablespace
				else if(elem_name==ParsersAttributes::TABLESPACE)
				{
					obj_type=OBJ_TABLESPACE;
					xmlparser.getElementAttributes(attribs_aux);
					tabspc=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setTablespace(tabspc);
					has_error=(!tabspc && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's owner
				else if(elem_name==ParsersAttributes::ROLE)
				{
					obj_type=OBJ_ROLE;
					xmlparser.getElementAttributes(attribs_aux);
					owner=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setOwner(owner);
					has_error=(!owner && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::COLLATION)
				{
					obj_type=OBJ_COLLATION;
					xmlparser.getElementAttributes(attribs_aux);
					collation=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setCollation(collation);
					has_error=(!collation && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				else if(elem_name==ParsersAttributes::APPENDED_SQL)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
					object->setAppendedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
        else if(elem_name==ParsersAttributes::PREPENDED_SQL)
        {
					xmlparser.savePosition();
					xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
					object->setPrependedSQL(xmlparser.getElementContent());
					xmlparser.restorePosition();
        }
				//Defines the object's position (only for graphical objects)
				else if(elem_name==ParsersAttributes::POSITION)
				{
					xmlparser.getElementAttributes(attribs);

					if(elem_name==ParsersAttributes::POSITION &&
						 (obj_type_aux!=OBJ_RELATIONSHIP &&
							obj_type_aux!=BASE_RELATIONSHIP))
					{
            dynamic_cast<BaseGraphicObject *>(object)->setPosition(QPointF(attribs[ParsersAttributes::X_POS].toDouble(),
                                                                   attribs[ParsersAttributes::Y_POS].toDouble()));


					}
				}
			}
		}
		while(!has_error && xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
	}

	xmlparser.restorePosition();
	object->setProtected(protected_obj);
	object->setSQLDisabled(sql_disabled);

	if(has_error)
	{
		throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                    .arg(object->getName())
										.arg(object->getTypeName())
                    .arg(attribs_aux[ParsersAttributes::NAME])
				.arg(BaseObject::getTypeName(obj_type)),
				ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(!object->getSchema() &&
					(obj_type_aux==OBJ_FUNCTION || obj_type_aux==OBJ_TABLE ||
					 obj_type_aux==OBJ_VIEW  || obj_type_aux==OBJ_DOMAIN ||
					 obj_type_aux==OBJ_AGGREGATE || obj_type_aux==OBJ_OPERATOR ||
					 obj_type_aux==OBJ_SEQUENCE || obj_type_aux==OBJ_CONVERSION ||
					 obj_type_aux==OBJ_TYPE || obj_type_aux==OBJ_OPFAMILY ||
					 obj_type_aux==OBJ_OPCLASS))
	{
		throw Exception(Exception::getErrorMessage(ERR_ALOC_OBJECT_NO_SCHEMA)
                    .arg(object->getName())
										.arg(object->getTypeName()),
										ERR_ALOC_OBJECT_NO_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
												 ParsersAttributes::REPLICATION };

	unsigned op_vect[]={ Role::OP_SUPERUSER, Role::OP_CREATEDB,
											 Role::OP_CREATEROLE, Role::OP_INHERIT,
											 Role::OP_LOGIN, Role::OP_ENCRYPTED,
											 Role::OP_REPLICATION };

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
		for(i=0; i < 7; i++)
		{
			marked=attribs[op_attribs[i]]==ParsersAttributes::_TRUE_;
			role->setOption(op_vect[i], marked);
		}

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
							role_type=Role::REF_ROLE;
						else if(attribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::MEMBER)
							role_type=Role::MEMBER_ROLE;
						else
							role_type=Role::ADMIN_ROLE;

						for(i=0; i < len; i++)
						{
							//Gets the role using the name from the model using the name from the list
							ref_role=dynamic_cast<Role *>(getObject(list[i].trimmed(),OBJ_ROLE));

							//Raises an error if the roles doesn't exists
							if(!ref_role)
							{
								throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                                .arg(role->getName())
																.arg(BaseObject::getTypeName(OBJ_ROLE))
                                .arg(list[i])
																.arg(BaseObject::getTypeName(OBJ_ROLE)),
																ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							role->addRole(role_type, ref_role);
						}
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
          obj_type=BaseObject::getObjectType(xmlparser.getElementName());

					if(obj_type==OBJ_FUNCTION)
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
							func=getObject(signature, OBJ_FUNCTION);

							//Raises an error if the function doesn't exists
							if(!func)
								throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                                .arg(lang->getName())
																.arg(lang->getTypeName())
                                .arg(signature)
																.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
																ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

							if(ref_type==ParsersAttributes::VALIDATOR_FUNC)
								lang->setFunction(dynamic_cast<Function *>(func), Language::VALIDATOR_FUNC);
							else if(ref_type==ParsersAttributes::HANDLER_FUNC)
								lang->setFunction(dynamic_cast<Function *>(func), Language::HANDLER_FUNC);
							else
								lang->setFunction(dynamic_cast<Function *>(func), Language::INLINE_FUNC);

						}
						else
							//Raises an error if the function type is invalid
							throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
							xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

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
							while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));

							xmlparser.restorePosition();
						}
						catch(Exception &e)
						{
							xmlparser.restorePosition();
							throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
						}
					}
					//Gets the function language
					else if(obj_type==OBJ_LANGUAGE)
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::NAME], obj_type);

						//Raises an error if the function doesn't exisits
						if(!object)
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(func->getName())
															.arg(func->getTypeName())
                              .arg(attribs[ParsersAttributes::NAME])
								.arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
						else if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
							func->setSourceCode(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		if(func)
		{
			str_aux=func->getName(true);
			delete(func);
		}

		if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
			throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                      .arg(str_aux)
											.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
											ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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

						collation=getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                              .arg(tpattrib.getName())
                              .arg(tpattrib.getTypeName())
                              .arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(OBJ_COLLATION)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						tpattrib.setCollation(collation);
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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
			throw Exception(ERR_REF_INEXIST_USER_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			type->setConfiguration(Type::BASE_TYPE);
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
			func_types[ParsersAttributes::INPUT_FUNC]=Type::INPUT_FUNC;
			func_types[ParsersAttributes::OUTPUT_FUNC]=Type::OUTPUT_FUNC;
			func_types[ParsersAttributes::SEND_FUNC]=Type::SEND_FUNC;
			func_types[ParsersAttributes::RECV_FUNC]=Type::RECV_FUNC;
			func_types[ParsersAttributes::TPMOD_IN_FUNC]=Type::TPMOD_IN_FUNC;
			func_types[ParsersAttributes::TPMOD_OUT_FUNC]=Type::TPMOD_OUT_FUNC;
			func_types[ParsersAttributes::ANALYZE_FUNC]=Type::ANALYZE_FUNC;
		}
		else if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::COMPOSITE_TYPE)
			type->setConfiguration(Type::COMPOSITE_TYPE);
		else if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::ENUM_TYPE)
			type->setConfiguration(Type::ENUMERATION_TYPE);
		else
		{
			type->setConfiguration(Type::RANGE_TYPE);
			func_types[ParsersAttributes::CANONICAL_FUNC]=Type::CANONICAL_FUNC;
			func_types[ParsersAttributes::SUBTYPE_DIFF_FUNC]=Type::SUBTYPE_DIFF_FUNC;
		}

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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

						if(type->getConfiguration()==Type::RANGE_TYPE)
							type->setSubtype(aux_type);
						else
							type->setLikeType(aux_type);
					}
					else if(elem==ParsersAttributes::COLLATION)
					{
						xmlparser.getElementAttributes(attribs);
						collation=getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                              .arg(type->getName())
                              .arg(type->getTypeName())
                              .arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(OBJ_COLLATION)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setCollation(collation);
					}
					if(elem==ParsersAttributes::OP_CLASS)
					{
						xmlparser.getElementAttributes(attribs);
						op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], OBJ_OPCLASS));

						//Raises an error if the operator class doesn't exists
						if(!op_class)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                              .arg(type->getName())
                              .arg(type->getTypeName())
                              .arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(OBJ_OPCLASS)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setSubtypeOpClass(op_class);
					}
					//Configuring the functions used by the type (only for BASE type)
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);

						//Tries to get the function from the model
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(type->getName())
															.arg(type->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						//Raises an error if the function type is invalid
						else if(func_types.count(attribs[ParsersAttributes::REF_TYPE])==0)
							throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						type->setFunction(func_types[attribs[ParsersAttributes::REF_TYPE]],	dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		if(type)
		{
			str_aux=type->getName(true);
			delete(type);
		}

		if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
			throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                      .arg(str_aux)
											.arg(type->getTypeName()),
											ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, getErrorExtraInfo());
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

		if(!attribs[ParsersAttributes::CONSTRAINT].isEmpty())
			domain->setConstraintName(attribs[ParsersAttributes::CONSTRAINT]);

		if(!attribs[ParsersAttributes::DEFAULT_VALUE].isEmpty())
			domain->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		domain->setNotNull(attribs[ParsersAttributes::NOT_NULL]==
				ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
						domain->setExpression(xmlparser.getElementContent());
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(xmlparser.NEXT_ELEMENT));
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
			cast->setCastType(Cast::IMPLICIT);
		else if(attribs[ParsersAttributes::CAST_TYPE]==ParsersAttributes::ASSIGNMENT)
			cast->setCastType(Cast::ASSIGNMENT);
		else
			cast->setCastType(Cast::EXPLICIT);

		cast->setInOut(attribs[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
							cast->setDataType(Cast::SRC_TYPE, type);
						else
							cast->setDataType(Cast::DST_TYPE, type);
						type_idx++;
					}
					//Extracts the conversion function
					else if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(cast->getName())
															.arg(cast->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						cast->setCastFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		conv->setEncoding(Conversion::SRC_ENCODING,
											EncodingType(attribs[ParsersAttributes::SRC_ENCODING]));

		conv->setEncoding(Conversion::DST_ENCODING,
											EncodingType(attribs[ParsersAttributes::DST_ENCODING]));

		conv->setDefault(attribs[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(conv->getName())
															.arg(conv->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						conv->setConversionFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[OBJ_OPERATOR])
					{
						xmlparser.getElementAttributes(attribs);
						oper_aux=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_OPERATOR);

						//Raises an error if the auxiliary operator doesn't exists
						if(!oper_aux && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(oper->getSignature(true))
															.arg(oper->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(oper->getName())
															.arg(oper->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setFunction(dynamic_cast<Function *>(func),
															func_types[attribs[ParsersAttributes::REF_TYPE]]);
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		elem_types[ParsersAttributes::FUNCTION]=OperatorClassElement::FUNCTION_ELEM;
		elem_types[ParsersAttributes::OPERATOR]=OperatorClassElement::OPERATOR_ELEM;
		elem_types[ParsersAttributes::STORAGE]=OperatorClassElement::STORAGE_ELEM;

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==objs_schemas[OBJ_OPFAMILY])
					{
						xmlparser.getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::NAME], OBJ_OPFAMILY);

						//Raises an error if the operator family doesn't exists
						if(!object && !attribs[ParsersAttributes::NAME].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(op_class->getName())
															.arg(op_class->getTypeName())
                              .arg(attribs[ParsersAttributes::NAME])
								.arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
						xmlparser.getElementAttributes(attribs);

						if(elem_type==OperatorClassElement::STORAGE_ELEM)
						{
							type=createPgSQLType();
							class_elem.setStorage(type);
						}
						else if(elem_type==OperatorClassElement::FUNCTION_ELEM)
						{
							object=getObject(attribs[ParsersAttributes::SIGNATURE],OBJ_FUNCTION);
							class_elem.setFunction(dynamic_cast<Function *>(object),stg_number);
						}
						else if(elem_type==OperatorClassElement::OPERATOR_ELEM)
						{
							object=getObject(attribs[ParsersAttributes::SIGNATURE],OBJ_OPERATOR);
							class_elem.setOperator(dynamic_cast<Operator *>(object),stg_number);

							if(xmlparser.hasElement(XMLParser::NEXT_ELEMENT))
							{
								xmlparser.savePosition();
								xmlparser.accessElement(XMLParser::NEXT_ELEMENT);
								xmlparser.getElementAttributes(attribs_aux);

								object=getObject(attribs_aux[ParsersAttributes::NAME],OBJ_OPFAMILY);

								if(!object && !attribs_aux[ParsersAttributes::NAME].isEmpty())
									throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                                  .arg(op_class->getName())
																	.arg(op_class->getTypeName())
                                  .arg(attribs_aux[ParsersAttributes::NAME])
																	.arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
										ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

								class_elem.setOperatorFamily(dynamic_cast<OperatorFamily *>(object));
								xmlparser.restorePosition();
							}
						}

						op_class->addElement(class_elem);
						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(aggreg->getName())
															.arg(aggreg->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::TRANSITION_FUNC)
							aggreg->setFunction(Aggregate::TRANSITION_FUNC,
																	dynamic_cast<Function *>(func));
						else
							aggreg->setFunction(Aggregate::FINAL_FUNC,
																	dynamic_cast<Function *>(func));
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

	try
	{
		table=new Table;
		setBasicAttributes(table);
		xmlparser.getElementAttributes(attribs);

		table->setWithOIDs(attribs[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);
		table->setUnlogged(attribs[ParsersAttributes::UNLOGGED]==ParsersAttributes::_TRUE_);
		table->setGenerateAlterCmds(attribs[ParsersAttributes::GEN_ALTER_CMDS]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();
					xmlparser.savePosition();
					object=nullptr;

					if(elem==BaseObject::objs_schemas[OBJ_COLUMN])
						object=createColumn();
					else if(elem==BaseObject::objs_schemas[OBJ_CONSTRAINT])
						object=createConstraint(table);
          else if(elem==BaseObject::objs_schemas[OBJ_TAG])
          {
						xmlparser.getElementAttributes(aux_attribs);
            tag=getObject(aux_attribs[ParsersAttributes::NAME] ,OBJ_TAG);

            if(!tag)
            {
              throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(attribs[ParsersAttributes::NAME])
                              .arg(BaseObject::getTypeName(OBJ_TABLE))
                              .arg(aux_attribs[ParsersAttributes::TABLE])
                              .arg(BaseObject::getTypeName(OBJ_TAG))
                              , ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
            }

            table->setTag(dynamic_cast<Tag *>(tag));
          }
          //Retrieving custom columns / constraint indexes
          else if(elem==ParsersAttributes::CUSTOMIDXS)
          {
						xmlparser.getElementAttributes(aux_attribs);
            obj_type=BaseObject::getObjectType(aux_attribs[ParsersAttributes::OBJECT_TYPE]);

						xmlparser.savePosition();

						if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
							while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));

              table->setRelObjectsIndexes(names, idxs, obj_type);
              names.clear();
              idxs.clear();
            }

						xmlparser.restorePosition();
          }

					if(object)
						table->addObject(object);

					xmlparser.restorePosition();
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

    if(!attribs[ParsersAttributes::SEQUENCE].isEmpty())
    {
      seq=getObject(attribs[ParsersAttributes::SEQUENCE], OBJ_SEQUENCE);

      if(!seq)
        throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                                .arg(attribs[ParsersAttributes::NAME])
                                .arg(BaseObject::getTypeName(OBJ_COLUMN))
                                .arg(attribs[ParsersAttributes::SEQUENCE])
                                .arg(BaseObject::getTypeName(OBJ_SEQUENCE)),
                        ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);


      column->setSequence(seq);
    }

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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
			if(obj_type==OBJ_TABLE)
				table=dynamic_cast<Table *>(parent_obj);
			else if(obj_type==OBJ_RELATIONSHIP)
				rel=dynamic_cast<Relationship *>(parent_obj);
			else
				//Raises an error if the user tries to create a constraint in a invalid parent
				throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		else
		{
			obj_type=OBJ_TABLE;
			table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));
			parent_obj=table;
			ins_constr_table=true;

			//Raises an error if the parent table doesn't exists
			if(!table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                .arg(attribs[ParsersAttributes::NAME])
						.arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
            .arg(attribs[ParsersAttributes::TABLE])
						.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			throw Exception(Exception::getErrorMessage(ERR_INV_PRIM_KEY_ALOCATION)
                      .arg(constr->getName()),
											ERR_INV_PRIM_KEY_ALOCATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    deferrable=(attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
    constr->setDeferrable(deferrable);

    if(deferrable && !attribs[ParsersAttributes::DEFER_TYPE].isEmpty())
      constr->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

		if(constr_type==ConstraintType::foreign_key)
		{
			if(!attribs[ParsersAttributes::COMPARISON_TYPE].isEmpty())
				constr->setMatchType(attribs[ParsersAttributes::COMPARISON_TYPE]);

			if(!attribs[ParsersAttributes::DEL_ACTION].isEmpty())
        constr->setActionType(attribs[ParsersAttributes::DEL_ACTION], Constraint::DELETE_ACTION);

			if(!attribs[ParsersAttributes::UPD_ACTION].isEmpty())
        constr->setActionType(attribs[ParsersAttributes::UPD_ACTION], Constraint::UPDATE_ACTION);

			ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_TABLE);

			if(!ref_table && table->getName(true)==attribs[ParsersAttributes::REF_TABLE])
				ref_table=table;

			//Raises an error if the referenced table doesn't exists
			if(!ref_table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                .arg(constr->getName())
								.arg(constr->getTypeName())
                .arg(attribs[ParsersAttributes::REF_TABLE])
						.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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


		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

						constr->setExpression(xmlparser.getElementContent());

						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::COLUMNS)
					{
						xmlparser.getElementAttributes(attribs);

						col_list=attribs[ParsersAttributes::NAMES].split(',');
						count=col_list.count();

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::SRC_COLUMNS)
							col_type=Constraint::SOURCE_COLS;
						else
							col_type=Constraint::REFERENCED_COLS;

						for(i=0; i < count; i++)
						{
							if(col_type==Constraint::SOURCE_COLS)
							{
								if(obj_type==OBJ_TABLE)
								{
									column=table->getColumn(col_list[i]);

									//If the column doesn't exists tries to get it searching by the old name
									if(!column)
										column=table->getColumn(col_list[i], true);
								}
								else
									column=dynamic_cast<Column *>(rel->getObject(col_list[i], OBJ_COLUMN));
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

	xml_elem=xmlparser.getElementName();

	if(xml_elem==ParsersAttributes::INDEX_ELEMENT || xml_elem==ParsersAttributes::EXCLUDE_ELEMENT)
	{
		xmlparser.getElementAttributes(attribs);

		elem.setSortingAttribute(Element::ASC_ORDER, attribs[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);
		elem.setSortingAttribute(Element::NULLS_FIRST, attribs[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
		elem.setSortingEnabled(attribs[ParsersAttributes::USE_SORTING]!=ParsersAttributes::_FALSE_);

		xmlparser.savePosition();
		xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

		do
		{
			xml_elem=xmlparser.getElementName();

			if(xmlparser.getElementType()==XML_ELEMENT_NODE)
			{
				if(xml_elem==ParsersAttributes::OP_CLASS)
				{
					xmlparser.getElementAttributes(attribs);
					op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], OBJ_OPCLASS));

					//Raises an error if the operator class doesn't exists
					if(!op_class)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                            .arg(tab_obj->getName())
                            .arg(tab_obj->getTypeName())
                            .arg(attribs[ParsersAttributes::NAME])
														.arg(BaseObject::getTypeName(OBJ_OPCLASS)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setOperatorClass(op_class);
				}
				//Checking if elem is a ExcludeElement to be able to assign an operator to it
				else if(xml_elem==ParsersAttributes::OPERATOR && dynamic_cast<ExcludeElement *>(&elem))
				{
					xmlparser.getElementAttributes(attribs);
					oper=dynamic_cast<Operator *>(getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_OPERATOR));

					//Raises an error if the operator doesn't exists
					if(!oper)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                            .arg(tab_obj->getName())
                            .arg(tab_obj->getTypeName())
                            .arg(attribs[ParsersAttributes::SIGNATURE])
														.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					dynamic_cast<ExcludeElement &>(elem).setOperator(oper);
				}
				else if(xml_elem==ParsersAttributes::COLLATION && dynamic_cast<IndexElement *>(&elem))
				{
					xmlparser.getElementAttributes(attribs);
					collation=dynamic_cast<Collation *>(getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION));

					//Raises an error if the operator class doesn't exists
					if(!collation)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                            .arg(tab_obj->getName())
                            .arg(tab_obj->getTypeName())
                            .arg(attribs[ParsersAttributes::NAME])
														.arg(BaseObject::getTypeName(OBJ_COLLATION)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					dynamic_cast<IndexElement &>(elem).setCollation(collation);
				}
				else if(xml_elem==ParsersAttributes::COLUMN)
				{
					xmlparser.getElementAttributes(attribs);

					if(parent_obj->getObjectType()==OBJ_TABLE)
					{
						column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[ParsersAttributes::NAME]);

						if(!column)
							column=dynamic_cast<Table *>(parent_obj)->getColumn(attribs[ParsersAttributes::NAME], true);
					}
					else
					{
						column=dynamic_cast<Column *>(dynamic_cast<Relationship *>(parent_obj)->getObject(attribs[ParsersAttributes::NAME], OBJ_COLUMN));
					}

					//Raises an error if the column doesn't exists
					if(!column)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                            .arg(tab_obj->getName())
                            .arg(tab_obj->getTypeName())
                            .arg(attribs[ParsersAttributes::NAME])
														.arg(BaseObject::getTypeName(OBJ_COLUMN)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setColumn(column);
				}
				else if(xml_elem==ParsersAttributes::EXPRESSION)
				{
					xmlparser.savePosition();
					xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
					elem.setExpression(xmlparser.getElementContent());
					xmlparser.restorePosition();
				}
			}
		}
		while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));

		xmlparser.restorePosition();
	}
}

XMLParser *DatabaseModel::getXMLParser(void)
{
  return(&xmlparser);
}

QString DatabaseModel::getAlterDefinition(BaseObject *object)
{
  DatabaseModel *db_aux=dynamic_cast<DatabaseModel *>(object);

  if(!db_aux)
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  try
  {
    QString alter_def=BaseObject::getAlterDefinition(object);

    if(this->conn_limit!=db_aux->conn_limit)
    {
      attributes[ParsersAttributes::CONN_LIMIT]=QString::number(db_aux->conn_limit);
      alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, false);
    }

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
	Table *table=nullptr;

	try
	{
		xmlparser.getElementAttributes(attribs);

		table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

		//Raises an error if the parent table doesn't exists
		if(!table)
		{
			str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
              .arg(attribs[ParsersAttributes::NAME])
              .arg(BaseObject::getTypeName(OBJ_INDEX))
              .arg(attribs[ParsersAttributes::TABLE])
              .arg(BaseObject::getTypeName(OBJ_TABLE));

			throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		index=new Index;
		setBasicAttributes(index);
		index->setParentTable(table);
		index->setIndexAttribute(Index::CONCURRENT, attribs[ParsersAttributes::CONCURRENT]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::UNIQUE, attribs[ParsersAttributes::UNIQUE]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::FAST_UPDATE, attribs[ParsersAttributes::FAST_UPDATE]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::BUFFERING, attribs[ParsersAttributes::BUFFERING]==ParsersAttributes::_TRUE_);
		index->setIndexingType(attribs[ParsersAttributes::INDEX_TYPE]);
		index->setFillFactor(attribs[ParsersAttributes::FACTOR].toUInt());

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
						str_aux=xmlparser.getElementContent();
						xmlparser.restorePosition();
            index->setPredicate(str_aux);
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}

		table->addIndex(index);
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

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_VIEW));

		if(!table)
			throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                      .arg(attribs[ParsersAttributes::NAME])
                      .arg(BaseObject::getTypeName(OBJ_RULE))
                      .arg(attribs[ParsersAttributes::TABLE])
                      .arg(BaseObject::getTypeName(OBJ_TABLE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		rule->setExecutionType(attribs[ParsersAttributes::EXEC_TYPE]);
		rule->setEventType(attribs[ParsersAttributes::EVENT_TYPE]);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

		if(!table)
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_VIEW));

		if(!table)
			throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                      .arg(attribs[ParsersAttributes::NAME])
                      .arg(BaseObject::getTypeName(OBJ_TRIGGER))
                      .arg(attribs[ParsersAttributes::TABLE])
                      .arg(BaseObject::getTypeName(OBJ_TABLE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);


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
			ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_TABLE);

			if(!ref_table)
				ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_VIEW);

			//Raises an error if the trigger is referencing a inexistent table
			if(!ref_table)
			{
				throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                        .arg(trigger->getName())
                        .arg(trigger->getTypeName())
                        .arg(attribs[ParsersAttributes::REF_TABLE])
												.arg(BaseObject::getTypeName(OBJ_TABLE)),
												ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			trigger->setReferecendTable(dynamic_cast<BaseTable *>(ref_table));
		}

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
						{
							str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                      .arg(trigger->getName())
											.arg(trigger->getTypeName())
                      .arg(attribs[ParsersAttributes::SIGNATURE])
                      .arg(BaseObject::getTypeName(OBJ_FUNCTION));

							throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						trigger->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==ParsersAttributes::CONDITION)
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
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
              column=dynamic_cast<Column *>(table->getObject(list_aux[i].trimmed(), OBJ_COLUMN));

							if(!column && dynamic_cast<Table *>(table))
                column=dynamic_cast<Table *>(table)->getColumn(list_aux[i].trimmed(), true);

							trigger->addColumn(column);
						}
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						xmlparser.getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                              .arg(event_trig->getName())
															.arg(event_trig->getTypeName())
                              .arg(attribs[ParsersAttributes::SIGNATURE])
															.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}

	}
	catch(Exception &e)
	{
		if(event_trig) delete(event_trig);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	return(event_trig);
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

			table=getObject(tab_name, OBJ_TABLE);

			//Raises an error if the column parent table doesn't exists
			if(!table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                .arg(sequence->getName())
								.arg(BaseObject::getTypeName(OBJ_SEQUENCE))
                .arg(tab_name)
								.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column=dynamic_cast<Table *>(table)->getColumn(col_name);

			if(!column)
				column=dynamic_cast<Table *>(table)->getColumn(col_name, true);

			//Raises an error if the column doesn't exists
			if(!column && !ignore_onwer)
				throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
                        .arg(sequence->getName(true)),
												ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

	try
	{
		view=new View;
		setBasicAttributes(view);

		xmlparser.getElementAttributes(attribs);
    view->setMaterialized(attribs[ParsersAttributes::MATERIALIZED]==ParsersAttributes::_TRUE_);
    view->setRecursive(attribs[ParsersAttributes::RECURSIVE]==ParsersAttributes::_TRUE_);
    view->setWithNoData(attribs[ParsersAttributes::WITH_NO_DATA]==ParsersAttributes::_TRUE_);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
							table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

							//Raises an error if the table doesn't exists
							if(!table)
							{
								str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                        .arg(view->getName())
												.arg(BaseObject::getTypeName(OBJ_VIEW))
                        .arg(attribs[ParsersAttributes::TABLE])
                        .arg(BaseObject::getTypeName(OBJ_TABLE));

								throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							if(!attribs[ParsersAttributes::COLUMN].isEmpty())
							{
								column=table->getColumn(attribs[ParsersAttributes::COLUMN]);

								if(!column)
									column=table->getColumn(attribs[ParsersAttributes::COLUMN], true);

								//Raises an error if the view references an inexistant column
								if(!column)
								{
									str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(view->getName())
													.arg(BaseObject::getTypeName(OBJ_VIEW))
                          .arg(attribs[ParsersAttributes::TABLE] + QString(".") +
                          attribs[ParsersAttributes::COLUMN])
											.arg(BaseObject::getTypeName(OBJ_COLUMN));

									throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
							}

							//Adds the configured reference to a temporarily list
							refs.push_back(Reference(table, column,
																			 attribs[ParsersAttributes::ALIAS],
														 attribs[ParsersAttributes::COLUMN_ALIAS]));
						}
						else
						{
							xmlparser.savePosition();
							str_aux=attribs[ParsersAttributes::ALIAS];

							xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
							xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
							refs.push_back(Reference(xmlparser.getElementContent(),str_aux));

							xmlparser.restorePosition();
						}
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						xmlparser.savePosition();
						xmlparser.getElementAttributes(attribs);
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

						if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CTE_EXPRESSION)
							view->setCommomTableExpression(xmlparser.getElementContent());
						else
						{
							if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
								type=Reference::SQL_REFER_SELECT;
							else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
								type=Reference::SQL_REFER_FROM;
							else
								type=Reference::SQL_REFER_WHERE;


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
          else if(elem==BaseObject::getSchemaName(OBJ_TAG))
          {
						xmlparser.getElementAttributes(aux_attribs);
            tag=getObject(aux_attribs[ParsersAttributes::NAME] ,OBJ_TAG);

            if(!tag)
            {
              throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(attribs[ParsersAttributes::NAME])
                              .arg(BaseObject::getTypeName(OBJ_TABLE))
                              .arg(aux_attribs[ParsersAttributes::TABLE])
                              .arg(BaseObject::getTypeName(OBJ_TAG))
                              , ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
            }

            view->setTag(dynamic_cast<Tag *>(tag));
          }
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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
			 view->addReference(*itr, Reference::SQL_VIEW_DEFINITION);
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
			copy_coll=this->getObject(attribs[ParsersAttributes::COLLATION], OBJ_COLLATION);

			//Raises an error if the copy collation doesn't exists
			if(!copy_coll)
			{
				throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                        .arg(collation->getName())
												.arg(BaseObject::getTypeName(OBJ_COLLATION))
                        .arg(attribs[ParsersAttributes::COLLATION])
                        .arg(BaseObject::getTypeName(OBJ_COLLATION)),
                        ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			collation->setCollation(dynamic_cast<Collation *>(copy_coll));
		}
		//Creating a collation using LC_COLLATE and LC_CTYPE params
		else
		{
			collation->setLocalization(Collation::_LC_COLLATE, attribs[ParsersAttributes::_LC_COLLATE_]);
			collation->setLocalization(Collation::_LC_CTYPE, attribs[ParsersAttributes::_LC_CTYPE_]);
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
		extension->setVersion(Extension::CUR_VERSION, attribs[ParsersAttributes::CUR_VERSION]);
		extension->setVersion(Extension::OLD_VERSION, attribs[ParsersAttributes::OLD_VERSION]);
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

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

		if(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::ITALIC_TXT, true);

		if(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::BOLD_TXT, true);

		if(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::UNDERLINE_TXT, true);

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
	attribs_map attribs;
	map<QString, unsigned> labels_id;
	BaseRelationship *base_rel=nullptr;
	Relationship *rel=nullptr;
	BaseTable *tables[2]={nullptr, nullptr};
    bool src_mand, dst_mand, identifier, protect, deferrable, sql_disabled, single_pk_col;
	DeferralType defer_type;
  ActionType del_action, upd_action;
	unsigned rel_type=0, i;
	ObjectType table_types[2]={OBJ_VIEW, OBJ_TABLE}, obj_rel_type;
	QString str_aux, elem,
			tab_attribs[2]={ ParsersAttributes::SRC_TABLE,
                       ParsersAttributes::DST_TABLE };
	QColor custom_color=Qt::transparent;

	try
	{
		labels_id[ParsersAttributes::NAME_LABEL]=BaseRelationship::REL_NAME_LABEL;
		labels_id[ParsersAttributes::SRC_LABEL]=BaseRelationship::SRC_CARD_LABEL;
		labels_id[ParsersAttributes::DST_LABEL]=BaseRelationship::DST_CARD_LABEL;

		xmlparser.getElementAttributes(attribs);
		protect=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::CUSTOM_COLOR].isEmpty())
			custom_color=QColor(attribs[ParsersAttributes::CUSTOM_COLOR]);

		if(attribs[ParsersAttributes::TYPE]!=ParsersAttributes::RELATION_TAB_VIEW &&
			 attribs[ParsersAttributes::TYPE]!=ParsersAttributes::RELATIONSHIP_FK)
		{
			table_types[0]=OBJ_TABLE;
			obj_rel_type=OBJ_RELATIONSHIP;
		}
		else
		{
			if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
				table_types[0]=OBJ_TABLE;

			obj_rel_type=BASE_RELATIONSHIP;
		}

		//Gets the participant tables
		for(i=0; i < 2; i++)
		{
			tables[i]=dynamic_cast<BaseTable *>(getObject(attribs[tab_attribs[i]], table_types[i]));

			//Raises an error if some table doesn't exists
			if(!tables[i])
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                .arg(attribs[ParsersAttributes::NAME])
                .arg(BaseObject::getTypeName(obj_rel_type))
                .arg(attribs[tab_attribs[i]])
                .arg(BaseObject::getTypeName(table_types[i]));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		if(obj_rel_type==BASE_RELATIONSHIP)
		{
			base_rel=getRelationship(tables[0], tables[1]);

			/* Creates the fk relationship if it not exists. This generally happens when a foreign key is
			added to the table after its creation. */
			if(!base_rel && attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
			{
				vector<Constraint *> fks;
				dynamic_cast<Table *>(tables[0])->getForeignKeys(fks, false, dynamic_cast<Table *>(tables[1]));

				/* If the tables[0] doesn't has any fk that references the tables[1] indicates that the relationship
				is being created before the fk that represents it (inconsistence!). In this case an error is raised. */
				if(fks.empty())
				{
					throw Exception(Exception::getErrorMessage(ERR_ALOC_INV_FK_RELATIONSHIP)
													.arg(attribs[ParsersAttributes::NAME])
                          .arg(tables[0]->getName(true)),
													ERR_ALOC_INV_FK_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}

				base_rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK, tables[0], tables[1], false, false);
				base_rel->setName(attribs[ParsersAttributes::NAME]);
				addRelationship(base_rel);
			}
			else if(base_rel)
				base_rel->setName(attribs[ParsersAttributes::NAME]);

			if(!base_rel)
				throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                        .arg(this->getName())
												.arg(this->getTypeName())
                        .arg(attribs[ParsersAttributes::NAME])
                        .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                        ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			base_rel->disconnectRelationship();
		}
		else
		{
      QString pat_attrib[]= { ParsersAttributes::SRC_COL_PATTERN, ParsersAttributes::DST_COL_PATTERN,
															ParsersAttributes::SRC_FK_PATTERN, ParsersAttributes::DST_FK_PATTERN,
                              ParsersAttributes::PK_PATTERN, ParsersAttributes::UQ_PATTERN,
                              ParsersAttributes::PK_COL_PATTERN };

      unsigned 	pattern_id[]= { Relationship::SRC_COL_PATTERN, Relationship::DST_COL_PATTERN,
                                Relationship::SRC_FK_PATTERN, Relationship::DST_FK_PATTERN,
                                Relationship::PK_PATTERN, Relationship::UQ_PATTERN,
                                Relationship::PK_COL_PATTERN },
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
				rel_type=BaseRelationship::RELATIONSHIP_11;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_1N)
				rel_type=BaseRelationship::RELATIONSHIP_1N;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_NN)
				rel_type=BaseRelationship::RELATIONSHIP_NN;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_GEN)
				rel_type=BaseRelationship::RELATIONSHIP_GEN;
			else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_DEP)
				rel_type=BaseRelationship::RELATIONSHIP_DEP;

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
			base_rel=rel;

			//Configuring the name patterns
			for(i=0; i < pat_count; i++)
				rel->setNamePattern(pattern_id[i], attribs[pat_attrib[i]]);
		}

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					elem=xmlparser.getElementName();

					if(elem==ParsersAttributes::COLUMN && rel)
					{
						xmlparser.savePosition();
						rel->addObject(createColumn());
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::CONSTRAINT && rel)
					{
						xmlparser.savePosition();
						rel->addObject(createConstraint(rel));
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::LINE)
					{
						vector<QPointF> points;
						xmlparser.savePosition();
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

						do
						{
							xmlparser.getElementAttributes(attribs);
              points.push_back(QPointF(attribs[ParsersAttributes::X_POS].toDouble(),
                               attribs[ParsersAttributes::Y_POS].toDouble()));
						}
						while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));

						base_rel->setPoints(points);
						xmlparser.restorePosition();
					}
					else if(elem==ParsersAttributes::LABEL)
					{
						xmlparser.getElementAttributes(attribs);
						str_aux=attribs[ParsersAttributes::REF_TYPE];

						xmlparser.savePosition();
						xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
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
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		if(base_rel && base_rel->getObjectType()==OBJ_RELATIONSHIP)
			delete(base_rel);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, getErrorExtraInfo());
	}

	if(rel)
	{
		storeSpecialObjectsXML();
		addRelationship(rel);
	}

	base_rel->setProtected(protect);
	base_rel->setCustomColor(custom_color);

	if(base_rel && base_rel->getObjectType()==BASE_RELATIONSHIP)
		base_rel->connectRelationship();

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
	unsigned i, len, priv_type=Permission::PRIV_SELECT;
	bool priv_value, grant_op, revoke, cascade;

	try
	{  
		xmlparser.getElementAttributes(priv_attribs);
		revoke=priv_attribs[ParsersAttributes::REVOKE]==ParsersAttributes::_TRUE_;
		cascade=priv_attribs[ParsersAttributes::CASCADE]==ParsersAttributes::_TRUE_;

		xmlparser.savePosition();
		xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
		xmlparser.getElementAttributes(attribs);

    obj_type=BaseObject::getObjectType(attribs[ParsersAttributes::TYPE]);
		obj_name=attribs[ParsersAttributes::NAME];
    parent_name=attribs[ParsersAttributes::PARENT];

		//If the object is a column its needed to get the parent table
		if(obj_type==OBJ_COLUMN)
		{
			parent_table=dynamic_cast<Table *>(getObject(parent_name, OBJ_TABLE));

			if(parent_table)
				object=parent_table->getColumn(obj_name);
		}
		else if(obj_type==OBJ_DATABASE)
		{
			object=this;
		}
		else
			object=getObject(obj_name, obj_type);

		//Raises an error if the permission references an object that does not exists
		if(!object)
			throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                      .arg(obj_name)
											.arg(BaseObject::getTypeName(obj_type)),
											ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
					role=dynamic_cast<Role *>(getObject(list[i].trimmed(),OBJ_ROLE));

					//Raises an error if the referenced role doesn't exists
					if(!role)
					{
						throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                            .arg(list[i])
														.arg(BaseObject::getTypeName(OBJ_ROLE)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
							priv_type=Permission::PRIV_CONNECT;
						else if(itr->first==ParsersAttributes::CREATE_PRIV)
							priv_type=Permission::PRIV_CREATE;
						else if(itr->first==ParsersAttributes::DELETE_PRIV)
							priv_type=Permission::PRIV_DELETE;
						else if(itr->first==ParsersAttributes::EXECUTE_PRIV)
							priv_type=Permission::PRIV_EXECUTE;
						else if(itr->first==ParsersAttributes::INSERT_PRIV)
							priv_type=Permission::PRIV_INSERT;
						else if(itr->first==ParsersAttributes::REFERENCES_PRIV)
							priv_type=Permission::PRIV_REFERENCES;
						else if(itr->first==ParsersAttributes::SELECT_PRIV)
							priv_type=Permission::PRIV_SELECT;
						else if(itr->first==ParsersAttributes::TEMPORARY_PRIV)
							priv_type=Permission::PRIV_TEMPORARY;
						else if(itr->first==ParsersAttributes::TRIGGER_PRIV)
							priv_type=Permission::PRIV_TRIGGER;
						else if(itr->first==ParsersAttributes::TRUNCATE_PRIV)
							priv_type=Permission::PRIV_TRUNCATE;
						else if(itr->first==ParsersAttributes::UPDATE_PRIV)
							priv_type=Permission::PRIV_UPDATE;
						else if(itr->first==ParsersAttributes::USAGE_PRIV)
              priv_type=Permission::PRIV_USAGE;

						perm->setPrivilege(priv_type, (priv_value || grant_op), grant_op);
					}
					itr++;
				}
			}
		}
		while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));

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
			throw Exception(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE)
                      .arg(column->getParentTable()->getName(true) + QString(".") + column->getName(true))
											.arg(column->getTypeName())
                      .arg(refs[0]->getName(true))
				.arg(refs[0]->getTypeName()),
				ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			> Case the object is a constraint and its a table primary key */
			revalidate_rels=((obj_type==OBJ_COLUMN &&
												parent_tab->isConstraintRefColumn(dynamic_cast<Column *>(object), ConstraintType::primary_key)) ||
											 (obj_type==OBJ_CONSTRAINT &&
												dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::primary_key));

			/* Additional validation for columns: checks if the parent table participates on a
			generalization/copy as destination table */
			if(obj_type==OBJ_COLUMN)
			{
				itr=relationships.begin();
				itr_end=relationships.end();

				while(itr!=itr_end && !ref_tab_inheritance)
				{
					rel=dynamic_cast<Relationship *>(*itr);
					itr++;
					ref_tab_inheritance=(rel->getRelationshipType()==Relationship::RELATIONSHIP_GEN &&
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

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		QString loc_attribs[]={ ParsersAttributes::_LC_CTYPE_,  ParsersAttributes::_LC_COLLATE_ };

		if(encoding!=BaseType::null)
      attributes[ParsersAttributes::ENCODING]=QString("'%1'").arg(~encoding);

		for(unsigned i=0; i < 2; i++)
		{
			if(!localizations[i].isEmpty())
			{
				attributes[loc_attribs[i]]=localizations[i];

        if(localizations[i]!=QString("C") && encoding!=BaseType::null)
          attributes[loc_attribs[i]]+= QString(".") + ~encoding;

        attributes[loc_attribs[i]]=QString("'%1'").arg(attributes[loc_attribs[i]]);
			}
		}

	}
	else
	{
		attributes[ParsersAttributes::ENCODING]=(~encoding);
		attributes[ParsersAttributes::_LC_COLLATE_]=localizations[1];
		attributes[ParsersAttributes::_LC_CTYPE_]=localizations[0];
		attributes[ParsersAttributes::APPEND_AT_EOD]=(append_at_eod ? ParsersAttributes::_TRUE_ : QString());
    attributes[ParsersAttributes::PREPEND_AT_EOD]=(prepend_at_bod ? ParsersAttributes::_TRUE_ : QString());
	}

	attributes[ParsersAttributes::TEMPLATE_DB]=template_db;

	if(def_type==SchemaParser::SQL_DEFINITION && append_at_eod)
	{
		bkp_appended_sql=this->appended_sql;
		this->appended_sql.clear();
	}

  if(def_type==SchemaParser::SQL_DEFINITION && prepend_at_bod)
  {
    bkp_prepended_sql=this->prepended_sql;
    this->prepended_sql.clear();
  }

	try
	{
		def=this->BaseObject::__getCodeDefinition(def_type);

		if(def_type==SchemaParser::SQL_DEFINITION && append_at_eod)
			this->appended_sql=bkp_appended_sql;

    if(def_type==SchemaParser::SQL_DEFINITION && prepend_at_bod)
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
      msg=trUtf8("Generating %1 of the object `%2' (%3)"),
      attrib=ParsersAttributes::OBJECTS, attrib_aux,
      def_type_str=(def_type==SchemaParser::SQL_DEFINITION ? QString("SQL") : QString("XML"));
  Type *usr_type=nullptr;
  map<unsigned, BaseObject *> objects_map;
  ObjectType obj_type;

  try
  {
    objects_map=getCreationOrder(def_type);
    general_obj_cnt=this->getObjectCount();
    gen_defs_count=0;

    attribs_aux[ParsersAttributes::SHELL_TYPES]=QString();
    attribs_aux[ParsersAttributes::PERMISSION]=QString();
    attribs_aux[ParsersAttributes::SCHEMA]=QString();
    attribs_aux[ParsersAttributes::TABLESPACE]=QString();
    attribs_aux[ParsersAttributes::ROLE]=QString();

    if(def_type==SchemaParser::SQL_DEFINITION)
    {
      attribs_aux[ParsersAttributes::FUNCTION]=(!functions.empty() ? ParsersAttributes::_TRUE_ : QString());

      for(auto &type : types)
      {
        usr_type=dynamic_cast<Type *>(type);

        if(usr_type->getConfiguration()==Type::BASE_TYPE)
          usr_type->convertFunctionParameters();
      }
    }

    for(auto &obj_itr : objects_map)
    {
      object=obj_itr.second;
      obj_type=object->getObjectType();

      if(obj_type==OBJ_TYPE && def_type==SchemaParser::SQL_DEFINITION)
      {
        usr_type=dynamic_cast<Type *>(object);

        //Generating the shell type declaration (only for base types)
        if(usr_type->getConfiguration()==Type::BASE_TYPE)
          attribs_aux[ParsersAttributes::SHELL_TYPES]+=usr_type->getCodeDefinition(def_type, true);
        else
          attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
      }
      else if(obj_type==OBJ_DATABASE)
      {
        if(def_type==SchemaParser::SQL_DEFINITION)
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
      else if(obj_type==OBJ_PERMISSION)
      {
        attribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permission *>(object)->getCodeDefinition(def_type);
      }
      else if(obj_type==OBJ_CONSTRAINT)
      {
				attribs_aux[attrib]+=dynamic_cast<Constraint *>(object)->getCodeDefinition(def_type, true);
      }
      else if(obj_type==OBJ_ROLE || obj_type==OBJ_TABLESPACE ||  obj_type==OBJ_SCHEMA)
      {
        //The "public" schema does not have the SQL code definition generated
        if(def_type==SchemaParser::SQL_DEFINITION)
          attrib_aux=BaseObject::getSchemaName(obj_type);
        else
          attrib_aux=attrib;

        /* The Tablespace has the SQL code definition disabled when generating the
          code of the entire model because this object cannot be created from a multiline sql command */
        if(obj_type==OBJ_TABLESPACE && !object->isSystemObject() && def_type==SchemaParser::SQL_DEFINITION)
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
        else if((obj_type!=OBJ_SCHEMA && !object->isSystemObject()) ||
                (obj_type==OBJ_SCHEMA &&
                 ((object->getName()==QString("public") && def_type==SchemaParser::XML_DEFINITION) ||
                  (object->getName()!=QString("public") && object->getName()!=QString("pg_catalog")))))
        {
          if(object->getObjectType()==OBJ_SCHEMA)
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

      if((def_type==SchemaParser::SQL_DEFINITION && !object->isSQLDisabled()) ||
         (def_type==SchemaParser::XML_DEFINITION && !object->isSystemObject()))
      {
        emit s_objectLoaded((gen_defs_count/static_cast<unsigned>(general_obj_cnt)) * 100,
                              msg.arg(def_type_str)
                              .arg(object->getName())
                              .arg(object->getTypeName()),
                              object->getObjectType());
      }
    }

    attribs_aux[ParsersAttributes::SEARCH_PATH]=search_path;
    attribs_aux[ParsersAttributes::MODEL_AUTHOR]=author;
    attribs_aux[ParsersAttributes::PGMODELER_VERSION]=GlobalAttributes::PGMODELER_VERSION;

    if(def_type==SchemaParser::XML_DEFINITION)
    {
      attribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? ParsersAttributes::_TRUE_ : QString());
      attribs_aux[ParsersAttributes::LAST_POSITION]=QString("%1,%2").arg(last_pos.x()).arg(last_pos.y());
      attribs_aux[ParsersAttributes::LAST_ZOOM]=QString::number(last_zoom);
      attribs_aux[ParsersAttributes::DEFAULT_SCHEMA]=(default_objs[OBJ_SCHEMA] ? default_objs[OBJ_SCHEMA]->getName(true) : QString());
      attribs_aux[ParsersAttributes::DEFAULT_OWNER]=(default_objs[OBJ_ROLE] ? default_objs[OBJ_ROLE]->getName(true) : QString());
      attribs_aux[ParsersAttributes::DEFAULT_TABLESPACE]=(default_objs[OBJ_TABLESPACE] ? default_objs[OBJ_TABLESPACE]->getName(true) : QString());
      attribs_aux[ParsersAttributes::DEFAULT_COLLATION]=(default_objs[OBJ_COLLATION] ? default_objs[OBJ_COLLATION]->getName(true) : QString());
    }
    else
    {
      for(auto &type : types)
      {
        usr_type=dynamic_cast<Type *>(type);
        if(usr_type->getConfiguration()==Type::BASE_TYPE)
        {
          attribs_aux[attrib]+=usr_type->getCodeDefinition(def_type);
          usr_type->convertFunctionParameters(true);
        }
      }
    }
  }
  catch(Exception &e)
  {
    if(def_type==SchemaParser::SQL_DEFINITION)
    {
      for(auto &type : types)
      {
        usr_type=dynamic_cast<Type *>(type);
        if(usr_type->getConfiguration()==Type::BASE_TYPE)
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

  if(prepend_at_bod && def_type==SchemaParser::SQL_DEFINITION)
    def=QString("-- Prepended SQL commands --\n") +	this->prepended_sql + QString("\n---\n\n") + def;

  if(append_at_eod && def_type==SchemaParser::SQL_DEFINITION)
    def+=QString("-- Appended SQL commands --\n") +	this->appended_sql + QString("\n---\n");

  return(def);
}

map<unsigned, BaseObject *> DatabaseModel::getCreationOrder(unsigned def_type, bool incl_relnn_objs)
{
  BaseObject *object=nullptr;
  vector<BaseObject *> fkeys;
  vector<BaseObject *> fk_rels;
  vector<BaseObject *> *obj_list=nullptr;
  vector<BaseObject *>::iterator itr, itr_end;
  map<unsigned, BaseObject *> objects_map;
  Table *table=nullptr;
  Index *index=nullptr;
  Trigger *trigger=nullptr;
  Constraint *constr=nullptr;
	Rule *rule=nullptr;
	View *view=nullptr;
  Relationship *rel=nullptr;
  ObjectType aux_obj_types[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_TAG },
			obj_types[]={ OBJ_EVENT_TRIGGER, OBJ_COLLATION, OBJ_LANGUAGE, OBJ_FUNCTION, OBJ_TYPE,
                    OBJ_CAST, OBJ_CONVERSION, OBJ_EXTENSION,
                    OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
                    OBJ_AGGREGATE, OBJ_DOMAIN, OBJ_TEXTBOX, BASE_RELATIONSHIP,
                    OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE };
	unsigned i=0, aux_obj_cnt=sizeof(aux_obj_types)/sizeof(ObjectType), count=sizeof(obj_types)/sizeof(ObjectType);

  //The first objects on the map will be roles, tablespaces, schemas and tags
  for(i=0; i < aux_obj_cnt; i++)
  {
    if(aux_obj_types[i]!=OBJ_TAG || def_type==SchemaParser::XML_DEFINITION)
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
    if(def_type==SchemaParser::SQL_DEFINITION &&
       (obj_types[i]==OBJ_TEXTBOX || obj_types[i]==BASE_RELATIONSHIP))
      obj_list=nullptr;
    else
      obj_list=getObjectList(obj_types[i]);

    if(obj_list)
    {
      for(auto &object : (*obj_list))
      {
        /* If the object is a FK relationship it's stored in a separeted list in order to have the
             code generated at end of whole definition (after foreign keys definition) */
        if(object->getObjectType()==BASE_RELATIONSHIP &&
           dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
        {
          fk_rels.push_back(object);
        }
        else
        {
          if(def_type==SchemaParser::XML_DEFINITION || !incl_relnn_objs)
            objects_map[object->getObjectId()]=object;
          else
          {
            rel=dynamic_cast<Relationship *>(object);

            /* Avoiding many-to-many relationships to be included in the map.
             They are treated in a separated way below, because on the diff process (ModelsDiffHelper) the generated table
             need to be compared to other tables not the relationship itself */
            if(!incl_relnn_objs || !rel || (rel && rel->getRelationshipType()!=BaseRelationship::RELATIONSHIP_NN))
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

    count=table->getTriggerCount();
    for(i=0; i < count; i++)
    {
			trigger=table->getTrigger(i);
			objects_map[trigger->getObjectId()]=trigger;
    }

    count=table->getIndexCount();
    for(i=0; i < count; i++)
    {
			index=table->getIndex(i);
			objects_map[index->getObjectId()]=index;
    }

		count=table->getRuleCount();
		for(i=0; i < count; i++)
		{
			rule=table->getRule(i);
			objects_map[rule->getObjectId()]=rule;
		}
  }

	/* Getting and storing the special objects (which reference columns of tables added for relationships)
			on the map of objects. */
  for(auto &obj : views)
	{
		view=dynamic_cast<View *>(obj);

		count=view->getTriggerCount();
		for(i=0; i < count; i++)
		{
			trigger=view->getTrigger(i);
			objects_map[trigger->getObjectId()]=trigger;
		}

		count=view->getRuleCount();
		for(i=0; i < count; i++)
		{
			rule=view->getRule(i);
			objects_map[rule->getObjectId()]=rule;
		}
	}

  /* SPECIAL CASE: Generating the correct order for tables, views, relationships and sequences

     This generation is made in the following way:
     1) Based on the relationship list, participant tables comes before the relationship itself.
     2) Other tables came after the objects on the step 1.
     3) The sequences must have their code generated after the tables
     4) View are the last objects in the list avoiding table/column reference breaking */
  if(def_type==SchemaParser::SQL_DEFINITION)
  {
    BaseObject *objs[3]={nullptr, nullptr, nullptr};
    vector<BaseObject *> vet_aux;

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

      if(object->getObjectType()==OBJ_RELATIONSHIP)
      {
        rel=dynamic_cast<Relationship *>(object);
        objs[0]=rel->getTable(Relationship::SRC_TABLE);
        objs[1]=rel->getTable(Relationship::DST_TABLE);

        /* For many-to-many relationship, the generated table and the foreign keys that represents
           the link are included on the creation order map instead of the relationship itself. This is
           done to permit the table to be accessed and compared on the diff process */
        if(incl_relnn_objs &&
           rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN &&
           rel->getGeneratedTable())
        {
          table=rel->getGeneratedTable();
          objs[2]=table;

          for(BaseObject *tab_obj : *table->getObjectList(OBJ_CONSTRAINT))
          {
            constr=dynamic_cast<Constraint *>(tab_obj);

            if(constr->getConstraintType()==ConstraintType::foreign_key)
              fkeys.push_back(constr);
          }
        }
        else
          objs[2]=rel;

        for(i=0; i < 3; i++)
        {
          if(objects_map.count(objs[i]->getObjectId())==0)
            objects_map[objs[i]->getObjectId()]=objs[i];
        }
      }
      else
      {
        if(objects_map.count(object->getObjectId())==0)
          objects_map[object->getObjectId()]=object;
      }
    }
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
  ObjectType obj_type=BASE_OBJECT;

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
  if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW || obj_type==OBJ_SCHEMA)
  {
    vector<BaseObject *>::iterator end;

    if(obj_type==OBJ_SCHEMA)
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
        if((!constr && child->getObjectType()!=OBJ_COLUMN) ||
            (constr &&
             ((constr->getConstraintType()==ConstraintType::foreign_key) ||
              (constr->getConstraintType()!=ConstraintType::foreign_key &&
               constr->getConstraintType()!=ConstraintType::primary_key &&
               constr->isReferRelationshipAddedColumn()))))
        {
          __getObjectDependencies(child, objs);

          if(constr && constr->getReferencedTable())
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
  if(obj_type==OBJ_SCHEMA)
  {
    if(only_children)
    {
      children=getObjects(object);
      objs.insert(objs.end(), children.begin(), children.end());
    }

    for(BaseObject *obj : relationships)
    {
      rel=dynamic_cast<Relationship *>(obj);

      if(rel->getRelationshipType()==Relationship::RELATIONSHIP_NN &&
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

          if((!constr && child->getObjectType()!=OBJ_COLUMN) ||
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
        for(BaseObject *col : *table->getObjectList(OBJ_COLUMN))
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
		throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_WRITTEN).arg(filename),
										ERR_FILE_DIR_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		buf.append(this->getCodeDefinition(def_type));
		output.write(buf.data(),buf.size());
		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(filename),
										ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			if(obj_type==OBJ_OPCLASS)
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
			else if(obj_type==OBJ_DOMAIN)
			{
				BaseObject *usr_type=getObjectPgSQLType(dynamic_cast<Domain *>(object)->getType());

				if(usr_type)
					getObjectDependecies(usr_type, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for conversion **
			else if(obj_type==OBJ_CONVERSION)
			{
				Function *func=dynamic_cast<Conversion *>(object)->getConversionFunction();
				getObjectDependecies(func, deps, inc_indirect_deps);
			}
			//** Getting the dependecies for cast **
			else if(obj_type==OBJ_CAST)
			{
				Cast *cast=dynamic_cast<Cast *>(object);
				BaseObject *usr_type=nullptr;

				for(unsigned i=Cast::SRC_TYPE; i <= Cast::DST_TYPE; i++)
				{
					usr_type=getObjectPgSQLType(cast->getDataType(i));

					if(usr_type)
						getObjectDependecies(usr_type, deps, inc_indirect_deps);
				}

				getObjectDependecies(cast->getCastFunction(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for event trigger **
			else if(obj_type==OBJ_EVENT_TRIGGER)
			{
				getObjectDependecies(dynamic_cast<EventTrigger *>(object)->getFunction(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for function **
			else if(obj_type==OBJ_FUNCTION)
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
			else if(obj_type==OBJ_AGGREGATE)
			{
				Aggregate *aggreg=dynamic_cast<Aggregate *>(object);
				BaseObject *usr_type=nullptr;
				unsigned count, i;

				for(i=Aggregate::FINAL_FUNC; i <= Aggregate::TRANSITION_FUNC; i++)
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
			else if(obj_type==OBJ_LANGUAGE)
			{
				Language *lang=dynamic_cast<Language *>(object);

				for(unsigned i=Language::VALIDATOR_FUNC; i <= Language::INLINE_FUNC; i++)
				{
					if(lang->getFunction(i))
						getObjectDependecies(lang->getFunction(i), deps, inc_indirect_deps);
				}
			}
			//** Getting the dependecies for operator **
			else if(obj_type==OBJ_OPERATOR)
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
			else if(obj_type==OBJ_ROLE)
			{
				Role *role=dynamic_cast<Role *>(object);
				unsigned i, i1, count,
						role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

				for(i=0; i < 3; i++)
				{
					count=role->getRoleCount(role_types[i]);
					for(i1=0; i1 < count; i1++)
						getObjectDependecies(role->getRole(role_types[i], i1), deps, inc_indirect_deps);
				}
			}
			//** Getting the dependecies for relationships **
			else if(obj_type==OBJ_RELATIONSHIP)
			{
				Relationship *rel=dynamic_cast<Relationship *>(object);
				BaseObject *usr_type=nullptr;
				Constraint *constr=nullptr;
				unsigned i, count;

				getObjectDependecies(rel->getTable(Relationship::SRC_TABLE), deps, inc_indirect_deps);
				getObjectDependecies(rel->getTable(Relationship::DST_TABLE), deps, inc_indirect_deps);

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
			else if(obj_type==OBJ_SEQUENCE)
			{
				Sequence *seq=dynamic_cast<Sequence *>(object);
				if(seq->getOwnerColumn())
					getObjectDependecies(seq->getOwnerColumn()->getParentTable(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for column **
			else if(obj_type==OBJ_COLUMN)
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
			else if(obj_type==OBJ_TRIGGER)
			{
				Trigger *trig=dynamic_cast<Trigger *>(object);

				if(trig->getReferencedTable())
					getObjectDependecies(trig->getReferencedTable(), deps, inc_indirect_deps);

				if(trig->getFunction())
					getObjectDependecies(trig->getFunction(), deps, inc_indirect_deps);
			}
			//** Getting the dependecies for index **
			else if(obj_type==OBJ_INDEX)
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
			//** Getting the dependecies for table **
			else if(obj_type==OBJ_TABLE)
			{
				Table *tab=dynamic_cast<Table *>(object);
				BaseObject *usr_type=nullptr;
				Constraint *constr=nullptr;
				Trigger *trig=nullptr;
				Index *index=nullptr;
				Column *col=nullptr;
				unsigned count, i, count1, i1;

				count=tab->getColumnCount();
				for(i=0; i < count; i++)
				{
					col=tab->getColumn(i);
					usr_type=getObjectPgSQLType(col->getType());

					if(!col->isAddedByLinking() && usr_type)
						getObjectDependecies(usr_type, deps, inc_indirect_deps);
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
			}
			//** Getting the dependecies for user defined type **
			else if(obj_type==OBJ_TYPE)
			{
				Type *usr_type=dynamic_cast<Type *>(object);
				BaseObject *aux_type=nullptr;
				unsigned count, i;

				if(usr_type->getConfiguration()==Type::BASE_TYPE)
				{
					aux_type=getObjectPgSQLType(usr_type->getLikeType());

					if(aux_type)
						getObjectDependecies(aux_type, deps, inc_indirect_deps);

					for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
						getObjectDependecies(usr_type->getFunction(i), deps, inc_indirect_deps);
				}
				else if(usr_type->getConfiguration()==Type::COMPOSITE_TYPE)
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
			else if(obj_type==OBJ_VIEW)
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

		if(obj_type==OBJ_VIEW && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			View *view=dynamic_cast<View *>(object);
			vector<BaseObject *> tab_objs=view->getObjects();
			refs.insert(refs.end(), tab_objs.begin(), tab_objs.end());
		}

		if(obj_type==OBJ_TABLE && (!exclusion_mode || (exclusion_mode && !refer)))
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
			ObjectType tab_obj_types[3]={ OBJ_TRIGGER, OBJ_RULE, OBJ_INDEX };

			for(i=0; i < 3; i++)
			{
				tab_objs=table->getObjectList(tab_obj_types[i]);
				refs.insert(refs.end(), tab_objs->begin(), tab_objs->end());
			}

			itr=relationships.begin();
			itr_end=relationships.end();

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				base_rel=dynamic_cast<BaseRelationship *>(*itr);
				if(base_rel->getTable(BaseRelationship::SRC_TABLE)==table ||
					 base_rel->getTable(BaseRelationship::DST_TABLE)==table)
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
				if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK &&
					 (base_rel->getTable(BaseRelationship::SRC_TABLE)==table ||
						base_rel->getTable(BaseRelationship::DST_TABLE)==table))
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

				if(base_rel->getTable(BaseRelationship::SRC_TABLE)==table ||
					 base_rel->getTable(BaseRelationship::DST_TABLE)==table)
				{
					refer=true;
					refs.push_back(base_rel);
				}
				itr++;
			}
		}

		if(obj_type==OBJ_FUNCTION && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Function *func=dynamic_cast<Function *>(object);
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={OBJ_CAST, OBJ_EVENT_TRIGGER, OBJ_CONVERSION,
															OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_OPCLASS,
															OBJ_TABLE, OBJ_TYPE, OBJ_LANGUAGE };
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

				if(obj_types[i]==OBJ_CAST)
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
				else if(obj_types[i]==OBJ_EVENT_TRIGGER)
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
				else if(obj_types[i]==OBJ_CONVERSION)
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
				else if(obj_types[i]==OBJ_AGGREGATE)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						aggreg=dynamic_cast<Aggregate *>(*itr);

						if(aggreg->getFunction(Aggregate::FINAL_FUNC)==func ||
							 aggreg->getFunction(Aggregate::TRANSITION_FUNC)==func)
						{
							refer=true;
							refs.push_back(aggreg);
						}
						itr++;
					}
				}
				else if(obj_types[i]==OBJ_OPERATOR)
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
				else if(obj_types[i]==OBJ_OPCLASS)
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
				else if(obj_types[i]==OBJ_TABLE)
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
				else if(obj_types[i]==OBJ_TYPE)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						type=dynamic_cast<Type *>(*itr);
						itr++;

						for(i1=Type::INPUT_FUNC; i1 <= Type::ANALYZE_FUNC && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
						{
							if(type->getFunction(i1)==func)
							{
								refer=true;
								refs.push_back(type);
							}
						}
					}
				}
				else if(obj_types[i]==OBJ_LANGUAGE)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						lang=dynamic_cast<Language *>(*itr);
						itr++;

						if(lang->getFunction(Language::HANDLER_FUNC)==func ||
							 lang->getFunction(Language::VALIDATOR_FUNC)==func ||
							 lang->getFunction(Language::INLINE_FUNC)==func)
						{
							refer=true;
							refs.push_back(lang);
						}
					}
				}
			}
		}

		if(obj_type==OBJ_SCHEMA && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[11]={OBJ_FUNCTION, OBJ_TABLE, OBJ_VIEW,
																OBJ_DOMAIN, OBJ_AGGREGATE, OBJ_OPERATOR,
																OBJ_SEQUENCE, OBJ_CONVERSION,
																OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
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

		if((obj_type==OBJ_TYPE || obj_type==OBJ_DOMAIN || obj_type==OBJ_SEQUENCE ||
        obj_type==OBJ_TABLE || obj_type==OBJ_EXTENSION || obj_type==OBJ_VIEW)
			 && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={OBJ_TABLE, OBJ_OPCLASS, OBJ_CAST,
															OBJ_DOMAIN, OBJ_FUNCTION, OBJ_AGGREGATE,
															OBJ_OPERATOR, OBJ_TYPE, OBJ_RELATIONSHIP };
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
				case OBJ_TYPE: ptr_pgsqltype=dynamic_cast<Type*>(object); break;
				case OBJ_DOMAIN: ptr_pgsqltype=dynamic_cast<Domain*>(object); break;
				case OBJ_SEQUENCE: ptr_pgsqltype=dynamic_cast<Sequence*>(object); break;
				case OBJ_EXTENSION: ptr_pgsqltype=dynamic_cast<Extension*>(object); break;
        case OBJ_VIEW: ptr_pgsqltype=dynamic_cast<View*>(object); break;
				default: ptr_pgsqltype=dynamic_cast<Table*>(object); break;
			}

			for(i=0; i < tp_count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				if(obj_types[i]==OBJ_RELATIONSHIP)
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
				else if(obj_types[i]==OBJ_TABLE)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						tab=dynamic_cast<Table *>(*itr);
						itr++;

						count=tab->getColumnCount();
						for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
						{
							col=tab->getColumn(i1);

							if(!col->isAddedByRelationship() && col->getType()==ptr_pgsqltype)
							{
								refer=true;
								refs.push_back(col);
							}
						}
					}
				}
				else if(obj_types[i]==OBJ_OPCLASS)
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
				else if(obj_types[i]==OBJ_DOMAIN)
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
				else if(obj_types[i]==OBJ_TYPE)
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
				else if(obj_types[i]==OBJ_AGGREGATE)
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
				else if(obj_types[i]==OBJ_FUNCTION)
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
				else if(obj_types[i]==OBJ_OPERATOR)
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
				else if(obj_types[i]==OBJ_CAST)
				{
					while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						cast=dynamic_cast<Cast *>(*itr);
						itr++;

						if(cast->getDataType(Cast::SRC_TYPE)==ptr_pgsqltype ||
							 cast->getDataType(Cast::DST_TYPE)==ptr_pgsqltype)
						{
							refer=true;
							refs.push_back(cast);
						}
					}
				}
			}
		}

		if(obj_type==OBJ_ROLE && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[13]={OBJ_FUNCTION, OBJ_TABLE, OBJ_DOMAIN,
																OBJ_AGGREGATE, OBJ_SCHEMA, OBJ_OPERATOR,
																OBJ_SEQUENCE, OBJ_CONVERSION,
																OBJ_LANGUAGE, OBJ_TABLESPACE,
																OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
			unsigned i,i1, count;
			Role *role_aux=nullptr;
			Role *role=dynamic_cast<Role *>(object);
			unsigned role_types[3]={Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE};
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

		if(obj_type==OBJ_TABLESPACE && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==OBJ_LANGUAGE && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==OBJ_OPCLASS && (!exclusion_mode || (exclusion_mode && !refer)))
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

				itr++;
			}
		}

		if(obj_type==OBJ_OPERATOR && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
      ObjectType obj_types[]={OBJ_OPCLASS,
                             OBJ_AGGREGATE,
                              OBJ_OPERATOR,
                              OBJ_TABLE };
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

				if(obj_types[i]==OBJ_OPCLASS)
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
				else if(obj_types[i]==OBJ_AGGREGATE)
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
				else if(obj_types[i]==OBJ_OPERATOR)
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

		if(obj_type==OBJ_OPFAMILY && (!exclusion_mode || (exclusion_mode && !refer)))
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

		if(obj_type==OBJ_COLLATION && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			ObjectType  obj_types[]={ OBJ_DOMAIN, OBJ_COLLATION, OBJ_TYPE },
									tab_obj_types[]={ OBJ_COLUMN, OBJ_INDEX };
			unsigned i, count;
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			vector<TableObject *> *tab_obj_list=nullptr;
			vector<TableObject *>::iterator tab_itr, tab_itr_end;
			TableObject *tab_obj=nullptr;

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
			obj_list=getObjectList(OBJ_TABLE);
			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
				{
					tab_obj_list=dynamic_cast<Table *>(*itr)->getObjectList(tab_obj_types[i]);
					tab_itr=tab_obj_list->begin();
					tab_itr_end=tab_obj_list->end();

					while(tab_itr!=tab_itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
					{
						tab_obj=(*tab_itr);
						if((tab_obj->getObjectType()==OBJ_COLUMN && tab_obj->getCollation()==object) ||
							 (tab_obj->getObjectType()==OBJ_INDEX &&
								dynamic_cast<Index *>(tab_obj)->isReferCollation(dynamic_cast<Collation *>(object))))
						{
							refer=true;
							refs.push_back(*tab_itr);
						}
						tab_itr++;
					}
				}
				itr++;
			}
		}

		if(obj_type==OBJ_COLUMN && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Column *column=dynamic_cast<Column *>(object);
			vector<BaseObject *> *obj_list=nullptr;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType  obj_types[]={ OBJ_SEQUENCE, OBJ_VIEW, OBJ_TABLE, OBJ_RELATIONSHIP };
			unsigned i, count=sizeof(obj_types)/sizeof(ObjectType);

			for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !refer)); i++)
			{
				obj_list=getObjectList(obj_types[i]);
				itr=obj_list->begin();
				itr_end=obj_list->end();

				while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
				{
					if((obj_types[i]==OBJ_SEQUENCE && dynamic_cast<Sequence *>(*itr)->getOwnerColumn()==column) ||
						 (obj_types[i]==OBJ_VIEW && dynamic_cast<View *>(*itr)->isReferencingColumn(column)))
					{
						refer=true;
						refs.push_back(*itr);
					}
					else if(obj_types[i]==OBJ_TABLE)
					{
						Table *tab=dynamic_cast<Table *>(*itr);
            unsigned count, idx, count1, i1;
						Trigger *trig=nullptr;
            Index *index=nullptr;
            Constraint *constr=nullptr;

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
					}
					else if(obj_types[i]==OBJ_RELATIONSHIP)
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


    if(obj_type==OBJ_TAG && (!exclusion_mode || (exclusion_mode && !refer)))
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

    if(obj_type==OBJ_SEQUENCE && (!exclusion_mode || (exclusion_mode && !refer)))
    {
      Table *table=nullptr;
      vector<TableObject *> *cols=nullptr;
      vector<TableObject *>::iterator itr, itr_end;
      unsigned i, cnt=tables.size();

      for(i=0; i < cnt && (!exclusion_mode || (exclusion_mode && !refer)); i++)
      {
        table=dynamic_cast<Table *>(tables[i]);
        cols=table->getObjectList(OBJ_COLUMN);
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

void DatabaseModel::setObjectsModified(vector<ObjectType> types)
{
	ObjectType obj_types[]={OBJ_TABLE, OBJ_VIEW,
													OBJ_RELATIONSHIP, BASE_RELATIONSHIP,
													OBJ_TEXTBOX, OBJ_SCHEMA };
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
        if(obj_types[i]==OBJ_RELATIONSHIP || obj_types[i]==BASE_RELATIONSHIP)
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
		ObjectType tab_obj_types[]={OBJ_COLUMN, OBJ_CONSTRAINT,
																OBJ_TRIGGER, OBJ_RULE, OBJ_INDEX};
		for(unsigned i=0; i < 5; i++)
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
			return(this->getObject(*type, OBJ_TYPE));
		break;

		case UserTypeConfig::DOMAIN_TYPE:
			return(this->getObject(*type, OBJ_DOMAIN));
		break;

		case UserTypeConfig::TABLE_TYPE:
			return(this->getObject(*type, OBJ_TABLE));
		break;

		case UserTypeConfig::VIEW_TYPE:
			return(this->getObject(*type, OBJ_VIEW));
		break;

		case UserTypeConfig::SEQUENCE_TYPE:
			return(this->getObject(*type, OBJ_SEQUENCE));
		break;

		case UserTypeConfig::EXTENSION_TYPE:
			return(this->getObject(*type, OBJ_EXTENSION));
		break;

		default:
			return(nullptr);
		break;
	}
}

void DatabaseModel::validateSchemaRenaming(Schema *schema, const QString &prev_sch_name)
{
	ObjectType types[]={ OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN, OBJ_TYPE, OBJ_SEQUENCE };
	vector<BaseObject *> list, vet;
	BaseObject *obj=nullptr;
	QString prev_name;

	//Raise an error if the schema is not allocated
	if(!schema)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		if(obj->getObjectType()!=OBJ_VIEW)
		{
			//Configures the previous type name
      prev_name=BaseObject::formatName(prev_sch_name) + QString(".") +
								BaseObject::formatName(obj->getName(), false);

			/* Special case for tables. Need to make a dynamic_cast before the reinterpret_cast to get
			the correct reference to table */
			if(obj->getObjectType()==OBJ_TABLE)
				PgSQLType::renameUserType(prev_name, reinterpret_cast<void *>(dynamic_cast<Table *>(obj)), obj->getName(true));
			else
				PgSQLType::renameUserType(prev_name, reinterpret_cast<void *>(obj), obj->getName(true));
		}

		//For graphical objects set them as modified to redraw them
		if(obj->getObjectType()==OBJ_TABLE || obj->getObjectType()==OBJ_VIEW)
			dynamic_cast<BaseGraphicObject *>(obj)->setModified(true);

		list.pop_back();
	}
}

void DatabaseModel::createSystemObjects(bool create_public)
{
	Schema *public_sch=nullptr, *pg_catalog=nullptr;
	Language *lang=nullptr;
	Tablespace *tbspace=nullptr;
	LanguageType lang_types[]={ LanguageType::c, LanguageType::sql, LanguageType::plpgsql };
	Role *postgres=nullptr;
	Collation *collation=nullptr;
	QString collnames[]={ "default", "C", "POSIX" };

  /* The particular case is for public schema that is created only when the flag
	is set. This because the public schema is written on model file even being
	a system object. This strategy permits the user controls the schema rectangle behavior */
  if(create_public && getObjectIndex(QString("public"), OBJ_SCHEMA) < 0)
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
		if(getObjectIndex(~LanguageType(lang_types[i]), OBJ_LANGUAGE) < 0)
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
	postgres->setOption(Role::OP_SUPERUSER, true);
	postgres->setSystemObject(true);
	addRole(postgres);

  setDefaultObject(postgres);
  setDefaultObject(getObject(QString("public"), OBJ_SCHEMA), OBJ_SCHEMA);
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
			tables.insert(tables.end(), getObjectList(OBJ_TABLE)->begin(), getObjectList(OBJ_TABLE)->end());
			inc_tabs=true;
		}

		if(!inc_views && ((*itr_tp)==OBJ_RULE || (*itr_tp)==OBJ_TRIGGER))
		{
			tables.insert(tables.end(), getObjectList(OBJ_VIEW)->begin(), getObjectList(OBJ_VIEW)->end());
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

		if(obj_type==OBJ_DATABASE)
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

				if(tab->getObjectType()==OBJ_TABLE)
					tab_objs=dynamic_cast<Table *>(tab)->getObjectList(obj_type);
				else if(tab->getObjectType()==OBJ_VIEW &&
								 (obj_type==OBJ_TRIGGER || obj_type==OBJ_RULE))
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
		if(format_obj_names)
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
    throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(!object)
    default_objs[obj_type]=nullptr;
  else
    default_objs[object->getObjectType()]=object;
}

bool  DatabaseModel::isAppendAtEOD(void)
{
  return(append_at_eod);
}

bool DatabaseModel::isPrependedAtBOD(void)
{
  return(prepend_at_bod);
}
