#include "operationlist.h"

template <class Classe>
void copyObject(BaseObject **pobj_orig, Classe *obj_copia)
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

void copyObject(BaseObject **pobj_orig, BaseObject *obj_copia, ObjectType tipo)
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
    copyObject(pobj_orig, dynamic_cast<Column *>(obj_copia));
  break;
  case OBJ_CONSTRAINT:
    copyObject(pobj_orig, dynamic_cast<Restricao *>(obj_copia));
  break;
  case OBJ_TRIGGER:
    copyObject(pobj_orig, dynamic_cast<Gatilho *>(obj_copia));
  break;
  case OBJ_RULE:
    copyObject(pobj_orig, dynamic_cast<Regra *>(obj_copia));
  break;
  case OBJ_INDEX:
    copyObject(pobj_orig, dynamic_cast<Index *>(obj_copia));
  break;
  case OBJ_TABLE:
    copyObject(pobj_orig, dynamic_cast<Tabela *>(obj_copia));
  break;
  case OBJ_TEXTBOX:
    copyObject(pobj_orig, dynamic_cast<Textbox *>(obj_copia));
  break;
  case OBJ_OPCLASS:
    copyObject(pobj_orig, dynamic_cast<OperatorClass *>(obj_copia));
  break;
  case OBJ_CONVERSION:
    copyObject(pobj_orig, dynamic_cast<Conversion *>(obj_copia));
  break;
  case OBJ_CAST:
    copyObject(pobj_orig, dynamic_cast<Cast *>(obj_copia));
  break;
  case OBJ_DOMAIN:
    copyObject(pobj_orig, dynamic_cast<Domain *>(obj_copia));
  break;
  case OBJ_TABLESPACE:
    copyObject(pobj_orig, dynamic_cast<Tablespace *>(obj_copia));
  break;
  case OBJ_SCHEMA:
    copyObject(pobj_orig, dynamic_cast<Schema *>(obj_copia));
  break;
  case OBJ_OPFAMILY:
    copyObject(pobj_orig, dynamic_cast<OperatorFamily *>(obj_copia));
  break;
  case OBJ_FUNCTION:
    copyObject(pobj_orig, dynamic_cast<Function *>(obj_copia));
  break;
  case OBJ_AGGREGATE:
    copyObject(pobj_orig, dynamic_cast<Aggregate *>(obj_copia));
  break;
  case OBJ_LANGUAGE:
    copyObject(pobj_orig, dynamic_cast<Language *>(obj_copia));
  break;
  case OBJ_OPERATOR:
    copyObject(pobj_orig, dynamic_cast<Operador *>(obj_copia));
  break;
  case OBJ_ROLE:
    copyObject(pobj_orig, dynamic_cast<Papel *>(obj_copia));
  break;
  case OBJ_SEQUENCE:
    copyObject(pobj_orig, dynamic_cast<Sequencia *>(obj_copia));
  break;
  case OBJ_TYPE:
    copyObject(pobj_orig, dynamic_cast<Tipo *>(obj_copia));
  break;
  case OBJ_VIEW:
    copyObject(pobj_orig, dynamic_cast<Visao *>(obj_copia));
  break;
  default:
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }
}

unsigned OperationList::max_size=500;

OperationList::OperationList(ModeloBD *modelo)
{
 //Dispara uma exceção caso o modelo passado não esteja alocado
 if(!modelo)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->model=modelo;
 current_index=0;
 next_op_chain=Operation::NO_CHAIN;
 ignore_chain=false;
}

OperationList::~OperationList(void)
{
 removeOperations();
}

unsigned OperationList::getCurrentSize(void)
{
 return(operations.size());
}

unsigned OperationList::getMaximumSize(void)
{
 return(max_size);
}

int OperationList::getCurrentIndex(void)
{
 return(current_index);
}

void OperationList::startOperationChain(void)
{
 /* Caso o encadeamento esteja iniciado e o usuário tente inicializa-lo
    novamente, o encadeamento anterior será finalizado */
 if(next_op_chain!=Operation::NO_CHAIN)
  finishOperationChain();

 /* Marca o tipo de encadeamento da próxima operação com sendo
    primeira operação do encadeamento */
 next_op_chain=Operation::CHAIN_START;
}

void OperationList::finishOperationChain(void)
{
 /* Caso o encadeamento não esteja anulado,
    indica que o próximo elemento da lista já não
    fará parte do encadeamento */
 if(!ignore_chain)
  next_op_chain=Operation::NO_CHAIN;
 else if(ignore_chain)
  /* Caso o encadeamento esteja anulado,
    indica que o próximo elemento da lista
    continuará fazendo parte do encadeamento */
  next_op_chain=Operation::CHAIN_MIDDLE;

 if(operations.size() > 0 && !ignore_chain)
 {
  unsigned idx=operations.size()-1;

  /* Marca a ultima operação da lista como sendo a ultima do encadeamento
     caso esta esteja no meio do encadeamento */
  if(operations[idx]->chain_type==Operation::CHAIN_MIDDLE)
   operations[idx]->chain_type=Operation::CHAIN_END;

  /* Caso a última operação esteja marcada como ENC_INICIO indica que
     o encademanto foi aberto porém somente uma operação está registrada
     neste encadeamento, sendo assim a operação é marcada como SEM_ENCADEAMENTO
     pois por se tratar de apenas uma operação não há necessidade de tratá-la como
     encadeamento */
  else if(operations[idx]->chain_type==Operation::CHAIN_START)
   operations[idx]->chain_type=Operation::NO_CHAIN;
 }
}

void OperationList::ignoreOperationChain(bool valor)
{
 ignore_chain=valor;
}

bool OperationList::isOperationChainStarted(void)
{
 return(next_op_chain==Operation::CHAIN_START ||
        next_op_chain==Operation::CHAIN_MIDDLE);
}

bool OperationList::isRedoAvailable(void)
{
 /* Para que a operação de refazer possa ser executada
    o índice atual da lista de operações deve estar no máximo
    no penultimo elemento e a lista não pode estar vazia */
 return(!operations.empty() && current_index < static_cast<int>(operations.size()));
}

bool OperationList::isUndoAvailable(void)
{
 /* Para que a operação de desfazer possa ser executada é
    suficiente que a lista de operações não esteja vazia */
 return(!operations.empty() && current_index > 0);
}

void OperationList::setMaximumSize(unsigned tam_max)
{
 /* A lista de operações não pode ter valor 0 em seu tamanho máximo,
    sendo assim é disparada uma exceção caso isso ocorra */
 if(tam_max==0)
  throw Exception(ERR_ASG_INV_MAX_SIZE_OP_LIST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define o tamanho máximo da lista
 max_size=tam_max;
}

void OperationList::addToPool(BaseObject *objeto, unsigned tipo_op)
{
 ObjectType tipo_obj;

 /* Caso se tente inserir no pool um objeto não alocado
    é disparada uma exceção */
 if(!objeto)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 tipo_obj=objeto->getObjectType();

 /* Caso o objeto está prestes a ser removido ou modificado, será armazenado no pool
    uma cópia do mesmo e não o objeto em si */
 if(tipo_op==Operation::OBJECT_MODIFIED ||
    tipo_op==Operation::OBJECT_MOVED)
 {
  BaseObject *obj_copia=NULL;

  //Faz a cópia apenas de objetos descendetes das classes bases
  if(tipo_obj!=BASE_OBJECT && tipo_obj!=OBJ_DATABASE)
  {
   //Copia o objeto
   copyObject(&obj_copia, objeto, tipo_obj);
  }
  else
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Caso a cópia falhe esta retorna um objeto nulo e a adição de objeto
     será cancelada com o disparo da exceção */
  if(!obj_copia)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   //Insere a cópia do objeto no pool
   object_pool.push_back(obj_copia);
   objeto=obj_copia;
 }
 else
  //Insere o próprio objeto no pool
  object_pool.push_back(objeto);
}

void OperationList::removeOperations(void)
{
 BaseObject *objeto=NULL;
 TableObject *obj_tab=NULL;
 Tabela *tab=NULL;

 //Limpando a lista de operações
 while(!operations.empty())
 {
  delete(operations.back());
  operations.pop_back();
 }

 //Limpando o pool de objetos
 while(!object_pool.empty())
  removeFromPool(0);

 //Removendo os objetos que não foram excluídos do pool
 while(!not_removed_objs.empty())
 {
  objeto=not_removed_objs.back();
  obj_tab=dynamic_cast<TableObject *>(objeto);

  if(!obj_tab && model->obterIndiceObjeto(objeto) < 0)
   delete(objeto);
  else if(obj_tab)
  {
   tab=dynamic_cast<Tabela *>(obj_tab->getParentTable());
   if(!tab || (tab && tab->obterIndiceObjeto(obj_tab)) < 0)
    delete(obj_tab);
  }

  not_removed_objs.pop_back();
 }

 current_index=0;
}

void OperationList::validateOperations(void)
{
 vector<Operation *>::iterator itr, itr_end;
 Operation *operacao=NULL;

 itr=operations.begin();
 itr_end=operations.end();
 while(itr!=itr_end)
 {
  //Obtém a referência ao objeto
  operacao=(*itr);
  if(!isObjectOnPool((*itr)->pool_obj))
  {
   //Remove o elemento do pool
   operations.erase(itr);
   //Desaloca o objeto
   delete(operacao);
   itr=operations.begin();
   itr_end=operations.end();
  }
  else itr++;
 }
}

bool OperationList::isObjectOnPool(BaseObject *objeto)
{
 bool enc=false;
 vector<BaseObject *>::iterator itr, itr_end;

 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=object_pool.begin();
 itr_end=object_pool.end();
 while(!enc && itr!=itr_end)
 {
  enc=*(itr)==objeto;
  itr++;
 }

 return(enc);
}

void OperationList::removeFromPool(unsigned idx_obj)
{
 BaseObject *objeto=NULL;
 vector<BaseObject *>::iterator itr;

 //Caso o índice do objeto a ser excluído seja inválido é disparada uma exceção
 if(idx_obj >= object_pool.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o elemento a ser removido
 itr=object_pool.begin() + idx_obj;
 objeto=(*itr);

 //Remove o elemento do pool
 object_pool.erase(itr);

 /* Armazena o objeto que antes se encontrava no pool no vetor 'objs_nao_excluidos' para que
    o mesmo seja excluído no destrutor da lista. OBS.: O objeto não é deletado de imediato
    pois no modelo / tabela / lista de operações o mesmo ainda pode estar sendo referenciado
    ou passar a ser referenciado quando uma operação da lista for executada. */
  not_removed_objs.push_back(objeto);
}


void OperationList::registerObject(BaseObject *objeto, unsigned tipo_op, int idx_objeto,  BaseObject *objeto_pai)
{
 ObjectType tipo_obj;
 Operation *operacao=NULL;
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
  if(current_index == static_cast<int>(max_size-1))
  {
   removeOperations();
  }

  /* Caso se esteja adicionando uma operação e o índice atual não esteja apontando
     para o final da lista (operação de refazer disponível / usuário executou operações
     de desfazer), todos do índice atual até o fim da lista de operaçãoes serão excluídos
     bem como os objetos no pool que estavam ligados  s operações excluídas */

  if(current_index>=0 && static_cast<unsigned>(current_index)!=operations.size())
  {
   int i;

   //Obtém o índice da última operação
   i=operations.size()-1;

   /* Remove as operações enquanto o indice da operação a ser removida (i)
      não seja de uma operação anterior a  operação atual */
   while(i >= current_index)
   {
    //Remove a operação pelo seu indice
    removeFromPool(i);
    //Passa para operação anterior
    i--;
   }

   /* Valida as operaçãos e os objetos no pool,
      fazendo a sincronia entre as operações
      restantes e os objetos no pool, excluindo
      aquelas operações as quais referenciam objetos
      inexistentes no pool */
   validateOperations();
  }

  //Cria a operação a ser armazenada na lista
  operacao=new Operation;
  operacao->op_type=tipo_op;
  operacao->chain_type=next_op_chain;
  operacao->generator_obj=objeto;

  //Adiciona o objeto no pool
  addToPool(objeto, tipo_op);

  //Atribui   operação o objeto inserido no pool
  operacao->pool_obj=object_pool.back();

  /* Caso o tipo de encadeamento atual da operação seja de início de
     encademaneto, configura o tipo para a próxima operação a ser
     inserida como sendo uma operação no meio do encadeamento. */
   if(next_op_chain==Operation::CHAIN_START)
    next_op_chain=Operation::CHAIN_MIDDLE;

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
   if(tipo_obj==OBJ_COLUMN && tipo_op==Operation::OBJECT_REMOVED)
    model->removerPermissoes(obj_tab);
   else if(((tipo_obj==OBJ_TRIGGER && dynamic_cast<Gatilho *>(obj_tab)->isReferRelationshipColumn()) ||
            (tipo_obj==OBJ_INDEX && dynamic_cast<Index *>(obj_tab)->isReferRelationshipColumn()) ||
            (tipo_obj==OBJ_CONSTRAINT && dynamic_cast<Restricao *>(obj_tab)->referenciaColunaIncRelacao())))
   {
    if(tipo_op==Operation::OBJECT_REMOVED)
     obj_tab->setParentTable(tabela_pai);

    operacao->xml_definition=model->validarDefinicaoObjeto(obj_tab, SchemaParser::XML_DEFINITION);
   }

   operacao->parent_obj=objeto_pai;

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
    idx_obj=model->obterIndiceObjeto(objeto);
   else
    //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
    idx_obj=idx_objeto;
  }

  operacao->object_idx=idx_obj;

  //Insere a operação na lista de operações
  operations.push_back(operacao);

  //O índice atual de operação será o próprio tamanho da lista
  current_index=operations.size();
 }
 catch(Exception &e)
 {
  if(operacao)
  {
   removeFromPool(object_pool.size()-1);
   delete(operacao);
  }
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void OperationList::getOperationData(unsigned idx_oper, unsigned &tipo_oper, QString &nome_obj, ObjectType &tipo_obj)
{
 Operation *operacao=NULL;

 if(idx_oper >= operations.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 operacao=operations[idx_oper];
 tipo_oper=operacao->op_type;

 tipo_obj=operacao->pool_obj->getObjectType();

 if(tipo_obj==OBJ_CAST)
  nome_obj=operacao->pool_obj->getName();
 else
  nome_obj=operacao->pool_obj->getName(true);

 //Formata o nome especificamente para objetos da tabela incluindo o nome da tabela pai ao nome do objeto
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT || tipo_obj==OBJ_RULE ||
    tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_INDEX)
 {
  nome_obj=operacao->parent_obj->getName(true) + "." + nome_obj;
 }
}

unsigned OperationList::getChainSize(void)
{
 int i=current_index-1;
 unsigned tam=0;

 if(i < 0 && !operations.empty())
  i=0;

 //Verifica se a operação atual é de encadeamento
 if(!operations.empty() &&
    operations[i]->chain_type!=Operation::NO_CHAIN)
 {
  unsigned tipo_enc=0;
  int inc=0;

  //Caso seja um encadeamento final varre a lista de tras para frente
  if(operations[i]->chain_type==Operation::CHAIN_END)
  {
   tipo_enc=Operation::CHAIN_START;
   inc=-1;
  }
  //Caso seja um encadeamento inicial varre a lista de frente para tras
  else if(operations[i]->chain_type==Operation::CHAIN_START)
  {
   tipo_enc=Operation::CHAIN_END;
   inc=1;
  }

  //Varre a lista na sequencia definida contando as operações encadeadas
  while(i>=0 && i < static_cast<int>(operations.size()) && operations[i]->chain_type!=tipo_enc)
  {
   i+=inc;
   tam++;
  }
 }

 return(tam);
}

void OperationList::undoOperation(void)
{
 if(isUndoAvailable())
 {
  Operation *operacao=NULL;
  bool enc_ativo=false;
  Exception erro;
  unsigned tam_enc=0, pos=0;

  if(!this->signalsBlocked())
   tam_enc=getChainSize();

  do
  {
   /* Na operação de desfazer, é necessário obter o objeto
      no índice atual decrementado em 1 pois o índice atual sempre
      aponta para o elemento posterior ao último elemento */
   operacao=operations[current_index-1];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!ignore_chain && !enc_ativo &&
       operacao->chain_type!=Operation::NO_CHAIN)
     enc_ativo=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(enc_ativo &&
           (operacao->chain_type==Operation::CHAIN_END ||
            operacao->chain_type==Operation::NO_CHAIN))
    break;

   try
   {
    if(!this->signalsBlocked() && tam_enc > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operationExecuted((pos/static_cast<float>(tam_enc))*100,
                               trUtf8("Undoing operation on object: %1 (%2)")
                                      .arg(operacao->pool_obj->getName())
                                      .arg(operacao->pool_obj->getTypeName()),
                                       operacao->pool_obj->getObjectType());
    }

    //Executa a operação de desfazer
    executeOperation(operacao, false);
   }
   catch(Exception &e)
   {
    erro=e;
   }

   current_index--;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de desfazer esteja habilidata */
  while(!ignore_chain && isUndoAvailable() && operacao->chain_type!=Operation::NO_CHAIN);

  if(erro.getErrorType()!=ERR_CUSTOM)
   throw Exception(erro.getErrorMessage(), erro.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void OperationList::redoOperation(void)
{
 if(isRedoAvailable())
 {
  Operation *operacao=NULL;
  bool enc_ativo=false;
  Exception erro;
  unsigned tam_enc=0, pos=0;

  //Calcula o tamanho do encademanto de operações atual
  if(!this->signalsBlocked())
   tam_enc=getChainSize();

  do
  {
   //Obtém elemento atual da lista de operações
   operacao=operations[current_index];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!ignore_chain && !enc_ativo &&
      operacao->chain_type!=Operation::NO_CHAIN)
    enc_ativo=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(enc_ativo &&
           (operacao->chain_type==Operation::CHAIN_START ||
            operacao->chain_type==Operation::NO_CHAIN))
    break;

   try
   {
    if(tam_enc > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operationExecuted((pos/static_cast<float>(tam_enc))*100,
                               trUtf8("Redoing operation on object:: %1 (%2)")
                                      .arg(operacao->pool_obj->getName())
                                      .arg(operacao->pool_obj->getTypeName()),
                                       operacao->pool_obj->getObjectType());
    }

    //Executa a operação de refazer (segundo parametro = true)
    executeOperation(operacao, true);
   }
   catch(Exception &e)
   {
    erro=e;
   }
   current_index++;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de refazer esteja habilidata */
  while(!ignore_chain && isRedoAvailable()  && operacao->chain_type!=Operation::NO_CHAIN);

  if(erro.getErrorType()!=ERR_CUSTOM)
   throw Exception(erro.getErrorMessage(), erro.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void OperationList::executeOperation(Operation *oper, bool refazer)
{
  if(oper)
  {
   BaseObject *obj_orig=NULL, *obj_bkp=NULL, *objeto=NULL, *obj_aux=NULL;
   ObjectType tipo;
   Tabela *tabela_pai=NULL;
   Relacionamento *relac_pai=NULL;

   objeto=oper->pool_obj;
   tipo=objeto->getObjectType();

   /* Convertendo o objeto pai, caso exista, para a classe correta conforme
      o tipo do objeto pai. Caso seja OBJETO_TABELA, o ponteiro
      'tabela_pai' receberá a referência da tabela e
      será usado como referencial nas operações abaixo.
      Caso o objeto pai seja um relacionamento, o ponteiro
      'relac_pai' receberá a referência ao relacionamento */
   if(oper->parent_obj)
   {
    if(oper->parent_obj->getObjectType()==OBJ_TABLE)
     tabela_pai=dynamic_cast<Tabela *>(oper->parent_obj);
    else
     relac_pai=dynamic_cast<Relacionamento *>(oper->parent_obj);
   }

   /* Caso a definição xml do objeto esteja definida
      indica que o mesmo referencia uma coluna incluída por relacionamento */
   if(!oper->xml_definition.isEmpty() &&
      ((oper->op_type==Operation::OBJECT_REMOVED && !refazer) ||
       (oper->op_type==Operation::OBJECT_CREATED && refazer) ||
       (oper->op_type==Operation::OBJECT_MODIFIED ||
        oper->op_type==Operation::OBJECT_MOVED)))
   {
    //Reinicia o parser e carrega o buffer do mesmo com o código xml da operação
    XMLParser::restartParser();
    XMLParser::loadXMLBuffer(oper->xml_definition);

    //Cria uma cópia do objeto conforme o tipo do mesmo
    if(tipo==OBJ_TRIGGER)
     obj_aux=model->criarGatilho(tabela_pai);
    else if(tipo==OBJ_INDEX)
     obj_aux=model->criarIndice(tabela_pai);
    else if(tipo==OBJ_CONSTRAINT)
     obj_aux=model->criarRestricao(oper->parent_obj);
    else if(tipo==OBJ_SEQUENCE)
     obj_aux=model->criarSequencia();
   }

   /* Caso a operação seja de objeto modificado, a cópia do objeto
      armazenada no pool (no caso o parâmetro 'objeto') será restaurada */
   if(oper->op_type==Operation::OBJECT_MODIFIED ||
      oper->op_type==Operation::OBJECT_MOVED)
   {
    if(tipo==OBJ_RELATIONSHIP)
    {
     /* Devido a complexidade da classe Relacionamento e a forte ligação entre todos os
        relacinamentos do modelo, é necessário armazenar o XML dos objetos especiais e
        desconectar TODOS os relacionamentos, executar a modificação no
        relacionamento e logo após revalidar todos os demais */
     model->obterXMLObjetosEspeciais();
     model->desconectarRelacionamentos();
    }

    //Obtém a objeto atual da tabela/relacionamento pai no índice especificado
    if(tabela_pai)
     obj_orig=dynamic_cast<TableObject *>(tabela_pai->obterObjeto(oper->object_idx, tipo));
    else if(relac_pai)
     obj_orig=dynamic_cast<TableObject *>(relac_pai->obterObjeto(oper->object_idx, tipo));
    else
     obj_orig=model->obterObjeto(oper->object_idx, tipo);

    if(obj_aux)
     oper->xml_definition=model->validarDefinicaoObjeto(obj_orig, SchemaParser::SQL_DEFINITION);

     /* A objeto original (obtido da tabela, relacionamento pai ou modelo) tera seus valores anteriores
        restaurados com a cópia existente no pool. Após a restauração o objeto
        no pool terá como atributos os mesmo do objeto antes de ser restaurado,
        para possibilitar a operação de refazer */
    copyObject(reinterpret_cast<BaseObject **>(&obj_bkp), obj_orig, tipo);
    copyObject(reinterpret_cast<BaseObject **>(&obj_orig), objeto, tipo);
    copyObject(reinterpret_cast<BaseObject **>(&objeto), obj_bkp, tipo);
    objeto=obj_orig;

    if(obj_aux)
     copyObject(reinterpret_cast<BaseObject **>(&objeto), obj_aux, tipo);
   }
   /* Caso a operação seja de objeto removido e não seja uma operação de refazer, ou
      se o objeto foi criado anteriormente e se deseja refazer a operação.
      A objeto existente no pool será inserida na tabela ou relacionamento pai e no seu índice original */
   else if((oper->op_type==Operation::OBJECT_REMOVED && !refazer) ||
           (oper->op_type==Operation::OBJECT_CREATED && refazer))
   {
    if(obj_aux)
     copyObject(reinterpret_cast<BaseObject **>(&objeto), obj_aux, tipo);

    if(tabela_pai)
     tabela_pai->adicionarObjeto(dynamic_cast<TableObject *>(objeto), oper->object_idx);
    else if(relac_pai)
     relac_pai->adicionarObjeto(dynamic_cast<TableObject *>(objeto), oper->object_idx);
    else
     if(dynamic_cast<Tabela *>(objeto))
      dynamic_cast<Tabela *>(objeto)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
     model->adicionarObjeto(objeto, oper->object_idx);
   }
   /* Caso a operação seja de objeto criado anteriormente ou caso o objeto
     foi removido e se deseja refazer a operação o mesmo será
     excluído da tabela ou relacionamento pai */
   else if((oper->op_type==Operation::OBJECT_CREATED && !refazer) ||
           (oper->op_type==Operation::OBJECT_REMOVED && refazer))
   {
    if(tabela_pai)
     tabela_pai->removerObjeto(oper->object_idx,tipo);
    else if(relac_pai)
     relac_pai->removerObjeto(oper->object_idx,tipo);
    else
     model->removerObjeto(objeto, oper->object_idx);
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
     model->validarRelacObjetoTabela(dynamic_cast<TableObject *>(objeto), tabela_pai);
    else if(relac_pai)
     model->validarRelacionamentos();
   }
   /* Caso o objeto em questão seja um grafico o mesmo tem seus flags
      de modificado marcado em true e de selecionado em falso, para
      forçar o redesenho do mesmo no momento de sua restauração */
   else if(tipo==OBJ_TABLE || tipo==OBJ_VIEW ||
      tipo==BASE_RELATIONSHIP || tipo==OBJ_RELATIONSHIP ||
      tipo==OBJ_TEXTBOX)
   {
    BaseGraphicObject *obj_grafico=dynamic_cast<BaseGraphicObject *>(objeto);

    if(oper->op_type==Operation::OBJECT_MODIFIED ||
       oper->op_type==Operation::OBJECT_MOVED)
     obj_grafico->setModefied(true);

    //Caso seja uma visão atualiza os relacionamentos entre as tabelas e a visão
    if(tipo==OBJ_VIEW && oper->op_type==Operation::OBJECT_MODIFIED)
     model->atualizarRelTabelaVisao(dynamic_cast<Visao *>(obj_grafico));
    else if((tipo==OBJ_RELATIONSHIP ||
             (tipo==OBJ_TABLE && model->obterRelacionamento(dynamic_cast<BaseTable *>(objeto), NULL))) &&
            oper->op_type==Operation::OBJECT_MODIFIED)
     model->validarRelacionamentos();
   }
  }
}

void OperationList::removeLastOperation(void)
{
 if(!operations.empty())
 {
  Operation *oper=NULL;
  bool fim=false;
  vector<Operation *>::reverse_iterator itr;
  unsigned idx_obj=operations.size()-1;

  //Obtém a última operação da lista, utilizando um iterador reverso
  itr=operations.rbegin();

  while(!fim)
  {
   //Obtém a operação armazenada no iterador para sua devida remoção
   oper=(*itr);
   //Remove o objeto relacionado   operação e que se encontrar no pool
   removeFromPool(idx_obj);
   /* Condição de parada de remoção da operação:
      1) A operação não é encadeada com outras, ou
      2) Caso a ultima operação seja o final de um encadeamento
         todos as operações da cadeia serão removidas incluíndo
         a primeira operação do encadeamento, quando esta última
         é removida a iteração é interrompida.
   */
   fim=(ignore_chain ||
        (!ignore_chain &&
         (oper->chain_type==Operation::NO_CHAIN ||
          oper->chain_type==Operation::CHAIN_START)));

   //Passa para a operação anterior   atual
   itr++; idx_obj--;
  }

 /* Se o cabeça do encademanto for removido (ENC_INICIO)
    marca que o próximo elemento da lista será o novo
    início do encadeamnto */
 if(oper && oper->chain_type==Operation::CHAIN_START)
  next_op_chain=Operation::CHAIN_START;

  /* Executa a validação das operações removendo aquelas
     que possivelmente referenciam objetos inexistentes
     no pool de objetos */
  validateOperations();

  /* Atualiza o indice atual da lista, caso este aponte
     para um item além do final da lista de operações
     o mesmo passará a apontar para o ultimo elemento */
  if(static_cast<unsigned>(current_index) > operations.size())
   current_index=operations.size();
 }
}

void OperationList::updateObjectIndex(BaseObject *objeto, unsigned idx_novo)
{
 vector<Operation *>::iterator itr, itr_end;
 Operation *oper=NULL;

 if(!objeto)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=operations.begin();
 itr_end=operations.end();

 while(itr!=itr_end)
 {
  oper=(*itr);
  if(oper->generator_obj==objeto)
   oper->object_idx=idx_novo;
  itr++;
 }
}

