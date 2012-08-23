#include "indice.h"
//***********************************************************
Indice::Indice(void)
{
 atrib_indice[UNIQUE]=false;
 atrib_indice[CONCORRENTE]=false;
 tipo_objeto=OBJETO_INDICE;
 fator_preenc=90;
 atributos[AtributosParsers::UNIQUE]="";
 atributos[AtributosParsers::CONCORRENTE]="";
 atributos[AtributosParsers::TABELA]="";
 atributos[AtributosParsers::TIPO_INDEXACAO]="";
 atributos[AtributosParsers::COLUNAS]="";
 atributos[AtributosParsers::EXPRESSAO]="";
 atributos[AtributosParsers::FATOR]="";
 atributos[AtributosParsers::CONDICAO]="";
 atributos[AtributosParsers::CLASSE_OPERADORES]="";
 atributos[AtributosParsers::NULOS_PRIMEIRO]="";
 atributos[AtributosParsers::ORDEM_ASCENDENTE]="";
 atributos[AtributosParsers::DECL_DENTRO_TABELA]="";
 atributos[AtributosParsers::ELEMENTOS]="";
 atributos[AtributosParsers::ATUAL_RAPIDA]="";
}
//-----------------------------------------------------------
void Indice::definirAtributoElementos(unsigned tipo_def)
{
 QString str_elem;
 unsigned i, qtd;

 qtd=elementos.size();
 for(i=0; i < qtd; i++)
 {
  str_elem+=elementos[i].obterDefinicaoObjeto(tipo_def);
  if(i < (qtd-1) && tipo_def==ParserEsquema::DEFINICAO_SQL) str_elem+=",";
 }

 atributos[AtributosParsers::ELEMENTOS]=str_elem;
}
//-----------------------------------------------------------
int Indice::elementoExiste(Coluna *coluna)
{
 int qtd, idx;
 Coluna *col=NULL;
 bool enc=false;

 idx=0;
 qtd=elementos.size();

 while(idx < qtd && !enc)
 {
  //Obtém a coluna
  col=elementos[idx].obterColuna();

  if(col && coluna)
   enc=(col==coluna || col->obterNome()==coluna->obterNome());

  if(!enc) idx++;
 }

 if(!enc) idx=-1;
 return(idx);
}
//-----------------------------------------------------------
int Indice::elementoExiste(const QString &expressao)
{
 int qtd, idx;
 bool enc=false;

 idx=0;
 qtd=elementos.size();

 while(idx < qtd && !enc)
 {
  enc=(!expressao.isEmpty() && elementos[idx].obterExpressao()==expressao);
  if(!enc) idx++;
 }

 if(!enc) idx=-1;
 return(idx);
}
//-----------------------------------------------------------
void Indice::adicionarElemento(const QString &expressao, ClasseOperadores *classe_oper, bool ordem_asc, bool nulos_primeiro)
{
 //Caso a expressão esteja vazia, dispara exceção.
 if(expressao.isEmpty())
 {
  throw Excecao(ERR_PGMODELER_ATREXPRINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  ElementoIndice elem;

  //Caso a expressão a ser atribuída ao índice já exista, dispara-se uma exceção
  if(elementoExiste(expressao) >= 0)
   throw Excecao(ERR_PGMODELER_INSELEMDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.definirExpressao(expressao);
  elem.definirClasseOperadores(classe_oper);
  elem.definirAtributo(ElementoIndice::NULOS_PRIMEIRO, nulos_primeiro);
  elem.definirAtributo(ElementoIndice::ORDEM_ASCENDENTE, ordem_asc);

  //Adiciona o elemento ao final da lista de elementos do índice
  elementos.push_back(elem);
 }
}
//-----------------------------------------------------------
void Indice::adicionarElemento(Coluna *coluna, ClasseOperadores *classe_oper, bool ordem_asc, bool nulos_primeiro)
{
 //Caso a coluna não esteja aloca, dispara exceção.
 if(!coluna)
 {
 throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLNAOALOC)
                        .arg(this->obterNome())
                        .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_INDICE)),
               ERR_PGMODELER_ATRCOLNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  ElementoIndice elem;

  //Caso a coluna a ser atribuída ao índice já exista, dispara-se uma exceção
  if(elementoExiste(coluna) >= 0)
   throw Excecao(ERR_PGMODELER_INSCOLDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.definirColuna(coluna);
  elem.definirClasseOperadores(classe_oper);
  elem.definirAtributo(ElementoIndice::NULOS_PRIMEIRO, nulos_primeiro);
  elem.definirAtributo(ElementoIndice::ORDEM_ASCENDENTE, ordem_asc);

  //Adiciona o elemento ao final da lista de elementos do índice
  elementos.push_back(elem);
 }
}
//-----------------------------------------------------------
void Indice::removerElemento(unsigned idx_elem)
{
 vector<ElementoIndice>::iterator itr;

 /* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(idx_elem < elementos.size())
  throw Excecao(ERR_PGMODELER_REFELEMIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  elementos.erase(elementos.begin() + idx_elem);
}
//-----------------------------------------------------------
void Indice::removerElementos(void)
{
 elementos.clear();
}
//-----------------------------------------------------------
void Indice::definirAtributo(unsigned id_atrib, bool valor)
{
 if(id_atrib > ATUAL_RAPIDA)
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 atrib_indice[id_atrib]=valor;
}
//-----------------------------------------------------------
void Indice::definirFatorPreenchimento(unsigned fator)
{
 fator_preenc=fator;
}
//-----------------------------------------------------------
void Indice::definirTipoIndexacao(TipoIndexacao tipo_indexacao)
{
 this->tipo_indexacao=tipo_indexacao;
}
//-----------------------------------------------------------
void Indice::definirExpCondicional(const QString &exp)
{
 exp_condicional=exp;
}
//-----------------------------------------------------------
unsigned Indice::obterFatorPreenchimento(void)
{
 return(fator_preenc);
}
//-----------------------------------------------------------
ElementoIndice Indice::obterElemento(unsigned idx_elem)
{
/* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(idx_elem >= elementos.size())
  throw Excecao(ERR_PGMODELER_REFELEMIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(elementos[idx_elem]);
}
//-----------------------------------------------------------
unsigned Indice::obterNumElementos(void)
{
 return(elementos.size());
}
//-----------------------------------------------------------
bool Indice::obterAtributo(unsigned id_atrib)
{
 if(id_atrib > ATUAL_RAPIDA)
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atrib_indice[id_atrib]);
}
//-----------------------------------------------------------
TipoIndexacao Indice::obterTipoIndexacao(void)
{
 return(tipo_indexacao);
}
//-----------------------------------------------------------
QString Indice::obterExpCondicional(void)
{
 return(exp_condicional);
}
//-----------------------------------------------------------
bool Indice::referenciaColunaIncRelacao(void)
{
 vector<ElementoIndice>::iterator itr, itr_end;
 Coluna *col=NULL;
 bool enc=false;

 /* Primeira lista de elementos é que será varrida
    para isso as referências ao primeiro e ultimo elementos
    serão obtidas */
 itr=elementos.begin();
 itr_end=elementos.end();

 /* Efetua uma iteração verifica se as colunas dos elementos
    foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que a restrição referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !enc)
 {
  //Obtém a coluna
  col=(*itr).obterColuna();

  //Obtém se a coluna foi incluída por relacionamento ou não
  enc=(col && col->incluidoPorRelacionamento());
  //Passa para a próxima coluna
  itr++;
 }

 return(enc);
}
//-----------------------------------------------------------
QString Indice::obterDefinicaoObjeto(unsigned tipo_def)
{
 definirAtributoElementos(tipo_def);
 atributos[AtributosParsers::UNIQUE]=(atrib_indice[UNIQUE] ? "1" : "");
 atributos[AtributosParsers::CONCORRENTE]=(atrib_indice[CONCORRENTE] ? "1" : "");
 atributos[AtributosParsers::ATUAL_RAPIDA]=(atrib_indice[ATUAL_RAPIDA] ? "1" : "");
 atributos[AtributosParsers::TIPO_INDEXACAO]=(~tipo_indexacao);
 atributos[AtributosParsers::CONDICAO]=exp_condicional;

 if(this->tabela_pai)
  atributos[AtributosParsers::TABELA]=this->tabela_pai->obterNome(true);

 atributos[AtributosParsers::FATOR]=QString("%1").arg(fator_preenc);

 /* Caso o índice não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração do índice está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!referenciaColunaIncRelacao())
  atributos[AtributosParsers::DECL_DENTRO_TABELA]="1";

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
