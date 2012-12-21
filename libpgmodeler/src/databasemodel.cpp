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
 //Retorna a referencia da lista equivalente ao tipo passado
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
  /* Verifica se a definição SQL/XML do objeto é valida por intermédio
     do parser de esquemas, o qual retorna um erro quando existe
     algum problema com a definição sql do objeto */
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
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
    throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                           .arg(QString::fromUtf8(object->getName(true)))
                           .arg(object->getTypeName()),
                  ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   else
    //Caso o parser dispare os demais erros, apenas redireciona o erro
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
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
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

 /* Caso o objeto seja um espaço de tabela verifica se não existem
    outro espaço de tabela que esteja apontando para o mesmo diretório
    o que é considerado erro */
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

   //Caso o diretório dos mesmos sejam iguais um erro é disparado
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

 //Verifica se o objeto a ser inserido já existe no modelo, buscando através do nome.
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
  //Valida a definição sql do objeto
  DatabaseModel::validateObjectDefinition(object, SchemaParser::SQL_DEFINITION);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }

 //Obtém a lista de objetos do tipo do novo objeto
 obj_list=getObjectList(object->getObjectType());

 //Insere o novo elemento na lista
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

 //Emite um sinal indicando a adição do objeto no modelo
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

  //Obtém a lista de acordo com o tipo do objeto
  obj_list=getObjectList(obj_type);
  if(!obj_list)
   throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Caso o índice do objeto não foi especificado o método tenta descobrí-lo
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
    //Remove as permissões do objeto
    removePermissions(object);
    obj_list->erase(obj_list->begin() + obj_idx);
   }
  }

  //Emite um sinal indicando que o objeto foi removido
  if(!signalsBlocked())
   emit s_objectRemoved(object);
 }
}

vector<BaseObject *> DatabaseModel::getObjects(ObjectType obj_type, BaseObject *schema)
{
 vector<BaseObject *> *obj_list=NULL, sel_list;
 vector<BaseObject *>::iterator itr, itr_end;

 //Obtém a lista de acordo com o tipo do objeto
 obj_list=getObjectList(obj_type);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
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

 //Obtém a lista de acordo com o tipo do objeto
 obj_list=getObjectList(obj_type);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!obj_list)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Obtém as referências para o inicio e o fim da lista
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
    /* Caso o nome do objeto atual seja igual ao nome passado
      o while será quebrado */
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
    /* Caso especial para função/oeradores: para saber se uma função/operador está duplicada
       deve-se verificar a assinatura da mesma e não apenas o nome, pois
       o PostgreSQL permite a sobrecarga de funções e operadores. Neste caso especial
       o parâmetro informado ao método deve ser uma assinatura de função/operador
       e não seu nome */
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
   //Obtém a referência do objeto e calcula seu índice
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

 //Obtém a lista de acordo com o tipo do objeto
 obj_list=getObjectList(obj_type);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!obj_list)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o índice do objeto a ser obtido esteja fora do
    intervalo de elementos da lista */
 else if(obj_idx >= obj_list->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(obj_list->at(obj_idx));
}

unsigned DatabaseModel::getObjectCount(ObjectType obj_type)
{
 vector<BaseObject *> *obj_list=NULL;

 //Obtém a lista de acordo com o tipo do objeto
 obj_list=getObjectList(obj_type);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
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
 /* A ordem de destruição de objetos deve ser seguida de forma que
    os objetos menos dependidos sejam destruídos primeiro para
    evitar falha de segmentação onde, no momento da destruição de algum
    objeto, um dos objetos mais dependendidos sejam referenciados porém
    ja foram destruídos anteriormente. */
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
  /* Ao ser inserido uma nova tabela a mesma tem
   seu nome é adicionad  lista de tipos válidos
   do PostgreSQL */
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

  //Obtém as referênca   tabela
  getObjectReferences(table, refs, true);

  //Caso a tabela esteja sendo referenciada, a mesma não pode ser removida
  if(!refs.empty())
  {
   ErrorType err_type;
   unsigned i=0, count=refs.size();

   while(i < count)
   {
     /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
        do objeto referenciado */
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
      BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(refs[i])->getParentTable();

      /* Um erro só é disparado para uma objetos de tabela quando este
         não tem como pai a própria tabela a ser removida */
      if(obj_ref_pai != table)
      {
       //Formata a mensagem caso exista uma referência indireta ao objeto a ser removido
       err_type=ERR_REM_INDIRECT_REFERENCE;
       str_aux=QString(Exception::getErrorMessage(err_type))
               .arg(table->getName(true))
               .arg(table->getTypeName())
               .arg(refs[0]->getName(true))
               .arg(refs[0]->getTypeName())
               .arg(obj_ref_pai->getName(true))
               .arg(obj_ref_pai->getTypeName());

       throw Exception(str_aux, err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
     }

     i++;
    }
   }

  __removeObject(table, obj_idx);

   /* Ao ser removido do modelo a sequencia tem
    seu nome removido da lista de tipos válidos do PostgreSQL */
  PgSQLType::removeUserType(table->getName(true), table);

  //Remove qualquer relacionamento gerado por chave estrangeira
  updateTableFKRelationships(table);
 }
}

void DatabaseModel::addSequence(Sequence *sequence, int obj_idx)
{
 try
 {
  __addObject(sequence, obj_idx);
  /* Ao ser inserido uma nova sequencia a mesma tem
   seu nome é adicionad  lista de tipos válidos
   do PostgreSQL */
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
  /* Ao ser removido do modelo a sequencia tem
   seu nome removido da lista de tipos válidos do PostgreSQL */
  //TipoPgSQL::removerTipoUsuario(sequencia->getName(true), sequencia);
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

  //Caso a tabela foi excluida deve-se remove os relacionamentos
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
  //Atualiza os relacionamentos
  else
  {
   /* Remove os relacionamentos os quais estão inválidos, ou seja,
      a tabela do relacionametno não está sendo mais referenciada pela visao */
   itr1=base_relationships.begin();
   itr1_end=base_relationships.end();

   //Varre a lista de relacionamentos tabela-visão
   idx=0;
   while(itr1!=itr1_end)
   {
    //Obtém a referência ao relacionamento
    rel=dynamic_cast<BaseRelationship *>(*itr1);

    //Caso a visão seja um dos elementos do relacionamento
    if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK &&
       rel->getTable(BaseRelationship::SRC_TABLE)==table)
    {
     ref_tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));

       //Caso a visão não referencie mais a tabela
     if(!table->isReferTableOnForeignKey(ref_tab))
     {
      //Remove o relacionamento
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

   while(itr!=itr_end)
   {
    fk=(*itr);
    ref_tab=dynamic_cast<Table *>(fk->getReferencedTable());
    itr++;

    /* Caso a tabela exista, um relacionamento tabela-visão será automaticamente criado
       (caso este já não existe) e inserido no modelo */
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
  /* Quando uma visão é excluída, os relacionamentos tabela-visão os quais
     possuem a visão como um dos elementos serão excluídos automaticamente */
  itr=base_relationships.begin();
  itr_end=base_relationships.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<BaseRelationship *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->getTable(BaseRelationship::SRC_TABLE)==view ||
      rel->getTable(BaseRelationship::DST_TABLE)==view)
   {
    //Remove o relacionamento
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
  /* Remove os relacionamentos visão-tabela os quais estão inválidos, ou seja,
     a tabela do relacionametno não está sendo mais referenciada pela visao */
  itr=base_relationships.begin();
  itr_end=base_relationships.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<BaseRelationship *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->getTable(BaseRelationship::SRC_TABLE)==view ||
      rel->getTable(BaseRelationship::DST_TABLE)==view)
   {
    //Obtém a tabela do relacionamento
    if(rel->getTable(BaseRelationship::SRC_TABLE)->getObjectType()==OBJ_TABLE)
     tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::SRC_TABLE));
    else
     tab=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));

    //Caso a visão não referencie mais a tabela
    if(!view->isReferencingTable(tab))
    {
     //Remove o relacionamento
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

  /* Cria automaticamente os relacionamentos entre as tabelas e a visão.
     As tabelas referenciadas são obtidas das referências na parte SELECT
     da consulta que gera a visão  */
  ref_count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

  for(i=0; i < ref_count; i++)
  {
   ref=view->getReference(i, Reference::SQL_REFER_SELECT);
   tab=ref.getTable();

   /* Caso a tabela exista, um relacionamento tabela-visão será automaticamente criado
      (caso este já não existe) e inserido no modelo */
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

  /* Varre a lista geral de relacionamentos
     do último ao primeiro, desconectando os últimos relacionamentos primeiro
     para evitar falha de segmentação */
  ritr_rel=relationships.rbegin();
  ritr_rel_end=relationships.rend();

  while(ritr_rel!=ritr_rel_end)
  {
   //Converte o ponteiro de relacionamento da conexão para a classe base de relacionametno
   base_rel=dynamic_cast<BaseRelationship *>(*ritr_rel);
   ritr_rel++;

   //Caso o relacionamento obtido da lista seja entre tabelas
   if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Converte o objeto para relacionamento tabela-tabela
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

 //Obtém os iterador para varredura inicial na lista global de relacionamentos
 itr=relationships.begin();
 itr_end=relationships.end();

 do
 {
  /* Marca como falso a flag indicativa de relacionamento invalidado.
     Este flag sempre será marcado quando 1 ou mais relacionamentos
     forem encontrados durante as interações. */
  found_inval_rel=false;

  while(itr!=itr_end)
  {
   //Obtém um relacionamento a partir do iterador
   base_rel=dynamic_cast<BaseRelationship *>(*itr);
   itr++;

   //Caso o relacionamento obtido seja tabela-tabela
   if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Converte o relacionamento base para rel. tabela-tabela
    rel=dynamic_cast<Relationship *>(base_rel);

    //Caso o relacionamento esteja invalidado
    if(rel->isInvalidated())
    {
     //Insere-o na lista de relacionamentos invalidados
     vet_rel_inv.push_back(base_rel);
     /* Marca a flag indicando que pelo menos 1 relacionamento
        invalidado foi encontrado */
     found_inval_rel=true;
    }
    else
     //Caso não esteja invalidado, insere-o na lista de relacionamentos válidos
     vet_rel.push_back(base_rel);
   }
  }

  //Caso haja algum relacionamento invalidado
  if(found_inval_rel || !xml_special_objs.empty())
  {
   if(!loading_model && xml_special_objs.empty())
    storeSpecialObjectsXML();

   //Desconecta todos os relacionamentos
   disconnectRelationships();

   //Concatena ambas as lista para reconexão dos relacionamentos
   rels=vet_rel;

   /* Os relacionamentos invalidados vão para o final da lista pois
      estes precisam ser conectados por último para se tornarem válidos */
   rels.insert(rels.end(), vet_rel_inv.begin(), vet_rel_inv.end());

   /* Após criação da lista de relacionamentos temporária (rels) limpa
      as duas listas de relacionamentos usadas na iteração */
   vet_rel.clear();
   vet_rel_inv.clear();

   //Varre a lista temporária de relacionamentos
   itr=rels.begin();
   itr_end=rels.end();
   idx=0;

   while(itr!=itr_end)
   {
    //Obtém o relacionamento
    rel=dynamic_cast<Relationship *>(*itr);
    itr_ant=itr;

    //Passa para o próximo relacionamento da lista
    itr++;

    try
    {
     //Executa a conexão do relacionamento
     rel->connectRelationship();
     idx++;
    }
    /* Caso um erro seja disparado durante a conexão do relacionamento o
       mesmo é considerado inválido permanentemente sendo necessário
       sua remoção do modelo */
    catch(Exception &e)
    {
     /* Remove o relacionamento chamando o método de remoção
        de objetos se verificação */
     __removeObject(rel);

     /* Remove o iterador da lista de relacionamentos inválidos para
        evitar que o mesmo seja verificado novamente */
     rels.erase(itr_ant);

     /* Reconfigura a navegação na lista de relacionamentos invalidados.
        Para evitar de que a lista seja varrida novamente desde seu início
        a variável auxiliar 'idx' é usada para apontar o iterador atual
        para o elemento posterior ao que foi removido */
     itr_end=rels.end();
     itr=rels.begin() + idx;

     //Desaloca o relacionamento
     delete(rel);

     //Armazena o erro capturado na lista de erros
     errors.push_back(e);
    }
   }

   /* A nova lista (rels) criada com a nova ordenação de relacionamentos
      passará a ser a lista de relacionamentos a ser varrida nas iterações
      pois é nela que a ordenação de conexão dos relacionamentos é rearranjada. */
   itr=rels.begin();

   /* Caso o flag de modelo em carregamento não esteja marcado será
      feita a recriação dos objetos. Os objetos especiais são recriados
      a cada revalidação dos relacionamentos, sendo assim, tais objetos
      especiais em um dado momento podem ser criados com sucesso e em outros
      não ser criados por quebrarem as referências devido a conexão e
      desconexão constante dos relacionamentos. Quando um erro é encontrado
      na recriação de um objeto especial o mesmo é armazenado em um mapa de
      controle de erros do objeto, onde a chave deste mapa é o identificador
      do próprio objeto gerador do erro. Enquanto tal objeto não for recriado
      com sucesso o erro de geração é armazenado. Ao final, quando todos os
      relacionamentos estiverem sido válidados e o mapa de erros estiver com
      algum elemento de erro, os mesmos são disparados ao usuário indicando
      que alguns objetos especiais não puderam ser criados na validação dos
      relacionamentos. */
   //Obtém os iteradores da lista de xml dos objetos especiais
   itr1=xml_special_objs.begin();
   itr1_end=xml_special_objs.end();

   if(!loading_model && itr1!=itr1_end)
   {
    do
    {
     try
     {
      //Tenta criar o objeto especial a partir do iterador atual
      createSpecialObject(itr1->second, itr1->first);

      /* Caso algum erro anterior de criação do objeto especial em questão for
         detectado o mesmo é removido do mapa de controle de erros */
      if(error_map.count(itr1->first))
       error_map.erase(error_map.find(itr1->first));

      /* Remove a definição xml do objeto atual da lista indicando
         que o mesmo foi criado com sucesso */
      xml_special_objs.erase(itr1);

      //Reinicia a navegação na lista de objetos especiais
      itr1=xml_special_objs.begin();
      itr1_end=xml_special_objs.end();
     }
     catch(Exception &e)
     {
      /* Caso uma exceção for caputarada na criação do objeto especial
         armazena o erro no mapa de erros sendo que a chave do mapa
         é o identificador do objeto gerador do erro */
      error_map[itr1->first]=e;
      //Passa para o próximo objeto especial para tentar sua recriação
      itr1++; idx++;
     }
    }
    while(itr1!=itr1_end);
   }
  }
 }
 //Continua a iteração enquanto houver relacionamentos invalidados
 while(found_inval_rel);

 /* Caso hajam elementos no mapa de controle de erros de criação
    dos objetos especiais, os mesmos são inseridos na lista geral
    de erros de validação para serem disparados ao final do método */
 itr2=error_map.begin();
 itr2_end=error_map.end();
 while(itr2!=itr2_end)
 {
  //Insere o erro na lista geral de erros
  errors.push_back(itr2->second);
  itr2++;
 }

 /* Caso a lista geral de erros não esteja vazia os erros serão
    disparados ao usuário */
 if(!errors.empty())
 {
  /* Limpa a lista de objetos especiais para garantir que eventuais objetos
    não recriados pela quebra das referências não sejam trabalhados pelo
    método de recriação dos objetos especiais */
  xml_special_objs.clear();

  /* Revalida os relacionamentos gerados por chave estrangeira em caso de erro,
     pois se uma chave estrangeira que referenciava uma coluna que deixou de existir
     o relacionamento que a representa também deve ser removido */
  itr=base_relationships.begin();
  itr_end=base_relationships.end();

  while(itr!=itr_end)
  {
   //Obtém um relacionamento generico
   base_rel=dynamic_cast<BaseRelationship *>(*itr);

   if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
    this->updateTableFKRelationships(dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SRC_TABLE)));

   itr++;
  }

  //Define os objetos como modificados para forçar seu redesenho
  this->setObjectsModified();

  throw Exception(ERR_INVALIDATED_OBJECTS,__PRETTY_FUNCTION__,__FILE__,__LINE__,errors);
 }
}

void DatabaseModel::checkRelationshipRedundancy(Relationship *rel)
{
 try
 {
  unsigned rel_type;

  /* Retorna um erro caso se tente verificar redundância
     de relacionamentos a partir de um relacionamento
     não alocado */
  if(!rel)
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  rel_type=rel->getRelationshipType();

  /* Auto relacionamentos são desconsiderados da verificação.
     Já relacionamentos identificadores ou relacionamentos
     os quais possuem atributos identificadores são verificados */
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

   //Obtém tabela referencia e tabela receptora do relacionamento
   recv_table=rel->getReceiverTable();
   ref_table=rel->getReferenceTable();

   itr=relationships.begin();
   itr_end=relationships.end();

   /* Com base nas tabela obtidas, varre a lista de relacionamento em busca do ciclo.
      Um ciclo será detectado quando a tabela referencia de um relacionamento for
      a tabela recptora do relacionamento usado na validação. */
   while(itr!=itr_end && !found_cycle)
   {
    base_rel=dynamic_cast<BaseRelationship *>(*itr);
    itr++;

    if(base_rel->getObjectType()==OBJ_RELATIONSHIP)
    {
     //Obtém um relacionamento da lista
     rel_aux=dynamic_cast<Relationship *>(base_rel);
     //Obtém o tipo do relacionamento atual
     aux_rel_type=rel_aux->getRelationshipType();
     //Obtém a tabela referência do relacionamento atual
     src_table=rel_aux->getReferenceTable();

     /* Caso a tabela referencia obtida seja igual a tabela receptora
        do relacionamento usado na validação e o tipo do relacionamento
        atual seja o mesmo do relacionamento da validação, isso pode indicar
        um princípio de fechamento de ciclo, para isso a validação prosseguirá
        com a tabela receptora do relacionamento atual até que o a tabela receptora
        seja a própria tabela de referência do relacionamento da validação. */
     if(recv_table==src_table &&  aux_rel_type==rel_type &&
        ((!rel_aux->isSelfRelationship() &&
          (rel_aux->isIdentifier() ||
           rel_aux->hasIndentifierAttribute())) ||
         (aux_rel_type==Relationship::RELATIONSHIP_GEN ||
          aux_rel_type==Relationship::RELATIONSHIP_DEP)))

     {
      //A tabela receptora passará a ser a tabela receptora do relacionamento atual
      recv_table=rel_aux->getReceiverTable();
      //Armazena o nome do relacionamento para disparar erro caso um ciclo seja fechado
      str_aux+=rel_aux->getName() + ", ";
      /* Um ciclo será detectado caso a tabela receptora seja igual a tabela referência
         do relacionamento usado na validação */
      found_cycle=(recv_table==ref_table);
      //Volta ao início da lista de relacionamento para uma nova validação
      itr=relationships.begin();
     }
    }
   }

   /* Caso um ciclo seja encontrado, dispara um erro contendo os nomes dos
      relacionamentos os quais fecham o ciclo */
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
  /* Primeiramente, varre a lista de tabelas em busca de restrições, gatilhos e índices
     os quais possam estar referenciando colunas adicionadas por relacionamentos */
  itr=tables.begin();
  itr_end=tables.end();

  while(itr!=itr_end)
  {
   //Obtém a tabela a partir do iterador atual
   table=dynamic_cast<Table *>(*itr);
   itr++;

   //Varre as listas de restrição e índice
   for(type_id=0; type_id < 3; type_id++)
   {
    //Obtém o a quantidade de objetos do tipo de objeto de tabela atual
    count=table->getObjectCount(tab_obj_type[type_id]);

    //Varre a lista atual de tipo de objetos de tabela
    for(i=0; i < count; i++)
    {
     //Obtém um objeto da lista atual na posição atual
     tab_obj=dynamic_cast<TableObject *>(table->getObject(i, tab_obj_type[type_id]));
     found=false;

     //Caso seja uma restrição
     if(tab_obj_type[type_id]==OBJ_CONSTRAINT)
     {
      //Converte o objeto genérico (ObjetoTabela) para restrição
      constr=dynamic_cast<Constraint *>(tab_obj);

      /* Uma restrição só será considerada como caso especial quando a mesma não foi
         incluída por relacionamento, ou seja, tal restrição a qual referencia colunas
         adicionadas por relacionamentos foi criada pelo usuário. */
      found=(!constr->isAddedByRelationship() &&
            constr->isReferRelationshipAddedColumn() &&
            constr->getConstraintType()!=ConstraintType::primary_key);

      /* Caso uma restrição seja encontrada obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(found)
       xml_special_objs[constr->getObjectId()]=constr->getCodeDefinition(SchemaParser::XML_DEFINITION, true);
     }
     else if(tab_obj_type[type_id]==OBJ_TRIGGER)
     {
      //Converte o objeto tabela genérico em gatilho
      trigger=dynamic_cast<Trigger *>(tab_obj);

      /* O gatilho só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      found=trigger->isReferRelationshipAddedColumn();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(found)
       xml_special_objs[trigger->getObjectId()]=trigger->getCodeDefinition(SchemaParser::XML_DEFINITION);
     }
     //Caso seja um índice
     else
     {
      //Converte o objeto tabela genérico em índice
      index=dynamic_cast<Index *>(tab_obj);

      /* O índice só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      found=index->isReferRelationshipAddedColumn();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(found)
       xml_special_objs[index->getObjectId()]=index->getCodeDefinition(SchemaParser::XML_DEFINITION);
     }

     //Caso algum objeto especial for encontrado
     if(found)
     {
      //Remove o mesmo da tabela possuidora
      table->removeObject(tab_obj->getName(), tab_obj->getObjectType());

      //Remove as permissões existentes para o objeto
      removePermissions(tab_obj);

      //Decrementa os controladores da iteração para reiniciar a varredura
      i--; count--;
     }
    }
   }
  }

  /* Varre a lista de sequencias para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=sequences.begin();
  itr_end=sequences.end();

  while(itr!=itr_end)
  {
   //Obtém a sequencia atual através do iterador atual
   sequence=dynamic_cast<Sequence *>(*itr);
   itr++;

   /* Caso a coluna for incluída por relacionamento considera
      a sequencia como objeto especial */
   if(sequence->isReferRelationshipAddedColumn())
   {
    xml_special_objs[sequence->getObjectId()]=sequence->getCodeDefinition(SchemaParser::XML_DEFINITION);
    removeSequence(sequence);
    delete(sequence);
   }
  }

  /* Varre a lista de visões para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=views.begin();
  itr_end=views.end();

  while(itr!=itr_end)
  {
   //Obtém a visão atual através do iterador atual
   view=dynamic_cast<View *>(*itr);
   itr++;

   /* Caso a visão esteja referenciando uma coluna incluída por
      relacionamento considera a mesma como objeto especial */
   if(view->isReferRelationshipAddedColumn())
   {
    //Armazena a definição XML da visão
    xml_special_objs[view->getObjectId()]=view->getCodeDefinition(SchemaParser::XML_DEFINITION);

    /* Caso hajam relacionamentos ligando a visão e as tabelas referenciadas
       os mesmo também serão armazenados como objetos especiais para posterior
       recriação */

    //Obtém a quantidade de referências as tabelas
    count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

    for(i=0; i < count; i++)
    {
     //Obtém uma referência
     ref=view->getReference(i, Reference::SQL_REFER_SELECT);
     //Obtém a tabela da referência
     table=ref.getTable();

     if(table)
     {
      /* Caso a tabela exista e um relacionamento tabela-visão entra a visão
         em questão e a tabela obtida, o mesmo será obtido do modelo e
         sua definição XML armazenada */
      rel=getRelationship(view, table);

      if(rel)
      {
       //Armazena a definição xml do relacionamento
       xml_special_objs[rel->getObjectId()]=rel->getCodeDefinition();
       //Remove o mesmo do modelo
       removeRelationship(rel);
       //Desaloca o relacionamento obtido
       delete(rel);
      }
     }
    }

    //Remove a visão do modelo e a desaloca
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

void DatabaseModel::createSpecialObject(const QString &def_xml_obj, unsigned id_obj)
{
 ObjectType tipo_obj;
 BaseObject *objeto=NULL;

 try
 {
  //Reinicia o parser XML para nova leitura de buffer arquivo
  XMLParser::restartParser();

  //Carrega o buffer XML do parser com a definição XML do objeto especial
  XMLParser::loadXMLBuffer(def_xml_obj);

  //Identificando o tipo de objeto de acordo com o elemento obtido anteriormente
  tipo_obj=getObjectType(XMLParser::getElementName());

  //Cria o objeto de acordo com o tipo identificado
  if(tipo_obj==OBJ_SEQUENCE)
   objeto=createSequence(true);
  else
   objeto=createObject(tipo_obj);

  if(tipo_obj==OBJ_SEQUENCE)
   addSequence(dynamic_cast<Sequence *>(objeto));
  else if(tipo_obj==OBJ_VIEW)
   addView(dynamic_cast<View *>(objeto));

  /* Modifica o id do objeto para o valor do id passado no parâmetro.
     Como um novo id é atribuído quando o objeto é instanciado e levando
     em conta que os objetos especias são realocados e recebem novo id
     isso é necessário para manter a ordem dos objetos da forma como
     os mesmos foram carregados a partir do arquivo evitando a quebra
     de referências. */
  if(objeto && id_obj!=0)
   objeto->object_id=id_obj;
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, def_xml_obj);
 }
}

void DatabaseModel::addRelationship(BaseRelationship *relacao, int idx_obj)
{
 try
 {
  BaseTable *tab1=NULL, *tab2=NULL;
  QString msg;


  if(relacao)
  {
   /* Caso o relacionamento esteja alocado verifica se já não existe um
      relacionamento no modelo entre as tabelas envolvidas */
   //tipo_rel=relacao->obterTipoObjeto();
   tab1=relacao->getTable(BaseRelationship::SRC_TABLE);
   tab2=relacao->getTable(BaseRelationship::DST_TABLE);

   //Caso exista um relacionamento entre as tabelas será disparado um erro
   if(getRelationship(tab1,tab2))
   {
    //__removerObjeto(relacao);
    msg=Exception::getErrorMessage(ERR_DUPLIC_RELATIONSHIP)
        .arg(tab1->getTypeName())
        .arg(tab1->getName(true))
        .arg(tab2->getTypeName())
        .arg(tab2->getName(true));
    throw Exception(msg,ERR_DUPLIC_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  //Adiciona o objeto no modelo
  if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   checkRelationshipRedundancy(dynamic_cast<Relationship *>(relacao));

  __addObject(relacao, idx_obj);

  if(relacao->getObjectType()==OBJ_RELATIONSHIP)
  {
   //Conecta o novo relacionamento
   dynamic_cast<Relationship *>(relacao)->connectRelationship();

   //Valida os relacionamentos para propagação das colunas
   validateRelationships();
  }
  else
   relacao->connectRelationship();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeRelationship(BaseRelationship *relacao, int idx_obj)
{
 try
 {
  if(getObjectIndex(relacao) >= 0)
  {
   if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Desconecta os relacionamentos
    storeSpecialObjectsXML();
    disconnectRelationships();
   }
   else if(relacao->getObjectType()==BASE_RELATIONSHIP)
    relacao->disconnectRelationship();

   __removeObject(relacao, idx_obj);

   if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Valida os relacionamentos após a remoção de o relacionamento atual
    validateRelationships();
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

BaseRelationship *DatabaseModel::getRelationship(unsigned idx_obj, ObjectType tipo_rel)
{
 //Caso o tipo de relacionamento seja inválido
 if(tipo_rel!=OBJ_RELATIONSHIP && tipo_rel!=BASE_RELATIONSHIP)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(dynamic_cast<BaseRelationship *>(getObject(idx_obj, tipo_rel)));
}

BaseRelationship *DatabaseModel::getRelationship(BaseTable *tab_orig, BaseTable *tab_dest)
{
 vector<BaseObject *>::iterator itr, itr_end;
 vector<BaseObject *> rel_list;
 BaseRelationship *rel=NULL;
 bool enc=false, buscar_tab_unica=false;
 BaseTable *tab1=NULL, *tab2=NULL;

 if(tab_orig)
 {
  /* Caso a tabela de destino não esteja especificada,
     indica que que o método deve procurar por relacionamentos
     onde a tabela de origem (tab_orig)  existe não importando se ela é a
     referência ou a receptora. A execução normal (tab_dest!=NULL)
     o método só retorna um relacionamento onde ambas as tabelas estão
     presentas */
  if(!tab_dest)
  {
   tab_dest=tab_orig;
   buscar_tab_unica=true;
  }

  /* Definindo os iteradores de acordo com os objetos envolvidos
     no relacionamento */

  /* Caso um dos objetos seja uma visão, a lista de relacionamentos
     tabela-visão será varrida, caso contrário a lista de relacionamentos
     tabela-tabela será usada */
  if(tab_orig->getObjectType()==OBJ_VIEW ||
     tab_dest->getObjectType()==OBJ_VIEW)
  {
   itr=base_relationships.begin();
   itr_end=base_relationships.end();
  }
  else
  {
   rel_list.assign(base_relationships.begin(), base_relationships.end());
   rel_list.insert(rel_list.end(), relationships.begin(), relationships.end());
   //itr=relacionamentos.begin();
   //itr_end=relacionamentos.end();
   itr=rel_list.begin();
   itr_end=rel_list.end();
  }

  while(itr!=itr_end && !enc)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<BaseRelationship *>(*itr);
   tab1=rel->getTable(BaseRelationship::SRC_TABLE);
   tab2=rel->getTable(BaseRelationship::DST_TABLE);

   /* Verifica se os elementos do parâmetro coincidem com os elementos
      do relacionamento */
   enc=((tab1==tab_orig && tab2==tab_dest) ||
        (tab2==tab_orig && tab1==tab_dest) ||
        (buscar_tab_unica && (tab1==tab_orig || tab2==tab_orig)));
   if(!enc)
   { rel=NULL; itr++; }
  }
 }

 return(rel);
}

void DatabaseModel::addTextbox(Textbox *caixa, int idx_obj)
{
 try
 {
  __addObject(caixa, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeTextbox(Textbox *caixa, int idx_obj)
{
 __removeObject(caixa, idx_obj);
}

Textbox *DatabaseModel::getTextbox(unsigned idx_obj)
{
 return(dynamic_cast<Textbox *>(getObject(idx_obj, OBJ_TEXTBOX)));
}

void DatabaseModel::addSchema(Schema *esquema, int idx_obj)
{
 try
 {
  __addObject(esquema, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Schema *DatabaseModel::getSchema(unsigned idx_obj)
{
 return(dynamic_cast<Schema *>(getObject(idx_obj, OBJ_SCHEMA)));
}

void DatabaseModel::removeSchema(Schema *esquema, int idx_obj)
{
 if(esquema)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao esquema
  getObjectReferences(esquema, vet_refs, true);

 /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */

   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(esquema->getName(true))
                 .arg(esquema->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(esquema, idx_obj);
 }
}

void DatabaseModel::addRole(Role *papel, int idx_obj)
{
 try
 {
  __addObject(papel, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Role *DatabaseModel::getRole(unsigned idx_obj)
{
 return(dynamic_cast<Role *>(getObject(idx_obj, OBJ_ROLE)));
}

void DatabaseModel::removeRole(Role *papel, int idx_obj)
{
 if(papel)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao papel
  getObjectReferences(papel, vet_refs, true);

 /* Caso o papel esteja sendo referenciado, por algum objeto a
     mesma não pode ser removido */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(papel->getName(true))
                 .arg(papel->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(papel, idx_obj);
 }
}

void DatabaseModel::addTablespace(Tablespace *espaco_tab, int idx_obj)
{
 try
 {
  __addObject(espaco_tab, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tablespace *DatabaseModel::getTablespace(unsigned idx_obj)
{
 return(dynamic_cast<Tablespace *>(getObject(idx_obj, OBJ_TABLESPACE)));
}

void DatabaseModel::removeTablespace(Tablespace *espaco_tab, int idx_obj)
{
 if(espaco_tab)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referências ao espaço de tabela
  getObjectReferences(espaco_tab, vet_refs, true);

  /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referencia direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(espaco_tab->getName(true))
            .arg(espaco_tab->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());
   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();
    //Formata a mensagem para referencia indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(espaco_tab->getName(true))
            .arg(espaco_tab->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());

   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(espaco_tab, idx_obj);
 }
}

void DatabaseModel::addCast(Cast *conv_tipo, int idx_obj)
{
 try
 {
  __addObject(conv_tipo, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeCast(Cast *conv_tipo, int idx_obj)
{
 __removeObject(conv_tipo, idx_obj);
}

Cast *DatabaseModel::getCast(unsigned idx_obj)
{
 return(dynamic_cast<Cast *>(getObject(idx_obj, OBJ_CAST)));
}

void DatabaseModel::addConversion(Conversion *conv_codificacao, int idx_obj)
{
 try
 {
  __addObject(conv_codificacao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeConversion(Conversion *conv_codificacao, int idx_obj)
{
 __removeObject(conv_codificacao, idx_obj);
}

Conversion *DatabaseModel::getConversion(unsigned idx_obj)
{
 return(dynamic_cast<Conversion *>(getObject(idx_obj,
 OBJ_CONVERSION)));
}

void DatabaseModel::addLanguage(Language *linguagem, int idx_obj)
{
 try
 {
  __addObject(linguagem, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Language *DatabaseModel::getLanguage(unsigned idx_obj)
{
 return(dynamic_cast<Language *>(getObject(idx_obj, OBJ_LANGUAGE)));
}

void DatabaseModel::removeLanguage(Language *linguagem, int idx_obj)
{
 if(linguagem)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referênca   linguagem
  getObjectReferences(linguagem, vet_refs, true);

 /* Caso a linguagem esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(linguagem->getName(true))
                 .arg(linguagem->getTypeName())
                 .arg(dynamic_cast<Function *>(vet_refs[0])->getSignature())
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(linguagem, idx_obj);
 }
}

void DatabaseModel::addFunction(Function *funcao, int idx_obj)
{
 try
 {
  __addObject(funcao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Function *DatabaseModel::getFunction(unsigned idx_obj)
{
 return(dynamic_cast<Function *>(getObject(idx_obj, OBJ_FUNCTION)));
}

void DatabaseModel::removeFunction(Function *funcao, int idx_obj)
{
 if(funcao)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referênca   função
  getObjectReferences(funcao, vet_refs, true);

 /* Caso a função esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
            .arg(funcao->getSignature())
            .arg(funcao->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());

   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();
    //Formata a mensagem para referência indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE))
            .arg(funcao->getSignature())
            .arg(funcao->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());

   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(funcao, idx_obj);
 }
}

void DatabaseModel::addAggregate(Aggregate *func_agregacao, int idx_obj)
{
 try
 {
  __addObject(func_agregacao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Aggregate *DatabaseModel::getAggregate(unsigned idx_obj)
{
 return(dynamic_cast<Aggregate *>(getObject(idx_obj, OBJ_AGGREGATE)));
}

void DatabaseModel::removeAggregate(Aggregate *func_agregacao, int idx_obj)
{
 __removeObject(func_agregacao, idx_obj);
}

void DatabaseModel::addDomain(Domain *dominio, int idx_obj)
{
 if(dominio)
 {
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de tipos para verificar se existe algum tipo
     com o mesmo nome do domínio */
  itr=types.begin();
  itr_end=types.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->getName(true)==dominio->getName(true));
   itr++;
  }

  //Caso exista um tipo de mesmo nome que o domínio dispara o erro
  if(enc)
  {
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(dominio->getName(true))
           .arg(dominio->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o domínio
   __addObject(dominio, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionad  lista de tipos válidos
    do PostgreSQL */
   PgSQLType::addUserType(dominio->getName(true), dominio, this, UserTypeConfig::DOMAIN_TYPE);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void DatabaseModel::removeDomain(Domain *dominio, int idx_obj)
{
 try
 {
  removeUserType(dominio, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Domain *DatabaseModel::getDomain(unsigned idx_obj)
{
 return(dynamic_cast<Domain *>(getObject(idx_obj, OBJ_DOMAIN)));
}

void DatabaseModel::addOperatorFamily(OperatorFamily *familia_op, int idx_obj)
{
 try
 {
  __addObject(familia_op, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

OperatorFamily *DatabaseModel::getOperatorFamily(unsigned idx_obj)
{
 return(dynamic_cast<OperatorFamily *>(getObject(idx_obj, OBJ_OPFAMILY)));
}

void DatabaseModel::removeOperatorFamily(OperatorFamily *familia_op, int idx_obj)
{
 if(familia_op)
 {
  vector<BaseObject *> vet_refs;

  //OBtém as referências ao objeto
  getObjectReferences(familia_op, vet_refs, true);

   /* Caso a familía esteja sendo referenciada, por algum objeto a
       mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(familia_op->getName(true))
                 .arg(familia_op->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(familia_op, idx_obj);
 }
}

void DatabaseModel::addOperatorClass(OperatorClass *classe_op, int idx_obj)
{
 try
 {
  __addObject(classe_op, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeOperatorClass(OperatorClass *classe_op, int idx_obj)
{
 __removeObject(classe_op, idx_obj);
}

OperatorClass *DatabaseModel::getOperatorClass(unsigned idx_obj)
{
 return(dynamic_cast<OperatorClass *>(getObject(idx_obj, OBJ_OPCLASS)));
}

void DatabaseModel::addOperator(Operator *operador, int idx_obj)
{
 try
 {
  __addObject(operador, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removeOperator(Operator *operador, int idx_obj)
{
 if(operador)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao operador
  getObjectReferences(operador, vet_refs, true);

 /* Caso o operador esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   //Formata a mensagem para referência direta
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(operador->getSignature(true))
                 .arg(operador->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(operador, idx_obj);
 }
}

Operator *DatabaseModel::getOperator(unsigned idx_obj)
{
 return(dynamic_cast<Operator *>(getObject(idx_obj, OBJ_OPERATOR)));
}

void DatabaseModel::addType(Type *tipo, int idx_obj)
{
 if(tipo)
 {
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de dominios para verificar se existe algum
     domínio com o mesmo nome do tipo */
  itr=domains.begin();
  itr_end=domains.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->getName(true)==tipo->getName(true));
   itr++;
  }

  //Caso exista um dominio de mesmo nome que o tipo dispara o erro
  if(enc)
  {
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(tipo->getName(true))
           .arg(tipo->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o tipo
   __addObject(tipo, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionad  lista de tipos válidos
    do PostgreSQL */
   PgSQLType::addUserType(tipo->getName(true), tipo, this, UserTypeConfig::BASE_TYPE);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void DatabaseModel::removeType(Type *tipo, int idx_obj)
{
 try
 {
  removeUserType(tipo, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Type *DatabaseModel::getType(unsigned idx_obj)
{
 return(dynamic_cast<Type *>(getObject(idx_obj, OBJ_TYPE)));
}

void DatabaseModel::removeUserType(BaseObject *objeto, int idx_obj)
{
 if(objeto)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referências ao objeto
  getObjectReferences(objeto, vet_refs, true);

  /* Caso o tipo esteja sendo referenciado, por algum objeto o
     mesmo não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(objeto->getName(true))
            .arg(objeto->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());
   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();

    //Formata a mensagem para referência indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(objeto->getName(true))
            .arg(objeto->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());
   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removeObject(objeto, idx_obj);
   /* Ao ser removido do modelo o objeto (tipo ou domínio) classe tem
    seu nome removido da lista de tipos válidos do PostgreSQL */
  PgSQLType::removeUserType(objeto->getName(true), objeto);
 }
}

void DatabaseModel::addPermission(Permission *permissao)
{
 try
 {
  if(!permissao)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(getPermissionIndex(permissao) >=0)
  {
  /* Caso haja uma permissão semelhante a que está sendo inserida, ou seja,
    o resultado da chamada ao metodo obterIndicePermissao() sejá >= 0,
    um erro será disparado */
   throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
                .arg(QString::fromUtf8(permissao->getObject()->getName()))
                .arg(permissao->getObject()->getTypeName()),
                ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  permissions.push_back(permissao);
 }
 catch(Exception &e)
 {
  if(e.getErrorType()==ERR_ASG_DUPLIC_OBJECT)
   throw
   Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
           .arg(QString::fromUtf8(permissao->getObject()->getName()))
           .arg(permissao->getObject()->getTypeName()),
           ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

  else
   throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void DatabaseModel::removePermission(Permission *permissao)
{
 if(permissao)
 {
  __removeObject(permissao);
 }
}

void DatabaseModel::removePermissions(BaseObject *objeto)
{
 vector<Permission *> vet_perm;
 vector<BaseObject *>::iterator itr, itr_end;
 Permission *permissao=NULL;
 unsigned idx=0;

 /* Caso o objeto o qual terá todas as permissões relacionadas removidas não
    esteja alocado um erro será disparado pois o usuário está
    tentando remover permissões de um objeto inexistente */
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissions.begin();
 itr_end=permissions.end();

 /* Varre a lista de permissões removendo as permissões cujo
    objeto relacionado seja o mesmo do objeto do parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permission *>(*itr);

  //Verifica se o objeto da permissão é igual ao objeto do parâmetro
  if(permissao->getObject()==objeto)
  {
   //Remove o iterador que possui a permissão
   permissions.erase(itr);
   //Desaloca a permissão
   delete(permissao);
   //Reinicializa os iteradores para uma nova varredura
   itr=itr_end=permissions.end();

   /* Caso a lista não esteja vazia após remover a permissão
      desloca o iterador para a posição em que a varredura
      parou ao invés de iniciar uma nova a partir do início
      da lista */
   if(!permissions.empty())
    itr=permissions.begin() + idx;
  }
  else
  { itr++; idx++; }
 }
}

void DatabaseModel::getPermissions(BaseObject *objeto, vector<Permission *> &vet_perm)
{
 vector<BaseObject *>::iterator itr, itr_end;
 Permission *permissao=NULL;

 /* Caso o objeto o qual terá todas as permissões relacionadas
    obtidas não esteja alocado um erro será disparado pois o
    usuário está tentando obter permissões de um objeto inexistente */
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissions.begin();
 itr_end=permissions.end();

 //Limpa a lista receptora de permissões
 vet_perm.clear();

 /* Varre a lista de permissões em busca daquelas
    cujo objeto seja o mesmo do objeto especificado
    no parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permission *>(*itr);

  /* Caso a permissão esteja relacionada ao mesmo
     objeto do parâmetro insere tal permissão
       lista de permissões */
  if(permissao->getObject()==objeto)
   vet_perm.push_back(permissao);

  itr++;
 }
}

int DatabaseModel::getPermissionIndex(Permission *permissao)
{
 int idx_perm=-1;

 //Obtém o índice da permissão somente se a mesma estive alocada
 if(permissao)
 {
  Permission *perm_aux=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  BaseObject *objeto=NULL;
  Role *papel=NULL;
  unsigned qtd, i;
  bool papel_ref=false;

  itr=permissions.begin();
  itr_end=permissions.end();

  //Obtém o objeto da permissão
  objeto=permissao->getObject();

  /* Varre a lista de permissões enquanto não chegar ao final
     da lista ou enquanto o índice da permissão não for
     descoberto */
  while(itr!=itr_end && idx_perm < 0)
  {
   //Obtém uma permissão da lista
   perm_aux=dynamic_cast<Permission *>(*itr);

   /*CAso o objeto do parâmetro seja o mesmo da permissão
     será efetuada uma validação se todos os papeis
     de ambas as permissões são iguais, isso indica
     que ambas possuem o mesmo efeito */
   if(objeto==perm_aux->getObject())
   {
    //Obtém a quantidade de papéis
    qtd=permissao->getRoleCount();

    //Varre a lista de papéis das permissões
    for(i=0; i < qtd && !papel_ref; i++)
    {
     //Obtém um papel da permissão do parâmetro
     papel=permissao->getRole(i);
     /* Com o papel obtido verifica-se se o mesmo é referenciado
        no papel obtido da lista principal de permissões */
     papel_ref=perm_aux->isRoleExists(papel);
    }
   }

   /* Caso um papel ambas as permissões seja idênticas
      (mesmo endereço de memória) ou todos os papéis referenciados
      por ambas sejam os mesmos, o índice da permissão na lista
      será calculado */
   if(permissao==perm_aux || papel_ref)
    idx_perm=itr-permissions.begin();

   itr++;
  }
 }

 return(idx_perm);
}

BaseObject *DatabaseModel::getObject(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 return(getObject(nome, tipo_obj, idx));
}

int DatabaseModel::getObjectIndex(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 getObject(nome, tipo_obj, idx);
 return(idx);
}

int DatabaseModel::getObjectIndex(BaseObject *objeto)
{
 if(!objeto)
  return(-1);
 else
 {
  ObjectType tipo_obj=objeto->getObjectType();
  vector<BaseObject *> *lista_obj=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;

  //Obtém a lista de acordo com o tipo do objeto
  lista_obj=getObjectList(tipo_obj);

  /* Caso a lista não exista indica que foi passado um tipo inválido
     de objeto, dessa forma será retornado um erro */
  if(!lista_obj)
   throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Obtém as referências para o inicio e o fim da lista
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end && !enc)
   {
    enc=((*itr)==objeto);
    if(!enc) itr++;
   }

   if(enc)
    return(itr-lista_obj->begin());
   else
    return(-1);
  }
 }
}

void DatabaseModel::loadModel(const QString &nome_arq)
{
 if(nome_arq!="")
 {
  QString arq_dtd, str_aux, nome_elem;
  ObjectType tipo_obj;
  map<QString, QString> atributos;
  BaseObject *objeto=NULL;
  deque<const xmlNode *> objetos_incomp;
  bool reaval_objetos=false, modelo_protegido=false;
  const xmlNode *elem_aux=NULL;
  deque<Exception> vet_erros;
  map<unsigned, QString>::iterator itr, itr_end;

  /* Montando o caminho padrão para localização dos esquemas DTD.
     Por padrão esta pasta se encontra na raiz dentro da pasta esquemas/xml*/
  arq_dtd=GlobalAttributes::SCHEMAS_ROOT_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           GlobalAttributes::XML_SCHEMA_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           GlobalAttributes::OBJECT_DTD_DIR +
           GlobalAttributes::DIR_SEPARATOR;

  try
  {
   loading_model=true;

   //Reinicia o parser XML para a leitura do arquivo
   XMLParser::restartParser();
   //Faz com que o parser carregue a DTD que armazena a sintaxe do arquivo de modelos
   XMLParser::setDTDFile(arq_dtd + GlobalAttributes::ROOT_DTD +
                                GlobalAttributes::OBJECT_DTD_EXT,
                                GlobalAttributes::ROOT_DTD);

   //Carrega o arquivo validando-o de acordo com a DTD informada
   XMLParser::loadXMLFile(nome_arq);

   //Obter as informações de versão, autor do modelo e versão postgresql
   XMLParser::getElementAttributes(atributos);
   this->author=atributos[ParsersAttributes::MODEL_AUTHOR];

   modelo_protegido=(atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

   //Passa para o próximo elemento que provavelmente será um <role> ou <tablespace>
   if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
   {
    do
    {
     if(XMLParser::getElementType()==XML_ELEMENT_NODE)
     {
      nome_elem=XMLParser::getElementName();

      /* Caso o nome do elemento atual seja o que define uma permissão
         indica que o parser leu todos os objetos do modelo e o mesmo
         se encontra no trecho final do arquivo onde são armazenadas
         as permissões sobre tais objetos. A obtenção das permissões
         só serão feitas caso não hajam objetos para serem reavaliados,
         caso contrário, os objetos são reavalias e logo apos as permissões
         obtidas. */
      if(nome_elem==ParsersAttributes::PERMISSION && objetos_incomp.size()==0)
      {
       /* Caso a lista de objetos especiais esteja com elementos
          efetua a recriação dos mesmos. Obs: Este processo é executado
          apenas 1 vez caso a lista de objetos especiais contenha algum item
          independente do número de permissões a serem criadas pois esta lista
          é preenchida durante o processo de criação dos objetos e não na criação
          das permissões. */
       if(!xml_special_objs.empty())
       {
        /* Efetua a cópia do buffer atual do parser pois
           a restauração de objetos especiais apaga o buffer
           recente do parser para serem usados os códigos xml
           de tais objetos */
        str_aux=XMLParser::getXMLBuffer();

        itr=xml_special_objs.begin();
        itr_end=xml_special_objs.end();

        while(itr!=itr_end)
        {
         /* Cria o objeto especial a partir do iterador atual
            sendo que o elemento 'itr->second' possui a definição xml
            do mesmo e o elemento 'itr->first' possui o id do objeto
            especial a ser recriado */
         createSpecialObject(itr->second, itr->first);
         itr++;
        }

        /* Limpa a lista de objetos especiais para evitar que os mesmos
           tentem ser recriados posteriormente */
        xml_special_objs.clear();

        /* Após a restauração dos objetos especiais é necessário recarregar
          o buffer do parser com o buffer usando anteriormente   restauração
          deos objetos especiais para que as permissões possam ser criadas
          a partir do XML */
        XMLParser::restartParser();
        XMLParser::loadXMLBuffer(str_aux);
        XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

        /* Executa a navegação sobre os elementos até que o primeiro elemento
           que define uma permissão seja localizado */
        while(XMLParser::getElementName()!=ParsersAttributes::PERMISSION &&
              XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
       }

       //Executa acriação da permissão atual a partir do xml
       addPermission(createPermission());
      }
      else
      {
       //Obtém o tipo do elemento de acordo com a tag atual que o parser carregou
       tipo_obj=getObjectType(nome_elem);

       /* Este bloco faz a reavaliação de objetos. A reavaliação consiste em
          ler um trecho do arquivo XML novamente onde algum objeto foi referenciado
          porém ainda não criado causando o disparo de exceções reclamando de
          objetos com definição SQL incompleta.

          Condição para reavaliação dos objetos:
           * A flag de reavaliação (reaval_objetos) não esteja marcada indicando
             que uma reavaliação não está ocorrendo
           * A lista de objetos incompletos possua pelo menos 1 item, indicando que
             em algum trecho do XML existe um objeto o qual referencia outro que não
             foi encontrado (criado)
       */
       if(!reaval_objetos && objetos_incomp.size() > 0 &&
          tipo_obj!=OBJ_FUNCTION && tipo_obj!=OBJ_SCHEMA &&
          tipo_obj!=OBJ_TABLESPACE && tipo_obj!=OBJ_LANGUAGE &&
          tipo_obj!=OBJ_TYPE && tipo_obj!=OBJ_TABLE)
       {
        //Faz o parser XML voltar ao elemento anterior
        XMLParser::accessElement(XMLParser::PREVIOUS_ELEMENT);
        //Armazena a posição atual do parser
        elem_aux=XMLParser::getCurrentElement();

        reaval_objetos=true;
        //Restaura o parser para a posição onde o objeto incompleto foi lido
        XMLParser::restorePosition(objetos_incomp.front());
        //Obtém o tipo deste objeto
        tipo_obj=getObjectType(XMLParser::getElementName());
       }

       //Lendo um objeto banco de dados
       if(tipo_obj==OBJ_DATABASE)
       {
        XMLParser::getElementAttributes(atributos);
        encoding=atributos[ParsersAttributes::ENCODING];
        template_db=atributos[ParsersAttributes::TEMPLATE_DB];
        localizations[0]=atributos[ParsersAttributes::LC_CTYPE_DB];
        localizations[1]=atributos[ParsersAttributes::LC_COLLATE_DB];

        if(!atributos[ParsersAttributes::CONN_LIMIT].isEmpty())
         conn_limit=atributos[ParsersAttributes::CONN_LIMIT].toInt();

        setBasicAttributes(this);
       }
       else
       {
        try
        {
         /* Para os demais objetos, sempre a posição do parser XMl é salva
            antes da leitura e criação dos mesmos, para que a reavaliação
            seja possível, quando for necessária */
         XMLParser::savePosition();
         //Cria o objeto de acordo com  o tipo obtido
         objeto=createObject(tipo_obj);

         //Caso o objeto foi criado com sucesso
         if(objeto)
         {
          if(!dynamic_cast<TableObject *>(objeto) &&
             tipo_obj!=OBJ_RELATIONSHIP && tipo_obj!=BASE_RELATIONSHIP)
          {
           //Usa o método de inserção de objetos genéricos
           addObject(objeto);
          }

          //Dispara um sinal com o progresso (aproximado) do carregamento de objetos
          if(!signalsBlocked())
          {
           emit s_objectLoaded(XMLParser::getCurrentBufferLine()/XMLParser::getBufferLineCount(),
                                  trUtf8("Loading object: %1 (%2)")
                                   .arg(QString::fromUtf8(objeto->getName()))
                                   .arg(objeto->getTypeName()),
                                  tipo_obj);
          }
         }

         //Restaura a posição do parser para o elemento anterior ao atual
         XMLParser::restorePosition();
        }
        catch(Exception &e)
        {
         if(!reaval_objetos &&
            (((e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL && tipo_obj==OBJ_TABLE)) ||
             (((e.getErrorType()==ERR_ASG_OBJ_INV_DEFINITION ||
                e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL ||
                e.getErrorType()==ERR_ASG_INV_TYPE_OBJECT) &&
               (tipo_obj==OBJ_LANGUAGE || tipo_obj==OBJ_FUNCTION || tipo_obj==OBJ_TYPE || tipo_obj==OBJ_OPERATOR)))))
         {
          /* Adiciona o nó da arvore o qual possui o elemento incompleto
             para que o mesmo tente ser recriado após suas possíveis dependências
             serem carregadas */
          XMLParser::restorePosition();
          objetos_incomp.push_back(XMLParser::getCurrentElement());
          //Armazena a exceção capturada para verificações ao final do carregamento do modelo
          vet_erros.push_back(e);
         }
         else
         {
          QString info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);
          throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
         }
        }
       }
      }

      /* Caso esteja num processo de reavaliação de objetos, o bloco abaixo
         faz a retirada do objeto que acaba de ser reavaliado da lista de
         objetos incompletos */
      if(reaval_objetos && objetos_incomp.size() > 0)
      {
       //Remove o objeto incompleto da lista
       objetos_incomp.pop_front();

       //Remove a exceção relacionada ao objeto incompleto
       vet_erros.pop_front();

       /* Caso a lista ainda possua elementos o parser será voltado para o
          elemento incompleto anterior ao atual */
       if(objetos_incomp.size() > 0)
        XMLParser::restorePosition(objetos_incomp.front());
       else
       {
        //Caso a lista esteja vazia o processo de reavaliação é interrompido
        reaval_objetos=false;
        //O parser é retornad  posição em que se encontrava antes da reavaliação
        XMLParser::restorePosition(elem_aux);
       }
      }
     }
    }
    while((!reaval_objetos && XMLParser::accessElement(XMLParser::NEXT_ELEMENT)) ||
          (reaval_objetos));
   }

   /* Verificação final, caso hajam erros pendentes na lista de erros capturados durante o processo
      de leitura do arquivo os mesmo serão redirecionados e o processo de leitura do arquivo
      cancelado */
   if(vet_erros.size() > 0)
    throw Exception(vet_erros[0].getErrorMessage(),vet_erros[0].getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Protege o modelo com base no atributo obtido do xml
   this->BaseObject::setProtected(modelo_protegido);
   loading_model=false;

   /* Faz uma última validação nos relacionamentos para abranger qualquer modificação de colunas
     não feitas durante o carregamento */
   this->validateRelationships();
  }
  catch(Exception &e)
  {
   QString info_adicional;
   loading_model=false;
   destroyObjects();

   if(XMLParser::getCurrentElement())
    info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);

   //Caso o erro seja na biblioteca de parsers
   if(e.getErrorType()>=ERR_INVALID_SYNTAX)
   {
    str_aux=QString(Exception::getErrorMessage(ERR_LOAD_INV_MODEL_FILE)).arg(nome_arq);
    throw Exception(str_aux,ERR_LOAD_INV_MODEL_FILE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
   }
   else
    //Captura e redireciona erros das demais bibliotecas
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
  }
 }
}

ObjectType DatabaseModel::getObjectType(const QString &str_tipo)
{
 ObjectType tipo_obj=BASE_OBJECT;
 int i;

 for(i=0; i < BaseObject::OBJECT_TYPE_COUNT; i++)
 {
  if(objs_schemas[i]==str_tipo)
  {
   tipo_obj=static_cast<ObjectType>(i);
   break;
  }
 }

 return(tipo_obj);
}

BaseObject *DatabaseModel::createObject(ObjectType tipo_obj)
{
 BaseObject *objeto=NULL;

 if(tipo_obj!=BASE_OBJECT)
 {
  if(tipo_obj==OBJ_ROLE)
   objeto=createRole();
  else if(tipo_obj==OBJ_TABLESPACE)
   objeto=createTablespace();
  else if(tipo_obj==OBJ_SCHEMA)
   objeto=createSchema();
  else if(tipo_obj==OBJ_LANGUAGE)
   objeto=createLanguage();
  else if(tipo_obj==OBJ_FUNCTION)
   objeto=createFunction();
  else if(tipo_obj==OBJ_TYPE)
   objeto=createType();
  else if(tipo_obj==OBJ_DOMAIN)
   objeto=createDomain();
  else if(tipo_obj==OBJ_CAST)
   objeto=createCast();
  else if(tipo_obj==OBJ_CONVERSION)
   objeto=createConversion();
  else if(tipo_obj==OBJ_OPERATOR)
   objeto=createOperator();
  else if(tipo_obj==OBJ_OPFAMILY)
   objeto=createOperatorFamily();
  else if(tipo_obj==OBJ_OPCLASS)
   objeto=createOperatorClass();
  else if(tipo_obj==OBJ_AGGREGATE)
   objeto=createAggregate();
  else if(tipo_obj==OBJ_TABLE)
   objeto=createTable();
  else if(tipo_obj==OBJ_SEQUENCE)
   objeto=createSequence();
  else if(tipo_obj==OBJ_VIEW)
   objeto=createView();
  else if(tipo_obj==OBJ_TEXTBOX)
   objeto=createTextbox();
  else if(tipo_obj==OBJ_CONSTRAINT)
   objeto=createConstraint(NULL);
  else if(tipo_obj==OBJ_TRIGGER)
   objeto=createTrigger(NULL);
  else if(tipo_obj==OBJ_INDEX)
   objeto=createIndex(NULL);
  else if(tipo_obj==OBJ_RELATIONSHIP ||
          tipo_obj==BASE_RELATIONSHIP)
   objeto=createRelationship();
 }

 return(objeto);
}

void DatabaseModel::setBasicAttributes(BaseObject *objeto)
{
 map<QString, QString> atributos, atribs_aux;
 QString nome_elem;//, str_aux;
 BaseObject *esp_tabela=NULL, *dono=NULL;
 Schema *esquema=NULL;
 ObjectType tipo_obj=BASE_OBJECT, tipo_obj_aux;
 bool erro=false, protegido=false;

 //Caso o objeto não esteja alocado uma exceção é disparada
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém os atributos do elemento
 XMLParser::getElementAttributes(atributos);

 tipo_obj_aux=objeto->getObjectType();
 if(tipo_obj_aux!=OBJ_CAST)
  objeto->setName(atributos[ParsersAttributes::NAME]);

 //Definindo se o objeto está protegido ou não
 protegido=atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;

 XMLParser::savePosition();

 //Passa para os elementos filhos que provavelmente serão <onwer>, <tablespace> e <comment>
 if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
 {
  do
  {
   /* Certificando que só elementos xml serão lidos do parser, qualquer outro
      tipo de objeto xml será ignorado */
   if(XMLParser::getElementType()==XML_ELEMENT_NODE)
   {
    //Obtém o nome do elemento filho
    nome_elem=XMLParser::getElementName();

    //Caso o elemento filho seja um comentáio <comment>
    if(nome_elem==ParsersAttributes::COMMENT)
    {
     /* Para se extraír o comentário, é necessário salvar a posição de navegação
        do parser, pois o conteúdo do comentário é um elemento filho do elemento
        atual do parser XML */
     XMLParser::savePosition();
     //Acessa o elemento filho o qual contém o conteúdo do comentário
     XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
     //Atributo o conteúdo do elemento filho ao atributo cometário do novo papel
     objeto->setComment(XMLParser::getElementContent());
     //Restaura a posição de navegação do parser, ou seja, volta para o elemento <comment>
     XMLParser::restorePosition();
    }
    //Caso o elemento filho seja uma referência a um esquema <schema>
    else if(nome_elem==ParsersAttributes::SCHEMA)
    {
     tipo_obj=OBJ_SCHEMA;
     XMLParser::getElementAttributes(atribs_aux);
     esquema=dynamic_cast<Schema *>(getObject(atribs_aux[ParsersAttributes::NAME], tipo_obj));
     objeto->setSchema(esquema);
     erro=(!esquema && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um tablespace <tablespace>
    else if(nome_elem==ParsersAttributes::TABLESPACE)
    {
     tipo_obj=OBJ_TABLESPACE;
     XMLParser::getElementAttributes(atribs_aux);
     esp_tabela=getObject(atribs_aux[ParsersAttributes::NAME], tipo_obj);
     objeto->setTablespace(esp_tabela);
     erro=(!esp_tabela && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um dono (role/papel) <role>
    else if(nome_elem==ParsersAttributes::ROLE)
    {
     tipo_obj=OBJ_ROLE;
     XMLParser::getElementAttributes(atribs_aux);
     dono=getObject(atribs_aux[ParsersAttributes::NAME], tipo_obj);
     objeto->setOwner(dono);
     erro=(!dono && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }
    //Obténdo o atributo a posição do objeto (apenas para objetos gráficos)
    else if(nome_elem==ParsersAttributes::POSITION)
    {
     XMLParser::getElementAttributes(atributos);

     if(nome_elem==ParsersAttributes::POSITION &&
        (tipo_obj_aux!=OBJ_RELATIONSHIP &&
         tipo_obj_aux!=BASE_RELATIONSHIP))
     {
      dynamic_cast<BaseGraphicObject *>(objeto)->setPosition(QPointF(atributos[ParsersAttributes::X_POS].toFloat(),
                                                                       atributos[ParsersAttributes::Y_POS].toFloat()));


     }
    }
   }
  }
  /* A extração de elementos continuará até que se chegue no último elemento
     filho do elemento */
  while(!erro && XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
 }

 XMLParser::restorePosition();
 objeto->setProtected(protegido);

 if(erro)
 {
  /* Caso o flag de erro esteja marcado indica que o novo objeto
     está referenciando um outro objeto o qual não existe no modelo */

  //Dispara a exceção
  throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                   .arg(QString::fromUtf8(objeto->getName()))
                   .arg(objeto->getTypeName())
                   .arg(QString::fromUtf8(atribs_aux[ParsersAttributes::NAME]))
                   .arg(BaseObject::getTypeName(tipo_obj)),
                ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 //Caso o objeto não esteja atribuído a nenhum esquema gera um erro
 else if(!objeto->getSchema() &&
         (tipo_obj_aux==OBJ_FUNCTION || tipo_obj_aux==OBJ_TABLE ||
          tipo_obj_aux==OBJ_VIEW  || tipo_obj_aux==OBJ_DOMAIN ||
          tipo_obj_aux==OBJ_AGGREGATE || tipo_obj_aux==OBJ_OPERATOR ||
          tipo_obj_aux==OBJ_SEQUENCE || tipo_obj_aux==OBJ_CONVERSION ||
          tipo_obj_aux==OBJ_TYPE || tipo_obj_aux==OBJ_OPFAMILY ||
          tipo_obj_aux==OBJ_OPCLASS))
 {
  throw Exception(Exception::getErrorMessage(ERR_ALOC_OBJECT_NO_SCHEMA)
                        .arg(QString::fromUtf8(objeto->getName()))
                        .arg(objeto->getTypeName()),
          ERR_ALOC_OBJECT_NO_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

Role *DatabaseModel::createRole(void)
{
 map<QString, QString> atributos, atribs_aux;
 Role *papel=NULL, *papel_ref=NULL;
 int i, tam;
 bool marcado;
 QStringList lista;
 QString nome_elem;
 unsigned tipo_papel;

 /* Os elementos dos dois elementos a seguir deve aparecer na
    mesma ordem pois seus valores serão trabalhados em conjunto
    na interação onde ser precisa identificar as opções do papel
    o qual está sendo criado */
 QString atrib_ops[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                       ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                       ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED };

 unsigned vet_ops[]={ Role::OP_SUPERUSER, Role::OP_CREATEDB,
                      Role::OP_CREATEROLE, Role::OP_INHERIT,
                      Role::OP_LOGIN, Role::OP_ENCRYPTED };

 try
 {
  //Aloca no novo papel
  papel=new Role;
  setBasicAttributes(papel);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do papel
  papel->setPassword(atributos[ParsersAttributes::PASSWORD]);
  papel->setValidity(atributos[ParsersAttributes::VALIDITY]);

  /* Caso o atributo de id de usuário esteja atribuído no xml.
     (atributos[AtributosParsers::UID] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[ParsersAttributes::SYSID].isEmpty())
   papel->setSysid(atributos[ParsersAttributes::SYSID].toInt());

  /* Caso o atributo de limite de conexão esteja atribuído no xml.
     (atributos[AtributosParsers::LIMITE_CONEXAO] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[ParsersAttributes::CONN_LIMIT].isEmpty())
   papel->setConnectionLimit(atributos[ParsersAttributes::CONN_LIMIT].toInt());

  /* Identificando as opções do papel. Caso o atributo referet   uma
     estive com valor "true" no documento XML quer dizer que aquele
     atributo está marcado para o papel */
  for(i=0; i < 6; i++)
  {
   //Verifica se a opção está marcada no XML, valor de atributo = true
   marcado=atributos[atrib_ops[i]]==ParsersAttributes::_TRUE_;
   papel->setOption(vet_ops[i], marcado);
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
     nome_elem=XMLParser::getElementName();

     //Caso o elemento filho seja uma lista de papeis <roles>
     if(nome_elem==ParsersAttributes::ROLES)
     {
      //Obtém os atributos do elemento <roles>, neste caso são names e reftype
      XMLParser::getElementAttributes(atribs_aux);

      /* O atributo names armazena uma lista de nomes de papéis as quais o novo papel
         referenciará. A lista tem os elementos separados por vírgula, sendo assim a
         string será quebrada usando o delimitador ',') */
      lista=atribs_aux[ParsersAttributes::NAMES].split(',');

      //Obtém a quantidade de nomes de papéis na lista
      tam=lista.size();

      /* Identificando o tipo da lista de papéis a qual será inserido os objetos
         cujos nomes foram extraídos acima */
      if(atribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::REFER)
       tipo_papel=Role::REF_ROLE;
      else if(atribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::MEMBER)
       tipo_papel=Role::MEMBER_ROLE;
      else
       tipo_papel=Role::ADMIN_ROLE;

      //Varre a lista de nomes de papéis
      for(i=0; i < tam; i++)
      {
       //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
       papel_ref=dynamic_cast<Role *>(getObject(lista[i].trimmed(),OBJ_ROLE));

       /* Caso esse papel não exista um erro será disparado pois um novo papel
          não pode referenciar um outro papel que ainda nem foi criado */
       if(!papel_ref)
       {
        //Dispara a exceção
        throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(papel->getName()))
                               .arg(BaseObject::getTypeName(OBJ_ROLE))
                               .arg(QString::fromUtf8(lista[i]))
                               .arg(BaseObject::getTypeName(OBJ_ROLE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       //Caso o papel exista no modelo, o mesmo será relacionado ao novo papel
       papel->addRole(tipo_papel, papel_ref);
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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o papel alocado
  if(papel) delete(papel);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Retorna o novo papel criado
 return(papel);
}

Tablespace *DatabaseModel::createTablespace(void)
{
 map<QString, QString> atributos;
 Tablespace *esp_tabela=NULL;

 try
 {
  //Aloca no novo espaço de tabelas
  esp_tabela=new Tablespace;
  setBasicAttributes(esp_tabela);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do papel
  esp_tabela->setDirectory(atributos[ParsersAttributes::DIRECTORY]);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(esp_tabela) delete(esp_tabela);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esp_tabela);
}

Schema *DatabaseModel::createSchema(void)
{
 map<QString, QString> atributos;
 Schema *esquema=NULL;

 try
 {
  //Aloca no novo esquema
  esquema=new Schema;

  //Lê do parser os atributos basicos
  setBasicAttributes(esquema);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(esquema) delete(esquema);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esquema);
}

Language *DatabaseModel::createLanguage(void)
{
 map<QString, QString> atributos;
 Language *linguagem=NULL;
 BaseObject *funcao=NULL;
 QString assinatura, tipo_ref;
 ObjectType tipo_obj;

 try
 {
  linguagem=new Language;

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  setBasicAttributes(linguagem);

  linguagem->setTrusted(atributos[ParsersAttributes::TRUSTED]==
                               ParsersAttributes::_TRUE_);

   if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
   {
    do
    {
     /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
     if(XMLParser::getElementType()==XML_ELEMENT_NODE)
     {
      tipo_obj=getObjectType(XMLParser::getElementName());

      if(tipo_obj==OBJ_FUNCTION)
      {
       XMLParser::getElementAttributes(atributos);
       //Obtém o tipo de referência da função
       tipo_ref=atributos[ParsersAttributes::REF_TYPE];

       //Caso seja uma função handler ou validator
       if(tipo_ref==ParsersAttributes::VALIDATOR_FUNC ||
          tipo_ref==ParsersAttributes::HANDLER_FUNC ||
          tipo_ref==ParsersAttributes::INLINE_FUNC)
       {
        //Obtém a assinatura da função
        assinatura=atributos[ParsersAttributes::SIGNATURE];

        //Obtém a função do modelo
        funcao=getObject(assinatura, OBJ_FUNCTION);

        //Caso a função não seja encontrada
        if(!funcao)
         //Dispara a exceção indicando que o objeto está incompleto
         throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(linguagem->getName()))
                               .arg(linguagem->getTypeName())
                               .arg(QString::fromUtf8(assinatura))
                               .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                       ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

        if(tipo_ref==ParsersAttributes::VALIDATOR_FUNC)

         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::VALIDATOR_FUNC);
        else if(tipo_ref==ParsersAttributes::HANDLER_FUNC)
         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::HANDLER_FUNC);
        else
         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::INLINE_FUNC);

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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove a linguagem alocada
  if(linguagem) delete(linguagem);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(linguagem);
}

Function *DatabaseModel::createFunction(void)
{
 map<QString, QString> atributos, atrib_aux;
 Function *funcao=NULL;
 ObjectType tipo_obj;
 BaseObject *objeto=NULL;
 PgSQLType tipo;
 Parameter param;
 QString str_aux, elem;

 try
 {
  funcao=new Function;
  //Lê do parser os atributos basicos
  setBasicAttributes(funcao);

  //Obtém os atributos da função
  XMLParser::getElementAttributes(atributos);

  //Define se a função retorna setof, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::RETURNS_SETOF].isEmpty())
   funcao->setReturnSetOf(atributos[ParsersAttributes::RETURNS_SETOF]==
                               ParsersAttributes::_TRUE_);

  //Define se a função é do tipo janela, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::WINDOW_FUNC].isEmpty())
   funcao->setWindowFunction(atributos[ParsersAttributes::WINDOW_FUNC]==
                               ParsersAttributes::_TRUE_);

  //Define a configuração de retorno da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::BEHAVIOR_TYPE].isEmpty())
   funcao->setBehaviorType(BehaviorType(atributos[ParsersAttributes::BEHAVIOR_TYPE]));

  //Define o tipo da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::FUNCTION_TYPE].isEmpty())
   funcao->setFunctionType(FunctionType(atributos[ParsersAttributes::FUNCTION_TYPE]));

  //Define o tipo de segurança da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::SECURITY_TYPE].isEmpty())
   funcao->setSecurityType(SecurityType(atributos[ParsersAttributes::SECURITY_TYPE]));

  //Define o custo de execução da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::EXECUTION_COST].isEmpty())
   funcao->setExecutionCost(atributos[ParsersAttributes::EXECUTION_COST].toInt());

  //Define a quantidade de linhas retornadas pela função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::ROW_AMOUNT].isEmpty())
   funcao->setRowAmount(atributos[ParsersAttributes::ROW_AMOUNT].toInt());

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();
     tipo_obj=getObjectType(elem);

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
          tipo=createPgSQLType();
          //Atribui ao retorno da função
          funcao->setReturnType(tipo);
         }
         /* Criação dos tipo de retorno de tabela da função. Os mesmos vem descritos
            dentro da tag <return-type> em forma de parâmetros */
         else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
         {
          param=createParameter();
          //Adiciona o tipo de retorno   função
          funcao->addReturnedTableColumn(param.getName(), param.getType());
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
     else if(tipo_obj==OBJ_LANGUAGE)
     {
      //Obtém seus atributos
      XMLParser::getElementAttributes(atributos);

      //Busca a linguagem no modelo
      objeto=getObject(atributos[ParsersAttributes::NAME], tipo_obj);

      //Caso a linguagem não existe será disparada uma exceção
      if(!objeto)
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(funcao->getName()))
                              .arg(funcao->getTypeName())
                              .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                              .arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a linguagem da função
      funcao->setLanguage(dynamic_cast<Language *>(objeto));
     }
     else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
     {
      param=createParameter();
      //Adiciona o parâmet  função
      funcao->addParameter(param);
     }
     //Extraíndo a definição (corpo) da função (tag <definition>)
     else if(XMLParser::getElementName()==ParsersAttributes::DEFINITION)
     {
      XMLParser::savePosition();

      //Obtém os atributos da biblioteca
      XMLParser::getElementAttributes(atrib_aux);

      if(!atrib_aux[ParsersAttributes::LIBRARY].isEmpty())
      {
       funcao->setLibrary(atrib_aux[ParsersAttributes::LIBRARY]);
       funcao->setSymbol(atrib_aux[ParsersAttributes::SYMBOL]);
      }
      /* Para se ter acesso ao código que define a função é preciso acessar
         o filho da tag <definition> e obter seu conteúdo */
      else if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
       funcao->setSourceCode(XMLParser::getElementContent());

      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(funcao)
  {
   str_aux=funcao->getName(true);
   delete(funcao);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
                              .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(funcao);
}

Parameter DatabaseModel::createParameter(void)
{
 PgSQLType tipo;
 Parameter param;
 map<QString, QString> atributos;
 QString elem;

 try
 {
  //Salva a posição do parser
  XMLParser::savePosition();
  //Obtem os atributos do parâmetro (nome, in e out)
  XMLParser::getElementAttributes(atributos);

  param.setName(atributos[ParsersAttributes::NAME]);
  /* Configurando atributos in e out do parâmetro caso estes estejam
     definidos como true no XML */
  param.setIn(atributos[ParsersAttributes::PARAM_IN]==ParsersAttributes::_TRUE_);
  param.setOut(atributos[ParsersAttributes::PARAM_OUT]==ParsersAttributes::_TRUE_);
  param.setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Restaura a posição do parser
  XMLParser::restorePosition();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(param);
}

PgSQLType DatabaseModel::createPgSQLType(void)
{
 map<QString, QString> atributos;
 vector<void *> vet_ptipos;
 vector<void *>::iterator itr, itr_end;
 unsigned comprimento=1, dimensao=0, idx_tipo=0;
 int precisao=-1;
 QString nome;
 Type *tipo_usr=NULL;
 void *ptipo=NULL;
 //TipoPgSQL tipo;
 bool enc=false, com_timezone;
 IntervalType tipo_interv;
 SpatialType tipo_esp;


 //Obtém os atributos do tipo
 XMLParser::getElementAttributes(atributos);

 if(!atributos[ParsersAttributes::LENGTH].isEmpty())
  comprimento=atributos[ParsersAttributes::LENGTH].toUInt();

 if(!atributos[ParsersAttributes::DIMENSION].isEmpty())
  dimensao=atributos[ParsersAttributes::DIMENSION].toUInt();

 if(!atributos[ParsersAttributes::PRECISION].isEmpty())
  precisao=atributos[ParsersAttributes::PRECISION].toInt();

 com_timezone=(atributos[ParsersAttributes::WITH_TIMEZONE]==ParsersAttributes::_TRUE_);
 tipo_interv=atributos[ParsersAttributes::INTERVAL_TYPE];

 if(!atributos[ParsersAttributes::SPATIAL_TYPE].isEmpty())
  tipo_esp=SpatialType(atributos[ParsersAttributes::SPATIAL_TYPE],
                        atributos[ParsersAttributes::VARIATION].toUInt());

 nome=atributos[ParsersAttributes::NAME];

 idx_tipo=PgSQLType::getBaseTypeIndex(nome);
 if(idx_tipo!=PgSQLType::null)
 {
  return(PgSQLType(nome,comprimento,dimensao,precisao,com_timezone,tipo_interv, tipo_esp));
 }
 else
 {
  //Obtém a lista de tipos definidios pelo usuario
  PgSQLType::getUserTypes(vet_ptipos, this,
                               UserTypeConfig::BASE_TYPE |
                               UserTypeConfig::DOMAIN_TYPE |
                               UserTypeConfig::TABLE_TYPE |
                               UserTypeConfig::SEQUENCE_TYPE);
  itr=vet_ptipos.begin();
  itr_end=vet_ptipos.end();

  //Faz uma varredura na lista de tipos
  while(itr!=itr_end && !enc)
  {
   //Converte o elemento atual para a classe Tipo
   ptipo=(*itr);
   tipo_usr=reinterpret_cast<Type *>(ptipo);

   //Caso o nome passado no XML for igual ao nome do objeto Tipo atual
   enc=(tipo_usr->getName(true)==nome);
   itr++;
  }

  /* Caso o tipo não foi encontrado na lista de tipos quer dizer existe
     a tentativa de se referenciar um tipo definido pelo usuário sem que o
     mesmo exista no modelo, sendo assim, um erro será disparado e
     a criação do tipo será abortada */
  if(!enc)
    throw Exception(ERR_REF_INEXIST_USER_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  idx_tipo=PgSQLType::getUserTypeIndex(nome, ptipo);
  return(PgSQLType(idx_tipo,comprimento,dimensao,precisao,com_timezone,tipo_interv,tipo_esp));
 }
}

Type *DatabaseModel::createType(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 Type *tipo=NULL;
 int qtd, i;
 QStringList enums;
 QString elem, str_aux;
 Parameter param;
 BaseObject *funcao=NULL;
 unsigned tipo_func=0;
 PgSQLType tipo_copia;

 try
 {
  tipo=new Type;
  //Lê do parser os atributos basicos
  setBasicAttributes(tipo);

  //Obtém os atributos do tipo
  XMLParser::getElementAttributes(atributos);

  //Define a configuração do tipo
  if(atributos[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE)
  {
   tipo->setConfiguration(Type::BASE_TYPE);

   //Definindos os atributos específicos para tipo base

   //Definindo se o tipo é passado por valor ou não
   tipo->setByValue(atributos[ParsersAttributes::BY_VALUE]==ParsersAttributes::_TRUE_);

   //Definindo o comprimento interno do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::INTERNAL_LENGHT].isEmpty())
    tipo->setInternalLength(atributos[ParsersAttributes::INTERNAL_LENGHT].toUInt());

   //Definindo o alinhamento interno do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::ALIGNMENT].isEmpty())
    tipo->setAlignment(atributos[ParsersAttributes::ALIGNMENT]);

   //Definindo o tipo de armazenamento do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::STORAGE].isEmpty())
    tipo->setStorage(atributos[ParsersAttributes::STORAGE]);

   //Definindo o elemento do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::ELEMENT].isEmpty())
    tipo->setElement(atributos[ParsersAttributes::ELEMENT]);

   //Definindo o delimitador do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::DELIMITER].isEmpty())
    tipo->setDelimiter(atributos[ParsersAttributes::DELIMITER][0].toAscii());

   //Definindo o valor padrão do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::DEFAULT_VALUE].isEmpty())
    tipo->setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::CATEGORY].isEmpty())
    tipo->setCategory(atributos[ParsersAttributes::CATEGORY]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::PREFERRED].isEmpty())
    tipo->setPreferred(atributos[ParsersAttributes::PREFERRED]==ParsersAttributes::_TRUE_);


   /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
   tipo_funcoes[ParsersAttributes::INPUT_FUNC]=Type::INPUT_FUNC;
   tipo_funcoes[ParsersAttributes::OUTPUT_FUNC]=Type::OUTPUT_FUNC;
   tipo_funcoes[ParsersAttributes::SEND_FUNC]=Type::SEND_FUNC;
   tipo_funcoes[ParsersAttributes::RECV_FUNC]=Type::RECV_FUNC;
   tipo_funcoes[ParsersAttributes::TPMOD_IN_FUNC]=Type::TPMOD_IN_FUNC;
   tipo_funcoes[ParsersAttributes::TPMOD_OUT_FUNC]=Type::TPMOD_OUT_FUNC;
   tipo_funcoes[ParsersAttributes::ANALYZE_FUNC]=Type::ANALYZE_FUNC;
  }
  else if(atributos[ParsersAttributes::CONFIGURATION]==ParsersAttributes::COMPOSITE_TYPE)
   tipo->setConfiguration(Type::COMPOSITE_TYPE);
  else
   tipo->setConfiguration(Type::ENUMERATION_TYPE);

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
      XMLParser::getElementAttributes(atributos);
      /* Como se trata de uma lista de enumerações separadas por vírgulas
         a mesma será quebrada e transformada num vetor */
      enums=atributos[ParsersAttributes::VALUES].split(",");

      //Adiciona ao tipo todas as enumerações presentes no vetor
      qtd=enums.size();
      for(i=0; i < qtd; i++)
       tipo->addEnumeration(enums[i]);
     }
     //Operação específica para tipo COMPOSTO
     else if(elem==ParsersAttributes::PARAMETER)
     {
      /* No caso de tipo composto, o mesmo possui indefinida quatidade
         de elementos <parameter> os quais simbolizam os atributos do
         tipo */
      param=createParameter();
      tipo->addAttribute(param);
     }
     //Operação específica para tipo BASE
     else if(elem==ParsersAttributes::TYPE)
     {
      tipo_copia=createPgSQLType();
      tipo->setLikeType(tipo_copia);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso de tipo base, serão extraídas referência a funções do modelo,
        as quais serão atribuía  s funções que compoem o tipo base. */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida di XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      /* Dispara uma exceção caso o tipo de referencia a função seja inválido ou
         se a função referenciada não existe */
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(tipo->getName()))
                              .arg(tipo->getTypeName())
                              .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                              .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      else if(tipo_funcoes.count(atributos[ParsersAttributes::REF_TYPE])==0)
       throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      tipo->setFunction(tipo_func, dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o tipo alocado
  if(tipo)
  {
   str_aux=tipo->getName(true);
   delete(tipo);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
                              .arg(tipo->getTypeName()),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tipo);
}

Domain *DatabaseModel::createDomain(void)
{
 map<QString, QString> atributos;
 Domain *dominio=NULL;
 QString elem;

 try
 {
  dominio=new Domain;

  //Lê do parser os atributos basicos
  setBasicAttributes(dominio);

  //Obtém os atributos do domíno
  XMLParser::getElementAttributes(atributos);

  if(!atributos[ParsersAttributes::CONSTRAINT].isEmpty())
   dominio->setConstraintName(atributos[ParsersAttributes::CONSTRAINT]);

  if(!atributos[ParsersAttributes::DEFAULT_VALUE].isEmpty())
   dominio->setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

  dominio->setNotNull(atributos[ParsersAttributes::NOT_NULL]==
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
      dominio->setType(createPgSQLType());
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
      dominio->setExpression(XMLParser::getElementContent());
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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o domínio alocado
  if(dominio) delete(dominio);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(dominio);
}

Cast *DatabaseModel::createCast(void)
{
 map<QString, QString> atributos;
 Cast *conv_tipo=NULL;
 QString elem;
 unsigned idx_tipo=0;
 PgSQLType tipo;
 BaseObject *funcao=NULL;

 try
 {
  conv_tipo=new Cast;

  //Lê do parser os atributos basicos
  setBasicAttributes(conv_tipo);

  //Obtém os atributos do domíno
  XMLParser::getElementAttributes(atributos);

  if(atributos[ParsersAttributes::CAST_TYPE]==
      ParsersAttributes::IMPLICIT)
   conv_tipo->setCastType(Cast::IMPLICIT);
  else
   conv_tipo->setCastType(Cast::ASSIGNMENT);

  conv_tipo->setInOut(atributos[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

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
      tipo=createPgSQLType();
      if(idx_tipo==0)
       conv_tipo->setDataType(Cast::SRC_TYPE, tipo);
      else
       conv_tipo->setDataType(Cast::DST_TYPE, tipo);
      idx_tipo++;
     }
     //Extraíndo a função de conversão do XML
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso da conversão, será extraída a refeênia   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribud   conversão */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(conv_tipo->getName()))
                             .arg(conv_tipo->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
      conv_tipo->setCastFunction(dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(conv_tipo) delete(conv_tipo);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_tipo);
}

Conversion *DatabaseModel::createConversion(void)
{
 map<QString, QString> atributos;
 Conversion *conv_codif=NULL;
 QString elem;
 BaseObject *funcao=NULL;

 try
 {
  conv_codif=new Conversion;

  //Lê do parser os atributos basicos
  setBasicAttributes(conv_codif);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  conv_codif->setEncoding(Conversion::SRC_ENCODING,
                                 EncodingType(atributos[ParsersAttributes::SRC_ENCODING]));

  conv_codif->setEncoding(Conversion::DST_ENCODING,
                                 EncodingType(atributos[ParsersAttributes::DST_ENCODING]));

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
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(conv_codif->getName()))
                             .arg(conv_codif->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
      conv_codif->setConversionFunction(dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(conv_codif) delete(conv_codif);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_codif);
}

Operator *DatabaseModel::createOperator(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 map<QString, unsigned> tipo_operadores;
 Operator *operador=NULL;
 QString elem;
 BaseObject *funcao=NULL,*oper_aux=NULL;
 unsigned tipo_func, tipo_oper, tipo_arg;
 PgSQLType tipo;

 try
 {
  operador=new Operator;

  //Lê do parser os atributos basicos
  setBasicAttributes(operador);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  operador->setMerges(atributos[ParsersAttributes::MERGES]==ParsersAttributes::_TRUE_);
  operador->setHashes(atributos[ParsersAttributes::HASHES]==ParsersAttributes::_TRUE_);

  /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
  tipo_funcoes[ParsersAttributes::OPERATOR_FUNC]=Operator::FUNC_OPERATOR;
  tipo_funcoes[ParsersAttributes::JOIN_FUNC]=Operator::FUNC_JOIN;
  tipo_funcoes[ParsersAttributes::RESTRICTION_FUNC]=Operator::FUNC_RESTRICTION;

  /* O mapa de tipos de operadores abaixo é usado para se atribuir de forma
      mais simples, sem comparações, o operador que for obtida do XML a qual
      o operador em construção referencia */
  tipo_operadores[ParsersAttributes::COMMUTATOR_OP]=Operator::OPER_COMMUTATOR;
  tipo_operadores[ParsersAttributes::GREATER_OP]=Operator::OPER_GREATER;
  tipo_operadores[ParsersAttributes::LESS_OP]=Operator::OPER_LESS;
  tipo_operadores[ParsersAttributes::NEGATOR_OP]=Operator::OPER_NEGATOR;
  tipo_operadores[ParsersAttributes::SORT_OP]=Operator::OPER_SORT1;
  tipo_operadores[ParsersAttributes::SORT2_OP]=Operator::OPER_SORT2;

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
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      oper_aux=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_OPERATOR);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!oper_aux && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(operador->getSignature(true)))
                             .arg(operador->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_OPERATOR)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_oper=tipo_operadores[atributos[ParsersAttributes::REF_TYPE]];
      operador->setOperator(dynamic_cast<Operator *>(oper_aux),tipo_oper);
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      /* Obtém os atributos do tipo para saber se o mesmo é um tipo da
         esquerda ou da direita */
      XMLParser::getElementAttributes(atributos);

      //Obtém o tipo de referência do tipo base (esquerda ou direita)
      if(atributos[ParsersAttributes::REF_TYPE]!=ParsersAttributes::RIGHT_TYPE)
       tipo_arg=Operator::LEFT_ARG;
      else
       tipo_arg=Operator::RIGHT_ARG;

      tipo=createPgSQLType();
      operador->setArgumentType(tipo, tipo_arg);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso do operador, será extraída a refernca   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída ao operador */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(operador->getName()))
                             .arg(operador->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      operador->setFunction(dynamic_cast<Function *>(funcao), tipo_func);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(operador) delete(operador);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(operador);
}

OperatorClass *DatabaseModel::createOperatorClass(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipos_elem;
 BaseObject *objeto=NULL;
 QString elem;
 PgSQLType tipo;
 OperatorClass *classe_op=NULL;
 OperatorClassElement elem_classe;
 bool rechecar;
 unsigned num_estrategia, tp_elem;

 try
 {
  classe_op=new OperatorClass;

  //Lê do parser os atributos basicos
  setBasicAttributes(classe_op);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  classe_op->setIndexingType(IndexingType(atributos[ParsersAttributes::INDEX_TYPE]));
  classe_op->setDefault(atributos[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

  tipos_elem[ParsersAttributes::FUNCTION]=OperatorClassElement::FUNCTION_ELEM;
  tipos_elem[ParsersAttributes::OPERATOR]=OperatorClassElement::OPERATOR_ELEM;
  tipos_elem[ParsersAttributes::STORAGE]=OperatorClassElement::STORAGE_ELEM;

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
      XMLParser::getElementAttributes(atributos);

      /* Com o nome da família do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe um objeto correspondente */
      objeto=getObject(atributos[ParsersAttributes::NAME], OBJ_OPFAMILY);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!objeto && !atributos[ParsersAttributes::NAME].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(classe_op->getName()))
                             .arg(classe_op->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      classe_op->setFamily(dynamic_cast<OperatorFamily *>(objeto));
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      //Obtém os atributos do tipo
      XMLParser::getElementAttributes(atributos);
      tipo=createPgSQLType();
      classe_op->setDataType(tipo);
     }
     else if(elem==ParsersAttributes::ELEMENT)
     {
      XMLParser::getElementAttributes(atributos);

      rechecar=atributos[ParsersAttributes::RECHECK]==ParsersAttributes::_TRUE_;
      num_estrategia=atributos[ParsersAttributes::STRATEGY_NUM].toUInt();
      tp_elem=tipos_elem[atributos[ParsersAttributes::TYPE]];

      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      XMLParser::getElementAttributes(atributos);

      if(tp_elem==OperatorClassElement::STORAGE_ELEM)
      {
       tipo=createPgSQLType();
       elem_classe.setStorage(tipo);
      }
      else if(tp_elem==OperatorClassElement::FUNCTION_ELEM)
      {
       objeto=getObject(atributos[ParsersAttributes::SIGNATURE],OBJ_FUNCTION);
       elem_classe.setFunction(dynamic_cast<Function *>(objeto),num_estrategia);
      }
      else if(tp_elem==OperatorClassElement::OPERATOR_ELEM)
      {
       objeto=getObject(atributos[ParsersAttributes::SIGNATURE],OBJ_OPERATOR);
       elem_classe.setOperator(dynamic_cast<Operator *>(objeto),num_estrategia,rechecar);
      }

      classe_op->addElement(elem_classe);
      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(classe_op) delete(classe_op);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(classe_op);
}

OperatorFamily *DatabaseModel::createOperatorFamily(void)
{
 map<QString, QString> atributos;
 OperatorFamily *familia_op=NULL;

 try
 {
  familia_op=new OperatorFamily;
  setBasicAttributes(familia_op);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do objeto
  familia_op->setIndexingType(IndexingType(atributos[ParsersAttributes::INDEX_TYPE]));
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(familia_op) delete(familia_op);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(familia_op);
}

Aggregate *DatabaseModel::createAggregate(void)
{
 map<QString, QString> atributos;
 BaseObject *funcao=NULL;
 QString elem;
 PgSQLType tipo;
 Aggregate *func_agreg=NULL;

 try
 {
  func_agreg=new Aggregate;

  //Lê do parser os atributos basicos
  setBasicAttributes(func_agreg);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);
  func_agreg->setInitialCondition(atributos[ParsersAttributes::INITIAL_COND]);

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
      XMLParser::getElementAttributes(atributos);
      tipo=createPgSQLType();

      //Define o tipo   função agregada de acordo com o tipo de referência do mesmo
      if(atributos[ParsersAttributes::REF_TYPE]==
          ParsersAttributes::STATE_TYPE)
       func_agreg->setStateType(tipo);
      else
       func_agreg->addDataType(tipo);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(func_agreg->getName()))
                             .arg(func_agreg->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a função de acordo com o tipo de referência da mesma
      if(atributos[ParsersAttributes::REF_TYPE]==ParsersAttributes::TRANSITION_FUNC)
       func_agreg->setFunction(Aggregate::TRANSITION_FUNC,
                                 dynamic_cast<Function *>(funcao));
      else
       func_agreg->setFunction(Aggregate::FINAL_FUNC,
                                 dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(func_agreg) delete(func_agreg);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(func_agreg);
}

Table *DatabaseModel::createTable(void)
{
 map<QString, QString> atributos;
 QString elem;
 Table *tabela=NULL;
 TableObject *objeto=NULL;

 try
 {
  tabela=new Table;

  //Lê do parser os atributos basicos
  setBasicAttributes(tabela);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);
  tabela->setWithOIDs(atributos[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);

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
     objeto=NULL;

     if(elem==BaseObject::objs_schemas[OBJ_COLUMN])
      objeto=createColumn();
     else if(elem==BaseObject::objs_schemas[OBJ_CONSTRAINT])
      objeto=createConstraint(tabela);
     else if(elem==BaseObject::objs_schemas[OBJ_INDEX])
      objeto=createIndex(tabela);
     else if(elem==BaseObject::objs_schemas[OBJ_RULE])
      objeto=createRule();
     else if(elem==BaseObject::objs_schemas[OBJ_TRIGGER])
      objeto=createTrigger(tabela);

     if(objeto)
      tabela->addObject(objeto);

     XMLParser::restorePosition();
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  tabela->setProtected(tabela->isProtected());
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  XMLParser::restorePosition();
  if(tabela) delete(tabela);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tabela);
}

Column *DatabaseModel::createColumn(void)
{
 map<QString, QString> atributos;
 Column *coluna=NULL;
 QString elem;

 try
 {
  coluna=new Column;
  setBasicAttributes(coluna);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  coluna->setNotNull(atributos[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);
  coluna->setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

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
      coluna->setType(createPgSQLType());
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(coluna) delete(coluna);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(coluna);
}

Constraint *DatabaseModel::createConstraint(BaseObject *objeto)
{
 map<QString, QString> atributos;
 Constraint *restricao=NULL;
 BaseObject *tabela_ref=NULL;
 Table *tabela=NULL,*tabela_aux=NULL;
 Column *coluna=NULL;
 Relationship *relacao=NULL;
 QString elem, str_aux;
 bool postergavel, ins_rest_tabela=false;
 ConstraintType tipo_rest;
 QStringList lista_cols;
 int qtd, i;
 unsigned tipo_coluna;
 ObjectType tipo_objeto;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Caso o objeto o qual será possuidor da restrição esteja alocado
  if(objeto)
  {
   tipo_objeto=objeto->getObjectType();
   if(tipo_objeto==OBJ_TABLE)
    tabela=dynamic_cast<Table *>(objeto);
   else if(tipo_objeto==OBJ_RELATIONSHIP)
    relacao=dynamic_cast<Relationship *>(objeto);
   else
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Caso o objeto não esteja especificado então o objeto possuidor será considerado
     como sendo sempre uma tabela e com base nisso o atributo "table" no código
     xml da restrição será usado para localizar a tabela possuidora no modelo */
  else
  {
   tipo_objeto=OBJ_TABLE;
   tabela=dynamic_cast<Table *>(getObject(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   ins_rest_tabela=true;
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  restricao=new Constraint;
  restricao->setParentTable(tabela);

  //Configurando o tipo da restrição
  if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::CK_CONSTR)
   tipo_rest=ConstraintType::check;
  else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::PK_CONSTR)
   tipo_rest=ConstraintType::primary_key;
  else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::FK_CONSTR)
   tipo_rest=ConstraintType::foreign_key;
  else
   tipo_rest=ConstraintType::unique;

  restricao->setConstraintType(tipo_rest);
  if(!atributos[ParsersAttributes::FACTOR].isEmpty())
   restricao->setFillFactor(atributos[ParsersAttributes::FACTOR].toUInt());
  setBasicAttributes(restricao);

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
  if(!objeto && tipo_rest==ConstraintType::primary_key)
    throw Exception(Exception::getErrorMessage(ERR_INV_PRIM_KEY_ALOCATION)
                  .arg(QString::fromUtf8(restricao->getName())),
                  ERR_INV_PRIM_KEY_ALOCATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Efetuando configurações específicas para chaves estrangeiras
  if(tipo_rest==ConstraintType::foreign_key /*&& tipo_objeto==OBJETO_TABELA*/)
  {
   //Define se a restrição é postergavel (apenas para chaves estrangeiras)
   postergavel=(atributos[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
   restricao->setDeferrable(postergavel);

   if(postergavel && !atributos[ParsersAttributes::DEFER_TYPE].isEmpty())
    restricao->setDeferralType(atributos[ParsersAttributes::DEFER_TYPE]);

   if(!atributos[ParsersAttributes::COMPARISON_TYPE].isEmpty())
    restricao->setMatchType(atributos[ParsersAttributes::COMPARISON_TYPE]);

   //Definindo os tipos de ação nos eventos DELETE e UPDATE
   if(!atributos[ParsersAttributes::DEL_ACTION].isEmpty())
    restricao->setActionType(atributos[ParsersAttributes::DEL_ACTION], false);

   if(!atributos[ParsersAttributes::UPD_ACTION].isEmpty())
    restricao->setActionType(atributos[ParsersAttributes::UPD_ACTION], true);

   //Obtém a tabela referenciada na chave estrangeira
   tabela_ref=getObject(atributos[ParsersAttributes::REF_TABLE], OBJ_TABLE);

   /* Caso a tabela referenciada não seja encontrada verifica se esta não é a própria
      tabela a qual receberá a restrição (usado para auto-relacionamentos) */
   if(!tabela_ref && tabela->getName(true)==atributos[ParsersAttributes::REF_TABLE])
    tabela_ref=tabela;

   /* Caso a tabela referenciada não foi encontrada uma exceção será disparada pois
      não se pode criar uma chave estrangeira sem referenciar uma tabela de destino */
   if(!tabela_ref)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(restricao->getName()))
          .arg(restricao->getTypeName())
          .arg(QString::fromUtf8(atributos[ParsersAttributes::REF_TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Define a tabela de destino da chave estrangeira
   restricao->setReferencedTable(tabela_ref);
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
      restricao->setCheckExpression(XMLParser::getElementContent());
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(atributos);

      /* Obtém os nomes das colunas participantes da restrição
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_cols=atributos[ParsersAttributes::NAMES].split(',');
      qtd=lista_cols.count();

      /* Obtém o tipo de referência das colunas de acordo com o atributo
         tipo de referência vindo do XML */
      if(atributos[ParsersAttributes::REF_TYPE]==ParsersAttributes::SRC_COLUMNS)
       tipo_coluna=Constraint::SOURCE_COLS;
      else
       tipo_coluna=Constraint::REFERENCED_COLS;

      //Varre a lista de nomes de colunas e as obtém da tabela a qual possuirá a restrição
      for(i=0; i < qtd; i++)
      {
       if(tipo_coluna==Constraint::SOURCE_COLS)
       {
        if(tipo_objeto==OBJ_TABLE)
        {
         coluna=tabela->getColumn(lista_cols[i]);
         //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
         if(!coluna)
          coluna=tabela->getColumn(lista_cols[i], true);
        }
        else
         //Para os demais tipos de relacionamento as colunas a serem obtidas são os atributos do relacionamento
          coluna=dynamic_cast<Column *>(relacao->getObject(lista_cols[i], OBJ_COLUMN));
       }
       else
       {
        tabela_aux=dynamic_cast<Table *>(tabela_ref);
        coluna=tabela_aux->getColumn(lista_cols[i]);
        //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
        if(!coluna)
         coluna=tabela_aux->getColumn(lista_cols[i], true);
       }

       //Adiciona a coluna   restrição
       restricao->addColumn(coluna, tipo_coluna);
      }
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(ins_rest_tabela)
  {
   //Caso a restrição criada não seja uma chave primária insere-a normalmente na tabela
   if(restricao->getConstraintType()!=ConstraintType::primary_key)
   {
    tabela->addConstraint(restricao);

    /* Caso a tabela receptora da restrição esteja inserida no modelo, força o seu redesenho.
       Isso é útil para atualizar tabelas as quais tiveram restrições adicionadas após a sua
       criação */
    if(this->getObjectIndex(tabela) >= 0)
      tabela->setModified(true);
   }
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(restricao)
   delete(restricao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(restricao);
}

Index *DatabaseModel::createIndex(Table *tabela)
{
 map<QString, QString> atributos;
 Index *indice=NULL;
 Column *coluna=NULL;
 OperatorClass *classe_oper=NULL;
 QString elem, str_aux, expr;
 bool inc_ind_tabela=false,
      ordem_asc=false, nulos_primeiro=false;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  if(!tabela)
  {
   inc_ind_tabela=true;
   tabela=dynamic_cast<Table *>(getObject(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_INDEX))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  indice=new Index;
  setBasicAttributes(indice);
  indice->setParentTable(tabela);
  indice->setIndexAttribute(Index::CONCURRENT, atributos[ParsersAttributes::CONCURRENT]==ParsersAttributes::_TRUE_);
  indice->setIndexAttribute(Index::UNIQUE, atributos[ParsersAttributes::UNIQUE]==ParsersAttributes::_TRUE_);
  indice->setIndexAttribute(Index::FAST_UPDATE, atributos[ParsersAttributes::FAST_UPDATE]==ParsersAttributes::_TRUE_);

  indice->setIndexingType(atributos[ParsersAttributes::INDEX_TYPE]);
  indice->setFillFactor(atributos[ParsersAttributes::FACTOR].toUInt());

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
      nulos_primeiro=(atributos[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
      ordem_asc=(atributos[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);

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
         XMLParser::getElementAttributes(atributos);
         classe_oper=dynamic_cast<OperatorClass *>(getObject(atributos[ParsersAttributes::NAME], OBJ_OPCLASS));

         //Caso o índice esteja referenciando uma classe de operadores inexistente
         if(!classe_oper)
         {
          //Configura os argumentos da mensagem de erro
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(indice->getName()))
                          .arg(BaseObject::getTypeName(OBJ_INDEX))
                          .arg(QString::fromUtf8(atributos[ParsersAttributes::OP_CLASS]))
                          .arg(BaseObject::getTypeName(OBJ_OPCLASS));
          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }
        //Caso o elemento atual seja um  <column>
        else if(elem==ParsersAttributes::COLUMN)
        {
         //Obtém a coluna que o elemento referencia
         XMLParser::getElementAttributes(atributos);
         coluna=tabela->getColumn(atributos[ParsersAttributes::NAME]);

         /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
            seu nome antigo */
         if(!coluna)
          coluna=tabela->getColumn(atributos[ParsersAttributes::NAME], true);
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
       indice->addElement(expr, classe_oper, ordem_asc, nulos_primeiro);
      else
       indice->addElement(coluna, classe_oper, ordem_asc, nulos_primeiro);

      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();
      indice->setConditionalExpression(str_aux);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(inc_ind_tabela)
  {
   tabela->addIndex(indice);
   tabela->setModified(true);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(indice) delete(indice);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(indice);
}

Rule *DatabaseModel::createRule(void)
{
 map<QString, QString> atributos;
 QStringList lista_cmd;
 Rule *regra=NULL;
 QString elem, str_aux;
 int qtd, i;

 try
 {
  regra=new Rule;
  setBasicAttributes(regra);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  regra->setExecutionType(atributos[ParsersAttributes::EXEC_TYPE]);
  regra->setEventType(atributos[ParsersAttributes::EVENT_TYPE]);

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
       lista_cmd=str_aux.split(';');
       qtd=lista_cmd.count();
       for(i=0; i < qtd; i++)
       {
        //Caso o comando não esteja vazio
        if(!lista_cmd[i].isEmpty())
         regra->addCommand(lista_cmd[i]);
       }
      }
      else
       regra->setConditionalExpression(str_aux);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(regra) delete(regra);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(regra);
}

Trigger *DatabaseModel::createTrigger(Table *tabela)
{
 map<QString, QString> atributos;
 Trigger *gatilho=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 int qtd, i;
 BaseObject *tabela_ref=NULL, *funcao=NULL;
 Column *coluna=NULL;
 bool inc_gat_tabela=false;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  if(!tabela && atributos[ParsersAttributes::TABLE].isEmpty())
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(!tabela && !atributos[ParsersAttributes::TABLE].isEmpty())
  {
   inc_gat_tabela=true;
   tabela=dynamic_cast<Table *>(getObject(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   if(!tabela)
    throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                  .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                  .arg(BaseObject::getTypeName(OBJ_TRIGGER))
                  .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
                  .arg(BaseObject::getTypeName(OBJ_TABLE)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  gatilho=new Trigger;
  gatilho->setParentTable(tabela);

  setBasicAttributes(gatilho);

  //Marcando os eventos de execução do gatilho
  gatilho->setEvent(EventType::on_insert,
                        (atributos[ParsersAttributes::INS_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(EventType::on_delete,
                        (atributos[ParsersAttributes::DEL_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(EventType::on_update,
                        (atributos[ParsersAttributes::UPD_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(EventType::on_truncate,
                        (atributos[ParsersAttributes::TRUNC_EVENT]==
                         ParsersAttributes::_TRUE_));

  //Marcando e o gatilho é executado por linha ou não
  gatilho->setExecutePerRow(atributos[ParsersAttributes::PER_ROW]==
                            ParsersAttributes::_TRUE_);

  //Define o modo de disparo do gatilho
  gatilho->setFiringType(FiringType(atributos[ParsersAttributes::FIRING_TYPE]));


  /* Atribuindo os argumentos vindo do XML ao gatilho.
     No XML os argumentos estão separados por vírgula,
     sendo assim o método split é usado para quebrar a
     string de argumentos e atribui-los ao objeto */
  lista_aux=atributos[ParsersAttributes::ARGUMENTS].split(',');
  qtd=lista_aux.count();
  for(i=0; i < qtd; i++)
  {
   if(!lista_aux[i].isEmpty())
    gatilho->addArgument(lista_aux[i]);
  }

  //Caso o objeto alocado seja um gatilho
  gatilho->setDeferrable(atributos[ParsersAttributes::DEFERRABLE]==
                              ParsersAttributes::_TRUE_);
  if(gatilho->isDeferrable())
    gatilho->setDeferralType(atributos[ParsersAttributes::DEFER_TYPE]);

  //Obtém a tabela referenciada no gatilho
  tabela_ref=getObject(atributos[ParsersAttributes::REF_TABLE], OBJ_TABLE);
  gatilho->setReferecendTable(tabela_ref);

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
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=getObject(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
      {
       str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
               .arg(QString::fromUtf8(gatilho->getName()))
               .arg(gatilho->getTypeName())
               .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
               .arg(BaseObject::getTypeName(OBJ_FUNCTION));

       //Dispara a exceção
       throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }

      //Define a função executada pelo gatilho
      gatilho->setFunction(dynamic_cast<Function *>(funcao));
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();
      gatilho->setCondition(str_aux);
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(atributos);

      /* Obtém os nomes das colunas participantes do gatilho
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_aux=atributos[ParsersAttributes::NAMES].split(',');
      qtd=lista_aux.count();

      for(i=0; i < qtd; i++)
      {
       coluna=tabela->getColumn(lista_aux[i]);

       /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
          seu nome antigo */
       if(!coluna)
        coluna=tabela->getColumn(lista_aux[i], true);

       gatilho->addColumn(coluna);
      }
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(inc_gat_tabela)
  {
   tabela->addObject(gatilho);
   tabela->setModified(true);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(gatilho) delete(gatilho);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(gatilho);
}

Sequence *DatabaseModel::createSequence(bool ignorar_possuidora)
{
 map<QString, QString> atributos;
 Sequence *sequencia=NULL;
 BaseObject *tabela=NULL;
 Column *coluna=NULL;
 QString elem, str_aux, nome_tab, nome_col;
 QStringList lista_elem;
 int qtd;

 try
 {
  sequencia=new Sequence;
  setBasicAttributes(sequencia);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  sequencia->setValues(atributos[ParsersAttributes::MIN_VALUE],
                            atributos[ParsersAttributes::MAX_VALUE],
                            atributos[ParsersAttributes::INCREMENT],
                            atributos[ParsersAttributes::START],
                            atributos[ParsersAttributes::CACHE]);
  sequencia->setCycle(atributos[ParsersAttributes::CYCLE]==ParsersAttributes::_TRUE_);

  //Caso o atributo de coluna possuidora da sequencia esteja preenchido
  if(!atributos[ParsersAttributes::OWNER_COLUMN].isEmpty())
  {
   //Quebra o valor do atributo por .
   lista_elem=atributos[ParsersAttributes::OWNER_COLUMN].split('.');
   qtd=lista_elem.count();

   /* Caso a lista de nomes gerada possua 3 elementos indica
      que a coluna possuidora foi está no formato
      [ESQUEMA].[TABELA].[COLUNA] caso contrário
      supõe-se que esteja no formato
      [TABELA].[COLUNA] */
   if(qtd==3)
   {
    nome_tab=lista_elem[0] + "." + lista_elem[1];
    nome_col=lista_elem[2];
   }
   else
   {
    nome_tab=lista_elem[0];
    nome_col=lista_elem[1];
   }

   //Obtém a tabela do modelo
   tabela=getObject(nome_tab, OBJ_TABLE);

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabela)
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
            .arg(QString::fromUtf8(sequencia->getName()))
            .arg(BaseObject::getTypeName(OBJ_SEQUENCE))
            .arg(QString::fromUtf8(nome_tab))
            .arg(BaseObject::getTypeName(OBJ_TABLE));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Tenta obter a coluna da tabela com o nome vindo do XML
   coluna=dynamic_cast<Table *>(tabela)->getColumn(nome_col);

   //Caso a coluna não for encontrada tenta obtê-la referenciando o antigo nome
   if(!coluna)
    coluna=dynamic_cast<Table *>(tabela)->getColumn(nome_col, true);

   /* Caso a coluna não exista porém a mesma esteja sendo referenciada no xml
      um erro será disparado */
   if(!coluna && !ignorar_possuidora)
    throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
                  .arg(QString::fromUtf8(sequencia->getName(true))),
                  ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   sequencia->setOwnerColumn(coluna);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(sequencia) delete(sequencia);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(sequencia);
}

View *DatabaseModel::createView(void)
{
 map<QString, QString> atributos;
 View *visao=NULL;
 Column *coluna=NULL;
 Table *tabela=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 vector<Reference> vet_refs;
 unsigned tipo;
 int idx_ref, i, qtd;

 try
 {
  visao=new View;
  setBasicAttributes(visao);

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
      XMLParser::getElementAttributes(atributos);

      /* Caso o nome da tabela referenciada esteja preenchido,
         tentar criar uma referência específica a uma tabela/coluna */
      if(!atributos[ParsersAttributes::TABLE].isEmpty())
      {
       coluna=NULL;
       tabela=dynamic_cast<Table *>(getObject(atributos[ParsersAttributes::TABLE], OBJ_TABLE));

       //Dispara uma exceção caso a tabela referenciada não exista
       if(!tabela)
       {
        str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                        .arg(QString::fromUtf8(visao->getName()))
                        .arg(BaseObject::getTypeName(OBJ_VIEW))
                        .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
                        .arg(BaseObject::getTypeName(OBJ_TABLE));

        //Dispara a exceção
        throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }

       if(!atributos[ParsersAttributes::COLUMN].isEmpty())
       {
        //Tenta obter a colna referenciada da tabela
        coluna=tabela->getColumn(atributos[ParsersAttributes::COLUMN]);

        //Caso a coluna não exista tenta obtê-la referenciando o nome antigo da mesma
        if(!coluna)
         coluna=tabela->getColumn(atributos[ParsersAttributes::COLUMN], true);

        /* Caso o atributo coluna da referencia esteja preenchido mas um objeto coluna
           não foi encontrado na tabela, uma exceção será disparada pois a visão está
           referenciando uma coluna inexistente na tabela */
         if(!coluna)
         {
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(visao->getName()))
                          .arg(BaseObject::getTypeName(OBJ_VIEW))
                          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]) + "." +
                               QString::fromUtf8(atributos[ParsersAttributes::COLUMN]))
                         .arg(BaseObject::getTypeName(OBJ_COLUMN));

          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }

       //Adiciona a referência configurad  lista temporária de referências
       vet_refs.push_back(Reference(tabela, coluna,
                                     atributos[ParsersAttributes::ALIAS],
                                     atributos[ParsersAttributes::COLUMN_ALIAS]));
      }
      //Extraindo uma referênci  uma expressão
      else
      {
       XMLParser::savePosition();
       //Armazena o alias da expressão
       str_aux=atributos[ParsersAttributes::ALIAS];

       //Acessa e obtém o conteúdo da expressão
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       vet_refs.push_back(Reference(XMLParser::getElementContent(),str_aux));

       XMLParser::restorePosition();
      }
     }
     /* Extraindo as expressões as quais formam as partes da declaração da visão,
        ou seja, expressões e referências as quais estão entre SELECT-FROM,
        FROM-WHERE */
     else if(elem==ParsersAttributes::EXPRESSION)
     {
      XMLParser::savePosition();
      XMLParser::getElementAttributes(atributos);

      //Armazena o alias da expressão
      if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
       tipo=Reference::SQL_REFER_SELECT;
      else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
       tipo=Reference::SQL_REFER_FROM;
      else
       tipo=Reference::SQL_REFER_WHERE;

      //Acessa e obtém o conteúdo da expressão
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      lista_aux=XMLParser::getElementContent().split(',');
      qtd=lista_aux.size();

      //Construindo cada expressão na visão
      for(i=0; i < qtd; i++)
      {
       //Obtém o índice da referência e a adiioa   visão
       idx_ref=lista_aux[i].toInt();
       visao->addReference(vet_refs[idx_ref],tipo);
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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(visao) delete(visao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(visao);
}

Textbox *DatabaseModel::createTextbox(void)
{
 Textbox *caixa_texto=NULL;
 map<QString, QString> atributos;

 try
 {
  caixa_texto=new Textbox;
  setBasicAttributes(caixa_texto);

  XMLParser::getElementAttributes(atributos);

  if(atributos[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::ITALIC_TXT, true);

  if(atributos[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::BOLD_TXT, true);

  if(atributos[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::UNDERLINE_TXT, true);

  if(!atributos[ParsersAttributes::COLOR].isEmpty())
   caixa_texto->setTextColor(QColor(atributos[ParsersAttributes::COLOR]));
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(caixa_texto) delete(caixa_texto);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(caixa_texto);
}

BaseRelationship *DatabaseModel::createRelationship(void)
{
 vector<unsigned> cols_pk_especial;
 map<QString, QString> atributos;
 BaseRelationship *relacao_base=NULL;
 Relationship *relacao=NULL;
 BaseTable *tabelas[2]={NULL, NULL};
 bool obrig_orig, obrig_dest, identificador, protegido, postergavel, sufixo_auto;
 DeferralType tipo_postergacao;
 unsigned tipo_relac=0, i;
 ObjectType tipos_tab[2]={OBJ_VIEW, OBJ_TABLE}, tipo_obj_rel;
 QString str_aux, elem,
         atribs[2]={ ParsersAttributes::SRC_TABLE,
                     ParsersAttributes::DST_TABLE };

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  protegido=(atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

  if(atributos[ParsersAttributes::TYPE]!=ParsersAttributes::RELATION_TAB_VIEW &&
     atributos[ParsersAttributes::TYPE]!=ParsersAttributes::RELATIONSHIP_FK)
  {
   tipos_tab[0]=OBJ_TABLE;
   tipo_obj_rel=OBJ_RELATIONSHIP;
  }
  else
  {
   if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
    tipos_tab[0]=OBJ_TABLE;

   tipo_obj_rel=BASE_RELATIONSHIP;
  }

  /* Esta iteração obtém as tabelas participantes do relacionamento a
     partir do modelo com base nos nomes das tabelas vindos do XML */
  for(i=0; i < 2; i++)
  {
   //Localiza a tabela
   tabelas[i]=dynamic_cast<BaseTable *>(getObject(atributos[atribs[i]], tipos_tab[i]));

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabelas[i])
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                    .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                    .arg(BaseObject::getTypeName(tipo_obj_rel))
                    .arg(QString::fromUtf8(atributos[atribs[i]]))
                    .arg(BaseObject::getTypeName(tipos_tab[i]));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATION_TAB_VIEW)
  {
   //Caso o relacionamento entre tabela e visão exista
   relacao_base=getRelationship(tabelas[0], tabelas[1]);

   //Caso o relacionamento tabela-visão nao seja encontrado o erro será disparado
   if(!relacao_base)
    throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(this->getName()))
                             .arg(this->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Desconecta o relacionamento para configurá-lo
   relacao_base->disconnectRelationship();
   relacao_base->setName(atributos[ParsersAttributes::NAME]);
  }
  else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_FK)
  {
   relacao_base=new BaseRelationship(BaseRelationship::RELATIONSHIP_FK,
                                     tabelas[0], tabelas[1], false, false);
   relacao_base->setName(atributos[ParsersAttributes::NAME]);
  }
  /* Caso o tipo de relacionamento não seja tabela-visão, isso indica que
     um relacionamento tabela-tabela deverá ser criado */
  else
  {
   //Obtém os atributos do relacionamento a partir do XML
   obrig_orig=atributos[ParsersAttributes::SRC_REQUIRED]==ParsersAttributes::_TRUE_;
   obrig_dest=atributos[ParsersAttributes::DST_REQUIRED]==ParsersAttributes::_TRUE_;
   identificador=atributos[ParsersAttributes::IDENTIFIER]==ParsersAttributes::_TRUE_;
   postergavel=atributos[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_;
   sufixo_auto=(!atributos[ParsersAttributes::AUTO_SUFFIX].isEmpty() &&
                atributos[ParsersAttributes::AUTO_SUFFIX]==ParsersAttributes::_TRUE_);
   tipo_postergacao=DeferralType(atributos[ParsersAttributes::DEFER_TYPE]);

   //Configura o tipo do novo relacionamento
   if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_11)
    tipo_relac=BaseRelationship::RELATIONSHIP_11;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_1N)
    tipo_relac=BaseRelationship::RELATIONSHIP_1N;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_NN)
    tipo_relac=BaseRelationship::RELATIONSHIP_NN;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_GEN)
    tipo_relac=BaseRelationship::RELATIONSHIP_GEN;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_DEP)
    tipo_relac=BaseRelationship::RELATIONSHIP_DEP;

   //Cria o novo relacionamento
   relacao=new Relationship(tipo_relac,
                              dynamic_cast<Table *>(tabelas[0]),
                              dynamic_cast<Table *>(tabelas[1]),
                              obrig_orig, obrig_dest,
                              sufixo_auto, atributos[ParsersAttributes::SRC_SUFFIX],
                              atributos[ParsersAttributes::DST_SUFFIX],
                              identificador, postergavel, tipo_postergacao);

    if(!atributos[ParsersAttributes::TABLE_NAME].isEmpty())
    relacao->setTableNameRelNN(atributos[ParsersAttributes::TABLE_NAME]);

   /* Faz com que o ponteiro relacao_base aponte para o novo relacionamento
      para executar as configurações geréricas as quais se aplicam tanto
      para relacionametno tabela-visao quanto para rel. tabela-tabela */
   relacao_base=relacao;
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

     if(elem==ParsersAttributes::COLUMN && relacao)
     {
      XMLParser::savePosition();
      relacao->addObject(createColumn());
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::CONSTRAINT && relacao)
     {
      XMLParser::savePosition();
      relacao->addObject(createConstraint(relacao));
      XMLParser::restorePosition();
     }
     //Configurando a linha do relacionamento
     else if(elem==ParsersAttributes::LINE)
     {
      vector<QPointF> pontos;
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       //Lê o ponto do XML
       XMLParser::getElementAttributes(atributos);
       pontos.push_back(QPointF(atributos[ParsersAttributes::X_POS].toFloat(),
                                atributos[ParsersAttributes::Y_POS].toFloat()));
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      relacao_base->setPoints(pontos);
      XMLParser::restorePosition();
     }
     //Configurando a posição dos rótulos
     else if(elem==ParsersAttributes::LABEL)
     {
      XMLParser::getElementAttributes(atributos);
      //Obtém o tipo de rótulo a ser configurado
      str_aux=atributos[ParsersAttributes::REF_TYPE];

      //Acessa o elemento filho da tag <label> o qual armazena a posição do rótulo
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      XMLParser::getElementAttributes(atributos);
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::SPECIAL_PK_COLS && relacao)
     {
      QList<QString> lista_cols;

      //Obtém os atributos da tag <special-pk-cols>
      XMLParser::getElementAttributes(atributos);
      lista_cols=atributos[ParsersAttributes::INDEXES].split(',');

      while(!lista_cols.isEmpty())
      {
       cols_pk_especial.push_back(lista_cols.front().toUInt());
       lista_cols.pop_front();
      }

      //Define as colunas que fazem parte da chave primária especila
      relacao->setSpecialPrimaryKeyCols(cols_pk_especial);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(relacao_base && relacao_base->getObjectType()==OBJ_RELATIONSHIP)
   delete(relacao_base);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Caso o relacionamento tabela-tabela foi criado o mesmo será adicionado no modelo
 if(relacao)
 {
  storeSpecialObjectsXML();
  addRelationship(relacao);
 }
 else if(relacao_base->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
  addRelationship(relacao_base);

 //Define a proteção do relacionamento
 relacao_base->setProtected(protegido);

 //Reconecta o relacionamento caso o mesmo seja um rel. tabela-visao
 if(relacao_base && relacao_base->getObjectType()==BASE_RELATIONSHIP)
  relacao_base->connectRelationship();

 return(relacao_base);
}

Permission *DatabaseModel::createPermission(void)
{
 Permission *permissao=NULL;
 BaseObject *objeto=NULL;
 Table *tabela_pai=NULL;
 Role *papel=NULL;
 map<QString, QString> atrib_priv, atributos;
 map<QString, QString>::iterator itr, itr_end;
 ObjectType tipo_obj;
 QString obj_pai, nome_obj;
 QStringList lista;
 unsigned i, tam, tipo_priv=Permission::PRIV_SELECT;
 bool valor_priv, op_concessao;

 try
 {
  //Obtém os privilégios configurados para a permissão
  XMLParser::getElementAttributes(atrib_priv);

  /* Acessa o elemento filho <object> o qual armazena o objeto
     do modelo relacionado   permissão */
  XMLParser::savePosition();
  XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
  XMLParser::getElementAttributes(atributos);

  //Obtém os atributos do objeto que é referenciado pela  permissão
  tipo_obj=getObjectType(atributos[ParsersAttributes::TYPE]);
  nome_obj=atributos[ParsersAttributes::NAME];
  obj_pai=atributos[ParsersAttributes::PARENT];

  //Caso o objeto seja uma coluna a mesma será obtida da tabela pai
  if(tipo_obj==OBJ_COLUMN)
  {
   //Primeiramente a tabela pai é obtida do modelo
   tabela_pai=dynamic_cast<Table *>(getObject(obj_pai, OBJ_TABLE));

   /* Caso a tabela pai existe obtém o objeto filho da mesma
      o qual é referenciado pela permissão */
   if(tabela_pai)
    objeto=tabela_pai->getColumn(nome_obj);
  }
  else if(tipo_obj==OBJ_DATABASE)
  {
   objeto=this;
  }
  else
   /* Para os demais tipos de objetos, aceitos como referenciados
      por permissões, serão obtidos do modelo em si */
   objeto=getObject(nome_obj, tipo_obj);

  /* Caso o objeto não exista será disparada uma exceção pois uma permissão
     não pode existir sem que referencie um objeto */
  if(!objeto)
   throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                          .arg(QString::fromUtf8(nome_obj))
                          .arg(BaseObject::getTypeName(tipo_obj)),
                      ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Aloca a permissão relacionando-a com o objeto localizado
  permissao=new Permission(objeto);

  /* Acessa o elemento que armazena os privilégios dos papéis os quais
     compartilham da mesma permissão sobre o objeto. Informar estes
     papéis não é obrigatório de acordo com a DTD e com a especificação
     SQL para permissões */
  do
  {
   if(XMLParser::getElementName()==ParsersAttributes::ROLES)
   {
    //Obtém os atributos do elemento <roles>, neste caso são names e reftype
    XMLParser::getElementAttributes(atributos);

    /* O atributo names armazena uma lista de nomes de papéis as quais a permissão
       referenciará. A lista tem os elementos separados por vírgula, sendo assim a
       string será quebrada usando o delimitador ',') */
    lista=atributos[ParsersAttributes::NAMES].split(',');

    //Obtém a quantidade de nomes de papéis na lista
    tam=lista.size();

    //Varre a lista de nomes de papéis
    for(i=0; i < tam; i++)
    {
     //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
     papel=dynamic_cast<Role *>(getObject(lista[i].trimmed(),OBJ_ROLE));

     /* Caso esse papel não exista um erro será disparado pois um novo papel
        não pode referenciar um outro papel que ainda nem foi criado */
     if(!papel)
     {
      //Dispara a exceção
      throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                              .arg(QString::fromUtf8(objeto->getName()))
                              .arg(objeto->getTypeName())
                              .arg(QString::fromUtf8(lista[i]))
                              .arg(BaseObject::getTypeName(OBJ_ROLE)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

     }
     //Adiciona o papel   permissão
     permissao->addRole(papel);
    }
   }
   else if(XMLParser::getElementName()==ParsersAttributes::PRIVILEGES)
   {
    //Obtém os atributos do elemento <privileges>
    XMLParser::getElementAttributes(atrib_priv);

    //Atribui os privilégio  permissão recém criada
    itr=atrib_priv.begin();
    itr_end=atrib_priv.end();

    /* Varre o mapa de privilégios configurando-os na permissão caso
       estes estejam definidos no XML */
    while(itr!=itr_end)
    {
     if(itr->first!=ParsersAttributes::GRANT_OP)
     {
      //Obtém o valor do privilégio (true/false)
      valor_priv=(itr->second==ParsersAttributes::_TRUE_);
      op_concessao=(itr->second==ParsersAttributes::GRANT_OP);

      //Identifica o tipo de privilégio atual
      if(itr->first==ParsersAttributes::CONNECT_PRIV)
       tipo_priv=Permission::PRIV_CONNECT;
      else if(itr->first==ParsersAttributes::CREATE_PRIV)
       tipo_priv=Permission::PRIV_CREATE;
      else if(itr->first==ParsersAttributes::DELETE_PRIV)
       tipo_priv=Permission::PRIV_DELETE;
      else if(itr->first==ParsersAttributes::EXECUTE_PRIV)
       tipo_priv=Permission::PRIV_EXECUTE;
      else if(itr->first==ParsersAttributes::INSERT_PRIV)
       tipo_priv=Permission::PRIV_INSERT;
      else if(itr->first==ParsersAttributes::REFERENCES_PRIV)
       tipo_priv=Permission::PRIV_REFERENCES;
      else if(itr->first==ParsersAttributes::SELECT_PRIV)
       tipo_priv=Permission::PRIV_SELECT;
      else if(itr->first==ParsersAttributes::TEMPORARY_PRIV)
       tipo_priv=Permission::PRIV_TEMPORARY;
      else if(itr->first==ParsersAttributes::TRIGGER_PRIV)
       tipo_priv=Permission::PRIV_TRIGGER;
      else if(itr->first==ParsersAttributes::TRUNCATE_PRIV)
       tipo_priv=Permission::PRIV_TRUNCATE;
      else if(itr->first==ParsersAttributes::UPDATE_PRIV)
       tipo_priv=Permission::PRIV_UPDATE;
      else if(itr->first==ParsersAttributes::USAGE_PRIV)
       tipo_priv=Permission::PRIV_USAGE;

      //Configura o privilégio na permissão
      permissao->setPrivilege(tipo_priv, (valor_priv || op_concessao), op_concessao);
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
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(permissao) delete(permissao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(permissao);
}

void DatabaseModel::validateColumnRemoval(Column *coluna)
{
 if(coluna && coluna->getParentTable())
 {
  vector<BaseObject *> vet_refs;
  getObjectReferences(coluna, vet_refs);

  //Caso um objeto seja encontrado o qual referencia a coluna
  if(!vet_refs.empty())
   //Dispara um erro informando que a coluna não pde ser remove e qual objeto a referencia
   throw Exception(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE)
                 .arg(QString::fromUtf8(coluna->getParentTable()->getName(true)) + "." + QString::fromUtf8(coluna->getName(true)))
                 .arg(coluna->getTypeName())
                 .arg(QString::fromUtf8(vet_refs[0]->getName(true)))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void DatabaseModel::validateRelationships(TableObject *objeto, Table *tabela_pai)
{
 try
 {
  bool revalidar_rels=false, tab_ref_heranca=false;
  Relationship *rel=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  ObjectType tipo;

  if(objeto && tabela_pai)
  {
   tipo=objeto->getObjectType();

   /* Condição de revalidação de relacionamentos:
      > Caso seja uma coluna e a mesma é referenciada pela chave primária da tabela pai
      > Caso seja uma restrição e a mesma seja uma chave primária da tabela */
    revalidar_rels=((tipo==OBJ_COLUMN &&
                     tabela_pai->isConstraintRefColumn(dynamic_cast<Column *>(objeto), ConstraintType::primary_key)) ||
                    (tipo==OBJ_CONSTRAINT &&
                     dynamic_cast<Constraint *>(objeto)->getConstraintType()==ConstraintType::primary_key));

   /* Caso seja uma coluna, verfica se a tabela pai participa de um relacionamento
     de generalização como tabela de destino (aquela que tem suas colunas copiadas
     para a tabela qua a herda) */
   if(tipo==OBJ_COLUMN)
   {
    itr=relationships.begin();
    itr_end=relationships.end();

    while(itr!=itr_end && !tab_ref_heranca)
    {
     rel=dynamic_cast<Relationship *>(*itr);
     itr++;
     tab_ref_heranca=(rel->getRelationshipType()==Relationship::RELATIONSHIP_GEN &&
                      rel->getReferenceTable()==tabela_pai);
    }
   }

   //Caso as duas condições acima sejam atendidas
   if(revalidar_rels || tab_ref_heranca)
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

QString DatabaseModel::__getCodeDefinition(unsigned tipo_def)
{
 if(conn_limit >= 0)
  attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(conn_limit);

 if(tipo_def==SchemaParser::SQL_DEFINITION)
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
 return(this->BaseObject::__getCodeDefinition(tipo_def));
}

QString DatabaseModel::getCodeDefinition(unsigned tipo_def)
{
 return(this->getCodeDefinition(tipo_def, true));
}

QString DatabaseModel::getCodeDefinition(unsigned tipo_def, bool exportar_arq)
{
 map<QString, QString> atribs_aux;
 unsigned qtd1, i, qtd;
 float qtd_geral_obj, qtd_defs_geradas;
 BaseObject *objeto=NULL;
 vector<BaseObject *> *lista_obj=NULL;
 vector<BaseObject *>::iterator itr, itr_end;
 vector<unsigned>::iterator itr1, itr1_end;
 QString msg=trUtf8("Generating %1 of the object: %2 (%3)"),
         atrib=ParsersAttributes::OBJECTS,
         tipo_def_str=(tipo_def==SchemaParser::SQL_DEFINITION ? "SQL" : "XML");
 Type *tipo_usr=NULL;
 map<unsigned, BaseObject *> mapa_objetos;
 vector<unsigned> vet_id_objs, vet_id_objs_tab;
 vector<Constraint *> vet_fks;
 Table *tabela=NULL;
 Index *indice=NULL;
 Trigger *gatilho=NULL;
 Constraint *restricao=NULL;
 Relationship *relacao=NULL;
 ObjectType tipo_obj,
                tipos_obj_aux[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA },
                tipos_obj[]={ OBJ_LANGUAGE, OBJ_FUNCTION, OBJ_TYPE,
                              OBJ_CAST, OBJ_CONVERSION,
                              OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
                              OBJ_AGGREGATE, OBJ_DOMAIN, OBJ_TEXTBOX, BASE_RELATIONSHIP,
                              OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE };
 try
 {
  qtd_geral_obj=this->getObjectCount();
  qtd_defs_geradas=0;

  /* Tratandos os objetos os quais tem ids fixos são eles: Papel, Espaço de Tabela,
     e Esquema. Estes precisam ser tratados separadamente no laço abaixo pois não
     entram na ordenação de ids executada para os demais tipos de objetos. */
  for(i=0; i < 3; i++)
  {
   //Obtém a lista de objetos do tipo
   lista_obj=getObjectList(tipos_obj_aux[i]);

   /* Caso o tipo de definição seja SQL obtem o nome do atributo
      do tipo do objeto nos esquema SQL */
   if(tipo_def==SchemaParser::SQL_DEFINITION)
   {
    atrib=BaseObject::objs_schemas[tipos_obj_aux[i]];
    atribs_aux[atrib]="";
   }

   //Obtém o início e o fim da lista para uma varredura
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end)
   {
    //Obtém o objeto atual
    objeto=(*itr);

    if(objeto->getObjectType()!=OBJ_SCHEMA ||
       (objeto->getObjectType()==OBJ_SCHEMA &&
        objeto->getName()!="public"))
    {
     //Gera o codigo e o concatena com os demais já gerados
     atribs_aux[atrib]+=validateObjectDefinition(objeto, tipo_def);
     //Dispara um sinal para sinalizar o progresso da geração do códgio
     qtd_defs_geradas++;
     if(!signalsBlocked())
     {
      emit s_objectLoaded((qtd_defs_geradas/qtd_geral_obj) * 100,
                             msg.arg(tipo_def_str)
                               .arg(QString::fromUtf8(objeto->getName()))
                               .arg(objeto->getTypeName()),
                            objeto->getObjectType());
     }
    }
    itr++;
   }
  }

  /* Armazena os próprio objeto de modelo de objetos pois o mesmo também precisa estar na
     ordenação de objetos para ser criado na ordem correta quando o parser xml ler
     a definição */
  mapa_objetos[this->getObjectId()]=this;
  vet_id_objs.push_back(this->getObjectId());


  /* Armazenando os demais tipos de objetos no mapa de objetos para
     serem ordenados pelos ids. */
  /* Para definição XML todos os 16 tipos da lista 'tipos_objs' são tratados.
     Para definição SQL são tratados apenas os 12 primeiros (tabelas, relacionamentos, visões e sequências)
     são tratadas separadamente pois existe uma ordem específica em que elas precisam ser criadas e essa
     ordem é definida na interação após a ordenação dos objetos */
  if(tipo_def==SchemaParser::XML_DEFINITION)
   qtd=16;
  else
   qtd=12;

  for(i=0; i < qtd; i++)
  {
   /* Seleciona a lista de objetos para obter os identificadores e armazenar seus
      elementos no mapa de objetos. No caso de definição SQL apenas os  objetos
      caixa de texto e relacionamento tabela-visão não são obtidos pois os mesmos
      não possuem código SQL */
   if(tipo_def==SchemaParser::SQL_DEFINITION &&
      (tipos_obj[i]==OBJ_TEXTBOX || tipos_obj[i]==BASE_RELATIONSHIP))
    lista_obj=NULL;
   else
    //Obtém a lista de objeto de acorodo com o tipo
    lista_obj=getObjectList(tipos_obj[i]);

   if(lista_obj)
   {
    //Obtém o início e o fim da lista para uma varredura
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end)
    {
     //Obtém o objeto atual
     objeto=(*itr);
     //Armazena o objeto em si no mapa de objetos
     mapa_objetos[objeto->getObjectId()]=objeto;
     //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
     vet_id_objs.push_back(objeto->getObjectId());
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
    tabela=dynamic_cast<Table *>(*itr);
    itr++;

    //Varre a lista de restrições da tabela
    qtd=tabela->getConstraintCount();
    for(i=0; i < qtd; i++)
    {
     restricao=tabela->getConstraint(i);

     /* Caso a restrição seja um objeto especial armazena o mesmo no mapa de objetos.
        Idenpendente da configuração, chaves estrangeiras sempre serão descartadas nesta
        iteração pois ao final do método as mesmas tem seu código SQL concatenado   definição
        do modelo */
     if((tipo_def==SchemaParser::XML_DEFINITION ||
         (tipo_def==SchemaParser::SQL_DEFINITION &&
          restricao->getConstraintType()!=ConstraintType::foreign_key)) &&

        (!restricao->isAddedByLinking() &&
          ((restricao->getConstraintType()!=ConstraintType::primary_key && restricao->isReferRelationshipAddedColumn()) ||
           (restricao->getConstraintType()==ConstraintType::foreign_key))))
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[restricao->getObjectId()]=restricao;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(restricao->getObjectId());
     }
    }

    //Varre a lista de gatilhos da tabela
    qtd=tabela->getTriggerCount();
    for(i=0; i < qtd; i++)
    {
     gatilho=tabela->getTrigger(i);

     //Caso o gatilho seja um objeto especial armazena-o no mapa de objetos
     if(gatilho->isReferRelationshipAddedColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[gatilho->getObjectId()]=gatilho;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(gatilho->getObjectId());
     }
    }

    //Varre a lista de índices da tabela
    qtd=tabela->getIndexCount();
    for(i=0; i < qtd; i++)
    {
     indice=tabela->getIndex(i);

     //Caso o índice seja um objeto especial armazena-o no mapa de objetos
     if(indice->isReferRelationshipAddedColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[indice->getObjectId()]=indice;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(indice->getObjectId());
     }
    }
   }

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     antes da ordenação caso a definição seja XML */
  if(tipo_def==SchemaParser::XML_DEFINITION)
   vet_id_objs.insert(vet_id_objs.end(), vet_id_objs_tab.begin(), vet_id_objs_tab.end());

  //Ordena o vetor de identificadores em ordem crescente
  sort(vet_id_objs.begin(), vet_id_objs.end());

  /* CASO ESPECIAL: Gerando a SQL de tabelas, visões, relacionamentos e sequencias de forma ordenada.

     Esta geração é feita da seguinte forma:
     1) Com a lista de relacionamentos, gera o SQL de ambas as tabelas do relacionametno e logo em seguida a
        SQL do relacionametno em si.
     2) Para as demais tabelas que não participaram de relacionamentos, gerar o SQL de cada uma.
     3) As sequencias devem ter seus SQL gerados após as tabelas restantes
     4) Visões devem ser as últimas a terem o SQL gerado para não terem referências a colunas e tabelas quebradas
  */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   BaseObject *objetos[3]={NULL, NULL, NULL};
   vector<BaseObject *> vet_aux;
   qtd=16;

   vet_aux=relationships;
   vet_aux.insert(vet_aux.end(), tables.begin(),tables.end());
   vet_aux.insert(vet_aux.end(), sequences.begin(),sequences.end());
   vet_aux.insert(vet_aux.end(), views.begin(),views.end());;
   itr=vet_aux.begin();
   itr_end=vet_aux.end();

   while(itr!=itr_end)
   {
    objeto=(*itr);
    itr++;

    //Stores the table's user added foreign keys
    if(objeto->getObjectType()==OBJ_TABLE)
     dynamic_cast<Table *>(objeto)->getForeignKeys(vet_fks);

    if(objeto->getObjectType()==OBJ_RELATIONSHIP)
    {
     relacao=dynamic_cast<Relationship *>(objeto);
     objetos[0]=relacao->getTable(Relationship::SRC_TABLE);
     objetos[1]=relacao->getTable(Relationship::DST_TABLE);
     objetos[2]=relacao;

     for(i=0; i < 3; i++)
     {
      if(mapa_objetos.count(objetos[i]->getObjectId())==0)
      {
       mapa_objetos[objetos[i]->getObjectId()]=objetos[i];
       vet_id_objs.push_back(objetos[i]->getObjectId());
      }
     }
    }
    else
    {
     if(mapa_objetos.count(objeto->getObjectId())==0)
     {
      mapa_objetos[objeto->getObjectId()]=objeto;
      vet_id_objs.push_back(objeto->getObjectId());
     }
    }
   }
  }

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     após a ordenação caso a definição seja SQL */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   vet_id_objs.insert(vet_id_objs.end(), vet_id_objs_tab.begin(), vet_id_objs_tab.end());

  atribs_aux[ParsersAttributes::SHELL_TYPES]="";

  /* Caso a definição seja SQL e existam tipos definidos pelo usuário
     faz a conversão dos parâmetros das funções usadas internamente
     por estes */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   itr=types.begin();
   itr_end=types.end();

   while(itr!=itr_end)
   {
    tipo_usr=dynamic_cast<Type *>(*itr);
    itr++;

    if(tipo_usr->getConfiguration()==Type::BASE_TYPE)
     tipo_usr->convertFunctionParameters();
   }
  }

  //Varre a lista de ids de objetos para obtê-los do mapa de objetos
  itr1=vet_id_objs.begin();
  itr1_end=vet_id_objs.end();

  atrib=ParsersAttributes::OBJECTS;
  while(itr1!=itr1_end)
  {
   /* Obtém o objeto do mapa a partir do seu identificador
      armazenado no iterador atual */
   objeto=mapa_objetos[(*itr1)];

   //Obtém o tipo do objeto
   tipo_obj=objeto->getObjectType();
   itr1++;

   /* Caso seja um objeto tipo e a definição seja SQL armazena a
      definição shell dos tipos os quais são declarados antes da criação
      dos tipos definidos pelo usuário */
   if(tipo_obj==OBJ_TYPE && tipo_def==SchemaParser::SQL_DEFINITION)
   {
    tipo_usr=dynamic_cast<Type *>(objeto);

    //Obtendo a definição do tipo em forma de shell type
    if(tipo_usr->getConfiguration()==Type::BASE_TYPE)
     atribs_aux[ParsersAttributes::SHELL_TYPES]+=tipo_usr->getCodeDefinition(tipo_def, true);
    else
     atribs_aux[atrib]+=tipo_usr->getCodeDefinition(tipo_def);
   }
   else if(tipo_obj==OBJ_DATABASE)
   {
    if(tipo_def==SchemaParser::SQL_DEFINITION)
     atribs_aux[this->getSchemaName()]+=this->__getCodeDefinition(tipo_def);
    else
     atribs_aux[atrib]+=this->__getCodeDefinition(tipo_def);
   }
   else if(tipo_obj==OBJ_CONSTRAINT)
   {
    atribs_aux[atrib]+=dynamic_cast<Constraint *>(objeto)->getCodeDefinition(tipo_def, true);
   }
   else
   {
    /* Desprezando as linguagens c e sql
       pois as mesmas não precisam ser declaradas explicitamente poir serem built-in */
    if(//tipo_def==ParserEsquema::DEFINICAO_SQL &&
       (tipo_obj==OBJ_LANGUAGE &&
        (objeto->getName()==~LanguageType("c") ||
         objeto->getName()==~LanguageType("sql") ||
         objeto->getName()==~LanguageType("plpgsql"))))
     atribs_aux[atrib]+="";
    else
     atribs_aux[atrib]+=validateObjectDefinition(objeto, tipo_def);
   }

   //Dispara um sinal para sinalizar o progresso da geração do códgio
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objectLoaded((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8(objeto->getName()))
                              .arg(objeto->getTypeName()),
                           objeto->getObjectType());
   }
  }

  //Creates the SQL definition for user added foreign keys
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   while(!vet_fks.empty())
   {
    atribs_aux[atrib]+=vet_fks.back()->getCodeDefinition(tipo_def, true);
    vet_fks.pop_back();
   }
  }

  //Gerando a definição sql/xml das permissões
  itr=permissions.begin();
  itr_end=permissions.end();
  atribs_aux[ParsersAttributes::PERMISSION]="";

  while(itr!=itr_end)
  {
   atribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permission *>(*itr)->getCodeDefinition(tipo_def);

   //Dispara um sinal para sinalizar o progresso final da geração de código
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objectLoaded((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8((*itr)->getName()))
                              .arg(objeto->getTypeName()),
                           objeto->getObjectType());
   }

   itr++;
  }

  //Configura os atributos específicos do modelo de banco
  atribs_aux[ParsersAttributes::MODEL_AUTHOR]=author;

  if(tipo_def==SchemaParser::XML_DEFINITION)
  {
   atribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? "1" : "");
  }
  else
  {
   /* Ao final da obtenção da definição SQL do modelo, faz a conversão inversa
      dos parâmetros das funções usadas pelos tipos base */
   qtd1=types.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Type *>(types[i]);
    if(tipo_usr->getConfiguration()==Type::BASE_TYPE)
    {
     atribs_aux[atrib]+=tipo_usr->getCodeDefinition(tipo_def);
     tipo_usr->convertFunctionParameters(true);
    }
   }
  }
 }
 catch(Exception &e)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   /* Em caso de erro na geração da definição SQL do modelo, faz a conversão inversa
     dos parâmetros das funções usadas pelos tipos base */
   qtd1=types.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Type *>(types[i]);
    if(tipo_usr->getConfiguration()==Type::BASE_TYPE)
     tipo_usr->convertFunctionParameters(true);
   }
  }
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }

 //Armazena o atributo que indica se a exportação é para arquivo ou não
 atribs_aux[ParsersAttributes::EXPORT_TO_FILE]=(exportar_arq ? "1" : "");

 //Retorna a definição do modelo completa
 return(SchemaParser::getCodeDefinition(ParsersAttributes::DB_MODEL, atribs_aux, tipo_def));
}

void DatabaseModel::saveModel(const QString &nome_arq, unsigned tipo_def)
{
 QString str_aux;
 QFile saida(nome_arq);

 saida.open(QFile::WriteOnly);

 //Caso não consiga abrir o arquivo para gravação
 if(!saida.isOpen())
 {
  str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(nome_arq));
  throw Exception(str_aux,ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 try
 {
  str_aux=this->getCodeDefinition(tipo_def);
  saida.write(str_aux.toStdString().c_str(),str_aux.size());
  saida.close();
 }
 catch(Exception &e)
 {
  if(saida.isOpen()) saida.close();
  str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(nome_arq));
  throw Exception(str_aux,ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void DatabaseModel::getObjectDependecies(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_dep_indiretas)
{
 //Caso o objeto esteja alocado e o mesmo ainda não exista na lista de dependências
 if(objeto && std::find(vet_deps.begin(), vet_deps.end(), objeto)==vet_deps.end())
 {
  //Adiciona o objeto na lista de dependências
  vet_deps.push_back(objeto);


 if((vet_deps.size()==1 && !inc_dep_indiretas) || inc_dep_indiretas)
 {
  ObjectType tipo_obj=objeto->getObjectType();

  /* Caso o objeto possua esquema, espaço de tabela e dono,
     busca e inclui se necessário as dependências desses
     objetos na lista */
  if(objeto->getSchema() && inc_dep_indiretas)
   getObjectDependecies(objeto->getSchema(), vet_deps, inc_dep_indiretas);

  if(objeto->getTablespace() && inc_dep_indiretas)
   getObjectDependecies(objeto->getTablespace(), vet_deps, inc_dep_indiretas);

  if(objeto->getOwner()  && inc_dep_indiretas)
   getObjectDependecies(objeto->getOwner(), vet_deps, inc_dep_indiretas);

  //** Obtendo as dependências de Classe de Operadores **
  if(tipo_obj==OBJ_OPCLASS)
  {
   OperatorClass *classe_op=dynamic_cast<OperatorClass *>(objeto);

   /* Obtém a referência para o tipo de dado da classe, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
  BaseObject *tipo_usr=getObjectPgSQLType(classe_op->getDataType());
  //obterObjeto(*classe_op->obterTipoDado(), OBJETO_TIPO);

   if(tipo_usr)
    getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);

   //Caso haja uma família de operadores obtém as dependências dela também
   if(classe_op->getFamily())
    getObjectDependecies(classe_op->getFamily(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Domínios **
  else if(tipo_obj==OBJ_DOMAIN)
  {
   /* Obtém a referência para o tipo de dado do domínio, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
   BaseObject *tipo_usr=getObjectPgSQLType(dynamic_cast<Domain *>(objeto)->getType());
   //obterObjeto(*dynamic_cast<Dominio *>(objeto)->obterTipo(), OBJETO_TIPO);

   if(tipo_usr)
    getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Codificação **
  else if(tipo_obj==OBJ_CONVERSION)
  {
   //Obtém as dependências da função de conversão que define a conversão de codificação
   Function *func=dynamic_cast<Conversion *>(objeto)->getConversionFunction();
   getObjectDependecies(func, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Tipo **
  else if(tipo_obj==OBJ_CAST)
  {
   Cast *conv=dynamic_cast<Cast *>(objeto);
   BaseObject *tipo_usr=NULL;

   //Obtém as dependências dos tipos usados na conversão de tipo
   for(unsigned i=Cast::SRC_TYPE; i <= Cast::DST_TYPE; i++)
   {
    tipo_usr=getObjectPgSQLType(conv->getDataType(i));
      //obterObjeto(*conv->obterTipoDado(i), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências da função de conversão que define a conversão de tipo
   getObjectDependecies(conv->getCastFunction(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Funções **
  else if(tipo_obj==OBJ_FUNCTION)
  {
   Function *func=dynamic_cast<Function *>(objeto);
   BaseObject *tipo_usr=getObjectPgSQLType(func->getReturnType());
     //obterObjeto(*func->obterTipoRetorno(), OBJETO_TIPO);
   unsigned qtd, i;

   //Caso a linguagem da função não seja C ou SQL obtém as dependências da mesma
   if(func->getLanguage()->getName()!=~LanguageType("c") &&
      func->getLanguage()->getName()!=~LanguageType("sql"))
    getObjectDependecies(func->getLanguage(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências do tipo de retorno caso o mesmo seja um tipo definido pelo usuário
   if(tipo_usr)
    getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos dos parâmetros
   qtd=func->getParameterCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=getObjectPgSQLType(func->getParameter(i).getType());
    //obterObjeto(*func->obterParametro(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos elementos da tabela de retorno
   qtd=func->getReturnedTableColumnCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=getObjectPgSQLType(func->getReturnedTableColumn(i).getType());
    //obterObjeto(*func->obterTipoRetTabela(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Funções de Agregação **
  else if(tipo_obj==OBJ_AGGREGATE)
  {
   Aggregate *func=dynamic_cast<Aggregate *>(objeto);
   BaseObject *tipo_usr=NULL;
   unsigned qtd, i;

   //Obtém as dependências das funções que definem a função de agregação
   for(i=Aggregate::FINAL_FUNC; i <= Aggregate::TRANSITION_FUNC; i++)
    getObjectDependecies(func->getFunction(i), vet_deps, inc_dep_indiretas);

   //Obtém a dependência do tipo de estado da função de agregação
   tipo_usr=getObjectPgSQLType(func->getStateType());
     //obterObjeto(*func->obterTipoEstado(), OBJETO_TIPO);

   if(tipo_usr)
    getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências do operador de ordenação caso este esteja alocado
   if(func->getSortOperator())
    getObjectDependecies(func->getSortOperator(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos de dados usados na função de agregação
   qtd=func->getDataTypeCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=getObjectPgSQLType(func->getDataType(i));
      //obterObjeto(*func->obterTipoDado(i), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Linguagens **
  else if(tipo_obj==OBJ_LANGUAGE)
  {
   Language *ling=dynamic_cast<Language *>(objeto);

   for(unsigned i=Language::VALIDATOR_FUNC; i <= Language::INLINE_FUNC; i++)
   {
    if(ling->getFunction(i))
     getObjectDependecies(ling->getFunction(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Operadores **
  else if(tipo_obj==OBJ_OPERATOR)
  {
   Operator *oper=dynamic_cast<Operator *>(objeto);
   BaseObject *tipo_usr=NULL;
   unsigned i;

   //Obtém as dependências das funções usadas pelo operador
   for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
   {
    if(oper->getFunction(i))
     getObjectDependecies(oper->getFunction(i), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos tipos dos argumentos do operador
   for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
   {
    tipo_usr=getObjectPgSQLType(oper->getArgumentType(i));
      //obterObjeto(*oper->obterTipoDadoArgumento(i), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos operadores auxiliares
   for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
   {
    if(oper->getOperator(i))
     getObjectDependecies(oper->getOperator(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Papéis **
  else if(tipo_obj==OBJ_ROLE)
  {
   Role *papel=dynamic_cast<Role *>(objeto);
   unsigned i, i1, qtd,
            tipos[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

   //Obtém as dependências dos papéis membros, papéis admins e papéis referenciados
   for(i=0; i < 3; i++)
   {
    qtd=papel->getRoleCount(tipos[i]);
    for(i1=0; i1 < qtd; i1++)
     getObjectDependecies(papel->getRole(tipos[i], i1), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Relacionamentos **
  else if(tipo_obj==OBJ_RELATIONSHIP)
  {
   Relationship *rel=dynamic_cast<Relationship *>(objeto);
   BaseObject *tipo_usr=NULL;
   Constraint *rest=NULL;
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pelo relacionamento
   getObjectDependecies(rel->getTable(Relationship::SRC_TABLE), vet_deps, inc_dep_indiretas);
   getObjectDependecies(rel->getTable(Relationship::DST_TABLE), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos usados pelos atributos do relacionamento
   qtd=rel->getAttributeCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=getObjectPgSQLType(rel->getAttribute(i)->getType());
      //obterObjeto(*rel->obterAtributo(i)->obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições (somente chave estrangeira) do relacionamento
   qtd=rel->getConstraintCount();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Constraint *>(rel->getConstraint(i));
    if(rest->getConstraintType()==ConstraintType::foreign_key)
     getObjectDependecies(rest->getReferencedTable(), vet_deps, inc_dep_indiretas);

    if(rest->getTablespace())
     getObjectDependecies(rest->getTablespace(), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Sequências **
  else if(tipo_obj==OBJ_SEQUENCE)
  {
   Sequence *seq=dynamic_cast<Sequence *>(objeto);
   if(seq->getOwnerColumn())
    getObjectDependecies(seq->getOwnerColumn()->getParentTable(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Tabelas **
  else if(tipo_obj==OBJ_TABLE)
  {
   Table *tab=dynamic_cast<Table *>(objeto);
   BaseObject *tipo_usr=NULL;
   Constraint *rest=NULL;
   Trigger *gat=NULL;
   Index *ind=NULL;
   Column *col=NULL;
   unsigned qtd, qtd1, i, i1;

   //Obtém as dependências dos tipos das colunas não incluídas por relacionamento
   qtd=tab->getColumnCount();
   for(i=0; i < qtd; i++)
   {
    col=tab->getColumn(i);
    tipo_usr=getObjectPgSQLType(col->getType());
      //obterObjeto(*col->obterTipo(), OBJETO_TIPO);

    if(!col->isAddedByLinking() && tipo_usr)
     getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições não incluídas por relacionamento
   qtd=tab->getConstraintCount();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Constraint *>(tab->getConstraint(i));
    if(inc_dep_indiretas &&
       !rest->isAddedByLinking() &&
        rest->getConstraintType()==ConstraintType::foreign_key)
     getObjectDependecies(rest->getReferencedTable(), vet_deps, inc_dep_indiretas);

    if(!rest->isAddedByLinking() && rest->getTablespace())
     getObjectDependecies(rest->getTablespace(), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das tabelas referenciadas nos gatilhos e as funções
   qtd=tab->getTriggerCount();
   for(i=0; i < qtd; i++)
   {
    gat=dynamic_cast<Trigger *>(tab->getTrigger(i));
    if(gat->getReferencedTable())
     getObjectDependecies(gat->getReferencedTable(), vet_deps, inc_dep_indiretas);

    if(gat->getFunction())
     getObjectDependecies(gat->getFunction(), vet_deps, inc_dep_indiretas);

    //qtd1=gat->obterNumColunas();
    //for(i1=0; i1 < qtd1; i1++)
    //obterDependenciasObjeto(gat->obterColuna(i1), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das colunas ou classe de operadores usados nos elementos do índices
   qtd=tab->getIndexCount();
   for(i=0; i < qtd; i++)
   {
    ind=dynamic_cast<Index *>(tab->getIndex(i));
    qtd1=ind->getElementCount();

    for(i1=0; i1 < qtd1; i1++)
    {
     if(ind->getElement(i1).getOperatorClass())
      getObjectDependecies(ind->getElement(i1).getOperatorClass(), vet_deps, inc_dep_indiretas);
     else if(ind->getElement(i1).getColumn())
     {
      tipo_usr=getObjectPgSQLType(ind->getElement(i1).getColumn()->getType());
        //obterObjeto(*ind->obterElemento(i1).obterColuna()->obterTipo(), OBJETO_TIPO);

      if(tipo_usr)
       getObjectDependecies(tipo_usr, vet_deps, inc_dep_indiretas);
     }
    }
   }
  }
  //** Obtendo as dependências de Tipos definidos pelo usuário **
  else if(tipo_obj==OBJ_TYPE)
  {
   Type *tipo_usr=dynamic_cast<Type *>(objeto);
   BaseObject *tipo_aux=NULL;
   unsigned qtd, i;

   //Caso o tipo de usuário seja tipo base
   if(tipo_usr->getConfiguration()==Type::BASE_TYPE)
   {
    //Obtém as dependências do tipo de cópia
    tipo_aux=getObjectPgSQLType(tipo_usr->getLikeType());
      //obterObjeto(*tipo_usr->obterTipoCopia(), OBJETO_TIPO);

    if(tipo_aux)
     getObjectDependecies(tipo_aux, vet_deps, inc_dep_indiretas);

    //Obtém as dependências das funções usadas pelo tipo
    for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
     getObjectDependecies(tipo_usr->getFunction(i), vet_deps, inc_dep_indiretas);
   }
   //Caso seja um tipo composto
   else if(tipo_usr->getConfiguration()==Type::COMPOSITE_TYPE)
   {
    //Obtém as dependências dos tipos dos atributos
    qtd=tipo_usr->getAttributeCount();
    for(i=0; i < qtd; i++)
    {
     tipo_aux=getObjectPgSQLType(tipo_usr->getAttribute(i).getType());
       //obterObjeto(*tipo_usr->obterAtributo(i).obterTipo(), OBJETO_TIPO);

     if(tipo_aux)
      getObjectDependecies(tipo_aux, vet_deps, inc_dep_indiretas);
    }
   }
  }
  //** Obtendo as dependências de Visões **
  else if(tipo_obj==OBJ_VIEW)
  {
   View *visao=dynamic_cast<View *>(objeto);
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pela visão
   qtd=visao->getReferenceCount();
   for(i=0; i < qtd; i++)
   {
    if(visao->getReference(i).getTable())
     getObjectDependecies(visao->getReference(i).getTable(), vet_deps, inc_dep_indiretas);
   }
  }
 }
 }
}

void DatabaseModel::getObjectReferences(BaseObject *objeto, vector<BaseObject *> &vet_refs, bool modo_exclusao)
{
 vet_refs.clear();

 if(objeto)
 {
  ObjectType tipo_obj=objeto->getObjectType();
  bool refer=false;

  if(tipo_obj==OBJ_TABLE)
  {
   Table *tabela=dynamic_cast<Table *>(objeto);
   Sequence *seq=NULL;
   Constraint *rest=NULL;
   Table *tab=NULL;
   Trigger *gat=NULL;
   BaseRelationship *rel_base=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, qtd;

   /* Vericando se existe algum relacionamento (tabela-tabela)
      o qual um dos objetos participantes é a tabela
      do parâmetro */
   itr=relationships.begin();
   itr_end=relationships.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<BaseRelationship *>(*itr);
    if(rel_base->getTable(BaseRelationship::SRC_TABLE)==tabela ||
       rel_base->getTable(BaseRelationship::DST_TABLE)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base);
    }
    itr++;
   }

   /* Vericando se existe alguma sequência a qual referencia
      a tabela do parâmetro */
   itr=sequences.begin();
   itr_end=sequences.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    seq=dynamic_cast<Sequence *>(*itr);
    if(seq->getOwnerColumn() &&
       seq->getOwnerColumn()->getParentTable()==tabela)
    {
     refer=true;
     vet_refs.push_back(seq);
    }

    itr++;
   }

   /* Vericando se existe alguma tabela cuja uma tabela cópia é
      a própria tabela a ser excluída, verifica também se as
      restrições das tabelas, especificamente, as chaves estrangeiras
      estão referenciado a tabela */
   itr=tables.begin();
   itr_end=tables.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    tab=dynamic_cast<Table *>(*itr);

    //Verificando as restrições
    qtd=tab->getConstraintCount();
    for(i=0; i < qtd&& (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->getConstraint(i);
     if(rest->getConstraintType()==ConstraintType::foreign_key &&
        rest->getReferencedTable()==tabela)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    //Verificando os gatilhos
    qtd=tab->getTriggerCount();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     gat=tab->getTrigger(i);
     if(gat->getReferencedTable()==tabela)
     {
      refer=true;
      vet_refs.push_back(gat);
     }
    }

    itr++;
   }

   /* Vericando se existe algum relacionamento (tabela-visao)
      o qual um dos objetos participantes é a tabela */
   itr=base_relationships.begin();
   itr_end=base_relationships.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<BaseRelationship *>(*itr);
    if(rel_base->getTable(BaseRelationship::SRC_TABLE)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->getTable(BaseRelationship::DST_TABLE));
    }
    else if(rel_base->getTable(BaseRelationship::DST_TABLE)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->getTable(BaseRelationship::SRC_TABLE));
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_FUNCTION)
  {
   Function *funcao=dynamic_cast<Function *>(objeto);
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[7]={OBJ_CAST, OBJ_CONVERSION,
                            OBJ_AGGREGATE, OBJ_OPERATOR,
                            OBJ_TABLE, OBJ_TYPE, OBJ_LANGUAGE };
   unsigned i, i1, qtd;
   Table *tab=NULL;
   Aggregate *func_ag=NULL;
   Operator *oper=NULL;
   Trigger *gat=NULL;
   Type *tipo=NULL;
   Language *ling=NULL;

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente uma função */
   for(i=0; i < 7 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=getObjectList(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_CAST)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<Cast *>(*itr)->getCastFunction()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_CONVERSION)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<Conversion *>(*itr)->getConversionFunction()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      func_ag=dynamic_cast<Aggregate *>(*itr);
      //Verifica se o objeto não referencia o papel
      if(func_ag->getFunction(Aggregate::FINAL_FUNC)==funcao ||
         func_ag->getFunction(Aggregate::TRANSITION_FUNC)==funcao)
      {
       refer=true;
       vet_refs.push_back(func_ag);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);

      //Verifica se o objeto não referencia o papel
      if(oper->getFunction(Operator::FUNC_OPERATOR)==funcao ||
         oper->getFunction(Operator::FUNC_JOIN)==funcao  ||
         oper->getFunction(Operator::FUNC_RESTRICTION)==funcao)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_TABLE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tab=dynamic_cast<Table *>(*itr);
      itr++;
      qtd=tab->getTriggerCount();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       gat=tab->getTrigger(i1);
       //Verifica se o gatilho não referencia a função
       if(gat->getFunction()==funcao)
       {
        refer=true;
        vet_refs.push_back(gat);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_TYPE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Type *>(*itr);
      itr++;

      for(i1=Type::INPUT_FUNC; i1 <= Type::ANALYZE_FUNC && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o tipo não referencia a função
       if(tipo->getFunction(i1)==funcao)
       {
        refer=true;
        vet_refs.push_back(tipo);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_LANGUAGE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      ling=dynamic_cast<Language *>(*itr);
      itr++;
      //Verifica se a função a ser removida é uma função handler ou validator da linguagem
      if(ling->getFunction(Language::HANDLER_FUNC)==funcao ||
         ling->getFunction(Language::VALIDATOR_FUNC)==funcao ||
         ling->getFunction(Language::INLINE_FUNC)==funcao)
      {
       refer=true;
       vet_refs.push_back(ling);
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_SCHEMA)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[11]={OBJ_FUNCTION, OBJ_TABLE, OBJ_VIEW,
                             OBJ_DOMAIN, OBJ_AGGREGATE, OBJ_OPERATOR,
                             OBJ_SEQUENCE, OBJ_CONVERSION,
                             OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
   unsigned i;

   /* Varre todas as listas de objetos os quais pode pertencer a
      um esquema e verifica se o mesmos não estão referenciando
      o esquema a ser removido */
   for(i=0; i < 11 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=getObjectList(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o esquema
     if((*itr)->getSchema()==objeto)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }
  }

  if(tipo_obj==OBJ_TYPE || tipo_obj==OBJ_DOMAIN ||
     tipo_obj==OBJ_SEQUENCE || tipo_obj==OBJ_TABLE)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[]={OBJ_TABLE, OBJ_OPCLASS, OBJ_CAST,
                           OBJ_DOMAIN, OBJ_FUNCTION, OBJ_AGGREGATE,
                           OBJ_OPERATOR, OBJ_TYPE };
   unsigned i, i1, qtd;
   OperatorClass *classe_op=NULL;
   Table *tab=NULL;
   Column *col=NULL;
   Cast *conv_tipo=NULL;
   Domain *dom=NULL;
   Function *func=NULL;
   Aggregate *func_ag=NULL;
   Operator *oper=NULL;
   Type *tipo=NULL;
   void *ptr_tipopgsql=NULL;

   /* Devido a particuladade de se atribuir ponteiros de objetos como tipos base do pgsql (ver TipoPgSQL)
      é necessário fazer um dynamic_cast para a classe correta do 'objeto'. Caso o dynamic_cast não seja
      feita, mesmo que o objeto seja um tipopgsql válido o mesmo não será localizado na lista de tipos
      base do pgsql.  */
   switch(tipo_obj)
   {
    case OBJ_TYPE: ptr_tipopgsql=dynamic_cast<Type*>(objeto); break;
    case OBJ_DOMAIN: ptr_tipopgsql=dynamic_cast<Domain*>(objeto); break;
    case OBJ_SEQUENCE: ptr_tipopgsql=dynamic_cast<Sequence*>(objeto); break;
    default: ptr_tipopgsql=dynamic_cast<Table*>(objeto); break;
   }

   /* Varre todas as listas de objetos os quais podem
     referenciar direta ou indiretamente um tipo definido
     pelo usuário */
   for(i=0; i < 8 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=getObjectList(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_TABLE)
    {
     //Verifica se as colunas das tabelas não referenciam o tipo
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a tabela
      tab=dynamic_cast<Table *>(*itr);
      itr++;

      //Varre a lista de colunas da tabela
      qtd=tab->getColumnCount();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       col=tab->getColumn(i1);
       //Verifica se o tipo da coluna é o próprio tipo a ser excluído
       if(col->getType()==objeto)
       {
        refer=true;
        vet_refs.push_back(col);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_OPCLASS)
    {
     /* Varre a lista de classe operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Verifica se o tipo de dado da classe é o próprio tipo a ser removido
      if(classe_op->getDataType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(classe_op);
      }
     }
    }
    else if(tipos[i]==OBJ_DOMAIN)
    {
     /* Varre a lista de domínios e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      dom=dynamic_cast<Domain *>(*itr);
      itr++;

      //Verifica se o tipo de dado do dominio é o próprio tipo a ser removido
      if(dom->getType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(dom);
      }
     }
    }
    else if(tipos[i]==OBJ_TYPE)
    {
     /* Varre a lista de tipo definidos pelo usuário e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Type *>(*itr);
      itr++;

      if(tipo->getAlignment()==ptr_tipopgsql ||
         tipo->getElement()==ptr_tipopgsql ||
         tipo->getLikeType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(tipo);
      }
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     /* Varre a lista de funções agregadas e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func_ag=dynamic_cast<Aggregate *>(*itr);
      itr++;

      /* Verifica se os tipos de dados da função agregada referenciam
         tipo a ser removido */
      qtd=func_ag->getDataTypeCount();
      for(i1=0; i1 < qtd  && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(func_ag->getDataType(i1)==ptr_tipopgsql)
       {
        refer=true;
        vet_refs.push_back(func_ag);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_FUNCTION)
    {
     /* Varre a lista de funções e verifica se algum
        objeto (parâmetros e tipo de retorno) referencia
        o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func=dynamic_cast<Function *>(*itr);
      itr++;

      //Verifica se o tipo de retorno é o próprio tipo a ser removido
      if(func->getReturnType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(func);
      }
      else
      {
       /* Verifica se os tipos de dados dos parâmetros da função
          referenciam tipo a ser removido */
       qtd=func->getParameterCount();
       for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(func->getParameter(i1).getType()==ptr_tipopgsql)
        {
         refer=true;
         vet_refs.push_back(func);
        }
       }
      }
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     /* Varre a lista de operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);
      itr++;

      //Verifica se um dos argumentos do operador é o próprio tipo a ser removido
      if(oper->getArgumentType(Operator::LEFT_ARG)==ptr_tipopgsql ||
         oper->getArgumentType(Operator::RIGHT_ARG)==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
     }
    }
    else if(tipos[i]==OBJ_CAST)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      conv_tipo=dynamic_cast<Cast *>(*itr);
      itr++;

      //Verifica se o objeto não referencia o tipo
      if(conv_tipo->getDataType(Cast::SRC_TYPE)==ptr_tipopgsql ||
         conv_tipo->getDataType(Cast::DST_TYPE)==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(conv_tipo);
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_ROLE)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[13]={OBJ_FUNCTION, OBJ_TABLE, OBJ_DOMAIN,
                             OBJ_AGGREGATE, OBJ_SCHEMA, OBJ_OPERATOR,
                             OBJ_SEQUENCE, OBJ_CONVERSION,
                             OBJ_LANGUAGE, OBJ_TABLESPACE,
                             OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
   unsigned i,i1, qtd;
   Role *papel_aux=NULL;
   Role *papel=dynamic_cast<Role *>(objeto);
   unsigned tipo_papel[3]={Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE};

   /* Caso especial: Varre a lista de papéis e verifica se o papel a ser
      removido se encontra em uma das três listas de papeis da classe
      Papel (ref, membro ou admin) */
   itr=roles.begin();
   itr_end=roles.end();
   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém um papel
    papel_aux=dynamic_cast<Role *>(*itr);
    itr++;

    for(i1=0; i1 < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
    {
     //Obtém a quantidade de papeis presentes na lista atual (tipo_papel[i1])
     qtd=papel_aux->getRoleCount(tipo_papel[i1]);
     for(i=0; i < qtd && !refer; i++)
     {
      /* Caso o papel a ser excluído seja igual ao elemento atual
         da lista de papéis do papel aux */
      if(papel_aux->getRole(tipo_papel[i1], i)==papel)
      {
       refer=true;
       vet_refs.push_back(papel_aux);
      }
     }
    }
   }

   /* Varre todas as listas de objetos os quais pode pertencer a
      um dono e verifica se o mesmos não estão referenciando
      o dono a ser removido */
   for(i=0; i < 13 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=getObjectList(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o papel
     if((*itr)->getOwner()==papel)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }

   /*Caso especial: Verifica se o papel é a ser removido é dono
     do próprio banco de dados */
   if((!modo_exclusao || (modo_exclusao && !refer)) && this->getOwner()==papel)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }

  if(tipo_obj==OBJ_TABLESPACE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, qtd;
   Table *tab=NULL;
   Index *ind=NULL;
   Constraint *rest=NULL;

   /* Varre a lista de tabelas e verifica se estas e seus índices
      não estão referenciando o espaço de tabelas a ser removido */
   itr=tables.begin();
   itr_end=tables.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém a tabela
    tab=dynamic_cast<Table *>(*itr);

    //Verifica se referencia o espaço de tabela
    if(tab->getTablespace()==objeto)
    {
     refer=true;
     vet_refs.push_back(tab);
    }


    /* Verifica se algum dos índices da tabela referecia
       o espaço de tabela */
    qtd=tab->getIndexCount();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     ind=tab->getIndex(i);
     if(ind->getTablespace()==objeto)
     {
      refer=true;
      vet_refs.push_back(ind);
     }
    }

    /* Verifica se alguma restrição da tabela referecia
       o espaço de tabela */
    qtd=tab->getConstraintCount();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->getConstraint(i);
     if(rest->getTablespace()==objeto)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    itr++;
   }

   if((!modo_exclusao || (modo_exclusao && !refer)) && this->BaseObject::getTablespace()==objeto)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }

  if(tipo_obj==OBJ_LANGUAGE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   Function *func=NULL;

   /* Varre a lista de funções e verifica se estas
      não estão referenciando a linguage a ser removida */
   itr=functions.begin();
   itr_end=functions.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Verifica se referencia o espaço de tabela
    func=dynamic_cast<Function *>(*itr);
    if(func->getLanguage()==objeto)
    {
     refer=true;
     vet_refs.push_back(func);
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_OPERATOR)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[3]={OBJ_OPCLASS,
                            OBJ_AGGREGATE,
                            OBJ_OPERATOR};
   unsigned i, i1, qtd;
   OperatorClass *classe_op=NULL;
   Operator *oper=NULL, *operador=dynamic_cast<Operator *>(objeto);

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente um operador */
   for(i=0; i < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=getObjectList(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_OPCLASS)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Varre a lista de elementos da classe de operadores
      qtd=classe_op->getElementCount();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o objeto não referencia o operador
       if(classe_op->getElement(i1).getOperator()==operador)
       {
        refer=true;
        vet_refs.push_back(classe_op);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      if(dynamic_cast<Aggregate *>(*itr)->getSortOperator()==operador)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     while(itr!=itr_end && !refer)
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);
      itr++;

      /* Verifica se um dos operadores agregados ao operador atual não referencia
         o objeto não referencia o operador */
      for(i1=Operator::OPER_COMMUTATOR; i1 <= Operator::OPER_GREATER &&
            (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(oper->getOperator(i1)==operador)
       {
        refer=true;
        vet_refs.push_back(oper);
       }
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_OPFAMILY)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   OperatorFamily *familia_op=dynamic_cast<OperatorFamily *>(objeto);

   /* Varre a lista de classe de operadores e verifica se um
      de seus elementos está referenciando a família de operadores */
   itr=op_classes.begin();
   itr_end=op_classes.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    if(dynamic_cast<OperatorClass *>(*itr)->getFamily()==familia_op)
    {
     refer=true;
     vet_refs.push_back(*itr);
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_COLUMN)
  {
   Column *coluna=dynamic_cast<Column *>(objeto);
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType  tipos_obj[]={ OBJ_SEQUENCE, OBJ_VIEW, OBJ_TABLE, OBJ_RELATIONSHIP };
   unsigned i, qtd=4;

   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém a lista do tipo atual
    lista_obj=getObjectList(tipos_obj[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    //Varre a lista selecionada verificado se a coluna passada é referenciada
    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     /* Caso 1: o tipo atual seja sequência, faz o cast para a classe
        e chama do método da sequencia que retorna a coluna possuidora
        da mesma.

        Caso 2: o tipo atual seja visão, faz o cast para a classe
        e chama do método da visão o qual retorna se a coluna
        é referenciada pelo elementos da visão. */
     if((tipos_obj[i]==OBJ_SEQUENCE && dynamic_cast<Sequence *>(*itr)->getOwnerColumn()==coluna) ||
        (tipos_obj[i]==OBJ_VIEW && dynamic_cast<View *>(*itr)->isReferencingColumn(coluna)))
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     else if(tipos_obj[i]==OBJ_TABLE)
     {
      Table *tab=dynamic_cast<Table *>(*itr);
      unsigned qtd_gat, qtd_rest, idx, qtd1, i1;
      Trigger *gat=NULL;

      qtd_rest=tab->getConstraintCount();
      for(idx=0; idx < qtd_rest && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       if(tab->getConstraint(idx)->isColumnExists(coluna, Constraint::SOURCE_COLS) ||
          tab->getConstraint(idx)->isColumnExists(coluna, Constraint::REFERENCED_COLS))
       {
        refer=true;
        vet_refs.push_back(tab->getConstraint(idx));
       }
      }

      qtd_gat=tab->getTriggerCount();
      for(idx=0; idx < qtd_gat && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       gat=tab->getTrigger(idx);
       qtd1=gat->getColumnCount();

       for(i1=0; i1 < qtd1 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(gat->getColumn(i1)==coluna)
        {
         refer=true;
         vet_refs.push_back(gat);
        }
       }
      }
     }
     else if(tipos_obj[i]==OBJ_RELATIONSHIP)
     {
      Relationship *rel=dynamic_cast<Relationship *>(*itr);
      unsigned qtd_rest, idx;

      qtd_rest=rel->getConstraintCount();
      for(idx=0; idx < qtd_rest && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       if(rel->getConstraint(idx)->isColumnExists(coluna, Constraint::SOURCE_COLS) ||
          rel->getConstraint(idx)->isColumnExists(coluna, Constraint::REFERENCED_COLS))
       {
        refer=true;
        vet_refs.push_back(rel);
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
 ObjectType tipos[]={OBJ_TABLE, OBJ_VIEW,
                         OBJ_RELATIONSHIP, BASE_RELATIONSHIP,
                         OBJ_TEXTBOX};
 vector<BaseObject *>::iterator itr, itr_end;
 vector<BaseObject *> *lista_obj=NULL;
 Textbox *rot=NULL;
 BaseRelationship *rel=NULL;
 unsigned i, i1;

 for(i=0; i < 5; i++)
 {
  lista_obj=getObjectList(tipos[i]);
  itr=lista_obj->begin();
  itr_end=lista_obj->end();

  while(itr!=itr_end)
  {
   dynamic_cast<BaseGraphicObject *>(*itr)->setModified(true);

   /* Caso especial: Caso o objeto seja um relacionamento, os rótulos
      do mesmo que são caixas de texto, devem também ser marcados
      como modificado */
   if(tipos[i]==OBJ_RELATIONSHIP || tipos[i]==BASE_RELATIONSHIP)
   {
    rel=dynamic_cast<BaseRelationship *>(*itr);
    for(i1=0; i1 < 3; i1++)
    {
     rot=rel->getLabel(i1);
     if(rot) rot->setModified(true);
    }
   }

   itr++;
  }
 }
}

BaseObject *DatabaseModel::getObjectPgSQLType(PgSQLType tipo)
{
 switch(tipo.getUserTypeConfig())
 {
  case UserTypeConfig::BASE_TYPE:
   return(this->getObject(*tipo, OBJ_TYPE));
  break;

  case UserTypeConfig::DOMAIN_TYPE:
   return(this->getObject(*tipo, OBJ_DOMAIN));
  break;

  case UserTypeConfig::TABLE_TYPE:
   return(this->getObject(*tipo, OBJ_TABLE));
  break;

  case UserTypeConfig::SEQUENCE_TYPE:
   return(this->getObject(*tipo, OBJ_SEQUENCE));
  break;

  default:
   return(NULL);
  break;
 }
}

