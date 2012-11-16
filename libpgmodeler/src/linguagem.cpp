#include "linguagem.h"

Language::Language(void)
{
 obj_type=OBJ_LANGUAGE;
 is_trusted=false;

 for(unsigned i=VALIDATOR_FUNC; i <= INLINE_FUNC; i++)
  functions[i]=NULL;

 attributes[ParsersAttributes::TRUSTED]="";
 attributes[ParsersAttributes::HANDLER_FUNC]="";
 attributes[ParsersAttributes::VALIDATOR_FUNC]="";
 attributes[ParsersAttributes::INLINE_FUNC]="";
}

void Language::setName(const QString &name)
{
 /* Tratando nome de linguagem. Nomes como SQL, C são reservados
    para o SGBD e não podem ser criados pelo usuário */
 if(name.toLower()==~TipoLinguagem("c") || name.toLower()==~TipoLinguagem("sql"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
                ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 BaseObject::setName(name); //Chama o método da classe base
}

void Language::setTrusted(bool value)
{
 is_trusted=value;
}

void  Language::setFunction(Function *func, unsigned func_type)
{
 TipoLinguagem lang=TipoLinguagem::c;

 if(!func ||
     (func &&
      /* Obrigatóriamente uma função de manipulação deve ser escrita em C,
         e possuir tipo de retorno "language_handler" */
      ((func_type==HANDLER_FUNC &&
        func->getReturnType()=="language_handler" &&
        func->getParameterCount()==0 &&
        func->getLanguage()->getName()==(~lang)) ||
       /* Obrigatoriamente a função de validator de ser escrita em C,
          retornar "void", possuir apenas 1 parâmetro e que o mesmo
          seja do tipo "oid" */
       (func_type==VALIDATOR_FUNC &&
        func->getReturnType()=="void" &&
        func->getParameterCount()==1 &&
        func->getParameter(0).getType() == "oid" &&
        func->getLanguage()->getName()==(~lang)) ||
      /* Obrigatoriamente a função inline de ser escrita em C,
         retornar "void", possuir apenas 1 parâmetro e que o mesmo
         seja do tipo "internal" */
       (func_type==INLINE_FUNC &&
        func->getReturnType()=="void" &&
        func->getParameterCount()==1 &&
        func->getParameter(0).getType() == "internal" &&
        func->getLanguage()->getName()==(~lang)) )))
 {
  this->functions[func_type]=func;
 }
 //Disparando uma exceção caso o tipo de retorno da função não coincida com o aceito para cada tipo de função
 else if((func_type==HANDLER_FUNC && func->getReturnType()!="language_handler") ||
         ((func_type==VALIDATOR_FUNC || func_type==INLINE_FUNC) && func->getReturnType()!="void"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
                         .arg(this->getName(true))
                         .arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
                ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Dispara um erro se a função possuir parâmetros inválidos
  throw Exception(ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Function * Language::getFunction(unsigned func_type)
{
 if(func_type > INLINE_FUNC)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(functions[func_type]);
}

bool Language::isTrusted(void)
{
 return(is_trusted);
}

QString Language::getCodeDefinition(unsigned def_type)
{
 return(this->getCodeDefinition(def_type, false));
}

QString Language::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 unsigned i;
 QString attribs_func[3]={ParsersAttributes::VALIDATOR_FUNC,
                          ParsersAttributes::HANDLER_FUNC,
                          ParsersAttributes::INLINE_FUNC};

 attributes[ParsersAttributes::TRUSTED]=(is_trusted ? "1" : "");

 if(!reduced_form && def_type==SchemaParser::XML_DEFINITION)
  reduced_form=(!functions[VALIDATOR_FUNC] && !functions[HANDLER_FUNC] && !functions[INLINE_FUNC] && !this->getOwner());

 for(i=0; i < 3; i++)
 {
  if(functions[i])
  {
   if(def_type==SchemaParser::SQL_DEFINITION)
    attributes[attribs_func[i]]=functions[i]->getName(true);
   else
   {
    functions[i]->setAttribute(ParsersAttributes::REF_TYPE, attribs_func[i]);
    attributes[attribs_func[i]]=functions[i]->getCodeDefinition(def_type, true);
   }
  }
 }

 return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

