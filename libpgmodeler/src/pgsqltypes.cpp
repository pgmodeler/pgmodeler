#include "pgsqltypes.h"

/********************
 * CLASSE: TipoBase *
 ********************/
QString BaseType::tipos[types_count]=
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

 //Tipos espaciais específicos da extensão PostGis (integrantes da classe TipoPgSQL)
 //offsets 62 a 66
 "box2d","box3d","geometry",
 "geometry_dump","geography",

 //Tipos identificadores de objeto (OID)
 //offsets 67 a 78
 "oid", "regproc", "regprocedure",
 "regoper", "regoperator", "regclass",
 "regtype", "regconfig", "regdictionary",
 "xid", "cid", "tid",

 //Pseudo-tipos
 //offsets 79 a 90
 "any","anyarray","anyelement","anyenum",
 "anynonarray","cstring","internal","language_handler",
 "record","trigger","void","opaque",

 /* Tipos utilizados pela classe TipoIntervalo usado
    como auxiliar da classe TipoPgSQL quando se trata
    do tipo de dado "interval" */
 //offsets 91 a 103
 "YEAR", "MONTH", "DAY", "HOUR",
 "MINUTE", "SECOND","YEAR TO MONTH",
 "DAY TO HOUR","DAY TO MINUTE","DAY TO SECOND",
 "HOUR TO MINUTE","HOUR TO SECOND","MINUTE TO SECOND",

 //Tipos utilizados pela classe TipoComportamento
 //offsets 104 a 106
 "CALLED ON NULL INPUT",
 "RETURNS NULL ON NULL INPUT",
 "STRICT",

 //Tipos utilizados pela classe TipoSeguranca
 //offsets 107 a 108
 "SECURITY INVOKER",
 "SECURITY DEFINER",

 //Tipos utilizados pela classe TipoLinguagem
 //offsets 109 a 114
 "sql",
 "c",
 "plpgsql",
 "pltcl",
 "plperl",
 "plpython",

 //Tipos utilizados pela classe TipoCodificacao
 //offsets 115 a 155
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
 //offsets 156 a 159
 "plain",
 "external",
 "extended",
 "main",

 //Tipos utilizados pela classe TipoComparacao
 //offsets 160 a 162
 "MATCH FULL",
 "MATCH PARTIAL",
 "MATCH SIMPLE",

 //Tipos utilizados pela classe TipoPostergacao
 //offsets 163 a 164
 "INITIALLY IMMEDIATE",
 "INITIALLY DEFERRED",

 //Tipos utilizados pela classe TipoCategoria
 //offsets 165 a 178 - Vide tabela 44-43 da Documentação do PostgreSQL 8.4
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
 //offsets 179 a 181
 "BEFORE",
 "AFTER",
 "INSTEAD OF",

 /* Tipos auxiliares usados pela classe TipoEspacial na configuração de
    tipos de objetos do PostGiS na class TipoPgSQL.
    Estes tipos aceitam as variações Z, M e ZM.
     > Exemplo: POINT, POINTZ, POINTM, POINTZM
    Referência: http://postgis.refractions.net/documentation/manual-2.0/using_postgis_dbmanagement.html */
 //offsets 182 a 188
 "POINT",
 "LINESTRING",
 "POLYGON",
 "MULTIPOINT",
 "MULTILINESTRING",
 "MULTIPOLYGON",
 "GEOMETRYCOLLECTION"
};

BaseType::BaseType(void)
{
 type_idx=0;
}

QString BaseType::getTypeString(unsigned type_id)
{
 if(type_id > types_count)
  throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(tipos[type_id]);
}

void BaseType::setType(unsigned type_id,unsigned offset,unsigned count)
{
 /* Caso a quantidade de tipos seja nula ou maior do que o tamanho da lista de tipos
    da classe base, dispara um exceção indicando o fato */
 if(count==0 || count > this->types_count)
  throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Caso o tipo a ser atribuido não seja pertecente a classe de tipo atual
 else if(!isTypeValid(type_id,offset,count))
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  type_idx=type_id;
}

bool BaseType::isTypeValid(unsigned type_id,unsigned offset,unsigned count)
{
 //Retorna se o indice do tipo está no intervalo (offset-qtd_tipos) da classe
 return((type_id>=offset && type_id<=(offset+count-1)) || type_id==0);
}

void BaseType::getTypes(QStringList &types,unsigned offset,unsigned count)
{
 /* Caso a quantidade de tipos seja nula ou maior do que o tamanho da lista de tipos
    da classe base, dispara um exceção indicando o fato */
 if(count==0 || count > BaseType::types_count)
  throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  unsigned idx,total;

  types.clear(); //Limpa a lista de tipos
  total=offset+count; //Calcula a quantidade total de tipos a serem obtidos

  for(idx=offset; idx<total; idx++)
   //Insere na lista os tipos que vao do offset ao total de tipos
   types.push_back(BaseType::tipos[idx]);
 }
}

unsigned BaseType::getType(const QString &type_name,unsigned offset,unsigned count)
{
 unsigned idx,total;
 bool found=false;

 if(type_name=="")
  return(BaseType::null);
 else
 {
  total=offset + count; //Calculando o total de tipos da classe

  /*Verifica se o tipo passado pelo parametro está no conjunto de
   tipos da classe */
  for(idx=offset; idx<total && !found; idx++)
   found=(type_name==BaseType::tipos[idx]);

  if(found)
  { idx--; return(idx); }
  else
   return(BaseType::null);
 }
}

QString BaseType::operator ~ (void)
{
 return(tipos[type_idx]);
}

unsigned BaseType::operator ! (void)
{
 return(type_idx);
}

bool BaseType::operator == (BaseType &type)
{
 return(type.type_idx == type_idx);
}

bool BaseType::operator == (unsigned type_id)
{
 return(type_idx==type_id);
}

bool BaseType::operator != (BaseType &type)
{
 return(type.type_idx != type_idx);
}

bool BaseType::operator != (unsigned type_id)
{
 return(type_idx!=type_id);
}

/********************
 * CLASSE: TipoAcao *
 ********************/
ActionType::ActionType(unsigned tipo)
{
 (*this)=tipo;
}

ActionType::ActionType(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

ActionType::ActionType(void)
{
 type_idx=offset;
}

void ActionType::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned ActionType::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned ActionType::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/**************************
 * CLASSE: TipoRestricao *
 **************************/

TipoRestricao::TipoRestricao(unsigned tipo)
{
 (*this)=tipo;
}

TipoRestricao::TipoRestricao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoRestricao::TipoRestricao(void)
{
 type_idx=offset;
}

void TipoRestricao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoRestricao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoRestricao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/**********************
 * CLASSE: TipoEvento *
 **********************/
TipoEvento::TipoEvento(void)
{
 type_idx=offset;
}

TipoEvento::TipoEvento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoEvento::TipoEvento(unsigned tipo)
{
 (*this)=tipo;
}

void TipoEvento::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoEvento::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoEvento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

bool TipoEvento::operator < (TipoEvento tipo) const
{
 return(type_idx < tipo.type_idx);
}

bool TipoEvento::operator < (unsigned idx) const
{
 return(type_idx < idx);
}

/************************
 * CLASSE: TipoExecucao *
 ************************/
TipoExecucao::TipoExecucao(void)
{
 type_idx=offset;
}

TipoExecucao::TipoExecucao(unsigned tipo)
{
 (*this)=tipo;
}

TipoExecucao::TipoExecucao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoExecucao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoExecucao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoExecucao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/**********************
 * CLASSE: TipoFuncao *
 **********************/
TipoFuncao::TipoFuncao(unsigned tipo)
{
 (*this)=tipo;
}

TipoFuncao::TipoFuncao(void)
{
 type_idx=offset;
}

TipoFuncao::TipoFuncao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoFuncao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoFuncao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoFuncao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/**********************
 * CLASSE: TipoIndexacao *
 **********************/
TipoIndexacao::TipoIndexacao(unsigned tipo)
{
 (*this)=tipo;
}

TipoIndexacao::TipoIndexacao(void)
{
 type_idx=offset;
}

TipoIndexacao::TipoIndexacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoIndexacao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoIndexacao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoIndexacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/**********************
 * CLASSE: TipoIntervalo *
 **********************/
TipoIntervalo::TipoIntervalo(unsigned tipo)
{
 (*this)=tipo;
}

TipoIntervalo::TipoIntervalo(void)
{
 type_idx=BaseType::null;
}

TipoIntervalo::TipoIntervalo(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoIntervalo::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoIntervalo::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoIntervalo::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/************************
 * CLASSE: TipoEspacial *
 ************************/
TipoEspacial::TipoEspacial(const QString &nome_tipo, unsigned variacao)
{
 BaseType::setType(BaseType::getType(nome_tipo, offset, types_count),
                       offset, types_count);
 definirVariacao(variacao);
}

TipoEspacial::TipoEspacial(unsigned tipo, unsigned variacao)
{
 BaseType::setType(tipo,offset,types_count);
 definirVariacao(variacao);
}

TipoEspacial::TipoEspacial(void)
{
 type_idx=point;
 variacao=no_var;
}

void TipoEspacial::definirVariacao(unsigned var)
{
 if(var > var_zm)
  variacao=var_zm;
 else
  variacao=var;
}

unsigned TipoEspacial::obterVariacao(void)
{
 return(variacao);
}

void TipoEspacial::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

QString TipoEspacial::operator * (void)
{
 QString var_str;

 switch(variacao)
 {
  case var_z: var_str+="Z"; break;
  case var_m: var_str+="M"; break;
  case var_zm: var_str+="ZM"; break;
  default: var_str=""; break;
 }

 //Atualmente o PostGiS aceita somente SRID = 4326 (Vide documentação Postgis 2.0)
 return(QString("(%1%2, 4326)").arg(tipos[type_idx]).arg(var_str));
}

/*********************
 * CLASSE: TipoPgSQL *
 *********************/
//Inicializando a lista estática da classe
vector<ConfigTipoUsuario> TipoPgSQL::tipos_usr;

TipoPgSQL::TipoPgSQL(void)
{
 type_idx=offset;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}

TipoPgSQL::TipoPgSQL(const QString &nome_tipo)
{
 (*this)=nome_tipo;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}

TipoPgSQL::TipoPgSQL(void *ptipo)
{
 (*this) << ptipo;
 comprimento=1;
 precisao=-1;
 dimensao=0;
 com_timezone=false;
}

TipoPgSQL::TipoPgSQL(void *ptipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv, TipoEspacial tipo_esp)
{
 (*this) << ptipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
 definirTipoEspacial(tipo_esp);
}

TipoPgSQL::TipoPgSQL(const QString &tipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv, TipoEspacial tipo_esp)
{
 (*this)=tipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
 definirTipoEspacial(tipo_esp);
}

TipoPgSQL::TipoPgSQL(unsigned idx_tipo, unsigned comprimento, unsigned dimensao, int precisao, bool com_timezone, TipoIntervalo tipo_interv, TipoEspacial tipo_esp)
{
 (*this)=idx_tipo;
 definirComprimento(comprimento);
 definirDimensao(dimensao);
 definirPrecisao(precisao);
 definirComTimezone(com_timezone);
 definirTipoIntervalo(tipo_interv);
 definirTipoEspacial(tipo_esp);
}

void TipoPgSQL::getTypes(QStringList &tipos, bool tipo_oid, bool pseudos)
{
 unsigned idx,total;

 tipos.clear(); //Limpa a lista de tipos
 total=offset+types_count; //Calcula a quantidade total de tipos a serem obtidos

 for(idx=offset; idx<total; idx++)
 {
  //Insere na lista os tipos que vao do offset ao total de tipos
  if(idx<ini_oid ||
    (tipo_oid && idx>=ini_oid && idx<=fim_oid) ||
    (pseudos && idx>=ini_pseudo && idx<=fim_pseudo))
  tipos.push_back(BaseType::tipos[idx]);
 }
}

unsigned TipoPgSQL::operator = (unsigned tipo)
{
 if(tipo>=offset)
  definirTipoUsuario(tipo);
 else if(tipo > 0)
  BaseType::setType(tipo,offset,types_count);
 else if(tipo==0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(type_idx);
}

unsigned TipoPgSQL::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo, idx_tipo_usr;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 idx_tipo_usr=obterIndiceTipoUsuario(nome_tipo, NULL);

 if(idx_tipo==0 && idx_tipo_usr==0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(idx_tipo!=0)
 {
  BaseType::setType(idx_tipo,offset,types_count);
  return(idx_tipo);
 }
 else
 {
  definirTipoUsuario(idx_tipo_usr);
  return(idx_tipo_usr);
 }
}

void *TipoPgSQL::obterRefTipoUsuario(void)
{
 if(this->tipoUsuario())
  return(tipos_usr[this->type_idx - (fim_pseudo + 1)].ptipo);
 else
  return(NULL);
}

unsigned TipoPgSQL::obterConfTipoUsuario(void)
{
 if(this->tipoUsuario())
  return(tipos_usr[this->type_idx - (fim_pseudo + 1)].conf_tipo);
 else
  return(0);
}

bool TipoPgSQL::operator == (unsigned idx_tipo)
{
 return(this->type_idx==idx_tipo);
}

bool TipoPgSQL::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + types_count; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==BaseType::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(type_idx==idx);
}

bool TipoPgSQL::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}

bool TipoPgSQL::operator != (TipoPgSQL tipo)
{
 return(this->type_idx!=tipo.type_idx);
}

bool TipoPgSQL::operator != (unsigned idx_tipo)
{
 return(this->type_idx!=idx_tipo);
}

bool TipoPgSQL::operator == (TipoPgSQL tipo)
{
 return(this->type_idx==tipo.type_idx);
}

bool TipoPgSQL::operator == (void *ptipo)
{
 int idx;
 idx=obterIndiceTipoUsuario("",ptipo);
 return(static_cast<int>(type_idx) == idx);
}

TipoIntervalo TipoPgSQL::obterTipoIntervalo(void)
{
 return(tipo_intervalo);
}

TipoEspacial TipoPgSQL::obterTipoEspacial(void)
{
 return(tipo_espacial);
}

bool TipoPgSQL::comTimezone(void)
{
 return(com_timezone);
}

bool TipoPgSQL::tipoOID(void)
{
 //Retorna se o tipo está no conjunto de tipos identificadores de objetos (oid)
 return(type_idx>=ini_oid && type_idx<=fim_oid);
}

bool TipoPgSQL::pseudoTipo(void)
{
 //Retorna se o tipo está no conjunto de pseudotipos
 return(type_idx>=ini_pseudo && type_idx<=fim_pseudo);
}

unsigned TipoPgSQL::operator << (void *ptipo)
{
 definirTipoUsuario(ptipo);
 return(type_idx);
}

void TipoPgSQL::definirTipoIntervalo(TipoIntervalo tipo_interv)
{
 tipo_intervalo=tipo_interv;
}

void TipoPgSQL::definirTipoEspacial(TipoEspacial tipo_esp)
{
 tipo_espacial=tipo_esp;
}

void TipoPgSQL::definirComTimezone(bool com_timezone)
{
 this->com_timezone=com_timezone;
}

void TipoPgSQL::definirTipoUsuario(unsigned idx)
{
 unsigned lim1, lim2;

 //lim1=offset + qtd_tipos +;
 //lim2=offset + qtd_tipos + TipoPgSQL::tipos_usr.size();
 lim1=fim_pseudo + 1;
 lim2=lim1 + TipoPgSQL::tipos_usr.size();

 if(TipoPgSQL::tipos_usr.size() > 0 &&
    (idx >= lim1 && idx < lim2))
  type_idx=idx;
 else
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void TipoPgSQL::definirTipoUsuario(void *ptipo)
{
 int idx;

 idx=obterIndiceTipoUsuario("",ptipo);
 if(idx <= 0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  type_idx=idx;
}

void TipoPgSQL::adicionarTipoUsuario(const QString &nome, void *ptipo, void *pmodelo, unsigned conf_tipo_usr)
{
 if(nome!="" && ptipo && pmodelo &&
    (conf_tipo_usr==ConfigTipoUsuario::TIPO_DOMINIO ||
     conf_tipo_usr==ConfigTipoUsuario::TIPO_SEQUENCIA ||
     conf_tipo_usr==ConfigTipoUsuario::TIPO_TABELA ||
     conf_tipo_usr==ConfigTipoUsuario::TIPO_BASE) &&
    obterIndiceTipoUsuario(nome,ptipo,pmodelo)==0)
 {
  ConfigTipoUsuario cfg;

  cfg.nome=nome;
  cfg.ptipo=ptipo;
  cfg.pmodelo=pmodelo;
  cfg.conf_tipo=conf_tipo_usr;
  TipoPgSQL::tipos_usr.push_back(cfg);
 }
}

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

void TipoPgSQL::renomearTipoUsuario(const QString &nome, void *ptipo,const QString &novo_nome)
{
 if(TipoPgSQL::tipos_usr.size() > 0 &&
    nome!="" && ptipo && nome!=novo_nome)
 {
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

unsigned TipoPgSQL::obterIndiceTipoBase(const QString &nome)
{
 return(getType(nome,offset,types_count));
}

unsigned TipoPgSQL::obterIndiceTipoUsuario(const QString &nome, void *ptipo, void *pmodelo)
{
 if(TipoPgSQL::tipos_usr.size() > 0 && (nome!="" || ptipo))
 {
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
   //return(offset + qtd_tipos + idx);
   return(fim_pseudo + 1 + idx);
  else
   return(BaseType::null);
 }
 else return(BaseType::null);
}

QString TipoPgSQL::getNameTipoUsuario(unsigned idx)
{
 unsigned lim1, lim2;

 /*lim1=offset + qtd_tipos;
 lim2=offset + qtd_tipos + TipoPgSQL::tipos_usr.size();*/

 lim1=fim_pseudo + 1;
 lim2=lim1 + TipoPgSQL::tipos_usr.size();


 if(TipoPgSQL::tipos_usr.size() > 0 &&
    (idx >= lim1 && idx < lim2))
  return(TipoPgSQL::tipos_usr[idx - lim1].nome);
 else
  return("");
}

void TipoPgSQL::obterTiposUsuario(QStringList &tipos, void *pmodelo, unsigned inc_tipos_usr)
{
 unsigned idx,total;

 tipos.clear(); //Limpa a lista de tipos
 total=TipoPgSQL::tipos_usr.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(tipos_usr[idx].pmodelo==pmodelo &&
     ((inc_tipos_usr & tipos_usr[idx].conf_tipo) == tipos_usr[idx].conf_tipo))
   tipos.push_back(tipos_usr[idx].nome);
 }
}

void TipoPgSQL::obterTiposUsuario(vector<void *> &ptipos, void *pmodelo, unsigned inc_tipos_usr)
{
 unsigned idx, total;

 ptipos.clear(); //Limpa a lista de tipos
 total=TipoPgSQL::tipos_usr.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(tipos_usr[idx].pmodelo==pmodelo &&
     ((inc_tipos_usr & tipos_usr[idx].conf_tipo) == tipos_usr[idx].conf_tipo))
   ptipos.push_back(tipos_usr[idx].ptipo);
 }
}

QString TipoPgSQL::operator ~ (void)
{
 if(type_idx >= fim_pseudo + 1)//offset + qtd_tipos)
  return(tipos_usr[type_idx - (fim_pseudo + 1)].nome);
 else
  return(BaseType::tipos[type_idx]);
}

bool TipoPgSQL::tipoArray(void)
{
 return(dimensao > 0);
}

bool TipoPgSQL::tipoUsuario(void)
{
 return(type_idx > fim_pseudo);
}

bool TipoPgSQL::tipoCompVariavel(void )
{
return(tipos[this->type_idx]=="numeric" || tipos[this->type_idx]=="decimal" ||
       tipos[this->type_idx]=="character varying" || tipos[this->type_idx]=="varchar" ||
       tipos[this->type_idx]=="character" || tipos[this->type_idx]=="char" ||
       tipos[this->type_idx]=="bit" || tipos[this->type_idx]=="bit varying" ||
       tipos[this->type_idx]=="varbit");
}

bool TipoPgSQL::tipoAceitaPrecisao(void )
{
 return(tipos[this->type_idx]=="numeric" || tipos[this->type_idx]=="decimal" ||
        tipos[this->type_idx]=="time" || tipos[this->type_idx]=="timestamp" ||
        tipos[this->type_idx]=="interval");
}

void TipoPgSQL::definirDimensao(unsigned dim)
{
 if(dim > 0 && this->tipoUsuario())
 {
  int idx=obterIndiceTipoUsuario(~(*this), NULL);
  if(tipos_usr[idx].conf_tipo==ConfigTipoUsuario::TIPO_DOMINIO ||
     tipos_usr[idx].conf_tipo==ConfigTipoUsuario::TIPO_SEQUENCIA)
    throw Exception(ERR_ASG_INV_DOMAIN_ARRAY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 dimensao=dim;
}

void TipoPgSQL::definirComprimento(unsigned comp)
{
 //Caso o usuário tente criar um tipo de tamanho zero
 if(comp==0)
  //throw Excecao("Atribuição de comprimento igual a zero!");
  throw Exception(ERR_ASG_ZERO_LENGTH,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define o comprimento do tipo da coluna
  this->comprimento=comp;
}

void TipoPgSQL::definirPrecisao(int prec)
{
 //Caso o usuário tente definir uma precisao maior que o comprimento do tipo
 if(((BaseType::tipos[type_idx]=="numeric" ||
      BaseType::tipos[type_idx]=="decimal") && prec > static_cast<int>(comprimento)))
  throw Exception(ERR_ASG_INV_PRECISION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(((BaseType::tipos[type_idx]=="time" ||
           BaseType::tipos[type_idx]=="timestamp" ||
           BaseType::tipos[type_idx]=="interval") && prec > 6))
  throw Exception(ERR_ASG_INV_PREC_TIMESTAMP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define a precisão do tipo da coluna
  this->precisao=prec;
}

unsigned TipoPgSQL::obterDimensao(void)
{
 return(dimensao);
}

unsigned TipoPgSQL::obterComprimento(void)
{
 return(comprimento);
}

int TipoPgSQL::obterPrecisao(void)
{
 return(precisao);
}

QString TipoPgSQL::obterDefinicaoObjeto(unsigned tipo_def,QString tipo_ref)
{
 if(tipo_def==SchemaParser::SQL_DEFINITION)
  return(*(*this));
 else
 {
  map<QString, QString> atributos;

  atributos[ParsersAttributes::LENGTH]="";
  atributos[ParsersAttributes::DIMENSION]="";
  atributos[ParsersAttributes::PRECISION]="";
  atributos[ParsersAttributes::WITH_TIMEZONE]="";
  atributos[ParsersAttributes::INTERVAL_TYPE]="";
  atributos[ParsersAttributes::SPATIAL_TYPE]="";
  atributos[ParsersAttributes::VARIATION]="";
  atributos[ParsersAttributes::REF_TYPE]=tipo_ref;

  atributos[ParsersAttributes::NAME]=(~(*this));

  if(comprimento > 1)
   atributos[ParsersAttributes::LENGTH]=QString("%1").arg(this->comprimento);

  if(dimensao > 0)
   atributos[ParsersAttributes::DIMENSION]=QString("%1").arg(this->dimensao);

  if(precisao >= 0)
   atributos[ParsersAttributes::PRECISION]=QString("%1").arg(this->precisao);

  if(tipo_intervalo != BaseType::null)
   atributos[ParsersAttributes::INTERVAL_TYPE]=(~tipo_intervalo);

  if(!tipoUsuario() && tipo_espacial != BaseType::null)
  {
   atributos[ParsersAttributes::SPATIAL_TYPE]=(~tipo_espacial);
   atributos[ParsersAttributes::VARIATION]=QString("%1").arg(tipo_espacial.obterVariacao());
  }

  if(com_timezone)
   atributos[ParsersAttributes::WITH_TIMEZONE]="1";

  return(SchemaParser::getObjectDefinition("basetype",atributos, tipo_def));
 }
}

QString TipoPgSQL::operator * (void)
{
 QString tipo_fmt, tipo, aux;
 unsigned idx;

 tipo=~(*this);

 //Gerando definição de tipos espaciais (PostGiS)
 if(tipo=="geometry" || tipo=="geography")
  tipo_fmt=tipo + (*tipo_espacial);
 else if(comprimento > 1 && tipoCompVariavel())
 {
  /* Tratando o caso de tipos que necessitam de uma precisão.
     A sintaxe desses tipos se altera ficando na forma TIPO(COMPRIMENTO,PRECISÃO).*/
  if((tipo=="numeric" || tipo=="decimal") && precisao>=0 &&
     precisao<=static_cast<int>(comprimento))
   aux=QString("%1(%2,%3)").arg(BaseType::tipos[type_idx]).arg(comprimento).arg(precisao);
   /* Trantado o caso dos tipos que necessitam apenas do comprimento */
  else
   aux=QString("%1(%2)").arg(BaseType::tipos[type_idx]).arg(comprimento);

  tipo_fmt=aux;
 }
 else if(tipo!="numeric" && tipo!="decimal" && tipoAceitaPrecisao())
 {
  /* Tratando os tipos time e timestampe que possuem o formato:
     [TIME|TIMESTAMP] (PRECISÃO) [WITH|WITHOUT] TIMEZONE */
  if(tipo!="interval")
  {
   aux=BaseType::tipos[type_idx];

   if(precisao >= 0)
    aux+=QString("(%1)").arg(precisao);

   //aux+=(com_timezone ? " with" : " without");
   //aux+=" time zone";
   if(com_timezone)
    aux+=" with time zone";
  }
  /* Tratando o tipo interval que possuem o formato:
     INTERVAL [TIPO_INTERVALO](PRECISÃO) */
  else
  {
   aux=BaseType::tipos[type_idx];

   if(tipo_intervalo!=BaseType::null)
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

/*************************
 * CLASSE: TipoCompFuncao *
 *************************/
TipoComportamento::TipoComportamento(unsigned tipo)
{
 (*this)=tipo;
}

TipoComportamento::TipoComportamento(void)
{
 type_idx=offset;
}

TipoComportamento::TipoComportamento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoComportamento::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoComportamento::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoComportamento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/*************************
 * CLASSE: TipoSeguranca *
 *************************/
TipoSeguranca::TipoSeguranca(void)
{
 type_idx=offset;
}

TipoSeguranca::TipoSeguranca(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoSeguranca::TipoSeguranca(unsigned tipo)
{
 (*this)=tipo;
}

void TipoSeguranca::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoSeguranca::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoSeguranca::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/*************************
 * CLASSE: TipoLinguagem *
 *************************/
TipoLinguagem::TipoLinguagem(void)
{
 type_idx=offset;
}

TipoLinguagem::TipoLinguagem(unsigned tipo)
{
 (*this)=tipo;
}

TipoLinguagem::TipoLinguagem(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoLinguagem::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoLinguagem::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoLinguagem::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/***************************
 * CLASSE: TipoCodificacao *
 ***************************/
TipoCodificacao::TipoCodificacao(void)
{
 type_idx=offset;
}

TipoCodificacao::TipoCodificacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoCodificacao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoCodificacao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoCodificacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

bool TipoCodificacao::operator == (const char *nome_tipo)
{
 return((*this)==QString(nome_tipo));
}

bool TipoCodificacao::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + types_count; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==BaseType::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(type_idx==idx);
}

bool TipoCodificacao::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}

bool TipoCodificacao::operator != (TipoCodificacao tipo)
{
 return(this->type_idx!=tipo.type_idx);
}

/*****************************
 * CLASSE: TipoArmazenamento *
 *****************************/
TipoArmazenamento::TipoArmazenamento(void)
{
 type_idx=offset;
}

TipoArmazenamento::TipoArmazenamento(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoArmazenamento::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoArmazenamento::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoArmazenamento::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

bool TipoArmazenamento::operator == (const char *nome_tipo)
{
 return((*this)==QString(nome_tipo));
}

bool TipoArmazenamento::operator == (const QString &nome_tipo)
{
 unsigned idx,total;
 bool enc=false;

 total=offset + types_count; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !enc; idx++)
  enc=(nome_tipo==BaseType::tipos[idx]);

 if(enc) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(type_idx==idx);
}

bool TipoArmazenamento::operator != (const QString &nome_tipo)
{
 return(!((*this)==nome_tipo));
}

bool TipoArmazenamento::operator != (TipoArmazenamento tipo)
{
 return(this->type_idx!=tipo.type_idx);
}

/*************************
 * CLASSE: TipoComparacao *
 *************************/
TipoComparacao::TipoComparacao(void)
{
 type_idx=offset;
}

TipoComparacao::TipoComparacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoComparacao::TipoComparacao(unsigned tipo)
{
 (*this)=tipo;
}

void TipoComparacao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoComparacao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoComparacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/***************************
 * CLASSE: TipoPostergacao *
 ***************************/
TipoPostergacao::TipoPostergacao(void)
{
 type_idx=offset;
}

TipoPostergacao::TipoPostergacao(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoPostergacao::TipoPostergacao(unsigned tipo)
{
 (*this)=tipo;
}

void TipoPostergacao::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoPostergacao::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoPostergacao::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/***************************
 * CLASSE: TipoCategoria *
 ***************************/
TipoCategoria::TipoCategoria(void)
{
 type_idx=offset;
}

TipoCategoria::TipoCategoria(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

TipoCategoria::TipoCategoria(unsigned tipo)
{
 (*this)=tipo;
}

void TipoCategoria::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoCategoria::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoCategoria::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

/************************
 * CLASSE: TipoExecucao *
 ************************/
TipoDisparo::TipoDisparo(void)
{
 type_idx=offset;
}

TipoDisparo::TipoDisparo(unsigned tipo)
{
 (*this)=tipo;
}

TipoDisparo::TipoDisparo(const QString &nome_tipo)
{
 (*this)=nome_tipo;
}

void TipoDisparo::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned TipoDisparo::operator = (unsigned tipo)
{
 BaseType::setType(tipo,offset,types_count);
 return(type_idx);
}

unsigned TipoDisparo::operator = (const QString &nome_tipo)
{
 unsigned idx_tipo;

 idx_tipo=BaseType::getType(nome_tipo, offset, types_count);
 BaseType::setType(idx_tipo,offset,types_count);
 return(idx_tipo);
}

