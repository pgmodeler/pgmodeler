#include "elemclasseoperadores.h"
//***********************************************************
ElemClasseOperadores::ElemClasseOperadores(void)
{
 tipo_elemento=ELEM_OPERADOR;
 funcao=NULL;
 operador=NULL;
 rechecar=false;
 num_estrategia=0;
}
//-----------------------------------------------------------
void ElemClasseOperadores::definirFuncao(Funcao *funcao, unsigned num_estrategia)
{
 //Caso a função não esteja alocada dispara uma exceção
 if(!funcao)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o número de estratégia seja zero (inválido) dispara exceção
 else if(num_estrategia==0)
  throw Excecao(ERR_PGMODELER_ATRVALORINVNUMEST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Limpa os atributos não pertinentes ao tipo ELEM_FUNCAO
 operador=NULL;

 //Configura os atributos pertinentes ao tipo
 this->funcao=funcao;
 this->num_estrategia=num_estrategia;
 tipo_elemento=ELEM_FUNCAO;
}
//-----------------------------------------------------------
void ElemClasseOperadores::definirOperador(Operador *operador, unsigned num_estrategia, bool rechecar)
{
 //Caso o operador não esteja alocada dispara uma exceção
 if(!operador)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o número de estratégia seja zero (inválido) dispara exceção
 else if(num_estrategia==0)
  throw Excecao(ERR_PGMODELER_ATRVALORINVNUMEST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Limpa os atributos não pertinentes ao tipo ELEM_OPERADOR
 funcao=NULL;

 //Configura os atributos pertinentes ao tipo
 this->operador=operador;
 this->num_estrategia=num_estrategia;
 this->rechecar=rechecar;
 tipo_elemento=ELEM_OPERADOR;
}
//-----------------------------------------------------------
void ElemClasseOperadores::definirArmazenamento(TipoPgSQL tipo_armaz)
{
 //Limpa os atributos não pertinentes ao tipo ELEM_ARMAZENAMENTO
 funcao=NULL;
 operador=NULL;
 num_estrategia=0;

 //Configura os atributos pertinentes ao tipo
 this->tipo_armaz=tipo_armaz;
 tipo_elemento=ELEM_ARMAZENAMENTO;
}
//-----------------------------------------------------------
unsigned ElemClasseOperadores::obterTipoElemento(void)
{
 return(tipo_elemento);
}
//-----------------------------------------------------------
Funcao *ElemClasseOperadores::obterFuncao(void)
{
 return(funcao);
}
//-----------------------------------------------------------
Operador *ElemClasseOperadores::obterOperador(void)
{
 return(operador);
}
//-----------------------------------------------------------
TipoPgSQL ElemClasseOperadores::obterTipoArmazenamento(void)
{
 return(tipo_armaz);
}
//-----------------------------------------------------------
bool ElemClasseOperadores::rechecarElemento(void)
{
 return(rechecar);
}
//-----------------------------------------------------------
unsigned ElemClasseOperadores::obterNumEstrategia(void)
{
 return(num_estrategia);
}
//-----------------------------------------------------------
QString ElemClasseOperadores::obterDefinicaoObjeto(unsigned tipo_def)
{
 map<QString, QString> atributos;

 atributos[AtributosParsers::TIPO]="";
 atributos[AtributosParsers::NUM_ESTRATEGIA]="";
 atributos[AtributosParsers::ASSINATURA]="";
 atributos[AtributosParsers::RECHECAR]="";
 atributos[AtributosParsers::FUNCAO]="";
 atributos[AtributosParsers::OPERADOR]="";
 atributos[AtributosParsers::ARMAZENAMENTO]="";
 atributos[AtributosParsers::DEFINICAO]="";

 if(tipo_elemento==ELEM_FUNCAO && funcao && num_estrategia > 0)
 {
  //FUNCTION support_number [ ( op_type [ , op_type ] ) ] funcname ( argument_type [, ...] )
  atributos[AtributosParsers::FUNCAO]="1";
  atributos[AtributosParsers::NUM_ESTRATEGIA]=QString("%1").arg(num_estrategia);

  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::ASSINATURA]=funcao->obterAssinatura();
  else
   atributos[AtributosParsers::DEFINICAO]=funcao->obterDefinicaoObjeto(tipo_def,true);
 }
 else if(tipo_elemento==ELEM_OPERADOR && operador && num_estrategia > 0)
 {
  //OPERATOR strategy_number operator_name [ ( op_type, op_type ) ] [ RECHECK ]
  atributos[AtributosParsers::OPERADOR]="1";
  atributos[AtributosParsers::NUM_ESTRATEGIA]=QString("%1").arg(num_estrategia);
  if(rechecar) atributos[AtributosParsers::RECHECAR]="1";

  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::ASSINATURA]=operador->obterAssinatura();
  else
   atributos[AtributosParsers::DEFINICAO]=operador->obterDefinicaoObjeto(tipo_def,true);
 }
 else if(tipo_elemento==ELEM_ARMAZENAMENTO && tipo_armaz!=TipoPgSQL::nulo)
 {
  //STORAGE storage_type
  atributos[AtributosParsers::ARMAZENAMENTO]="1";

  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::TIPO]=(*tipo_armaz);
  else
   atributos[AtributosParsers::DEFINICAO]=tipo_armaz.obterDefinicaoObjeto(tipo_def);
 }

 return(ParserEsquema::obterDefinicaoObjeto(AtributosParsers::ELEMENTO,atributos, tipo_def));
}
//-----------------------------------------------------------
bool ElemClasseOperadores::operator == (ElemClasseOperadores &elem)
{
 return(this->tipo_elemento == elem.tipo_elemento &&
        this->tipo_armaz == elem.tipo_armaz &&
        this->funcao == elem.funcao &&
        this->operador == elem.operador &&
        this->num_estrategia == elem.num_estrategia);
}
//***********************************************************
