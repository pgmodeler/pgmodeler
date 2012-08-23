#include "tipospgsql.h"
//***********************************************************
/********************
 * CLASSE: TipoBase *
 ********************/
QString TipoBase::tipos[qtd_tipos]=
{
 /* Tipo vazio, quando se instância a classe TipoBase ao usar o operador ~
    este será o único tipo retornado */
  //offsets 0
  "",

 //Tipos utilizados pela classe TipoAcao
 //offsets 1 a 5
 "NO ACTION",
 "RESTRICT",
 "CASCADE",
 "SET NULL",
 "SET DEFAULT",

 //Tipos utilizados pela classe TipoRestricao
 //offsets 6 a 9
 "PRIMARY KEY",
 "FOREIGN KEY",
 "CHECK",
 "UNIQUE",

 //Tipos utilizados pela classe TipoEvento
 //offsets 10 a 14
 "ON SELECT",
 "ON INSERT",
 "ON DELETE",
 "ON UPDATE",
 "ON TRUNCATE",

 //Tipos utilizados pela classe TipoExecucao
 //offsets 15 a 16
 "ALSO",
 "INSTEAD",

 //Tipos utilizados pela classe TipoFuncao
 //offsets 17 a 19
 "VOLATILE",
 "STABLE",
 "IMMUTABLE",

 //Tipos utilizados pela classe TipoIndexacao
 //offsets 20 a 24
 "btree",
 "rtree",
 "gist",
 "hash",
 "gin",

 //Tipos utilizados pela classe TipoPgSQL
 //offsets 25 a 61
 "smallint", "integer", "bigint", "decimal", "numeric",
 "real", "double precision", "float", "serial", "bigserial", "money",
 "character varying", "varchar", "character",
 "char", "text", "bytea",
 "timestamp", "date", "time",
 "interval", "boolean",
 "point", "line", "lseg", "box", "path",
 "polygon", "circle", "cidr", "inet",
 "macaddr", "bit", "bit varying", "varbit", "uuid", "xml",


 //Tipos identificadores de objeto (OID)
 //offsets 62 a 73
 "oid", "regproc", "regprocedure",
 "regoper", "regoperator", "regclass",
 "regtype", "regconfig", "regdictionary",
 "xid", "cid", "tid",

 //Pseudo-tipos
 //offsets 74 a 85
 "any","anyarray","anyelement","anyenum",
 "anynonarray","cstring","internal","language_handler",
 "record","trigger","void","opaque",

 /* Tipos utilizados pela classe TipoIntervalo usado
    como auxiliar da classe TipoPgSQL quando se trata
    do tipo de dado "interval" */
 //offsets 86 a 98
 "YEAR", "MONTH", "DAY", "HOUR",
 "MINUTE", "SECOND","YEAR TO MONTH",
 "DAY TO HOUR","DAY TO MINUTE","DAY TO SECOND",
 "HOUR TO MINUTE","HOUR TO SECOND","MINUTE TO SECOND",

 //Tipos utilizados pela classe TipoComportamento
 //offsets 99 a 101
 "CALLED ON NULL INPUT",
 "RETURNS NULL ON NULL INPUT",
 "STRICT",

 //Tipos utilizados pela classe TipoSeguranca
 //offsets 102 a 103
 "SECURITY INVOKER",
 "SECURITY DEFINER",

 //Tipos utilizados pela classe TipoLinguagem
 //offsets 104 a 109
 "sql",
 "c",
 "plpgsql",
 "pltcl",
 "plperl",
 "plpython",

 //Tipos utilizados pela classe TipoCodificacao
 //offsets 110 a 150
 "UTF8", "BIG5", "EUC_CN",  "EUC_JP", "EUC_JIS_2004", "EUC_KR",
 "EUC_TW", "GB18030", "GBK", "ISO_8859_5", "ISO_8859_6",
 "ISO_8859_7", "ISO_8859_8", "JOHAB", "KOI", "LATIN1",
 "LATIN2", "LATIN3", "LATIN4", "LATIN5", "LATIN6",
 "LATIN7", "LATIN8", "LATIN9", "LATIN10", "MULE_INTERNAL",
 "SJIS", "SHIFT_JIS_2004", "SQL_ASCII", "UHC",
 "WIN866", "WIN874", "WIN1250", "WIN1251", "WIN1252",
 "WIN1253", "WIN1254", "WIN1255", "WIN1256", "WIN1257",
 "WIN1258",

 //Tipos utilizados pela classe TipoArmazenamento
 //offsets 151 a 154
 "plain",
 "external",
 "extended",
 "main",

 //Tipos utilizados pela classe TipoComparacao
 //offsets 155 a 157
 "MATCH FULL",
 "MATCH PARTIAL",
 "MATCH SIMPLE",

 //Tipos utilizados pela classe TipoPostergacao
 //offsets 158 a 159
 "INITIALLY IMMEDIATE",
 "INITIALLY DEFERRED",

 //Tipos utilizados pela classe TipoCategoria
 //offsets 160 a 173 - Vide tabela 44-43 da Documentação do PostgreSQL 8.4
 "U", //User-defined types
 "A", //Array types
 "B", //Boolean types
 "C", //Composite types
 "D", //Date/time types
 "E", //Enum types
 "G", //Geometric types
 "I", //Network address types
 "N", //Numeric types
 "P", //Pseudo-types
 "S", //String types
 "T", //Timespan types
 "V", //Bit-string types
 "X", //Unknown type

 //Tipos utilizados pela classe TipoDisparo
 //offsets 174 a 176
 "BEFORE",
 "AFTER",
 "INSTEAD OF"
};
//-----------------------------------------------------------
TipoBase::TipoBase(void)
{
 idx_tipo=0;
}
//-----------------------------------------------------------
QString TipoBase::obterStringTipo(unsigned tipo)
{
 if(tipo > qtd_tipos)
  throw Excecao(ERR_PGMODELER_REFTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(tipos[tipo]);
}
//-----------------------------------------------------------
void TipoBase::definirTipo(unsigned tipo,unsigned offset,unsigned qtd_tipos)
{
 /* Caso a quantidade de tipos seja nula ou maior do que o tamanho da lista de tipos
    da classe base, dispara um exceção indicando o fato */
 if(qtd_tipos==0 || qtd_tipos > this->qtd_tipos)
  throw Excecao(ERR_PGMODELER_OBTQTDTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Caso o tipo a ser atribuido não seja pertecente a classe de tipo atual
 else if(!tipoValido(tipo,offset,qtd_tipos))
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  idx_tipo=tipo;
}
//-----------------------------------------------------------
bool TipoBase::tipoValido(unsigned idx_tipo,unsigned offset,unsigned qtd_tipos)
{
 //Retorna se o indice do tipo está no intervalo (offset-qtd_tipos) da classe
 return((idx_tipo>=offset && idx_tipo<=(offset+qtd_tipos-1)) || idx_tipo==0);
}
//-----------------------------------------------------------
void TipoBase::obterTipos(QStringList &tipos,unsigned offset,unsigned qtd_tipos)
{
 /* Caso a quantidade de tipos seja nula ou maior do que o tamanho da lista de tipos
    da classe base, dispara um exceção indicando o fato */
 if(qtd_tipos==0 || qtd_tipos > TipoBase::qtd_tipos)
  throw Excecao(ERR_PGMODELER_OBTQTDTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  unsigned idx,total;

  tipos.clear(); //Limpa a lista de tipos
  total=offset+qtd_tipos; //Calcula a quantidade total de tipos a serem obtidos

  for(idx=offset; idx<total; idx++)
   //Insere na lista os tipos que vao do offset ao total de tipos
   tipos.push_back(TipoBase::tipos[idx]);
 }
}
//-----------------------------------------------------------
unsigned TipoBase::obterTipo(const QString &nome_tipo,unsigned offset,unsigned qtd_tipos)
{
 unsigned idx,total;
 bool enc=false;

 if(nome_tipo=="")
  return(TipoBase::nulo);
 else
 {
  total=offset + qtd_tipos; //Calculando o total de tipos da classe

  /*Verifica se o tipo passado pelo parametro está no conjunto de
   tipos da classe */
  for(idx=offset; idx<total && !enc; idx++)
   enc=(nome_tipo==TipoBase::tipos[idx]);

  if(enc)
  { idx--; return(idx); }
  else
   return(TipoBase::nulo);
 }
}
//-----------------------------------------------------------
QString TipoBase::operator ~ (void)
{
 return(tipos[idx_tipo]);
}
//-----------------------------------------------------------
unsigned TipoBase::operator ! (void)
{
 return(idx_tipo);
}
//-----------------------------------------------------------
bool TipoBase::operator == (TipoBase &tipo)
{
 return(tipo.idx_tipo == idx_tipo);
}
//-----------------------------------------------------------
bool TipoBase::operator == (unsigned tipo)
{
 return(idx_tipo==tipo);
}
//-----------------------------------------------------------
bool TipoBase::operator != (TipoBase &tipo)
{
 return(tipo.idx_tipo != idx_tipo);
}
//-----------------------------------------------------------
bool TipoBase::operator != (unsigned tipo)
{
 return(idx_tipo!=tipo);
}
//***********************************************************
/********************
 * CLASSE: TipoAcao *
 ********************/
TipoAcao::TipoAcao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoAcao::TipoAcao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoAcao::TipoAcao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
void TipoAcao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoAcao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoAcao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/**************************
 * CLASSE: TipoRestricao *
 **************************/
//-----------------------------------------------------------
TipoRestricao::TipoRestricao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoRestricao::TipoRestricao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoRestricao::TipoRestricao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
void TipoRestricao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoRestricao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoRestricao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/**********************
 * CLASSE: TipoEvento *
 **********************/
TipoEvento::TipoEvento(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoEvento::TipoEvento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoEvento::TipoEvento(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
void TipoEvento::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoEvento::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoEvento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/************************
 * CLASSE: TipoExecucao *
 ************************/
TipoExecucao::TipoExecucao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoExecucao::TipoExecucao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoExecucao::TipoExecucao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoExecucao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoExecucao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoExecucao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/**********************
 * CLASSE: TipoFuncao *
 **********************/
TipoFuncao::TipoFuncao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoFuncao::TipoFuncao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoFuncao::TipoFuncao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoFuncao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoFuncao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoFuncao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/**********************
 * CLASSE: TipoIndexacao *
 **********************/
TipoIndexacao::TipoIndexacao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoIndexacao::TipoIndexacao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoIndexacao::TipoIndexacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoIndexacao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoIndexacao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoIndexacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/**********************
 * CLASSE: TipoIntervalo *
 **********************/
TipoIntervalo::TipoIntervalo(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoIntervalo::TipoIntervalo(void)
{
 idx_tipo=TipoBase::nulo;
}
//-----------------------------------------------------------
TipoIntervalo::TipoIntervalo(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoIntervalo::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoIntervalo::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoIntervalo::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/*********************
 * CLASSE: TipoPgSQL *
 *********************/
//Inicializando a lista estática da classe
vector<TipoPgSQL::ConfigTipoUsuario> TipoPgSQL::tipos_usr;
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(void)
{
 idx_tipo=offset;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(const QString &nome_tipo)
{
 (*this)=nome_tipo;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(void *ptipo)
{
 (*this) << ptipo;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(void *ptipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv)
{
 (*this) << ptipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
}
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(const QString &tipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv)
{
 (*this)=tipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
}
//-----------------------------------------------------------
TipoPgSQL::TipoPgSQL(unsigned idx_tipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv)
{
 (*this)=idx_tipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
}
//-----------------------------------------------------------
void TipoPgSQL::obterTipos(QStringList &tipos, bool tipo_oid, bool pseudos)
{
 unsigned idx,total;

 tipos.clear(); //Limpa a lista de tipos
 total=offset+qtd_tipos; //Calcula a quantidade total de tipos a serem obtidos

 for(idx=offset; idx<total; idx++)
 {
  //Insere na lista os tipos que vao do offset ao total de tipos
  if(idx<ini_oid ||
    (tipo_oid && idx>=ini_oid && idx<=fim_oid) ||
    (pseudos && idx>=ini_pseudo && idx<=fim_pseudo))
  tipos.push_back(TipoBase::tipos[idx]);
 }
}
//-----------------------------------------------------------
unsigned TipoPgSQL::operator = (unsigned tipo)
{
 if(tipo>=offset)
  definirTipoUsuario(tipo);
 else if(tipo > 0)
  TipoBase::definirTipo(tipo,offset,qtd_tipos);
 else if(tipo==0)
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoPgSQL::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo, idx_tipo_usr;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 idx_tipo_usr=obterIndiceTipoUsuario(nome_tipo, NULL);

 if(idx_tipo==0 && idx_tipo_usr==0)
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(idx_tipo!=0)
 {
  TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
  return(idx_tipo);
 }
 else
 {
  definirTipoUsuario(idx_tipo_usr);
  return(idx_tipo_usr);
 }
}
//-----------------------------------------------------------
void *TipoPgSQL::obterRefTipoUsuario(void)
{
 if(this->tipoUsuario())
  return(tipos_usr[this->idx_tipo].ptipo);
 else
  return(NULL);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator == (unsigned idx_tipo)
{
 return(this->idx_tipo==idx_tipo);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + qtd_tipos; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==TipoBase::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(idx_tipo==idx);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}
//-----------------------------------------------------------
bool TipoPgSQL::operator != (TipoPgSQL tipo)
{
 return(this->idx_tipo!=tipo.idx_tipo);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator != (unsigned idx_tipo)
{
 return(this->idx_tipo!=idx_tipo);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator == (TipoPgSQL tipo)
{
 return(this->idx_tipo==tipo.idx_tipo);
}
//-----------------------------------------------------------
bool TipoPgSQL::operator == (void *ptipo)
{
 int idx;
 idx=obterIndiceTipoUsuario("",ptipo);
 return(static_cast<int>(idx_tipo) == idx);
}
//-----------------------------------------------------------
TipoIntervalo TipoPgSQL::obterTipoIntervalo(void)
{
 return(tipo_intervalo);
}
//-----------------------------------------------------------
bool TipoPgSQL::comTimezone(void)
{
 return(com_timezone);
}
//-----------------------------------------------------------
bool TipoPgSQL::tipoOID(void)
{
 //Retorna se o tipo está no conjunto de tipos identificadores de objetos (oid)
 return(idx_tipo>=ini_oid && idx_tipo<=fim_oid);
}
//-----------------------------------------------------------
bool TipoPgSQL::pseudoTipo(void)
{
 //Retorna se o tipo está no conjunto de pseudotipos
 return(idx_tipo>=ini_pseudo && idx_tipo<=fim_pseudo);
}
//-----------------------------------------------------------
unsigned TipoPgSQL::operator << (void *ptipo)
{
 definirTipoUsuario(ptipo);
 return(idx_tipo);
}
//-----------------------------------------------------------
void TipoPgSQL::definirTipoIntervalo(TipoIntervalo tipo_interv)
{
 tipo_intervalo=tipo_interv;
}
//-----------------------------------------------------------
void TipoPgSQL::definirComTimezone(bool com_timezone)
{
 this->com_timezone=com_timezone;
}
//-----------------------------------------------------------
void TipoPgSQL::definirTipoUsuario(unsigned idx)
{
 unsigned lim1, lim2;

 lim1=offset + qtd_tipos;
 lim2=offset + qtd_tipos + TipoPgSQL::tipos_usr.size();

 if(TipoPgSQL::tipos_usr.size() > 0 &&
    (idx >= lim1 && idx < lim2))
  idx_tipo=idx;
 else
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
void TipoPgSQL::definirTipoUsuario(void *ptipo)
{
 int idx;

 idx=obterIndiceTipoUsuario("",ptipo);
 if(idx <= 0)
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  idx_tipo=idx;
}
//-----------------------------------------------------------
void TipoPgSQL::adicionarTipoUsuario(const QString &nome, void *ptipo, void *pmodelo, bool dominio)
{
 if(nome!="" && ptipo && pmodelo && obterIndiceTipoUsuario(nome,ptipo,pmodelo)==0)
 {
  ConfigTipoUsuario cfg;

  cfg.nome=nome;
  cfg.ptipo=ptipo;
  cfg.pmodelo=pmodelo;
  cfg.dominio=dominio;
  TipoPgSQL::tipos_usr.push_back(cfg);
 }
}
//-----------------------------------------------------------
void TipoPgSQL::removerTipoUsuario(const QString &nome, void *ptipo)
{
 if(TipoPgSQL::tipos_usr.size() > 0 &&
    nome!="" && ptipo)
 {
  ConfigTipoUsuario cfg;
  vector<ConfigTipoUsuario>::iterator itr, itr_end;

  itr=TipoPgSQL::tipos_usr.begin();
  itr_end=TipoPgSQL::tipos_usr.end();

  while(itr!=itr_end)
  {
   if(itr->nome==nome && itr->ptipo==ptipo) break;
   else itr++;
  }

  if(itr!=itr_end)
   TipoPgSQL::tipos_usr.erase(itr);
 }
}
//-----------------------------------------------------------
void TipoPgSQL::renomearTipoUsuario(const QString &nome, void *ptipo,const QString &novo_nome)
{
 if(TipoPgSQL::tipos_usr.size() > 0 &&
    nome!="" && ptipo && nome!=novo_nome)
 {
  ConfigTipoUsuario cfg;
  vector<ConfigTipoUsuario>::iterator itr, itr_end;

  itr=TipoPgSQL::tipos_usr.begin();
  itr_end=TipoPgSQL::tipos_usr.end();

  while(itr!=itr_end)
  {
   if(itr->nome==nome && itr->ptipo==ptipo)
   {
    itr->nome=novo_nome;
    break;
   }
   itr++;
  }
 }
}
//-----------------------------------------------------------
unsigned TipoPgSQL::obterIndiceTipoBase(const QString &nome)
{
 return(obterTipo(nome,offset,qtd_tipos));
}
//-----------------------------------------------------------
unsigned TipoPgSQL::obterIndiceTipoUsuario(const QString &nome, void *ptipo, void *pmodelo)
{
 if(TipoPgSQL::tipos_usr.size() > 0 && (nome!="" || ptipo))
 {
  ConfigTipoUsuario cfg;
  vector<ConfigTipoUsuario>::iterator itr, itr_end;
  int idx=0;

  itr=TipoPgSQL::tipos_usr.begin();
  itr_end=TipoPgSQL::tipos_usr.end();

  while(itr!=itr_end)
  {
   if(((nome!="" && itr->nome==nome) || (ptipo && itr->ptipo==ptipo)) &&
      ((pmodelo && itr->pmodelo==pmodelo) || !pmodelo))
    break;

   idx++;
   itr++;
  }

  if(itr!=itr_end)
   return(offset + qtd_tipos + idx);
  else
   return(TipoBase::nulo);
 }
 else return(TipoBase::nulo);
}
//-----------------------------------------------------------
QString TipoPgSQL::obterNomeTipoUsuario(unsigned idx)
{
 unsigned lim1, lim2;

 lim1=offset + qtd_tipos;
 lim2=offset + qtd_tipos + TipoPgSQL::tipos_usr.size();

 if(TipoPgSQL::tipos_usr.size() > 0 &&
    (idx >= lim1 && idx < lim2))
  return(TipoPgSQL::tipos_usr[idx - lim1].nome);
 else
  return("");
}
//-----------------------------------------------------------
void TipoPgSQL::obterTiposUsuario(QStringList &tipos, void *pmodelo, bool inc_tipo_usr, bool inc_dominio)
{
 unsigned idx,total;

 tipos.clear(); //Limpa a lista de tipos
 total=TipoPgSQL::tipos_usr.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(tipos_usr[idx].pmodelo==pmodelo &&
    ((inc_tipo_usr && !tipos_usr[idx].dominio) ||
     (inc_dominio && tipos_usr[idx].dominio)))
   tipos.push_back(tipos_usr[idx].nome);
 }
}
//-----------------------------------------------------------
void TipoPgSQL::obterTiposUsuario(vector<void *> &ptipos, void *pmodelo, bool inc_tipo_usr, bool inc_dominio)
{
 unsigned idx, total;

 ptipos.clear(); //Limpa a lista de tipos
 total=TipoPgSQL::tipos_usr.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(tipos_usr[idx].pmodelo==pmodelo &&
    ((inc_tipo_usr && !tipos_usr[idx].dominio) ||
     (inc_dominio && tipos_usr[idx].dominio)))
   ptipos.push_back(tipos_usr[idx].ptipo);
 }
}
//-----------------------------------------------------------
QString TipoPgSQL::operator ~ (void)
{
 if(idx_tipo >= offset + qtd_tipos)
  return(tipos_usr[idx_tipo - (offset + qtd_tipos)].nome);
 else
  return(TipoBase::tipos[idx_tipo]);
}
//-----------------------------------------------------------
bool TipoPgSQL::tipoArray(void)
{
 return(dimensao > 0);
}
//-----------------------------------------------------------
bool TipoPgSQL::tipoUsuario(void)
{
 return(idx_tipo > fim_pseudo);
}
//-----------------------------------------------------------
bool TipoPgSQL::tipoCompVariavel(void )
{
return(tipos[this->idx_tipo]=="numeric" || tipos[this->idx_tipo]=="decimal" ||
       tipos[this->idx_tipo]=="character varying" || tipos[this->idx_tipo]=="varchar" ||
       tipos[this->idx_tipo]=="character" || tipos[this->idx_tipo]=="char" ||
       tipos[this->idx_tipo]=="bit" || tipos[this->idx_tipo]=="bit varying" ||
       tipos[this->idx_tipo]=="varbit");
}
//-----------------------------------------------------------
bool TipoPgSQL::tipoAceitaPrecisao(void )
{
 return(tipos[this->idx_tipo]=="numeric" || tipos[this->idx_tipo]=="decimal" ||
        tipos[this->idx_tipo]=="time" || tipos[this->idx_tipo]=="timestamp" ||
        tipos[this->idx_tipo]=="interval");
}
//-----------------------------------------------------------
void TipoPgSQL::definirDimensao(unsigned dim)
{
 if(dim > 0 && this->tipoUsuario())
 {
  int idx=obterIndiceTipoUsuario(~(*this), NULL);
  if(tipos_usr[idx].dominio)
    throw Excecao(ERR_PGMODELER_ATRDIMENSAOINVDOMINIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 dimensao=dim;
}
//-----------------------------------------------------------
void TipoPgSQL::definirComprimento(unsigned comp)
{
 //Caso o usuário tente criar um tipo de tamanho zero
 if(comp==0)
  //throw Excecao("Atribuição de comprimento igual a zero!");
  throw Excecao(ERR_PGMODELER_ATRCOMPRIMZERO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define o comprimento do tipo da coluna
  this->comprimento=comp;
}
//-----------------------------------------------------------
void TipoPgSQL::definirPrecisao(int prec)
{
 //Caso o usuário tente definir uma precisao maior que o comprimento do tipo
 if(((TipoBase::tipos[idx_tipo]=="numeric" ||
      TipoBase::tipos[idx_tipo]=="decimal") && prec > static_cast<int>(comprimento)))
  throw Excecao(ERR_PGMODELER_ATRPRECISAOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(((TipoBase::tipos[idx_tipo]=="time" ||
           TipoBase::tipos[idx_tipo]=="timestamp" ||
           TipoBase::tipos[idx_tipo]=="interval") && prec > 6))
  throw Excecao(ERR_PGMODELER_ATRPRECTIPOTIMEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define a precisão do tipo da coluna
  this->precisao=prec;
}
//-----------------------------------------------------------
unsigned TipoPgSQL::obterDimensao(void)
{
 return(dimensao);
}
//-----------------------------------------------------------
unsigned TipoPgSQL::obterComprimento(void)
{
 return(comprimento);
}
//-----------------------------------------------------------
int TipoPgSQL::obterPrecisao(void)
{
 return(precisao);
}
//-----------------------------------------------------------
QString TipoPgSQL::obterDefinicaoObjeto(unsigned tipo_def,QString tipo_ref)
{
 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  return(*(*this));
 else
 {
  map<QString, QString> atributos;

  atributos[AtributosParsers::COMPRIMENTO]="";
  atributos[AtributosParsers::DIMENSAO]="";
  atributos[AtributosParsers::PRECISAO]="";
  atributos[AtributosParsers::COM_TIMEZONE]="";
  atributos[AtributosParsers::TIPO_INTERVALO]="";
  atributos[AtributosParsers::TIPO_REFERENCIA]=tipo_ref;

  atributos[AtributosParsers::NOME]=(~(*this));

  if(comprimento > 1)
   atributos[AtributosParsers::COMPRIMENTO]=QString("%1").arg(this->comprimento);

  if(dimensao > 0)
   atributos[AtributosParsers::DIMENSAO]=QString("%1").arg(this->dimensao);

  if(precisao >= 0)
   atributos[AtributosParsers::PRECISAO]=QString("%1").arg(this->precisao);

  if(tipo_intervalo != TipoBase::nulo)
   atributos[AtributosParsers::TIPO_INTERVALO]=(~tipo_intervalo);

  if(com_timezone)
   atributos[AtributosParsers::COM_TIMEZONE]="1";

  return(ParserEsquema::obterDefinicaoObjeto("basetype",atributos, tipo_def));
 }
}
//-----------------------------------------------------------
QString TipoPgSQL::operator * (void)
{
 QString tipo_fmt, tipo, aux;
 unsigned idx;

 tipo=~(*this);
 if(comprimento > 1 && tipoCompVariavel())
 {
  /* Tratando o caso de tipos que necessitam de uma precisão.
     A sintaxe desses tipos se altera ficando na forma TIPO(COMPRIMENTO,PRECISÃO).*/
  if((tipo=="numeric" || tipo=="decimal") && precisao>=0 &&
     precisao<=static_cast<int>(comprimento))
   aux=QString("%1(%2,%3)").arg(TipoBase::tipos[idx_tipo]).arg(comprimento).arg(precisao);
   /* Trantado o caso dos tipos que necessitam apenas do comprimento */
  else
   aux=QString("%1(%2)").arg(TipoBase::tipos[idx_tipo]).arg(comprimento);

  tipo_fmt=aux;
 }
 else if(tipo!="numeric" && tipo!="decimal" && tipoAceitaPrecisao())
 {
  /* Tratando os tipos time e timestampe que possuem o formato:
     [TIME|TIMESTAMP] (PRECISÃO) [WITH|WITHOUT] TIMEZONE */
  if(tipo!="interval")
  {
   aux=TipoBase::tipos[idx_tipo];

   if(precisao >= 0)
    aux+=QString("(%1)").arg(precisao);

   //aux+=(com_timezone ? " with" : " without");
   //aux+=" time zone";
   if(com_timezone)
    aux+="with time zone";
  }
  /* Tratando o tipo interval que possuem o formato:
     INTERVAL [TIPO_INTERVALO](PRECISÃO) */
  else
  {
   aux=TipoBase::tipos[idx_tipo];

   if(tipo_intervalo!=TipoBase::nulo)
    aux+=QString("[%1]").arg(~tipo_intervalo);

   if(precisao >= 0)
    aux+=QString("(%1)").arg(precisao);
  }

  tipo_fmt=aux;
 }
 else
  tipo_fmt=tipo;


 if(tipo!="void" && dimensao > 0)
 {
  for(idx=0; idx < dimensao; idx++)
   tipo_fmt+="[]";
 }

 return(tipo_fmt);
}
//***********************************************************
/*************************
 * CLASSE: TipoCompFuncao *
 *************************/
TipoComportamento::TipoComportamento(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoComportamento::TipoComportamento(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoComportamento::TipoComportamento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoComportamento::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoComportamento::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoComportamento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/*************************
 * CLASSE: TipoSeguranca *
 *************************/
TipoSeguranca::TipoSeguranca(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoSeguranca::TipoSeguranca(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoSeguranca::TipoSeguranca(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
void TipoSeguranca::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoSeguranca::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoSeguranca::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/*************************
 * CLASSE: TipoLinguagem *
 *************************/
TipoLinguagem::TipoLinguagem(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoLinguagem::TipoLinguagem(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoLinguagem::TipoLinguagem(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoLinguagem::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoLinguagem::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoLinguagem::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/***************************
 * CLASSE: TipoCodificacao *
 ***************************/
TipoCodificacao::TipoCodificacao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoCodificacao::TipoCodificacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoCodificacao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoCodificacao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoCodificacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
bool TipoCodificacao::operator == (const char *nome_tipo)
{
 return((*this)==QString(nome_tipo));
}
//-----------------------------------------------------------
bool TipoCodificacao::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + qtd_tipos; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==TipoBase::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(idx_tipo==idx);
}
//-----------------------------------------------------------
bool TipoCodificacao::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}
//-----------------------------------------------------------
bool TipoCodificacao::operator != (TipoCodificacao tipo)
{
 return(this->idx_tipo!=tipo.idx_tipo);
}
//***********************************************************
/*****************************
 * CLASSE: TipoArmazenamento *
 *****************************/
TipoArmazenamento::TipoArmazenamento(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoArmazenamento::TipoArmazenamento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoArmazenamento::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoArmazenamento::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoArmazenamento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
bool TipoArmazenamento::operator == (const char *nome_tipo)
{
 return((*this)==QString(nome_tipo));
}
//-----------------------------------------------------------
bool TipoArmazenamento::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + qtd_tipos; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==TipoBase::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(idx_tipo==idx);
}
//-----------------------------------------------------------
bool TipoArmazenamento::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}
//-----------------------------------------------------------
bool TipoArmazenamento::operator != (TipoArmazenamento tipo)
{
 return(this->idx_tipo!=tipo.idx_tipo);
}
//***********************************************************
/*************************
 * CLASSE: TipoComparacao *
 *************************/
TipoComparacao::TipoComparacao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoComparacao::TipoComparacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoComparacao::TipoComparacao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
void TipoComparacao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoComparacao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoComparacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/***************************
 * CLASSE: TipoPostergacao *
 ***************************/
TipoPostergacao::TipoPostergacao(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoPostergacao::TipoPostergacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoPostergacao::TipoPostergacao(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
void TipoPostergacao::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoPostergacao::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoPostergacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/***************************
 * CLASSE: TipoCategoria *
 ***************************/
TipoCategoria::TipoCategoria(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoCategoria::TipoCategoria(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
TipoCategoria::TipoCategoria(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
void TipoCategoria::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoCategoria::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoCategoria::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
/************************
 * CLASSE: TipoExecucao *
 ************************/
TipoDisparo::TipoDisparo(void)
{
 idx_tipo=offset;
}
//-----------------------------------------------------------
TipoDisparo::TipoDisparo(unsigned tipo)
{
 (*this)=tipo;
}
//-----------------------------------------------------------
TipoDisparo::TipoDisparo(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}
//-----------------------------------------------------------
void TipoDisparo::obterTipos(QStringList &tipos)
{
 TipoBase::obterTipos(tipos,offset,qtd_tipos);
}
//-----------------------------------------------------------
unsigned TipoDisparo::operator = (unsigned tipo)
{
 TipoBase::definirTipo(tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//-----------------------------------------------------------
unsigned TipoDisparo::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=TipoBase::obterTipo(nome_tipo, offset, qtd_tipos);
 TipoBase::definirTipo(idx_tipo,offset,qtd_tipos);
 return(idx_tipo);
}
//***********************************************************
