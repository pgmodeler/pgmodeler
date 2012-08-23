#include "sequencia.h"
//***********************************************************
const QString Sequencia::VALOR_MAX_POSITIVO="+9223372036854775807";
const QString Sequencia::VALOR_MAX_NEGATIVO="-9223372036854775808";
//-----------------------------------------------------------
Sequencia::Sequencia(void)
{
 tipo_objeto=OBJETO_SEQUENCIA;
 ciclica=false;
 incremento=inicio=cache="1";
 valor_min="0";
 valor_max=VALOR_MAX_POSITIVO;
 coluna=NULL;

 atributos[AtributosParsers::INCREMENTO]="";
 atributos[AtributosParsers::VALOR_MINIMO]="";
 atributos[AtributosParsers::VALOR_MAXIMO]="";
 atributos[AtributosParsers::INICIO]="";
 atributos[AtributosParsers::CACHE]="";
 atributos[AtributosParsers::CICLICA]="";
 atributos[AtributosParsers::POSSUIDORA]="";
}
//-----------------------------------------------------------
bool Sequencia::valorNulo(const QString &valor)
{
 unsigned i, qtd;
 bool nulo;

 i=0;
 nulo=true;
 qtd=valor.size();
 while(i < qtd && nulo)
 {
  nulo=(valor[i]=='0' || valor[i]=='+' || valor[i]=='-');
  i++;
 }
 return(nulo);
}
//-----------------------------------------------------------
bool Sequencia::valorValido(const QString &valor)
{
 /*
  Para que um valor seja válido o mesmo deve ou não iniciar com
  operador + ou - ser constituído apenas de números. E o seu
  tamanho não deve ultrapassar o tamanho da constante VALOR_MAX_POSITIVO
 */
 if(valor.size() > VALOR_MAX_POSITIVO.size())
  return(false);
 else
 {
  unsigned i, qtd;
  bool oper=false, num=false, valido=true;

  qtd=valor.size();
  for(i=0; i < qtd && valido; i++)
  {
   if((valor[i]=='-' || valor[i]=='+') && !num)
   {
    if(!oper) oper=true;
   }
   else if((valor[i]>='0' && valor[i]<='9'))
   {
    if(!num) num=true;
   }
   else valido=false;
  }

  if(!num) valido=false;
  return(valido);
 }
}
//-----------------------------------------------------------
QString Sequencia::formatarValor(const QString &valor)
{
 QString valor_fmt;

 //Verifica se o valor é válido
 if(valorValido(valor))
 {
  unsigned i, qtd, qtd_neg;

  i=qtd_neg=0;
  qtd=valor.size();
  /* Conta a quantidade de operadores negativo, pois
     dependendo da quantidade o mesmo pode interferir
     no sinal do número */
  while((valor[i]=='+' || valor[i]=='-') && i < qtd)
  {
   if(valor[i]=='-') qtd_neg++;
   i++;
  }

  //Caso a quantidade de negativos seja ímpar o número será negativo
  if(qtd_neg % 2 != 0) valor_fmt+="-";
  //valor_fmt+=valor.substr(i, qtd);
  valor_fmt+=valor.mid(i, qtd);
 }

 return(valor_fmt);
}
//-----------------------------------------------------------
int Sequencia::compararValores(QString valor1, QString valor2)
{
 if(valor1==valor2)
  return(0);
 else
 {
  char ops[2];
  unsigned i, idx, qtd;
  QString *vet_valores[2]={&valor1, &valor2}, valor_aux;

  for(i=0; i < 2; i++)
  {
   //Obtém o sinal do número
   ops[i]=vet_valores[i]->at(0).toAscii();
   //Caso não possua sinal, um + será adicionado
   if(ops[i]!='-' && ops[i]!='+') ops[i]='+';

   //Obtém o restante do número sem o sinal
   idx=1;
   qtd=vet_valores[i]->size();
   while(idx < qtd)
   {
    if(vet_valores[i]->at(idx)!='0')
    valor_aux+=vet_valores[i]->at(idx);
    idx++;
   }
   (*vet_valores[i])=valor_aux;
   valor_aux="";
  }

  //Compara os sinais e os valores, caso sejam iguais retorna 0
  if(ops[0]==ops[1] && valor1==valor2)
   return(0);
  /* Caso os operadores sejam iguais e o valor1 for menor que o valor2 ou
     se os sinais sejam diferentes */
  else if((ops[0]=='-' && ops[1]=='-' && valor1 > valor2) ||
          (ops[0]=='+' && ops[1]=='+' && valor1 < valor2) ||
          (ops[0]=='-' && ops[1]=='+'))
   //Retorna -1 indicando que o valor 1 é menor que o valor 2
   return(-1);
  else
   //Retorna 1 indicando que o valor2 é maior que valor1
   return(1);
 }
}
//-----------------------------------------------------------
void Sequencia::definirEsquema(ObjetoBase *esquema)
{
 Tabela *tabela=NULL;

 //Caso a coluna possuidora da sequencia exista
 if(coluna)
 {
  //Obtém a tabela pai da coluna
  tabela=dynamic_cast<Tabela *>(coluna->obterTabelaPai());

  //Verifica se o esquema sendo atribuído à seqüência é o mesmo da tabela possuidora
  if(tabela && tabela->obterEsquema()!=esquema)
    throw Excecao(ERR_PGMODELER_ATRESQDIFSEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 //Atribui o esquema à sequencia
 ObjetoBase::definirEsquema(esquema);
}
//-----------------------------------------------------------
void Sequencia::definirCiclica(bool valor)
{
 ciclica=valor;
}
//-----------------------------------------------------------
void Sequencia::definirValores(QString vmin, QString vmax, QString inc, QString inicio, QString cache)
{
 vmin=formatarValor(vmin);
 vmax=formatarValor(vmax);
 inc=formatarValor(inc);
 inicio=formatarValor(inicio);
 cache=formatarValor(cache);

 /* Caso algum atributo após a formatação esteja vazio quer dizer
    que seu valor é invalido, sendo assim uma exceção é disparada*/
 if(vmin==""   || vmax=="" || inc=="" ||
    inicio=="" || cache=="")
  throw Excecao(ERR_PGMODELER_ATRESQVALORINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(compararValores(vmin,vmax) > 0)
  throw Excecao(ERR_PGMODELER_ATRESQVALORMININV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(compararValores(inicio, vmin) < 0 ||
         compararValores(inicio, vmax) > 0)
  throw Excecao(ERR_PGMODELER_ATRESQVALORINIINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(valorNulo(inc))
  throw Excecao(ERR_PGMODELER_ATRESQINCNULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(valorNulo(cache))
  throw Excecao(ERR_PGMODELER_ATRESQCACHENULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->valor_min=vmin;
 this->valor_max=vmax;
 this->incremento=inc;
 this->cache=cache;
 this->inicio=inicio;
}
//-----------------------------------------------------------
void Sequencia::definirPossuidora(Tabela *tabela, const QString &nome_coluna)
{
 if(!tabela || nome_coluna=="")
  this->coluna=NULL;
 else if(tabela)
 {
  // Verifica se a tabela não pertence ao mesmo esquema da sequencia.
  //   Caso não pertença, dispara uma exceção.
  if(tabela->obterEsquema()!=this->esquema)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRESQDIFTAB).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRESQDIFTAB,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    /* Verifica se a tabela não pertence ao mesmo dono da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(tabela->obterDono()!=this->dono)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRDONODIFTAB).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRDONODIFTAB,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Obtém a coluna da tabela com base no nome passado
  this->coluna=tabela->obterColuna(nome_coluna);

  if(this->coluna && this->coluna->incluidoPorRelacionamento() &&
     this->coluna->obterIdObjeto() > this->id_objeto)
   this->id_objeto=ObjetoBase::obterIdGlobal();


  //Caso a coluna não exista
  if(!this->coluna)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLPOSINDEF).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRCOLPOSINDEF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
void Sequencia::definirPossuidora(Coluna *coluna)
{
 Tabela *tabela=NULL;

 if(!coluna)
  this->coluna=NULL;
 else
 {
  tabela=dynamic_cast<Tabela *>(coluna->obterTabelaPai());

  //CAso a coluna possuidor não seja de uma tabela
  if(!tabela)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLPOSNAORELAC).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRCOLPOSNAORELAC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verifica se a tabela não pertence ao mesmo esquema da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(tabela->obterEsquema()!=this->esquema)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRESQDIFTAB).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRESQDIFTAB,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verifica se a tabela não pertence ao mesmo dono da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(tabela->obterDono()!=this->dono)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRDONODIFTAB).arg(this->obterNome(true)),
                 ERR_PGMODELER_ATRDONODIFTAB,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  this->coluna=coluna;

  if(coluna && coluna->incluidoPorRelacionamento() &&
     coluna->obterIdObjeto() > this->id_objeto)
   this->id_objeto=ObjetoBase::obterIdGlobal();
 }
}
//-----------------------------------------------------------
bool Sequencia::referenciaColunaIncRelacao(void)
{
 return(coluna && coluna->incluidoPorRelacionamento());
}
//-----------------------------------------------------------
bool Sequencia::sequenciaCiclica(void)
{
 return(ciclica);
}
//-----------------------------------------------------------
QString Sequencia::obterValorMax(void)
{
 return(valor_max);
}
//-----------------------------------------------------------
QString Sequencia::obterValorMin(void)
{
 return(valor_min);
}
//-----------------------------------------------------------
QString Sequencia::obterCache(void)
{
 return(cache);
}
//-----------------------------------------------------------
QString Sequencia::obterIncremento(void)
{
 return(incremento);
}
//-----------------------------------------------------------
QString Sequencia::obterInicio(void)
{
 return(inicio);
}
//-----------------------------------------------------------
Coluna *Sequencia::obterPossuidora(void)
{
 return(coluna);
}
//-----------------------------------------------------------
QString Sequencia::obterDefinicaoObjeto(unsigned tipo_def)
{
 QString str_aux;
 Tabela *tabela=NULL;

 //Caso haja uma coluna possuidora
 if(coluna)
 {
  tabela=dynamic_cast<Tabela *>(coluna->obterTabelaPai());
  /* Formata o atributo possuidora como sendo o nome da tabela
     e a coluna possuidora */
  str_aux=tabela->obterNome(true) + "." + coluna->obterNome(true);
 }
 atributos[AtributosParsers::POSSUIDORA]=str_aux;

 atributos[AtributosParsers::INCREMENTO]=incremento;
 atributos[AtributosParsers::VALOR_MINIMO]=valor_min;
 atributos[AtributosParsers::VALOR_MAXIMO]=valor_max;
 atributos[AtributosParsers::INICIO]=inicio;
 atributos[AtributosParsers::CACHE]=cache;
 atributos[AtributosParsers::CICLICA]=(ciclica ? "1" : "");

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
