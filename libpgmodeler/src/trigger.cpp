#include "trigger.h"

Gatilho::Gatilho(void)
{
 unsigned i;
 TipoEvento tipos[4]={TipoEvento::on_insert, TipoEvento::on_delete,
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
 attributes[ParsersAttributes::PER_ROW]="";
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

 events[event]=value;
}

void Gatilho::setFunction(Function *func)
{
 //Case the function is null an error is raised
 if(!func)
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_TRIGGER)),
                ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Case the function doesn't returns 'trigger' it cannot be used with the trigger thus raise an error
  if(func->getReturnType()!="trigger")
   throw Exception(ERR_ASG_INV_TRIGGER_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  //Case the functions does not has any parameter raise an error
  else if(func->getParameterCount()==0)
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
 //Raises an error if the argument index is invalid (out of bound)
 if(arg_idx>=arguments.size())
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 vector<QString>::iterator itr;

 itr=arguments.begin()+arg_idx;
 (*itr)=new_arg;
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
 //Raises an error if the argument index is invalid (out of bound)
 if(arg_idx>=arguments.size())
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(arguments[arg_idx]);
}

Column *Gatilho::getColumn(unsigned col_idx)
{
 //Raises an error if the column index is invalid (out of bound)
 if(col_idx>=upd_columns.size())
  throw Exception(ERR_REF_COLUMN_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
 //Raises an error if the argument index is invalid (out of bound)
 if(arg_idx>=arguments.size())
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 vector<QString>::iterator itr;
 itr=arguments.begin()+arg_idx;
 arguments.erase(itr);
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
 //If the referenced table isn't valid raises an error
 if(ref_table && ref_table->getObjectType()!=OBJ_TABLE)
  throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

 while(itr!=itr_end && !enc)
 {
  col=(*itr);
  enc=col->isAddedByRelationship();
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

 /* Case the trigger doesn't referece some column added by relationship it will be declared
    inside the parent table construction by the use of 'decl-in-table' schema attribute */
 if(!isReferRelationshipColumn())
  attributes[ParsersAttributes::DECL_IN_TABLE]="1";

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::FIRING_TYPE]=(~firing_type);

 //** Constraint trigger MUST execute per row **
 attributes[ParsersAttributes::PER_ROW]=((is_exec_per_row && !referenced_table) || referenced_table ? "1" : "");

 attributes[ParsersAttributes::CONDITION]=condition;

 if(referenced_table)
 {
  attributes[ParsersAttributes::REF_TABLE]=referenced_table->getName(true);
  attributes[ParsersAttributes::DEFERRABLE]=(is_deferrable ? "1" : "");
  attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);
 }

 return(BaseObject::__getCodeDefinition(def_type));
}

