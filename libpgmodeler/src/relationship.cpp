#include "relationship.h"
#include <QApplication>

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

  /* Raises an error if the user tries to create a relationship which some
     table doesn't has a primary key */
  if(((rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N) &&
      !this->getReferenceTable()->obterChavePrimaria()) ||
     (rel_type==RELATIONSHIP_NN && (!src_tab->obterChavePrimaria() || !dst_tab->obterChavePrimaria())))
     throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                           .arg(QString::fromUtf8(obj_name))
                           .arg(QString::fromUtf8(src_tab->getName(true)))
                           .arg(QString::fromUtf8(dst_tab->getName(true))),
                  ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   this->src_suffix=src_suffix;
  this->dst_suffix=dst_suffix;
  this->auto_suffix=auto_suffix;

  table_relnn=NULL;
  fk_rel1n=pk_relident=pk_special=uq_rel11=NULL;
  this->deferrable=deferrable;
  this->deferral_type=deferral_type;
  this->invalidated=true;

  if(rel_type==RELATIONSHIP_11)
   str_aux=QApplication::translate("Relationship","%1_has_one_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_1N)
   str_aux=QApplication::translate("Relationship","%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_NN)
   str_aux=QApplication::translate("Relationship","many_%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(rel_type==RELATIONSHIP_GEN)
   str_aux=QApplication::translate("Relationship","%1_inherits_%2","",QApplication::UnicodeUTF8);
  else
   str_aux=QApplication::translate("Relationship","%1_copies_%2","",QApplication::UnicodeUTF8);

  if(rel_type!=RELATIONSHIP_NN)
   str_aux=str_aux.arg(this->getReferenceTable()->getName())
                     .arg(this->getReceiverTable()->getName());
  else
   str_aux=str_aux.arg(this->src_table->getName())
                  .arg(this->dst_table->getName());

  setName(str_aux);

  //Setting up the n-n relationship table name based on the suffixes, when they are defined
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
 /* Raises an error if the user try to set an self relationship, n-n relationship,
    generalization or copy as identifier. Only 1-1, 1-n relationships can be
    set as identifier. */
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
 /* Raises an error if the user try to set columns for special primary key when the
    relationship type is n-n or identifier or self relationship */
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

  /* Allocates the primary key with the following feature:
     1) Protected and included by linking in order to be easily identified
        on internal operations of the relationship

     2) Use the same tablespace as the receiver table */
  pk_special=new Constraint;
  pk_special->setName(this->getName() + QString("_pk"));
  pk_special->setConstraintType(TipoRestricao::primary_key);
  pk_special->setAddedByLinking(true);
  pk_special->setProtected(true);
  pk_special->setTablespace(dynamic_cast<Tablespace *>(getReceiverTable()->getTablespace()));

  //Adds the columns to the primary key
  count=column_ids_pk_rel.size();
  for(i=0; i < count; i++)
  {
   if(column_ids_pk_rel[i] < ref_columns.size() &&
      !pk_special->isColumnExists(ref_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS))
    pk_special->addColumn(ref_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS);
  }

  try
  {
   this->addObject(pk_special);
  }
  catch(Exception &e)
  {
   //Case some error is raised deletes the special primary key
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

 //Raises an error if the object is not allocated
 if(!object)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Selecting the correct list using the object type
 obj_type=object->getObjectType();
 if(obj_type==OBJ_COLUMN)
  list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  list=&rel_constraints;
 else
  //Raises an error if the object type isn't valid (not a column or constraint)
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=list->begin();
 itr_end=list->end();

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

 //Raises an error if the column is not allocated
 if(!column)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=ref_columns.begin();
 itr_end=ref_columns.end();

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

 /* Raises an error if the user try to add  manually a special primary key on
    the relationship and the relationship type is not generalization or copy */
 if((rel_type==RELATIONSHIP_GEN ||
     rel_type==RELATIONSHIP_DEP) &&
    !(tab_obj->isAddedByRelationship() &&
      tab_obj->isProtected() &&
      tab_obj->getObjectType()==OBJ_CONSTRAINT))
  throw Exception(ERR_ASG_OBJ_INV_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 try
 {
  //Checks if the object isn't exists on the relationshi and doesn't belongs to a table
  if(!tab_obj->getParentTable() &&
     getObjectIndex(tab_obj) < 0)
  {
   //Gets the object list according the object type
   obj_type=tab_obj->getObjectType();
   if(obj_type==OBJ_COLUMN)
    obj_list=&rel_attributes;
   else if(obj_type==OBJ_CONSTRAINT)
    obj_list=&rel_constraints;
   else
    //Raises an error if the object type isn't valid (not a column or constraint)
    throw Exception(ERR_ASG_OBJECT_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Defines the parent table for the object only for validation
   tab_obj->setParentTable(src_table);

   //Generates the code for the object only for validation
   if(obj_type==OBJ_COLUMN)
    dynamic_cast<Column *>(tab_obj)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
   else
   {
    Constraint *rest=NULL;
    rest=dynamic_cast<Constraint *>(tab_obj);

    //Raises an error if the user try to add as foreign key to relationship
    if(rest->getConstraintType()==TipoRestricao::foreign_key)
     throw Exception(ERR_ASG_FOREIGN_KEY_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);
   }

   //Switch back to null the object parent
   tab_obj->setParentTable(NULL);

   if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
    obj_list->push_back(tab_obj);
   else
   {
    if(obj_list->size() > 0)
     obj_list->insert((obj_list->begin() + obj_idx), tab_obj);
    else
     obj_list->push_back(tab_obj);
   }

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

 if(obj_type==OBJ_COLUMN)
  obj_list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  obj_list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Raises an error if the object index is out of bound
 if(obj_id >= obj_list->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
   //Check is the column is referenced by one relationship constraints
   refer=(constr->getColumn(col->getName(), Constraint::SOURCE_COLS) ||
          constr->getColumn(col->getName(), Constraint::REFERENCED_COLS));
   itr++;
  }

  //Raises an error if the column to be removed is referenced by a relationship constraint
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

 //Before the column removal is necessary disconnect the relationship
 disconnectRelationship(false);

 //Removes the column
 obj_list->erase(obj_list->begin() + obj_id);

 //Reconnects the relationship
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

 if(obj_type==OBJ_COLUMN)
  list=&rel_attributes;
 else if(obj_type==OBJ_CONSTRAINT)
  list=&rel_constraints;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=list->begin();
 itr_end=list->end();

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
 //Raises an error if the attribute index is out of bound
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
 //Raises an error if the constraint index is out of bound
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

void Relationship::addConstraints(Tabela *dst_tab)
{
 Constraint *constr=NULL, *pk=NULL;
 unsigned constr_id, constr_cnt, i, count;
 QString name, orig_name, aux;

 try
 {
  constr_cnt=rel_constraints.size();

  for(constr_id=0; constr_id < constr_cnt; constr_id++)
  {
   constr=dynamic_cast<Constraint *>(rel_constraints[constr_id]);

   //Breaks the iteration if the constraist has a parent
   if(constr->getParentTable())
    break;

   if(constr->getConstraintType()!=TipoRestricao::primary_key)
   {
    i=1; aux[0]='\0';
    name="";

    //Configures the name of the constraint in order to avoid name duplication errors
    orig_name=constr->getName();
    while(dst_tab->obterRestricao(orig_name + aux))
    {
     aux=QString("%1").arg(i);
     name=orig_name + aux;
     i++;
    }

    if(name!="") constr->setName(name);

    //Adds the constraint to the table
    dst_tab->adicionarRestricao(constr);
   }
   else
   {
    /* Case the constraint is a primary key it will be merged with the
       table's primary key */

    //Gets the table primary key
    pk=dst_tab->obterChavePrimaria();

    if(pk)
    {
     count=constr->getColumnCount(Constraint::SOURCE_COLS);

     for(i=0; i < count; i++)
      //Adds the colums from the constraint into the table's primary key
      pk->addColumn(constr->getColumn(i, Constraint::SOURCE_COLS),
                          Constraint::SOURCE_COLS);
    }
    else
     //Case the table doens't has a primary key the constraint will the be it
     dst_tab->adicionarRestricao(constr);

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
 Tabela *src_tab=NULL, *dst_tab=NULL,
        *parent_tab=NULL, *aux_tab=NULL;
 Column *src_col=NULL, *dst_col=NULL,
        *column=NULL, *aux_col=NULL;
 unsigned src_count, dst_count,
          i, i1, i2, id_tab,
          idx, tab_count;
 vector<Column *> columns;
 ObjectType types[2]={OBJ_TABLE, BASE_TABLE};
 ErrorType err_type=ERR_CUSTOM;
 bool duplic=false, cond,
      /* 0 -> Column created by inheritance relationship
         1 -> Column created by copy relationship */
      src_flags[2]={false,false},
      dst_flags[2]={false,false};
 QString str_aux, msg;
 TipoPgSQL src_type, dst_type;

 try
 {
  src_tab=dynamic_cast<Tabela *>(src_table);
  dst_tab=dynamic_cast<Tabela *>(dst_table);

  //Gets the column count from participant tables
  src_count=src_tab->obterNumColunas();
  dst_count=dst_tab->obterNumColunas();
  rejected_col_count=0;

  /* This for compares the columns of the receiver table
     with the columns of the reference table in order to
     resolve the conflicting names */
  for(i=0; i < dst_count && err_type==ERR_CUSTOM; i++)
  {
   //Gets the column from the receiver (destination) table
   dst_col=dst_tab->obterColuna(i);

   /* The copied column have the 'serial' like types converted to
      integer like types in order to avoid error when configuring the
      relationship foreign key */
   dst_type=dst_col->getType();

   if(dst_type=="serial") dst_type="integer";
   else if(dst_type=="bigserial") dst_type="bigint";

   /* This flag indicates that the column name is registered
      in the other table column (duplication). This situation need
      to be resolved in order to evict the creation of duplicated column
      on the receiver table */
   duplic=false;

   for(i1=0; i1 < src_count && !duplic; i1++)
   {
    //Gets the reference (source) column converting its type
    src_col=src_tab->obterColuna(i1);
    src_type=src_col->getType();

    if(src_type=="serial") src_type="integer";
    else if(src_type=="bigserial") src_type="bigint";

    //Check the duplication on the column names
    duplic=(src_col->getName()==dst_col->getName());

    //In case of duplication
    if(duplic)
    {
     /* It is necessary to check if the source column (reference) is of the table itself,
        if it came from a parent table or a table copy. The same verification is the
        destination column.

        The duplicity of columns only generates error when the source column is
        of the table itself and the target column was not from a parent table
        of the receiver table in the case of a copy relationship.

        If the source column is of the reference table or coming from a
        copy relationship and the type of the current relationship is
        inheritance, the only case in which the duplicity generates error is
        the type incompatibility of the columns involved, otherwise they are merged. */
     for(id_tab=0; id_tab < 2; id_tab++)
     {
      if(id_tab==0)
      {
       aux_col=src_col;
       aux_tab=src_tab;
      }
      else
      {
       aux_col=dst_col;
       aux_tab=dst_tab;
      }

      for(i2=0; i2 < 2; i2++)
      {
       tab_count=aux_tab->obterNumObjetos(types[i2]);

       //Checking if the column came from a inheritance or copy relationship
       for(idx=0; idx < tab_count; idx++)
       {
        parent_tab=dynamic_cast<Tabela *>(aux_tab->obterObjeto(idx, types[i2]));
        cond=(parent_tab->obterColuna(aux_col->getName()));

        if(id_tab==0)
         src_flags[i2]=cond;
        else
         dst_flags[i2]=cond;
       }
      }
     }

     /* Error condition 1: The relationship type is dependency and the source
        column is from the table itself or it came from a copy table and the
        destination column is from the destination table or came from a copy table
        of the destination table itself */
     if(rel_type==RELATIONSHIP_DEP &&

       ((!src_flags[0] && !src_flags[1]) ||
        (!src_flags[0] &&  src_flags[1])) &&

       ((!dst_flags[0] && !dst_flags[1]) ||
        (!dst_flags[0] &&  dst_flags[1])))
     {
      err_type=ERR_DUPLIC_COLS_COPY_REL;
     }
     /* Error condition 2: The relationship type is generalization and the column
        types is incompatible */
     else if(rel_type==RELATIONSHIP_GEN &&
             src_type!=dst_type)
      err_type=ERR_INCOMP_COLS_INHERIT_REL;
    }
   }

   //In case that no error was detected (ERR_CUSTOM)
   if(err_type==ERR_CUSTOM)
   {
    //In case there is no column duplicity
    if(!duplic)
    {
     //Creates a new column making the initial configurations
     column=new Column;

     (*column)=(*dst_col);

     if(rel_type==RELATIONSHIP_GEN)
      column->setAddedByGeneralization(true);
     else
      column->setAddedByCopy(true);

     column->setParentTable(NULL);

     //Converte the type
     if(column->getType()=="serial")
      column->setType(TipoPgSQL("integer"));
     else if(column->getType()=="bigserial")
      column->setType(TipoPgSQL("bigint"));

     //Adds the new column to the temporary column list
     columns.push_back(column);
    }
    else
     /* If there is duplicity, the column is discarded and not included in the list,
        instead, increases the attribute which counts the amount
        duplicate columns of which were rejected by already exist
        in the target (receiver) table */
     rejected_col_count++;
   }
  }

  //In case that no duplicity error is detected
  if(err_type==ERR_CUSTOM)
  {
   vector<Column *>::iterator itr, itr_end;

   /* The columns of the temporary list will be inserted
      in the list of referencing columns, and additionally the
      relationship columns will also be inserted directly in the
      source table, which inherits or copy table columns from target table */
   ref_columns=columns;
   itr=ref_columns.begin();
   itr_end=ref_columns.end();
   while(itr!=itr_end)
   {
    src_tab->adicionarColuna((*itr));
    itr++;
   }
  }
  else
  {
   //In case of duplicity error the temporary columns are destroyed
   while(!columns.empty())
   {
    delete(columns.back());
    columns.pop_back();
   }

   str_aux=Exception::getErrorMessage(err_type);

   if(err_type==ERR_DUPLIC_COLS_COPY_REL)
   {
    msg=QString(str_aux)
        .arg(dst_col->getName())
        .arg(dst_tab->getName())
        .arg(src_tab->getName());
   }
   else
   {
    msg=QString(str_aux)
        .arg(dst_col->getName())
        .arg(dst_tab->getName())
        .arg(src_col->getName())
        .arg(src_tab->getName());
   }

   throw Exception(msg, err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Creates the special primary key if exists
  this->createSpecialPrimaryKey();

  //Adds the constraint on the receiver table
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

void Relationship::configureIndentifierRel(Tabela *dst_tab)
{
 Constraint *pk=NULL;
 unsigned i, count;
 QString name, aux;
 bool new_pk=false;

 try
 {
  /* Caso seja um relacionamento identificador, a chave primária
     da tabela na qual se insere as colunas, caso exista, será composta
     pelas colunas da chave primária da tabela de origem do relacionamento
     (entidade forte) e pelas colunas da chave primária da tabela de destino
     (entidade fraca) */

  //Obtém a chave primária da tabela de destino
  pk=dst_tab->obterChavePrimaria();

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

   new_pk=true;
   i=1;
   aux[0]='\0';
   name=dst_tab->getName() + SUFFIX_SEPARATOR + "pk";

   /* Verifica se já não existe uma restrição na tabela a qual se adiciona
      as retrições cujo nome seja o mesmo configurado acima. Enquanto isso
      ocorrer, será concatenado um número */
   while(dst_tab->obterRestricao(name + aux))
   {
    aux=QString("%1").arg(i);
    i++;
   }

   //Atribui o nome configurado   chave primaria criada
   pk->setName(name);
  }

  /* Adicionando as colunas da chave primária da entidade forte na chave
     primária da entidade fraca */
  count=ref_columns.size();
  for(i=0; i < count; i++)
   pk->addColumn(ref_columns[i], Constraint::SOURCE_COLS);

  //Caso a tabela não tenha uma chave primária a mesma será atrua   ela
  if(new_pk)
   dst_tab->adicionarRestricao(pk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addUniqueKey(Tabela *ref_tab, Tabela *recv_tab)
{
 Constraint *uq=NULL;
 unsigned i, count;
 QString name, aux;

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
  count=ref_columns.size();
  i=0;

  while(i < count)
   uq->addColumn(ref_columns[i++], Constraint::SOURCE_COLS);

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  name=ref_tab->getName() + SUFFIX_SEPARATOR + "uq";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave única. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(recv_tab->obterRestricao(name + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave única
  uq->setName(name + aux);

  /* Após configurada a chave única que define o
     relacionamento é adicionado na tabela */
  recv_tab->adicionarRestricao(uq);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addForeignKey(Tabela *ref_tab, Tabela *recv_tab, TipoAcao del_act, TipoAcao upd_act)
{
 Constraint *pk=NULL, *pk_aux=NULL, *fk=NULL;
 unsigned i, i1, qtd;
 Column *column=NULL, *column_aux=NULL;
 QString name, aux;

 try
 {
  //Aloca uma chave estrangeira para ser configurada
  if((rel_type==RELATIONSHIP_NN) ||
     (!fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)))
  {
   fk=new Constraint;
   fk->setDeferrable(this->deferrable);
   fk->setDeferralType(this->deferral_type);
   fk->setConstraintType(TipoRestricao::foreign_key);
   fk->setAddedByLinking(true);
   //Define a tabela de destino da chave estrangeira
   fk->setReferencedTable(ref_tab);

   /* Caso o relacionamento seja 1-1 ou 1-n a chave estrangeira alocada
      será atribud   chave estrangeira que representa o relacionamento */
   if(rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)
    fk_rel1n=fk;
  }

  //Configura a ação de ON DELETE e ON UPDATE da chave estrangeira
  fk->setActionType(del_act, false);
  fk->setActionType(upd_act, true);

  /* Obtém a chave primária da tabela de origem para ser referenciada
     pela chave estrangeira */
  pk=ref_tab->obterChavePrimaria();

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
   if((!isSelfRelationship() && ref_tab==src_table) ||
      (isSelfRelationship() && table_relnn->obterNumRestricoes()==0))
    qtd-=dynamic_cast<Tabela *>(dst_table)->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS);
   //Caso 2: deslocando o índice de varredura
   else if(ref_tab==dst_table)
   {
    pk_aux=dynamic_cast<Tabela *>(src_table)->obterChavePrimaria();
    i=pk_aux->getColumnCount(Constraint::SOURCE_COLS);
   }
  }

  while(i < qtd)
  {
   //Obtém um coluna referenciada
   column=ref_columns[i];
   //Obtém uma coluna da chave primária da tabela de origem
   column_aux=pk->getColumn(i1, Constraint::SOURCE_COLS);
   //Faz a ligação das colunas na chave estrangeira
   fk->addColumn(column, Constraint::SOURCE_COLS);
   fk->addColumn(column_aux, Constraint::REFERENCED_COLS);
   i++; i1++;
  }

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  name=ref_tab->getName() + SUFFIX_SEPARATOR + "fk";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave estrangeira. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(recv_tab->obterRestricao(name + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave estrangeira
  fk->setName(name + aux);

  /* Após configurada a chave estrangeira que define o
     relacionamento é adicionado na tabela */
  recv_tab->adicionarRestricao(fk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addAttributes(Tabela *recv_tab)
{
 unsigned i, count, i1;
 Column *column=NULL;
 QString name, aux;

 try
 {
  //Adicionando os atributos do relacionamento na tabela
  count=rel_attributes.size();
  aux[0]='\0';

  for(i=0, i1=1; i < count; i++)
  {
   //Obtém o atributo
   column=dynamic_cast<Column *>(rel_attributes[i]);

   //Caso o atributo já pertença a uma tabela interrompe o processamento
   if(column->getParentTable())
    break;

   name=column->getName();

   /* Verifica o se o nome da coluna já não existe na tabela. Equanto
      existir, incrementa e concatena um número (i1) ao final do nome,
      até que este nome não exista nas colunas da tabela onde será
      inserido o atributo */
   while(recv_tab->obterColuna(name + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }
   //Define o nome do atributo
   column->setName(name + aux);
   aux[0]='\0';

   //Adiciona o atributo na tabela
   recv_tab->adicionarColuna(column);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::copyColumns(Tabela *ref_tab, Tabela *recv_tab, bool not_null)
{
 Constraint *dst_pk=NULL, *src_pk=NULL, *pk=NULL;
 unsigned i, count, i1;
 Column *column=NULL, *column_aux=NULL;
 QString name, suffix, aux, prev_name;

 try
 {
  //Obtém as chaves primárias das tabelas de origem e destino
  dst_pk=recv_tab->obterChavePrimaria();
  pk=src_pk=ref_tab->obterChavePrimaria();

   /* Selecionando a lista de colunas correta de acordo com a forma do relacionamento.
     Caso a tabela a qual receberá a chave estrangeira (tab_dest) for uma
     referênci  tabela de origem do relacionamento, o sufixo das colunas a serem criadas
     será configurado como sendo o sufixo da tabela de origem. Caso contrário  o
      será o da própria tabela de destino. */
  if(auto_suffix)
  {
   if(rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)
   {
    suffix=SUFFIX_SEPARATOR + ref_tab->getName();

    if(ref_tab==src_table)
     src_suffix=suffix;
    else
     dst_suffix=suffix;
   }
   else if(rel_type==RELATIONSHIP_NN)
   {
    if(ref_tab==dst_table)
    suffix=dst_suffix=SUFFIX_SEPARATOR + dst_table->getName();
   else
    suffix=src_suffix=SUFFIX_SEPARATOR + src_table->getName();
   }
  }
  else if(((rel_type!=RELATIONSHIP_NN && recv_tab==src_table) ||
           (rel_type==RELATIONSHIP_NN && ref_tab==dst_table))
      && !dst_suffix.isEmpty())
   suffix=SUFFIX_SEPARATOR + dst_suffix;
  else if(((rel_type!=RELATIONSHIP_NN && recv_tab==dst_table) ||
           (rel_type==RELATIONSHIP_NN && ref_tab==src_table))
           && !src_suffix.isEmpty())
   suffix=SUFFIX_SEPARATOR + src_suffix;

  /* Caso o relacionamento seja 1-n e a tabela de origem não possua
     uma chave primária ou se o relacionamento seja n-n e ambas as tabelas
     não possuam chave primária, uma exceção será disparada */
  if((!src_pk && (rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)) ||
     (!src_pk && !dst_pk && rel_type==RELATIONSHIP_NN))
   throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                          .arg(QString::fromUtf8(this->obj_name))
                          .arg(QString::fromUtf8(ref_tab->getName(true)))
                          .arg(QString::fromUtf8(recv_tab->getName(true))),
                 ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);


  /* Obtém a quantidade de colunas referenciadas na chave
     primária selecionada */
  count=pk->getColumnCount(Constraint::SOURCE_COLS);

  /* Varre a chave primária, obtendo as colunas e adicionando na lista
     de colunas selecionada acima. Isso é feito para se saber quais são
     as colunas da tabela, dona da chave primária, que serão usadas
    no caso de se criar chaves estrangeiras */
  for(i=0; i < count; i++)
  {
   i1=1;
   aux="";

   //Aloca uma nova coluna
   column=new Column;
   ref_columns.push_back(column);

   /* Copia o conteúdo da coluna da chave primária no indice i para
       a nova coluna criada */
   column_aux=pk->getColumn(i, Constraint::SOURCE_COLS);
   pk_columns.push_back(column_aux);

   (*column)=(*column_aux);
   column->setNotNull(not_null);

   //Obtém o nome anterior da coluna antes da desconexão do relacionamento
   prev_name=prev_ref_col_names[column->getObjectId()];

   //Protege a nova coluna, evitando que o usuário a modifique ou remova
   column->setAddedByLinking(true);

   //Desfaz a referência da coluna a uma tabela pai
   column->setParentTable(NULL);

   /* Caso o tipo da nova coluna seja "serial" o mesmo será
      convertido para "integer" */
   if(column->getType()=="serial")
    column->setType(TipoPgSQL("integer"));
   else if(column->getType()=="bigserial")
    column->setType(TipoPgSQL("bigint"));
   /* O nome da nova coluna, será o nome original concatenado
      com o sufixo da tabela a qual ela pertence. Isso é feito
      para se saber de onde tal coluna foi originada */
   name=column->getName() + suffix;

   /* Verifica se o nome da coluna já não existe na tabela
      na qual será inserida, caso exista, um número será concatenado
      ao final de seu nome. Enquanto esta condição for verdadeira
      este número (i1) será incrementado até que não exista uma coluna
      com tal nome (nome original + sufixo + número) */
   while(recv_tab->obterColuna(name + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }

   //Armazena o sufixo gerado para validações posteriores do relacionamento
   col_suffixes.push_back(suffix + aux);

   //Concatena a string auxiliar ao nome inteiro da coluna
   name+=aux;

   //Primeiramente a coluna é renomeada com seu nome antigo para manter o histórico
   if(prev_name!="")
    column->setName(prev_name);

   /* Define o nome da coluna como sendo o nome configurado acima, desta forma a
      coluna passará a ter como nome antigo o nome atribuido na iteração acima */
   column->setName(name);

   /* Caso o nome anteriro atribuíd  coluna seja diferente do nome atual, o nome
      atual da coluna passará a ser o nome antigo da mesma quando o relacionamento
      for desconectado e reconectado novamente, desta forma o histórico de nomes da
      colunas não se perde mesmo quando as colunas do relacionamento são desalocadas,
      isso evita a quebra de referências as colunas criadas pelo relacionamento.
      Essa operação só é executada para relacionamentos 1-n e 1-n para relacionamentos
      n-n as colunas são sempre recriadas sem a necessidade de manter o histórico pois
      o usuário não consegue referenciar as colunas criadas pelos relacionamentos n-n.*/
    if(prev_name!=name && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
     prev_ref_col_names[column->getObjectId()]=column->getName();

   /* Adiciona a coluna na tabela a qual foi definida para receber os
      atributos, colunas e restições */
   recv_tab->adicionarColuna(column);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relationship::addColumnsRel11(void)
{
 Tabela *ref_tab=NULL, *recv_tab=NULL;

 try
 {
  TipoAcao del_action;

  ref_tab=dynamic_cast<Tabela *>(this->getReferenceTable());
  recv_tab=dynamic_cast<Tabela *>(this->getReceiverTable());

  //Caso a tabela de referência seja obrigatória seta como RESTRICT a ação de delete na chave estrangeira
  if((ref_tab==this->src_table && this->isTableMandatory(SRC_TABLE)) ||
     (ref_tab==this->dst_table && this->isTableMandatory(DST_TABLE)))
    del_action=TipoAcao::restrict;
  else
    del_action=TipoAcao::set_null;

  if(isSelfRelationship())
  {
   addAttributes(recv_tab);
   addConstraints(recv_tab);
   copyColumns(ref_tab, recv_tab, false);
   addForeignKey(ref_tab, recv_tab, del_action, TipoAcao::cascade);
   addUniqueKey(ref_tab, recv_tab);
  }
  else
  {
   copyColumns(ref_tab, recv_tab, false);

   if(identifier)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->setMandatoryTable(DST_TABLE, false);
    this->setMandatoryTable(SRC_TABLE, false);

    if(ref_tab==this->src_table)
     this->setMandatoryTable(SRC_TABLE, true);
    else
     this->setMandatoryTable(DST_TABLE, true);

    configureIndentifierRel(recv_tab);
   }

   addAttributes(recv_tab);
   addConstraints(recv_tab);

   if(identifier)
   {
    addForeignKey(ref_tab, recv_tab, TipoAcao::cascade, TipoAcao::cascade);
   }
   else
   {
    addForeignKey(ref_tab, recv_tab, del_action,  TipoAcao::cascade);
    addUniqueKey(ref_tab, recv_tab);
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
 Tabela *ref_tab=NULL, *recv_tab=NULL;
 bool not_null=false;
 TipoAcao del_action=TipoAcao::set_null, upd_action=TipoAcao::cascade;

 try
 {
  /* Para relacionamentos 1-n a ordem das tabelas não se alteram, ou seja,
     as colunas da chave estrangeira são sempre adicionadas na tabela
     de origem */
  recv_tab=dynamic_cast<Tabela *>(this->getReceiverTable());
  ref_tab=dynamic_cast<Tabela *>(this->getReferenceTable());

  /* Caso o relacionamento não seja identificador e a participação da tabela
     de referência (origem) seja obrigatória (1,1)--<>--(0|1,n) as colunas da chave estrangeiras
     não podem aceitar valores nulos e além disso as ações ON DELETE e ON UPDATE
     será RESTRIC */
  if(!identifier && src_mandatory)
  {
   if(!deferrable)
    del_action=TipoAcao::restrict;
   else
    del_action=TipoAcao::no_action;

   not_null=true;
  }
  /* Caso o relacionamento seja identificador configura as ações ON DELETE e ON UPDATE
     da chave estrangeira como "cascade" pois a entidade fraca só existe se
     a entidade forte existir, isto é, se uma tupla da tabela "entidade forte" for
     removida todas as tuplas da tabela "entidade fraca" também serão excluídas */
  else if(identifier)
   del_action=TipoAcao::cascade;

  if(isSelfRelationship())
  {
   addAttributes(recv_tab);
   addConstraints(recv_tab);
   copyColumns(ref_tab, recv_tab, not_null);
   addForeignKey(ref_tab, recv_tab, del_action, upd_action);
  }
  else
  {
   copyColumns(ref_tab, recv_tab, not_null);

   if(identifier)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->setMandatoryTable(SRC_TABLE, true);
    this->setMandatoryTable(DST_TABLE, false);

    configureIndentifierRel(recv_tab);
   }

   addAttributes(recv_tab);
   addConstraints(recv_tab);

   addForeignKey(ref_tab, recv_tab, del_action, upd_action);
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
 bool src_not_null=false, dst_not_null=false;
 TipoAcao acao_del_orig=TipoAcao::restrict, acao_del_dest=TipoAcao::restrict,
          acao_upd_orig=TipoAcao::cascade, acao_upd_dest=TipoAcao::cascade;
 unsigned i, count;

 try
 {
  /* Para relacionamentos n-n, a tabela de destino onde serão adicionadas as colunas
     será a tabela que representa o relacionamento (tabela_relnn). As tabelas
     participantes do relacionamento sempre serão as tabelas de origem pois as colunas
     destas precisam ser adicionadas na tabela que define o relacionamento */
  tab=dynamic_cast<Tabela *>(src_table);
  tab1=dynamic_cast<Tabela *>(dst_table);

  copyColumns(tab, table_relnn, src_not_null);
  copyColumns(tab1, table_relnn, dst_not_null);

  /* Cria a chave primária padrão da tabela que consiste nas colunas que
     identificam cada chave estrangeira na tabela. */
  pk_tabnn=new Constraint;
  pk_tabnn->setName(table_relnn->getName() + "_pk");
  pk_tabnn->setConstraintType(TipoRestricao::primary_key);
  pk_tabnn->setAddedByLinking(true);
  count=ref_columns.size();

  for(i=0; i < count; i++)
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

void Relationship::removeTableObjectsRefCols(Tabela *table)
{
 Gatilho *trigger=NULL;
 Index *index=NULL;
 Constraint *constr=NULL;
 int i, count;

 //Remove os gatilhos que referenciam alguma coluna inc. por relação
 count=table->obterNumGatilhos();
 for(i=0; i < count; i++)
 {
  trigger=table->obterGatilho(i);
  if(trigger->isReferRelationshipColumn())
  {
   table->removerObjeto(trigger);
   delete(trigger);
   count--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove os índices que referenciam alguma coluna inc. por relação
 count=table->obterNumIndices();
 for(i=0; i < count; i++)
 {
  index=table->obterIndice(i);
  if(index->isReferRelationshipColumn())
  {
   table->removerObjeto(index);
   delete(index);
   count--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove as restrições que referenciam alguma coluna inc. por relação
 count=table->obterNumRestricoes();
 for(i=0; i < count; i++)
 {
  constr=table->obterRestricao(i);
  if(!constr->isAddedByRelationship() &&
     constr->getConstraintType()!=TipoRestricao::primary_key &&
     constr->isReferRelationshipColumn())
  {
   table->removerObjeto(constr);
   delete(constr);
   count--; i--;
   if(i < 0) i=0;
  }
 }
}

void Relationship::removeColumnsFromTablePK(Tabela *table)
{
 if(table)
 {
  Constraint *pk=NULL;
  Column *column=NULL;
  unsigned i, count;

  /* Obtém a chave primáira da tabela e remove desta
     as colunas adicionadas por relacionamento */
  pk=table->obterChavePrimaria();

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
   count=pk->getColumnCount(Constraint::SOURCE_COLS);

   for(i=0; i < count; i++)
   {
    //Obtém uma coluna da chave primária
    column=pk->getColumn(i, Constraint::SOURCE_COLS);

    /* Caso a coluna foi incluída por relacionamento e a mesma pertence
       ao próprio relacionamento */
    if(column->isAddedByRelationship() &&
      (isColumnExists(column) || getObjectIndex(column) >= 0))
    {
     //Remove a coluna da chave primária
     pk->removeColumn(column->getName(), Constraint::SOURCE_COLS);
     i--; count--;
    }
   }
  }
 }
}

void Relationship::disconnectRelationship(bool rem_tab_objs)
{
 try
 {
  if(connected)
  {
   vector<Column *>::iterator itr, itr_end;
   Column *column=NULL;
   Tabela *table=NULL;
   unsigned list_idx=0;
   vector<TableObject *> *attr_list=NULL;
   vector<TableObject *>::iterator itr_atrib, itr_atrib_end;
   TableObject *tab_obj=NULL;

   if(rel_type==RELATIONSHIP_GEN ||
      rel_type==RELATIONSHIP_DEP)
   {
    table=getReceiverTable();

    if(rem_tab_objs)
     removeTableObjectsRefCols(table);

     removeColumnsFromTablePK(table);

    //Remove a tabela cópia/pai dependendo do tipo do relacionamento
    if(rel_type==RELATIONSHIP_GEN)
     table->removerObjeto(getReferenceTable()->getName(true), OBJ_TABLE);
    else
     table->removerObjeto(getReferenceTable()->getName(true), BASE_TABLE);
   }
   else
   {
    Constraint *pk=NULL, *constr=NULL;
    unsigned i, count;

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
     table=dynamic_cast<Tabela *>(fk_rel1n->getParentTable());

     //Remove a chave estrangeira da tabela
     table->removerRestricao(fk_rel1n->getName());

     /* Obtém a chave primária da tabela para checar se a mesma é igual a chave primária
        que define o relacionamento identificador */
     pk=table->obterChavePrimaria();

     //Remove as colunas criadas pelo relacionamento da chave primária da tabela
     removeColumnsFromTablePK(table);

     if(rem_tab_objs)
      removeTableObjectsRefCols(table);

     /* Remove as colunas da chave estrangeira e a desaloca, apenas para
        relacionamentos 1-1 e 1-n. */
     fk_rel1n->removeColumns();
     delete(fk_rel1n);
     fk_rel1n=NULL;

     if(uq_rel11)
     {
      //Remove a chave única da tabela
      table->removerRestricao(uq_rel11->getName());
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
      table=dynamic_cast<Tabela *>(pk_relident->getParentTable());
      //Remove a chave primária da tabela
      table->removerRestricao(pk_relident->getName());

      //Desaloca a chave primária
      delete(pk);
      pk_relident=NULL;
     }
    }
    else if(rel_type==RELATIONSHIP_NN)
    {
     /* Caso o relacionamento seja n-n, apenas remove as restrições sem
        qualquer validação */
     count=table_relnn->obterNumRestricoes();

     for(i=0; i < count ; i++)
     {
      //Obtém a restrição
      constr=table_relnn->obterRestricao(i);

      /* Caso a restrição foi adicionada por lgação porém não é uma restrição criada pelo usuário e
         e sim uma restrição criada quando o relacionamento é conectado (ex. chave primária e chaves estrangeiras).
         A segunda parte da condição obterIndiceObjeto(rest) < 0 verifica se a restrição a ser removida não fazer
         parte da lista de restrições criadas pelo usuário, caso faça parte, não será destruída */
      if(constr->isAddedByRelationship() && getObjectIndex(constr) < 0)
      {
       //Remove a restrição da tabela
       table_relnn->removerRestricao(constr->getName());
       i--; count--;
       delete(constr);
      }
     }
    }
   }

   /* Remover os atributos e restrições do relacionamento os quais
      estão incluídos nas tabelas participantes. */
   table=getReceiverTable();
   while(list_idx <= 1)
   {
    /* Seleciona a lista de objetos, a primeira lista sempre deve ser a
       de restrições para evitar de se remover colunas antes das restrições
       gerando erro de remoção de coluna referenciada */
    attr_list=(list_idx==0 ? &rel_constraints : &rel_attributes);

    itr_atrib=attr_list->begin();
    itr_atrib_end=attr_list->end();

    //Varre a lista de atributos selecionada
    while(itr_atrib!=itr_atrib_end)
    {
     //Obtém o atributo
     tab_obj=(*itr_atrib);
     //tipo_obj=obj_tab->obterTipoObjeto();

     /* É necessário verificar se o objeto pertence ao relacionamento
        a fim de evitar que atributos adicionados por outros relacionamentos
          tabela também sejam removidos */
     if(table && getObjectIndex(tab_obj) >= 0)
     {
      //Remove o atributo da tabela através do nome e tipo
      table->removerObjeto(tab_obj->getName(), tab_obj->getObjectType());
      tab_obj->setParentTable(NULL);
     }
     //Para para o atributo posterior
     itr_atrib++;
    }
    //Passa para a lista seguinte de atributos
    list_idx++;
   }

   //Excluindo as colunas incluídas na tabela pelo relacionamento
   itr=ref_columns.begin();
   itr_end=ref_columns.end();

   /*  Varre a lista de colunas do relacionamento removendo cada uma
      da tabela onde foi inserida e desalocando o espaço que esta
      ocupa em memória */
   while(itr!=itr_end)
   {
    column=(*itr);
    //Remove a coluna da tabela pai
    table->removerColuna(column->getName());
    itr++;
    //Desaloca a coluna
    delete(column);
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
 Constraint *constr=NULL;
 bool found=false;

 itr=rel_constraints.begin();
 itr_end=rel_constraints.end();

 //Varre a lista de restrições
 while(itr!=itr_end && !found)
 {
  //Obtém uma restrição da lista
  constr=dynamic_cast<Constraint *>(*itr);
  /* Um relacionamento será considerado possuir um
     atributo identificandor quando for encontrada
     uma chave primária em sua lista de restrições */
  found=(constr->getConstraintType()==TipoRestricao::primary_key);
  itr++;
 }

 return(found);
}

bool Relationship::isInvalidated(void)
{
 unsigned rel_cols_count, tab_cols_count, i, i1, count;
 Tabela *table=NULL, *table1=NULL;
 Constraint *fk=NULL, *fk1=NULL, *constr=NULL, *pk=NULL;
 bool valid=false;
 Column *col1=NULL, *col2=NULL, *col3=NULL;
 QString col_name;

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
   table=getReferenceTable();

   //Obtém a quantidade de colunas da chave estrangeira que representa a relação
   rel_cols_count=fk_rel1n->getColumnCount(Constraint::SOURCE_COLS);

   //O relacionamento estára invalidado caso a tabela referência não possua uma chave primária
   pk=table->obterChavePrimaria();

   if(pk)
   {
    //Obtém a quantidade de colunas da chave primária da tabela
    tab_cols_count=pk->getColumnCount(Constraint::SOURCE_COLS);

    //Obtém a tabela de referencia do relacionamento
    table1=getReceiverTable();

    //Faz a verificação de quantidade de colunas
    valid=(rel_cols_count==tab_cols_count);

    //A próxima validação é a de nome e tipo das colunas
    for(i=0; i < rel_cols_count && valid; i++)
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
     col_name=col1->getName() + col_suffixes[i];
     valid=(col1==col3 &&
             (col_name==col2->getName()) &&
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
   table=getReferenceTable();

   //Obtém a tabela de referencia do relacionamento
   table1=getReceiverTable();

   /* Obtém o número total de colunas do relacionamento, incluído as
      que foram adicionadas por relacionamentos */
   tab_cols_count=table->obterNumColunas();

   /* Obtém a quantidade de colunas criadas com a conexão da relação somando-se
      com a quantidade de colunas rejeitadas no momento da ligaçã de acordo
      com as regras */
   rel_cols_count=ref_columns.size() + rejected_col_count;
   valid=(rel_cols_count == tab_cols_count);

   /* Checando se as colunas criadas com a herança/dependência ainda existem
      na tabela de referência */
   for(i=0; i < ref_columns.size() && valid; i++)
    valid=table->obterColuna(ref_columns[i]->getName(true));

   /* Checando se as colunas da tabela referência existem na tabela
      receptora. Na teoria todas as colunas devem existir pois uma
      herda a outra logo todos possuirão as mesmas colunas. Caso isso
      não acontença indica que uma coluna da tabela de referência foi
      renomeada */
   for(i=0; i < tab_cols_count && valid; i++)
    valid=table1->obterColuna(table->obterColuna(i)->getName(true));
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
   table=dynamic_cast<Tabela *>(src_table);
   table1=dynamic_cast<Tabela *>(dst_table);

   /* Para se validado um relacionamento n-n, a primeira condição é que
      ambas as tabelas possuam chave estrangeira */
   if(table->obterChavePrimaria() && table1->obterChavePrimaria())
   {
    //Obtém a quantidade de restrições da tabela criada pelo relacionamento
    count=table_relnn->obterNumRestricoes();

    //Varre a lista de restrições da tabela
    for(i=0; i < count; i++)
    {
     //Obtém uma restrição
     constr=table_relnn->obterRestricao(i);

     //Caso a mesma seja uma chave estrangeira
     if(constr->getConstraintType()==TipoRestricao::foreign_key)
     {
      /* Verifica se a tabela referenciada pela chave é a tabela de origem
         caso seja, armazena seu endereço na referêni   chave estrangeira
         da origem */
      if(!fk && constr->getReferencedTable()==table)
       fk=constr;
      else if(!fk1 && constr->getReferencedTable()==table1)
       fk1=constr;
     }
    }

    /* A quantidade de colunas do relacionamento é calculada pela soma
       das quantidades de colunas das chaves estrangeiras obtidas */
    rel_cols_count=fk->getColumnCount(Constraint::REFERENCED_COLS) + fk1->getColumnCount(Constraint::REFERENCED_COLS);
    /* A quantidade de colunas da tabela é obtida pela soma da quantidade
       de colunas das chaves primárias envolvidas no relacionamentos */
    tab_cols_count=table->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS) +
                 table1->obterChavePrimaria()->getColumnCount(Constraint::SOURCE_COLS);
    valid=(rel_cols_count == tab_cols_count);

    /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de referência */
    count=fk->getColumnCount(Constraint::SOURCE_COLS);
    for(i=0; i < count && valid; i++)
    {
     col_name=fk->getColumn(i, Constraint::SOURCE_COLS)->getName();

     /* Caso o sufixo da origem esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de origem */
     if(!col_suffixes[i].isEmpty())
      col_name=col_name.remove(col_suffixes[i]);

     //Verifica a existencia da coluna na tabela
     col1=table->obterColuna(col_name);
     valid=col1 &&
            table->obterChavePrimaria()->isColumnExists(col1, Constraint::SOURCE_COLS);
    }

   /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de receptora */
    i1=count;
    count+=fk1->getColumnCount(Constraint::SOURCE_COLS);
    for(i=0; i1 < count && valid; i1++)
    {
     col_name=fk1->getColumn(i++, Constraint::SOURCE_COLS)->getName();

     /* Caso o sufixo do destino esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de destino */
     if(!col_suffixes[i1].isEmpty())
      col_name=col_name.remove(col_suffixes[i1]);

     //Verifica a existencia da coluna na tabela
     col1=table1->obterColuna(col_name);
     valid=col1 &&
            table1->obterChavePrimaria()->isColumnExists(col1, Constraint::SOURCE_COLS);
    }
   }
  }
  return(!valid);
 }
 else
  return(true);
}

QString Relationship::getCodeDefinition(unsigned def_type)
{
 if(def_type==SchemaParser::SQL_DEFINITION)
 {
  if(fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
  {
   unsigned count, i;

   attributes[ParsersAttributes::RELATIONSHIP_1N]="1";
   attributes[ParsersAttributes::CONSTRAINTS]=fk_rel1n->getCodeDefinition(def_type);

   if(uq_rel11)
    attributes[ParsersAttributes::CONSTRAINTS]+=uq_rel11->getCodeDefinition(def_type);

   count=rel_constraints.size();
   for(i=0; i < count; i++)
   {
    if(dynamic_cast<Constraint *>(rel_constraints[i])->getConstraintType()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
                                              getCodeDefinition(def_type, false);

   }

   attributes[ParsersAttributes::TABLE]=getReceiverTable()->getName(true);
  }
  else if(table_relnn && rel_type==RELATIONSHIP_NN)
  {
   unsigned count, i;

   attributes[ParsersAttributes::RELATIONSHIP_NN]="1";
   attributes[ParsersAttributes::TABLE]=table_relnn->getCodeDefinition(def_type);

   count=table_relnn->obterNumRestricoes();
   for(i=0; i < count; i++)
   {
    if(table_relnn->obterRestricao(i)->getConstraintType()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=table_relnn->obterRestricao(i)->getCodeDefinition(def_type, true);
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
  unsigned count, i;
  bool reduced_form;

  setRelationshipAttributes();
  attributes[ParsersAttributes::SRC_SUFFIX]=(!auto_suffix ? src_suffix : "");
  attributes[ParsersAttributes::DST_SUFFIX]=(!auto_suffix ? dst_suffix : "");
  attributes[ParsersAttributes::IDENTIFIER]=(identifier ? "1" : "");
  attributes[ParsersAttributes::DEFERRABLE]=(deferrable ? "1" : "");
  attributes[ParsersAttributes::AUTO_SUFFIX]=(auto_suffix ? "1" : "");
  attributes[ParsersAttributes::DEFER_TYPE]=~deferral_type;
  attributes[ParsersAttributes::TABLE_NAME]=tab_name_relnn;


  attributes[ParsersAttributes::COLUMNS]="";
  count=rel_attributes.size();
  for(i=0; i < count; i++)
  {
   attributes[ParsersAttributes::COLUMNS]+=dynamic_cast<Column *>(rel_attributes[i])->
                    getCodeDefinition(SchemaParser::XML_DEFINITION);
  }

  attributes[ParsersAttributes::CONSTRAINTS]="";
  count=rel_constraints.size();
  for(i=0; i < count; i++)
  {
   if(!rel_constraints[i]->isProtected())
    attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
                       getCodeDefinition(SchemaParser::XML_DEFINITION, true);
  }

  count=column_ids_pk_rel.size();
  for(i=0; i < count; i++)
  {
   //Armazena o nome das colunas da chave primária especial se houver
   if(!ref_columns.empty() && i < ref_columns.size())
   {
    attributes[ParsersAttributes::SPECIAL_PK_COLS]+=QString("%1").arg(column_ids_pk_rel[i]);
    if(i < count-1) attributes[ParsersAttributes::SPECIAL_PK_COLS]+=",";
   }
  }

  /* Caso não haja colunas, restrições e linha definida no relacionamento
     a definição XML do rel. será em forma reduzida */
  reduced_form=(attributes[ParsersAttributes::COLUMNS].isEmpty() &&
                  attributes[ParsersAttributes::CONSTRAINTS].isEmpty() &&
                  attributes[ParsersAttributes::POINTS].isEmpty() &&
                  attributes[ParsersAttributes::SPECIAL_PK_COLS].isEmpty());


  return(this->BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION, reduced_form));
 }
}

void Relationship::setAutomaticSuffix(bool value)
{
 this->invalidated=(this->auto_suffix!=value);
 this->auto_suffix=value;
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

