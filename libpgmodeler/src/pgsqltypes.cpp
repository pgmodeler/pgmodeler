#include "pgsqltypes.h"

/********************
 * CLASSE: TipoBase *
 ********************/
QString BaseType::type_list[types_count]=
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

 return(type_list[type_id]);
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
   types.push_back(BaseType::type_list[idx]);
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
   found=(type_name==BaseType::type_list[idx]);

  if(found)
  { idx--; return(idx); }
  else
   return(BaseType::null);
 }
}

QString BaseType::operator ~ (void)
{
 return(type_list[type_idx]);
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
ActionType::ActionType(unsigned type_id)
{
 (*this)=type_id;
}

ActionType::ActionType(const QString &type_name)
{
 (*this)=type_name;
}

ActionType::ActionType(void)
{
 type_idx=offset;
}

void ActionType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned ActionType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned ActionType::operator = (const QString &type_name)
{
 unsigned type_idx;

 type_idx=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_idx,offset,types_count);
 return(type_idx);
}

/**************************
 * CLASSE: TipoRestricao *
 **************************/

ConstraintType::ConstraintType(unsigned type_id)
{
 (*this)=type_id;
}

ConstraintType::ConstraintType(const QString &type_name)
{
 (*this)=type_name;
}

ConstraintType::ConstraintType(void)
{
 type_idx=offset;
}

void ConstraintType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned ConstraintType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned ConstraintType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

/**********************
 * CLASSE: TipoEvento *
 **********************/
EventType::EventType(void)
{
 type_idx=offset;
}

EventType::EventType(const QString &type_name)
{
 (*this)=type_name;
}

EventType::EventType(unsigned type_id)
{
 (*this)=type_id;
}

void EventType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned EventType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned EventType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

bool EventType::operator < (EventType type) const
{
 return(type_idx < type.type_idx);
}

bool EventType::operator < (unsigned type_id) const
{
 return(type_idx < type_id);
}

/************************
 * CLASSE: TipoExecucao *
 ************************/
ExecutionType::ExecutionType(void)
{
 type_idx=offset;
}

ExecutionType::ExecutionType(unsigned type_id)
{
 (*this)=type_id;
}

ExecutionType::ExecutionType(const QString &type_name)
{
 (*this)=type_name;
}

void ExecutionType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned ExecutionType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned ExecutionType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

/**********************
 * CLASSE: TipoFuncao *
 **********************/
FunctionType::FunctionType(unsigned type_id)
{
 (*this)=type_id;
}

FunctionType::FunctionType(void)
{
 type_idx=offset;
}

FunctionType::FunctionType(const QString &type_name)
{
 (*this)=type_name;
}

void FunctionType::getTypes(QStringList &tipos)
{
 BaseType::getTypes(tipos,offset,types_count);
}

unsigned FunctionType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned FunctionType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

/**********************
 * CLASSE: TipoIndexacao *
 **********************/
IndexingType::IndexingType(unsigned type_id)
{
 (*this)=type_id;
}

IndexingType::IndexingType(void)
{
 type_idx=offset;
}

IndexingType::IndexingType(const QString &type_name)
{
 (*this)=type_name;
}

void IndexingType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned IndexingType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned IndexingType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

/**********************
 * CLASSE: TipoIntervalo *
 **********************/
IntervalType::IntervalType(unsigned type_id)
{
 (*this)=type_id;
}

IntervalType::IntervalType(void)
{
 type_idx=BaseType::null;
}

IntervalType::IntervalType(const QString &type_name)
{
 (*this)=type_name;
}

void IntervalType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

unsigned IntervalType::operator = (unsigned type_id)
{
 BaseType::setType(type_id,offset,types_count);
 return(type_idx);
}

unsigned IntervalType::operator = (const QString &type_name)
{
 unsigned type_id;

 type_id=BaseType::getType(type_name, offset, types_count);
 BaseType::setType(type_id,offset,types_count);
 return(type_id);
}

/************************
 * CLASSE: TipoEspacial *
 ************************/
SpatialType::SpatialType(const QString &type_name, unsigned variation_id)
{
 BaseType::setType(BaseType::getType(type_name, offset, types_count),
                       offset, types_count);
 setVariation(variation_id);
}

SpatialType::SpatialType(unsigned type_id, unsigned var_id)
{
 BaseType::setType(type_id,offset,types_count);
 setVariation(var_id);
}

SpatialType::SpatialType(void)
{
 type_idx=point;
 variacao=no_var;
}

void SpatialType::setVariation(unsigned var)
{
 if(var > var_zm)
  variacao=var_zm;
 else
  variacao=var;
}

unsigned SpatialType::getVariation(void)
{
 return(variacao);
}

void SpatialType::getTypes(QStringList &type_list)
{
 BaseType::getTypes(type_list,offset,types_count);
}

QString SpatialType::operator * (void)
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
 return(QString("(%1%2, 4326)").arg(type_list[type_idx]).arg(var_str));
}

/*********************
 * CLASSE: TipoPgSQL *
 *********************/
//Inicializando a lista estática da classe
vector<UserTypeConfig> PgSQLType::user_types;

PgSQLType::PgSQLType(void)
{
 type_idx=offset;
 length=1;
 precision=-1;
 dimension=0;
 with_timezone=false;
}

PgSQLType::PgSQLType(const QString &type_name)
{
 (*this)=type_name;
 length=1;
 precision=-1;
 dimension=0;
 with_timezone=false;
}

PgSQLType::PgSQLType(void *ptype)
{
 (*this) << ptype;
 length=1;
 precision=-1;
 dimension=0;
 with_timezone=false;
}

PgSQLType::PgSQLType(void *ptype, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
 (*this) << ptype;
 setLength(length);
 setDimension(dimension);
 setPrecision(precision);
 setWithTimezone(with_timezone);
 setIntervalType(interv_type);
 setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(const QString &type_name, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
 (*this)=type_name;
 setLength(length);
 setDimension(dimension);
 setPrecision(precision);
 setWithTimezone(with_timezone);
 setIntervalType(interv_type);
 setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(unsigned type_id, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
 (*this)=type_id;
 setLength(length);
 setDimension(dimension);
 setPrecision(precision);
 setWithTimezone(with_timezone);
 setIntervalType(interv_type);
 setSpatialType(spatial_type);
}

void PgSQLType::getTypes(QStringList &type_list, bool oids, bool pseudos)
{
 unsigned idx,total;

 type_list.clear(); //Limpa a lista de tipos
 total=offset+types_count; //Calcula a quantidade total de tipos a serem obtidos

 for(idx=offset; idx<total; idx++)
 {
  //Insere na lista os tipos que vao do offset ao total de tipos
  if(idx<oid_start ||
    (oids && idx>=oid_start && idx<=oid_end) ||
    (pseudos && idx>=pseudo_start && idx<=pseudo_end))
  type_list.push_back(BaseType::type_list[idx]);
 }
}

unsigned PgSQLType::operator = (unsigned type_id)
{
 if(type_id>=offset)
  setUserType(type_id);
 else if(type_id > 0)
  BaseType::setType(type_id,offset,types_count);
 else if(type_id==0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(type_idx);
}

unsigned PgSQLType::operator = (const QString &type_name)
{
 unsigned type_idx, usr_type_idx;

 type_idx=BaseType::getType(type_name, offset, types_count);
 usr_type_idx=getUserTypeIndex(type_name, NULL);

 if(type_idx==0 && usr_type_idx==0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(type_idx!=0)
 {
  BaseType::setType(type_idx,offset,types_count);
  return(type_idx);
 }
 else
 {
  setUserType(usr_type_idx);
  return(usr_type_idx);
 }
}

void *PgSQLType::getUserTypeReference(void)
{
 if(this->isUserType())
  return(user_types[this->type_idx - (pseudo_end + 1)].ptype);
 else
  return(NULL);
}

unsigned PgSQLType::getUserTypeConfig(void)
{
 if(this->isUserType())
  return(user_types[this->type_idx - (pseudo_end + 1)].type_conf);
 else
  return(0);
}

bool PgSQLType::operator == (unsigned type_id)
{
 return(this->type_idx==type_id);
}

bool PgSQLType::operator == (const QString &type_name)
{
 unsigned idx,total;
 bool found=false;

 total=offset + types_count; //Calculando o total de tipos da classe

 /*Verifica se o tipo passado pelo parametro está no conjunto de
  tipos da classe */
 for(idx=offset; idx<total && !found; idx++)
  found=(type_name==BaseType::type_list[idx]);

 if(found) idx--;

 //Verifica se o código do tipo encontrado é igual ao codigo do tipo atual
 return(type_idx==idx);
}

bool PgSQLType::operator != (const QString &type_name)
{
 return(!((*this)==type_name));
}

bool PgSQLType::operator != (PgSQLType type)
{
 return(this->type_idx!=type.type_idx);
}

bool PgSQLType::operator != (unsigned type_id)
{
 return(this->type_idx!=type_id);
}

bool PgSQLType::operator == (PgSQLType type)
{
 return(this->type_idx==type.type_idx);
}

bool PgSQLType::operator == (void *ptype)
{
 int idx;
 idx=getUserTypeIndex("",ptype);
 return(static_cast<int>(type_idx) == idx);
}

IntervalType PgSQLType::getIntervalType(void)
{
 return(interval_type);
}

SpatialType PgSQLType::getSpatialType(void)
{
 return(spatial_type);
}

bool PgSQLType::isWithTimezone(void)
{
 return(with_timezone);
}

bool PgSQLType::isOIDType(void)
{
 //Retorna se o tipo está no conjunto de tipos identificadores de objetos (oid)
 return(type_idx>=oid_start && type_idx<=oid_end);
}

bool PgSQLType::isPseudoType(void)
{
 //Retorna se o tipo está no conjunto de pseudotipos
 return(type_idx>=pseudo_start && type_idx<=pseudo_end);
}

unsigned PgSQLType::operator << (void *ptype)
{
 setUserType(ptype);
 return(type_idx);
}

void PgSQLType::setIntervalType(IntervalType interv_type)
{
 interval_type=interv_type;
}

void PgSQLType::setSpatialType(SpatialType spat_type)
{
 spatial_type=spat_type;
}

void PgSQLType::setWithTimezone(bool with_tz)
{
 this->with_timezone=with_tz;
}

void PgSQLType::setUserType(unsigned type_id)
{
 unsigned lim1, lim2;

 lim1=pseudo_end + 1;
 lim2=lim1 + PgSQLType::user_types.size();

 if(PgSQLType::user_types.size() > 0 &&
    (type_id >= lim1 && type_id < lim2))
  type_idx=type_id;
 else
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void PgSQLType::setUserType(void *ptype)
{
 int idx;

 idx=getUserTypeIndex("",ptype);
 if(idx <= 0)
  throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  type_idx=idx;
}

void PgSQLType::addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf)
{
 if(type_name!="" && ptype && pmodel &&
    (type_conf==UserTypeConfig::DOMAIN_TYPE ||
     type_conf==UserTypeConfig::SEQUENCE_TYPE ||
     type_conf==UserTypeConfig::TABLE_TYPE ||
     type_conf==UserTypeConfig::BASE_TYPE) &&
    getUserTypeIndex(type_name,ptype,pmodel)==0)
 {
  UserTypeConfig cfg;

  cfg.name=type_name;
  cfg.ptype=ptype;
  cfg.pmodel=pmodel;
  cfg.type_conf=type_conf;
  PgSQLType::user_types.push_back(cfg);
 }
}

void PgSQLType::removeUserType(const QString &type_name, void *ptype)
{
 if(PgSQLType::user_types.size() > 0 &&
    type_name!="" && ptype)
 {
  UserTypeConfig cfg;
  vector<UserTypeConfig>::iterator itr, itr_end;

  itr=PgSQLType::user_types.begin();
  itr_end=PgSQLType::user_types.end();

  while(itr!=itr_end)
  {
   if(itr->name==type_name && itr->ptype==ptype) break;
   else itr++;
  }

  if(itr!=itr_end)
   PgSQLType::user_types.erase(itr);
 }
}

void PgSQLType::renameUserType(const QString &type_name, void *ptype,const QString &new_name)
{
 if(PgSQLType::user_types.size() > 0 &&
    type_name!="" && ptype && type_name!=new_name)
 {
  vector<UserTypeConfig>::iterator itr, itr_end;

  itr=PgSQLType::user_types.begin();
  itr_end=PgSQLType::user_types.end();

  while(itr!=itr_end)
  {
   if(itr->name==type_name && itr->ptype==ptype)
   {
    itr->name=new_name;
    break;
   }
   itr++;
  }
 }
}

unsigned PgSQLType::getBaseTypeIndex(const QString &type_name)
{
 return(getType(type_name,offset,types_count));
}

unsigned PgSQLType::getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel)
{
 if(PgSQLType::user_types.size() > 0 && (type_name!="" || ptype))
 {
  vector<UserTypeConfig>::iterator itr, itr_end;
  int idx=0;

  itr=PgSQLType::user_types.begin();
  itr_end=PgSQLType::user_types.end();

  while(itr!=itr_end)
  {
   if(((type_name!="" && itr->name==type_name) || (ptype && itr->ptype==ptype)) &&
      ((pmodel && itr->pmodel==pmodel) || !pmodel))
    break;

   idx++;
   itr++;
  }

  if(itr!=itr_end)
   //return(offset + qtd_tipos + idx);
   return(pseudo_end + 1 + idx);
  else
   return(BaseType::null);
 }
 else return(BaseType::null);
}

QString PgSQLType::getUserTypeName(unsigned type_id)
{
 unsigned lim1, lim2;

 lim1=pseudo_end + 1;
 lim2=lim1 + PgSQLType::user_types.size();


 if(PgSQLType::user_types.size() > 0 &&
    (type_id >= lim1 && type_id < lim2))
  return(PgSQLType::user_types[type_id - lim1].name);
 else
  return("");
}

void PgSQLType::getUserTypes(QStringList &type_list, void *pmodel, unsigned inc_usr_types)
{
 unsigned idx,total;

 type_list.clear(); //Limpa a lista de tipos
 total=PgSQLType::user_types.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(user_types[idx].pmodel==pmodel &&
     ((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
   type_list.push_back(user_types[idx].name);
 }
}

void PgSQLType::getUserTypes(vector<void *> &ptypes, void *pmodel, unsigned inc_usr_types)
{
 unsigned idx, total;

 ptypes.clear(); //Limpa a lista de tipos
 total=PgSQLType::user_types.size();

 for(idx=0; idx < total; idx++)
 {
  //Só obtem os tipos definidos pelo usuário do modelo especificado
  if(user_types[idx].pmodel==pmodel &&
     ((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
   ptypes.push_back(user_types[idx].ptype);
 }
}

QString PgSQLType::operator ~ (void)
{
 if(type_idx >= pseudo_end + 1)//offset + qtd_tipos)
  return(user_types[type_idx - (pseudo_end + 1)].name);
 else
  return(BaseType::type_list[type_idx]);
}

bool PgSQLType::isArrayType(void)
{
 return(dimension > 0);
}

bool PgSQLType::isUserType(void)
{
 return(type_idx > pseudo_end);
}

bool PgSQLType::hasVariableLength(void )
{
return(type_list[this->type_idx]=="numeric" || type_list[this->type_idx]=="decimal" ||
       type_list[this->type_idx]=="character varying" || type_list[this->type_idx]=="varchar" ||
       type_list[this->type_idx]=="character" || type_list[this->type_idx]=="char" ||
       type_list[this->type_idx]=="bit" || type_list[this->type_idx]=="bit varying" ||
       type_list[this->type_idx]=="varbit");
}

bool PgSQLType::acceptsPrecision(void )
{
 return(type_list[this->type_idx]=="numeric" || type_list[this->type_idx]=="decimal" ||
        type_list[this->type_idx]=="time" || type_list[this->type_idx]=="timestamp" ||
        type_list[this->type_idx]=="interval");
}

void PgSQLType::setDimension(unsigned dim)
{
 if(dim > 0 && this->isUserType())
 {
  int idx=getUserTypeIndex(~(*this), NULL);
  if(user_types[idx].type_conf==UserTypeConfig::DOMAIN_TYPE ||
     user_types[idx].type_conf==UserTypeConfig::SEQUENCE_TYPE)
    throw Exception(ERR_ASG_INV_DOMAIN_ARRAY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 dimension=dim;
}

void PgSQLType::setLength(unsigned len)
{
 //Caso o usuário tente criar um tipo de tamanho zero
 if(len==0)
  //throw Excecao("Atribuição de comprimento igual a zero!");
  throw Exception(ERR_ASG_ZERO_LENGTH,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define o comprimento do tipo da coluna
  this->length=len;
}

void PgSQLType::setPrecision(int prec)
{
 //Caso o usuário tente definir uma precisao maior que o comprimento do tipo
 if(((BaseType::type_list[type_idx]=="numeric" ||
      BaseType::type_list[type_idx]=="decimal") && prec > static_cast<int>(length)))
  throw Exception(ERR_ASG_INV_PRECISION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(((BaseType::type_list[type_idx]=="time" ||
           BaseType::type_list[type_idx]=="timestamp" ||
           BaseType::type_list[type_idx]=="interval") && prec > 6))
  throw Exception(ERR_ASG_INV_PREC_TIMESTAMP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Define a precisão do tipo da coluna
  this->precision=prec;
}

unsigned PgSQLType::getDimension(void)
{
 return(dimension);
}

unsigned PgSQLType::getLength(void)
{
 return(length);
}

int PgSQLType::getPrecision(void)
{
 return(precision);
}

QString PgSQLType::getObjectDefinition(unsigned def_type,QString ref_type)
{
 if(def_type==SchemaParser::SQL_DEFINITION)
  return(*(*this));
 else
 {
  map<QString, QString> attribs;

  attribs[ParsersAttributes::LENGTH]="";
  attribs[ParsersAttributes::DIMENSION]="";
  attribs[ParsersAttributes::PRECISION]="";
  attribs[ParsersAttributes::WITH_TIMEZONE]="";
  attribs[ParsersAttributes::INTERVAL_TYPE]="";
  attribs[ParsersAttributes::SPATIAL_TYPE]="";
  attribs[ParsersAttributes::VARIATION]="";
  attribs[ParsersAttributes::REF_TYPE]=ref_type;

  attribs[ParsersAttributes::NAME]=(~(*this));

  if(length > 1)
   attribs[ParsersAttributes::LENGTH]=QString("%1").arg(this->length);

  if(dimension > 0)
   attribs[ParsersAttributes::DIMENSION]=QString("%1").arg(this->dimension);

  if(precision >= 0)
   attribs[ParsersAttributes::PRECISION]=QString("%1").arg(this->precision);

  if(interval_type != BaseType::null)
   attribs[ParsersAttributes::INTERVAL_TYPE]=(~interval_type);

  if(!isUserType() && spatial_type != BaseType::null)
  {
   attribs[ParsersAttributes::SPATIAL_TYPE]=(~spatial_type);
   attribs[ParsersAttributes::VARIATION]=QString("%1").arg(spatial_type.getVariation());
  }

  if(with_timezone)
   attribs[ParsersAttributes::WITH_TIMEZONE]="1";

  return(SchemaParser::getObjectDefinition("basetype",attribs, def_type));
 }
}

QString PgSQLType::operator * (void)
{
 QString fmt_type, type, aux;
 unsigned idx;

 type=~(*this);

 //Gerando definição de tipos espaciais (PostGiS)
 if(type=="geometry" || type=="geography")
  fmt_type=type + (*spatial_type);
 else if(length > 1 && hasVariableLength())
 {
  /* Tratando o caso de tipos que necessitam de uma precisão.
     A sintaxe desses tipos se altera ficando na forma TIPO(COMPRIMENTO,PRECISÃO).*/
  if((type=="numeric" || type=="decimal") && precision>=0 &&
     precision<=static_cast<int>(length))
   aux=QString("%1(%2,%3)").arg(BaseType::type_list[type_idx]).arg(length).arg(precision);
   /* Trantado o caso dos tipos que necessitam apenas do comprimento */
  else
   aux=QString("%1(%2)").arg(BaseType::type_list[type_idx]).arg(length);

  fmt_type=aux;
 }
 else if(type!="numeric" && type!="decimal" && acceptsPrecision())
 {
  /* Tratando os tipos time e timestampe que possuem o formato:
     [TIME|TIMESTAMP] (PRECISÃO) [WITH|WITHOUT] TIMEZONE */
  if(type!="interval")
  {
   aux=BaseType::type_list[type_idx];

   if(precision >= 0)
    aux+=QString("(%1)").arg(precision);

   if(with_timezone)
    aux+=" with time zone";
  }
  /* Tratando o tipo interval que possuem o formato:
     INTERVAL [TIPO_INTERVALO](PRECISÃO) */
  else
  {
   aux=BaseType::type_list[type_idx];

   if(interval_type!=BaseType::null)
    aux+=QString("[%1]").arg(~interval_type);

   if(precision >= 0)
    aux+=QString("(%1)").arg(precision);
  }

  fmt_type=aux;
 }
 else
  fmt_type=type;


 if(type!="void" && dimension > 0)
 {
  for(idx=0; idx < dimension; idx++)
   fmt_type+="[]";
 }

 return(fmt_type);
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
  enc=(nome_tipo==BaseType::type_list[idx]);

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
  enc=(nome_tipo==BaseType::type_list[idx]);

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

