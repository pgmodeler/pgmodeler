#include "reference.h"

Reference::Reference(void)
{
 this->table=NULL;
 this->column=NULL;
}

Reference::Reference(Tabela *tabela, Column *coluna, const QString &alias_tab, const QString &alias_col)
{
 if(!tabela)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o alias atribuido   tabela/expressão ou coluna seja inválido
    de acordo com a regra de nomenclatura do PostgreSQL */
 else if((!alias_tab.isEmpty() && !BaseObject::isValidName(alias_tab)) ||
         (!alias_col.isEmpty() && !BaseObject::isValidName(alias_col)))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso se tente criar uma referência a uma coluna cuja tabela pai seja
    diferente da tabela informada no parâmetro */
 else if(coluna && coluna->getParentTable()!=tabela)
  throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui os parâmetros aos atributos do objeto
 this->table=tabela;
 this->column=coluna;
 this->alias=alias_tab;
 this->column_alias=alias_col;
}

Reference::Reference(const QString &expressao, const QString &alias_exp)
{
 if(expressao=="")
  throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o alias da expressão seja inválido de acordo com as regras de
    nomenclatura do PostgreSQL */
 else if(!BaseObject::isValidName(alias_exp))
  throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 table=NULL;
 column=NULL;

 //Atribui os parâmetros aos atributos do objeto
 alias=alias_exp;
 this->expression=expressao;
}

Tabela *Reference::getTable(void)
{
 return(table);
}

Column *Reference::getColumn(void)
{
 return(column);
}

QString Reference::getColumnAlias(void)
{
 return(column_alias);
}

QString Reference::getAlias(void)
{
 return(alias);
}

QString Reference::getExpression(void)
{
 return(expression);
}

unsigned Reference::getReferenceType(void)
{
 /* Caso o atributo expressão esteja preenchido o tipo do
    objeto será referência a uma expressão */
 if(expression=="")
  return(REFER_COLUMN);
 else
  return(REFER_EXPRESSION);
}

QString Reference::getSQLDefinition(unsigned tipo_sql)
{
 QString def_sql, nome_tab, nome_cols;
 unsigned tipo_refer;

 //Obtém o tipo de referência do objeto this
 tipo_refer=getReferenceType();

 /* Caso seja uma referênci  parte SELECT-FROM,
    formata a SQL do objeto de acordo com este tipo */
 if(tipo_sql==SQL_REFER_SELECT)
 {
  //Caso seja um referênci  objetos (colunas, esquemas, tabelas, alias)
  if(tipo_refer==REFER_COLUMN)
  {
   /*
    Definição SQL gerada:
    {NOME_TABELA|ALIAS_TABELA}.{NOME_COLUNA | *} [AS ALIAS_COLUNA]
   */

   /* Caso um alias de tabela não esteja definido,
      o nome da própria tabela será usada */
   if(alias=="")
    nome_tab=table->getName(true);
   else
    //Caso haja um alias de tabela, formata o seu nome e passa a usá-la na def. SQL
    nome_tab=BaseObject::formatName(alias);

   /* Adiciona um ponto logo após o nome da tabela (ou alias), para
      a concatenação do nome da coluna */
   nome_tab+=".";

   /* Caso não haja uma coluna definida, todas as colunas da tabela
      serão considerada, desta forma um '*' será concatenado */
   if(!column)
    def_sql=nome_tab + "*";
   else
   {
    //Caso haja uma coluna definida, atribui o seu nome   definição SQL
    def_sql=nome_tab + column->getName(true);

    //Caso haja um alias para a coluna o mesmo será concatenad  definição
    if(column_alias!="")
     def_sql+=" AS " + BaseObject::formatName(column_alias);
   }
  }
  //Caso seja um referênci  uma expressão na parte SELECT-FROM
  else
  {
   /*
    Definição SQL gerada:
    {expressão} [AS ALIAS]
   */
   def_sql=expression;
   if(alias!="")
    def_sql+=" AS " + BaseObject::formatName(alias);
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
  if(tipo_refer==REFER_COLUMN)
  {
   //Concatena o nome da tabela
   def_sql+=table->getName(true);

   //Caso um alias exista
   if(alias!="")
    def_sql+=" AS " + BaseObject::formatName(alias);
   def_sql+=", ";
  }
  else
   def_sql=expression;
 }
 //Último tipo de referência, [JOIN | WHERE]-...
 else
 {
  /* Caso seja uma referência a um objeto e uma
     coluna exista */
  if(tipo_refer==REFER_COLUMN && column)
  {
   /*
    Definição SQL gerada:

    ... WHERE {NOME_TABELA | ALIAS}.{NOME_COLUNA}
   */

   /* Caso não existe um alias de tabel, o próprio nome
      da mesma é concatenad  definição */
   if(alias=="")
    def_sql=table->getName(true);
   else
    def_sql=BaseObject::formatName(alias);

   def_sql+=".";

   //Concatena o nome da coluna
   if(column)
    def_sql+=column->getName(true);
  }
  else if(tipo_refer==REFER_EXPRESSION)
   def_sql=expression;
 }

 return(def_sql);
}

QString Reference::getXMLDefinition(void)
{
 QString def_xml;
 map<QString, QString> atributos;

 atributos[ParsersAttributes::TABLE]="";
 atributos[ParsersAttributes::COLUMN]="";

 if(table)
  atributos[ParsersAttributes::TABLE]=table->getName(true);

 if(column)
  atributos[ParsersAttributes::COLUMN]=column->getName();

 atributos[ParsersAttributes::EXPRESSION]=expression;
 atributos[ParsersAttributes::ALIAS]=alias;
 atributos[ParsersAttributes::COLUMN_ALIAS]=column_alias;

 //Retorna a definição XML da referencia
 return(SchemaParser::getObjectDefinition(ParsersAttributes::REFERENCE,
                                            atributos, SchemaParser::XML_DEFINITION));
}

bool Reference::operator == (Reference &refer)
{
 unsigned tipo_ref;

 tipo_ref=this->getReferenceType();

 //Compara o tipo de referência dos objetos
 if(tipo_ref==refer.getReferenceType())
 {
  /* Caso o tipo de referencia do objeto for de objeto
     compara apenas os atributos pertinentes a este tipo */
  if(tipo_ref==REFER_COLUMN)
  {
   return(this->table==refer.table &&
          this->column==refer.column &&
          this->alias==refer.alias &&
          this->column_alias==refer.column_alias);
  }
  else
  {
   //Compara os atributos pertinentes ao tipo expressão
   return(this->expression==refer.expression &&
          this->alias==refer.alias);
  }
 }
 else
  return(false);
}

