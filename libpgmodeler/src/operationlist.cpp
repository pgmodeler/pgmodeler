#include "operationlist.h"

template <class Classe>
void copyObject(BaseObject **psrc_obj, Classe *copy_obj)
{
 Classe *orig_obj=NULL;

 //Gets the objects stored in the pointer
 orig_obj=dynamic_cast<Classe *>(*psrc_obj);

 //Raises an error if the copy object is not allocated
 if(!copy_obj)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Allocates the source object if its not allocated
 if(!orig_obj)
 {
  orig_obj=new Classe;
  (*psrc_obj)=orig_obj;
 }

 //Makes the copy between the objects
 (*orig_obj)=(*copy_obj);
}

void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type)
{
 switch(obj_type)
 {
  case OBJ_RELATIONSHIP:
    Relacionamento *rel1;
    rel1=new Relacionamento(dynamic_cast<Relacionamento *>(copy_obj));
    if(!(*psrc_obj))
     (*psrc_obj)=rel1;
    else
     (*(dynamic_cast<Relacionamento *>(*psrc_obj)))=(*rel1);
  break;
  case BASE_RELATIONSHIP:
    RelacionamentoBase *rel;
    rel=new RelacionamentoBase(dynamic_cast<RelacionamentoBase *>(copy_obj));
    if(!(*psrc_obj))
     (*psrc_obj)=rel;
    else
     (*(dynamic_cast<RelacionamentoBase *>(*psrc_obj)))=(*rel);
  break;
  case OBJ_COLUMN:
    copyObject(psrc_obj, dynamic_cast<Column *>(copy_obj));
  break;
  case OBJ_CONSTRAINT:
    copyObject(psrc_obj, dynamic_cast<Restricao *>(copy_obj));
  break;
  case OBJ_TRIGGER:
    copyObject(psrc_obj, dynamic_cast<Gatilho *>(copy_obj));
  break;
  case OBJ_RULE:
    copyObject(psrc_obj, dynamic_cast<Regra *>(copy_obj));
  break;
  case OBJ_INDEX:
    copyObject(psrc_obj, dynamic_cast<Index *>(copy_obj));
  break;
  case OBJ_TABLE:
    copyObject(psrc_obj, dynamic_cast<Tabela *>(copy_obj));
  break;
  case OBJ_TEXTBOX:
    copyObject(psrc_obj, dynamic_cast<Textbox *>(copy_obj));
  break;
  case OBJ_OPCLASS:
    copyObject(psrc_obj, dynamic_cast<OperatorClass *>(copy_obj));
  break;
  case OBJ_CONVERSION:
    copyObject(psrc_obj, dynamic_cast<Conversion *>(copy_obj));
  break;
  case OBJ_CAST:
    copyObject(psrc_obj, dynamic_cast<Cast *>(copy_obj));
  break;
  case OBJ_DOMAIN:
    copyObject(psrc_obj, dynamic_cast<Domain *>(copy_obj));
  break;
  case OBJ_TABLESPACE:
    copyObject(psrc_obj, dynamic_cast<Tablespace *>(copy_obj));
  break;
  case OBJ_SCHEMA:
    copyObject(psrc_obj, dynamic_cast<Schema *>(copy_obj));
  break;
  case OBJ_OPFAMILY:
    copyObject(psrc_obj, dynamic_cast<OperatorFamily *>(copy_obj));
  break;
  case OBJ_FUNCTION:
    copyObject(psrc_obj, dynamic_cast<Function *>(copy_obj));
  break;
  case OBJ_AGGREGATE:
    copyObject(psrc_obj, dynamic_cast<Aggregate *>(copy_obj));
  break;
  case OBJ_LANGUAGE:
    copyObject(psrc_obj, dynamic_cast<Language *>(copy_obj));
  break;
  case OBJ_OPERATOR:
    copyObject(psrc_obj, dynamic_cast<Operador *>(copy_obj));
  break;
  case OBJ_ROLE:
    copyObject(psrc_obj, dynamic_cast<Papel *>(copy_obj));
  break;
  case OBJ_SEQUENCE:
    copyObject(psrc_obj, dynamic_cast<Sequencia *>(copy_obj));
  break;
  case OBJ_TYPE:
    copyObject(psrc_obj, dynamic_cast<Tipo *>(copy_obj));
  break;
  case OBJ_VIEW:
    copyObject(psrc_obj, dynamic_cast<Visao *>(copy_obj));
  break;
  default:
    throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }
}

unsigned OperationList::max_size=500;

OperationList::OperationList(ModeloBD *model)
{
 /* Raises an error if the user tries to allocate an operation list linked to
    to an unallocated model */
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
 /* If the chaining is started and the user try it initializes
    again, the earlier chaining is finished */
 if(next_op_chain!=Operation::NO_CHAIN)
  finishOperationChain();

 /* The next operation inserted on the list will be the start
    of the chaining */
 next_op_chain=Operation::CHAIN_START;
}

void OperationList::finishOperationChain(void)
{
 /* If the chain is not ignored indicates that the next
    element of the list no longer will be part of chaining */
 if(!ignore_chain)
  next_op_chain=Operation::NO_CHAIN;
 else if(ignore_chain)
  /* If the chain is canceled indicates that the next element
     of the list still part of the chaining */
  next_op_chain=Operation::CHAIN_MIDDLE;

 if(operations.size() > 0 && !ignore_chain)
 {
  unsigned idx=operations.size()-1;

  /* Marks the last operatin as being the end of chaining in case it is
     on the middle of chain */
  if(operations[idx]->chain_type==Operation::CHAIN_MIDDLE)
   operations[idx]->chain_type=Operation::CHAIN_END;

  /* If the last operation is marked as CHAIN_START indicates that
     the chaining was open but only one operation is recorded
     and thus the operation is marked as NO_CHAIN because as it is
     only one operation there is no need to treat it as chaining */
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
 /* The redo operation only can be performed
    if the current index from the list of operations is at most
    the penultimate element and the list can not be empty */
 return(!operations.empty() && current_index < static_cast<int>(operations.size()));
}

bool OperationList::isUndoAvailable(void)
{
 /* For the undo operation be performed is
    enough that the list of operations is not empty */
 return(!operations.empty() && current_index > 0);
}

void OperationList::setMaximumSize(unsigned max)
{
 //Raises an error if a zero max size is assigned to the list
 if(max==0)
  throw Exception(ERR_ASG_INV_MAX_SIZE_OP_LIST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 max_size=max;
}

void OperationList::addToPool(BaseObject *object, unsigned op_type)
{
 ObjectType obj_type;

 //Raises an error if the object to be added is not allocated
 if(!object)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 obj_type=object->getObjectType();

 //Stores a copy of the object if its about to be moved or modified
 if(op_type==Operation::OBJECT_MODIFIED ||
    op_type==Operation::OBJECT_MOVED)
 {
  BaseObject *copy_obj=NULL;

  if(obj_type!=BASE_OBJECT && obj_type!=OBJ_DATABASE)
   copyObject(&copy_obj, object, obj_type);
  else
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Raises an error if the copy fails (returning a null object)
  if(!copy_obj)
   throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   //Inserts the copy on the pool
   object_pool.push_back(copy_obj);
   //object=copy_obj;
 }
 else
  //Inserts the original object on the pool (in case of adition or deletion operations)
  object_pool.push_back(object);
}

void OperationList::removeOperations(void)
{
 BaseObject *object=NULL;
 TableObject *tab_obj=NULL;
 Tabela *tab=NULL;

 //Destroy the operations
 while(!operations.empty())
 {
  delete(operations.back());
  operations.pop_back();
 }

 //Destroy the object pool
 while(!object_pool.empty())
  removeFromPool(0);

 //Case there is not removed object
 while(!not_removed_objs.empty())
 {
  object=not_removed_objs.back();
  tab_obj=dynamic_cast<TableObject *>(object);

  //Deletes the object if its not referenced on the model
  if(!tab_obj && model->obterIndiceObjeto(object) < 0)
   delete(object);
  else if(tab_obj)
  {
   //Deletes the object if its not referenced by some table
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
  oper=(*itr);
  //Case the object isn't on the pool
  if(!isObjectOnPool((*itr)->pool_obj))
  {
   //Remove the operation
   operations.erase(itr);
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

 //Raises an error if the object index is invalid (out of bound)
 if(obj_idx >= object_pool.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Gets the element to bo removed through its index
 itr=object_pool.begin() + obj_idx;
 object=(*itr);

 //Removes the object from pool
 object_pool.erase(itr);

 /* Stores the object that was in the pool on the 'not_removed_objs' vector.
    The object will be deleted in the destructor of the list. Note: The object is not
    deleted immediately because the model / table / list of operations may still
    referencing the object or it becomes referenced when an operation is performed on the list. */
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
  //Raises an error if the user tries to register an operation with null object
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

  //If the operations list is full makes the automatic cleaning before inserting a new operation
  if(current_index == static_cast<int>(max_size-1))
   removeOperations();

  /* If adding an operation and the current index is not pointing
     to the end of the list (available redo / user ran undo operations)
     all elements from the current index to the end of the list will be deleted
     as well as the objects in the pool that were linked to the excluded operations */
  if(current_index>=0 && static_cast<unsigned>(current_index)!=operations.size())
  {
   //Gets the last operation index
   int i=operations.size()-1;

   //Removes all the operation while the current index isn't reached
   while(i >= current_index)
   {
    removeFromPool(i);
    i--;
   }

   //Validates the remaining operatoins after the deletion
   validateOperations();
  }

  //Creates the new operation
  operation=new Operation;
  operation->op_type=op_type;
  operation->chain_type=next_op_chain;
  operation->original_obj=object;

  //Adds the object on te pool
  addToPool(object, op_type);

  //Assigns the pool object to the operation
  operation->pool_obj=object_pool.back();

  if(next_op_chain==Operation::CHAIN_START)
   next_op_chain=Operation::CHAIN_MIDDLE;

 /*  Performing specific operations according to the type of object.
     If the object has a parent object, it must be discovered
     and moreover it is necessary to find and store the index of the object
     in the list on the parent object */
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

   /* Specific case to columns: on removal operations the permissions of the objects
      must be removed too */
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

   /* If there is a parent relationship will get the index of the object.
      Only columns and constraints are handled case the parent is a relationship */
   if(parent_rel && (obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT))
   {
    //Case a specific index wasn't specified
    if(object_idx < 0)
     //Stores on the operation the index on its parent
     obj_idx=parent_rel->obterIndiceObjeto(tab_obj);
    else
     //Assigns the specific index to operation
     obj_idx=object_idx;
   }
   //Case there is a parent table will get the object's index
   else if(parent_tab)
   {
    if(object_idx < 0)
     //Stores on the operation the index of object on its parent
     obj_idx=parent_tab->obterIndiceObjeto(object->getName(false), obj_type);
    else
     obj_idx=object_idx;
   }
   //Raises an error if both parent table / relationship isn't allocated
   else
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  else
  {
   //Case a specific index wasn't specified
   if(object_idx < 0)
    //Stores on the operation the object index on the model
    obj_idx=model->obterIndiceObjeto(object);
   else
    //Assigns the specific index to object
    obj_idx=object_idx;
  }

  operation->object_idx=obj_idx;
  operations.push_back(operation);
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
  if(oper->original_obj==object)
   oper->object_idx=new_idx;
  itr++;
 }
}

