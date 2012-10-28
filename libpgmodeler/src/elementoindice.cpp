#include "elementoindice.h"

ElementoIndice::ElementoIndice(void)
{
 coluna=NULL;
 classe_oper=NULL;
 atrib_elemento[NULOS_PRIMEIRO]=false;
 atrib_elemento[ORDEM_ASCENDENTE]=true;
}

void ElementoIndice::definirColuna(Coluna *coluna)
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

void ElementoIndice::definirClasseOperadores(ClasseOperadores *classe_oper)
{
 this->classe_oper=classe_oper;
}

void ElementoIndice::definirAtributo(unsigned id_atrib, bool valor)
{
 if(id_atrib > NULOS_PRIMEIRO)
  throw Exception(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 atrib_elemento[id_atrib]=valor;
}

bool ElementoIndice::obterAtributo(unsigned id_atrib)
{
 if(id_atrib > NULOS_PRIMEIRO)
  throw Exception(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 
 return(atrib_elemento[id_atrib]);
}

Coluna *ElementoIndice::obterColuna(void)
{
 return(coluna);
}

QString ElementoIndice::obterExpressao(void)
{
 return(expressao);
}

ClasseOperadores *ElementoIndice::obterClasseOperadores(void)
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
   atributos[AtributosParsers::COLUNA]=coluna->obterTabelaPai()->obterNome(true) + "." + coluna->obterNome(true);
  else */
   atributos[ParsersAttributes::COLUMN]=coluna->obterNome(true);
 }
 else
  atributos[ParsersAttributes::EXPRESSION]=expressao;
 
 if(classe_oper)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   atributos[ParsersAttributes::OP_CLASS]=classe_oper->obterNome(true);
  else
   atributos[ParsersAttributes::OP_CLASS]=classe_oper->obterDefinicaoObjeto(tipo_def, true);
 }
 
 return(SchemaParser::getObjectDefinition(ParsersAttributes::INDEX_ELEMENT,atributos, tipo_def));
}

