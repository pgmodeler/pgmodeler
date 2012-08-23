#include "dominio.h"
//***********************************************************
Dominio::Dominio(void)
{
 tipo_objeto=OBJETO_DOMINIO;
 nao_nulo=false;
 atributos[AtributosParsers::VALOR_PADRAO]="";
 atributos[AtributosParsers::NAO_NULO]="";
 atributos[AtributosParsers::EXPRESSAO]="";
 atributos[AtributosParsers::TIPO]="";
 atributos[AtributosParsers::RESTRICAO]="";
}
//-----------------------------------------------------------
Dominio::~Dominio(void)
{
 TipoPgSQL::removerTipoUsuario(this->obterNome(true), this);
}
//-----------------------------------------------------------
void Dominio::definirNome(const QString &nome)
{
 QString nome_ant, novo_nome;

 nome_ant=this->obterNome(true);
 ObjetoBase::definirNome(nome);
 novo_nome=this->obterNome(true);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, novo_nome);
}
//-----------------------------------------------------------
void Dominio::definirEsquema(ObjetoBase *esquema)
{
 QString nome_ant, nome;

 nome_ant=this->obterNome(true);
 ObjetoBase::definirEsquema(esquema);
 nome=this->obterNome(true);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, nome);
}
//-----------------------------------------------------------
void Dominio::definirNomeRestricao(const QString &nome_constr)
{
 /* Verifica se o nome da constraint é valido de acordo com as
    regras de nomenclatura do PostgreSQL */
 if(!ObjetoBase::nomeValido(nome_constr))
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->nome_rest=nome_constr;
}
//-----------------------------------------------------------
void Dominio::definirExpressao(const QString &expressao)
{
 this->expressao=expressao;
}
//-----------------------------------------------------------
void Dominio::definirValorPadrao(const QString &valor_padrao)
{
 this->valor_padrao=valor_padrao;
}
//-----------------------------------------------------------
void Dominio::definirNaoNulo(bool valor)
{
 nao_nulo=valor;
}
//-----------------------------------------------------------
void Dominio::definirTipo(TipoPgSQL tipo)
{
 this->tipo=tipo;
}
//-----------------------------------------------------------
QString Dominio::obterNomeRestricao(void)
{
 return(nome_rest);
}
//-----------------------------------------------------------
QString Dominio::obterExpressao(void)
{
 return(expressao);
}
//-----------------------------------------------------------
QString Dominio::obterValorPadrao(void)
{
 return(valor_padrao);
}
//-----------------------------------------------------------
bool Dominio::naoNulo(void)
{
 return(nao_nulo);
}
//-----------------------------------------------------------
TipoPgSQL Dominio::obterTipo(void)
{
 return(tipo);
}
//-----------------------------------------------------------
QString Dominio::obterDefinicaoObjeto(unsigned tipo_def)
{
 atributos[AtributosParsers::NAO_NULO]=(nao_nulo ? "1" : "");
 atributos[AtributosParsers::VALOR_PADRAO]=valor_padrao;
 atributos[AtributosParsers::EXPRESSAO]=expressao;
 atributos[AtributosParsers::RESTRICAO]=ObjetoBase::formatarNome(nome_rest);

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  atributos[AtributosParsers::TIPO]=(*tipo);
 else
 {
  atributos[AtributosParsers::TIPO]=tipo.obterDefinicaoObjeto(tipo_def);
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//-----------------------------------------------------------
void Dominio::operator = (Dominio &dominio)
{
 QString nome_ant, novo_nome;

 nome_ant=this->obterNome(true);

 *(dynamic_cast<ObjetoBase *>(this))=dynamic_cast<ObjetoBase &>(dominio);
 this->nome_rest=dominio.nome_rest;
 this->expressao=dominio.expressao;
 this->nao_nulo=dominio.nao_nulo;
 this->valor_padrao=dominio.valor_padrao;
 this->tipo=dominio.tipo;

 novo_nome=this->obterNome(true);
 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, novo_nome);
}
//***********************************************************
