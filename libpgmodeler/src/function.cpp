#include "function.h"

Parameter::Parameter(void)
{
 obj_type=OBJ_PARAMETER;
 is_in=is_out=false;
}

void Parameter::setType(TipoPgSQL type)
{
 this->type=type;
}

void Parameter::setIn(bool value)
{
 is_in=value;
}

void Parameter::setOut(bool value)
{
 is_out=value;
}

bool Parameter::isIn(void)
{
 return(is_in);
}

bool Parameter::isOut(void)
{
 return(is_out);
}

void Parameter::operator = (const Parameter &param)
{
 this->obj_name=param.obj_name;
 this->type=param.type;
 this->default_value=param.default_value;
 this->is_in=param.is_in;
 this->is_out=param.is_out;
}

QString Parameter::getCodeDefinition(unsigned def_type)
{
 if(def_type==SchemaParser::SQL_DEFINITION)
  attributes[ParsersAttributes::NAME]=BaseObject::formatName(obj_name);
 else
  attributes[ParsersAttributes::NAME]=obj_name;

 attributes[ParsersAttributes::PARAM_IN]=(is_in ? "1" : "");
 attributes[ParsersAttributes::PARAM_OUT]=(is_out ? "1" : "");
 attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
 attributes[ParsersAttributes::TYPE]=type.obterDefinicaoObjeto(def_type);

 return(BaseObject::__getCodeDefinition(def_type));
}

unsigned Function::function_id=40000;

Function::Function(void)
{
 object_id=Function::function_id++;
 return_type=TipoPgSQL("void");
 language=NULL;
 returns_setof=false;
 is_wnd_function=false;
 obj_type=OBJ_FUNCTION;

 //Este dois valores são criados por padrão pelo SGBD
 execution_cost=100;
 row_amount=1000;

 attributes[ParsersAttributes::PARAMETERS]="";
 attributes[ParsersAttributes::EXECUTION_COST]="";
 attributes[ParsersAttributes::ROW_AMOUNT]="";
 attributes[ParsersAttributes::RETURN_TYPE]="";
 attributes[ParsersAttributes::FUNCTION_TYPE]="";
 attributes[ParsersAttributes::LANGUAGE]="";
 attributes[ParsersAttributes::RETURNS_SETOF]="";
 attributes[ParsersAttributes::SECURITY_TYPE]="";
 attributes[ParsersAttributes::BEHAVIOR_TYPE]="";
 attributes[ParsersAttributes::DEFINITION]="";
 attributes[ParsersAttributes::SIGNATURE]="";
 attributes[ParsersAttributes::REF_TYPE]="";
 attributes[ParsersAttributes::WINDOW_FUNC]="";
 attributes[ParsersAttributes::RETURN_TABLE]="";
 attributes[ParsersAttributes::LIBRARY]="";
 attributes[ParsersAttributes::SYMBOL]="";
}

void Function::setName(const QString &name)
{
 BaseObject::setName(name);
 createSignature();
}

void Function::setSchema(BaseObject *schema)
{
 BaseObject::setSchema(schema);
 createSignature();
}

void Function::addParameter(Parameter param)
{
 vector<Parameter>::iterator itr,itr_end;
 bool found=false;

 itr=parameters.begin();
 itr_end=parameters.end();

 /* Faz uma busca pela lista de parâmetros verificando se
    não existe um parâmetro com mesmo nome daquele que está
    sendo inserido. */
 while(itr!=itr_end && !found)
 {
  /* Verifica se o nome do parametro atual ('itr->nome')
     é igual ao nome do novo param. ('nome') */
  found=(itr->getName()==param.getName());
  itr++;
 }

 //Caso seja encontrado um parâmetro com mesmo nome
 if(found)
  //Dispara exceção relatando o erro
  throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PARAM_FUNCTION)
                .arg(QString::fromUtf8(param.getName()))
                .arg(QString::fromUtf8(this->signature)),
                ERR_ASG_DUPLIC_PARAM_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Insere o parâmetro na lista de parâmetros
  parameters.push_back(param);
 }
 createSignature();
}

void Function::addTableReturnType(const QString &name, TipoPgSQL type)
{
 //Verifica se o nome do elemento não está vazio
 if(name=="")
  //Caso esteja vazio, dispara uma exceção relatando o erro
  throw Exception(ERR_ASG_EMPTY_NAME_RET_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 //Caso não esteja vazio
 {
  vector<Parameter>::iterator itr,itr_end;
  bool found=false;

  itr=table_return_types.begin();
  itr_end=table_return_types.end();

  /* Faz uma busca pela lista de de retorno verificando se
     não existe um elemento com mesmo nome daquele que está
     sendo inserido. */
  while(itr!=itr_end && !found)
  {
   /* Verifica se o nome do parametro atual ('itr->nome')
      é igual ao nome do novo param. ('nome') */
   found=(itr->getName()==name);
   itr++;
  }

  //Caso seja encontrado um parâmetro com mesmo nome
  if(found)
   //Dispara exceção relatando o erro
   throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_RET_TAB_TYPE)
                 .arg(QString::fromUtf8(name))
                 .arg(QString::fromUtf8(this->signature)),
                 ERR_INS_DUPLIC_RET_TAB_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {//Dispara exceção relatando o erro
   Parameter p;

   //Insere o parâmetro na lista de parâmetros
   p.setName(name);
   p.setType(type);
   table_return_types.push_back(p);
  }
 }
}

void Function::setParametersAttribute(unsigned def_type)
{
 QString str_param;
 unsigned i, count;

 count=parameters.size();
 for(i=0; i < count; i++)
 {
  str_param+=parameters[i].getCodeDefinition(def_type);
 }

 if(def_type==SchemaParser::SQL_DEFINITION)
  str_param.remove(str_param.size()-2,2);

 attributes[ParsersAttributes::PARAMETERS]=str_param;
}

void Function::setTableReturnTypeAttribute(unsigned def_type)
{
 QString str_type;
 unsigned i, count;

 count=table_return_types.size();
 for(i=0; i < count; i++)
 {
  str_type+=table_return_types[i].getCodeDefinition(def_type);
 }

 if(def_type==SchemaParser::SQL_DEFINITION)
  str_type.remove(str_type.size()-2,2);

 attributes[ParsersAttributes::RETURN_TABLE]=str_type;
}

void Function::setExecutionCost(unsigned exec_cost)
{
 execution_cost=exec_cost;
}

void Function::setRowAmount(unsigned row_amount)
{
 this->row_amount=row_amount;
}

void Function::setLibrary(const QString &library)
{
 if(language->getName().toLower()!=~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                .arg(QString::fromUtf8(this->getSignature())),
                ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->library=library;
}

void Function::setSymbol(const QString &symbol)
{
 if(language->getName().toLower()!=~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                .arg(QString::fromUtf8(this->getSignature())),
                ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->symbol=symbol;
}

void Function::setReturnType(TipoPgSQL type)
{
 return_type=type;
}

void Function::setFunctionType(TipoFuncao func_type)
{
 function_type=func_type;
}

void Function::setLanguage(BaseObject *language)
{
 /* Caso se tente atribuir uma linguagem não alocada  função
    um erro é gerado */
 if(!language)
  throw Exception(ERR_ASG_NOT_ALOC_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso se tente inserir um objeto alocado porém que não é uma linguagem
    um erro é gerado */
 else if(language->getObjectType()!=OBJ_LANGUAGE)
  throw Exception(ERR_ASG_INV_LANGUAGE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  this->language=language;
 }
}

void Function::setReturnSetOf(bool value)
{
 returns_setof=value;
}

void Function::setWindowFunction(bool value)
{
 is_wnd_function=value;
}

void Function::setSecurityType(TipoSeguranca sec_type)
{
 security_type=sec_type;
}

void Function::setBehaviorType(TipoComportamento behav_type)
{
 behavior_type=behav_type;
}

void Function::setSourceCode(const QString &src_code)
{
 if(language->getName().toLower()==~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_CODE_FUNC_C_LANGUAGE)
                .arg(QString::fromUtf8(this->getSignature())),
                ERR_ASG_CODE_FUNC_C_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->source_code=src_code;
}

TipoPgSQL Function::getReturnType(void)
{
 return(return_type);
}

TipoFuncao Function::getFunctionType(void)
{
 return(function_type);
}

BaseObject *Function::getLanguage(void)
{
 return(language);
}

unsigned Function::getParameterCount(void)
{
 return(parameters.size());
}

unsigned Function::getTableReturnTypeCount(void)
{
 return(table_return_types.size());
}

bool Function::isReturnSetOf(void)
{
 return(returns_setof);
}

bool Function::isReturnTable(void)
{
 return(table_return_types.size() > 0);
}

bool Function::isWindowFunction(void)
{
 return(is_wnd_function);
}

TipoSeguranca Function::getSecurityType(void)
{
 return(security_type);
}

TipoComportamento Function::getBehaviorType(void)
{
 return(behavior_type);
}

QString Function::getSourceCode(void)
{
 return(source_code);
}

Parameter Function::getParameter(unsigned param_idx)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(param_idx>=parameters.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o parâmetro no índice desejado
  return(parameters[param_idx]);
}

Parameter Function::getTableReturnType(unsigned type_idx)
{
 /* Caso o índice do tipo esteja fora da capacidade
    da lista de tipos de retorno */
 if(type_idx>=table_return_types.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o item no índice desejado
  return(table_return_types[type_idx]);
}

unsigned Function::getExecutionCost(void)
{
 return(execution_cost);
}

unsigned Function::getRowAmount(void)
{
 return(row_amount);
}

QString Function::getLibrary(void)
{
 return(library);
}

QString Function::getSymbol(void)
{
 return(symbol);
}

void Function::removeParameters(void)
{
 parameters.clear();
 createSignature();
}

void Function::removeTableReturnTypes(void)
{
 table_return_types.clear();
}

void Function::removeParameter(const QString &name, TipoPgSQL type)
{
 vector<Parameter>::iterator itr,itr_end;

 itr=parameters.begin();
 itr_end=parameters.end();

 //Faz uma busca pela lista de parâmetros
 while(itr!=itr_end)
 {
  /* Verifica se o parâmetro atual (itr) tem o nome igual ao passado pelo método,
     o mesmo vale para o tipo */
  if(itr->getName()==name && itr->getType()==(~type))
  {
   //Caso o parâmetro seja encontrado
   parameters.erase(itr); //Exclui da lista
   break;
  }
  itr++;
 }

 createSignature();
}

void Function::removeParameter(unsigned param_idx)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(param_idx>=parameters.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<Parameter>::iterator itr;
  itr=parameters.begin()+param_idx;
  parameters.erase(itr); //Remove o parâmetro encontrado
 }

 createSignature();
}

void Function::removeTableReturnType(unsigned type_idx)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(type_idx>=table_return_types.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<Parameter>::iterator itr;
  itr=table_return_types.begin()+type_idx;
  table_return_types.erase(itr); //Remove o parâmetro encontrado
 }
}

QString Function::getSignature(void)
{
 return(signature);
}

void Function::createSignature(bool format)
{
 Parameter param;
 QString str_param;
 unsigned i, count;

 count=parameters.size();
 for(i=0; i < count; i++)
 {
  param=parameters[i];
  str_param+=(*param.getType());

  if(i < (count-1)) str_param+=",";
 }

 //Formato da assinatura NOME(TIPO_PARAM1,TIPO_PARAM2,...,TIPO_PARAMn)
 signature=this->getName(format) + QString("(") + str_param + QString(")");
}

QString Function::getCodeDefinition(unsigned def_type)
{
 return(this->getCodeDefinition(def_type, false));
}

QString Function::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 setParametersAttribute(def_type);

 attributes[ParsersAttributes::EXECUTION_COST]=QString("%1").arg(execution_cost);
 attributes[ParsersAttributes::ROW_AMOUNT]=QString("%1").arg(row_amount);
 attributes[ParsersAttributes::FUNCTION_TYPE]=(~function_type);

 if(def_type==SchemaParser::SQL_DEFINITION)
 {
  attributes[ParsersAttributes::LANGUAGE]=language->getName(false);
  attributes[ParsersAttributes::RETURN_TYPE]=(*return_type);
 }
 else
 {
  attributes[ParsersAttributes::LANGUAGE]=language->getCodeDefinition(def_type,true);
  attributes[ParsersAttributes::RETURN_TYPE]=return_type.obterDefinicaoObjeto(def_type);
 }

 setTableReturnTypeAttribute(def_type);

 attributes[ParsersAttributes::RETURNS_SETOF]=(returns_setof ? "1" : "");
 attributes[ParsersAttributes::WINDOW_FUNC]=(is_wnd_function ? "1" : "");
 attributes[ParsersAttributes::SECURITY_TYPE]=(~security_type);
 attributes[ParsersAttributes::BEHAVIOR_TYPE]=(~behavior_type);
 attributes[ParsersAttributes::DEFINITION]=source_code;

 if(language->getName()==~TipoLinguagem(TipoLinguagem::c))
 {
  attributes[ParsersAttributes::SYMBOL]=symbol;
  attributes[ParsersAttributes::LIBRARY]=library;
 }

 attributes[ParsersAttributes::SIGNATURE]=signature;
 return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

