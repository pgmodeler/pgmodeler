#include "referencia.h"

Referencia::Referencia(void)
{
 this->tabela=NULL;
 this->coluna=NULL;
}

Referencia::Referencia(Tabela *tabela, Coluna *coluna, const QString &alias_tab, const QString &alias_col)
{
 if(!tabela)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o alias atribuido   tabela/expressão ou coluna seja inválido
    de acordo com a regra de nomenclatura do PostgreSQL */
 else if((!alias_tab.isEmpty() && !ObjetoBase::nomeValido(alias_tab)) ||
         (!alias_col.isEmpty() && !ObjetoBase::nomeValido(alias_col)))
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso se tente criar uma referência a uma coluna cuja tabela pai seja
    diferente da tabela informada no parâmetro */
 else if(coluna && coluna->obterTabelaPai()!=tabela)
  throw Excecao(ERR_PGMODELER_ATROBJRELAC ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui os parâmetros aos atributos do objeto
 this->tabela=tabela;
 this->coluna=coluna;
 this->alias=alias_tab;
 this->alias_coluna=alias_col;
}

Referencia::Referencia(const QString &expressao, const QString &alias_exp)
{
 if(expressao=="")
  throw Excecao(ERR_PGMODELER_ATREXPRINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o alias da expressão seja inválido de acordo com as regras de
    nomenclatura do PostgreSQL */
 else if(!ObjetoBase::nomeValido(alias_exp))
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 tabela=NULL;
 coluna=NULL;

 //Atribui os parâmetros aos atributos do objeto
 alias=alias_exp;
 this->expressao=expressao;
}

Tabela *Referencia::obterTabela(void)
{
 return(tabela);
}

Coluna *Referencia::obterColuna(void)
{
 return(coluna);
}

QString Referencia::obterAliasColuna(void)
{
 return(alias_coluna);
}

QString Referencia::obterAlias(void)
{
 return(alias);
}

QString Referencia::obterExpressao(void)
{
 return(expressao);
}

unsigned Referencia::obterTipoReferencia(void)
{
 /* Caso o atributo expressão esteja preenchido o tipo do
    objeto será referência a uma expressão */
 if(expressao=="")
  return(REFER_COLUNA);
 else
  return(REFER_EXPRESSAO);
}

QString Referencia::obterDefinicaoSQL(unsigned tipo_sql)
{
 QString def_sql, nome_tab, nome_cols;
 unsigned tipo_refer;

 //Obtém o tipo de referência do objeto this
 tipo_refer=obterTipoReferencia();

 /* Caso seja uma referênci�  parte SELECT-FROM,
    formata a SQL do objeto de acordo com este tipo */
 if(tipo_sql==SQL_REFER_SELECT)
 {
  //Caso seja um referênci�  objetos (colunas, esquemas, tabelas, alias)
  if(tipo_refer==REFER_COLUNA)
  {
   /*
    Definição SQL gerada:
    {NOME_TABELA|ALIAS_TABELA}.{NOME_COLUNA | *} [AS ALIAS_COLUNA]
   */

   /* Caso um alias de tabela não esteja definido,
      o nome da própria tabela será usada */
   if(alias=="")
    nome_tab=tabela->obterNome(true);
   else
    //Caso haja um alias de tabela, formata o seu nome e passa a usá-la na def. SQL
    nome_tab=ObjetoBase::formatarNome(alias);

   /* Adiciona um ponto logo após o nome da tabela (ou alias), para
      a concatenação do nome da coluna */
   nome_tab+=".";

   /* Caso não haja uma coluna definida, todas as colunas da tabela
      serão considerada, desta forma um '*' será concatenado */
   if(!coluna)
    def_sql=nome_tab + "*";
   else
   {
    //Caso haja uma coluna definida, atribui o seu nome   definição SQL
    def_sql=nome_tab + coluna->obterNome(true);

    //Caso haja um alias para a coluna o mesmo será concatenad�  definição
    if(alias_coluna!="")
     def_sql+=" AS " + ObjetoBase::formatarNome(alias_coluna);
   }
  }
  //Caso seja um referênci�  uma expressão na parte SELECT-FROM
  else
  {
   /*
    Definição SQL gerada:
    {expressão} [AS ALIAS]
   */
   def_sql=expressao;
   if(alias!="")
    def_sql+=" AS " + ObjetoBase::formatarNome(alias);
  }
  def_sql+=", ";
 }
 //Caso seja um referência na parte FROM-[JOIN | WHERE]
 else if(tipo_sql==SQL_REFER_FROM)
 {
  /* Caso seja uma referência a uma objeto, apenas
     o nome e alias da tabela é usado, isso é feito
     para que a definição SQL seja na forma:
     ... FROM {NOME_TABELA} [AS ALIAS] ou
     ... FROM {EXPRESSÃO}
  */
  if(tipo_refer==REFER_COLUNA)
  {
   //Concatena o nome da tabela
   def_sql+=tabela->obterNome(true);

   //Caso um alias exista
   if(alias!="")
    def_sql+=" AS " + ObjetoBase::formatarNome(alias);
   def_sql+=", ";
  }
  else
   def_sql=expressao;
 }
 //Último tipo de referência, [JOIN | WHERE]-...
 else
 {
  /* Caso seja uma referência a um objeto e uma
     coluna exista */
  if(tipo_refer==REFER_COLUNA && coluna)
  {
   /*
    Definição SQL gerada:

    ... WHERE {NOME_TABELA | ALIAS}.{NOME_COLUNA}
   */

   /* Caso não existe um alias de tabel, o próprio nome
      da mesma é concatenad�  definição */
   if(alias=="")
    def_sql=tabela->obterNome(true);
   else
    def_sql=ObjetoBase::formatarNome(alias);

   def_sql+=".";

   //Concatena o nome da coluna
   if(coluna)
    def_sql+=coluna->obterNome(true);
  }
  else if(tipo_refer==REFER_EXPRESSAO)
   def_sql=expressao;
 }

 return(def_sql);
}

QString Referencia::obterDefinicaoXML(void)
{
 QString def_xml;
 map<QString, QString> atributos;

 atributos[AtributosParsers::TABELA]="";
 atributos[AtributosParsers::COLUNA]="";

 if(tabela)
  atributos[AtributosParsers::TABELA]=tabela->obterNome(true);

 if(coluna)
  atributos[AtributosParsers::COLUNA]=coluna->obterNome();

 atributos[AtributosParsers::EXPRESSAO]=expressao;
 atributos[AtributosParsers::ALIAS]=alias;
 atributos[AtributosParsers::ALIAS_COLUNA]=alias_coluna;

 //Retorna a definição XML da referencia
 return(ParserEsquema::obterDefinicaoObjeto(AtributosParsers::REFERENCIA,
                                            atributos, ParserEsquema::DEFINICAO_XML));
}

bool Referencia::operator == (Referencia &refer)
{
 unsigned tipo_ref;

 tipo_ref=this->obterTipoReferencia();

 //Compara o tipo de referência dos objetos
 if(tipo_ref==refer.obterTipoReferencia())
 {
  /* Caso o tipo de referencia do objeto for de objeto
     compara apenas os atributos pertinentes a este tipo */
  if(tipo_ref==REFER_COLUNA)
  {
   return(this->tabela==refer.tabela &&
          this->coluna==refer.coluna &&
          this->alias==refer.alias &&
          this->alias_coluna==refer.alias_coluna);
  }
  else
  {
   //Compara os atributos pertinentes ao tipo expressão
   return(this->expressao==refer.expressao &&
          this->alias==refer.alias);
  }
 }
 else
  return(false);
}

