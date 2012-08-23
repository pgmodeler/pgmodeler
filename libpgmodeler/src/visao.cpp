#include "visao.h"
//***********************************************************
Visao::Visao(void) : TabelaBase()
{
 //Definindo configurações inicias para a visão
 tipo_objeto=OBJETO_VISAO;
 atributos[AtributosParsers::DECLARACAO]="";
 atributos[AtributosParsers::REFERENCIAS]="";
 atributos[AtributosParsers::EXP_SELECT]="";
 atributos[AtributosParsers::EXP_FROM]="";
 atributos[AtributosParsers::EXP_SIMPLES]="";
}
//-----------------------------------------------------------
Visao::~Visao(void)
{
 //Limpa a lista de referências da visão
 referencias.clear();

 //Limpa as demais listas
 exp_select.clear();
 exp_from.clear();
 exp_where.clear();
}
//-----------------------------------------------------------
int Visao::obterIndiceReferencia(Referencia &refer)
{
 vector<Referencia>::iterator itr, itr_end;
 bool existe=false;
 int idx=-1;

 /* Varre a lista de referencias da visão para
    comparar os elementos */
 itr=referencias.begin();
 itr_end=referencias.end();

 while(itr!=itr_end && !existe)
 {
  /* Compara o conteúdo do elemento atual (itr) com
     o conteúdo da referência do parâmetro */
  existe=((*itr)==refer);
  itr++;
  idx++;
 }

 /* Caso a referência não exista retorna -1 caso
    contrário retorna o próprio índice */
 if(!existe) idx=-1;
 return(idx);
}
//-----------------------------------------------------------
void Visao::adicionarReferencia(Referencia &refer, unsigned tipo_sql, int id_exp)
{
 int idx;
 vector<unsigned> *vet_idref=NULL;
 Coluna *col=NULL;

 //Verifica se a referência já existe na visão
 idx=obterIndiceReferencia(refer);

 //Caso não seja encontrada (idx = -1)
 if(idx < 0)
 {
  //Insere a nova referência na lista
  referencias.push_back(refer);
  idx=referencias.size()-1;
 }

 /* Selecionando a lista de expressões de acordo o
    parâmetro tipo_sql. */
 if(tipo_sql==Referencia::SQL_REFER_SELECT)
  vet_idref=&exp_select;
 else if(tipo_sql==Referencia::SQL_REFER_FROM)
  vet_idref=&exp_from;
 else
  vet_idref=&exp_where;

 //Insere a referência na posição especificada por id_exp
 if(id_exp >= 0 && id_exp < static_cast<int>(vet_idref->size()))
  vet_idref->insert(vet_idref->begin() + id_exp, static_cast<unsigned>(idx));

 /* Caso se tente inserir uma referência num índice inválido,
    é disparada uma exceção */
 else if(id_exp >= 0 && id_exp >= static_cast<int>(vet_idref->size()))
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Insere a referência no fim da lista de expressões
 else
  vet_idref->push_back(static_cast<unsigned>(idx));

 col=refer.obterColuna();
 if(col && col->incluidoPorRelacionamento() &&
    col->obterIdObjeto() > this->id_objeto)
  this->id_objeto=ObjetoBase::obterIdGlobal();
}
//-----------------------------------------------------------
unsigned Visao::obterNumReferencias(void)
{
 return(referencias.size());
}
//-----------------------------------------------------------
unsigned Visao::obterNumReferencias(unsigned tipo_sql, int tipo_ref)
{
 vector<unsigned> *vet_idref=NULL;

 /* Selecionando a lista de expressões de acordo o
    parâmetro tipo_sql. */
 if(tipo_sql==Referencia::SQL_REFER_SELECT)
  vet_idref=&exp_select;
 else if(tipo_sql==Referencia::SQL_REFER_FROM)
  vet_idref=&exp_from;
 else
  vet_idref=&exp_where;

 /* Caso o tipo de referência não esteja especificado
    retorna o tamanho total do vetor de expressões */
 if(tipo_ref < 0)
  return(vet_idref->size());

 /* Caso contrário, faz a contagem dos elementos da lista
    de acordo com o tipo da referência */
 else
 {
  vector<unsigned>::iterator itr, itr_end;
  unsigned qtd;

  qtd=0;
  itr=vet_idref->begin();
  itr_end=vet_idref->end();
  while(itr!=itr_end)
  {
   /* Caso o tipo da referência atual seja igual ao tipo da referência
      passada pelo parâmetro, incrementa a quantidade */
   if(referencias[(*itr)].obterTipoReferencia()==static_cast<unsigned>(tipo_ref)) qtd++;
   itr++;
  }

  //Retorna a quantidade calculada
  return(qtd);
 }
}
//-----------------------------------------------------------
Referencia Visao::obterReferencia(unsigned id_ref)
{
 /* Caso tente acessar uma referência com índice
    inválido, dispara uma exceção */
 if(id_ref >= referencias.size())
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(referencias[id_ref]);
}
//-----------------------------------------------------------
Referencia Visao::obterReferencia(unsigned id_ref, unsigned tipo_sql)
{
 vector<unsigned> *vet_idref=NULL;
 unsigned idx;

 /* Selecionando a lista de expressões de acordo o
    parâmetro tipo_sql. */
 if(tipo_sql==Referencia::SQL_REFER_SELECT)
  vet_idref=&exp_select;
 else if(tipo_sql==Referencia::SQL_REFER_FROM)
  vet_idref=&exp_from;
 else
  vet_idref=&exp_where;

 //Retorna a referência na posição indicada por id_ref
 idx=vet_idref->at(id_ref);
 return(referencias[idx]);
}
//-----------------------------------------------------------
void Visao::removerReferencia(unsigned id_ref)
{
 vector<unsigned> *vet_idref[3]={&exp_select, &exp_from, &exp_where};
 vector<unsigned>::iterator itr, itr_aux, itr_end;
 unsigned i;

 /* Dispara uma exceção caso se tente remover uma referência
    com índice inválido */
 if(id_ref >= referencias.size())
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Varre as três listas de expressões para verificar se o
    a referência não está sendo usada em uma delas */
 for(i=0; i < 3; i++)
 {
  //Obtém o ínicio e o fim da lista
  itr=vet_idref[i]->begin();
  itr_end=vet_idref[i]->end();

  //Varre a lista
  while(itr!=itr_end && !vet_idref[i]->empty())
  {
   /* Caso a referencia atual seja igual à referência
      a ser removeda, o item da lista de expressão atual
      o qual contém a referência a ser excluída será
      removido. */
   if(referencias[*itr]==referencias[id_ref])
    vet_idref[i]->erase(itr);

   itr++;
  }
 }

 //Remove a referência da lista
 referencias.erase(referencias.begin() + id_ref);
}
//-----------------------------------------------------------
void Visao::removerReferencias(void)
{
 referencias.clear();
 exp_select.clear();
 exp_from.clear();
 exp_where.clear();
}
//-----------------------------------------------------------
void Visao::removerReferencia(unsigned id_exp, unsigned tipo_sql)
{
 vector<unsigned> *vet_idref=NULL;

 /* Selecionando a lista de expressões de acordo o
    parâmetro tipo_sql. */
 if(tipo_sql==Referencia::SQL_REFER_SELECT)
  vet_idref=&exp_select;
 else if(tipo_sql==Referencia::SQL_REFER_FROM)
  vet_idref=&exp_from;
 else
  vet_idref=&exp_where;

 if(id_exp >= vet_idref->size())
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove a referência da lista de expressão
 vet_idref->erase(vet_idref->begin() + id_exp);
}
//-----------------------------------------------------------
int Visao::obterIndiceReferencia(Referencia &ref, unsigned tipo_sql)
{
 vector<unsigned> *vet_idref=NULL;
 vector<unsigned>::iterator itr, itr_aux, itr_end;
 int idx_ref;
 bool enc=false;

  /* Selecionando a lista de expressões de acordo o
    parâmetro tipo_sql. */
 if(tipo_sql==Referencia::SQL_REFER_SELECT)
  vet_idref=&exp_select;
 else if(tipo_sql==Referencia::SQL_REFER_FROM)
  vet_idref=&exp_from;
 else
  vet_idref=&exp_where;

 idx_ref=obterIndiceReferencia(ref);
 itr=vet_idref->begin();
 itr_end=vet_idref->end();

 while(itr!=itr_end && !enc)
 {
  enc=(static_cast<int>(*itr)==idx_ref);
  if(!enc) itr++;
 }

 if(!enc)
  return(-1);
 else
  return(itr-vet_idref->begin());
}
//-----------------------------------------------------------
void Visao::definirAtributoDeclaracao(void)
{
 QString decl;

 if(exp_select.size() > 0)
 {
  vector<unsigned> *vet_ref[3]={&exp_select, &exp_from, &exp_where};
  vector<unsigned>::iterator itr, itr_end;
  QString palavras[3]={"SELECT ", "\n FROM ", "\n WHERE "};
  unsigned i, qtd, idx, tipo_sql[3]={Referencia::SQL_REFER_SELECT,
                                     Referencia::SQL_REFER_FROM,
                                     Referencia::SQL_REFER_WHERE};

  //Varre as listas de expressões
  for(i=0; i < 3; i++)
  {
   //Caso a lista não esteja vazia
   if(vet_ref[i]->size() > 0)
   {
    //Concatena a palavra referente à lista de expressões
    decl+=palavras[i];

    itr=vet_ref[i]->begin();
    itr_end=vet_ref[i]->end();
    while(itr!=itr_end)
    {
     /* Concatena à declaração da visão a sql da referência atual
        especificando o tipo da sql a ser gerada */
     idx=(*itr);
     decl+=referencias[idx].obterDefinicaoSQL(tipo_sql[i]);
     itr++;
    }

    if(tipo_sql[i]==Referencia::SQL_REFER_SELECT ||
       tipo_sql[i]==Referencia::SQL_REFER_FROM)
    {
     //Removendo os espaços e vírgula finais
     qtd=decl.size();
     if(decl[qtd-2]==',')
       //decl.erase(qtd-2);
       decl.remove(qtd-2,2);
    }
   }
  }
 }
 atributos[AtributosParsers::DECLARACAO]=decl;
}
//-----------------------------------------------------------
void Visao::definirAtributoReferencias(void)
{
 QString str_aux;
 QString vet_atrib[]={ AtributosParsers::EXP_SELECT,
                       AtributosParsers::EXP_FROM,
                       AtributosParsers::EXP_SIMPLES };
 vector<unsigned> *vet_exp[]={&exp_select, &exp_from, &exp_where};
 int qtd, i, i1;

 //Obtém a definição XMl das referências
 qtd=referencias.size();
 for(i=0; i < qtd; i++)
  str_aux+=referencias[i].obterDefinicaoXML();
 atributos[AtributosParsers::REFERENCIAS]=str_aux;

 /* Este bloco concatena os indices de referências
    em cada vetor de expressão separando-os por vírgula */
 for(i=0; i < 3; i++)
 {
  str_aux="";
  qtd=vet_exp[i]->size();
  for(i1=0; i1 < qtd; i1++)
  {
   str_aux+=QString("%1").arg(vet_exp[i]->at(i1));
   if(i1 < qtd-1) str_aux+=",";
  }
  atributos[vet_atrib[i]]=str_aux;
 }
}
//-----------------------------------------------------------
bool Visao::referenciaColunaIncRelacao(void)
{
 Coluna *coluna=NULL;
 unsigned qtd, i;
 bool enc=false;

 qtd=referencias.size();

 for(i=0; i < qtd && !enc; i++)
 {
  coluna=referencias[i].obterColuna();
  enc=(coluna && coluna->incluidoPorRelacionamento());
 }

 return(enc);
}
//-----------------------------------------------------------
bool Visao::referenciaTabela(Tabela *tab)
{
 Tabela *tab_aux=NULL;
 unsigned qtd, i;
 bool enc=false;

 qtd=referencias.size();

 for(i=0; i < qtd && !enc; i++)
 {
  tab_aux=referencias[i].obterTabela();
  enc=(tab_aux && (tab_aux == tab));
 }

 return(enc);
}
//-----------------------------------------------------------
bool Visao::referenciaColuna(Coluna *col)
{
 unsigned qtd, i;
 bool enc=false;

 if(col)
 {
  qtd=referencias.size();
  for(i=0; i < qtd && !enc; i++)
   enc=(col==referencias[i].obterColuna());
 }
 return(enc);
}
//-----------------------------------------------------------
QString Visao::obterDefinicaoObjeto(unsigned tipo_def)
{
 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  definirAtributoDeclaracao();
 else
 {
  definirAtributoPosicao();
  definirAtributoReferencias();
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//-----------------------------------------------------------
void Visao::operator = (Visao &visao)
{
 (*dynamic_cast<ObjetoGraficoBase *>(this))=reinterpret_cast<ObjetoGraficoBase &>(visao);

 this->referencias=visao.referencias;
 this->exp_select=visao.exp_select;
 this->exp_from=visao.exp_from;
 this->exp_where=visao.exp_where;
}
//***********************************************************
