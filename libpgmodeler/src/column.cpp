#include "column.h"

Column::Column(void)
{
 obj_type=OBJ_COLUMN;
 not_null=false;
 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::DEFAULT_VALUE]="";
 attributes[ParsersAttributes::NOT_NULL]="";
 attributes[ParsersAttributes::TABLE]="";
}

void Column::setName(const QString &name)
{
 try
 {
  QString prev_name;

  //Obtém o nome atual da coluna o qual passará a ser o antigo
  prev_name=this->obj_name;

  //Tenta definir o novo nome da coluna
  BaseObject::setName(name);
  /* Caso nenhuma exceção foi disparada atribui o nome anterior
     obtido ao respectivo atributo da coluna */
  this->old_name=prev_name;
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Column::setType(TipoPgSQL type)
{
 /* Caso o tipo a ser atribuíd  coluna seja um pseudo-tipo
    será disparada uma exceção */
 if(type.pseudoTipo())
  throw Exception(ERR_ASG_PSDTYPE_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  this->type=type;
}

void Column::setDefaultValue(const QString &value)
{
 default_value=value;
}

void Column::setNotNull(bool value)
{
 not_null=value;
}

TipoPgSQL Column::getType(void)
{
 return(type);
}

bool Column::isNotNull(void)
{
 return(not_null);
}

QString Column::getTypeReference(void)
{
 if(parent_table)
  return(parent_table->getName(true) + QString(".") + this->getName(true) + QString("%TYPE"));
 else
  return("");
}

QString Column::getDefaultValue(void)
{
 return(default_value);
}

QString Column::getOldName(bool format)
{
 if(format)
  return(BaseObject::formatName(old_name));
 else
  return(old_name);
}

QString Column::getCodeDefinition(unsigned def_type)
{
 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::TYPE]=type.obterDefinicaoObjeto(def_type);
 attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
 attributes[ParsersAttributes::NOT_NULL]=(!not_null ? "" : "1");
 return(BaseObject::__getCodeDefinition(def_type));
}

void Column::operator = (Column &col)
{
 this->comment=col.comment;
 this->object_id=col.object_id;
 this->is_protected=col.is_protected;

 this->obj_name=col.obj_name;
 this->old_name=col.old_name;

 this->type=col.type;
 this->default_value=col.default_value;

 this->parent_table=col.parent_table;
 this->add_by_copy=false;
 this->add_by_generalization=false;
 this->add_by_linking=false;
}

