#include "operatorclasselement.h"

OperatorClassElement::OperatorClassElement(void)
{
 element_type=OPERATOR_ELEM;
 function=NULL;
 _operator=NULL;
 recheck=false;
 strategy_number=0;
}

void OperatorClassElement::setFunction(Function *func, unsigned stg_number)
{
 //Raises an exception case the function is not allocated
 if(!func)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Raises an error case the stratagy number is zero (invalid)
 else if(stg_number==0)
  throw Exception(ERR_ASG_INV_SUPSTG_NUMBER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Clear the attributes not related to the FUNCTION element type
 this->_operator=NULL;

 //Configure the attributes that belongs to the element type
 this->function=func;
 this->strategy_number=stg_number;
 this->element_type=FUNCTION_ELEM;
}

void OperatorClassElement::setOperator(Operador *oper, unsigned stg_number, bool recheck)
{
 //Raises an error in case the operator is not allocated
 if(!oper)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Raises an error case the stratagy number is zero (invalid)
 else if(stg_number==0)
  throw Exception(ERR_ASG_INV_SUPSTG_NUMBER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Clear the attributes not related to the OPERATOR element type
 this->function=NULL;

 //Configure the attributes that belongs to the element type
 this->_operator=oper;
 this->strategy_number=stg_number;
 this->recheck=recheck;
 this->element_type=OPERATOR_ELEM;
}

void OperatorClassElement::setStorage(TipoPgSQL storage)
{
 //Clear the attributes not related to the STORAGE element type
 this->function=NULL;
 this->_operator=NULL;
 this->strategy_number=0;

 //Configure the attributes that belongs to the element type
 this->storage=storage;
 this->element_type=STORAGE_ELEM;
}

unsigned OperatorClassElement::getElementType(void)
{
 return(element_type);
}

Function *OperatorClassElement::getFunction(void)
{
 return(function);
}

Operador *OperatorClassElement::getOperator(void)
{
 return(_operator);
}

TipoPgSQL OperatorClassElement::getStorage(void)
{
 return(storage);
}

bool OperatorClassElement::isRecheck(void)
{
 return(recheck);
}

unsigned OperatorClassElement::getStrategyNumber(void)
{
 return(strategy_number);
}

QString OperatorClassElement::getCodeDefinition(unsigned def_type)
{
 map<QString, QString> attributes;

 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::STRATEGY_NUM]="";
 attributes[ParsersAttributes::SIGNATURE]="";
 attributes[ParsersAttributes::RECHECK]="";
 attributes[ParsersAttributes::FUNCTION]="";
 attributes[ParsersAttributes::OPERATOR]="";
 attributes[ParsersAttributes::STORAGE]="";
 attributes[ParsersAttributes::DEFINITION]="";

 if(element_type==FUNCTION_ELEM && function && strategy_number > 0)
 {
  //FUNCTION support_number [ ( op_type [ , op_type ] ) ] funcname ( argument_type [, ...] )
  attributes[ParsersAttributes::FUNCTION]="1";
  attributes[ParsersAttributes::STRATEGY_NUM]=QString("%1").arg(strategy_number);

  if(def_type==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::SIGNATURE]=function->getSignature();
  else
   attributes[ParsersAttributes::DEFINITION]=function->getCodeDefinition(def_type,true);
 }
 else if(element_type==OPERATOR_ELEM && _operator && strategy_number > 0)
 {
  //OPERATOR strategy_number operator_name [ ( op_type, op_type ) ] [ RECHECK ]
  attributes[ParsersAttributes::OPERATOR]="1";
  attributes[ParsersAttributes::STRATEGY_NUM]=QString("%1").arg(strategy_number);
  if(recheck) attributes[ParsersAttributes::RECHECK]="1";

  if(def_type==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::SIGNATURE]=_operator->obterAssinatura();
  else
   attributes[ParsersAttributes::DEFINITION]=_operator->getCodeDefinition(def_type,true);
 }
 else if(element_type==STORAGE_ELEM && storage!=TipoPgSQL::nulo)
 {
  //STORAGE storage_type
  attributes[ParsersAttributes::STORAGE]="1";

  if(def_type==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::TYPE]=(*storage);
  else
   attributes[ParsersAttributes::DEFINITION]=storage.obterDefinicaoObjeto(def_type);
 }

 return(SchemaParser::getObjectDefinition(ParsersAttributes::ELEMENT,attributes, def_type));
}

bool OperatorClassElement::operator == (OperatorClassElement &elem)
{
 return(this->element_type == elem.element_type &&
        this->storage == elem.storage &&
        this->function == elem.function &&
        this->_operator == elem._operator &&
        this->strategy_number == elem.strategy_number);
}

