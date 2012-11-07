#include "listaoperacoes.h"

template <class Classe>
void copiarObjeto(BaseObject **pobj_orig, Classe *obj_copia)
{
 Classe *obj_orig=NULL;

 //Obtém o endereço do objeto armazenado no ponteiro
 obj_orig=dynamic_cast<Classe *>(*pobj_orig);

 //Caso o objeto de cópia não esteja aloca será disparada uma exceção
 if(!obj_copia)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o objeto de origem não esteja alocado
    o mesmo será alocado */
 if(!obj_orig)
 {
  obj_orig=new Classe;
  (*pobj_orig)=obj_orig;
 }

 //Efetua a cópia entre os objetos
 (*obj_orig)=(*obj_copia);
}

void copiarObjeto(BaseObject **pobj_orig, BaseObject *obj_copia, ObjectType tipo)
{
 switch(tipo)
 {
  case OBJ_RELATIONSHIP:
    Relacionamento *rel1;
    rel1=new Relacionamento(dynamic_cast<Relacionamento *>(obj_copia));
    if(!(*pobj_orig))
     (*pobj_orig)=rel1;
    else
     (*(dynamic_cast<Relacionamento *>(*pobj_orig)))=(*rel1);
  break;
  case BASE_RELATIONSHIP:
    RelacionamentoBase *rel;
    rel=new RelacionamentoBase(dynamic_cast<RelacionamentoBase *>(obj_copia));
    if(!(*pobj_orig))
     (*pobj_orig)=rel;
    else
     (*(dynamic_cast<RelacionamentoBase *>(*pobj_orig)))=(*rel);
  break;
  case OBJ_COLUMN:
    copiarObjeto(pobj_orig, dynamic_cast<Column *>(obj_copia));
  break;
  case OBJ_CONSTRAINT:
    copiarObjeto(pobj_orig, dynamic_cast<Restricao *>(obj_copia));
  break;
  case OBJ_TRIGGER:
    copiarObjeto(pobj_orig, dynamic_cast<Gatilho *>(obj_copia));
  break;
  case OBJ_RULE:
    copiarObjeto(pobj_orig, dynamic_cast<Regra *>(obj_copia));
  break;
  case OBJ_INDEX:
    copiarObjeto(pobj_orig, dynamic_cast<Indice *>(obj_copia));
  break;
  case OBJ_TABLE:
    copiarObjeto(pobj_orig, dynamic_cast<Tabela *>(obj_copia));
  break;
  case OBJ_TEXTBOX:
    copiarObjeto(pobj_orig, dynamic_cast<Textbox *>(obj_copia));
  break;
  case OBJ_OPCLASS:
    copiarObjeto(pobj_orig, dynamic_cast<OperatorClass *>(obj_copia));
  break;
  case OBJ_CONVERSION:
    copiarObjeto(pobj_orig, dynamic_cast<Conversion *>(obj_copia));
  break;
  case OBJ_CAST:
    copiarObjeto(pobj_orig, dynamic_cast<ConversaoTipo *>(obj_copia));
  break;
  case OBJ_DOMAIN:
    copiarObjeto(pobj_orig, dynamic_cast<Dominio *>(obj_copia));
  break;
  case OBJ_TABLESPACE:
    copiarObjeto(pobj_orig, dynamic_cast<EspacoTabela *>(obj_copia));
  break;
  case OBJ_SCHEMA:
    copiarObjeto(pobj_orig, dynamic_cast<Esquema *>(obj_copia));
  break;
  case OBJ_OPFAMILY:
    copiarObjeto(pobj_orig, dynamic_cast<FamiliaOperadores *>(obj_copia));
  break;
  case OBJ_FUNCTION:
    copiarObjeto(pobj_orig, dynamic_cast<Funcao *>(obj_copia));
  break;
  case OBJ_AGGREGATE:
    copiarObjeto(pobj_orig, dynamic_cast<FuncaoAgregacao *>(obj_copia));
  break;
  case OBJ_LANGUAGE:
    copiarObjeto(pobj_orig, dynamic_cast<Linguagem *>(obj_copia));
  break;
  case OBJ_OPERATOR:
    copiarObjeto(pobj_orig, dynamic_cast<Operador *>(obj_copia));
  break;
  case OBJ_ROLE:
    copiarObjeto(pobj_orig, dynamic_cast<Papel *>(obj_copia));
  break;
  case OBJ_SEQUENCE:
    copiarObjeto(pobj_orig, dynamic_cast<Sequencia *>(obj_copia));
  break;
  case OBJ_TYPE:
    copiarObjeto(pobj_orig, dynamic_cast<Tipo *>(obj_copia));
  break;
  case OBJ_VIEW:
    copiarObjeto(pobj_orig, dynamic_cast<Visao *>(obj_copia));
  break;
  default:
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }
}

unsigned ListaOperacoes::tam_maximo=500;

ListaOperacoes::ListaOperacoes(ModeloBD *modelo)
{
 //Dispara uma exceção caso o modelo passado não esteja alocado
 if(!modelo)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->modelo=modelo;
 idx_atual=0;
 enc_prox_oper=Operacao::SEM_ENCADEAMENTO;
 anular_enc=false;
}

ListaOperacoes::~ListaOperacoes(void)
{
 removerOperacoes();
}

unsigned ListaOperacoes::obterTamanhoAtual(void)
{
 return(operacoes.size());
}

unsigned ListaOperacoes::obterTamanhoMaximo(void)
{
 return(tam_maximo);
}

int ListaOperacoes::obterIndiceAtual(void)
{
 return(idx_atual);
}

void ListaOperacoes::iniciarEncadeamentoOperacoes(void)
{
 /* Caso o encadeamento esteja iniciado e o usuário tente inicializa-lo
    novamente, o encadeamento anterior será finalizado */
 if(enc_prox_oper!=Operacao::SEM_ENCADEAMENTO)
  finalizarEncadeamentoOperacoes();

 /* Marca o tipo de encadeamento da próxima operação com sendo
    primeira operação do encadeamento */
 enc_prox_oper=Operacao::ENC_INICIO;
}

void ListaOperacoes::finalizarEncadeamentoOperacoes(void)
{
 /* Caso o encadeamento não esteja anulado,
    indica que o próximo elemento da lista já não
    fará parte do encadeamento */
 if(!anular_enc)
  enc_prox_oper=Operacao::SEM_ENCADEAMENTO;
 else if(anular_enc)
  /* Caso o encadeamento esteja anulado,
    indica que o próximo elemento da lista
    continuará fazendo parte do encadeamento */
  enc_prox_oper=Operacao::ENC_MEIO;

 if(operacoes.size() > 0 && !anular_enc)
 {
  unsigned idx=operacoes.size()-1;

  /* Marca a ultima operação da lista como sendo a ultima do encadeamento
     caso esta esteja no meio do encadeamento */
  if(operacoes[idx]->tipo_enc==Operacao::ENC_MEIO)
   operacoes[idx]->tipo_enc=Operacao::ENC_FINAL;

  /* Caso a última operação esteja marcada como ENC_INICIO indica que
     o encademanto foi aberto porém somente uma operação está registrada
     neste encadeamento, sendo assim a operação é marcada como SEM_ENCADEAMENTO
     pois por se tratar de apenas uma operação não há necessidade de tratá-la como
     encadeamento */
  else if(operacoes[idx]->tipo_enc==Operacao::ENC_INICIO)
   operacoes[idx]->tipo_enc=Operacao::SEM_ENCADEAMENTO;
 }
}

void ListaOperacoes::anularEncadeamentoOperacoes(bool valor)
{
 anular_enc=valor;
}

bool ListaOperacoes::encadeamentoIniciado(void)
{
 return(enc_prox_oper==Operacao::ENC_INICIO ||
        enc_prox_oper==Operacao::ENC_MEIO);
}

bool ListaOperacoes::refazerHabilitado(void)
{
 /* Para que a operação de refazer possa ser executada
    o índice atual da lista de operações deve estar no máximo
    no penultimo elemento e a lista não pode estar vazia */
 return(!operacoes.empty() && idx_atual < static_cast<int>(operacoes.size()));
}

bool ListaOperacoes::desfazerHabilitado(void)
{
 /* Para que a operação de desfazer possa ser executada é
    suficiente que a lista de operações não esteja vazia */
 return(!operacoes.empty() && idx_atual > 0);
}

void ListaOperacoes::definirTamanhoMaximo(unsigned tam_max)
{
 /* A lista de operações não pode ter valor 0 em seu tamanho máximo,
    sendo assim é disparada uma exceção caso isso ocorra */
 if(tam_max==0)
  throw Exception(ERR_ASG_INV_MAX_SIZE_OP_LIST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define o tamanho máximo da lista
 tam_maximo=tam_max;
}

void ListaOperacoes::adicionarObjetoPool(BaseObject *objeto, unsigned tipo_op)
{
 ObjectType tipo_obj;

 /* Caso se tente inserir no pool um objeto não alocado
    é disparada uma exceção */
 if(!objeto)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 tipo_obj=objeto->getObjectType();

 /* Caso o objeto está prestes a ser removido ou modificado, será armazenado no pool
    uma cópia do mesmo e não o objeto em si */
 if(tipo_op==Operacao::OBJETO_MODIFICADO ||
    tipo_op==Operacao::OBJETO_MOVIMENTADO)
 {
  BaseObject *obj_copia=NULL;

  //Faz a cópia apenas de objetos descendetes das classes bases
  if(tipo_obj!=BASE_OBJECT && tipo_obj!=OBJ_DATABASE)
  {
   //Copia o objeto
   copiarObjeto(&obj_copia, objeto, tipo_obj);
  }
  else
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Caso a cópia falhe esta retorna um objeto nulo e a adição de objeto
     será cancelada com o disparo da exceção */
  if(!obj_copia)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   //Insere a cópia do objeto no pool
   pool_objetos.push_back(obj_copia);
   objeto=obj_copia;
 }
 else
  //Insere o próprio objeto no pool
  pool_objetos.push_back(objeto);
}

void ListaOperacoes::removerOperacoes(void)
{
 BaseObject *objeto=NULL;
 TableObject *obj_tab=NULL;
 Tabela *tab=NULL;

 //Limpando a lista de operações
 while(!operacoes.empty())
 {
  delete(operacoes.back());
  operacoes.pop_back();
 }

 //Limpando o pool de objetos
 while(!pool_objetos.empty())
  removerObjetoPool(0);

 //Removendo os objetos que não foram excluídos do pool
 while(!objs_nao_excluidos.empty())
 {
  objeto=objs_nao_excluidos.back();
  obj_tab=dynamic_cast<TableObject *>(objeto);

  if(!obj_tab && modelo->obterIndiceObjeto(objeto) < 0)
   delete(objeto);
  else if(obj_tab)
  {
   tab=dynamic_cast<Tabela *>(obj_tab->getParentTable());
   if(!tab || (tab && tab->obterIndiceObjeto(obj_tab)) < 0)
    delete(obj_tab);
  }

  objs_nao_excluidos.pop_back();
 }

 idx_atual=0;
}

void ListaOperacoes::validarOperacoes(void)
{
 vector<Operacao *>::iterator itr, itr_end;
 Operacao *operacao=NULL;

 itr=operacoes.begin();
 itr_end=operacoes.end();
 while(itr!=itr_end)
 {
  //Obtém a referência ao objeto
  operacao=(*itr);
  if(!objetoNoPool((*itr)->obj_pool))
  {
   //Remove o elemento do pool
   operacoes.erase(itr);
   //Desaloca o objeto
   delete(operacao);
   itr=operacoes.begin();
   itr_end=operacoes.end();
  }
  else itr++;
 }
}

bool ListaOperacoes::objetoNoPool(BaseObject *objeto)
{
 bool enc=false;
 vector<BaseObject *>::iterator itr, itr_end;

 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=pool_objetos.begin();
 itr_end=pool_objetos.end();
 while(!enc && itr!=itr_end)
 {
  enc=*(itr)==objeto;
  itr++;
 }

 return(enc);
}

void ListaOperacoes::removerObjetoPool(unsigned idx_obj)
{
 BaseObject *objeto=NULL;
 vector<BaseObject *>::iterator itr;

 //Caso o índice do objeto a ser excluído seja inválido é disparada uma exceção
 if(idx_obj >= pool_objetos.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o elemento a ser removido
 itr=pool_objetos.begin() + idx_obj;
 objeto=(*itr);

 //Remove o elemento do pool
 pool_objetos.erase(itr);

 /* Armazena o objeto que antes se encontrava no pool no vetor 'objs_nao_excluidos' para que
    o mesmo seja excluído no destrutor da lista. OBS.: O objeto não é deletado de imediato
    pois no modelo / tabela / lista de operações o mesmo ainda pode estar sendo referenciado
    ou passar a ser referenciado quando uma operação da lista for executada. */
  objs_nao_excluidos.push_back(objeto);
}


void ListaOperacoes::adicionarObjeto(BaseObject *objeto, unsigned tipo_op, int idx_objeto,  BaseObject *objeto_pai)
{
 ObjectType tipo_obj;
 Operacao *operacao=NULL;
 Tabela *tabela_pai=NULL;
 Relacionamento *relac_pai=NULL;
 int idx_obj=-1;

 try
 {
  //Caso se tente adicionar uma operação com objeto não alocado, dispara uma exceção
  if(!objeto)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  tipo_obj=objeto->getObjectType();
  if((tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
      tipo_obj==OBJ_INDEX || tipo_obj==OBJ_TRIGGER ||
      tipo_obj==OBJ_RULE) && !objeto_pai)
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  else if(objeto_pai &&
     (((tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT) &&
      (objeto_pai->getObjectType()!=OBJ_RELATIONSHIP && objeto_pai->getObjectType()!=OBJ_TABLE)) ||

      ((tipo_obj==OBJ_INDEX || tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_RULE) &&
        objeto_pai->getObjectType()!=OBJ_TABLE)))
   throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Caso a lista de operações esteja cheia, faz a limpeza automática antes de inserir uma nova operação
  if(idx_atual == static_cast<int>(tam_maximo-1))
  {
   removerOperacoes();
  }

  /* Caso se esteja adicionando uma operação e o índice atual não esteja apontando
     para o final da lista (operação de refazer disponível / usuário executou operações
     de desfazer), todos do índice atual até o fim da lista de operaçãoes serão excluídos
     bem como os objetos no pool que estavam ligados  s operações excluídas */

  if(idx_atual>=0 && static_cast<unsigned>(idx_atual)!=operacoes.size())
  {
   int i;

   //Obtém o índice da última operação
   i=operacoes.size()-1;

   /* Remove as operações enquanto o indice da operação a ser removida (i)
      não seja de uma operação anterior a  operação atual */
   while(i >= idx_atual)
   {
    //Remove a operação pelo seu indice
    removerObjetoPool(i);
    //Passa para operação anterior
    i--;
   }

   /* Valida as operaçãos e os objetos no pool,
      fazendo a sincronia entre as operações
      restantes e os objetos no pool, excluindo
      aquelas operações as quais referenciam objetos
      inexistentes no pool */
   validarOperacoes();
  }

  //Cria a operação a ser armazenada na lista
  operacao=new Operacao;
  operacao->tipo_op=tipo_op;
  operacao->tipo_enc=enc_prox_oper;
  operacao->obj_gerador=objeto;

  //Adiciona o objeto no pool
  adicionarObjetoPool(objeto, tipo_op);

  //Atribui   operação o objeto inserido no pool
  operacao->obj_pool=pool_objetos.back();

  /* Caso o tipo de encadeamento atual da operação seja de início de
     encademaneto, configura o tipo para a próxima operação a ser
     inserida como sendo uma operação no meio do encadeamento. */
   if(enc_prox_oper==Operacao::ENC_INICIO)
    enc_prox_oper=Operacao::ENC_MEIO;

  /* Executando operações específicada de acorodo com o tipo de objeto.
     Caso o objeto possua um objeto pai, o mesmo precisa ser descoberto
     e além disso é necessário descobrir e armazenar o índice do objeto
     na lista contida em seu objeto pai */
  if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
     tipo_obj==OBJ_INDEX || tipo_obj==OBJ_TRIGGER ||
     tipo_obj==OBJ_RULE)
  {
   TableObject *obj_tab=NULL;
   obj_tab=dynamic_cast<TableObject *>(objeto);

   if(objeto_pai->getObjectType()==OBJ_TABLE)
    tabela_pai=dynamic_cast<Tabela *>(objeto_pai);
   else
    relac_pai=dynamic_cast<Relacionamento *>(objeto_pai);

   /* Caso específico para colunas: em operações de remoção do objeto
      as permissões daquele objeto precisam ser removidas. */
   if(tipo_obj==OBJ_COLUMN && tipo_op==Operacao::OBJETO_REMOVIDO)
    modelo->removerPermissoes(obj_tab);
   else if(((tipo_obj==OBJ_TRIGGER && dynamic_cast<Gatilho *>(obj_tab)->referenciaColunaIncRelacao()) ||
            (tipo_obj==OBJ_INDEX && dynamic_cast<Indice *>(obj_tab)->referenciaColunaIncRelacao()) ||
            (tipo_obj==OBJ_CONSTRAINT && dynamic_cast<Restricao *>(obj_tab)->referenciaColunaIncRelacao())))
   {
    if(tipo_op==Operacao::OBJETO_REMOVIDO)
     obj_tab->setParentTable(tabela_pai);

    operacao->def_xml=modelo->validarDefinicaoObjeto(obj_tab, SchemaParser::XML_DEFINITION);
   }

   operacao->obj_pai=objeto_pai;

   /* Caso haja um relacionamento pai será obtido o índice do objeto.
      Apenas colunas e restrições são manipuladas caso o pai seja
      um relacionamento */
   if(relac_pai && (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT))
   {
    //Caso um índice específico para o objeto não foi especificado
    if(idx_objeto < 0)
     //Armazena na operação o índice do objeto em seu pai
     idx_obj=relac_pai->obterIndiceObjeto(obj_tab);
    else
     //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
     idx_obj=idx_objeto;
   }
   //Caso haja uma tabela pai será obtido o índice do objeto
   else if(tabela_pai)
   {
    //Caso um índice específico para o objeto não foi especificado
    if(idx_objeto < 0)
     //Armazena na operação o índice do objeto em seu pai
     idx_obj=tabela_pai->obterIndiceObjeto(objeto->getName(false), tipo_obj);
    else
     //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
     idx_obj=idx_objeto;
   }
   /* Caso nem tabela pai nem relacionametno pai estejam alocados
      retorna um erro ao usuário e desaloca a operção criada */
   else
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  else
  {
   //Caso um índice específico para o objeto não foi especificado
   if(idx_objeto < 0)
    //Armazena na operação o índice do objeto no modelo
    idx_obj=modelo->obterIndiceObjeto(objeto);
   else
    //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
    idx_obj=idx_objeto;
  }

  operacao->idx_obj=idx_obj;

  //Insere a operação na lista de operações
  operacoes.push_back(operacao);

  //O índice atual de operação será o próprio tamanho da lista
  idx_atual=operacoes.size();
 }
 catch(Exception &e)
 {
  if(operacao)
  {
   removerObjetoPool(pool_objetos.size()-1);
   delete(operacao);
  }
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void ListaOperacoes::obterDadosOperacao(unsigned idx_oper, unsigned &tipo_oper, QString &nome_obj, ObjectType &tipo_obj)
{
 Operacao *operacao=NULL;

 if(idx_oper >= operacoes.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 operacao=operacoes[idx_oper];
 tipo_oper=operacao->tipo_op;

 tipo_obj=operacao->obj_pool->getObjectType();

 if(tipo_obj==OBJ_CAST)
  nome_obj=operacao->obj_pool->getName();
 else
  nome_obj=operacao->obj_pool->getName(true);

 //Formata o nome especificamente para objetos da tabela incluindo o nome da tabela pai ao nome do objeto
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT || tipo_obj==OBJ_RULE ||
    tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_INDEX)
 {
  nome_obj=operacao->obj_pai->getName(true) + "." + nome_obj;
 }
}

unsigned ListaOperacoes::obterTamanhoEncadeamento(void)
{
 int i=idx_atual-1;
 unsigned tam=0;

 if(i < 0 && !operacoes.empty())
  i=0;

 //Verifica se a operação atual é de encadeamento
 if(!operacoes.empty() &&
    operacoes[i]->tipo_enc!=Operacao::SEM_ENCADEAMENTO)
 {
  unsigned tipo_enc=0;
  int inc=0;

  //Caso seja um encadeamento final varre a lista de tras para frente
  if(operacoes[i]->tipo_enc==Operacao::ENC_FINAL)
  {
   tipo_enc=Operacao::ENC_INICIO;
   inc=-1;
  }
  //Caso seja um encadeamento inicial varre a lista de frente para tras
  else if(operacoes[i]->tipo_enc==Operacao::ENC_INICIO)
  {
   tipo_enc=Operacao::ENC_FINAL;
   inc=1;
  }

  //Varre a lista na sequencia definida contando as operações encadeadas
  while(i>=0 && i < static_cast<int>(operacoes.size()) && operacoes[i]->tipo_enc!=tipo_enc)
  {
   i+=inc;
   tam++;
  }
 }

 return(tam);
}

void ListaOperacoes::desfazerOperacao(void)
{
 if(desfazerHabilitado())
 {
  Operacao *operacao=NULL;
  bool enc_ativo=false;
  Exception erro;
  unsigned tam_enc=0, pos=0;

  if(!this->signalsBlocked())
   tam_enc=obterTamanhoEncadeamento();

  do
  {
   /* Na operação de desfazer, é necessário obter o objeto
      no índice atual decrementado em 1 pois o índice atual sempre
      aponta para o elemento posterior ao último elemento */
   operacao=operacoes[idx_atual-1];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!anular_enc && !enc_ativo &&
       operacao->tipo_enc!=Operacao::SEM_ENCADEAMENTO)
     enc_ativo=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(enc_ativo &&
           (operacao->tipo_enc==Operacao::ENC_FINAL ||
            operacao->tipo_enc==Operacao::SEM_ENCADEAMENTO))
    break;

   try
   {
    if(!this->signalsBlocked() && tam_enc > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operacaoExecutada((pos/static_cast<float>(tam_enc))*100,
                               trUtf8("Undoing operation on object: %1 (%2)")
                                      .arg(operacao->obj_pool->getName())
                                      .arg(operacao->obj_pool->getTypeName()),
                                       operacao->obj_pool->getObjectType());
    }

    //Executa a operação de desfazer
    executarOperacao(operacao, false);
   }
   catch(Exception &e)
   {
    erro=e;
   }

   idx_atual--;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de desfazer esteja habilidata */
  while(!anular_enc && desfazerHabilitado() && operacao->tipo_enc!=Operacao::SEM_ENCADEAMENTO);

  if(erro.getErrorType()!=ERR_CUSTOM)
   throw Exception(erro.getErrorMessage(), erro.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void ListaOperacoes::refazerOperacao(void)
{
 if(refazerHabilitado())
 {
  Operacao *operacao=NULL;
  bool enc_ativo=false;
  Exception erro;
  unsigned tam_enc=0, pos=0;

  //Calcula o tamanho do encademanto de operações atual
  if(!this->signalsBlocked())
   tam_enc=obterTamanhoEncadeamento();

  do
  {
   //Obtém elemento atual da lista de operações
   operacao=operacoes[idx_atual];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!anular_enc && !enc_ativo &&
      operacao->tipo_enc!=Operacao::SEM_ENCADEAMENTO)
    enc_ativo=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(enc_ativo &&
           (operacao->tipo_enc==Operacao::ENC_INICIO ||
            operacao->tipo_enc==Operacao::SEM_ENCADEAMENTO))
    break;

   try
   {
    if(tam_enc > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operacaoExecutada((pos/static_cast<float>(tam_enc))*100,
                               trUtf8("Redoing operation on object:: %1 (%2)")
                                      .arg(operacao->obj_pool->getName())
                                      .arg(operacao->obj_pool->getTypeName()),
                                       operacao->obj_pool->getObjectType());
    }

    //Executa a operação de refazer (segundo parametro = true)
    executarOperacao(operacao, true);
   }
   catch(Exception &e)
   {
    erro=e;
   }
   idx_atual++;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de refazer esteja habilidata */
  while(!anular_enc && refazerHabilitado()  && operacao->tipo_enc!=Operacao::SEM_ENCADEAMENTO);

  if(erro.getErrorType()!=ERR_CUSTOM)
   throw Exception(erro.getErrorMessage(), erro.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void ListaOperacoes::executarOperacao(Operacao *oper, bool refazer)
{
  if(oper)
  {
   BaseObject *obj_orig=NULL, *obj_bkp=NULL, *objeto=NULL, *obj_aux=NULL;
   ObjectType tipo;
   Tabela *tabela_pai=NULL;
   Relacionamento *relac_pai=NULL;

   objeto=oper->obj_pool;
   tipo=objeto->getObjectType();

   /* Convertendo o objeto pai, caso exista, para a classe correta conforme
      o tipo do objeto pai. Caso seja OBJETO_TABELA, o ponteiro
      'tabela_pai' receberá a referência da tabela e
      será usado como referencial nas operações abaixo.
      Caso o objeto pai seja um relacionamento, o ponteiro
      'relac_pai' receberá a referência ao relacionamento */
   if(oper->obj_pai)
   {
    if(oper->obj_pai->getObjectType()==OBJ_TABLE)
     tabela_pai=dynamic_cast<Tabela *>(oper->obj_pai);
    else
     relac_pai=dynamic_cast<Relacionamento *>(oper->obj_pai);
   }

   /* Caso a definição xml do objeto esteja definida
      indica que o mesmo referencia uma coluna incluída por relacionamento */
   if(!oper->def_xml.isEmpty() &&
      ((oper->tipo_op==Operacao::OBJETO_REMOVIDO && !refazer) ||
       (oper->tipo_op==Operacao::OBJETO_CRIADO && refazer) ||
       (oper->tipo_op==Operacao::OBJETO_MODIFICADO ||
        oper->tipo_op==Operacao::OBJETO_MOVIMENTADO)))
   {
    //Reinicia o parser e carrega o buffer do mesmo com o código xml da operação
    XMLParser::restartParser();
    XMLParser::loadXMLBuffer(oper->def_xml);

    //Cria uma cópia do objeto conforme o tipo do mesmo
    if(tipo==OBJ_TRIGGER)
     obj_aux=modelo->criarGatilho(tabela_pai);
    else if(tipo==OBJ_INDEX)
     obj_aux=modelo->criarIndice(tabela_pai);
    else if(tipo==OBJ_CONSTRAINT)
     obj_aux=modelo->criarRestricao(oper->obj_pai);
    else if(tipo==OBJ_SEQUENCE)
     obj_aux=modelo->criarSequencia();
   }

   /* Caso a operação seja de objeto modificado, a cópia do objeto
      armazenada no pool (no caso o parâmetro 'objeto') será restaurada */
   if(oper->tipo_op==Operacao::OBJETO_MODIFICADO ||
      oper->tipo_op==Operacao::OBJETO_MOVIMENTADO)
   {
    if(tipo==OBJ_RELATIONSHIP)
    {
     /* Devido a complexidade da classe Relacionamento e a forte ligação entre todos os
        relacinamentos do modelo, é necessário armazenar o XML dos objetos especiais e
        desconectar TODOS os relacionamentos, executar a modificação no
        relacionamento e logo após revalidar todos os demais */
     modelo->obterXMLObjetosEspeciais();
     modelo->desconectarRelacionamentos();
    }

    //Obtém a objeto atual da tabela/relacionamento pai no índice especificado
    if(tabela_pai)
     obj_orig=dynamic_cast<TableObject *>(tabela_pai->obterObjeto(oper->idx_obj, tipo));
    else if(relac_pai)
     obj_orig=dynamic_cast<TableObject *>(relac_pai->obterObjeto(oper->idx_obj, tipo));
    else
     obj_orig=modelo->obterObjeto(oper->idx_obj, tipo);

    if(obj_aux)
     oper->def_xml=modelo->validarDefinicaoObjeto(obj_orig, SchemaParser::SQL_DEFINITION);

     /* A objeto original (obtido da tabela, relacionamento pai ou modelo) tera seus valores anteriores
        restaurados com a cópia existente no pool. Após a restauração o objeto
        no pool terá como atributos os mesmo do objeto antes de ser restaurado,
        para possibilitar a operação de refazer */
    copiarObjeto(reinterpret_cast<BaseObject **>(&obj_bkp), obj_orig, tipo);
    copiarObjeto(reinterpret_cast<BaseObject **>(&obj_orig), objeto, tipo);
    copiarObjeto(reinterpret_cast<BaseObject **>(&objeto), obj_bkp, tipo);
    objeto=obj_orig;

    if(obj_aux)
     copiarObjeto(reinterpret_cast<BaseObject **>(&objeto), obj_aux, tipo);
   }
   /* Caso a operação seja de objeto removido e não seja uma operação de refazer, ou
      se o objeto foi criado anteriormente e se deseja refazer a operação.
      A objeto existente no pool será inserida na tabela ou relacionamento pai e no seu índice original */
   else if((oper->tipo_op==Operacao::OBJETO_REMOVIDO && !refazer) ||
           (oper->tipo_op==Operacao::OBJETO_CRIADO && refazer))
   {
    if(obj_aux)
     copiarObjeto(reinterpret_cast<BaseObject **>(&objeto), obj_aux, tipo);

    if(tabela_pai)
     tabela_pai->adicionarObjeto(dynamic_cast<TableObject *>(objeto), oper->idx_obj);
    else if(relac_pai)
     relac_pai->adicionarObjeto(dynamic_cast<TableObject *>(objeto), oper->idx_obj);
    else
     if(dynamic_cast<Tabela *>(objeto))
      dynamic_cast<Tabela *>(objeto)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
     modelo->adicionarObjeto(objeto, oper->idx_obj);
   }
   /* Caso a operação seja de objeto criado anteriormente ou caso o objeto
     foi removido e se deseja refazer a operação o mesmo será
     excluído da tabela ou relacionamento pai */
   else if((oper->tipo_op==Operacao::OBJETO_CRIADO && !refazer) ||
           (oper->tipo_op==Operacao::OBJETO_REMOVIDO && refazer))
   {
    if(tabela_pai)
     tabela_pai->removerObjeto(oper->idx_obj,tipo);
    else if(relac_pai)
     relac_pai->removerObjeto(oper->idx_obj,tipo);
    else
     modelo->removerObjeto(objeto, oper->idx_obj);
   }

   /* Caso a tabela pai ou relacionamento pai esteja setados
      indica que a operação foi executada em um objeto filho desses objetos */
   if(tabela_pai || relac_pai)
   {
    if(tabela_pai)
     //Marca que a tabela pai foi modificada forçando assim seu redimensionamento
     tabela_pai->setModefied(true);
    else
     //Marca que o relacionamento pai foi modificado forçando assim seu redimensionamento
    relac_pai->setModefied(true);

    if(tabela_pai &&
      (objeto->getObjectType()==OBJ_COLUMN ||
       objeto->getObjectType()==OBJ_CONSTRAINT))
     modelo->validarRelacObjetoTabela(dynamic_cast<TableObject *>(objeto), tabela_pai);
    else if(relac_pai)
     modelo->validarRelacionamentos();
   }
   /* Caso o objeto em questão seja um grafico o mesmo tem seus flags
      de modificado marcado em true e de selecionado em falso, para
      forçar o redesenho do mesmo no momento de sua restauração */
   else if(tipo==OBJ_TABLE || tipo==OBJ_VIEW ||
      tipo==BASE_RELATIONSHIP || tipo==OBJ_RELATIONSHIP ||
      tipo==OBJ_TEXTBOX)
   {
    BaseGraphicObject *obj_grafico=dynamic_cast<BaseGraphicObject *>(objeto);

    if(oper->tipo_op==Operacao::OBJETO_MODIFICADO ||
       oper->tipo_op==Operacao::OBJETO_MOVIMENTADO)
     obj_grafico->setModefied(true);

    //Caso seja uma visão atualiza os relacionamentos entre as tabelas e a visão
    if(tipo==OBJ_VIEW && oper->tipo_op==Operacao::OBJETO_MODIFICADO)
     modelo->atualizarRelTabelaVisao(dynamic_cast<Visao *>(obj_grafico));
    else if((tipo==OBJ_RELATIONSHIP ||
             (tipo==OBJ_TABLE && modelo->obterRelacionamento(dynamic_cast<BaseTable *>(objeto), NULL))) &&
            oper->tipo_op==Operacao::OBJETO_MODIFICADO)
     modelo->validarRelacionamentos();
   }
  }
}

void ListaOperacoes::removerUltimaOperacao(void)
{
 if(!operacoes.empty())
 {
  Operacao *oper=NULL;
  bool fim=false;
  vector<Operacao *>::reverse_iterator itr;
  unsigned idx_obj=operacoes.size()-1;

  //Obtém a última operação da lista, utilizando um iterador reverso
  itr=operacoes.rbegin();

  while(!fim)
  {
   //Obtém a operação armazenada no iterador para sua devida remoção
   oper=(*itr);
   //Remove o objeto relacionado   operação e que se encontrar no pool
   removerObjetoPool(idx_obj);
   /* Condição de parada de remoção da operação:
      1) A operação não é encadeada com outras, ou
      2) Caso a ultima operação seja o final de um encadeamento
         todos as operações da cadeia serão removidas incluíndo
         a primeira operação do encadeamento, quando esta última
         é removida a iteração é interrompida.
   */
   fim=(anular_enc ||
        (!anular_enc &&
         (oper->tipo_enc==Operacao::SEM_ENCADEAMENTO ||
          oper->tipo_enc==Operacao::ENC_INICIO)));

   //Passa para a operação anterior   atual
   itr++; idx_obj--;
  }

 /* Se o cabeça do encademanto for removido (ENC_INICIO)
    marca que o próximo elemento da lista será o novo
    início do encadeamnto */
 if(oper && oper->tipo_enc==Operacao::ENC_INICIO)
  enc_prox_oper=Operacao::ENC_INICIO;

  /* Executa a validação das operações removendo aquelas
     que possivelmente referenciam objetos inexistentes
     no pool de objetos */
  validarOperacoes();

  /* Atualiza o indice atual da lista, caso este aponte
     para um item além do final da lista de operações
     o mesmo passará a apontar para o ultimo elemento */
  if(static_cast<unsigned>(idx_atual) > operacoes.size())
   idx_atual=operacoes.size();
 }
}

void ListaOperacoes::atualizarIndiceObjeto(BaseObject *objeto, unsigned idx_novo)
{
 vector<Operacao *>::iterator itr, itr_end;
 Operacao *oper=NULL;

 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=operacoes.begin();
 itr_end=operacoes.end();

 while(itr!=itr_end)
 {
  oper=(*itr);
  if(oper->obj_gerador==objeto)
   oper->idx_obj=idx_novo;
  itr++;
 }
}

