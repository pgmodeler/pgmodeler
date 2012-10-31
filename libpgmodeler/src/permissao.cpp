#include "permissao.h"

Permissao::Permissao(ObjetoBase *objeto)
{
 TipoObjetoBase tipo_obj;
 unsigned id_priv;

 //Inicializa todos os privilégios como desmarcados
 for(id_priv=PRIV_SELECT; id_priv<=PRIV_USAGE; id_priv++)
  privilegios[id_priv]=op_concessao[id_priv]=false;

 //Caso o usuário tente atribuir um objeto não alocd �  permissão
 if(!objeto)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o tipo do objeto
 tipo_obj=objeto->obterTipoObjeto();

 /* Caso o tipo do objeto a ser atribuído não seja válido de acordo com a regra
    (vide definição da Classe) dispara uma exceção */
 if(tipo_obj!=OBJETO_TABELA && tipo_obj!=OBJETO_COLUNA && tipo_obj!=OBJETO_VISAO &&
    tipo_obj!=OBJETO_SEQUENCIA && tipo_obj!=OBJETO_BANCO_DADOS && tipo_obj!=OBJETO_FUNCAO &&
    tipo_obj!=OBJETO_FUNC_AGREGACAO && tipo_obj!=OBJETO_LINGUAGEM && tipo_obj!=OBJETO_ESQUEMA &&
    tipo_obj!=OBJETO_ESPACO_TABELA)
  throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui o objeto   permissão
 this->objeto=objeto;
 this->tipo_objeto=OBJETO_PERMISSAO;

 atributos[ParsersAttributes::OBJECT]="";
 atributos[ParsersAttributes::TYPE]="";
 atributos[ParsersAttributes::PARENT]="";
 atributos[ParsersAttributes::GRANT_OP]="";
 atributos[ParsersAttributes::ROLES]="";
 atributos[ParsersAttributes::PRIVILEGES]="";
 atributos[ParsersAttributes::PRIVILEGES_GOP]="";
}

bool Permissao::papelReferenciado(Papel *papel)
{
 vector<Papel *>::iterator itr, itr_end;
 bool enc=false;

 //Verifica a existencia do papel na lista de papeis já relacionado�  permissão
 itr=papeis.begin();
 itr_end=papeis.end();

 while(itr!=itr_end && !enc)
 {
  enc=(*itr)==papel;
  itr++;
 }

 return(enc);
}

void Permissao::adicionarPapel(Papel *papel)
{
 //Caso o usuário tente atribuir um papel não alocado um erro será disparado
 if(!papel)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o papel já esteja inserido em tal lista um erro será disparado
 if(papelReferenciado(papel))
   throw Exception(ERR_INS_DUP_ROLE_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Adiciona o papel   lista de papeis da permissão
 papeis.push_back(papel);
 gerarIdPermissao();
}

void Permissao::definirPrivilegio(unsigned privilegio, bool valor, bool op_concessao)
{
 TipoObjetoBase tipo_obj;

 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(privilegio > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 tipo_obj=objeto->obterTipoObjeto();

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
 if((tipo_obj==OBJETO_TABELA && privilegio!=PRIV_SELECT && privilegio!=PRIV_INSERT &&
                                privilegio!=PRIV_UPDATE && privilegio!=PRIV_DELETE &&
                                privilegio!=PRIV_TRUNCATE && privilegio!=PRIV_REFERENCES &&
                                privilegio!=PRIV_TRIGGER) ||
    //Validando o privilégio em relação ao tipo de objeto Coluna
    (tipo_obj==OBJETO_COLUNA && privilegio!=PRIV_SELECT && privilegio!=PRIV_INSERT &&
                                privilegio!=PRIV_UPDATE && privilegio!=PRIV_REFERENCES) ||
    //Validando o privilégio em relação ao tipo de objeto Sequencia
    (tipo_obj==OBJETO_SEQUENCIA && privilegio!=PRIV_USAGE && privilegio!=PRIV_SELECT &&
                                   privilegio!=PRIV_UPDATE) ||
    //Validando o privilégio em relação ao tipo de objeto Banco de Dados
    (tipo_obj==OBJETO_BANCO_DADOS && privilegio!=PRIV_CREATE && privilegio!=PRIV_CONNECT &&
                                     privilegio!=PRIV_TEMPORARY) ||
    //Validando o privilégio em relação ao tipo de objeto Função (de Agregação)
    ((tipo_obj==OBJETO_FUNCAO || tipo_obj==OBJETO_FUNC_AGREGACAO) && privilegio!=PRIV_EXECUTE) ||
    //Validando o privilégio em relação ao tipo de objeto Linguagem
    (tipo_obj==OBJETO_LINGUAGEM && privilegio!=PRIV_USAGE) ||
    //Validando o privilégio em relação ao tipo de objeto Esquema
    (tipo_obj==OBJETO_ESQUEMA && privilegio!=PRIV_USAGE && privilegio!=PRIV_CREATE) ||
    //Validando o privilégio em relação ao tipo de objeto Espaço de Tabela
    (tipo_obj==OBJETO_ESPACO_TABELA && privilegio!=PRIV_CREATE) ||
    //Validando o privilégio em relação ao tipo de objeto Visão
    (tipo_obj==OBJETO_VISAO && privilegio!=PRIV_SELECT))
   /* Caso o privilégio a ser atribuído ao objeto seja incompatível com seu tipo
      um erro será retornado ao usuário */
   throw Exception(ERR_ASG_INCOMP_PRIV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Marca o privilégio com o valor passado
 privilegios[privilegio]=valor;
 this->op_concessao[privilegio]=op_concessao;
}

void Permissao::removerPapel(unsigned idx_papel)
{
 if(idx_papel > papeis.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 gerarIdPermissao();
}

void Permissao::removerPapeis(void)
{
 papeis.clear();
 gerarIdPermissao();
}

Papel *Permissao::obterPapel(unsigned idx_papel)
{
 if(idx_papel > papeis.size())
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(papeis[idx_papel]);
}

unsigned Permissao::obterNumPapeis(void)
{
 return(papeis.size());
}

ObjetoBase *Permissao::obterObjeto(void)
{
 return(objeto);
}

bool Permissao::obterPrivilegio(unsigned privilegio)
{
 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(privilegio > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(privilegios[privilegio]);
}

bool Permissao::obterOpConcessao(unsigned privilegio)
{
 //Caso o tipo de privilégio sejá inválido dispara uma exceção
 if(privilegio > PRIV_USAGE)
  throw Exception(ERR_REF_INV_PRIVILEGE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(op_concessao[privilegio]);
}

QString Permissao::obterStringPrivilegios(void)
{
 unsigned char cod_privilegios[13]="rawdDxtCcTXU";
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
  if(privilegios[i])
   str_priv.append(cod_privilegios[i]);

  if(op_concessao[i])
  str_priv.append(QChar('*'));
 }

 return(str_priv);
}

void Permissao::gerarIdPermissao(void)
{
 vector<Papel *>::iterator itr, itr_end;
 vector<QString> vet_end;
 Papel *papel=NULL;
 QString str_aux;
 QString endereco;
 unsigned i, qtd;
 QTextStream stream(&endereco);

 //Caso haja algum papel associado   permissão
 if(papeis.size() > 0)
 {
  itr=papeis.begin();
  itr_end=papeis.end();

  /* Converte o endereço dos mesmos em string pois este é um
     identificador único de cada papel */
  while(itr!=itr_end)
  {
   papel=(*itr);
   //Converte o endereço
   //endereco.sprintf("%x",reinterpret_cast<unsigned *>(papel));
   stream << reinterpret_cast<unsigned *>(papel);
   //Armazena-o em um vetor
   vet_end.push_back(endereco.mid(2));
   itr++;
  }

  //Ordena os endereços obtido
  sort(vet_end.begin(), vet_end.end());
  qtd=vet_end.size();

  //Concatena os endereços ordenados separando-os por '.'
  for(i=0; i < qtd; i++)
  {
   str_aux+=QString("%1").arg(vet_end[i]);
   if(i < qtd-1) str_aux+=".";
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
 this->nome=QString(ParsersAttributes::PERMISSION + "_%1.%2")
            .arg(objeto->obterIdObjeto())
            .arg(str_aux);
}

QString Permissao::obterDefinicaoObjeto(unsigned tipo_def)
{
 unsigned i, qtd;
 TipoObjetoBase tipo_obj;
 QString vet_priv[12]={ ParsersAttributes::SELECT_PRIV, ParsersAttributes::INSERT_PRIV,
                        ParsersAttributes::UPDATE_PRIV, ParsersAttributes::DELETE_PRIV,
                        ParsersAttributes::TRUNCATE_PRIV, ParsersAttributes::REFERENCES_PRIV,
                        ParsersAttributes::TRIGGER_PRIV, ParsersAttributes::CREATE_PRIV,
                        ParsersAttributes::CONNECT_PRIV, ParsersAttributes::TEMPORARY_PRIV,
                        ParsersAttributes::EXECUTE_PRIV, ParsersAttributes::USAGE_PRIV };

 tipo_obj=objeto->obterTipoObjeto();

 if(tipo_obj==OBJETO_FUNCAO)
  atributos[ParsersAttributes::OBJECT]=dynamic_cast<Funcao *>(objeto)->obterAssinatura();
 else
  atributos[ParsersAttributes::OBJECT]=objeto->obterNome(true);

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  atributos[ParsersAttributes::TYPE]=ObjetoBase::obterNomeSQLObjeto(objeto->obterTipoObjeto());
 else
  atributos[ParsersAttributes::TYPE]=ObjetoBase::obterNomeEsquemaObjeto(objeto->obterTipoObjeto());

 if(tipo_obj==OBJETO_COLUNA)
  atributos[ParsersAttributes::PARENT]=dynamic_cast<Coluna *>(objeto)->obterTabelaPai()->obterNome(true);

 if(tipo_def==SchemaParser::XML_DEFINITION)
 {
  //Marca os privilégios que estão setados atribuídos ao objeto
  for(i=0; i < 12; i++)
  {
   if(privilegios[i] && op_concessao[i])
    atributos[vet_priv[i]]=ParsersAttributes::GRANT_OP;
   else if(privilegios[i])
    atributos[vet_priv[i]]=ParsersAttributes::_TRUE_;
   else
    atributos[vet_priv[i]]="";
  }
 }
 else
 {
  /* Concatena os nomes dos privilégios para que os mesmos sejam incluídos
     na SQL que cria a permissão em BD */
  for(i=0; i < 12; i++)
  {
   if(privilegios[i] && !op_concessao[i])
    atributos[ParsersAttributes::PRIVILEGES]+=vet_priv[i].toUpper() + ",";
   else if(op_concessao[i])
    atributos[ParsersAttributes::PRIVILEGES_GOP]+=vet_priv[i].toUpper() + ",";
  }

  atributos[ParsersAttributes::PRIVILEGES].remove(atributos[ParsersAttributes::PRIVILEGES].size()-1,1);
  atributos[ParsersAttributes::PRIVILEGES_GOP].remove(atributos[ParsersAttributes::PRIVILEGES_GOP].size()-1,1);
 }

 qtd=papeis.size();

 /* Concatena os nomes dos papéis que participam da
    permissão separando-os por vírgula */
 for(i=0; i < qtd; i++)
 {
  atributos[ParsersAttributes::ROLES]+=papeis[i]->obterNome(true) + ",";
 }

 atributos[ParsersAttributes::ROLES].remove(atributos[ParsersAttributes::ROLES].size()-1,1);

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}

