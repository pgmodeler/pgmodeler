#include "modelobd.h"

unsigned ModeloBD::dbmodel_id=20000;

ModeloBD::ModeloBD(void)
{
 object_id=ModeloBD::dbmodel_id++;

 obj_type=OBJ_DATABASE;
 BaseObject::setName(QObject::trUtf8("new_database").toUtf8());

 lim_conexao=-1;
 carregando_modelo=false;
 attributes[ParsersAttributes::ENCODING]="";
 attributes[ParsersAttributes::TEMPLATE_DB]="";
 attributes[ParsersAttributes::CONN_LIMIT]="";
 attributes[ParsersAttributes::LC_COLLATE_DB]="";
 attributes[ParsersAttributes::LC_CTYPE_DB]="";
}

ModeloBD::~ModeloBD(void)
{
 this->blockSignals(true);
 destruirObjetos();
}

void ModeloBD::definirCodificacao(TipoCodificacao tipo_cod)
{
 tipo_codif=tipo_cod;
}

void ModeloBD::definirLocalizacao(int cod_local, const QString &valor)
{
 unsigned idx=0;

 switch(cod_local)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 localizacoes[idx]=valor;
}

void ModeloBD::definirLimiteConexao(int lim_conexao)
{
 if(lim_conexao < -1) lim_conexao=-1;
 this->lim_conexao=lim_conexao;
}

void ModeloBD::definirBDModelo(const QString &bd_modelo)
{
 if(!bd_modelo.isEmpty() && !BaseObject::isValidName(bd_modelo))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->bd_modelo=bd_modelo;
}

void ModeloBD::definirAutor(const QString &autor)
{
 this->autor=autor;
}

vector<BaseObject *> *ModeloBD::obterListaObjetos(ObjectType tipo_obj)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(tipo_obj==OBJ_TEXTBOX)
  return(&caixas);
 else if(tipo_obj==OBJ_TABLE)
  return(&tabelas);
 else if(tipo_obj==OBJ_FUNCTION)
  return(&funcoes);
 else if(tipo_obj==OBJ_AGGREGATE)
  return(&func_agregadas);
 else if(tipo_obj==OBJ_SCHEMA)
  return(&esquemas);
 else if(tipo_obj==OBJ_VIEW)
  return(&visoes);
 else if(tipo_obj==OBJ_TYPE)
  return(&tipos);
 else if(tipo_obj==OBJ_ROLE)
  return(&papeis);
 else if(tipo_obj==OBJ_TABLESPACE)
  return(&espacos_tabela);
 else if(tipo_obj==OBJ_LANGUAGE)
  return(&linguagens);
 else if(tipo_obj==OBJ_CAST)
  return(&conv_tipos);
 else if(tipo_obj==OBJ_CONVERSION)
  return(&conv_codificacao);
 else if(tipo_obj==OBJ_OPERATOR)
  return(&operadores);
 else if(tipo_obj==OBJ_OPCLASS)
  return(&classe_ops);
 else if(tipo_obj==OBJ_OPFAMILY)
  return(&familia_ops);
 else if(tipo_obj==OBJ_DOMAIN)
  return(&dominios);
 else if(tipo_obj==OBJ_SEQUENCE)
  return(&sequencias);
 else if(tipo_obj==BASE_RELATIONSHIP)
  return(&relac_visoes);
 else if(tipo_obj==OBJ_RELATIONSHIP)
  return(&relacionamentos);
 else if(tipo_obj==OBJ_PERMISSION)
  return(&permissoes);
 else
  return(NULL);
}

QString ModeloBD::validarDefinicaoObjeto(BaseObject *objeto, unsigned tipo_def)
{
 ObjectType tipo_obj;
 QString def_obj;

 if(objeto)
 {
  /* Verifica se a definição SQL/XML do objeto é valida por intermédio
     do parser de esquemas, o qual retorna um erro quando existe
     algum problema com a definição sql do objeto */
  try
  {
   tipo_obj=objeto->getObjectType();

   if(tipo_obj==BASE_RELATIONSHIP && tipo_def==SchemaParser::XML_DEFINITION)
    def_obj=dynamic_cast<RelacionamentoBase *>(objeto)->getCodeDefinition();
   else if(tipo_obj==OBJ_TEXTBOX && tipo_def==SchemaParser::XML_DEFINITION)
    def_obj=dynamic_cast<Textbox *>(objeto)->getCodeDefinition();
   else
    def_obj=objeto->getCodeDefinition(tipo_def);
  }
  catch(Exception &e)
  {
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
    throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                           .arg(QString::fromUtf8(objeto->getName(true)))
                           .arg(objeto->getTypeName()),
                  ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   else
    //Caso o parser dispare os demais erros, apenas redireciona o erro
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }

 return(def_obj);
}

void ModeloBD::adicionarObjeto(BaseObject *objeto, int idx_obj)
{
 ObjectType tipo_obj;

 if(objeto)
 {
  try
  {
   tipo_obj=objeto->getObjectType();

   if(tipo_obj==OBJ_RELATIONSHIP ||
      tipo_obj==BASE_RELATIONSHIP)
    adicionarRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TEXTBOX)
    adicionarCaixaTexto(dynamic_cast<Textbox *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TABLE)
    adicionarTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_FUNCTION)
    adicionarFuncao(dynamic_cast<Function *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_AGGREGATE)
    adicionarFuncaoAgregacao(dynamic_cast<Aggregate *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_SCHEMA)
    adicionarEsquema(dynamic_cast<Schema *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_VIEW)
    adicionarVisao(dynamic_cast<Visao *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TYPE)
    adicionarTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_ROLE)
    adicionarPapel(dynamic_cast<Papel *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TABLESPACE)
    adicionarEspacoTabela(dynamic_cast<Tablespace *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_LANGUAGE)
    adicionarLinguagem(dynamic_cast<Language *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_CAST)
    adicionarConversaoTipo(dynamic_cast<Cast *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_CONVERSION)
    adicionarConversaoCodificacao(dynamic_cast<Conversion *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPERATOR)
    adicionarOperador(dynamic_cast<Operator *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPCLASS)
    adicionarClasseOperadores(dynamic_cast<OperatorClass *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPFAMILY)
    adicionarFamiliaOperadores(dynamic_cast<OperatorFamily *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_DOMAIN)
    adicionarDominio(dynamic_cast<Domain *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_SEQUENCE)
    adicionarSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_PERMISSION)
    adicionarPermissao(dynamic_cast<Permissao *>(objeto));
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void ModeloBD::removerObjeto(BaseObject *objeto, int idx_obj)
{
 ObjectType tipo_obj;


 if(objeto)
 {
  try
  {
   tipo_obj=objeto->getObjectType();

   if(tipo_obj==OBJ_RELATIONSHIP ||
      tipo_obj==BASE_RELATIONSHIP)
    removerRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TEXTBOX)
    removerCaixaTexto(dynamic_cast<Textbox *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TABLE)
    removerTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_FUNCTION)
    removerFuncao(dynamic_cast<Function *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_AGGREGATE)
    removerFuncaoAgregacao(dynamic_cast<Aggregate *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_SCHEMA)
    removerEsquema(dynamic_cast<Schema *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_VIEW)
    removerVisao(dynamic_cast<Visao *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TYPE)
    removerTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_ROLE)
    removerPapel(dynamic_cast<Papel *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_TABLESPACE)
    removerEspacoTabela(dynamic_cast<Tablespace *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_LANGUAGE)
    removerLinguagem(dynamic_cast<Language *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_CAST)
    removerConversaoTipo(dynamic_cast<Cast *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_CONVERSION)
    removerConversaoCodificacao(dynamic_cast<Conversion *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPERATOR)
    removerOperador(dynamic_cast<Operator *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPCLASS)
    removerClasseOperadores(dynamic_cast<OperatorClass *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_OPFAMILY)
    removerFamiliaOperadores(dynamic_cast<OperatorFamily *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_DOMAIN)
    removerDominio(dynamic_cast<Domain *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_SEQUENCE)
    removerSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
   else if(tipo_obj==OBJ_PERMISSION)
    removerPermissao(dynamic_cast<Permissao *>(objeto));
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void ModeloBD::removerObjeto(unsigned idx_obj, ObjectType tipo_obj)
{
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
    tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_INDEX ||
    tipo_obj==OBJ_RULE ||
    tipo_obj==BASE_OBJECT || tipo_obj==BASE_RELATIONSHIP ||
    tipo_obj==OBJ_DATABASE)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else
 {
  vector<BaseObject *> *lista_obj=NULL;
  BaseObject *objeto=NULL;

  lista_obj=obterListaObjetos(tipo_obj);
  if(idx_obj >= lista_obj->size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  objeto=(*lista_obj)[idx_obj];
  if(tipo_obj==OBJ_TEXTBOX)
   removerCaixaTexto(dynamic_cast<Textbox *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_TABLE)
   removerTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_FUNCTION)
   removerFuncao(dynamic_cast<Function *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_AGGREGATE)
   removerFuncaoAgregacao(dynamic_cast<Aggregate *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_SCHEMA)
   removerEsquema(dynamic_cast<Schema *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_VIEW)
   removerVisao(dynamic_cast<Visao *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_TYPE)
   removerTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
  else if(tipo_obj==OBJ_ROLE)
   removerPapel(dynamic_cast<Papel *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_TABLESPACE)
   removerEspacoTabela(dynamic_cast<Tablespace *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_LANGUAGE)
   removerLinguagem(dynamic_cast<Language *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_CAST)
   removerConversaoTipo(dynamic_cast<Cast *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_CONVERSION)
   removerConversaoCodificacao(dynamic_cast<Conversion *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_OPERATOR)
  removerOperador(dynamic_cast<Operator *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_OPCLASS)
  removerClasseOperadores(dynamic_cast<OperatorClass *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_OPFAMILY)
  removerFamiliaOperadores(dynamic_cast<OperatorFamily *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_DOMAIN)
  removerDominio(dynamic_cast<Domain *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_SEQUENCE)
  removerSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_RELATIONSHIP || tipo_obj==BASE_RELATIONSHIP)
  removerRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
 else if(tipo_obj==OBJ_PERMISSION)
  removerPermissao(dynamic_cast<Permissao *>(objeto));
 }
}

void ModeloBD::__adicionarObjeto(BaseObject *objeto, int idx_obj)
{
 int idx;
 ObjectType tipo_obj;
 vector<BaseObject *> *lista_obj=NULL;
 vector<BaseObject *>::iterator itr, itr_end;

 if(!objeto)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);


 tipo_obj=objeto->getObjectType();

 /* Caso o objeto seja um espaço de tabela verifica se não existem
    outro espaço de tabela que esteja apontando para o mesmo diretório
    o que é considerado erro */
 if(tipo_obj==OBJ_TABLESPACE)
 {
  Tablespace *esp_tab=NULL, *esp_tab_aux=NULL;

  lista_obj=obterListaObjetos(tipo_obj);
  itr=lista_obj->begin();
  itr_end=lista_obj->end();
  esp_tab=dynamic_cast<Tablespace *>(objeto);

  while(itr!=itr_end)
  {
   esp_tab_aux=dynamic_cast<Tablespace *>(*itr);

   //Caso o diretório dos mesmos sejam iguais um erro é disparado
   if(esp_tab->getDirectory()==esp_tab_aux->getDirectory())
   {
    throw Exception(Exception::getErrorMessage(ERR_ASG_DUP_TABLESPACE_DIR)
                         .arg(QString::fromUtf8(esp_tab->getName()))
                         .arg(esp_tab_aux->getName()),
                  ERR_ASG_DUP_TABLESPACE_DIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   itr++;
  }
 }

 //Verifica se o objeto a ser inserido já existe no modelo, buscando através do nome.
 if((tipo_obj!=OBJ_FUNCTION && obterObjeto(objeto->getName(true), tipo_obj, idx)) ||
    (tipo_obj==OBJ_FUNCTION &&
     obterObjeto(dynamic_cast<Function *>(objeto)->getSignature(), tipo_obj, idx)))
 {
  QString str_aux;

  str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
          .arg(objeto->getName(true))
          .arg(objeto->getTypeName())
          .arg(this->getName(true))
          .arg(this->getTypeName());

  throw Exception(str_aux,ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 try
 {
  //Valida a definição sql do objeto
  ModeloBD::validarDefinicaoObjeto(objeto, SchemaParser::SQL_DEFINITION);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }

 //Obtém a lista de objetos do tipo do novo objeto
 lista_obj=obterListaObjetos(objeto->getObjectType());

 //Insere o novo elemento na lista
 if(idx_obj < 0 || idx_obj >= static_cast<int>(lista_obj->size()))
  lista_obj->push_back(objeto);
 else
 {
  if(idx_obj >=0 && idx < 0)
   idx=idx_obj;
  else if(idx_obj < 0 && idx < 0)
   idx=0;

  if(lista_obj->size() > 0)
   lista_obj->insert((lista_obj->begin() + idx), objeto);
  else
   lista_obj->push_back(objeto);
 }

 //Emite um sinal indicando a adição do objeto no modelo
 if(!signalsBlocked())
  emit s_objetoAdicionado(objeto);
}

void ModeloBD::__removerObjeto(BaseObject *objeto, int idx_obj)
{
 if(!objeto)
  throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<BaseObject *> *lista_obj=NULL;
  ObjectType tipo_obj;

  tipo_obj=objeto->getObjectType();

  //Obtém a lista de acordo com o tipo do objeto
  lista_obj=obterListaObjetos(tipo_obj);
  if(!lista_obj)
   throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Caso o índice do objeto não foi especificado o método tenta descobrí-lo
   if(idx_obj < 0)
   {
    if(tipo_obj!=OBJ_FUNCTION && tipo_obj!=OBJ_OPERATOR)
     obterObjeto(objeto->getName(true), tipo_obj, idx_obj);
    else if(tipo_obj==OBJ_FUNCTION)
     obterObjeto(dynamic_cast<Function *>(objeto)->getSignature(), tipo_obj, idx_obj);
    else
     obterObjeto(dynamic_cast<Operator *>(objeto)->obterAssinatura(), tipo_obj, idx_obj);
   }

   if(idx_obj >= 0)
   {
    //Remove as permissões do objeto
    removerPermissoes(objeto);
    lista_obj->erase(lista_obj->begin() + idx_obj);
   }
  }

  //Emite um sinal indicando que o objeto foi removido
  if(!signalsBlocked())
   emit s_objetoRemovido(objeto);
 }
}

vector<BaseObject *> ModeloBD::obterObjetos(ObjectType tipo_obj, BaseObject *esquema)
{
 vector<BaseObject *> *lista_obj=NULL, lista_sel;
 vector<BaseObject *>::iterator itr, itr_end;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista_obj->begin();
 itr_end=lista_obj->end();

 while(itr!=itr_end)
 {
  if((*itr)->getSchema()==esquema)
   lista_sel.push_back(*itr);
  itr++;
 }

 return(lista_sel);
}

BaseObject *ModeloBD::obterObjeto(const QString &nome, ObjectType tipo_obj, int &idx_obj)
{
 BaseObject *objeto=NULL;
 vector<BaseObject *> *lista_obj=NULL;
 vector<BaseObject *>::iterator itr, itr_end;
 bool enc=false;
 int qtd;
 QString nome_aux, nome_aux1;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Obtém as referências para o inicio e o fim da lista
  itr=lista_obj->begin();
  itr_end=lista_obj->end();
  idx_obj=-1;

  nome_aux1=nome;
  qtd=nome_aux1.count(QChar('\0'));
  if(qtd >=1) nome_aux1.chop(qtd);

  if(tipo_obj!=OBJ_FUNCTION && tipo_obj!=OBJ_OPERATOR)
  {
   nome_aux1=BaseObject::formatName(nome_aux1);

   while(itr!=itr_end && !enc)
   {
    /* Caso o nome do objeto atual seja igual ao nome passado
      o while será quebrado */
    nome_aux=(*itr)->getName(true);
    enc=(nome_aux==nome_aux1);
    if(!enc) itr++;
   }
  }
  else
  {
   QString assinatura;

   while(itr!=itr_end && !enc)
   {
    /* Caso especial para função/oeradores: para saber se uma função/operador está duplicada
       deve-se verificar a assinatura da mesma e não apenas o nome, pois
       o PostgreSQL permite a sobrecarga de funções e operadores. Neste caso especial
       o parâmetro informado ao método deve ser uma assinatura de função/operador
       e não seu nome */
    if(tipo_obj==OBJ_FUNCTION)
     assinatura=dynamic_cast<Function *>(*itr)->getSignature();
    else
     assinatura=dynamic_cast<Operator *>(*itr)->obterAssinatura();

    enc=(assinatura==nome_aux1);
    if(!enc) itr++;
   }
  }

  if(enc)
  {
   //Obtém a referência do objeto e calcula seu índice
   objeto=(*itr);
   idx_obj=(itr-lista_obj->begin());
  }
  else idx_obj=-1;
 }

 return(objeto);
}

BaseObject *ModeloBD::obterObjeto(unsigned idx_obj, ObjectType tipo_obj)
{
 vector<BaseObject *> *lista_obj=NULL;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o índice do objeto a ser obtido esteja fora do
    intervalo de elementos da lista */
 else if(idx_obj >= lista_obj->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(lista_obj->at(idx_obj));
}

unsigned ModeloBD::obterNumObjetos(ObjectType tipo_obj)
{
 vector<BaseObject *> *lista_obj=NULL;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(lista_obj->size());
}

unsigned ModeloBD::obterNumObjetos(void)
{
 ObjectType tipos[20]={
          BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
          OBJ_AGGREGATE, OBJ_OPERATOR,
          OBJ_SEQUENCE, OBJ_CONVERSION,
          OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
          BASE_RELATIONSHIP, OBJ_TEXTBOX,
          OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
          OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
 unsigned qtd=0, i;

 for(i=0; i < 20; i++)
  qtd+=obterListaObjetos(tipos[i])->size();

 return(qtd);
}

QString ModeloBD::obterLocalizacao(int cod_local)
{
 unsigned idx=0;

 switch(cod_local)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 return(localizacoes[idx]);
}

int ModeloBD::obterLimiteConexao(void)
{
 return(lim_conexao);
}

QString ModeloBD::obterBDModelo(void)
{
 return(bd_modelo);
}

TipoCodificacao ModeloBD::obterCodificacao(void)
{
 return(tipo_codif);
}

QString ModeloBD::obterAutor(void)
{
 return(autor);
}

void ModeloBD::setProtected(bool protegido)
{
 ObjectType tipos[19]={
          OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
          OBJ_AGGREGATE, OBJ_OPERATOR,
          OBJ_SEQUENCE, OBJ_CONVERSION,
          OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
          BASE_RELATIONSHIP, OBJ_TEXTBOX,
          OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
          OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
 vector<BaseObject *> *lista=NULL;
 vector<BaseObject *>::iterator itr, itr_end;
 BaseObject *objeto=NULL;
 unsigned i;

 for(i=0; i < 19; i++)
 {
  lista=obterListaObjetos(tipos[i]);
  itr=lista->begin();
  itr_end=lista->end();

  while(itr!=itr_end)
  {
   objeto=(*itr);
   objeto->setProtected(protegido);
   itr++;
  }
 }

 BaseObject::setProtected(protegido);
}

void ModeloBD::destruirObjetos(void)
{
 /* A ordem de destruição de objetos deve ser seguida de forma que
    os objetos menos dependidos sejam destruídos primeiro para
    evitar falha de segmentação onde, no momento da destruição de algum
    objeto, um dos objetos mais dependendidos sejam referenciados porém
    ja foram destruídos anteriormente. */
 ObjectType tipos[20]={
          BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
          OBJ_AGGREGATE, OBJ_OPERATOR,
          OBJ_SEQUENCE, OBJ_CONVERSION,
          OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
          BASE_RELATIONSHIP, OBJ_TEXTBOX,
          OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
          OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE };
 vector<BaseObject *> *lista=NULL;
 BaseObject *objeto=NULL;
 unsigned i;

 desconectarRelacionamentos();

 for(i=0; i < 20; i++)
 {
  lista=obterListaObjetos(tipos[i]);

  while(!lista->empty())
  {
   objeto=lista->back();

   if(objeto->getObjectType()==OBJ_RELATIONSHIP)
    dynamic_cast<Relacionamento *>(objeto)->destruirObjetos();

   delete(objeto);
   lista->pop_back();
  }
 }
}

void ModeloBD::adicionarTabela(Tabela *tabela, int idx_obj)
{
 try
 {
  __adicionarObjeto(tabela, idx_obj);
  /* Ao ser inserido uma nova tabela a mesma tem
   seu nome é adicionad  lista de tipos válidos
   do PostgreSQL */
  TipoPgSQL::adicionarTipoUsuario(tabela->getName(true), tabela, this, ConfigTipoUsuario::TIPO_TABELA);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tabela *ModeloBD::obterTabela(unsigned idx_obj)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_obj, OBJ_TABLE)));
}

void ModeloBD::removerTabela(Tabela *tabela, int idx_obj)
{
 if(tabela)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referênca   tabela
  obterReferenciasObjeto(tabela, vet_refs, true);

  //Caso a tabela esteja sendo referenciada, a mesma não pode ser removida
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
       do objeto referenciado */
    if(!dynamic_cast<TableObject *>(vet_refs[0]))
    {
     tipo_err=ERR_REM_DIRECT_REFERENCE;
     str_aux=QString(Exception::getErrorMessage(tipo_err))
             .arg(tabela->getName(true))
             .arg(tabela->getTypeName())
             .arg(vet_refs[0]->getName(true))
             .arg(vet_refs[0]->getTypeName());
    }
    else
    {
     BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();

     //Formata a mensagem caso exista uma referência indireta ao objeto a ser removido
     tipo_err=ERR_REM_INDIRECT_REFERENCE;
     str_aux=QString(Exception::getErrorMessage(tipo_err))
             .arg(tabela->getName(true))
             .arg(tabela->getTypeName())
             .arg(vet_refs[0]->getName(true))
             .arg(vet_refs[0]->getTypeName())
             .arg(obj_ref_pai->getName(true))
             .arg(obj_ref_pai->getTypeName());
    }

    throw Exception(str_aux, tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

  __removerObjeto(tabela, idx_obj);

  /* Ao ser removido do modelo a sequencia tem
   seu nome removido da lista de tipos válidos do PostgreSQL */
 TipoPgSQL::removerTipoUsuario(tabela->getName(true), tabela);
 }
}

void ModeloBD::adicionarSequencia(Sequencia *sequencia, int idx_obj)
{
 try
 {
  __adicionarObjeto(sequencia, idx_obj);
  /* Ao ser inserido uma nova sequencia a mesma tem
   seu nome é adicionad  lista de tipos válidos
   do PostgreSQL */
  TipoPgSQL::adicionarTipoUsuario(sequencia->getName(true), sequencia, this, ConfigTipoUsuario::TIPO_SEQUENCIA);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Sequencia *ModeloBD::obterSequencia(unsigned idx_obj)
{
 return(dynamic_cast<Sequencia *>(obterObjeto(idx_obj, OBJ_SEQUENCE)));
}

void ModeloBD::removerSequencia(Sequencia *sequencia, int idx_obj)
{
 if(sequencia)
 {
  removerTipoUsuario(sequencia, idx_obj);
  __removerObjeto(sequencia, idx_obj);
  /* Ao ser removido do modelo a sequencia tem
   seu nome removido da lista de tipos válidos do PostgreSQL */
  //TipoPgSQL::removerTipoUsuario(sequencia->getName(true), sequencia);
 }
}

void ModeloBD::adicionarVisao(Visao *visao, int idx_obj)
{
 if(visao)
 {
  try
  {
   __adicionarObjeto(visao, idx_obj);
   atualizarRelTabelaVisao(visao);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

Visao *ModeloBD::obterVisao(unsigned idx_obj)
{
 return(dynamic_cast<Visao *>(obterObjeto(idx_obj, OBJ_VIEW)));
}

void ModeloBD::removerVisao(Visao *visao, int idx_obj)
{
 if(visao)
 {
  __removerObjeto(visao, idx_obj);
  atualizarRelTabelaVisao(visao);
 }
}

void ModeloBD::atualizarRelTabelaVisao(Visao *visao)
{
 Tabela *tab=NULL;
 RelacionamentoBase *rel=NULL;
 Referencia ref;
 unsigned i, qtd_ref, idx;
 vector<BaseObject *>::iterator itr, itr_end;

 if(!visao)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(obterIndiceObjeto(visao) < 0)
 {
  /* Quando uma visão é excluída, os relacionamentos tabela-visão os quais
     possuem a visão como um dos elementos serão excluídos automaticamente */
  itr=relac_visoes.begin();
  itr_end=relac_visoes.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==visao ||
      rel->obterTabela(RelacionamentoBase::TABELA_DESTINO)==visao)
   {
    //Remove o relacionamento
    removerRelacionamento(rel);
    itr=relac_visoes.begin() + idx;
    itr_end=relac_visoes.end();
   }
   else
   {
    itr++; idx++;
   }
  }
 }
 else
 {
    /* Remove os relacionamentos visão-tabela os quais estão inválidos, ou seja,
     a tabela do relacionametno não está sendo mais referenciada pela visao */
  itr=relac_visoes.begin();
  itr_end=relac_visoes.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==visao ||
      rel->obterTabela(RelacionamentoBase::TABELA_DESTINO)==visao)
   {
    //Obtém a tabela do relacionamento
    if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)->getObjectType()==OBJ_TABLE)
     tab=dynamic_cast<Tabela *>(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM));
    else
     tab=dynamic_cast<Tabela *>(rel->obterTabela(RelacionamentoBase::TABELA_DESTINO));

    //Caso a visão não referencie mais a tabela
    if(!visao->referenciaTabela(tab))
    {
     //Remove o relacionamento
     removerRelacionamento(rel);
     itr=relac_visoes.begin() + idx;
     itr_end=relac_visoes.end();
    }
    else
    {
     itr++; idx++;
    }
   }
   else
   {
    itr++; idx++;
   }
  }

  /* Cria automaticamente os relacionamentos entre as tabelas e a visão.
     As tabelas referenciadas são obtidas das referências na parte SELECT
     da consulta que gera a visão  */
  qtd_ref=visao->obterNumReferencias(Referencia::SQL_REFER_SELECT);

  for(i=0; i < qtd_ref; i++)
  {
   ref=visao->obterReferencia(i, Referencia::SQL_REFER_SELECT);
   tab=ref.obterTabela();

   /* Caso a tabela exista, um relacionamento tabela-visão será automaticamente criado
      (caso este já não existe) e inserido no modelo */
   rel=obterRelacionamento(visao,tab);
   if(tab && !rel)
   {
    /* rel=new RelacionamentoBase("rel_" + visao->getName() + "_" + tab->getName(),
                               RelacionamentoBase::RELACIONAMENTO_DEP,visao,tab,false,false); */
    rel=new RelacionamentoBase(RelacionamentoBase::RELACIONAMENTO_DEP,visao,tab,false,false);
    adicionarRelacionamento(rel);
   }
  }
 }
}

void ModeloBD::desconectarRelacionamentos(void)
{
 try
 {
  RelacionamentoBase *rel_base=NULL;
  Relacionamento *rel=NULL;
  vector<BaseObject *>::reverse_iterator ritr_rel, ritr_rel_end;

  /* Varre a lista geral de relacionamentos
     do último ao primeiro, desconectando os últimos relacionamentos primeiro
     para evitar falha de segmentação */
  ritr_rel=relacionamentos.rbegin();
  ritr_rel_end=relacionamentos.rend();

  while(ritr_rel!=ritr_rel_end)
  {
   //Converte o ponteiro de relacionamento da conexão para a classe base de relacionametno
   rel_base=dynamic_cast<RelacionamentoBase *>(*ritr_rel);
   ritr_rel++;

   //Caso o relacionamento obtido da lista seja entre tabelas
   if(rel_base->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Converte o objeto para relacionamento tabela-tabela
    rel=dynamic_cast<Relacionamento *>(rel_base);
    rel->desconectarRelacionamento();
   }
   else
    rel_base->desconectarRelacionamento();
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::validarRelacionamentos(void)
{
 vector<BaseObject *>::iterator itr, itr_end, itr_ant;
 //vector<unsigned> vet_id_objs;
 Relacionamento *rel=NULL;
 RelacionamentoBase *rel_base=NULL;
 vector<BaseObject *> vet_rel, vet_rel_inv, rels;
 bool enc_rel_inv;
 vector<Exception> vet_erros;
 map<unsigned, QString>::iterator itr1, itr1_end;
 map<unsigned, Exception> mapa_erros;
 map<unsigned, Exception>::iterator itr2, itr2_end;
 unsigned idx;

 //Obtém os iterador para varredura inicial na lista global de relacionamentos
 itr=relacionamentos.begin();
 itr_end=relacionamentos.end();

 do
 {
  /* Marca como falso a flag indicativa de relacionamento invalidado.
     Este flag sempre será marcado quando 1 ou mais relacionamentos
     forem encontrados durante as interações. */
  enc_rel_inv=false;

  while(itr!=itr_end)
  {
   //Obtém um relacionamento a partir do iterador
   rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
   itr++;

   //Caso o relacionamento obtido seja tabela-tabela
   if(rel_base->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Converte o relacionamento base para rel. tabela-tabela
    rel=dynamic_cast<Relacionamento *>(rel_base);

    //Caso o relacionamento esteja invalidado
    if(rel->relacionamentoInvalidado())
    {
     //Insere-o na lista de relacionamentos invalidados
     vet_rel_inv.push_back(rel_base);
     /* Marca a flag indicando que pelo menos 1 relacionamento
        invalidado foi encontrado */
     enc_rel_inv=true;
    }
    else
     //Caso não esteja invalidado, insere-o na lista de relacionamentos válidos
     vet_rel.push_back(rel_base);
   }
  }

  //Caso haja algum relacionamento invalidado
  if(enc_rel_inv || !xml_objs_especiais.empty())
  {
   if(!carregando_modelo && xml_objs_especiais.empty())
    obterXMLObjetosEspeciais();

   //Desconecta todos os relacionamentos
   desconectarRelacionamentos();

   //Concatena ambas as lista para reconexão dos relacionamentos
   rels=vet_rel;

   /* Os relacionamentos invalidados vão para o final da lista pois
      estes precisam ser conectados por último para se tornarem válidos */
   rels.insert(rels.end(), vet_rel_inv.begin(), vet_rel_inv.end());

   /* Após criação da lista de relacionamentos temporária (rels) limpa
      as duas listas de relacionamentos usadas na iteração */
   vet_rel.clear();
   vet_rel_inv.clear();

   //Varre a lista temporária de relacionamentos
   itr=rels.begin();
   itr_end=rels.end();
   idx=0;

   while(itr!=itr_end)
   {
    //Obtém o relacionamento
    rel=dynamic_cast<Relacionamento *>(*itr);
    itr_ant=itr;

    //Passa para o próximo relacionamento da lista
    itr++;

    try
    {
     //Executa a conexão do relacionamento
     rel->conectarRelacionamento();
     idx++;
    }
    /* Caso um erro seja disparado durante a conexão do relacionamento o
       mesmo é considerado inválido permanentemente sendo necessário
       sua remoção do modelo */
    catch(Exception &e)
    {
     /* Remove o relacionamento chamando o método de remoção
        de objetos se verificação */
     __removerObjeto(rel);

     /* Remove o iterador da lista de relacionamentos inválidos para
        evitar que o mesmo seja verificado novamente */
     rels.erase(itr_ant);

     /* Reconfigura a navegação na lista de relacionamentos invalidados.
        Para evitar de que a lista seja varrida novamente desde seu início
        a variável auxiliar 'idx' é usada para apontar o iterador atual
        para o elemento posterior ao que foi removido */
     itr_end=rels.end();
     itr=rels.begin() + idx;

     //Desaloca o relacionamento
     delete(rel);

     //Armazena o erro capturado na lista de erros
     vet_erros.push_back(e);
    }
   }

   /* A nova lista (rels) criada com a nova ordenação de relacionamentos
      passará a ser a lista de relacionamentos a ser varrida nas iterações
      pois é nela que a ordenação de conexão dos relacionamentos é rearranjada. */
   itr=rels.begin();

   /* Caso o flag de modelo em carregamento não esteja marcado será
      feita a recriação dos objetos. Os objetos especiais são recriados
      a cada revalidação dos relacionamentos, sendo assim, tais objetos
      especiais em um dado momento podem ser criados com sucesso e em outros
      não ser criados por quebrarem as referências devido a conexão e
      desconexão constante dos relacionamentos. Quando um erro é encontrado
      na recriação de um objeto especial o mesmo é armazenado em um mapa de
      controle de erros do objeto, onde a chave deste mapa é o identificador
      do próprio objeto gerador do erro. Enquanto tal objeto não for recriado
      com sucesso o erro de geração é armazenado. Ao final, quando todos os
      relacionamentos estiverem sido válidados e o mapa de erros estiver com
      algum elemento de erro, os mesmos são disparados ao usuário indicando
      que alguns objetos especiais não puderam ser criados na validação dos
      relacionamentos. */
   //Obtém os iteradores da lista de xml dos objetos especiais
   itr1=xml_objs_especiais.begin();
   itr1_end=xml_objs_especiais.end();

   if(!carregando_modelo && itr1!=itr1_end)
   {
    do
    {
     try
     {
      //Tenta criar o objeto especial a partir do iterador atual
      criarObjetoEspecial(itr1->second, itr1->first);

      /* Caso algum erro anterior de criação do objeto especial em questão for
         detectado o mesmo é removido do mapa de controle de erros */
      if(mapa_erros.count(itr1->first))
       mapa_erros.erase(mapa_erros.find(itr1->first));

      /* Remove a definição xml do objeto atual da lista indicando
         que o mesmo foi criado com sucesso */
      xml_objs_especiais.erase(itr1);

      //Reinicia a navegação na lista de objetos especiais
      itr1=xml_objs_especiais.begin();
      itr1_end=xml_objs_especiais.end();
     }
     catch(Exception &e)
     {
      /* Caso uma exceção for caputarada na criação do objeto especial
         armazena o erro no mapa de erros sendo que a chave do mapa
         é o identificador do objeto gerador do erro */
      mapa_erros[itr1->first]=e;
      //Passa para o próximo objeto especial para tentar sua recriação
      itr1++; idx++;
     }
    }
    while(itr1!=itr1_end);
   }
  }
 }
 //Continua a iteração enquanto houver relacionamentos invalidados
 while(enc_rel_inv);

 /* Caso hajam elementos no mapa de controle de erros de criação
    dos objetos especiais, os mesmos são inseridos na lista geral
    de erros de validação para serem disparados ao final do método */
 itr2=mapa_erros.begin();
 itr2_end=mapa_erros.end();
 while(itr2!=itr2_end)
 {
  //Insere o erro na lista geral de erros
  vet_erros.push_back(itr2->second);
  itr2++;
 }

 /* Caso a lista geral de erros não esteja vazia os erros serão
    disparados ao usuário */
 if(!vet_erros.empty())
 {
  /* Limpa a lista de objetos especiais para garantir que eventuais objetos
    não recriados pela quebra das referências não sejam trabalhados pelo
    método de recriação dos objetos especiais */
  xml_objs_especiais.clear();

  throw Exception(ERR_INVALIDATED_OBJECTS,__PRETTY_FUNCTION__,__FILE__,__LINE__,vet_erros);
 }
}

void ModeloBD::verificarRedundanciaRelacoes(Relacionamento *rel)
{
 try
 {
  unsigned tipo_rel;

  /* Retorna um erro caso se tente verificar redundância
     de relacionamentos a partir de um relacionamento
     não alocado */
  if(!rel)
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  tipo_rel=rel->obterTipoRelacionamento();

  /* Auto relacionamentos são desconsiderados da verificação.
     Já relacionamentos identificadores ou relacionamentos
     os quais possuem atributos identificadores são verificados */
  if((!rel->autoRelacionamento() &&
      (rel->relacionamentoIdentificador() ||
       rel->possuiAtributoIdentificador())) ||

     (tipo_rel==Relacionamento::RELACIONAMENTO_GEN ||
      tipo_rel==Relacionamento::RELACIONAMENTO_DEP))
  {
   BaseTable *tabela_ref=NULL, *tab_orig=NULL;
   Tabela *tabela_rec=NULL;
   Relacionamento *rel_aux=NULL;
   RelacionamentoBase *rel_base=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   bool ciclo_enc=false;
   unsigned tipo_rel_aux;
   QString str_aux, msg;

   //Obtém tabela referencia e tabela receptora do relacionamento
   tabela_rec=rel->obterTabelaReceptora();
   tabela_ref=rel->obterTabelaReferencia();

   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();

   /* Com base nas tabela obtidas, varre a lista de relacionamento em busca do ciclo.
      Um ciclo será detectado quando a tabela referencia de um relacionamento for
      a tabela recptora do relacionamento usado na validação. */
   while(itr!=itr_end && !ciclo_enc)
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    itr++;

    if(rel_base->getObjectType()==OBJ_RELATIONSHIP)
    {
     //Obtém um relacionamento da lista
     rel_aux=dynamic_cast<Relacionamento *>(rel_base);
     //Obtém o tipo do relacionamento atual
     tipo_rel_aux=rel_aux->obterTipoRelacionamento();
     //Obtém a tabela referência do relacionamento atual
     tab_orig=rel_aux->obterTabelaReferencia();

     /* Caso a tabela referencia obtida seja igual a tabela receptora
        do relacionamento usado na validação e o tipo do relacionamento
        atual seja o mesmo do relacionamento da validação, isso pode indicar
        um princípio de fechamento de ciclo, para isso a validação prosseguirá
        com a tabela receptora do relacionamento atual até que o a tabela receptora
        seja a própria tabela de referência do relacionamento da validação. */
     if(tabela_rec==tab_orig &&  tipo_rel_aux==tipo_rel &&
        ((!rel_aux->autoRelacionamento() &&
          (rel_aux->relacionamentoIdentificador() ||
           rel_aux->possuiAtributoIdentificador())) ||
         (tipo_rel_aux==Relacionamento::RELACIONAMENTO_GEN ||
          tipo_rel_aux==Relacionamento::RELACIONAMENTO_DEP)))

     {
      //A tabela receptora passará a ser a tabela receptora do relacionamento atual
      tabela_rec=rel_aux->obterTabelaReceptora();
      //Armazena o nome do relacionamento para disparar erro caso um ciclo seja fechado
      str_aux+=rel_aux->getName() + ", ";
      /* Um ciclo será detectado caso a tabela receptora seja igual a tabela referência
         do relacionamento usado na validação */
      ciclo_enc=(tabela_rec==tabela_ref);
      //Volta ao início da lista de relacionamento para uma nova validação
      itr=relacionamentos.begin();
     }
    }
   }

   /* Caso um ciclo seja encontrado, dispara um erro contendo os nomes dos
      relacionamentos os quais fecham o ciclo */
   if(ciclo_enc)
   {
    str_aux+=rel->getName();
    msg=Exception::getErrorMessage(ERR_INS_REL_GENS_REDUNDACY)
        .arg(rel->getName())
        .arg(str_aux);
    throw Exception(msg,ERR_INS_REL_GENS_REDUNDACY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::obterXMLObjetosEspeciais(void)
{
 unsigned qtd, i, id_tipo;

 vector<BaseObject *>::iterator itr, itr_end;
 Sequencia *sequencia=NULL;
 Tabela *tabela=NULL;
 TableObject *obj_tab=NULL;
 Restricao *restricao=NULL;
 Index *indice=NULL;
 Gatilho *gatilho=NULL;
 Visao *visao=NULL;
 RelacionamentoBase *rel=NULL;
 Referencia ref;
 ObjectType tipo_obj_tab[3]={ OBJ_CONSTRAINT, OBJ_TRIGGER, OBJ_INDEX };
 bool enc=false;

 try
 {
  /* Primeiramente, varre a lista de tabelas em busca de restrições, gatilhos e índices
     os quais possam estar referenciando colunas adicionadas por relacionamentos */
  itr=tabelas.begin();
  itr_end=tabelas.end();

  while(itr!=itr_end)
  {
   //Obtém a tabela a partir do iterador atual
   tabela=dynamic_cast<Tabela *>(*itr);
   itr++;

   //Varre as listas de restrição e índice
   for(id_tipo=0; id_tipo < 3; id_tipo++)
   {
    //Obtém o a quantidade de objetos do tipo de objeto de tabela atual
    qtd=tabela->obterNumObjetos(tipo_obj_tab[id_tipo]);

    //Varre a lista atual de tipo de objetos de tabela
    for(i=0; i < qtd; i++)
    {
     //Obtém um objeto da lista atual na posição atual
     obj_tab=dynamic_cast<TableObject *>(tabela->obterObjeto(i, tipo_obj_tab[id_tipo]));
     enc=false;

     //Caso seja uma restrição
     if(tipo_obj_tab[id_tipo]==OBJ_CONSTRAINT)
     {
      //Converte o objeto genérico (ObjetoTabela) para restrição
      restricao=dynamic_cast<Restricao *>(obj_tab);

      /* Uma restrição só será considerada como caso especial quando a mesma não foi
         incluída por relacionamento, ou seja, tal restrição a qual referencia colunas
         adicionadas por relacionamentos foi criada pelo usuário. */
      enc=(!restricao->isAddedByRelationship() &&
            restricao->referenciaColunaIncRelacao() &&
            restricao->obterTipoRestricao()!=TipoRestricao::primary_key);

      /* Caso uma restrição seja encontrada obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[restricao->getObjectId()]=restricao->getCodeDefinition(SchemaParser::XML_DEFINITION, true);
     }
     else if(tipo_obj_tab[id_tipo]==OBJ_TRIGGER)
     {
      //Converte o objeto tabela genérico em gatilho
      gatilho=dynamic_cast<Gatilho *>(obj_tab);

      /* O gatilho só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      enc=gatilho->isReferRelationshipColumn();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[gatilho->getObjectId()]=gatilho->getCodeDefinition(SchemaParser::XML_DEFINITION);
     }
     //Caso seja um índice
     else
     {
      //Converte o objeto tabela genérico em índice
      indice=dynamic_cast<Index *>(obj_tab);

      /* O índice só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      enc=indice->isReferRelationshipColumn();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[indice->getObjectId()]=indice->getCodeDefinition(SchemaParser::XML_DEFINITION);
     }

     //Caso algum objeto especial for encontrado
     if(enc)
     {
      //Remove o mesmo da tabela possuidora
      tabela->removerObjeto(obj_tab->getName(), obj_tab->getObjectType());

      //Remove as permissões existentes para o objeto
      removerPermissoes(obj_tab);

      //Decrementa os controladores da iteração para reiniciar a varredura
      i--; qtd--;
     }
    }
   }
  }

  /* Varre a lista de sequencias para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=sequencias.begin();
  itr_end=sequencias.end();

  while(itr!=itr_end)
  {
   //Obtém a sequencia atual através do iterador atual
   sequencia=dynamic_cast<Sequencia *>(*itr);
   itr++;

   /* Caso a coluna for incluída por relacionamento considera
      a sequencia como objeto especial */
   if(sequencia->referenciaColunaIncRelacao())
   {
    xml_objs_especiais[sequencia->getObjectId()]=sequencia->getCodeDefinition(SchemaParser::XML_DEFINITION);
    removerSequencia(sequencia);
    delete(sequencia);
   }
  }

  /* Varre a lista de visões para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=visoes.begin();
  itr_end=visoes.end();

  while(itr!=itr_end)
  {
   //Obtém a visão atual através do iterador atual
   visao=dynamic_cast<Visao *>(*itr);
   itr++;

   /* Caso a visão esteja referenciando uma coluna incluída por
      relacionamento considera a mesma como objeto especial */
   if(visao->referenciaColunaIncRelacao())
   {
    //Armazena a definição XML da visão
    xml_objs_especiais[visao->getObjectId()]=visao->getCodeDefinition(SchemaParser::XML_DEFINITION);

    /* Caso hajam relacionamentos ligando a visão e as tabelas referenciadas
       os mesmo também serão armazenados como objetos especiais para posterior
       recriação */

    //Obtém a quantidade de referências as tabelas
    qtd=visao->obterNumReferencias(Referencia::SQL_REFER_SELECT);

    for(i=0; i < qtd; i++)
    {
     //Obtém uma referência
     ref=visao->obterReferencia(i, Referencia::SQL_REFER_SELECT);
     //Obtém a tabela da referência
     tabela=ref.obterTabela();

     if(tabela)
     {
      /* Caso a tabela exista e um relacionamento tabela-visão entra a visão
         em questão e a tabela obtida, o mesmo será obtido do modelo e
         sua definição XML armazenada */
      rel=obterRelacionamento(visao, tabela);

      if(rel)
      {
       //Armazena a definição xml do relacionamento
       xml_objs_especiais[rel->getObjectId()]=rel->getCodeDefinition();
       //Remove o mesmo do modelo
       removerRelacionamento(rel);
       //Desaloca o relacionamento obtido
       delete(rel);
      }
     }
    }

    //Remove a visão do modelo e a desaloca
    removerVisao(visao);
    delete(visao);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::criarObjetoEspecial(const QString &def_xml_obj, unsigned id_obj)
{
 ObjectType tipo_obj;
 BaseObject *objeto=NULL;

 try
 {
  //Reinicia o parser XML para nova leitura de buffer arquivo
  XMLParser::restartParser();

  //Carrega o buffer XML do parser com a definição XML do objeto especial
  XMLParser::loadXMLBuffer(def_xml_obj);

  //Identificando o tipo de objeto de acordo com o elemento obtido anteriormente
  tipo_obj=getObjectType(XMLParser::getElementName());

  //Cria o objeto de acordo com o tipo identificado
  if(tipo_obj==OBJ_SEQUENCE)
   objeto=criarSequencia(true);
  else
   objeto=criarObjeto(tipo_obj);

  if(tipo_obj==OBJ_SEQUENCE)
   adicionarSequencia(dynamic_cast<Sequencia *>(objeto));
  else if(tipo_obj==OBJ_VIEW)
   adicionarVisao(dynamic_cast<Visao *>(objeto));

  /* Modifica o id do objeto para o valor do id passado no parâmetro.
     Como um novo id é atribuído quando o objeto é instanciado e levando
     em conta que os objetos especias são realocados e recebem novo id
     isso é necessário para manter a ordem dos objetos da forma como
     os mesmos foram carregados a partir do arquivo evitando a quebra
     de referências. */
  if(objeto && id_obj!=0)
   objeto->object_id=id_obj;
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, def_xml_obj);
 }
}

void ModeloBD::adicionarRelacionamento(RelacionamentoBase *relacao, int idx_obj)
{
 try
 {
  BaseTable *tab1=NULL, *tab2=NULL;
  QString msg;


  if(relacao)
  {
   /* Caso o relacionamento esteja alocado verifica se já não existe um
      relacionamento no modelo entre as tabelas envolvidas */
   //tipo_rel=relacao->obterTipoObjeto();
   tab1=relacao->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
   tab2=relacao->obterTabela(RelacionamentoBase::TABELA_DESTINO);

   //Caso exista um relacionamento entre as tabelas será disparado um erro
   if(obterRelacionamento(tab1,tab2))
   {
    //__removerObjeto(relacao);
    msg=Exception::getErrorMessage(ERR_DUPLIC_RELATIONSHIP)
        .arg(tab1->getTypeName())
        .arg(tab1->getName(true))
        .arg(tab2->getTypeName())
        .arg(tab2->getName(true));
    throw Exception(msg,ERR_DUPLIC_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  //Adiciona o objeto no modelo
  if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   verificarRedundanciaRelacoes(dynamic_cast<Relacionamento *>(relacao));

  __adicionarObjeto(relacao, idx_obj);

  if(relacao->getObjectType()==OBJ_RELATIONSHIP)
  {
   //Conecta o novo relacionamento
   dynamic_cast<Relacionamento *>(relacao)->conectarRelacionamento();

   //Valida os relacionamentos para propagação das colunas
   validarRelacionamentos();
  }
  else
   relacao->conectarRelacionamento();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerRelacionamento(RelacionamentoBase *relacao, int idx_obj)
{
 try
 {
  if(obterIndiceObjeto(relacao) >= 0)
  {
   if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Desconecta os relacionamentos
    obterXMLObjetosEspeciais();
    desconectarRelacionamentos();
   }
   else if(relacao->getObjectType()==BASE_RELATIONSHIP)
    relacao->desconectarRelacionamento();

   __removerObjeto(relacao, idx_obj);

   if(relacao->getObjectType()==OBJ_RELATIONSHIP)
   {
    //Valida os relacionamentos após a remoção de o relacionamento atual
    validarRelacionamentos();
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

RelacionamentoBase *ModeloBD::obterRelacionamento(unsigned idx_obj, ObjectType tipo_rel)
{
 //Caso o tipo de relacionamento seja inválido
 if(tipo_rel!=OBJ_RELATIONSHIP && tipo_rel!=BASE_RELATIONSHIP)
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(dynamic_cast<RelacionamentoBase *>(obterObjeto(idx_obj, tipo_rel)));
}

RelacionamentoBase *ModeloBD::obterRelacionamento(BaseTable *tab_orig, BaseTable *tab_dest)
{
 vector<BaseObject *>::iterator itr, itr_end;
 RelacionamentoBase *rel=NULL;
 bool enc=false, buscar_tab_unica=false;
 BaseTable *tab1=NULL, *tab2=NULL;

 if(tab_orig)
 {
  /* Caso a tabela de destino não esteja especificada,
     indica que que o método deve procurar por relacionamentos
     onde a tabela de origem (tab_orig)  existe não importando se ela é a
     referência ou a receptora. A execução normal (tab_dest!=NULL)
     o método só retorna um relacionamento onde ambas as tabelas estão
     presentas */
  if(!tab_dest)
  {
   tab_dest=tab_orig;
   buscar_tab_unica=true;
 }

  /* Definindo os iteradores de acordo com os objetos envolvidos
     no relacionamento */

  /* Caso um dos objetos seja uma visão, a lista de relacionamentos
     tabela-visão será varrida, caso contrário a lista de relacionamentos
     tabela-tabela será usada */
  if(tab_orig->getObjectType()==OBJ_VIEW ||
     tab_dest->getObjectType()==OBJ_VIEW)
  {
   itr=relac_visoes.begin();
   itr_end=relac_visoes.end();
  }
  else
  {
   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();
  }

  while(itr!=itr_end && !enc)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);
   tab1=rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
   tab2=rel->obterTabela(RelacionamentoBase::TABELA_DESTINO);

   /* Verifica se os elementos do parâmetro coincidem com os elementos
      do relacionamento */
   enc=((tab1==tab_orig && tab2==tab_dest) ||
        (tab2==tab_orig && tab1==tab_dest) ||
        (buscar_tab_unica && (tab1==tab_orig || tab2==tab_orig)));
   if(!enc)
   { rel=NULL; itr++; }
  }
 }

 return(rel);
}

void ModeloBD::adicionarCaixaTexto(Textbox *caixa, int idx_obj)
{
 try
 {
  __adicionarObjeto(caixa, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerCaixaTexto(Textbox *caixa, int idx_obj)
{
 __removerObjeto(caixa, idx_obj);
}

Textbox *ModeloBD::obterCaixaTexto(unsigned idx_obj)
{
 return(dynamic_cast<Textbox *>(obterObjeto(idx_obj, OBJ_TEXTBOX)));
}

void ModeloBD::adicionarEsquema(Schema *esquema, int idx_obj)
{
 try
 {
  __adicionarObjeto(esquema, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Schema *ModeloBD::getSchema(unsigned idx_obj)
{
 return(dynamic_cast<Schema *>(obterObjeto(idx_obj, OBJ_SCHEMA)));
}

void ModeloBD::removerEsquema(Schema *esquema, int idx_obj)
{
 if(esquema)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao esquema
  obterReferenciasObjeto(esquema, vet_refs, true);

 /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */

   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(esquema->getName(true))
                 .arg(esquema->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(esquema, idx_obj);
 }
}

void ModeloBD::adicionarPapel(Papel *papel, int idx_obj)
{
 try
 {
  __adicionarObjeto(papel, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Papel *ModeloBD::obterPapel(unsigned idx_obj)
{
 return(dynamic_cast<Papel *>(obterObjeto(idx_obj, OBJ_ROLE)));
}

void ModeloBD::removerPapel(Papel *papel, int idx_obj)
{
 if(papel)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao papel
  obterReferenciasObjeto(papel, vet_refs, true);

 /* Caso o papel esteja sendo referenciado, por algum objeto a
     mesma não pode ser removido */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(papel->getName(true))
                 .arg(papel->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(papel, idx_obj);
 }
}

void ModeloBD::adicionarEspacoTabela(Tablespace *espaco_tab, int idx_obj)
{
 try
 {
  __adicionarObjeto(espaco_tab, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tablespace *ModeloBD::getTablespace(unsigned idx_obj)
{
 return(dynamic_cast<Tablespace *>(obterObjeto(idx_obj, OBJ_TABLESPACE)));
}

void ModeloBD::removerEspacoTabela(Tablespace *espaco_tab, int idx_obj)
{
 if(espaco_tab)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referências ao espaço de tabela
  obterReferenciasObjeto(espaco_tab, vet_refs, true);

  /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referencia direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(espaco_tab->getName(true))
            .arg(espaco_tab->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());
   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();
    //Formata a mensagem para referencia indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(espaco_tab->getName(true))
            .arg(espaco_tab->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());

   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(espaco_tab, idx_obj);
 }
}

void ModeloBD::adicionarConversaoTipo(Cast *conv_tipo, int idx_obj)
{
 try
 {
  __adicionarObjeto(conv_tipo, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerConversaoTipo(Cast *conv_tipo, int idx_obj)
{
 __removerObjeto(conv_tipo, idx_obj);
}

Cast *ModeloBD::obterConversaoTipo(unsigned idx_obj)
{
 return(dynamic_cast<Cast *>(obterObjeto(idx_obj, OBJ_CAST)));
}

void ModeloBD::adicionarConversaoCodificacao(Conversion *conv_codificacao, int idx_obj)
{
 try
 {
  __adicionarObjeto(conv_codificacao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerConversaoCodificacao(Conversion *conv_codificacao, int idx_obj)
{
 __removerObjeto(conv_codificacao, idx_obj);
}

Conversion *ModeloBD::obterConversaoCodificacao(unsigned idx_obj)
{
 return(dynamic_cast<Conversion *>(obterObjeto(idx_obj,
 OBJ_CONVERSION)));
}

void ModeloBD::adicionarLinguagem(Language *linguagem, int idx_obj)
{
 try
 {
  __adicionarObjeto(linguagem, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Language *ModeloBD::obterLinguagem(unsigned idx_obj)
{
 return(dynamic_cast<Language *>(obterObjeto(idx_obj, OBJ_LANGUAGE)));
}

void ModeloBD::removerLinguagem(Language *linguagem, int idx_obj)
{
 if(linguagem)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referênca   linguagem
  obterReferenciasObjeto(linguagem, vet_refs, true);

 /* Caso a linguagem esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(linguagem->getName(true))
                 .arg(linguagem->getTypeName())
                 .arg(dynamic_cast<Function *>(vet_refs[0])->getSignature())
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(linguagem, idx_obj);
 }
}

void ModeloBD::adicionarFuncao(Function *funcao, int idx_obj)
{
 try
 {
  __adicionarObjeto(funcao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Function *ModeloBD::obterFuncao(unsigned idx_obj)
{
 return(dynamic_cast<Function *>(obterObjeto(idx_obj, OBJ_FUNCTION)));
}

void ModeloBD::removerFuncao(Function *funcao, int idx_obj)
{
 if(funcao)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referênca   função
  obterReferenciasObjeto(funcao, vet_refs, true);

 /* Caso a função esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
            .arg(funcao->getSignature())
            .arg(funcao->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());

   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();
    //Formata a mensagem para referência indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE))
            .arg(funcao->getSignature())
            .arg(funcao->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());

   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(funcao, idx_obj);
 }
}

void ModeloBD::adicionarFuncaoAgregacao(Aggregate *func_agregacao, int idx_obj)
{
 try
 {
  __adicionarObjeto(func_agregacao, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Aggregate *ModeloBD::obterFuncaoAgregacao(unsigned idx_obj)
{
 return(dynamic_cast<Aggregate *>(obterObjeto(idx_obj, OBJ_AGGREGATE)));
}

void ModeloBD::removerFuncaoAgregacao(Aggregate *func_agregacao, int idx_obj)
{
 __removerObjeto(func_agregacao, idx_obj);
}

void ModeloBD::adicionarDominio(Domain *dominio, int idx_obj)
{
 if(dominio)
 {
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de tipos para verificar se existe algum tipo
     com o mesmo nome do domínio */
  itr=tipos.begin();
  itr_end=tipos.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->getName(true)==dominio->getName(true));
   itr++;
  }

  //Caso exista um tipo de mesmo nome que o domínio dispara o erro
  if(enc)
  {
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(dominio->getName(true))
           .arg(dominio->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o domínio
   __adicionarObjeto(dominio, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionad  lista de tipos válidos
    do PostgreSQL */
   TipoPgSQL::adicionarTipoUsuario(dominio->getName(true), dominio, this, ConfigTipoUsuario::TIPO_DOMINIO);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void ModeloBD::removerDominio(Domain *dominio, int idx_obj)
{
 try
 {
  removerTipoUsuario(dominio, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Domain *ModeloBD::obterDominio(unsigned idx_obj)
{
 return(dynamic_cast<Domain *>(obterObjeto(idx_obj, OBJ_DOMAIN)));
}

void ModeloBD::adicionarFamiliaOperadores(OperatorFamily *familia_op, int idx_obj)
{
 try
 {
  __adicionarObjeto(familia_op, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

OperatorFamily *ModeloBD::obterFamiliaOperadores(unsigned idx_obj)
{
 return(dynamic_cast<OperatorFamily *>(obterObjeto(idx_obj, OBJ_OPFAMILY)));
}

void ModeloBD::removerFamiliaOperadores(OperatorFamily *familia_op, int idx_obj)
{
 if(familia_op)
 {
  vector<BaseObject *> vet_refs;

  //OBtém as referências ao objeto
  obterReferenciasObjeto(familia_op, vet_refs, true);

   /* Caso a familía esteja sendo referenciada, por algum objeto a
       mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(familia_op->getName(true))
                 .arg(familia_op->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(familia_op, idx_obj);
 }
}

void ModeloBD::adicionarClasseOperadores(OperatorClass *classe_op, int idx_obj)
{
 try
 {
  __adicionarObjeto(classe_op, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerClasseOperadores(OperatorClass *classe_op, int idx_obj)
{
 __removerObjeto(classe_op, idx_obj);
}

OperatorClass *ModeloBD::obterClasseOperadores(unsigned idx_obj)
{
 return(dynamic_cast<OperatorClass *>(obterObjeto(idx_obj, OBJ_OPCLASS)));
}

void ModeloBD::adicionarOperador(Operator *operador, int idx_obj)
{
 try
 {
  __adicionarObjeto(operador, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerOperador(Operator *operador, int idx_obj)
{
 if(operador)
 {
  vector<BaseObject *> vet_refs;

  //Obtém as referências ao operador
  obterReferenciasObjeto(operador, vet_refs, true);

 /* Caso o operador esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   //Formata a mensagem para referência direta
   throw Exception(QString(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE))
                 .arg(operador->obterAssinatura(true))
                 .arg(operador->getTypeName())
                 .arg(vet_refs[0]->getName(true))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(operador, idx_obj);
 }
}

Operator *ModeloBD::obterOperador(unsigned idx_obj)
{
 return(dynamic_cast<Operator *>(obterObjeto(idx_obj, OBJ_OPERATOR)));
}

void ModeloBD::adicionarTipo(Tipo *tipo, int idx_obj)
{
 if(tipo)
 {
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de dominios para verificar se existe algum
     domínio com o mesmo nome do tipo */
  itr=dominios.begin();
  itr_end=dominios.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->getName(true)==tipo->getName(true));
   itr++;
  }

  //Caso exista um dominio de mesmo nome que o tipo dispara o erro
  if(enc)
  {
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(tipo->getName(true))
           .arg(tipo->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux, ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o tipo
   __adicionarObjeto(tipo, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionad  lista de tipos válidos
    do PostgreSQL */
   TipoPgSQL::adicionarTipoUsuario(tipo->getName(true), tipo, this, ConfigTipoUsuario::TIPO_BASE);
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}

void ModeloBD::removerTipo(Tipo *tipo, int idx_obj)
{
 try
 {
  removerTipoUsuario(tipo, idx_obj);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tipo *ModeloBD::obterTipo(unsigned idx_obj)
{
 return(dynamic_cast<Tipo *>(obterObjeto(idx_obj, OBJ_TYPE)));
}

void ModeloBD::removerTipoUsuario(BaseObject *objeto, int idx_obj)
{
 if(objeto)
 {
  vector<BaseObject *> vet_refs;
  QString str_aux;

  //Obtém as referências ao objeto
  obterReferenciasObjeto(objeto, vet_refs, true);

  /* Caso o tipo esteja sendo referenciado, por algum objeto o
     mesmo não pode ser removida */
  if(!vet_refs.empty())
  {
   ErrorType tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<TableObject *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_REM_DIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(objeto->getName(true))
            .arg(objeto->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName());
   }
   else
   {
    BaseObject *obj_ref_pai=dynamic_cast<TableObject *>(vet_refs[0])->getParentTable();

    //Formata a mensagem para referência indireta
    tipo_err=ERR_REM_INDIRECT_REFERENCE;
    str_aux=QString(Exception::getErrorMessage(tipo_err))
            .arg(objeto->getName(true))
            .arg(objeto->getTypeName())
            .arg(vet_refs[0]->getName(true))
            .arg(vet_refs[0]->getTypeName())
            .arg(obj_ref_pai->getName(true))
            .arg(obj_ref_pai->getTypeName());
   }

   throw Exception(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(objeto, idx_obj);
   /* Ao ser removido do modelo o objeto (tipo ou domínio) classe tem
    seu nome removido da lista de tipos válidos do PostgreSQL */
  TipoPgSQL::removerTipoUsuario(objeto->getName(true), objeto);
 }
}

void ModeloBD::adicionarPermissao(Permissao *permissao)
{
 try
 {
  if(!permissao)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(obterIndicePermissao(permissao) >=0)
  {
  /* Caso haja uma permissão semelhante a que está sendo inserida, ou seja,
    o resultado da chamada ao metodo obterIndicePermissao() sejá >= 0,
    um erro será disparado */
   throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
                .arg(QString::fromUtf8(permissao->obterObjeto()->getName()))
                .arg(permissao->obterObjeto()->getTypeName()),
                ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  permissoes.push_back(permissao);
 }
 catch(Exception &e)
 {
  if(e.getErrorType()==ERR_ASG_DUPLIC_OBJECT)
   throw
   Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
           .arg(QString::fromUtf8(permissao->obterObjeto()->getName()))
           .arg(permissao->obterObjeto()->getTypeName()),
           ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

  else
   throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ModeloBD::removerPermissao(Permissao *permissao)
{
 if(permissao)
 {
  __removerObjeto(permissao);
 }
}

void ModeloBD::removerPermissoes(BaseObject *objeto)
{
 vector<Permissao *> vet_perm;
 vector<BaseObject *>::iterator itr, itr_end;
 Permissao *permissao=NULL;
 unsigned idx=0;

 /* Caso o objeto o qual terá todas as permissões relacionadas removidas não
    esteja alocado um erro será disparado pois o usuário está
    tentando remover permissões de um objeto inexistente */
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissoes.begin();
 itr_end=permissoes.end();

 /* Varre a lista de permissões removendo as permissões cujo
    objeto relacionado seja o mesmo do objeto do parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permissao *>(*itr);

  //Verifica se o objeto da permissão é igual ao objeto do parâmetro
  if(permissao->obterObjeto()==objeto)
  {
   //Remove o iterador que possui a permissão
   permissoes.erase(itr);
   //Desaloca a permissão
   delete(permissao);
   //Reinicializa os iteradores para uma nova varredura
   itr=itr_end=permissoes.end();

   /* Caso a lista não esteja vazia após remover a permissão
      desloca o iterador para a posição em que a varredura
      parou ao invés de iniciar uma nova a partir do início
      da lista */
   if(!permissoes.empty())
    itr=permissoes.begin() + idx;
  }
  else
  { itr++; idx++; }
 }
}

void ModeloBD::obterPermissoes(BaseObject *objeto, vector<Permissao *> &vet_perm)
{
 vector<BaseObject *>::iterator itr, itr_end;
 Permissao *permissao=NULL;

 /* Caso o objeto o qual terá todas as permissões relacionadas
    obtidas não esteja alocado um erro será disparado pois o
    usuário está tentando obter permissões de um objeto inexistente */
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissoes.begin();
 itr_end=permissoes.end();

 //Limpa a lista receptora de permissões
 vet_perm.clear();

 /* Varre a lista de permissões em busca daquelas
    cujo objeto seja o mesmo do objeto especificado
    no parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permissao *>(*itr);

  /* Caso a permissão esteja relacionada ao mesmo
     objeto do parâmetro insere tal permissão
       lista de permissões */
  if(permissao->obterObjeto()==objeto)
   vet_perm.push_back(permissao);

  itr++;
 }
}

int ModeloBD::obterIndicePermissao(Permissao *permissao)
{
 int idx_perm=-1;

 //Obtém o índice da permissão somente se a mesma estive alocada
 if(permissao)
 {
  Permissao *perm_aux=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  BaseObject *objeto=NULL;
  Papel *papel=NULL;
  unsigned qtd, i;
  bool papel_ref=false;

  itr=permissoes.begin();
  itr_end=permissoes.end();

  //Obtém o objeto da permissão
  objeto=permissao->obterObjeto();

  /* Varre a lista de permissões enquanto não chegar ao final
     da lista ou enquanto o índice da permissão não for
     descoberto */
  while(itr!=itr_end && idx_perm < 0)
  {
   //Obtém uma permissão da lista
   perm_aux=dynamic_cast<Permissao *>(*itr);

   /*CAso o objeto do parâmetro seja o mesmo da permissão
     será efetuada uma validação se todos os papeis
     de ambas as permissões são iguais, isso indica
     que ambas possuem o mesmo efeito */
   if(objeto==perm_aux->obterObjeto())
   {
    //Obtém a quantidade de papéis
    qtd=permissao->obterNumPapeis();

    //Varre a lista de papéis das permissões
    for(i=0; i < qtd && !papel_ref; i++)
    {
     //Obtém um papel da permissão do parâmetro
     papel=permissao->obterPapel(i);
     /* Com o papel obtido verifica-se se o mesmo é referenciado
        no papel obtido da lista principal de permissões */
     papel_ref=perm_aux->papelReferenciado(papel);
    }
   }

   /* Caso um papel ambas as permissões seja idênticas
      (mesmo endereço de memória) ou todos os papéis referenciados
      por ambas sejam os mesmos, o índice da permissão na lista
      será calculado */
   if(permissao==perm_aux || papel_ref)
    idx_perm=itr-permissoes.begin();

   itr++;
  }
 }

 return(idx_perm);
}

BaseObject *ModeloBD::obterObjeto(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 return(obterObjeto(nome, tipo_obj, idx));
}

int ModeloBD::obterIndiceObjeto(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 obterObjeto(nome, tipo_obj, idx);
 return(idx);
}

int ModeloBD::obterIndiceObjeto(BaseObject *objeto)
{
 if(!objeto)
  return(-1);
 else
 {
  ObjectType tipo_obj=objeto->getObjectType();
  vector<BaseObject *> *lista_obj=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  bool enc=false;

  //Obtém a lista de acordo com o tipo do objeto
  lista_obj=obterListaObjetos(tipo_obj);

  /* Caso a lista não exista indica que foi passado um tipo inválido
     de objeto, dessa forma será retornado um erro */
  if(!lista_obj)
   throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Obtém as referências para o inicio e o fim da lista
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end && !enc)
   {
    enc=((*itr)==objeto);
    if(!enc) itr++;
   }

   if(enc)
    return(itr-lista_obj->begin());
   else
    return(-1);
  }
 }
}

bool ModeloBD::carregandoModelo(void)
{
 return(carregando_modelo);
}

void ModeloBD::carregarModelo(const QString &nome_arq)
{
 if(nome_arq!="")
 {
  QString arq_dtd, str_aux, nome_elem;
  ObjectType tipo_obj;
  map<QString, QString> atributos;
  BaseObject *objeto=NULL;
  deque<const xmlNode *> objetos_incomp;
  bool reaval_objetos=false, modelo_protegido=false;
  const xmlNode *elem_aux=NULL;
  deque<Exception> vet_erros;
  map<unsigned, QString>::iterator itr, itr_end;

  /* Montando o caminho padrão para localização dos esquemas DTD.
     Por padrão esta pasta se encontra na raiz dentro da pasta esquemas/xml*/
  arq_dtd=GlobalAttributes::SCHEMAS_ROOT_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           GlobalAttributes::XML_SCHEMA_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           GlobalAttributes::OBJECT_DTD_DIR +
           GlobalAttributes::DIR_SEPARATOR;

  try
  {
   carregando_modelo=true;

   //Reinicia o parser XML para a leitura do arquivo
   XMLParser::restartParser();
   //Faz com que o parser carregue a DTD que armazena a sintaxe do arquivo de modelos
   XMLParser::setDTDFile(arq_dtd + GlobalAttributes::ROOT_DTD +
                                GlobalAttributes::OBJECT_DTD_EXT,
                                GlobalAttributes::ROOT_DTD);

   //Carrega o arquivo validando-o de acordo com a DTD informada
   XMLParser::loadXMLFile(nome_arq);

   //Obter as informações de versão, autor do modelo e versão postgresql
   XMLParser::getElementAttributes(atributos);
   this->autor=atributos[ParsersAttributes::MODEL_AUTHOR];

   modelo_protegido=(atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

   //Passa para o próximo elemento que provavelmente será um <role> ou <tablespace>
   if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
   {
    do
    {
     if(XMLParser::getElementType()==XML_ELEMENT_NODE)
     {
      nome_elem=XMLParser::getElementName();

      /* Caso o nome do elemento atual seja o que define uma permissão
         indica que o parser leu todos os objetos do modelo e o mesmo
         se encontra no trecho final do arquivo onde são armazenadas
         as permissões sobre tais objetos. A obtenção das permissões
         só serão feitas caso não hajam objetos para serem reavaliados,
         caso contrário, os objetos são reavalias e logo apos as permissões
         obtidas. */
      if(nome_elem==ParsersAttributes::PERMISSION && objetos_incomp.size()==0)
      {
       /* Caso a lista de objetos especiais esteja com elementos
          efetua a recriação dos mesmos. Obs: Este processo é executado
          apenas 1 vez caso a lista de objetos especiais contenha algum item
          independente do número de permissões a serem criadas pois esta lista
          é preenchida durante o processo de criação dos objetos e não na criação
          das permissões. */
       if(!xml_objs_especiais.empty())
       {
        /* Efetua a cópia do buffer atual do parser pois
           a restauração de objetos especiais apaga o buffer
           recente do parser para serem usados os códigos xml
           de tais objetos */
        str_aux=XMLParser::getXMLBuffer();

        itr=xml_objs_especiais.begin();
        itr_end=xml_objs_especiais.end();

        while(itr!=itr_end)
        {
         /* Cria o objeto especial a partir do iterador atual
            sendo que o elemento 'itr->second' possui a definição xml
            do mesmo e o elemento 'itr->first' possui o id do objeto
            especial a ser recriado */
         criarObjetoEspecial(itr->second, itr->first);
         itr++;
        }

        /* Limpa a lista de objetos especiais para evitar que os mesmos
           tentem ser recriados posteriormente */
        xml_objs_especiais.clear();

        /* Após a restauração dos objetos especiais é necessário recarregar
          o buffer do parser com o buffer usando anteriormente   restauração
          deos objetos especiais para que as permissões possam ser criadas
          a partir do XML */
        XMLParser::restartParser();
        XMLParser::loadXMLBuffer(str_aux);
        XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

        /* Executa a navegação sobre os elementos até que o primeiro elemento
           que define uma permissão seja localizado */
        while(XMLParser::getElementName()!=ParsersAttributes::PERMISSION &&
              XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
       }

       //Executa acriação da permissão atual a partir do xml
       adicionarPermissao(criarPermissao());
      }
      else
      {
       //Obtém o tipo do elemento de acordo com a tag atual que o parser carregou
       tipo_obj=getObjectType(nome_elem);

       /* Este bloco faz a reavaliação de objetos. A reavaliação consiste em
          ler um trecho do arquivo XML novamente onde algum objeto foi referenciado
          porém ainda não criado causando o disparo de exceções reclamando de
          objetos com definição SQL incompleta.

          Condição para reavaliação dos objetos:
           * A flag de reavaliação (reaval_objetos) não esteja marcada indicando
             que uma reavaliação não está ocorrendo
           * A lista de objetos incompletos possua pelo menos 1 item, indicando que
             em algum trecho do XML existe um objeto o qual referencia outro que não
             foi encontrado (criado)
       */
       if(!reaval_objetos && objetos_incomp.size() > 0 &&
          tipo_obj!=OBJ_FUNCTION && tipo_obj!=OBJ_SCHEMA &&
          tipo_obj!=OBJ_TABLESPACE && tipo_obj!=OBJ_LANGUAGE &&
          tipo_obj!=OBJ_TYPE && tipo_obj!=OBJ_TABLE)
       {
        //Faz o parser XML voltar ao elemento anterior
        XMLParser::accessElement(XMLParser::PREVIOUS_ELEMENT);
        //Armazena a posição atual do parser
        elem_aux=XMLParser::getCurrentElement();

        reaval_objetos=true;
        //Restaura o parser para a posição onde o objeto incompleto foi lido
        XMLParser::restorePosition(objetos_incomp.front());
        //Obtém o tipo deste objeto
        tipo_obj=getObjectType(XMLParser::getElementName());
       }

       //Lendo um objeto banco de dados
       if(tipo_obj==OBJ_DATABASE)
       {
        XMLParser::getElementAttributes(atributos);
        tipo_codif=atributos[ParsersAttributes::ENCODING];
        bd_modelo=atributos[ParsersAttributes::TEMPLATE_DB];
        localizacoes[0]=atributos[ParsersAttributes::LC_CTYPE_DB];
        localizacoes[1]=atributos[ParsersAttributes::LC_COLLATE_DB];

        if(!atributos[ParsersAttributes::CONN_LIMIT].isEmpty())
         lim_conexao=atributos[ParsersAttributes::CONN_LIMIT].toInt();

        definirAtributosBasicos(this);
       }
       else
       {
        try
        {
         /* Para os demais objetos, sempre a posição do parser XMl é salva
            antes da leitura e criação dos mesmos, para que a reavaliação
            seja possível, quando for necessária */
         XMLParser::savePosition();
         //Cria o objeto de acordo com  o tipo obtido
         objeto=criarObjeto(tipo_obj);

         //Caso o objeto foi criado com sucesso
         if(objeto)
         {
          if(!dynamic_cast<TableObject *>(objeto) &&
             tipo_obj!=OBJ_RELATIONSHIP && tipo_obj!=BASE_RELATIONSHIP)
          {
           //Usa o método de inserção de objetos genéricos
           adicionarObjeto(objeto);
          }

          //Dispara um sinal com o progresso (aproximado) do carregamento de objetos
          if(!signalsBlocked())
          {
           emit s_objetoCarregado(XMLParser::getCurrentBufferLine()/XMLParser::getBufferLineCount(),
                                  trUtf8("Loading object: %1 (%2)")
                                   .arg(QString::fromUtf8(objeto->getName()))
                                   .arg(objeto->getTypeName()),
                                  tipo_obj);
          }
         }

         //Restaura a posição do parser para o elemento anterior ao atual
         XMLParser::restorePosition();
        }
        catch(Exception &e)
        {
         if(!reaval_objetos &&
            (((e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL && tipo_obj==OBJ_TABLE)) ||
             (((e.getErrorType()==ERR_ASG_OBJ_INV_DEFINITION ||
                e.getErrorType()==ERR_REF_OBJ_INEXISTS_MODEL ||
                e.getErrorType()==ERR_ASG_INV_TYPE_OBJECT) &&
               (tipo_obj==OBJ_LANGUAGE || tipo_obj==OBJ_FUNCTION || tipo_obj==OBJ_TYPE || tipo_obj==OBJ_OPERATOR)))))
         {
          /* Adiciona o nó da arvore o qual possui o elemento incompleto
             para que o mesmo tente ser recriado após suas possíveis dependências
             serem carregadas */
          XMLParser::restorePosition();
          objetos_incomp.push_back(XMLParser::getCurrentElement());
          //Armazena a exceção capturada para verificações ao final do carregamento do modelo
          vet_erros.push_back(e);
         }
         else
         {
          QString info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);
          throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
         }
        }
       }
      }

      /* Caso esteja num processo de reavaliação de objetos, o bloco abaixo
         faz a retirada do objeto que acaba de ser reavaliado da lista de
         objetos incompletos */
      if(reaval_objetos && objetos_incomp.size() > 0)
      {
       //Remove o objeto incompleto da lista
       objetos_incomp.pop_front();

       //Remove a exceção relacionada ao objeto incompleto
       vet_erros.pop_front();

       /* Caso a lista ainda possua elementos o parser será voltado para o
          elemento incompleto anterior ao atual */
       if(objetos_incomp.size() > 0)
        XMLParser::restorePosition(objetos_incomp.front());
       else
       {
        //Caso a lista esteja vazia o processo de reavaliação é interrompido
        reaval_objetos=false;
        //O parser é retornad  posição em que se encontrava antes da reavaliação
        XMLParser::restorePosition(elem_aux);
       }
      }
     }
    }
    while((!reaval_objetos && XMLParser::accessElement(XMLParser::NEXT_ELEMENT)) ||
          (reaval_objetos));
   }

   /* Verificação final, caso hajam erros pendentes na lista de erros capturados durante o processo
      de leitura do arquivo os mesmo serão redirecionados e o processo de leitura do arquivo
      cancelado */
   if(vet_erros.size() > 0)
    throw Exception(vet_erros[0].getErrorMessage(),vet_erros[0].getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Protege o modelo com base no atributo obtido do xml
   this->BaseObject::setProtected(modelo_protegido);
   carregando_modelo=false;

   /* Faz uma última validação nos relacionamentos para abranger qualquer modificação de colunas
     não feitas durante o carregamento */
   this->validarRelacionamentos();
  }
  catch(Exception &e)
  {
   QString info_adicional;
   carregando_modelo=false;
   destruirObjetos();

   if(XMLParser::getCurrentElement())
    info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename()).arg(XMLParser::getCurrentElement()->line);

   //Caso o erro seja na biblioteca de parsers
   if(e.getErrorType()>=ERR_INVALID_SYNTAX)
   {
    str_aux=QString(Exception::getErrorMessage(ERR_LOAD_INV_MODEL_FILE)).arg(nome_arq);
    throw Exception(str_aux,ERR_LOAD_INV_MODEL_FILE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
   }
   else
    //Captura e redireciona erros das demais bibliotecas
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
  }
 }
}

ObjectType ModeloBD::getObjectType(const QString &str_tipo)
{
 ObjectType tipo_obj=BASE_OBJECT;
 int i;

 for(i=0; i < BaseObject::OBJECT_TYPE_COUNT; i++)
 {
  if(objs_schemas[i]==str_tipo)
  {
   tipo_obj=static_cast<ObjectType>(i);
   break;
  }
 }

 return(tipo_obj);
}

BaseObject *ModeloBD::criarObjeto(ObjectType tipo_obj)
{
 BaseObject *objeto=NULL;

 if(tipo_obj!=BASE_OBJECT)
 {
  if(tipo_obj==OBJ_ROLE)
   objeto=criarPapel();
  else if(tipo_obj==OBJ_TABLESPACE)
   objeto=criarEspacoTabela();
  else if(tipo_obj==OBJ_SCHEMA)
   objeto=criarEsquema();
  else if(tipo_obj==OBJ_LANGUAGE)
   objeto=criarLinguagem();
  else if(tipo_obj==OBJ_FUNCTION)
   objeto=criarFuncao();
  else if(tipo_obj==OBJ_TYPE)
   objeto=criarTipo();
  else if(tipo_obj==OBJ_DOMAIN)
   objeto=criarDominio();
  else if(tipo_obj==OBJ_CAST)
   objeto=criarConversaoTipo();
  else if(tipo_obj==OBJ_CONVERSION)
   objeto=criarConversaoCodificacao();
  else if(tipo_obj==OBJ_OPERATOR)
   objeto=criarOperador();
  else if(tipo_obj==OBJ_OPFAMILY)
   objeto=criarFamiliaOperadores();
  else if(tipo_obj==OBJ_OPCLASS)
   objeto=criarClasseOperadores();
  else if(tipo_obj==OBJ_AGGREGATE)
   objeto=criarFuncaoAgregacao();
  else if(tipo_obj==OBJ_TABLE)
   objeto=criarTabela();
  else if(tipo_obj==OBJ_SEQUENCE)
   objeto=criarSequencia();
  else if(tipo_obj==OBJ_VIEW)
   objeto=criarVisao();
  else if(tipo_obj==OBJ_TEXTBOX)
   objeto=criarCaixaTexto();
  else if(tipo_obj==OBJ_CONSTRAINT)
   objeto=criarRestricao(NULL);
  else if(tipo_obj==OBJ_TRIGGER)
   objeto=criarGatilho(NULL);
  else if(tipo_obj==OBJ_INDEX)
   objeto=criarIndice(NULL);
  else if(tipo_obj==OBJ_RELATIONSHIP ||
          tipo_obj==BASE_RELATIONSHIP)
   objeto=criarRelacionamento();
 }

 return(objeto);
}

void ModeloBD::definirAtributosBasicos(BaseObject *objeto)
{
 map<QString, QString> atributos, atribs_aux;
 QString nome_elem;//, str_aux;
 BaseObject *esp_tabela=NULL, *dono=NULL;
 Schema *esquema=NULL;
 ObjectType tipo_obj=BASE_OBJECT, tipo_obj_aux;
 bool erro=false, protegido=false;

 //Caso o objeto não esteja alocado uma exceção é disparada
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém os atributos do elemento
 XMLParser::getElementAttributes(atributos);

 tipo_obj_aux=objeto->getObjectType();
 if(tipo_obj_aux!=OBJ_CAST)
  objeto->setName(atributos[ParsersAttributes::NAME]);

 //Definindo se o objeto está protegido ou não
 protegido=atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_;

 XMLParser::savePosition();

 //Passa para os elementos filhos que provavelmente serão <onwer>, <tablespace> e <comment>
 if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
 {
  do
  {
   /* Certificando que só elementos xml serão lidos do parser, qualquer outro
      tipo de objeto xml será ignorado */
   if(XMLParser::getElementType()==XML_ELEMENT_NODE)
   {
    //Obtém o nome do elemento filho
    nome_elem=XMLParser::getElementName();

    //Caso o elemento filho seja um comentáio <comment>
    if(nome_elem==ParsersAttributes::COMMENT)
    {
     /* Para se extraír o comentário, é necessário salvar a posição de navegação
        do parser, pois o conteúdo do comentário é um elemento filho do elemento
        atual do parser XML */
     XMLParser::savePosition();
     //Acessa o elemento filho o qual contém o conteúdo do comentário
     XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
     //Atributo o conteúdo do elemento filho ao atributo cometário do novo papel
     objeto->setComment(XMLParser::getElementContent());
     //Restaura a posição de navegação do parser, ou seja, volta para o elemento <comment>
     XMLParser::restorePosition();
    }
    //Caso o elemento filho seja uma referência a um esquema <schema>
    else if(nome_elem==ParsersAttributes::SCHEMA)
    {
     tipo_obj=OBJ_SCHEMA;
     XMLParser::getElementAttributes(atribs_aux);
     esquema=dynamic_cast<Schema *>(obterObjeto(atribs_aux[ParsersAttributes::NAME], tipo_obj));
     objeto->setSchema(esquema);
     erro=(!esquema && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um tablespace <tablespace>
    else if(nome_elem==ParsersAttributes::TABLESPACE)
    {
     tipo_obj=OBJ_TABLESPACE;
     XMLParser::getElementAttributes(atribs_aux);
     esp_tabela=obterObjeto(atribs_aux[ParsersAttributes::NAME], tipo_obj);
     objeto->setTablespace(esp_tabela);
     erro=(!esp_tabela && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um dono (role/papel) <role>
    else if(nome_elem==ParsersAttributes::ROLE)
    {
     tipo_obj=OBJ_ROLE;
     XMLParser::getElementAttributes(atribs_aux);
     dono=obterObjeto(atribs_aux[ParsersAttributes::NAME], tipo_obj);
     objeto->setOwner(dono);
     erro=(!dono && !atribs_aux[ParsersAttributes::NAME].isEmpty());
    }
    //Obténdo o atributo a posição do objeto (apenas para objetos gráficos)
    else if(nome_elem==ParsersAttributes::POSITION)
    {
     XMLParser::getElementAttributes(atributos);

     if(nome_elem==ParsersAttributes::POSITION &&
        (tipo_obj_aux!=OBJ_RELATIONSHIP &&
         tipo_obj_aux!=BASE_RELATIONSHIP))
     {
      dynamic_cast<BaseGraphicObject *>(objeto)->setPosition(QPointF(atributos[ParsersAttributes::X_POS].toFloat(),
                                                                       atributos[ParsersAttributes::Y_POS].toFloat()));


     }
    }
   }
  }
  /* A extração de elementos continuará até que se chegue no último elemento
     filho do elemento */
  while(!erro && XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
 }

 XMLParser::restorePosition();
 objeto->setProtected(protegido);

 if(erro)
 {
  /* Caso o flag de erro esteja marcado indica que o novo objeto
     está referenciando um outro objeto o qual não existe no modelo */

  //Dispara a exceção
  throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                   .arg(QString::fromUtf8(objeto->getName()))
                   .arg(objeto->getTypeName())
                   .arg(QString::fromUtf8(atribs_aux[ParsersAttributes::NAME]))
                   .arg(BaseObject::getTypeName(tipo_obj)),
                ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 //Caso o objeto não esteja atribuído a nenhum esquema gera um erro
 else if(!objeto->getSchema() &&
         (tipo_obj_aux==OBJ_FUNCTION || tipo_obj_aux==OBJ_TABLE ||
          tipo_obj_aux==OBJ_VIEW  || tipo_obj_aux==OBJ_DOMAIN ||
          tipo_obj_aux==OBJ_AGGREGATE || tipo_obj_aux==OBJ_OPERATOR ||
          tipo_obj_aux==OBJ_SEQUENCE || tipo_obj_aux==OBJ_CONVERSION ||
          tipo_obj_aux==OBJ_TYPE || tipo_obj_aux==OBJ_OPFAMILY ||
          tipo_obj_aux==OBJ_OPCLASS))
 {
  throw Exception(Exception::getErrorMessage(ERR_ALOC_OBJECT_NO_SCHEMA)
                        .arg(QString::fromUtf8(objeto->getName()))
                        .arg(objeto->getTypeName()),
          ERR_ALOC_OBJECT_NO_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

Papel *ModeloBD::criarPapel(void)
{
 map<QString, QString> atributos, atribs_aux;
 Papel *papel=NULL, *papel_ref=NULL;
 int i, tam;
 bool marcado;
 QStringList lista;
 QString nome_elem;
 unsigned tipo_papel;

 /* Os elementos dos dois elementos a seguir deve aparecer na
    mesma ordem pois seus valores serão trabalhados em conjunto
    na interação onde ser precisa identificar as opções do papel
    o qual está sendo criado */
 QString atrib_ops[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                       ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                       ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED };

 unsigned vet_ops[]={ Papel::OP_SUPERUSER, Papel::OP_CREATEDB,
                      Papel::OP_CREATEROLE, Papel::OP_INHERIT,
                      Papel::OP_LOGIN, Papel::OP_ENCRYPTED };

 try
 {
  //Aloca no novo papel
  papel=new Papel;
  definirAtributosBasicos(papel);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do papel
  papel->definirSenha(atributos[ParsersAttributes::PASSWORD]);
  papel->definirValidade(atributos[ParsersAttributes::VALIDITY]);

  /* Caso o atributo de id de usuário esteja atribuído no xml.
     (atributos[AtributosParsers::UID] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[ParsersAttributes::SYSID].isEmpty())
   papel->definirSysid(atributos[ParsersAttributes::SYSID].toInt());

  /* Caso o atributo de limite de conexão esteja atribuído no xml.
     (atributos[AtributosParsers::LIMITE_CONEXAO] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[ParsersAttributes::CONN_LIMIT].isEmpty())
   papel->definirLimiteConexao(atributos[ParsersAttributes::CONN_LIMIT].toInt());

  /* Identificando as opções do papel. Caso o atributo referet   uma
     estive com valor "true" no documento XML quer dizer que aquele
     atributo está marcado para o papel */
  for(i=0; i < 6; i++)
  {
   //Verifica se a opção está marcada no XML, valor de atributo = true
   marcado=atributos[atrib_ops[i]]==ParsersAttributes::_TRUE_;
   papel->definirOpcao(vet_ops[i], marcado);
  }

  //Passa para os elementos filhos que provavelmente serão <roles> e <comment>
  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser, qualquer outro
       tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     //Obtém o nome do elemento filho
     nome_elem=XMLParser::getElementName();

     //Caso o elemento filho seja uma lista de papeis <roles>
     if(nome_elem==ParsersAttributes::ROLES)
     {
      //Obtém os atributos do elemento <roles>, neste caso são names e reftype
      XMLParser::getElementAttributes(atribs_aux);

      /* O atributo names armazena uma lista de nomes de papéis as quais o novo papel
         referenciará. A lista tem os elementos separados por vírgula, sendo assim a
         string será quebrada usando o delimitador ',') */
      lista=atribs_aux[ParsersAttributes::NAMES].split(',');

      //Obtém a quantidade de nomes de papéis na lista
      tam=lista.size();

      /* Identificando o tipo da lista de papéis a qual será inserido os objetos
         cujos nomes foram extraídos acima */
      if(atribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::REFER)
       tipo_papel=Papel::PAPEL_REF;
      else if(atribs_aux[ParsersAttributes::ROLE_TYPE]==ParsersAttributes::MEMBER)
       tipo_papel=Papel::PAPEL_MEMBRO;
      else
       tipo_papel=Papel::PAPEL_ADMIN;

      //Varre a lista de nomes de papéis
      for(i=0; i < tam; i++)
      {
       //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
       papel_ref=dynamic_cast<Papel *>(obterObjeto(lista[i].trimmed(),OBJ_ROLE));

       /* Caso esse papel não exista um erro será disparado pois um novo papel
          não pode referenciar um outro papel que ainda nem foi criado */
       if(!papel_ref)
       {
        //Dispara a exceção
        throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(papel->getName()))
                               .arg(BaseObject::getTypeName(OBJ_ROLE))
                               .arg(QString::fromUtf8(lista[i]))
                               .arg(BaseObject::getTypeName(OBJ_ROLE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       //Caso o papel exista no modelo, o mesmo será relacionado ao novo papel
       papel->definirPapel(tipo_papel, papel_ref);
      }
     }
    }
   }
   /* A extração de elementos continuará até que se chegue no último elemento
      filho do elemento <role> */
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o papel alocado
  if(papel) delete(papel);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Retorna o novo papel criado
 return(papel);
}

Tablespace *ModeloBD::criarEspacoTabela(void)
{
 map<QString, QString> atributos;
 Tablespace *esp_tabela=NULL;

 try
 {
  //Aloca no novo espaço de tabelas
  esp_tabela=new Tablespace;
  definirAtributosBasicos(esp_tabela);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do papel
  esp_tabela->setDirectory(atributos[ParsersAttributes::DIRECTORY]);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(esp_tabela) delete(esp_tabela);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esp_tabela);
}

Schema *ModeloBD::criarEsquema(void)
{
 map<QString, QString> atributos;
 Schema *esquema=NULL;

 try
 {
  //Aloca no novo esquema
  esquema=new Schema;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(esquema);
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(esquema) delete(esquema);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esquema);
}

Language *ModeloBD::criarLinguagem(void)
{
 map<QString, QString> atributos;
 Language *linguagem=NULL;
 BaseObject *funcao=NULL;
 QString assinatura, tipo_ref;
 ObjectType tipo_obj;

 try
 {
  linguagem=new Language;

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  definirAtributosBasicos(linguagem);

  linguagem->setTrusted(atributos[ParsersAttributes::TRUSTED]==
                               ParsersAttributes::_TRUE_);

   if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
   {
    do
    {
     /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
     if(XMLParser::getElementType()==XML_ELEMENT_NODE)
     {
      tipo_obj=getObjectType(XMLParser::getElementName());

      if(tipo_obj==OBJ_FUNCTION)
      {
       XMLParser::getElementAttributes(atributos);
       //Obtém o tipo de referência da função
       tipo_ref=atributos[ParsersAttributes::REF_TYPE];

       //Caso seja uma função handler ou validator
       if(tipo_ref==ParsersAttributes::VALIDATOR_FUNC ||
          tipo_ref==ParsersAttributes::HANDLER_FUNC ||
          tipo_ref==ParsersAttributes::INLINE_FUNC)
       {
        //Obtém a assinatura da função
        assinatura=atributos[ParsersAttributes::SIGNATURE];

        //Obtém a função do modelo
        funcao=obterObjeto(assinatura, OBJ_FUNCTION);

        //Caso a função não seja encontrada
        if(!funcao)
         //Dispara a exceção indicando que o objeto está incompleto
         throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                               .arg(QString::fromUtf8(linguagem->getName()))
                               .arg(linguagem->getTypeName())
                               .arg(QString::fromUtf8(assinatura))
                               .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                       ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

        if(tipo_ref==ParsersAttributes::VALIDATOR_FUNC)

         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::VALIDATOR_FUNC);
        else if(tipo_ref==ParsersAttributes::HANDLER_FUNC)
         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::HANDLER_FUNC);
        else
         linguagem->setFunction(dynamic_cast<Function *>(funcao), Language::INLINE_FUNC);

       }
       else
        //Dispara uma exceção caso o tipo de referencia a função seja inválido
        throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
     }
    }
    while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
   }

 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove a linguagem alocada
  if(linguagem) delete(linguagem);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(linguagem);
}

Function *ModeloBD::criarFuncao(void)
{
 map<QString, QString> atributos, atrib_aux;
 Function *funcao=NULL;
 ObjectType tipo_obj;
 BaseObject *objeto=NULL;
 TipoPgSQL tipo;
 Parameter param;
 QString str_aux, elem;

 try
 {
  funcao=new Function;
  //Lê do parser os atributos basicos
  definirAtributosBasicos(funcao);

  //Obtém os atributos da função
  XMLParser::getElementAttributes(atributos);

  //Define se a função retorna setof, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::RETURNS_SETOF].isEmpty())
   funcao->setReturnSetOf(atributos[ParsersAttributes::RETURNS_SETOF]==
                               ParsersAttributes::_TRUE_);

  //Define se a função é do tipo janela, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::WINDOW_FUNC].isEmpty())
   funcao->setWindowFunction(atributos[ParsersAttributes::WINDOW_FUNC]==
                               ParsersAttributes::_TRUE_);

  //Define a configuração de retorno da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::BEHAVIOR_TYPE].isEmpty())
   funcao->setBehaviorType(TipoComportamento(atributos[ParsersAttributes::BEHAVIOR_TYPE]));

  //Define o tipo da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::FUNCTION_TYPE].isEmpty())
   funcao->setFunctionType(TipoFuncao(atributos[ParsersAttributes::FUNCTION_TYPE]));

  //Define o tipo de segurança da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::SECURITY_TYPE].isEmpty())
   funcao->setSecurityType(TipoSeguranca(atributos[ParsersAttributes::SECURITY_TYPE]));

  //Define o custo de execução da função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::EXECUTION_COST].isEmpty())
   funcao->setExecutionCost(atributos[ParsersAttributes::EXECUTION_COST].toInt());

  //Define a quantidade de linhas retornadas pela função, caso o atributo esteja marcado no XML
  if(!atributos[ParsersAttributes::ROW_AMOUNT].isEmpty())
   funcao->setRowAmount(atributos[ParsersAttributes::ROW_AMOUNT].toInt());

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();
     tipo_obj=getObjectType(elem);

     //Caso o parser acesso a tag que determina o tipo de retorno da função
     if(elem==ParsersAttributes::RETURN_TYPE)
     {
      XMLParser::savePosition();

      try
      {
       //Acesso os elementos filhos os quais podem ser um <type> ou vários <parameter>
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

       do
       {
        if(XMLParser::getElementType()==XML_ELEMENT_NODE)
        {
         /* Caso o elemento atual no parser seja um <type>, indica que
          será extraído o tipo de retorno da função */
         if(XMLParser::getElementName()==ParsersAttributes::TYPE)
         {
          //Cria o tipo
          tipo=criarTipoPgSQL();
          //Atribui ao retorno da função
          funcao->setReturnType(tipo);
         }
         /* Criação dos tipo de retorno de tabela da função. Os mesmos vem descritos
            dentro da tag <return-type> em forma de parâmetros */
         else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
         {
          param=criarParametro();
          //Adiciona o tipo de retorno   função
          funcao->addReturnedTableColumn(param.getName(), param.getType());
         }
        }
       }
       while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

       XMLParser::restorePosition();
      }
      catch(Exception &e)
      {
       XMLParser::restorePosition();
       throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
      }
     }
     //Obtendo a linguagem da função
     else if(tipo_obj==OBJ_LANGUAGE)
     {
      //Obtém seus atributos
      XMLParser::getElementAttributes(atributos);

      //Busca a linguagem no modelo
      objeto=obterObjeto(atributos[ParsersAttributes::NAME], tipo_obj);

      //Caso a linguagem não existe será disparada uma exceção
      if(!objeto)
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(funcao->getName()))
                              .arg(funcao->getTypeName())
                              .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                              .arg(BaseObject::getTypeName(OBJ_LANGUAGE)),
                      ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a linguagem da função
      funcao->setLanguage(dynamic_cast<Language *>(objeto));
     }
     else if(XMLParser::getElementName()==ParsersAttributes::PARAMETER)
     {
      param=criarParametro();
      //Adiciona o parâmet  função
      funcao->addParameter(param);
     }
     //Extraíndo a definição (corpo) da função (tag <definition>)
     else if(XMLParser::getElementName()==ParsersAttributes::DEFINITION)
     {
      XMLParser::savePosition();

      //Obtém os atributos da biblioteca
      XMLParser::getElementAttributes(atrib_aux);

      if(!atrib_aux[ParsersAttributes::LIBRARY].isEmpty())
      {
       funcao->setLibrary(atrib_aux[ParsersAttributes::LIBRARY]);
       funcao->setSymbol(atrib_aux[ParsersAttributes::SYMBOL]);
      }
      /* Para se ter acesso ao código que define a função é preciso acessar
         o filho da tag <definition> e obter seu conteúdo */
      else if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
       funcao->setSourceCode(XMLParser::getElementContent());

      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o espaco de tabela alocado
  if(funcao)
  {
   str_aux=funcao->getName(true);
   delete(funcao);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
                              .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(funcao);
}

Parameter ModeloBD::criarParametro(void)
{
 TipoPgSQL tipo;
 Parameter param;
 map<QString, QString> atributos;
 QString elem;

 try
 {
  //Salva a posição do parser
  XMLParser::savePosition();
  //Obtem os atributos do parâmetro (nome, in e out)
  XMLParser::getElementAttributes(atributos);

  param.setName(atributos[ParsersAttributes::NAME]);
  /* Configurando atributos in e out do parâmetro caso estes estejam
     definidos como true no XML */
  param.setIn(atributos[ParsersAttributes::PARAM_IN]==ParsersAttributes::_TRUE_);
  param.setOut(atributos[ParsersAttributes::PARAM_OUT]==ParsersAttributes::_TRUE_);
  param.setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

  //Acessa os elementos filhos do parâmetro, que no caso será apenas <type> ou <domain>
  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::TYPE)
     {
      param.setType(criarTipoPgSQL());
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  //Restaura a posição do parser
  XMLParser::restorePosition();
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Restaura a posição do parser
  XMLParser::restorePosition();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(param);
}

TipoPgSQL ModeloBD::criarTipoPgSQL(void)
{
 map<QString, QString> atributos;
 vector<void *> vet_ptipos;
 vector<void *>::iterator itr, itr_end;
 unsigned comprimento=1, dimensao=0, idx_tipo=0;
 int precisao=-1;
 QString nome;
 Tipo *tipo_usr=NULL;
 void *ptipo=NULL;
 //TipoPgSQL tipo;
 bool enc=false, com_timezone;
 TipoIntervalo tipo_interv;
 TipoEspacial tipo_esp;


 //Obtém os atributos do tipo
 XMLParser::getElementAttributes(atributos);

 if(!atributos[ParsersAttributes::LENGTH].isEmpty())
  comprimento=atributos[ParsersAttributes::LENGTH].toUInt();

 if(!atributos[ParsersAttributes::DIMENSION].isEmpty())
  dimensao=atributos[ParsersAttributes::DIMENSION].toUInt();

 if(!atributos[ParsersAttributes::PRECISION].isEmpty())
  precisao=atributos[ParsersAttributes::PRECISION].toInt();

 com_timezone=(atributos[ParsersAttributes::WITH_TIMEZONE]==ParsersAttributes::_TRUE_);
 tipo_interv=atributos[ParsersAttributes::INTERVAL_TYPE];

 if(!atributos[ParsersAttributes::SPATIAL_TYPE].isEmpty())
  tipo_esp=TipoEspacial(atributos[ParsersAttributes::SPATIAL_TYPE],
                        atributos[ParsersAttributes::VARIATION].toUInt());

 nome=atributos[ParsersAttributes::NAME];

 idx_tipo=TipoPgSQL::obterIndiceTipoBase(nome);
 if(idx_tipo!=TipoPgSQL::nulo)
 {
  return(TipoPgSQL(nome,comprimento,dimensao,precisao,com_timezone,tipo_interv, tipo_esp));
 }
 else
 {
  //Obtém a lista de tipos definidios pelo usuario
  TipoPgSQL::obterTiposUsuario(vet_ptipos, this,
                               ConfigTipoUsuario::TIPO_BASE |
                               ConfigTipoUsuario::TIPO_DOMINIO |
                               ConfigTipoUsuario::TIPO_TABELA |
                               ConfigTipoUsuario::TIPO_SEQUENCIA);
  itr=vet_ptipos.begin();
  itr_end=vet_ptipos.end();

  //Faz uma varredura na lista de tipos
  while(itr!=itr_end && !enc)
  {
   //Converte o elemento atual para a classe Tipo
   ptipo=(*itr);
   tipo_usr=reinterpret_cast<Tipo *>(ptipo);

   //Caso o nome passado no XML for igual ao nome do objeto Tipo atual
   enc=(tipo_usr->getName(true)==nome);
   itr++;
  }

  /* Caso o tipo não foi encontrado na lista de tipos quer dizer existe
     a tentativa de se referenciar um tipo definido pelo usuário sem que o
     mesmo exista no modelo, sendo assim, um erro será disparado e
     a criação do tipo será abortada */
  if(!enc)
    throw Exception(ERR_REF_INEXIST_USER_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  idx_tipo=TipoPgSQL::obterIndiceTipoUsuario(nome, ptipo);
  return(TipoPgSQL(idx_tipo,comprimento,dimensao,precisao,com_timezone,tipo_interv,tipo_esp));
 }
}

Tipo *ModeloBD::criarTipo(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 Tipo *tipo=NULL;
 int qtd, i;
 QStringList enums;
 QString elem, str_aux;
 Parameter param;
 BaseObject *funcao=NULL;
 unsigned tipo_func=0;
 TipoPgSQL tipo_copia;

 try
 {
  tipo=new Tipo;
  //Lê do parser os atributos basicos
  definirAtributosBasicos(tipo);

  //Obtém os atributos do tipo
  XMLParser::getElementAttributes(atributos);

  //Define a configuração do tipo
  if(atributos[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE)
  {
   tipo->definirConfiguracao(Tipo::TIPO_BASE);

   //Definindos os atributos específicos para tipo base

   //Definindo se o tipo é passado por valor ou não
   tipo->definirPorValor(atributos[ParsersAttributes::BY_VALUE]==ParsersAttributes::_TRUE_);

   //Definindo o comprimento interno do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::INTERNAL_LENGHT].isEmpty())
    tipo->definirCompInterno(atributos[ParsersAttributes::INTERNAL_LENGHT].toUInt());

   //Definindo o alinhamento interno do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::ALIGNMENT].isEmpty())
    tipo->definirAlinhamento(atributos[ParsersAttributes::ALIGNMENT]);

   //Definindo o tipo de armazenamento do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::STORAGE].isEmpty())
    tipo->definirArmazenamento(atributos[ParsersAttributes::STORAGE]);

   //Definindo o elemento do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::ELEMENT].isEmpty())
    tipo->definirElemento(atributos[ParsersAttributes::ELEMENT]);

   //Definindo o delimitador do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::DELIMITER].isEmpty())
    tipo->definirDelimitador(atributos[ParsersAttributes::DELIMITER][0].toAscii());

   //Definindo o valor padrão do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::DEFAULT_VALUE].isEmpty())
    tipo->definirValorPadrao(atributos[ParsersAttributes::DEFAULT_VALUE]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::CATEGORY].isEmpty())
    tipo->definirCategoria(atributos[ParsersAttributes::CATEGORY]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[ParsersAttributes::PREFERRED].isEmpty())
    tipo->definirPreferido(atributos[ParsersAttributes::PREFERRED]==ParsersAttributes::_TRUE_);


   /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
   tipo_funcoes[ParsersAttributes::INPUT_FUNC]=Tipo::FUNCAO_INPUT;
   tipo_funcoes[ParsersAttributes::OUTPUT_FUNC]=Tipo::FUNCAO_OUTPUT;
   tipo_funcoes[ParsersAttributes::SEND_FUNC]=Tipo::FUNCAO_SEND;
   tipo_funcoes[ParsersAttributes::RECV_FUNC]=Tipo::FUNCAO_RECV;
   tipo_funcoes[ParsersAttributes::TPMOD_IN_FUNC]=Tipo::FUNCAO_TPMOD_IN;
   tipo_funcoes[ParsersAttributes::TPMOD_OUT_FUNC]=Tipo::FUNCAO_TPMOD_OUT;
   tipo_funcoes[ParsersAttributes::ANALYZE_FUNC]=Tipo::FUNCAO_ANALYZE;
  }
  else if(atributos[ParsersAttributes::CONFIGURATION]==ParsersAttributes::COMPOSITE_TYPE)
   tipo->definirConfiguracao(Tipo::TIPO_COMPOSTO);
  else
   tipo->definirConfiguracao(Tipo::TIPO_ENUMERACAO);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     //Operação específica para tipo ENUM
     if(elem==ParsersAttributes::ENUM_TYPE)
     {
      //Obtém o atributo da tag <enumerations>
      XMLParser::getElementAttributes(atributos);
      /* Como se trata de uma lista de enumerações separadas por vírgulas
         a mesma será quebrada e transformada num vetor */
      enums=atributos[ParsersAttributes::VALUES].split(",");

      //Adiciona ao tipo todas as enumerações presentes no vetor
      qtd=enums.size();
      for(i=0; i < qtd; i++)
       tipo->adicionarEnumeracao(enums[i]);
     }
     //Operação específica para tipo COMPOSTO
     else if(elem==ParsersAttributes::PARAMETER)
     {
      /* No caso de tipo composto, o mesmo possui indefinida quatidade
         de elementos <parameter> os quais simbolizam os atributos do
         tipo */
      param=criarParametro();
      tipo->adicionarAtributo(param);
     }
     //Operação específica para tipo BASE
     else if(elem==ParsersAttributes::TYPE)
     {
      tipo_copia=criarTipoPgSQL();
      tipo->definirTipoCopia(tipo_copia);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso de tipo base, serão extraídas referência a funções do modelo,
        as quais serão atribuía  s funções que compoem o tipo base. */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida di XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      /* Dispara uma exceção caso o tipo de referencia a função seja inválido ou
         se a função referenciada não existe */
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                              .arg(QString::fromUtf8(tipo->getName()))
                              .arg(tipo->getTypeName())
                              .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                              .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      else if(tipo_funcoes.count(atributos[ParsersAttributes::REF_TYPE])==0)
       throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      tipo->definirFuncao(tipo_func, dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o tipo alocado
  if(tipo)
  {
   str_aux=tipo->getName(true);
   delete(tipo);
  }

  //Redireciona qualquer exceção capturada
  if(e.getErrorType()==ERR_REF_INEXIST_USER_TYPE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(str_aux))
                              .arg(tipo->getTypeName()),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tipo);
}

Domain *ModeloBD::criarDominio(void)
{
 map<QString, QString> atributos;
 Domain *dominio=NULL;
 QString elem;

 try
 {
  dominio=new Domain;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(dominio);

  //Obtém os atributos do domíno
  XMLParser::getElementAttributes(atributos);

  if(!atributos[ParsersAttributes::CONSTRAINT].isEmpty())
   dominio->setConstraintName(atributos[ParsersAttributes::CONSTRAINT]);

  if(!atributos[ParsersAttributes::DEFAULT_VALUE].isEmpty())
   dominio->setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

  dominio->setNotNull(atributos[ParsersAttributes::NOT_NULL]==
                            ParsersAttributes::_TRUE_);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo ao qual o domínio se aplica */
     if(elem==ParsersAttributes::TYPE)
     {
      dominio->setType(criarTipoPgSQL());
     }
     //Caso o elemento seja uma expressão
     else if(elem==ParsersAttributes::EXPRESSION)
     {
      /* Para se extraír a expressão, é necessário salvar a posição de navegação
         do parser, pois o conteúdo da mesma é um elemento filho do elemento
         atual do parser XML */
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //dominio->definirExpressao(QString::fromUtf8(ParserXML::obterConteudoElemento()));
      dominio->setExpression(XMLParser::getElementContent());
      //Restaura a posição de navegação do parser, ou seja, volta para o elemento <expression>
      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  //Remove o domínio alocado
  if(dominio) delete(dominio);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(dominio);
}

Cast *ModeloBD::criarConversaoTipo(void)
{
 map<QString, QString> atributos;
 Cast *conv_tipo=NULL;
 QString elem;
 unsigned idx_tipo=0;
 TipoPgSQL tipo;
 BaseObject *funcao=NULL;

 try
 {
  conv_tipo=new Cast;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(conv_tipo);

  //Obtém os atributos do domíno
  XMLParser::getElementAttributes(atributos);

  if(atributos[ParsersAttributes::CAST_TYPE]==
      ParsersAttributes::IMPLICIT)
   conv_tipo->setCastType(Cast::IMPLICIT);
  else
   conv_tipo->setCastType(Cast::ASSIGNMENT);

  conv_tipo->setInOut(atributos[ParsersAttributes::IO_CAST]==ParsersAttributes::_TRUE_);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo (de origem ou destino) da conversao */
     if(elem==ParsersAttributes::TYPE)
     {
      tipo=criarTipoPgSQL();
      if(idx_tipo==0)
       conv_tipo->setDataType(Cast::SRC_TYPE, tipo);
      else
       conv_tipo->setDataType(Cast::DST_TYPE, tipo);
      idx_tipo++;
     }
     //Extraíndo a função de conversão do XML
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso da conversão, será extraída a refeênia   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribud   conversão */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(conv_tipo->getName()))
                             .arg(conv_tipo->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
      conv_tipo->setCastFunction(dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(conv_tipo) delete(conv_tipo);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_tipo);
}

Conversion *ModeloBD::criarConversaoCodificacao(void)
{
 map<QString, QString> atributos;
 Conversion *conv_codif=NULL;
 QString elem;
 BaseObject *funcao=NULL;

 try
 {
  conv_codif=new Conversion;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(conv_codif);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  conv_codif->setEncoding(Conversion::SRC_ENCODING,
                                 TipoCodificacao(atributos[ParsersAttributes::SRC_ENCODING]));

  conv_codif->setEncoding(Conversion::DST_ENCODING,
                                 TipoCodificacao(atributos[ParsersAttributes::DST_ENCODING]));

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso da conversão, será extraída a refeênia   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribud   conversão */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(conv_codif->getName()))
                             .arg(conv_codif->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a funç   conversão de tipos
      conv_codif->setConversionFunction(dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(conv_codif) delete(conv_codif);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_codif);
}

Operator *ModeloBD::criarOperador(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 map<QString, unsigned> tipo_operadores;
 Operator *operador=NULL;
 QString elem;
 BaseObject *funcao=NULL,*oper_aux=NULL;
 unsigned tipo_func, tipo_oper, tipo_arg;
 TipoPgSQL tipo;

 try
 {
  operador=new Operator;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(operador);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  operador->definirMerges(atributos[ParsersAttributes::MERGES]==ParsersAttributes::_TRUE_);
  operador->definirHashes(atributos[ParsersAttributes::HASHES]==ParsersAttributes::_TRUE_);

  /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
  tipo_funcoes[ParsersAttributes::OPERATOR_FUNC]=Operator::FUNC_OPERADOR;
  tipo_funcoes[ParsersAttributes::JOIN_FUNC]=Operator::FUNC_JUNCAO;
  tipo_funcoes[ParsersAttributes::RESTRICTION_FUNC]=Operator::FUNC_RESTRICAO;

  /* O mapa de tipos de operadores abaixo é usado para se atribuir de forma
      mais simples, sem comparações, o operador que for obtida do XML a qual
      o operador em construção referencia */
  tipo_operadores[ParsersAttributes::COMMUTATOR_OP]=Operator::OPER_COMUTACAO;
  tipo_operadores[ParsersAttributes::GREATER_OP]=Operator::OPER_MAIOR;
  tipo_operadores[ParsersAttributes::LESS_OP]=Operator::OPER_MENOR;
  tipo_operadores[ParsersAttributes::NEGATOR_OP]=Operator::OPER_NEGACAO;
  tipo_operadores[ParsersAttributes::SORT_OP]=Operator::OPER_ORDENACAO1;
  tipo_operadores[ParsersAttributes::SORT2_OP]=Operator::OPER_ORDENACAO2;

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==objs_schemas[OBJ_OPERATOR])
     {
      //Obtém os atributos do operador referenciado
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      oper_aux=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_OPERATOR);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!oper_aux && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(operador->obterAssinatura(true)))
                             .arg(operador->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_OPERATOR)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_oper=tipo_operadores[atributos[ParsersAttributes::REF_TYPE]];
      operador->definirOperador(dynamic_cast<Operator *>(oper_aux),tipo_oper);
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      /* Obtém os atributos do tipo para saber se o mesmo é um tipo da
         esquerda ou da direita */
      XMLParser::getElementAttributes(atributos);

      //Obtém o tipo de referência do tipo base (esquerda ou direita)
      if(atributos[ParsersAttributes::REF_TYPE]!=ParsersAttributes::RIGHT_TYPE)
       tipo_arg=Operator::ARG_ESQUERDA;
      else
       tipo_arg=Operator::ARG_DIREITA;

      tipo=criarTipoPgSQL();
      operador->definirTipoDadoArgumento(tipo, tipo_arg);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      /*No caso do operador, será extraída a refernca   função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída ao operador */
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(operador->getName()))
                             .arg(operador->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[ParsersAttributes::REF_TYPE]];

      //Atribui a função ao tipo na configuração obtida
      operador->definirFuncao(dynamic_cast<Function *>(funcao), tipo_func);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(operador) delete(operador);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(operador);
}

OperatorClass *ModeloBD::criarClasseOperadores(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipos_elem;
 BaseObject *objeto=NULL;
 QString elem;
 TipoPgSQL tipo;
 OperatorClass *classe_op=NULL;
 OperatorClassElement elem_classe;
 bool rechecar;
 unsigned num_estrategia, tp_elem;

 try
 {
  classe_op=new OperatorClass;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(classe_op);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);

  classe_op->setIndexingType(TipoIndexacao(atributos[ParsersAttributes::INDEX_TYPE]));
  classe_op->setDefault(atributos[ParsersAttributes::DEFAULT]==ParsersAttributes::_TRUE_);

  tipos_elem[ParsersAttributes::FUNCTION]=OperatorClassElement::FUNCTION_ELEM;
  tipos_elem[ParsersAttributes::OPERATOR]=OperatorClassElement::OPERATOR_ELEM;
  tipos_elem[ParsersAttributes::STORAGE]=OperatorClassElement::STORAGE_ELEM;

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==objs_schemas[OBJ_OPFAMILY])
     {
      //Obtém os atributos do operador referenciado
      XMLParser::getElementAttributes(atributos);

      /* Com o nome da família do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe um objeto correspondente */
      objeto=obterObjeto(atributos[ParsersAttributes::NAME], OBJ_OPFAMILY);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!objeto && !atributos[ParsersAttributes::NAME].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(classe_op->getName()))
                             .arg(classe_op->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(OBJ_OPFAMILY)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      classe_op->setFamily(dynamic_cast<OperatorFamily *>(objeto));
     }
     else if(elem==ParsersAttributes::TYPE)
     {
      //Obtém os atributos do tipo
      XMLParser::getElementAttributes(atributos);
      tipo=criarTipoPgSQL();
      classe_op->setDataType(tipo);
     }
     else if(elem==ParsersAttributes::ELEMENT)
     {
      XMLParser::getElementAttributes(atributos);

      rechecar=atributos[ParsersAttributes::RECHECK]==ParsersAttributes::_TRUE_;
      num_estrategia=atributos[ParsersAttributes::STRATEGY_NUM].toUInt();
      tp_elem=tipos_elem[atributos[ParsersAttributes::TYPE]];

      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      XMLParser::getElementAttributes(atributos);

      if(tp_elem==OperatorClassElement::STORAGE_ELEM)
      {
       tipo=criarTipoPgSQL();
       elem_classe.setStorage(tipo);
      }
      else if(tp_elem==OperatorClassElement::FUNCTION_ELEM)
      {
       objeto=obterObjeto(atributos[ParsersAttributes::SIGNATURE],OBJ_FUNCTION);
       elem_classe.setFunction(dynamic_cast<Function *>(objeto),num_estrategia);
      }
      else if(tp_elem==OperatorClassElement::OPERATOR_ELEM)
      {
       objeto=obterObjeto(atributos[ParsersAttributes::SIGNATURE],OBJ_OPERATOR);
       elem_classe.setOperator(dynamic_cast<Operator *>(objeto),num_estrategia,rechecar);
      }

      classe_op->addElement(elem_classe);
      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(classe_op) delete(classe_op);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(classe_op);
}

OperatorFamily *ModeloBD::criarFamiliaOperadores(void)
{
 map<QString, QString> atributos;
 OperatorFamily *familia_op=NULL;

 try
 {
  familia_op=new OperatorFamily;
  definirAtributosBasicos(familia_op);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Definindo os valores de atributos básicos do objeto
  familia_op->setIndexingType(TipoIndexacao(atributos[ParsersAttributes::INDEX_TYPE]));
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(familia_op) delete(familia_op);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(familia_op);
}

Aggregate *ModeloBD::criarFuncaoAgregacao(void)
{
 map<QString, QString> atributos;
 BaseObject *funcao=NULL;
 QString elem;
 TipoPgSQL tipo;
 Aggregate *func_agreg=NULL;

 try
 {
  func_agreg=new Aggregate;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(func_agreg);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);
  func_agreg->setInitialCondition(atributos[ParsersAttributes::INITIAL_COND]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::TYPE)
     {
      //Obtém os atributos do tipo
      XMLParser::getElementAttributes(atributos);
      tipo=criarTipoPgSQL();

      //Define o tipo   função agregada de acordo com o tipo de referência do mesmo
      if(atributos[ParsersAttributes::REF_TYPE]==
          ParsersAttributes::STATE_TYPE)
       func_agreg->setStateType(tipo);
      else
       func_agreg->addDataType(tipo);
     }
     else if(elem==ParsersAttributes::FUNCTION)
     {
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
       throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(func_agreg->getName()))
                             .arg(func_agreg->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
                             .arg(BaseObject::getTypeName(OBJ_FUNCTION)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a função de acordo com o tipo de referência da mesma
      if(atributos[ParsersAttributes::REF_TYPE]==ParsersAttributes::TRANSITION_FUNC)
       func_agreg->setFunction(Aggregate::TRANSITION_FUNC,
                                 dynamic_cast<Function *>(funcao));
      else
       func_agreg->setFunction(Aggregate::FINAL_FUNC,
                                 dynamic_cast<Function *>(funcao));
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(func_agreg) delete(func_agreg);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(func_agreg);
}

Tabela *ModeloBD::criarTabela(void)
{
 map<QString, QString> atributos;
 QString elem;
 Tabela *tabela=NULL;
 TableObject *objeto=NULL;

 try
 {
  tabela=new Tabela;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(tabela);

  //Obtém os atributos
  XMLParser::getElementAttributes(atributos);
  tabela->definirAceitaOids(atributos[ParsersAttributes::OIDS]==ParsersAttributes::_TRUE_);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();
     XMLParser::savePosition();
     objeto=NULL;

     if(elem==BaseObject::objs_schemas[OBJ_COLUMN])
      objeto=criarColuna();
     else if(elem==BaseObject::objs_schemas[OBJ_CONSTRAINT])
      objeto=criarRestricao(tabela);
     else if(elem==BaseObject::objs_schemas[OBJ_INDEX])
      objeto=criarIndice(tabela);
     else if(elem==BaseObject::objs_schemas[OBJ_RULE])
      objeto=criarRegra();
     else if(elem==BaseObject::objs_schemas[OBJ_TRIGGER])
      objeto=criarGatilho(tabela);

     if(objeto)
      tabela->adicionarObjeto(objeto);

     XMLParser::restorePosition();
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  tabela->setProtected(tabela->isProtected());
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  XMLParser::restorePosition();
  if(tabela) delete(tabela);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tabela);
}

Column *ModeloBD::criarColuna(void)
{
 map<QString, QString> atributos;
 Column *coluna=NULL;
 QString elem;

 try
 {
  coluna=new Column;
  definirAtributosBasicos(coluna);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  coluna->setNotNull(atributos[ParsersAttributes::NOT_NULL]==ParsersAttributes::_TRUE_);
  coluna->setDefaultValue(atributos[ParsersAttributes::DEFAULT_VALUE]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::TYPE)
     {
      coluna->setType(criarTipoPgSQL());
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(coluna) delete(coluna);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(coluna);
}

Restricao *ModeloBD::criarRestricao(BaseObject *objeto)
{
 map<QString, QString> atributos;
 Restricao *restricao=NULL;
 BaseObject *tabela_ref=NULL;
 Tabela *tabela=NULL,*tabela_aux=NULL;
 Column *coluna=NULL;
 Relacionamento *relacao=NULL;
 QString elem, str_aux;
 bool postergavel, ins_rest_tabela=false;
 TipoRestricao tipo_rest;
 QStringList lista_cols;
 int qtd, i;
 unsigned tipo_coluna;
 ObjectType tipo_objeto;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  //Caso o objeto o qual será possuidor da restrição esteja alocado
  if(objeto)
  {
   tipo_objeto=objeto->getObjectType();
   if(tipo_objeto==OBJ_TABLE)
    tabela=dynamic_cast<Tabela *>(objeto);
   else if(tipo_objeto==OBJ_RELATIONSHIP)
    relacao=dynamic_cast<Relacionamento *>(objeto);
   else
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Caso o objeto não esteja especificado então o objeto possuidor será considerado
     como sendo sempre uma tabela e com base nisso o atributo "table" no código
     xml da restrição será usado para localizar a tabela possuidora no modelo */
  else
  {
   tipo_objeto=OBJ_TABLE;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   ins_rest_tabela=true;
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  restricao=new Restricao;
  restricao->setParentTable(tabela);

  //Configurando o tipo da restrição
  if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::CK_CONSTR)
   tipo_rest=TipoRestricao::check;
  else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::PK_CONSTR)
   tipo_rest=TipoRestricao::primary_key;
  else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::FK_CONSTR)
   tipo_rest=TipoRestricao::foreign_key;
  else
   tipo_rest=TipoRestricao::unique;

  restricao->definirTipo(tipo_rest);
  if(!atributos[ParsersAttributes::FACTOR].isEmpty())
   restricao->definirFatorPreenchimento(atributos[ParsersAttributes::FACTOR].toUInt());
  definirAtributosBasicos(restricao);

  /* Caso o tipo de restrição seja uma chave primária uma verificação importante
     é feita. Se a chave primária está sendo criada como filha de uma tag
     <table> ou <relationship> (vide exemplo 1).

     Exemplo 1:

      <table ...>        <relationship ...>
         ...                ...
         <constraint>       <constraint>
      </table>           </relationship>

     Esta restrição está declarada dentro de um bloco de código de uma tabela ou
     de relacionamento a mesma sendo assim a mesma será construída normalmente.

     Caso a mesma esteja sendo construída fora de ambos os blocos (vide exemplo 2)
     e referenciando apenas uma tabela no modelo um erro será gerado pois chaves
     primárias devem sempre ser construídas dentro de um dos dois blocos apresentados.

     Exemplo 2:
     <table name="tabela_X">
       ...
     </table>
     <relationship>
       ...
     </relationship>
     <constraint ... table="tabela_X">
       ...
     </constraint>  */

  /* Na prática, para se fazer a verificação acima, é suficiente
     verificar se o parâmetro 'objeto' está alocado pois o mesmo
     armazena o endereço da tabela ou relacionamento recém criados
     a partir do bloco <table> ou <relationship>, respectivamente.
     Quando tal parâmetro está nulo indica que a restrição será criada
     e atribuíd  tabela cujo nome está no atributo 'table' no XML
     significando que a mesma está declarada fora dos dois blocos indicados.
     Adicionalmente é necessário verificar o tipo da restrição para se
     ter certeza que a mesma é uma chave primária. */
  if(!objeto && tipo_rest==TipoRestricao::primary_key)
    throw Exception(Exception::getErrorMessage(ERR_INV_PRIM_KEY_ALOCATION)
                  .arg(QString::fromUtf8(restricao->getName())),
                  ERR_INV_PRIM_KEY_ALOCATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Efetuando configurações específicas para chaves estrangeiras
  if(tipo_rest==TipoRestricao::foreign_key /*&& tipo_objeto==OBJETO_TABELA*/)
  {
   //Define se a restrição é postergavel (apenas para chaves estrangeiras)
   postergavel=(atributos[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
   restricao->definirPostergavel(postergavel);

   if(postergavel && !atributos[ParsersAttributes::DEFER_TYPE].isEmpty())
    restricao->definirTipoPostergacao(atributos[ParsersAttributes::DEFER_TYPE]);

   if(!atributos[ParsersAttributes::COMPARISON_TYPE].isEmpty())
    restricao->definirTipoComparacao(atributos[ParsersAttributes::COMPARISON_TYPE]);

   //Definindo os tipos de ação nos eventos DELETE e UPDATE
   if(!atributos[ParsersAttributes::DEL_ACTION].isEmpty())
    restricao->definirTipoAcao(atributos[ParsersAttributes::DEL_ACTION], false);

   if(!atributos[ParsersAttributes::UPD_ACTION].isEmpty())
    restricao->definirTipoAcao(atributos[ParsersAttributes::UPD_ACTION], true);

   //Obtém a tabela referenciada na chave estrangeira
   tabela_ref=obterObjeto(atributos[ParsersAttributes::REF_TABLE], OBJ_TABLE);

   /* Caso a tabela referenciada não seja encontrada verifica se esta não é a própria
      tabela a qual receberá a restrição (usado para auto-relacionamentos) */
   if(!tabela_ref && tabela->getName(true)==atributos[ParsersAttributes::REF_TABLE])
    tabela_ref=tabela;

   /* Caso a tabela referenciada não foi encontrada uma exceção será disparada pois
      não se pode criar uma chave estrangeira sem referenciar uma tabela de destino */
   if(!tabela_ref)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(restricao->getName()))
          .arg(restricao->getTypeName())
          .arg(QString::fromUtf8(atributos[ParsersAttributes::REF_TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Define a tabela de destino da chave estrangeira
   restricao->definirTabReferenciada(tabela_ref);
  }

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::EXPRESSION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //Obtém o contéudo do elemento <expression>
      restricao->definirExpChecagem(XMLParser::getElementContent());
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(atributos);

      /* Obtém os nomes das colunas participantes da restrição
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_cols=atributos[ParsersAttributes::NAMES].split(',');
      qtd=lista_cols.count();

      /* Obtém o tipo de referência das colunas de acordo com o atributo
         tipo de referência vindo do XML */
      if(atributos[ParsersAttributes::REF_TYPE]==ParsersAttributes::SRC_COLUMNS)
       tipo_coluna=Restricao::COLUNA_ORIGEM;
      else
       tipo_coluna=Restricao::COLUNA_REFER;

      //Varre a lista de nomes de colunas e as obtém da tabela a qual possuirá a restrição
      for(i=0; i < qtd; i++)
      {
       if(tipo_coluna==Restricao::COLUNA_ORIGEM)
       {
        if(tipo_objeto==OBJ_TABLE)
        {
         coluna=tabela->obterColuna(lista_cols[i]);
         //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
         if(!coluna)
          coluna=tabela->obterColuna(lista_cols[i], true);
        }
        else
         //Para os demais tipos de relacionamento as colunas a serem obtidas são os atributos do relacionamento
          coluna=dynamic_cast<Column *>(relacao->obterObjeto(lista_cols[i], OBJ_COLUMN));
       }
       else
       {
        tabela_aux=dynamic_cast<Tabela *>(tabela_ref);
        coluna=tabela_aux->obterColuna(lista_cols[i]);
        //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
        if(!coluna)
         coluna=tabela_aux->obterColuna(lista_cols[i], true);
       }

       //Adiciona a coluna   restrição
       restricao->adicionarColuna(coluna, tipo_coluna);
      }
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(ins_rest_tabela)
  {
   //Caso a restrição criada não seja uma chave primária insere-a normalmente na tabela
   if(restricao->obterTipoRestricao()!=TipoRestricao::primary_key)
   {
    tabela->adicionarRestricao(restricao);

    /* Caso a tabela receptora da restrição esteja inserida no modelo, força o seu redesenho.
       Isso é útil para atualizar tabelas as quais tiveram restrições adicionadas após a sua
       criação */
    if(this->obterIndiceObjeto(tabela) >= 0)
      tabela->setModified(true);
   }
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(restricao)
   delete(restricao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(restricao);
}

Index *ModeloBD::criarIndice(Tabela *tabela)
{
 map<QString, QString> atributos;
 Index *indice=NULL;
 Column *coluna=NULL;
 OperatorClass *classe_oper=NULL;
 QString elem, str_aux, expr;
 bool inc_ind_tabela=false,
      ordem_asc=false, nulos_primeiro=false;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  if(!tabela)
  {
   inc_ind_tabela=true;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
          .arg(BaseObject::getTypeName(OBJ_INDEX))
          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
          .arg(BaseObject::getTypeName(OBJ_TABLE));
    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  indice=new Index;
  definirAtributosBasicos(indice);
  indice->setParentTable(tabela);
  indice->setIndexAttribute(Index::CONCURRENT, atributos[ParsersAttributes::CONCURRENT]==ParsersAttributes::_TRUE_);
  indice->setIndexAttribute(Index::UNIQUE, atributos[ParsersAttributes::UNIQUE]==ParsersAttributes::_TRUE_);
  indice->setIndexAttribute(Index::FAST_UPDATE, atributos[ParsersAttributes::FAST_UPDATE]==ParsersAttributes::_TRUE_);

  indice->setIndexingType(atributos[ParsersAttributes::INDEX_TYPE]);
  indice->setFillFactor(atributos[ParsersAttributes::FACTOR].toUInt());

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     /* Caso o elemento atual for do tipo <idxelement> indica que
        os elementos filhos que podem ser extraídos são
        <column>, <expression> ou <opclass> */
     if(elem==ParsersAttributes::INDEX_ELEMENT)
     {
      nulos_primeiro=(atributos[ParsersAttributes::NULLS_FIRST]==ParsersAttributes::_TRUE_);
      ordem_asc=(atributos[ParsersAttributes::ASC_ORDER]==ParsersAttributes::_TRUE_);

      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       elem=XMLParser::getElementName();

       if(XMLParser::getElementType()==XML_ELEMENT_NODE)
       {
        //Caso o elemento atual seja um  <opclass>
        if(elem==ParsersAttributes::OP_CLASS)
        {
         XMLParser::getElementAttributes(atributos);
         classe_oper=dynamic_cast<OperatorClass *>(obterObjeto(atributos[ParsersAttributes::NAME], OBJ_OPCLASS));

         //Caso o índice esteja referenciando uma classe de operadores inexistente
         if(!classe_oper)
         {
          //Configura os argumentos da mensagem de erro
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(indice->getName()))
                          .arg(BaseObject::getTypeName(OBJ_INDEX))
                          .arg(QString::fromUtf8(atributos[ParsersAttributes::OP_CLASS]))
                          .arg(BaseObject::getTypeName(OBJ_OPCLASS));
          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }
        //Caso o elemento atual seja um  <column>
        else if(elem==ParsersAttributes::COLUMN)
        {
         //Obtém a coluna que o elemento referencia
         XMLParser::getElementAttributes(atributos);
         coluna=tabela->obterColuna(atributos[ParsersAttributes::NAME]);

         /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
            seu nome antigo */
         if(!coluna)
          coluna=tabela->obterColuna(atributos[ParsersAttributes::NAME], true);
        }
        //Caso o elemento atual seja um  <expression>
        else if(elem==ParsersAttributes::EXPRESSION)
        {
         XMLParser::savePosition();
         //Acessa o elemento filho o qual contém o conteúdo da expressão
         XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
         expr=XMLParser::getElementContent();
         XMLParser::restorePosition();
        }
       }
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      if(!expr.isEmpty())
       indice->addElement(expr, classe_oper, ordem_asc, nulos_primeiro);
      else
       indice->addElement(coluna, classe_oper, ordem_asc, nulos_primeiro);

      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();
      indice->setConditionalExpression(str_aux);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(inc_ind_tabela)
  {
   tabela->adicionarIndice(indice);
   tabela->setModified(true);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(indice) delete(indice);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(indice);
}

Regra *ModeloBD::criarRegra(void)
{
 map<QString, QString> atributos;
 QStringList lista_cmd;
 Regra *regra=NULL;
 QString elem, str_aux;
 int qtd, i;

 try
 {
  regra=new Regra;
  definirAtributosBasicos(regra);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  regra->definirTipoExecucao(atributos[ParsersAttributes::EXEC_TYPE]);
  regra->definirTipoEvento(atributos[ParsersAttributes::EVENT_TYPE]);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::COMMANDS ||
        elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da condição ou comandos
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      //str_aux=QString::fromUtf8(ParserXML::obterConteudoElemento());
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();

      if(elem==ParsersAttributes::COMMANDS)
      {
       /* A lista de comandos é quebrada por ; e os comandos
          inseridos um a um na regra */
       lista_cmd=str_aux.split(';');
       qtd=lista_cmd.count();
       for(i=0; i < qtd; i++)
       {
        //Caso o comando não esteja vazio
        if(!lista_cmd[i].isEmpty())
         regra->adicionarComando(lista_cmd[i]);
       }
      }
      else
       regra->definirExpCondicional(str_aux);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(regra) delete(regra);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(regra);
}

Gatilho *ModeloBD::criarGatilho(Tabela *tabela)
{
 map<QString, QString> atributos;
 Gatilho *gatilho=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 int qtd, i;
 BaseObject *tabela_ref=NULL, *funcao=NULL;
 Column *coluna=NULL;
 bool inc_gat_tabela=false;

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);

  if(!tabela && atributos[ParsersAttributes::TABLE].isEmpty())
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(!tabela && !atributos[ParsersAttributes::TABLE].isEmpty())
  {
   inc_gat_tabela=true;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[ParsersAttributes::TABLE], OBJ_TABLE));
   if(!tabela)
    throw Exception(QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                  .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                  .arg(BaseObject::getTypeName(OBJ_TRIGGER))
                  .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
                  .arg(BaseObject::getTypeName(OBJ_TABLE)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  gatilho=new Gatilho;
  gatilho->setParentTable(tabela);

  definirAtributosBasicos(gatilho);

  //Marcando os eventos de execução do gatilho
  gatilho->setEvent(TipoEvento::on_insert,
                        (atributos[ParsersAttributes::INS_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(TipoEvento::on_delete,
                        (atributos[ParsersAttributes::DEL_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(TipoEvento::on_update,
                        (atributos[ParsersAttributes::UPD_EVENT]==
                         ParsersAttributes::_TRUE_));

  gatilho->setEvent(TipoEvento::on_truncate,
                        (atributos[ParsersAttributes::TRUNC_EVENT]==
                         ParsersAttributes::_TRUE_));

  //Marcando e o gatilho é executado por linha ou não
  gatilho->setExecutePerRow(atributos[ParsersAttributes::PER_ROW]==
                            ParsersAttributes::_TRUE_);

  //Define o modo de disparo do gatilho
  gatilho->setFiringType(TipoDisparo(atributos[ParsersAttributes::FIRING_TYPE]));


  /* Atribuindo os argumentos vindo do XML ao gatilho.
     No XML os argumentos estão separados por vírgula,
     sendo assim o método split é usado para quebrar a
     string de argumentos e atribui-los ao objeto */
  lista_aux=atributos[ParsersAttributes::ARGUMENTS].split(',');
  qtd=lista_aux.count();
  for(i=0; i < qtd; i++)
  {
   if(!lista_aux[i].isEmpty())
    gatilho->addArgument(lista_aux[i]);
  }

  //Caso o objeto alocado seja um gatilho
  gatilho->setDeferrable(atributos[ParsersAttributes::DEFERRABLE]==
                              ParsersAttributes::_TRUE_);
  if(gatilho->isDeferrable())
    gatilho->setDeferralType(atributos[ParsersAttributes::DEFER_TYPE]);

  //Obtém a tabela referenciada no gatilho
  tabela_ref=obterObjeto(atributos[ParsersAttributes::REF_TABLE], OBJ_TABLE);
  gatilho->setReferecendTable(tabela_ref);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::FUNCTION)
     {
      XMLParser::getElementAttributes(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[ParsersAttributes::SIGNATURE], OBJ_FUNCTION);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[ParsersAttributes::SIGNATURE].isEmpty())
      {
       str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
               .arg(QString::fromUtf8(gatilho->getName()))
               .arg(gatilho->getTypeName())
               .arg(QString::fromUtf8(atributos[ParsersAttributes::SIGNATURE]))
               .arg(BaseObject::getTypeName(OBJ_FUNCTION));

       //Dispara a exceção
       throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }

      //Define a função executada pelo gatilho
      gatilho->setFunction(dynamic_cast<Function *>(funcao));
     }
     else if(elem==ParsersAttributes::CONDITION)
     {
      XMLParser::savePosition();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      str_aux=XMLParser::getElementContent();
      XMLParser::restorePosition();
      gatilho->setCondition(str_aux);
     }
     else if(elem==ParsersAttributes::COLUMNS)
     {
      //Obtém os atributos da tag <columns>
      XMLParser::getElementAttributes(atributos);

      /* Obtém os nomes das colunas participantes do gatilho
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_aux=atributos[ParsersAttributes::NAMES].split(',');
      qtd=lista_aux.count();

      for(i=0; i < qtd; i++)
      {
       coluna=tabela->obterColuna(lista_aux[i]);

       /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
          seu nome antigo */
       if(!coluna)
        coluna=tabela->obterColuna(lista_aux[i], true);

       gatilho->addColumn(coluna);
      }
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }

  if(inc_gat_tabela)
  {
   tabela->adicionarObjeto(gatilho);
   tabela->setModified(true);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(gatilho) delete(gatilho);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(gatilho);
}

Sequencia *ModeloBD::criarSequencia(bool ignorar_possuidora)
{
 map<QString, QString> atributos;
 Sequencia *sequencia=NULL;
 BaseObject *tabela=NULL;
 Column *coluna=NULL;
 QString elem, str_aux, nome_tab, nome_col;
 QStringList lista_elem;
 int qtd;

 try
 {
  sequencia=new Sequencia;
  definirAtributosBasicos(sequencia);

  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  sequencia->definirValores(atributos[ParsersAttributes::MIN_VALUE],
                            atributos[ParsersAttributes::MAX_VALUE],
                            atributos[ParsersAttributes::INCREMENT],
                            atributos[ParsersAttributes::START],
                            atributos[ParsersAttributes::CACHE]);
  sequencia->definirCiclica(atributos[ParsersAttributes::CYCLE]==ParsersAttributes::_TRUE_);

  //Caso o atributo de coluna possuidora da sequencia esteja preenchido
  if(!atributos[ParsersAttributes::OWNER_COLUMN].isEmpty())
  {
   //Quebra o valor do atributo por .
   lista_elem=atributos[ParsersAttributes::OWNER_COLUMN].split('.');
   qtd=lista_elem.count();

   /* Caso a lista de nomes gerada possua 3 elementos indica
      que a coluna possuidora foi está no formato
      [ESQUEMA].[TABELA].[COLUNA] caso contrário
      supõe-se que esteja no formato
      [TABELA].[COLUNA] */
   if(qtd==3)
   {
    nome_tab=lista_elem[0] + "." + lista_elem[1];
    nome_col=lista_elem[2];
   }
   else
   {
    nome_tab=lista_elem[0];
    nome_col=lista_elem[1];
   }

   //Obtém a tabela do modelo
   tabela=obterObjeto(nome_tab, OBJ_TABLE);

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabela)
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
            .arg(QString::fromUtf8(sequencia->getName()))
            .arg(BaseObject::getTypeName(OBJ_SEQUENCE))
            .arg(QString::fromUtf8(nome_tab))
            .arg(BaseObject::getTypeName(OBJ_TABLE));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Tenta obter a coluna da tabela com o nome vindo do XML
   coluna=dynamic_cast<Tabela *>(tabela)->obterColuna(nome_col);

   //Caso a coluna não for encontrada tenta obtê-la referenciando o antigo nome
   if(!coluna)
    coluna=dynamic_cast<Tabela *>(tabela)->obterColuna(nome_col, true);

   /* Caso a coluna não exista porém a mesma esteja sendo referenciada no xml
      um erro será disparado */
   if(!coluna && !ignorar_possuidora)
    throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
                  .arg(QString::fromUtf8(sequencia->getName(true))),
                  ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   sequencia->definirPossuidora(coluna);
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(sequencia) delete(sequencia);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(sequencia);
}

Visao *ModeloBD::criarVisao(void)
{
 map<QString, QString> atributos;
 Visao *visao=NULL;
 Column *coluna=NULL;
 Tabela *tabela=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 vector<Referencia> vet_refs;
 unsigned tipo;
 int idx_ref, i, qtd;

 try
 {
  visao=new Visao;
  definirAtributosBasicos(visao);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::REFERENCE)
     {
      //Obtém os atributos da referência
      XMLParser::getElementAttributes(atributos);

      /* Caso o nome da tabela referenciada esteja preenchido,
         tentar criar uma referência específica a uma tabela/coluna */
      if(!atributos[ParsersAttributes::TABLE].isEmpty())
      {
       coluna=NULL;
       tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[ParsersAttributes::TABLE], OBJ_TABLE));

       //Dispara uma exceção caso a tabela referenciada não exista
       if(!tabela)
       {
        str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                        .arg(QString::fromUtf8(visao->getName()))
                        .arg(BaseObject::getTypeName(OBJ_VIEW))
                        .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]))
                        .arg(BaseObject::getTypeName(OBJ_TABLE));

        //Dispara a exceção
        throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }

       if(!atributos[ParsersAttributes::COLUMN].isEmpty())
       {
        //Tenta obter a colna referenciada da tabela
        coluna=tabela->obterColuna(atributos[ParsersAttributes::COLUMN]);

        //Caso a coluna não exista tenta obtê-la referenciando o nome antigo da mesma
        if(!coluna)
         coluna=tabela->obterColuna(atributos[ParsersAttributes::COLUMN], true);

        /* Caso o atributo coluna da referencia esteja preenchido mas um objeto coluna
           não foi encontrado na tabela, uma exceção será disparada pois a visão está
           referenciando uma coluna inexistente na tabela */
         if(!coluna)
         {
          str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                          .arg(QString::fromUtf8(visao->getName()))
                          .arg(BaseObject::getTypeName(OBJ_VIEW))
                          .arg(QString::fromUtf8(atributos[ParsersAttributes::TABLE]) + "." +
                               QString::fromUtf8(atributos[ParsersAttributes::COLUMN]))
                         .arg(BaseObject::getTypeName(OBJ_COLUMN));

          //Dispara a exceção
          throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }

       //Adiciona a referência configurad  lista temporária de referências
       vet_refs.push_back(Referencia(tabela, coluna,
                                     atributos[ParsersAttributes::ALIAS],
                                     atributos[ParsersAttributes::COLUMN_ALIAS]));
      }
      //Extraindo uma referênci  uma expressão
      else
      {
       XMLParser::savePosition();
       //Armazena o alias da expressão
       str_aux=atributos[ParsersAttributes::ALIAS];

       //Acessa e obtém o conteúdo da expressão
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
       vet_refs.push_back(Referencia(XMLParser::getElementContent(),str_aux));

       XMLParser::restorePosition();
      }
     }
     /* Extraindo as expressões as quais formam as partes da declaração da visão,
        ou seja, expressões e referências as quais estão entre SELECT-FROM,
        FROM-WHERE */
     else if(elem==ParsersAttributes::EXPRESSION)
     {
      XMLParser::savePosition();
      XMLParser::getElementAttributes(atributos);

      //Armazena o alias da expressão
      if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::SELECT_EXP)
       tipo=Referencia::SQL_REFER_SELECT;
      else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::FROM_EXP)
       tipo=Referencia::SQL_REFER_FROM;
      else
       tipo=Referencia::SQL_REFER_WHERE;

      //Acessa e obtém o conteúdo da expressão
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      lista_aux=XMLParser::getElementContent().split(',');
      qtd=lista_aux.size();

      //Construindo cada expressão na visão
      for(i=0; i < qtd; i++)
      {
       //Obtém o índice da referência e a adiioa   visão
       idx_ref=lista_aux[i].toInt();
       visao->adicionarReferencia(vet_refs[idx_ref],tipo);
      }

      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(visao) delete(visao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(visao);
}

Textbox *ModeloBD::criarCaixaTexto(void)
{
 Textbox *caixa_texto=NULL;
 map<QString, QString> atributos;

 try
 {
  caixa_texto=new Textbox;
  definirAtributosBasicos(caixa_texto);

  XMLParser::getElementAttributes(atributos);

  if(atributos[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::ITALIC_TXT, true);

  if(atributos[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::BOLD_TXT, true);

  if(atributos[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_)
   caixa_texto->setTextAttribute(Textbox::UNDERLINE_TXT, true);

  if(!atributos[ParsersAttributes::COLOR].isEmpty())
   caixa_texto->setTextColor(QColor(atributos[ParsersAttributes::COLOR]));
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(caixa_texto) delete(caixa_texto);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(caixa_texto);
}

RelacionamentoBase *ModeloBD::criarRelacionamento(void)
{
 vector<unsigned> cols_pk_especial;
 map<QString, QString> atributos;
 RelacionamentoBase *relacao_base=NULL;
 Relacionamento *relacao=NULL;
 BaseTable *tabelas[2]={NULL, NULL};
 bool obrig_orig, obrig_dest, identificador, protegido, postergavel, sufixo_auto;
 TipoPostergacao tipo_postergacao;
 unsigned tipo_relac=0, i;
 ObjectType tipos_tab[2]={OBJ_VIEW, OBJ_TABLE}, tipo_obj_rel;
 QString str_aux, elem,
         atribs[2]={ ParsersAttributes::SRC_TABLE,
                     ParsersAttributes::DST_TABLE };

 try
 {
  //Obtém os atributos do elemento
  XMLParser::getElementAttributes(atributos);
  protegido=(atributos[ParsersAttributes::PROTECTED]==ParsersAttributes::_TRUE_);

  if(atributos[ParsersAttributes::TYPE]!=ParsersAttributes::RELATION_TAB_VIEW)
  {
   tipos_tab[0]=OBJ_TABLE;
   tipo_obj_rel=OBJ_RELATIONSHIP;
  }
  else
   tipo_obj_rel=BASE_RELATIONSHIP;

  /* Esta iteração obtém as tabelas participantes do relacionamento a
     partir do modelo com base nos nomes das tabelas vindos do XML */
  for(i=0; i < 2; i++)
  {
   //Localiza a tabela
   tabelas[i]=dynamic_cast<BaseTable *>(obterObjeto(atributos[atribs[i]], tipos_tab[i]));

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabelas[i])
   {
    str_aux=QString(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL))
                    .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                    .arg(BaseObject::getTypeName(tipo_obj_rel))
                    .arg(QString::fromUtf8(atributos[atribs[i]]))
                    .arg(BaseObject::getTypeName(tipos_tab[i]));

    //Dispara a exceção
    throw Exception(str_aux,ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  //Caso o relacionamento entre tabela e visão exista
  relacao_base=obterRelacionamento(tabelas[0], tabelas[1]);
  if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATION_TAB_VIEW)
  {
   //Caso o relacionamento tabela-visão nao seja encontrado o erro será disparado
   if(!relacao_base)
    throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
                             .arg(QString::fromUtf8(this->getName()))
                             .arg(this->getTypeName())
                             .arg(QString::fromUtf8(atributos[ParsersAttributes::NAME]))
                             .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                  ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Desconecta o relacionamento para configurá-lo
   relacao_base->desconectarRelacionamento();
   //relacao_base->definirNome(atributos[ParsersAttributes::NAME]);
  }
  /* Caso o tipo de relacionamento não seja tabela-visão, isso indica que
     um relacionamento tabela-tabela deverá ser criado */
  else if(atributos[ParsersAttributes::TYPE]!=ParsersAttributes::RELATION_TAB_VIEW)
  {
   //Obtém os atributos do relacionamento a partir do XML
   obrig_orig=atributos[ParsersAttributes::SRC_REQUIRED]==ParsersAttributes::_TRUE_;
   obrig_dest=atributos[ParsersAttributes::DST_REQUIRED]==ParsersAttributes::_TRUE_;
   identificador=atributos[ParsersAttributes::IDENTIFIER]==ParsersAttributes::_TRUE_;
   postergavel=atributos[ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_;
   sufixo_auto=(!atributos[ParsersAttributes::AUTO_SUFFIX].isEmpty() &&
                atributos[ParsersAttributes::AUTO_SUFFIX]==ParsersAttributes::_TRUE_);
   tipo_postergacao=TipoPostergacao(atributos[ParsersAttributes::DEFER_TYPE]);

   //Configura o tipo do novo relacionamento
   if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_11)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_11;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_1N)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_1N;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_NN)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_NN;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_GEN)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_GEN;
   else if(atributos[ParsersAttributes::TYPE]==ParsersAttributes::RELATIONSHIP_DEP)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_DEP;

   //Cria o novo relacionamento
   relacao=new Relacionamento(tipo_relac,
                              dynamic_cast<Tabela *>(tabelas[0]),
                              dynamic_cast<Tabela *>(tabelas[1]),
                              obrig_orig, obrig_dest,
                              sufixo_auto, atributos[ParsersAttributes::SRC_SUFFIX],
                              atributos[ParsersAttributes::DST_SUFFIX],
                              identificador, postergavel, tipo_postergacao);

    if(!atributos[ParsersAttributes::TABLE_NAME].isEmpty())
    relacao->definirNomeTabelaRelNN(atributos[ParsersAttributes::TABLE_NAME]);

   /* Faz com que o ponteiro relacao_base aponte para o novo relacionamento
      para executar as configurações geréricas as quais se aplicam tanto
      para relacionametno tabela-visao quanto para rel. tabela-tabela */
   relacao_base=relacao;
  }

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     elem=XMLParser::getElementName();

     if(elem==ParsersAttributes::COLUMN && relacao)
     {
      XMLParser::savePosition();
      relacao->adicionarObjeto(criarColuna());
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::CONSTRAINT && relacao)
     {
      XMLParser::savePosition();
      relacao->adicionarObjeto(criarRestricao(relacao));
      XMLParser::restorePosition();
     }
     //Configurando a linha do relacionamento
     else if(elem==ParsersAttributes::LINE)
     {
      vector<QPointF> pontos;
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       //Lê o ponto do XML
       XMLParser::getElementAttributes(atributos);
       pontos.push_back(QPointF(atributos[ParsersAttributes::X_POS].toFloat(),
                                atributos[ParsersAttributes::Y_POS].toFloat()));
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      relacao_base->definirPontos(pontos);
      XMLParser::restorePosition();
     }
     //Configurando a posição dos rótulos
     else if(elem==ParsersAttributes::LABEL)
     {
      XMLParser::getElementAttributes(atributos);
      //Obtém o tipo de rótulo a ser configurado
      str_aux=atributos[ParsersAttributes::REF_TYPE];

      //Acessa o elemento filho da tag <label> o qual armazena a posição do rótulo
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
      XMLParser::getElementAttributes(atributos);
      XMLParser::restorePosition();
     }
     else if(elem==ParsersAttributes::SPECIAL_PK_COLS && relacao)
     {
      QList<QString> lista_cols;

      //Obtém os atributos da tag <special-pk-cols>
      XMLParser::getElementAttributes(atributos);
      lista_cols=atributos[ParsersAttributes::INDEXES].split(',');

      while(!lista_cols.isEmpty())
      {
       cols_pk_especial.push_back(lista_cols.front().toUInt());
       lista_cols.pop_front();
      }

      //Define as colunas que fazem parte da chave primária especila
      relacao->definirColsChavePrimariaEspecial(cols_pk_especial);
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);

  if(relacao_base && relacao_base->getObjectType()==OBJ_RELATIONSHIP)
   delete(relacao_base);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Caso o relacionamento tabela-tabela foi criado o mesmo será adicionado no modelo
 if(relacao)
 {
  obterXMLObjetosEspeciais();
  adicionarRelacionamento(relacao);
 }

 //Define a proteção do relacionamento
 relacao_base->setProtected(protegido);

 //Reconecta o relacionamento caso o mesmo seja um rel. tabela-visao
 if(relacao_base && relacao_base->getObjectType()==BASE_RELATIONSHIP)
  relacao_base->conectarRelacionamento();

 return(relacao_base);
}

Permissao *ModeloBD::criarPermissao(void)
{
 Permissao *permissao=NULL;
 BaseObject *objeto=NULL;
 Tabela *tabela_pai=NULL;
 Papel *papel=NULL;
 map<QString, QString> atrib_priv, atributos;
 map<QString, QString>::iterator itr, itr_end;
 ObjectType tipo_obj;
 QString obj_pai, nome_obj;
 QStringList lista;
 unsigned i, tam, tipo_priv=Permissao::PRIV_SELECT;
 bool valor_priv, op_concessao;

 try
 {
  //Obtém os privilégios configurados para a permissão
  XMLParser::getElementAttributes(atrib_priv);

  /* Acessa o elemento filho <object> o qual armazena o objeto
     do modelo relacionado   permissão */
  XMLParser::savePosition();
  XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
  XMLParser::getElementAttributes(atributos);

  //Obtém os atributos do objeto que é referenciado pela  permissão
  tipo_obj=getObjectType(atributos[ParsersAttributes::TYPE]);
  nome_obj=atributos[ParsersAttributes::NAME];
  obj_pai=atributos[ParsersAttributes::PARENT];

  //Caso o objeto seja uma coluna a mesma será obtida da tabela pai
  if(tipo_obj==OBJ_COLUMN)
  {
   //Primeiramente a tabela pai é obtida do modelo
   tabela_pai=dynamic_cast<Tabela *>(obterObjeto(obj_pai, OBJ_TABLE));

   /* Caso a tabela pai existe obtém o objeto filho da mesma
      o qual é referenciado pela permissão */
   if(tabela_pai)
    objeto=tabela_pai->obterColuna(nome_obj);
  }
  else if(tipo_obj==OBJ_DATABASE)
  {
   objeto=this;
  }
  else
   /* Para os demais tipos de objetos, aceitos como referenciados
      por permissões, serão obtidos do modelo em si */
   objeto=obterObjeto(nome_obj, tipo_obj);

  /* Caso o objeto não exista será disparada uma exceção pois uma permissão
     não pode existir sem que referencie um objeto */
  if(!objeto)
   throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                          .arg(QString::fromUtf8(nome_obj))
                          .arg(BaseObject::getTypeName(tipo_obj)),
                      ERR_PERM_REF_INEXIST_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Aloca a permissão relacionando-a com o objeto localizado
  permissao=new Permissao(objeto);

  /* Acessa o elemento que armazena os privilégios dos papéis os quais
     compartilham da mesma permissão sobre o objeto. Informar estes
     papéis não é obrigatório de acordo com a DTD e com a especificação
     SQL para permissões */
  do
  {
   if(XMLParser::getElementName()==ParsersAttributes::ROLES)
   {
    //Obtém os atributos do elemento <roles>, neste caso são names e reftype
    XMLParser::getElementAttributes(atributos);

    /* O atributo names armazena uma lista de nomes de papéis as quais a permissão
       referenciará. A lista tem os elementos separados por vírgula, sendo assim a
       string será quebrada usando o delimitador ',') */
    lista=atributos[ParsersAttributes::NAMES].split(',');

    //Obtém a quantidade de nomes de papéis na lista
    tam=lista.size();

    //Varre a lista de nomes de papéis
    for(i=0; i < tam; i++)
    {
     //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
     papel=dynamic_cast<Papel *>(obterObjeto(lista[i].trimmed(),OBJ_ROLE));

     /* Caso esse papel não exista um erro será disparado pois um novo papel
        não pode referenciar um outro papel que ainda nem foi criado */
     if(!papel)
     {
      //Dispara a exceção
      throw Exception(Exception::getErrorMessage(ERR_PERM_REF_INEXIST_OBJECT)
                              .arg(QString::fromUtf8(objeto->getName()))
                              .arg(objeto->getTypeName())
                              .arg(QString::fromUtf8(lista[i]))
                              .arg(BaseObject::getTypeName(OBJ_ROLE)),
                     ERR_REF_OBJ_INEXISTS_MODEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

     }
     //Adiciona o papel   permissão
     permissao->adicionarPapel(papel);
    }
   }
   else if(XMLParser::getElementName()==ParsersAttributes::PRIVILEGES)
   {
    //Obtém os atributos do elemento <privileges>
    XMLParser::getElementAttributes(atrib_priv);

    //Atribui os privilégio  permissão recém criada
    itr=atrib_priv.begin();
    itr_end=atrib_priv.end();

    /* Varre o mapa de privilégios configurando-os na permissão caso
       estes estejam definidos no XML */
    while(itr!=itr_end)
    {
     if(itr->first!=ParsersAttributes::GRANT_OP)
     {
      //Obtém o valor do privilégio (true/false)
      valor_priv=(itr->second==ParsersAttributes::_TRUE_);
      op_concessao=(itr->second==ParsersAttributes::GRANT_OP);

      //Identifica o tipo de privilégio atual
      if(itr->first==ParsersAttributes::CONNECT_PRIV)
       tipo_priv=Permissao::PRIV_CONNECT;
      else if(itr->first==ParsersAttributes::CREATE_PRIV)
       tipo_priv=Permissao::PRIV_CREATE;
      else if(itr->first==ParsersAttributes::DELETE_PRIV)
       tipo_priv=Permissao::PRIV_DELETE;
      else if(itr->first==ParsersAttributes::EXECUTE_PRIV)
       tipo_priv=Permissao::PRIV_EXECUTE;
      else if(itr->first==ParsersAttributes::INSERT_PRIV)
       tipo_priv=Permissao::PRIV_INSERT;
      else if(itr->first==ParsersAttributes::REFERENCES_PRIV)
       tipo_priv=Permissao::PRIV_REFERENCES;
      else if(itr->first==ParsersAttributes::SELECT_PRIV)
       tipo_priv=Permissao::PRIV_SELECT;
      else if(itr->first==ParsersAttributes::TEMPORARY_PRIV)
       tipo_priv=Permissao::PRIV_TEMPORARY;
      else if(itr->first==ParsersAttributes::TRIGGER_PRIV)
       tipo_priv=Permissao::PRIV_TRIGGER;
      else if(itr->first==ParsersAttributes::TRUNCATE_PRIV)
       tipo_priv=Permissao::PRIV_TRUNCATE;
      else if(itr->first==ParsersAttributes::UPDATE_PRIV)
       tipo_priv=Permissao::PRIV_UPDATE;
      else if(itr->first==ParsersAttributes::USAGE_PRIV)
       tipo_priv=Permissao::PRIV_USAGE;

      //Configura o privilégio na permissão
      permissao->definirPrivilegio(tipo_priv, (valor_priv || op_concessao), op_concessao);
     }
     itr++;
    }
   }
  }
  while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

  XMLParser::restorePosition();
 }
 catch(Exception &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (line: %2)")).arg(XMLParser::getLoadedFilename())
                                   .arg(XMLParser::getCurrentElement()->line);
  if(permissao) delete(permissao);

  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(permissao);
}

void ModeloBD::validarRemocaoColuna(Column *coluna)
{
 if(coluna && coluna->getParentTable())
 {
  vector<BaseObject *> vet_refs;
  obterReferenciasObjeto(coluna, vet_refs);

  //Caso um objeto seja encontrado o qual referencia a coluna
  if(!vet_refs.empty())
   //Dispara um erro informando que a coluna não pde ser remove e qual objeto a referencia
   throw Exception(Exception::getErrorMessage(ERR_REM_DIRECT_REFERENCE)
                 .arg(QString::fromUtf8(coluna->getParentTable()->getName(true)) + "." + QString::fromUtf8(coluna->getName(true)))
                 .arg(coluna->getTypeName())
                 .arg(QString::fromUtf8(vet_refs[0]->getName(true)))
                 .arg(vet_refs[0]->getTypeName()),
                 ERR_REM_DIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void ModeloBD::validarRelacObjetoTabela(TableObject *objeto, Tabela *tabela_pai)
{
 try
 {
  bool revalidar_rels=false, tab_ref_heranca=false;
  Relacionamento *rel=NULL;
  vector<BaseObject *>::iterator itr, itr_end;
  ObjectType tipo;

  if(objeto && tabela_pai)
  {
   tipo=objeto->getObjectType();

   /* Condição de revalidação de relacionamentos:
      > Caso seja uma coluna e a mesma é referenciada pela chave primária da tabela pai
      > Caso seja uma restrição e a mesma seja uma chave primária da tabela */
    revalidar_rels=((tipo==OBJ_COLUMN &&
                     tabela_pai->restricaoReferenciaColuna(dynamic_cast<Column *>(objeto), TipoRestricao::primary_key)) ||
                    (tipo==OBJ_CONSTRAINT &&
                     dynamic_cast<Restricao *>(objeto)->obterTipoRestricao()==TipoRestricao::primary_key));

   /* Caso seja uma coluna, verfica se a tabela pai participa de um relacionamento
     de generalização como tabela de destino (aquela que tem suas colunas copiadas
     para a tabela qua a herda) */
   if(tipo==OBJ_COLUMN)
   {
    itr=relacionamentos.begin();
    itr_end=relacionamentos.end();

    while(itr!=itr_end && !tab_ref_heranca)
    {
     rel=dynamic_cast<Relacionamento *>(*itr);
     itr++;
     tab_ref_heranca=(rel->obterTipoRelacionamento()==Relacionamento::RELACIONAMENTO_GEN &&
                      rel->obterTabelaReferencia()==tabela_pai);
    }
   }

   //Caso as duas condições acima sejam atendidas
   if(revalidar_rels || tab_ref_heranca)
   {
    //(Re)valida os relacionamento e os reconecta
    desconectarRelacionamentos();
    validarRelacionamentos();
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

QString ModeloBD::__obterDefinicaoObjeto(unsigned tipo_def)
{
 if(lim_conexao >= 0)
  attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(lim_conexao);

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  attributes[ParsersAttributes::ENCODING]="'" + (~tipo_codif) + "'";

  if(!localizacoes[1].isEmpty())
   attributes[ParsersAttributes::LC_COLLATE_DB]="'" + localizacoes[1] + "'";

  if(!localizacoes[0].isEmpty())
   attributes[ParsersAttributes::LC_CTYPE_DB]="'" + localizacoes[0]  + "'";
 }
 else
 {
  attributes[ParsersAttributes::ENCODING]=(~tipo_codif);
  attributes[ParsersAttributes::LC_COLLATE_DB]=localizacoes[1];
  attributes[ParsersAttributes::LC_CTYPE_DB]=localizacoes[0];
 }

 attributes[ParsersAttributes::TEMPLATE_DB]=bd_modelo;
 return(this->BaseObject::__getCodeDefinition(tipo_def));
}

QString ModeloBD::getCodeDefinition(unsigned tipo_def)
{
 return(this->getCodeDefinition(tipo_def, true));
}

QString ModeloBD::getCodeDefinition(unsigned tipo_def, bool exportar_arq)
{
 map<QString, QString> atribs_aux;
 unsigned qtd1, i, qtd;
 float qtd_geral_obj, qtd_defs_geradas;
 BaseObject *objeto=NULL;
 vector<BaseObject *> *lista_obj=NULL;
 vector<BaseObject *>::iterator itr, itr_end;
 vector<unsigned>::iterator itr1, itr1_end;
 QString msg=trUtf8("Generating %1 of the object: %2 (%3)"),
         atrib=ParsersAttributes::OBJECTS,
         tipo_def_str=(tipo_def==SchemaParser::SQL_DEFINITION ? "SQL" : "XML");
 Tipo *tipo_usr=NULL;
 map<unsigned, BaseObject *> mapa_objetos;
 vector<unsigned> vet_id_objs, vet_id_objs_tab;
 Tabela *tabela=NULL;
 Index *indice=NULL;
 Gatilho *gatilho=NULL;
 Restricao *restricao=NULL;
 Relacionamento *relacao=NULL;
 ObjectType tipo_obj,
                tipos_obj_aux[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA },
                tipos_obj[]={ OBJ_LANGUAGE, OBJ_FUNCTION, OBJ_TYPE,
                              OBJ_CAST, OBJ_CONVERSION,
                              OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
                              OBJ_AGGREGATE, OBJ_DOMAIN, OBJ_TEXTBOX, BASE_RELATIONSHIP,
                              OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE };
 try
 {
  qtd_geral_obj=this->obterNumObjetos();
  qtd_defs_geradas=0;

  /* Tratandos os objetos os quais tem ids fixos são eles: Papel, Espaço de Tabela,
     e Esquema. Estes precisam ser tratados separadamente no laço abaixo pois não
     entram na ordenação de ids executada para os demais tipos de objetos. */
  for(i=0; i < 3; i++)
  {
   //Obtém a lista de objetos do tipo
   lista_obj=obterListaObjetos(tipos_obj_aux[i]);

   /* Caso o tipo de definição seja SQL obtem o nome do atributo
      do tipo do objeto nos esquema SQL */
   if(tipo_def==SchemaParser::SQL_DEFINITION)
   {
    atrib=BaseObject::objs_schemas[tipos_obj_aux[i]];
    atribs_aux[atrib]="";
   }

   //Obtém o início e o fim da lista para uma varredura
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end)
   {
    //Obtém o objeto atual
    objeto=(*itr);

    if(objeto->getObjectType()!=OBJ_SCHEMA ||
       (objeto->getObjectType()==OBJ_SCHEMA &&
        objeto->getName()!="public"))
    {
     //Gera o codigo e o concatena com os demais já gerados
     atribs_aux[atrib]+=validarDefinicaoObjeto(objeto, tipo_def);
     //Dispara um sinal para sinalizar o progresso da geração do códgio
     qtd_defs_geradas++;
     if(!signalsBlocked())
     {
      emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                             msg.arg(tipo_def_str)
                               .arg(QString::fromUtf8(objeto->getName()))
                               .arg(objeto->getTypeName()),
                            objeto->getObjectType());
     }
    }
    itr++;
   }
  }

  /* Armazena os próprio objeto de modelo de objetos pois o mesmo também precisa estar na
     ordenação de objetos para ser criado na ordem correta quando o parser xml ler
     a definição */
  mapa_objetos[this->getObjectId()]=this;
  vet_id_objs.push_back(this->getObjectId());


  /* Armazenando os demais tipos de objetos no mapa de objetos para
     serem ordenados pelos ids. */
  /* Para definição XML todos os 16 tipos da lista 'tipos_objs' são tratados.
     Para definição SQL são tratados apenas os 12 primeiros (tabelas, relacionamentos, visões e sequências)
     são tratadas separadamente pois existe uma ordem específica em que elas precisam ser criadas e essa
     ordem é definida na interação após a ordenação dos objetos */
  if(tipo_def==SchemaParser::XML_DEFINITION)
   qtd=16;
  else
   qtd=12;

  for(i=0; i < qtd; i++)
  {
   /* Seleciona a lista de objetos para obter os identificadores e armazenar seus
      elementos no mapa de objetos. No caso de definição SQL apenas os  objetos
      caixa de texto e relacionamento tabela-visão não são obtidos pois os mesmos
      não possuem código SQL */
   if(tipo_def==SchemaParser::SQL_DEFINITION &&
      (tipos_obj[i]==OBJ_TEXTBOX || tipos_obj[i]==BASE_RELATIONSHIP))
    lista_obj=NULL;
   else
    //Obtém a lista de objeto de acorodo com o tipo
    lista_obj=obterListaObjetos(tipos_obj[i]);

   if(lista_obj)
   {
    //Obtém o início e o fim da lista para uma varredura
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end)
    {
     //Obtém o objeto atual
     objeto=(*itr);
     //Armazena o objeto em si no mapa de objetos
     mapa_objetos[objeto->getObjectId()]=objeto;
     //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
     vet_id_objs.push_back(objeto->getObjectId());
     itr++;
    }
   }
  }

  /* ** Trecho especial **:
     Obtenção e armazenamento dos objetos especiais (os quais
     referenciam colunas de tabelas adicionadas por relacionamentos)
     no mapa de objetos e seus ids em um vetor auxiliar de ids 'vet_id_objs_tab'.
     Para isso a lista de tabelas é varrida novamente e suas restrições e índices
     são validados como especiais ou não. O vetor de ids é concatenado ao vetor de ids principal
     antes de sua ordenação quando a definição é XML ou concatenado após a ordenação para def. SQL, desta
     forma os objetos especiais são criados de forma correta em ambas as linguagens */
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end)
   {
    tabela=dynamic_cast<Tabela *>(*itr);
    itr++;

    //Varre a lista de restrições da tabela
    qtd=tabela->obterNumRestricoes();
    for(i=0; i < qtd; i++)
    {
     restricao=tabela->obterRestricao(i);

     //Caso a restrição seja um objeto especial armazena o mesmo no mapa de objetos
     if(!restricao->isAddedByLinking() &&
         restricao->obterTipoRestricao()!=TipoRestricao::primary_key &&
         restricao->referenciaColunaIncRelacao())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[restricao->getObjectId()]=restricao;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(restricao->getObjectId());
     }
    }

    //Varre a lista de gatilhos da tabela
    qtd=tabela->obterNumGatilhos();
    for(i=0; i < qtd; i++)
    {
     gatilho=tabela->obterGatilho(i);

     //Caso o gatilho seja um objeto especial armazena-o no mapa de objetos
     if(gatilho->isReferRelationshipColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[gatilho->getObjectId()]=gatilho;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(gatilho->getObjectId());
     }
    }

    //Varre a lista de índices da tabela
    qtd=tabela->obterNumIndices();
    for(i=0; i < qtd; i++)
    {
     indice=tabela->obterIndice(i);

     //Caso o índice seja um objeto especial armazena-o no mapa de objetos
     if(indice->isReferRelationshipColumn())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[indice->getObjectId()]=indice;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs_tab.push_back(indice->getObjectId());
     }
    }
   }

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     antes da ordenação caso a definição seja XML */
  if(tipo_def==SchemaParser::XML_DEFINITION)
   vet_id_objs.insert(vet_id_objs.end(), vet_id_objs_tab.begin(), vet_id_objs_tab.end());

  //Ordena o vetor de identificadores em ordem crescente
  sort(vet_id_objs.begin(), vet_id_objs.end());

  /* CASO ESPECIAL: Gerando a SQL de tabelas, visões, relacionamentos e sequencias de forma ordenada.

     Esta geração é feita da seguinte forma:
     1) Com a lista de relacionamentos, gera o SQL de ambas as tabelas do relacionametno e logo em seguida a
        SQL do relacionametno em si.
     2) Para as demais tabelas que não participaram de relacionamentos, gerar o SQL de cada uma.
     3) As sequencias devem ter seus SQL gerados após as tabelas restantes
     4) Visões devem ser as últimas a terem o SQL gerado para não terem referências a colunas e tabelas quebradas
  */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   BaseObject *objetos[3]={NULL, NULL, NULL};
   vector<BaseObject *> vet_aux;
   qtd=16;

   vet_aux=relacionamentos;
   vet_aux.insert(vet_aux.end(), tabelas.begin(),tabelas.end());
   vet_aux.insert(vet_aux.end(), sequencias.begin(),sequencias.end());
   vet_aux.insert(vet_aux.end(), visoes.begin(),visoes.end());;
   itr=vet_aux.begin();
   itr_end=vet_aux.end();

   while(itr!=itr_end)
   {
    objeto=(*itr);
    itr++;

    if(objeto->getObjectType()==OBJ_RELATIONSHIP)
    {
     relacao=dynamic_cast<Relacionamento *>(objeto);
     objetos[0]=relacao->obterTabela(Relacionamento::TABELA_ORIGEM);
     objetos[1]=relacao->obterTabela(Relacionamento::TABELA_DESTINO);
     objetos[2]=relacao;

     for(i=0; i < 3; i++)
     {
      if(mapa_objetos.count(objetos[i]->getObjectId())==0)
      {
       mapa_objetos[objetos[i]->getObjectId()]=objetos[i];
       vet_id_objs.push_back(objetos[i]->getObjectId());
      }
     }
    }
    else
    {
     if(mapa_objetos.count(objeto->getObjectId())==0)
     {
      mapa_objetos[objeto->getObjectId()]=objeto;
      vet_id_objs.push_back(objeto->getObjectId());
     }
    }
   }
  }

  /* Concatena o vetor de ids auxiliar (ids de objetos especiais) ao vetor de ids principal
     após a ordenação caso a definição seja SQL */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   vet_id_objs.insert(vet_id_objs.end(), vet_id_objs_tab.begin(), vet_id_objs_tab.end());

  atribs_aux[ParsersAttributes::SHELL_TYPES]="";

  /* Caso a definição seja SQL e existam tipos definidos pelo usuário
     faz a conversão dos parâmetros das funções usadas internamente
     por estes */
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   itr=tipos.begin();
   itr_end=tipos.end();

   while(itr!=itr_end)
   {
    tipo_usr=dynamic_cast<Tipo *>(*itr);
    itr++;

    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     tipo_usr->converterParametrosFuncoes();
   }
  }

  //Varre a lista de ids de objetos para obtê-los do mapa de objetos
  itr1=vet_id_objs.begin();
  itr1_end=vet_id_objs.end();

  atrib=ParsersAttributes::OBJECTS;
  while(itr1!=itr1_end)
  {
   /* Obtém o objeto do mapa a partir do seu identificador
      armazenado no iterador atual */
   objeto=mapa_objetos[(*itr1)];

   //Obtém o tipo do objeto
   tipo_obj=objeto->getObjectType();
   itr1++;

   /* Caso seja um objeto tipo e a definição seja SQL armazena a
      definição shell dos tipos os quais são declarados antes da criação
      dos tipos definidos pelo usuário */
   if(tipo_obj==OBJ_TYPE && tipo_def==SchemaParser::SQL_DEFINITION)
   {
    tipo_usr=dynamic_cast<Tipo *>(objeto);

    //Obtendo a definição do tipo em forma de shell type
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     atribs_aux[ParsersAttributes::SHELL_TYPES]+=tipo_usr->getCodeDefinition(tipo_def, true);
    else
     atribs_aux[atrib]+=tipo_usr->getCodeDefinition(tipo_def);
   }
   else if(tipo_obj==OBJ_DATABASE)
   {
    if(tipo_def==SchemaParser::SQL_DEFINITION)
     atribs_aux[this->getSchemaName()]+=this->__obterDefinicaoObjeto(tipo_def);
    else
     atribs_aux[atrib]+=this->__obterDefinicaoObjeto(tipo_def);
   }
   else if(tipo_obj==OBJ_CONSTRAINT)
   {
    atribs_aux[atrib]+=dynamic_cast<Restricao *>(objeto)->getCodeDefinition(tipo_def, true);
   }
   else
   {
    /* Desprezando as linguagens c e sql
       pois as mesmas não precisam ser declaradas explicitamente poir serem built-in */
    if(//tipo_def==ParserEsquema::DEFINICAO_SQL &&
       (tipo_obj==OBJ_LANGUAGE &&
        (objeto->getName()==~TipoLinguagem("c") ||
         objeto->getName()==~TipoLinguagem("sql") ||
         objeto->getName()==~TipoLinguagem("plpgsql"))))
     atribs_aux[atrib]+="";
    else
     atribs_aux[atrib]+=validarDefinicaoObjeto(objeto, tipo_def);
   }

   //Dispara um sinal para sinalizar o progresso da geração do códgio
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8(objeto->getName()))
                              .arg(objeto->getTypeName()),
                           objeto->getObjectType());
   }
  }

  //Gerando a definição sql/xml das permissões
  itr=permissoes.begin();
  itr_end=permissoes.end();
  atribs_aux[ParsersAttributes::PERMISSION]="";

  while(itr!=itr_end)
  {
   atribs_aux[ParsersAttributes::PERMISSION]+=dynamic_cast<Permissao *>(*itr)->getCodeDefinition(tipo_def);

   //Dispara um sinal para sinalizar o progresso final da geração de código
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8((*itr)->getName()))
                              .arg(objeto->getTypeName()),
                           objeto->getObjectType());
   }

   itr++;
  }

  //Configura os atributos específicos do modelo de banco
  atribs_aux[ParsersAttributes::MODEL_AUTHOR]=autor;

  if(tipo_def==SchemaParser::XML_DEFINITION)
  {
   atribs_aux[ParsersAttributes::PROTECTED]=(this->is_protected ? "1" : "");
  }
  else
  {
   /* Ao final da obtenção da definição SQL do modelo, faz a conversão inversa
      dos parâmetros das funções usadas pelos tipos base */
   qtd1=tipos.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Tipo *>(tipos[i]);
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
    {
     atribs_aux[atrib]+=tipo_usr->getCodeDefinition(tipo_def);
     tipo_usr->converterParametrosFuncoes(true);
    }
   }
  }
 }
 catch(Exception &e)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   /* Em caso de erro na geração da definição SQL do modelo, faz a conversão inversa
     dos parâmetros das funções usadas pelos tipos base */
   qtd1=tipos.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Tipo *>(tipos[i]);
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     tipo_usr->converterParametrosFuncoes(true);
   }
  }
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }

 //Armazena o atributo que indica se a exportação é para arquivo ou não
 atribs_aux[ParsersAttributes::EXPORT_TO_FILE]=(exportar_arq ? "1" : "");

 //Retorna a definição do modelo completa
 return(SchemaParser::getObjectDefinition(ParsersAttributes::DB_MODEL, atribs_aux, tipo_def));
}

void ModeloBD::salvarModelo(const QString &nome_arq, unsigned tipo_def)
{
 QString str_aux;
 QFile saida(nome_arq);

 saida.open(QFile::WriteOnly);

 //Caso não consiga abrir o arquivo para gravação
 if(!saida.isOpen())
 {
  str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(nome_arq));
  throw Exception(str_aux,ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 try
 {
  str_aux=this->getCodeDefinition(tipo_def);
  saida.write(str_aux.toStdString().c_str(),str_aux.size());
  saida.close();
 }
 catch(Exception &e)
 {
  if(saida.isOpen()) saida.close();
  str_aux=QString(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(nome_arq));
  throw Exception(str_aux,ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ModeloBD::obterDependenciasObjeto(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_dep_indiretas)
{
 //Caso o objeto esteja alocado e o mesmo ainda não exista na lista de dependências
 if(objeto && std::find(vet_deps.begin(), vet_deps.end(), objeto)==vet_deps.end())
 {
  //Adiciona o objeto na lista de dependências
  vet_deps.push_back(objeto);
 }

 if((vet_deps.size()==1 && !inc_dep_indiretas) || inc_dep_indiretas)
 {
  ObjectType tipo_obj=objeto->getObjectType();

  /* Caso o objeto possua esquema, espaço de tabela e dono,
     busca e inclui se necessário as dependências desses
     objetos na lista */
  if(objeto->getSchema() && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->getSchema(), vet_deps, inc_dep_indiretas);

  if(objeto->getTablespace() && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->getTablespace(), vet_deps, inc_dep_indiretas);

  if(objeto->getOwner()  && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->getOwner(), vet_deps, inc_dep_indiretas);

  //** Obtendo as dependências de Classe de Operadores **
  if(tipo_obj==OBJ_OPCLASS)
  {
   OperatorClass *classe_op=dynamic_cast<OperatorClass *>(objeto);

   /* Obtém a referência para o tipo de dado da classe, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
  BaseObject *tipo_usr=obterObjetoTipoPgSQL(classe_op->getDataType());
  //obterObjeto(*classe_op->obterTipoDado(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Caso haja uma família de operadores obtém as dependências dela também
   if(classe_op->getFamily())
    obterDependenciasObjeto(classe_op->getFamily(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Domínios **
  else if(tipo_obj==OBJ_DOMAIN)
  {
   /* Obtém a referência para o tipo de dado do domínio, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
   BaseObject *tipo_usr=obterObjetoTipoPgSQL(dynamic_cast<Domain *>(objeto)->getType());
   //obterObjeto(*dynamic_cast<Dominio *>(objeto)->obterTipo(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Codificação **
  else if(tipo_obj==OBJ_CONVERSION)
  {
   //Obtém as dependências da função de conversão que define a conversão de codificação
   Function *func=dynamic_cast<Conversion *>(objeto)->getConversionFunction();
   obterDependenciasObjeto(func, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Tipo **
  else if(tipo_obj==OBJ_CAST)
  {
   Cast *conv=dynamic_cast<Cast *>(objeto);
   BaseObject *tipo_usr=NULL;

   //Obtém as dependências dos tipos usados na conversão de tipo
   for(unsigned i=Cast::SRC_TYPE; i <= Cast::DST_TYPE; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(conv->getDataType(i));
      //obterObjeto(*conv->obterTipoDado(i), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências da função de conversão que define a conversão de tipo
   obterDependenciasObjeto(conv->getCastFunction(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Funções **
  else if(tipo_obj==OBJ_FUNCTION)
  {
   Function *func=dynamic_cast<Function *>(objeto);
   BaseObject *tipo_usr=obterObjetoTipoPgSQL(func->getReturnType());
     //obterObjeto(*func->obterTipoRetorno(), OBJETO_TIPO);
   unsigned qtd, i;

   //Caso a linguagem da função não seja C ou SQL obtém as dependências da mesma
   if(func->getLanguage()->getName()!=~TipoLinguagem("c") &&
      func->getLanguage()->getName()!=~TipoLinguagem("sql"))
    obterDependenciasObjeto(func->getLanguage(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências do tipo de retorno caso o mesmo seja um tipo definido pelo usuário
   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos dos parâmetros
   qtd=func->getParameterCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(func->getParameter(i).getType());
    //obterObjeto(*func->obterParametro(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos elementos da tabela de retorno
   qtd=func->getReturnedTableColumnCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(func->getReturnedTableColumn(i).getType());
    //obterObjeto(*func->obterTipoRetTabela(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Funções de Agregação **
  else if(tipo_obj==OBJ_AGGREGATE)
  {
   Aggregate *func=dynamic_cast<Aggregate *>(objeto);
   BaseObject *tipo_usr=NULL;
   unsigned qtd, i;

   //Obtém as dependências das funções que definem a função de agregação
   for(i=Aggregate::FINAL_FUNC; i <= Aggregate::TRANSITION_FUNC; i++)
    obterDependenciasObjeto(func->getFunction(i), vet_deps, inc_dep_indiretas);

   //Obtém a dependência do tipo de estado da função de agregação
   tipo_usr=obterObjetoTipoPgSQL(func->getStateType());
     //obterObjeto(*func->obterTipoEstado(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências do operador de ordenação caso este esteja alocado
   if(func->getSortOperator())
    obterDependenciasObjeto(func->getSortOperator(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos de dados usados na função de agregação
   qtd=func->getDataTypeCount();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(func->getDataType(i));
      //obterObjeto(*func->obterTipoDado(i), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Linguagens **
  else if(tipo_obj==OBJ_LANGUAGE)
  {
   Language *ling=dynamic_cast<Language *>(objeto);

   for(unsigned i=Language::VALIDATOR_FUNC; i <= Language::INLINE_FUNC; i++)
   {
    if(ling->getFunction(i))
     obterDependenciasObjeto(ling->getFunction(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Operadores **
  else if(tipo_obj==OBJ_OPERATOR)
  {
   Operator *oper=dynamic_cast<Operator *>(objeto);
   BaseObject *tipo_usr=NULL;
   unsigned i;

   //Obtém as dependências das funções usadas pelo operador
   for(i=Operator::FUNC_OPERADOR; i <= Operator::FUNC_RESTRICAO; i++)
   {
    if(oper->obterFuncao(i))
     obterDependenciasObjeto(oper->obterFuncao(i), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos tipos dos argumentos do operador
   for(i=Operator::ARG_ESQUERDA; i <= Operator::ARG_DIREITA; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(oper->obterTipoDadoArgumento(i));
      //obterObjeto(*oper->obterTipoDadoArgumento(i), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos operadores auxiliares
   for(i=Operator::OPER_COMUTACAO; i <= Operator::OPER_MAIOR; i++)
   {
    if(oper->obterOperador(i))
     obterDependenciasObjeto(oper->obterOperador(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Papéis **
  else if(tipo_obj==OBJ_ROLE)
  {
   Papel *papel=dynamic_cast<Papel *>(objeto);
   unsigned i, i1, qtd,
            tipos[3]={ Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN };

   //Obtém as dependências dos papéis membros, papéis admins e papéis referenciados
   for(i=0; i < 3; i++)
   {
    qtd=papel->obterNumPapeis(tipos[i]);
    for(i1=0; i1 < qtd; i1++)
     obterDependenciasObjeto(papel->obterPapel(tipos[i], i1), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Relacionamentos **
  else if(tipo_obj==OBJ_RELATIONSHIP)
  {
   Relacionamento *rel=dynamic_cast<Relacionamento *>(objeto);
   BaseObject *tipo_usr=NULL;
   Restricao *rest=NULL;
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pelo relacionamento
   obterDependenciasObjeto(rel->obterTabela(Relacionamento::TABELA_ORIGEM), vet_deps, inc_dep_indiretas);
   obterDependenciasObjeto(rel->obterTabela(Relacionamento::TABELA_DESTINO), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos usados pelos atributos do relacionamento
   qtd=rel->obterNumAtributos();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjetoTipoPgSQL(rel->obterAtributo(i)->getType());
      //obterObjeto(*rel->obterAtributo(i)->obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições (somente chave estrangeira) do relacionamento
   qtd=rel->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Restricao *>(rel->obterRestricao(i));
    if(rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     obterDependenciasObjeto(rest->obterTabReferenciada(), vet_deps, inc_dep_indiretas);

    if(rest->getTablespace())
     obterDependenciasObjeto(rest->getTablespace(), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Sequências **
  else if(tipo_obj==OBJ_SEQUENCE)
  {
   Sequencia *seq=dynamic_cast<Sequencia *>(objeto);
   if(seq->obterPossuidora())
    obterDependenciasObjeto(seq->obterPossuidora()->getParentTable(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Tabelas **
  else if(tipo_obj==OBJ_TABLE)
  {
   Tabela *tab=dynamic_cast<Tabela *>(objeto);
   BaseObject *tipo_usr=NULL;
   Restricao *rest=NULL;
   Gatilho *gat=NULL;
   Index *ind=NULL;
   Column *col=NULL;
   unsigned qtd, qtd1, i, i1;

   //Obtém as dependências dos tipos das colunas não incluídas por relacionamento
   qtd=tab->obterNumColunas();
   for(i=0; i < qtd; i++)
   {
    col=tab->obterColuna(i);
    tipo_usr=obterObjetoTipoPgSQL(col->getType());
      //obterObjeto(*col->obterTipo(), OBJETO_TIPO);

    if(!col->isAddedByLinking() && tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições não incluídas por relacionamento
   qtd=tab->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Restricao *>(tab->obterRestricao(i));
    if(!rest->isAddedByLinking() &&
        rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     obterDependenciasObjeto(rest->obterTabReferenciada(), vet_deps, inc_dep_indiretas);

    if(!rest->isAddedByLinking() && rest->getTablespace())
     obterDependenciasObjeto(rest->getTablespace(), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das tabelas referenciadas nos gatilhos e as funções
   qtd=tab->obterNumGatilhos();
   for(i=0; i < qtd; i++)
   {
    gat=dynamic_cast<Gatilho *>(tab->obterGatilho(i));
    if(gat->getReferencedTable())
     obterDependenciasObjeto(gat->getReferencedTable(), vet_deps, inc_dep_indiretas);

    if(gat->getFunction())
     obterDependenciasObjeto(gat->getFunction(), vet_deps, inc_dep_indiretas);

    //qtd1=gat->obterNumColunas();
    //for(i1=0; i1 < qtd1; i1++)
    //obterDependenciasObjeto(gat->obterColuna(i1), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das colunas ou classe de operadores usados nos elementos do índices
   qtd=tab->obterNumIndices();
   for(i=0; i < qtd; i++)
   {
    ind=dynamic_cast<Index *>(tab->obterIndice(i));
    qtd1=ind->getElementCount();

    for(i1=0; i1 < qtd1; i1++)
    {
     if(ind->getElement(i1).getOperatorClass())
      obterDependenciasObjeto(ind->getElement(i1).getOperatorClass(), vet_deps, inc_dep_indiretas);
     else if(ind->getElement(i1).getColumn())
     {
      tipo_usr=obterObjetoTipoPgSQL(ind->getElement(i1).getColumn()->getType());
        //obterObjeto(*ind->obterElemento(i1).obterColuna()->obterTipo(), OBJETO_TIPO);

      if(tipo_usr)
       obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
     }
    }
   }
  }
  //** Obtendo as dependências de Tipos definidos pelo usuário **
  else if(tipo_obj==OBJ_TYPE)
  {
   Tipo *tipo_usr=dynamic_cast<Tipo *>(objeto);
   BaseObject *tipo_aux=NULL;
   unsigned qtd, i;

   //Caso o tipo de usuário seja tipo base
   if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
   {
    //Obtém as dependências do tipo de cópia
    tipo_aux=obterObjetoTipoPgSQL(tipo_usr->obterTipoCopia());
      //obterObjeto(*tipo_usr->obterTipoCopia(), OBJETO_TIPO);

    if(tipo_aux)
     obterDependenciasObjeto(tipo_aux, vet_deps, inc_dep_indiretas);

    //Obtém as dependências das funções usadas pelo tipo
    for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
     obterDependenciasObjeto(tipo_usr->obterFuncao(i), vet_deps, inc_dep_indiretas);
   }
   //Caso seja um tipo composto
   else if(tipo_usr->obterConfiguracao()==Tipo::TIPO_COMPOSTO)
   {
    //Obtém as dependências dos tipos dos atributos
    qtd=tipo_usr->obterNumAtributos();
    for(i=0; i < qtd; i++)
    {
     tipo_aux=obterObjetoTipoPgSQL(tipo_usr->obterAtributo(i).getType());
       //obterObjeto(*tipo_usr->obterAtributo(i).obterTipo(), OBJETO_TIPO);

     if(tipo_aux)
      obterDependenciasObjeto(tipo_aux, vet_deps, inc_dep_indiretas);
    }
   }
  }
  //** Obtendo as dependências de Visões **
  else if(tipo_obj==OBJ_VIEW)
  {
   Visao *visao=dynamic_cast<Visao *>(objeto);
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pela visão
   qtd=visao->obterNumReferencias();
   for(i=0; i < qtd; i++)
   {
    if(visao->obterReferencia(i).obterTabela())
     obterDependenciasObjeto(visao->obterReferencia(i).obterTabela(), vet_deps, inc_dep_indiretas);
   }
  }
 }
}

void ModeloBD::obterReferenciasObjeto(BaseObject *objeto, vector<BaseObject *> &vet_refs, bool modo_exclusao)
{
 vet_refs.clear();

 if(objeto)
 {
  ObjectType tipo_obj=objeto->getObjectType();
  bool refer=false;

  if(tipo_obj==OBJ_TABLE)
  {
   Tabela *tabela=dynamic_cast<Tabela *>(objeto);
   Sequencia *seq=NULL;
   Restricao *rest=NULL;
   Tabela *tab=NULL;
   Gatilho *gat=NULL;
   RelacionamentoBase *rel_base=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, qtd;

   /* Vericando se existe algum relacionamento (tabela-tabela)
      o qual um dos objetos participantes é a tabela
      do parâmetro */
   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    if(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==tabela ||
       rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base);
    }
    itr++;
   }

   /* Vericando se existe alguma sequência a qual referencia
      a tabela do parâmetro */
   itr=sequencias.begin();
   itr_end=sequencias.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    seq=dynamic_cast<Sequencia *>(*itr);
    if(seq->obterPossuidora() &&
       seq->obterPossuidora()->getParentTable()==tabela)
    {
     refer=true;
     vet_refs.push_back(seq);
    }

    itr++;
   }

   /* Vericando se existe alguma tabela cuja uma tabela cópia é
      a própria tabela a ser excluída, verifica também se as
      restrições das tabelas, especificamente, as chaves estrangeiras
      estão referenciado a tabela */
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    tab=dynamic_cast<Tabela *>(*itr);

    //Verificando as restrições
    qtd=tab->obterNumRestricoes();
    for(i=0; i < qtd&& (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->obterRestricao(i);
     if(rest->obterTipoRestricao()==TipoRestricao::foreign_key &&
        rest->obterTabReferenciada()==tabela)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    //Verificando os gatilhos
    qtd=tab->obterNumGatilhos();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     gat=tab->obterGatilho(i);
     if(gat->getReferencedTable()==tabela)
     {
      refer=true;
      vet_refs.push_back(gat);
     }
    }

    itr++;
   }

   /* Vericando se existe algum relacionamento (tabela-visao)
      o qual um dos objetos participantes é a tabela */
   itr=relac_visoes.begin();
   itr_end=relac_visoes.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    if(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO));
    }
    else if(rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM));
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_FUNCTION)
  {
   Function *funcao=dynamic_cast<Function *>(objeto);
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[7]={OBJ_CAST, OBJ_CONVERSION,
                            OBJ_AGGREGATE, OBJ_OPERATOR,
                            OBJ_TABLE, OBJ_TYPE, OBJ_LANGUAGE };
   unsigned i, i1, qtd;
   Tabela *tab=NULL;
   Aggregate *func_ag=NULL;
   Operator *oper=NULL;
   Gatilho *gat=NULL;
   Tipo *tipo=NULL;
   Language *ling=NULL;

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente uma função */
   for(i=0; i < 7 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_CAST)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<Cast *>(*itr)->getCastFunction()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_CONVERSION)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<Conversion *>(*itr)->getConversionFunction()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      func_ag=dynamic_cast<Aggregate *>(*itr);
      //Verifica se o objeto não referencia o papel
      if(func_ag->getFunction(Aggregate::FINAL_FUNC)==funcao ||
         func_ag->getFunction(Aggregate::TRANSITION_FUNC)==funcao)
      {
       refer=true;
       vet_refs.push_back(func_ag);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);

      //Verifica se o objeto não referencia o papel
      if(oper->obterFuncao(Operator::FUNC_OPERADOR)==funcao ||
         oper->obterFuncao(Operator::FUNC_JUNCAO)==funcao  ||
         oper->obterFuncao(Operator::FUNC_RESTRICAO)==funcao)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_TABLE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tab=dynamic_cast<Tabela *>(*itr);
      itr++;
      qtd=tab->obterNumGatilhos();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       gat=tab->obterGatilho(i1);
       //Verifica se o gatilho não referencia a função
       if(gat->getFunction()==funcao)
       {
        refer=true;
        vet_refs.push_back(gat);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_TYPE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Tipo *>(*itr);
      itr++;

      for(i1=Tipo::FUNCAO_INPUT; i1 <= Tipo::FUNCAO_ANALYZE && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o tipo não referencia a função
       if(tipo->obterFuncao(i1)==funcao)
       {
        refer=true;
        vet_refs.push_back(tipo);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_LANGUAGE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      ling=dynamic_cast<Language *>(*itr);
      itr++;
      //Verifica se a função a ser removida é uma função handler ou validator da linguagem
      if(ling->getFunction(Language::HANDLER_FUNC)==funcao ||
         ling->getFunction(Language::VALIDATOR_FUNC)==funcao ||
         ling->getFunction(Language::INLINE_FUNC)==funcao)
      {
       refer=true;
       vet_refs.push_back(ling);
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_SCHEMA)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[11]={OBJ_FUNCTION, OBJ_TABLE, OBJ_VIEW,
                             OBJ_DOMAIN, OBJ_AGGREGATE, OBJ_OPERATOR,
                             OBJ_SEQUENCE, OBJ_CONVERSION,
                             OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
   unsigned i;

   /* Varre todas as listas de objetos os quais pode pertencer a
      um esquema e verifica se o mesmos não estão referenciando
      o esquema a ser removido */
   for(i=0; i < 11 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o esquema
     if((*itr)->getSchema()==objeto)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }
  }

  if(tipo_obj==OBJ_TYPE || tipo_obj==OBJ_DOMAIN ||
     tipo_obj==OBJ_SEQUENCE || tipo_obj==OBJ_TABLE)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[]={OBJ_TABLE, OBJ_OPCLASS, OBJ_CAST,
                           OBJ_DOMAIN, OBJ_FUNCTION, OBJ_AGGREGATE,
                           OBJ_OPERATOR, OBJ_TYPE };
   unsigned i, i1, qtd;
   OperatorClass *classe_op=NULL;
   Tabela *tab=NULL;
   Column *col=NULL;
   Cast *conv_tipo=NULL;
   Domain *dom=NULL;
   Function *func=NULL;
   Aggregate *func_ag=NULL;
   Operator *oper=NULL;
   Tipo *tipo=NULL;
   void *ptr_tipopgsql=NULL;

   /* Devido a particuladade de se atribuir ponteiros de objetos como tipos base do pgsql (ver TipoPgSQL)
      é necessário fazer um dynamic_cast para a classe correta do 'objeto'. Caso o dynamic_cast não seja
      feita, mesmo que o objeto seja um tipopgsql válido o mesmo não será localizado na lista de tipos
      base do pgsql.  */
   switch(tipo_obj)
   {
    case OBJ_TYPE: ptr_tipopgsql=dynamic_cast<Tipo*>(objeto); break;
    case OBJ_DOMAIN: ptr_tipopgsql=dynamic_cast<Domain*>(objeto); break;
    case OBJ_SEQUENCE: ptr_tipopgsql=dynamic_cast<Sequencia*>(objeto); break;
    default: ptr_tipopgsql=dynamic_cast<Tabela*>(objeto); break;
   }

   /* Varre todas as listas de objetos os quais podem
     referenciar direta ou indiretamente um tipo definido
     pelo usuário */
   for(i=0; i < 8 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_TABLE)
    {
     //Verifica se as colunas das tabelas não referenciam o tipo
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a tabela
      tab=dynamic_cast<Tabela *>(*itr);
      itr++;

      //Varre a lista de colunas da tabela
      qtd=tab->obterNumColunas();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       col=tab->obterColuna(i1);
       //Verifica se o tipo da coluna é o próprio tipo a ser excluído
       if(col->getType()==objeto)
       {
        refer=true;
        vet_refs.push_back(col);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_OPCLASS)
    {
     /* Varre a lista de classe operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Verifica se o tipo de dado da classe é o próprio tipo a ser removido
      if(classe_op->getDataType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(classe_op);
      }
     }
    }
    else if(tipos[i]==OBJ_DOMAIN)
    {
     /* Varre a lista de domínios e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      dom=dynamic_cast<Domain *>(*itr);
      itr++;

      //Verifica se o tipo de dado do dominio é o próprio tipo a ser removido
      if(dom->getType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(dom);
      }
     }
    }
    else if(tipos[i]==OBJ_TYPE)
    {
     /* Varre a lista de tipo definidos pelo usuário e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Tipo *>(*itr);
      itr++;

      if(tipo->obterAlinhamento()==ptr_tipopgsql ||
         tipo->obterElemento()==ptr_tipopgsql ||
         tipo->obterTipoCopia()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(tipo);
      }
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     /* Varre a lista de funções agregadas e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func_ag=dynamic_cast<Aggregate *>(*itr);
      itr++;

      /* Verifica se os tipos de dados da função agregada referenciam
         tipo a ser removido */
      qtd=func_ag->getDataTypeCount();
      for(i1=0; i1 < qtd  && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(func_ag->getDataType(i1)==ptr_tipopgsql)
       {
        refer=true;
        vet_refs.push_back(func_ag);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_FUNCTION)
    {
     /* Varre a lista de funções e verifica se algum
        objeto (parâmetros e tipo de retorno) referencia
        o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func=dynamic_cast<Function *>(*itr);
      itr++;

      //Verifica se o tipo de retorno é o próprio tipo a ser removido
      if(func->getReturnType()==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(func);
      }
      else
      {
       /* Verifica se os tipos de dados dos parâmetros da função
          referenciam tipo a ser removido */
       qtd=func->getParameterCount();
       for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(func->getParameter(i1).getType()==ptr_tipopgsql)
        {
         refer=true;
         vet_refs.push_back(func);
        }
       }
      }
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     /* Varre a lista de operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);
      itr++;

      //Verifica se um dos argumentos do operador é o próprio tipo a ser removido
      if(oper->obterTipoDadoArgumento(Operator::ARG_ESQUERDA)==ptr_tipopgsql ||
         oper->obterTipoDadoArgumento(Operator::ARG_DIREITA)==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
     }
    }
    else if(tipos[i]==OBJ_CAST)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      conv_tipo=dynamic_cast<Cast *>(*itr);
      itr++;

      //Verifica se o objeto não referencia o tipo
      if(conv_tipo->getDataType(Cast::SRC_TYPE)==ptr_tipopgsql ||
         conv_tipo->getDataType(Cast::DST_TYPE)==ptr_tipopgsql)
      {
       refer=true;
       vet_refs.push_back(conv_tipo);
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_ROLE)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[13]={OBJ_FUNCTION, OBJ_TABLE, OBJ_DOMAIN,
                             OBJ_AGGREGATE, OBJ_SCHEMA, OBJ_OPERATOR,
                             OBJ_SEQUENCE, OBJ_CONVERSION,
                             OBJ_LANGUAGE, OBJ_TABLESPACE,
                             OBJ_TYPE, OBJ_OPFAMILY, OBJ_OPCLASS};
   unsigned i,i1, qtd;
   Papel *papel_aux=NULL;
   Papel *papel=dynamic_cast<Papel *>(objeto);
   unsigned tipo_papel[3]={Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN};

   /* Caso especial: Varre a lista de papéis e verifica se o papel a ser
      removido se encontra em uma das três listas de papeis da classe
      Papel (ref, membro ou admin) */
   itr=papeis.begin();
   itr_end=papeis.end();
   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém um papel
    papel_aux=dynamic_cast<Papel *>(*itr);
    itr++;

    for(i1=0; i1 < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
    {
     //Obtém a quantidade de papeis presentes na lista atual (tipo_papel[i1])
     qtd=papel_aux->obterNumPapeis(tipo_papel[i1]);
     for(i=0; i < qtd && !refer; i++)
     {
      /* Caso o papel a ser excluído seja igual ao elemento atual
         da lista de papéis do papel aux */
      if(papel_aux->obterPapel(tipo_papel[i1], i)==papel)
      {
       refer=true;
       vet_refs.push_back(papel_aux);
      }
     }
    }
   }

   /* Varre todas as listas de objetos os quais pode pertencer a
      um dono e verifica se o mesmos não estão referenciando
      o dono a ser removido */
   for(i=0; i < 13 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o papel
     if((*itr)->getOwner()==papel)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }

   /*Caso especial: Verifica se o papel é a ser removido é dono
     do próprio banco de dados */
   if((!modo_exclusao || (modo_exclusao && !refer)) && this->getOwner()==papel)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }

  if(tipo_obj==OBJ_TABLESPACE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   unsigned i, qtd;
   Tabela *tab=NULL;
   Index *ind=NULL;
   Restricao *rest=NULL;

   /* Varre a lista de tabelas e verifica se estas e seus índices
      não estão referenciando o espaço de tabelas a ser removido */
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém a tabela
    tab=dynamic_cast<Tabela *>(*itr);

    //Verifica se referencia o espaço de tabela
    if(tab->getTablespace()==objeto)
    {
     refer=true;
     vet_refs.push_back(tab);
    }


    /* Verifica se algum dos índices da tabela referecia
       o espaço de tabela */
    qtd=tab->obterNumIndices();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     ind=tab->obterIndice(i);
     if(ind->getTablespace()==objeto)
     {
      refer=true;
      vet_refs.push_back(ind);
     }
    }

    /* Verifica se alguma restrição da tabela referecia
       o espaço de tabela */
    qtd=tab->obterNumRestricoes();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->obterRestricao(i);
     if(rest->getTablespace()==objeto)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    itr++;
   }

   if((!modo_exclusao || (modo_exclusao && !refer)) && this->BaseObject::getTablespace()==objeto)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }

  if(tipo_obj==OBJ_LANGUAGE)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   Function *func=NULL;

   /* Varre a lista de funções e verifica se estas
      não estão referenciando a linguage a ser removida */
   itr=funcoes.begin();
   itr_end=funcoes.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Verifica se referencia o espaço de tabela
    func=dynamic_cast<Function *>(*itr);
    if(func->getLanguage()==objeto)
    {
     refer=true;
     vet_refs.push_back(func);
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_OPERATOR)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[3]={OBJ_OPCLASS,
                            OBJ_AGGREGATE,
                            OBJ_OPERATOR};
   unsigned i, i1, qtd;
   OperatorClass *classe_op=NULL;
   Operator *oper=NULL, *operador=dynamic_cast<Operator *>(objeto);

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente um operador */
   for(i=0; i < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJ_OPCLASS)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<OperatorClass *>(*itr);
      itr++;

      //Varre a lista de elementos da classe de operadores
      qtd=classe_op->getElementCount();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o objeto não referencia o operador
       if(classe_op->getElement(i1).getOperator()==operador)
       {
        refer=true;
        vet_refs.push_back(classe_op);
       }
      }
     }
    }
    else if(tipos[i]==OBJ_AGGREGATE)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      if(dynamic_cast<Aggregate *>(*itr)->getSortOperator()==operador)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJ_OPERATOR)
    {
     while(itr!=itr_end && !refer)
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operator *>(*itr);
      itr++;

      /* Verifica se um dos operadores agregados ao operador atual não referencia
         o objeto não referencia o operador */
      for(i1=Operator::OPER_COMUTACAO; i1 <= Operator::OPER_MAIOR &&
            (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(oper->obterOperador(i1)==operador)
       {
        refer=true;
        vet_refs.push_back(oper);
       }
      }
     }
    }
   }
  }

  if(tipo_obj==OBJ_OPFAMILY)
  {
   vector<BaseObject *>::iterator itr, itr_end;
   OperatorFamily *familia_op=dynamic_cast<OperatorFamily *>(objeto);

   /* Varre a lista de classe de operadores e verifica se um
      de seus elementos está referenciando a família de operadores */
   itr=classe_ops.begin();
   itr_end=classe_ops.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    if(dynamic_cast<OperatorClass *>(*itr)->getFamily()==familia_op)
    {
     refer=true;
     vet_refs.push_back(*itr);
    }
    itr++;
   }
  }

  if(tipo_obj==OBJ_COLUMN)
  {
   Column *coluna=dynamic_cast<Column *>(objeto);
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType  tipos_obj[]={ OBJ_SEQUENCE, OBJ_VIEW, OBJ_TABLE, OBJ_RELATIONSHIP };
   unsigned i, qtd=4;

   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém a lista do tipo atual
    lista_obj=obterListaObjetos(tipos_obj[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    //Varre a lista selecionada verificado se a coluna passada é referenciada
    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     /* Caso 1: o tipo atual seja sequência, faz o cast para a classe
        e chama do método da sequencia que retorna a coluna possuidora
        da mesma.

        Caso 2: o tipo atual seja visão, faz o cast para a classe
        e chama do método da visão o qual retorna se a coluna
        é referenciada pelo elementos da visão. */
     if((tipos_obj[i]==OBJ_SEQUENCE && dynamic_cast<Sequencia *>(*itr)->obterPossuidora()==coluna) ||
        (tipos_obj[i]==OBJ_VIEW && dynamic_cast<Visao *>(*itr)->referenciaColuna(coluna)))
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     else if(tipos_obj[i]==OBJ_TABLE)
     {
      Tabela *tab=dynamic_cast<Tabela *>(*itr);
      unsigned qtd_gat, qtd_rest, idx, qtd1, i1;
      Gatilho *gat=NULL;

      qtd_rest=tab->obterNumRestricoes();
      for(idx=0; idx < qtd_rest && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       if(tab->obterRestricao(idx)->colunaExistente(coluna, Restricao::COLUNA_ORIGEM) ||
          tab->obterRestricao(idx)->colunaExistente(coluna, Restricao::COLUNA_REFER))
       {
        refer=true;
        vet_refs.push_back(tab->obterRestricao(idx));
       }
      }

      qtd_gat=tab->obterNumGatilhos();
      for(idx=0; idx < qtd_gat && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       gat=tab->obterGatilho(idx);
       qtd1=gat->getColumnCount();

       for(i1=0; i1 < qtd1 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(gat->getColumn(i1)==coluna)
        {
         refer=true;
         vet_refs.push_back(gat);
        }
       }
      }
     }
     else if(tipos_obj[i]==OBJ_RELATIONSHIP)
     {
      Relacionamento *rel=dynamic_cast<Relacionamento *>(*itr);
      unsigned qtd_rest, idx;

      qtd_rest=rel->obterNumRestricoes();
      for(idx=0; idx < qtd_rest && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       if(rel->obterRestricao(idx)->colunaExistente(coluna, Restricao::COLUNA_ORIGEM) ||
          rel->obterRestricao(idx)->colunaExistente(coluna, Restricao::COLUNA_REFER))
       {
        refer=true;
        vet_refs.push_back(rel);
       }
      }
     }

     itr++;
    }
   }
  }
 }
}

void ModeloBD::definirObjetosModificados(void)
{
 ObjectType tipos[]={OBJ_TABLE, OBJ_VIEW,
                         OBJ_RELATIONSHIP, BASE_RELATIONSHIP,
                         OBJ_TEXTBOX};
 vector<BaseObject *>::iterator itr, itr_end;
 vector<BaseObject *> *lista_obj=NULL;
 Textbox *rot=NULL;
 RelacionamentoBase *rel=NULL;
 unsigned i, i1;

 for(i=0; i < 5; i++)
 {
  lista_obj=obterListaObjetos(tipos[i]);
  itr=lista_obj->begin();
  itr_end=lista_obj->end();

  while(itr!=itr_end)
  {
   dynamic_cast<BaseGraphicObject *>(*itr)->setModified(true);

   /* Caso especial: Caso o objeto seja um relacionamento, os rótulos
      do mesmo que são caixas de texto, devem também ser marcados
      como modificado */
   if(tipos[i]==OBJ_RELATIONSHIP || tipos[i]==BASE_RELATIONSHIP)
   {
    rel=dynamic_cast<RelacionamentoBase *>(*itr);
    for(i1=0; i1 < 3; i1++)
    {
     rot=rel->obterRotulo(i1);
     if(rot) rot->setModified(true);
    }
   }

   itr++;
  }
 }
}

BaseObject *ModeloBD::obterObjetoTipoPgSQL(TipoPgSQL tipo)
{
 switch(tipo.obterConfTipoUsuario())
 {
  case ConfigTipoUsuario::TIPO_BASE:
   return(this->obterObjeto(*tipo, OBJ_TYPE));
  break;

  case ConfigTipoUsuario::TIPO_DOMINIO:
   return(this->obterObjeto(*tipo, OBJ_DOMAIN));
  break;

  case ConfigTipoUsuario::TIPO_TABELA:
   return(this->obterObjeto(*tipo, OBJ_TABLE));
  break;

  case ConfigTipoUsuario::TIPO_SEQUENCIA:
   return(this->obterObjeto(*tipo, OBJ_SEQUENCE));
  break;

  default:
   return(NULL);
  break;
 }
}

