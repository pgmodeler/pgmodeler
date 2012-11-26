#include "relacionamento.h"
#include <QApplication>

//Inicialização de atributos estáticos da classe
const QString Relacionamento::SEPARADOR_SUFIXO("_");

Relacionamento::Relacionamento(Relacionamento *relacao) : RelacionamentoBase(relacao)
{
 if(!relacao)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 (*(this))=(*relacao);
}

/*Relacionamento::~Relacionamento(void)
{
 if(!conectado)
 {
  Tabela *tab=this->obterTabelaReceptora();

  while(!restricoes_rel.empty())
  {
   if(tab && tab->obterIndiceObjeto(restricoes_rel.back()) < 0)
    delete(restricoes_rel.back());
   restricoes_rel.pop_back();
  }

  while(!atributos_rel.empty())
  {
   if(tab && tab->obterIndiceObjeto(atributos_rel.back()) < 0)
    delete(atributos_rel.back());
   atributos_rel.pop_back();
  }
 }
}*/

Relacionamento::Relacionamento(/*const QString &nome,*/ unsigned tipo_rel, Tabela *tab_orig,
                               Tabela *tab_dest, bool obrig_orig, bool obrig_dest,
                               bool sufixo_auto, const QString &sufix_orig, const QString &sufix_dest,
                               bool identificador,  bool postergavel, TipoPostergacao tipo_postergacao) :
                RelacionamentoBase(tipo_rel, tab_orig, tab_dest, obrig_orig, obrig_dest)
{
 try
 {
  obj_type=OBJ_RELATIONSHIP;
  QString str_aux;

  if(((tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N) &&
      !this->obterTabelaReferencia()->obterChavePrimaria()) ||
     (tipo_relac==RELACIONAMENTO_NN && (!tab_orig->obterChavePrimaria() || !tab_dest->obterChavePrimaria())))
     throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                           .arg(QString::fromUtf8(obj_name))
                           .arg(QString::fromUtf8(tab_orig->getName(true)))
                           .arg(QString::fromUtf8(tab_dest->getName(true))),
                  ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Atribuindo os sufixos ao relacionamento.
    Sufixos são palavras concatenadas ao final do nome de
    colunas as quais são adicionadas automaticamente nas tabelas
    pelo relacionamento e que participam de chaves estrangeiras */
  this->sufixo_orig=sufix_orig;
   this->sufixo_dest=sufix_dest;
  this->sufixo_auto=sufixo_auto;

  tabela_relnn=NULL;
  fk_rel1n=pk_relident=pk_especial=uq_rel11=NULL;
  this->postergavel=postergavel;
  this->tipo_postergacao=tipo_postergacao;
  this->invalidado=true;

  //Configura o nome do relacionamento conforme o tipo
  if(tipo_rel==RELACIONAMENTO_11)
   str_aux=QApplication::translate("Relacionamento","%1_has_one_%2","",QApplication::UnicodeUTF8);
  else if(tipo_rel==RELACIONAMENTO_1N)
   str_aux=QApplication::translate("Relacionamento","%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(tipo_rel==RELACIONAMENTO_NN)
   str_aux=QApplication::translate("Relacionamento","many_%1_has_many_%2","",QApplication::UnicodeUTF8);
  else if(tipo_rel==RELACIONAMENTO_GEN)
   str_aux=QApplication::translate("Relacionamento","%1_inherits_%2","",QApplication::UnicodeUTF8);
  else
   str_aux=QApplication::translate("Relacionamento","%1_copies_%2","",QApplication::UnicodeUTF8);

  if(tipo_rel!=RELACIONAMENTO_NN)
   str_aux=str_aux.arg(this->obterTabelaReferencia()->getName())
                     .arg(this->obterTabelaReceptora()->getName());
  else
   str_aux=str_aux.arg(this->tabela_orig->getName())
                  .arg(this->tabela_dest->getName());

  setName(str_aux);

  /* Caso a os sufixos estejam especificados o nome da tabela será
     a junção dos sufixos separados pelo separador de sufixos. Caso
     contrário o nome da tabela será o próprio nome do relacionamento */
  if(sufixo_orig!="" && sufixo_dest!="")
   nome_tab_relnn=sufixo_orig + SEPARADOR_SUFIXO + sufixo_dest;
  else
   nome_tab_relnn=this->obj_name;

  qtd_cols_rejeitadas=0;
  definirIdentificador(identificador);

  attributes[ParsersAttributes::CONSTRAINTS]="";
  attributes[ParsersAttributes::TABLE]="";
  attributes[ParsersAttributes::RELATIONSHIP_NN]="";
  attributes[ParsersAttributes::RELATIONSHIP_GEN]="";
  attributes[ParsersAttributes::RELATIONSHIP_1N]="";
  attributes[ParsersAttributes::ANCESTOR_TABLE]="";
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

vector<QString> Relacionamento::obterColunasRelacionamento(void)
{
 unsigned qtd, i;
 vector<QString> vet_nomes;

 qtd=colunas_ref.size();
 for(i=0; i < qtd; i++)
 {
  vet_nomes.push_back(QString::fromUtf8(colunas_ref[i]->getName()) + " (" +
                      QString::fromUtf8(*colunas_ref[i]->getType()) + ")");
 }

 return(vet_nomes);
}

void Relacionamento::definirTabelaObrigatoria(unsigned id_tabela, bool valor)
{
 RelacionamentoBase::definirTabelaObrigatoria(id_tabela, valor);
 this->invalidado=true;
}

void Relacionamento::definirSufixoTabela(unsigned tipo_tab, const QString &sufixo)
{
 if(tipo_tab > TABELA_DESTINO)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(!sufixo.isEmpty() && !BaseObject::isValidName(sufixo))
  throw Exception(Exception::getErrorMessage(ERR_ASG_INV_SUFFIX_REL)
                .arg(QString::fromUtf8(this->getName())),
                ERR_ASG_INV_SUFFIX_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(tipo_tab==TABELA_ORIGEM)
  sufixo_orig=sufixo;
 else
  sufixo_dest=sufixo;

 this->invalidado=true;
}

QString Relacionamento::obterSufixoTabela(unsigned tipo_tab)
{
 if(tipo_tab > TABELA_DESTINO)
  throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(tipo_tab==TABELA_ORIGEM)
  return(sufixo_orig);
 else
  return(sufixo_dest);
}

void Relacionamento::definirPostergavel(bool valor)
{
 postergavel=valor;
 this->invalidado=true;
}

void Relacionamento::definirIdentificador(bool valor)
{
 /* Validando o relacionamento identificador.
    Relacionamento identificador não pode ser criado quando este é um
    autorelacionamento, relacionamento n-n, de generalização, de dependência
    ou do tipo criado pelo próprio usuário. */
 if(valor &&
   (tabela_orig==tabela_dest ||
    (tipo_relac==RELACIONAMENTO_NN ||
     tipo_relac==RELACIONAMENTO_GEN ||
     tipo_relac==RELACIONAMENTO_DEP)))
  throw Exception(ERR_INV_IDENT_RELATIOSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 identificador=valor;
 this->invalidado=true;
}

void Relacionamento::definirColsChavePrimariaEspecial(vector<unsigned> &cols)
{
 /* Dispara um erro caso o usuário tente usar a chave primária especial em autorelacionamento
    e/ou relacionamento n-n */
 if(autoRelacionamento() || relacionamentoIdentificador() || tipo_relac==RELACIONAMENTO_NN)
  throw Exception(Exception::getErrorMessage(ERR_INV_USE_ESPECIAL_PK)
                .arg(QString::fromUtf8(this->getName())),
                ERR_INV_USE_ESPECIAL_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->id_colunas_pk_rel=cols;
}

vector<unsigned> Relacionamento::obterColChavePrimariaEspecial(void)
{
 return(this->id_colunas_pk_rel);
}

void Relacionamento::criarChavePrimariaEspecial(void)
{
 if(!id_colunas_pk_rel.empty())
 {
  unsigned i, qtd;

  /* Aloca a chave primária com as seguintes características:
     1) A mesma é marcada como protegida e incluída por relacionamento, desta
        forma ela é identificada únicamente nas operações internas do relacionamento

     2) O espaço de tabelas usado na restrição é o mesmo da tabela receptora */
  pk_especial=new Constraint;
  pk_especial->setName(this->getName() + QString("_pk"));
  pk_especial->definirTipo(TipoRestricao::primary_key);
  pk_especial->setAddedByLinking(true);
  pk_especial->setProtected(true);
  pk_especial->setTablespace(dynamic_cast<Tablespace *>(obterTabelaReceptora()->getTablespace()));

  //Adiciona as colunas   chave primária obtendo-as através dos seus índices armazenados em 'id_colunas_pk_rel'
  qtd=id_colunas_pk_rel.size();
  for(i=0; i < qtd; i++)
  {
   if(id_colunas_pk_rel[i] < colunas_ref.size() &&
      !pk_especial->colunaExistente(colunas_ref[id_colunas_pk_rel[i]], Constraint::COLUNA_ORIGEM))
    pk_especial->adicionarColuna(colunas_ref[id_colunas_pk_rel[i]], Constraint::COLUNA_ORIGEM);
  }

  try
  {
   //Tenta adicionar a restrição ao relacionamento
   this->adicionarObjeto(pk_especial);
  }
  catch(Exception &e)
  {
   /* Caso algum erro for detectado a restrição é removida mas o relacionamento não é invalidado
      só será criado sem a chave primária especial */
   delete(pk_especial);
   pk_especial=NULL;
  }
 }
}

void Relacionamento::definirNomeTabelaRelNN(const QString &nome)
{
 if(!BaseObject::isValidName(nome))
  throw Exception(ERR_ASG_INV_NAME_TABLE_RELNN, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 nome_tab_relnn=nome;
 this->invalidado=true;
}

QString Relacionamento::getNameTabelaRelNN(void)
{
 return(nome_tab_relnn);
}

bool Relacionamento::obterPostergavel(void)
{
 return(postergavel);
}

void Relacionamento::definirTipoPostergacao(TipoPostergacao tipo_post)
{
 tipo_postergacao=tipo_post;
 this->invalidado=true;
}

TipoPostergacao Relacionamento::obterTipoPostergacao(void)
{
 return(tipo_postergacao);
}

int Relacionamento::obterIndiceObjeto(TableObject *objeto)
{
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *lista=NULL;
 TableObject *obj_aux=NULL;
 ObjectType tipo_obj;
 bool enc=false;

 //Dispara uma exceção caso o objeto a ser buscado não esteja alocado
 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 tipo_obj=objeto->getObjectType();
 if(tipo_obj==OBJ_COLUMN)
  lista=&atributos_rel;
 else if(tipo_obj==OBJ_CONSTRAINT)
  lista=&restricoes_rel;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista->begin();
 itr_end=lista->end();

 /* Varre a lista de objetos selecionada verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista ou mesmo
    se os endereços dos objetos envolvidos são iguais */
 while(itr!=itr_end && !enc)
 {
  obj_aux=(*itr);
  enc=(obj_aux==objeto || obj_aux->getName()==objeto->getName());
  itr++;
 }

 if(enc)
  return((itr-lista->begin())-1);
 else
  return(-1);
}

bool Relacionamento::colunaExistente(Column *coluna)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col_aux=NULL;
 bool enc=false;

 //Caso a coluna a ser buscada não esteja aloca, dispara uma exceção
 if(!coluna)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=colunas_ref.begin();
 itr_end=colunas_ref.end();

 /* Varre a lista de colunas selecionada verificando se o nome da
    coluna é igual ao nome de uma das colunas da lista ou mesmo
    se os endereços das colunas envolvidas são iguais */
 while(itr!=itr_end && !enc)
 {
  col_aux=(*itr);
  enc=(col_aux==coluna || col_aux->getName()==coluna->getName());
  itr++;
 }

 return(enc);
}

void Relacionamento::adicionarObjeto(TableObject *objeto_tab, int idx_obj)
{
 ObjectType tipo_obj;
 vector<TableObject *> *lista_obj=NULL;

 /* Somente a chave primária especial (criada pelo relacionamento)
    só pode ser adicionado a um relacionamento de generalização ou dependência.
    Caso o tipo do objeto a ser adionado não obedeça a esta condição um erro é retornado */
 if((tipo_relac==RELACIONAMENTO_GEN ||
     tipo_relac==RELACIONAMENTO_DEP) &&
    !(objeto_tab->isAddedByRelationship() &&
      objeto_tab->isProtected() &&
      objeto_tab->getObjectType()==OBJ_CONSTRAINT))
  throw Exception(ERR_ASG_OBJ_INV_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 try
 {
  /* Verifica se o objeto já não foi inserido.
     Um atributo/restrição de um relacionamento não pode
     ser atribuido ao relacionamento caso este já pertença a uma tabela,
     caso isso aconteça o método aborta a inserção do objeto */
  if(!objeto_tab->getParentTable() &&
     obterIndiceObjeto(objeto_tab) < 0)
  {
   /* Obtém a lista de objetos de acordo com o tipo
      do objeto a ser inserido */
   tipo_obj=objeto_tab->getObjectType();
   if(tipo_obj==OBJ_COLUMN)
    lista_obj=&atributos_rel;
   else if(tipo_obj==OBJ_CONSTRAINT)
    lista_obj=&restricoes_rel;

   /* Tenta gerar a definição SQL do objeto para ver se o mesmo
      está bem configurado. Caso não esteja, uma exceção será
      disparada e o objeto não será inserido na lista */

   /* Como atributos recém criados não possuem uma tabela pai até que sejam
      adicionados   tabela receptora, para medidas de validação do código do atributo,
      a tabela de origem do relacionamento é atribuída como tabela pai */
   objeto_tab->setParentTable(tabela_orig);

   if(tipo_obj==OBJ_COLUMN)
    dynamic_cast<Column *>(objeto_tab)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
   else
   {
    Constraint *rest=NULL;
    rest=dynamic_cast<Constraint *>(objeto_tab);
    rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);

    /* Caso se tente inserir uma chave estrangeira como restrição do relacionamento
       retorna um erro pois este é o único tipo que não pode ser incluído */
    if(rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     throw Exception(ERR_ASG_FOREIGN_KEY_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Após a validação do novo objeto a tabela pai é setada como nula
   objeto_tab->setParentTable(NULL);

   /* Caso o índice passado seja menor que zero ou superior ao tamanho da lista
      o objeto será inserido ao final da mesma */
   if(idx_obj < 0 || idx_obj >= static_cast<int>(lista_obj->size()))
    lista_obj->push_back(objeto_tab);
   else
   {
    //Caso o índice seja válido, insere o objeto na posição especificada
    if(lista_obj->size() > 0)
     lista_obj->insert((lista_obj->begin() + idx_obj), objeto_tab);
    else
     lista_obj->push_back(objeto_tab);
   }

   /* Apenas para os relacinamentos n-n os atributos/restrições
      não são marcados como "incluídos por ligação" pois estes atributos
      e restrições são aproveitados quando o relacionamento n-n é convertido
      em tabela */
   //if(tipo_relac!=RELACIONAMENTO_NN)
   objeto_tab->setAddedByLinking(true);

   this->invalidado=true;
  }
  else
   throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                 .arg(objeto_tab->getName(true))
                 .arg(objeto_tab->getTypeName())
                 .arg(this->getName(true))
                 .arg(this->getTypeName()),
                 ERR_ASG_DUPLIC_OBJECT, __PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 catch(Exception &e)
 {
  /* Caso a exceção capturada seja de atributo não preenchido, indice que
     a definição SQL do objeto está incompleta */
  if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
   throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(objeto_tab->getName()))
                              .arg(objeto_tab->getTypeName()),
                 ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  else
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void Relacionamento::removerObjetos(void)
{
 atributos_rel.clear();
 restricoes_rel.clear();
}

void Relacionamento::destruirObjetos(void)
{
 while(!restricoes_rel.empty())
 {
  delete(restricoes_rel.back());
  restricoes_rel.pop_back();
 }

 while(!atributos_rel.empty())
 {
  delete(atributos_rel.back());
  atributos_rel.pop_back();
 }
}

void Relacionamento::removerObjeto(unsigned id_obj, ObjectType tipo_obj)
{
 vector<TableObject *> *lista_obj=NULL;

 //Seleciona a lista de objetos de acordo com o tipo passado
 if(tipo_obj==OBJ_COLUMN)
  lista_obj=&atributos_rel;
 else if(tipo_obj==OBJ_CONSTRAINT)
  lista_obj=&restricoes_rel;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Se o índice do objeto for inválido, dispara o erro
 if(id_obj >= lista_obj->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Verificação específica para coluna. Caso a coluna esteja sendo
    referenciada por uma restrição do relacionamento será disparado
    um erro. */
 if(tipo_obj==OBJ_COLUMN)
 {
  Column *coluna=NULL;
  Constraint *rest=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool refer=false;

  itr=restricoes_rel.begin();
  itr_end=restricoes_rel.end();
  coluna=dynamic_cast<Column *>(lista_obj->at(id_obj));

  while(itr!=itr_end && !refer)
  {
   rest=dynamic_cast<Constraint *>(*itr);
   //Verifica se a coluna está em uma das listas das restições
   refer=(rest->obterColuna(coluna->getName(), Constraint::COLUNA_ORIGEM) ||
          rest->obterColuna(coluna->getName(), Constraint::COLUNA_REFER));
   itr++;
  }

  //Caso haja referência
  if(refer)
   throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                           .arg(QString::fromUtf8(coluna->getName()))
                           .arg(coluna->getTypeName())
                           .arg(QString::fromUtf8(rest->getName()))
                           .arg(rest->getTypeName())
                           .arg(QString::fromUtf8(this->getName(true)))
                           .arg(this->getTypeName()),
                 ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 /* Antes da remoção de qualquer objeto é necessário desconectar
    o relacionamento pois o objeto a ser removido pode estar sendo
    referenciado por outros objetos */
 desconectarRelacionamento(false);

 //Remove o item da lista
 lista_obj->erase(lista_obj->begin() + id_obj);

 //Reconecta o relacionamento
 conectarRelacionamento();
}

void Relacionamento::removerObjeto(TableObject *objeto)
{
 if(!objeto)
  throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 removerObjeto(obterIndiceObjeto(objeto),objeto->getObjectType());
}

void Relacionamento::removerAtributo(unsigned id_atrib)
{
 removerObjeto(id_atrib, OBJ_COLUMN);
}

void Relacionamento::removerRestricao(unsigned id_rest)
{
 removerObjeto(id_rest, OBJ_CONSTRAINT);
}

Column *Relacionamento::obterColunaReferenciada(const QString &nome_col)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col=NULL;
 bool enc=false, formatar;

 formatar=nome_col.contains("\"");
 itr=colunas_ref.begin();
 itr_end=colunas_ref.end();

 /* Varre a lista colunas referenciadas verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista */
 while(itr!=itr_end && !enc)
 {
  col=(*itr);
  enc=(col->getName(formatar)==nome_col);
  itr++;
 }

 if(enc)
  return(col);
 else
  return(NULL);
}

TableObject *Relacionamento::obterObjeto(unsigned idx_obj, ObjectType tipo_obj)
{
 vector<TableObject *> *lista=NULL;

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 if(tipo_obj==OBJ_COLUMN)
  lista=&atributos_rel;
 else if(tipo_obj==OBJ_CONSTRAINT)
  lista=&restricoes_rel;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(idx_obj >= lista->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(lista->at(idx_obj));
}

TableObject *Relacionamento::obterObjeto(const QString &nome_atrib, ObjectType tipo_obj)
{
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *lista=NULL;
 TableObject *obj_aux=NULL;
 bool enc=false;

 //Selecionando a lista de objetos de acordo com o tipo do objeto
 if(tipo_obj==OBJ_COLUMN)
  lista=&atributos_rel;
 else if(tipo_obj==OBJ_CONSTRAINT)
  lista=&restricoes_rel;
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista->begin();
 itr_end=lista->end();

 /* Varre a lista de objetos selecionada verificando se o nome do
    do objeto é igual ao nome de um dos objetos da lista ou mesmo
    se os endereços dos objetos envolvidos são iguais */
 while(itr!=itr_end && !enc)
 {
  obj_aux=(*itr);
  enc=(obj_aux->getName()==nome_atrib);
  itr++;
 }

 if(enc)
  return(obj_aux);
 else
  return(NULL);
}

Column *Relacionamento::obterAtributo(unsigned id_atrib)
{
 /* Caso o índice do atributo esteja fora da quantidade da lista de
    atributos dispara uma exceção */
 if(id_atrib >= atributos_rel.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(dynamic_cast<Column *>(atributos_rel[id_atrib]));
}

Column *Relacionamento::obterAtributo(const QString &nome_atrib)
{
 return(dynamic_cast<Column *>(obterObjeto(nome_atrib,OBJ_COLUMN)));
}

Constraint *Relacionamento::obterRestricao(unsigned id_rest)
{
 /* Caso o índice da restrição esteja fora da quantidade da lista de
    restrições dispara uma exceção */
 if(id_rest >= restricoes_rel.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(dynamic_cast<Constraint *>(restricoes_rel[id_rest]));
}

Constraint *Relacionamento::obterRestricao(const QString &nome_rest)
{
 return(dynamic_cast<Constraint *>(obterObjeto(nome_rest,OBJ_CONSTRAINT)));
}

unsigned Relacionamento::obterNumAtributos(void)
{
 return(atributos_rel.size());
}

unsigned Relacionamento::obterNumRestricoes(void)
{
 return(restricoes_rel.size());
}

unsigned Relacionamento::obterNumObjetos(ObjectType tipo_obj)
{
 if(tipo_obj==OBJ_COLUMN)
  return(atributos_rel.size());
 else if(tipo_obj==OBJ_CONSTRAINT)
  return(restricoes_rel.size());
 else
  throw Exception(ERR_REF_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Relacionamento::adicionarRestricoes(Tabela *tab_dest)
{
 Constraint *rest=NULL, *pk=NULL;
 //vector<ObjetoTabela *>::iterator itr, itr_end;
 unsigned id_rest, qtd_rest, i, qtd;
 QString nome, nome_orig, aux;

 try
 {
  //itr=restricoes_rel.begin();
  //itr_end=restricoes_rel.end();

  qtd_rest=restricoes_rel.size();

  //Varre a lista de restrições do relacionamento
  //while(itr!=itr_end)
  for(id_rest=0; id_rest < qtd_rest; id_rest++)
  {
   //Obtém a restrição
   //rest=dynamic_cast<Restricao *>(*itr);
   rest=dynamic_cast<Constraint *>(restricoes_rel[id_rest]);

   /* Interrompe o processamento caso a restrição já
      tenha sido incluída a uma das tabelas */
   if(rest->getParentTable())
    break;

   /* Caso ela não seja uma chave primária, a mesma é inserida
      na tabela */
   if(rest->obterTipoRestricao()!=TipoRestricao::primary_key)
   {
    i=1; aux[0]='\0';
    nome="";
    //Configura o nome da restrição a fim de resolver problemas de duplicidade
    nome_orig=rest->getName();
    while(tab_dest->obterRestricao(nome_orig + aux))
    {
     aux=QString("%1").arg(i);
     nome=nome_orig + aux;
     i++;
    }

    if(nome!="") rest->setName(nome);

    //Adiciona a restrição na tabela
    tab_dest->adicionarRestricao(rest);
   }
   else
   {
    /* Caso a restição seja uma chave primária, esta será fundida com a
       chave primária da tabela. */

    //Obtém a chave primária da tabela
    pk=tab_dest->obterChavePrimaria();

    //Caso ela exista será executada a fusão
    if(pk)
    {
     //Obtém a quantidade de colunas da restrição
     qtd=rest->obterNumColunas(Constraint::COLUNA_ORIGEM);

     for(i=0; i < qtd; i++)
      //Adiciona cada coluna da restrição na chave primária da tabela
      pk->adicionarColuna(rest->obterColuna(i, Constraint::COLUNA_ORIGEM),
                          Constraint::COLUNA_ORIGEM);
    }
    else
     /* Caso a tabela não possua uma chave primária, a própria restrição do relacionamento
        será a chave primária da tabela */
     tab_dest->adicionarRestricao(rest);

    //Remove a chave primária especial da lista de restrições
    if(rest==pk_especial)
    {
     restricoes_rel.erase(restricoes_rel.begin()+id_rest);
     qtd_rest=restricoes_rel.size();
    }
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarColunasRelGen(void)
{
 Tabela *tab_orig=NULL, *tab_dest=NULL,
        *tab_pai=NULL, *tab_aux=NULL;
 Column *col_orig=NULL, *col_dest=NULL,
        *coluna=NULL, *col_aux=NULL;
 unsigned qtd_orig, qtd_dest,
          i, i1, i2, id_tab,
          idx, qtd_tab;
 vector<Column *> colunas;
 ObjectType tipos[2]={OBJ_TABLE, BASE_TABLE};
 ErrorType tipo_erro=ERR_CUSTOM;
 bool duplic=false, cond,
      /* 0 -> Coluna vinda de herança
         1 -> Coluna vinda de cópia */
      flags_orig[2]={false,false},
      flags_dest[2]={false,false};
 QString str_aux, msg;
 TipoPgSQL tipo_orig, tipo_dest;

 try
 {
  tab_orig=dynamic_cast<Tabela *>(tabela_orig);
  tab_dest=dynamic_cast<Tabela *>(tabela_dest);

  //Obtém o número de colunas de ambas as tabelas
  qtd_orig=tab_orig->obterNumColunas();
  qtd_dest=tab_dest->obterNumColunas();
  qtd_cols_rejeitadas=0;

  /* Este for compara cada coluna da tabela de destino com
     cada coluna da tabela de origem */
  for(i=0; i < qtd_dest && tipo_erro==ERR_CUSTOM; i++)
  {
   //Obtém uma coluna do destino
   col_dest=tab_dest->obterColuna(i);
   /* Obtém o seu tipo e o converte para 'integer' caso seja
      serial, pois em caso de se comparar um 'serial' de um coluna
      com o integer de outra, pode-se gerar um erro, mesmo estes
      tipos sendo compatíveis */
   tipo_dest=col_dest->getType();
   if(tipo_dest=="serial") tipo_dest="integer";
   else if(tipo_dest=="bigserial") tipo_dest="bigint";

   /* Este flag indica que a coluna de uma tabela possui o
      mesmo nome de uma coluna da outra */
   duplic=false;

   /* Com a coluna de destino obtida, será feita uma comparação com
      todas as colunas da origem */
   for(i1=0; i1 < qtd_orig && !duplic; i1++)
   {
    //Obtém uma coluna da origem e converte seu tipo
    col_orig=tab_orig->obterColuna(i1);
    tipo_orig=col_orig->getType();
    if(tipo_orig=="serial") tipo_orig="integer";
    else if(tipo_orig=="bigserial") tipo_orig="bigint";

    //Compara o nome das duas colunas
    duplic=(col_orig->getName()==col_dest->getName());

    //Caso haja duplicidade de nomes
    if(duplic)
    {
     /* É necessário verificar se a coluna de origem é da própria tabela,
        se veio de uma tabela pai ou de uma tabela de cópia. A mesma
        verificação é feita na coluna de destino.

        A duplicidade de colunas só gera erro quando a coluna de origem é
        da própria tabela e a coluna de destino não veio de uma tabela pai
        da tabela de destino no caso de um relacionamento de dependência.

        Caso a coluna de origem seja da tabela de origem ou vinda de um
        relacionamento de dependência e o tipo do relacionamento atual seja
        de herança, o único caso em que a duplicidade gera erro é a incompatibilidade
        dos tipos das colunas envolvidas, caso contrário as mesmas são fundidas. */
     for(id_tab=0; id_tab < 2; id_tab++)
     {
      if(id_tab==0)
      {
       /* Selecionando a coluna de origem e a tabela
          de origem para fazer a comparação descrita acima */
       col_aux=col_orig;
       tab_aux=tab_orig;
      }
      else
      {
       /* Selecionando a coluna de destino e a tabela
          de destino para fazer a comparação descrita acima */
       col_aux=col_dest;
       tab_aux=tab_dest;
      }

      /* Varre a lista de tabelas pai (tipos[0]) e de
         cópia tipos[1] da origem e destino */
      for(i2=0; i2 < 2; i2++)
      {
       /* Obtém o número de objetos do tipo OBJETO_TABELA (tabela pai) ou
          OBJETO_TABELA_BASE (tabela cópia), da tabela selecionada acima */
       qtd_tab=tab_aux->obterNumObjetos(tipos[i2]);

       /* Este for obtém cada tabela da lista e verifica se a coluna auxiliar
          existe na lista de colunas de tal tabela, isso é suficiente para
          se saber se a coluna auxiliar veio de uma relacionamento de herança
          ou de dependência */
       for(idx=0; idx < qtd_tab; idx++)
       {
        tab_pai=dynamic_cast<Tabela *>(tab_aux->obterObjeto(idx, tipos[i2]));
        //Verifica se a coluna existe na tabela obtida
        cond=(tab_pai->obterColuna(col_aux->getName()));

        /* Caso o id_tab==0, indica que desejamos atribuir o resultado
           da comparação acima ao vetor de flags na posição relativa
           ao relacionamento de herança que indica que a coluna auxiliar
           existe ou não na tabela pai */
        if(id_tab==0)
         flags_orig[i2]=cond;
        else
        /* Caso o id_tab==1, indica que desejamos atribuir o resultado
           da comparação acima ao vetor de flags na posição relativa
           ao relacionamento de dependência que indica que a coluna auxiliar
           existe ou não na tabela cópia */
         flags_dest[i2]=cond;
       }
      }
     }

     /* Condição de erro 1: O tipo de relacionamento é de dependência
        e a coluna de origem é da própria tabela ou vinda de uma tabela cópia
        e a coluna de destino é da própria tabela de destino ou veio de uma
        tabela de cópia da própria tabela de destino */
     if(tipo_relac==RELACIONAMENTO_DEP &&

       ((!flags_orig[0] && !flags_orig[1]) ||
        (!flags_orig[0] &&  flags_orig[1])) &&

       ((!flags_dest[0] && !flags_dest[1]) ||
        (!flags_dest[0] &&  flags_dest[1])))
     {
      tipo_erro=ERR_DUPLIC_COLS_COPY_REL;
     }
     /* Condição de erro 2: O tipo de relacionamento é de generalização e o tipo
        das colunas são incompatíveis */
     else if(tipo_relac==RELACIONAMENTO_GEN &&
             tipo_orig!=tipo_dest)
      tipo_erro=ERR_INCOMP_COLS_INHERIT_REL;
    }
   }

   //Caso não foi detectado nenhum erro
   if(tipo_erro==ERR_CUSTOM)
   {
    //Caso não haja duplicidade
    if(!duplic)
    {
     //Cria uma nova coluna e faz as configurações iniciais
     coluna=new Column;

     (*coluna)=(*col_dest);

     if(tipo_relac==RELACIONAMENTO_GEN)
      coluna->setAddedByGeneralization(true);
     else
      coluna->setAddedByCopy(true);

     coluna->setParentTable(NULL);

     //Converte seu tipo de '' para 'integer', se necessário
     if(coluna->getType()=="serial")
      coluna->setType(TipoPgSQL("integer"));
     else if(coluna->getType()=="bigserial")
      coluna->setType(TipoPgSQL("bigint"));

     //Adiciona a nova coluna   lista temporária de colunas
     colunas.push_back(coluna);
    }
    else
     /* Caso haja duplicidade, a coluna é rejeitada e não incluída na lista,
        ao invés disso, incrementa o atributo o qual contabiliza a quantidade
        de colunas duplicadas as quais foram rejeitadas por já existirem
        na tabela de destino */
     qtd_cols_rejeitadas++;
   }
  }

  //Caso nenhum erro de duplicidade foi detectado
  if(tipo_erro==ERR_CUSTOM)
  {
   vector<Column *>::iterator itr, itr_end;

   /* As colunas da lista temporária serão inseridas
      na lista de colunas de referência, do relacionamento e além disso
      as colunas também serão inseridas diretamente na tabela de origem
      do relacionamento, a qual herda ou copia as colunas da tabela
      de destino */
   colunas_ref=colunas;
   itr=colunas_ref.begin();
   itr_end=colunas_ref.end();
   while(itr!=itr_end)
   {
    tab_orig->adicionarColuna((*itr));
    itr++;
   }
  }
  else
  {
   /* Caso haja algum erro de duplicidade, a lista temporária de
      colunas é destruida */
   while(!colunas.empty())
   {
    delete(colunas.back());
    colunas.pop_back();
   }

   /* Obtém a mensagem de erro que será mostarada ao usuário de acordo com o
      tipo de erro de duplicidade identificado */
   str_aux=Exception::getErrorMessage(tipo_erro);

   //Formata a mensagem de erro de acordo com o tipo do erro
   if(tipo_erro==ERR_DUPLIC_COLS_COPY_REL)
   {
    msg=QString(str_aux)
        .arg(col_dest->getName())
        .arg(tab_dest->getName())
        .arg(tab_orig->getName());
   }
   else
   {
    msg=QString(str_aux)
        .arg(col_dest->getName())
        .arg(tab_dest->getName())
        .arg(col_orig->getName())
        .arg(tab_orig->getName());
   }

   //Dispara a exeção acusando a duplicidade
   throw Exception(msg, tipo_erro,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Cria a chave primária especial se houve
  this->criarChavePrimariaEspecial();
  this->adicionarRestricoes(obterTabelaReceptora());
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::conectarRelacionamento(void)
{
 try
 {
  if(!conectado)
  {
   if(tipo_relac==RELACIONAMENTO_GEN)
   {
    /* Definindo que a tabela de destino é a tabela pai
       da tabela de origem como indicado pelo relacionamento
       de generalização */
    adicionarColunasRelGen();

    //Adicionar a tabela referência como tabela pai da tabela receptora
    obterTabelaReceptora()->adicionarTabelaPai(dynamic_cast<Tabela *>(obterTabelaReferencia()));
   }
   else if(tipo_relac==RELACIONAMENTO_DEP)
   {
    /* Definindo que a tabela de origem depende da tabela
       de destino pois parte de seus atributos virão da
       primeira como indicado pelo relacionamento de dependência */
    adicionarColunasRelGen();
    //Adiciona a tabela referência como tabela cópia da tabela receptora
    obterTabelaReceptora()->adicionarTabelaCopia(dynamic_cast<Tabela *>(obterTabelaReferencia()));
   }
   else if(tipo_relac==RELACIONAMENTO_11 ||
           tipo_relac==RELACIONAMENTO_1N)
   {
    if(tipo_relac==RELACIONAMENTO_11)
     adicionarColunasRel11();
    else
     adicionarColunasRel1n();
   }
   else if(tipo_relac==RELACIONAMENTO_NN)
   {
    if(!tabela_relnn)
     /* Caso o tipo de relacionamento seja n-n e a tabela que representa
        o relacionamento será alocada e configurado o nome automaticamente */
     tabela_relnn=new Tabela;

     /* O esquema e espaço de tabelas da tabela resultante será, por padrão,
        os mesmos da tabela de origem */
    tabela_relnn->setName(nome_tab_relnn);
    tabela_relnn->setSchema(tabela_orig->getSchema());
    tabela_relnn->setTablespace(tabela_orig->getTablespace());

    adicionarColunasRelNn();
   }

   /* Faz uma chamada ao método de conexão do relacionamento da
      classe base */
   RelacionamentoBase::conectarRelacionamento();

   /* Indica que o relacionameto foi conetado corretamente e que não está
   invalidado por modificação de atributos */
   this->invalidado=false;
  }
 }
 catch(Exception &e)
 {
  if(tabela_relnn)
  {
   delete(tabela_relnn);
   tabela_relnn=NULL;
  }
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::configurarRelIdentificador(Tabela *tab_receptora)
{
 Constraint *pk=NULL;
 unsigned i, qtd;
 QString nome, aux;
 bool nova_pk=false;

 try
 {
  /* Caso seja um relacionamento identificador, a chave primária
     da tabela na qual se insere as colunas, caso exista, será composta
     pelas colunas da chave primária da tabela de origem do relacionamento
     (entidade forte) e pelas colunas da chave primária da tabela de destino
     (entidade fraca) */

  //Obtém a chave primária da tabela de destino
  pk=tab_receptora->obterChavePrimaria();

  //Caso não exista a chave primária na entidade fraca, a mesma será criada
  if(!pk)
  {
   //Cria e configura a chave primária da entidade fraca
   if(!pk_relident)
   {
    pk=new Constraint;
    pk->definirTipo(TipoRestricao::primary_key);
    pk->setAddedByLinking(true);
    this->pk_relident=pk;
   }
   else
    pk=this->pk_relident;

   nova_pk=true;
   i=1;
   aux[0]='\0';
   nome=tab_receptora->getName() + SEPARADOR_SUFIXO + "pk";

   /* Verifica se já não existe uma restrição na tabela a qual se adiciona
      as retrições cujo nome seja o mesmo configurado acima. Enquanto isso
      ocorrer, será concatenado um número */
   while(tab_receptora->obterRestricao(nome + aux))
   {
    aux=QString("%1").arg(i);
    i++;
   }

   //Atribui o nome configurado   chave primaria criada
   pk->setName(nome);
  }

  /* Adicionando as colunas da chave primária da entidade forte na chave
     primária da entidade fraca */
  qtd=colunas_ref.size();
  for(i=0; i < qtd; i++)
   pk->adicionarColuna(colunas_ref[i], Constraint::COLUNA_ORIGEM);

  //Caso a tabela não tenha uma chave primária a mesma será atrua   ela
  if(nova_pk)
   tab_receptora->adicionarRestricao(pk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarChaveUnica(Tabela *tab_referencia, Tabela *tab_receptora)
{
 Constraint *uq=NULL;
 unsigned i, qtd;
 QString nome, aux;

 try
 {
  //Aloca a chave única
  if(!uq_rel11)
  {
   uq=new Constraint;
   uq->definirTipo(TipoRestricao::unique);
   uq->setAddedByLinking(true);
   uq_rel11=uq;
  }

  //Insere as colunas do relacionamentos   chave única
  qtd=colunas_ref.size();
  i=0;

  while(i < qtd)
   uq->adicionarColuna(colunas_ref[i++], Constraint::COLUNA_ORIGEM);

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  nome=tab_referencia->getName() + SEPARADOR_SUFIXO + "uq";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave única. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(tab_receptora->obterRestricao(nome + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave única
  uq->setName(nome + aux);

  /* Após configurada a chave única que define o
     relacionamento é adicionado na tabela */
  tab_receptora->adicionarRestricao(uq);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarChaveEstrangeira(Tabela *tab_referencia, Tabela *tab_receptora, TipoAcao acao_del, TipoAcao acao_upd)
{
 Constraint *pk=NULL, *pk_aux=NULL, *fk=NULL;
 unsigned i, i1, qtd;
 Column *coluna=NULL, *coluna_aux=NULL;
 QString nome, aux;

 try
 {
  //Aloca uma chave estrangeira para ser configurada
  if((tipo_relac==RELACIONAMENTO_NN) ||
     (!fk_rel1n && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N)))
     //(!fk_rel1n && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N)))
  {
   fk=new Constraint;
   fk->definirPostergavel(this->postergavel);
   fk->definirTipoPostergacao(this->tipo_postergacao);
   fk->definirTipo(TipoRestricao::foreign_key);
   fk->setAddedByLinking(true);
   //Define a tabela de destino da chave estrangeira
   fk->definirTabReferenciada(tab_referencia);

   /* Caso o relacionamento seja 1-1 ou 1-n a chave estrangeira alocada
      será atribud   chave estrangeira que representa o relacionamento */
   if(tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N)
    fk_rel1n=fk;
  }
  /*else if(fk_rel1n)
  {
   fk=fk_rel1n;
   fk->removerColunas();
  } */

  //Configura a ação de ON DELETE e ON UPDATE da chave estrangeira
  fk->definirTipoAcao(acao_del, false);
  fk->definirTipoAcao(acao_upd, true);

  /* Obtém a chave primária da tabela de origem para ser referenciada
     pela chave estrangeira */
  pk=tab_referencia->obterChavePrimaria();

  /* Relacionas as colunas da tabela de origem com as colunas da chave
     primária da tabela de destino, na chave estrangeira do relacionamento */
  qtd=colunas_ref.size();
  i=i1=0;

  /* Condição especial para relacionamentos n-n.
     Como as colunas copiadas das tabelas participantes do relacinamentos
     são armazenadas em uma só lista, é necessário fazer um deslocamento
     na varredura para que as colunas nao sejam relacionadas de forma
     incorreta na chave estrangeira.

     Caso 1: A quantidade de colunas (qtd) precisa ser decrementada da quantidade
             de colunas presentes na chave primária da tabela de destino. Isso
             quando o ponteirio 'tab_orig' aponta para a própria tabela de origem
             do relacionamento. Desta forma, evita-se que colunas além da posição
             final da lista de colunas da chave primária de origem sejam acessadas.

     Caso 2: O índice inicial de varredura (i) apontará para a primeira coluna
             na lista do relacionamento a qual corresponde ao conjunto de colunas
             da tabela de destino. A primeira coluna referente a lista de colunas da tabela de
             destino sempre terá seu indice como sendo a quantidade de colunas existentes na
             chave primária da tabela de origem, pois, sempre são inseridas na lista de colunas
             as colunas da origem (vindas da chave primária da origem) e depois as colunas
             da chave primária de destino. */
  if(tipo_relac==RELACIONAMENTO_NN)
  {
   //Caso 1: decrementando a quantidade de colunas a serem varridas
   if((!autoRelacionamento() && tab_referencia==tabela_orig) ||
      (autoRelacionamento() && tabela_relnn->obterNumRestricoes()==0))
    qtd-=dynamic_cast<Tabela *>(tabela_dest)->obterChavePrimaria()->obterNumColunas(Constraint::COLUNA_ORIGEM);
   //Caso 2: deslocando o índice de varredura
   else if(tab_referencia==tabela_dest)
   {
    pk_aux=dynamic_cast<Tabela *>(tabela_orig)->obterChavePrimaria();
    i=pk_aux->obterNumColunas(Constraint::COLUNA_ORIGEM);
   }
  }

  while(i < qtd)
  {
   //Obtém um coluna referenciada
   coluna=colunas_ref[i];
   //Obtém uma coluna da chave primária da tabela de origem
   coluna_aux=pk->obterColuna(i1, Constraint::COLUNA_ORIGEM);
   //Faz a ligação das colunas na chave estrangeira
   fk->adicionarColuna(coluna, Constraint::COLUNA_ORIGEM);
   fk->adicionarColuna(coluna_aux, Constraint::COLUNA_REFER);
   i++; i1++;
  }

  //Configura o nome da chave estrangeira
  i=1;
  aux[0]='\0';
  nome=tab_referencia->getName() + SEPARADOR_SUFIXO + "fk";

  /* Verifica a existencia de alguma restrição com mesmo nome
     na tabela a qual receberá a chave estrangeira. Enquanto existir
     um novo nome será gerado concatenando um número inteiro para
     pode diferenciar dos demais */
  while(tab_receptora->obterRestricao(nome + aux))
  {
   aux=QString("%1").arg(i);
   i++;
  }

  //Atribui o nome configurado   chave estrangeira
  fk->setName(nome + aux);

  /* Após configurada a chave estrangeira que define o
     relacionamento é adicionado na tabela */
  tab_receptora->adicionarRestricao(fk);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarAtributos(Tabela *tab_receptora)
{
 unsigned i, qtd, i1;
 Column *coluna=NULL;
 QString nome, aux;

 try
 {
  //Adicionando os atributos do relacionamento na tabela
  qtd=atributos_rel.size();
  aux[0]='\0';

  for(i=0, i1=1; i < qtd; i++)
  {
   //Obtém o atributo
   coluna=dynamic_cast<Column *>(atributos_rel[i]);

   //Caso o atributo já pertença a uma tabela interrompe o processamento
   if(coluna->getParentTable())
    break;

   nome=coluna->getName();

   /* Verifica o se o nome da coluna já não existe na tabela. Equanto
      existir, incrementa e concatena um número (i1) ao final do nome,
      até que este nome não exista nas colunas da tabela onde será
      inserido o atributo */
   while(tab_receptora->obterColuna(nome + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }
   //Define o nome do atributo
   coluna->setName(nome + aux);
   aux[0]='\0';

   //Adiciona o atributo na tabela
   tab_receptora->adicionarColuna(coluna);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::copiarColunas(Tabela *tab_referencia, Tabela *tab_receptora, bool nao_nulo)
{
 Constraint *pk_dest=NULL, *pk_orig=NULL, *pk=NULL;
 unsigned i, qtd, i1;
 Column *coluna=NULL, *coluna_aux=NULL;
 QString nome, sufixo, aux, nome_ant;

 try
 {
  //Obtém as chaves primárias das tabelas de origem e destino
  pk_dest=tab_receptora->obterChavePrimaria();
  pk=pk_orig=tab_referencia->obterChavePrimaria();

   /* Selecionando a lista de colunas correta de acordo com a forma do relacionamento.
     Caso a tabela a qual receberá a chave estrangeira (tab_dest) for uma
     referênci  tabela de origem do relacionamento, o sufixo das colunas a serem criadas
     será configurado como sendo o sufixo da tabela de origem. Caso contrário  o
      será o da própria tabela de destino. */
  if(sufixo_auto)
  {
   if(tipo_relac==RELACIONAMENTO_1N || tipo_relac==RELACIONAMENTO_11)
   {
    sufixo=SEPARADOR_SUFIXO + tab_referencia->getName();

    if(tab_referencia==tabela_orig)
     sufixo_orig=sufixo;
    else
     sufixo_dest=sufixo;
   }
   else if(tipo_relac==RELACIONAMENTO_NN)
   {
    if(tab_referencia==tabela_dest)
    sufixo=sufixo_dest=SEPARADOR_SUFIXO + tabela_dest->getName();
   else
    sufixo=sufixo_orig=SEPARADOR_SUFIXO + tabela_orig->getName();
   }
  }
  else if(((tipo_relac!=RELACIONAMENTO_NN && tab_receptora==tabela_orig) ||
           (tipo_relac==RELACIONAMENTO_NN && tab_referencia==tabela_dest))
      && !sufixo_dest.isEmpty())
   sufixo=SEPARADOR_SUFIXO + sufixo_dest;
  else if(((tipo_relac!=RELACIONAMENTO_NN && tab_receptora==tabela_dest) ||
           (tipo_relac==RELACIONAMENTO_NN && tab_referencia==tabela_orig))
           && !sufixo_orig.isEmpty())
   sufixo=SEPARADOR_SUFIXO + sufixo_orig;

  /* Caso o relacionamento seja 1-n e a tabela de origem não possua
     uma chave primária ou se o relacionamento seja n-n e ambas as tabelas
     não possuam chave primária, uma exceção será disparada */
  if((!pk_orig && (tipo_relac==RELACIONAMENTO_1N || tipo_relac==RELACIONAMENTO_11)) ||
     (!pk_orig && !pk_dest && tipo_relac==RELACIONAMENTO_NN))
   throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
                          .arg(QString::fromUtf8(this->obj_name))
                          .arg(QString::fromUtf8(tab_referencia->getName(true)))
                          .arg(QString::fromUtf8(tab_receptora->getName(true))),
                 ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);


  /* Obtém a quantidade de colunas referenciadas na chave
     primária selecionada */
  qtd=pk->obterNumColunas(Constraint::COLUNA_ORIGEM);

  /* Varre a chave primária, obtendo as colunas e adicionando na lista
     de colunas selecionada acima. Isso é feito para se saber quais são
     as colunas da tabela, dona da chave primária, que serão usadas
    no caso de se criar chaves estrangeiras */
  for(i=0; i < qtd; i++)
  {
   i1=1;
   aux="";

   //Aloca uma nova coluna
   coluna=new Column;
   colunas_ref.push_back(coluna);

   /* Copia o conteúdo da coluna da chave primária no indice i para
       a nova coluna criada */
   coluna_aux=pk->obterColuna(i, Constraint::COLUNA_ORIGEM);
   colunas_pk.push_back(coluna_aux);

   (*coluna)=(*coluna_aux);
   coluna->setNotNull(nao_nulo);

   //Obtém o nome anterior da coluna antes da desconexão do relacionamento
   nome_ant=nome_ant_cols_ref[coluna->getObjectId()];

   //Protege a nova coluna, evitando que o usuário a modifique ou remova
   coluna->setAddedByLinking(true);

   //Desfaz a referência da coluna a uma tabela pai
   coluna->setParentTable(NULL);

   /* Caso o tipo da nova coluna seja "serial" o mesmo será
      convertido para "integer" */
   if(coluna->getType()=="serial")
    coluna->setType(TipoPgSQL("integer"));
   else if(coluna->getType()=="bigserial")
    coluna->setType(TipoPgSQL("bigint"));
   /* O nome da nova coluna, será o nome original concatenado
      com o sufixo da tabela a qual ela pertence. Isso é feito
      para se saber de onde tal coluna foi originada */
   nome=coluna->getName() + sufixo;

   /* Verifica se o nome da coluna já não existe na tabela
      na qual será inserida, caso exista, um número será concatenado
      ao final de seu nome. Enquanto esta condição for verdadeira
      este número (i1) será incrementado até que não exista uma coluna
      com tal nome (nome original + sufixo + número) */
   while(tab_receptora->obterColuna(nome + aux))
   {
    aux=QString("%1").arg(i1);
    i1++;
   }

   //Armazena o sufixo gerado para validações posteriores do relacionamento
   sufixo_cols.push_back(sufixo + aux);

   //Concatena a string auxiliar ao nome inteiro da coluna
   nome+=aux;

   //Primeiramente a coluna é renomeada com seu nome antigo para manter o histórico
   if(nome_ant!="")
    coluna->setName(nome_ant);

   /* Define o nome da coluna como sendo o nome configurado acima, desta forma a
      coluna passará a ter como nome antigo o nome atribuido na iteração acima */
   coluna->setName(nome);

   /* Caso o nome anteriro atribuíd  coluna seja diferente do nome atual, o nome
      atual da coluna passará a ser o nome antigo da mesma quando o relacionamento
      for desconectado e reconectado novamente, desta forma o histórico de nomes da
      colunas não se perde mesmo quando as colunas do relacionamento são desalocadas,
      isso evita a quebra de referências as colunas criadas pelo relacionamento.
      Essa operação só é executada para relacionamentos 1-n e 1-n para relacionamentos
      n-n as colunas são sempre recriadas sem a necessidade de manter o histórico pois
      o usuário não consegue referenciar as colunas criadas pelos relacionamentos n-n.*/
    if(nome_ant!=nome && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N))
     nome_ant_cols_ref[coluna->getObjectId()]=coluna->getName();

   /* Adiciona a coluna na tabela a qual foi definida para receber os
      atributos, colunas e restições */
   tab_receptora->adicionarColuna(coluna);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarColunasRel11(void)
{
 Tabela *tab_ref=NULL, *tab_recep=NULL;

 try
 {
  /* Definindo a ordem das tabelas. O parâmetro
     tab_orig indica de onde está partindo o relacionamento
     e o tab_dest onde está chegando o relacionamento e também
     a tabela que receberá as colunas que representam a ligação.
     Além disso, os atributos e restrições do relacionamento serão
     sempre incluídos na tabela de destino */

  /* Caso 1: (0,1) ---<>--- (0,1)
     Caso 2: (1,1) ---<>--- (0,1) */
  /*if((!obrig_orig && !obrig_dest) ||
     (obrig_orig && !obrig_dest))
  {
   tab_ref=dynamic_cast<Tabela *>(tabela_orig);
   tab_recep=dynamic_cast<Tabela *>(tabela_dest);
  }*/
  /* Caso 3: (0,1) ---<>--- (1,1)
             Adição de colunas na tabela cuja cardinalidade mínima é 0
             (opcionalidade de participação no relacionamento) */
  /*else if(!obrig_orig && obrig_dest)
  {
   tab_ref=dynamic_cast<Tabela *>(tabela_dest);
   tab_recep=dynamic_cast<Tabela *>(tabela_orig);
  }*/
  //### Caso 4: (1,1) ---<>--- (1,1) (não implementado por quebrar a modelagem) ###
  TipoAcao acao_del;

  tab_ref=dynamic_cast<Tabela *>(this->obterTabelaReferencia());
  tab_recep=dynamic_cast<Tabela *>(this->obterTabelaReceptora());

  //Caso a tabela de referência seja obrigatória seta como RESTRICT a ação de delete na chave estrangeira
  if((tab_ref==this->tabela_orig && this->tabelaObrigatoria(TABELA_ORIGEM)) ||
     (tab_ref==this->tabela_dest && this->tabelaObrigatoria(TABELA_DESTINO)))
    acao_del=TipoAcao::restrict;
  else
    acao_del=TipoAcao::set_null;

  if(autoRelacionamento())
  {
   adicionarAtributos(tab_recep);
   adicionarRestricoes(tab_recep);
   copiarColunas(tab_ref, tab_recep, false);
   adicionarChaveEstrangeira(tab_ref, tab_recep, acao_del, TipoAcao::cascade);
   adicionarChaveUnica(tab_ref, tab_recep);
  }
  else
  {
   copiarColunas(tab_ref, tab_recep, false);

   if(identificador)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->definirTabelaObrigatoria(TABELA_DESTINO, false);
    this->definirTabelaObrigatoria(TABELA_ORIGEM, false);

    if(tab_ref==this->tabela_orig)
     this->definirTabelaObrigatoria(TABELA_ORIGEM, true);
    else
     this->definirTabelaObrigatoria(TABELA_DESTINO, true);

    configurarRelIdentificador(tab_recep);
   }

   adicionarAtributos(tab_recep);
   adicionarRestricoes(tab_recep);

   if(identificador)
   {
    adicionarChaveEstrangeira(tab_ref, tab_recep, TipoAcao::cascade, TipoAcao::cascade);
   }
   else
   {
    adicionarChaveEstrangeira(tab_ref, tab_recep, acao_del,  TipoAcao::cascade);
    adicionarChaveUnica(tab_ref, tab_recep);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarColunasRel1n(void)
{
 Tabela *tab_ref=NULL, *tab_recep=NULL;
 bool nao_nulo=false;
 TipoAcao acao_del=TipoAcao::set_null, acao_upd=TipoAcao::cascade;

 try
 {
  /* Para relacionamentos 1-n a ordem das tabelas não se alteram, ou seja,
     as colunas da chave estrangeira são sempre adicionadas na tabela
     de origem */
  tab_recep=dynamic_cast<Tabela *>(this->obterTabelaReceptora());
  tab_ref=dynamic_cast<Tabela *>(this->obterTabelaReferencia());

  /* Caso o relacionamento não seja identificador e a participação da tabela
     de referência (origem) seja obrigatória (1,1)--<>--(0|1,n) as colunas da chave estrangeiras
     não podem aceitar valores nulos e além disso as ações ON DELETE e ON UPDATE
     será RESTRIC */
  if(!identificador && obrig_orig)
  {
   if(!postergavel)
    acao_del=TipoAcao::restrict;
   else
    acao_del=TipoAcao::no_action;

   nao_nulo=true;
  }
  /* Caso o relacionamento seja identificador configura as ações ON DELETE e ON UPDATE
     da chave estrangeira como "cascade" pois a entidade fraca só existe se
     a entidade forte existir, isto é, se uma tupla da tabela "entidade forte" for
     removida todas as tuplas da tabela "entidade fraca" também serão excluídas */
  else if(identificador)
   acao_del=TipoAcao::cascade;

  if(autoRelacionamento())
  {
   adicionarAtributos(tab_recep);
   adicionarRestricoes(tab_recep);
   copiarColunas(tab_ref, tab_recep, nao_nulo);
   adicionarChaveEstrangeira(tab_ref, tab_recep, acao_del, acao_upd);
  }
  else
  {
   copiarColunas(tab_ref, tab_recep, nao_nulo);

   if(identificador)
   {
    /* Quando o relacionamento é identificador, serão desprezadas as cardinalidades
       das tabelas pois, obrigatóriamente a entidade forte tem participação mandatória
        na entidade fraca, sendo assim, marca a tabela de referência como obrigatória */
    this->definirTabelaObrigatoria(TABELA_ORIGEM, true);
    this->definirTabelaObrigatoria(TABELA_DESTINO, false);

    configurarRelIdentificador(tab_recep);
   }

   adicionarAtributos(tab_recep);
   adicionarRestricoes(tab_recep);

   adicionarChaveEstrangeira(tab_ref, tab_recep, acao_del, acao_upd);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Relacionamento::adicionarColunasRelNn(void)
{
 Tabela *tab=NULL, *tab1=NULL;
 Constraint *pk_tabnn=NULL;
 bool nao_nulo_orig=false, nao_nulo_dest=false;
 //TipoAcao acao_orig=TipoAcao::set_null, acao_dest=TipoAcao::set_null;
 TipoAcao acao_del_orig=TipoAcao::restrict, acao_del_dest=TipoAcao::restrict,
          acao_upd_orig=TipoAcao::cascade, acao_upd_dest=TipoAcao::cascade;
 unsigned i, qtd;

 try
 {
  /* Para relacionamentos n-n, a tabela de destino onde serão adicionadas as colunas
     será a tabela que representa o relacionamento (tabela_relnn). As tabelas
     participantes do relacionamento sempre serão as tabelas de origem pois as colunas
     destas precisam ser adicionadas na tabela que define o relacionamento */
  tab=dynamic_cast<Tabela *>(tabela_orig);
  tab1=dynamic_cast<Tabela *>(tabela_dest);

  /* Pela implementação correta (documental) a cardinalidade do rel. n-n é ignorada
     gerando sempre uma tabela própria cuja chave primária é a junção das
     colunas que representam ambas as tabelas, logo a implementação abaixo
     foi desativada */
  /* if(obrig_orig)
  {
   acao_orig=TipoAcao::restrict;
   nao_nulo_orig=true;
  }

  if(obrig_dest)
  {
   acao_dest=TipoAcao::restrict;
   nao_nulo_dest=true;
  } */

  copiarColunas(tab, tabela_relnn, nao_nulo_orig);
  copiarColunas(tab1, tabela_relnn, nao_nulo_dest);

  /* Cria a chave primária padrão da tabela que consiste nas colunas que
     identificam cada chave estrangeira na tabela. */
  pk_tabnn=new Constraint;
  pk_tabnn->setName(tabela_relnn->getName() + "_pk");
  pk_tabnn->definirTipo(TipoRestricao::primary_key);
  pk_tabnn->setAddedByLinking(true);
  qtd=colunas_ref.size();

  for(i=0; i < qtd; i++)
   pk_tabnn->adicionarColuna(colunas_ref[i],Constraint::COLUNA_ORIGEM);

  tabela_relnn->adicionarRestricao(pk_tabnn);

  adicionarAtributos(tabela_relnn);
  adicionarRestricoes(tabela_relnn);

  adicionarChaveEstrangeira(tab, tabela_relnn, acao_del_orig, acao_upd_orig);
  adicionarChaveEstrangeira(tab1, tabela_relnn, acao_del_dest, acao_upd_dest);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

Tabela *Relacionamento::obterTabelaReferencia(void)
{
 /* Para relacionamentos n-n que possuem 2 tabelas de refência,
    este método sempre retornará NULL. */
 if(tipo_relac==RELACIONAMENTO_NN)
  return(NULL);
 else
 {
  /* Caso a tabela de origem do relacionamento seja também a
     tabela receptora das colunas que representam o relacionamento,
     retorna que a tabela de destino do relacionamento e a tabela
     de referência para cópia de colunas. */
  if(tabela_orig==obterTabelaReceptora())
   return(dynamic_cast<Tabela *>(tabela_dest));
  else
   return(dynamic_cast<Tabela *>(tabela_orig));
 }
}

Tabela *Relacionamento::obterTabelaReceptora(void)
{
 if(tipo_relac==RELACIONAMENTO_11)
 {
  /* Caso 1: (0,1) ---<>--- (0,1)
     Caso 2: (1,1) ---<>--- (0,1) */
  if((!obrig_orig && !obrig_dest) ||
      (obrig_orig && !obrig_dest))
   return(dynamic_cast<Tabela *>(tabela_dest));
  /* Caso 3: (0,1) ---<>--- (1,1)
              Adição de colunas na tabela cuja cardinalidade mínima é 0
              (opcionalidade de participação no relacionamento) */
  else if(!obrig_orig && obrig_dest)
   return(dynamic_cast<Tabela *>(tabela_orig));
   // Caso 4: (1,1) ---<>--- (1,1)
  else
    /* Caso o usuário tente criar um relacionamento 1-1 onde ambas as entidades
     têm participação obrigatório, será retornado um valor nulo pois este tipo
     de relacionamento não está implementado por quebrar a modelagem feita
     pelo usuário */
  return(NULL);
 }
 /* Para relacionamentos 1-n , a ordem das
    tabelas não se alteram, ou seja, as colunas são sempre adicionadas
    na tabela de destino */
 else if(tipo_relac==RELACIONAMENTO_1N)
  return(dynamic_cast<Tabela *>(tabela_dest));
  //return(dynamic_cast<Tabela *>(tabela_orig));
 /* Para relacionamentos gen ou dep as colunas são sempre adicionadas
    na tabela de origem do relacionamento */
 else if(tipo_relac==RELACIONAMENTO_GEN ||
         tipo_relac==RELACIONAMENTO_DEP)
  return(dynamic_cast<Tabela *>(tabela_orig));
 /* Para relacionamentos n-n, a tabela de destino onde serão adicionadas as colunas
    será a tabela que representa o relacionamento (tabela_relnn). As tabelas
    participantes do relacionamento sempre serão as tabelas de origem pois as colunas
   destas precisam ser adicionadas na tabela que define o relacionamento */
 else
  return(dynamic_cast<Tabela *>(tabela_relnn));
}

void Relacionamento::removerObjetosTabelaRefCols(Tabela *tabela)
{
 Gatilho *gat=NULL;
 Index *ind=NULL;
 Constraint *rest=NULL;
 int i, qtd;

 //Remove os gatilhos que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumGatilhos();
 for(i=0; i < qtd; i++)
 {
  gat=tabela->obterGatilho(i);
  if(gat->isReferRelationshipColumn())
  {
   tabela->removerObjeto(gat);
   delete(gat);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove os índices que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumIndices();
 for(i=0; i < qtd; i++)
 {
  ind=tabela->obterIndice(i);
  if(ind->isReferRelationshipColumn())
  {
   tabela->removerObjeto(ind);
   delete(ind);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }

 //Remove as restrições que referenciam alguma coluna inc. por relação
 qtd=tabela->obterNumRestricoes();
 for(i=0; i < qtd; i++)
 {
  rest=tabela->obterRestricao(i);
  if(!rest->isAddedByRelationship() &&
     rest->obterTipoRestricao()!=TipoRestricao::primary_key &&
     rest->referenciaColunaIncRelacao())
  {
   tabela->removerObjeto(rest);
   delete(rest);
   qtd--; i--;
   if(i < 0) i=0;
  }
 }
}

void Relacionamento::removerColsChavePrimariaTabela(Tabela *tabela)
{
 if(tabela)
 {
  Constraint *pk=NULL;
  Column *coluna=NULL;
  unsigned i, qtd;

  /* Obtém a chave primáira da tabela e remove desta
     as colunas adicionadas por relacionamento */
  pk=tabela->obterChavePrimaria();

  /* Verifica se a chave estrangeira da tabela existe.
     Normalmente, esta chave sempre existirá, porém
     caso a mesma deixe de existir por algum erro anterior a desconexão
     do relacionamento, como é o caso do método de validação
     ModeloBD::validarRefColsIncRelacao() o qual remove restrições caso
     estas se tornem inválidas ou seja referenciem colunas de tabelas
     que deixaram de existir, por isso é prudente sempre verificar se
     a chave primária da tabela existe para não provocar segmentations fault */
  if(pk)
  {
   qtd=pk->obterNumColunas(Constraint::COLUNA_ORIGEM);

   for(i=0; i < qtd; i++)
   {
    //Obtém uma coluna da chave primária
    coluna=pk->obterColuna(i, Constraint::COLUNA_ORIGEM);

    /* Caso a coluna foi incluída por relacionamento e a mesma pertence
       ao próprio relacionamento */
    if(coluna->isAddedByRelationship() &&
      (colunaExistente(coluna) || obterIndiceObjeto(coluna) >= 0))
    {
     //Remove a coluna da chave primária
     pk->removerColuna(coluna->getName(), Constraint::COLUNA_ORIGEM);
     i--; qtd--;
    }
   }
  }
 }
}

void Relacionamento::desconectarRelacionamento(bool rem_objs_tab)
{
 try
 {
  if(conectado)
  {
   vector<Column *>::iterator itr, itr_end;
   Column *coluna=NULL;
   Tabela *tabela=NULL;
   unsigned idx_lista=0;
   vector<TableObject *> *lista_atrib=NULL;
   vector<TableObject *>::iterator itr_atrib, itr_atrib_end;
   TableObject *obj_tab=NULL;
   //TipoObjetoBase tipo_obj;

   if(tipo_relac==RELACIONAMENTO_GEN ||
      tipo_relac==RELACIONAMENTO_DEP)
   {
    tabela=obterTabelaReceptora();

    if(rem_objs_tab)
     removerObjetosTabelaRefCols(tabela);

     removerColsChavePrimariaTabela(tabela);

    //Remove a tabela cópia/pai dependendo do tipo do relacionamento
    if(tipo_relac==RELACIONAMENTO_GEN)
     tabela->removerObjeto(obterTabelaReferencia()->getName(true), OBJ_TABLE);
    else
     tabela->removerObjeto(obterTabelaReferencia()->getName(true), BASE_TABLE);
   }
   else
   {
    Constraint *pk=NULL, *rest=NULL;
    unsigned i, qtd;

    /* No caso de relacionamento 1-1 e 1-n é necessário
       remover a chave estrangeira que representa o
       relacionamento da tabela, além disso colunas
       adicionadas   chave primária (no caso de um
       relacionamento identificador) precisam ser removidas */
    if(fk_rel1n && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N))
    {
     /* Obtém a tabela a qual possui a chave estrangeira que representa o
        relacionamento (tabela esta onde foi inserida a chave estrangeira
        no momento da conexão do relacionamento) */
     tabela=dynamic_cast<Tabela *>(fk_rel1n->getParentTable());

     //Remove a chave estrangeira da tabela
     tabela->removerRestricao(fk_rel1n->getName());

     /* Obtém a chave primária da tabela para checar se a mesma é igual a chave primária
        que define o relacionamento identificador */
     pk=tabela->obterChavePrimaria();

     //Remove as colunas criadas pelo relacionamento da chave primária da tabela
     removerColsChavePrimariaTabela(tabela);

     if(rem_objs_tab)
      removerObjetosTabelaRefCols(tabela);

     /* Remove as colunas da chave estrangeira e a desaloca, apenas para
        relacionamentos 1-1 e 1-n. */
     fk_rel1n->removerColunas();
     delete(fk_rel1n);
     fk_rel1n=NULL;

     if(uq_rel11)
     {
      //Remove a chave única da tabela
      tabela->removerRestricao(uq_rel11->getName());
      uq_rel11->removerColunas();
      delete(uq_rel11);
      uq_rel11=NULL;
     }

     /* Remove a chave primária da tabela caso esta foi criada automaticamente
        (caso de relacionamento identificador e entidade fraca sem chave primária) */
     if(pk && pk==this->pk_relident)
     {
      /* Obtém a tabela a qual possui a chave primária criada pelo relacionamento
         caso este seja um relacionamento identificador */
      tabela=dynamic_cast<Tabela *>(pk_relident->getParentTable());
      //Remove a chave primária da tabela
      tabela->removerRestricao(pk_relident->getName());

      //Desaloca a chave primária
      delete(pk);
      pk_relident=NULL;
     }
    }
    else if(tipo_relac==RELACIONAMENTO_NN)
    {
     /* Caso o relacionamento seja n-n, apenas remove as restrições sem
        qualquer validação */
     qtd=tabela_relnn->obterNumRestricoes();

     for(i=0; i < qtd ; i++)
     {
      //Obtém a restrição
      rest=tabela_relnn->obterRestricao(i);

      /* Caso a restrição foi adicionada por lgação porém não é uma restrição criada pelo usuário e
         e sim uma restrição criada quando o relacionamento é conectado (ex. chave primária e chaves estrangeiras).
         A segunda parte da condição obterIndiceObjeto(rest) < 0 verifica se a restrição a ser removida não fazer
         parte da lista de restrições criadas pelo usuário, caso faça parte, não será destruída */
      if(rest->isAddedByRelationship() && obterIndiceObjeto(rest) < 0)
      {
       //Remove a restrição da tabela
       tabela_relnn->removerRestricao(rest->getName());
       i--; qtd--;
       delete(rest);
      }
     }
    }
   }

   /* Remover os atributos e restrições do relacionamento os quais
      estão incluídos nas tabelas participantes. */
   tabela=obterTabelaReceptora();
   while(idx_lista <= 1)
   {
    /* Seleciona a lista de objetos, a primeira lista sempre deve ser a
       de restrições para evitar de se remover colunas antes das restrições
       gerando erro de remoção de coluna referenciada */
    lista_atrib=(idx_lista==0 ? &restricoes_rel : &atributos_rel);

    itr_atrib=lista_atrib->begin();
    itr_atrib_end=lista_atrib->end();

    //Varre a lista de atributos selecionada
    while(itr_atrib!=itr_atrib_end)
    {
     //Obtém o atributo
     obj_tab=(*itr_atrib);
     //tipo_obj=obj_tab->obterTipoObjeto();

     /* É necessário verificar se o objeto pertence ao relacionamento
        a fim de evitar que atributos adicionados por outros relacionamentos
          tabela também sejam removidos */
     if(tabela && obterIndiceObjeto(obj_tab) >= 0)
     {
      //Remove o atributo da tabela através do nome e tipo
      tabela->removerObjeto(obj_tab->getName(), obj_tab->getObjectType());
      obj_tab->setParentTable(NULL);
     }
     //Para para o atributo posterior
     itr_atrib++;
    }
    //Passa para a lista seguinte de atributos
    idx_lista++;
   }

   //Excluindo as colunas incluídas na tabela pelo relacionamento
   itr=colunas_ref.begin();
   itr_end=colunas_ref.end();

   /*  Varre a lista de colunas do relacionamento removendo cada uma
      da tabela onde foi inserida e desalocando o espaço que esta
      ocupa em memória */
   while(itr!=itr_end)
   {
    coluna=(*itr);
    //Remove a coluna da tabela pai
    tabela->removerColuna(coluna->getName());
    itr++;
    //Desaloca a coluna
    delete(coluna);
   }

   //Limpa as lista de coluna do relacionamento
   colunas_ref.clear();
   colunas_pk.clear();
   sufixo_cols.clear();

   /* Caso o relacionamento seja n-n desaloca a tabela que representa
      o relacionamento */
   if(tabela_relnn)
   {
    delete(tabela_relnn);
    tabela_relnn=NULL;
   }

   //Caso a chave primária especial esteja alocada-remove-a
   if(pk_especial)
   {
    delete(pk_especial);
    pk_especial=NULL;
   }

   //Executa o método de desconexão de relacionamento da classe base
   RelacionamentoBase::desconectarRelacionamento();
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

bool Relacionamento::relacionamentoIdentificador(void)
{
 return(identificador);
}

bool Relacionamento::possuiAtributoIdentificador(void)
{
 vector<TableObject *>::iterator itr, itr_end;
 Constraint *rest=NULL;
 bool enc=false;

 itr=restricoes_rel.begin();
 itr_end=restricoes_rel.end();

 //Varre a lista de restrições
 while(itr!=itr_end && !enc)
 {
  //Obtém uma restrição da lista
  rest=dynamic_cast<Constraint *>(*itr);
  /* Um relacionamento será considerado possuir um
     atributo identificandor quando for encontrada
     uma chave primária em sua lista de restrições */
  enc=(rest->obterTipoRestricao()==TipoRestricao::primary_key);
  itr++;
 }

 return(enc);
}

bool Relacionamento::relacionamentoInvalidado(void)
{
 unsigned qtd_cols_rel, qtd_cols_tab, i, i1, qtd;
 Tabela *tabela=NULL, *tabela1=NULL;
 Constraint *fk=NULL, *fk1=NULL, *rest=NULL, *pk=NULL;
 bool valido=false;
 Column *col1=NULL, *col2=NULL, *col3=NULL;
 QString nome_col;

 /* Caso o relacionamento foi invaldado por modificação
    de atributos críticos já retorna a invalidez sem
    executar os procedimentos de validação abaixo */
 if(invalidado)
 {
  /* Caso o relacionamento seja identificador, remove a chave primária
     criada automaticamente quando o mesmo é conectado para forçar
     a tabela receptora fique sem chave primária em consequência
     todo e qualquer relacionamento 1-1, 1-n ou n-n ligado a ela
     deverá ser revalidado */
  if(pk_relident && pk_relident->isAddedByLinking())
  {
   dynamic_cast<Tabela *>(pk_relident->getParentTable())->removerObjeto(pk_relident);
   pk_relident=NULL;
  }
  return(true);
 }
 /* Caso o relacionamento esteja conectado e não esteja invalidado
    por modificação de atributos */
 else if(conectado)
 {
   /* Valida os sufixos caso a geração automática de sufixos esteja ativa.
      Checa se os sufixos, quando preenchidos, coincidem  com os nomes das tabelas respectivas */
   if(sufixo_auto &&
      ((!sufixo_orig.isEmpty() &&  sufixo_orig!=QString(SEPARADOR_SUFIXO) + tabela_orig->getName()) ||
       (!sufixo_dest.isEmpty() &&  sufixo_dest!=QString(SEPARADOR_SUFIXO) + tabela_dest->getName())))
    return(true);

  /* Pare relacionamentos 1-1 e 1-n a verificação de
     invalidação do relacionamento baseia-se na comparação da
     quantidade de colunas da chave estrangeira configurada
     pela conexão do mesmo com a quantidade de colunas da
     chave primária da tabela de origem do relacionamento */
  if(tipo_relac==RELACIONAMENTO_11 ||
     tipo_relac==RELACIONAMENTO_1N)
  {
   //Obtém a tabela de referencia do relacionamento
   tabela=obterTabelaReferencia();

   //Obtém a quantidade de colunas da chave estrangeira que representa a relação
   qtd_cols_rel=fk_rel1n->obterNumColunas(Constraint::COLUNA_ORIGEM);

   //O relacionamento estára invalidado caso a tabela referência não possua uma chave primária
   pk=tabela->obterChavePrimaria();

   if(pk)
   {
    //Obtém a quantidade de colunas da chave primária da tabela
    qtd_cols_tab=pk->obterNumColunas(Constraint::COLUNA_ORIGEM);

    //Obtém a tabela de referencia do relacionamento
    tabela1=obterTabelaReceptora();

    //Faz a verificação de quantidade de colunas
    valido=(qtd_cols_rel==qtd_cols_tab);

    //A próxima validação é a de nome e tipo das colunas
    for(i=0; i < qtd_cols_rel && valido; i++)
    {
     //Obtém a da chave estrangeira
     col2=colunas_ref[i];

     //Obtém a coluna da chave primária ligd   coluna atual da chave estrangeira
     col1=colunas_pk[i];

     /* Obtém a coluna da pk em si. Com esta referência será verificado se os endereços são iguais
        caso não sejam invalida o relacionamento */
     col3=pk->obterColuna(i, Constraint::COLUNA_ORIGEM);

     /* Para se validar as colunas entre si as seguintes regras são seguidas:

      1) Verifica se os nomes são iguais. Caso seja iguais nada é feito.
         Caso sejam diferentes é necessário verificar se já existe uma coluna
         na tabela receptora com o mesmo nome da coluna atual da chave primária,
         isso indica que a coluna da chave primária precisou ser renomeada na tabela
         receptora pois esta já possuia uma coluna de mesmo nome.

      2) Verifica se os tipos das colunas são compatíveis.
         A única exceção aceita é se o tipo da coluna de origem é do tipo 'serial' ou 'bigserial'
         e da coluna de destino seja 'integer' ou 'bigint'

      3) Checa se a coluna (endereço) vindo do vetor colunas_pk é iga   coluna
         obtida diretamente da chave primária */
     nome_col=col1->getName() + sufixo_cols[i];
     valido=(col1==col3 &&
             (nome_col==col2->getName()) &&
             (col1->getType()==col2->getType() ||
             (col1->getType()=="serial" && col2->getType()=="integer") ||
             (col1->getType()=="bigserial" && col2->getType()=="bigint")));
    }
   }
  }
  /* Para relacionamentos de dependência e generalização,
     obtem-se a quantidade de colunas criadas pela ligação do
     relacionamento e a compara com a quantidade de colunas
     da tabela de origem */
  else if(tipo_relac==RELACIONAMENTO_DEP ||
          tipo_relac==RELACIONAMENTO_GEN)
  {
   //Obtém a tabela de referencia do relacionamento
   tabela=obterTabelaReferencia();

   //Obtém a tabela de referencia do relacionamento
   tabela1=obterTabelaReceptora();

   /* Obtém o número total de colunas do relacionamento, incluído as
      que foram adicionadas por relacionamentos */
   qtd_cols_tab=tabela->obterNumColunas();

   /* Obtém a quantidade de colunas criadas com a conexão da relação somando-se
      com a quantidade de colunas rejeitadas no momento da ligaçã de acordo
      com as regras */
   qtd_cols_rel=colunas_ref.size() + qtd_cols_rejeitadas;
   valido=(qtd_cols_rel == qtd_cols_tab);

   /* Checando se as colunas criadas com a herança/dependência ainda existem
      na tabela de referência */
   for(i=0; i < colunas_ref.size() && valido; i++)
    valido=tabela->obterColuna(colunas_ref[i]->getName(true));

   /* Checando se as colunas da tabela referência existem na tabela
      receptora. Na teoria todas as colunas devem existir pois uma
      herda a outra logo todos possuirão as mesmas colunas. Caso isso
      não acontença indica que uma coluna da tabela de referência foi
      renomeada */
   for(i=0; i < qtd_cols_tab && valido; i++)
    valido=tabela1->obterColuna(tabela->obterColuna(i)->getName(true));
  }
  /* Para relacionamentos n-n, é necessário as comparações:
     1) Pega-se a chave estrangeira da tabela criada pela ligação
     a qual referencia a tabela de origem e verifica se as quantidades
     de colunas coincidem. O mesmo é feito para a segunda chave estrangeira
     só que em rela   chave primaria da tabela de destino
     2) É necessário validar se os nomes das colunas da tabela gerada
        coincidem com os nomes das colunas das tabelas originárias */
  else if(tipo_relac==RELACIONAMENTO_NN)
  {
   //Obtém a referência para a tabela de destino do relacionamento
   tabela=dynamic_cast<Tabela *>(tabela_orig);
   tabela1=dynamic_cast<Tabela *>(tabela_dest);

   /* Para se validado um relacionamento n-n, a primeira condição é que
      ambas as tabelas possuam chave estrangeira */
   if(tabela->obterChavePrimaria() && tabela1->obterChavePrimaria())
   {
    //Obtém a quantidade de restrições da tabela criada pelo relacionamento
    qtd=tabela_relnn->obterNumRestricoes();

    //Varre a lista de restrições da tabela
    for(i=0; i < qtd; i++)
    {
     //Obtém uma restrição
     rest=tabela_relnn->obterRestricao(i);

     //Caso a mesma seja uma chave estrangeira
     if(rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     {
      /* Verifica se a tabela referenciada pela chave é a tabela de origem
         caso seja, armazena seu endereço na referêni   chave estrangeira
         da origem */
      if(!fk && rest->obterTabReferenciada()==tabela)
       fk=rest;
      else if(!fk1 && rest->obterTabReferenciada()==tabela1)
       fk1=rest;
     }
    }

    /* A quantidade de colunas do relacionamento é calculada pela soma
       das quantidades de colunas das chaves estrangeiras obtidas */
    qtd_cols_rel=fk->obterNumColunas(Constraint::COLUNA_REFER) + fk1->obterNumColunas(Constraint::COLUNA_REFER);
    /* A quantidade de colunas da tabela é obtida pela soma da quantidade
       de colunas das chaves primárias envolvidas no relacionamentos */
    qtd_cols_tab=tabela->obterChavePrimaria()->obterNumColunas(Constraint::COLUNA_ORIGEM) +
                 tabela1->obterChavePrimaria()->obterNumColunas(Constraint::COLUNA_ORIGEM);
    valido=(qtd_cols_rel == qtd_cols_tab);

    /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de referência */
    qtd=fk->obterNumColunas(Constraint::COLUNA_ORIGEM);
    for(i=0; i < qtd && valido; i++)
    {
     nome_col=fk->obterColuna(i, Constraint::COLUNA_ORIGEM)->getName();

     /* Caso o sufixo da origem esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de origem */
     if(!sufixo_cols[i].isEmpty())
      nome_col=nome_col.remove(sufixo_cols[i]);

     //Verifica a existencia da coluna na tabela
     col1=tabela->obterColuna(nome_col);
     valido=col1 &&
            tabela->obterChavePrimaria()->colunaExistente(col1, Constraint::COLUNA_ORIGEM);
    }

   /* Checando se as colunas criadas com a ligação ainda existem
      na tabela de receptora */
    i1=qtd;
    qtd+=fk1->obterNumColunas(Constraint::COLUNA_ORIGEM);
    for(i=0; i1 < qtd && valido; i1++)
    {
     nome_col=fk1->obterColuna(i++, Constraint::COLUNA_ORIGEM)->getName();

     /* Caso o sufixo do destino esteja especificado remove o mesmo do nome
        da coluna para que a mesma seja localizada na tabela de destino */
     if(!sufixo_cols[i1].isEmpty())
      nome_col=nome_col.remove(sufixo_cols[i1]);

     //Verifica a existencia da coluna na tabela
     col1=tabela1->obterColuna(nome_col);
     valido=col1 &&
            tabela1->obterChavePrimaria()->colunaExistente(col1, Constraint::COLUNA_ORIGEM);
    }
   }
  }
  return(!valido);
 }
 else
  return(true);
}

QString Relacionamento::getCodeDefinition(unsigned tipo_def)
{
 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  if(fk_rel1n && (tipo_relac==RELACIONAMENTO_11 || tipo_relac==RELACIONAMENTO_1N))
  {
   unsigned qtd, i;

   attributes[ParsersAttributes::RELATIONSHIP_1N]="1";
   attributes[ParsersAttributes::CONSTRAINTS]=fk_rel1n->getCodeDefinition(tipo_def);

   if(uq_rel11)
    attributes[ParsersAttributes::CONSTRAINTS]+=uq_rel11->getCodeDefinition(tipo_def);

   qtd=restricoes_rel.size();
   for(i=0; i < qtd; i++)
   {
    if(dynamic_cast<Constraint *>(restricoes_rel[i])->obterTipoRestricao()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(restricoes_rel[i])->
                                              getCodeDefinition(tipo_def, false);

   }

   attributes[ParsersAttributes::TABLE]=obterTabelaReceptora()->getName(true);
  }
  else if(tabela_relnn && tipo_relac==RELACIONAMENTO_NN)
  {
   unsigned qtd, i;

   attributes[ParsersAttributes::RELATIONSHIP_NN]="1";
   attributes[ParsersAttributes::TABLE]=tabela_relnn->getCodeDefinition(tipo_def);

   qtd=tabela_relnn->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    if(tabela_relnn->obterRestricao(i)->obterTipoRestricao()!=TipoRestricao::primary_key)
     attributes[ParsersAttributes::CONSTRAINTS]+=tabela_relnn->obterRestricao(i)->getCodeDefinition(tipo_def, true);
   }
  }
  else if(tipo_relac==RELACIONAMENTO_GEN)
  {
   attributes[ParsersAttributes::RELATIONSHIP_GEN]="1";
   attributes[ParsersAttributes::TABLE]=obterTabelaReceptora()->getName(true);
   attributes[ParsersAttributes::ANCESTOR_TABLE]=obterTabelaReferencia()->getName(true);
  }

  return(this->BaseObject::__getCodeDefinition(SchemaParser::SQL_DEFINITION));
 }
 else
 {
  unsigned qtd, i;
  bool forma_reduzida;

  definirAtributosRelacionamento();
  attributes[ParsersAttributes::SRC_SUFFIX]=(!sufixo_auto ? sufixo_orig : "");
  attributes[ParsersAttributes::DST_SUFFIX]=(!sufixo_auto ? sufixo_dest : "");
  attributes[ParsersAttributes::IDENTIFIER]=(identificador ? "1" : "");
  attributes[ParsersAttributes::DEFERRABLE]=(postergavel ? "1" : "");
  attributes[ParsersAttributes::AUTO_SUFFIX]=(sufixo_auto ? "1" : "");
  attributes[ParsersAttributes::DEFER_TYPE]=~tipo_postergacao;
  attributes[ParsersAttributes::TABLE_NAME]=nome_tab_relnn;


  attributes[ParsersAttributes::COLUMNS]="";
  qtd=atributos_rel.size();
  for(i=0; i < qtd; i++)
  {
   attributes[ParsersAttributes::COLUMNS]+=dynamic_cast<Column *>(atributos_rel[i])->
                    getCodeDefinition(SchemaParser::XML_DEFINITION);
  }

  attributes[ParsersAttributes::CONSTRAINTS]="";
  qtd=restricoes_rel.size();
  for(i=0; i < qtd; i++)
  {
   if(!restricoes_rel[i]->isProtected())
    attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(restricoes_rel[i])->
                       getCodeDefinition(SchemaParser::XML_DEFINITION, true);
  }

  qtd=id_colunas_pk_rel.size();
  for(i=0; i < qtd; i++)
  {
   //Armazena o nome das colunas da chave primária especial se houver
   if(!colunas_ref.empty() && i < colunas_ref.size())
   {
    attributes[ParsersAttributes::SPECIAL_PK_COLS]+=QString("%1").arg(id_colunas_pk_rel[i]);
    if(i < qtd-1) attributes[ParsersAttributes::SPECIAL_PK_COLS]+=",";
   }
  }

  /* Caso não haja colunas, restrições e linha definida no relacionamento
     a definição XML do rel. será em forma reduzida */
  forma_reduzida=(attributes[ParsersAttributes::COLUMNS].isEmpty() &&
                  attributes[ParsersAttributes::CONSTRAINTS].isEmpty() &&
                  attributes[ParsersAttributes::POINTS].isEmpty() &&
                  attributes[ParsersAttributes::SPECIAL_PK_COLS].isEmpty());


  return(this->BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION, forma_reduzida));
 }
}

void Relacionamento::definirSufixoAutomatico(bool valor)
{
 this->invalidado=(this->sufixo_auto!=valor);
 this->sufixo_auto=valor;
}

bool Relacionamento::obterSufixoAutomatico(void)
{
 return(this->sufixo_auto);
}

void Relacionamento::operator = (Relacionamento &rel)
{
 (*dynamic_cast<RelacionamentoBase *>(this))=dynamic_cast<RelacionamentoBase &>(rel);
 this->invalidado=true;
 this->id_colunas_pk_rel=rel.id_colunas_pk_rel;
 this->atributos_rel=rel.atributos_rel;
 this->restricoes_rel=rel.restricoes_rel;
 this->sufixo_dest=rel.sufixo_dest;
 this->sufixo_orig=rel.sufixo_orig;
 this->identificador=rel.identificador;
 this->tipo_postergacao=rel.tipo_postergacao;
 this->postergavel=rel.postergavel;
 this->nome_tab_relnn=rel.nome_tab_relnn;
 this->tabela_relnn=NULL;
 this->fk_rel1n=pk_relident=pk_especial=NULL;
 this->colunas_ref.clear();
 this->sufixo_auto=rel.sufixo_auto;
}

