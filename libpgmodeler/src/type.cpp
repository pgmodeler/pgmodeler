#include "type.h"

unsigned Type::type_id=50000;

Type::Type(void)
{
 object_id=Type::type_id++;
 obj_type=OBJ_TYPE;
 config=TIPO_ENUMERACAO;
 //nome="novo_tipo";
 alinhamento="integer";
 delimitador='\0';
 armazenamento=TipoArmazenamento::plain;
 elemento="any";
 comp_interno=0;

 categoria=TipoCategoria::userdefined;
 preferido=false;
 tipo_copia="any";

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

void Type::setName(const QString &nome)
{
 QString nome_ant;

 nome_ant=this->getName(true);//this->nome;
 BaseObject::setName(nome);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

void Type::setSchema(BaseObject *esquema)
{
 QString nome_ant;

 nome_ant=this->getName(true);
 BaseObject::setSchema(esquema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

bool Type::atributoExiste(const QString &nome_atrib)
{
 vector<Parameter>::iterator itr, itr_end;
 bool enc=false;

 itr=attributes.begin();
 itr_end=attributes.end();

 while(itr!=itr_end && !enc)
 {
  enc=(itr->getName()==nome_atrib);
  itr++;
 }

 return(enc);
}

void Type::adicionarAtributo(Parameter atrib)
{
 //O atributo não pode ter o nome vazio nem tipo nulo
 if(atrib.getName()=="" || atrib.getType()==TipoPgSQL::nulo)
  throw Exception(ERR_INS_INV_TYPE_ATTRIB,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !atrib.getType())
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se o atributo com mesmo nome já não foi inserido no tipo
 else if(atributoExiste(atrib.getName()))
  throw Exception(ERR_INS_DUPLIC_ITEMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 attributes.push_back(atrib);
}

void Type::removerAtributo(unsigned idx_atrib)
{
 //Verifica se o índice do atributo é valido
 if(idx_atrib >= attributes.size())
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove atributo no índice especificado
 attributes.erase(attributes.begin() + idx_atrib);
}

void Type::removerAtributos(void)
{
 attributes.clear();
}

bool Type::enumeracaoExiste(const QString &nome_enum)
{
 vector<QString>::iterator itr, itr_end;
 bool enc=false;

 itr=enumeracoes.begin();
 itr_end=enumeracoes.end();

 while(itr!=itr_end && !enc)
 {
  enc=((*itr)==nome_enum);
  itr++;
 }

 return(enc);
}

void Type::adicionarEnumeracao(const QString &enumer)
{
 //Verifica se a enumeração é vazia
 if(enumer=="")
  throw Exception(ERR_INS_INV_TYPE_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Verifica se o nome da enumeração é válida de acordo com
    com a regra de nomenclatura de identificadores no PostgreSQL */
 else if(!BaseObject::isValidName(enumer))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se uma enumeração com mesmo nome já não foi inserido no tipo
 else if(enumeracaoExiste(enumer))
  throw Exception(ERR_INS_DUPLIC_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumeracoes.push_back(enumer);
}

void Type::removerEnumeracao(unsigned idx_enum)
{
 if(idx_enum >= enumeracoes.size())
  throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumeracoes.erase(enumeracoes.begin() + idx_enum);
}

void Type::removerEnumeracoes(void)
{
 enumeracoes.clear();
}

void Type::definirConfiguracao(unsigned conf)
{
 unsigned idx;

 /* Ao se definir uma configuração de tipo os atributos não
    pertinentes a esta serão limpos. */

 //Verifica se a configuração a ser atribuída ao tipo é válida
 if(conf < TIPO_BASE || conf > TIPO_COMPOSTO)
  throw Exception(ERR_ASG_INV_TYPE_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(conf!=TIPO_BASE)
 {
  if(conf==TIPO_ENUMERACAO)
   attributes.clear();
  else
   enumeracoes.clear();

  for(idx=0; idx < 7; idx++)
   funcoes[idx]=NULL;

  valor_padrao="";
  elemento="any";
  delimitador='\0';
  por_valor=false;
  comp_interno=0;
 }
 else
 {
  attributes.clear();
  enumeracoes.clear();
 }

 this->config=conf;
}

void Type::definirFuncao(unsigned conf_func, Function *funcao)
{
 unsigned qtd_params;
 TipoLinguagem ling;
 ling=TipoLinguagem::c;

 //Verifica se o tipo da função é válido
 if(conf_func > FUNCAO_ANALYZE)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém a quantidade de parâmetros da função
 if(funcao)
  qtd_params=funcao->getParameterCount();

 /* Verifica se a função está alocada quando o tipo de função é INPUT ou OUTPUT,
    pois estas duas são obrigatórias para um tipo base */
 if(!funcao && (conf_func==FUNCAO_INPUT || conf_func==FUNCAO_OUTPUT))
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->getName(true)))
                         .arg(BaseObject::getTypeName(OBJ_TYPE)),
                ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 else if(funcao)
 {
  /* Verifica se a função está escrita em C. Para a criação de um tipo base
   apenas funções nesta linguagem podem ser atribuídas */
  if(funcao->getLanguage()->getName()!=(~ling))
   throw Exception(ERR_ASG_FUNC_INV_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verificando a quantidade de parâmetros da função em relação ao tipo.
     Funções INPUT e RECV devem possuir 1 ou 3 parâmetros, já as demais funções
     devem ter somente 1 parâmetro. */
  else if((qtd_params!=1 && qtd_params!=3 &&
          (conf_func==FUNCAO_INPUT || conf_func==FUNCAO_RECV)) ||
          (qtd_params!=1 &&
          (conf_func==FUNCAO_OUTPUT   || conf_func==FUNCAO_SEND ||
           conf_func==FUNCAO_TPMOD_IN || conf_func==FUNCAO_TPMOD_OUT ||
           conf_func==FUNCAO_ANALYZE)))
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
  else if((conf_func==FUNCAO_INPUT && funcao->getReturnType()!="any") ||
          (conf_func==FUNCAO_OUTPUT && funcao->getReturnType()!="cstring") ||
          (conf_func==FUNCAO_RECV && funcao->getReturnType()!="any") ||
          (conf_func==FUNCAO_SEND && funcao->getReturnType()!="bytea") ||
          (conf_func==FUNCAO_TPMOD_IN && funcao->getReturnType()!="integer") ||
          (conf_func==FUNCAO_TPMOD_OUT && funcao->getReturnType()!="cstring") ||
          (conf_func==FUNCAO_ANALYZE && funcao->getReturnType()!="boolean"))
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
  else if((conf_func==FUNCAO_INPUT &&
          (funcao->getParameter(0).getType()!="cstring" ||
           (qtd_params==3 &&
            (funcao->getParameter(1).getType()!="oid" ||
             funcao->getParameter(2).getType()!="integer")))) ||
          (conf_func==FUNCAO_OUTPUT && funcao->getParameter(0).getType()!="any") ||
          (conf_func==FUNCAO_RECV &&
           (funcao->getParameter(0).getType()!="internal" ||
            (qtd_params==3 &&
             (funcao->getParameter(1).getType()!="oid" ||
              funcao->getParameter(2).getType()!="integer")))) ||
           (conf_func==FUNCAO_SEND && funcao->getParameter(0).getType()!="any") ||
          (conf_func==FUNCAO_TPMOD_IN && *(funcao->getParameter(0).getType())!="cstring[]") ||
          (conf_func==FUNCAO_TPMOD_OUT && funcao->getParameter(0).getType()!="integer") ||
          (conf_func==FUNCAO_ANALYZE && funcao->getParameter(0).getType()!="internal"))
   throw Exception(ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  funcao->setProtected(false);
 }

 funcoes[conf_func]=funcao;
}

void Type::converterParametrosFuncoes(bool conv_inversa)
{
 unsigned i, conf_funcs[]={ FUNCAO_INPUT, FUNCAO_RECV,
                            FUNCAO_OUTPUT, FUNCAO_SEND };
 TipoFuncao conf_func;
 Parameter param;
 Function *funcao=NULL;

 for(i=0; i < 4; i++)
 {
  funcao=funcoes[conf_funcs[i]];

  if(funcao)
  {
   /* Para as funções OUTPUT e SEND a conversão normal é mudar o tipo do primeiro
      parâmetro de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   if(conf_funcs[i]==FUNCAO_OUTPUT || conf_funcs[i]==FUNCAO_SEND)
   {
    param=funcao->getParameter(0);
    funcao->removeParameter(0);

    if(!conv_inversa)
    {
     param.setType(TipoPgSQL(this));
     funcao->addParameter(param);
    }
    else
    {
     param.setType(TipoPgSQL("any"));
     funcao->addParameter(param);
    }
   }

   /* Para as funções INPUT e RECV a conversão normal é mudar o tipo de retorno
      de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   else if(conf_funcs[i]==FUNCAO_INPUT || conf_funcs[i]==FUNCAO_RECV)
   {
    if(!conv_inversa)
     funcao->setReturnType(TipoPgSQL(this));
    else
     funcao->setReturnType(TipoPgSQL("any"));
   }
  }
 }
}

void Type::definirCompInterno(unsigned comp)
{
 comp_interno=comp;
}

void Type::definirPorValor(bool valor)
{
 por_valor=valor;
}

void Type::definirAlinhamento(TipoPgSQL tipo)
{
 QString tp;

 tp=(*tipo);
 /* Verifica se o tipo a ser atribuído ao alinhamento é
    diferente de char, smallint, integer e double (os únicos aceitos) */
 if(tp!="char" && tp!="smallint" && tp!="integer" && tp!="double precision")
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ALIGNMENT_TYPE).arg(QString::fromUtf8(this->getName(true))),
                ERR_ASG_INV_ALIGNMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Type::definirArmazenamento(TipoArmazenamento armaz)
{
 armazenamento=armaz;
}

void Type::definirValorPadrao(const QString &valor_padrao)
{
 this->valor_padrao=valor_padrao;
}

void Type::definirElemento(TipoPgSQL elemento)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !elemento)
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(elemento!="any" &&
   (elemento.tipoOID() || elemento.pseudoTipo() ||
    elemento.tipoUsuario() || elemento.tipoArray()))
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ELEMENT_TYPE).arg(QString::fromUtf8(this->getName(true))),
                ERR_ASG_INV_ELEMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->elemento=elemento;
}

void Type::definirDelimitador(char delim)
{
 delimitador=delim;
}

void Type::definirAtributoElementos(unsigned tipo_def)
{
 Parameter param;
 QString str_elem;
 unsigned i, qtd;

 qtd=Type::attributes.size();
 for(i=0; i < qtd; i++)
 {
  param=Type::attributes[i];

  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   str_elem+=param.getName() + " " + (*param.getType());
   if(i < (qtd-1)) str_elem+=",";
  }
  else
  {
   str_elem+=param.getCodeDefinition(tipo_def);
  }
 }

 BaseObject::attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

void Type::definirAtributoEnumeracoes(unsigned tipo_def)
{
 QString str_enum;
 unsigned i, qtd;

 qtd=enumeracoes.size();
 for(i=0; i < qtd; i++)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   str_enum+="'" + enumeracoes[i] + "'";
  else
   str_enum+=enumeracoes[i];

  if(i < (qtd-1)) str_enum+=",";
 }

 BaseObject::attributes[ParsersAttributes::ENUMARATIONS]=str_enum;
}

void Type::definirCategoria(TipoCategoria categoria)
{
 this->categoria=categoria;
}

void Type::definirPreferido(bool preferido)
{
 this->preferido=preferido;
}

void Type::definirTipoCopia(TipoPgSQL tipo_copia)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->getName(true), this) == !tipo_copia)
  throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(QString::fromUtf8(this->getName(true))),
                ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->tipo_copia=tipo_copia;
}

Parameter Type::obterAtributo(unsigned idx_atrib)
{
 if(idx_atrib >= attributes.size())
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(attributes[idx_atrib]);
}

unsigned Type::obterNumAtributos(void)
{
 return(attributes.size());
}

QString Type::obterEnumeracao(unsigned idx_enum)
{
 if(idx_enum >= enumeracoes.size())
  throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(enumeracoes[idx_enum]);
}

unsigned Type::obterNumEnumeracoes(void)
{
 return(enumeracoes.size());
}

Function *Type::obterFuncao(unsigned conf_func)
{
 if(conf_func > FUNCAO_ANALYZE)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(funcoes[conf_func]);
}

unsigned Type::obterCompInterno(void)
{
 return(comp_interno);
}

bool Type::passadoPorValor(void)
{
 return(por_valor);
}

TipoPgSQL Type::obterAlinhamento(void)
{
 return(alinhamento);
}

TipoArmazenamento Type::obterArmazenamento(void)
{
 return(armazenamento);
}

QString Type::obterValorPadrao(void)
{
 return(valor_padrao);
}

TipoPgSQL Type::obterElemento(void)
{
 return(elemento);
}

char Type::obterDelimitador(void)
{
 return(delimitador);
}

unsigned Type::obterConfiguracao(void)
{
 return(config);
}

TipoCategoria Type::obterCategoria(void)
{
 return(categoria);
}

bool Type::tipoPreferido(void)
{
 return(preferido);
}

TipoPgSQL Type::obterTipoCopia(void)
{
 return(tipo_copia);
}

QString Type::getCodeDefinition(unsigned tipo_def)
{
 return(this->getCodeDefinition(tipo_def, false));
}

QString Type::getCodeDefinition(unsigned tipo_def, bool forma_reduzida)
{
 if(config==TIPO_ENUMERACAO)
 {
  BaseObject::attributes[ParsersAttributes::ENUM_TYPE]="1";
  definirAtributoEnumeracoes(tipo_def);
 }
 else if(config==TIPO_COMPOSTO)
 {
  BaseObject::attributes[ParsersAttributes::COMPOSITE_TYPE]="1";
  definirAtributoElementos(tipo_def);
 }
 else
 {
  unsigned i;
  QString atrib_func[7]={ParsersAttributes::INPUT_FUNC,
                         ParsersAttributes::OUTPUT_FUNC,
                         ParsersAttributes::RECV_FUNC,
                         ParsersAttributes::SEND_FUNC,
                         ParsersAttributes::TPMOD_IN_FUNC,
                         ParsersAttributes::TPMOD_OUT_FUNC,
                         ParsersAttributes::ANALYZE_FUNC};

  BaseObject::attributes[ParsersAttributes::BASE_TYPE]="1";

  for(i=0; i < 7; i++)
  {
   if(funcoes[i])
   {
    if(tipo_def==SchemaParser::SQL_DEFINITION)
     BaseObject::attributes[atrib_func[i]]=funcoes[i]->getName();
    else
    {
     funcoes[i]->setAttribute(ParsersAttributes::REF_TYPE, atrib_func[i]);
     BaseObject::attributes[atrib_func[i]]=funcoes[i]->getCodeDefinition(tipo_def, true);
    }
   }
  }

  if(comp_interno==0 && tipo_def==SchemaParser::SQL_DEFINITION)
   BaseObject::attributes[ParsersAttributes::INTERNAL_LENGHT]="VARIABLE";
  else
   BaseObject::attributes[ParsersAttributes::INTERNAL_LENGHT]=QString("%1").arg(comp_interno);

  BaseObject::attributes[ParsersAttributes::BY_VALUE]=(por_valor ? "1" : "");
  BaseObject::attributes[ParsersAttributes::ALIGNMENT]=(*alinhamento);
  BaseObject::attributes[ParsersAttributes::STORAGE]=(~armazenamento);
  BaseObject::attributes[ParsersAttributes::DEFAULT_VALUE]=valor_padrao;

  if(elemento!="any")
   BaseObject::attributes[ParsersAttributes::ELEMENT]=(*elemento);

  if(delimitador!='\0')
   BaseObject::attributes[ParsersAttributes::DELIMITER]=delimitador;

  BaseObject::attributes[ParsersAttributes::CATEGORY]=~(categoria);

  BaseObject::attributes[ParsersAttributes::PREFERRED]=(preferido ? "1" : "");

  if(tipo_copia!="any")
   BaseObject::attributes[ParsersAttributes::LIKE_TYPE]=(*tipo_copia);
 }

 return(BaseObject::getCodeDefinition(tipo_def, forma_reduzida));
}

void Type::operator = (Type &tipo)
{
 QString nome_ant;
 unsigned i=0;

 nome_ant=this->getName(true);
 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(tipo);

 this->config=tipo.config;
 this->attributes=tipo.attributes;
 this->enumeracoes=tipo.enumeracoes;
 this->comp_interno=tipo.comp_interno;
 this->por_valor=tipo.por_valor;
 this->alinhamento=tipo.alinhamento;
 this->elemento=tipo.elemento;
 this->armazenamento=tipo.armazenamento;
 this->valor_padrao=tipo.valor_padrao;
 this->categoria=tipo.categoria;
 this->preferido=tipo.preferido;
 this->tipo_copia=tipo.tipo_copia;
 this->delimitador=tipo.delimitador;

 while(i < 7)
 {
  this->funcoes[i]=tipo.funcoes[i];
  i++;
 }

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

