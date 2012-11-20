#include "operator.h"

Operator::Operator(void)
{
 unsigned i;

 obj_type=OBJ_OPERATOR;

 for(i=0; i < 3; i++)
  functions[i]=NULL;

 for(i=0; i < 6; i++)
  operators[i]=NULL;

 hashes=merges=false;
 argument_types[ARG_LEFT]=TipoPgSQL("any");
 argument_types[ARG_RIGHT]=TipoPgSQL("any");

 attributes[ParsersAttributes::LEFT_TYPE]="";
 attributes[ParsersAttributes::RIGHT_TYPE]="";
 attributes[ParsersAttributes::COMMUTATOR_OP]="";
 attributes[ParsersAttributes::NEGATOR_OP]="";
 attributes[ParsersAttributes::SORT_OP]="";
 attributes[ParsersAttributes::SORT2_OP]="";
 attributes[ParsersAttributes::LESS_OP]="";
 attributes[ParsersAttributes::GREATER_OP]="";
 attributes[ParsersAttributes::RESTRICTION_FUNC]="";
 attributes[ParsersAttributes::JOIN_FUNC]="";
 attributes[ParsersAttributes::OPERATOR_FUNC]="";
 attributes[ParsersAttributes::HASHES]="";
 attributes[ParsersAttributes::MERGES]="";
 attributes[ParsersAttributes::SIGNATURE]="";
 attributes[ParsersAttributes::REF_TYPE]="";
}

bool Operator::isValidName(const QString &name)
{
 //ATENÇÃO: Não alterar a seqüência em que aparecem os caracteres.
 QString valid_chars="+-*/<>=~!@#%^&|'?";
 int pos, len;
 bool valid=true;

 //Verifica se o tamanho do nome é válido
 valid=(name.size() <= static_cast<int>(OBJECT_NAME_MAX_LENGTH));

 //Verificando se o nome é válido de acordo com as condições:
 /* 1) O nome possui apenas caracteres do conjunto
       definido por chr_validos */
 len=name.size();
 for(pos=0; pos < len && valid; pos++)
  valid=!(valid_chars.find(name[pos]) < 0);

 /* 2) O nome não possui as sequencias -- ou / * as quais
       definem início de comentário SQL */
 if(valid) valid=(name.indexOf("--") < 0);
 if(valid) valid=(name.indexOf("/*") < 0);

 /* 3) Se o nome terminar com um caractere - ou + o mesmo deve
       possuir um ou mais caracteres do conjunto ~!@#%^&|'? */
 if(name[name.size()-1]=='-' || name[name.size()-1]=='+')
 {
  /* O conjunto ~!@#%^&|'? é definido a partir da
     posição 7 do conjunto chr_validos */
  len=valid_chars.size();
  for(pos=7; pos < len && valid; pos++)
   valid=(name.indexOf(valid_chars[pos]) < 0);
 }

 return(valid);
}

void Operator::setName(const QString &name)
{
 if(name=="")
  throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(!isValidName(name))
   throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   this->obj_name=name;
 }
}

void Operator::setFunction(Function *func, unsigned func_type)
{
 //Caso o tipo de função seja inválido
 if(func_type > FUNC_RESTRICTION)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(func_type==FUNC_OPERATOR)
 {
  /* Caso a função não esteja alocada e se tente atribuí-la
     como função executada pelo operador */
  if(!func)
    throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->getName(true)))
                         .arg(BaseObject::getTypeName(OBJ_OPERATOR)),
                  ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  /* Caso o número de parâmetros da função seja inválido. Para operadores
     a mesma deve possuir 1 ou 2 parâmetros */
  else if(func->getParameterCount()==0 || func->getParameterCount() > 2)
   throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_OPERATOR)),
                 ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   unsigned param_count=func->getParameterCount();
   TipoPgSQL param_type1=TipoPgSQL("any"), param_type2=TipoPgSQL("any");

   //Obtém os prâmetros da função
   param_type1=func->getParameter(0).getType();
   if(param_count==2) param_type2=func->getParameter(1).getType();

   /* Verificando os parâmetros da função de acordo com o tipo dos
      argumentos dos operadores */

   //A função não pode possuir parametros do tipo any
   if((param_type1=="any" || (param_count==2 && param_type2=="any")) ||

      /* Caso a quantidade de parametros é 1 e ambos os argumentos do
         operador esteja com tipos configurados (operador binário) */
      (param_count==1 && argument_types[0]!="any" && argument_types[1]!="any") ||

      /* Caso a quantidade de parâmetros seja 2 verifica se ambos os argumentos
         do operador estão com tipos configurados */
      (param_count==2 && ((argument_types[0]=="any" && argument_types[1]!="any") ||
                         (argument_types[0]!="any" && argument_types[1]=="any"))) ||

    /* Quando a função possui 2 parametros os mesmo deve ser do mesmo tipo
      que os argumentos do operador */
      (param_count==2 &&
      ((argument_types[0]=="any" || argument_types[1]=="any") ||
       (argument_types[0]!=param_type1 || argument_types[1]!=param_type2))) ||

       /* Quando possui apenas 1 parâmetro o tipo do mesmo deve coincidir
          com um dos argumentos do operador, sendo que um deles deve
          ser do tipo "any" indicando que apenas um argumento foi setado para o operador */
      (param_count==1 &&
       ((argument_types[0]!="any" && argument_types[0]!=param_type1) ||
        (argument_types[1]!="any" && argument_types[1]!=param_type1))))
     throw Exception(ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }

 functions[func_type]=func;
}

void Operator::setArgumentType(TipoPgSQL arg_type, unsigned arg_id)
{
 //Caso o tipo de argumento seja inválido
 if(arg_id > ARG_RIGHT)
  throw Exception( ERR_REF_OPER_ARG_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  argument_types[arg_id]=arg_type;
}

void Operator::setOperator(Operator *oper, unsigned op_type)
{
 //Caso o tipo de operador seja inválido
 if(op_type > OPER_GREATER)
   throw Exception(ERR_REF_OPER_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  /* Validando OP Comutação: De acordo com a documentação do PostgreSQL o operador
     de comutação deve possuir seu argumet   direita do mesmo tipo do argumento
       esquerda do operador comutado. Ou seja, se o operador ++ (tipoA, tipoB)
     está sendo definido e seu operador de comutação é +*+ então a assinatura
     deste último deve ser +*+(tipoB, tipoA). A condição testa abaixo é a situação
     contrária, ou seja quando o operador de comutação não atende aos requisitos
     da documentação  */
  if(oper && op_type==OPER_COMMUTATION && argument_types[ARG_LEFT]!=oper->argument_types[ARG_RIGHT])
  {
   throw Exception(Exception::getErrorMessage(ERR_ASG_INV_COM_OPEERATOR)
                         .arg(QString::fromUtf8(oper->getSignature(true)))
                         .arg(QString::fromUtf8(this->getSignature(true))),
                 ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Validando OP Negação: De acordo com a documentação do PostgreSQL o operador
     de negação deve possuir seus argumentos do mesmo tipo dos argumentos
     do operador a ser definido. Ou seja, se o operador !! (tipoA) está sendo
     definido e seu operador de negação é !*! então a assinatura deste
     último deve ser !*!(tipoA). A condição testa abaixo é a situação contrária,
     ou seja quando o operador de negação não atende aos requisitos da documentação */
  else if(oper && op_type==OPER_NEGATION &&
          (argument_types[ARG_LEFT]!=oper->argument_types[ARG_LEFT] &&
           argument_types[ARG_RIGHT]!=oper->argument_types[ARG_RIGHT]))
  {
   throw Exception(Exception::getErrorMessage(ERR_ASG_INV_NEG_OPERATOR)
                         .arg(QString::fromUtf8(oper->getSignature(true)))
                         .arg(QString::fromUtf8(this->getSignature(true))),
                 ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  else
   operators[op_type]=oper;
 }
}

void Operator::setHashes(bool value)
{
 hashes=value;
}

void Operator::setMerges(bool value)
{
 merges=value;
}

Function *Operator::getFunction(unsigned func_type)
{
 //Caso o tipo de função seja inválido
 if(func_type > FUNC_RESTRICTION)
  throw Exception(ERR_REF_OPER_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(functions[func_type]);
}

TipoPgSQL Operator::getArgumentType(unsigned arg_id)
{
 //Caso o tipo de argumento seja inválido
 if(arg_id > ARG_RIGHT)
  throw Exception( ERR_REF_OPER_ARG_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(argument_types[arg_id]);
}

Operator *Operator::getOperator(unsigned op_type)
{
 //Caso o tipo de operador seja inválido
 if(op_type > OPER_GREATER)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(operators[op_type]);
}

bool Operator::isHashes(void)
{
 return(hashes);
}

bool Operator::isMerges(void)
{
 return(merges);
}

QString Operator::getSignature(bool format_name)
{
 QString signature, str_aux;
 unsigned i;

 signature=this->getName(format_name);

 for(i=0; i < 2; i++)
 {
  if(argument_types[i]!="any")
  {
   if(argument_types[i]=="any") str_aux+="NONE";
   else str_aux+=(*argument_types[i]);
   if(i==0) str_aux+=",";
  }
 }
 signature+="(" + str_aux + ")";
 return(signature);
}

QString Operator::getCodeDefinition(unsigned def_type)
{
 return(this->getCodeDefinition(def_type, false));
}

QString Operator::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 unsigned i;
 QString atribs_tipos[]={ParsersAttributes::LEFT_TYPE, ParsersAttributes::RIGHT_TYPE},
        atribs_ops[]={ ParsersAttributes::COMMUTATOR_OP,
                      ParsersAttributes::NEGATOR_OP,
                      ParsersAttributes::SORT_OP,
                      ParsersAttributes::SORT2_OP,
                      ParsersAttributes::LESS_OP,
                      ParsersAttributes::GREATER_OP},
        atribs_funcoes[]={ParsersAttributes::OPERATOR_FUNC,
                          ParsersAttributes::JOIN_FUNC,
                          ParsersAttributes::RESTRICTION_FUNC};

 for(i=Operator::ARG_LEFT; i <= Operator::ARG_RIGHT; i++)
 {
  if(def_type==SchemaParser::SQL_DEFINITION)
  {
   if(argument_types[i]!="any")
    attributes[atribs_tipos[i]]=(*argument_types[i]);
  }
  else
  {
   attributes[atribs_tipos[i]]=argument_types[i].
                obterDefinicaoObjeto(SchemaParser::XML_DEFINITION,atribs_tipos[i]);
  }
 }

 for(i=Operator::OPER_COMMUTATION; i <= Operator::OPER_GREATER; i++)
 {
  if(operators[i])
  {
   if(def_type==SchemaParser::SQL_DEFINITION)
    attributes[atribs_ops[i]]=operators[i]->getName(true);
   else
   {
    operators[i]->attributes[ParsersAttributes::REF_TYPE]=atribs_ops[i];
    attributes[atribs_ops[i]]=operators[i]->getCodeDefinition(def_type, true);
   }
  }
 }

 for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
 {
  if(functions[i])
  {
   if(def_type==SchemaParser::SQL_DEFINITION)
    attributes[atribs_funcoes[i]]=functions[i]->getSignature();
   else
   {
    functions[i]->setAttribute(ParsersAttributes::REF_TYPE, atribs_funcoes[i]);
    attributes[atribs_funcoes[i]]=functions[i]->getCodeDefinition(def_type, true);
   }
  }
 }

 attributes[ParsersAttributes::HASHES]=(hashes ? "1" : "");
 attributes[ParsersAttributes::MERGES]=(merges ? "1" : "");
 attributes[ParsersAttributes::SIGNATURE]=getSignature();

 return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

