#include "tabela.h"

Tabela::Tabela(void) : BaseTable()
{
 obj_type=OBJ_TABLE;
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::CONSTRAINTS]="";
 attributes[ParsersAttributes::INDEXES]="";
 attributes[ParsersAttributes::TRIGGERS]="";
 attributes[ParsersAttributes::RULES]="";
 attributes[ParsersAttributes::OIDS]="";
 attributes[ParsersAttributes::COLS_COMMENT]="";
 this->setName(trUtf8("new_table").toUtf8());
}

Tabela::~Tabela(void)
{
 ObjectType tipos[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE,
                          OBJ_CONSTRAINT, OBJ_COLUMN };
 vector<TableObject *> *lista=NULL;

 for(unsigned i=0; i < 5; i++)
 {
  lista=obterListaObjetos(tipos[i]);
  while(!lista->empty())
  {
   delete(lista->back());
   lista->pop_back();
  }
 }

 tabelas_copia.clear();
 tabelas_pai.clear();
}

void Tabela::setName(const QString &nome)
{
 QString nome_ant=this->getName(true);
 BaseObject::setName(nome);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

void Tabela::setSchema(BaseObject *esquema)
{
 QString nome_ant=this->getName(true);

 //Atribui o esquema   tabela
 BaseObject::setSchema(esquema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

void Tabela::definirAceitaOids(bool valor)
{
 aceita_oids=valor;
}

void Tabela::setProtected(bool protegido)
{
 ObjectType tipos_obj[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
                              OBJ_INDEX, OBJ_RULE, OBJ_TRIGGER };
 unsigned i;
 vector<TableObject *>::iterator itr, itr_end;
 vector<TableObject *> *lista=NULL;
 TableObject *obj_tab=NULL;

 for(i=0; i < 5; i++)
 {
  //Obtém a lista de objetos
  lista=obterListaObjetos(tipos_obj[i]);
  itr=lista->begin();
  itr_end=lista->end();

  //Varre a lista protegendo/desprotegendo os elementos
  while(itr!=itr_end)
  {
   //Obtém um objeto da lista
   obj_tab=(*itr);
   /* Só protegerá o objeto caso o mesmo não foi incluído
      por relacionamento pois quando é o caso os mesmos
      já vem protegidos */
   if(!obj_tab->isAddedByRelationship())
    obj_tab->setProtected(protegido);
   itr++;
  }
 }

 BaseGraphicObject::setProtected(protegido);
}

void Tabela::definirAtributoComentario(TableObject *obj_tab)
{
 if(obj_tab && !obj_tab->getComment().isEmpty())
 {
  map<QString, QString> attribs;

  attribs[ParsersAttributes::DIF_SQL]="1";
  attribs[ParsersAttributes::SQL_OBJECT]=obj_tab->getSQLName();
  attribs[ParsersAttributes::COLUMN]=(obj_tab->getObjectType()==OBJ_COLUMN ? "1" : "");
  attribs[ParsersAttributes::CONSTRAINT]=(obj_tab->getObjectType()==OBJ_CONSTRAINT ? "1" : "");
  attribs[ParsersAttributes::TABLE]=this->getName(true);
  attribs[ParsersAttributes::NAME]=obj_tab->getName(true);
  attribs[ParsersAttributes::COMMENT]=obj_tab->getComment();

  SchemaParser::setIgnoreUnkownAttributes(true);
  attributes[ParsersAttributes::COLS_COMMENT]+=SchemaParser::getObjectDefinition(ParsersAttributes::COMMENT, attribs,
                                                                                 SchemaParser::SQL_DEFINITION);
  SchemaParser::setIgnoreUnkownAttributes(false);
 }
}

void Tabela::definirAtributoColunas(unsigned tipo_def)
{
 QString str_cols;
 unsigned i, qtd;

 //Concatena a definiação SQL das colunas
 qtd=colunas.size();
 for(i=0; i < qtd; i++)
 {
  /*
   Para a definição de uma colunar ser inclía   definição da tabela
   a mesma tem de ser incluída por relacionamento simples. Caso a coluna
   foi adicionar   tabela por relacionamento de dependência ou generalização
   a mesma não será incluída na tabela.
  */
  /*if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
      (colunas[i]->incluidoPorLigacao() || colunas[i]->incluidoPorDependencia())) ||
     (!colunas[i]->incluidoPorRelacionamento())) */
  if(tipo_def==SchemaParser::SQL_DEFINITION ||
     (tipo_def==SchemaParser::XML_DEFINITION &&
      !colunas[i]->isAddedByRelationship()))
  {
   str_cols+=colunas[i]->getCodeDefinition(tipo_def);

   if(tipo_def==SchemaParser::SQL_DEFINITION)
    definirAtributoComentario(colunas[i]);
  }
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_cols!="")
  {
   qtd=str_cols.size();
   if(str_cols[qtd-2]==',' || str_cols[qtd-2]=='\n')
    str_cols.remove(qtd-2,2);
  }
 }

 attributes[ParsersAttributes::COLUMNS]=str_cols;
}

void Tabela::definirAtributoRestricoes(unsigned tipo_def)
{
 QString str_rest;
 unsigned i, qtd;
 bool inc_insporrelacao;
 Constraint *rest=NULL;

 //Concatena a definiação SQL das restrições
 qtd=restricoes.size();
 for(i=0; i < qtd; i++)
 {
  rest=dynamic_cast<Constraint *>(restricoes[i]);

  //Chaves estrangeiras criadas pelo usuário não são incluídas na definição SQL da tabela
  if((tipo_def==SchemaParser::SQL_DEFINITION &&
      rest->getConstraintType()!=ConstraintType::foreign_key &&
      (!rest->isReferRelationshipColumn() || rest->getConstraintType()==ConstraintType::primary_key)) ||

     (tipo_def==SchemaParser::XML_DEFINITION && !rest->isAddedByRelationship() &&
      rest->getConstraintType()!=ConstraintType::foreign_key &&
      ((rest->getConstraintType()!=ConstraintType::primary_key && !rest->isReferRelationshipColumn()) ||
       (rest->getConstraintType()==ConstraintType::primary_key))))
  {
   inc_insporrelacao=(tipo_def==SchemaParser::SQL_DEFINITION);
   str_rest+=rest->getCodeDefinition(tipo_def,inc_insporrelacao);

   if(tipo_def==SchemaParser::SQL_DEFINITION)
    definirAtributoComentario(rest);
  }
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  /* Caso a operação acima tenha retornado uma QString
     não vazia, qualquer vírgula ou quebra de linha final
     será removida */
  if(str_rest!="")
  {
   qtd=str_rest.size();
   if(str_rest[qtd-2]==',' || str_rest[qtd-2]=='\n')
   str_rest.remove(qtd-2,2);
  }
 }

 attributes[ParsersAttributes::CONSTRAINTS]=str_rest;
}

void Tabela::definirAtributoGatilhos(unsigned tipo_def)
{
 QString str_gat;
 unsigned i, qtd;
 Trigger *gat=NULL;

 //Concatena a definição SQL dos gatilhos da tabela
 qtd=gatilhos.size();
 for(i=0; i < qtd; i++)
 {
  //Obtém um gatilho
  gat=dynamic_cast<Trigger *>(gatilhos.at(i));

  if((/*!gat->isProtected() &&*/
      !gat->isReferRelationshipColumn() &&
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
  {
   str_gat+=gat->getCodeDefinition(tipo_def);
  }
 }

 attributes[ParsersAttributes::TRIGGERS]=str_gat;
}

void Tabela::definirAtributoIndices(unsigned tipo_def)
{
 QString str_ind;
 unsigned i, qtd;
 Index *ind=NULL;

 //Concatena a definição SQL dos índices da tabela
 qtd=indices.size();
 for(i=0; i < qtd; i++)
 {
  ind=dynamic_cast<Index *>(indices[i]);
  if((!ind->isAddedByRelationship() &&
      !ind->isReferRelationshipColumn() &&
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
   str_ind+=ind->getCodeDefinition(tipo_def);
 }

 attributes[ParsersAttributes::INDEXES]=str_ind;
}

void Tabela::definirAtributoRegras(unsigned tipo_def)
{
 QString str_reg;
 unsigned i, qtd;

 //Concatena a definição SQL das regras da tabela
 qtd=regras.size();
 for(i=0; i < qtd; i++)
 {
  if((/* !regras[i]->isProtected() && */
      tipo_def==SchemaParser::XML_DEFINITION) ||
      tipo_def==SchemaParser::SQL_DEFINITION)
   str_reg+=regras[i]->getCodeDefinition(tipo_def);
 }

 attributes[ParsersAttributes::RULES]=str_reg;
}

vector<TableObject *> *Tabela::obterListaObjetos(ObjectType tipo_obj)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(tipo_obj==OBJ_COLUMN)
  return(&colunas);
 else if(tipo_obj==OBJ_CONSTRAINT)
  return(&restricoes);
 else if(tipo_obj==OBJ_RULE)
  return(&regras);
 else if(tipo_obj==OBJ_TRIGGER)
  return(&gatilhos);
 else if(tipo_obj==OBJ_INDEX)
  return(&indices);
 else
  //Dispara uma exceção caso o tipo de objeto seja inválido
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Tabela::adicionarObjeto(BaseObject *obj, int idx_obj, bool tab_copia)
{
 ObjectType tipo_obj;

 if(!obj)
  //Caso o objeto não esteja alocado, dispara uma exceção
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int idx;
  tipo_obj=obj->getObjectType();
  ConstraintType tipo_rest;
  QString str_aux;

  try
  {
  /* Se o flag de tabela de cópia estiver marcado e o objeto passado
     for uma tabela muda o tipo para OBJETO_TABELA_BASE para forçar a
     inserção da tabela na lista de tabelas cópia */
  if(tab_copia && tipo_obj==OBJ_TABLE)
   tipo_obj=BASE_TABLE;

  /* Verifica se o objeto a ser adicionado   tabela possua o mesmo nome de um objeto que já
     existente,efetuando uma busca na lista de objetos através do nome do novo objeto */
  if(obterObjeto(obj->getName(),tipo_obj,idx))
  {
   //Dispara uma exceçaõ indica a duplicidade de nomes de objetos
   str_aux=QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
           .arg(obj->getName(true))
           .arg(obj->getTypeName())
           .arg(this->getName(true))
           .arg(this->getTypeName());
   throw Exception(str_aux,ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  /* Caso o objeto seja uma tabela (herança) e a mesma for a própria tabela a se adicionar o objeto,
     significa um caso de herança inválida onde uma tabela tenta herdar atributos dela mesma */
  else if(tipo_obj==OBJ_TABLE && obj==this)
   //Dispara a exceção indica o erro
   throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Insere o objeto   lista de acordo com seu tipo
   switch(tipo_obj)
   {
    case OBJ_COLUMN:
    case OBJ_CONSTRAINT:
    case OBJ_TRIGGER:
    case OBJ_INDEX:
    case OBJ_RULE:
      TableObject *obj_tab;
      vector<TableObject *> *lista_obj;

      //Converte o objeto base para objeto de tabela
      obj_tab=dynamic_cast<TableObject *>(obj);

      //Verifica se o objeto não está relacionado com uma outra tabela
      if(!obj_tab->getParentTable())
       obj_tab->setParentTable(this);
      else if(obj_tab->getParentTable()!=this)
       throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Testando a definição sql do objeto
      if(tipo_obj==OBJ_COLUMN)
      {
       Column *col;
       col=dynamic_cast<Column *>(obj_tab);
       col->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_CONSTRAINT)
      {
       Constraint *rest;
       rest=dynamic_cast<Constraint *>(obj_tab);
       rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);
       tipo_rest=rest->getConstraintType();
      }
      else if(tipo_obj==OBJ_INDEX)
      {
       Index *ind;
       ind=dynamic_cast<Index *>(obj_tab);
       ind->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_RULE)
      {
       Rule *reg;
       reg=dynamic_cast<Rule *>(obj_tab);
       reg->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }
      else if(tipo_obj==OBJ_TRIGGER)
      {
       Trigger *gat;
       gat=dynamic_cast<Trigger *>(obj_tab);
       gat->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }

      //Insere o objeto na lista correta
      lista_obj=obterListaObjetos(tipo_obj);

      /* Caso o usuário tente inserir mais uma chave primária na tabela
         será retornado um erro pois uma tabela pode possuir apenas 1
         chave primária */
      if(tipo_obj==OBJ_CONSTRAINT &&
         tipo_rest==ConstraintType::primary_key &&
         this->obterChavePrimaria())
       throw Exception(ERR_ASG_EXISTING_PK_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Caso o índice passado seja menor que zero ou superior ao tamanho da lista
         o objeto será inserido ao final da mesma */
      if(idx_obj < 0 || idx_obj >= static_cast<int>(lista_obj->size()))
       lista_obj->push_back(obj_tab);
      else
      {
       //Caso o índice seja válido, insere o objeto na posição especificada
       if(lista_obj->size() > 0)
        lista_obj->insert((lista_obj->begin() + idx_obj), obj_tab);
       else
        lista_obj->push_back(obj_tab);
      }
    break;

    case OBJ_TABLE:
     Tabela *tab;
     tab=dynamic_cast<Tabela *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(tabelas_pai.size()))
      tabelas_pai.push_back(tab);
     else
      tabelas_pai.insert((tabelas_pai.begin() + idx_obj), tab);
    break;

    case BASE_TABLE:
     Tabela *tab1;
     tab1=dynamic_cast<Tabela *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(tabelas_copia.size()))
      tabelas_copia.push_back(tab1);
     else
      tabelas_copia.insert((tabelas_copia.begin() + idx_obj), tab1);
    break;

    default:
     throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    break;
   }
  }
  catch(Exception &e)
  {
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
    throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
                              .arg(QString::fromUtf8(obj->getName()))
                              .arg(obj->getTypeName()),
                  ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
   else
    //Caso o a exceção não seja de def. SQL mal-formada, apenas redireciona o erro
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

void Tabela::adicionarColuna(Column *col, int idx_col)
{
 adicionarObjeto(col, idx_col);
}

void Tabela::adicionarGatilho(Trigger *gat, int idx_gat)
{
 adicionarObjeto(gat, idx_gat);
}

void Tabela::adicionarIndice(Index *ind, int idx_ind)
{
 adicionarObjeto(ind, idx_ind);
}

void Tabela::adicionarRegra(Rule *reg, int idx_reg)
{
 adicionarObjeto(reg, idx_reg);
}

void Tabela::adicionarRestricao(Constraint *constr, int idx_rest)
{
 adicionarObjeto(constr, idx_rest);
}

void Tabela::adicionarTabelaPai(Tabela *tab, int idx_tab)
{
 adicionarObjeto(tab, idx_tab);
}

void Tabela::adicionarTabelaCopia(Tabela *tab, int idx_tab)
{
 adicionarObjeto(tab, idx_tab, true);
}

void Tabela::removerObjeto(BaseObject *objeto)
{
 if(objeto)
  removerObjeto(objeto->getName(), objeto->getObjectType());
}

void Tabela::removerObjeto(const QString &nome, ObjectType tipo_obj)
{
 int idx;

 //Obtém o índice do objeto localizando-o na lista de objetos
 obterObjeto(nome,tipo_obj,idx);

 //Caso seja encontrado (indice maior que 0)
 if(idx>=0)
  //Remove objeto através de seu índice
  removerObjeto(static_cast<unsigned>(idx),tipo_obj);
}

void Tabela::removerObjeto(unsigned idx_obj, ObjectType tipo_obj)
{
 /* Verfica se o tipo do objeto especifica está fora do conjunto dos que são
    aceitos pela classe tabela */
 if(tipo_obj!=OBJ_COLUMN && tipo_obj!=OBJ_CONSTRAINT &&
    tipo_obj!=OBJ_TRIGGER && tipo_obj!=OBJ_INDEX &&
    tipo_obj!=OBJ_RULE && tipo_obj!=OBJ_TABLE &&
    tipo_obj!=BASE_TABLE)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else if(tipo_obj==OBJ_TABLE && idx_obj < tabelas_pai.size())
 {
  vector<Tabela *>::iterator itr;

  itr=tabelas_pai.begin() + idx_obj; //Obtendo a referênci  tabela pai
  tabelas_pai.erase(itr); //Remove a tabela pai da lista
 }
 //Caso o objeto a ser removido seja uma tabela cópia e seu índice seja válido
 else if(tipo_obj==BASE_TABLE && idx_obj < tabelas_copia.size())
 {
  vector<Tabela *>::iterator itr;

  itr=tabelas_copia.begin() + idx_obj; //Obtendo a referênci  tabela copia
  tabelas_copia.erase(itr); //Remove a tabela pai da lista
 }
 else if(tipo_obj!=OBJ_TABLE && tipo_obj!=BASE_TABLE)
 {
  vector<TableObject *> *lista_obj=NULL;
  vector<TableObject *>::iterator itr;

  //Obtém a lista de objetos de acordo com o tipo passado
  lista_obj=obterListaObjetos(tipo_obj);

  //Caso o indice do objeto extrapole o tamanho da lista de objetos
  if(idx_obj >= lista_obj->size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Executando a remoção genérica de objetos que não sejam colunas
  if(tipo_obj!=OBJ_COLUMN)
  {
   itr=lista_obj->begin() + idx_obj; //Obtendo a referência ao objeto
   (*itr)->setParentTable(NULL);
   lista_obj->erase(itr); //Remove a regra da lista
  }
  else
  {
   vector<TableObject *> vet_refs;
   Column *coluna=NULL;

   //Obtém a referência para a coluna
   itr=lista_obj->begin() + idx_obj;
   coluna=dynamic_cast<Column *>(*itr); //Obtém a coluna
   obterReferenciasColuna(coluna, vet_refs, true);

   /* Caso a coluna esteja sendo referenciada no gatilho, interrompe a
      remoção da mesma disparando uma exceção. */
   if(!vet_refs.empty())
   {
    throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                          .arg(QString::fromUtf8(coluna->getName()))
                          .arg(coluna->getTypeName())
                          .arg(QString::fromUtf8(vet_refs[0]->getName()))
                          .arg(vet_refs[0]->getTypeName())
                          .arg(QString::fromUtf8(this->getName(true)))
                          .arg(this->getTypeName()),
                 ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   coluna->setParentTable(NULL);
   colunas.erase(itr); //Remove a coluna da lista
  }
 }
}

void Tabela::removerColuna(const QString &nome)
{
 removerObjeto(nome,OBJ_COLUMN);
}

void Tabela::removerColuna(unsigned idx_col)
{
 removerObjeto(idx_col,OBJ_COLUMN);
}

void Tabela::removerGatilho(const QString &nome)
{
 removerObjeto(nome,OBJ_TRIGGER);
}

void Tabela::removerGatilho(unsigned idx_gat)
{
 removerObjeto(idx_gat,OBJ_TRIGGER);
}

void Tabela::removerIndice(const QString &nome)
{
 removerObjeto(nome,OBJ_INDEX);
}

void Tabela::removerIndice(unsigned idx_ind)
{
 removerObjeto(idx_ind,OBJ_INDEX);
}

void Tabela::removerRegra(const QString &nome)
{
 removerObjeto(nome,OBJ_RULE);
}

void Tabela::removerRegra(unsigned idx_reg)
{
 removerObjeto(idx_reg,OBJ_RULE);
}

void Tabela::removerRestricao(const QString &nome)
{
 removerObjeto(nome,OBJ_CONSTRAINT);
}

void Tabela::removerRestricao(unsigned idx_const)
{
 removerObjeto(idx_const,OBJ_CONSTRAINT);
}

void Tabela::removerTabelaPai(const QString &nome)
{
 removerObjeto(nome,OBJ_TABLE);
}

void Tabela::removerTabelaPai(unsigned idx_tab)
{
 removerObjeto(idx_tab,OBJ_TABLE);
}

void Tabela::removerTabelaCopia(const QString &nome)
{
 removerObjeto(nome,BASE_TABLE);
}

void Tabela::removerTabelaCopia(unsigned idx_tab)
{
 removerObjeto(idx_tab,BASE_TABLE);
}

int Tabela::obterIndiceObjeto(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 obterObjeto(nome, tipo_obj, idx);
 return(idx);
}

int Tabela::obterIndiceObjeto(TableObject *objeto)
{
 if(!objeto)
  return(-1);
 else
  return(obterIndiceObjeto(objeto->getName(true), objeto->getObjectType()));
}

BaseObject *Tabela::obterObjeto(const QString &nome, ObjectType tipo_obj)
{
 int idx;
 return(obterObjeto(nome, tipo_obj, idx));
}

BaseObject *Tabela::obterObjeto(const QString &nome, ObjectType tipo_obj, int &idx_obj)
{
 BaseObject *objeto=NULL;
 bool enc=false, formatar=false;

 formatar=nome.contains("\"");

 //Caso o objeto a ser obtido seja uma coluna
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
    tipo_obj==OBJ_TRIGGER ||
    tipo_obj==OBJ_INDEX || tipo_obj==OBJ_RULE)
 {
  vector<TableObject *>::iterator itr, itr_end;
  vector<TableObject *> *lista_obj=NULL;
  int qtd;
  QString nome_aux=nome;

  qtd=nome_aux.count(QChar('\0'));
  if(qtd >=1) nome_aux.chop(qtd);

  lista_obj=obterListaObjetos(tipo_obj);

  //Obtém as referências ao primeiro e ultimo elementos da lista
  itr=lista_obj->begin();
  itr_end=lista_obj->end();

  //Varre a lista de objetos
  while(itr!=itr_end)
  {
   //Verifica se o nome a ser encontrado é igual o nome do objeto atual
   enc=((*itr)->getName(formatar)==nome_aux);
   if(!enc) itr++; //Caso não seja encontrado passa para outro elemento da lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(enc)
  {
   //Define seu índice
   idx_obj=(itr-lista_obj->begin());

   //Armazena o objeto em um ponteiro que será retornado
   objeto=(*itr);
  }
  else idx_obj=-1;
 }

 //Caso o objeto a ser obtido seja uma tabela pai
 else if(tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
 {
  vector<Tabela *>::iterator itr_tab, itr_end_tab;
  vector<Tabela *> *tabelas;
  QString nome_aux;

  //Obrigatoriamente todo nome de tabela deve ser formatado por conta do esquema
  nome_aux=BaseObject::formatName(nome);

  if(tipo_obj==OBJ_TABLE)
   tabelas=&tabelas_pai;
  else
   tabelas=&tabelas_copia;

  //Obtém as referências ao primeiro e ultimo elementos da lista de tabelas descendentes
  itr_tab=tabelas->begin();
  itr_end_tab=tabelas->end();

  //Varre a lista de tabelas descendentes
  while(itr_tab!=itr_end_tab)
  {
   //Verifica se o nome a ser encontrado é igual o nome da tabela pai atual
   /* Diferente de outros tipos de objeto, as tabelas são sempre comparadas
      COM O NOME FORMATADO por precisam ser 'schema-qualified' evitando que
      uma tabelas de nomes iguais porém de esquemas diferentes sejam confundidas */
   enc=((*itr_tab)->getName(true)==nome_aux);
   if(!enc) itr_tab++; //Caso seja encontrado, interrompe a varredura pela lista
   else break; //Caso seja encontrado, interrompe a varredura pela lista
  }

  //Caso o objeto seja encontrado
  if(enc)
  {
   //Define seu índice
   idx_obj=(itr_tab-tabelas->begin());

   //Armazena o objeto em um ponteiro que será retornado
   objeto=(*itr_tab);
  }
 }
 else
  throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o objeto encontrado
 return(objeto);
}

BaseObject *Tabela::obterObjeto(unsigned idx_obj, ObjectType tipo_obj)
{
 vector<TableObject *> *lista_obj=NULL;

 if(tipo_obj==OBJ_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= tabelas_pai.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela pai no índice especificado
  return(tabelas_pai[idx_obj]);
 }
 else if(tipo_obj==BASE_TABLE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= tabelas_copia.size())
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela cópia no índice especificado
  return(tabelas_copia[idx_obj]);
 }
 else
 {
  /* Obtém um objeto de uma determinada lista de acordo com seu tipo
     e se seu índice seja válido */
  lista_obj=obterListaObjetos(tipo_obj);
  if(idx_obj < lista_obj->size())
   return(lista_obj->at(idx_obj));
  else
   throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

Tabela *Tabela::obterTabelaPai(const QString &nome)
{
 int idx;
 return(dynamic_cast<Tabela *>(obterObjeto(nome,OBJ_TABLE,idx)));
}

Tabela *Tabela::obterTabelaPai(unsigned idx_tab)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_tab,OBJ_TABLE)));
}

Column *Tabela::obterColuna(const QString &nome, bool ref_nome_antigo)
{
 if(!ref_nome_antigo)
 {
  int idx;
  return(dynamic_cast<Column *>(obterObjeto(nome,OBJ_COLUMN,idx)));
 }
 else
 {
  Column *coluna=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool enc=false, formatar=false;

  formatar=nome.contains("\"");
  itr=colunas.begin();
  itr_end=colunas.end();

  //Varre a lista de colunas buscando pelo nome antigo das mesmas
  while(itr!=itr_end && !enc)
  {
   coluna=dynamic_cast<Column *>(*itr);
   itr++;
   enc=(nome!="" && coluna->getOldName(formatar)==nome);
  }

  /* Caso nenhuma coluna for encontrada zera o ponteiro de coluna
     para forçar o retorno de uma coluna nula indicando que a
     busca não retornou objeto */
  if(!enc) coluna=NULL;
  return(coluna);
 }
}

Column *Tabela::obterColuna(unsigned idx_col)
{
 return(dynamic_cast<Column *>(obterObjeto(idx_col,OBJ_COLUMN)));
}

Trigger *Tabela::obterGatilho(const QString &nome)
{
 int idx;
 return(dynamic_cast<Trigger *>(obterObjeto(nome,OBJ_TRIGGER,idx)));
}

Trigger *Tabela::obterGatilho(unsigned idx_gat)
{
 return(dynamic_cast<Trigger *>(obterObjeto(idx_gat,OBJ_TRIGGER)));
}

Constraint *Tabela::obterRestricao(const QString &nome)
{
 int idx;
 return(dynamic_cast<Constraint *>(obterObjeto(nome,OBJ_CONSTRAINT,idx)));
}

Constraint *Tabela::obterRestricao(unsigned idx_constr)
{
 return(dynamic_cast<Constraint *>(obterObjeto(idx_constr,OBJ_CONSTRAINT)));
}

Index *Tabela::obterIndice(const QString &nome)
{
 int idx;
 return(dynamic_cast<Index *>(obterObjeto(nome,OBJ_INDEX,idx)));
}

Index *Tabela::obterIndice(unsigned idx_ind)
{
 return(dynamic_cast<Index *>(obterObjeto(idx_ind,OBJ_INDEX)));
}

Rule *Tabela::obterRegra(const QString &nome)
{
 int idx;
 return(dynamic_cast<Rule *>(obterObjeto(nome,OBJ_RULE,idx)));
}

Rule *Tabela::obterRegra(unsigned idx_reg)
{
 return(dynamic_cast<Rule *>(obterObjeto(idx_reg,OBJ_RULE)));
}

Tabela *Tabela::obterTabelaCopia(const QString &nome)
{
 int idx;
 return(dynamic_cast<Tabela *>(obterObjeto(nome,BASE_TABLE,idx)));
}

Tabela *Tabela::obterTabelaCopia(unsigned idx_tab)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_tab,BASE_TABLE)));
}

unsigned Tabela::obterNumColunas(void)
{
 return(colunas.size());
}

unsigned Tabela::obterNumGatilhos(void)
{
 return(gatilhos.size());
}

unsigned Tabela::obterNumRestricoes(void)
{
 return(restricoes.size());
}

unsigned Tabela::obterNumIndices(void)
{
 return(indices.size());
}

unsigned Tabela::obterNumRegras(void)
{
 return(regras.size());
}

unsigned Tabela::obterNumTabelasPai(void)
{
 return(tabelas_pai.size());
}

unsigned Tabela::obterNumTabelasCopia(void)
{
 return(tabelas_copia.size());
}

unsigned Tabela::obterNumObjetos(ObjectType tipo_obj, bool inc_insporrelacao)
{
 if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
    tipo_obj==OBJ_TRIGGER ||/* tipo_obj==OBJETO_GAT_RESTRICAO ||*/
    tipo_obj==OBJ_INDEX || tipo_obj==OBJ_RULE ||
    tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
 {
  if(tipo_obj==OBJ_TABLE || tipo_obj==BASE_TABLE)
  {
   if(tipo_obj==OBJ_TABLE)
    return(tabelas_pai.size());
   else
    return(tabelas_copia.size());
  }
  else
  {
   vector<TableObject *> *lista=NULL;
   lista=obterListaObjetos(tipo_obj);

   //Faz a contagem apenas dos objetos que não são auto-incluidos (incluídos por relacionamento)
   if(!inc_insporrelacao)
   {
    vector<TableObject *>::iterator itr, itr_end;
    unsigned qtd=0;

    itr=lista->begin();
    itr_end=lista->end();
    while(itr!=itr_end)
    {
     if(!(*itr)->isAddedByRelationship()) qtd++;
     itr++;
    }

    return(qtd);
   }
   else
    return(lista->size());
  }
 }
 else
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Tabela::obterChavePrimaria(void)
{
 unsigned qtd,i;
 Constraint *pk=NULL, *rest=NULL;

 qtd=restricoes.size();
 for(i=0; i < qtd && !pk; i++)
 {
  rest=dynamic_cast<Constraint *>(restricoes[i]);
  pk=(rest->getConstraintType()==ConstraintType::primary_key ? rest : NULL);
 }

 return(pk);
}

void Tabela::obterChavesEstrangeiras(vector<Constraint *> &vet_fks, bool inc_insporrelacao)
{
 unsigned qtd,i;
 Constraint *constr=NULL;

 qtd=restricoes.size();
 for(i=0; i < qtd; i++)
 {
  constr=dynamic_cast<Constraint *>(restricoes[i]);

  /* Adiciona a restrição caso seja chave estrangeira, caso esta não foi
     incluída por relacionamento ou se foi e o parametro inc_insporrelacao está
     setado */
  if(constr->getConstraintType()==ConstraintType::foreign_key &&
     (!constr->isAddedByLinking() ||
      (constr->isAddedByLinking() && inc_insporrelacao)))
   vet_fks.push_back(constr);
 }
}

bool Tabela::aceitaOids(void)
{
 return(aceita_oids);
}

bool Tabela::referenciaTabelaChaveEstrangeira(Tabela *tab_ref)
{
 unsigned qtd,i;
 Constraint *constr=NULL;
 bool found=false;

 qtd=restricoes.size();
 for(i=0; i < qtd && !found; i++)
 {
  constr=dynamic_cast<Constraint *>(restricoes[i]);
  found=(constr->getConstraintType()==ConstraintType::foreign_key &&
         !constr->isAddedByLinking() &&
          constr->getReferencedTable() == tab_ref);
 }

 return(found);
}

bool Tabela::restricaoReferenciaColuna(Column *coluna, ConstraintType tipo_rest)
{
 bool enc=false;
 vector<TableObject *>::iterator itr, itr_end;
 Constraint *rest=NULL;

 if(coluna)
 {
  itr=restricoes.begin();
  itr_end=restricoes.end();
  while(itr!=itr_end && !enc)
  {
   rest=dynamic_cast<Constraint *>(*itr);
   itr++;
   enc=(rest->getConstraintType()==tipo_rest &&
        (rest->isColumnExists(coluna, Constraint::SOURCE_COLS) ||
         rest->isColumnExists(coluna, Constraint::REFERENCED_COLS)));
  }
 }

 return(enc);
}

QString Tabela::getCodeDefinition(unsigned tipo_def)
{
 attributes[ParsersAttributes::OIDS]=(aceita_oids ? "1" : "");
 definirAtributoColunas(tipo_def);
 definirAtributoRestricoes(tipo_def);
 definirAtributoGatilhos(tipo_def);
 definirAtributoIndices(tipo_def);
 definirAtributoRegras(tipo_def);

 if(tipo_def==SchemaParser::XML_DEFINITION)
  setPositionAttribute();

 return(BaseObject::__getCodeDefinition(tipo_def));
}

void Tabela::operator = (Tabela &tabela)
{
 QString nome_ant = this->getName(true);

 (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tabela);
 this->aceita_oids=tabela.aceita_oids;
 setProtected(tabela.is_protected);

 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->getName(true));
}

bool Tabela::referenciaObjetoIncRelacao(void)
{
 vector<TableObject *>::iterator itr, itr_end;
 ObjectType tipos[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
 bool enc=false;

 for(unsigned i=0; i < 2 && !enc; i++)
 {
  //Obtém a lista de tipos atual
  itr=obterListaObjetos(tipos[i])->begin();
  itr_end=obterListaObjetos(tipos[i])->end();

  /* Efetua uma iteração verifica se os objetos da lista
     atual foram incluídas por relacionamento, caso uma objeto
     for detectada como incluída desta forma é suficiente
     dizer que a tabela referencia um objeto vindo de
     relacionamento */
  while(itr!=itr_end && !enc)
  {
   //Obtém se a coluna foi incluída por relacionamento ou não
   enc=(*itr)->isAddedByRelationship();
   //Passa para a próxima coluna
   itr++;
  }
 }

 return(enc);
}

void Tabela::trocarIndicesObjetos(ObjectType tipo_obj, unsigned idx1, unsigned idx2)
{
 vector<TableObject *> *lista_obj=NULL;
 vector<TableObject *>::iterator itr1, itr2;
 TableObject *obj_aux=NULL;

 try
 {
  if(idx1!=idx2)
  {
   lista_obj=obterListaObjetos(tipo_obj);

   if(idx1 >= lista_obj->size() || idx2 >= lista_obj->size())
    throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   obj_aux=lista_obj->at(idx1);
   itr1=lista_obj->begin() + idx1;
   itr2=lista_obj->begin() + idx2;

   (*itr1)=(*itr2);
   (*itr2)=obj_aux;
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Tabela::obterReferenciasColuna(Column *coluna, vector<TableObject *> &vet_refs, bool modo_exclusao)
{
 /* Caso a coluna não foi incluída por relacionamento, será verificado se esta
    não está sendo referenciada. Para colunas adicionadas automaticamente por
    relacionamentos, sua remoção é feita sem verificação alguma */
 if(coluna && !coluna->isAddedByRelationship())
 {
  unsigned qtd, i;
  IndexElement elem;
  Column *col=NULL, *col1=NULL;
  vector<TableObject *>::iterator itr, itr_end;
  bool enc=false;
  Index *ind=NULL;
  Constraint *rest=NULL;
  Trigger *gat=NULL;

  //Verificando se a coluna não é referenciada nos indices
  itr=indices.begin();
  itr_end=indices.end();

  while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   ind=dynamic_cast<Index *>(*itr);
   itr++;

   qtd=ind->getElementCount();
   for(i=0; i < qtd  && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    elem=ind->getElement(i);
    col=elem.getColumn();
    if(col && col==coluna)
    {
     enc=true;
     vet_refs.push_back(ind);
    }
   }
  }

  //Verificando se a coluna não é referenciada nas constraints
  itr=restricoes.begin();
  itr_end=restricoes.end();

  while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   rest=dynamic_cast<Constraint *>(*itr);
   itr++;

   col=rest->getColumn(coluna->getName(),true);
   col1=rest->getColumn(coluna->getName(),false);

   if((col && col==coluna) || (col1 && col1==coluna))
   {
    enc=true;
    vet_refs.push_back(rest);
   }
  }

  //Verificando se a coluna não é referenciada nos gatilhos
  itr=gatilhos.begin();
  itr_end=gatilhos.end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   //Verfica se a coluna a ser removida não está presente nas daus lista de colunas da contraint
   gat=dynamic_cast<Trigger *>(*itr);
   itr++;

   qtd=gat->getColumnCount();
   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    if(gat->getColumn(i)==coluna)
    {
     enc=true;
     vet_refs.push_back(gat);
    }
   }
  }
 }
}

