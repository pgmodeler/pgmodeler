#include "elementoindice.h"

ElementoIndice::ElementoIndice(void)
{
 coluna=NULL;
 classe_oper=NULL;
 atrib_elemento[NULOS_PRIMEIRO]=false;
 atrib_elemento[ORDEM_ASCENDENTE]=true;
}

void ElementoIndice::definirColuna(Column *coluna)
{
 if(coluna)
 {
  this->coluna=coluna;
  this->expressao="";
 }
}

void ElementoIndice::definirExpressao(const QString &expressao)
{
 if(!expressao.isEmpty())
 {
  this->expressao=expressao;
  this->coluna=NULL;
 }
}

void ElementoIndice::definirClasseOperadores(OperatorClass *classe_oper)
{
 this->classe_oper=classe_oper;
}

void ElementoIndice::definirAtributo(unsigned id_atrib, bool valor)
{
 if(id_atrib > NULOS_PRIMEIRO)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 atrib_elemento[id_atrib]=valor;
}

bool ElementoIndice::obterAtributo(unsigned id_atrib)
{
 if(id_atrib > NULOS_PRIMEIRO)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 return(atrib_elemento[id_atrib]);
}

Column *ElementoIndice::obterColuna(void)
{
 return(coluna);
}

QString ElementoIndice::obterExpressao(void)
{
 return(expressao);
}

OperatorClass *ElementoIndice::obterClasseOperadores(void)
{
 return(classe_oper);
}

QString ElementoIndice::obterDefinicaoObjeto(unsigned tipo_def)
{
 map<QString, QString> atributos;

 atributos[ParsersAttributes::COLUMN]="";
 atributos[ParsersAttributes::EXPRESSION]="";
 atributos[ParsersAttributes::OP_CLASS]="";
 
 atributos[ParsersAttributes::NULLS_FIRST]=(atrib_elemento[NULOS_PRIMEIRO] ? "1" : "");
 atributos[ParsersAttributes::ASC_ORDER]=(atrib_elemento[ORDEM_ASCENDENTE] ? "1" : "");
 
 if(coluna)
 {
  /*if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::COLUNA]=coluna->obterTabelaPai()->getName(true) + "." + coluna->getName(true);
  else */
   atributos[ParsersAttributes::COLUMN]=coluna->getName(true);
 }
 else
  atributos[ParsersAttributes::EXPRESSION]=expressao;
 
 if(classe_oper)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   atributos[ParsersAttributes::OP_CLASS]=classe_oper->getName(true);
  else
   atributos[ParsersAttributes::OP_CLASS]=classe_oper->getCodeDefinition(tipo_def, true);
 }
 
 return(SchemaParser::getObjectDefinition(ParsersAttributes::INDEX_ELEMENT,atributos, tipo_def));
}

