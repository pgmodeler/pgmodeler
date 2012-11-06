#include "dominio.h"

Dominio::Dominio(void)
{
 obj_type=OBJ_DOMAIN;
 nao_nulo=false;
 attributes[ParsersAttributes::DEFAULT_VALUE]="";
 attributes[ParsersAttributes::NOT_NULL]="";
 attributes[ParsersAttributes::EXPRESSION]="";
 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::CONSTRAINT]="";
}

Dominio::~Dominio(void)
{
 TipoPgSQL::removerTipoUsuario(this->getName(true), this);
}

void Dominio::setName(const QString &nome)
{
 QString nome_ant, novo_nome;

 nome_ant=this->getName(true);
 BaseObject::setName(nome);
 novo_nome=this->getName(true);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, novo_nome);
}

void Dominio::setSchema(BaseObject *esquema)
{
 QString nome_ant;

 nome_ant=this->getName(true);
 BaseObject::setSchema(esquema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

void Dominio::definirNomeRestricao(const QString &nome_constr)
{
 /* Verifica se o nome da constraint é valido de acordo com as
    regras de nomenclatura do PostgreSQL */
 if(!BaseObject::isValidName(nome_constr))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->nome_rest=nome_constr;
}

void Dominio::definirExpressao(const QString &expressao)
{
 this->expressao=expressao;
}

void Dominio::definirValorPadrao(const QString &valor_padrao)
{
 this->valor_padrao=valor_padrao;
}

void Dominio::definirNaoNulo(bool valor)
{
 nao_nulo=valor;
}

void Dominio::definirTipo(TipoPgSQL tipo)
{
 this->tipo=tipo;
}

QString Dominio::getNameRestricao(void)
{
 return(nome_rest);
}

QString Dominio::obterExpressao(void)
{
 return(expressao);
}

QString Dominio::obterValorPadrao(void)
{
 return(valor_padrao);
}

bool Dominio::naoNulo(void)
{
 return(nao_nulo);
}

TipoPgSQL Dominio::obterTipo(void)
{
 return(tipo);
}

QString Dominio::getCodeDefinition(unsigned tipo_def)
{
 attributes[ParsersAttributes::NOT_NULL]=(nao_nulo ? "1" : "");
 attributes[ParsersAttributes::DEFAULT_VALUE]=valor_padrao;
 attributes[ParsersAttributes::EXPRESSION]=expressao;
 attributes[ParsersAttributes::CONSTRAINT]=BaseObject::formatName(nome_rest);

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  attributes[ParsersAttributes::TYPE]=(*tipo);
 else
 {
  attributes[ParsersAttributes::TYPE]=tipo.obterDefinicaoObjeto(tipo_def);
 }

 return(BaseObject::__getCodeDefinition(tipo_def));
}

void Dominio::operator = (Dominio &dominio)
{
 QString nome_ant=this->getName(true);

 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(dominio);
 this->nome_rest=dominio.nome_rest;
 this->expressao=dominio.expressao;
 this->nao_nulo=dominio.nao_nulo;
 this->valor_padrao=dominio.valor_padrao;
 this->tipo=dominio.tipo;

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

