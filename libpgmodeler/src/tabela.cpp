#include "tabela.h"
//***********************************************************
Tabela::Tabela(void) : TabelaBase()
{
 tipo_objeto=OBJETO_TABELA;
 atributos[AtributosParsers::COLUNAS]="";
 atributos[AtributosParsers::RESTRICOES]="";
 atributos[AtributosParsers::INDICES]="";
 atributos[AtributosParsers::GATILHOS]="";
 atributos[AtributosParsers::REGRAS]="";
 atributos[AtributosParsers::OIDS]="";
}
//-----------------------------------------------------------
Tabela::~Tabela(void)
{
 TipoObjetoBase tipos[]={ OBJETO_GATILHO, OBJETO_INDICE, OBJETO_REGRA,
                          OBJETO_RESTRICAO, OBJETO_COLUNA };
 vector<ObjetoTabela *> *lista=NULL;

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
//-----------------------------------------------------------
void Tabela::definirAceitaOids(bool valor)
{
 aceita_oids=valor;
}
//-----------------------------------------------------------
void Tabela::definirProtegido(bool protegido)
{
 TipoObjetoBase tipos_obj[]={ OBJETO_COLUNA, OBJETO_RESTRICAO,
                              OBJETO_INDICE, OBJETO_REGRA, OBJETO_GATILHO };
 unsigned i;
 vector<ObjetoTabela *>::iterator itr, itr_end;
 vector<ObjetoTabela *> *lista=NULL;
 ObjetoTabela *obj_tab=NULL;

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
   if(!obj_tab->incluidoPorRelacionamento())
    obj_tab->definirProtegido(protegido);
   itr++;
  }
 }

 ObjetoGraficoBase::definirProtegido(protegido);
}
//-----------------------------------------------------------
void Tabela::definirAtributoColunas(unsigned tipo_def)
{
 QString str_cols;
 unsigned i, qtd;

 //Concatena a definiação SQL das colunas
 qtd=colunas.size();
 for(i=0; i < qtd; i++)
 {
  /*
   Para a definição de uma colunar ser incluída à definição da tabela
   a mesma tem de ser incluída por relacionamento simples. Caso a coluna
   foi adicionar à tabela por relacionamento de dependência ou generalização
   a mesma não será incluída na tabela.
  */
  /*if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
      (colunas[i]->incluidoPorLigacao() || colunas[i]->incluidoPorDependencia())) ||
     (!colunas[i]->incluidoPorRelacionamento())) */
  if(tipo_def==ParserEsquema::DEFINICAO_SQL ||
     (tipo_def==ParserEsquema::DEFINICAO_XML &&
      !colunas[i]->incluidoPorRelacionamento()))
   str_cols+=colunas[i]->obterDefinicaoObjeto(tipo_def);
 }

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
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

 atributos[AtributosParsers::COLUNAS]=str_cols;
}
//-----------------------------------------------------------
void Tabela::definirAtributoRestricoes(unsigned tipo_def)
{
 QString str_rest;
 unsigned i, qtd;
 bool inc_insporrelacao;
 Restricao *rest=NULL;

 //Concatena a definiação SQL das restrições
 qtd=restricoes.size();
 for(i=0; i < qtd; i++)
 {
  rest=dynamic_cast<Restricao *>(restricoes[i]);

  if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
      (!rest->referenciaColunaIncRelacao() || rest->obterTipoRestricao()==TipoRestricao::primary_key)) ||

     (tipo_def==ParserEsquema::DEFINICAO_XML && !rest->incluidoPorRelacionamento() &&
      ((rest->obterTipoRestricao()!=TipoRestricao::primary_key && !rest->referenciaColunaIncRelacao()) ||
       (rest->obterTipoRestricao()==TipoRestricao::primary_key))))
  {
   inc_insporrelacao=(tipo_def==ParserEsquema::DEFINICAO_SQL);
   str_rest+=rest->obterDefinicaoObjeto(tipo_def,inc_insporrelacao);
  }
 }

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
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

 atributos[AtributosParsers::RESTRICOES]=str_rest;
}
//-----------------------------------------------------------
void Tabela::definirAtributoGatilhos(unsigned tipo_def)
{
 QString str_gat;
 unsigned i, qtd;
 Gatilho *gat=NULL;

 //Concatena a definição SQL dos gatilhos da tabela
 qtd=gatilhos.size();
 for(i=0; i < qtd; i++)
 {
  //Obtém um gatilho
  gat=dynamic_cast<Gatilho *>(gatilhos.at(i));

  if((/*!gat->objetoProtegido() &&*/
      !gat->referenciaColunaIncRelacao() &&
      tipo_def==ParserEsquema::DEFINICAO_XML) ||
      tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   str_gat+=gat->obterDefinicaoObjeto(tipo_def);
  }
 }

 atributos[AtributosParsers::GATILHOS]=str_gat;
}
//-----------------------------------------------------------
void Tabela::definirAtributoIndices(unsigned tipo_def)
{
 QString str_ind;
 unsigned i, qtd;
 Indice *ind=NULL;

 //Concatena a definição SQL dos índices da tabela
 qtd=indices.size();
 for(i=0; i < qtd; i++)
 {
  ind=dynamic_cast<Indice *>(indices[i]);
  if((!ind->incluidoPorRelacionamento() &&
      !ind->referenciaColunaIncRelacao() &&
      tipo_def==ParserEsquema::DEFINICAO_XML) ||
      tipo_def==ParserEsquema::DEFINICAO_SQL)
   str_ind+=ind->obterDefinicaoObjeto(tipo_def);
 }

 atributos[AtributosParsers::INDICES]=str_ind;
}
//-----------------------------------------------------------
void Tabela::definirAtributoRegras(unsigned tipo_def)
{
 QString str_reg;
 unsigned i, qtd;

 //Concatena a definição SQL das regras da tabela
 qtd=regras.size();
 for(i=0; i < qtd; i++)
 {
  if((/* !regras[i]->objetoProtegido() && */
      tipo_def==ParserEsquema::DEFINICAO_XML) ||
      tipo_def==ParserEsquema::DEFINICAO_SQL)
   str_reg+=regras[i]->obterDefinicaoObjeto(tipo_def);
 }

 atributos[AtributosParsers::REGRAS]=str_reg;
}
//-----------------------------------------------------------
vector<ObjetoTabela *> *Tabela::obterListaObjetos(TipoObjetoBase tipo_obj)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(tipo_obj==OBJETO_COLUNA)
  return(&colunas);
 else if(tipo_obj==OBJETO_RESTRICAO)
  return(&restricoes);
 else if(tipo_obj==OBJETO_REGRA)
  return(&regras);
 else if(tipo_obj==OBJETO_GATILHO)
  return(&gatilhos);
 else if(tipo_obj==OBJETO_INDICE)
  return(&indices);
 else
  //Dispara uma exceção caso o tipo de objeto seja inválido
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
void Tabela::adicionarObjeto(ObjetoBase *obj, int idx_obj, bool tab_copia)
{
 TipoObjetoBase tipo_obj;

 if(!obj)
  //Caso o objeto não esteja alocado, dispara uma exceção
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int idx;
  tipo_obj=obj->obterTipoObjeto();
  TipoRestricao tipo_rest;
  QString str_aux;

  try
  {
  /* Se o flag de tabela de cópia estiver marcado e o objeto passado
     for uma tabela muda o tipo para OBJETO_TABELA_BASE para forçar a
     inserção da tabela na lista de tabelas cópia */
  if(tab_copia && tipo_obj==OBJETO_TABELA)
   tipo_obj=OBJETO_TABELA_BASE;

  /* Verifica se o objeto a ser adicionado à tabela possua o mesmo nome de um objeto que já
     existente,efetuando uma busca na lista de objetos através do nome do novo objeto */
  if(obterObjeto(obj->obterNome(),tipo_obj,idx))
  {
   //Dispara uma exceçaõ indica a duplicidade de nomes de objetos
   str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLIC))
           .arg(obj->obterNome(true))
           .arg(obj->obterNomeTipoObjeto())
           .arg(this->obterNome(true))
           .arg(this->obterNomeTipoObjeto());
   throw Excecao(str_aux,ERR_PGMODELER_ATROBJDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  /* Caso o objeto seja uma tabela (herança) e a mesma for a própria tabela a se adicionar o objeto,
     significa um caso de herança inválida onde uma tabela tenta herdar atributos dela mesma */
  else if(tipo_obj==OBJETO_TABELA && obj==this)
   //Dispara a exceção indica o erro
   throw Excecao(ERR_PGMODELER_HERANCATABINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Insere o objeto à lista de acordo com seu tipo
   switch(tipo_obj)
   {
    case OBJETO_COLUNA:
    case OBJETO_RESTRICAO:
    case OBJETO_GATILHO:
    case OBJETO_INDICE:
    case OBJETO_REGRA:
      ObjetoTabela *obj_tab;
      vector<ObjetoTabela *> *lista_obj;

      //Converte o objeto base para objeto de tabela
      obj_tab=dynamic_cast<ObjetoTabela *>(obj);

      //Verifica se o objeto não está relacionado com uma outra tabela
      if(!obj_tab->obterTabelaPai())
       obj_tab->definirTabelaPai(this);
      else if(obj_tab->obterTabelaPai()!=this)
       throw Excecao(ERR_PGMODELER_ATROBJRELAC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Testando a definição sql do objeto
      if(tipo_obj==OBJETO_COLUNA)
      {
       Coluna *col;
       col=dynamic_cast<Coluna *>(obj_tab);
       col->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL);
      }
      else if(tipo_obj==OBJETO_RESTRICAO)
      {
       Restricao *rest;
       rest=dynamic_cast<Restricao *>(obj_tab);
       rest->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL);
       tipo_rest=rest->obterTipoRestricao();
      }
      else if(tipo_obj==OBJETO_INDICE)
      {
       Indice *ind;
       ind=dynamic_cast<Indice *>(obj_tab);
       ind->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL);
      }
      else if(tipo_obj==OBJETO_REGRA)
      {
       Regra *reg;
       reg=dynamic_cast<Regra *>(obj_tab);
       reg->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL);
      }
      else if(tipo_obj==OBJETO_GATILHO)
      {
       Gatilho *gat;
       gat=dynamic_cast<Gatilho *>(obj_tab);
       gat->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL);
      }

      //Insere o objeto na lista correta
      lista_obj=obterListaObjetos(tipo_obj);

      /* Caso o usuário tente inserir mais uma chave primária na tabela
         será retornado um erro pois uma tabela pode possuir apenas 1
         chave primária */
      if(tipo_obj==OBJETO_RESTRICAO &&
         tipo_rest==TipoRestricao::primary_key &&
         this->obterChavePrimaria())
       throw Excecao(ERR_PGMODELER_ATRPKEXISTETAB,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

    case OBJETO_TABELA:
     Tabela *tab;
     tab=dynamic_cast<Tabela *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(tabelas_pai.size()))
      tabelas_pai.push_back(tab);
     else
      tabelas_pai.insert((tabelas_pai.begin() + idx_obj), tab);
    break;

    case OBJETO_TABELA_BASE:
     Tabela *tab1;
     tab1=dynamic_cast<Tabela *>(obj);
     if(idx_obj < 0 || idx_obj >= static_cast<int>(tabelas_copia.size()))
      tabelas_copia.push_back(tab1);
     else
      tabelas_copia.insert((tabelas_copia.begin() + idx_obj), tab1);
    break;

    default:
     throw Excecao(ERR_PGMODELER_ATROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    break;
   }
  }
  catch(Excecao &e)
  {
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.obterTipoErro()==ERR_PARSERS_ATRIBVALORNULO)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDEFSQLINV)
                              .arg(obj->obterNome())
                              .arg(obj->obterNomeTipoObjeto()),
                  ERR_PGMODELER_ATROBJDEFSQLINV,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
   else
    //Caso o a exceção não seja de def. SQL mal-formada, apenas redireciona o erro
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}
//-----------------------------------------------------------
void Tabela::adicionarColuna(Coluna *col, int idx_col)
{
 adicionarObjeto(col, idx_col);
}
//-----------------------------------------------------------
void Tabela::adicionarGatilho(Gatilho *gat, int idx_gat)
{
 adicionarObjeto(gat, idx_gat);
}
//-----------------------------------------------------------
void Tabela::adicionarIndice(Indice *ind, int idx_ind)
{
 adicionarObjeto(ind, idx_ind);
}
//-----------------------------------------------------------
void Tabela::adicionarRegra(Regra *reg, int idx_reg)
{
 adicionarObjeto(reg, idx_reg);
}
//-----------------------------------------------------------
void Tabela::adicionarRestricao(Restricao *constr, int idx_rest)
{
 adicionarObjeto(constr, idx_rest);
}
//-----------------------------------------------------------
void Tabela::adicionarTabelaPai(Tabela *tab, int idx_tab)
{
 adicionarObjeto(tab, idx_tab);
}
//-----------------------------------------------------------
void Tabela::adicionarTabelaCopia(Tabela *tab, int idx_tab)
{
 adicionarObjeto(tab, idx_tab, true);
}
//-----------------------------------------------------------
void Tabela::removerObjeto(ObjetoBase *objeto)
{
 if(objeto)
  removerObjeto(objeto->obterNome(), objeto->obterTipoObjeto());
}
//-----------------------------------------------------------
void Tabela::removerObjeto(const QString &nome, TipoObjetoBase tipo_obj)
{
 int idx;

 //Obtém o índice do objeto localizando-o na lista de objetos
 obterObjeto(nome,tipo_obj,idx);

 //Caso seja encontrado (indice maior que 0)
 if(idx>=0)
  //Remove objeto através de seu índice
  removerObjeto(static_cast<unsigned>(idx),tipo_obj);
}
//-----------------------------------------------------------
void Tabela::removerObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj)
{
 /* Verfica se o tipo do objeto especifica está fora do conjunto dos que são
    aceitos pela classe tabela */
 if(tipo_obj!=OBJETO_COLUNA && tipo_obj!=OBJETO_RESTRICAO &&
    tipo_obj!=OBJETO_GATILHO && tipo_obj!=OBJETO_INDICE &&
    tipo_obj!=OBJETO_REGRA && tipo_obj!=OBJETO_TABELA &&
    tipo_obj!=OBJETO_TABELA_BASE)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Excecao(ERR_PGMODELER_REMOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else if(tipo_obj==OBJETO_TABELA && idx_obj < tabelas_pai.size())
 {
  vector<Tabela *>::iterator itr;

  itr=tabelas_pai.begin() + idx_obj; //Obtendo a referência à tabela pai
  tabelas_pai.erase(itr); //Remove a tabela pai da lista
 }
 //Caso o objeto a ser removido seja uma tabela cópia e seu índice seja válido
 else if(tipo_obj==OBJETO_TABELA_BASE && idx_obj < tabelas_copia.size())
 {
  vector<Tabela *>::iterator itr;

  itr=tabelas_copia.begin() + idx_obj; //Obtendo a referência à tabela copia
  tabelas_copia.erase(itr); //Remove a tabela pai da lista
 }
 else if(tipo_obj!=OBJETO_TABELA && tipo_obj!=OBJETO_TABELA_BASE)
 {
  vector<ObjetoTabela *> *lista_obj=NULL;
  vector<ObjetoTabela *>::iterator itr;

  //Obtém a lista de objetos de acordo com o tipo passado
  lista_obj=obterListaObjetos(tipo_obj);

  //Caso o indice do objeto extrapole o tamanho da lista de objetos
  if(idx_obj >= lista_obj->size())
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Executando a remoção genérica de objetos que não sejam colunas
  if(tipo_obj!=OBJETO_COLUNA)
  {
   itr=lista_obj->begin() + idx_obj; //Obtendo a referência ao objeto
   (*itr)->definirTabelaPai(NULL);
   lista_obj->erase(itr); //Remove a regra da lista
  }
  else
  {
   vector<ObjetoTabela *> vet_refs;
   Coluna *coluna=NULL;

   //Obtém a referência para a coluna
   itr=lista_obj->begin() + idx_obj;
   coluna=dynamic_cast<Coluna *>(*itr); //Obtém a coluna
   obterReferenciasColuna(coluna, vet_refs, true);

   /* Caso a coluna esteja sendo referenciada no gatilho, interrompe a
      remoção da mesma disparando uma exceção. */
   if(!vet_refs.empty())
   {
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERIND)
                          .arg(coluna->obterNome())
                          .arg(coluna->obterNomeTipoObjeto())
                          .arg(vet_refs[0]->obterNome())
                          .arg(vet_refs[0]->obterNomeTipoObjeto())
                          .arg(this->obterNome(true))
                          .arg(this->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERIND,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   coluna->definirTabelaPai(NULL);
   colunas.erase(itr); //Remove a coluna da lista
  }
 }
}
//-----------------------------------------------------------
void Tabela::removerColuna(const QString &nome)
{
 removerObjeto(nome,OBJETO_COLUNA);
}
//-----------------------------------------------------------
void Tabela::removerColuna(unsigned idx_col)
{
 removerObjeto(idx_col,OBJETO_COLUNA);
}
//-----------------------------------------------------------
void Tabela::removerGatilho(const QString &nome)
{
 removerObjeto(nome,OBJETO_GATILHO);
}
//-----------------------------------------------------------
void Tabela::removerGatilho(unsigned idx_gat)
{
 removerObjeto(idx_gat,OBJETO_GATILHO);
}
//-----------------------------------------------------------
void Tabela::removerIndice(const QString &nome)
{
 removerObjeto(nome,OBJETO_INDICE);
}
//-----------------------------------------------------------
void Tabela::removerIndice(unsigned idx_ind)
{
 removerObjeto(idx_ind,OBJETO_INDICE);
}
//-----------------------------------------------------------
void Tabela::removerRegra(const QString &nome)
{
 removerObjeto(nome,OBJETO_REGRA);
}
//-----------------------------------------------------------
void Tabela::removerRegra(unsigned idx_reg)
{
 removerObjeto(idx_reg,OBJETO_REGRA);
}
//-----------------------------------------------------------
void Tabela::removerRestricao(const QString &nome)
{
 removerObjeto(nome,OBJETO_RESTRICAO);
}
//-----------------------------------------------------------
void Tabela::removerRestricao(unsigned idx_const)
{
 removerObjeto(idx_const,OBJETO_RESTRICAO);
}
//-----------------------------------------------------------
void Tabela::removerTabelaPai(const QString &nome)
{
 removerObjeto(nome,OBJETO_TABELA);
}
//-----------------------------------------------------------
void Tabela::removerTabelaPai(unsigned idx_tab)
{
 removerObjeto(idx_tab,OBJETO_TABELA);
}
//-----------------------------------------------------------
void Tabela::removerTabelaCopia(const QString &nome)
{
 removerObjeto(nome,OBJETO_TABELA_BASE);
}
//-----------------------------------------------------------
void Tabela::removerTabelaCopia(unsigned idx_tab)
{
 removerObjeto(idx_tab,OBJETO_TABELA_BASE);
}
//-----------------------------------------------------------
int Tabela::obterIndiceObjeto(const QString &nome, TipoObjetoBase tipo_obj)
{
 int idx;
 obterObjeto(nome, tipo_obj, idx);
 return(idx);
}
//-----------------------------------------------------------
int Tabela::obterIndiceObjeto(ObjetoTabela *objeto)
{
 if(!objeto)
  return(-1);
 else
  return(obterIndiceObjeto(objeto->obterNome(true), objeto->obterTipoObjeto()));
}
//-----------------------------------------------------------
ObjetoBase *Tabela::obterObjeto(const QString &nome, TipoObjetoBase tipo_obj)
{
 int idx;
 return(obterObjeto(nome, tipo_obj, idx));
}
//-----------------------------------------------------------
ObjetoBase *Tabela::obterObjeto(const QString &nome, TipoObjetoBase tipo_obj, int &idx_obj)
{
 ObjetoBase *objeto=NULL;
 bool enc=false, formatar=false;

 formatar=nome.contains("\"");

 //Caso o objeto a ser obtido seja uma coluna
 if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO ||
    tipo_obj==OBJETO_GATILHO ||
    tipo_obj==OBJETO_INDICE || tipo_obj==OBJETO_REGRA)
 {
  vector<ObjetoTabela *>::iterator itr, itr_end;
  vector<ObjetoTabela *> *lista_obj=NULL;
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
   enc=((*itr)->obterNome(formatar)==nome_aux);
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
 else if(tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_TABELA_BASE)
 {
  vector<Tabela *>::iterator itr_tab, itr_end_tab;
  vector<Tabela *> *tabelas;
  QString nome_aux;

  //Obrigatoriamente todo nome de tabela deve ser formatado por conta do esquema
  nome_aux=ObjetoBase::formatarNome(nome);

  if(tipo_obj==OBJETO_TABELA)
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
   enc=((*itr_tab)->obterNome(true)==nome_aux);
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
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o objeto encontrado
 return(objeto);
}
//-----------------------------------------------------------
ObjetoBase *Tabela::obterObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj)
{
 vector<ObjetoTabela *> *lista_obj=NULL;

 if(tipo_obj==OBJETO_TABELA)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= tabelas_pai.size())
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Retorna a tabela pai no índice especificado
  return(tabelas_pai[idx_obj]);
 }
 else if(tipo_obj==OBJETO_TABELA_BASE)
 {
  //Caso o índice do objeto seja inválido dispara uma exceção
  if(idx_obj >= tabelas_copia.size())
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
Tabela *Tabela::obterTabelaPai(const QString &nome)
{
 int idx;
 return(dynamic_cast<Tabela *>(obterObjeto(nome,OBJETO_TABELA,idx)));
}
//-----------------------------------------------------------
Tabela *Tabela::obterTabelaPai(unsigned idx_tab)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_tab,OBJETO_TABELA)));
}
//-----------------------------------------------------------
Coluna *Tabela::obterColuna(const QString &nome, bool ref_nome_antigo)
{
 if(!ref_nome_antigo)
 {
  int idx;
  return(dynamic_cast<Coluna *>(obterObjeto(nome,OBJETO_COLUNA,idx)));
 }
 else
 {
  Coluna *coluna=NULL;
  vector<ObjetoTabela *>::iterator itr, itr_end;
  bool enc=false, formatar=false;

  formatar=nome.contains("\"");
  itr=colunas.begin();
  itr_end=colunas.end();

  //Varre a lista de colunas buscando pelo nome antigo das mesmas
  while(itr!=itr_end && !enc)
  {
   coluna=dynamic_cast<Coluna *>(*itr);
   itr++;
   enc=(nome!="" && coluna->obterNomeAntigo(formatar)==nome);
  }

  /* Caso nenhuma coluna for encontrada zera o ponteiro de coluna
     para forçar o retorno de uma coluna nula indicando que a
     busca não retornou objeto */
  if(!enc) coluna=NULL;
  return(coluna);
 }
}
//-----------------------------------------------------------
Coluna *Tabela::obterColuna(unsigned idx_col)
{
 return(dynamic_cast<Coluna *>(obterObjeto(idx_col,OBJETO_COLUNA)));
}
//-----------------------------------------------------------
Gatilho *Tabela::obterGatilho(const QString &nome)
{
 int idx;
 return(dynamic_cast<Gatilho *>(obterObjeto(nome,OBJETO_GATILHO,idx)));
}
//-----------------------------------------------------------
Gatilho *Tabela::obterGatilho(unsigned idx_gat)
{
 return(dynamic_cast<Gatilho *>(obterObjeto(idx_gat,OBJETO_GATILHO)));
}
//-----------------------------------------------------------
Restricao *Tabela::obterRestricao(const QString &nome)
{
 int idx;
 return(dynamic_cast<Restricao *>(obterObjeto(nome,OBJETO_RESTRICAO,idx)));
}
//-----------------------------------------------------------
Restricao *Tabela::obterRestricao(unsigned idx_constr)
{
 return(dynamic_cast<Restricao *>(obterObjeto(idx_constr,OBJETO_RESTRICAO)));
}
//-----------------------------------------------------------
Indice *Tabela::obterIndice(const QString &nome)
{
 int idx;
 return(dynamic_cast<Indice *>(obterObjeto(nome,OBJETO_INDICE,idx)));
}
//-----------------------------------------------------------
Indice *Tabela::obterIndice(unsigned idx_ind)
{
 return(dynamic_cast<Indice *>(obterObjeto(idx_ind,OBJETO_INDICE)));
}
//-----------------------------------------------------------
Regra *Tabela::obterRegra(const QString &nome)
{
 int idx;
 return(dynamic_cast<Regra *>(obterObjeto(nome,OBJETO_REGRA,idx)));
}
//-----------------------------------------------------------
Regra *Tabela::obterRegra(unsigned idx_reg)
{
 return(dynamic_cast<Regra *>(obterObjeto(idx_reg,OBJETO_REGRA)));
}
//-----------------------------------------------------------
Tabela *Tabela::obterTabelaCopia(const QString &nome)
{
 int idx;
 return(dynamic_cast<Tabela *>(obterObjeto(nome,OBJETO_TABELA_BASE,idx)));
}
//-----------------------------------------------------------
Tabela *Tabela::obterTabelaCopia(unsigned idx_tab)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_tab,OBJETO_TABELA_BASE)));
}
//-----------------------------------------------------------
unsigned Tabela::obterNumColunas(void)
{
 return(colunas.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumGatilhos(void)
{
 return(gatilhos.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumRestricoes(void)
{
 return(restricoes.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumIndices(void)
{
 return(indices.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumRegras(void)
{
 return(regras.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumTabelasPai(void)
{
 return(tabelas_pai.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumTabelasCopia(void)
{
 return(tabelas_copia.size());
}
//-----------------------------------------------------------
unsigned Tabela::obterNumObjetos(TipoObjetoBase tipo_obj, bool inc_insporrelacao)
{
 if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO ||
    tipo_obj==OBJETO_GATILHO ||/* tipo_obj==OBJETO_GAT_RESTRICAO ||*/
    tipo_obj==OBJETO_INDICE || tipo_obj==OBJETO_REGRA ||
    tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_TABELA_BASE)
 {
  if(tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_TABELA_BASE)
  {
   if(tipo_obj==OBJETO_TABELA)
    return(tabelas_pai.size());
   else
    return(tabelas_copia.size());
  }
  else
  {
   vector<ObjetoTabela *> *lista=NULL;
   lista=obterListaObjetos(tipo_obj);

   //Faz a contagem apenas dos objetos que não são auto-incluidos (incluídos por relacionamento)
   if(!inc_insporrelacao)
   {
    vector<ObjetoTabela *>::iterator itr, itr_end;
    unsigned qtd=0;

    itr=lista->begin();
    itr_end=lista->end();
    while(itr!=itr_end)
    {
     if(!(*itr)->incluidoPorRelacionamento()) qtd++;
     itr++;
    }

    return(qtd);
   }
   else
    return(lista->size());
  }
 }
 else
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
Restricao *Tabela::obterChavePrimaria(void)
{
 unsigned qtd,i;
 Restricao *pk=NULL, *rest=NULL;

 qtd=restricoes.size();
 for(i=0; i < qtd && !pk; i++)
 {
  rest=dynamic_cast<Restricao *>(restricoes[i]);
  pk=(rest->obterTipoRestricao()==TipoRestricao::primary_key ? rest : NULL);
 }

 return(pk);
}
//-----------------------------------------------------------
bool Tabela::aceitaOids(void)
{
 return(aceita_oids);
}
//-----------------------------------------------------------
bool Tabela::restricaoReferenciaColuna(Coluna *coluna, TipoRestricao tipo_rest)
{
 bool enc=false;
 vector<ObjetoTabela *>::iterator itr, itr_end;
 Restricao *rest=NULL;

 if(coluna)
 {
  itr=restricoes.begin();
  itr_end=restricoes.end();
  while(itr!=itr_end && !enc)
  {
   rest=dynamic_cast<Restricao *>(*itr);
   itr++;
   enc=(rest->obterTipoRestricao()==tipo_rest &&
        (rest->colunaExistente(coluna, Restricao::COLUNA_ORIGEM) ||
         rest->colunaExistente(coluna, Restricao::COLUNA_REFER)));
  }
 }

 return(enc);
}
//-----------------------------------------------------------
QString Tabela::obterDefinicaoObjeto(unsigned tipo_def)
{
 atributos[AtributosParsers::OIDS]=(aceita_oids ? "1" : "");
 definirAtributoColunas(tipo_def);
 definirAtributoRestricoes(tipo_def);
 definirAtributoGatilhos(tipo_def);
 definirAtributoIndices(tipo_def);
 definirAtributoRegras(tipo_def);

 if(tipo_def==ParserEsquema::DEFINICAO_XML)
  definirAtributoPosicao();

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//-----------------------------------------------------------
void Tabela::operator = (Tabela &tabela)
{
 (*dynamic_cast<ObjetoGraficoBase *>(this))=dynamic_cast<ObjetoGraficoBase &>(tabela);
 this->aceita_oids=tabela.aceita_oids;
 definirProtegido(tabela.protegido);
}
//-----------------------------------------------------------
bool Tabela::referenciaObjetoIncRelacao(void)
{
 vector<ObjetoTabela *>::iterator itr, itr_end;
 TipoObjetoBase tipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO };
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
   enc=(*itr)->incluidoPorRelacionamento();
   //Passa para a próxima coluna
   itr++;
  }
 }

 return(enc);
}
//-----------------------------------------------------------
void Tabela::trocarIndicesObjetos(TipoObjetoBase tipo_obj, unsigned idx1, unsigned idx2)
{
 vector<ObjetoTabela *> *lista_obj=NULL;
 vector<ObjetoTabela *>::iterator itr1, itr2;
 ObjetoTabela *obj_aux=NULL;

 try
 {
  if(idx1!=idx2)
  {
   lista_obj=obterListaObjetos(tipo_obj);

   if(idx1 >= lista_obj->size() || idx2 >= lista_obj->size())
    throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   obj_aux=lista_obj->at(idx1);
   itr1=lista_obj->begin() + idx1;
   itr2=lista_obj->begin() + idx2;

   (*itr1)=(*itr2);
   (*itr2)=obj_aux;
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void Tabela::obterReferenciasColuna(Coluna *coluna, vector<ObjetoTabela *> &vet_refs, bool modo_exclusao)
{
 /* Caso a coluna não foi incluída por relacionamento, será verificado se esta
    não está sendo referenciada. Para colunas adicionadas automaticamente por
    relacionamentos, sua remoção é feita sem verificação alguma */
 if(coluna && !coluna->incluidoPorRelacionamento())
 {
  unsigned qtd, i;
  ElementoIndice elem;
  Coluna *col=NULL, *col1=NULL;
  vector<ObjetoTabela *>::iterator itr, itr_end;
  bool enc=false;
  Indice *ind=NULL;
  Restricao *rest=NULL;
  Gatilho *gat=NULL;

  //Verificando se a coluna não é referenciada nos indices
  itr=indices.begin();
  itr_end=indices.end();

  while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !enc)))
  {
   ind=dynamic_cast<Indice *>(*itr);
   itr++;

   qtd=ind->obterNumElementos();
   for(i=0; i < qtd  && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    elem=ind->obterElemento(i);
    col=elem.obterColuna();
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
   rest=dynamic_cast<Restricao *>(*itr);
   itr++;

   col=rest->obterColuna(coluna->obterNome(),true);
   col1=rest->obterColuna(coluna->obterNome(),false);

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
   gat=dynamic_cast<Gatilho *>(*itr);
   itr++;

   qtd=gat->obterNumColunas();
   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !enc)); i++)
   {
    if(gat->obterColuna(i)==coluna)
    {
     enc=true;
     vet_refs.push_back(gat);
    }
   }
  }
 }
}
//***********************************************************
