#include "role.h"

unsigned Role::role_id=0;

Role::Role(void)
{
 obj_type=OBJ_ROLE;
 object_id=Role::role_id++;
 sysid=100;

 for(unsigned i=0; i < 6; i++)
  options[i]=false;

 conn_limit=-1;

 attributes[ParsersAttributes::SUPERUSER]="";
 attributes[ParsersAttributes::CREATEDB]="";
 attributes[ParsersAttributes::CREATEROLE]="";
 attributes[ParsersAttributes::INHERIT]="";
 attributes[ParsersAttributes::LOGIN]="";
 attributes[ParsersAttributes::CONN_LIMIT]="";
 attributes[ParsersAttributes::PASSWORD]="";
 attributes[ParsersAttributes::ENCRYPTED]="";
 attributes[ParsersAttributes::VALIDITY]="";
 attributes[ParsersAttributes::REF_ROLES]="";
 attributes[ParsersAttributes::MEMBER_ROLES]="";
 attributes[ParsersAttributes::ADMIN_ROLES]="";
 attributes[ParsersAttributes::SYSID]="";
 attributes[ParsersAttributes::GROUP]="";
}

void Role::setSysid(unsigned sysid)
{
 /* IDs de usuário abaixo de 100 são reservados ao SGBD, sendo assim
    um erro será gerado */
 if(sysid < 100)
  throw Exception(ERR_ASG_INV_ID_USER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->sysid=sysid;
}

void Role::setOption(unsigned tipo_op, bool valor)
{
 //Caso o tipo de opção seja válido, atribui-se o valor ao mesmo
 if(tipo_op <= OP_ENCRYPTED)
  options[tipo_op]=valor;
 else
  //Dispara-se uma exceção caso se use um tipo de opção inválido
  throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Role::addRole(unsigned tipo_papel, Role *papel)
{
 /* Caso o papel a ser inserido na lista não esteja alocado,
    um erro e disparado */
 if(!papel)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o usuário tente atribuir como membro do papel 'this'
    o mesmo objeto representado por este ultimo */
 else if(papel && this==papel)
  throw Exception(Exception::getErrorMessage(ERR_ROLE_MEMBER_ITSELF)
                               .arg(QString::fromUtf8(papel->getName())),
                ERR_ROLE_MEMBER_ITSELF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  bool papel_ref, papel_mem, papel_adm,
       papel_ref1, papel_mem1, papel_adm1;


  /* Verifica se o papel a ser atribuído ao papel 'this' já esta sendo
     referenciado em uma das listas do papel 'this' */
  papel_ref=this->isRoleExists(REF_ROLE, papel);
  papel_mem=this->isRoleExists(MEMBER_ROLE, papel);
  papel_adm=this->isRoleExists(ADMIN_ROLE, papel);

  /* Verifica se o papel 'this' está sendo referenciado em uma das
     listas do papel vindo do parâmetro */
  papel_ref1=papel->isRoleExists(REF_ROLE, this);
  papel_mem1=papel->isRoleExists(MEMBER_ROLE, this);
  papel_adm1=papel->isRoleExists(ADMIN_ROLE, this);

  /* Erro de duplicação, disparado quando o papel a ser inserido
     já existe na lista do tipo de papel selecionado */
  if((tipo_papel==REF_ROLE && papel_ref) ||
     (tipo_papel==MEMBER_ROLE && (papel_mem || papel_adm)) ||
     (tipo_papel==ADMIN_ROLE && (papel_adm || papel_mem)))
     //Dispara um erro caso o papel já foi inserido anteriormente na lista
   throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
                               .arg(QString::fromUtf8(papel->getName()))
                               .arg(QString::fromUtf8(this->getName())),
                 ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  /* Erros de referência redundante ou seja um papel
     referencia o outro indefinidamente. Uma referência redundante pode acontecer
     quando:
       1) O papel 'this' já faz parte da lista de papeis_refs do papel do parâmetro
          e o usuário tente definir o objeto 'papel' (do parâmetro) como um elemento
          da lista de papeis_refs do papel 'this'

       2) O papel 'this' já faz parte da lista de papeis_membros do papel do parâmetro
          e o usuário tente definir o objeto 'papel' (do parâmetro) como um elemento
          da lista de papeis_membros do papel 'this'

       3) O papel 'this' já faz parte da lista de papeis_admin do papel do parâmetro
          e o usuário tente definir o objeto 'papel' (do parâmetro) como um elemento
          da lista de papeis_admin do papel 'this'

       4) O papel 'papel' já faz parte da lista de papeis_membros ou papeis_admin do papel
          'this' e o usuário tente definir o objeto 'papel' (do parâmetro) como um elemento
          da lista de papeis_refs do papel 'this'

       5) O papel 'papel' já faz parte da lista de papeis_refs do papel
          'this' e o usuário tente definir o objeto 'papel' (do parâmetro) como um elemento
          da lista de papeis_membros ou papeis_adm do papel 'this'
     */
  else if((tipo_papel==REF_ROLE && ((papel_mem || papel_adm) || papel_ref1)) ||
          (tipo_papel==MEMBER_ROLE && ((papel_mem1 || papel_adm1) || papel_ref)) ||
          (tipo_papel==ADMIN_ROLE &&  ((papel_mem1 || papel_adm1) || papel_ref)))
   throw Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
                               .arg(QString::fromUtf8(this->getName()))
                               .arg(QString::fromUtf8(papel->getName())),
                 ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   switch(tipo_papel)
   {
    case MEMBER_ROLE: member_roles.push_back(papel); break;
    case ADMIN_ROLE:  admin_roles.push_back(papel); break;
    case REF_ROLE:
    default:
     ref_roles.push_back(papel);
    break;
   }
  }
 }
}

void Role::setConnectionLimit(int limite)
{
 conn_limit=limite;
}

void Role::setValidity(const QString &data)
{
 validity=data;
}

void Role::setPassword(const QString &senha)
{
 this->password=senha;
}

void Role::setRoleAttribute(unsigned tipo_papel)
{
 QString str_papeis, atrib;
 unsigned i, qtd;
 vector<Role *>  *vet_papeis=NULL;

 switch(tipo_papel)
 {
  case MEMBER_ROLE:
   vet_papeis=&member_roles;
   atrib=ParsersAttributes::MEMBER_ROLES;
  break;
  case ADMIN_ROLE:
   vet_papeis=&admin_roles;
   atrib=ParsersAttributes::ADMIN_ROLES;
  break;
  case REF_ROLE:
  default:
   vet_papeis=&ref_roles;
   atrib=ParsersAttributes::REF_ROLES;
  break;
 }

 qtd=vet_papeis->size();
 for(i=0; i < qtd; i++)
 {
  str_papeis+=vet_papeis->at(i)->getName(true);
  if(i < (qtd-1)) str_papeis+=",";
 }

 attributes[atrib]=str_papeis;
}

void Role::removeRole(unsigned tipo_papel, unsigned idx_papel)
{
 vector<Role *> *lista=NULL;
 vector<Role *>::iterator itr;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case REF_ROLE: lista=&ref_roles; break;
  case MEMBER_ROLE: lista=&member_roles; break;
  case ADMIN_ROLE: lista=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 if(idx_papel >= lista->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista->begin() + idx_papel;
 lista->erase(itr);
}

void Role::removeRoles(unsigned tipo_papel)
{
 vector<Role *> *lista=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case REF_ROLE: lista=&ref_roles; break;
  case MEMBER_ROLE: lista=&member_roles; break;
  case ADMIN_ROLE: lista=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 lista->clear();
}

bool Role::isRoleExists(unsigned tipo_papel, Role *papel)
{
 vector<Role *> *lista=NULL;
 vector<Role *>::iterator itr, itr_end;
 bool enc=false;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case REF_ROLE: lista=&ref_roles; break;
  case MEMBER_ROLE: lista=&member_roles; break;
  case ADMIN_ROLE: lista=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

  /* Varre a lista de papeis selecionada e verifica
    se o papel passado no parâmetro já existe em tal lista */
 itr=lista->begin();
 itr_end=lista->end();
 while(!enc && itr!=itr_end)
 {
  if((*itr)==papel)
    enc=true;
  else
    itr++;
 }

 return(enc);
}

unsigned Role::getSysid(void)
{
 return(sysid);
}

bool Role::getOption(unsigned tipo_op)
{
 if(tipo_op <= OP_ENCRYPTED)
  return(options[tipo_op]);
 else
  throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Role *Role::getRole(unsigned tipo_papel, unsigned idx_papel)
{
 Role *papel=NULL;
 vector<Role *> *lista=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case REF_ROLE: lista=&ref_roles; break;
  case MEMBER_ROLE: lista=&member_roles; break;
  case ADMIN_ROLE: lista=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 /* Caso o índice do papel a ser obtido seja inválido, um
    erro é gerado */
 if(idx_papel > lista->size())
  throw Exception(ERR_REF_ROLE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Obtém o papel na posição especificada
  papel=lista->at(idx_papel);

 return(papel);
}

unsigned Role::getRoleCount(unsigned tipo_papel)
{
 vector<Role *> *lista=NULL;

 switch(tipo_papel)
 {
  case REF_ROLE: lista=&ref_roles; break;
  case MEMBER_ROLE: lista=&member_roles; break;
  case ADMIN_ROLE: lista=&admin_roles; break;
  default:
   //Dispara um erro caso se referencie um tipo inválido de lista de papéis
   throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 //Retorna o tamanho da lista selecionada
 return(lista->size());
}

unsigned Role::getConnectionLimit(void)
{
 return(conn_limit);
}

QString Role::getValidity(void)
{
 return(validity);
}

QString Role::getPassword(void)
{
 return(password);
}

QString Role::getCodeDefinition(unsigned tipo_def)
{
 unsigned i;
 QString atrib_ops[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                       ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                       ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED };

 setRoleAttribute(REF_ROLE);
 setRoleAttribute(MEMBER_ROLE);
 setRoleAttribute(ADMIN_ROLE);

 for(i=0; i < 6; i++)
  attributes[atrib_ops[i]]=(options[i] ? "1" : "");

 attributes[ParsersAttributes::PASSWORD]=password;
 attributes[ParsersAttributes::VALIDITY]=validity;

 /* Configurando um atributo indicando se o papel é um group ou não,
    isso é usado apenas para manter a compatibilidade com o postgresql 8.0
    por ser o único que considera usuário e grupo, as demais versões tratam
    ambos como papeis. Um papel assume a forma de um grupo quando o mesmo
    não possui a opção de login setada */
 attributes[ParsersAttributes::GROUP]=(options[OP_LOGIN] ? "" : "1");

 if(conn_limit >= 0)
  attributes[ParsersAttributes::CONN_LIMIT]=QString("%1").arg(conn_limit);

 attributes[ParsersAttributes::SYSID]=QString("%1").arg(sysid);

 return(BaseObject::__getCodeDefinition(tipo_def));
}

