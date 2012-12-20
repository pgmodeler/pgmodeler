#include "table.h"

Table::Table(void) : BaseTable()
{
 obj_type=OBJ_TABLE;
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::CONSTRAINTS]="";
 attributes[ParsersAttributes::INDEXES]="";
 attributes[ParsersAttributes::TRIGGERS]="";
 attributes[ParsersAttributes::RULES]="";
 attributes[ParsersAttributes::OIDS]="";
 attributes[ParsersAttributes::COLS_COMMENT]="";
 this->setName(trUtf8("new_table").toUtf8());
}

Table::~Table(void)
{
 ObjectType types[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE,
                          OBJ_CONSTRAINT, OBJ_COLUMN };
 vector<TableObject *> *list=NULL;

 for(unsigned i=0; i < 5; i++)
 {
  list=getObjectList(types[i]);
  while(!list->empty())
  {
   delete(list->back());
   list->pop_back();
  }
 }

 copy_tables.clear();
 ancestor_tables.clear();
}

void Table::setName(const QString &name)
{
 QString prev_name=this->getName(true);

 BaseObject::setName(name);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setSchema(BaseObject *schema)
{
 QString prev_name=this->getName(true);

 //Atribui o esquema   tabela
 BaseObject::setSchema(schema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setWithOIDs(bool value)
{
 with_oid=value;
}

void Table::setProtected(bool value)
{
 ObjectType obj_types[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
                          OBJ_INDEX, OBJ_RULE, OBJ_TRIGGER };
 unsigned i;
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *list=NULL;
 TableObject *tab_obj=NULL;

 for(i=0; i < 5; i++)
 {
  //Obtém a lista de objetos
  list=getObjectList(obj_types[i]);
  itr=list->begin();
  itr_end=list->end();

  //Varre a lista protegendo/desprotegendo os elementos
  while(itr!=itr_end)
  {
   //Obtém um objeto da lista
   tab_obj=(*itr);
   /* Só protegerá o objeto caso o mesmo não foi incluído
      por relacionamento pois quando é o caso os mesmos
      já vem protegidos */
   if(!tab_obj->isAddedByRelationship())
    tab_obj->setProtected(value);
   itr++;
  }
 }

 BaseGraphicObject::setProtected(value);
}

void Table::setCommentAttribute(TableObject *tab_obj)
{
 if(tab_obj && !tab_obj->getComment().isEmpty())
 {
  map<QString, QString> attribs;

  attribs[ParsersAttributes::DIF_SQL]="1";
  attribs[ParsersAttributes::SQL_OBJECT]=tab_obj->getSQLName();
  attribs[ParsersAttributes::COLUMN]=(tab_obj->getObjectType()==OBJ_COLUMN ? "1" : "");
  attribs[ParsersAttributes::CONSTRAINT]=(tab_obj->getObjectType()==OBJ_CONSTRAINT ? "1" : "");
  attribs[ParsersAttributes::TABLE]=this->getName(true);
  attribs[ParsersAttributes::NAME]=tab_obj->getName(true);
  attribs[ParsersAttributes::COMMENT]=tab_obj->getComment();

  SchemaParser::setIgnoreUnkownAttributes(true);
  attributes[ParsersAttributes::COLS_COMMENT]+=SchemaParser::getObjectDefinition(ParsersAttributes::COMMENT, attribs,
                                                                                 SchemaParser::SQL_DEFINITION);
  SchemaParser::setIgnoreUnkownAttributes(false);
 }
}

void Table::setColumnsAttribute(unsigned def_type)
{
 QString str_cols;
 unsigned i, count;

 //Concatena a definiação SQL das colunas
 count=columns.size();
 for(i=0; i < count; i++)
 {
  /*
   Para a definição de uma colunar ser inclía   definição da tabela
   a mesma tem de ser incluída por relacionamento simples. Caso a coluna
   foi adicionar   tabela por relacionamento de dependência ou generalização
   a mesma não será incluída na tabela.
  */
  /*if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
      (colunas[i]->incluidoPorLigacao() || colunas[i]->incluidoPorDependencia())) ||
     (!colunas[i]->incluidoPorRelacionamento())) */
  if(def_type==SchemaParser::SQL_DEFINITION ||
     (def_type==SchemaParser::XML_DEFINITION &&
      !columns[i]->isAddedByRelationship()))
  {
   str_cols+=columns[i]->getCodeDefinition(def_type);

   if(def_type==SchemaParser::SQL_DEFINITION)
    setCommentAttribute(columns[i]);
  }
 }

 if(def_type==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_cols!="")
  {
   count=str_cols.size();
   if(str_cols[count-2]==',' || str_cols[count-2]=='\n')
    str_cols.remove(count-2,2);
  }
 }

 attributes[ParsersAttributes::COLUMNS]=str_cols;
}

void Table::setConstraintsAttribute(unsigned def_type)
{
 QString str_constr;
 unsigned i, count;
 bool inc_added_by_rel;
 Constraint *constr=NULL;

 //Concatena a definiação SQL das restrições
 count=constraints.size();
 for(i=0; i < count; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);

  //Chaves estrangeiras criadas pelo usuário não são incluídas na definição SQL da tabela
  if((def_type==SchemaParser::SQL_DEFINITION &&
      constr->getConstraintType()!=ConstraintType::foreign_key &&
      (!constr->isReferRelationshipAddedColumn() || constr->getConstraintType()==ConstraintType::primary_key)) ||

     (def_type==SchemaParser::XML_DEFINITION && !constr->isAddedByRelationship() &&
      constr->getConstraintType()!=ConstraintType::foreign_key &&
      ((constr->getConstraintType()!=ConstraintType::primary_key && !constr->isReferRelationshipAddedColumn()) ||
       (constr->getConstraintType()==ConstraintType::primary_key))))
  {
   inc_added_by_rel=(def_type==SchemaParser::SQL_DEFINITION);
   str_constr+=constr->getCodeDefinition(def_type,inc_added_by_rel);

   if(def_type==SchemaParser::SQL_DEFINITION)
    setCommentAttribute(constr);
  }
 }

 if(def_type==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_constr!="")
  {
   count=str_constr.size();
   if(str_constr[count-2]==',' || str_constr[count-2]=='\n')
   str_constr.remove(count-2,2);
  }
 }

 attributes[ParsersAttributes::CONSTRAINTS]=str_constr;
}

void Table::setTriggersAttribute(unsigned def_type)
{
 QString str_trig;
 unsigned i, count;
 Trigger *trig=NULL;

 //Concatena a definição SQL dos gatilhos da tabela
 count=triggers.size();
 for(i=0; i < count; i++)
 {
  //Obtém um gatilho
  trig=dynamic_cast<Trigger *>(triggers.at(i));

  if((/*!gat->isProtected() &&*/
      !trig->isReferRelationshipAddedColumn() &&
      def_type==SchemaParser::XML_DEFINITION) ||
      def_type==SchemaParser::SQL_DEFINITION)
  {
   str_trig+=trig->getCodeDefinition(def_type);
  }
 }

 attributes[ParsersAttributes::TRIGGERS]=str_trig;
}

void Table::setIndexesAttribute(unsigned def_type)
{
 QString str_ind;
 unsigned i, count;
 Index *ind=NULL;

 //Concatena a definição SQL dos índices da tabela
 count=indexes.size();
 for(i=0; i < count; i++)
 {
  ind=dynamic_cast<Index *>(indexes[i]);
  if((!ind->isAddedByRelationship() &&
      !ind->isReferRelationshipAddedColumn() &&
      def_type==SchemaParser::XML_DEFINITION) ||
      def_type==SchemaParser::SQL_DEFINITION)
   str_ind+=ind->getCodeDefinition(def_type);
 }

 attributes[ParsersAttributes::INDEXES]=str_ind;
}

void Table::setRulesAttribute(unsigned def_type)
{
 QString str_rule;
 unsigned i, count;

 //Concatena a definição SQL das regras da tabela
 count=rules.size();
 for(i=0; i < count; i++)
 {
  if((/* !regras[i]->isProtected() && */
      def_type==SchemaParser::XML_DEFINITION) ||
      def_type==SchemaParser::SQL_DEFINITION)
   str_rule+=rules[i]->getCodeDefinition(def_type);
 }

 attributes[ParsersAttributes::RULES]=str_rule;
}

vector<TableObject *> *Table::getObjectList(ObjectType obj_type)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(obj_type==OBJ_COLUMN)
  return(&columns);
 else if(obj_type==OBJ_CONSTRAINT)
  return(&constraints);
 else if(obj_type==OBJ_RULE)
  return(&rules);
 else if(obj_type==OBJ_TRIGGER)
  return(&triggers);
 else if(obj_type==OBJ_INDEX)
  return(&indexes);
 else
  //Dispara uma exceção caso o tipo de objeto seja inválido
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Table::addObject(BaseObject *obj, int obj_idx, bool copy_tab)
{
 ObjectType obj_type;

 if(!obj)
  //Caso o objeto não esteja alocado, dispara uma exceção
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int idx;
  obj_type=obj->getObjectType();
  ConstraintType constr_type;
  QString str_aux;

  try
  {
  /* Se o flag de tabela de cópia estiver marcado e o objeto passado
     for uma tabela muda o tipo para OBJETO_TABELA_BASE para forçar a
     inserção da tabela na lista de tabelas cópia */
  if(copy_tab && obj_type==OBJ_TABLE)
   obj_type=BASE_TABLE;

  /* Verifica se o objeto a ser adicionado   tabela possua o mesmo nome de um objeto que já
     existente,efetuando uma busca na lista de objetos através do nome do novo objeto */
  if(getObject(obj->getName(),obj_type,idx))
  {
   //Dispara uma exceçaõ indica a duplicidade de nomes de objetos
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(obj->getName(true))
           .arg(obj->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux,ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  /* Caso o objeto seja uma tabela (herança) e a mesma for a própria tabela a se adicionar o objeto,
     significa um caso de herança inválida onde uma tabela tenta herdar atributos dela mesma */
  else if(obj_type==OBJ_TABLE && obj==this)
   //Dispara a exceção indica o erro
   throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Insere o objeto   lista de acordo com seu tipo
   switch(obj_type)
   {
    case OBJ_COLUMN:
    case OBJ_CONSTRAINT:
    case OBJ_TRIGGER:
    case OBJ_INDEX:
    case OBJ_RULE:
      TableObject *tab_obj;
      vector<TableObject *> *obj_list;

      //Converte o objeto base para objeto de tabela
      tab_obj=dynamic_cast<TableObject *>(obj);

      //Verifica se o objeto não está relacionado com uma outra tabela
      if(!tab_obj->getParentTable())
       tab_obj->setParentTable(this);
      else if(tab_obj->getParentTable()!=this)
       throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Testando a definição sql do objeto
      if(obj_type==OBJ_COLUMN)
      {
       Column *col;
       col=dynamic_cast<Column *>(tab_obj);
       col->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(obj_type==OBJ_CONSTRAINT)
      {
       Constraint *constr;
       constr=dynamic_cast<Constraint *>(tab_obj);
       constr->getCodeDefinition(SchemaParser::SQL_DEFINITION);
       constr_type=constr->getConstraintType();
      }
      else if(obj_type==OBJ_INDEX)
      {
       Index *ind;
       ind=dynamic_cast<Index *>(tab_obj);
       ind->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(obj_type==OBJ_RULE)
      {
       Rule *rule;
       rule=dynamic_cast<Rule *>(tab_obj);
       rule->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(obj_type==OBJ_TRIGGER)
      {
       Trigger *trig;
       trig=dynamic_cast<Trigger *>(tab_obj);
       trig->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }

      //Insere o objeto na lista correta
      obj_list=getObjectList(obj_type);

      /* Caso o usuário tente inserir mais uma chave primária na tabela
         será retornado um erro pois uma tabela pode possuir apenas 1
         chave primária */
      if(obj_type==OBJ_CONSTRAINT &&
         constr_type==ConstraintType::primary_key &&
         this->getPrimaryKey())
       throw Exception(ERR_ASG_EXISTING_PK_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Caso o índice passado seja menor que zero ou superior ao tamanho da lista
         o objeto será inserido ao final da mesma */
      if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
       obj_list->push_back(tab_obj);
      else
      {
       //Caso o índice seja válido, insere o objeto na posição especificada
       if(obj_list->size() > 0)
        obj_list->insert((obj_list->begin() + obj_idx), tab_obj);
       else
        obj_list->push_back(tab_obj);
      }
    break;

    case OBJ_TABLE:
     Table *tab;
     tab=dynamic_cast<Table *>(obj);
     if(obj_idx < 0 || obj_idx >= static_cast<int>(ancestor_tables.size()))
      ancestor_tables.push_back(tab);
     else
      ancestor_tables.insert((ancestor_tables.begin() + obj_idx), tab);
    break;

    case BASE_TABLE:
     Table *tab1;
     tab1=dynamic_cast<Table *>(obj);
     if(obj_idx < 0 || obj_idx >= static_cast<int>(copy_tables.size()))
      copy_tables.push_back(tab1);
     else
      copy_tables.insert((copy_tables.begin() + obj_idx), tab1);
    break;

    default:
     throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    break;
   }
  }
  catch(Exception &e)
  {
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
    throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(obj->getName()))
                              .arg(obj->getTypeName()),
                  ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
   else
    //Caso o a exceção não seja de def. SQL mal-formada, apenas redireciona o erro
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

void Table::addColumn(Column *col, int idx)
{
 addObject(col, idx);
}

void Table::addTrigger(Trigger *trig, int idx)
{
 addObject(trig, idx);
}

void Table::addIndex(Index *ind, int idx)
{
 addObject(ind, idx);
}

void Table::addRule(Rule *reg, int idx_reg)
{
 addObject(reg, idx_reg);
}

void Table::addConstraint(Constraint *constr, int idx)
{
 addObject(constr, idx);
}

void Table::addAncestorTable(Table *tab, int idx)
{
 addObject(tab, idx);
}

void Table::addCopyTable(Table *tab, int idx)
{
 addObject(tab, idx, true);
}

void Table::removeObject(BaseObject *obj)
{
 if(obj)
  removeObject(obj->getName(), obj->getObjectType());
}

void Table::removeObject(const QString &name, ObjectType obj_type)
{
 int idx;

 //Obtém o índice do objeto localizando-o na lista de objetos
 getObject(name,obj_type,idx);

 //Caso seja encontrado (indice maior que 0)
 if(idx>=0)
  //Remove objeto através de seu índice
  removeObject(static_cast<unsigned>(idx),obj_type);
}

void Table::removeObject(unsigned obj_idx, ObjectType obj_type)
{
 /* Verfica se o tipo do objeto especifica está fora do conjunto dos que são
    aceitos pela classe tabela */
 if(obj_type!=OBJ_COLUMN && obj_type!=OBJ_CONSTRAINT &&
    obj_type!=OBJ_TRIGGER && obj_type!=OBJ_INDEX &&
    obj_type!=OBJ_RULE && obj_type!=OBJ_TABLE &&
    obj_type!=BASE_TABLE)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else if(obj_type==OBJ_TABLE && obj_idx < ancestor_tables.size())
 {
  vector<Table *>::iterator itr;

  itr=ancestor_tables.begin() + obj_idx; //Obtendo a referênci  tabela pai
  ancestor_tables.erase(itr); //Remove a tabela pai da lista
 }
 //Caso o objeto a ser removido seja uma tabela cópia e seu índice seja válido
 else if(obj_type==BASE_TABLE && obj_idx < copy_tables.size())
 {
  vector<Table *>::iterator itr;

  itr=copy_tables.begin() + obj_idx; //Obtendo a referênci  tabela copia
  copy_tables.erase(itr); //Remove a tabela pai da lista
 }
 else if(obj_type!=OBJ_TABLE && obj_type!=BASE_TABLE)
 {
  vector<TableObject *> *obj_list=NULL;
  vector<TableObject *>::iterator itr;

  //Obtém a lista de objetos de acordo com o tipo passado
  obj_list=getObjectList(obj_type);

  //Caso o indice do objeto extrapole o tamanho da lista de objetos
  if(obj_idx >= obj_list->size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Executando a remoção genérica de objetos que não sejam colunas
  if(obj_type!=OBJ_COLUMN)
  {
   itr=obj_list->begin() + obj_idx; //Obtendo a referência ao objeto
   (*itr)->setParentTable(NULL);
   obj_list->erase(itr); //Remove a regra da lista
  }
  else
  {
   vector<TableObject *> refs;
   Column *column=NULL;

   //Obtém a referência para a coluna
   itr=obj_list->begin() + obj_idx;
   column=dynamic_cast<Column *>(*itr); //Obtém a coluna
   getColumnReferences(column, refs, true);

   /* Caso a coluna esteja sendo referenciada no gatilho, interrompe a
      remoção da mesma disparando uma exceção. */
   if(!refs.empty())
   {
    throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                          .arg(QString::fromUtf8(column->getName()))
                          .arg(column->getTypeName())
                          .arg(QString::fromUtf8(refs[0]->getName()))
                          .arg(refs[0]->getTypeName())
                          .arg(QString::fromUtf8(this->getName(true)))
                          .arg(this->getTypeName()),
                 ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   column->setParentTable(NULL);
   columns.erase(itr); //Remove a coluna da lista
  }
 }
}

void Table::removeColumn(const QString &name)
{
 removeObject(name,OBJ_COLUMN);
}

void Table::removeColumn(unsigned idx)
{
 removeObject(idx,OBJ_COLUMN);
}

void Table::removeTrigger(const QString &name)
{
 removeObject(name,OBJ_TRIGGER);
}

void Table::removeTrigger(unsigned idx)
{
 removeObject(idx,OBJ_TRIGGER);
}

void Table::removeIndex(const QString &name)
{
 removeObject(name,OBJ_INDEX);
}

void Table::removeIndex(unsigned idx)
{
 removeObject(idx,OBJ_INDEX);
}

void Table::removeRule(const QString &name)
{
 removeObject(name,OBJ_RULE);
}

void Table::removeRule(unsigned idx)
{
 removeObject(idx,OBJ_RULE);
}

void Table::removeConstraint(const QString &name)
{
 removeObject(name,OBJ_CONSTRAINT);
}

void Table::removeConstraint(unsigned idx)
{
 removeObject(idx,OBJ_CONSTRAINT);
}

void Table::removeAncestorTable(const QString &name)
{
 removeObject(name,OBJ_TABLE);
}

void Table::removeAncestorTable(unsigned idx)
{
 removeObject(idx,OBJ_TABLE);
}

void Table::removeCopyTable(const QString &name)
{
 removeObject(name,BASE_TABLE);
}

void Table::removeCopyTable(unsigned idx)
{
 removeObject(idx,BASE_TABLE);
}

int Table::getObjectIndex(const QString &name, ObjectType obj_type)
{
 int idx;
 getObject(name, obj_type, idx);
 return(idx);
}

int Table::getObjectIndex(TableObject *obj)
{
 if(!obj)
  return(-1);
 else
  return(getObjectIndex(obj->getName(true), obj->getObjectType()));
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type)
{
 int idx;
 return(getObject(name, obj_type, idx));
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type, int &obj_idx)
{
 BaseObject *object=NULL;
 bool found=false, format=false;

 format=name.contains("\"");

 //Caso o objeto a ser obtido seja uma coluna
 if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
    obj_type==OBJ_TRIGGER ||
    obj_type==OBJ_INDEX || obj_type==OBJ_RULE)
 {
  vector<TableObject *>::iterator itr, itr_end;
  vector<TableObject *> *obj_list=NULL;
  int count;
  QString aux_name=name;

  count=aux_name.count(QChar('\0'));
  if(count >=1) aux_name.chop(count);

  obj_list=getObjectList(obj_type);

  //Obtém as referências ao primeiro e ultimo elementos da lista
  itr=obj_list->begin();
  itr_end=obj_list->end();

  //Varre a lista de objetos
  while(itr!=itr_end)
  {
   //Verifica se o nome a ser encontrado é igual o nome do objeto atual
   found=((*itr)->getName(format)==aux_name);
   if(!found) itr++; //Caso não seja encontrado passa para outro elemento da lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(found)
  {
   //Define seu índice
   obj_idx=(itr-obj_list->begin());

   //Armazena o objeto em um ponteiro que será retornado
   object=(*itr);
  }
  else obj_idx=-1;
 }

 //Caso o objeto a ser obtido seja uma tabela pai
 else if(obj_type==OBJ_TABLE || obj_type==BASE_TABLE)
 {
  vector<Table *>::iterator itr_tab, itr_end_tab;
  vector<Table *> *tables;
  QString aux_name;

  //Obrigatoriamente todo nome de tabela deve ser formatado por conta do esquema
  aux_name=BaseObject::formatName(name);

  if(obj_type==OBJ_TABLE)
   tables=&ancestor_tables;
  else
   tables=&copy_tables;

  //Obtém as referências ao primeiro e ultimo elementos da lista de tabelas descendentes
  itr_tab=tables->begin();
  itr_end_tab=tables->end();

  //Varre a lista de tabelas descendentes
  while(itr_tab!=itr_end_tab)
  {
   //Verifica se o nome a ser encontrado é igual o nome da tabela pai atual
   /* Diferente de outros tipos de objeto, as tabelas são sempre comparadas
      COM O NOME FORMATADO por precisam ser 'schema-qualified' evitando que
      uma tabelas de nomes iguais porém de esquemas diferentes sejam confundidas */
   found=((*itr_tab)->getName(true)==aux_name);
   if(!found) itr_tab++; //Caso seja encontrado, interrompe a varredura pela lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(found)
  {
   //Define seu índice
   obj_idx=(itr_tab-tables->begin());

   //Armazena o objeto em um ponteiro que será retornado
   object=(*itr_tab);
  }
 }
 else
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o objeto encontrado
 return(object);
}

BaseObject *Table::getObject(unsigned obj_idx, ObjectType obj_type)
{
 vector<TableObject *> *obj_list=NULL;

 if(obj_type==OBJ_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(obj_idx >= ancestor_tables.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela pai no índice especificado
  return(ancestor_tables[obj_idx]);
 }
 else if(obj_type==BASE_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(obj_idx >= copy_tables.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela cópia no índice especificado
  return(copy_tables[obj_idx]);
 }
 else
 {
  /* Obtém um objeto de uma determinada lista de acordo com seu tipo
     e se seu índice seja válido */
  obj_list=getObjectList(obj_type);
  if(obj_idx < obj_list->size())
   return(obj_list->at(obj_idx));
  else
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

Table *Table::getAncestorTable(const QString &name)
{
 int idx;
 return(dynamic_cast<Table *>(getObject(name,OBJ_TABLE,idx)));
}

Table *Table::getAncestorTable(unsigned idx)
{
 return(dynamic_cast<Table *>(getObject(idx,OBJ_TABLE)));
}

Column *Table::getColumn(const QString &name, bool ref_old_name)
{
 if(!ref_old_name)
 {
  int idx;
  return(dynamic_cast<Column *>(getObject(name,OBJ_COLUMN,idx)));
 }
 else
 {
  Column *column=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool found=false, format=false;

  format=name.contains("\"");
  itr=columns.begin();
  itr_end=columns.end();

  //Varre a lista de colunas buscando pelo nome antigo das mesmas
  while(itr!=itr_end && !found)
  {
   column=dynamic_cast<Column *>(*itr);
   itr++;
   found=(name!="" && column->getOldName(format)==name);
  }

  /* Caso nenhuma coluna for encontrada zera o ponteiro de coluna
     para forçar o retorno de uma coluna nula indicando que a
     busca não retornou objeto */
  if(!found) column=NULL;
  return(column);
 }
}

Column *Table::getColumn(unsigned idx)
{
 return(dynamic_cast<Column *>(getObject(idx,OBJ_COLUMN)));
}

Trigger *Table::getTrigger(const QString &name)
{
 int idx;
 return(dynamic_cast<Trigger *>(getObject(name,OBJ_TRIGGER,idx)));
}

Trigger *Table::getTrigger(unsigned idx)
{
 return(dynamic_cast<Trigger *>(getObject(idx,OBJ_TRIGGER)));
}

Constraint *Table::getConstraint(const QString &name)
{
 int idx;
 return(dynamic_cast<Constraint *>(getObject(name,OBJ_CONSTRAINT,idx)));
}

Constraint *Table::getConstraint(unsigned idx)
{
 return(dynamic_cast<Constraint *>(getObject(idx,OBJ_CONSTRAINT)));
}

Index *Table::getIndex(const QString &name)
{
 int idx;
 return(dynamic_cast<Index *>(getObject(name,OBJ_INDEX,idx)));
}

Index *Table::getIndex(unsigned idx)
{
 return(dynamic_cast<Index *>(getObject(idx,OBJ_INDEX)));
}

Rule *Table::getRule(const QString &name)
{
 int idx;
 return(dynamic_cast<Rule *>(getObject(name,OBJ_RULE,idx)));
}

Rule *Table::getRule(unsigned idx)
{
 return(dynamic_cast<Rule *>(getObject(idx,OBJ_RULE)));
}

Table *Table::getCopyTable(const QString &name)
{
 int idx;
 return(dynamic_cast<Table *>(getObject(name,BASE_TABLE,idx)));
}

Table *Table::getCopyTable(unsigned idx)
{
 return(dynamic_cast<Table *>(getObject(idx,BASE_TABLE)));
}

unsigned Table::getColumnCount(void)
{
 return(columns.size());
}

unsigned Table::getTriggerCount(void)
{
 return(triggers.size());
}

unsigned Table::getConstraintCount(void)
{
 return(constraints.size());
}

unsigned Table::getIndexCount(void)
{
 return(indexes.size());
}

unsigned Table::getRuleCount(void)
{
 return(rules.size());
}

unsigned Table::getAncestorTable(void)
{
 return(ancestor_tables.size());
}

unsigned Table::getCopyTable(void)
{
 return(copy_tables.size());
}

unsigned Table::getObjectCount(ObjectType obj_type, bool inc_added_by_rel)
{
 if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
    obj_type==OBJ_TRIGGER ||/* tipo_obj==OBJETO_GAT_RESTRICAO ||*/
    obj_type==OBJ_INDEX || obj_type==OBJ_RULE ||
    obj_type==OBJ_TABLE || obj_type==BASE_TABLE)
 {
  if(obj_type==OBJ_TABLE || obj_type==BASE_TABLE)
  {
   if(obj_type==OBJ_TABLE)
    return(ancestor_tables.size());
   else
    return(copy_tables.size());
  }
  else
  {
   vector<TableObject *> *list=NULL;
   list=getObjectList(obj_type);

   //Faz a contagem apenas dos objetos que não são auto-incluidos (incluídos por relacionamento)
   if(!inc_added_by_rel)
   {
    vector<TableObject *>::iterator itr, itr_end;
    unsigned count=0;

    itr=list->begin();
    itr_end=list->end();
    while(itr!=itr_end)
    {
     if(!(*itr)->isAddedByRelationship()) count++;
     itr++;
    }

    return(count);
   }
   else
    return(list->size());
  }
 }
 else
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Table::getPrimaryKey(void)
{
 unsigned count,i;
 Constraint *pk=NULL, *constr=NULL;

 count=constraints.size();
 for(i=0; i < count && !pk; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);
  pk=(constr->getConstraintType()==ConstraintType::primary_key ? constr : NULL);
 }

 return(pk);
}

void Table::getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel)
{
 unsigned count,i;
 Constraint *constr=NULL;

 count=constraints.size();
 for(i=0; i < count; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);

  /* Adiciona a restrição caso seja chave estrangeira, caso esta não foi
     incluída por relacionamento ou se foi e o parametro inc_insporrelacao está
     setado */
  if(constr->getConstraintType()==ConstraintType::foreign_key &&
     (!constr->isAddedByLinking() ||
      (constr->isAddedByLinking() && inc_added_by_rel)))
   fks.push_back(constr);
 }
}

bool Table::isWithOIDs(void)
{
 return(with_oid);
}

bool Table::isReferTableOnForeignKey(Table *ref_tab)
{
 unsigned count,i;
 Constraint *constr=NULL;
 bool found=false;

 count=constraints.size();
 for(i=0; i < count && !found; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);
  found=(constr->getConstraintType()==ConstraintType::foreign_key &&
         !constr->isAddedByLinking() &&
          constr->getReferencedTable() == ref_tab);
 }

 return(found);
}

bool Table::isConstraintRefColumn(Column *column, ConstraintType constr_type)
{
 bool found=false;
 vector<TableObject *>::iterator itr, itr_end;
 Constraint *constr=NULL;

 if(column)
 {
  itr=constraints.begin();
  itr_end=constraints.end();
  while(itr!=itr_end && !found)
  {
   constr=dynamic_cast<Constraint *>(*itr);
   itr++;
   found=(constr->getConstraintType()==constr_type &&
        (constr->isColumnExists(column, Constraint::SOURCE_COLS) ||
         constr->isColumnExists(column, Constraint::REFERENCED_COLS)));
  }
 }

 return(found);
}

QString Table::getCodeDefinition(unsigned def_type)
{
 attributes[ParsersAttributes::OIDS]=(with_oid ? "1" : "");
 setColumnsAttribute(def_type);
 setConstraintsAttribute(def_type);
 setTriggersAttribute(def_type);
 setIndexesAttribute(def_type);
 setRulesAttribute(def_type);

 if(def_type==SchemaParser::XML_DEFINITION)
  setPositionAttribute();

 return(BaseObject::__getCodeDefinition(def_type));
}

void Table::operator = (Table &tab)
{
 QString prev_name = this->getName(true);

 (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tab);
 this->with_oid=tab.with_oid;
 setProtected(tab.is_protected);

 PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

bool Table::isReferRelationshipAddedObject(void)
{
 vector<TableObject *>::iterator itr, itr_end;
 ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
 bool found=false;

 for(unsigned i=0; i < 2 && !found; i++)
 {
  //Obtém a lista de tipos atual
  itr=getObjectList(types[i])->begin();
  itr_end=getObjectList(types[i])->end();

  /* Efetua uma iteração verifica se os objetos da lista
     atual foram incluídas por relacionamento, caso uma objeto
     for detectada como incluída desta forma é suficiente
     dizer que a tabela referencia um objeto vindo de
     relacionamento */
  while(itr!=itr_end && !found)
  {
   //Obtém se a coluna foi incluída por relacionamento ou não
   found=(*itr)->isAddedByRelationship();
   //Passa para a próxima coluna
   itr++;
  }
 }

 return(found);
}

void Table::swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2)
{
 vector<TableObject *> *obj_list=NULL;
 vector<TableObject *>::iterator itr1, itr2;
 TableObject *aux_obj=NULL;

 try
 {
  if(idx1!=idx2)
  {
   obj_list=getObjectList(obj_type);

   if(idx1 >= obj_list->size() || idx2 >= obj_list->size())
    throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   aux_obj=obj_list->at(idx1);
   itr1=obj_list->begin() + idx1;
   itr2=obj_list->begin() + idx2;

   (*itr1)=(*itr2);
   (*itr2)=aux_obj;
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Table::getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode)
{
 /* Caso a coluna não foi incluída por relacionamento, será verificado se esta
    não está sendo referenciada. Para colunas adicionadas automaticamente por
    relacionamentos, sua remoção é feita sem verificação alguma */
 if(column && !column->isAddedByRelationship())
 {
  unsigned count, i;
  IndexElement elem;
  Column *col=NULL, *col1=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool found=false;
  Index *ind=NULL;
  Constraint *constr=NULL;
  Trigger *trig=NULL;

  //Verificando se a coluna não é referenciada nos indices
  itr=indexes.begin();
  itr_end=indexes.end();

  while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
  {
   ind=dynamic_cast<Index *>(*itr);
   itr++;

   count=ind->getElementCount();
   for(i=0; i < count  && (!exclusion_mode || (exclusion_mode && !found)); i++)
   {
    elem=ind->getElement(i);
    col=elem.getColumn();
    if(col && col==column)
    {
     found=true;
     refs.push_back(ind);
    }
   }
  }

  //Verificando se a coluna não é referenciada nas constraints
  itr=constraints.begin();
  itr_end=constraints.end();

  while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   constr=dynamic_cast<Constraint *>(*itr);
   itr++;

   col=constr->getColumn(column->getName(),true);
   col1=constr->getColumn(column->getName(),false);

   if((col && col==column) || (col1 && col1==column))
   {
    found=true;
    refs.push_back(constr);
   }
  }

  //Verificando se a coluna não é referenciada nos gatilhos
  itr=triggers.begin();
  itr_end=triggers.end();

    while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   trig=dynamic_cast<Trigger *>(*itr);
   itr++;

   count=trig->getColumnCount();
   for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !found)); i++)
   {
    if(trig->getColumn(i)==column)
    {
     found=true;
     refs.push_back(trig);
    }
   }
  }
 }
}

