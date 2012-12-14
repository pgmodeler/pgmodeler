#include "type.h"

unsigned Type::type_id=50000;

Type::Type(void)
{
 object_id=Type::type_id++;
 obj_type=OBJ_TYPE;
 config=ENUMERATION_TYPE;

 alignment="integer";
 delimiter='\0';
 storage=TipoArmazenamento::plain;
 element="any";
 internal_len=0;

 category=TipoCategoria::userdefined;
 preferred=false;
 like_type="any";

 BaseObject::attributes[ParsersAttributes::BASE_TYPE]="";
 BaseObject::attributes[ParsersAttributes::COMPOSITE_TYPE]="";
 BaseObject::attributes[ParsersAttributes::ELEMENTS]="";
 BaseObject::attributes[ParsersAttributes::ENUM_TYPE]="";
 BaseObject::attributes[ParsersAttributes::ENUMARATIONS]="";
 BaseObject::attributes[ParsersAttributes::INPUT_FUNC]="";
 BaseObject::attributes[ParsersAttributes::OUTPUT_FUNC]="";
 BaseObject::attributes[ParsersAttributes::RECV_FUNC]="";
 BaseObject::attributes[ParsersAttributes::SEND_FUNC]="";
 BaseObject::attributes[ParsersAttributes::TPMOD_IN_FUNC]="";
 BaseObject::attributes[ParsersAttributes::TPMOD_OUT_FUNC]="";
 BaseObject::attributes[ParsersAttributes::ANALYZE_FUNC]="";
 BaseObject::attributes[ParsersAttributes::INTERNAL_LENGHT]="";
 BaseObject::attributes[ParsersAttributes::BY_VALUE]="";
 BaseObject::attributes[ParsersAttributes::ALIGNMENT]="";
 BaseObject::attributes[ParsersAttributes::STORAGE]="";
 BaseObject::attributes[ParsersAttributes::DEFAULT_VALUE]="";
 BaseObject::attributes[ParsersAttributes::ELEMENT]="";
 BaseObject::attributes[ParsersAttributes::DELIMITER]="";
 BaseObject::attributes[ParsersAttributes::REDUCED_FORM]="";
 BaseObject::attributes[ParsersAttributes::CATEGORY]="";
 BaseObject::attributes[ParsersAttributes::PREFERRED]="";
 BaseObject::attributes[ParsersAttributes::LIKE_TYPE]="";
}

Type::~Type(void)
{
 /* Ao ser destruído um objeto desta classe tem
    seu nome removido da lista de tipos válidos
    do PostgreSQL */
 TipoPgSQL::removerTipoUsuario(this->getName(true), this);
}

void Type::setName(const QString &name)
{
 QString prev_name;

 prev_name=this->getName(true);//this->nome;
 BaseObject::setName(name);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

void Type::setSchema(BaseObject *schema)
{
 QString prev_name;

 prev_name=this->getName(true);
 BaseObject::setSchema(schema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

bool Type::isAttributeExists(const QString &attrib_name)
{
 vector<Parameter>::iterator itr, itr_end;
 bool found=false;

 itr=attributes.begin();
 itr_end=attributes.end();

 while(itr!=itr_end && !found)
 {
  found=(itr->getName()==attrib_name);
  itr++;
 }

 return(found);
}

void Type::addAttribute(Parameter attrib)
{
 //O atributo não pode ter o nome vazio nem tipo nulo
 if(attrib.getName()=="" || attrib.getType()==TipoPgSQL::nulo)
  throw Exception(ERR_INS_INV_TYPE_ATTRIB,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !attrib.getType())
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se o atributo com mesmo nome já não foi inserido no tipo
 else if(isAttributeExists(attrib.getName()))
  throw Exception(ERR_INS_DUPLIC_ITEMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 attributes.push_back(attrib);
}

void Type::removeAttribute(unsigned attrib_idx)
{
 //Verifica se o índice do atributo é valido
 if(attrib_idx >= attributes.size())
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove atributo no índice especificado
 attributes.erase(attributes.begin() + attrib_idx);
}

void Type::removeAttributes(void)
{
 attributes.clear();
}

bool Type::isEnumerationExists(const QString &enum_name)
{
 vector<QString>::iterator itr, itr_end;
 bool found=false;

 itr=enumerations.begin();
 itr_end=enumerations.end();

 while(itr!=itr_end && !found)
 {
  found=((*itr)==enum_name);
  itr++;
 }

 return(found);
}

void Type::addEnumeration(const QString &enum_name)
{
 //Verifica se a enumeração é vazia
 if(enum_name=="")
  throw Exception(ERR_INS_INV_TYPE_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Verifica se o nome da enumeração é válida de acordo com
    com a regra de nomenclatura de identificadores no PostgreSQL */
 else if(!BaseObject::isValidName(enum_name))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se uma enumeração com mesmo nome já não foi inserido no tipo
 else if(isEnumerationExists(enum_name))
  throw Exception(ERR_INS_DUPLIC_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumerations.push_back(enum_name);
}

void Type::removeEnumeration(unsigned enum_idx)
{
 if(enum_idx >= enumerations.size())
  throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumerations.erase(enumerations.begin() + enum_idx);
}

void Type::removeEnumerations(void)
{
 enumerations.clear();
}

void Type::setConfiguration(unsigned conf)
{
 unsigned idx;

 /* Ao se definir uma configuração de tipo os atributos não
    pertinentes a esta serão limpos. */

 //Verifica se a configuração a ser atribuída ao tipo é válida
 if(conf < BASE_TYPE || conf > COMPOSITE_TYPE)
  throw Exception(ERR_ASG_INV_TYPE_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(conf!=BASE_TYPE)
 {
  if(conf==ENUMERATION_TYPE)
   attributes.clear();
  else
   enumerations.clear();

  for(idx=0; idx < 7; idx++)
   functions[idx]=NULL;

  default_value="";
  element="any";
  delimiter='\0';
  by_value=false;
  internal_len=0;
 }
 else
 {
  attributes.clear();
  enumerations.clear();
 }

 this->config=conf;
}

void Type::setFunction(unsigned func_id, Function *func)
{
 unsigned param_count;
 TipoLinguagem lang;
 lang=TipoLinguagem::c;

 //Verifica se o tipo da função é válido
 if(func_id > ANALYZE_FUNC)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém a quantidade de parâmetros da função
 if(func)
  param_count=func->getParameterCount();

 /* Verifica se a função está alocada quando o tipo de função é INPUT ou OUTPUT,
    pois estas duas são obrigatórias para um tipo base */
 if(!func && (func_id==INPUT_FUNC || func_id==OUTPUT_FUNC))
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->getName(true)))
                         .arg(BaseObject::getTypeName(OBJ_TYPE)),
                ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 else if(func)
 {
  /* Verifica se a função está escrita em C. Para a criação de um tipo base
   apenas funções nesta linguagem podem ser atribuídas */
  if(func->getLanguage()->getName()!=(~lang))
   throw Exception(ERR_ASG_FUNC_INV_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verificando a quantidade de parâmetros da função em relação ao tipo.
     Funções INPUT e RECV devem possuir 1 ou 3 parâmetros, já as demais funções
     devem ter somente 1 parâmetro. */
  else if((param_count!=1 && param_count!=3 &&
          (func_id==INPUT_FUNC || func_id==RECV_FUNC)) ||
          (param_count!=1 &&
          (func_id==OUTPUT_FUNC   || func_id==SEND_FUNC ||
           func_id==TPMOD_IN_FUNC || func_id==TPMOD_OUT_FUNC ||
           func_id==ANALYZE_FUNC)))
   throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                          .arg(QString::fromUtf8(this->getName()))
                          .arg(BaseObject::getTypeName(OBJ_TYPE)),
                 ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verificando os tipos de returno da função em relação ao tipo.
     Funções do tipo INPUT e RECV devem retornar dados do próprio tipo que está sendo
     definido de acordo com a documentação, porém para facilitar a implementação esta função
     precisa retornar dados do tipo 'any' o qual será substituído pelo nome do tipo no momento
     da geração do SQL da função. Funções do tipo OUTPUT e TPMOD_OUT devem retornar cstring.
     As demais funções SEND, TPMOD_IN e ANALYZE devem retornar bytea, integer e boolean,
     respectivamente. */
  else if((func_id==INPUT_FUNC && func->getReturnType()!="any") ||
          (func_id==OUTPUT_FUNC && func->getReturnType()!="cstring") ||
          (func_id==RECV_FUNC && func->getReturnType()!="any") ||
          (func_id==SEND_FUNC && func->getReturnType()!="bytea") ||
          (func_id==TPMOD_IN_FUNC && func->getReturnType()!="integer") ||
          (func_id==TPMOD_OUT_FUNC && func->getReturnType()!="cstring") ||
          (func_id==ANALYZE_FUNC && func->getReturnType()!="boolean"))
   throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
                          .arg(QString::fromUtf8(this->getName()))
                          .arg(BaseObject::getTypeName(OBJ_TYPE)),
                 ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Validando os tipos do parâmetro da função em rao   configuração do tipo.
     A função INPUT deve possuir os parâmetros com tipo (cstring, oid, integer).
     As funções OUTPUT e SEND devem possuir um parâmetro do mesmo tipo sendo definido,
     neste caso, para facilitar a implementação basta usar um parâmetro de tipo 'any'.
     A função RECV deve possuir os parâmetros (internal, oid, integer).
     A função TPMOD_IN deve possuir um parâmetro do tipo (ctring[]).
     A função TPMOD_OUT deve possuir um parâmetro do tipo (integer).
     A função ANALYZE deve possuir um parâmetro do tipo (internal). */
  else if((func_id==INPUT_FUNC &&
          (func->getParameter(0).getType()!="cstring" ||
           (param_count==3 &&
            (func->getParameter(1).getType()!="oid" ||
             func->getParameter(2).getType()!="integer")))) ||
          (func_id==OUTPUT_FUNC && func->getParameter(0).getType()!="any") ||
          (func_id==RECV_FUNC &&
           (func->getParameter(0).getType()!="internal" ||
            (param_count==3 &&
             (func->getParameter(1).getType()!="oid" ||
              func->getParameter(2).getType()!="integer")))) ||
           (func_id==SEND_FUNC && func->getParameter(0).getType()!="any") ||
          (func_id==TPMOD_IN_FUNC && *(func->getParameter(0).getType())!="cstring[]") ||
          (func_id==TPMOD_OUT_FUNC && func->getParameter(0).getType()!="integer") ||
          (func_id==ANALYZE_FUNC && func->getParameter(0).getType()!="internal"))
   throw Exception(ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  func->setProtected(false);
 }

 functions[func_id]=func;
}

void Type::convertFunctionParameters(bool inverse_conv)
{
 unsigned i, conf_funcs[]={ INPUT_FUNC, RECV_FUNC,
                            OUTPUT_FUNC, SEND_FUNC };
 Parameter param;
 Function *func=NULL;

 for(i=0; i < 4; i++)
 {
  func=functions[conf_funcs[i]];

  if(func)
  {
   /* Para as funções OUTPUT e SEND a conversão normal é mudar o tipo do primeiro
      parâmetro de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   if(conf_funcs[i]==OUTPUT_FUNC || conf_funcs[i]==SEND_FUNC)
   {
    param=func->getParameter(0);
    func->removeParameter(0);

    if(!inverse_conv)
    {
     param.setType(TipoPgSQL(this));
     func->addParameter(param);
    }
    else
    {
     param.setType(TipoPgSQL("any"));
     func->addParameter(param);
    }
   }

   /* Para as funções INPUT e RECV a conversão normal é mudar o tipo de retorno
      de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   else if(conf_funcs[i]==INPUT_FUNC || conf_funcs[i]==RECV_FUNC)
   {
    if(!inverse_conv)
     func->setReturnType(TipoPgSQL(this));
    else
     func->setReturnType(TipoPgSQL("any"));
   }
  }
 }
}

void Type::setInternalLength(unsigned length)
{
 internal_len=length;
}

void Type::setByValue(bool value)
{
 by_value=value;
}

void Type::setAlignment(TipoPgSQL type)
{
 QString tp=(*type);

 /* Verifica se o tipo a ser atribuído ao alinhamento é
    diferente de char, smallint, integer e double (os únicos aceitos) */
 if(tp!="char" && tp!="smallint" && tp!="integer" && tp!="double precision")
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ALIGNMENT_TYPE).arg(QString::fromUtf8(this->getName(true))),
                ERR_ASG_INV_ALIGNMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Type::setStorage(TipoArmazenamento strg)
{
 storage=strg;
}

void Type::setDefaultValue(const QString &value)
{
 this->default_value=value;
}

void Type::setElement(TipoPgSQL elem)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !elem)
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(elem!="any" &&
   (elem.tipoOID() || elem.pseudoTipo() ||
    elem.tipoUsuario() || elem.tipoArray()))
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ELEMENT_TYPE).arg(QString::fromUtf8(this->getName(true))),
                ERR_ASG_INV_ELEMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->element=elem;
}

void Type::setDelimiter(char delim)
{
 delimiter=delim;
}

void Type::setElementsAttribute(unsigned def_type)
{
 Parameter param;
 QString str_elem;
 unsigned i, count;

 count=Type::attributes.size();
 for(i=0; i < count; i++)
 {
  param=Type::attributes[i];

  if(def_type==SchemaParser::SQL_DEFINITION)
  {
   str_elem+=param.getName() + " " + (*param.getType());
   if(i < (count-1)) str_elem+=",";
  }
  else
  {
   str_elem+=param.getCodeDefinition(def_type);
  }
 }

 BaseObject::attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

void Type::setEnumerationsAttribute(unsigned def_type)
{
 QString str_enum;
 unsigned i, count;

 count=enumerations.size();
 for(i=0; i < count; i++)
 {
  if(def_type==SchemaParser::SQL_DEFINITION)
   str_enum+="'" + enumerations[i] + "'";
  else
   str_enum+=enumerations[i];

  if(i < (count-1)) str_enum+=",";
 }

 BaseObject::attributes[ParsersAttributes::ENUMARATIONS]=str_enum;
}

void Type::setCategory(TipoCategoria categ)
{
 this->category=categ;
}

void Type::setPreferred(bool value)
{
 this->preferred=value;
}

void Type::setLikeType(TipoPgSQL like_type)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !like_type)
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->like_type=like_type;
}

Parameter Type::getAttribute(unsigned attrib_idx)
{
 if(attrib_idx >= attributes.size())
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(attributes[attrib_idx]);
}

unsigned Type::getAttributeCount(void)
{
 return(attributes.size());
}

QString Type::getEnumeration(unsigned idx_enum)
{
 if(idx_enum >= enumerations.size())
  throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(enumerations[idx_enum]);
}

unsigned Type::getEnumerationCount(void)
{
 return(enumerations.size());
}

Function *Type::getFunction(unsigned func_id)
{
 if(func_id > ANALYZE_FUNC)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(functions[func_id]);
}

unsigned Type::getInternalLength(void)
{
 return(internal_len);
}

bool Type::isByValue(void)
{
 return(by_value);
}

TipoPgSQL Type::getAlignment(void)
{
 return(alignment);
}

TipoArmazenamento Type::getStorage(void)
{
 return(storage);
}

QString Type::getDefaultValue(void)
{
 return(default_value);
}

TipoPgSQL Type::getElement(void)
{
 return(element);
}

char Type::getDelimiter(void)
{
 return(delimiter);
}

unsigned Type::getConfiguration(void)
{
 return(config);
}

TipoCategoria Type::getCategory(void)
{
 return(category);
}

bool Type::isPreferred(void)
{
 return(preferred);
}

TipoPgSQL Type::getLikeType(void)
{
 return(like_type);
}

QString Type::getCodeDefinition(unsigned def_type)
{
 return(this->getCodeDefinition(def_type, false));
}

QString Type::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 if(config==ENUMERATION_TYPE)
 {
  BaseObject::attributes[ParsersAttributes::ENUM_TYPE]="1";
  setEnumerationsAttribute(def_type);
 }
 else if(config==COMPOSITE_TYPE)
 {
  BaseObject::attributes[ParsersAttributes::COMPOSITE_TYPE]="1";
  setElementsAttribute(def_type);
 }
 else
 {
  unsigned i;
  QString func_attrib[7]={ParsersAttributes::INPUT_FUNC,
                          ParsersAttributes::OUTPUT_FUNC,
                          ParsersAttributes::RECV_FUNC,
                          ParsersAttributes::SEND_FUNC,
                          ParsersAttributes::TPMOD_IN_FUNC,
                          ParsersAttributes::TPMOD_OUT_FUNC,
                          ParsersAttributes::ANALYZE_FUNC};

  BaseObject::attributes[ParsersAttributes::BASE_TYPE]="1";

  for(i=0; i < 7; i++)
  {
   if(functions[i])
   {
    if(def_type==SchemaParser::SQL_DEFINITION)
     BaseObject::attributes[func_attrib[i]]=functions[i]->getName();
    else
    {
     functions[i]->setAttribute(ParsersAttributes::REF_TYPE, func_attrib[i]);
     BaseObject::attributes[func_attrib[i]]=functions[i]->getCodeDefinition(def_type, true);
    }
   }
  }

  if(internal_len==0 && def_type==SchemaParser::SQL_DEFINITION)
   BaseObject::attributes[ParsersAttributes::INTERNAL_LENGHT]="VARIABLE";
  else
   BaseObject::attributes[ParsersAttributes::INTERNAL_LENGHT]=QString("%1").arg(internal_len);

  BaseObject::attributes[ParsersAttributes::BY_VALUE]=(by_value ? "1" : "");
  BaseObject::attributes[ParsersAttributes::ALIGNMENT]=(*alignment);
  BaseObject::attributes[ParsersAttributes::STORAGE]=(~storage);
  BaseObject::attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;

  if(element!="any")
   BaseObject::attributes[ParsersAttributes::ELEMENT]=(*element);

  if(delimiter!='\0')
   BaseObject::attributes[ParsersAttributes::DELIMITER]=delimiter;

  BaseObject::attributes[ParsersAttributes::CATEGORY]=~(category);

  BaseObject::attributes[ParsersAttributes::PREFERRED]=(preferred ? "1" : "");

  if(like_type!="any")
   BaseObject::attributes[ParsersAttributes::LIKE_TYPE]=(*like_type);
 }

 return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

void Type::operator = (Type &type)
{
 QString prev_name;
 unsigned i=0;

 prev_name=this->getName(true);
 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(type);

 this->config=type.config;
 this->attributes=type.attributes;
 this->enumerations=type.enumerations;
 this->internal_len=type.internal_len;
 this->by_value=type.by_value;
 this->alignment=type.alignment;
 this->element=type.element;
 this->storage=type.storage;
 this->default_value=type.default_value;
 this->category=type.category;
 this->preferred=type.preferred;
 this->like_type=type.like_type;
 this->delimiter=type.delimiter;

 while(i < 7)
 {
  this->functions[i]=type.functions[i];
  i++;
 }

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

