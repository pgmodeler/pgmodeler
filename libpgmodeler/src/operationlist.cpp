#include "operationlist.h"

template <class Classe>
void copyObject(BaseObject **porig_obj, Classe *copy_obj)
{
 Classe *orig_obj=NULL;

 //Obtém o endereço do objeto armazenado no ponteiro
 orig_obj=dynamic_cast<Classe *>(*porig_obj);

 //Caso o objeto de cópia não esteja aloca será disparada uma exceção
 if(!copy_obj)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o objeto de origem não esteja alocado
    o mesmo será alocado */
 if(!orig_obj)
 {
  orig_obj=new Classe;
  (*porig_obj)=orig_obj;
 }

 //Efetua a cópia entre os objetos
 (*orig_obj)=(*copy_obj);
}

void copyObject(BaseObject **porig_obj, BaseObject *copy_obj, ObjectType obj_type)
{
 switch(obj_type)
 {
  case OBJ_RELATIONSHIP:
    Relacionamento *rel1;
    rel1=new Relacionamento(dynamic_cast<Relacionamento *>(copy_obj));
    if(!(*porig_obj))
     (*porig_obj)=rel1;
    else
     (*(dynamic_cast<Relacionamento *>(*porig_obj)))=(*rel1);
  break;
  case BASE_RELATIONSHIP:
    RelacionamentoBase *rel;
    rel=new RelacionamentoBase(dynamic_cast<RelacionamentoBase *>(copy_obj));
    if(!(*porig_obj))
     (*porig_obj)=rel;
    else
     (*(dynamic_cast<RelacionamentoBase *>(*porig_obj)))=(*rel);
  break;
  case OBJ_COLUMN:
    copyObject(porig_obj, dynamic_cast<Column *>(copy_obj));
  break;
  case OBJ_CONSTRAINT:
    copyObject(porig_obj, dynamic_cast<Restricao *>(copy_obj));
  break;
  case OBJ_TRIGGER:
    copyObject(porig_obj, dynamic_cast<Gatilho *>(copy_obj));
  break;
  case OBJ_RULE:
    copyObject(porig_obj, dynamic_cast<Regra *>(copy_obj));
  break;
  case OBJ_INDEX:
    copyObject(porig_obj, dynamic_cast<Index *>(copy_obj));
  break;
  case OBJ_TABLE:
    copyObject(porig_obj, dynamic_cast<Tabela *>(copy_obj));
  break;
  case OBJ_TEXTBOX:
    copyObject(porig_obj, dynamic_cast<Textbox *>(copy_obj));
  break;
  case OBJ_OPCLASS:
    copyObject(porig_obj, dynamic_cast<OperatorClass *>(copy_obj));
  break;
  case OBJ_CONVERSION:
    copyObject(porig_obj, dynamic_cast<Conversion *>(copy_obj));
  break;
  case OBJ_CAST:
    copyObject(porig_obj, dynamic_cast<Cast *>(copy_obj));
  break;
  case OBJ_DOMAIN:
    copyObject(porig_obj, dynamic_cast<Domain *>(copy_obj));
  break;
  case OBJ_TABLESPACE:
    copyObject(porig_obj, dynamic_cast<Tablespace *>(copy_obj));
  break;
  case OBJ_SCHEMA:
    copyObject(porig_obj, dynamic_cast<Schema *>(copy_obj));
  break;
  case OBJ_OPFAMILY:
    copyObject(porig_obj, dynamic_cast<OperatorFamily *>(copy_obj));
  break;
  case OBJ_FUNCTION:
    copyObject(porig_obj, dynamic_cast<Function *>(copy_obj));
  break;
  case OBJ_AGGREGATE:
    copyObject(porig_obj, dynamic_cast<Aggregate *>(copy_obj));
  break;
  case OBJ_LANGUAGE:
    copyObject(porig_obj, dynamic_cast<Language *>(copy_obj));
  break;
  case OBJ_OPERATOR:
    copyObject(porig_obj, dynamic_cast<Operador *>(copy_obj));
  break;
  case OBJ_ROLE:
    copyObject(porig_obj, dynamic_cast<Papel *>(copy_obj));
  break;
  case OBJ_SEQUENCE:
    copyObject(porig_obj, dynamic_cast<Sequencia *>(copy_obj));
  break;
  case OBJ_TYPE:
    copyObject(porig_obj, dynamic_cast<Tipo *>(copy_obj));
  break;
  case OBJ_VIEW:
    copyObject(porig_obj, dynamic_cast<Visao *>(copy_obj));
  break;
  default:
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }
}

unsigned OperationList::max_size=500;

OperationList::OperationList(ModeloBD *model)
{
 //Dispara uma exceção caso o modelo passado não esteja alocado
 if(!model)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->model=model;
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

void OperationList::ignoreOperationChain(bool value)
{
 ignore_chain=value;
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

void OperationList::setMaximumSize(unsigned max)
{
 /* A lista de operações não pode ter valor 0 em seu tamanho máximo,
    sendo assim é disparada uma exceção caso isso ocorra */
 if(max==0)
  throw Exception(ERR_ASG_INV_MAX_SIZE_OP_LIST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define o tamanho máximo da lista
 max_size=max;
}

void OperationList::addToPool(BaseObject *object, unsigned op_type)
{
 ObjectType obj_type;

 /* Caso se tente inserir no pool um objeto não alocado
    é disparada uma exceção */
 if(!object)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 obj_type=object->getObjectType();

 /* Caso o objeto está prestes a ser removido ou modificado, será armazenado no pool
    uma cópia do mesmo e não o objeto em si */
 if(op_type==Operation::OBJECT_MODIFIED ||
    op_type==Operation::OBJECT_MOVED)
 {
  BaseObject *copy_obj=NULL;

  //Faz a cópia apenas de objetos descendetes das classes bases
  if(obj_type!=BASE_OBJECT && obj_type!=OBJ_DATABASE)
  {
   //Copia o objeto
   copyObject(&copy_obj, object, obj_type);
  }
  else
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Caso a cópia falhe esta retorna um objeto nulo e a adição de objeto
     será cancelada com o disparo da exceção */
  if(!copy_obj)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   //Insere a cópia do objeto no pool
   object_pool.push_back(copy_obj);
   object=copy_obj;
 }
 else
  //Insere o próprio objeto no pool
  object_pool.push_back(object);
}

void OperationList::removeOperations(void)
{
 BaseObject *object=NULL;
 TableObject *tab_obj=NULL;
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
  object=not_removed_objs.back();
  tab_obj=dynamic_cast<TableObject *>(object);

  if(!tab_obj && model->obterIndiceObjeto(object) < 0)
   delete(object);
  else if(tab_obj)
  {
   tab=dynamic_cast<Tabela *>(tab_obj->getParentTable());
   if(!tab || (tab && tab->obterIndiceObjeto(tab_obj)) < 0)
    delete(tab_obj);
  }

  not_removed_objs.pop_back();
 }

 current_index=0;
}

void OperationList::validateOperations(void)
{
 vector<Operation *>::iterator itr, itr_end;
 Operation *oper=NULL;

 itr=operations.begin();
 itr_end=operations.end();
 while(itr!=itr_end)
 {
  //Obtém a referência ao objeto
  oper=(*itr);
  if(!isObjectOnPool((*itr)->pool_obj))
  {
   //Remove o elemento do pool
   operations.erase(itr);
   //Desaloca o objeto
   delete(oper);
   itr=operations.begin();
   itr_end=operations.end();
  }
  else itr++;
 }
}

bool OperationList::isObjectOnPool(BaseObject *object)
{
 bool found=false;
 vector<BaseObject *>::iterator itr, itr_end;

 if(!object)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=object_pool.begin();
 itr_end=object_pool.end();
 while(!found && itr!=itr_end)
 {
  found=*(itr)==object;
  itr++;
 }

 return(found);
}

void OperationList::removeFromPool(unsigned obj_idx)
{
 BaseObject *object=NULL;
 vector<BaseObject *>::iterator itr;

 //Caso o índice do objeto a ser excluído seja inválido é disparada uma exceção
 if(obj_idx >= object_pool.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o elemento a ser removido
 itr=object_pool.begin() + obj_idx;
 object=(*itr);

 //Remove o elemento do pool
 object_pool.erase(itr);

 /* Armazena o objeto que antes se encontrava no pool no vetor 'objs_nao_excluidos' para que
    o mesmo seja excluído no destrutor da lista. OBS.: O objeto não é deletado de imediato
    pois no modelo / tabela / lista de operações o mesmo ainda pode estar sendo referenciado
    ou passar a ser referenciado quando uma operação da lista for executada. */
  not_removed_objs.push_back(object);
}


void OperationList::registerObject(BaseObject *object, unsigned op_type, int object_idx,  BaseObject *parent_obj)
{
 ObjectType obj_type;
 Operation *operation=NULL;
 Tabela *parent_tab=NULL;
 Relacionamento *parent_rel=NULL;
 int obj_idx=-1;

 try
 {
  //Caso se tente adicionar uma operação com objeto não alocado, dispara uma exceção
  if(!object)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  obj_type=object->getObjectType();
  if((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
      obj_type==OBJ_INDEX || obj_type==OBJ_TRIGGER ||
      obj_type==OBJ_RULE) && !parent_obj)
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  else if(parent_obj &&
     (((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT) &&
      (parent_obj->getObjectType()!=OBJ_RELATIONSHIP && parent_obj->getObjectType()!=OBJ_TABLE)) ||

      ((obj_type==OBJ_INDEX || obj_type==OBJ_TRIGGER || obj_type==OBJ_RULE) &&
        parent_obj->getObjectType()!=OBJ_TABLE)))
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
  operation=new Operation;
  operation->op_type=op_type;
  operation->chain_type=next_op_chain;
  operation->generator_obj=object;

  //Adiciona o objeto no pool
  addToPool(object, op_type);

  //Atribui   operação o objeto inserido no pool
  operation->pool_obj=object_pool.back();

  /* Caso o tipo de encadeamento atual da operação seja de início de
     encademaneto, configura o tipo para a próxima operação a ser
     inserida como sendo uma operação no meio do encadeamento. */
   if(next_op_chain==Operation::CHAIN_START)
    next_op_chain=Operation::CHAIN_MIDDLE;

  /* Executando operações específicada de acorodo com o tipo de objeto.
     Caso o objeto possua um objeto pai, o mesmo precisa ser descoberto
     e além disso é necessário descobrir e armazenar o índice do objeto
     na lista contida em seu objeto pai */
  if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
     obj_type==OBJ_INDEX || obj_type==OBJ_TRIGGER ||
     obj_type==OBJ_RULE)
  {
   TableObject *tab_obj=NULL;
   tab_obj=dynamic_cast<TableObject *>(object);

   if(parent_obj->getObjectType()==OBJ_TABLE)
    parent_tab=dynamic_cast<Tabela *>(parent_obj);
   else
    parent_rel=dynamic_cast<Relacionamento *>(parent_obj);

   /* Caso específico para colunas: em operações de remoção do objeto
      as permissões daquele objeto precisam ser removidas. */
   if(obj_type==OBJ_COLUMN && op_type==Operation::OBJECT_REMOVED)
    model->removerPermissoes(tab_obj);
   else if(((obj_type==OBJ_TRIGGER && dynamic_cast<Gatilho *>(tab_obj)->isReferRelationshipColumn()) ||
            (obj_type==OBJ_INDEX && dynamic_cast<Index *>(tab_obj)->isReferRelationshipColumn()) ||
            (obj_type==OBJ_CONSTRAINT && dynamic_cast<Restricao *>(tab_obj)->referenciaColunaIncRelacao())))
   {
    if(op_type==Operation::OBJECT_REMOVED)
     tab_obj->setParentTable(parent_tab);

    operation->xml_definition=model->validarDefinicaoObjeto(tab_obj, SchemaParser::XML_DEFINITION);
   }

   operation->parent_obj=parent_obj;

   /* Caso haja um relacionamento pai será obtido o índice do objeto.
      Apenas colunas e restrições são manipuladas caso o pai seja
      um relacionamento */
   if(parent_rel && (obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT))
   {
    //Caso um índice específico para o objeto não foi especificado
    if(object_idx < 0)
     //Armazena na operação o índice do objeto em seu pai
     obj_idx=parent_rel->obterIndiceObjeto(tab_obj);
    else
     //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
     obj_idx=object_idx;
   }
   //Caso haja uma tabela pai será obtido o índice do objeto
   else if(parent_tab)
   {
    //Caso um índice específico para o objeto não foi especificado
    if(object_idx < 0)
     //Armazena na operação o índice do objeto em seu pai
     obj_idx=parent_tab->obterIndiceObjeto(object->getName(false), obj_type);
    else
     //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
     obj_idx=object_idx;
   }
   /* Caso nem tabela pai nem relacionametno pai estejam alocados
      retorna um erro ao usuário e desaloca a operção criada */
   else
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  else
  {
   //Caso um índice específico para o objeto não foi especificado
   if(object_idx < 0)
    //Armazena na operação o índice do objeto no modelo
    obj_idx=model->obterIndiceObjeto(object);
   else
    //Atribui o índice específico definido pelo usuário ao índice do objeto na operação
    obj_idx=object_idx;
  }

  operation->object_idx=obj_idx;

  //Insere a operação na lista de operações
  operations.push_back(operation);

  //O índice atual de operação será o próprio tamanho da lista
  current_index=operations.size();
 }
 catch(Exception &e)
 {
  if(operation)
  {
   removeFromPool(object_pool.size()-1);
   delete(operation);
  }
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void OperationList::getOperationData(unsigned oper_idx, unsigned &oper_type, QString &obj_name, ObjectType &obj_type)
{
 Operation *operation=NULL;

 if(oper_idx >= operations.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 operation=operations[oper_idx];
 oper_type=operation->op_type;

 obj_type=operation->pool_obj->getObjectType();

 if(obj_type==OBJ_CAST)
  obj_name=operation->pool_obj->getName();
 else
  obj_name=operation->pool_obj->getName(true);

 //Formata o nome especificamente para objetos da tabela incluindo o nome da tabela pai ao nome do objeto
 if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT || obj_type==OBJ_RULE ||
    obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX)
 {
  obj_name=operation->parent_obj->getName(true) + "." + obj_name;
 }
}

unsigned OperationList::getChainSize(void)
{
 int i=current_index-1;
 unsigned size=0;

 if(i < 0 && !operations.empty())
  i=0;

 //Verifica se a operação atual é de encadeamento
 if(!operations.empty() &&
    operations[i]->chain_type!=Operation::NO_CHAIN)
 {
  unsigned chain_type=0;
  int inc=0;

  //Caso seja um encadeamento final varre a lista de tras para frente
  if(operations[i]->chain_type==Operation::CHAIN_END)
  {
   chain_type=Operation::CHAIN_START;
   inc=-1;
  }
  //Caso seja um encadeamento inicial varre a lista de frente para tras
  else if(operations[i]->chain_type==Operation::CHAIN_START)
  {
   chain_type=Operation::CHAIN_END;
   inc=1;
  }

  //Varre a lista na sequencia definida contando as operações encadeadas
  while(i>=0 && i < static_cast<int>(operations.size()) && operations[i]->chain_type!=chain_type)
  {
   i+=inc;
   size++;
  }
 }

 return(size);
}

void OperationList::undoOperation(void)
{
 if(isUndoAvailable())
 {
  Operation *operation=NULL;
  bool chain_active=false;
  Exception error;
  unsigned chain_size=0, pos=0;

  if(!this->signalsBlocked())
   chain_size=getChainSize();

  do
  {
   /* Na operação de desfazer, é necessário obter o objeto
      no índice atual decrementado em 1 pois o índice atual sempre
      aponta para o elemento posterior ao último elemento */
   operation=operations[current_index-1];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!ignore_chain && !chain_active &&
       operation->chain_type!=Operation::NO_CHAIN)
     chain_active=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(chain_active &&
           (operation->chain_type==Operation::CHAIN_END ||
            operation->chain_type==Operation::NO_CHAIN))
    break;

   try
   {
    if(!this->signalsBlocked() && chain_size > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operationExecuted((pos/static_cast<float>(chain_size))*100,
                               trUtf8("Undoing operation on object: %1 (%2)")
                                      .arg(operation->pool_obj->getName())
                                      .arg(operation->pool_obj->getTypeName()),
                                       operation->pool_obj->getObjectType());
    }

    //Executa a operação de desfazer
    executeOperation(operation, false);
   }
   catch(Exception &e)
   {
    error=e;
   }

   current_index--;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de desfazer esteja habilidata */
  while(!ignore_chain && isUndoAvailable() && operation->chain_type!=Operation::NO_CHAIN);

  if(error.getErrorType()!=ERR_CUSTOM)
   throw Exception(error.getErrorMessage(), error.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void OperationList::redoOperation(void)
{
 if(isRedoAvailable())
 {
  Operation *operation=NULL;
  bool chain_active=false;
  Exception error;
  unsigned chain_size=0, pos=0;

  //Calcula o tamanho do encademanto de operações atual
  if(!this->signalsBlocked())
   chain_size=getChainSize();

  do
  {
   //Obtém elemento atual da lista de operações
   operation=operations[current_index];

   /* Caso seja detectada que a operação é encadeada com outras
      e o flag de encadeamento ativo esteja desmarcado, marca o
      flag para dar início a executação de várias operações de uma
      só vez */
   if(!ignore_chain && !chain_active &&
      operation->chain_type!=Operation::NO_CHAIN)
    chain_active=true;

   /* Caso o encadeamento esteja ativo e a operação atual não faça parte do
      encadeamento, aborta a execução da operação */
   else if(chain_active &&
           (operation->chain_type==Operation::CHAIN_START ||
            operation->chain_type==Operation::NO_CHAIN))
    break;

   try
   {
    if(chain_size > 0)
    {
     //Dispara um sinal com o progresso da operação encadeada
     pos++;
     emit s_operationExecuted((pos/static_cast<float>(chain_size))*100,
                               trUtf8("Redoing operation on object:: %1 (%2)")
                                      .arg(operation->pool_obj->getName())
                                      .arg(operation->pool_obj->getTypeName()),
                                       operation->pool_obj->getObjectType());
    }

    //Executa a operação de refazer (segundo parametro = true)
    executeOperation(operation, true);
   }
   catch(Exception &e)
   {
    error=e;
   }
   current_index++;
  }
  /* Executa a operação enquanto a operação faça parte de encadeamento
     ou a opção de refazer esteja habilidata */
  while(!ignore_chain && isRedoAvailable()  && operation->chain_type!=Operation::NO_CHAIN);

  if(error.getErrorType()!=ERR_CUSTOM)
   throw Exception(error.getErrorMessage(), error.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void OperationList::executeOperation(Operation *oper, bool redo)
{
  if(oper)
  {
   BaseObject *orig_obj=NULL, *bkp_obj=NULL, *object=NULL, *aux_obj=NULL;
   ObjectType obj_type;
   Tabela *parent_tab=NULL;
   Relacionamento *parent_rel=NULL;

   object=oper->pool_obj;
   obj_type=object->getObjectType();

   /* Convertendo o objeto pai, caso exista, para a classe correta conforme
      o tipo do objeto pai. Caso seja OBJETO_TABELA, o ponteiro
      'tabela_pai' receberá a referência da tabela e
      será usado como referencial nas operações abaixo.
      Caso o objeto pai seja um relacionamento, o ponteiro
      'relac_pai' receberá a referência ao relacionamento */
   if(oper->parent_obj)
   {
    if(oper->parent_obj->getObjectType()==OBJ_TABLE)
     parent_tab=dynamic_cast<Tabela *>(oper->parent_obj);
    else
     parent_rel=dynamic_cast<Relacionamento *>(oper->parent_obj);
   }

   /* Caso a definição xml do objeto esteja definida
      indica que o mesmo referencia uma coluna incluída por relacionamento */
   if(!oper->xml_definition.isEmpty() &&
      ((oper->op_type==Operation::OBJECT_REMOVED && !redo) ||
       (oper->op_type==Operation::OBJECT_CREATED && redo) ||
       (oper->op_type==Operation::OBJECT_MODIFIED ||
        oper->op_type==Operation::OBJECT_MOVED)))
   {
    //Reinicia o parser e carrega o buffer do mesmo com o código xml da operação
    XMLParser::restartParser();
    XMLParser::loadXMLBuffer(oper->xml_definition);

    //Cria uma cópia do objeto conforme o tipo do mesmo
    if(obj_type==OBJ_TRIGGER)
     aux_obj=model->criarGatilho(parent_tab);
    else if(obj_type==OBJ_INDEX)
     aux_obj=model->criarIndice(parent_tab);
    else if(obj_type==OBJ_CONSTRAINT)
     aux_obj=model->criarRestricao(oper->parent_obj);
    else if(obj_type==OBJ_SEQUENCE)
     aux_obj=model->criarSequencia();
   }

   /* Caso a operação seja de objeto modificado, a cópia do objeto
      armazenada no pool (no caso o parâmetro 'objeto') será restaurada */
   if(oper->op_type==Operation::OBJECT_MODIFIED ||
      oper->op_type==Operation::OBJECT_MOVED)
   {
    if(obj_type==OBJ_RELATIONSHIP)
    {
     /* Devido a complexidade da classe Relacionamento e a forte ligação entre todos os
        relacinamentos do modelo, é necessário armazenar o XML dos objetos especiais e
        desconectar TODOS os relacionamentos, executar a modificação no
        relacionamento e logo após revalidar todos os demais */
     model->obterXMLObjetosEspeciais();
     model->desconectarRelacionamentos();
    }

    //Obtém a objeto atual da tabela/relacionamento pai no índice especificado
    if(parent_tab)
     orig_obj=dynamic_cast<TableObject *>(parent_tab->obterObjeto(oper->object_idx, obj_type));
    else if(parent_rel)
     orig_obj=dynamic_cast<TableObject *>(parent_rel->obterObjeto(oper->object_idx, obj_type));
    else
     orig_obj=model->obterObjeto(oper->object_idx, obj_type);

    if(aux_obj)
     oper->xml_definition=model->validarDefinicaoObjeto(orig_obj, SchemaParser::SQL_DEFINITION);

     /* A objeto original (obtido da tabela, relacionamento pai ou modelo) tera seus valores anteriores
        restaurados com a cópia existente no pool. Após a restauração o objeto
        no pool terá como atributos os mesmo do objeto antes de ser restaurado,
        para possibilitar a operação de refazer */
    copyObject(reinterpret_cast<BaseObject **>(&bkp_obj), orig_obj, obj_type);
    copyObject(reinterpret_cast<BaseObject **>(&orig_obj), object, obj_type);
    copyObject(reinterpret_cast<BaseObject **>(&object), bkp_obj, obj_type);
    object=orig_obj;

    if(aux_obj)
     copyObject(reinterpret_cast<BaseObject **>(&object), aux_obj, obj_type);
   }
   /* Caso a operação seja de objeto removido e não seja uma operação de refazer, ou
      se o objeto foi criado anteriormente e se deseja refazer a operação.
      A objeto existente no pool será inserida na tabela ou relacionamento pai e no seu índice original */
   else if((oper->op_type==Operation::OBJECT_REMOVED && !redo) ||
           (oper->op_type==Operation::OBJECT_CREATED && redo))
   {
    if(aux_obj)
     copyObject(reinterpret_cast<BaseObject **>(&object), aux_obj, obj_type);

    if(parent_tab)
     parent_tab->adicionarObjeto(dynamic_cast<TableObject *>(object), oper->object_idx);
    else if(parent_rel)
     parent_rel->adicionarObjeto(dynamic_cast<TableObject *>(object), oper->object_idx);
    else
     if(dynamic_cast<Tabela *>(object))
      dynamic_cast<Tabela *>(object)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
     model->adicionarObjeto(object, oper->object_idx);
   }
   /* Caso a operação seja de objeto criado anteriormente ou caso o objeto
     foi removido e se deseja refazer a operação o mesmo será
     excluído da tabela ou relacionamento pai */
   else if((oper->op_type==Operation::OBJECT_CREATED && !redo) ||
           (oper->op_type==Operation::OBJECT_REMOVED && redo))
   {
    if(parent_tab)
     parent_tab->removerObjeto(oper->object_idx,obj_type);
    else if(parent_rel)
     parent_rel->removerObjeto(oper->object_idx,obj_type);
    else
     model->removerObjeto(object, oper->object_idx);
   }

   /* Caso a tabela pai ou relacionamento pai esteja setados
      indica que a operação foi executada em um objeto filho desses objetos */
   if(parent_tab || parent_rel)
   {
    if(parent_tab)
     //Marca que a tabela pai foi modificada forçando assim seu redimensionamento
     parent_tab->setModified(true);
    else
     //Marca que o relacionamento pai foi modificado forçando assim seu redimensionamento
    parent_rel->setModified(true);

    if(parent_tab &&
      (object->getObjectType()==OBJ_COLUMN ||
       object->getObjectType()==OBJ_CONSTRAINT))
     model->validarRelacObjetoTabela(dynamic_cast<TableObject *>(object), parent_tab);
    else if(parent_rel)
     model->validarRelacionamentos();
   }
   /* Caso o objeto em questão seja um grafico o mesmo tem seus flags
      de modificado marcado em true e de selecionado em falso, para
      forçar o redesenho do mesmo no momento de sua restauração */
   else if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW ||
      obj_type==BASE_RELATIONSHIP || obj_type==OBJ_RELATIONSHIP ||
      obj_type==OBJ_TEXTBOX)
   {
    BaseGraphicObject *obj_grafico=dynamic_cast<BaseGraphicObject *>(object);

    if(oper->op_type==Operation::OBJECT_MODIFIED ||
       oper->op_type==Operation::OBJECT_MOVED)
     obj_grafico->setModified(true);

    //Caso seja uma visão atualiza os relacionamentos entre as tabelas e a visão
    if(obj_type==OBJ_VIEW && oper->op_type==Operation::OBJECT_MODIFIED)
     model->atualizarRelTabelaVisao(dynamic_cast<Visao *>(obj_grafico));
    else if((obj_type==OBJ_RELATIONSHIP ||
             (obj_type==OBJ_TABLE && model->obterRelacionamento(dynamic_cast<BaseTable *>(object), NULL))) &&
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
  bool end=false;
  vector<Operation *>::reverse_iterator itr;
  unsigned obj_idx=operations.size()-1;

  //Obtém a última operação da lista, utilizando um iterador reverso
  itr=operations.rbegin();

  while(!end)
  {
   //Obtém a operação armazenada no iterador para sua devida remoção
   oper=(*itr);
   //Remove o objeto relacionado   operação e que se encontrar no pool
   removeFromPool(obj_idx);
   /* Condição de parada de remoção da operação:
      1) A operação não é encadeada com outras, ou
      2) Caso a ultima operação seja o final de um encadeamento
         todos as operações da cadeia serão removidas incluíndo
         a primeira operação do encadeamento, quando esta última
         é removida a iteração é interrompida.
   */
   end=(ignore_chain ||
        (!ignore_chain &&
         (oper->chain_type==Operation::NO_CHAIN ||
          oper->chain_type==Operation::CHAIN_START)));

   //Passa para a operação anterior   atual
   itr++; obj_idx--;
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

void OperationList::updateObjectIndex(BaseObject *object, unsigned new_idx)
{
 vector<Operation *>::iterator itr, itr_end;
 Operation *oper=NULL;

 if(!object)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=operations.begin();
 itr_end=operations.end();

 while(itr!=itr_end)
 {
  oper=(*itr);
  if(oper->generator_obj==object)
   oper->object_idx=new_idx;
  itr++;
 }
}

