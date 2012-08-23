#include "familiaoperadores.h"
//***********************************************************
FamiliaOperadores::FamiliaOperadores(void)
{
 tipo_objeto=OBJETO_FAMILIA_OPER;
 atributos[AtributosParsers::TIPO_INDEXACAO]="";
}
//-----------------------------------------------------------
void FamiliaOperadores::definirTipoIndexacao(TipoIndexacao tipo)
{
 tipo_index=tipo;
}
//-----------------------------------------------------------
TipoIndexacao FamiliaOperadores::obterTipoIndexacao(void)
{
 return(tipo_index);
}
//-----------------------------------------------------------
QString FamiliaOperadores::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, false));
}
//-----------------------------------------------------------
QString FamiliaOperadores::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 atributos[AtributosParsers::TIPO_INDEXACAO]=(~tipo_index);
 return(ObjetoBase::obterDefinicaoObjeto(tipo_def,forma_reduzida));
}
//***********************************************************
