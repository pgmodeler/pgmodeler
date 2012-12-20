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
 ObjectType tipos[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE,
                          OBJ_CONSTRAINT, OBJ_COLUMN };
 vector<TableObject *> *lista=NULL;

 for(unsigned i=0; i < 5; i++)
 {
  lista=getObjectList(tipos[i]);
  while(!lista->empty())
  {
   delete(lista->back());
   lista->pop_back();
  }
 }

 copy_tables.clear();
 ancestor_tables.clear();
}

void Table::setName(const QString &nome)
{
 QString nome_ant=this->getName(true);
 BaseObject::setName(nome);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 PgSQLType::renameUserType(nome_ant, this, this->getName(true));
}

void Table::setSchema(BaseObject *esquema)
{
 QString nome_ant=this->getName(true);

 //Atribui o esquema   tabela
 BaseObject::setSchema(esquema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 PgSQLType::renameUserType(nome_ant, this, this->getName(true));
}

void Table::setWithOIDs(bool valor)
{
 with_oid=valor;
}

void Table::setProtected(bool protegido)
{
 ObjectType tipos_obj[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
                              OBJ_INDEX, OBJ_RULE, OBJ_TRIGGER };
 unsigned i;
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *lista=NULL;
 TableObject *obj_tab=NULL;

 for(i=0; i < 5; i++)
 {
  //Obtém a lista de objetos
  lista=getObjectList(tipos_obj[i]);
  itr=lista->begin();
  itr_end=lista->end();

  //Varre a lista protegendo/desprotegendo os elementos
  while(itr!=itr_end)
  {
   //Obtém um objeto da lista
   obj_tab=(*itr);
   /* Só protegerá o objeto caso o mesmo não foi incluído
      por relacionamento pois quando é o caso os mesmos
      já vem protegidos */
   if(!obj_tab->isAddedByRelationship())
    obj_tab->setProtected(protegido);
   itr++;
  }
 }

 BaseGraphicObject::setProtected(protegido);
}

void Table::setCommentAttribute(TableObject *obj_tab)
{
 if(obj_tab && !obj_tab->getComment().isEmpty())
 {
  map<QString, QString> attribs;

  attribs[ParsersAttributes::DIF_SQL]="1";
  attribs[ParsersAttributes::SQL_OBJECT]=obj_tab->getSQLName();
  attribs[ParsersAttributes::COLUMN]=(obj_tab->getObjectType()==OBJ_COLUMN ? "1" : "");
  attribs[ParsersAttributes::CONSTRAINT]=(obj_tab->getObjectType()==OBJ_CONSTRAINT ? "1" : "");
  attribs[ParsersAttributes::TABLE]=this->getName(true);
  attribs[ParsersAttributes::NAME]=obj_tab->getName(true);
  attribs[ParsersAttributes::COMMENT]=obj_tab->getComment();

  SchemaParser::setIgnoreUnkownAttributes(true);
  attributes[ParsersAttributes::COLS_COMMENT]+=SchemaParser::getObjectDefinition(ParsersAttributes::COMMENT, attribs,
                                                                                 SchemaParser::SQL_DEFINITION);
  SchemaParser::setIgnoreUnkownAttributes(false);
 }
}

void Table::setColumnsAttribute(unsigned tipo_def)
{
 QString str_cols;
 unsigned i, qtd;

 //Concatena a definiação SQL das colunas
 qtd=columns.size();
 for(i=0; i < qtd; i++)
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
  if(tipo_def==SchemaParser::SQL_DEFINITION ||
     (tipo_def==SchemaParser::XML_DEFINITION &&
      !columns[i]->isAddedByRelationship()))
  {
   str_cols+=columns[i]->getCodeDefinition(tipo_def);

   if(tipo_def==SchemaParser::SQL_DEFINITION)
    setCommentAttribute(columns[i]);
  }
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_cols!="")
  {
   qtd=str_cols.size();
   if(str_cols[qtd-2]==',' || str_cols[qtd-2]=='\n')
    str_cols.remove(qtd-2,2);
  }
 }

 attributes[ParsersAttributes::COLUMNS]=str_cols;
}

void Table::setConstraintsAttribute(unsigned tipo_def)
{
 QString str_rest;
 unsigned i, qtd;
 bool inc_insporrelacao;
 Constraint *rest=NULL;

 //Concatena a definiação SQL das restrições
 qtd=constraints.size();
 for(i=0; i < qtd; i++)
 {
  rest=dynamic_cast<Constraint *>(constraints[i]);

  //Chaves estrangeiras criadas pelo usuário não são incluídas na definição SQL da tabela
  if((tipo_def==SchemaParser::SQL_DEFINITION &&
      rest->getConstraintType()!=ConstraintType::foreign_key &&
      (!rest->isReferRelationshipAddedColumn() || rest->getConstraintType()==ConstraintType::primary_key)) ||

     (tipo_def==SchemaParser::XML_DEFINITION && !rest->isAddedByRelationship() &&
      rest->getConstraintType()!=ConstraintType::foreign_key &&
      ((rest->getConstraintType()!=ConstraintType::primary_key && !rest->isReferRelationshipAddedColumn()) ||
       (rest->getConstraintType()==ConstraintType::primary_key))))
  {
   inc_insporrelacao=(tipo_def==SchemaParser::SQL_DEFINITION);
   str_rest+=rest->getCodeDefinition(tipo_def,inc_insporrelacao);

   if(tipo_def==SchemaParser::SQL_DEFINITION)
    setCommentAttribute(rest);
  }
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_rest!="")
  {
   qtd=str_rest.size();
   if(str_rest[qtd-2]==',' || str_rest[qtd-2]=='\n')
   str_rest.remove(qtd-2,2);
  }
 }

 attributes[ParsersAttributes::CONSTRAINTS]=str_rest;
}

void Table::setTriggersAttribute(unsigned tipo_def)
{
 QString str_gat;
 unsigned i, qtd;
 Trigger *gat=NULL;

 //Concatena a definição SQL dos gatilhos da tabela
 qtd=triggers.size();
 for(i=0; i < qtd; i++)
 {
  //Obtém um gatilho
  gat=dynamic_cast<Trigger *>(triggers.at(i));

  if((/*!gat->isProtected() &&*/
      !gat->isReferRelationshipAddedColumn() &&
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
  {
   str_gat+=gat->getCodeDefinition(tipo_def);
  }
 }

 attributes[ParsersAttributes::TRIGGERS]=str_gat;
}

void Table::setIndexesAttribute(unsigned tipo_def)
{
 QString str_ind;
 unsigned i, qtd;
 Index *ind=NULL;

 //Concatena a definição SQL dos índices da tabela
 qtd=indexes.size();
 for(i=0; i < qtd; i++)
 {
  ind=dynamic_cast<Index *>(indexes[i]);
  if((!ind->isAddedByRelationship() &&
      !ind->isReferRelationshipAddedColumn() &&
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
   str_ind+=ind->getCodeDefinition(tipo_def);
 }

 attributes[ParsersAttributes::INDEXES]=str_ind;
}

void Table::setRulesAttribute(unsigned tipo_def)
{
 QString str_reg;
 unsigned i, qtd;

 //Concatena a definição SQL das regras da tabela
 qtd=rules.size();
 for(i=0; i < qtd; i++)
 {
  if((/* !regras[i]->isProtected() && */
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
   str_reg+=rules[i]->getCodeDefinition(tipo_def);
 }

 attributes[ParsersAttributes::RULES]=str_reg;
}

vector<TableObject *> *Table::getObjectList(ObjectType tipo_obj)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(tipo_obj==OBJ_COLUMN)
  return(&columns);
 else if(tipo_obj==OBJ_CONSTRAINT)
  return(&constraints);
 else if(tipo_obj==OBJ_RULE)
  return(&rules);
 else if(tipo_obj==OBJ_TRIGGER)
  return(&triggers);
 else if(tipo_obj==OBJ_INDEX)
  return(&indexes);
 else
  //Dispara uma exceção caso o tipo de objeto seja inválido
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Table::addObject(BaseObject *obj, int idx_obj, bool tab_copia)
{
 ObjectType tipo_obj;

 if(!obj)
  //Caso o objeto não esteja alocado, dispara uma exceção
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int idx;
  tipo_obj=obj->getObjectType();
  ConstraintType tipo_rest;
  QString str_aux;

  try
  {
  /* Se o flag de tabela de cópia estiver marcado e o objeto passado
     for uma tabela muda o tipo para OBJETO_TABELA_BASE para forçar a
     inserção da tabela na lista de tabelas cópia */
  if(tab_copia && tipo_obj==OBJ_TABLE)
   tipo_obj=BASE_TABLE;

  /* Verifica se o objeto a ser adicionado   tabela possua o mesmo nome de um objeto que já
     existente,efetuando uma busca na lista de objetos através do nome do novo objeto */
  if(getObject(obj->getName(),tipo_obj,idx))
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
  else if(tipo_obj==OBJ_TABLE && obj==this)
   //Dispara a exceção indica o erro
   throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Insere o objeto   lista de acordo com seu tipo
   switch(tipo_obj)
   {
    case OBJ_COLUMN:
    case OBJ_CONSTRAINT:
    case OBJ_TRIGGER:
    case OBJ_INDEX:
    case OBJ_RULE:
      TableObject *obj_tab;
      vector<TableObject *> *lista_obj;

      //Converte o objeto base para objeto de tabela
      obj_tab=dynamic_cast<TableObject *>(obj);

      //Verifica se o objeto não está relacionado com uma outra tabela
      if(!obj_tab->getParentTable())
       obj_tab->setParentTable(this);
      else if(obj_tab->getParentTable()!=this)
       throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Testando a definição sql do objeto
      if(tipo_obj==OBJ_COLUMN)
      {
       Column *col;
       col=dynamic_cast<Column *>(obj_tab);
       col->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_CONSTRAINT)
      {
       Constraint *rest;
       rest=dynamic_cast<Constraint *>(obj_tab);
       rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);
       tipo_rest=rest->getConstraintType();
      }
      else if(tipo_obj==OBJ_INDEX)
      {
       Index *ind;
       ind=dynamic_cast<Index *>(obj_tab);
       ind->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_RULE)
      {
       Rule *reg;
       reg=dynamic_cast<Rule *>(obj_tab);
       reg->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_TRIGGER)
      {
       Trigger *gat;
       gat=dynamic_cast<Trigger *>(obj_tab);
       gat->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }

      //Insere o objeto na lista correta
      lista_obj=getObjectList(tipo_obj);

      /* Caso o usuário tente inserir mais uma chave primária na tabela
         será retornado um erro pois uma tabela pode possuir apenas 1
         chave primária */
      if(tipo_obj==OBJ_CONSTRAINT &&
         tipo_rest==ConstraintType::primary_key &&
         this->getPrimaryKey())
       throw Exception(ERR_ASG_EXISTING_PK_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Caso o índice passado seja menor que zero ou superior ao tamanho da lista
         o objeto será inserido ao final da mesma */
      if(idx_obj < 0 || idx_obj >= static_cast<int>(lista_obj->size()))
       lista_obj->push_back(obj_tab);
      else
      {
       //Caso o índice seja válido, insere o objeto na posição especificada
       if(lista_obj->size() > 0)
        lista_obj->insert((lista_obj->begin() + idx_obj), obj_tab);
       else
        lista_obj->push_back(obj_tab);
      }
    break;

    case OBJ_TABLE:
     Table *tab;
     tab=dynamic_cast<Table *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(ancestor_tables.size()))
      ancestor_tables.push_back(tab);
     else
      ancestor_tables.insert((ancestor_tables.begin() + idx_obj), tab);
    break;

    case BASE_TABLE:
     Table *tab1;
     tab1=dynamic_cast<Table *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(copy_tables.size()))
      copy_tables.push_back(tab1);
     else
      copy_tables.insert((copy_tables.begin() + idx_obj), tab1);
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

void Table::addColumn(Column *col, int idx_col)
{
 addObject(col, idx_col);
}

void Table::addTrigger(Trigger *gat, int idx_gat)
{
 addObject(gat, idx_gat);
}

void Table::addIndex(Index *ind, int idx_ind)
{
 addObject(ind, idx_ind);
}

void Table::addRule(Rule *reg, int idx_reg)
{
 addObject(reg, idx_reg);
}

void Table::addConstraint(Constraint *constr, int idx_rest)
{
 addObject(constr, idx_rest);
}

void Table::addAncestorTable(Table *tab, int idx_tab)
{
 addObject(tab, idx_tab);
}

void Table::addCopyTable(Table *tab, int idx_tab)
{
 addObject(tab, idx_tab, true);
}

void Table::removeObject(BaseObject *objeto)
{
 if(objeto)
  removeObject(objeto->getName(), objeto->getObjectType());
}

void Table::removeObject(const QString &nome, ObjectType tipo_obj)
{
 int idx;

 //Obtém o índice do objeto localizando-o na lista de objetos
 getObject(nome,tipo_obj,idx);

 //Caso seja encontrado (indice maior que 0)
 if(idx>=0)
  //Remove objeto através de seu índice
  removeObject(static_cast<unsigned>(idx),tipo_obj);
}

void Table::removeObject(unsigned idx_obj, ObjectType tipo_obj)
{
 /* Verfica se o tipo do objeto especifica está fora do conjunto dos que são
    aceitos pela classe tabela */
 if(tipo_obj!=OBJ_COLUMN && tipo_obj!=OBJ_CONSTRAINT &&
    tipo_obj!=OBJ_TRIGGER && tipo_obj!=OBJ_INDEX &&
    tipo_obj!=OBJ_RULE && tipo_obj!=OBJ_TABLE &&
    tipo_obj!=BASE_TABLE)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else if(tipo_obj==OBJ_TABLE && idx_obj < ancestor_tables.size())
 {
  vector<Table *>::iterator itr;

  itr=ancestor_tables.begin() + idx_obj; //Obtendo a referênci  tabela pai
  ancestor_tables.erase(itr); //Remove a tabela pai da lista
 }
 //Caso o objeto a ser removido seja uma tabela cópia e seu índice seja válido
 else if(tipo_obj==BASE_TABLE && idx_obj < copy_tables.size())
 {
  vector<Table *>::iterator itr;

  itr=copy_tables.begin() + idx_obj; //Obtendo a referênci  tabela copia
  copy_tables.erase(itr); //Remove a tabela pai da lista
 }
 else if(tipo_obj!=OBJ_TABLE && tipo_obj!=BASE_TABLE)
 {
  vector<TableObject *> *lista_obj=NULL;
  vector<TableObject *>::iterator itr;

  //Obtém a lista de objetos de acordo com o tipo passado
  lista_obj=getObjectList(tipo_obj);

  //Caso o indice do objeto extrapole o tamanho da lista de objetos
  if(idx_obj >= lista_obj->size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Executando a remoção genérica de objetos que não sejam colunas
  if(tipo_obj!=OBJ_COLUMN)
  {
   itr=lista_obj->begin() + idx_obj; //Obtendo a referência ao objeto
   (*itr)->setParentTable(NULL);
   lista_obj->erase(itr); //Remove a regra da lista
  }
  else
  {
   vector<TableObject *> vet_refs;
   Column *coluna=NULL;

   //Obtém a referência para a coluna
   itr=lista_obj->begin() + idx_obj;
   coluna=dynamic_cast<Column *>(*itr); //Obtém a coluna
   getColumnReferences(coluna, vet_refs, true);

   /* Caso a coluna esteja sendo referenciada no gatilho, interrompe a
      remoção da mesma disparando uma exceção. */
   if(!vet_refs.empty())
   {
    throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                          .arg(QString::fromUtf8(coluna->getName()))
                          .arg(coluna->getTypeName())
                          .arg(QString::fromUtf8(vet_refs[0]->getName()))
                          .arg(vet_refs[0]->getTypeName())
                          .arg(QString::fromUtf8(this->getName(true)))
                          .arg(this->getTypeName()),
                 ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   coluna->setParentTable(NULL);
   columns.erase(itr); //Remove a coluna da lista
  }
 }
}

void Table::removeColumn(const QString &nome)
{
 removeObject(nome,OBJ_COLUMN);
}

void Table::removeColumn(unsigned idx_col)
{
 removeObject(idx_col,OBJ_COLUMN);
}

void Table::removeTrigger(const QString &nome)
{
 removeObject(nome,OBJ_TRIGGER);
}

void Table::removeTrigger(unsigned idx_gat)
{
 removeObject(idx_gat,OBJ_TRIGGER);
}

void Table::removeIndex(const QString &nome)
{
 removeObject(nome,OBJ_INDEX);
}

void Table::removeIndex(unsigned idx_ind)
{
 removeObject(idx_ind,OBJ_INDEX);
}

void Table::removeRule(const QString &nome)
{
 removeObject(nome,OBJ_RULE);
}

void Table::removeRule(unsigned idx_reg)
{
 removeObject(idx_reg,OBJ_RULE);
}

void Table::removeConstraint(const QString &nome)
{
 removeObject(nome,OBJ_CONSTRAINT);
}

void Table::removeConstraint(unsigned idx_const)
{
 removeObject(idx_const,OBJ_CONSTRAINT);
}

void Table::removeAncestorTable(const QString &nome)
{
 removeObject(nome,OBJ_TABLE);
}

void Table::removeAncestorTable(unsigned idx_tab)
{
 removeObject(idx_tab,OBJ_TABLE);
}

void Table::removeCopyTable(const QString &nome)
{
 removeObject(nome,BASE_TABLE);
}

void Table::removeCopyTable(unsigned idx_tab)
{
 removeObject(idx_tab,BASE_TABLE);
}

int Table::getObjectIndex(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 getObject(nome, tipo_obj, idx);
 return(idx);
}

int Table::getObjectIndex(TableObject *objeto)
{
 if(!objeto)
  return(-1);
 else
  return(getObjectIndex(objeto->getName(true), objeto->getObjectType()));
}

BaseObject *Table::getObject(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 return(getObject(nome, tipo_obj, idx));
}

BaseObject *Table::getObject(const QString &nome, ObjectType tipo_obj, int &idx_obj)
{
 BaseObject *objeto=NULL;
 bool enc=false, formatar=false;

 formatar=nome.contains("\"");

 //Caso o objeto a ser obtido seja uma coluna
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
    tipo_obj==OBJ_TRIGGER ||
    tipo_obj==OBJ_INDEX || tipo_obj==OBJ_RULE)
 {
  vector<TableObject *>::iterator itr, itr_end;
  vector<TableObject *> *lista_obj=NULL;
  int qtd;
  QString nome_aux=nome;

  qtd=nome_aux.count(QChar('\0'));
  if(qtd >=1) nome_aux.chop(qtd);

  lista_obj=getObjectList(tipo_obj);

  //Obtém as referências ao primeiro e ultimo elementos da lista
  itr=lista_obj->begin();
  itr_end=lista_obj->end();

  //Varre a lista de objetos
  while(itr!=itr_end)
  {
   //Verifica se o nome a ser encontrado é igual o nome do objeto atual
   enc=((*itr)->getName(formatar)==nome_aux);
   if(!enc) itr++; //Caso não seja encontrado passa para outro elemento da lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(enc)
  {
   //Define seu índice
   idx_obj=(itr-lista_obj->begin());

   //Armazena o objeto em um ponteiro que será retornado
   objeto=(*itr);
  }
  else idx_obj=-1;
 }

 //Caso o objeto a ser obtido seja uma tabela pai
 else if(tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
 {
  vector<Table *>::iterator itr_tab, itr_end_tab;
  vector<Table *> *tabelas;
  QString nome_aux;

  //Obrigatoriamente todo nome de tabela deve ser formatado por conta do esquema
  nome_aux=BaseObject::formatName(nome);

  if(tipo_obj==OBJ_TABLE)
   tabelas=&ancestor_tables;
  else
   tabelas=&copy_tables;

  //Obtém as referências ao primeiro e ultimo elementos da lista de tabelas descendentes
  itr_tab=tabelas->begin();
  itr_end_tab=tabelas->end();

  //Varre a lista de tabelas descendentes
  while(itr_tab!=itr_end_tab)
  {
   //Verifica se o nome a ser encontrado é igual o nome da tabela pai atual
   /* Diferente de outros tipos de objeto, as tabelas são sempre comparadas
      COM O NOME FORMATADO por precisam ser 'schema-qualified' evitando que
      uma tabelas de nomes iguais porém de esquemas diferentes sejam confundidas */
   enc=((*itr_tab)->getName(true)==nome_aux);
   if(!enc) itr_tab++; //Caso seja encontrado, interrompe a varredura pela lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(enc)
  {
   //Define seu índice
   idx_obj=(itr_tab-tabelas->begin());

   //Armazena o objeto em um ponteiro que será retornado
   objeto=(*itr_tab);
  }
 }
 else
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o objeto encontrado
 return(objeto);
}

BaseObject *Table::getObject(unsigned idx_obj, ObjectType tipo_obj)
{
 vector<TableObject *> *lista_obj=NULL;

 if(tipo_obj==OBJ_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= ancestor_tables.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela pai no índice especificado
  return(ancestor_tables[idx_obj]);
 }
 else if(tipo_obj==BASE_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= copy_tables.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela cópia no índice especificado
  return(copy_tables[idx_obj]);
 }
 else
 {
  /* Obtém um objeto de uma determinada lista de acordo com seu tipo
     e se seu índice seja válido */
  lista_obj=getObjectList(tipo_obj);
  if(idx_obj < lista_obj->size())
   return(lista_obj->at(idx_obj));
  else
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

Table *Table::getAncestorTable(const QString &nome)
{
 int idx;
 return(dynamic_cast<Table *>(getObject(nome,OBJ_TABLE,idx)));
}

Table *Table::getAncestorTable(unsigned idx_tab)
{
 return(dynamic_cast<Table *>(getObject(idx_tab,OBJ_TABLE)));
}

Column *Table::getColumn(const QString &nome, bool ref_nome_antigo)
{
 if(!ref_nome_antigo)
 {
  int idx;
  return(dynamic_cast<Column *>(getObject(nome,OBJ_COLUMN,idx)));
 }
 else
 {
  Column *coluna=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool enc=false, formatar=false;

  formatar=nome.contains("\"");
  itr=columns.begin();
  itr_end=columns.end();

  //Varre a lista de colunas buscando pelo nome antigo das mesmas
  while(itr!=itr_end && !enc)
  {
   coluna=dynamic_cast<Column *>(*itr);
   itr++;
   enc=(nome!="" && coluna->getOldName(formatar)==nome);
  }

  /* Caso nenhuma coluna for encontrada zera o ponteiro de coluna
     para forçar o retorno de uma coluna nula indicando que a
     busca não retornou objeto */
  if(!enc) coluna=NULL;
  return(coluna);
 }
}

Column *Table::getColumn(unsigned idx_col)
{
 return(dynamic_cast<Column *>(getObject(idx_col,OBJ_COLUMN)));
}

Trigger *Table::getTrigger(const QString &nome)
{
 int idx;
 return(dynamic_cast<Trigger *>(getObject(nome,OBJ_TRIGGER,idx)));
}

Trigger *Table::getTrigger(unsigned idx_gat)
{
 return(dynamic_cast<Trigger *>(getObject(idx_gat,OBJ_TRIGGER)));
}

Constraint *Table::getConstraint(const QString &nome)
{
 int idx;
 return(dynamic_cast<Constraint *>(getObject(nome,OBJ_CONSTRAINT,idx)));
}

Constraint *Table::getConstraint(unsigned idx_constr)
{
 return(dynamic_cast<Constraint *>(getObject(idx_constr,OBJ_CONSTRAINT)));
}

Index *Table::getIndex(const QString &nome)
{
 int idx;
 return(dynamic_cast<Index *>(getObject(nome,OBJ_INDEX,idx)));
}

Index *Table::getIndex(unsigned idx_ind)
{
 return(dynamic_cast<Index *>(getObject(idx_ind,OBJ_INDEX)));
}

Rule *Table::getRule(const QString &nome)
{
 int idx;
 return(dynamic_cast<Rule *>(getObject(nome,OBJ_RULE,idx)));
}

Rule *Table::getRule(unsigned idx_reg)
{
 return(dynamic_cast<Rule *>(getObject(idx_reg,OBJ_RULE)));
}

Table *Table::getCopyTable(const QString &nome)
{
 int idx;
 return(dynamic_cast<Table *>(getObject(nome,BASE_TABLE,idx)));
}

Table *Table::getCopyTable(unsigned idx_tab)
{
 return(dynamic_cast<Table *>(getObject(idx_tab,BASE_TABLE)));
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

unsigned Table::getObjectCount(ObjectType tipo_obj, bool inc_insporrelacao)
{
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
    tipo_obj==OBJ_TRIGGER ||/* tipo_obj==OBJETO_GAT_RESTRICAO ||*/
    tipo_obj==OBJ_INDEX || tipo_obj==OBJ_RULE ||
    tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
 {
  if(tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
  {
   if(tipo_obj==OBJ_TABLE)
    return(ancestor_tables.size());
   else
    return(copy_tables.size());
  }
  else
  {
   vector<TableObject *> *lista=NULL;
   lista=getObjectList(tipo_obj);

   //Faz a contagem apenas dos objetos que não são auto-incluidos (incluídos por relacionamento)
   if(!inc_insporrelacao)
   {
    vector<TableObject *>::iterator itr, itr_end;
    unsigned qtd=0;

    itr=lista->begin();
    itr_end=lista->end();
    while(itr!=itr_end)
    {
     if(!(*itr)->isAddedByRelationship()) qtd++;
     itr++;
    }

    return(qtd);
   }
   else
    return(lista->size());
  }
 }
 else
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Table::getPrimaryKey(void)
{
 unsigned qtd,i;
 Constraint *pk=NULL, *rest=NULL;

 qtd=constraints.size();
 for(i=0; i < qtd && !pk; i++)
 {
  rest=dynamic_cast<Constraint *>(constraints[i]);
  pk=(rest->getConstraintType()==ConstraintType::primary_key ? rest : NULL);
 }

 return(pk);
}

void Table::getForeignKeys(vector<Constraint *> &vet_fks, bool inc_insporrelacao)
{
 unsigned qtd,i;
 Constraint *constr=NULL;

 qtd=constraints.size();
 for(i=0; i < qtd; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);

  /* Adiciona a restrição caso seja chave estrangeira, caso esta não foi
     incluída por relacionamento ou se foi e o parametro inc_insporrelacao está
     setado */
  if(constr->getConstraintType()==ConstraintType::foreign_key &&
     (!constr->isAddedByLinking() ||
      (constr->isAddedByLinking() && inc_insporrelacao)))
   vet_fks.push_back(constr);
 }
}

bool Table::isWithOIDs(void)
{
 return(with_oid);
}

bool Table::isReferTableOnForeignKey(Table *tab_ref)
{
 unsigned qtd,i;
 Constraint *constr=NULL;
 bool found=false;

 qtd=constraints.size();
 for(i=0; i < qtd && !found; i++)
 {
  constr=dynamic_cast<Constraint *>(constraints[i]);
  found=(constr->getConstraintType()==ConstraintType::foreign_key &&
         !constr->isAddedByLinking() &&
          constr->getReferencedTable() == tab_ref);
 }

 return(found);
}

bool Table::isConstraintRefColumn(Column *coluna, ConstraintType tipo_rest)
{
 bool enc=false;
 vector<TableObject *>::iterator itr, itr_end;
 Constraint *rest=NULL;

 if(coluna)
 {
  itr=constraints.begin();
  itr_end=constraints.end();
  while(itr!=itr_end && !enc)
  {
   rest=dynamic_cast<Constraint *>(*itr);
   itr++;
   enc=(rest->getConstraintType()==tipo_rest &&
        (rest->isColumnExists(coluna, Constraint::SOURCE_COLS) ||
         rest->isColumnExists(coluna, Constraint::REFERENCED_COLS)));
  }
 }

 return(enc);
}

QString Table::getCodeDefinition(unsigned tipo_def)
{
 attributes[ParsersAttributes::OIDS]=(with_oid ? "1" : "");
 setColumnsAttribute(tipo_def);
 setConstraintsAttribute(tipo_def);
 setTriggersAttribute(tipo_def);
 setIndexesAttribute(tipo_def);
 setRulesAttribute(tipo_def);

 if(tipo_def==SchemaParser::XML_DEFINITION)
  setPositionAttribute();

 return(BaseObject::__getCodeDefinition(tipo_def));
}

void Table::operator = (Table &tabela)
{
 QString nome_ant = this->getName(true);

 (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tabela);
 this->with_oid=tabela.with_oid;
 setProtected(tabela.is_protected);

 PgSQLType::renameUserType(nome_ant, this, this->getName(true));
}

bool Table::isReferRelationshipAddedObject(void)
{
 vector<TableObject *>::iterator itr, itr_end;
 ObjectType tipos[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
 bool enc=false;

 for(unsigned i=0; i < 2 && !enc; i++)
 {
  //Obtém a lista de tipos atual
  itr=getObjectList(tipos[i])->begin();
  itr_end=getObjectList(tipos[i])->end();

  /* Efetua uma iteração verifica se os objetos da lista
     atual foram incluídas por relacionamento, caso uma objeto
     for detectada como incluída desta forma é suficiente
     dizer que a tabela referencia um objeto vindo de
     relacionamento */
  while(itr!=itr_end && !enc)
  {
   //Obtém se a coluna foi incluída por relacionamento ou não
   enc=(*itr)->isAddedByRelationship();
   //Passa para a próxima coluna
   itr++;
  }
 }

 return(enc);
}

void Table::swapObjectsIndexes(ObjectType tipo_obj, unsigned idx1, unsigned idx2)
{
 vector<TableObject *> *lista_obj=NULL;
 vector<TableObject *>::iterator itr1, itr2;
 TableObject *obj_aux=NULL;

 try
 {
  if(idx1!=idx2)
  {
   lista_obj=getObjectList(tipo_obj);

   if(idx1 >= lista_obj->size() || idx2 >= lista_obj->size())
    throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   obj_aux=lista_obj->at(idx1);
   itr1=lista_obj->begin() + idx1;
   itr2=lista_obj->begin() + idx2;

   (*itr1)=(*itr2);
   (*itr2)=obj_aux;
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Table::getColumnReferences(Column *coluna, vector<TableObject *> &vet_refs, bool modo_exclusao)
{
 /* Caso a coluna não foi incluída por relacionamento, será verificado se esta
    não está sendo referenciada. Para colunas adicionadas automaticamente por
    relacionamentos, sua remoção é feita sem verificação alguma */
 if(coluna && !coluna->isAddedByRelationship())
 {
  unsigned qtd, i;
  IndexElement elem;
  Column *col=NULL, *col1=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool enc=false;
  Index *ind=NULL;
  Constraint *rest=NULL;
  Trigger *gat=NULL;

  //Verificando se a coluna não é referenciada nos indices
  itr=indexes.begin();
  itr_end=indexes.end();

  while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   ind=dynamic_cast<Index *>(*itr);
   itr++;

   qtd=ind->getElementCount();
   for(i=0; i < qtd  && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    elem=ind->getElement(i);
    col=elem.getColumn();
    if(col && col==coluna)
    {
     enc=true;
     vet_refs.push_back(ind);
    }
   }
  }

  //Verificando se a coluna não é referenciada nas constraints
  itr=constraints.begin();
  itr_end=constraints.end();

  while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   rest=dynamic_cast<Constraint *>(*itr);
   itr++;

   col=rest->getColumn(coluna->getName(),true);
   col1=rest->getColumn(coluna->getName(),false);

   if((col && col==coluna) || (col1 && col1==coluna))
   {
    enc=true;
    vet_refs.push_back(rest);
   }
  }

  //Verificando se a coluna não é referenciada nos gatilhos
  itr=triggers.begin();
  itr_end=triggers.end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   gat=dynamic_cast<Trigger *>(*itr);
   itr++;

   qtd=gat->getColumnCount();
   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    if(gat->getColumn(i)==coluna)
    {
     enc=true;
     vet_refs.push_back(gat);
    }
   }
  }
 }
}

