#include "papel.h"
//***********************************************************
Papel::Papel(void)
{
 tipo_objeto=OBJETO_PAPEL;
 id_objeto=ObjetoBase::id_papel++;
 sysid=100;

 for(unsigned i=0; i < 6; i++)
  opcoes[i]=false;

 lim_conexao=-1;

 atributos[AtributosParsers::SUPER_USUARIO]="";
 atributos[AtributosParsers::CRIAR_BD]="";
 atributos[AtributosParsers::CRIAR_PAPEL]="";
 atributos[AtributosParsers::HERDA_ATRIBUTOS]="";
 atributos[AtributosParsers::LOGIN]="";
 atributos[AtributosParsers::LIMITE_CONEXAO]="";
 atributos[AtributosParsers::SENHA]="";
 atributos[AtributosParsers::ENCRIPTADA]="";
 atributos[AtributosParsers::VALIDADE]="";
 atributos[AtributosParsers::PAPEIS_REFS]="";
 atributos[AtributosParsers::PAPEIS_MEMBROS]="";
 atributos[AtributosParsers::PAPEIS_ADMINS]="";
 atributos[AtributosParsers::SYSID]="";
 atributos[AtributosParsers::GRUPO]="";
}
//-----------------------------------------------------------
void Papel::definirSysid(unsigned sysid)
{
 /* IDs de usuário abaixo de 100 são reservados ao SGBD, sendo assim
    um erro será gerado */
 if(sysid < 100)
  throw Excecao(ERR_PGMODELER_ATRIDUSUARIOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->sysid=sysid;
}
//-----------------------------------------------------------
void Papel::definirOpcao(unsigned tipo_op, bool valor)
{
 //Caso o tipo de opção seja válido, atribui-se o valor ao mesmo
 if(tipo_op <= OP_ENCRYPTED)
  opcoes[tipo_op]=valor;
 else
  //Dispara-se uma exceção caso se use um tipo de opção inválido
  throw Excecao(ERR_PGMODELER_ATROPPAPELTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
void Papel::definirPapel(unsigned tipo_papel, Papel *papel)
{
 /* Caso o papel a ser inserido na lista não esteja alocado,
    um erro e disparado */
 if(!papel)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o usuário tente atribuir como membro do papel 'this'
    o mesmo objeto representado por este ultimo */
 else if(papel && this==papel)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRMEBROPROPPAPEL)
                               .arg(QString::fromUtf8(papel->obterNome())),
                ERR_PGMODELER_ATRMEBROPROPPAPEL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  bool papel_ref, papel_mem, papel_adm,
       papel_ref1, papel_mem1, papel_adm1;


  /* Verifica se o papel a ser atribuído ao papel 'this' já esta sendo
     referenciado em uma das listas do papel 'this' */
  papel_ref=this->papelExiste(PAPEL_REF, papel);
  papel_mem=this->papelExiste(PAPEL_MEMBRO, papel);
  papel_adm=this->papelExiste(PAPEL_ADMIN, papel);

  /* Verifica se o papel 'this' está sendo referenciado em uma das
     listas do papel vindo do parâmetro */
  papel_ref1=papel->papelExiste(PAPEL_REF, this);
  papel_mem1=papel->papelExiste(PAPEL_MEMBRO, this);
  papel_adm1=papel->papelExiste(PAPEL_ADMIN, this);

  /* Erro de duplicação, disparado quando o papel a ser inserido
     já existe na lista do tipo de papel selecionado */
  if((tipo_papel==PAPEL_REF && papel_ref) ||
     (tipo_papel==PAPEL_MEMBRO && (papel_mem || papel_adm)) ||
     (tipo_papel==PAPEL_ADMIN && (papel_adm || papel_mem)))
     //Dispara um erro caso o papel já foi inserido anteriormente na lista
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_INSITEMPAPELDUPLIC)
                               .arg(QString::fromUtf8(papel->obterNome()))
                               .arg(QString::fromUtf8(this->obterNome())),
                 ERR_PGMODELER_INSITEMPAPELDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
  else if((tipo_papel==PAPEL_REF && ((papel_mem || papel_adm) || papel_ref1)) ||
          (tipo_papel==PAPEL_MEMBRO && ((papel_mem1 || papel_adm1) || papel_ref)) ||
          (tipo_papel==PAPEL_ADMIN &&  ((papel_mem1 || papel_adm1) || papel_ref)))
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFREDUNDANTEPAPEIS)
                               .arg(QString::fromUtf8(this->obterNome()))
                               .arg(QString::fromUtf8(papel->obterNome())),
                 ERR_PGMODELER_REFREDUNDANTEPAPEIS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   switch(tipo_papel)
   {
    case PAPEL_MEMBRO: papeis_membros.push_back(papel); break;
    case PAPEL_ADMIN:  papeis_admins.push_back(papel); break;
    case PAPEL_REF:
    default:
     papeis_refs.push_back(papel);
    break;
   }
  }
 }
}
//-----------------------------------------------------------
void Papel::definirLimiteConexao(int limite)
{
 lim_conexao=limite;
}
//-----------------------------------------------------------
void Papel::definirValidade(const QString &data)
{
 validade=data;
}
//-----------------------------------------------------------
void Papel::definirSenha(const QString &senha)
{
 this->senha=senha;
}
//-----------------------------------------------------------
void Papel::definirAtributoPapel(unsigned tipo_papel)
{
 QString str_papeis, atrib;
 unsigned i, qtd;
 vector<Papel *>  *vet_papeis=NULL;

 switch(tipo_papel)
 {
  case PAPEL_MEMBRO:
   vet_papeis=&papeis_membros;
   atrib=AtributosParsers::PAPEIS_MEMBROS;
  break;
  case PAPEL_ADMIN:
   vet_papeis=&papeis_admins;
   atrib=AtributosParsers::PAPEIS_ADMINS;
  break;
  case PAPEL_REF:
  default:
   vet_papeis=&papeis_refs;
   atrib=AtributosParsers::PAPEIS_REFS;
  break;
 }

 qtd=vet_papeis->size();
 for(i=0; i < qtd; i++)
 {
  str_papeis+=vet_papeis->at(i)->obterNome(true);
  if(i < (qtd-1)) str_papeis+=",";
 }

 atributos[atrib]=str_papeis;
}
//-----------------------------------------------------------
void Papel::removerPapel(unsigned tipo_papel, unsigned idx_papel)
{
 vector<Papel *> *lista=NULL;
 vector<Papel *>::iterator itr;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case PAPEL_REF: lista=&papeis_refs; break;
  case PAPEL_MEMBRO: lista=&papeis_membros; break;
  case PAPEL_ADMIN: lista=&papeis_admins; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Excecao(ERR_PGMODELER_REFPAPELINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 if(idx_papel >= lista->size())
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista->begin() + idx_papel;
 lista->erase(itr);
}
//-----------------------------------------------------------
void Papel::removerPapeis(unsigned tipo_papel)
{
 vector<Papel *> *lista=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case PAPEL_REF: lista=&papeis_refs; break;
  case PAPEL_MEMBRO: lista=&papeis_membros; break;
  case PAPEL_ADMIN: lista=&papeis_admins; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Excecao(ERR_PGMODELER_REFPAPELINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 lista->clear();
}
//-----------------------------------------------------------
bool Papel::papelExiste(unsigned tipo_papel, Papel *papel)
{
 vector<Papel *> *lista=NULL;
 vector<Papel *>::iterator itr, itr_end;
 bool enc=false;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case PAPEL_REF: lista=&papeis_refs; break;
  case PAPEL_MEMBRO: lista=&papeis_membros; break;
  case PAPEL_ADMIN: lista=&papeis_admins; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Excecao(ERR_PGMODELER_REFPAPELINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
//-----------------------------------------------------------
unsigned Papel::obterSysid(void)
{
 return(sysid);
}
//-----------------------------------------------------------
bool Papel::obterOpcao(unsigned tipo_op)
{
 if(tipo_op <= OP_ENCRYPTED)
  return(opcoes[tipo_op]);
 else
  throw Excecao(ERR_PGMODELER_ATROPPAPELTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
Papel *Papel::obterPapel(unsigned tipo_papel, unsigned idx_papel)
{
 Papel *papel=NULL;
 vector<Papel *> *lista=NULL;

 //Selecionando a lista de papéis de acordo com o tipo passado
 switch(tipo_papel)
 {
  case PAPEL_REF: lista=&papeis_refs; break;
  case PAPEL_MEMBRO: lista=&papeis_membros; break;
  case PAPEL_ADMIN: lista=&papeis_admins; break;
  default:
    //Dispara um erro caso se referencie um tipo inválido de lista de papéis
    throw Excecao(ERR_PGMODELER_REFPAPELINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 /* Caso o índice do papel a ser obtido seja inválido, um
    erro é gerado */
 if(idx_papel > lista->size())
  throw Excecao(ERR_PGMODELER_REFPAPELIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Obtém o papel na posição especificada
  papel=lista->at(idx_papel);

 return(papel);
}
//-----------------------------------------------------------
unsigned Papel::obterNumPapeis(unsigned tipo_papel)
{
 vector<Papel *> *lista=NULL;

 switch(tipo_papel)
 {
  case PAPEL_REF: lista=&papeis_refs; break;
  case PAPEL_MEMBRO: lista=&papeis_membros; break;
  case PAPEL_ADMIN: lista=&papeis_admins; break;
  default:
   //Dispara um erro caso se referencie um tipo inválido de lista de papéis
   throw Excecao(ERR_PGMODELER_REFPAPELINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  break;
 }

 //Retorna o tamanho da lista selecionada
 return(lista->size());
}
//-----------------------------------------------------------
unsigned Papel::obterLimiteConexao(void)
{
 return(lim_conexao);
}
//-----------------------------------------------------------
QString Papel::obterValidade(void)
{
 return(validade);
}
//-----------------------------------------------------------
QString Papel::obterSenha(void)
{
 return(senha);
}
//-----------------------------------------------------------
QString Papel::obterDefinicaoObjeto(unsigned tipo_def)
{
 unsigned i;
 QString atrib_ops[]={ AtributosParsers::SUPER_USUARIO, AtributosParsers::CRIAR_BD,
                       AtributosParsers::CRIAR_PAPEL, AtributosParsers::HERDA_ATRIBUTOS,
                       AtributosParsers::LOGIN, AtributosParsers::ENCRIPTADA };

 definirAtributoPapel(PAPEL_REF);
 definirAtributoPapel(PAPEL_MEMBRO);
 definirAtributoPapel(PAPEL_ADMIN);

 for(i=0; i < 6; i++)
  atributos[atrib_ops[i]]=(opcoes[i] ? "1" : "");

 atributos[AtributosParsers::SENHA]=senha;
 atributos[AtributosParsers::VALIDADE]=validade;

 /* Configurando um atributo indicando se o papel é um group ou não,
    isso é usado apenas para manter a compatibilidade com o postgresql 8.0
    por ser o único que considera usuário e grupo, as demais versões tratam
    ambos como papeis. Um papel assume a forma de um grupo quando o mesmo
    não possui a opção de login setada */
 atributos[AtributosParsers::GRUPO]=(opcoes[OP_LOGIN] ? "" : "1");

 if(lim_conexao >= 0)
  atributos[AtributosParsers::LIMITE_CONEXAO]=QString("%1").arg(lim_conexao);

 atributos[AtributosParsers::SYSID]=QString("%1").arg(sysid);

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
