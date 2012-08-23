#include "classeoperadores.h"
//***********************************************************
ClasseOperadores::ClasseOperadores(void)
{
 tipo_objeto=OBJETO_CLASSE_OPER;
 familia=NULL;
 padrao=false;
 atributos[AtributosParsers::FAMILIA]="";
 atributos[AtributosParsers::ELEMENTOS]="";
 atributos[AtributosParsers::TIPO_INDEXACAO]="";
 atributos[AtributosParsers::TIPO]="";
 atributos[AtributosParsers::PADRAO]="";
}
//-----------------------------------------------------------
ClasseOperadores::~ClasseOperadores(void)
{
 elementos.clear();
}
//-----------------------------------------------------------
void ClasseOperadores::definirTipoDado(TipoPgSQL tipo_dado)
{
 /* Caso se tente atribuir um tipo de dado nulo ao objeto,
    é disparada uma exceção */
 if(tipo_dado==TipoPgSQL::nulo)
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->tipo_dado=tipo_dado;
}
//-----------------------------------------------------------
void ClasseOperadores::definirFamilia(FamiliaOperadores *familia)
{
 this->familia=familia;
}
//-----------------------------------------------------------
void ClasseOperadores::definirTipoIndexacao(TipoIndexacao tipo_index)
{
 this->tipo_index=tipo_index;
}
//-----------------------------------------------------------
void ClasseOperadores::definirPadrao(bool valor)
{
 padrao=valor;
}
//-----------------------------------------------------------
void ClasseOperadores::definirAtributoElementos(unsigned tipo_def)
{
 QString str_elems;
 unsigned i, qtd;

 /* Concatena as definições dos elementos e adicionando
    uma vírgula e uma quebra de linha ao final */
 qtd=elementos.size();
 for(i=0; i < qtd; i++)
 {
  str_elems+=elementos[i].obterDefinicaoObjeto(tipo_def);
  if(tipo_def==ParserEsquema::DEFINICAO_SQL &&
     i < qtd-1) str_elems+=",\n";
 }

 atributos[AtributosParsers::ELEMENTOS]=str_elems;
}
//-----------------------------------------------------------
void ClasseOperadores::adicionarElementoClasse(ElemClasseOperadores elemento)
{
 //Verifica se um elemento igual já não existe na lista de elementos
 if(elementoExiste(elemento))
  throw Excecao(ERR_PGMODELER_INSELEMDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 elementos.push_back(elemento);
}
//-----------------------------------------------------------
void ClasseOperadores::removerElementoClasse(unsigned idx_elem)
{
 /* Caso o índice do elemento a se remover seja superior ao tamanho da lista
    de elementos, é disparada uma exceção */
 if(idx_elem >= elementos.size())
  throw Excecao(ERR_PGMODELER_REFELEMIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove o item da lista referente ao elemento
 elementos.erase(elementos.begin() + idx_elem);
}
//-----------------------------------------------------------
void ClasseOperadores::removerElementosClasse(void)
{
 elementos.clear();
}
//-----------------------------------------------------------
ElemClasseOperadores ClasseOperadores::obterElementoClasse(unsigned idx_elem)
{
 /* Caso o índice do elemento a obtido seja superior ao tamanho da lista
    de elementos, é disparada uma exceção */
 if(idx_elem >= elementos.size())
  throw Excecao(ERR_PGMODELER_REFELEMIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o elemento no índice especificado
 return(elementos[idx_elem]);
}
//-----------------------------------------------------------
bool ClasseOperadores::elementoExiste(ElemClasseOperadores elemento)
{
 bool existe=false;
 vector<ElemClasseOperadores>::iterator itr, itr_end;
 ElemClasseOperadores elem;

 itr=elementos.begin();
 itr_end=elementos.end();

 while(itr!=itr_end && !existe)
 {
  elem=(*itr);
  //Verifica se o elemento atual (itr) é igual ao passado no parametro
  existe=(elem==elemento);
  itr++;
 }

 return(existe);
}
//-----------------------------------------------------------
unsigned ClasseOperadores::obterNumElemClasseOperadores(void)
{
 return(elementos.size());
}
//-----------------------------------------------------------
TipoPgSQL ClasseOperadores::obterTipoDado(void)
{
 return(tipo_dado);
}
//-----------------------------------------------------------
FamiliaOperadores *ClasseOperadores::obterFamilia(void)
{
 return(familia);
}
//-----------------------------------------------------------
TipoIndexacao ClasseOperadores::obterTipoIndexacao(void)
{
 return(tipo_index);
}
//-----------------------------------------------------------
bool ClasseOperadores::classePadrao(void)
{
 return(padrao);
}
//-----------------------------------------------------------
QString ClasseOperadores::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, false));
}
//-----------------------------------------------------------
QString ClasseOperadores::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 definirAtributoElementos(tipo_def);
 atributos[AtributosParsers::TIPO_INDEXACAO]=(~tipo_index);
 atributos[AtributosParsers::PADRAO]=(padrao ? "1" : "");

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  atributos[AtributosParsers::TIPO]=(*tipo_dado);
 else
  atributos[AtributosParsers::TIPO]=tipo_dado.obterDefinicaoObjeto(tipo_def);

 if(familia)
 {
  atributos[AtributosParsers::FAMILIA]=familia->obterNome(true);
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def, forma_reduzida));
}
//***********************************************************
