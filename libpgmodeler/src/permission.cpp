#include "permission.h"

Permission::Permission(BaseObject *obj)
{
 ObjectType obj_type;
 unsigned priv_id;

 //Inicializa todos os privilégios como desmarcados
 for(priv_id=PRIV_SELECT; priv_id<=PRIV_USAGE; priv_id++)
  privileges[priv_id]=grant_option[priv_id]=false;

 //Caso o usuário tente atribuir um objeto não alocd   permissão
 if(!obj)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o tipo do objeto
 obj_type=obj->getObjectType();

 /* Caso o tipo do objeto a ser atribuído não seja válido de acordo com a regra
    (vide definição da Classe) dispara uma exceção */
 if(obj_type!=OBJ_TABLE && obj_type!=OBJ_COLUMN && obj_type!=OBJ_VIEW &&
    obj_type!=OBJ_SEQUENCE && obj_type!=OBJ_DATABASE && obj_type!=OBJ_FUNCTION &&
    obj_type!=OBJ_AGGREGATE && obj_type!=OBJ_LANGUAGE && obj_type!=OBJ_SCHEMA &&
    obj_type!=OBJ_TABLESPACE)
  throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui o objeto   permissão
 this->object=obj;
 this->obj_type=OBJ_PERMISSION;

 attributes[ParsersAttributes::OBJECT]="";
 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::PARENT]="";
 attributes[ParsersAttributes::GRANT_OP]="";
 attributes[ParsersAttributes::ROLES]="";
 attributes[ParsersAttributes::PRIVILEGES]="";
 attributes[ParsersAttributes::PRIVILEGES_GOP]="";
}

bool Permission::isRoleReferenced(Role *role)
{
 vector<Role *>::iterator itr, itr_end;
 bool found=false;

 //Verifica a existencia do papel na lista de papeis já relacionado  permissão
 itr=roles.begin();
 itr_end=roles.end();

 while(itr!=itr_end && !found)
 {
  found=(*itr)==role;
  itr++;
 }

 return(found);
}

void Permission::addRole(Role *role)
{
 //Caso o usuário tente atribuir um papel não alocado um erro será disparado
 if(!role)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o papel já esteja inserido em tal lista um erro será disparado
 if(isRoleReferenced(role))
   throw Exception(ERR_INS_DUP_ROLE_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Adiciona o papel   lista de papeis da permissão
 roles.push_back(role);
 generatePermissionId();
}

void Permission::setPrivilege(unsigned priv_id, bool value, bool grant_op)
{
 ObjectType obj_type;

 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(priv_id > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 obj_type=object->getObjectType();

 /* Alguns privilégios são válidos apenas para certos tipos
    de objetos. Caso o usuário tente atribuir um privilégio P
    para um objeto que não aceita este privilégio o mesmo
    será ignorado. O esquema abaixo mostra os privilégios para
    cada objeto:

    Tabela:   SELECT | INSERT | UPDATE | DELETE | TRUNCATE | REFERENCES | TRIGGER
    Coluna:   SELECT | INSERT | UPDATE | REFERENCES
    Sequência:  USAGE | SELECT | UPDATE
    Banco de Dados: CREATE | CONNECT | TEMPORARY | TEMP
    Função: EXECUTE
    Função Agregada: EXECUTE
    Linguagem: USAGE
    Esquema: CREATE | USAGE
    Espaço de Tabela: CREATE
    Visão: SELECT
 */
    //Validando o privilégio em relação ao tipo de objeto Tabela
 if((obj_type==OBJ_TABLE && priv_id!=PRIV_SELECT && priv_id!=PRIV_INSERT &&
                                priv_id!=PRIV_UPDATE && priv_id!=PRIV_DELETE &&
                                priv_id!=PRIV_TRUNCATE && priv_id!=PRIV_REFERENCES &&
                                priv_id!=PRIV_TRIGGER) ||
    //Validando o privilégio em relação ao tipo de objeto Coluna
    (obj_type==OBJ_COLUMN && priv_id!=PRIV_SELECT && priv_id!=PRIV_INSERT &&
                                priv_id!=PRIV_UPDATE && priv_id!=PRIV_REFERENCES) ||
    //Validando o privilégio em relação ao tipo de objeto Sequencia
    (obj_type==OBJ_SEQUENCE && priv_id!=PRIV_USAGE && priv_id!=PRIV_SELECT &&
                                   priv_id!=PRIV_UPDATE) ||
    //Validando o privilégio em relação ao tipo de objeto Banco de Dados
    (obj_type==OBJ_DATABASE && priv_id!=PRIV_CREATE && priv_id!=PRIV_CONNECT &&
                                     priv_id!=PRIV_TEMPORARY) ||
    //Validando o privilégio em relação ao tipo de objeto Função (de Agregação)
    ((obj_type==OBJ_FUNCTION || obj_type==OBJ_AGGREGATE) && priv_id!=PRIV_EXECUTE) ||
    //Validando o privilégio em relação ao tipo de objeto Linguagem
    (obj_type==OBJ_LANGUAGE && priv_id!=PRIV_USAGE) ||
    //Validando o privilégio em relação ao tipo de objeto Esquema
    (obj_type==OBJ_SCHEMA && priv_id!=PRIV_USAGE && priv_id!=PRIV_CREATE) ||
    //Validando o privilégio em relação ao tipo de objeto Espaço de Tabela
    (obj_type==OBJ_TABLESPACE && priv_id!=PRIV_CREATE) ||
    //Validando o privilégio em relação ao tipo de objeto Visão
    (obj_type==OBJ_VIEW && priv_id!=PRIV_SELECT))
   /* Caso o privilégio a ser atribuído ao objeto seja incompatível com seu tipo
      um erro será retornado ao usuário */
   throw Exception(ERR_ASG_INCOMP_PRIV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Marca o privilégio com o valor passado
 privileges[priv_id]=value;
 this->grant_option[priv_id]=grant_op;
}

void Permission::removeRole(unsigned role_idx)
{
 if(role_idx > roles.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 generatePermissionId();
}

void Permission::removeRoles(void)
{
 roles.clear();
 generatePermissionId();
}

Role *Permission::getRole(unsigned role_idx)
{
 if(role_idx > roles.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(roles[role_idx]);
}

unsigned Permission::getRoleCount(void)
{
 return(roles.size());
}

BaseObject *Permission::getObject(void)
{
 return(object);
}

bool Permission::getPrivilege(unsigned priv_id)
{
 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(priv_id > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(privileges[priv_id]);
}

bool Permission::getGrantOption(unsigned priv_id)
{
 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(priv_id > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(grant_option[priv_id]);
}

QString Permission::getPrivilegeString(void)
{
 unsigned char priv_codes[13]="rawdDxtCcTXU";
 QString str_priv;
 unsigned i;

 /* Montagem da string de acordo com a documentação:

    rolename=xxxx -- privileges granted to a role
            =xxxx -- privileges granted to PUBLIC

            r -- SELECT ("read")
            a -- INSERT ("append")
            w -- UPDATE ("write")
            d -- DELETE
            D -- TRUNCATE
            x -- REFERENCES
            t -- TRIGGER
            C -- CREATE
            c -- CONNECT
            T -- TEMPORARY
            X -- EXECUTE
            U -- USAGE

      arwdDxt -- ALL PRIVILEGES (for tables, varies for other objects)
            * -- grant option for preceding privilege
        /yyyy -- role that granted this privilege  */
 for(i=0; i < 12; i++)
 {
  if(privileges[i])
   str_priv.append(priv_codes[i]);

  if(grant_option[i])
  str_priv.append(QChar('*'));
 }

 return(str_priv);
}

void Permission::generatePermissionId(void)
{
 vector<Role *>::iterator itr, itr_end;
 vector<QString> addr_vect;
 Role *role=NULL;
 QString str_aux;
 QString addr;
 unsigned i, count;
 QTextStream stream(&addr);

 //Caso haja algum papel associado   permissão
 if(roles.size() > 0)
 {
  itr=roles.begin();
  itr_end=roles.end();

  /* Converte o endereço dos mesmos em string pois este é um
     identificador único de cada papel */
  while(itr!=itr_end)
  {
   role=(*itr);
   //Converte o endereço
   stream << reinterpret_cast<unsigned *>(role);
   //Armazena-o em um vetor
   addr_vect.push_back(addr.mid(2));
   itr++;
  }

  //Ordena os endereços obtido
  sort(addr_vect.begin(), addr_vect.end());
  count=addr_vect.size();

  //Concatena os endereços ordenados separando-os por '.'
  for(i=0; i < count; i++)
  {
   str_aux+=QString("%1").arg(addr_vect[i]);
   if(i < count-1) str_aux+=".";
  }

 }
 /* Caso nenhum papel esteja associado a permissão (public)
    gera um identificador com zeros indicando que a permissão
    não está ligada diretamente a nenhum papel do modelo */
 else
   str_aux="000000";

 /* Configura o nome da permissão da seguinte forma:
    grant_[ID_OBJETO]_([END_PAPEL1].[END_PAPELN])

    Com esse formato de nome é possivel criar um identificador
    interno para a permissão e assim
    podendo gerar erros quando o usuário tenta criar uma permissão
    com o mesmo conjunto de papéis relacionados ao objeto */
 this->obj_name=QString(ParsersAttributes::PERMISSION + "_%1.%2")
            .arg(object->getObjectId())
            .arg(str_aux);
}

QString Permission::getCodeDefinition(unsigned def_type)
{
 unsigned i, count;
 ObjectType obj_type;
 QString priv_vect[12]={ ParsersAttributes::SELECT_PRIV, ParsersAttributes::INSERT_PRIV,
                         ParsersAttributes::UPDATE_PRIV, ParsersAttributes::DELETE_PRIV,
                         ParsersAttributes::TRUNCATE_PRIV, ParsersAttributes::REFERENCES_PRIV,
                         ParsersAttributes::TRIGGER_PRIV, ParsersAttributes::CREATE_PRIV,
                         ParsersAttributes::CONNECT_PRIV, ParsersAttributes::TEMPORARY_PRIV,
                         ParsersAttributes::EXECUTE_PRIV, ParsersAttributes::USAGE_PRIV };

 obj_type=object->getObjectType();

 if(obj_type==OBJ_FUNCTION)
  attributes[ParsersAttributes::OBJECT]=dynamic_cast<Function *>(object)->getSignature();
 else
  attributes[ParsersAttributes::OBJECT]=object->getName(true);

 if(def_type==SchemaParser::SQL_DEFINITION)
  attributes[ParsersAttributes::TYPE]=BaseObject::getSQLName(object->getObjectType());
 else
  attributes[ParsersAttributes::TYPE]=BaseObject::getSchemaName(object->getObjectType());

 if(obj_type==OBJ_COLUMN)
  attributes[ParsersAttributes::PARENT]=dynamic_cast<Column *>(object)->getParentTable()->getName(true);

 if(def_type==SchemaParser::XML_DEFINITION)
 {
  //Marca os privilégios que estão setados atribuídos ao objeto
  for(i=0; i < 12; i++)
  {
   if(privileges[i] && grant_option[i])
    attributes[priv_vect[i]]=ParsersAttributes::GRANT_OP;
   else if(privileges[i])
    attributes[priv_vect[i]]=ParsersAttributes::_TRUE_;
   else
    attributes[priv_vect[i]]="";
  }
 }
 else
 {
  /* Concatena os nomes dos privilégios para que os mesmos sejam incluídos
     na SQL que cria a permissão em BD */
  for(i=0; i < 12; i++)
  {
   if(privileges[i] && !grant_option[i])
    attributes[ParsersAttributes::PRIVILEGES]+=priv_vect[i].toUpper() + ",";
   else if(grant_option[i])
    attributes[ParsersAttributes::PRIVILEGES_GOP]+=priv_vect[i].toUpper() + ",";
  }

  attributes[ParsersAttributes::PRIVILEGES].remove(attributes[ParsersAttributes::PRIVILEGES].size()-1,1);
  attributes[ParsersAttributes::PRIVILEGES_GOP].remove(attributes[ParsersAttributes::PRIVILEGES_GOP].size()-1,1);
 }

 count=roles.size();

 /* Concatena os nomes dos papéis que participam da
    permissão separando-os por vírgula */
 for(i=0; i < count; i++)
 {
  attributes[ParsersAttributes::ROLES]+=roles[i]->getName(true) + ",";
 }

 attributes[ParsersAttributes::ROLES].remove(attributes[ParsersAttributes::ROLES].size()-1,1);

 return(BaseObject::__getCodeDefinition(def_type));
}

