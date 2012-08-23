#include "coluna.h"
//***********************************************************
Coluna::Coluna(void)
{
 tipo_objeto=OBJETO_COLUNA;
 nao_nulo=false;
 atributos[AtributosParsers::TIPO]="";
 atributos[AtributosParsers::VALOR_PADRAO]="";
 atributos[AtributosParsers::NAO_NULO]="";
 atributos[AtributosParsers::TABELA]="";
}
//-----------------------------------------------------------
void Coluna::definirNome(const QString &nome)
{
 try
 {
  QString nome_ant;

  //Obtém o nome atual da coluna o qual passará a ser o antigo
  nome_ant=this->nome;

  //Tenta definir o novo nome da coluna
  ObjetoBase::definirNome(nome);
  /* Caso nenhuma exceção foi disparada atribui o nome anterior
     obtido ao respectivo atributo da coluna */
  this->nome_antigo=nome_ant;
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void Coluna::definirTipo(TipoPgSQL tipo)
{
 /* Caso o tipo a ser atribuído à coluna seja um pseudo-tipo
    será disparada uma exceção */
 if(tipo.pseudoTipo())
  throw Excecao(ERR_PGMODELER_ATRPSDTIPOCOL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  this->tipo=tipo;
}
//-----------------------------------------------------------
void Coluna::definirValorPadrao(const QString &valor)
{
 valor_padrao=valor;
}
//-----------------------------------------------------------
void Coluna::definirNaoNulo(bool valor)
{
 nao_nulo=valor;
}
//-----------------------------------------------------------
TipoPgSQL Coluna::obterTipo(void)
{
 return(tipo);
}
//-----------------------------------------------------------
bool Coluna::naoNulo(void)
{
 return(nao_nulo);
}
//-----------------------------------------------------------
QString Coluna::obterReferenciaTipo(void)
{
 if(tabela_pai)
  return(tabela_pai->obterNome(true) + QString(".") + this->obterNome(true) + QString("%TYPE"));
 else
  return("");
}
//-----------------------------------------------------------
QString Coluna::obterValorPadrao(void)
{
 return(valor_padrao);
}
//-----------------------------------------------------------
QString Coluna::obterNomeAntigo(bool formatar)
{
 if(formatar)
  return(ObjetoBase::formatarNome(nome_antigo));
 else
  return(nome_antigo);
}
//-----------------------------------------------------------
QString Coluna::obterDefinicaoObjeto(unsigned tipo_def)
{
 if(this->tabela_pai)
  atributos[AtributosParsers::TABELA]=this->tabela_pai->obterNome(true);

 atributos[AtributosParsers::TIPO]=tipo.obterDefinicaoObjeto(tipo_def);
 atributos[AtributosParsers::VALOR_PADRAO]=valor_padrao;
 atributos[AtributosParsers::NAO_NULO]=(!nao_nulo ? "" : "1");
 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//-----------------------------------------------------------
void Coluna::operator = (Coluna &coluna)
{
 this->comentario=coluna.comentario;
 this->id_objeto=coluna.id_objeto;
 this->protegido=coluna.protegido;

 this->nome=coluna.nome;
 this->nome_antigo=coluna.nome_antigo;

 this->tipo=coluna.tipo;
 this->valor_padrao=coluna.valor_padrao;

 this->tabela_pai=coluna.tabela_pai;
 /*this->inc_dependencia=coluna.inc_dependencia;
 this->inc_ligacao=coluna.inc_ligacao;
 this->inc_generalizacao=coluna.inc_generalizacao;*/
 this->inc_dependencia=false;
 this->inc_generalizacao=false;
 this->inc_ligacao=false;
}
//***********************************************************
