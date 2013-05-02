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

#include "databasemodel.h"

unsigned DatabaseModel::dbmodel_id=20000;

DatabaseModel::DatabaseModel(void)
{
	object_id=DatabaseModel::dbmodel_id++;
	obj_type=OBJ_DATABASE;

	encoding=BaseType::null;
	BaseObject::setName(QObject::trUtf8("new_database").toUtf8());

	conn_limit=-1;
	loading_model=false;
	attributes[ParsersAttributes::ENCODING]="";
	attributes[ParsersAttributes::TEMPLATE_DB]="";
	attributes[ParsersAttributes::CONN_LIMIT]="";
	attributes[ParsersAttributes::_LC_COLLATE_]="";
	attributes[ParsersAttributes::_LC_CTYPE_]="";
}

DatabaseModel::~DatabaseModel(void)
{
	this->blockSignals(true);
	destroyObjects();
}

void DatabaseModel::setEncoding(EncodingType encod)
{
	encoding=encod;
}

void DatabaseModel::setLocalization(int localiz_id, const QString &value)
{
	switch(localiz_id)
	{
		case LC_CTYPE:
			localizations[0]=value;
		break;
		case LC_COLLATE:
			localizations[1]=value;
		break;
		default:
			throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}
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
	else
		return(NULL);
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
			else if(obj_type==OBJ_PERMISSION)
				addPermission(dynamic_cast<Permission *>(object));
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
	if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
		 obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX ||
		 obj_type==OBJ_RULE ||
		 obj_type==BASE_OBJECT || obj_type==BASE_RELATIONSHIP ||
		 obj_type==OBJ_DATABASE)
		throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else
	{
		vector<BaseObject *> *obj_list=NULL;
		BaseObject *object=NULL;

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
	vector<BaseObject *> *obj_list=NULL;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!object)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);


	obj_type=object->getObjectType();

	if(obj_type==OBJ_TABLESPACE)
	{
		Tablespace *tabspc=NULL, *aux_tabspc=NULL;

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
												.arg(Utf8String::create(tabspc->getName()))
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
		 (obj_type==OBJ_FUNCTION &&	getObject(dynamic_cast<Function *>(object)->getSignature(), obj_type, idx)) ||
		 (obj_type==OBJ_OPERATOR &&	getObject(dynamic_cast<Operator *>(object)->getSignature(), obj_type, idx)) ||
		 (obj_type!=OBJ_FUNCTION && getObject(object->getName(true), obj_type, idx)))
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

	if(!signalsBlocked())
		emit s_objectAdded(object);
}

void DatabaseModel::__removeObject(BaseObject *object, int obj_idx)
{
	if(!object)
		throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		vector<BaseObject *> *obj_list=NULL;
		ObjectType obj_type;

		obj_type=object->getObjectType();
		obj_list=getObjectList(obj_type);

		if(!obj_list)
			throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			vector<BaseObject *> refs;

			//Get the table references
			getObjectReferences(object, refs, true);

			//If there are objects referencing the table
			if(!refs.empty())
			{
				ErrorType err_type;

				//Raises an error indicating the object that is referencing the table
				if(!dynamic_cast<TableObject *>(refs[0]))
				{
					err_type=ERR_REM_DIRECT_REFERENCE;
					throw Exception(QString(Exception::getErrorMessage(err_type))
													.arg(Utf8String::create(object->getName(true)))
													.arg(Utf8String::create(object->getTypeName()))
													.arg(Utf8String::create(refs[0]->getName(true)))
							.arg(Utf8String::create(refs[0]->getTypeName())),
							err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
				else
				{
					BaseObject *ref_obj_parent=dynamic_cast<TableObject *>(refs[0])->getParentTable();

					err_type=ERR_REM_INDIRECT_REFERENCE;
					throw Exception(QString(Exception::getErrorMessage(err_type))
													.arg(Utf8String::create(object->getName(true)))
													.arg(Utf8String::create(object->getTypeName()))
													.arg(Utf8String::create(refs[0]->getName(true)))
							.arg(Utf8String::create(refs[0]->getTypeName()))
							.arg(Utf8String::create(ref_obj_parent->getName(true)))
							.arg(Utf8String::create(ref_obj_parent->getTypeName())),
							err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			if(obj_idx < 0)
			{
				if(obj_type!=OBJ_FUNCTION && obj_type!=OBJ_OPERATOR)
					getObject(object->getName(true), obj_type, obj_idx);
				else if(obj_type==OBJ_FUNCTION)
					getObject(dynamic_cast<Function *>(object)->getSignature(), obj_type, obj_idx);
				else
					getObject(dynamic_cast<Operator *>(object)->getSignature(), obj_type, obj_idx);
			}

			if(obj_idx >= 0)
			{
				removePermissions(object);
				obj_list->erase(obj_list->begin() + obj_idx);
			}
		}

		object->setDatabase(NULL);

		if(!signalsBlocked())
			emit s_objectRemoved(object);
	}
}

vector<BaseObject *> DatabaseModel::getObjects(ObjectType obj_type, BaseObject *schema)
{
	vector<BaseObject *> *obj_list=NULL, sel_list;
	vector<BaseObject *>::iterator itr, itr_end;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=obj_list->begin();
	itr_end=obj_list->end();

	while(itr!=itr_end)
	{
		if((*itr)->getSchema()==schema)
			sel_list.push_back(*itr);
		itr++;
	}

	return(sel_list);
}

BaseObject *DatabaseModel::getObject(const QString &name, ObjectType obj_type, int &obj_idx)
{
	BaseObject *object=NULL;
	vector<BaseObject *> *obj_list=NULL;
	vector<BaseObject *>::iterator itr, itr_end;
	bool found=false;
	int count;
	QString aux_name, aux_name1;

	obj_list=getObjectList(obj_type);

	if(!obj_list)
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		itr=obj_list->begin();
		itr_end=obj_list->end();
		obj_idx=-1;

		aux_name1=name;
		count=aux_name1.count(QChar('\0'));
		if(count >=1) aux_name1.chop(count);

		if(obj_type!=OBJ_FUNCTION && obj_type!=OBJ_OPERATOR)
		{
			aux_name1=BaseObject::formatName(aux_name1);

			while(itr!=itr_end && !found)
			{
				aux_name=(*itr)->getName(true);
				found=(aux_name==aux_name1);
				if(!found) itr++;
			}
		}
		else
		{
			QString signature;

			while(itr!=itr_end && !found)
			{
				/* Special case for functions/operators: to check duplicity the signature must be
			 compared and not only the name */
				if(obj_type==OBJ_FUNCTION)
					signature=dynamic_cast<Function *>(*itr)->getSignature();
				else
					signature=dynamic_cast<Operator *>(*itr)->getSignature();

				found=(signature==aux_name1);
				if(!found) itr++;
			}
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
	vector<BaseObject *> *obj_list=NULL;

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
	vector<BaseObject *> *obj_list=NULL;

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

QString DatabaseModel::getLocalization(int localiz_id)
{
	switch(localiz_id)
	{
		case LC_CTYPE:
			return(localizations[0]);
		break;
		case LC_COLLATE:
			return(localizations[1]);
		default:
			throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}
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
	vector<BaseObject *> *lista=NULL;
	vector<BaseObject *>::iterator itr, itr_end;
	BaseObject *objeto=NULL;
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
	vector<BaseObject *> *list=NULL;
	BaseObject *object=NULL;
	unsigned i, cnt=sizeof(types)/sizeof(ObjectType);

	disconnectRelationships();

	for(i=0; i < cnt; i++)
	{
		list=getObjectList(types[i]);

		while(!list->empty())
		{
			object=list->back();

			if(object->getObjectType()==OBJ_RELATIONSHIP)
				dynamic_cast<Relationship *>(object)->destroyObjects();

			delete(object);
			list->pop_back();
		}
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
	return(dynamic_cast<Table *>(getObject(obj_idx, OBJ_TABLE)));
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

void DatabaseModel::removeView(View *view, int obj_idx)
{
	try
	{
		__removeObject(view, obj_idx);
		PgSQLType::removeUserType(view->getName(true), view);
		updateViewRelationships(view);
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
	else
	{
		Table *ref_tab=NULL;
		BaseRelationship *rel=NULL;
		Constraint *fk=NULL;
		unsigned idx;
		vector<Constraint *> fks;
		vector<Constraint *>::iterator itr, itr_end;
		vector<BaseObject *>::iterator itr1, itr1_end;

		table->getForeignKeys(fks);
		itr=fks.begin();
		itr_end=fks.end();

		//Case the table is removed the fk relationship must be removed too
		if(!fks.empty() && getObjectIndex(table) < 0)
		{
			while(itr!=itr_end)
			{
				fk=(*itr);
				ref_tab=dynamic_cast<Table *>(fk->getReferencedTable());
				itr++;

				rel=getRelationship(table, ref_tab);

				if(rel)
					removeRelationship(rel);
			}
		}
		//Update the relationships
		else
		{
			/* First remove the invalid relationships (the foreign key that generates the
			relationship no longer exists) */
			itr1=base_relationships.begin();
			itr1_end=base_relationships.end();

			idx=0;
			while(itr1!=itr1_end)
			{
				rel=dynamic_cast<BaseRelationship *>(*itr1);

				if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK &&
					 rel->getTable(BaseRelationship::SRC_TABLE)==table)
				{
					ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));

					//Removes the relationship if the table does'nt references the 'ref_tab'
					if(!table->isReferTableOnForeignKey(ref_tab))
					{
						removeRelationship(rel);
						itr1=base_relationships.begin() + idx;
						itr1_end=base_relationships.end();
					}
					else
					{
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

				if(!rel)
				{
					rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK,
																	 table, ref_tab, false, false);
					addRelationship(rel);
				}
			}
		}
	}
}

void DatabaseModel::updateViewRelationships(View *view)
{
	Table *tab=NULL;
	BaseRelationship *rel=NULL;
	Reference ref;
	unsigned i, ref_count, idx;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!view)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(getObjectIndex(view) < 0)
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
		BaseRelationship *base_rel=NULL;
		Relationship *rel=NULL;
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
	Relationship *rel=NULL;
	BaseRelationship *base_rel=NULL;
	vector<BaseObject *> vet_rel, vet_rel_inv, rels;
	bool found_inval_rel;
	vector<Exception> errors;
	map<unsigned, QString>::iterator itr1, itr1_end;
	map<unsigned, Exception> error_map;
	map<unsigned, Exception>::iterator itr2, itr2_end;
	unsigned idx;
	vector<Schema *> schemas;
	BaseTable *tab1=NULL, *tab2=NULL;

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
				}
				/* Case some error is raised during the connection the relationship is
			 permanently invalidated and need to be removed from the model */
				catch(Exception &e)
				{
					//Removes the relationship
					__removeObject(rel);

					//Removes the iterator that stores the relationship from the list
					rels.erase(itr_ant);

					/* Points the searching to the iterator immediately after the removed iterator
				evicting to walk on the list from the first item */
					itr_end=rels.end();
					itr=rels.begin() + idx;

					//Stores the error raised in a list
					errors.push_back(e);
				}
			}

			itr=rels.begin();

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
			BaseTable *ref_table=NULL, *src_table=NULL;
			Table *recv_table=NULL;
			Relationship *rel_aux=NULL;
			BaseRelationship *base_rel=NULL;
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
						str_aux+=rel_aux->getName() + ", ";

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
	unsigned count, i, type_id;
	vector<BaseObject *>::iterator itr, itr_end;
	Sequence *sequence=NULL;
	Permission *permission=NULL;
	Table *table=NULL;
	TableObject *tab_obj=NULL;
	Constraint *constr=NULL;
	Index *index=NULL;
	Trigger *trigger=NULL;
	View *view=NULL;
	BaseRelationship *rel=NULL;
	Reference ref;
	ObjectType tab_obj_type[3]={ OBJ_CONSTRAINT, OBJ_TRIGGER, OBJ_INDEX };
	bool found=false;

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

		itr=sequences.begin();
		itr_end=sequences.end();

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

		itr=views.begin();
		itr_end=views.end();

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

				removeView(view);
				delete(view);
			}
		}

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
	BaseObject *object=NULL;

	try
	{
		//Restart the XML parser to read the passed xml buffer
		XMLParser::restartParser();
		XMLParser::loadXMLBuffer(xml_def);

		//Identifies the object type through the start element on xml buffer
		obj_type=getObjectType(XMLParser::getElementName());

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
		BaseTable *tab1=NULL, *tab2=NULL;
		QString msg;

		if(rel)
		{
			tab1=rel->getTable(BaseRelationship::SRC_TABLE);
			tab2=rel->getTable(BaseRelationship::DST_TABLE);

			//Raises an error if already exists an relationship between the tables
			if(getRelationship(tab1,tab2))
			{
				msg=Exception::getErrorMessage(ERR_DUPLIC_RELATIONSHIP)
						.arg(tab1->getTypeName())
						.arg(tab1->getName(true))
						.arg(tab2->getTypeName())
						.arg(tab2->getName(true));
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
			if(rel->getObjectType()==OBJ_RELATIONSHIP)
			{
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

BaseRelationship *DatabaseModel::getRelationship(BaseTable *src_tab, BaseTable *dst_tab)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> rel_list;
	BaseRelationship *rel=NULL;
	bool found=false, search_uniq_tab=false;
	BaseTable *tab1=NULL, *tab2=NULL;

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
			{ rel=NULL; itr++; }
		}
	}

	return(rel);
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
							.arg(Utf8String::create(type->getName(true)))
							.arg(type->getTypeName())
							.arg(Utf8String::create(this->getName(true)))
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

void DatabaseModel::addPermissions(vector<Permission *> &perms)
{
	vector<Permission *>::iterator itr=perms.begin(), itr_end=perms.end();

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

		if(getPermissionIndex(perm) >=0)
		{
			throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
											.arg(Utf8String::create(perm->getObject()->getName()))
											.arg(perm->getObject()->getTypeName()),
											ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Raises an error if the permission is referencing an object that does not exists on model
		else if(perm->getObject()!=this &&
						((tab_obj && (getObjectIndex(tab_obj->getParentTable()) < 0)) ||
						 (!tab_obj && (getObjectIndex(perm->getObject()) < 0))))
			throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
											.arg(Utf8String::create(perm->getName()))
											.arg(Utf8String::create(perm->getObject()->getTypeName()))
											.arg(Utf8String::create(perm->getObject()->getName()))
											.arg(Utf8String::create(perm->getObject()->getTypeName())),
											ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		permissions.push_back(perm);
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==ERR_ASG_DUPLIC_OBJECT)
			throw
			Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
								.arg(Utf8String::create(perm->getObject()->getName()))
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
	Permission *perm=NULL;
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
	Permission *perm=NULL;

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

int DatabaseModel::getPermissionIndex(Permission *perm)
{
	int perm_idx=-1;

	if(perm)
	{
		Permission *perm_aux=NULL;
		vector<BaseObject *>::iterator itr, itr_end;
		BaseObject *object=NULL;
		Role *role=NULL;
		unsigned count, i;
		bool ref_role=false;

		itr=permissions.begin();
		itr_end=permissions.end();

		object=perm->getObject();

		while(itr!=itr_end && perm_idx < 0)
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
				perm_idx=itr-permissions.begin();

			itr++;
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
		vector<BaseObject *> *obj_list=NULL;
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

void DatabaseModel::loadModel(const QString &filename)
{
	if(filename!="")
	{
		QString dtd_file, str_aux, elem_name;
		ObjectType obj_type;
		map<QString, QString> attribs;
		BaseObject *object=NULL;
		deque<const xmlNode *> incomplt_objs;
		bool reeval_objs=false, protected_model=false;
		const xmlNode *elem_aux=NULL;
		deque<Exception> errors;
		map<unsigned, QString>::iterator itr, itr_end;

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
			XMLParser::restartParser();

			//Loads the root DTD
			XMLParser::setDTDFile(dtd_file + GlobalAttributes::ROOT_DTD +
														GlobalAttributes::OBJECT_DTD_EXT,
														GlobalAttributes::ROOT_DTD);

			//Loads the file validating it against the root DTD
			XMLParser::loadXMLFile(filename);

			//Gets the basic model information
			XMLParser::getElementAttributes(attribs);

			this->author=attribs[ParsersAttributes::MODEL_AUTHOR];
			protected_model=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

			if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
			{
				do
				{
					if(XMLParser::getElementType()==XML_ELEMENT_NODE)
					{
						elem_name=XMLParser::getElementName();

						/* When the current element is a permission, indicates that the parser created all the
				 other objects. Thus, if there is no incomplete objects that need to be recreated
				 the permissions will be loaded */
						if(elem_name==ParsersAttributes::PERMISSION && incomplt_objs.size()==0)
						{
							//Recreates the special objects before load the permissions
							if(!xml_special_objs.empty())
							{
								//Makes a backup of the main buffer of the xml parser that contains all the model file definition
								str_aux=XMLParser::getXMLBuffer();

								itr=xml_special_objs.begin();
								itr_end=xml_special_objs.end();

								while(itr!=itr_end)
								{
									createSpecialObject(itr->second, itr->first);
									itr++;
								}

								xml_special_objs.clear();


								//Reload the main buffer
								XMLParser::restartParser();
								XMLParser::loadXMLBuffer(str_aux);
								XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

								//Moves the parser to the first permission on the buffer
								while(XMLParser::getElementName()!=ParsersAttributes::PERMISSION &&
											XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
							}

							addPermission(createPermission());
						}
						else
						{
							//Indentifies the object type to be load according to the current element on the parser
							obj_type=getObjectType(elem_name);

							/* This block makes the object re-evaluation, this means, read a code snippet again where,
					after raising an error, an object was referecend before its creation.

					Conditions for object re evaluation:
					* the 'reeval_objs' is not set
					* the incomplete objects list is not empty
					* the object type is not a function, schema, tablespace, language, type or table. */
							if(!reeval_objs && incomplt_objs.size() > 0 &&
								 obj_type!=OBJ_FUNCTION && obj_type!=OBJ_SCHEMA &&
								 obj_type!=OBJ_TABLESPACE && obj_type!=OBJ_LANGUAGE &&
								 obj_type!=OBJ_TYPE && obj_type!=OBJ_TABLE)
							{
								XMLParser::accessElement(XMLParser::PREVIOUS_ELEMENT);
								elem_aux=XMLParser::getCurrentElement();
								reeval_objs=true;

								//Restore the parser at the position of the incomplet object
								XMLParser::restorePosition(incomplt_objs.front());

								//Gets the incomplete object type
								obj_type=getObjectType(XMLParser::getElementName());
							}

							if(obj_type==OBJ_DATABASE)
							{
								XMLParser::getElementAttributes(attribs);
								encoding=attribs[ParsersAttributes::ENCODING];
								template_db=attribs[ParsersAttributes::TEMPLATE_DB];
								localizations[0]=attribs[ParsersAttributes::_LC_CTYPE_];
								localizations[1]=attribs[ParsersAttributes::_LC_COLLATE_];

								if(!attribs[ParsersAttributes::CONN_LIMIT].isEmpty())
									conn_limit=attribs[ParsersAttributes::CONN_LIMIT].toInt();

								setBasicAttributes(this);
							}
							else
							{
								try
								{
									//Saves the current position of the parser before create any object
									XMLParser::savePosition();

									object=createObject(obj_type);

									if(object)
									{
										if(!dynamic_cast<TableObject *>(object) &&
											 obj_type!=OBJ_RELATIONSHIP && obj_type!=BASE_RELATIONSHIP)
										{
											addObject(object);
										}

										if(!signalsBlocked())
										{
											emit s_objectLoaded(XMLParser::getCurrentBufferLine()/XMLParser::getBufferLineCount(),
																					trUtf8("Loading object: %1 (%2)")
																					.arg(Utf8String::create(object->getName()))
																					.arg(object->getTypeName()),
																					obj_type);
										}
									}

									XMLParser::restorePosition();
								}
								catch(Exception &e)
								{
									if(!reeval_objs &&
										 (((e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL && obj_type==OBJ_TABLE)) ||
											(((e.getErrorType()==ERR_ASG_OBJ_INV_DEFINITION ||
												 e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL ||
												 e.getErrorType()==ERR_ASG_INV_TYPE_OBJECT) &&
												(obj_type==OBJ_LANGUAGE || obj_type==OBJ_FUNCTION || obj_type==OBJ_TYPE || obj_type==OBJ_OPERATOR)))))
									{
										XMLParser::restorePosition();
										incomplt_objs.push_back(XMLParser::getCurrentElement());
										errors.push_back(e);
									}
									else
									{
										QString info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);
										throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
									}
								}
							}
						}

						/* If the current process is re-evaluate incomplete objects, removes the object
				 in the front of the incomplete objects because at this point it was
				 successfully recreated */
						if(reeval_objs && incomplt_objs.size() > 0)
						{
							incomplt_objs.pop_front();
							errors.pop_front();

							//If already exists incomplete objects, the parser will try to recreated the next incomplete object
							if(incomplt_objs.size() > 0)
								XMLParser::restorePosition(incomplt_objs.front());
							else
							{
								reeval_objs=false;
								XMLParser::restorePosition(elem_aux);
							}
						}
					}
				}
				while((!reeval_objs && XMLParser::accessElement(XMLParser::NEXT_ELEMENT)) ||
							(reeval_objs));
			}

			if(errors.size() > 0)
				throw Exception(errors[0].getErrorMessage(),errors[0].getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__);

			this->BaseObject::setProtected(protected_model);
			loading_model=false;
			this->validateRelationships();
		}
		catch(Exception &e)
		{
			QString extra_info;
			loading_model=false;
			destroyObjects();

			if(XMLParser::getCurrentElement())
				extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);

			if(e.getErrorType()>=ERR_INVALID_SYNTAX)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_LOAD_INV_MODEL_FILE)).arg(filename);
				throw Exception(str_aux,ERR_LOAD_INV_MODEL_FILE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
			}
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
		}
	}
}

ObjectType DatabaseModel::getObjectType(const QString &type_name)
{
	ObjectType obj_type=BASE_OBJECT;
	int i;

	for(i=0; i < BaseObject::OBJECT_TYPE_COUNT; i++)
	{
		if(objs_schemas[i]==type_name)
		{
			obj_type=static_cast<ObjectType>(i);
			break;
		}
	}

	return(obj_type);
}

BaseObject *DatabaseModel::createObject(ObjectType obj_type)
{
	BaseObject *object=NULL;

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
			object=createConstraint(NULL);
		else if(obj_type==OBJ_TRIGGER)
			object=createTrigger(NULL);
		else if(obj_type==OBJ_INDEX)
			object=createIndex(NULL);
		else if(obj_type==OBJ_RELATIONSHIP ||
						obj_type==BASE_RELATIONSHIP)
			object=createRelationship();
		else if(obj_type==OBJ_COLLATION)
			object=createCollation();
		else if(obj_type==OBJ_EXTENSION)
			object=createExtension();
	}

	return(object);
}

void DatabaseModel::setBasicAttributes(BaseObject *object)
{
	map<QString, QString> attribs, attribs_aux;
	QString elem_name;
	BaseObject *tabspc=NULL, *owner=NULL, *collation=NULL;
	Schema *schema=NULL;
	ObjectType obj_type=BASE_OBJECT, obj_type_aux;
	bool has_error=false, protected_obj=false, sql_disabled=false;

	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	XMLParser::getElementAttributes(attribs);

	obj_type_aux=object->getObjectType();
	if(obj_type_aux!=OBJ_CAST)
		object->setName(attribs[ParsersAttributes::NAME]);

	protected_obj=attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;
	sql_disabled=attribs[ParsersAttributes::SQL_DISABLED]==ParsersAttributes::_TRUE_;

	XMLParser::savePosition();

	if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
	{
		do
		{
			if(XMLParser::getElementType()==XML_ELEMENT_NODE)
			{
				elem_name=XMLParser::getElementName();

				//Defines the object's comment
				if(elem_name==ParsersAttributes::COMMENT)
				{
					XMLParser::savePosition();
					XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
					object->setComment(XMLParser::getElementContent());
					XMLParser::restorePosition();
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::SCHEMA)
				{
					obj_type=OBJ_SCHEMA;
					XMLParser::getElementAttributes(attribs_aux);
					schema=dynamic_cast<Schema *>(getObject(attribs_aux[ParsersAttributes::NAME], obj_type));
					object->setSchema(schema);
					has_error=(!schema && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's tablespace
				else if(elem_name==ParsersAttributes::TABLESPACE)
				{
					obj_type=OBJ_TABLESPACE;
					XMLParser::getElementAttributes(attribs_aux);
					tabspc=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setTablespace(tabspc);
					has_error=(!tabspc && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's owner
				else if(elem_name==ParsersAttributes::ROLE)
				{
					obj_type=OBJ_ROLE;
					XMLParser::getElementAttributes(attribs_aux);
					owner=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setOwner(owner);
					has_error=(!owner && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's schema
				else if(elem_name==ParsersAttributes::COLLATION)
				{
					obj_type=OBJ_COLLATION;
					XMLParser::getElementAttributes(attribs_aux);
					collation=getObject(attribs_aux[ParsersAttributes::NAME], obj_type);
					object->setCollation(collation);
					has_error=(!collation && !attribs_aux[ParsersAttributes::NAME].isEmpty());
				}
				//Defines the object's position (only for graphical objects)
				else if(elem_name==ParsersAttributes::POSITION)
				{
					XMLParser::getElementAttributes(attribs);

					if(elem_name==ParsersAttributes::POSITION &&
						 (obj_type_aux!=OBJ_RELATIONSHIP &&
							obj_type_aux!=BASE_RELATIONSHIP))
					{
						dynamic_cast<BaseGraphicObject *>(object)->setPosition(QPointF(attribs[ParsersAttributes::X_POS].toFloat(),
																																	 attribs[ParsersAttributes::Y_POS].toFloat()));


					}
				}
			}
		}
		while(!has_error && XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
	}

	XMLParser::restorePosition();
	object->setProtected(protected_obj);
	object->setSQLDisabled(sql_disabled);

	if(has_error)
	{
		throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
										.arg(Utf8String::create(object->getName()))
										.arg(object->getTypeName())
										.arg(Utf8String::create(attribs_aux[ParsersAttributes::NAME]))
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
										.arg(Utf8String::create(object->getName()))
										.arg(object->getTypeName()),
										ERR_ALOC_OBJECT_NO_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

Role *DatabaseModel::createRole(void)
{
	map<QString, QString> attribs, attribs_aux;
	Role *role=NULL, *ref_role=NULL;
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
											 Role::OP_LOGIN, Role::OP_ENCRYPTED };

	try
	{
		role=new Role;
		setBasicAttributes(role);

		//Gets all the attributes values from the XML
		XMLParser::getElementAttributes(attribs);

		role->setPassword(attribs[ParsersAttributes::PASSWORD]);
		role->setValidity(attribs[ParsersAttributes::VALIDITY]);

		if(!attribs[ParsersAttributes::CONN_LIMIT].isEmpty())
			role->setConnectionLimit(attribs[ParsersAttributes::CONN_LIMIT].toInt());

		//Setting up the role options according to the configured on the XML
		for(i=0; i < 6; i++)
		{
			marked=attribs[op_attribs[i]]==ParsersAttributes::_TRUE_;
			role->setOption(op_vect[i], marked);
		}

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem_name=XMLParser::getElementName();

					//Getting the member roles
					if(elem_name==ParsersAttributes::ROLES)
					{
						//Gets the member roles attributes
						XMLParser::getElementAttributes(attribs_aux);

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
																.arg(Utf8String::create(role->getName()))
																.arg(BaseObject::getTypeName(OBJ_ROLE))
																.arg(Utf8String::create(list[i]))
																.arg(BaseObject::getTypeName(OBJ_ROLE)),
																ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							role->addRole(role_type, ref_role);
						}
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(role) delete(role);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(role);
}

Tablespace *DatabaseModel::createTablespace(void)
{
	map<QString, QString> attribs;
	Tablespace *tabspc=NULL;

	try
	{
		tabspc=new Tablespace;
		setBasicAttributes(tabspc);
		XMLParser::getElementAttributes(attribs);
		tabspc->setDirectory(attribs[ParsersAttributes::DIRECTORY]);
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(tabspc) delete(tabspc);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(tabspc);
}

Schema *DatabaseModel::createSchema(void)
{
	Schema *schema=NULL;
	map<QString, QString> attribs;

	try
	{
		schema=new Schema;
		XMLParser::getElementAttributes(attribs);
		setBasicAttributes(schema);
		schema->setFillColor(QColor(attribs[ParsersAttributes::FILL_COLOR]));
		schema->setRectVisible(attribs[ParsersAttributes::RECT_VISIBLE]==ParsersAttributes::_TRUE_);
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(schema) delete(schema);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(schema);
}

Language *DatabaseModel::createLanguage(void)
{
	map<QString, QString> attribs;
	Language *lang=NULL;
	BaseObject *func=NULL;
	QString signature, ref_type;
	ObjectType obj_type;

	try
	{
		lang=new Language;
		XMLParser::getElementAttributes(attribs);
		setBasicAttributes(lang);

		lang->setTrusted(attribs[ParsersAttributes::TRUSTED]==ParsersAttributes::_TRUE_);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					obj_type=getObjectType(XMLParser::getElementName());

					if(obj_type==OBJ_FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);

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
																.arg(Utf8String::create(lang->getName()))
																.arg(lang->getTypeName())
																.arg(Utf8String::create(signature))
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
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(lang) delete(lang);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(lang);
}

Function *DatabaseModel::createFunction(void)
{
	map<QString, QString> attribs, attribs_aux;
	Function *func=NULL;
	ObjectType obj_type;
	BaseObject *object=NULL;
	PgSQLType type;
	Parameter param;
	QString str_aux, elem;

	try
	{
		func=new Function;
		setBasicAttributes(func);
		XMLParser::getElementAttributes(attribs);

		if(!attribs[ParsersAttributes::RETURNS_SETOF].isEmpty())
			func->setReturnSetOf(attribs[ParsersAttributes::RETURNS_SETOF]==
					ParsersAttributes::_TRUE_);

		if(!attribs[ParsersAttributes::WINDOW_FUNC].isEmpty())
			func->setWindowFunction(attribs[ParsersAttributes::WINDOW_FUNC]==
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

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();
					obj_type=getObjectType(elem);

					//Gets the function return type from the XML
					if(elem==ParsersAttributes::RETURN_TYPE)
					{
						XMLParser::savePosition();

						try
						{
							XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

							do
							{
								if(XMLParser::getElementType()==XML_ELEMENT_NODE)
								{
									//when the element found is a TYPE indicates that the function return type is a single one
									if(XMLParser::getElementName()==ParsersAttributes::TYPE)
									{
										type=createPgSQLType();
										func->setReturnType(type);
									}
									//when the element found is a PARAMETER indicates that the function return type is a table
									else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
									{
										param=createParameter();
										func->addReturnedTableColumn(param.getName(), param.getType());
									}
								}
							}
							while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

							XMLParser::restorePosition();
						}
						catch(Exception &e)
						{
							XMLParser::restorePosition();
							throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
						}
					}
					//Gets the function language
					else if(obj_type==OBJ_LANGUAGE)
					{
						XMLParser::getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::NAME], obj_type);

						//Raises an error if the function doesn't exisits
						if(!object)
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(func->getName()))
															.arg(func->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
								.arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						func->setLanguage(dynamic_cast<Language *>(object));
					}
					//Gets a function parameter
					else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
					{
						param=createParameter();
						func->addParameter(param);
					}
					//Gets the function code definition
					else if(XMLParser::getElementName()==ParsersAttributes::DEFINITION)
					{
						XMLParser::savePosition();
						XMLParser::getElementAttributes(attribs_aux);

						if(!attribs_aux[ParsersAttributes::LIBRARY].isEmpty())
						{
							func->setLibrary(attribs_aux[ParsersAttributes::LIBRARY]);
							func->setSymbol(attribs_aux[ParsersAttributes::SYMBOL]);
						}
						else if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
							func->setSourceCode(XMLParser::getElementContent());

						XMLParser::restorePosition();
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(func)
		{
			str_aux=func->getName(true);
			delete(func);
		}

		if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
			throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
											.arg(Utf8String::create(str_aux))
											.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
											ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, extra_info);
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(func);
}

Parameter DatabaseModel::createParameter(void)
{
	Parameter param;
	map<QString, QString> attribs;
	QString elem;

	try
	{
		XMLParser::savePosition();
		XMLParser::getElementAttributes(attribs);

		param.setName(attribs[ParsersAttributes::NAME]);
		param.setIn(attribs[ParsersAttributes::PARAM_IN]==ParsersAttributes::_TRUE_);
		param.setOut(attribs[ParsersAttributes::PARAM_OUT]==ParsersAttributes::_TRUE_);
		param.setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						param.setType(createPgSQLType());
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		XMLParser::restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		XMLParser::restorePosition();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(param);
}

TypeAttribute DatabaseModel::createTypeAttribute(void)
{
	TypeAttribute tpattrib;
	map<QString, QString> attribs;
	QString elem;
	BaseObject *collation=NULL;

	try
	{
		XMLParser::savePosition();
		XMLParser::getElementAttributes(attribs);

		tpattrib.setName(attribs[ParsersAttributes::NAME]);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						tpattrib.setType(createPgSQLType());
					}
					else if(elem==ParsersAttributes::COLLATION)
					{
						XMLParser::getElementAttributes(attribs);

						collation=getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
															.arg(Utf8String::create(tpattrib.getName()))
															.arg(Utf8String::create(tpattrib.getTypeName()))
															.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
															.arg(BaseObject::getTypeName(OBJ_COLLATION)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						tpattrib.setCollation(collation);
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		XMLParser::restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		XMLParser::restorePosition();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(tpattrib);
}

PgSQLType DatabaseModel::createPgSQLType(void)
{
	map<QString, QString> attribs;
	unsigned length=1, dimension=0, type_idx=0;
	int precision=-1;
	QString name;
	void *ptype=NULL;
	bool with_timezone;
	IntervalType interv_type;
	SpatialType spatial_type;

	XMLParser::getElementAttributes(attribs);

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

	type_idx=PgSQLType::getBaseTypeIndex(name);
	if(type_idx!=PgSQLType::null)
	{
		return(PgSQLType(name,length,dimension,precision,with_timezone,interv_type, spatial_type));
	}
	else
	{
		//Raises an error if the referenced type name doesn't exists
		if(PgSQLType::getUserTypeIndex(name,NULL,this) == BaseType::null)
			throw Exception(ERR_REF_INEXIST_USER_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		type_idx=PgSQLType::getUserTypeIndex(name, ptype);
		return(PgSQLType(type_idx,length,dimension,precision,with_timezone,interv_type,spatial_type));
	}
}

Type *DatabaseModel::createType(void)
{
	map<QString, QString> attribs;
	map<QString, unsigned> func_types;
	Type *type=NULL;
	int count, i;
	QStringList enums;
	QString elem, str_aux;
	BaseObject *func=NULL, *collation=NULL;
	OperatorClass *op_class=NULL;
	PgSQLType aux_type;

	try
	{
		type=new Type;
		setBasicAttributes(type);
		XMLParser::getElementAttributes(attribs);

		if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE)
		{
			type->setConfiguration(Type::BASE_TYPE);
			type->setByValue(attribs[ParsersAttributes::BY_VALUE]==ParsersAttributes::_TRUE_);

			if(!attribs[ParsersAttributes::INTERNAL_LENGHT].isEmpty())
				type->setInternalLength(attribs[ParsersAttributes::INTERNAL_LENGHT].toUInt());

			if(!attribs[ParsersAttributes::ALIGNMENT].isEmpty())
				type->setAlignment(attribs[ParsersAttributes::ALIGNMENT]);

			if(!attribs[ParsersAttributes::STORAGE].isEmpty())
				type->setStorage(attribs[ParsersAttributes::STORAGE]);

			if(!attribs[ParsersAttributes::ELEMENT].isEmpty())
				type->setElement(attribs[ParsersAttributes::ELEMENT]);

			if(!attribs[ParsersAttributes::DELIMITER].isEmpty())
				type->setDelimiter(attribs[ParsersAttributes::DELIMITER][0].toAscii());

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

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					//Specific operations for ENUM type
					if(elem==ParsersAttributes::ENUM_TYPE)
					{
						XMLParser::getElementAttributes(attribs);
						enums=attribs[ParsersAttributes::VALUES].split(",");

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
						XMLParser::getElementAttributes(attribs);
						collation=getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION);

						//Raises an error if the operator class doesn't exists
						if(!collation)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
															.arg(Utf8String::create(type->getName()))
															.arg(Utf8String::create(type->getTypeName()))
															.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
															.arg(BaseObject::getTypeName(OBJ_COLLATION)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setCollation(collation);
					}
					if(elem==ParsersAttributes::OP_CLASS)
					{
						XMLParser::getElementAttributes(attribs);
						op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], OBJ_OPCLASS));

						//Raises an error if the operator class doesn't exists
						if(!op_class)
						{
							throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
															.arg(Utf8String::create(type->getName()))
															.arg(Utf8String::create(type->getTypeName()))
															.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
															.arg(BaseObject::getTypeName(OBJ_OPCLASS)),
															ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						type->setSubtypeOpClass(op_class);
					}
					//Configuring the functions used by the type (only for BASE type)
					else if(elem==ParsersAttributes::FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);

						//Tries to get the function from the model
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(type->getName()))
															.arg(type->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						//Raises an error if the function type is invalid
						else if(func_types.count(attribs[ParsersAttributes::REF_TYPE])==0)
							throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						type->setFunction(func_types[attribs[ParsersAttributes::REF_TYPE]],	dynamic_cast<Function *>(func));
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(type)
		{
			str_aux=type->getName(true);
			delete(type);
		}

		if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
			throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
											.arg(Utf8String::create(str_aux))
											.arg(type->getTypeName()),
											ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, extra_info);
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(type);
}

Domain *DatabaseModel::createDomain(void)
{
	map<QString, QString> attribs;
	Domain *domain=NULL;
	QString elem;

	try
	{
		domain=new Domain;
		setBasicAttributes(domain);
		XMLParser::getElementAttributes(attribs);

		if(!attribs[ParsersAttributes::CONSTRAINT].isEmpty())
			domain->setConstraintName(attribs[ParsersAttributes::CONSTRAINT]);

		if(!attribs[ParsersAttributes::DEFAULT_VALUE].isEmpty())
			domain->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		domain->setNotNull(attribs[ParsersAttributes::NOT_NULL]==
				ParsersAttributes::_TRUE_);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					//If a type element is found it'll be extracted an type which the domain is applied
					if(elem==ParsersAttributes::TYPE)
					{
						domain->setType(createPgSQLType());
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
						domain->setExpression(XMLParser::getElementContent());
						XMLParser::restorePosition();
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(domain) delete(domain);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(domain);
}

Cast *DatabaseModel::createCast(void)
{
	map<QString, QString> attribs;
	Cast *cast=NULL;
	QString elem;
	unsigned type_idx=0;
	PgSQLType type;
	BaseObject *func=NULL;

	try
	{
		cast=new Cast;
		setBasicAttributes(cast);
		XMLParser::getElementAttributes(attribs);

		if(attribs[ParsersAttributes::CAST_TYPE]==ParsersAttributes::IMPLICIT)
			cast->setCastType(Cast::IMPLICIT);
		else
			cast->setCastType(Cast::ASSIGNMENT);

		cast->setInOut(attribs[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

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
						XMLParser::getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(cast->getName()))
															.arg(cast->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						cast->setCastFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(cast) delete(cast);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(cast);
}

Conversion *DatabaseModel::createConversion(void)
{
	map<QString, QString> attribs;
	Conversion *conv=NULL;
	QString elem;
	BaseObject *func=NULL;

	try
	{
		conv=new Conversion;
		setBasicAttributes(conv);
		XMLParser::getElementAttributes(attribs);

		conv->setEncoding(Conversion::SRC_ENCODING,
											EncodingType(attribs[ParsersAttributes::SRC_ENCODING]));

		conv->setEncoding(Conversion::DST_ENCODING,
											EncodingType(attribs[ParsersAttributes::DST_ENCODING]));

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(conv->getName()))
															.arg(conv->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						conv->setConversionFunction(dynamic_cast<Function *>(func));
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(conv) delete(conv);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(conv);
}

Operator *DatabaseModel::createOperator(void)
{
	map<QString, QString> attribs;
	map<QString, unsigned> func_types;
	map<QString, unsigned> oper_types;
	Operator *oper=NULL;
	QString elem;
	BaseObject *func=NULL,*oper_aux=NULL;
	unsigned arg_type;
	PgSQLType type;

	try
	{
		oper=new Operator;
		setBasicAttributes(oper);
		XMLParser::getElementAttributes(attribs);

		oper->setMerges(attribs[ParsersAttributes::MERGES]==ParsersAttributes::_TRUE_);
		oper->setHashes(attribs[ParsersAttributes::HASHES]==ParsersAttributes::_TRUE_);

		func_types[ParsersAttributes::OPERATOR_FUNC]=Operator::FUNC_OPERATOR;
		func_types[ParsersAttributes::JOIN_FUNC]=Operator::FUNC_JOIN;
		func_types[ParsersAttributes::RESTRICTION_FUNC]=Operator::FUNC_RESTRICT;

		oper_types[ParsersAttributes::COMMUTATOR_OP]=Operator::OPER_COMMUTATOR;
		oper_types[ParsersAttributes::NEGATOR_OP]=Operator::OPER_NEGATOR;

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==objs_schemas[OBJ_OPERATOR])
					{
						XMLParser::getElementAttributes(attribs);
						oper_aux=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_OPERATOR);

						//Raises an error if the auxiliary operator doesn't exists
						if(!oper_aux && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(oper->getSignature(true)))
															.arg(oper->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
								.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setOperator(dynamic_cast<Operator *>(oper_aux),
															oper_types[attribs[ParsersAttributes::REF_TYPE]]);
					}
					else if(elem==ParsersAttributes::TYPE)
					{
						XMLParser::getElementAttributes(attribs);

						if(attribs[ParsersAttributes::REF_TYPE]!=ParsersAttributes::RIGHT_TYPE)
							arg_type=Operator::LEFT_ARG;
						else
							arg_type=Operator::RIGHT_ARG;

						type=createPgSQLType();
						oper->setArgumentType(type, arg_type);
					}
					else if(elem==ParsersAttributes::FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(oper->getName()))
															.arg(oper->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
								.arg(BaseObject::getTypeName(OBJ_FUNCTION)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						oper->setFunction(dynamic_cast<Function *>(func),
															func_types[attribs[ParsersAttributes::REF_TYPE]]);
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(oper) delete(oper);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(oper);
}

OperatorClass *DatabaseModel::createOperatorClass(void)
{
	map<QString, QString> attribs, attribs_aux;
	map<QString, unsigned> elem_types;
	BaseObject *object=NULL;
	QString elem;
	PgSQLType type;
	OperatorClass *op_class=NULL;
	OperatorClassElement class_elem;
	bool for_order_by;
	unsigned stg_number, elem_type;

	try
	{
		op_class=new OperatorClass;
		setBasicAttributes(op_class);
		XMLParser::getElementAttributes(attribs);

		op_class->setIndexingType(IndexingType(attribs[ParsersAttributes::INDEX_TYPE]));
		op_class->setDefault(attribs[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

		elem_types[ParsersAttributes::FUNCTION]=OperatorClassElement::FUNCTION_ELEM;
		elem_types[ParsersAttributes::OPERATOR]=OperatorClassElement::OPERATOR_ELEM;
		elem_types[ParsersAttributes::STORAGE]=OperatorClassElement::STORAGE_ELEM;

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==objs_schemas[OBJ_OPFAMILY])
					{
						XMLParser::getElementAttributes(attribs);
						object=getObject(attribs[ParsersAttributes::NAME], OBJ_OPFAMILY);

						//Raises an error if the operator family doesn't exists
						if(!object && !attribs[ParsersAttributes::NAME].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(op_class->getName()))
															.arg(op_class->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
								.arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
								ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

						op_class->setFamily(dynamic_cast<OperatorFamily *>(object));
					}
					else if(elem==ParsersAttributes::TYPE)
					{
						XMLParser::getElementAttributes(attribs);
						type=createPgSQLType();
						op_class->setDataType(type);
					}
					else if(elem==ParsersAttributes::ELEMENT)
					{
						XMLParser::getElementAttributes(attribs);

						for_order_by=attribs[ParsersAttributes::FOR_ORDER_BY]==ParsersAttributes::_TRUE_;
						stg_number=attribs[ParsersAttributes::STRATEGY_NUM].toUInt();
						elem_type=elem_types[attribs[ParsersAttributes::TYPE]];

						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
						XMLParser::getElementAttributes(attribs);

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

							if(XMLParser::hasElement(XMLParser::NEXT_ELEMENT))
							{
								XMLParser::savePosition();
								XMLParser::accessElement(XMLParser::NEXT_ELEMENT);
								XMLParser::getElementAttributes(attribs_aux);

								object=getObject(attribs_aux[ParsersAttributes::NAME],OBJ_OPFAMILY);

								if(!object && !attribs_aux[ParsersAttributes::NAME].isEmpty())
									throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
																	.arg(Utf8String::create(op_class->getName()))
																	.arg(op_class->getTypeName())
																	.arg(Utf8String::create(attribs_aux[ParsersAttributes::NAME]))
																	.arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
										ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

								class_elem.setOperatorFamily(dynamic_cast<OperatorFamily *>(object), for_order_by);
								XMLParser::restorePosition();
							}
						}

						op_class->addElement(class_elem);
						XMLParser::restorePosition();
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(op_class) delete(op_class);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(op_class);
}

OperatorFamily *DatabaseModel::createOperatorFamily(void)
{
	map<QString, QString> attribs;
	OperatorFamily *op_family=NULL;

	try
	{
		op_family=new OperatorFamily;
		setBasicAttributes(op_family);
		XMLParser::getElementAttributes(attribs);
		op_family->setIndexingType(IndexingType(attribs[ParsersAttributes::INDEX_TYPE]));
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(op_family) delete(op_family);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(op_family);
}

Aggregate *DatabaseModel::createAggregate(void)
{
	map<QString, QString> attribs;
	BaseObject *func=NULL;
	QString elem;
	PgSQLType type;
	Aggregate *aggreg=NULL;

	try
	{
		aggreg=new Aggregate;
		setBasicAttributes(aggreg);
		XMLParser::getElementAttributes(attribs);

		aggreg->setInitialCondition(attribs[ParsersAttributes::INITIAL_COND]);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						XMLParser::getElementAttributes(attribs);
						type=createPgSQLType();

						if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::STATE_TYPE)
							aggreg->setStateType(type);
						else
							aggreg->addDataType(type);
					}
					else if(elem==ParsersAttributes::FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists on the model
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
							throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
															.arg(Utf8String::create(aggreg->getName()))
															.arg(aggreg->getTypeName())
															.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
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
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(aggreg) delete(aggreg);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(aggreg);
}

Table *DatabaseModel::createTable(void)
{
	map<QString, QString> attribs;
	QString elem;
	Table *table=NULL;
	TableObject *object=NULL;

	try
	{
		table=new Table;
		setBasicAttributes(table);
		XMLParser::getElementAttributes(attribs);

		table->setWithOIDs(attribs[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();
					XMLParser::savePosition();
					object=NULL;

					if(elem==BaseObject::objs_schemas[OBJ_COLUMN])
						object=createColumn();
					else if(elem==BaseObject::objs_schemas[OBJ_CONSTRAINT])
						object=createConstraint(table);
					else if(elem==BaseObject::objs_schemas[OBJ_INDEX])
						object=createIndex(table);
					else if(elem==BaseObject::objs_schemas[OBJ_RULE])
						object=createRule();
					else if(elem==BaseObject::objs_schemas[OBJ_TRIGGER])
						object=createTrigger(table);

					if(object)
						table->addObject(object);

					XMLParser::restorePosition();
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		table->setProtected(table->isProtected());
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		XMLParser::restorePosition();

		if(table) delete(table);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(table);
}

Column *DatabaseModel::createColumn(void)
{
	map<QString, QString> attribs;
	Column *column=NULL;
	QString elem;

	try
	{
		column=new Column;
		setBasicAttributes(column);

		XMLParser::getElementAttributes(attribs);
		column->setNotNull(attribs[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);
		column->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::TYPE)
					{
						column->setType(createPgSQLType());
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(column) delete(column);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(column);
}

Constraint *DatabaseModel::createConstraint(BaseObject *parent_obj)
{
	map<QString, QString> attribs;
	Constraint *constr=NULL;
	BaseObject *ref_table=NULL;
	Table *table=NULL,*table_aux=NULL;
	Column *column=NULL;
	Relationship *rel=NULL;
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
		XMLParser::getElementAttributes(attribs);

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
								.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
						.arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
						.arg(Utf8String::create(attribs[ParsersAttributes::TABLE]))
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
											.arg(Utf8String::create(constr->getName())),
											ERR_INV_PRIM_KEY_ALOCATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(constr_type==ConstraintType::foreign_key)
		{
			deferrable=(attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
			constr->setDeferrable(deferrable);

			if(deferrable && !attribs[ParsersAttributes::DEFER_TYPE].isEmpty())
				constr->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

			if(!attribs[ParsersAttributes::COMPARISON_TYPE].isEmpty())
				constr->setMatchType(attribs[ParsersAttributes::COMPARISON_TYPE]);

			if(!attribs[ParsersAttributes::DEL_ACTION].isEmpty())
				constr->setActionType(attribs[ParsersAttributes::DEL_ACTION], false);

			if(!attribs[ParsersAttributes::UPD_ACTION].isEmpty())
				constr->setActionType(attribs[ParsersAttributes::UPD_ACTION], true);

			ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_TABLE);

			if(!ref_table && table->getName(true)==attribs[ParsersAttributes::REF_TABLE])
				ref_table=table;

			//Raises an error if the referenced table doesn't exists
			if(!ref_table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
								.arg(Utf8String::create(constr->getName()))
								.arg(constr->getTypeName())
								.arg(Utf8String::create(attribs[ParsersAttributes::REF_TABLE]))
						.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			constr->setReferencedTable(dynamic_cast<BaseTable *>(ref_table));
		}
		else if(constr_type==ConstraintType::check)
		{
			constr->setNoInherit(attribs[ParsersAttributes::NO_INHERIT]==ParsersAttributes::_TRUE_);
		}


		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::EXCLUDE_ELEMENT)
					{
						createElement(exc_elem, constr, parent_obj);
						constr->addExcludeElement(exc_elem);
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

						constr->setCheckExpression(XMLParser::getElementContent());

						XMLParser::restorePosition();
					}
					else if(elem==ParsersAttributes::COLUMNS)
					{
						XMLParser::getElementAttributes(attribs);

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
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
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
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(constr) delete(constr);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(constr);
}

void DatabaseModel::createElement(Element &elem, TableObject *tab_obj, BaseObject *parent_obj)
{
	map<QString, QString> attribs;
	Column *column=NULL;
	OperatorClass *op_class=NULL;
	Operator *oper=NULL;
	Collation *collation=NULL;
	QString xml_elem;

	xml_elem=XMLParser::getElementName();

	if(xml_elem==ParsersAttributes::INDEX_ELEMENT || xml_elem==ParsersAttributes::EXCLUDE_ELEMENT)
	{
		XMLParser::getElementAttributes(attribs);

		elem.setSortingAttribute(Element::ASC_ORDER, attribs[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);
		elem.setSortingAttribute(Element::NULLS_FIRST, attribs[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
		elem.setSortingEnabled(attribs[ParsersAttributes::USE_SORTING]!=ParsersAttributes::_FALSE_);

		XMLParser::savePosition();
		XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

		do
		{
			xml_elem=XMLParser::getElementName();

			if(XMLParser::getElementType()==XML_ELEMENT_NODE)
			{
				if(xml_elem==ParsersAttributes::OP_CLASS)
				{
					XMLParser::getElementAttributes(attribs);
					op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], OBJ_OPCLASS));

					//Raises an error if the operator class doesn't exists
					if(!op_class)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
														.arg(Utf8String::create(tab_obj->getName()))
														.arg(Utf8String::create(tab_obj->getTypeName()))
														.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
														.arg(BaseObject::getTypeName(OBJ_OPCLASS)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setOperatorClass(op_class);
				}
				//Checking if elem is a ExcludeElement to be able to assign an operator to it
				else if(xml_elem==ParsersAttributes::OPERATOR && dynamic_cast<ExcludeElement *>(&elem))
				{
					XMLParser::getElementAttributes(attribs);
					oper=dynamic_cast<Operator *>(getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_OPERATOR));

					//Raises an error if the operator doesn't exists
					if(!oper)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
														.arg(Utf8String::create(tab_obj->getName()))
														.arg(Utf8String::create(tab_obj->getTypeName()))
														.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
														.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					dynamic_cast<ExcludeElement &>(elem).setOperator(oper);
				}
				else if(xml_elem==ParsersAttributes::COLLATION && dynamic_cast<IndexElement *>(&elem))
				{
					XMLParser::getElementAttributes(attribs);
					collation=dynamic_cast<Collation *>(getObject(attribs[ParsersAttributes::NAME], OBJ_COLLATION));

					//Raises an error if the operator class doesn't exists
					if(!collation)
					{
						throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
														.arg(Utf8String::create(tab_obj->getName()))
														.arg(Utf8String::create(tab_obj->getTypeName()))
														.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
														.arg(BaseObject::getTypeName(OBJ_COLLATION)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					dynamic_cast<IndexElement &>(elem).setCollation(collation);
				}
				else if(xml_elem==ParsersAttributes::COLUMN)
				{
					XMLParser::getElementAttributes(attribs);

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
														.arg(Utf8String::create(tab_obj->getName()))
														.arg(Utf8String::create(tab_obj->getTypeName()))
														.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
														.arg(BaseObject::getTypeName(OBJ_COLUMN)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					elem.setColumn(column);
				}
				else if(xml_elem==ParsersAttributes::EXPRESSION)
				{
					XMLParser::savePosition();
					XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
					elem.setExpression(XMLParser::getElementContent());
					XMLParser::restorePosition();
				}
			}
		}
		while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

		XMLParser::restorePosition();
	}
}

Index *DatabaseModel::createIndex(Table *table)
{
	map<QString, QString> attribs;
	Index *index=NULL;
	QString elem, str_aux;
	bool inc_idx_table=false;
	IndexElement idx_elem;

	try
	{
		XMLParser::getElementAttributes(attribs);

		if(!table)
		{
			inc_idx_table=true;
			table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

			//Raises an error if the parent table doesn't exists
			if(!table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
								.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
						.arg(BaseObject::getTypeName(OBJ_INDEX))
						.arg(Utf8String::create(attribs[ParsersAttributes::TABLE]))
						.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		index=new Index;
		setBasicAttributes(index);
		index->setParentTable(table);
		index->setIndexAttribute(Index::CONCURRENT, attribs[ParsersAttributes::CONCURRENT]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::UNIQUE, attribs[ParsersAttributes::UNIQUE]==ParsersAttributes::_TRUE_);
		index->setIndexAttribute(Index::FAST_UPDATE, attribs[ParsersAttributes::FAST_UPDATE]==ParsersAttributes::_TRUE_);

		index->setIndexingType(attribs[ParsersAttributes::INDEX_TYPE]);
		index->setFillFactor(attribs[ParsersAttributes::FACTOR].toUInt());

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::INDEX_ELEMENT)
					{
						createElement(idx_elem, index, table);
						index->addIndexElement(idx_elem);
					}
					else if(elem==ParsersAttributes::CONDITION)
					{
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
						str_aux=XMLParser::getElementContent();
						XMLParser::restorePosition();
						index->setConditionalExpression(str_aux);
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		if(inc_idx_table)
		{
			table->addIndex(index);
			table->setModified(true);
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(index) delete(index);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(index);
}

Rule *DatabaseModel::createRule(void)
{
	map<QString, QString> attribs;
	QStringList cmd_list;
	Rule *rule=NULL;
	QString elem, str_aux;
	int count, i;

	try
	{
		rule=new Rule;
		setBasicAttributes(rule);

		XMLParser::getElementAttributes(attribs);
		rule->setExecutionType(attribs[ParsersAttributes::EXEC_TYPE]);
		rule->setEventType(attribs[ParsersAttributes::EVENT_TYPE]);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::COMMANDS ||
						 elem==ParsersAttributes::CONDITION)
					{
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

						str_aux=XMLParser::getElementContent();
						XMLParser::restorePosition();

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
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(rule) delete(rule);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(rule);
}

Trigger *DatabaseModel::createTrigger(BaseTable *table)
{
	map<QString, QString> attribs;
	Trigger *trigger=NULL;
	QString elem, str_aux;
	QStringList list_aux;
	int count, i;
	BaseObject *ref_table=NULL, *func=NULL;
	Column *column=NULL;
	bool inc_trig_table=false;

	try
	{
		XMLParser::getElementAttributes(attribs);

		if(!table && attribs[ParsersAttributes::TABLE].isEmpty())
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!table && !attribs[ParsersAttributes::TABLE].isEmpty())
		{
			inc_trig_table=true;
			table=dynamic_cast<BaseTable *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

			if(!table)
				throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
												.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
					.arg(BaseObject::getTypeName(OBJ_TRIGGER))
					.arg(Utf8String::create(attribs[ParsersAttributes::TABLE]))
					.arg(BaseObject::getTypeName(OBJ_TABLE)),
					ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

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
												.arg(Utf8String::create(trigger->getName()))
												.arg(Utf8String::create(trigger->getTypeName()))
												.arg(Utf8String::create(attribs[ParsersAttributes::REF_TABLE]))
												.arg(BaseObject::getTypeName(OBJ_TABLE)),
												ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			trigger->setReferecendTable(dynamic_cast<BaseTable *>(ref_table));
		}

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::FUNCTION)
					{
						XMLParser::getElementAttributes(attribs);
						func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

						//Raises an error if the function doesn't exists
						if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
						{
							str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
											.arg(Utf8String::create(trigger->getName()))
											.arg(trigger->getTypeName())
											.arg(Utf8String::create(attribs[ParsersAttributes::SIGNATURE]))
									.arg(BaseObject::getTypeName(OBJ_FUNCTION));

							throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						trigger->setFunction(dynamic_cast<Function *>(func));
					}
					else if(elem==ParsersAttributes::CONDITION)
					{
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
						str_aux=XMLParser::getElementContent();
						XMLParser::restorePosition();
						trigger->setCondition(str_aux);
					}
					else if(elem==ParsersAttributes::COLUMNS)
					{
						XMLParser::getElementAttributes(attribs);

						list_aux=attribs[ParsersAttributes::NAMES].split(',');
						count=list_aux.count();

						for(i=0; i < count; i++)
						{
							column=dynamic_cast<Column *>(table->getObject(list_aux[i], OBJ_COLUMN));

							if(!column && dynamic_cast<Table *>(table))
								column=dynamic_cast<Table *>(table)->getColumn(list_aux[i], true);

							trigger->addColumn(column);
						}
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		if(inc_trig_table)
		{
			table->addObject(trigger);
			table->setModified(true);
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(trigger) delete(trigger);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(trigger);
}

Sequence *DatabaseModel::createSequence(bool ignore_onwer)
{
	map<QString, QString> attribs;
	Sequence *sequence=NULL;
	BaseObject *table=NULL;
	Column *column=NULL;
	QString str_aux, tab_name, col_name;
	QStringList elem_list;
	int count;

	try
	{
		sequence=new Sequence;
		setBasicAttributes(sequence);
		XMLParser::getElementAttributes(attribs);

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
				tab_name=elem_list[0] + "." + elem_list[1];
				col_name=elem_list[2];
			}
			else
			{
				tab_name=elem_list[0];
				col_name=elem_list[1];
			}

			table=getObject(tab_name, OBJ_TABLE);

			//Raises an error if the column parent table doesn't exists
			if(!table)
			{
				str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
								.arg(Utf8String::create(sequence->getName()))
								.arg(BaseObject::getTypeName(OBJ_SEQUENCE))
								.arg(Utf8String::create(tab_name))
								.arg(BaseObject::getTypeName(OBJ_TABLE));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column=dynamic_cast<Table *>(table)->getColumn(col_name);

			if(!column)
				column=dynamic_cast<Table *>(table)->getColumn(col_name, true);

			//Raises an error if the column doesn't exists
			if(!column && !ignore_onwer)
				throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
												.arg(Utf8String::create(sequence->getName(true))),
												ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			sequence->setOwnerColumn(column);
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(sequence) delete(sequence);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(sequence);
}

View *DatabaseModel::createView(void)
{
	map<QString, QString> attribs;
	View *view=NULL;
	Column *column=NULL;
	Table *table=NULL;
	QString elem, str_aux;
	QStringList list_aux;
	vector<Reference> refs;
	unsigned type;
	int ref_idx, i, count;
	bool refs_in_expr=false;

	try
	{
		view=new View;
		setBasicAttributes(view);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::REFERENCE)
					{
						XMLParser::getElementAttributes(attribs);

						//If the table name is specified tries to create a reference to a table/column
						if(!attribs[ParsersAttributes::TABLE].isEmpty())
						{
							column=NULL;
							table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

							//Raises an error if the table doesn't exists
							if(!table)
							{
								str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
												.arg(Utf8String::create(view->getName()))
												.arg(BaseObject::getTypeName(OBJ_VIEW))
												.arg(Utf8String::create(attribs[ParsersAttributes::TABLE]))
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
													.arg(Utf8String::create(view->getName()))
													.arg(BaseObject::getTypeName(OBJ_VIEW))
													.arg(Utf8String::create(attribs[ParsersAttributes::TABLE]) + "." +
													Utf8String::create(attribs[ParsersAttributes::COLUMN]))
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
							XMLParser::savePosition();
							str_aux=attribs[ParsersAttributes::ALIAS];

							XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
							XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
							refs.push_back(Reference(XMLParser::getElementContent(),str_aux));

							XMLParser::restorePosition();
						}
					}
					else if(elem==ParsersAttributes::EXPRESSION)
					{
						XMLParser::savePosition();
						XMLParser::getElementAttributes(attribs);
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

						if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CTE_EXPRESSION)
							view->setCommomTableExpression(XMLParser::getElementContent());
						else
						{
							if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
								type=Reference::SQL_REFER_SELECT;
							else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
								type=Reference::SQL_REFER_FROM;
							else
								type=Reference::SQL_REFER_WHERE;


							list_aux=XMLParser::getElementContent().split(',');
							count=list_aux.size();

							//Indicates that some of the references were used in the expressions
							if(cout > 0 && !refs_in_expr)
								refs_in_expr=true;

							for(i=0; i < count; i++)
							{
								ref_idx=list_aux[i].toInt();
								view->addReference(refs[ref_idx],type);
							}
						}

						XMLParser::restorePosition();
					}
					else if(elem==BaseObject::getSchemaName(OBJ_RULE))
						view->addRule(createRule());
					else if(elem==BaseObject::getSchemaName(OBJ_TRIGGER))
						view->addTrigger(createTrigger(view));
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		/** Special case for refereces used as view definition **

			If the flag 'refs_in_expr' isn't set indicates that the user defined a reference
			but no used to define the view declaration, this way pgModeler will consider these
			references as View definition expressions and will force the insertion them as
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
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(view) delete(view);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(view);
}

Collation *DatabaseModel::createCollation(void)
{
	Collation *collation=NULL;
	BaseObject *copy_coll=NULL;
	EncodingType encoding;
	map<QString, QString> attribs;

	try
	{
		collation=new Collation;
		setBasicAttributes(collation);

		XMLParser::getElementAttributes(attribs);

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
												.arg(Utf8String::create(collation->getName()))
												.arg(BaseObject::getTypeName(OBJ_COLLATION))
												.arg(Utf8String::create(attribs[ParsersAttributes::COLLATION]))
												.arg(BaseObject::getTypeName(OBJ_COLLATION)),
							ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			collation->setCollation(dynamic_cast<Collation *>(copy_coll));
		}
		//Creating a collation using LC_COLLATE and LC_CTYPE params
		else
		{
			collation->setLocalization(LC_COLLATE, attribs[ParsersAttributes::_LC_COLLATE_]);
			collation->setLocalization(LC_CTYPE, attribs[ParsersAttributes::_LC_CTYPE_]);
		}
	}
	catch(Exception &e)
	{
		QString extra_info;

		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
									 .arg(XMLParser::getCurrentElement()->line);

		if(collation) delete(collation);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(collation);
}

Extension *DatabaseModel::createExtension(void)
{
	Extension *extension=NULL;
	map<QString, QString> attribs;

	try
	{
		extension=new Extension;
		XMLParser::getElementAttributes(attribs);
		setBasicAttributes(extension);

		extension->setHandlesType(attribs[ParsersAttributes::HANDLES_TYPE]==ParsersAttributes::_TRUE_);
		extension->setVersion(Extension::CUR_VERSION, attribs[ParsersAttributes::CUR_VERSION]);
		extension->setVersion(Extension::OLD_VERSION, attribs[ParsersAttributes::OLD_VERSION]);
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(extension) delete(extension);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(extension);
}

Textbox *DatabaseModel::createTextbox(void)
{
	Textbox *txtbox=NULL;
	map<QString, QString> attribs;

	try
	{
		txtbox=new Textbox;
		setBasicAttributes(txtbox);

		XMLParser::getElementAttributes(attribs);

		if(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::ITALIC_TXT, true);

		if(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::BOLD_TXT, true);

		if(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_)
			txtbox->setTextAttribute(Textbox::UNDERLINE_TXT, true);

		if(!attribs[ParsersAttributes::COLOR].isEmpty())
			txtbox->setTextColor(QColor(attribs[ParsersAttributes::COLOR]));
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
									 .arg(XMLParser::getCurrentElement()->line);

		if(txtbox) delete(txtbox);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	return(txtbox);
}

BaseRelationship *DatabaseModel::createRelationship(void)
{
	vector<unsigned> cols_special_pk;
	map<QString, QString> attribs;
	BaseRelationship *base_rel=NULL;
	Relationship *rel=NULL;
	BaseTable *tables[2]={NULL, NULL};
	bool src_mand, dst_mand, identifier, protect, deferrable, auto_suffix;
	DeferralType defer_type;
	unsigned rel_type=0, i;
	ObjectType table_types[2]={OBJ_VIEW, OBJ_TABLE}, obj_rel_type;
	QString str_aux, elem,
			tab_attribs[2]={ ParsersAttributes::SRC_TABLE,
											 ParsersAttributes::DST_TABLE };

	try
	{
		XMLParser::getElementAttributes(attribs);
		protect=(attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

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
								.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
						.arg(BaseObject::getTypeName(obj_rel_type))
						.arg(Utf8String::create(attribs[tab_attribs[i]]))
						.arg(BaseObject::getTypeName(table_types[i]));

				throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATION_TAB_VIEW)
		{
			base_rel=getRelationship(tables[0], tables[1]);

			if(!base_rel)
				throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
												.arg(Utf8String::create(this->getName()))
												.arg(this->getTypeName())
												.arg(Utf8String::create(attribs[ParsersAttributes::NAME]))
					.arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
					ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			base_rel->disconnectRelationship();
			base_rel->setName(attribs[ParsersAttributes::NAME]);
		}
		else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
		{
			base_rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK,
																		tables[0], tables[1], false, false);
			base_rel->setName(attribs[ParsersAttributes::NAME]);
		}
		else
		{
			src_mand=attribs[ParsersAttributes::SRC_REQUIRED]==ParsersAttributes::_TRUE_;
			dst_mand=attribs[ParsersAttributes::DST_REQUIRED]==ParsersAttributes::_TRUE_;
			identifier=attribs[ParsersAttributes::IDENTIFIER]==ParsersAttributes::_TRUE_;
			deferrable=attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_;
			auto_suffix=(!attribs[ParsersAttributes::AUTO_SUFFIX].isEmpty() &&
									attribs[ParsersAttributes::AUTO_SUFFIX]==ParsersAttributes::_TRUE_);
			defer_type=DeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

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
					auto_suffix, attribs[ParsersAttributes::SRC_SUFFIX],
					attribs[ParsersAttributes::DST_SUFFIX],
					identifier, deferrable, defer_type,
					CopyOptions(attribs[ParsersAttributes::COPY_MODE].toUInt(),
											attribs[ParsersAttributes::COPY_OPTIONS].toUInt()));

			if(!attribs[ParsersAttributes::TABLE_NAME].isEmpty())
				rel->setTableNameRelNN(attribs[ParsersAttributes::TABLE_NAME]);

			rel->setName(attribs[ParsersAttributes::NAME]);
			base_rel=rel;
		}

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::COLUMN && rel)
					{
						XMLParser::savePosition();
						rel->addObject(createColumn());
						XMLParser::restorePosition();
					}
					else if(elem==ParsersAttributes::CONSTRAINT && rel)
					{
						XMLParser::savePosition();
						rel->addObject(createConstraint(rel));
						XMLParser::restorePosition();
					}
					else if(elem==ParsersAttributes::LINE)
					{
						vector<QPointF> points;
						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

						do
						{
							XMLParser::getElementAttributes(attribs);
							points.push_back(QPointF(attribs[ParsersAttributes::X_POS].toFloat(),
															 attribs[ParsersAttributes::Y_POS].toFloat()));
						}
						while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

						base_rel->setPoints(points);
						XMLParser::restorePosition();
					}
					else if(elem==ParsersAttributes::LABEL)
					{
						XMLParser::getElementAttributes(attribs);
						str_aux=attribs[ParsersAttributes::REF_TYPE];

						XMLParser::savePosition();
						XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
						XMLParser::getElementAttributes(attribs);
						XMLParser::restorePosition();
					}
					else if(elem==ParsersAttributes::SPECIAL_PK_COLS && rel)
					{
						QList<QString> col_list;

						XMLParser::getElementAttributes(attribs);
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
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);

		if(base_rel && base_rel->getObjectType()==OBJ_RELATIONSHIP)
			delete(base_rel);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
	}

	if(rel)
	{
		storeSpecialObjectsXML();
		addRelationship(rel);
	}
	else if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
		addRelationship(base_rel);

	base_rel->setProtected(protect);

	if(base_rel && base_rel->getObjectType()==BASE_RELATIONSHIP)
		base_rel->connectRelationship();

	return(base_rel);
}

Permission *DatabaseModel::createPermission(void)
{
	Permission *perm=NULL;
	BaseObject *object=NULL;
	Table *parent_table=NULL;
	Role *role=NULL;
	map<QString, QString> priv_attribs, attribs;
	map<QString, QString>::iterator itr, itr_end;
	ObjectType obj_type;
	QString parent_name, obj_name;
	QStringList list;
	unsigned i, len, priv_type=Permission::PRIV_SELECT;
	bool priv_value, grant_op, revoke, cascade;

	try
	{
		XMLParser::getElementAttributes(priv_attribs);
		revoke=priv_attribs[ParsersAttributes::REVOKE]==ParsersAttributes::_TRUE_;
		cascade=priv_attribs[ParsersAttributes::CASCADE]==ParsersAttributes::_TRUE_;

		XMLParser::savePosition();
		XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
		XMLParser::getElementAttributes(attribs);

		obj_type=getObjectType(attribs[ParsersAttributes::TYPE]);
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
											.arg(Utf8String::create(obj_name))
											.arg(BaseObject::getTypeName(obj_type)),
											ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		perm=new Permission(object);
		perm->setRevoke(revoke);
		perm->setCascade(cascade);

		do
		{
			if(XMLParser::getElementName()==ParsersAttributes::ROLES)
			{
				XMLParser::getElementAttributes(attribs);

				list=attribs[ParsersAttributes::NAMES].split(',');
				len=list.size();

				for(i=0; i < len; i++)
				{
					role=dynamic_cast<Role *>(getObject(list[i].trimmed(),OBJ_ROLE));

					//Raises an error if the referenced role doesn't exists
					if(!role)
					{
						//Dispara a exceção
						throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
														.arg(Utf8String::create(object->getName()))
														.arg(object->getTypeName())
														.arg(Utf8String::create(list[i]))
														.arg(BaseObject::getTypeName(OBJ_ROLE)),
														ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					}

					perm->addRole(role);
				}
			}
			else if(XMLParser::getElementName()==ParsersAttributes::PRIVILEGES)
			{
				XMLParser::getElementAttributes(priv_attribs);

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
		while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

		XMLParser::restorePosition();
	}
	catch(Exception &e)
	{
		QString extra_info;
		extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
							 .arg(XMLParser::getCurrentElement()->line);
		if(perm) delete(perm);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
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
											.arg(Utf8String::create(column->getParentTable()->getName(true)) + "." + Utf8String::create(column->getName(true)))
											.arg(column->getTypeName())
											.arg(Utf8String::create(refs[0]->getName(true)))
				.arg(refs[0]->getTypeName()),
				ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

void DatabaseModel::validateRelationships(TableObject *object, Table *parent_tab)
{
	try
	{
		bool revalidate_rels=false, ref_tab_inheritance=false;
		Relationship *rel=NULL;
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
	if(conn_limit >= 0)
		attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(conn_limit);

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		if(encoding!=BaseType::null)
			attributes[ParsersAttributes::ENCODING]="'" + (~encoding) + "'";

		if(!localizations[1].isEmpty())
			attributes[ParsersAttributes::_LC_COLLATE_]="'" + localizations[1] + "'";

		if(!localizations[0].isEmpty())
			attributes[ParsersAttributes::_LC_CTYPE_]="'" + localizations[0]  + "'";
	}
	else
	{
		attributes[ParsersAttributes::ENCODING]=(~encoding);
		attributes[ParsersAttributes::_LC_COLLATE_]=localizations[1];
		attributes[ParsersAttributes::_LC_CTYPE_]=localizations[0];
	}

	attributes[ParsersAttributes::TEMPLATE_DB]=template_db;
	return(this->BaseObject::__getCodeDefinition(def_type));
}

QString DatabaseModel::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, true));
}

QString DatabaseModel::getCodeDefinition(unsigned def_type, bool export_file)
{
	map<QString, QString> attribs_aux;
	unsigned count1, i, count;
	float general_obj_cnt, gen_defs_count;
	BaseObject *object=NULL;
	vector<BaseObject *> *obj_list=NULL;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<unsigned>::iterator itr1, itr1_end;
	QString msg=trUtf8("Generating %1 of the object: %2 (%3)"),
			attrib=ParsersAttributes::OBJECTS,
			def_type_str=(def_type==SchemaParser::SQL_DEFINITION ? "SQL" : "XML");
	Type *usr_type=NULL;
	map<unsigned, BaseObject *> objects_map;
	vector<unsigned> ids_objs, ids_tab_objs;
	vector<Constraint *> fks;
	Table *table=NULL;
	Index *index=NULL;
	Trigger *trigger=NULL;
	Constraint *constr=NULL;
	Relationship *rel=NULL;
	ObjectType obj_type,
			aux_obj_types[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA },
			obj_types[]={ OBJ_COLLATION, OBJ_LANGUAGE, OBJ_FUNCTION, OBJ_TYPE,
										OBJ_CAST, OBJ_CONVERSION,
										OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
										OBJ_AGGREGATE, OBJ_DOMAIN, OBJ_TEXTBOX, BASE_RELATIONSHIP,
										OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE };
	try
	{
		general_obj_cnt=this->getObjectCount();
		gen_defs_count=0;

		/* Treating the objects which have fixed ids, they are: Paper, table space,
		 and Schema. They need to be treated separately in the loop down because they do not
		 enter in the id sorting performed for other types of objects. */
		for(i=0; i < 3; i++)
		{
			obj_list=getObjectList(aux_obj_types[i]);

			if(def_type==SchemaParser::SQL_DEFINITION)
			{
				attrib=BaseObject::objs_schemas[aux_obj_types[i]];
				attribs_aux[attrib]="";
			}

			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end)
			{
				object=(*itr);

				//The "public" schema does not have the SQL code definition generated
				if(object->getObjectType()!=OBJ_SCHEMA ||
					 (object->getObjectType()==OBJ_SCHEMA && object->getName()!="public") ||
					 (object->getObjectType()==OBJ_SCHEMA && object->getName()=="public" && def_type==SchemaParser::XML_DEFINITION))
				{
					//Generates the code definition and concatenates to the others
					attribs_aux[attrib]+=object->getCodeDefinition(def_type);

					//Increments the generated definition count and emits the signal
					gen_defs_count++;
					if(!signalsBlocked())
					{
						emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
																msg.arg(def_type_str)
																.arg(Utf8String::create(object->getName()))
																.arg(object->getTypeName()),
																object->getObjectType());
					}
				}
				itr++;
			}
		}

		//Includes the database model on the objects map permitting to create the code in a correct order
		objects_map[this->getObjectId()]=this;
		ids_objs.push_back(this->getObjectId());

		//Generating the definition for the other objects type
		if(def_type==SchemaParser::XML_DEFINITION)
			count=17;
		else
			count=13;

		for(i=0; i < count; i++)
		{
			//For SQL definition, only the textbox and base relationship does not has the code generated
			if(def_type==SchemaParser::SQL_DEFINITION &&
				 (obj_types[i]==OBJ_TEXTBOX || obj_types[i]==BASE_RELATIONSHIP))
				obj_list=NULL;
			else
				obj_list=getObjectList(obj_types[i]);

			if(obj_list)
			{
				itr=obj_list->begin();
				itr_end=obj_list->end();

				while(itr!=itr_end)
				{
					object=(*itr);
					objects_map[object->getObjectId()]=object;
					ids_objs.push_back(object->getObjectId());
					itr++;
				}
			}
		}

		/* Getting and storing the special objects (which reference columns of tables added for relationships)
			on the map of objects and their ids in an array of auxiliary ids 'ids_tab_objs'.
			For this list of tables is scanned again and its constraints and indexes are validated as special or not.
			The vector of ids is concatenated to the main vector of ids before his ordination when the definition is XML
			or concatenated after ordination to SQL definition, so the special objects are created correctly in both languages */
		itr=tables.begin();
		itr_end=tables.end();

		while(itr!=itr_end)
		{
			table=dynamic_cast<Table *>(*itr);
			itr++;

			count=table->getConstraintCount();
			for(i=0; i < count; i++)
			{
				constr=table->getConstraint(i);

				/* Case the constraint is a special object stores it on the objects map. Independently to the
				configuration, foreign keys are discarded in this iteration because on the end of the method
				they have the definition generated */
				if((def_type==SchemaParser::XML_DEFINITION ||
						(def_type==SchemaParser::SQL_DEFINITION &&
						 constr->getConstraintType()!=ConstraintType::foreign_key)) &&

					 (!constr->isAddedByLinking() &&
						((constr->getConstraintType()!=ConstraintType::primary_key && constr->isReferRelationshipAddedColumn()) ||
						 (constr->getConstraintType()==ConstraintType::foreign_key))))
				{
					objects_map[constr->getObjectId()]=constr;
					ids_tab_objs.push_back(constr->getObjectId());
				}
			}

			count=table->getTriggerCount();
			for(i=0; i < count; i++)
			{
				trigger=table->getTrigger(i);

				if(trigger->isReferRelationshipAddedColumn())
				{
					objects_map[trigger->getObjectId()]=trigger;
					ids_tab_objs.push_back(trigger->getObjectId());
				}
			}

			count=table->getIndexCount();
			for(i=0; i < count; i++)
			{
				index=table->getIndex(i);

				if(index->isReferRelationshipAddedColumn())
				{
					objects_map[index->getObjectId()]=index;
					ids_tab_objs.push_back(index->getObjectId());
				}
			}
		}

		if(def_type==SchemaParser::XML_DEFINITION)
			ids_objs.insert(ids_objs.end(), ids_tab_objs.begin(), ids_tab_objs.end());

		//Sort the objects id vector to created the definition in a correct way
		sort(ids_objs.begin(), ids_objs.end());

		/* SPECIAL CASE: Generating the SQL for tables, views, relationships and sequences

		 This generations is made in the following way:
		 1) Based on the relationship list, generates the SQL for the participant tables and after this the
				SQL for the relationship itself.
		 2) Generates the SQL for the other tables (that does not participates in relationships)
		 3) The sequences must have its code generated after the tables
		 4) View are the last objects that has the code generated avoiding table/column reference breaking */
		if(def_type==SchemaParser::SQL_DEFINITION)
		{
			BaseObject *objs[3]={NULL, NULL, NULL};
			vector<BaseObject *> vet_aux;
			//count=17;

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

				//Stores the table's user added foreign keys
				if(object->getObjectType()==OBJ_TABLE)
					dynamic_cast<Table *>(object)->getForeignKeys(fks);

				if(object->getObjectType()==OBJ_RELATIONSHIP)
				{
					rel=dynamic_cast<Relationship *>(object);
					objs[0]=rel->getTable(Relationship::SRC_TABLE);
					objs[1]=rel->getTable(Relationship::DST_TABLE);
					objs[2]=rel;

					for(i=0; i < 3; i++)
					{
						if(objects_map.count(objs[i]->getObjectId())==0)
						{
							objects_map[objs[i]->getObjectId()]=objs[i];
							ids_objs.push_back(objs[i]->getObjectId());
						}
					}
				}
				else
				{
					if(objects_map.count(object->getObjectId())==0)
					{
						objects_map[object->getObjectId()]=object;
						ids_objs.push_back(object->getObjectId());
					}
				}
			}
		}

		if(def_type==SchemaParser::SQL_DEFINITION)
			ids_objs.insert(ids_objs.end(), ids_tab_objs.begin(), ids_tab_objs.end());

		attribs_aux[ParsersAttributes::SHELL_TYPES]="";

		if(def_type==SchemaParser::SQL_DEFINITION)
		{
			itr=types.begin();
			itr_end=types.end();

			while(itr!=itr_end)
			{
				usr_type=dynamic_cast<Type *>(*itr);
				itr++;

				if(usr_type->getConfiguration()==Type::BASE_TYPE)
					usr_type->convertFunctionParameters();
			}
		}

		itr1=ids_objs.begin();
		itr1_end=ids_objs.end();

		attrib=ParsersAttributes::OBJECTS;
		while(itr1!=itr1_end)
		{
			object=objects_map[(*itr1)];
			obj_type=object->getObjectType();
			itr1++;

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
					attribs_aux[this->getSchemaName()]+=this->__getCodeDefinition(def_type);
				else
					attribs_aux[attrib]+=this->__getCodeDefinition(def_type);
			}
			else if(obj_type==OBJ_CONSTRAINT)
			{
				attribs_aux[attrib]+=dynamic_cast<Constraint *>(object)->getCodeDefinition(def_type, true);
			}
			else
			{
				if(object->isSystemObject())
					attribs_aux[attrib]+="";
				else
					attribs_aux[attrib]+=object->getCodeDefinition(def_type);
			}

			gen_defs_count++;
			if(!signalsBlocked())
			{
				emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
														msg.arg(def_type_str)
														.arg(Utf8String::create(object->getName()))
														.arg(object->getTypeName()),
														object->getObjectType());
			}
		}

		//Creates the code definition for user added foreign keys
		while(!fks.empty())
		{
			attribs_aux[attrib]+=fks.back()->getCodeDefinition(def_type, true);
			fks.pop_back();
		}

		//Gernerating the SQL/XML code for permissions
		itr=permissions.begin();
		itr_end=permissions.end();
		attribs_aux[ParsersAttributes::PERMISSION]="";

		while(itr!=itr_end)
		{
			attribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permission *>(*itr)->getCodeDefinition(def_type);

			gen_defs_count++;
			if(!signalsBlocked())
			{
				emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
														msg.arg(def_type_str)
														.arg(Utf8String::create((*itr)->getName()))
														.arg(object->getTypeName()),
														object->getObjectType());
			}

			itr++;
		}

		attribs_aux[ParsersAttributes::MODEL_AUTHOR]=author;

		if(def_type==SchemaParser::XML_DEFINITION)
		{
			attribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? "1" : "");
		}
		else
		{
			count1=types.size();
			for(i=0; i < count1; i++)
			{
				usr_type=dynamic_cast<Type *>(types[i]);
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
			count1=types.size();
			for(i=0; i < count1; i++)
			{
				usr_type=dynamic_cast<Type *>(types[i]);
				if(usr_type->getConfiguration()==Type::BASE_TYPE)
					usr_type->convertFunctionParameters(true);
			}
		}
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	attribs_aux[ParsersAttributes::EXPORT_TO_FILE]=(export_file ? "1" : "");

	return(SchemaParser::getCodeDefinition(ParsersAttributes::DB_MODEL, attribs_aux, def_type));
}

void DatabaseModel::saveModel(const QString &filename, unsigned def_type)
{
	QString str_aux;
	QFile output(filename);

	output.open(QFile::WriteOnly);

	if(!output.isOpen())
	{
		str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(filename));
		throw Exception(str_aux,ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	try
	{
		str_aux=this->getCodeDefinition(def_type);
		output.write(str_aux.toStdString().c_str(),str_aux.size());
		output.close();
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(filename));
		throw Exception(str_aux,ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			if(object->getSchema() /*&& inc_indirect_deps*/)
				getObjectDependecies(object->getSchema(), deps, inc_indirect_deps);

			if(object->getTablespace() /*&& inc_indirect_deps*/)
				getObjectDependecies(object->getTablespace(), deps, inc_indirect_deps);

			if(object->getOwner() /*&& inc_indirect_deps*/)
				getObjectDependecies(object->getOwner(), deps, inc_indirect_deps);

			if(object->getCollation() /*&& inc_indirect_deps*/)
				getObjectDependecies(object->getCollation(), deps, inc_indirect_deps);

			//** Getting the dependecies for operator class **
			if(obj_type==OBJ_OPCLASS)
			{
				OperatorClass *op_class=dynamic_cast<OperatorClass *>(object);
				BaseObject *usr_type=getObjectPgSQLType(op_class->getDataType());

				if(usr_type)
					getObjectDependecies(usr_type, deps, inc_indirect_deps);

				if(op_class->getFamily())
					getObjectDependecies(op_class->getFamily(), deps, inc_indirect_deps);
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
				BaseObject *usr_type=NULL;

				for(unsigned i=Cast::SRC_TYPE; i <= Cast::DST_TYPE; i++)
				{
					usr_type=getObjectPgSQLType(cast->getDataType(i));

					if(usr_type)
						getObjectDependecies(usr_type, deps, inc_indirect_deps);
				}

				getObjectDependecies(cast->getCastFunction(), deps, inc_indirect_deps);
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
				BaseObject *usr_type=NULL;
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
				BaseObject *usr_type=NULL;
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
				BaseObject *usr_type=NULL;
				Constraint *constr=NULL;
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
			//** Getting the dependecies for table **
			else if(obj_type==OBJ_TABLE)
			{
				Table *tab=dynamic_cast<Table *>(object);
				BaseObject *usr_type=NULL;
				Constraint *constr=NULL;
				Trigger *trig=NULL;
				Index *index=NULL;
				Column *col=NULL;
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
				BaseObject *aux_type=NULL;
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

void DatabaseModel::getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode)
{
	refs.clear();

	if(object)
	{
		vector<BaseObject *>::iterator itr_perm, itr_perm_end;
		ObjectType obj_type=object->getObjectType();
		bool refer=false;
		Permission *perm=NULL;

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

		if(obj_type==OBJ_TABLE && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Table *table=dynamic_cast<Table *>(object);
			Sequence *seq=NULL;
			Constraint *constr=NULL;
			Table *tab=NULL;
			Trigger *gat=NULL;
			BaseRelationship *base_rel=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			unsigned i, count;

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
					if(constr->getConstraintType()==ConstraintType::foreign_key &&
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


			itr=base_relationships.begin();
			itr_end=base_relationships.end();

			while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
			{
				base_rel=dynamic_cast<BaseRelationship *>(*itr);
				if(base_rel->getTable(BaseRelationship::SRC_TABLE)==table)
				{
					refer=true;
					refs.push_back(base_rel->getTable(BaseRelationship::DST_TABLE));
				}
				else if(base_rel->getTable(BaseRelationship::DST_TABLE)==table)
				{
					refer=true;
					refs.push_back(base_rel->getTable(BaseRelationship::SRC_TABLE));
				}
				itr++;
			}
		}

		if(obj_type==OBJ_FUNCTION && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			Function *func=dynamic_cast<Function *>(object);
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[7]={OBJ_CAST, OBJ_CONVERSION,
															 OBJ_AGGREGATE, OBJ_OPERATOR,
															 OBJ_TABLE, OBJ_TYPE, OBJ_LANGUAGE };
			unsigned i, i1, count;
			Table *tab=NULL;
			Aggregate *aggreg=NULL;
			Operator *oper=NULL;
			Trigger *trig=NULL;
			Type *type=NULL;
			Language *lang=NULL;

			for(i=0; i < 7 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
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
			vector<BaseObject *> *obj_list=NULL;
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
				obj_type==OBJ_TABLE || obj_type==OBJ_EXTENSION)
			 && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[]={OBJ_TABLE, OBJ_OPCLASS, OBJ_CAST,
															OBJ_DOMAIN, OBJ_FUNCTION, OBJ_AGGREGATE,
															OBJ_OPERATOR, OBJ_TYPE, OBJ_RELATIONSHIP };
			unsigned i, i1, count, tp_count = sizeof(obj_types)/sizeof(ObjectType);
			OperatorClass *op_class=NULL;
			Table *tab=NULL;
			Column *col=NULL;
			Cast *cast=NULL;
			Domain *dom=NULL;
			Function *func=NULL;
			Aggregate *aggreg=NULL;
			Operator *oper=NULL;
			Type *type=NULL;
			Relationship *rel=NULL;
			void *ptr_pgsqltype=NULL;

			switch(obj_type)
			{
				case OBJ_TYPE: ptr_pgsqltype=dynamic_cast<Type*>(object); break;
				case OBJ_DOMAIN: ptr_pgsqltype=dynamic_cast<Domain*>(object); break;
				case OBJ_SEQUENCE: ptr_pgsqltype=dynamic_cast<Sequence*>(object); break;
				case OBJ_EXTENSION: ptr_pgsqltype=dynamic_cast<Extension*>(object); break;
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

							if(col->getType()==object)
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

							if(!col->isAddedByRelationship() && col->getType()==object)
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
							 type->getLikeType()==ptr_pgsqltype)
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
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[13]={OBJ_FUNCTION, OBJ_TABLE, OBJ_DOMAIN,
																OBJ_AGGREGATE, OBJ_SCHEMA, OBJ_OPERATOR,
																OBJ_SEQUENCE, OBJ_CONVERSION,
																OBJ_LANGUAGE, OBJ_TABLESPACE,
																OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
			unsigned i,i1, count;
			Role *role_aux=NULL;
			Role *role=dynamic_cast<Role *>(object);
			unsigned role_types[3]={Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE};

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
			Table *tab=NULL;
			Index *ind=NULL;
			Constraint *rest=NULL;

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
			Function *func=NULL;

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

		if(obj_type==OBJ_OPERATOR && (!exclusion_mode || (exclusion_mode && !refer)))
		{
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType obj_types[3]={OBJ_OPCLASS,
															 OBJ_AGGREGATE,
															 OBJ_OPERATOR};
			unsigned i, i1, count;
			OperatorClass *op_class=NULL;
			Operator *oper_aux=NULL, *oper=dynamic_cast<Operator *>(object);

			for(i=0; i < 3 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
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
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			vector<TableObject *> *tab_obj_list=NULL;
			vector<TableObject *>::iterator tab_itr, tab_itr_end;
			TableObject *tab_obj=NULL;

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
			vector<BaseObject *> *obj_list=NULL;
			vector<BaseObject *>::iterator itr, itr_end;
			ObjectType  obj_types[]={ OBJ_SEQUENCE, OBJ_VIEW, OBJ_TABLE, OBJ_RELATIONSHIP };
			unsigned i, count=4;

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
						unsigned trig_cnt, constr_cnt, idx, count1, i1;
						Trigger *trig=NULL;

						constr_cnt=tab->getConstraintCount();
						for(idx=0; idx < constr_cnt && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
						{
							/*if(tab->getConstraint(idx)->isColumnExists(column, Constraint::SOURCE_COLS) ||
								 tab->getConstraint(idx)->isColumnExists(column, Constraint::REFERENCED_COLS))*/
							if(tab->getConstraint(idx)->isColumnReferenced(column))
							{
								refer=true;
								refs.push_back(tab->getConstraint(idx));
							}

						}

						trig_cnt=tab->getTriggerCount();
						for(idx=0; idx < trig_cnt && (!exclusion_mode || (exclusion_mode && !refer)); idx++)
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
							/* if(rel->getConstraint(idx)->isColumnExists(column, Constraint::SOURCE_COLS) ||
								 rel->getConstraint(idx)->isColumnExists(column, Constraint::REFERENCED_COLS)) */
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
	}
}

void DatabaseModel::setObjectsModified(void)
{
	ObjectType obj_types[]={OBJ_TABLE, OBJ_VIEW,
													OBJ_RELATIONSHIP, BASE_RELATIONSHIP,
													OBJ_TEXTBOX, OBJ_SCHEMA };
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> *obj_list=NULL;
	Textbox *label=NULL;
	BaseRelationship *rel=NULL;
	unsigned i, i1;

	for(i=0; i < 6; i++)
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
			return(NULL);
		break;
	}
}

void DatabaseModel::validateSchemaRenaming(Schema *schema, const QString &prev_sch_name)
{
	ObjectType types[]={ OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN, OBJ_TYPE, OBJ_SEQUENCE };
	vector<BaseObject *> list, vet;
	BaseObject *obj=NULL;
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
			prev_name=BaseObject::formatName(prev_sch_name) + "." +
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
	Schema *public_sch=NULL;
	Language *lang=NULL;
	LanguageType lang_types[]={ LanguageType::c, LanguageType::sql, LanguageType::plpgsql };


	/* The particular case is for public schema that is created only when the flag
	is set. This because the public schema is written on model file even being
	a system object. This strategy permits the user controls the schema rectangle behavior */
	if(create_public && getObjectIndex("public", OBJ_SCHEMA) < 0)
	{
		public_sch=new Schema;
		public_sch->setName("public");
		public_sch->setSystemObject(true);
		addObject(public_sch);
	}

	for(unsigned i=0; i < sizeof(lang_types)/sizeof(LanguageType); i++)
	{
		if(getObjectIndex(~LanguageType(lang_types[i]), OBJ_LANGUAGE) < 0)
		{
			lang=new Language;
			lang->BaseObject::setName(~LanguageType(lang_types[i]));
			lang->setSystemObject(true);
			addObject(lang);
		}
	}
}
