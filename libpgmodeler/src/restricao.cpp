#include "restricao.h"

Restricao::Restricao(void)
{
 tabela_ref=NULL;
 tipo_objeto=OBJETO_RESTRICAO;
 postergavel=false;
 fator_preenc=100;

 atributos[AtributosParsers::REST_PK]="";
 atributos[AtributosParsers::REST_FK]="";
 atributos[AtributosParsers::REST_CK]="";
 atributos[AtributosParsers::REST_UQ]="";
 atributos[AtributosParsers::TABELA_REF]="";
 atributos[AtributosParsers::COLUNAS_ORIGEM]="";
 atributos[AtributosParsers::COLUNAS_DESTINO]="";
 atributos[AtributosParsers::ACAO_DELETE]="";
 atributos[AtributosParsers::ACAO_UPDATE]="";
 atributos[AtributosParsers::EXPRESSAO]="";
 atributos[AtributosParsers::TIPO]="";
 atributos[AtributosParsers::TIPO_COMPARACAO]="";
 atributos[AtributosParsers::TIPO_POSTERGACAO]="";
 atributos[AtributosParsers::POSTERGAVEL]="";
 atributos[AtributosParsers::TABELA]="";
 atributos[AtributosParsers::DECL_DENTRO_TABELA]="";
 atributos[AtributosParsers::FATOR]="";
}

Restricao::~Restricao(void)
{
 removerColunas();
}

void Restricao::definirTipo(TipoRestricao tipo)
{
 this->tipo=tipo;
}

void Restricao::definirTipoAcao(TipoAcao tipo, bool upd)
{
 //Se upd==true o tipo de ação no update é que será definido
 if(upd)
  this->acao_upd=tipo;
 else
  this->acao_del=tipo;
}

void Restricao::definirExpChecagem(const QString &exp)
{
 exp_checagem=exp;
}

bool Restricao::colunaExistente(Coluna *coluna, unsigned tipo_coluna)
{
 vector<Coluna *>::iterator itr, itr_end;
 Coluna *col_aux=NULL;
 bool enc=false;

 //Caso a coluna a ser buscada não esteja aloca, dispara uma exceção
 if(!coluna)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(tipo_coluna==COLUNA_ORIGEM)
 {
  itr=colunas.begin();
  itr_end=colunas.end();
 }
 else
 {
  itr=colunas_ref.begin();
  itr_end=colunas_ref.end();
 }

 /* Varre a lista de colunas selecionada verificando se o nome da
    coluna é igual ao nome de uma das colunas da lista ou mesmo
    se os endereços das colunas envolvidas são iguais */
 while(itr!=itr_end && !enc)
 {
  col_aux=(*itr);
  enc=(col_aux==coluna || col_aux->obterNome()==coluna->obterNome());
  itr++;
 }

 return(enc);
}

void Restricao::adicionarColuna(Coluna *coluna, unsigned tipo_coluna)
{
 //Caso a coluna não esteja aloca, dispara exceção.
 if(!coluna)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLNAOALOC)
                        .arg(QString::fromUtf8(this->obterNome()))
                        .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_RESTRICAO)),
                 ERR_PGMODELER_ATRCOLNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(tipo!=TipoRestricao::check)
 {
  //Só adiciona a coluna em uma das lista caso a mesma não exista em uma delas
  if(!colunaExistente(coluna,tipo_coluna))
  {
   //Caso a coluna a ser atribuida seja da lista de colunas de destino
   if(tipo_coluna==COLUNA_REFER)
    //Insere a coluna na lista de destino
    colunas_ref.push_back(coluna);
   else
    colunas.push_back(coluna);
  }
 }
}

void Restricao::definirEspacoTabela(EspacoTabela *espacotabela)
{
 try
 {
  if(espacotabela &&
     tipo!=TipoRestricao::primary_key &&
     tipo!=TipoRestricao::unique)
   throw Excecao(ERR_PGMODELER_ATRESPTABRESTTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  ObjetoBase::definirEspacoTabela(espacotabela);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Restricao::definirAtributoColunas(unsigned tipo_coluna, unsigned tipo_def, bool inc_insporrelacao)
{
 vector<Coluna *> *vet_col=NULL;
 Coluna *col;
 QString str_cols, atrib;
 unsigned i, qtd;
 bool formatar=(tipo_def==ParserEsquema::DEFINICAO_SQL);

 /* Caso a coluna selecionada seja a de destino,
    obtém a lista de colunas de destino e marca
    que o atributo a ser configurado é do de
    colunas da tabela de destino */
 if(tipo_coluna==COLUNA_REFER)
 {
  vet_col=&colunas_ref;
  atrib=AtributosParsers::COLUNAS_DESTINO;
 }
 else
 {
  vet_col=&colunas;
  atrib=AtributosParsers::COLUNAS_ORIGEM;
 }

 /* Varre a lista de colunas contatenando os nomes
    dos elementos separando-os por vírgula */
 qtd=vet_col->size();
 for(i=0; i < qtd; i++)
 {
  col=vet_col->at(i);

  /* No caso de definição XML as colunas protegidas (adicionaa �  restrição
     por relacionamento) não podem ser incluídas pois estas serão inseridas
     na restrição no momento da criação do relacionamento a partir do XML respectivo
     por isso o parâmetro 'inc_insporrelacao' pode ser usado para resolver esse caso. */
  if((tipo_def==ParserEsquema::DEFINICAO_SQL) ||
     ((tipo_def==ParserEsquema::DEFINICAO_XML) &&
      ((inc_insporrelacao && col->incluidoPorRelacionamento()) ||
       (inc_insporrelacao && !col->incluidoPorRelacionamento()) ||
       (!inc_insporrelacao && !col->incluidoPorRelacionamento()))))
  {
   str_cols+=col->obterNome(formatar);
   str_cols+=",";
  }
 }

 str_cols.remove(str_cols.size()-1,1);
 atributos[atrib]=str_cols;
}

void Restricao::definirTabReferenciada(ObjetoBase *tab_ref)
{
 this->tabela_ref=tab_ref;
}

void Restricao::definirTipoPostergacao(TipoPostergacao tipo)
{
 tipo_postergacao=tipo;
}

void Restricao::definirPostergavel(bool valor)
{
 postergavel=valor;
}

void Restricao::definirTipoComparacao(TipoComparacao tipo)
{
 tipo_comp=tipo;
}

void Restricao::definirFatorPreenchimento(unsigned fator)
{
 if(fator < 10) fator=10;
 fator_preenc=fator;
}

unsigned Restricao::obterFatorPreenchimento(void)
{
 return(fator_preenc);
}

TipoRestricao Restricao::obterTipoRestricao(void)
{
 return(tipo);
}

TipoAcao Restricao::obterTipoAcao(bool upd)
{
 //Se upd==true o tipo de ação no update é que será retornado
 if(upd)
  return(acao_upd);
 else
  return(acao_del);
}

QString Restricao::obterExpChecagem(void)
{
 return(exp_checagem);
}

Coluna *Restricao::obterColuna(unsigned idx_col, unsigned tipo_coluna)
{
 vector<Coluna *> *lista_col=NULL;

 //Obtendo a lista de colunas de acorodo com o tipo de coluna selecionado
 lista_col=(tipo_coluna==COLUNA_ORIGEM ? &colunas : &colunas_ref);

 /* Caso o índice de coluna a ser obtido seja inválido, um erro
    será retornado */
 if(idx_col>=lista_col->size())
  throw Excecao(ERR_PGMODELER_REFCOLIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna a coluna no índice especificado
  return(lista_col->at(idx_col));
}

Coluna *Restricao::obterColuna(const QString &nome, unsigned tipo_coluna)
{
 bool enc=false;
 vector<Coluna *> *lista_col=NULL;
 vector<Coluna *>::iterator itr_col, itr_end_col;

 //Obtém a lista de colunas de acordo com o tipo
 lista_col=(tipo_coluna==COLUNA_ORIGEM? &colunas : &colunas_ref);

 //Obtém as referencias para o primeiro e o ultimo elemento da lista de colunas
 itr_col=lista_col->begin();
 itr_end_col=lista_col->end();

 //Varre a lista de colunas verificando se existe alguma ocorrencia do nome passado
 while(itr_col!=itr_end_col)
 {
  enc=((*itr_col)->obterNome()==nome);
  //Caso o no não seja encontrado passa para o próximo elemento
  if(!enc) itr_col++;
  else break; //Caso seja encontrado, encerra a execuação da varredura
 }

 //caso seja encontrada, retorna a coluna senão retorna nulo
 if(enc) return(*itr_col);
 else return(NULL);
}

ObjetoBase *Restricao::obterTabReferenciada(void)
{
 return(tabela_ref);
}

unsigned Restricao::obterNumColunas(unsigned tipo_coluna)
{
 if(tipo_coluna==COLUNA_REFER)
  return(colunas_ref.size());
 else
  return(colunas.size());
}

void Restricao::removerColunas(void)
{
 colunas.clear();
 colunas_ref.clear();
}

void Restricao::removerColuna(const QString &nome, unsigned tipo_coluna)
{
 vector<Coluna *>::iterator itr, itr_end;
 vector<Coluna *> *cols=NULL;
 Coluna *col=NULL;

 //Se col_dest==true, a lista a ser pesquisada será a de destino
 if(tipo_coluna==COLUNA_REFER)
  //Selecionando a lista de destino para pesquisa
  cols=&colunas_ref;
 else
  cols=&colunas;

 //Obtém a referência ao primeiro elemento da lista selecionada
 itr=cols->begin();
 //Obtém a referência ao ultimo elemento da lista selecionada
 itr_end=cols->end();

 /* Efetua um iteração comparando o nome de cada coluna da lista
    com o nome que se deseja encontrar */
 while(itr!=itr_end)
 {
  col=(*itr); //Obtém a coluna

  //Caso o nome da coluna coincida com o nome a se localizar
  if(col->obterNome()==nome)
  {
   //Remove o elemento da lista
   cols->erase(itr);
   break;
  }
  else itr++; //Passa para a próxima coluna
 }
}

TipoPostergacao Restricao::obterTipoPostergacao(void)
{
 return(tipo_postergacao);
}

bool Restricao::restricaoPostergavel(void)
{
 return(postergavel);
}

bool Restricao::referenciaColunaIncRelacao(void)
{
 vector<Coluna *>::iterator itr, itr_end;
 Coluna *col=NULL;
 bool enc=false;

 /* Primeira lista de colunas da origem é que será varrida
    para isso as referências ao primeiro e ultimo elementos
    serão obtidas */
 itr=colunas.begin();
 itr_end=colunas.end();

 /* Efetua uma iteração verifica se as colunas da lista
    atual foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que a restrição referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !enc)
 {
  //Obtém a coluna
  col=(*itr);
  //Obtém se a coluna foi incluída por relacionamento ou não
  enc=col->incluidoPorRelacionamento();
  //Passa para a próxima coluna
  itr++;

  /* Caso a lista de colunas de origem foi completamente varrida
     e nenhuma das colunas desta vieram de relacionamentos, a
     lista de colunas referenciadas é que será varrida com o
     mesmo intuito */
  if(itr==itr_end && itr_end!=colunas_ref.end() && !enc)
  {
   itr=colunas_ref.begin();
   itr_end=colunas_ref.end();
  }
 }

 return(enc);
}

TipoComparacao Restricao::obterTipoComparacao(void)
{
 return(tipo_comp);
}

QString Restricao::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(obterDefinicaoObjeto(tipo_def, false));
}

QString Restricao::obterDefinicaoObjeto(unsigned tipo_def, bool inc_insporrelacao)
{
 QString atrib;

 atributos[AtributosParsers::REST_PK]="";
 atributos[AtributosParsers::REST_FK]="";
 atributos[AtributosParsers::REST_CK]="";
 atributos[AtributosParsers::REST_UQ]="";

 switch(!tipo)
 {
  case TipoRestricao::check:
   atrib=AtributosParsers::REST_CK;
  break;
  case TipoRestricao::primary_key:
   atrib=AtributosParsers::REST_PK;
  break;
  case TipoRestricao::foreign_key:
   atrib=AtributosParsers::REST_FK;
  break;
  case TipoRestricao::unique:
   atrib=AtributosParsers::REST_UQ;
  break;
 }
 atributos[atrib]="1";

 atributos[AtributosParsers::TIPO]=atrib;
 atributos[AtributosParsers::ACAO_UPDATE]=(~acao_upd);
 atributos[AtributosParsers::ACAO_DELETE]=(~acao_del);
 atributos[AtributosParsers::EXPRESSAO]=exp_checagem;

 if(tipo!=TipoRestricao::check)
 {
  definirAtributoColunas(COLUNA_ORIGEM, tipo_def, inc_insporrelacao);

  /* Só gera a definição das colunas referenciadas da chave estrangeira
     caso o número de colunas da origem e destino sejam iguais, isso significa
     que a chave está configurada corretamente, caso contrário não gera o atributo
     forçando o parser de esquemas a retornar um erro pois a chave estrangeira está
     mal configurada. */
  if(tipo==TipoRestricao::foreign_key && colunas.size() == colunas_ref.size())
   definirAtributoColunas(COLUNA_REFER, tipo_def, inc_insporrelacao);
 }

 atributos[AtributosParsers::TABELA_REF]=(tabela_ref ? tabela_ref->obterNome(true) : "");
 atributos[AtributosParsers::POSTERGAVEL]=(postergavel ? "1" : "");
 atributos[AtributosParsers::TIPO_COMPARACAO]=(~tipo_comp);
 atributos[AtributosParsers::TIPO_POSTERGACAO]=(~tipo_postergacao);

 if(this->tabela_pai)
  atributos[AtributosParsers::TABELA]=this->tabela_pai->obterNome(true);

 /* Caso a restrição não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração da restrição está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!referenciaColunaIncRelacao() || tipo==TipoRestricao::primary_key)
  atributos[AtributosParsers::DECL_DENTRO_TABELA]="1";

 if(tipo==TipoRestricao::primary_key || tipo==TipoRestricao::unique)
  atributos[AtributosParsers::FATOR]=QString("%1").arg(fator_preenc);
 else
  atributos[AtributosParsers::FATOR]="";

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}

