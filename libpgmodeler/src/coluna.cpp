#include "coluna.h"

Coluna::Coluna(void)
{
 obj_type=OBJ_COLUMN;
 nao_nulo=false;
 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::DEFAULT_VALUE]="";
 attributes[ParsersAttributes::NOT_NULL]="";
 attributes[ParsersAttributes::TABLE]="";
}

void Coluna::setName(const QString &nome)
{
 try
 {
  QString nome_ant;

  //Obtém o nome atual da coluna o qual passará a ser o antigo
  nome_ant=this->obj_name;

  //Tenta definir o novo nome da coluna
  BaseObject::setName(nome);
  /* Caso nenhuma exceção foi disparada atribui o nome anterior
     obtido ao respectivo atributo da coluna */
  this->nome_antigo=nome_ant;
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Coluna::definirTipo(TipoPgSQL tipo)
{
 /* Caso o tipo a ser atribuíd  coluna seja um pseudo-tipo
    será disparada uma exceção */
 if(tipo.pseudoTipo())
  throw Exception(ERR_ASG_PSDTYPE_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  this->tipo=tipo;
}

void Coluna::definirValorPadrao(const QString &valor)
{
 valor_padrao=valor;
}

void Coluna::definirNaoNulo(bool valor)
{
 nao_nulo=valor;
}

TipoPgSQL Coluna::obterTipo(void)
{
 return(tipo);
}

bool Coluna::naoNulo(void)
{
 return(nao_nulo);
}

QString Coluna::obterReferenciaTipo(void)
{
 if(parent_table)
  return(parent_table->getName(true) + QString(".") + this->getName(true) + QString("%TYPE"));
 else
  return("");
}

QString Coluna::obterValorPadrao(void)
{
 return(valor_padrao);
}

QString Coluna::getNameAntigo(bool formatar)
{
 if(formatar)
  return(BaseObject::formatName(nome_antigo));
 else
  return(nome_antigo);
}

QString Coluna::getCodeDefinition(unsigned tipo_def)
{
 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::TYPE]=tipo.obterDefinicaoObjeto(tipo_def);
 attributes[ParsersAttributes::DEFAULT_VALUE]=valor_padrao;
 attributes[ParsersAttributes::NOT_NULL]=(!nao_nulo ? "" : "1");
 return(BaseObject::__getCodeDefinition(tipo_def));
}

void Coluna::operator = (Coluna &coluna)
{
 this->comment=coluna.comment;
 this->object_id=coluna.object_id;
 this->is_protected=coluna.is_protected;

 this->obj_name=coluna.obj_name;
 this->nome_antigo=coluna.nome_antigo;

 this->tipo=coluna.tipo;
 this->valor_padrao=coluna.valor_padrao;

 this->parent_table=coluna.parent_table;
 /*this->inc_dependencia=coluna.inc_dependencia;
 this->inc_ligacao=coluna.inc_ligacao;
 this->inc_generalizacao=coluna.inc_generalizacao;*/
 this->add_by_copy=false;
 this->add_by_generalization=false;
 this->add_by_linking=false;
}

