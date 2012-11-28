#include "relationship.h"
#include <QApplication>

//Inicialização de atributos estáticos da classe
const QString Relationship::SUFFIX_SEPARATOR("_");

Relationship::Relationship(Relationship *rel) : BaseRelationship(rel)
{
 if(!rel)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 (*(this))=(*rel);
}

Relationship::Relationship(unsigned rel_type, Tabela *src_tab,
                           Tabela *dst_tab, bool src_mdtry, bool dst_mdtry,
                           bool auto_suffix, const QString &src_suffix, const QString &dst_suffix,
                           bool identifier,  bool deferrable, TipoPostergacao deferral_type) :
              BaseRelationship(rel_type, src_tab, dst_tab, src_mdtry, dst_mdtry)
{
 try
 {
  obj_type=OBJ_RELATIONSHIP;
  QString str_aux;

  if(((rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N) &&
      !this->getReferenceTable()->obterChavePrimaria()) ||
     (rel_type==RELATIONSHIP_NN && (!src_tab->obterChavePrimaria() || !dst_tab->obterChavePrimaria())))
     throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                           .arg(QString::fromUtf8(obj_name))
                           .arg(QString::fromUtf8(src_tab->getName(true)))
                           .arg(QString::fromUtf8(dst_tab->getName(true))),
                  ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Atribuindo os sufixos ao relacionamento.
    Sufixos são palavras concatenadas ao final do nome de
    colunas as quais são adicionadas automaticamente nas tabelas
    pelo relacionamento e que participam de chaves estrangeiras */
  this->src_suffix=src_suffix;
  this->dst_suffix=dst_suffix;
  this->auto_suffix=auto_suffix;

  table_relnn=NULL;
  fk_rel1n=pk_relident=pk_special=uq_rel11=NULL;
  this->deferrable=deferrable;
  this->deferral_type=deferral_type;
  this->invalidated=true;

  //Configura o nome do relacionamento conforme o tipo
  if(rel_type==RELATIONSHIP_11)
   str_aux=QApplication::translate("Relacionamento","%1_has_one_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_1N)
   str_aux=QApplication::translate("Relacionamento","%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_NN)
   str_aux=QApplication::translate("Relacionamento","many_%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_GEN)
   str_aux=QApplication::translate("Relacionamento","%1_inherits_%2","",QApplication::UnicodeUTF8);
  else
   str_aux=QApplication::translate("Relacionamento","%1_copies_%2","",QApplication::UnicodeUTF8);

  if(rel_type!=RELATIONSHIP_NN)
   str_aux=str_aux.arg(this->getReferenceTable()->getName())
                     .arg(this->getReceiverTable()->getName());
  else
   str_aux=str_aux.arg(this->src_table->getName())
                  .arg(this->dst_table->getName());

  setName(str_aux);

  /* Caso a os sufixos estejam especificados o nome da tabela será
     a junção dos sufixos separados pelo separador de sufixos. Caso
     contrário o nome da tabela será o próprio nome do relacionamento */
  if(src_suffix!="" && dst_suffix!="")
   tab_name_relnn=src_suffix + SUFFIX_SEPARATOR + dst_suffix;
  else
   tab_name_relnn=this->obj_name;

  rejected_col_count=0;
  setIdentifier(identifier);

  attributes[ParsersAttributes::CONSTRAINTS]="";
  attributes[ParsersAttributes::TABLE]="";
  attributes[ParsersAttributes::RELATIONSHIP_NN]="";
  attributes[ParsersAttributes::RELATIONSHIP_GEN]="";
  attributes[ParsersAttributes::RELATIONSHIP_1N]="";
  attributes[ParsersAttributes::ANCESTOR_TABLE]="";
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

vector<QString> Relationship::getRelationshipColumns(void)
{
 unsigned count, i;
 vector<QString> name_vect;

 count=ref_columns.size();
 for(i=0; i < count; i++)
 {
  name_vect.push_back(QString::fromUtf8(ref_columns[i]->getName()) + " (" +
                      QString::fromUtf8(*ref_columns[i]->getType()) + ")");
 }

 return(name_vect);
}

void Relationship::setMandatoryTable(unsigned table_id, bool value)
{
 BaseRelationship::setMandatoryTable(table_id, value);
 this->invalidated=true;
}

void Relationship::setTableSuffix(unsigned table_id, const QString &suffix)
{
 if(table_id > DST_TABLE)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(!suffix.isEmpty() && !BaseObject::isValidName(suffix))
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_SUFFIX_REL)
                .arg(QString::fromUtf8(this->getName())),
                ERR_ASG_INV_SUFFIX_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(table_id==SRC_TABLE)
  src_suffix=suffix;
 else
  dst_suffix=suffix;

 this->invalidated=true;
}

QString Relationship::getTableSuffix(unsigned table_id)
{
 if(table_id > DST_TABLE)
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(table_id==SRC_TABLE)
  return(src_suffix);
 else
  return(dst_suffix);
}

void Relationship::setDeferrable(bool value)
{
 deferrable=value;
 this->invalidated=true;
}

void Relationship::setIdentifier(bool value)
{
 /* Validando o relacionamento identificador.
    Relacionamento identificador não pode ser criado quando este é um
    autorelacionamento, relacionamento n-n, de generalização, de dependência
    ou do tipo criado pelo próprio usuário. */
 if(value &&
   (src_table==dst_table ||
    (rel_type==RELATIONSHIP_NN ||
     rel_type==RELATIONSHIP_GEN ||
     rel_type==RELATIONSHIP_DEP)))
  throw Exception(ERR_INV_IDENT_RELATIOSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 identifier=value;
 this->invalidated=true;
}

void Relationship::setSpecialPrimaryKeyCols(vector<unsigned> &cols)
{
 /* Dispara um erro caso o usuário tente usar a chave primária especial em autorelacionamento
    e/ou relacionamento n-n */
 if(isSelfRelationship() || isIdentifier() || rel_type==RELATIONSHIP_NN)
  throw Exception(Exception::getErrorMessage(ERR_INV_USE_ESPECIAL_PK)
                .arg(QString::fromUtf8(this->getName())),
                ERR_INV_USE_ESPECIAL_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->column_ids_pk_rel=cols;
}

vector<unsigned> Relationship::getSpecialPrimaryKeyCols(void)
{
 return(this->column_ids_pk_rel);
}

void Relationship::createSpecialPrimaryKey(void)
{
 if(!column_ids_pk_rel.empty())
 {
  unsigned i, count;

  /* Aloca a chave primária com as seguintes características:
     1) A mesma é marcada como protegida e incluída por relacionamento, desta
        forma ela é identificada únicamente nas operações internas do relacionamento

     2) O espaço de tabelas usado na restrição é o mesmo da tabela receptora */
  pk_special=new Constraint;
  pk_special->setName(this->getName() + QString("_pk"));
  pk_special->setConstraintType(TipoRestricao::primary_key);
  pk_special->setAddedByLinking(true);
  pk_special->setProtected(true);
  pk_special->setTablespace(dynamic_cast<Tablespace *>(getReceiverTable()->getTablespace()));

  //Adiciona as colunas   chave primária obtendo-as através dos seus índices armazenados em 'id_colunas_pk_rel'
  count=column_ids_pk_rel.size();
  for(i=0; i < count; i++)
  {
   if(column_ids_pk_rel[i] < ref_columns.size() &&
      !pk_special->isColumnExists(ref_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS))
    pk_special->addColumn(ref_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS);
  }

  try
  {
   //Tenta adicionar a restrição ao relacionamento
   this->addObject(pk_special);
  }
  catch(Exception &e)
  {
   /* Caso algum erro for detectado a restrição é removida mas o relacionamento não é invalidado
      só será criado sem a chave primária especial */
   delete(pk_special);
   pk_special=NULL;
  }
 }
}

void Relationship::setTableNameRelNN(const QString &name)
{
 if(!BaseObject::isValidName(name))
  throw Exception(ERR_ASG_INV_NAME_TABLE_RELNN, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 tab_name_relnn=name;
 this->invalidated=true;
}

QString Relationship::getTableNameRelNN(void)
{
 return(tab_name_relnn);
}

bool Relationship::isDeferrable(void)
{
 return(deferrable);
}

void Relationship::setDeferralType(TipoPostergacao defer_type)
{
 deferral_type=defer_type;
 this->invalidated=true;
}

TipoPostergacao Relationship::getDeferralType(void)
{
 return(deferral_type);
}

int Relationship::getObjectIndex(TableObject *object)
{
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *list=NULL;
 TableObject *obj_aux=NULL;
 ObjectType obj_type;
 bool found=false;

 //Dispara uma exceção caso o objeto a ser buscado não esteja alocado
 if(!object)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 obj_type=object->getObjectType();
 if(obj_type==OBJ_COLUMN)
  list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=list->begin();
 itr_end=list->end();

 /* Varre a lista de objetos selecionada verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista ou mesmo
    se os endereços dos objetos envolvidos são iguais */
 while(itr!=itr_end && !found)
 {
  obj_aux=(*itr);
  found=(obj_aux==object || obj_aux->getName()==object->getName());
  itr++;
 }

 if(found)
  return((itr-list->begin())-1);
 else
  return(-1);
}

bool Relationship::isColumnExists(Column *column)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col_aux=NULL;
 bool found=false;

 //Caso a coluna a ser buscada não esteja aloca, dispara uma exceção
 if(!column)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=ref_columns.begin();
 itr_end=ref_columns.end();

 /* Varre a lista de colunas selecionada verificando se o nome da
    coluna é igual ao nome de uma das colunas da lista ou mesmo
    se os endereços das colunas envolvidas são iguais */
 while(itr!=itr_end && !found)
 {
  col_aux=(*itr);
  found=(col_aux==column || col_aux->getName()==column->getName());
  itr++;
 }

 return(found);
}

void Relationship::addObject(TableObject *tab_obj, int obj_idx)
{
 ObjectType obj_type;
 vector<TableObject *> *obj_list=NULL;

 /* Somente a chave primária especial (criada pelo relacionamento)
    só pode ser adicionado a um relacionamento de generalização ou dependência.
    Caso o tipo do objeto a ser adionado não obedeça a esta condição um erro é retornado */
 if((rel_type==RELATIONSHIP_GEN ||
     rel_type==RELATIONSHIP_DEP) &&
    !(tab_obj->isAddedByRelationship() &&
      tab_obj->isProtected() &&
      tab_obj->getObjectType()==OBJ_CONSTRAINT))
  throw Exception(ERR_ASG_OBJ_INV_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 try
 {
  /* Verifica se o objeto já não foi inserido.
     Um atributo/restrição de um relacionamento não pode
     ser atribuido ao relacionamento caso este já pertença a uma tabela,
     caso isso aconteça o método aborta a inserção do objeto */
  if(!tab_obj->getParentTable() &&
     getObjectIndex(tab_obj) < 0)
  {
   /* Obtém a lista de objetos de acordo com o tipo
      do objeto a ser inserido */
   obj_type=tab_obj->getObjectType();
   if(obj_type==OBJ_COLUMN)
    obj_list=&rel_attributes;
   else if(obj_type==OBJ_CONSTRAINT)
    obj_list=&rel_constraints;

   /* Tenta gerar a definição SQL do objeto para ver se o mesmo
      está bem configurado. Caso não esteja, uma exceção será
      disparada e o objeto não será inserido na lista */

   /* Como atributos recém criados não possuem uma tabela pai até que sejam
      adicionados   tabela receptora, para medidas de validação do código do atributo,
      a tabela de origem do relacionamento é atribuída como tabela pai */
   tab_obj->setParentTable(src_table);

   if(obj_type==OBJ_COLUMN)
    dynamic_cast<Column *>(tab_obj)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
   else
   {
    Constraint *rest=NULL;
    rest=dynamic_cast<Constraint *>(tab_obj);
    rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);

    /* Caso se tente inserir uma chave estrangeira como restrição do relacionamento
       retorna um erro pois este é o único tipo que não pode ser incluído */
    if(rest->getConstraintType()==TipoRestricao::foreign_key)
     throw Exception(ERR_ASG_FOREIGN_KEY_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Após a validação do novo objeto a tabela pai é setada como nula
   tab_obj->setParentTable(NULL);

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

   /* Apenas para os relacinamentos n-n os atributos/restrições
      não são marcados como "incluídos por ligação" pois estes atributos
      e restrições são aproveitados quando o relacionamento n-n é convertido
      em tabela */
   //if(tipo_relac!=RELACIONAMENTO_NN)
   tab_obj->setAddedByLinking(true);

   this->invalidated=true;
  }
  else
   throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                 .arg(tab_obj->getName(true))
                 .arg(tab_obj->getTypeName())
                 .arg(this->getName(true))
                 .arg(this->getTypeName()),
                 ERR_ASG_DUPLIC_OBJECT, __PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 catch(Exception &e)
 {
  /* Caso a exceção capturada seja de atributo não preenchido, indice que
     a definição SQL do objeto está incompleta */
  if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(tab_obj->getName()))
                              .arg(tab_obj->getTypeName()),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void Relationship::removeObjects(void)
{
 rel_attributes.clear();
 rel_constraints.clear();
}

void Relationship::destroyObjects(void)
{
 while(!rel_constraints.empty())
 {
  delete(rel_constraints.back());
  rel_constraints.pop_back();
 }

 while(!rel_attributes.empty())
 {
  delete(rel_attributes.back());
  rel_attributes.pop_back();
 }
}

void Relationship::removeObject(unsigned obj_id, ObjectType obj_type)
{
 vector<TableObject *> *obj_list=NULL;

 //Seleciona a lista de objetos de acordo com o tipo passado
 if(obj_type==OBJ_COLUMN)
  obj_list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  obj_list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Se o índice do objeto for inválido, dispara o erro
 if(obj_id >= obj_list->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Verificação específica para coluna. Caso a coluna esteja sendo
    referenciada por uma restrição do relacionamento será disparado
    um erro. */
 if(obj_type==OBJ_COLUMN)
 {
  Column *col=NULL;
  Constraint *constr=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool refer=false;

  itr=rel_constraints.begin();
  itr_end=rel_constraints.end();
  col=dynamic_cast<Column *>(obj_list->at(obj_id));

  while(itr!=itr_end && !refer)
  {
   constr=dynamic_cast<Constraint *>(*itr);
   //Verifica se a coluna está em uma das listas das restições
   refer=(constr->getColumn(col->getName(), Constraint::SOURCE_COLS) ||
          constr->getColumn(col->getName(), Constraint::REFERENCED_COLS));
   itr++;
  }

  //Caso haja referência
  if(refer)
   throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                           .arg(QString::fromUtf8(col->getName()))
                           .arg(col->getTypeName())
                           .arg(QString::fromUtf8(constr->getName()))
                           .arg(constr->getTypeName())
                           .arg(QString::fromUtf8(this->getName(true)))
                           .arg(this->getTypeName()),
                 ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 /* Antes da remoção de qualquer objeto é necessário desconectar
    o relacionamento pois o objeto a ser removido pode estar sendo
    referenciado por outros objetos */
 disconnectRelationship(false);

 //Remove o item da lista
 obj_list->erase(obj_list->begin() + obj_id);

 //Reconecta o relacionamento
 connectRelationship();
}

void Relationship::removeObject(TableObject *object)
{
 if(!object)
  throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 removeObject(getObjectIndex(object),object->getObjectType());
}

void Relationship::removeAttribute(unsigned attrib_idx)
{
 removeObject(attrib_idx, OBJ_COLUMN);
}

void Relationship::removeConstraint(unsigned constr_idx)
{
 removeObject(constr_idx, OBJ_CONSTRAINT);
}

Column *Relationship::getReferencedColumn(const QString &col_name)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col=NULL;
 bool found=false, format;

 format=col_name.contains("\"");
 itr=ref_columns.begin();
 itr_end=ref_columns.end();

 /* Varre a lista colunas referenciadas verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista */
 while(itr!=itr_end && !found)
 {
  col=(*itr);
  found=(col->getName(format)==col_name);
  itr++;
 }

 if(found)
  return(col);
 else
  return(NULL);
}

TableObject *Relationship::getObject(unsigned obj_idx, ObjectType obj_type)
{
 vector<TableObject *> *list=NULL;

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 if(obj_type==OBJ_COLUMN)
  list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(obj_idx >= list->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(list->at(obj_idx));
}

TableObject *Relationship::getObject(const QString &name, ObjectType obj_type)
{
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *list=NULL;
 TableObject *obj_aux=NULL;
 bool found=false;

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 if(obj_type==OBJ_COLUMN)
  list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=list->begin();
 itr_end=list->end();

 /* Varre a lista de objetos selecionada verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista ou mesmo
    se os endereços dos objetos envolvidos são iguais */
 while(itr!=itr_end && !found)
 {
  obj_aux=(*itr);
  found=(obj_aux->getName()==name);
  itr++;
 }

 if(found)
  return(obj_aux);
 else
  return(NULL);
}

Column *Relationship::getAttribute(unsigned attrib_idx)
{
 /* Caso o índice do atributo esteja fora da quantidade da lista de
    atributos dispara uma exceção */
 if(attrib_idx >= rel_attributes.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(dynamic_cast<Column *>(rel_attributes[attrib_idx]));
}

Column *Relationship::getAttribute(const QString &name)
{
 return(dynamic_cast<Column *>(getObject(name,OBJ_COLUMN)));
}

Constraint *Relationship::getConstraint(unsigned constr_idx)
{
 /* Caso o índice da restrição esteja fora da quantidade da lista de
    restrições dispara uma exceção */
 if(constr_idx >= rel_constraints.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(dynamic_cast<Constraint *>(rel_constraints[constr_idx]));
}

Constraint *Relationship::getConstraint(const QString &name)
{
 return(dynamic_cast<Constraint *>(getObject(name,OBJ_CONSTRAINT)));
}

unsigned Relationship::getAttributeCount(void)
{
 return(rel_attributes.size());
}

unsigned Relationship::getConstraintCount(void)
{
 return(rel_constraints.size());
}

unsigned Relationship::getObjectCount(ObjectType obj_type)
{
 if(obj_type==OBJ_COLUMN)
  return(rel_attributes.size());
 else if(obj_type==OBJ_CONSTRAINT)
  return(rel_constraints.size());
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Relationship::addConstraints(Tabela *dst_table)
{
 Constraint *constr=NULL, *pk=NULL;
 unsigned constr_id, constr_cnt, i, count;
 QString name, orig_name, aux;

 try
 {
  constr_cnt=rel_constraints.size();

  //Varre a lista de restrições do relacionamento
  for(constr_id=0; constr_id < constr_cnt; constr_id++)
  {
   //Obtém a restrição
   constr=dynamic_cast<Constraint *>(rel_constraints[constr_id]);

   /* Interrompe o processamento caso a restrição já
      tenha sido incluída a uma das tabelas */
   if(constr->getParentTable())
    break;

   /* Caso ela não seja uma chave primária, a mesma é inserida
      na tabela */
   if(constr->getConstraintType()!=TipoRestricao::primary_key)
   {
    i=1; aux[0]='\0';
    name="";
    //Configura o nome da restrição a fim de resolver problemas de duplicidade
    orig_name=constr->getName();
    while(dst_table->obterRestricao(orig_name + aux))
    {
     aux=QString("%1").arg(i);
     name=orig_name + aux;
     i++;
    }

    if(name!="") constr->setName(name);

    //Adiciona a restrição na tabela
    dst_table->adicionarRestricao(constr);
   }
   else
   {
    /* Caso a restição seja uma chave primária, esta será fundida com a
       chave primária da tabela. */

    //Obtém a chave primária da tabela
    pk=dst_table->obterChavePrimaria();

    //Caso ela exista será executada a fusão
    if(pk)
    {
     //Obtém a quantidade de colunas da restrição
     count=constr->getColumnCount(Constraint::SOURCE_COLS);

     for(i=0; i < count; i++)
      //Adiciona cada coluna da restrição na chave primária da tabela
      pk->addColumn(constr->getColumn(i, Constraint::SOURCE_COLS),
                          Constraint::SOURCE_COLS);
    }
    else
     /* Caso a tabela não possua uma chave primária, a própria restrição do relacionamento
        será a chave primária da tabela */
     dst_table->adicionarRestricao(constr);

    //Remove a chave primária especial da lista de restrições
    if(constr==pk_special)
    {
     rel_constraints.erase(rel_constraints.begin()+constr_id);
     constr_cnt=rel_constraints.size();
    }
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addColumnsRelGen(void)
{
 Tabela *tab_orig=NULL, *tab_dest=NULL,
        *tab_pai=NULL, *tab_aux=NULL;
 Column *col_orig=NULL, *col_dest=NULL,
        *coluna=NULL, *col_aux=NULL;
 unsigned qtd_orig, qtd_dest,
          i, i1, i2, id_tab,
          idx, qtd_tab;
 vector<Column *> colunas;
 ObjectType tipos[2]={OBJ_TABLE, BASE_TABLE};
 ErrorType tipo_erro=ERR_CUSTOM;
 bool duplic=false, cond,
      /* 0 -> Coluna vinda de herança
         1 -> Coluna vinda de cópia */
      flags_orig[2]={false,false},
      flags_dest[2]={false,false};
 QString str_aux, msg;
 TipoPgSQL tipo_orig, tipo_dest;

 try
 {
  tab_orig=dynamic_cast<Tabela *>(src_table);
  tab_dest=dynamic_cast<Tabela *>(dst_table);

  //Obtém o número de colunas de ambas as tabelas
  qtd_orig=tab_orig->obterNumColunas();
  qtd_dest=tab_dest->obterNumColunas();
  rejected_col_count=0;

  /* Este for compara cada coluna da tabela de destino com
     cada coluna da tabela de origem */
  for(i=0; i < qtd_dest && tipo_erro==ERR_CUSTOM; i++)
  {
   //Obtém uma coluna do destino
   col_dest=tab_dest->obterColuna(i);
   /* Obtém o seu tipo e o converte para 'integer' caso seja
      serial, pois em caso de se comparar um 'serial' de um coluna
      com o integer de outra, pode-se gerar um erro, mesmo estes
      tipos sendo compatíveis */
   tipo_dest=col_dest->getType();
   if(tipo_dest=="serial") tipo_dest="integer";
   else if(tipo_dest=="bigserial") tipo_dest="bigint";

   /* Este flag indica que a coluna de uma tabela possui o
      mesmo nome de uma coluna da outra */
   duplic=false;

   /* Com a coluna de destino obtida, será feita uma comparação com
      todas as colunas da origem */
   for(i1=0; i1 < qtd_orig && !duplic; i1++)
   {
    //Obtém uma coluna da origem e converte seu tipo
    col_orig=tab_orig->obterColuna(i1);
    tipo_orig=col_orig->getType();
    if(tipo_orig=="serial") tipo_orig="integer";
    else if(tipo_orig=="bigserial") tipo_orig="bigint";

    //Compara o nome das duas colunas
    duplic=(col_orig->getName()==col_dest->getName());

    //Caso haja duplicidade de nomes
    if(duplic)
    {
     /* É necessário verificar se a coluna de origem é da própria tabela,
        se veio de uma tabela pai ou de uma tabela de cópia. A mesma
        verificação é feita na coluna de destino.

        A duplicidade de colunas só gera erro quando a coluna de origem é
        da própria tabela e a coluna de destino não veio de uma tabela pai
        da tabela de destino no caso de um relacionamento de dependência.

        Caso a coluna de origem seja da tabela de origem ou vinda de um
        relacionamento de dependência e o tipo do relacionamento atual seja
        de herança, o único caso em que a duplicidade gera erro é a incompatibilidade
        dos tipos das colunas envolvidas, caso contrário as mesmas são fundidas. */
     for(id_tab=0; id_tab < 2; id_tab++)
     {
      if(id_tab==0)
      {
       /* Selecionando a coluna de origem e a tabela
          de origem para fazer a comparação descrita acima */
       col_aux=col_orig;
       tab_aux=tab_orig;
      }
      else
      {
       /* Selecionando a coluna de destino e a tabela
          de destino para fazer a comparação descrita acima */
       col_aux=col_dest;
       tab_aux=tab_dest;
      }

      /* Varre a lista de tabelas pai (tipos[0]) e de
         cópia tipos[1] da origem e destino */
      for(i2=0; i2 < 2; i2++)
      {
       /* Obtém o número de objetos do tipo OBJETO_TABELA (tabela pai) ou
          OBJETO_TABELA_BASE (tabela cópia), da tabela selecionada acima */
       qtd_tab=tab_aux->obterNumObjetos(tipos[i2]);

       /* Este for obtém cada tabela da lista e verifica se a coluna auxiliar
          existe na lista de colunas de tal tabela, isso é suficiente para
          se saber se a coluna auxiliar veio de uma relacionamento de herança
          ou de dependência */
       for(idx=0; idx < qtd_tab; idx++)
       {
        tab_pai=dynamic_cast<Tabela *>(tab_aux->obterObjeto(idx, tipos[i2]));
        //Verifica se a coluna existe na tabela obtida
        cond=(tab_pai->obterColuna(col_aux->getName()));

        /* Caso o id_tab==0, indica que desejamos atribuir o resultado
           da comparação acima ao vetor de flags na posição relativa
           ao relacionamento de herança que indica que a coluna auxiliar
           existe ou não na tabela pai */
        if(id_tab==0)
         flags_orig[i2]=cond;
        else
        /* Caso o id_tab==1, indica que desejamos atribuir o resultado
           da comparação acima ao vetor de flags na posição relativa
           ao relacionamento de dependência que indica que a coluna auxiliar
           existe ou não na tabela cópia */
         flags_dest[i2]=cond;
       }
      }
     }

     /* Condição de erro 1: O tipo de relacionamento é de dependência
        e a coluna de origem é da própria tabela ou vinda de uma tabela cópia
        e a coluna de destino é da própria tabela de destino ou veio de uma
        tabela de cópia da própria tabela de destino */
     if(rel_type==RELATIONSHIP_DEP &&

       ((!flags_orig[0] && !flags_orig[1]) ||
        (!flags_orig[0] &&  flags_orig[1])) &&

       ((!flags_dest[0] && !flags_dest[1]) ||
        (!flags_dest[0] &&  flags_dest[1])))
     {
      tipo_erro=ERR_DUPLIC_COLS_COPY_REL;
     }
     /* Condição de erro 2: O tipo de relacionamento é de generalização e o tipo
        das colunas são incompatíveis */
     else if(rel_type==RELATIONSHIP_GEN &&
             tipo_orig!=tipo_dest)
      tipo_erro=ERR_INCOMP_COLS_INHERIT_REL;
    }
   }

   //Caso não foi detectado nenhum erro
   if(tipo_erro==ERR_CUSTOM)
   {
    //Caso não haja duplicidade
    if(!duplic)
    {
     //Cria uma nova coluna e faz as configurações iniciais
     coluna=new Column;

     (*coluna)=(*col_dest);

     if(rel_type==RELATIONSHIP_GEN)
      coluna->setAddedByGeneralization(true);
     else
      coluna->setAddedByCopy(true);

     coluna->setParentTable(NULL);

     //Converte seu tipo de '' para 'integer', se necessário
     if(coluna->getType()=="serial")
      coluna->setType(TipoPgSQL("integer"));
     else if(coluna->getType()=="bigserial")
      coluna->setType(TipoPgSQL("bigint"));

     //Adiciona a nova coluna   lista temporária de colunas
     colunas.push_back(coluna);
    }
    else
     /* Caso haja duplicidade, a coluna é rejeitada e não incluída na lista,
        ao invés disso, incrementa o atributo o qual contabiliza a quantidade
        de colunas duplicadas as quais foram rejeitadas por já existirem
        na tabela de destino */
     rejected_col_count++;
   }
  }

  //Caso nenhum erro de duplicidade foi detectado
  if(tipo_erro==ERR_CUSTOM)
  {
   vector<Column *>::iterator itr, itr_end;

   /* As colunas da lista temporária serão inseridas
      na lista de colunas de referência, do relacionamento e além disso
      as colunas também serão inseridas diretamente na tabela de origem
      do relacionamento, a qual herda ou copia as colunas da tabela
      de destino */
   ref_columns=colunas;
   itr=ref_columns.begin();
   itr_end=ref_columns.end();
   while(itr!=itr_end)
   {
    tab_orig->adicionarColuna((*itr));
    itr++;
   }
  }
  else
  {
   /* Caso haja algum erro de duplicidade, a lista temporária de
      colunas é destruida */
   while(!colunas.empty())
   {
    delete(colunas.back());
    colunas.pop_back();
   }

   /* Obtém a mensagem de erro que será mostarada ao usuário de acordo com o
      tipo de erro de duplicidade identificado */
   str_aux=Exception::getErrorMessage(tipo_erro);

   //Formata a mensagem de erro de acordo com o tipo do erro
   if(tipo_erro==ERR_DUPLIC_COLS_COPY_REL)
   {
    msg=QString(str_aux)
        .arg(col_dest->getName())
        .arg(tab_dest->getName())
        .arg(tab_orig->getName());
   }
   else
   {
    msg=QString(str_aux)
        .arg(col_dest->getName())
        .arg(tab_dest->getName())
        .arg(col_orig->getName())
        .arg(tab_orig->getName());
   }

   //Dispara a exeção acusando a duplicidade
   throw Exception(msg, tipo_erro,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Cria a chave primária especial se houve
  this->createSpecialPrimaryKey();
  this->addConstraints(getReceiverTable());
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::connectRelationship(void)
{
 try
 {
  if(!connected)
  {
   if(rel_type==RELATIONSHIP_GEN)
   {
    /* Definindo que a tabela de destino é a tabela pai
       da tabela de origem como indicado pelo relacionamento
       de generalização */
    addColumnsRelGen();

    //Adicionar a tabela referência como tabela pai da tabela receptora
    getReceiverTable()->adicionarTabelaPai(dynamic_cast<Tabela *>(getReferenceTable()));
   }
   else if(rel_type==RELATIONSHIP_DEP)
   {
    /* Definindo que a tabela de origem depende da tabela
       de destino pois parte de seus atributos virão da
       primeira como indicado pelo relacionamento de dependência */
    addColumnsRelGen();
    //Adiciona a tabela referência como tabela cópia da tabela receptora
    getReceiverTable()->adicionarTabelaCopia(dynamic_cast<Tabela *>(getReferenceTable()));
   }
   else if(rel_type==RELATIONSHIP_11 ||
           rel_type==RELATIONSHIP_1N)
   {
    if(rel_type==RELATIONSHIP_11)
     addColumnsRel11();
    else
     addColumnsRel1n();
   }
   else if(rel_type==RELATIONSHIP_NN)
   {
    if(!table_relnn)
     /* Caso o tipo de relacionamento seja n-n e a tabela que representa
        o relacionamento será alocada e configurado o nome automaticamente */
     table_relnn=new Tabela;

     /* O esquema e espaço de tabelas da tabela resultante será, por padrão,
        os mesmos da tabela de origem */
    table_relnn->setName(tab_name_relnn);
    table_relnn->setSchema(src_table->getSchema());
    table_relnn->setTablespace(src_table->getTablespace());

    addColumnsRelNn();
   }

   /* Faz uma chamada ao método de conexão do relacionamento da
      classe base */
   BaseRelationship::connectRelationship();

   /* Indica que o relacionameto foi conetado corretamente e que não está
   invalidado por modificação de atributos */
   this->invalidated=false;
  }
 }
 catch(Exception &e)
 {
  if(table_relnn)
  {
   delete(table_relnn);
   table_relnn=NULL;
  }
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::configureIndentifierRel(Tabela *tab_receptora)
{
 Constraint *pk=NULL;
 unsigned i, qtd;
 QString nome, aux;
 bool nova_pk=false;

 try
 {
  /* Caso seja um relacionamento identificador, a chave primária
     da tabela na qual se insere as colunas, caso exista, será composta
     pelas colunas da chave primária da tabela de origem do relacionamento
     (entidade forte) e pelas colunas da chave primária da tabela de destino
     (entidade fraca) */

  //Obtém a chave primária da tabela de destino
  pk=tab_receptora->obterChavePrimaria();

  //Caso não exista a chave primária na entidade fraca, a mesma será criada
  if(!pk)
  {
   //Cria e configura a chave primária da entidade fraca
   if(!pk_relident)
   {
    pk=new Constraint;
    pk->setConstraintType(TipoRestricao::primary_key);
    pk->setAddedByLinking(true);
    this->pk_relident=pk;
   }
   else
    pk=this->pk_relident;

   nova_pk=true;
   i=1;
   aux[0]='\0';
   nome=tab_receptora->getName() + SUFFIX_SEPARATOR + "pk";

   /* Verifica se já não existe uma restrição na tabela a qual se adiciona
      as retrições cujo nome seja o mesmo configurado acima. Enquanto isso
      ocorrer, será concatenado um número */
   while(tab_receptora->obterRestricao(nome + aux))
   {
    aux=QString("%1").arg(i);
    i++;
   }

   //Atribui o nome configurado   chave primaria criada
   pk->setName(nome);
  }

  /* Adicionando as colunas da chave primária da entidade forte na chave
     primária da entidade fraca */
  qtd=ref_columns.size();
  for(i=0; i < qtd; i++)
   pk->addColumn(ref_columns[i], Constraint::SOURCE_COLS);

  //Caso a tabela não tenha uma chave primária a mesma será atrua   ela
  if(nova_pk)
   tab_receptora->adicionarRestricao(pk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addUniqueKey(Tabela *tab_referencia, Tabela *tab_receptora)
{
 Constraint *uq=NULL;
 unsigned i, qtd;
 QString nome, aux;

 try
 {
  //Aloca a chave única
  if(!uq_rel11)
  {
   uq=new Constraint;
   uq->setConstraintType(TipoRestricao::unique);
   uq->setAddedByLinking(true);
   uq_rel11=uq;
  }

  //Insere as colunas do relacionamentos   chave única
  qtd=ref_columns.size();
  i=0;

  while(i < qtd)
   uq->addColumn(ref_columns[i++], Constraint::SOURCE_COLS);

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  nome=tab_referencia->getName() + SUFFIX_SEPARATOR + "uq";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave única. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(tab_receptora->obterRestricao(nome + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave única
  uq->setName(nome + aux);

  /* Após configurada a chave única que define o
     relacionamento é adicionado na tabela */
  tab_receptora->adicionarRestricao(uq);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addForeignKey(Tabela *tab_referencia, Tabela *tab_receptora, TipoAcao acao_del, TipoAcao acao_upd)
{
 Constraint *pk=NULL, *pk_aux=NULL, *fk=NULL;
 unsigned i, i1, qtd;
 Column *coluna=NULL, *coluna_aux=NULL;
 QString nome, aux;

 try
 {
  //Aloca uma chave estrangeira para ser configurada
  if((rel_type==RELATIONSHIP_NN) ||
     (!fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)))
     //(!fk_rel1n && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N)))
  {
   fk=new Constraint;
   fk->setDeferrable(this->deferrable);
   fk->setDeferralType(this->deferral_type);
   fk->setConstraintType(TipoRestricao::foreign_key);
   fk->setAddedByLinking(true);
   //Define a tabela de destino da chave estrangeira
   fk->setReferencedTable(tab_referencia);

   /* Caso o relacionamento seja 1-1 ou 1-n a chave estrangeira alocada
      será atribud   chave estrangeira que representa o relacionamento */
   if(rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)
    fk_rel1n=fk;
  }
  /*else if(fk_rel1n)
  {
   fk=fk_rel1n;
   fk->removerColunas();
  } */

  //Configura a ação de ON DELETE e ON UPDATE da chave estrangeira
  fk->setActionType(acao_del, false);
  fk->setActionType(acao_upd, true);

  /* Obtém a chave primária da tabela de origem para ser referenciada
     pela chave estrangeira */
  pk=tab_referencia->obterChavePrimaria();

  /* Relacionas as colunas da tabela de origem com as colunas da chave
     primária da tabela de destino, na chave estrangeira do relacionamento */
  qtd=ref_columns.size();
  i=i1=0;

  /* Condição especial para relacionamentos n-n.
     Como as colunas copiadas das tabelas participantes do relacinamentos
     são armazenadas em uma só lista, é necessário fazer um deslocamento
     na varredura para que as colunas nao sejam relacionadas de forma
     incorreta na chave estrangeira.

     Caso 1: A quantidade de colunas (qtd) precisa ser decrementada da quantidade
             de colunas presentes na chave primária da tabela de destino. Isso
             quando o ponteirio 'tab_orig' aponta para a própria tabela de origem
             do relacionamento. Desta forma, evita-se que colunas além da posição
             final da lista de colunas da chave primária de origem sejam acessadas.

     Caso 2: O índice inicial de varredura (i) apontará para a primeira coluna
             na lista do relacionamento a qual corresponde ao conjunto de colunas
             da tabela de destino. A primeira coluna referente a lista de colunas da tabela de
             destino sempre terá seu indice como sendo a quantidade de colunas existentes na
             chave primária da tabela de origem, pois, sempre são inseridas na lista de colunas
             as colunas da origem (vindas da chave primária da origem) e depois as colunas
             da chave primária de destino. */
  if(rel_type==RELATIONSHIP_NN)
  {
   //Caso 1: decrementando a quantidade de colunas a serem varridas
   if((!isSelfRelationship() && tab_referencia==src_table) ||
      (isSelfRelationship() && table_relnn->obterNumRestricoes()==0))
    qtd-=dynamic_cast<Tabela *>(dst_table)->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS);
   //Caso 2: deslocando o índice de varredura
   else if(tab_referencia==dst_table)
   {
    pk_aux=dynamic_cast<Tabela *>(src_table)->obterChavePrimaria();
    i=pk_aux->getColumnCount(Constraint::SOURCE_COLS);
   }
  }

  while(i < qtd)
  {
   //Obtém um coluna referenciada
   coluna=ref_columns[i];
   //Obtém uma coluna da chave primária da tabela de origem
   coluna_aux=pk->getColumn(i1, Constraint::SOURCE_COLS);
   //Faz a ligação das colunas na chave estrangeira
   fk->addColumn(coluna, Constraint::SOURCE_COLS);
   fk->addColumn(coluna_aux, Constraint::REFERENCED_COLS);
   i++; i1++;
  }

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  nome=tab_referencia->getName() + SUFFIX_SEPARATOR + "fk";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave estrangeira. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(tab_receptora->obterRestricao(nome + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave estrangeira
  fk->setName(nome + aux);

  /* Após configurada a chave estrangeira que define o
     relacionamento é adicionado na tabela */
  tab_receptora->adicionarRestricao(fk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addAttributes(Tabela *tab_receptora)
{
 unsigned i, qtd, i1;
 Column *coluna=NULL;
 QString nome, aux;

 try
 {
  //Adicionando os atributos do relacionamento na tabela
  qtd=rel_attributes.size();
  aux[0]='\0';

  for(i=0, i1=1; i < qtd; i++)
  {
   //Obtém o atributo
   coluna=dynamic_cast<Column *>(rel_attributes[i]);

   //Caso o atributo já pertença a uma tabela interrompe o processamento
   if(coluna->getParentTable())
    break;

   nome=coluna->getName();

   /* Verifica o se o nome da coluna já não existe na tabela. Equanto
      existir, incrementa e concatena um número (i1) ao final do nome,
      até que este nome não exista nas colunas da tabela onde será
      inserido o atributo */
   while(tab_receptora->obterColuna(nome + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }
   //Define o nome do atributo
   coluna->setName(nome + aux);
   aux[0]='\0';

   //Adiciona o atributo na tabela
   tab_receptora->adicionarColuna(coluna);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::copyColumns(Tabela *tab_referencia, Tabela *tab_receptora, bool nao_nulo)
{
 Constraint *pk_dest=NULL, *pk_orig=NULL, *pk=NULL;
 unsigned i, qtd, i1;
 Column *coluna=NULL, *coluna_aux=NULL;
 QString nome, sufixo, aux, nome_ant;

 try
 {
  //Obtém as chaves primárias das tabelas de origem e destino
  pk_dest=tab_receptora->obterChavePrimaria();
  pk=pk_orig=tab_referencia->obterChavePrimaria();

   /* Selecionando a lista de colunas correta de acordo com a forma do relacionamento.
     Caso a tabela a qual receberá a chave estrangeira (tab_dest) for uma
     referênci  tabela de origem do relacionamento, o sufixo das colunas a serem criadas
     será configurado como sendo o sufixo da tabela de origem. Caso contrário  o
      será o da própria tabela de destino. */
  if(auto_suffix)
  {
   if(rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)
   {
    sufixo=SUFFIX_SEPARATOR + tab_referencia->getName();

    if(tab_referencia==src_table)
     src_suffix=sufixo;
    else
     dst_suffix=sufixo;
   }
   else if(rel_type==RELATIONSHIP_NN)
   {
    if(tab_referencia==dst_table)
    sufixo=dst_suffix=SUFFIX_SEPARATOR + dst_table->getName();
   else
    sufixo=src_suffix=SUFFIX_SEPARATOR + src_table->getName();
   }
  }
  else if(((rel_type!=RELATIONSHIP_NN && tab_receptora==src_table) ||
           (rel_type==RELATIONSHIP_NN && tab_referencia==dst_table))
      && !dst_suffix.isEmpty())
   sufixo=SUFFIX_SEPARATOR + dst_suffix;
  else if(((rel_type!=RELATIONSHIP_NN && tab_receptora==dst_table) ||
           (rel_type==RELATIONSHIP_NN && tab_referencia==src_table))
           && !src_suffix.isEmpty())
   sufixo=SUFFIX_SEPARATOR + src_suffix;

  /* Caso o relacionamento seja 1-n e a tabela de origem não possua
     uma chave primária ou se o relacionamento seja n-n e ambas as tabelas
     não possuam chave primária, uma exceção será disparada */
  if((!pk_orig && (rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)) ||
     (!pk_orig && !pk_dest && rel_type==RELATIONSHIP_NN))
   throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                          .arg(QString::fromUtf8(this->obj_name))
                          .arg(QString::fromUtf8(tab_referencia->getName(true)))
                          .arg(QString::fromUtf8(tab_receptora->getName(true))),
                 ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);


  /* Obtém a quantidade de colunas referenciadas na chave
     primária selecionada */
  qtd=pk->getColumnCount(Constraint::SOURCE_COLS);

  /* Varre a chave primária, obtendo as colunas e adicionando na lista
     de colunas selecionada acima. Isso é feito para se saber quais são
     as colunas da tabela, dona da chave primária, que serão usadas
    no caso de se criar chaves estrangeiras */
  for(i=0; i < qtd; i++)
  {
   i1=1;
   aux="";

   //Aloca uma nova coluna
   coluna=new Column;
   ref_columns.push_back(coluna);

   /* Copia o conteúdo da coluna da chave primária no indice i para
       a nova coluna criada */
   coluna_aux=pk->getColumn(i, Constraint::SOURCE_COLS);
   pk_columns.push_back(coluna_aux);

   (*coluna)=(*coluna_aux);
   coluna->setNotNull(nao_nulo);

   //Obtém o nome anterior da coluna antes da desconexão do relacionamento
   nome_ant=prev_ref_col_names[coluna->getObjectId()];

   //Protege a nova coluna, evitando que o usuário a modifique ou remova
   coluna->setAddedByLinking(true);

   //Desfaz a referência da coluna a uma tabela pai
   coluna->setParentTable(NULL);

   /* Caso o tipo da nova coluna seja "serial" o mesmo será
      convertido para "integer" */
   if(coluna->getType()=="serial")
    coluna->setType(TipoPgSQL("integer"));
   else if(coluna->getType()=="bigserial")
    coluna->setType(TipoPgSQL("bigint"));
   /* O nome da nova coluna, será o nome original concatenado
      com o sufixo da tabela a qual ela pertence. Isso é feito
      para se saber de onde tal coluna foi originada */
   nome=coluna->getName() + sufixo;

   /* Verifica se o nome da coluna já não existe na tabela
      na qual será inserida, caso exista, um número será concatenado
      ao final de seu nome. Enquanto esta condição for verdadeira
      este número (i1) será incrementado até que não exista uma coluna
      com tal nome (nome original + sufixo + número) */
   while(tab_receptora->obterColuna(nome + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }

   //Armazena o sufixo gerado para validações posteriores do relacionamento
   col_suffixes.push_back(sufixo + aux);

   //Concatena a string auxiliar ao nome inteiro da coluna
   nome+=aux;

   //Primeiramente a coluna é renomeada com seu nome antigo para manter o histórico
   if(nome_ant!="")
    coluna->setName(nome_ant);

   /* Define o nome da coluna como sendo o nome configurado acima, desta forma a
      coluna passará a ter como nome antigo o nome atribuido na iteração acima */
   coluna->setName(nome);

   /* Caso o nome anteriro atribuíd  coluna seja diferente do nome atual, o nome
      atual da coluna passará a ser o nome antigo da mesma quando o relacionamento
      for desconectado e reconectado novamente, desta forma o histórico de nomes da
      colunas não se perde mesmo quando as colunas do relacionamento são desalocadas,
      isso evita a quebra de referências as colunas criadas pelo relacionamento.
      Essa operação só é executada para relacionamentos 1-n e 1-n para relacionamentos
      n-n as colunas são sempre recriadas sem a necessidade de manter o histórico pois
      o usuário não consegue referenciar as colunas criadas pelos relacionamentos n-n.*/
    if(nome_ant!=nome && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
     prev_ref_col_names[coluna->getObjectId()]=coluna->getName();

   /* Adiciona a coluna na tabela a qual foi definida para receber os
      atributos, colunas e restições */
   tab_receptora->adicionarColuna(coluna);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addColumnsRel11(void)
{
 Tabela *tab_ref=NULL, *tab_recep=NULL;

 try
 {
  /* Definindo a ordem das tabelas. O parâmetro
     tab_orig indica de onde está partindo o relacionamento
     e o tab_dest onde está chegando o relacionamento e também
     a tabela que receberá as colunas que representam a ligação.
     Além disso, os atributos e restrições do relacionamento serão
     sempre incluídos na tabela de destino */

  /* Caso 1: (0,1) ---<>--- (0,1)
     Caso 2: (1,1) ---<>--- (0,1) */
  /*if((!obrig_orig && !obrig_dest) ||
     (obrig_orig && !obrig_dest))
  {
   tab_ref=dynamic_cast<Tabela *>(tabela_orig);
   tab_recep=dynamic_cast<Tabela *>(tabela_dest);
  }*/
  /* Caso 3: (0,1) ---<>--- (1,1)
             Adição de colunas na tabela cuja cardinalidade mínima é 0
             (opcionalidade de participação no relacionamento) */
  /*else if(!obrig_orig && obrig_dest)
  {
   tab_ref=dynamic_cast<Tabela *>(tabela_dest);
   tab_recep=dynamic_cast<Tabela *>(tabela_orig);
  }*/
  //### Caso 4: (1,1) ---<>--- (1,1) (não implementado por quebrar a modelagem) ###
  TipoAcao acao_del;

  tab_ref=dynamic_cast<Tabela *>(this->getReferenceTable());
  tab_recep=dynamic_cast<Tabela *>(this->getReceiverTable());

  //Caso a tabela de referência seja obrigatória seta como RESTRICT a ação de delete na chave estrangeira
  if((tab_ref==this->src_table && this->isTableMandatory(SRC_TABLE)) ||
     (tab_ref==this->dst_table && this->isTableMandatory(DST_TABLE)))
    acao_del=TipoAcao::restrict;
  else
    acao_del=TipoAcao::set_null;

  if(isSelfRelationship())
  {
   addAttributes(tab_recep);
   addConstraints(tab_recep);
   copyColumns(tab_ref, tab_recep, false);
   addForeignKey(tab_ref, tab_recep, acao_del, TipoAcao::cascade);
   addUniqueKey(tab_ref, tab_recep);
  }
  else
  {
   copyColumns(tab_ref, tab_recep, false);

   if(identifier)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->setMandatoryTable(DST_TABLE, false);
    this->setMandatoryTable(SRC_TABLE, false);

    if(tab_ref==this->src_table)
     this->setMandatoryTable(SRC_TABLE, true);
    else
     this->setMandatoryTable(DST_TABLE, true);

    configureIndentifierRel(tab_recep);
   }

   addAttributes(tab_recep);
   addConstraints(tab_recep);

   if(identifier)
   {
    addForeignKey(tab_ref, tab_recep, TipoAcao::cascade, TipoAcao::cascade);
   }
   else
   {
    addForeignKey(tab_ref, tab_recep, acao_del,  TipoAcao::cascade);
    addUniqueKey(tab_ref, tab_recep);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addColumnsRel1n(void)
{
 Tabela *tab_ref=NULL, *tab_recep=NULL;
 bool nao_nulo=false;
 TipoAcao acao_del=TipoAcao::set_null, acao_upd=TipoAcao::cascade;

 try
 {
  /* Para relacionamentos 1-n a ordem das tabelas não se alteram, ou seja,
     as colunas da chave estrangeira são sempre adicionadas na tabela
     de origem */
  tab_recep=dynamic_cast<Tabela *>(this->getReceiverTable());
  tab_ref=dynamic_cast<Tabela *>(this->getReferenceTable());

  /* Caso o relacionamento não seja identificador e a participação da tabela
     de referência (origem) seja obrigatória (1,1)--<>--(0|1,n) as colunas da chave estrangeiras
     não podem aceitar valores nulos e além disso as ações ON DELETE e ON UPDATE
     será RESTRIC */
  if(!identifier && src_mandatory)
  {
   if(!deferrable)
    acao_del=TipoAcao::restrict;
   else
    acao_del=TipoAcao::no_action;

   nao_nulo=true;
  }
  /* Caso o relacionamento seja identificador configura as ações ON DELETE e ON UPDATE
     da chave estrangeira como "cascade" pois a entidade fraca só existe se
     a entidade forte existir, isto é, se uma tupla da tabela "entidade forte" for
     removida todas as tuplas da tabela "entidade fraca" também serão excluídas */
  else if(identifier)
   acao_del=TipoAcao::cascade;

  if(isSelfRelationship())
  {
   addAttributes(tab_recep);
   addConstraints(tab_recep);
   copyColumns(tab_ref, tab_recep, nao_nulo);
   addForeignKey(tab_ref, tab_recep, acao_del, acao_upd);
  }
  else
  {
   copyColumns(tab_ref, tab_recep, nao_nulo);

   if(identifier)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->setMandatoryTable(SRC_TABLE, true);
    this->setMandatoryTable(DST_TABLE, false);

    configureIndentifierRel(tab_recep);
   }

   addAttributes(tab_recep);
   addConstraints(tab_recep);

   addForeignKey(tab_ref, tab_recep, acao_del, acao_upd);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addColumnsRelNn(void)
{
 Tabela *tab=NULL, *tab1=NULL;
 Constraint *pk_tabnn=NULL;
 bool nao_nulo_orig=false, nao_nulo_dest=false;
 //TipoAcao acao_orig=TipoAcao::set_null, acao_dest=TipoAcao::set_null;
 TipoAcao acao_del_orig=TipoAcao::restrict, acao_del_dest=TipoAcao::restrict,
          acao_upd_orig=TipoAcao::cascade, acao_upd_dest=TipoAcao::cascade;
 unsigned i, qtd;

 try
 {
  /* Para relacionamentos n-n, a tabela de destino onde serão adicionadas as colunas
     será a tabela que representa o relacionamento (tabela_relnn). As tabelas
     participantes do relacionamento sempre serão as tabelas de origem pois as colunas
     destas precisam ser adicionadas na tabela que define o relacionamento */
  tab=dynamic_cast<Tabela *>(src_table);
  tab1=dynamic_cast<Tabela *>(dst_table);

  /* Pela implementação correta (documental) a cardinalidade do rel. n-n é ignorada
     gerando sempre uma tabela própria cuja chave primária é a junção das
     colunas que representam ambas as tabelas, logo a implementação abaixo
     foi desativada */
  /* if(obrig_orig)
  {
   acao_orig=TipoAcao::restrict;
   nao_nulo_orig=true;
  }

  if(obrig_dest)
  {
   acao_dest=TipoAcao::restrict;
   nao_nulo_dest=true;
  } */

  copyColumns(tab, table_relnn, nao_nulo_orig);
  copyColumns(tab1, table_relnn, nao_nulo_dest);

  /* Cria a chave primária padrão da tabela que consiste nas colunas que
     identificam cada chave estrangeira na tabela. */
  pk_tabnn=new Constraint;
  pk_tabnn->setName(table_relnn->getName() + "_pk");
  pk_tabnn->setConstraintType(TipoRestricao::primary_key);
  pk_tabnn->setAddedByLinking(true);
  qtd=ref_columns.size();

  for(i=0; i < qtd; i++)
   pk_tabnn->addColumn(ref_columns[i],Constraint::SOURCE_COLS);

  table_relnn->adicionarRestricao(pk_tabnn);

  addAttributes(table_relnn);
  addConstraints(table_relnn);

  addForeignKey(tab, table_relnn, acao_del_orig, acao_upd_orig);
  addForeignKey(tab1, table_relnn, acao_del_dest, acao_upd_dest);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tabela *Relationship::getReferenceTable(void)
{
 /* Para relacionamentos n-n que possuem 2 tabelas de refência,
    este método sempre retornará NULL. */
 if(rel_type==RELATIONSHIP_NN)
  return(NULL);
 else
 {
  /* Caso a tabela de origem do relacionamento seja também a
     tabela receptora das colunas que representam o relacionamento,
     retorna que a tabela de destino do relacionamento e a tabela
     de referência para cópia de colunas. */
  if(src_table==getReceiverTable())
   return(dynamic_cast<Tabela *>(dst_table));
  else
   return(dynamic_cast<Tabela *>(src_table));
 }
}

Tabela *Relationship::getReceiverTable(void)
{
 if(rel_type==RELATIONSHIP_11)
 {
  /* Caso 1: (0,1) ---<>--- (0,1)
     Caso 2: (1,1) ---<>--- (0,1) */
  if((!src_mandatory && !dst_mandatory) ||
      (src_mandatory && !dst_mandatory))
   return(dynamic_cast<Tabela *>(dst_table));
  /* Caso 3: (0,1) ---<>--- (1,1)
              Adição de colunas na tabela cuja cardinalidade mínima é 0
              (opcionalidade de participação no relacionamento) */
  else if(!src_mandatory && dst_mandatory)
   return(dynamic_cast<Tabela *>(src_table));
   // Caso 4: (1,1) ---<>--- (1,1)
  else
    /* Caso o usuário tente criar um relacionamento 1-1 onde ambas as entidades
     têm participação obrigatório, será retornado um valor nulo pois este tipo
     de relacionamento não está implementado por quebrar a modelagem feita
     pelo usuário */
  return(NULL);
 }
 /* Para relacionamentos 1-n , a ordem das
    tabelas não se alteram, ou seja, as colunas são sempre adicionadas
    na tabela de destino */
 else if(rel_type==RELATIONSHIP_1N)
  return(dynamic_cast<Tabela *>(dst_table));
  //return(dynamic_cast<Tabela *>(tabela_orig));
 /* Para relacionamentos gen ou dep as colunas são sempre adicionadas
    na tabela de origem do relacionamento */
 else if(rel_type==RELATIONSHIP_GEN ||
         rel_type==RELATIONSHIP_DEP)
  return(dynamic_cast<Tabela *>(src_table));
 /* Para relacionamentos n-n, a tabela de destino onde serão adicionadas as colunas
    será a tabela que representa o relacionamento (tabela_relnn). As tabelas
    participantes do relacionamento sempre serão as tabelas de origem pois as colunas
   destas precisam ser adicionadas na tabela que define o relacionamento */
 else
  return(dynamic_cast<Tabela *>(table_relnn));
}

void Relationship::removeTableObjectsRefCols(Tabela *tabela)
{
 Gatilho *gat=NULL;
 Index *ind=NULL;
 Constraint *rest=NULL;
 int i, qtd;

 //Remove os gatilhos que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumGatilhos();
 for(i=0; i < qtd; i++)
 {
  gat=tabela->obterGatilho(i);
  if(gat->isReferRelationshipColumn())
  {
   tabela->removerObjeto(gat);
   delete(gat);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove os índices que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumIndices();
 for(i=0; i < qtd; i++)
 {
  ind=tabela->obterIndice(i);
  if(ind->isReferRelationshipColumn())
  {
   tabela->removerObjeto(ind);
   delete(ind);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove as restrições que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumRestricoes();
 for(i=0; i < qtd; i++)
 {
  rest=tabela->obterRestricao(i);
  if(!rest->isAddedByRelationship() &&
     rest->getConstraintType()!=TipoRestricao::primary_key &&
     rest->isReferRelationshipColumn())
  {
   tabela->removerObjeto(rest);
   delete(rest);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }
}

void Relationship::removeColumnsFromTablePK(Tabela *tabela)
{
 if(tabela)
 {
  Constraint *pk=NULL;
  Column *coluna=NULL;
  unsigned i, qtd;

  /* Obtém a chave primáira da tabela e remove desta
     as colunas adicionadas por relacionamento */
  pk=tabela->obterChavePrimaria();

  /* Verifica se a chave estrangeira da tabela existe.
     Normalmente, esta chave sempre existirá, porém
     caso a mesma deixe de existir por algum erro anterior a desconexão
     do relacionamento, como é o caso do método de validação
     ModeloBD::validarRefColsIncRelacao() o qual remove restrições caso
     estas se tornem inválidas ou seja referenciem colunas de tabelas
     que deixaram de existir, por isso é prudente sempre verificar se
     a chave primária da tabela existe para não provocar segmentations fault */
  if(pk)
  {
   qtd=pk->getColumnCount(Constraint::SOURCE_COLS);

   for(i=0; i < qtd; i++)
   {
    //Obtém uma coluna da chave primária
    coluna=pk->getColumn(i, Constraint::SOURCE_COLS);

    /* Caso a coluna foi incluída por relacionamento e a mesma pertence
       ao próprio relacionamento */
    if(coluna->isAddedByRelationship() &&
      (isColumnExists(coluna) || getObjectIndex(coluna) >= 0))
    {
     //Remove a coluna da chave primária
     pk->removeColumn(coluna->getName(), Constraint::SOURCE_COLS);
     i--; qtd--;
    }
   }
  }
 }
}

void Relationship::disconnectRelationship(bool rem_objs_tab)
{
 try
 {
  if(connected)
  {
   vector<Column *>::iterator itr, itr_end;
   Column *coluna=NULL;
   Tabela *tabela=NULL;
   unsigned idx_lista=0;
   vector<TableObject *> *lista_atrib=NULL;
   vector<TableObject *>::iterator itr_atrib, itr_atrib_end;
   TableObject *obj_tab=NULL;
   //TipoObjetoBase tipo_obj;

   if(rel_type==RELATIONSHIP_GEN ||
      rel_type==RELATIONSHIP_DEP)
   {
    tabela=getReceiverTable();

    if(rem_objs_tab)
     removeTableObjectsRefCols(tabela);

     removeColumnsFromTablePK(tabela);

    //Remove a tabela cópia/pai dependendo do tipo do relacionamento
    if(rel_type==RELATIONSHIP_GEN)
     tabela->removerObjeto(getReferenceTable()->getName(true), OBJ_TABLE);
    else
     tabela->removerObjeto(getReferenceTable()->getName(true), BASE_TABLE);
   }
   else
   {
    Constraint *pk=NULL, *rest=NULL;
    unsigned i, qtd;

    /* No caso de relacionamento 1-1 e 1-n é necessário
       remover a chave estrangeira que representa o
       relacionamento da tabela, além disso colunas
       adicionadas   chave primária (no caso de um
       relacionamento identificador) precisam ser removidas */
    if(fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
    {
     /* Obtém a tabela a qual possui a chave estrangeira que representa o
        relacionamento (tabela esta onde foi inserida a chave estrangeira
        no momento da conexão do relacionamento) */
     tabela=dynamic_cast<Tabela *>(fk_rel1n->getParentTable());

     //Remove a chave estrangeira da tabela
     tabela->removerRestricao(fk_rel1n->getName());

     /* Obtém a chave primária da tabela para checar se a mesma é igual a chave primária
        que define o relacionamento identificador */
     pk=tabela->obterChavePrimaria();

     //Remove as colunas criadas pelo relacionamento da chave primária da tabela
     removeColumnsFromTablePK(tabela);

     if(rem_objs_tab)
      removeTableObjectsRefCols(tabela);

     /* Remove as colunas da chave estrangeira e a desaloca, apenas para
        relacionamentos 1-1 e 1-n. */
     fk_rel1n->removeColumns();
     delete(fk_rel1n);
     fk_rel1n=NULL;

     if(uq_rel11)
     {
      //Remove a chave única da tabela
      tabela->removerRestricao(uq_rel11->getName());
      uq_rel11->removeColumns();
      delete(uq_rel11);
      uq_rel11=NULL;
     }

     /* Remove a chave primária da tabela caso esta foi criada automaticamente
        (caso de relacionamento identificador e entidade fraca sem chave primária) */
     if(pk && pk==this->pk_relident)
     {
      /* Obtém a tabela a qual possui a chave primária criada pelo relacionamento
         caso este seja um relacionamento identificador */
      tabela=dynamic_cast<Tabela *>(pk_relident->getParentTable());
      //Remove a chave primária da tabela
      tabela->removerRestricao(pk_relident->getName());

      //Desaloca a chave primária
      delete(pk);
      pk_relident=NULL;
     }
    }
    else if(rel_type==RELATIONSHIP_NN)
    {
     /* Caso o relacionamento seja n-n, apenas remove as restrições sem
        qualquer validação */
     qtd=table_relnn->obterNumRestricoes();

     for(i=0; i < qtd ; i++)
     {
      //Obtém a restrição
      rest=table_relnn->obterRestricao(i);

      /* Caso a restrição foi adicionada por lgação porém não é uma restrição criada pelo usuário e
         e sim uma restrição criada quando o relacionamento é conectado (ex. chave primária e chaves estrangeiras).
         A segunda parte da condição obterIndiceObjeto(rest) < 0 verifica se a restrição a ser removida não fazer
         parte da lista de restrições criadas pelo usuário, caso faça parte, não será destruída */
      if(rest->isAddedByRelationship() && getObjectIndex(rest) < 0)
      {
       //Remove a restrição da tabela
       table_relnn->removerRestricao(rest->getName());
       i--; qtd--;
       delete(rest);
      }
     }
    }
   }

   /* Remover os atributos e restrições do relacionamento os quais
      estão incluídos nas tabelas participantes. */
   tabela=getReceiverTable();
   while(idx_lista <= 1)
   {
    /* Seleciona a lista de objetos, a primeira lista sempre deve ser a
       de restrições para evitar de se remover colunas antes das restrições
       gerando erro de remoção de coluna referenciada */
    lista_atrib=(idx_lista==0 ? &rel_constraints : &rel_attributes);

    itr_atrib=lista_atrib->begin();
    itr_atrib_end=lista_atrib->end();

    //Varre a lista de atributos selecionada
    while(itr_atrib!=itr_atrib_end)
    {
     //Obtém o atributo
     obj_tab=(*itr_atrib);
     //tipo_obj=obj_tab->obterTipoObjeto();

     /* É necessário verificar se o objeto pertence ao relacionamento
        a fim de evitar que atributos adicionados por outros relacionamentos
          tabela também sejam removidos */
     if(tabela && getObjectIndex(obj_tab) >= 0)
     {
      //Remove o atributo da tabela através do nome e tipo
      tabela->removerObjeto(obj_tab->getName(), obj_tab->getObjectType());
      obj_tab->setParentTable(NULL);
     }
     //Para para o atributo posterior
     itr_atrib++;
    }
    //Passa para a lista seguinte de atributos
    idx_lista++;
   }

   //Excluindo as colunas incluídas na tabela pelo relacionamento
   itr=ref_columns.begin();
   itr_end=ref_columns.end();

   /*  Varre a lista de colunas do relacionamento removendo cada uma
      da tabela onde foi inserida e desalocando o espaço que esta
      ocupa em memória */
   while(itr!=itr_end)
   {
    coluna=(*itr);
    //Remove a coluna da tabela pai
    tabela->removerColuna(coluna->getName());
    itr++;
    //Desaloca a coluna
    delete(coluna);
   }

   //Limpa as lista de coluna do relacionamento
   ref_columns.clear();
   pk_columns.clear();
   col_suffixes.clear();

   /* Caso o relacionamento seja n-n desaloca a tabela que representa
      o relacionamento */
   if(table_relnn)
   {
    delete(table_relnn);
    table_relnn=NULL;
   }

   //Caso a chave primária especial esteja alocada-remove-a
   if(pk_special)
   {
    delete(pk_special);
    pk_special=NULL;
   }

   //Executa o método de desconexão de relacionamento da classe base
   BaseRelationship::disconnectRelationship();
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

bool Relationship::isIdentifier(void)
{
 return(identifier);
}

bool Relationship::hasIndentifierAttribute(void)
{
 vector<TableObject *>::iterator itr, itr_end;
 Constraint *rest=NULL;
 bool enc=false;

 itr=rel_constraints.begin();
 itr_end=rel_constraints.end();

 //Varre a lista de restrições
 while(itr!=itr_end && !enc)
 {
  //Obtém uma restrição da lista
  rest=dynamic_cast<Constraint *>(*itr);
  /* Um relacionamento será considerado possuir um
     atributo identificandor quando for encontrada
     uma chave primária em sua lista de restrições */
  enc=(rest->getConstraintType()==TipoRestricao::primary_key);
  itr++;
 }

 return(enc);
}

bool Relationship::isInvalidated(void)
{
 unsigned qtd_cols_rel, qtd_cols_tab, i, i1, qtd;
 Tabela *tabela=NULL, *tabela1=NULL;
 Constraint *fk=NULL, *fk1=NULL, *rest=NULL, *pk=NULL;
 bool valido=false;
 Column *col1=NULL, *col2=NULL, *col3=NULL;
 QString nome_col;

 /* Caso o relacionamento foi invaldado por modificação
    de atributos críticos já retorna a invalidez sem
    executar os procedimentos de validação abaixo */
 if(invalidated)
 {
  /* Caso o relacionamento seja identificador, remove a chave primária
     criada automaticamente quando o mesmo é conectado para forçar
     a tabela receptora fique sem chave primária em consequência
     todo e qualquer relacionamento 1-1, 1-n ou n-n ligado a ela
     deverá ser revalidado */
  if(pk_relident && pk_relident->isAddedByLinking())
  {
   dynamic_cast<Tabela *>(pk_relident->getParentTable())->removerObjeto(pk_relident);
   pk_relident=NULL;
  }
  return(true);
 }
 /* Caso o relacionamento esteja conectado e não esteja invalidado
    por modificação de atributos */
 else if(connected)
 {
   /* Valida os sufixos caso a geração automática de sufixos esteja ativa.
      Checa se os sufixos, quando preenchidos, coincidem  com os nomes das tabelas respectivas */
   if(auto_suffix &&
      ((!src_suffix.isEmpty() &&  src_suffix!=QString(SUFFIX_SEPARATOR) + src_table->getName()) ||
       (!dst_suffix.isEmpty() &&  dst_suffix!=QString(SUFFIX_SEPARATOR) + dst_table->getName())))
    return(true);

  /* Pare relacionamentos 1-1 e 1-n a verificação de
     invalidação do relacionamento baseia-se na comparação da
     quantidade de colunas da chave estrangeira configurada
     pela conexão do mesmo com a quantidade de colunas da
     chave primária da tabela de origem do relacionamento */
  if(rel_type==RELATIONSHIP_11 ||
     rel_type==RELATIONSHIP_1N)
  {
   //Obtém a tabela de referencia do relacionamento
   tabela=getReferenceTable();

   //Obtém a quantidade de colunas da chave estrangeira que representa a relação
   qtd_cols_rel=fk_rel1n->getColumnCount(Constraint::SOURCE_COLS);

   //O relacionamento estára invalidado caso a tabela referência não possua uma chave primária
   pk=tabela->obterChavePrimaria();

   if(pk)
   {
    //Obtém a quantidade de colunas da chave primária da tabela
    qtd_cols_tab=pk->getColumnCount(Constraint::SOURCE_COLS);

    //Obtém a tabela de referencia do relacionamento
    tabela1=getReceiverTable();

    //Faz a verificação de quantidade de colunas
    valido=(qtd_cols_rel==qtd_cols_tab);

    //A próxima validação é a de nome e tipo das colunas
    for(i=0; i < qtd_cols_rel && valido; i++)
    {
     //Obtém a da chave estrangeira
     col2=ref_columns[i];

     //Obtém a coluna da chave primária ligd   coluna atual da chave estrangeira
     col1=pk_columns[i];

     /* Obtém a coluna da pk em si. Com esta referência será verificado se os endereços são iguais
        caso não sejam invalida o relacionamento */
     col3=pk->getColumn(i, Constraint::SOURCE_COLS);

     /* Para se validar as colunas entre si as seguintes regras são seguidas:

      1) Verifica se os nomes são iguais. Caso seja iguais nada é feito.
         Caso sejam diferentes é necessário verificar se já existe uma coluna
         na tabela receptora com o mesmo nome da coluna atual da chave primária,
         isso indica que a coluna da chave primária precisou ser renomeada na tabela
         receptora pois esta já possuia uma coluna de mesmo nome.

      2) Verifica se os tipos das colunas são compatíveis.
         A única exceção aceita é se o tipo da coluna de origem é do tipo 'serial' ou 'bigserial'
         e da coluna de destino seja 'integer' ou 'bigint'

      3) Checa se a coluna (endereço) vindo do vetor colunas_pk é iga   coluna
         obtida diretamente da chave primária */
     nome_col=col1->getName() + col_suffixes[i];
     valido=(col1==col3 &&
             (nome_col==col2->getName()) &&
             (col1->getType()==col2->getType() ||
             (col1->getType()=="serial" && col2->getType()=="integer") ||
             (col1->getType()=="bigserial" && col2->getType()=="bigint")));
    }
   }
  }
  /* Para relacionamentos de dependência e generalização,
     obtem-se a quantidade de colunas criadas pela ligação do
     relacionamento e a compara com a quantidade de colunas
     da tabela de origem */
  else if(rel_type==RELATIONSHIP_DEP ||
          rel_type==RELATIONSHIP_GEN)
  {
   //Obtém a tabela de referencia do relacionamento
   tabela=getReferenceTable();

   //Obtém a tabela de referencia do relacionamento
   tabela1=getReceiverTable();

   /* Obtém o número total de colunas do relacionamento, incluído as
      que foram adicionadas por relacionamentos */
   qtd_cols_tab=tabela->obterNumColunas();

   /* Obtém a quantidade de colunas criadas com a conexão da relação somando-se
      com a quantidade de colunas rejeitadas no momento da ligaçã de acordo
      com as regras */
   qtd_cols_rel=ref_columns.size() + rejected_col_count;
   valido=(qtd_cols_rel == qtd_cols_tab);

   /* Checando se as colunas criadas com a herança/dependência ainda existem
      na tabela de referência */
   for(i=0; i < ref_columns.size() && valido; i++)
    valido=tabela->obterColuna(ref_columns[i]->getName(true));

   /* Checando se as colunas da tabela referência existem na tabela
      receptora. Na teoria todas as colunas devem existir pois uma
      herda a outra logo todos possuirão as mesmas colunas. Caso isso
      não acontença indica que uma coluna da tabela de referência foi
      renomeada */
   for(i=0; i < qtd_cols_tab && valido; i++)
    valido=tabela1->obterColuna(tabela->obterColuna(i)->getName(true));
  }
  /* Para relacionamentos n-n, é necessário as comparações:
     1) Pega-se a chave estrangeira da tabela criada pela ligação
     a qual referencia a tabela de origem e verifica se as quantidades
     de colunas coincidem. O mesmo é feito para a segunda chave estrangeira
     só que em rela   chave primaria da tabela de destino
     2) É necessário validar se os nomes das colunas da tabela gerada
        coincidem com os nomes das colunas das tabelas originárias */
  else if(rel_type==RELATIONSHIP_NN)
  {
   //Obtém a referência para a tabela de destino do relacionamento
   tabela=dynamic_cast<Tabela *>(src_table);
   tabela1=dynamic_cast<Tabela *>(dst_table);

   /* Para se validado um relacionamento n-n, a primeira condição é que
      ambas as tabelas possuam chave estrangeira */
   if(tabela->obterChavePrimaria() && tabela1->obterChavePrimaria())
   {
    //Obtém a quantidade de restrições da tabela criada pelo relacionamento
    qtd=table_relnn->obterNumRestricoes();

    //Varre a lista de restrições da tabela
    for(i=0; i < qtd; i++)
    {
     //Obtém uma restrição
     rest=table_relnn->obterRestricao(i);

     //Caso a mesma seja uma chave estrangeira
     if(rest->getConstraintType()==TipoRestricao::foreign_key)
     {
      /* Verifica se a tabela referenciada pela chave é a tabela de origem
         caso seja, armazena seu endereço na referêni   chave estrangeira
         da origem */
      if(!fk && rest->getReferencedTable()==tabela)
       fk=rest;
      else if(!fk1 && rest->getReferencedTable()==tabela1)
       fk1=rest;
     }
    }

    /* A quantidade de colunas do relacionamento é calculada pela soma
       das quantidades de colunas das chaves estrangeiras obtidas */
    qtd_cols_rel=fk->getColumnCount(Constraint::REFERENCED_COLS) + fk1->getColumnCount(Constraint::REFERENCED_COLS);
    /* A quantidade de colunas da tabela é obtida pela soma da quantidade
       de colunas das chaves primárias envolvidas no relacionamentos */
    qtd_cols_tab=tabela->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS) +
                 tabela1->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS);
    valido=(qtd_cols_rel == qtd_cols_tab);

    /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de referência */
    qtd=fk->getColumnCount(Constraint::SOURCE_COLS);
    for(i=0; i < qtd && valido; i++)
    {
     nome_col=fk->getColumn(i, Constraint::SOURCE_COLS)->getName();

     /* Caso o sufixo da origem esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de origem */
     if(!col_suffixes[i].isEmpty())
      nome_col=nome_col.remove(col_suffixes[i]);

     //Verifica a existencia da coluna na tabela
     col1=tabela->obterColuna(nome_col);
     valido=col1 &&
            tabela->obterChavePrimaria()->isColumnExists(col1, Constraint::SOURCE_COLS);
    }

   /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de receptora */
    i1=qtd;
    qtd+=fk1->getColumnCount(Constraint::SOURCE_COLS);
    for(i=0; i1 < qtd && valido; i1++)
    {
     nome_col=fk1->getColumn(i++, Constraint::SOURCE_COLS)->getName();

     /* Caso o sufixo do destino esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de destino */
     if(!col_suffixes[i1].isEmpty())
      nome_col=nome_col.remove(col_suffixes[i1]);

     //Verifica a existencia da coluna na tabela
     col1=tabela1->obterColuna(nome_col);
     valido=col1 &&
            tabela1->obterChavePrimaria()->isColumnExists(col1, Constraint::SOURCE_COLS);
    }
   }
  }
  return(!valido);
 }
 else
  return(true);
}

QString Relationship::getCodeDefinition(unsigned tipo_def)
{
 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  if(fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
  {
   unsigned qtd, i;

   attributes[ParsersAttributes::RELATIONSHIP_1N]="1";
   attributes[ParsersAttributes::CONSTRAINTS]=fk_rel1n->getCodeDefinition(tipo_def);

   if(uq_rel11)
    attributes[ParsersAttributes::CONSTRAINTS]+=uq_rel11->getCodeDefinition(tipo_def);

   qtd=rel_constraints.size();
   for(i=0; i < qtd; i++)
   {
    if(dynamic_cast<Constraint *>(rel_constraints[i])->getConstraintType()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
                                              getCodeDefinition(tipo_def, false);

   }

   attributes[ParsersAttributes::TABLE]=getReceiverTable()->getName(true);
  }
  else if(table_relnn && rel_type==RELATIONSHIP_NN)
  {
   unsigned qtd, i;

   attributes[ParsersAttributes::RELATIONSHIP_NN]="1";
   attributes[ParsersAttributes::TABLE]=table_relnn->getCodeDefinition(tipo_def);

   qtd=table_relnn->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    if(table_relnn->obterRestricao(i)->getConstraintType()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=table_relnn->obterRestricao(i)->getCodeDefinition(tipo_def, true);
   }
  }
  else if(rel_type==RELATIONSHIP_GEN)
  {
   attributes[ParsersAttributes::RELATIONSHIP_GEN]="1";
   attributes[ParsersAttributes::TABLE]=getReceiverTable()->getName(true);
   attributes[ParsersAttributes::ANCESTOR_TABLE]=getReferenceTable()->getName(true);
  }

  return(this->BaseObject::__getCodeDefinition(SchemaParser::SQL_DEFINITION));
 }
 else
 {
  unsigned qtd, i;
  bool forma_reduzida;

  setRelationshipAttributes();
  attributes[ParsersAttributes::SRC_SUFFIX]=(!auto_suffix ? src_suffix : "");
  attributes[ParsersAttributes::DST_SUFFIX]=(!auto_suffix ? dst_suffix : "");
  attributes[ParsersAttributes::IDENTIFIER]=(identifier ? "1" : "");
  attributes[ParsersAttributes::DEFERRABLE]=(deferrable ? "1" : "");
  attributes[ParsersAttributes::AUTO_SUFFIX]=(auto_suffix ? "1" : "");
  attributes[ParsersAttributes::DEFER_TYPE]=~deferral_type;
  attributes[ParsersAttributes::TABLE_NAME]=tab_name_relnn;


  attributes[ParsersAttributes::COLUMNS]="";
  qtd=rel_attributes.size();
  for(i=0; i < qtd; i++)
  {
   attributes[ParsersAttributes::COLUMNS]+=dynamic_cast<Column *>(rel_attributes[i])->
                    getCodeDefinition(SchemaParser::XML_DEFINITION);
  }

  attributes[ParsersAttributes::CONSTRAINTS]="";
  qtd=rel_constraints.size();
  for(i=0; i < qtd; i++)
  {
   if(!rel_constraints[i]->isProtected())
    attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
                       getCodeDefinition(SchemaParser::XML_DEFINITION, true);
  }

  qtd=column_ids_pk_rel.size();
  for(i=0; i < qtd; i++)
  {
   //Armazena o nome das colunas da chave primária especial se houver
   if(!ref_columns.empty() && i < ref_columns.size())
   {
    attributes[ParsersAttributes::SPECIAL_PK_COLS]+=QString("%1").arg(column_ids_pk_rel[i]);
    if(i < qtd-1) attributes[ParsersAttributes::SPECIAL_PK_COLS]+=",";
   }
  }

  /* Caso não haja colunas, restrições e linha definida no relacionamento
     a definição XML do rel. será em forma reduzida */
  forma_reduzida=(attributes[ParsersAttributes::COLUMNS].isEmpty() &&
                  attributes[ParsersAttributes::CONSTRAINTS].isEmpty() &&
                  attributes[ParsersAttributes::POINTS].isEmpty() &&
                  attributes[ParsersAttributes::SPECIAL_PK_COLS].isEmpty());


  return(this->BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION, forma_reduzida));
 }
}

void Relationship::setAutomaticSuffix(bool valor)
{
 this->invalidated=(this->auto_suffix!=valor);
 this->auto_suffix=valor;
}

bool Relationship::isAutomaticSuffix(void)
{
 return(this->auto_suffix);
}

void Relationship::operator = (Relationship &rel)
{
 (*dynamic_cast<BaseRelationship *>(this))=dynamic_cast<BaseRelationship &>(rel);
 this->invalidated=true;
 this->column_ids_pk_rel=rel.column_ids_pk_rel;
 this->rel_attributes=rel.rel_attributes;
 this->rel_constraints=rel.rel_constraints;
 this->dst_suffix=rel.dst_suffix;
 this->src_suffix=rel.src_suffix;
 this->identifier=rel.identifier;
 this->deferral_type=rel.deferral_type;
 this->deferrable=rel.deferrable;
 this->tab_name_relnn=rel.tab_name_relnn;
 this->table_relnn=NULL;
 this->fk_rel1n=pk_relident=pk_special=NULL;
 this->ref_columns.clear();
 this->auto_suffix=rel.auto_suffix;
}

