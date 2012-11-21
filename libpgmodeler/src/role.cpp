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

void Role::setOption(unsigned op_type, bool value)
{
 //Caso o tipo de opção seja válido, atribui-se o valor ao mesmo
 if(op_type <= OP_ENCRYPTED)
  options[op_type]=value;
 else
  //Dispara-se uma exceção caso se use um tipo de opção inválido
  throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Role::addRole(unsigned role_type, Role *role)
{
 /* Caso o papel a ser inserido na lista não esteja alocado,
    um erro e disparado */
 if(!role)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o usuário tente atribuir como membro do papel 'this'
    o mesmo objeto representado por este ultimo */
 else if(role && this==role)
  throw Exception(Exception::getErrorMessage(ERR_ROLE_MEMBER_ITSELF)
                               .arg(QString::fromUtf8(role->getName())),
                ERR_ROLE_MEMBER_ITSELF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  bool role_ref, role_mem, role_adm,
       role_ref1, role_mem1, role_adm1;


  /* Verifica se o papel a ser atribuído ao papel 'this' já esta sendo
     referenciado em uma das listas do papel 'this' */
  role_ref=this->isRoleExists(REF_ROLE, role);
  role_mem=this->isRoleExists(MEMBER_ROLE, role);
  role_adm=this->isRoleExists(ADMIN_ROLE, role);

  /* Verifica se o papel 'this' está sendo referenciado em uma das
     listas do papel vindo do parâmetro */
  role_ref1=role->isRoleExists(REF_ROLE, this);
  role_mem1=role->isRoleExists(MEMBER_ROLE, this);
  role_adm1=role->isRoleExists(ADMIN_ROLE, this);

  /* Erro de duplicação, disparado quando o papel a ser inserido
     já existe na lista do tipo de papel selecionado */
  if((role_type==REF_ROLE && role_ref) ||
     (role_type==MEMBER_ROLE && (role_mem || role_adm)) ||
     (role_type==ADMIN_ROLE && (role_adm || role_mem)))
     //Dispara um erro caso o papel já foi inserido anteriormente na lista
   throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
                               .arg(QString::fromUtf8(role->getName()))
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
  else if((role_type==REF_ROLE && ((role_mem || role_adm) || role_ref1)) ||
          (role_type==MEMBER_ROLE && ((role_mem1 || role_adm1) || role_ref)) ||
          (role_type==ADMIN_ROLE &&  ((role_mem1 || role_adm1) || role_ref)))
   throw Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
                               .arg(QString::fromUtf8(this->getName()))
                               .arg(QString::fromUtf8(role->getName())),
                 ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   switch(role_type)
   {
    case MEMBER_ROLE: member_roles.push_back(role); break;
    case ADMIN_ROLE:  admin_roles.push_back(role); break;
    case REF_ROLE:
    default:
     ref_roles.push_back(role);
    break;
   }
  }
 }
}

void Role::setConnectionLimit(int limit)
{
 conn_limit=limit;
}

void Role::setValidity(const QString &date)
{
 validity=date;
}

void Role::setPassword(const QString &passwd)
{
 this->password=passwd;
}

void Role::setRoleAttribute(unsigned role_type)
{
 QString str_roles, attrib;
 unsigned i, count;
 vector<Role *>  *roles_vect=NULL;

 switch(role_type)
 {
  case MEMBER_ROLE:
   roles_vect=&member_roles;
   attrib=ParsersAttributes::MEMBER_ROLES;
  break;
  case ADMIN_ROLE:
   roles_vect=&admin_roles;
   attrib=ParsersAttributes::ADMIN_ROLES;
  break;
  case REF_ROLE:
  default:
   roles_vect=&ref_roles;
   attrib=ParsersAttributes::REF_ROLES;
  break;
 }

 count=roles_vect->size();
 for(i=0; i < count; i++)
 {
  str_roles+=roles_vect->at(i)->getName(true);
  if(i < (count-1)) str_roles+=",";
 }

 attributes[attrib]=str_roles;
}

void Role::removeRole(unsigned role_type, unsigned role_idx)
{
 vector<Role *> *list=NULL;
 vector<Role *>::iterator itr;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(role_type)
 {
  case REF_ROLE: list=&ref_roles; break;
  case MEMBER_ROLE: list=&member_roles; break;
  case ADMIN_ROLE: list=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 if(role_idx >= list->size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=list->begin() + role_idx;
 list->erase(itr);
}

void Role::removeRoles(unsigned role_type)
{
 vector<Role *> *list=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(role_type)
 {
  case REF_ROLE: list=&ref_roles; break;
  case MEMBER_ROLE: list=&member_roles; break;
  case ADMIN_ROLE: list=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 list->clear();
}

bool Role::isRoleExists(unsigned role_type, Role *role)
{
 vector<Role *> *list=NULL;
 vector<Role *>::iterator itr, itr_end;
 bool found=false;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(role_type)
 {
  case REF_ROLE: list=&ref_roles; break;
  case MEMBER_ROLE: list=&member_roles; break;
  case ADMIN_ROLE: list=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

  /* Varre a lista de papeis selecionada e verifica
    se o papel passado no parâmetro já existe em tal lista */
 itr=list->begin();
 itr_end=list->end();
 while(!found && itr!=itr_end)
 {
  if((*itr)==role)
    found=true;
  else
    itr++;
 }

 return(found);
}

unsigned Role::getSysid(void)
{
 return(sysid);
}

bool Role::getOption(unsigned op_type)
{
 if(op_type <= OP_ENCRYPTED)
  return(options[op_type]);
 else
  throw Exception(ERR_ASG_VAL_INV_ROLE_OPT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Role *Role::getRole(unsigned role_type, unsigned role_idx)
{
 Role *role=NULL;
 vector<Role *> *list=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(role_type)
 {
  case REF_ROLE: list=&ref_roles; break;
  case MEMBER_ROLE: list=&member_roles; break;
  case ADMIN_ROLE: list=&admin_roles; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 /* Caso o índice do papel a ser obtido seja inválido, um
    erro é gerado */
 if(role_idx > list->size())
  throw Exception(ERR_REF_ROLE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Obtém o papel na posição especificada
  role=list->at(role_idx);

 return(role);
}

unsigned Role::getRoleCount(unsigned role_type)
{
 vector<Role *> *list=NULL;

 switch(role_type)
 {
  case REF_ROLE: list=&ref_roles; break;
  case MEMBER_ROLE: list=&member_roles; break;
  case ADMIN_ROLE: list=&admin_roles; break;
  default:
   //Dispara um erro caso se referencie um tipo inválido de lista de papéis
   throw Exception(ERR_REF_INV_ROLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 //Retorna o tamanho da lista selecionada
 return(list->size());
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

QString Role::getCodeDefinition(unsigned def_type)
{
 unsigned i;
 QString op_attribs[]={ ParsersAttributes::SUPERUSER, ParsersAttributes::CREATEDB,
                       ParsersAttributes::CREATEROLE, ParsersAttributes::INHERIT,
                       ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED };

 setRoleAttribute(REF_ROLE);
 setRoleAttribute(MEMBER_ROLE);
 setRoleAttribute(ADMIN_ROLE);

 for(i=0; i < 6; i++)
  attributes[op_attribs[i]]=(options[i] ? "1" : "");

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

 return(BaseObject::__getCodeDefinition(def_type));
}

