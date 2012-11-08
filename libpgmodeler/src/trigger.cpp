#include "trigger.h"

Gatilho::Gatilho(void)
{
 unsigned i, tipos[4]={TipoEvento::on_insert, TipoEvento::on_delete,
                       TipoEvento::on_update, TipoEvento::on_truncate};

 function=NULL;
 is_exec_per_row=false;
 obj_type=OBJ_TRIGGER;
 referenced_table=NULL;

 for(i=0; i < 4; i++)
  events[tipos[i]]=false;

 attributes[ParsersAttributes::ARGUMENTS]="";
 attributes[ParsersAttributes::EVENTS]="";
 attributes[ParsersAttributes::TRIGGER_FUNC]="";
 attributes[ParsersAttributes::TABLE]="";
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::FIRING_TYPE]="";
 attributes[ParsersAttributes::PER_LINE]="";
 attributes[ParsersAttributes::INS_EVENT]="";
 attributes[ParsersAttributes::DEL_EVENT]="";
 attributes[ParsersAttributes::UPD_EVENT]="";
 attributes[ParsersAttributes::TRUNC_EVENT]="";
 attributes[ParsersAttributes::CONDITION]="";
 attributes[ParsersAttributes::REF_TABLE]="";
 attributes[ParsersAttributes::DEFER_TYPE]="";
 attributes[ParsersAttributes::DEFERRABLE]="";
 attributes[ParsersAttributes::DECL_IN_TABLE]="";
}

void Gatilho::addArgument(const QString &arg)
{
 arguments.push_back(arg);
}

void Gatilho::setArgumentAttribute(unsigned def_type)
{
 QString str_args;
 unsigned i, count;

 count=arguments.size();
 for(i=0; i < count; i++)
 {
  if(def_type==SchemaParser::SQL_DEFINITION)
   str_args+="'" + arguments[i] + "'";
  else
   str_args+=arguments[i];

  if(i < (count-1)) str_args+=",";
 }

 attributes[ParsersAttributes::ARGUMENTS]=str_args;
}

void Gatilho::setFiringType(TipoDisparo firing_type)
{
 this->firing_type=firing_type;
}

void Gatilho::setEvent(TipoEvento event, bool value)
{
 if(event==TipoEvento::on_select)
  throw Exception(ERR_REF_INV_TRIGGER_EVENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 events[!event]=value;
}

void Gatilho::setFunction(Function *func)
{
 //Caso a função a ser atribuida ao gatilho esteja nula
 if(!func)
  //Dispara exceção relatando o erro
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_TRIGGER)),
                ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Caso a função não possua tipo de retorno 'trigger', ela não pode ser usada em um gatilho
  if(func->getReturnType()!="trigger")
   //Dispara exceção relatando o erro
   throw Exception(ERR_ASG_INV_TRIGGER_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  //Caso a função não possua parâmetros, ela não pode ser usada em um gatilho
  else if(func->getParameterCount()==0)
   //Dispara exceção relatando o erro
   throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_TRIGGER)),
                 ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   this->function=func;
 }
}

void Gatilho::setCondition(const QString &cond)
{
 this->condition=cond;
}

void Gatilho::addColumn(Column *column)
{
 if(!column)
  throw Exception(QString(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN))
                .arg(this->getName(true))
                .arg(this->getTypeName()),
                ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(column->getParentTable() != this->getParentTable())
  throw Exception(QString(Exception::getErrorMessage(ERR_ASG_INV_COLUMN_TRIGGER))
                .arg(column->getName(true))
                .arg(this->getName(true)),
                ERR_ASG_INV_COLUMN_TRIGGER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  upd_columns.push_back(column);
}

void Gatilho::editArgument(unsigned arg_idx, const QString &new_arg)
{
 /* Caso o índice do argumento esteja fora da capacidade
    da lista de argumentos */
 if(arg_idx>=arguments.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<QString>::iterator itr;

  itr=arguments.begin()+arg_idx;
  (*itr)=new_arg; //Muda o valor do argumento
 }
}

void Gatilho::setExecutePerRow(bool value)
{
 is_exec_per_row=value;
}

bool Gatilho::isExecuteOnEvent(TipoEvento event)
{
 if(event==TipoEvento::on_select)
  throw Exception(ERR_REF_INV_TRIGGER_EVENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(events.at(!event));
}

QString Gatilho::getArgument(unsigned arg_idx)
{
 /* Caso o índice do argumento esteja fora da capacidade
    da lista de argumentos */
 if(arg_idx>=arguments.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o argumento no índice desejado
  return(arguments[arg_idx]);
}

Column *Gatilho::getColumn(unsigned col_idx)
{
 if(col_idx>=upd_columns.size())
  throw Exception(ERR_REF_COLUMN_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna a coluna no índice desejado
  return(upd_columns[col_idx]);
}

unsigned Gatilho::getArgumentCount(void)
{
 return(arguments.size());
}

unsigned Gatilho::getColumnCount(void)
{
 return(upd_columns.size());
}

Function *Gatilho::getFunction(void)
{
 return(function);
}

QString Gatilho::getCondition(void)
{
 return(condition);
}

TipoDisparo Gatilho::getFiringType(void)
{
 return(firing_type);
}

void Gatilho::removeArgument(unsigned arg_idx)
{
 /* Caso o índice do argumento steja fora da capacidade
    da lista de argumentos */
 if(arg_idx>=arguments.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<QString>::iterator itr;
  itr=arguments.begin()+arg_idx;
  arguments.erase(itr); //Remove o argumento encontrado
 }
}

void Gatilho::removeArguments(void)
{
 arguments.clear();
}

void Gatilho::removeColumns(void)
{
 upd_columns.clear();
}

void Gatilho::setReferecendTable(BaseObject *ref_table)
{
 /* Caso a tabela referenciada a ser atribuída esteja alocada, porém
    seu tipo não seja OBJETO_TABELA, isso gera um erro */
 if(ref_table && ref_table->getObjectType()!=OBJ_TABLE)
  throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui a tabela referenciada ao gatilho
 this->referenced_table=ref_table;
}

void Gatilho::setDeferralType(TipoPostergacao tipo)
{
 deferral_type=tipo;
}

void Gatilho::setDeferrable(bool valor)
{
 is_deferrable=valor;
}

BaseObject *Gatilho::getReferencedTable(void)
{
 return(referenced_table);
}

TipoPostergacao Gatilho::getDeferralType(void)
{
 return(deferral_type);
}

bool Gatilho::isDeferrable(void)
{
 return(is_deferrable);
}

bool Gatilho::isReferRelationshipColumn(void)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col=NULL;
 bool enc=false;

 itr=upd_columns.begin();
 itr_end=upd_columns.end();

 /* Efetua uma iteração verifica se as colunas da lista
    foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que o gatilho referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !enc)
 {
  //Obtém a coluna
  col=(*itr);
  //Obtém se a coluna foi incluída por relacionamento ou não
  enc=col->isAddedByRelationship();
  //Passa para a próxima coluna
  itr++;
 }

 return(enc);
}

void Gatilho::setBasicAttributes(unsigned def_type)
{
 QString str_aux,
         attribs[4]={ParsersAttributes::INS_EVENT, ParsersAttributes::DEL_EVENT,
                    ParsersAttributes::TRUNC_EVENT, ParsersAttributes::UPD_EVENT },
         sql_event[4]={"INSERT OR ", "DELETE OR ", "TRUNCATE OR ", "UPDATE   "};
 unsigned count, i, i1, event_types[4]={TipoEvento::on_insert, TipoEvento::on_delete,
                                      TipoEvento::on_truncate, TipoEvento::on_update};


 setArgumentAttribute(def_type);

 for(i=0; i < 4; i++)
 {
  if(events.at(event_types[i]))
  {
   str_aux+=sql_event[i];
   attributes[attribs[i]]="1";

   if(event_types[i]==TipoEvento::on_update)
   {
    count=upd_columns.size();
    attributes[ParsersAttributes::COLUMNS]="";
    for(i1=0; i1 < count; i1++)
    {
     attributes[ParsersAttributes::COLUMNS]+=upd_columns.at(i1)->getName(true);
     if(i1 < count-1)
      attributes[ParsersAttributes::COLUMNS]+=",";
    }
   }
  }
 }

 if(str_aux!="") str_aux.remove(str_aux.size()-3,3);
 attributes[ParsersAttributes::EVENTS]=str_aux;

 if(function)
 {
  if(def_type==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::TRIGGER_FUNC]=function->getName(true);
  else
   attributes[ParsersAttributes::TRIGGER_FUNC]=function->getCodeDefinition(def_type, true);
 }
}

QString Gatilho::getCodeDefinition(unsigned def_type)
{
 setBasicAttributes(def_type);

 /* Caso o gatilho não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração do gatilho está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!isReferRelationshipColumn())
  attributes[ParsersAttributes::DECL_IN_TABLE]="1";

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::FIRING_TYPE]=(~firing_type);

 //** Gatilhos Restrições SEMPRE devem executar por linha (FOR EACH ROW) **
 attributes[ParsersAttributes::PER_LINE]=((is_exec_per_row && !referenced_table) || referenced_table ? "1" : "");

 attributes[ParsersAttributes::CONDITION]=condition;

 if(referenced_table)
 {
  attributes[ParsersAttributes::REF_TABLE]=referenced_table->getName(true);
  attributes[ParsersAttributes::DEFERRABLE]=(is_deferrable ? "1" : "");
  attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);
 }

 return(BaseObject::__getCodeDefinition(def_type));
}

