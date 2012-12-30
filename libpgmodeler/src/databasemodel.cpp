#include "databasemodel.h"

unsigned DatabaseModel::dbmodel_id=20000;

DatabaseModel::DatabaseModel(void)
{
 object_id=DatabaseModel::dbmodel_id++;

 obj_type=OBJ_DATABASE;
 BaseObject::setName(QObject::trUtf8("new_database").toUtf8());

 conn_limit=-1;
 loading_model=false;
 attributes[ParsersAttributes::ENCODING]="";
 attributes[ParsersAttributes::TEMPLATE_DB]="";
 attributes[ParsersAttributes::CONN_LIMIT]="";
 attributes[ParsersAttributes::LC_COLLATE_DB]="";
 attributes[ParsersAttributes::LC_CTYPE_DB]="";
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
 unsigned idx=0;

 switch(localiz_id)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 localizations[idx]=value;
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
 else
  return(NULL);
}

QString DatabaseModel::validateObjectDefinition(BaseObject *object, unsigned def_type)
{
 ObjectType obj_type;
 QString obj_def;

 if(object)
 {
  try
  {
   obj_type=object->getObjectType();

   if(obj_type==BASE_RELATIONSHIP && def_type==SchemaParser::XML_DEFINITION)
    obj_def=dynamic_cast<BaseRelationship *>(object)->getCodeDefinition();
   else if(obj_type==OBJ_TEXTBOX && def_type==SchemaParser::XML_DEFINITION)
    obj_def=dynamic_cast<Textbox *>(object)->getCodeDefinition();
   else
    obj_def=object->getCodeDefinition(def_type);
  }
  catch(Exception &e)
  {
   if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
    throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                           .arg(QString::fromUtf8(object->getName(true)))
                           .arg(object->getTypeName()),
                  ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   else
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }

 return(obj_def);
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
                         .arg(QString::fromUtf8(tabspc->getName()))
                         .arg(aux_tabspc->getName()),
                  ERR_ASG_DUP_TABLESPACE_DIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   itr++;
  }
 }

 //Raises an error if there is an object with the same name
 if((obj_type!=OBJ_FUNCTION && getObject(object->getName(true), obj_type, idx)) ||
    (obj_type==OBJ_FUNCTION &&
     getObject(dynamic_cast<Function *>(object)->getSignature(), obj_type, idx)))
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
  DatabaseModel::validateObjectDefinition(object, SchemaParser::SQL_DEFINITION);
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
 unsigned idx=0;

 switch(localiz_id)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 return(localizations[idx]);
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
 ObjectType types[20]={
          BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
          OBJ_AGGREGATE, OBJ_OPERATOR,
          OBJ_SEQUENCE, OBJ_CONVERSION,
          OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
          BASE_RELATIONSHIP, OBJ_TEXTBOX,
          OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
          OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
 vector<BaseObject *> *list=NULL;
 BaseObject *object=NULL;
 unsigned i;

 disconnectRelationships();

 for(i=0; i < 20; i++)
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
 if(table)
 {
  vector<BaseObject *> refs;
  QString str_aux;

  //Get the table references
  getObjectReferences(table, refs, true);

  //If there are objects referencing the table
  if(!refs.empty())
  {
   ErrorType err_type;
   unsigned i=0, count=refs.size();

   while(i < count)
   {
     //Raises an error indicating the object that is referencing the table
     if(!dynamic_cast<TableObject *>(refs[i]))
     {
      err_type=ERR_REM_DIRECT_REFERENCE;
      str_aux=QString(Exception::getErrorMessage(err_type))
              .arg(table->getName(true))
              .arg(table->getTypeName())
              .arg(refs[0]->getName(true))
              .arg(refs[0]->getTypeName());

       throw Exception(str_aux, err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     }
     else
     {
      BaseObject *ref_obj_parent=dynamic_cast<TableObject *>(refs[i])->getParentTable();

      if(ref_obj_parent != table)
      {
       err_type=ERR_REM_INDIRECT_REFERENCE;
       str_aux=QString(Exception::getErrorMessage(err_type))
               .arg(table->getName(true))
               .arg(table->getTypeName())
               .arg(refs[0]->getName(true))
               .arg(refs[0]->getTypeName())
               .arg(ref_obj_parent->getName(true))
               .arg(ref_obj_parent->getTypeName());

       throw Exception(str_aux, err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
     }

     i++;
    }
   }

  __removeObject(table, obj_idx);
  PgSQLType::removeUserType(table->getName(true), table);
  updateTableFKRelationships(table);
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
 if(sequence)
 {
  removeUserType(sequence, obj_idx);
  __removeObject(sequence, obj_idx);
 }
}

void DatabaseModel::addView(View *view, int obj_idx)
{
 if(view)
 {
  try
  {
   __addObject(view, obj_idx);
   updateViewRelationships(view);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

View *DatabaseModel::getView(unsigned obj_idx)
{
 return(dynamic_cast<View *>(getObject(obj_idx, OBJ_VIEW)));
}

void DatabaseModel::removeView(View *view, int obj_idx)
{
 if(view)
 {
  __removeObject(view, obj_idx);
  updateViewRelationships(view);
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

     delete(rel);

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
       xml_special_objs[rel->getObjectId()]=rel->getCodeDefinition();
       removeRelationship(rel);
       delete(rel);
      }
     }
    }

    removeView(view);
    delete(view);
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
    rel->disconnectRelationship();

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
 __removeObject(txtbox, obj_idx);
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
 if(schema)
 {
  vector<BaseObject *> refs;

  getObjectReferences(schema, refs, true);

  //Raises an error if there is some object referencing the schema to be removed
  if(!refs.empty())
  {
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(schema->getName(true))
                 .arg(schema->getTypeName())
                 .arg(refs[0]->getName(true))
                 .arg(refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(schema, obj_idx);
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
 if(role)
 {
  vector<BaseObject *> refs;

  getObjectReferences(role, refs, true);

  //Raises an error if there is some object referencing the role to be removed
  if(!refs.empty())
  {
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(role->getName(true))
                 .arg(role->getTypeName())
                 .arg(refs[0]->getName(true))
                 .arg(refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(role, obj_idx);
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
 if(tabspc)
 {
  vector<BaseObject *> refs;
  QString str_aux;

  getObjectReferences(tabspc, refs, true);

  //Raises an error if there is some object referencing the tablespace to be removed
  if(!refs.empty())
  {
   ErrorType err_type;

   if(!dynamic_cast<TableObject *>(refs[0]))
   {
    err_type=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(err_type))
            .arg(tabspc->getName(true))
            .arg(tabspc->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName());
   }
   else
   {
    BaseObject *ref_obj_parent=dynamic_cast<TableObject *>(refs[0])->getParentTable();

    err_type=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(err_type))
            .arg(tabspc->getName(true))
            .arg(tabspc->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName())
            .arg(ref_obj_parent->getName(true))
            .arg(ref_obj_parent->getTypeName());

   }

   throw Exception(str_aux,err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(tabspc, obj_idx);
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
 __removeObject(cast, obj_idx);
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
 __removeObject(conv, obj_idx);
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
 if(lang)
 {
  vector<BaseObject *> refs;

  getObjectReferences(lang, refs, true);

  //Raises an error if there is some object referencing the language to be removed
  if(!refs.empty())
  {
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(lang->getName(true))
                 .arg(lang->getTypeName())
                 .arg(dynamic_cast<Function *>(refs[0])->getSignature())
                 .arg(refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(lang, obj_idx);
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
 if(func)
 {
  vector<BaseObject *> refs;
  QString str_aux;

  getObjectReferences(func, refs, true);

  //Raises an error if there is some object referencing the function to be removed
  if(!refs.empty())
  {
   ErrorType err_type;

   if(!dynamic_cast<TableObject *>(refs[0]))
   {
    err_type=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
            .arg(func->getSignature())
            .arg(func->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName());

   }
   else
   {
    BaseObject *ref_parent_obj=dynamic_cast<TableObject *>(refs[0])->getParentTable();

    err_type=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE))
            .arg(func->getSignature())
            .arg(func->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName())
            .arg(ref_parent_obj->getName(true))
            .arg(ref_parent_obj->getTypeName());

   }

   throw Exception(str_aux,err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(func, obj_idx);
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
 __removeObject(aggreg, obj_idx);
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
 if(op_family)
 {
  vector<BaseObject *> refs;

  getObjectReferences(op_family, refs, true);

  if(!refs.empty())
  {
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(op_family->getName(true))
                 .arg(op_family->getTypeName())
                 .arg(refs[0]->getName(true))
                 .arg(refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(op_family, obj_idx);
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
 __removeObject(op_class, obj_idx);
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
 if(oper)
 {
  vector<BaseObject *> refs;

  getObjectReferences(oper, refs, true);

  if(!refs.empty())
  {

   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(oper->getSignature(true))
                 .arg(oper->getTypeName())
                 .arg(refs[0]->getName(true))
                 .arg(refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(oper, obj_idx);
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

void DatabaseModel::removeUserType(BaseObject *object, int obj_idx)
{
 if(object)
 {
  vector<BaseObject *> refs;
  QString str_aux;

  getObjectReferences(object, refs, true);

  if(!refs.empty())
  {
   ErrorType err_type;

   if(!dynamic_cast<TableObject *>(refs[0]))
   {
    err_type=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(err_type))
            .arg(object->getName(true))
            .arg(object->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName());
   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(refs[0])->getParentTable();

    err_type=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(err_type))
            .arg(object->getName(true))
            .arg(object->getTypeName())
            .arg(refs[0]->getName(true))
            .arg(refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());
   }

   throw Exception(str_aux,err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(object, obj_idx);

  //Removes the user type from the list of base types of pgsql
  PgSQLType::removeUserType(object->getName(true), object);
 }
}

void DatabaseModel::addPermission(Permission *perm)
{
 try
 {
  if(!perm)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(getPermissionIndex(perm) >=0)
  {
   throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
                .arg(QString::fromUtf8(perm->getObject()->getName()))
                .arg(perm->getObject()->getTypeName()),
                ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  permissions.push_back(perm);
 }
 catch(Exception &e)
 {
  if(e.getErrorType()==ERR_ASG_DUPLIC_OBJECT)
   throw
   Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
           .arg(QString::fromUtf8(perm->getObject()->getName()))
           .arg(perm->getObject()->getTypeName()),
           ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

  else
   throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removePermission(Permission *perm)
{
 if(perm)
 {
  __removeObject(perm);
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
   delete(perm);

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

   if(perm==perm_aux || ref_role)
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
        localizations[0]=attribs[ParsersAttributes::LC_CTYPE_DB];
        localizations[1]=attribs[ParsersAttributes::LC_COLLATE_DB];

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
                                   .arg(QString::fromUtf8(object->getName()))
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
 }

 return(object);
}

void DatabaseModel::setBasicAttributes(BaseObject *object)
{
 map<QString, QString> attribs, attribs_aux;
 QString elem_name;
 BaseObject *tabspc=NULL, *owner=NULL;
 Schema *schema=NULL;
 ObjectType obj_type=BASE_OBJECT, obj_type_aux;
 bool has_error=false, protected_obj=false;

 if(!object)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 XMLParser::getElementAttributes(attribs);

 obj_type_aux=object->getObjectType();
 if(obj_type_aux!=OBJ_CAST)
  object->setName(attribs[ParsersAttributes::NAME]);

 protected_obj=attribs[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;

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

 if(has_error)
 {
  throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                   .arg(QString::fromUtf8(object->getName()))
                   .arg(object->getTypeName())
                   .arg(QString::fromUtf8(attribs_aux[ParsersAttributes::NAME]))
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
                        .arg(QString::fromUtf8(object->getName()))
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

 /* Os elementos dos dois elementos a seguir deve aparecer na
    mesma ordem pois seus valores serão trabalhados em conjunto
    na interação onde ser precisa identificar as opções do papel
    o qual está sendo criado */
 QString op_attribs[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                        ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                        ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED };

 unsigned op_vect[]={ Role::OP_SUPERUSER, Role::OP_CREATEDB,
                      Role::OP_CREATEROLE, Role::OP_INHERIT,
                      Role::OP_LOGIN, Role::OP_ENCRYPTED };

 try
 {
  //Aloca no novo papel
  role=new Role;
  setBasicAttributes(role);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  //Definindo os valores de atributos básicos do papel
  role->setPassword(attribs[ParsersAttributes::PASSWORD]);
  role->setValidity(attribs[ParsersAttributes::VALIDITY]);

  /* Caso o atributo de id de usuário esteja atribuído no xml.
     (atributos[AtributosParsers::UID] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!attribs[ParsersAttributes::SYSID].isEmpty())
   role->setSysid(attribs[ParsersAttributes::SYSID].toInt());

  /* Caso o atributo de limite de conexão esteja atribuído no xml.
     (atributos[AtributosParsers::LIMITE_CONEXAO] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!attribs[ParsersAttributes::CONN_LIMIT].isEmpty())
   role->setConnectionLimit(attribs[ParsersAttributes::CONN_LIMIT].toInt());

  /* Identificando as opções do papel. Caso o atributo referet   uma
     estive com valor "true" no documento XML quer dizer que aquele
     atributo está marcado para o papel */
  for(i=0; i < 6; i++)
  {
   //Verifica se a opção está marcada no XML, valor de atributo = true
   marked=attribs[op_attribs[i]]==ParsersAttributes::_TRUE_;
   role->setOption(op_vect[i], marked);
  }

  //Passa para os elementos filhos que provavelmente serão <roles> e <comment>
  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser, qualquer outro
       tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     //Obtém o nome do elemento filho
     elem_name=XMLParser::getElementName();

     //Caso o elemento filho seja uma lista de papeis <roles>
     if(elem_name==ParsersAttributes::ROLES)
     {
      //Obtém os atributos do elemento <roles>, neste caso são names e reftype
      XMLParser::getElementAttributes(attribs_aux);

      /* O atributo names armazena uma lista de nomes de papéis as quais o novo papel
         referenciará. A lista tem os elementos separados por vírgula, sendo assim a
         string será quebrada usando o delimitador ',') */
      list=attribs_aux[ParsersAttributes::NAMES].split(',');

      //Obtém a quantidade de nomes de papéis na lista
      len=list.size();

      /* Identificando o tipo da lista de papéis a qual será inserido os objetos
         cujos nomes foram extraídos acima */
      if(attribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::REFER)
       role_type=Role::REF_ROLE;
      else if(attribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::MEMBER)
       role_type=Role::MEMBER_ROLE;
      else
       role_type=Role::ADMIN_ROLE;

      //Varre a lista de nomes de papéis
      for(i=0; i < len; i++)
      {
       //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
       ref_role=dynamic_cast<Role *>(getObject(list[i].trimmed(),OBJ_ROLE));

       /* Caso esse papel não exista um erro será disparado pois um novo papel
          não pode referenciar um outro papel que ainda nem foi criado */
       if(!ref_role)
       {
        //Dispara a exceção
        throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(role->getName()))
                               .arg(BaseObject::getTypeName(OBJ_ROLE))
                               .arg(QString::fromUtf8(list[i]))
                               .arg(BaseObject::getTypeName(OBJ_ROLE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       //Caso o papel exista no modelo, o mesmo será relacionado ao novo papel
       role->addRole(role_type, ref_role);
      }
     }
    }
   }
   /* A extração de elementos continuará até que se chegue no último elemento
      filho do elemento <role> */
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString extra_info;
  extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o papel alocado
  if(role) delete(role);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 //Retorna o novo papel criado
 return(role);
}

Tablespace *DatabaseModel::createTablespace(void)
{
 map<QString, QString> attribs;
 Tablespace *tabspc=NULL;

 try
 {
  //Aloca no novo espaço de tabelas
  tabspc=new Tablespace;
  setBasicAttributes(tabspc);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  //Definindo os valores de atributos básicos do papel
  tabspc->setDirectory(attribs[ParsersAttributes::DIRECTORY]);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(tabspc) delete(tabspc);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tabspc);
}

Schema *DatabaseModel::createSchema(void)
{
 map<QString, QString> attribs;
 Schema *schema=NULL;

 try
 {
  //Aloca no novo esquema
  schema=new Schema;

  //Lê do parser os atributos basicos
  setBasicAttributes(schema);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(schema) delete(schema);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
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

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);
  setBasicAttributes(lang);

  lang->setTrusted(attribs[ParsersAttributes::TRUSTED]==
                               ParsersAttributes::_TRUE_);

   if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
   {
    do
    {
     /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
     if(XMLParser::getElementType()==XML_ELEMENT_NODE)
     {
      obj_type=getObjectType(XMLParser::getElementName());

      if(obj_type==OBJ_FUNCTION)
      {
       XMLParser::getElementAttributes(attribs);
       //Obtém o tipo de referência da função
       ref_type=attribs[ParsersAttributes::REF_TYPE];

       //Caso seja uma função handler ou validator
       if(ref_type==ParsersAttributes::VALIDATOR_FUNC ||
          ref_type==ParsersAttributes::HANDLER_FUNC ||
          ref_type==ParsersAttributes::INLINE_FUNC)
       {
        //Obtém a assinatura da função
        signature=attribs[ParsersAttributes::SIGNATURE];

        //Obtém a função do modelo
        func=getObject(signature, OBJ_FUNCTION);

        //Caso a função não seja encontrada
        if(!func)
         //Dispara a exceção indicando que o objeto está incompleto
         throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(lang->getName()))
                               .arg(lang->getTypeName())
                               .arg(QString::fromUtf8(signature))
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
        //Dispara uma exceção caso o tipo de referencia a função seja inválido
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

  //Remove a linguagem alocada
  if(lang) delete(lang);

  //Redireciona qualquer exceção capturada
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
  //Lê do parser os atributos basicos
  setBasicAttributes(func);

  //Obtém os atributos da função
  XMLParser::getElementAttributes(attribs);

  //Define se a função retorna setof, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::RETURNS_SETOF].isEmpty())
   func->setReturnSetOf(attribs[ParsersAttributes::RETURNS_SETOF]==
                               ParsersAttributes::_TRUE_);

  //Define se a função é do tipo janela, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::WINDOW_FUNC].isEmpty())
   func->setWindowFunction(attribs[ParsersAttributes::WINDOW_FUNC]==
                               ParsersAttributes::_TRUE_);

  //Define a configuração de retorno da função, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::BEHAVIOR_TYPE].isEmpty())
   func->setBehaviorType(BehaviorType(attribs[ParsersAttributes::BEHAVIOR_TYPE]));

  //Define o tipo da função, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::FUNCTION_TYPE].isEmpty())
   func->setFunctionType(FunctionType(attribs[ParsersAttributes::FUNCTION_TYPE]));

  //Define o tipo de segurança da função, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::SECURITY_TYPE].isEmpty())
   func->setSecurityType(SecurityType(attribs[ParsersAttributes::SECURITY_TYPE]));

  //Define o custo de execução da função, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::EXECUTION_COST].isEmpty())
   func->setExecutionCost(attribs[ParsersAttributes::EXECUTION_COST].toInt());

  //Define a quantidade de linhas retornadas pela função, caso o atributo esteja marcado no XML
  if(!attribs[ParsersAttributes::ROW_AMOUNT].isEmpty())
   func->setRowAmount(attribs[ParsersAttributes::ROW_AMOUNT].toInt());

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();
     obj_type=getObjectType(elem);

     //Caso o parser acesso a tag que determina o tipo de retorno da função
     if(elem==ParsersAttributes::RETURN_TYPE)
     {
      XMLParser::savePosition();

      try
      {
       //Acesso os elementos filhos os quais podem ser um <type> ou vários <parameter>
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

       do
       {
        if(XMLParser::getElementType()==XML_ELEMENT_NODE)
        {
         /* Caso o elemento atual no parser seja um <type>, indica que
          será extraído o tipo de retorno da função */
         if(XMLParser::getElementName()==ParsersAttributes::TYPE)
         {
          //Cria o tipo
          type=createPgSQLType();
          //Atribui ao retorno da função
          func->setReturnType(type);
         }
         /* Criação dos tipo de retorno de tabela da função. Os mesmos vem descritos
            dentro da tag <return-type> em forma de parâmetros */
         else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
         {
          param=createParameter();
          //Adiciona o tipo de retorno   função
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
     //Obtendo a linguagem da função
     else if(obj_type==OBJ_LANGUAGE)
     {
      //Obtém seus atributos
      XMLParser::getElementAttributes(attribs);

      //Busca a linguagem no modelo
      object=getObject(attribs[ParsersAttributes::NAME], obj_type);

      //Caso a linguagem não existe será disparada uma exceção
      if(!object)
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(func->getName()))
                              .arg(func->getTypeName())
                              .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
                              .arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a linguagem da função
      func->setLanguage(dynamic_cast<Language *>(object));
     }
     else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
     {
      param=createParameter();
      //Adiciona o parâmet  função
      func->addParameter(param);
     }
     //Extraíndo a definição (corpo) da função (tag <definition>)
     else if(XMLParser::getElementName()==ParsersAttributes::DEFINITION)
     {
      XMLParser::savePosition();

      //Obtém os atributos da biblioteca
      XMLParser::getElementAttributes(attribs_aux);

      if(!attribs_aux[ParsersAttributes::LIBRARY].isEmpty())
      {
       func->setLibrary(attribs_aux[ParsersAttributes::LIBRARY]);
       func->setSymbol(attribs_aux[ParsersAttributes::SYMBOL]);
      }
      /* Para se ter acesso ao código que define a função é preciso acessar
         o filho da tag <definition> e obter seu conteúdo */
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

  //Remove o espaco de tabela alocado
  if(func)
  {
   str_aux=func->getName(true);
   delete(func);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
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
  //Salva a posição do parser
  XMLParser::savePosition();
  //Obtem os atributos do parâmetro (nome, in e out)
  XMLParser::getElementAttributes(attribs);

  param.setName(attribs[ParsersAttributes::NAME]);
  /* Configurando atributos in e out do parâmetro caso estes estejam
     definidos como true no XML */
  param.setIn(attribs[ParsersAttributes::PARAM_IN]==ParsersAttributes::_TRUE_);
  param.setOut(attribs[ParsersAttributes::PARAM_OUT]==ParsersAttributes::_TRUE_);
  param.setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

  //Acessa os elementos filhos do parâmetro, que no caso será apenas <type> ou <domain>
  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
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

  //Restaura a posição do parser
  XMLParser::restorePosition();
 }
 catch(Exception &e)
 {
  QString extra_info;
  extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Restaura a posição do parser
  XMLParser::restorePosition();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(param);
}

PgSQLType DatabaseModel::createPgSQLType(void)
{
 map<QString, QString> attribs;
 vector<void *> ptypes_vect;
 vector<void *>::iterator itr, itr_end;
 unsigned length=1, dimension=0, type_idx=0;
 int precision=-1;
 QString name;
 Type *usr_type=NULL;
 void *ptype=NULL;
 bool found=false, with_timezone;
 IntervalType interv_type;
 SpatialType spatial_type;


 //Obtém os atributos do tipo
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
                        attribs[ParsersAttributes::VARIATION].toUInt());

 name=attribs[ParsersAttributes::NAME];

 type_idx=PgSQLType::getBaseTypeIndex(name);
 if(type_idx!=PgSQLType::null)
 {
  return(PgSQLType(name,length,dimension,precision,with_timezone,interv_type, spatial_type));
 }
 else
 {
  //Obtém a lista de tipos definidios pelo usuario
  PgSQLType::getUserTypes(ptypes_vect, this,
                               UserTypeConfig::BASE_TYPE |
                               UserTypeConfig::DOMAIN_TYPE |
                               UserTypeConfig::TABLE_TYPE |
                               UserTypeConfig::SEQUENCE_TYPE);
  itr=ptypes_vect.begin();
  itr_end=ptypes_vect.end();

  //Faz uma varredura na lista de tipos
  while(itr!=itr_end && !found)
  {
   //Converte o elemento atual para a classe Tipo
   ptype=(*itr);
   usr_type=reinterpret_cast<Type *>(ptype);

   //Caso o nome passado no XML for igual ao nome do objeto Tipo atual
   found=(usr_type->getName(true)==name);
   itr++;
  }

  /* Caso o tipo não foi encontrado na lista de tipos quer dizer existe
     a tentativa de se referenciar um tipo definido pelo usuário sem que o
     mesmo exista no modelo, sendo assim, um erro será disparado e
     a criação do tipo será abortada */
  if(!found)
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
 Parameter param;
 BaseObject *func=NULL;
 unsigned func_type=0;
 PgSQLType copy_type;

 try
 {
  type=new Type;
  //Lê do parser os atributos basicos
  setBasicAttributes(type);

  //Obtém os atributos do tipo
  XMLParser::getElementAttributes(attribs);

  //Define a configuração do tipo
  if(attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE)
  {
   type->setConfiguration(Type::BASE_TYPE);

   //Definindos os atributos específicos para tipo base

   //Definindo se o tipo é passado por valor ou não
   type->setByValue(attribs[ParsersAttributes::BY_VALUE]==ParsersAttributes::_TRUE_);

   //Definindo o comprimento interno do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::INTERNAL_LENGHT].isEmpty())
    type->setInternalLength(attribs[ParsersAttributes::INTERNAL_LENGHT].toUInt());

   //Definindo o alinhamento interno do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::ALIGNMENT].isEmpty())
    type->setAlignment(attribs[ParsersAttributes::ALIGNMENT]);

   //Definindo o tipo de armazenamento do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::STORAGE].isEmpty())
    type->setStorage(attribs[ParsersAttributes::STORAGE]);

   //Definindo o elemento do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::ELEMENT].isEmpty())
    type->setElement(attribs[ParsersAttributes::ELEMENT]);

   //Definindo o delimitador do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::DELIMITER].isEmpty())
    type->setDelimiter(attribs[ParsersAttributes::DELIMITER][0].toAscii());

   //Definindo o valor padrão do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::DEFAULT_VALUE].isEmpty())
    type->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::CATEGORY].isEmpty())
    type->setCategory(attribs[ParsersAttributes::CATEGORY]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!attribs[ParsersAttributes::PREFERRED].isEmpty())
    type->setPreferred(attribs[ParsersAttributes::PREFERRED]==ParsersAttributes::_TRUE_);


   /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
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
  else
   type->setConfiguration(Type::ENUMERATION_TYPE);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     //Operação específica para tipo ENUM
     if(elem==ParsersAttributes::ENUM_TYPE)
     {
      //Obtém o atributo da tag <enumerations>
      XMLParser::getElementAttributes(attribs);
      /* Como se trata de uma lista de enumerações separadas por vírgulas
         a mesma será quebrada e transformada num vetor */
      enums=attribs[ParsersAttributes::VALUES].split(",");

      //Adiciona ao tipo todas as enumerações presentes no vetor
      count=enums.size();
      for(i=0; i < count; i++)
       type->addEnumeration(enums[i]);
     }
     //Operação específica para tipo COMPOSTO
     else if(elem==ParsersAttributes::PARAMETER)
     {
      /* No caso de tipo composto, o mesmo possui indefinida quatidade
         de elementos <parameter> os quais simbolizam os atributos do
         tipo */
      param=createParameter();
      type->addAttribute(param);
     }
     //Operação específica para tipo BASE
     else if(elem==ParsersAttributes::TYPE)
     {
      copy_type=createPgSQLType();
      type->setLikeType(copy_type);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso de tipo base, serão extraídas referência a funções do modelo,
        as quais serão atribuía  s funções que compoem o tipo base. */
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida di XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      /* Dispara uma exceção caso o tipo de referencia a função seja inválido ou
         se a função referenciada não existe */
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(type->getName()))
                              .arg(type->getTypeName())
                              .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                              .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      else if(func_types.count(attribs[ParsersAttributes::REF_TYPE])==0)
       throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      func_type=func_types[attribs[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      type->setFunction(func_type, dynamic_cast<Function *>(func));
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

  //Remove o tipo alocado
  if(type)
  {
   str_aux=type->getName(true);
   delete(type);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
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

  //Lê do parser os atributos basicos
  setBasicAttributes(domain);

  //Obtém os atributos do domíno
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
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo ao qual o domínio se aplica */
     if(elem==ParsersAttributes::TYPE)
     {
      domain->setType(createPgSQLType());
     }
     //Caso o elemento seja uma expressão
     else if(elem==ParsersAttributes::EXPRESSION)
     {
      /* Para se extraír a expressão, é necessário salvar a posição de navegação
         do parser, pois o conteúdo da mesma é um elemento filho do elemento
         atual do parser XML */
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //dominio->definirExpressao(QString::fromUtf8(ParserXML::obterConteudoElemento()));
      domain->setExpression(XMLParser::getElementContent());
      //Restaura a posição de navegação do parser, ou seja, volta para o elemento <expression>
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

  //Remove o domínio alocado
  if(domain) delete(domain);

  //Redireciona qualquer exceção capturada
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

  //Lê do parser os atributos basicos
  setBasicAttributes(cast);

  //Obtém os atributos do domíno
  XMLParser::getElementAttributes(attribs);

  if(attribs[ParsersAttributes::CAST_TYPE]==
      ParsersAttributes::IMPLICIT)
   cast->setCastType(Cast::IMPLICIT);
  else
   cast->setCastType(Cast::ASSIGNMENT);

  cast->setInOut(attribs[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo (de origem ou destino) da conversao */
     if(elem==ParsersAttributes::TYPE)
     {
      type=createPgSQLType();
      if(type_idx==0)
       cast->setDataType(Cast::SRC_TYPE, type);
      else
       cast->setDataType(Cast::DST_TYPE, type);
      type_idx++;
     }
     //Extraíndo a função de conversão do XML
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso da conversão, será extraída a refeênia   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribud   conversão */
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(cast->getName()))
                             .arg(cast->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
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

  //Redireciona qualquer exceção capturada
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

  //Lê do parser os atributos basicos
  setBasicAttributes(conv);

  //Obtém os atributos
  XMLParser::getElementAttributes(attribs);

  conv->setEncoding(Conversion::SRC_ENCODING,
                                 EncodingType(attribs[ParsersAttributes::SRC_ENCODING]));

  conv->setEncoding(Conversion::DST_ENCODING,
                                 EncodingType(attribs[ParsersAttributes::DST_ENCODING]));

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso da conversão, será extraída a refeênia   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribud   conversão */
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(conv->getName()))
                             .arg(conv->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
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

  //Redireciona qualquer exceção capturada
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
 unsigned func_type, oper_type, arg_type;
 PgSQLType type;

 try
 {
  oper=new Operator;

  //Lê do parser os atributos basicos
  setBasicAttributes(oper);

  //Obtém os atributos
  XMLParser::getElementAttributes(attribs);

  oper->setMerges(attribs[ParsersAttributes::MERGES]==ParsersAttributes::_TRUE_);
  oper->setHashes(attribs[ParsersAttributes::HASHES]==ParsersAttributes::_TRUE_);

  /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
  func_types[ParsersAttributes::OPERATOR_FUNC]=Operator::FUNC_OPERATOR;
  func_types[ParsersAttributes::JOIN_FUNC]=Operator::FUNC_JOIN;
  func_types[ParsersAttributes::RESTRICTION_FUNC]=Operator::FUNC_RESTRICTION;

  /* O mapa de tipos de operadores abaixo é usado para se atribuir de forma
      mais simples, sem comparações, o operador que for obtida do XML a qual
      o operador em construção referencia */
  oper_types[ParsersAttributes::COMMUTATOR_OP]=Operator::OPER_COMMUTATOR;
  oper_types[ParsersAttributes::GREATER_OP]=Operator::OPER_GREATER;
  oper_types[ParsersAttributes::LESS_OP]=Operator::OPER_LESS;
  oper_types[ParsersAttributes::NEGATOR_OP]=Operator::OPER_NEGATOR;
  oper_types[ParsersAttributes::SORT_OP]=Operator::OPER_SORT1;
  oper_types[ParsersAttributes::SORT2_OP]=Operator::OPER_SORT2;

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==objs_schemas[OBJ_OPERATOR])
     {
      //Obtém os atributos do operador referenciado
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      oper_aux=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_OPERATOR);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!oper_aux && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(oper->getSignature(true)))
                             .arg(oper->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_OPERATOR)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      oper_type=oper_types[attribs[ParsersAttributes::REF_TYPE]];
      oper->setOperator(dynamic_cast<Operator *>(oper_aux),oper_type);
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      /* Obtém os atributos do tipo para saber se o mesmo é um tipo da
         esquerda ou da direita */
      XMLParser::getElementAttributes(attribs);

      //Obtém o tipo de referência do tipo base (esquerda ou direita)
      if(attribs[ParsersAttributes::REF_TYPE]!=ParsersAttributes::RIGHT_TYPE)
       arg_type=Operator::LEFT_ARG;
      else
       arg_type=Operator::RIGHT_ARG;

      type=createPgSQLType();
      oper->setArgumentType(type, arg_type);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso do operador, será extraída a refernca   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída ao operador */
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(oper->getName()))
                             .arg(oper->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      func_type=func_types[attribs[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      oper->setFunction(dynamic_cast<Function *>(func), func_type);
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

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(oper);
}

OperatorClass *DatabaseModel::createOperatorClass(void)
{
 map<QString, QString> attribs;
 map<QString, unsigned> elem_types;
 BaseObject *object=NULL;
 QString elem;
 PgSQLType type;
 OperatorClass *op_class=NULL;
 OperatorClassElement class_elem;
 bool recheck;
 unsigned stg_number, elem_type;

 try
 {
  op_class=new OperatorClass;

  //Lê do parser os atributos basicos
  setBasicAttributes(op_class);

  //Obtém os atributos
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
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==objs_schemas[OBJ_OPFAMILY])
     {
      //Obtém os atributos do operador referenciado
      XMLParser::getElementAttributes(attribs);

      /* Com o nome da família do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe um objeto correspondente */
      object=getObject(attribs[ParsersAttributes::NAME], OBJ_OPFAMILY);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!object && !attribs[ParsersAttributes::NAME].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(op_class->getName()))
                             .arg(op_class->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      op_class->setFamily(dynamic_cast<OperatorFamily *>(object));
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      //Obtém os atributos do tipo
      XMLParser::getElementAttributes(attribs);
      type=createPgSQLType();
      op_class->setDataType(type);
     }
     else if(elem==ParsersAttributes::ELEMENT)
     {
      XMLParser::getElementAttributes(attribs);

      recheck=attribs[ParsersAttributes::RECHECK]==ParsersAttributes::_TRUE_;
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
       class_elem.setOperator(dynamic_cast<Operator *>(object),stg_number,recheck);
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

  //Redireciona qualquer exceção capturada
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

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  //Definindo os valores de atributos básicos do objeto
  op_family->setIndexingType(IndexingType(attribs[ParsersAttributes::INDEX_TYPE]));
 }
 catch(Exception &e)
 {
  QString extra_info;
  extra_info=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(op_family) delete(op_family);

  //Redireciona qualquer exceção capturada
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

  //Lê do parser os atributos basicos
  setBasicAttributes(aggreg);

  //Obtém os atributos
  XMLParser::getElementAttributes(attribs);
  aggreg->setInitialCondition(attribs[ParsersAttributes::INITIAL_COND]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::TYPE)
     {
      //Obtém os atributos do tipo
      XMLParser::getElementAttributes(attribs);
      type=createPgSQLType();

      //Define o tipo   função agregada de acordo com o tipo de referência do mesmo
      if(attribs[ParsersAttributes::REF_TYPE]==
          ParsersAttributes::STATE_TYPE)
       aggreg->setStateType(type);
      else
       aggreg->addDataType(type);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(aggreg->getName()))
                             .arg(aggreg->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a função de acordo com o tipo de referência da mesma
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

  //Redireciona qualquer exceção capturada
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

  //Lê do parser os atributos basicos
  setBasicAttributes(table);

  //Obtém os atributos
  XMLParser::getElementAttributes(attribs);
  table->setWithOIDs(attribs[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
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

  //Redireciona qualquer exceção capturada
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

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);
  column->setNotNull(attribs[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);
  column->setDefaultValue(attribs[ParsersAttributes::DEFAULT_VALUE]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
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

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(column);
}

Constraint *DatabaseModel::createConstraint(BaseObject *object)
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

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  //Caso o objeto o qual será possuidor da restrição esteja alocado
  if(object)
  {
   obj_type=object->getObjectType();
   if(obj_type==OBJ_TABLE)
    table=dynamic_cast<Table *>(object);
   else if(obj_type==OBJ_RELATIONSHIP)
    rel=dynamic_cast<Relationship *>(object);
   else
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Caso o objeto não esteja especificado então o objeto possuidor será considerado
     como sendo sempre uma tabela e com base nisso o atributo "table" no código
     xml da restrição será usado para localizar a tabela possuidora no modelo */
  else
  {
   obj_type=OBJ_TABLE;
   table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));
   ins_constr_table=true;
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!table)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
          .arg(QString::fromUtf8(attribs[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  constr=new Constraint;
  constr->setParentTable(table);

  //Configurando o tipo da restrição
  if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CK_CONSTR)
   constr_type=ConstraintType::check;
  else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::PK_CONSTR)
   constr_type=ConstraintType::primary_key;
  else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FK_CONSTR)
   constr_type=ConstraintType::foreign_key;
  else
   constr_type=ConstraintType::unique;

  constr->setConstraintType(constr_type);
  if(!attribs[ParsersAttributes::FACTOR].isEmpty())
   constr->setFillFactor(attribs[ParsersAttributes::FACTOR].toUInt());
  setBasicAttributes(constr);

  /* Caso o tipo de restrição seja uma chave primária uma verificação importante
     é feita. Se a chave primária está sendo criada como filha de uma tag
     <table> ou <relationship> (vide exemplo 1).

     Exemplo 1:

      <table ...>        <relationship ...>
         ...                ...
         <constraint>       <constraint>
      </table>           </relationship>

     Esta restrição está declarada dentro de um bloco de código de uma tabela ou
     de relacionamento a mesma sendo assim a mesma será construída normalmente.

     Caso a mesma esteja sendo construída fora de ambos os blocos (vide exemplo 2)
     e referenciando apenas uma tabela no modelo um erro será gerado pois chaves
     primárias devem sempre ser construídas dentro de um dos dois blocos apresentados.

     Exemplo 2:
     <table name="tabela_X">
       ...
     </table>
     <relationship>
       ...
     </relationship>
     <constraint ... table="tabela_X">
       ...
     </constraint>  */

  /* Na prática, para se fazer a verificação acima, é suficiente
     verificar se o parâmetro 'objeto' está alocado pois o mesmo
     armazena o endereço da tabela ou relacionamento recém criados
     a partir do bloco <table> ou <relationship>, respectivamente.
     Quando tal parâmetro está nulo indica que a restrição será criada
     e atribuíd  tabela cujo nome está no atributo 'table' no XML
     significando que a mesma está declarada fora dos dois blocos indicados.
     Adicionalmente é necessário verificar o tipo da restrição para se
     ter certeza que a mesma é uma chave primária. */
  if(!object && constr_type==ConstraintType::primary_key)
    throw Exception(Exception::getErrorMessage(ERR_INV_PRIM_KEY_ALOCATION)
                  .arg(QString::fromUtf8(constr->getName())),
                  ERR_INV_PRIM_KEY_ALOCATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Efetuando configurações específicas para chaves estrangeiras
  if(constr_type==ConstraintType::foreign_key /*&& tipo_objeto==OBJETO_TABELA*/)
  {
   //Define se a restrição é postergavel (apenas para chaves estrangeiras)
   deferrable=(attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
   constr->setDeferrable(deferrable);

   if(deferrable && !attribs[ParsersAttributes::DEFER_TYPE].isEmpty())
    constr->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

   if(!attribs[ParsersAttributes::COMPARISON_TYPE].isEmpty())
    constr->setMatchType(attribs[ParsersAttributes::COMPARISON_TYPE]);

   //Definindo os tipos de ação nos eventos DELETE e UPDATE
   if(!attribs[ParsersAttributes::DEL_ACTION].isEmpty())
    constr->setActionType(attribs[ParsersAttributes::DEL_ACTION], false);

   if(!attribs[ParsersAttributes::UPD_ACTION].isEmpty())
    constr->setActionType(attribs[ParsersAttributes::UPD_ACTION], true);

   //Obtém a tabela referenciada na chave estrangeira
   ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_TABLE);

   /* Caso a tabela referenciada não seja encontrada verifica se esta não é a própria
      tabela a qual receberá a restrição (usado para auto-relacionamentos) */
   if(!ref_table && table->getName(true)==attribs[ParsersAttributes::REF_TABLE])
    ref_table=table;

   /* Caso a tabela referenciada não foi encontrada uma exceção será disparada pois
      não se pode criar uma chave estrangeira sem referenciar uma tabela de destino */
   if(!ref_table)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(constr->getName()))
          .arg(constr->getTypeName())
          .arg(QString::fromUtf8(attribs[ParsersAttributes::REF_TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Define a tabela de destino da chave estrangeira
   constr->setReferencedTable(ref_table);
  }

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::EXPRESSION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //Obtém o contéudo do elemento <expression>
      constr->setCheckExpression(XMLParser::getElementContent());
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(attribs);

      /* Obtém os nomes das colunas participantes da restrição
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      col_list=attribs[ParsersAttributes::NAMES].split(',');
      count=col_list.count();

      /* Obtém o tipo de referência das colunas de acordo com o atributo
         tipo de referência vindo do XML */
      if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::SRC_COLUMNS)
       col_type=Constraint::SOURCE_COLS;
      else
       col_type=Constraint::REFERENCED_COLS;

      //Varre a lista de nomes de colunas e as obtém da tabela a qual possuirá a restrição
      for(i=0; i < count; i++)
      {
       if(col_type==Constraint::SOURCE_COLS)
       {
        if(obj_type==OBJ_TABLE)
        {
         column=table->getColumn(col_list[i]);
         //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
         if(!column)
          column=table->getColumn(col_list[i], true);
        }
        else
         //Para os demais tipos de relacionamento as colunas a serem obtidas são os atributos do relacionamento
          column=dynamic_cast<Column *>(rel->getObject(col_list[i], OBJ_COLUMN));
       }
       else
       {
        table_aux=dynamic_cast<Table *>(ref_table);
        column=table_aux->getColumn(col_list[i]);
        //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
        if(!column)
         column=table_aux->getColumn(col_list[i], true);
       }

       //Adiciona a coluna   restrição
       constr->addColumn(column, col_type);
      }
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(ins_constr_table)
  {
   //Caso a restrição criada não seja uma chave primária insere-a normalmente na tabela
   if(constr->getConstraintType()!=ConstraintType::primary_key)
   {
    table->addConstraint(constr);

    /* Caso a tabela receptora da restrição esteja inserida no modelo, força o seu redesenho.
       Isso é útil para atualizar tabelas as quais tiveram restrições adicionadas após a sua
       criação */
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

  if(constr)
   delete(constr);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(constr);
}

Index *DatabaseModel::createIndex(Table *table)
{
 map<QString, QString> attribs;
 Index *index=NULL;
 Column *column=NULL;
 OperatorClass *op_class=NULL;
 QString elem, str_aux, expr;
 bool inc_idx_table=false,
      asc_order=false, nulls_first=false;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  if(!table)
  {
   inc_idx_table=true;
   table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!table)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_INDEX))
          .arg(QString::fromUtf8(attribs[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
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
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento atual for do tipo <idxelement> indica que
        os elementos filhos que podem ser extraídos são
        <column>, <expression> ou <opclass> */
     if(elem==ParsersAttributes::INDEX_ELEMENT)
     {
      nulls_first=(attribs[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
      asc_order=(attribs[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);

      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       elem=XMLParser::getElementName();

       if(XMLParser::getElementType()==XML_ELEMENT_NODE)
       {
        //Caso o elemento atual seja um  <opclass>
        if(elem==ParsersAttributes::OP_CLASS)
        {
         XMLParser::getElementAttributes(attribs);
         op_class=dynamic_cast<OperatorClass *>(getObject(attribs[ParsersAttributes::NAME], OBJ_OPCLASS));

         //Caso o índice esteja referenciando uma classe de operadores inexistente
         if(!op_class)
         {
          //Configura os argumentos da mensagem de erro
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(index->getName()))
                          .arg(BaseObject::getTypeName(OBJ_INDEX))
                          .arg(QString::fromUtf8(attribs[ParsersAttributes::OP_CLASS]))
                          .arg(BaseObject::getTypeName(OBJ_OPCLASS));
          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }
        //Caso o elemento atual seja um  <column>
        else if(elem==ParsersAttributes::COLUMN)
        {
         //Obtém a coluna que o elemento referencia
         XMLParser::getElementAttributes(attribs);
         column=table->getColumn(attribs[ParsersAttributes::NAME]);

         /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
            seu nome antigo */
         if(!column)
          column=table->getColumn(attribs[ParsersAttributes::NAME], true);
        }
        //Caso o elemento atual seja um  <expression>
        else if(elem==ParsersAttributes::EXPRESSION)
        {
         XMLParser::savePosition();
         //Acessa o elemento filho o qual contém o conteúdo da expressão
         XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
         expr=XMLParser::getElementContent();
         XMLParser::restorePosition();
        }
       }
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      if(!expr.isEmpty())
       index->addElement(expr, op_class, asc_order, nulls_first);
      else
       index->addElement(column, op_class, asc_order, nulls_first);

      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
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

  //Redireciona qualquer exceção capturada
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

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);
  rule->setExecutionType(attribs[ParsersAttributes::EXEC_TYPE]);
  rule->setEventType(attribs[ParsersAttributes::EVENT_TYPE]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::COMMANDS ||
        elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da condição ou comandos
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //str_aux=QString::fromUtf8(ParserXML::obterConteudoElemento());
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();

      if(elem==ParsersAttributes::COMMANDS)
      {
       /* A lista de comandos é quebrada por ; e os comandos
          inseridos um a um na regra */
       cmd_list=str_aux.split(';');
       count=cmd_list.count();
       for(i=0; i < count; i++)
       {
        //Caso o comando não esteja vazio
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

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(rule);
}

Trigger *DatabaseModel::createTrigger(Table *table)
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
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);

  if(!table && attribs[ParsersAttributes::TABLE].isEmpty())
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(!table && !attribs[ParsersAttributes::TABLE].isEmpty())
  {
   inc_trig_table=true;
   table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));
   if(!table)
    throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                  .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
                  .arg(BaseObject::getTypeName(OBJ_TRIGGER))
                  .arg(QString::fromUtf8(attribs[ParsersAttributes::TABLE]))
                  .arg(BaseObject::getTypeName(OBJ_TABLE)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  trigger=new Trigger;
  trigger->setParentTable(table);

  setBasicAttributes(trigger);

  //Marcando os eventos de execução do gatilho
  trigger->setEvent(EventType::on_insert,
                        (attribs[ParsersAttributes::INS_EVENT]==
                         ParsersAttributes::_TRUE_));

  trigger->setEvent(EventType::on_delete,
                        (attribs[ParsersAttributes::DEL_EVENT]==
                         ParsersAttributes::_TRUE_));

  trigger->setEvent(EventType::on_update,
                        (attribs[ParsersAttributes::UPD_EVENT]==
                         ParsersAttributes::_TRUE_));

  trigger->setEvent(EventType::on_truncate,
                        (attribs[ParsersAttributes::TRUNC_EVENT]==
                         ParsersAttributes::_TRUE_));

  //Marcando e o gatilho é executado por linha ou não
  trigger->setExecutePerRow(attribs[ParsersAttributes::PER_ROW]==
                            ParsersAttributes::_TRUE_);

  //Define o modo de disparo do gatilho
  trigger->setFiringType(FiringType(attribs[ParsersAttributes::FIRING_TYPE]));


  /* Atribuindo os argumentos vindo do XML ao gatilho.
     No XML os argumentos estão separados por vírgula,
     sendo assim o método split é usado para quebrar a
     string de argumentos e atribui-los ao objeto */
  list_aux=attribs[ParsersAttributes::ARGUMENTS].split(',');
  count=list_aux.count();
  for(i=0; i < count; i++)
  {
   if(!list_aux[i].isEmpty())
    trigger->addArgument(list_aux[i]);
  }

  //Caso o objeto alocado seja um gatilho
  trigger->setDeferrable(attribs[ParsersAttributes::DEFERRABLE]==
                              ParsersAttributes::_TRUE_);
  if(trigger->isDeferrable())
    trigger->setDeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

  //Obtém a tabela referenciada no gatilho
  ref_table=getObject(attribs[ParsersAttributes::REF_TABLE], OBJ_TABLE);
  trigger->setReferecendTable(ref_table);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::FUNCTION)
     {
      XMLParser::getElementAttributes(attribs);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      func=getObject(attribs[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!func && !attribs[ParsersAttributes::SIGNATURE].isEmpty())
      {
       str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
               .arg(QString::fromUtf8(trigger->getName()))
               .arg(trigger->getTypeName())
               .arg(QString::fromUtf8(attribs[ParsersAttributes::SIGNATURE]))
               .arg(BaseObject::getTypeName(OBJ_FUNCTION));

       //Dispara a exceção
       throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }

      //Define a função executada pelo gatilho
      trigger->setFunction(dynamic_cast<Function *>(func));
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();
      trigger->setCondition(str_aux);
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(attribs);

      /* Obtém os nomes das colunas participantes do gatilho
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      list_aux=attribs[ParsersAttributes::NAMES].split(',');
      count=list_aux.count();

      for(i=0; i < count; i++)
      {
       column=table->getColumn(list_aux[i]);

       /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
          seu nome antigo */
       if(!column)
        column=table->getColumn(list_aux[i], true);

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

  //Redireciona qualquer exceção capturada
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

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(attribs);
  sequence->setValues(attribs[ParsersAttributes::MIN_VALUE],
                            attribs[ParsersAttributes::MAX_VALUE],
                            attribs[ParsersAttributes::INCREMENT],
                            attribs[ParsersAttributes::START],
                            attribs[ParsersAttributes::CACHE]);
  sequence->setCycle(attribs[ParsersAttributes::CYCLE]==ParsersAttributes::_TRUE_);

  //Caso o atributo de coluna possuidora da sequencia esteja preenchido
  if(!attribs[ParsersAttributes::OWNER_COLUMN].isEmpty())
  {
   //Quebra o valor do atributo por .
   elem_list=attribs[ParsersAttributes::OWNER_COLUMN].split('.');
   count=elem_list.count();

   /* Caso a lista de nomes gerada possua 3 elementos indica
      que a coluna possuidora foi está no formato
      [ESQUEMA].[TABELA].[COLUNA] caso contrário
      supõe-se que esteja no formato
      [TABELA].[COLUNA] */
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

   //Obtém a tabela do modelo
   table=getObject(tab_name, OBJ_TABLE);

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!table)
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
            .arg(QString::fromUtf8(sequence->getName()))
            .arg(BaseObject::getTypeName(OBJ_SEQUENCE))
            .arg(QString::fromUtf8(tab_name))
            .arg(BaseObject::getTypeName(OBJ_TABLE));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Tenta obter a coluna da tabela com o nome vindo do XML
   column=dynamic_cast<Table *>(table)->getColumn(col_name);

   //Caso a coluna não for encontrada tenta obtê-la referenciando o antigo nome
   if(!column)
    column=dynamic_cast<Table *>(table)->getColumn(col_name, true);

   /* Caso a coluna não exista porém a mesma esteja sendo referenciada no xml
      um erro será disparado */
   if(!column && !ignore_onwer)
    throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
                  .arg(QString::fromUtf8(sequence->getName(true))),
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

  //Redireciona qualquer exceção capturada
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

 try
 {
  view=new View;
  setBasicAttributes(view);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::REFERENCE)
     {
      //Obtém os atributos da referência
      XMLParser::getElementAttributes(attribs);

      /* Caso o nome da tabela referenciada esteja preenchido,
         tentar criar uma referência específica a uma tabela/coluna */
      if(!attribs[ParsersAttributes::TABLE].isEmpty())
      {
       column=NULL;
       table=dynamic_cast<Table *>(getObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE));

       //Dispara uma exceção caso a tabela referenciada não exista
       if(!table)
       {
        str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                        .arg(QString::fromUtf8(view->getName()))
                        .arg(BaseObject::getTypeName(OBJ_VIEW))
                        .arg(QString::fromUtf8(attribs[ParsersAttributes::TABLE]))
                        .arg(BaseObject::getTypeName(OBJ_TABLE));

        //Dispara a exceção
        throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }

       if(!attribs[ParsersAttributes::COLUMN].isEmpty())
       {
        //Tenta obter a colna referenciada da tabela
        column=table->getColumn(attribs[ParsersAttributes::COLUMN]);

        //Caso a coluna não exista tenta obtê-la referenciando o nome antigo da mesma
        if(!column)
         column=table->getColumn(attribs[ParsersAttributes::COLUMN], true);

        /* Caso o atributo coluna da referencia esteja preenchido mas um objeto coluna
           não foi encontrado na tabela, uma exceção será disparada pois a visão está
           referenciando uma coluna inexistente na tabela */
         if(!column)
         {
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(view->getName()))
                          .arg(BaseObject::getTypeName(OBJ_VIEW))
                          .arg(QString::fromUtf8(attribs[ParsersAttributes::TABLE]) + "." +
                               QString::fromUtf8(attribs[ParsersAttributes::COLUMN]))
                         .arg(BaseObject::getTypeName(OBJ_COLUMN));

          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }

       //Adiciona a referência configurad  lista temporária de referências
       refs.push_back(Reference(table, column,
                                     attribs[ParsersAttributes::ALIAS],
                                     attribs[ParsersAttributes::COLUMN_ALIAS]));
      }
      //Extraindo uma referênci  uma expressão
      else
      {
       XMLParser::savePosition();
       //Armazena o alias da expressão
       str_aux=attribs[ParsersAttributes::ALIAS];

       //Acessa e obtém o conteúdo da expressão
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       refs.push_back(Reference(XMLParser::getElementContent(),str_aux));

       XMLParser::restorePosition();
      }
     }
     /* Extraindo as expressões as quais formam as partes da declaração da visão,
        ou seja, expressões e referências as quais estão entre SELECT-FROM,
        FROM-WHERE */
     else if(elem==ParsersAttributes::EXPRESSION)
     {
      XMLParser::savePosition();
      XMLParser::getElementAttributes(attribs);

      //Armazena o alias da expressão
      if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
       type=Reference::SQL_REFER_SELECT;
      else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
       type=Reference::SQL_REFER_FROM;
      else
       type=Reference::SQL_REFER_WHERE;

      //Acessa e obtém o conteúdo da expressão
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      list_aux=XMLParser::getElementContent().split(',');
      count=list_aux.size();

      //Construindo cada expressão na visão
      for(i=0; i < count; i++)
      {
       //Obtém o índice da referência e a adiioa   visão
       ref_idx=list_aux[i].toInt();
       view->addReference(refs[ref_idx],type);
      }

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
  if(view) delete(view);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 return(view);
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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(txtbox) delete(txtbox);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
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
  //Obtém os atributos do elemento
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

  /* Esta iteração obtém as tabelas participantes do relacionamento a
     partir do modelo com base nos nomes das tabelas vindos do XML */
  for(i=0; i < 2; i++)
  {
   //Localiza a tabela
   tables[i]=dynamic_cast<BaseTable *>(getObject(attribs[tab_attribs[i]], table_types[i]));

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tables[i])
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                    .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
                    .arg(BaseObject::getTypeName(obj_rel_type))
                    .arg(QString::fromUtf8(attribs[tab_attribs[i]]))
                    .arg(BaseObject::getTypeName(table_types[i]));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATION_TAB_VIEW)
  {
   //Caso o relacionamento entre tabela e visão exista
   base_rel=getRelationship(tables[0], tables[1]);

   //Caso o relacionamento tabela-visão nao seja encontrado o erro será disparado
   if(!base_rel)
    throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(this->getName()))
                             .arg(this->getTypeName())
                             .arg(QString::fromUtf8(attribs[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Desconecta o relacionamento para configurá-lo
   base_rel->disconnectRelationship();
   base_rel->setName(attribs[ParsersAttributes::NAME]);
  }
  else if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
  {
   base_rel=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK,
                                     tables[0], tables[1], false, false);
   base_rel->setName(attribs[ParsersAttributes::NAME]);
  }
  /* Caso o tipo de relacionamento não seja tabela-visão, isso indica que
     um relacionamento tabela-tabela deverá ser criado */
  else
  {
   //Obtém os atributos do relacionamento a partir do XML
   src_mand=attribs[ParsersAttributes::SRC_REQUIRED]==ParsersAttributes::_TRUE_;
   dst_mand=attribs[ParsersAttributes::DST_REQUIRED]==ParsersAttributes::_TRUE_;
   identifier=attribs[ParsersAttributes::IDENTIFIER]==ParsersAttributes::_TRUE_;
   deferrable=attribs[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_;
   auto_suffix=(!attribs[ParsersAttributes::AUTO_SUFFIX].isEmpty() &&
                attribs[ParsersAttributes::AUTO_SUFFIX]==ParsersAttributes::_TRUE_);
   defer_type=DeferralType(attribs[ParsersAttributes::DEFER_TYPE]);

   //Configura o tipo do novo relacionamento
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

   //Cria o novo relacionamento
   rel=new Relationship(rel_type,
                              dynamic_cast<Table *>(tables[0]),
                              dynamic_cast<Table *>(tables[1]),
                              src_mand, dst_mand,
                              auto_suffix, attribs[ParsersAttributes::SRC_SUFFIX],
                              attribs[ParsersAttributes::DST_SUFFIX],
                              identifier, deferrable, defer_type);

    if(!attribs[ParsersAttributes::TABLE_NAME].isEmpty())
    rel->setTableNameRelNN(attribs[ParsersAttributes::TABLE_NAME]);

   /* Faz com que o ponteiro relacao_base aponte para o novo relacionamento
      para executar as configurações geréricas as quais se aplicam tanto
      para relacionametno tabela-visao quanto para rel. tabela-tabela */
   base_rel=rel;
  }

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
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
     //Configurando a linha do relacionamento
     else if(elem==ParsersAttributes::LINE)
     {
      vector<QPointF> points;
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       //Lê o ponto do XML
       XMLParser::getElementAttributes(attribs);
       points.push_back(QPointF(attribs[ParsersAttributes::X_POS].toFloat(),
                                attribs[ParsersAttributes::Y_POS].toFloat()));
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      base_rel->setPoints(points);
      XMLParser::restorePosition();
     }
     //Configurando a posição dos rótulos
     else if(elem==ParsersAttributes::LABEL)
     {
      XMLParser::getElementAttributes(attribs);
      //Obtém o tipo de rótulo a ser configurado
      str_aux=attribs[ParsersAttributes::REF_TYPE];

      //Acessa o elemento filho da tag <label> o qual armazena a posição do rótulo
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      XMLParser::getElementAttributes(attribs);
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::SPECIAL_PK_COLS && rel)
     {
      QList<QString> col_list;

      //Obtém os atributos da tag <special-pk-cols>
      XMLParser::getElementAttributes(attribs);
      col_list=attribs[ParsersAttributes::INDEXES].split(',');

      while(!col_list.isEmpty())
      {
       cols_special_pk.push_back(col_list.front().toUInt());
       col_list.pop_front();
      }

      //Define as colunas que fazem parte da chave primária especila
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

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, extra_info);
 }

 //Caso o relacionamento tabela-tabela foi criado o mesmo será adicionado no modelo
 if(rel)
 {
  storeSpecialObjectsXML();
  addRelationship(rel);
 }
 else if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
  addRelationship(base_rel);

 //Define a proteção do relacionamento
 base_rel->setProtected(protect);

 //Reconecta o relacionamento caso o mesmo seja um rel. tabela-visao
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
 bool priv_value, grant_op;

 try
 {
  //Obtém os privilégios configurados para a permissão
  XMLParser::getElementAttributes(priv_attribs);

  /* Acessa o elemento filho <object> o qual armazena o objeto
     do modelo relacionado   permissão */
  XMLParser::savePosition();
  XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
  XMLParser::getElementAttributes(attribs);

  //Obtém os atributos do objeto que é referenciado pela  permissão
  obj_type=getObjectType(attribs[ParsersAttributes::TYPE]);
  obj_name=attribs[ParsersAttributes::NAME];
  parent_name=attribs[ParsersAttributes::PARENT];

  //Caso o objeto seja uma coluna a mesma será obtida da tabela pai
  if(obj_type==OBJ_COLUMN)
  {
   //Primeiramente a tabela pai é obtida do modelo
   parent_table=dynamic_cast<Table *>(getObject(parent_name, OBJ_TABLE));

   /* Caso a tabela pai existe obtém o objeto filho da mesma
      o qual é referenciado pela permissão */
   if(parent_table)
    object=parent_table->getColumn(obj_name);
  }
  else if(obj_type==OBJ_DATABASE)
  {
   object=this;
  }
  else
   /* Para os demais tipos de objetos, aceitos como referenciados
      por permissões, serão obtidos do modelo em si */
   object=getObject(obj_name, obj_type);

  /* Caso o objeto não exista será disparada uma exceção pois uma permissão
     não pode existir sem que referencie um objeto */
  if(!object)
   throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                          .arg(QString::fromUtf8(obj_name))
                          .arg(BaseObject::getTypeName(obj_type)),
                      ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Aloca a permissão relacionando-a com o objeto localizado
  perm=new Permission(object);

  /* Acessa o elemento que armazena os privilégios dos papéis os quais
     compartilham da mesma permissão sobre o objeto. Informar estes
     papéis não é obrigatório de acordo com a DTD e com a especificação
     SQL para permissões */
  do
  {
   if(XMLParser::getElementName()==ParsersAttributes::ROLES)
   {
    //Obtém os atributos do elemento <roles>, neste caso são names e reftype
    XMLParser::getElementAttributes(attribs);

    /* O atributo names armazena uma lista de nomes de papéis as quais a permissão
       referenciará. A lista tem os elementos separados por vírgula, sendo assim a
       string será quebrada usando o delimitador ',') */
    list=attribs[ParsersAttributes::NAMES].split(',');

    //Obtém a quantidade de nomes de papéis na lista
    len=list.size();

    //Varre a lista de nomes de papéis
    for(i=0; i < len; i++)
    {
     //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
     role=dynamic_cast<Role *>(getObject(list[i].trimmed(),OBJ_ROLE));

     /* Caso esse papel não exista um erro será disparado pois um novo papel
        não pode referenciar um outro papel que ainda nem foi criado */
     if(!role)
     {
      //Dispara a exceção
      throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                              .arg(QString::fromUtf8(object->getName()))
                              .arg(object->getTypeName())
                              .arg(QString::fromUtf8(list[i]))
                              .arg(BaseObject::getTypeName(OBJ_ROLE)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

     }
     //Adiciona o papel   permissão
     perm->addRole(role);
    }
   }
   else if(XMLParser::getElementName()==ParsersAttributes::PRIVILEGES)
   {
    //Obtém os atributos do elemento <privileges>
    XMLParser::getElementAttributes(priv_attribs);

    //Atribui os privilégio  permissão recém criada
    itr=priv_attribs.begin();
    itr_end=priv_attribs.end();

    /* Varre o mapa de privilégios configurando-os na permissão caso
       estes estejam definidos no XML */
    while(itr!=itr_end)
    {
     if(itr->first!=ParsersAttributes::GRANT_OP)
     {
      //Obtém o valor do privilégio (true/false)
      priv_value=(itr->second==ParsersAttributes::_TRUE_);
      grant_op=(itr->second==ParsersAttributes::GRANT_OP);

      //Identifica o tipo de privilégio atual
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

      //Configura o privilégio na permissão
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

  //Redireciona qualquer exceção capturada
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

  //Caso um objeto seja encontrado o qual referencia a coluna
  if(!refs.empty())
   //Dispara um erro informando que a coluna não pde ser remove e qual objeto a referencia
   throw Exception(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE)
                 .arg(QString::fromUtf8(column->getParentTable()->getName(true)) + "." + QString::fromUtf8(column->getName(true)))
                 .arg(column->getTypeName())
                 .arg(QString::fromUtf8(refs[0]->getName(true)))
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

   /* Condição de revalidação de relacionamentos:
      > Caso seja uma coluna e a mesma é referenciada pela chave primária da tabela pai
      > Caso seja uma restrição e a mesma seja uma chave primária da tabela */
    revalidate_rels=((obj_type==OBJ_COLUMN &&
                     parent_tab->isConstraintRefColumn(dynamic_cast<Column *>(object), ConstraintType::primary_key)) ||
                    (obj_type==OBJ_CONSTRAINT &&
                     dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::primary_key));

   /* Caso seja uma coluna, verfica se a tabela pai participa de um relacionamento
     de generalização como tabela de destino (aquela que tem suas colunas copiadas
     para a tabela qua a herda) */
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

   //Caso as duas condições acima sejam atendidas
   if(revalidate_rels || ref_tab_inheritance)
   {
    //(Re)valida os relacionamento e os reconecta
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
  attributes[ParsersAttributes::ENCODING]="'" + (~encoding) + "'";

  if(!localizations[1].isEmpty())
   attributes[ParsersAttributes::LC_COLLATE_DB]="'" + localizations[1] + "'";

  if(!localizations[0].isEmpty())
   attributes[ParsersAttributes::LC_CTYPE_DB]="'" + localizations[0]  + "'";
 }
 else
 {
  attributes[ParsersAttributes::ENCODING]=(~encoding);
  attributes[ParsersAttributes::LC_COLLATE_DB]=localizations[1];
  attributes[ParsersAttributes::LC_CTYPE_DB]=localizations[0];
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
                obj_types[]={ OBJ_LANGUAGE, OBJ_FUNCTION, OBJ_TYPE,
                              OBJ_CAST, OBJ_CONVERSION,
                              OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
                              OBJ_AGGREGATE, OBJ_DOMAIN, OBJ_TEXTBOX, BASE_RELATIONSHIP,
                              OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE };
 try
 {
  general_obj_cnt=this->getObjectCount();
  gen_defs_count=0;

  /* Tratandos os objetos os quais tem ids fixos são eles: Papel, Espaço de Tabela,
     e Esquema. Estes precisam ser tratados separadamente no laço abaixo pois não
     entram na ordenação de ids executada para os demais tipos de objetos. */
  for(i=0; i < 3; i++)
  {
   //Obtém a lista de objetos do tipo
   obj_list=getObjectList(aux_obj_types[i]);

   /* Caso o tipo de definição seja SQL obtem o nome do atributo
      do tipo do objeto nos esquema SQL */
   if(def_type==SchemaParser::SQL_DEFINITION)
   {
    attrib=BaseObject::objs_schemas[aux_obj_types[i]];
    attribs_aux[attrib]="";
   }

   //Obtém o início e o fim da lista para uma varredura
   itr=obj_list->begin();
   itr_end=obj_list->end();

   while(itr!=itr_end)
   {
    //Obtém o objeto atual
    object=(*itr);

    if(object->getObjectType()!=OBJ_SCHEMA ||
       (object->getObjectType()==OBJ_SCHEMA &&
        object->getName()!="public"))
    {
     //Gera o codigo e o concatena com os demais já gerados
     attribs_aux[attrib]+=validateObjectDefinition(object, def_type);
     //Dispara um sinal para sinalizar o progresso da geração do códgio
     gen_defs_count++;
     if(!signalsBlocked())
     {
      emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
                             msg.arg(def_type_str)
                               .arg(QString::fromUtf8(object->getName()))
                               .arg(object->getTypeName()),
                            object->getObjectType());
     }
    }
    itr++;
   }
  }

  /* Armazena os próprio objeto de modelo de objetos pois o mesmo também precisa estar na
     ordenação de objetos para ser criado na ordem correta quando o parser xml ler
     a definição */
  objects_map[this->getObjectId()]=this;
  ids_objs.push_back(this->getObjectId());


  /* Armazenando os demais tipos de objetos no mapa de objetos para
     serem ordenados pelos ids. */
  /* Para definição XML todos os 16 tipos da lista 'tipos_objs' são tratados.
     Para definição SQL são tratados apenas os 12 primeiros (tabelas, relacionamentos, visões e sequências)
     são tratadas separadamente pois existe uma ordem específica em que elas precisam ser criadas e essa
     ordem é definida na interação após a ordenação dos objetos */
  if(def_type==SchemaParser::XML_DEFINITION)
   count=16;
  else
   count=12;

  for(i=0; i < count; i++)
  {
   /* Seleciona a lista de objetos para obter os identificadores e armazenar seus
      elementos no mapa de objetos. No caso de definição SQL apenas os  objetos
      caixa de texto e relacionamento tabela-visão não são obtidos pois os mesmos
      não possuem código SQL */
   if(def_type==SchemaParser::SQL_DEFINITION &&
      (obj_types[i]==OBJ_TEXTBOX || obj_types[i]==BASE_RELATIONSHIP))
    obj_list=NULL;
   else
    //Obtém a lista de objeto de acorodo com o tipo
    obj_list=getObjectList(obj_types[i]);

   if(obj_list)
   {
    //Obtém o início e o fim da lista para uma varredura
    itr=obj_list->begin();
    itr_end=obj_list->end();

    while(itr!=itr_end)
    {
     //Obtém o objeto atual
     object=(*itr);
     //Armazena o objeto em si no mapa de objetos
     objects_map[object->getObjectId()]=object;
     //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
     ids_objs.push_back(object->getObjectId());
     itr++;
    }
   }
  }

  /* ** Trecho especial **:
     Obtenção e armazenamento dos objetos especiais (os quais
     referenciam colunas de tabelas adicionadas por relacionamentos)
     no mapa de objetos e seus ids em um vetor auxiliar de ids 'vet_id_objs_tab'.
     Para isso a lista de tabelas é varrida novamente e suas restrições e índices
     são validados como especiais ou não. O vetor de ids é concatenado ao vetor de ids principal
     antes de sua ordenação quando a definição é XML ou concatenado após a ordenação para def. SQL, desta
     forma os objetos especiais são criados de forma correta em ambas as linguagens */
   itr=tables.begin();
   itr_end=tables.end();

   while(itr!=itr_end)
   {
    table=dynamic_cast<Table *>(*itr);
    itr++;

    //Varre a lista de restrições da tabela
    count=table->getConstraintCount();
    for(i=0; i < count; i++)
    {
     constr=table->getConstraint(i);

     /* Caso a restrição seja um objeto especial armazena o mesmo no mapa de objetos.
        Idenpendente da configuração, chaves estrangeiras sempre serão descartadas nesta
        iteração pois ao final do método as mesmas tem seu código SQL concatenado   definição
        do modelo */
     if((def_type==SchemaParser::XML_DEFINITION ||
         (def_type==SchemaParser::SQL_DEFINITION &&
          constr->getConstraintType()!=ConstraintType::foreign_key)) &&

        (!constr->isAddedByLinking() &&
          ((constr->getConstraintType()!=ConstraintType::primary_key && constr->isReferRelationshipAddedColumn()) ||
           (constr->getConstraintType()==ConstraintType::foreign_key))))
     {
      //Armazena o objeto em si no mapa de objetos
      objects_map[constr->getObjectId()]=constr;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      ids_tab_objs.push_back(constr->getObjectId());
     }
    }

    //Varre a lista de gatilhos da tabela
    count=table->getTriggerCount();
    for(i=0; i < count; i++)
    {
     trigger=table->getTrigger(i);

     //Caso o gatilho seja um objeto especial armazena-o no mapa de objetos
     if(trigger->isReferRelationshipAddedColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      objects_map[trigger->getObjectId()]=trigger;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      ids_tab_objs.push_back(trigger->getObjectId());
     }
    }

    //Varre a lista de índices da tabela
    count=table->getIndexCount();
    for(i=0; i < count; i++)
    {
     index=table->getIndex(i);

     //Caso o índice seja um objeto especial armazena-o no mapa de objetos
     if(index->isReferRelationshipAddedColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      objects_map[index->getObjectId()]=index;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      ids_tab_objs.push_back(index->getObjectId());
     }
    }
   }

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     antes da ordenação caso a definição seja XML */
  if(def_type==SchemaParser::XML_DEFINITION)
   ids_objs.insert(ids_objs.end(), ids_tab_objs.begin(), ids_tab_objs.end());

  //Ordena o vetor de identificadores em ordem crescente
  sort(ids_objs.begin(), ids_objs.end());

  /* CASO ESPECIAL: Gerando a SQL de tabelas, visões, relacionamentos e sequencias de forma ordenada.

     Esta geração é feita da seguinte forma:
     1) Com a lista de relacionamentos, gera o SQL de ambas as tabelas do relacionametno e logo em seguida a
        SQL do relacionametno em si.
     2) Para as demais tabelas que não participaram de relacionamentos, gerar o SQL de cada uma.
     3) As sequencias devem ter seus SQL gerados após as tabelas restantes
     4) Visões devem ser as últimas a terem o SQL gerado para não terem referências a colunas e tabelas quebradas
  */
  if(def_type==SchemaParser::SQL_DEFINITION)
  {
   BaseObject *objs[3]={NULL, NULL, NULL};
   vector<BaseObject *> vet_aux;
   count=16;

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

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     após a ordenação caso a definição seja SQL */
  if(def_type==SchemaParser::SQL_DEFINITION)
   ids_objs.insert(ids_objs.end(), ids_tab_objs.begin(), ids_tab_objs.end());

  attribs_aux[ParsersAttributes::SHELL_TYPES]="";

  /* Caso a definição seja SQL e existam tipos definidos pelo usuário
     faz a conversão dos parâmetros das funções usadas internamente
     por estes */
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

  //Varre a lista de ids de objetos para obtê-los do mapa de objetos
  itr1=ids_objs.begin();
  itr1_end=ids_objs.end();

  attrib=ParsersAttributes::OBJECTS;
  while(itr1!=itr1_end)
  {
   /* Obtém o objeto do mapa a partir do seu identificador
      armazenado no iterador atual */
   object=objects_map[(*itr1)];

   //Obtém o tipo do objeto
   obj_type=object->getObjectType();
   itr1++;

   /* Caso seja um objeto tipo e a definição seja SQL armazena a
      definição shell dos tipos os quais são declarados antes da criação
      dos tipos definidos pelo usuário */
   if(obj_type==OBJ_TYPE && def_type==SchemaParser::SQL_DEFINITION)
   {
    usr_type=dynamic_cast<Type *>(object);

    //Obtendo a definição do tipo em forma de shell type
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
    /* Desprezando as linguagens c e sql
       pois as mesmas não precisam ser declaradas explicitamente poir serem built-in */
    if(//tipo_def==ParserEsquema::DEFINICAO_SQL &&
       (obj_type==OBJ_LANGUAGE &&
        (object->getName()==~LanguageType("c") ||
         object->getName()==~LanguageType("sql") ||
         object->getName()==~LanguageType("plpgsql"))))
     attribs_aux[attrib]+="";
    else
     attribs_aux[attrib]+=validateObjectDefinition(object, def_type);
   }

   //Dispara um sinal para sinalizar o progresso da geração do códgio
   gen_defs_count++;
   if(!signalsBlocked())
   {
    emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
                           msg.arg(def_type_str)
                              .arg(QString::fromUtf8(object->getName()))
                              .arg(object->getTypeName()),
                           object->getObjectType());
   }
  }

  //Creates the SQL definition for user added foreign keys
  if(def_type==SchemaParser::SQL_DEFINITION)
  {
   while(!fks.empty())
   {
    attribs_aux[attrib]+=fks.back()->getCodeDefinition(def_type, true);
    fks.pop_back();
   }
  }

  //Gerando a definição sql/xml das permissões
  itr=permissions.begin();
  itr_end=permissions.end();
  attribs_aux[ParsersAttributes::PERMISSION]="";

  while(itr!=itr_end)
  {
   attribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permission *>(*itr)->getCodeDefinition(def_type);

   //Dispara um sinal para sinalizar o progresso final da geração de código
   gen_defs_count++;
   if(!signalsBlocked())
   {
    emit s_objectLoaded((gen_defs_count/general_obj_cnt) * 100,
                           msg.arg(def_type_str)
                              .arg(QString::fromUtf8((*itr)->getName()))
                              .arg(object->getTypeName()),
                           object->getObjectType());
   }

   itr++;
  }

  //Configura os atributos específicos do modelo de banco
  attribs_aux[ParsersAttributes::MODEL_AUTHOR]=author;

  if(def_type==SchemaParser::XML_DEFINITION)
  {
   attribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? "1" : "");
  }
  else
  {
   /* Ao final da obtenção da definição SQL do modelo, faz a conversão inversa
      dos parâmetros das funções usadas pelos tipos base */
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
   /* Em caso de erro na geração da definição SQL do modelo, faz a conversão inversa
     dos parâmetros das funções usadas pelos tipos base */
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

 //Armazena o atributo que indica se a exportação é para arquivo ou não
 attribs_aux[ParsersAttributes::EXPORT_TO_FILE]=(export_file ? "1" : "");

 //Retorna a definição do modelo completa
 return(SchemaParser::getCodeDefinition(ParsersAttributes::DB_MODEL, attribs_aux, def_type));
}

void DatabaseModel::saveModel(const QString &filename, unsigned def_type)
{
 QString str_aux;
 QFile output(filename);

 output.open(QFile::WriteOnly);

 //Caso não consiga abrir o arquivo para gravação
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
 //Caso o objeto esteja alocado e o mesmo ainda não exista na lista de dependências
 if(object && std::find(deps.begin(), deps.end(), object)==deps.end())
 {
  //Adiciona o objeto na lista de dependências
  deps.push_back(object);


 if((deps.size()==1 && !inc_indirect_deps) || inc_indirect_deps)
 {
  ObjectType obj_type=object->getObjectType();

  /* Caso o objeto possua esquema, espaço de tabela e dono,
     busca e inclui se necessário as dependências desses
     objetos na lista */
  if(object->getSchema() && inc_indirect_deps)
   getObjectDependecies(object->getSchema(), deps, inc_indirect_deps);

  if(object->getTablespace() && inc_indirect_deps)
   getObjectDependecies(object->getTablespace(), deps, inc_indirect_deps);

  if(object->getOwner()  && inc_indirect_deps)
   getObjectDependecies(object->getOwner(), deps, inc_indirect_deps);

  //** Obtendo as dependências de Classe de Operadores **
  if(obj_type==OBJ_OPCLASS)
  {
   OperatorClass *op_class=dynamic_cast<OperatorClass *>(object);

   /* Obtém a referência para o tipo de dado da classe, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
  BaseObject *usr_type=getObjectPgSQLType(op_class->getDataType());
  //obterObjeto(*classe_op->obterTipoDado(), OBJETO_TIPO);

   if(usr_type)
    getObjectDependecies(usr_type, deps, inc_indirect_deps);

   //Caso haja uma família de operadores obtém as dependências dela também
   if(op_class->getFamily())
    getObjectDependecies(op_class->getFamily(), deps, inc_indirect_deps);
  }
  //** Obtendo as dependências de Domínios **
  else if(obj_type==OBJ_DOMAIN)
  {
   /* Obtém a referência para o tipo de dado do domínio, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
   BaseObject *usr_type=getObjectPgSQLType(dynamic_cast<Domain *>(object)->getType());
   //obterObjeto(*dynamic_cast<Dominio *>(objeto)->obterTipo(), OBJETO_TIPO);

   if(usr_type)
    getObjectDependecies(usr_type, deps, inc_indirect_deps);
  }
  //** Obtendo as dependências de Conversões de Codificação **
  else if(obj_type==OBJ_CONVERSION)
  {
   //Obtém as dependências da função de conversão que define a conversão de codificação
   Function *func=dynamic_cast<Conversion *>(object)->getConversionFunction();
   getObjectDependecies(func, deps, inc_indirect_deps);
  }
  //** Obtendo as dependências de Conversões de Tipo **
  else if(obj_type==OBJ_CAST)
  {
   Cast *cast=dynamic_cast<Cast *>(object);
   BaseObject *usr_type=NULL;

   //Obtém as dependências dos tipos usados na conversão de tipo
   for(unsigned i=Cast::SRC_TYPE; i <= Cast::DST_TYPE; i++)
   {
    usr_type=getObjectPgSQLType(cast->getDataType(i));
      //obterObjeto(*conv->obterTipoDado(i), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }

   //Obtém as dependências da função de conversão que define a conversão de tipo
   getObjectDependecies(cast->getCastFunction(), deps, inc_indirect_deps);
  }
  //** Obtendo as dependências de Funções **
  else if(obj_type==OBJ_FUNCTION)
  {
   Function *func=dynamic_cast<Function *>(object);
   BaseObject *usr_type=getObjectPgSQLType(func->getReturnType());
     //obterObjeto(*func->obterTipoRetorno(), OBJETO_TIPO);
   unsigned count, i;

   //Caso a linguagem da função não seja C ou SQL obtém as dependências da mesma
   if(func->getLanguage()->getName()!=~LanguageType("c") &&
      func->getLanguage()->getName()!=~LanguageType("sql"))
    getObjectDependecies(func->getLanguage(), deps, inc_indirect_deps);

   //Obtém as dependências do tipo de retorno caso o mesmo seja um tipo definido pelo usuário
   if(usr_type)
    getObjectDependecies(usr_type, deps, inc_indirect_deps);

   //Obtém as dependências dos tipos dos parâmetros
   count=func->getParameterCount();
   for(i=0; i < count; i++)
   {
    usr_type=getObjectPgSQLType(func->getParameter(i).getType());
    //obterObjeto(*func->obterParametro(i).obterTipo(), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }

   //Obtém as dependências dos elementos da tabela de retorno
   count=func->getReturnedTableColumnCount();
   for(i=0; i < count; i++)
   {
    usr_type=getObjectPgSQLType(func->getReturnedTableColumn(i).getType());
    //obterObjeto(*func->obterTipoRetTabela(i).obterTipo(), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Funções de Agregação **
  else if(obj_type==OBJ_AGGREGATE)
  {
   Aggregate *aggreg=dynamic_cast<Aggregate *>(object);
   BaseObject *usr_type=NULL;
   unsigned count, i;

   //Obtém as dependências das funções que definem a função de agregação
   for(i=Aggregate::FINAL_FUNC; i <= Aggregate::TRANSITION_FUNC; i++)
    getObjectDependecies(aggreg->getFunction(i), deps, inc_indirect_deps);

   //Obtém a dependência do tipo de estado da função de agregação
   usr_type=getObjectPgSQLType(aggreg->getStateType());
     //obterObjeto(*func->obterTipoEstado(), OBJETO_TIPO);

   if(usr_type)
    getObjectDependecies(usr_type, deps, inc_indirect_deps);

   //Obtém as dependências do operador de ordenação caso este esteja alocado
   if(aggreg->getSortOperator())
    getObjectDependecies(aggreg->getSortOperator(), deps, inc_indirect_deps);

   //Obtém as dependências dos tipos de dados usados na função de agregação
   count=aggreg->getDataTypeCount();
   for(i=0; i < count; i++)
   {
    usr_type=getObjectPgSQLType(aggreg->getDataType(i));
      //obterObjeto(*func->obterTipoDado(i), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Linguagens **
  else if(obj_type==OBJ_LANGUAGE)
  {
   Language *lang=dynamic_cast<Language *>(object);

   for(unsigned i=Language::VALIDATOR_FUNC; i <= Language::INLINE_FUNC; i++)
   {
    if(lang->getFunction(i))
     getObjectDependecies(lang->getFunction(i), deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Operadores **
  else if(obj_type==OBJ_OPERATOR)
  {
   Operator *oper=dynamic_cast<Operator *>(object);
   BaseObject *usr_type=NULL;
   unsigned i;

   //Obtém as dependências das funções usadas pelo operador
   for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
   {
    if(oper->getFunction(i))
     getObjectDependecies(oper->getFunction(i), deps, inc_indirect_deps);
   }

   //Obtém as dependências dos tipos dos argumentos do operador
   for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
   {
    usr_type=getObjectPgSQLType(oper->getArgumentType(i));
      //obterObjeto(*oper->obterTipoDadoArgumento(i), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }

   //Obtém as dependências dos operadores auxiliares
   for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
   {
    if(oper->getOperator(i))
     getObjectDependecies(oper->getOperator(i), deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Papéis **
  else if(obj_type==OBJ_ROLE)
  {
   Role *role=dynamic_cast<Role *>(object);
   unsigned i, i1, count,
            role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

   //Obtém as dependências dos papéis membros, papéis admins e papéis referenciados
   for(i=0; i < 3; i++)
   {
    count=role->getRoleCount(role_types[i]);
    for(i1=0; i1 < count; i1++)
     getObjectDependecies(role->getRole(role_types[i], i1), deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Relacionamentos **
  else if(obj_type==OBJ_RELATIONSHIP)
  {
   Relationship *rel=dynamic_cast<Relationship *>(object);
   BaseObject *usr_type=NULL;
   Constraint *constr=NULL;
   unsigned i, count;

   //Obtém as dependências das tabelas referenciadas pelo relacionamento
   getObjectDependecies(rel->getTable(Relationship::SRC_TABLE), deps, inc_indirect_deps);
   getObjectDependecies(rel->getTable(Relationship::DST_TABLE), deps, inc_indirect_deps);

   //Obtém as dependências dos tipos usados pelos atributos do relacionamento
   count=rel->getAttributeCount();
   for(i=0; i < count; i++)
   {
    usr_type=getObjectPgSQLType(rel->getAttribute(i)->getType());
      //obterObjeto(*rel->obterAtributo(i)->obterTipo(), OBJETO_TIPO);

    if(usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }

   //Obtém as dependências das restrições (somente chave estrangeira) do relacionamento
   count=rel->getConstraintCount();
   for(i=0; i < count; i++)
   {
    constr=dynamic_cast<Constraint *>(rel->getConstraint(i));
    if(constr->getConstraintType()==ConstraintType::foreign_key)
     getObjectDependecies(constr->getReferencedTable(), deps, inc_indirect_deps);

    if(constr->getTablespace())
     getObjectDependecies(constr->getTablespace(), deps, inc_indirect_deps);
   }
  }
  //** Obtendo as dependências de Sequências **
  else if(obj_type==OBJ_SEQUENCE)
  {
   Sequence *seq=dynamic_cast<Sequence *>(object);
   if(seq->getOwnerColumn())
    getObjectDependecies(seq->getOwnerColumn()->getParentTable(), deps, inc_indirect_deps);
  }
  //** Obtendo as dependências de Tabelas **
  else if(obj_type==OBJ_TABLE)
  {
   Table *tab=dynamic_cast<Table *>(object);
   BaseObject *usr_type=NULL;
   Constraint *constr=NULL;
   Trigger *trig=NULL;
   Index *index=NULL;
   Column *col=NULL;
   unsigned count, count1, i, i1;

   //Obtém as dependências dos tipos das colunas não incluídas por relacionamento
   count=tab->getColumnCount();
   for(i=0; i < count; i++)
   {
    col=tab->getColumn(i);
    usr_type=getObjectPgSQLType(col->getType());
      //obterObjeto(*col->obterTipo(), OBJETO_TIPO);

    if(!col->isAddedByLinking() && usr_type)
     getObjectDependecies(usr_type, deps, inc_indirect_deps);
   }

   //Obtém as dependências das restrições não incluídas por relacionamento
   count=tab->getConstraintCount();
   for(i=0; i < count; i++)
   {
    constr=dynamic_cast<Constraint *>(tab->getConstraint(i));
    if(inc_indirect_deps &&
       !constr->isAddedByLinking() &&
        constr->getConstraintType()==ConstraintType::foreign_key)
     getObjectDependecies(constr->getReferencedTable(), deps, inc_indirect_deps);

    if(!constr->isAddedByLinking() && constr->getTablespace())
     getObjectDependecies(constr->getTablespace(), deps, inc_indirect_deps);
   }

   //Obtém as dependências das tabelas referenciadas nos gatilhos e as funções
   count=tab->getTriggerCount();
   for(i=0; i < count; i++)
   {
    trig=dynamic_cast<Trigger *>(tab->getTrigger(i));
    if(trig->getReferencedTable())
     getObjectDependecies(trig->getReferencedTable(), deps, inc_indirect_deps);

    if(trig->getFunction())
     getObjectDependecies(trig->getFunction(), deps, inc_indirect_deps);

    //qtd1=gat->obterNumColunas();
    //for(i1=0; i1 < qtd1; i1++)
    //obterDependenciasObjeto(gat->obterColuna(i1), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das colunas ou classe de operadores usados nos elementos do índices
   count=tab->getIndexCount();
   for(i=0; i < count; i++)
   {
    index=dynamic_cast<Index *>(tab->getIndex(i));
    count1=index->getElementCount();

    for(i1=0; i1 < count1; i1++)
    {
     if(index->getElement(i1).getOperatorClass())
      getObjectDependecies(index->getElement(i1).getOperatorClass(), deps, inc_indirect_deps);
     else if(index->getElement(i1).getColumn())
     {
      usr_type=getObjectPgSQLType(index->getElement(i1).getColumn()->getType());
        //obterObjeto(*ind->obterElemento(i1).obterColuna()->obterTipo(), OBJETO_TIPO);

      if(usr_type)
       getObjectDependecies(usr_type, deps, inc_indirect_deps);
     }
    }
   }
  }
  //** Obtendo as dependências de Tipos definidos pelo usuário **
  else if(obj_type==OBJ_TYPE)
  {
   Type *usr_type=dynamic_cast<Type *>(object);
   BaseObject *aux_type=NULL;
   unsigned count, i;

   //Caso o tipo de usuário seja tipo base
   if(usr_type->getConfiguration()==Type::BASE_TYPE)
   {
    //Obtém as dependências do tipo de cópia
    aux_type=getObjectPgSQLType(usr_type->getLikeType());
      //obterObjeto(*tipo_usr->obterTipoCopia(), OBJETO_TIPO);

    if(aux_type)
     getObjectDependecies(aux_type, deps, inc_indirect_deps);

    //Obtém as dependências das funções usadas pelo tipo
    for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
     getObjectDependecies(usr_type->getFunction(i), deps, inc_indirect_deps);
   }
   //Caso seja um tipo composto
   else if(usr_type->getConfiguration()==Type::COMPOSITE_TYPE)
   {
    //Obtém as dependências dos tipos dos atributos
    count=usr_type->getAttributeCount();
    for(i=0; i < count; i++)
    {
     aux_type=getObjectPgSQLType(usr_type->getAttribute(i).getType());
       //obterObjeto(*tipo_usr->obterAtributo(i).obterTipo(), OBJETO_TIPO);

     if(aux_type)
      getObjectDependecies(aux_type, deps, inc_indirect_deps);
    }
   }
  }
  //** Obtendo as dependências de Visões **
  else if(obj_type==OBJ_VIEW)
  {
   View *view=dynamic_cast<View *>(object);
   unsigned i, count;

   //Obtém as dependências das tabelas referenciadas pela visão
   count=view->getReferenceCount();
   for(i=0; i < count; i++)
   {
    if(view->getReference(i).getTable())
     getObjectDependecies(view->getReference(i).getTable(), deps, inc_indirect_deps);
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
  ObjectType obj_type=object->getObjectType();
  bool refer=false;

  if(obj_type==OBJ_TABLE)
  {
   Table *table=dynamic_cast<Table *>(object);
   Sequence *seq=NULL;
   Constraint *constr=NULL;
   Table *tab=NULL;
   Trigger *gat=NULL;
   BaseRelationship *base_rel=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, count;

   /* Vericando se existe algum relacionamento (tabela-tabela)
      o qual um dos objetos participantes é a tabela
      do parâmetro */
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

   /* Vericando se existe alguma sequência a qual referencia
      a tabela do parâmetro */
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

   /* Vericando se existe alguma tabela cuja uma tabela cópia é
      a própria tabela a ser excluída, verifica também se as
      restrições das tabelas, especificamente, as chaves estrangeiras
      estão referenciado a tabela */
   itr=tables.begin();
   itr_end=tables.end();

   while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
   {
    tab=dynamic_cast<Table *>(*itr);

    //Verificando as restrições
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

    //Verificando os gatilhos
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

   /* Vericando se existe algum relacionamento (tabela-visao)
      o qual um dos objetos participantes é a tabela */
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

  if(obj_type==OBJ_FUNCTION)
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

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente uma função */
   for(i=0; i < 7 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
   {
    //Obtém uma lista
    obj_list=getObjectList(obj_types[i]);
    itr=obj_list->begin();
    itr_end=obj_list->end();

    if(obj_types[i]==OBJ_CAST)
    {
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Verifica se o objeto não referencia o papel
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
      //Verifica se o objeto não referencia o papel
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
      //Verifica se o objeto não referencia o papel
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
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);

      //Verifica se o objeto não referencia o papel
      if(oper->getFunction(Operator::FUNC_OPERATOR)==func ||
         oper->getFunction(Operator::FUNC_JOIN)==func  ||
         oper->getFunction(Operator::FUNC_RESTRICTION)==func)
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
      //Obtém a referência ao objeto
      tab=dynamic_cast<Table *>(*itr);
      itr++;
      count=tab->getTriggerCount();
      for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
      {
       trig=tab->getTrigger(i1);
       //Verifica se o gatilho não referencia a função
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
      //Obtém a referência ao objeto
      type=dynamic_cast<Type *>(*itr);
      itr++;

      for(i1=Type::INPUT_FUNC; i1 <= Type::ANALYZE_FUNC && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
      {
       //Verifica se o tipo não referencia a função
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
      //Obtém a referência ao objeto
      lang=dynamic_cast<Language *>(*itr);
      itr++;
      //Verifica se a função a ser removida é uma função handler ou validator da linguagem
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

  if(obj_type==OBJ_SCHEMA)
  {
   vector<BaseObject *> *obj_list=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType obj_types[11]={OBJ_FUNCTION, OBJ_TABLE, OBJ_VIEW,
                             OBJ_DOMAIN, OBJ_AGGREGATE, OBJ_OPERATOR,
                             OBJ_SEQUENCE, OBJ_CONVERSION,
                             OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
   unsigned i;

   /* Varre todas as listas de objetos os quais pode pertencer a
      um esquema e verifica se o mesmos não estão referenciando
      o esquema a ser removido */
   for(i=0; i < 11 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
   {
    //Obtém uma lista
    obj_list=getObjectList(obj_types[i]);
    itr=obj_list->begin();
    itr_end=obj_list->end();

    while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
    {
     //Verifica se o objeto não referencia o esquema
     if((*itr)->getSchema()==object)
     {
      refer=true;
      refs.push_back(*itr);
     }
     itr++;
    }
   }
  }

  if(obj_type==OBJ_TYPE || obj_type==OBJ_DOMAIN ||
     obj_type==OBJ_SEQUENCE || obj_type==OBJ_TABLE)
  {
   vector<BaseObject *> *obj_list=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType obj_types[]={OBJ_TABLE, OBJ_OPCLASS, OBJ_CAST,
                           OBJ_DOMAIN, OBJ_FUNCTION, OBJ_AGGREGATE,
                           OBJ_OPERATOR, OBJ_TYPE };
   unsigned i, i1, count;
   OperatorClass *op_class=NULL;
   Table *tab=NULL;
   Column *col=NULL;
   Cast *cast=NULL;
   Domain *dom=NULL;
   Function *func=NULL;
   Aggregate *aggreg=NULL;
   Operator *oper=NULL;
   Type *type=NULL;
   void *ptr_pgsqltype=NULL;

   /* Devido a particuladade de se atribuir ponteiros de objetos como tipos base do pgsql (ver TipoPgSQL)
      é necessário fazer um dynamic_cast para a classe correta do 'objeto'. Caso o dynamic_cast não seja
      feita, mesmo que o objeto seja um tipopgsql válido o mesmo não será localizado na lista de tipos
      base do pgsql.  */
   switch(obj_type)
   {
    case OBJ_TYPE: ptr_pgsqltype=dynamic_cast<Type*>(object); break;
    case OBJ_DOMAIN: ptr_pgsqltype=dynamic_cast<Domain*>(object); break;
    case OBJ_SEQUENCE: ptr_pgsqltype=dynamic_cast<Sequence*>(object); break;
    default: ptr_pgsqltype=dynamic_cast<Table*>(object); break;
   }

   /* Varre todas as listas de objetos os quais podem
     referenciar direta ou indiretamente um tipo definido
     pelo usuário */
   for(i=0; i < 8 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
   {
    //Obtém uma lista
    obj_list=getObjectList(obj_types[i]);
    itr=obj_list->begin();
    itr_end=obj_list->end();

    if(obj_types[i]==OBJ_TABLE)
    {
     //Verifica se as colunas das tabelas não referenciam o tipo
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a tabela
      tab=dynamic_cast<Table *>(*itr);
      itr++;

      //Varre a lista de colunas da tabela
      count=tab->getColumnCount();
      for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
      {
       col=tab->getColumn(i1);
       //Verifica se o tipo da coluna é o próprio tipo a ser excluído
       if(col->getType()==object)
       {
        refer=true;
        refs.push_back(col);
       }
      }
     }
    }
    else if(obj_types[i]==OBJ_OPCLASS)
    {
     /* Varre a lista de classe operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      op_class=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Verifica se o tipo de dado da classe é o próprio tipo a ser removido
      if(op_class->getDataType()==ptr_pgsqltype)
      {
       refer=true;
       refs.push_back(op_class);
      }
     }
    }
    else if(obj_types[i]==OBJ_DOMAIN)
    {
     /* Varre a lista de domínios e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      dom=dynamic_cast<Domain *>(*itr);
      itr++;

      //Verifica se o tipo de dado do dominio é o próprio tipo a ser removido
      if(dom->getType()==ptr_pgsqltype)
      {
       refer=true;
       refs.push_back(dom);
      }
     }
    }
    else if(obj_types[i]==OBJ_TYPE)
    {
     /* Varre a lista de tipo definidos pelo usuário e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
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
     /* Varre a lista de funções agregadas e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      aggreg=dynamic_cast<Aggregate *>(*itr);
      itr++;

      /* Verifica se os tipos de dados da função agregada referenciam
         tipo a ser removido */
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
     /* Varre a lista de funções e verifica se algum
        objeto (parâmetros e tipo de retorno) referencia
        o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      func=dynamic_cast<Function *>(*itr);
      itr++;

      //Verifica se o tipo de retorno é o próprio tipo a ser removido
      if(func->getReturnType()==ptr_pgsqltype)
      {
       refer=true;
       refs.push_back(func);
      }
      else
      {
       /* Verifica se os tipos de dados dos parâmetros da função
          referenciam tipo a ser removido */
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
     /* Varre a lista de operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);
      itr++;

      //Verifica se um dos argumentos do operador é o próprio tipo a ser removido
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
      //Obtém a referência ao objeto
      cast=dynamic_cast<Cast *>(*itr);
      itr++;

      //Verifica se o objeto não referencia o tipo
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

  if(obj_type==OBJ_ROLE)
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

   /* Caso especial: Varre a lista de papéis e verifica se o papel a ser
      removido se encontra em uma das três listas de papeis da classe
      Papel (ref, membro ou admin) */
   itr=roles.begin();
   itr_end=roles.end();
   while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
   {
    //Obtém um papel
    role_aux=dynamic_cast<Role *>(*itr);
    itr++;

    for(i1=0; i1 < 3 && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
    {
     //Obtém a quantidade de papeis presentes na lista atual (tipo_papel[i1])
     count=role_aux->getRoleCount(role_types[i1]);
     for(i=0; i < count && !refer; i++)
     {
      /* Caso o papel a ser excluído seja igual ao elemento atual
         da lista de papéis do papel aux */
      if(role_aux->getRole(role_types[i1], i)==role)
      {
       refer=true;
       refs.push_back(role_aux);
      }
     }
    }
   }

   /* Varre todas as listas de objetos os quais pode pertencer a
      um dono e verifica se o mesmos não estão referenciando
      o dono a ser removido */
   for(i=0; i < 13 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
   {
    //Obtém uma lista
    obj_list=getObjectList(obj_types[i]);
    itr=obj_list->begin();
    itr_end=obj_list->end();

    while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
    {
     //Verifica se o objeto não referencia o papel
     if((*itr)->getOwner()==role)
     {
      refer=true;
      refs.push_back(*itr);
     }
     itr++;
    }
   }

   /*Caso especial: Verifica se o papel é a ser removido é dono
     do próprio banco de dados */
   if((!exclusion_mode || (exclusion_mode && !refer)) && this->getOwner()==role)
   {
    refer=true;
    refs.push_back(this);
   }
  }

  if(obj_type==OBJ_TABLESPACE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, count;
   Table *tab=NULL;
   Index *ind=NULL;
   Constraint *rest=NULL;

   /* Varre a lista de tabelas e verifica se estas e seus índices
      não estão referenciando o espaço de tabelas a ser removido */
   itr=tables.begin();
   itr_end=tables.end();

   while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
   {
    //Obtém a tabela
    tab=dynamic_cast<Table *>(*itr);

    //Verifica se referencia o espaço de tabela
    if(tab->getTablespace()==object)
    {
     refer=true;
     refs.push_back(tab);
    }


    /* Verifica se algum dos índices da tabela referecia
       o espaço de tabela */
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

    /* Verifica se alguma restrição da tabela referecia
       o espaço de tabela */
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

  if(obj_type==OBJ_LANGUAGE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   Function *func=NULL;

   /* Varre a lista de funções e verifica se estas
      não estão referenciando a linguage a ser removida */
   itr=functions.begin();
   itr_end=functions.end();

   while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
   {
    //Verifica se referencia o espaço de tabela
    func=dynamic_cast<Function *>(*itr);
    if(func->getLanguage()==object)
    {
     refer=true;
     refs.push_back(func);
    }
    itr++;
   }
  }

  if(obj_type==OBJ_OPERATOR)
  {
   vector<BaseObject *> *obj_list=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType obj_types[3]={OBJ_OPCLASS,
                            OBJ_AGGREGATE,
                            OBJ_OPERATOR};
   unsigned i, i1, count;
   OperatorClass *op_class=NULL;
   Operator *oper_aux=NULL, *oper=dynamic_cast<Operator *>(object);

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente um operador */
   for(i=0; i < 3 && (!exclusion_mode || (exclusion_mode && !refer)); i++)
   {
    //Obtém uma lista
    obj_list=getObjectList(obj_types[i]);
    itr=obj_list->begin();
    itr_end=obj_list->end();

    if(obj_types[i]==OBJ_OPCLASS)
    {
     while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
     {
      //Obtém a referência ao objeto
      op_class=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Varre a lista de elementos da classe de operadores
      count=op_class->getElementCount();
      for(i1=0; i1 < count && (!exclusion_mode || (exclusion_mode && !refer)); i1++)
      {
       //Verifica se o objeto não referencia o operador
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
      //Obtém a referência ao objeto
      oper_aux=dynamic_cast<Operator *>(*itr);
      itr++;

      /* Verifica se um dos operadores agregados ao operador atual não referencia
         o objeto não referencia o operador */
      for(i1=Operator::OPER_COMMUTATOR; i1 <= Operator::OPER_GREATER &&
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

  if(obj_type==OBJ_OPFAMILY)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   OperatorFamily *op_family=dynamic_cast<OperatorFamily *>(object);

   /* Varre a lista de classe de operadores e verifica se um
      de seus elementos está referenciando a família de operadores */
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

  if(obj_type==OBJ_COLUMN)
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

    //Varre a lista selecionada verificado se a coluna passada é referenciada
    while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !refer)))
    {
     /* Caso 1: o tipo atual seja sequência, faz o cast para a classe
        e chama do método da sequencia que retorna a coluna possuidora
        da mesma.

        Caso 2: o tipo atual seja visão, faz o cast para a classe
        e chama do método da visão o qual retorna se a coluna
        é referenciada pelo elementos da visão. */
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
       if(tab->getConstraint(idx)->isColumnExists(column, Constraint::SOURCE_COLS) ||
          tab->getConstraint(idx)->isColumnExists(column, Constraint::REFERENCED_COLS))
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
       if(rel->getConstraint(idx)->isColumnExists(column, Constraint::SOURCE_COLS) ||
          rel->getConstraint(idx)->isColumnExists(column, Constraint::REFERENCED_COLS))
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
                         OBJ_TEXTBOX};
 vector<BaseObject *>::iterator itr, itr_end;
 vector<BaseObject *> *obj_list=NULL;
 Textbox *label=NULL;
 BaseRelationship *rel=NULL;
 unsigned i, i1;

 for(i=0; i < 5; i++)
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

  case UserTypeConfig::SEQUENCE_TYPE:
   return(this->getObject(*type, OBJ_SEQUENCE));
  break;

  default:
   return(NULL);
  break;
 }
}

