#include "objetotabela.h"
//***********************************************************
ObjetoTabela::ObjetoTabela(void)
{
 tabela_pai=NULL;
 inc_ligacao=inc_generalizacao=inc_dependencia=false;
}
//-----------------------------------------------------------
void ObjetoTabela::definirTabelaPai(ObjetoBase *tabela)
{
 /* Caso o objeto a ser atribuído não seja uma tabela
    um erro é gerado */
 if(tabela && tabela->obterTipoObjeto()!=OBJETO_TABELA)
  throw Excecao(ERR_PGMODELER_ATROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 tabela_pai=tabela;
}
//-----------------------------------------------------------
ObjetoBase *ObjetoTabela::obterTabelaPai(void)
{
 return(tabela_pai);
}
//-----------------------------------------------------------
void ObjetoTabela::definirIncPorLigacao(bool valor)
{
 inc_ligacao=valor;
 inc_generalizacao=false;
 inc_dependencia=false;
}
//-----------------------------------------------------------
bool ObjetoTabela::incluidoPorLigacao(void)
{
 return(inc_ligacao);
}
//-----------------------------------------------------------
void ObjetoTabela::definirIncPorGeneralizacao(bool valor)
{
 inc_generalizacao=valor;
 inc_ligacao=false;
 inc_dependencia=false;
}
//-----------------------------------------------------------
bool ObjetoTabela::incluidoPorGeneralizacao(void)
{
 return(inc_generalizacao);
}
//-----------------------------------------------------------
void ObjetoTabela::definirIncPorDependencia(bool valor)
{
 inc_dependencia=valor;
 inc_generalizacao=false;
 inc_ligacao=false;
}
//-----------------------------------------------------------
bool ObjetoTabela::incluidoPorDependencia(void)
{
 return(inc_dependencia);
}
//-----------------------------------------------------------
bool ObjetoTabela::incluidoPorRelacionamento(void)
{
 return(inc_ligacao || inc_generalizacao || inc_dependencia);
}
//-----------------------------------------------------------
void ObjetoTabela::operator = (ObjetoTabela &objeto)
{
 *(dynamic_cast<ObjetoBase *>(this))=dynamic_cast<ObjetoBase &>(objeto);
 this->tabela_pai=objeto.tabela_pai;
 /*this->inc_dependencia=objeto.inc_dependencia;
 this->inc_generalizacao=objeto.inc_generalizacao;
 this->inc_ligacao=objeto.inc_ligacao;*/
 this->inc_dependencia=false;
 this->inc_generalizacao=false;
 this->inc_ligacao=false;
}
//***********************************************************
