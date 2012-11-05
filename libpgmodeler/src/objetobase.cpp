#include "objetobase.h"
#include <QApplication>

/* Atenção: Se a ordem e quantidade das enumerações forem modificados
   então a ordem e quantidade dos elementos deste vetor
   também devem ser modificados */
QString BaseObject::objs_schemas[OBJECT_TYPE_COUNT]={
  "column",  "constraint", "function", "trigger",
  "index", "rule", "table", "view",
  "domain", "schema", "aggregate", "operator",
  "sequence", "role", "conversion", "cast",
  "language", "usertype", "tablespace",
  "opfamily", "opclass", "database","relationship","textbox",
   "grant","parameter","relationship"
};


QString BaseObject::obj_type_names[OBJECT_TYPE_COUNT]={
  QT_TR_NOOP("Column"), QT_TR_NOOP("Constraint"), QT_TR_NOOP("Function"),
  QT_TR_NOOP("Trigger"), QT_TR_NOOP("Index"), QT_TR_NOOP("Rule"),
  QT_TR_NOOP("Table"), QT_TR_NOOP("View"),  QT_TR_NOOP("Domain"),
  QT_TR_NOOP("Schema"), QT_TR_NOOP("Aggregate"), QT_TR_NOOP("Operator"),
  QT_TR_NOOP("Sequence"), QT_TR_NOOP("Role"), QT_TR_NOOP("Conversion"),
  QT_TR_NOOP("Cast"), QT_TR_NOOP("Language"), QT_TR_NOOP("Type"), QT_TR_NOOP("Tablespace"),
  QT_TR_NOOP("Operator Family"), QT_TR_NOOP("Operator Class"),
  QT_TR_NOOP("Database"), QT_TR_NOOP("Table-Table Relationship"),
  QT_TR_NOOP("Textbox"), QT_TR_NOOP("Permission"), QT_TR_NOOP("Parameter"),
  QT_TR_NOOP("Table-View Relationship")
};

QString BaseObject::objs_sql[OBJECT_TYPE_COUNT]={
  "COLUMN",  "CONSTRAINT", "FUNCTION", "TRIGGER",
  "INDEX", "RULE", "TABLE", "VIEW",
  "DOMAIN", "SCHEMA", "AGGREGATE", "OPERATOR",
  "SEQUENCE", "ROLE", "CONVERSION", "CAST",
  "LANGUAGE", "TYPE", "TABLESPACE",
  "OPERATOR FAMILY", "OPERATOR CLASS", "DATABASE"
};

/* Inicializa o id global o qual é compartilhado entre as instâncias
   de classes derivadas da classe ObjetoBase. O valor do id_global
   inicia-se em 60k pois as faixas de ids 0,10k,20k,30k,40k e 50k
   estão atribuídos respectivamente aos objetos das classes Papel, EspacoTabela,
   ModeloBD, Esquema, Função e Tipo */
unsigned BaseObject::global_id=60000;
unsigned BaseObject::type_id=50000;
unsigned BaseObject::function_id=40000;
unsigned BaseObject::schema_id=30000;
unsigned BaseObject::dbmodel_id=20000;
unsigned BaseObject::tabspace_id=10000;
unsigned BaseObject::role_id=0;

BaseObject::BaseObject(void)
{
 object_id=BaseObject::global_id++;
 protected_obj=false;
 //nome="";
 obj_type=BASE_OBJECT;
 schema=NULL;
 owner=NULL;
 tablespace=NULL;
 attributes[ParsersAttributes::NAME]="";
 attributes[ParsersAttributes::COMMENT]="";
 attributes[ParsersAttributes::OWNER]="";
 attributes[ParsersAttributes::TABLESPACE]="";
 attributes[ParsersAttributes::SCHEMA]="";
 attributes[ParsersAttributes::PROTECTED]="";
}

unsigned BaseObject::getGlobalId(void)
{
 return(global_id);
}

QString BaseObject::getTypeName(ObjectType tipo_objeto)
{
 if(tipo_objeto!=BASE_OBJECT)
  /* Devido a classe ObjetoBase não ser derivada de QObject a função tr() é ineficiente para traduzir os nomes
     dos tipos de objetos sendo assim é chamado o metódo de tradução diretamente da aplicação especificando o
     contexto (ObjetoBase) no arquivo .ts e o texto a ser traduzido */
  return(QApplication::translate("ObjetoBase",obj_type_names[tipo_objeto],"",QApplication::UnicodeUTF8));
  //return(QObject::trUtf8());
 else
  return("");
}

QString BaseObject::getSchemaName(ObjectType tipo_objeto)
{
 return(objs_schemas[tipo_objeto]);
}

QString BaseObject::getSQLName(ObjectType tipo_objeto)
{
 return(objs_sql[tipo_objeto]);
}

QString BaseObject::formatName(const QString &nome_obj, bool obj_operador)
{
 int i;
 bool formatado=false;
 QString nome_form;
 QChar chr, chr1, chr2;
 QRegExp vet_regexp[]={
                        QRegExp("(\")(.)+(\")"),
                        QRegExp("(\")(.)+(\")(\\.)(\")(.)+(\")"),
                        QRegExp("(\")(.)+(\")(\\.)(.)+"),
                        QRegExp("(.)+(\\.)(\")(.)+(\")"),
                        QRegExp("(.)+(\\.)(.)+")
                      };

 /* Verifica através de expressões regulares
    se o nome passado para formatação já não se encontra
    formatado. As formas prováveis de formatação são:
    1) "NOME_OBJETO"
    2) "NOME_ESQUEMA"."NOME_OBJETO"
    3) "NOME_ESQUEMA".NOME_OBJETO
    4) NOME_ESQUEMA."NOME_OBJETO"
    5) NOME_ESQUEMA.NOME_OBJETO */
 for(i=0; i < 5 && !formatado; i++)
  formatado=vet_regexp[i].exactMatch(nome_obj);

 /* Caso o nome não esteja formatado ou
    o mesmo simboliza o nome de um operador (o qual possui caracteres
    inválidos de acordo com a regra e é a única exceção a qual seu
    nome é formatado mesmo sendo inválido) ou se nome é valido de acordo
    com as regras do SGBD para os demais tipos de objetos */
 if(!formatado && (obj_operador || isValidName(nome_obj)))
 {
  bool maiusc=false;
  unsigned i, qtd;

  /* Verifica se o nome passado possui alguma
     letra maíuscula, caso possui, o nome será
     colocado entre aspas */
  qtd=nome_obj.size();
  //for(i=0; i < qtd && !maiusc; i++)
  i=0;
  while(i < qtd && !maiusc)
  {
   chr=nome_obj[i];

   if(((i + 1) < (qtd-1)) &&
       ((chr >= 0xC2 && chr <= 0xDF) ||
        (chr >= 0xE0 && chr <= 0xEF)))
    chr1=nome_obj[i+1];
   else
    chr1=0;

   if((i + 2) < (qtd-1) &&
      chr >= 0xE0 && chr <= 0xEF)
    chr2=nome_obj[i+2];
   else
    chr2=0;

   if(chr1!=0 && chr2!=0)
    i+=3;
   else if(chr1!=0 && chr2==0)
    i+=2;
   else
    i++;

   //Caractere UTF-8 com 2 bytes
   if((chr  >= 0xC2 && chr <= 0xDF &&
       chr1 >= 0x80 && chr1 <= 0xBF) ||

       //Caractere UTF-8 com 3 bytes
       (chr  >= 0xE0 && chr <= 0xEF &&
        chr1 >= 0x80 && chr1 <= 0xBF &&
        chr2 >= 0x80 && chr2 <= 0xBF) ||

      chr.isUpper())
   {
    maiusc=true;
   }

  }

  //Caso o nome possua letras maiúsculas
  if(maiusc)
   nome_form="\"" + nome_obj + "\"";
  else
   nome_form=nome_obj;
 }
 else if(formatado)
  nome_form=nome_obj;

 return(nome_form);
}

bool BaseObject::isValidName(const QString &nome_obj)
{
 int tam;

 /* No cálculo do tamanho de uma QString o '\0' é considerado,
    sendo assim, para evitar comparações desnecessárias com
    o caracterece \0, o tamanho original é decrementado em 1 */
 tam=nome_obj.size();

 /* Caso o nome seja maior do que o tamanho máximo
    aceito pelo SGBD (atualmente 63 bytes) */
 if(nome_obj.isEmpty() || tam > OBJECT_NAME_MAX_LENGTH)
  return(false);
 else
 {
  int i=0;
  bool valido=true;
  QChar chr='\0', chr1='\0', chr2='\0';

  chr=nome_obj[0];
  if(tam > 1)
   chr1=nome_obj[tam-1];

  /* Verifica se o nome do objeto está entre aspas ou não.
     Situações do tipo "nome ou nome" não são aceitas */
  if(chr=='\"' && chr1=='\"')
  {
   /* Valida o nome mas a contagem de caracteres se iniciará
      após a primeira aspa e finaliza antes de última */
   valido=true; i++; tam--;
  }

  while(valido && i < tam)
  {
   chr=nome_obj[i];

   //Validação de caracteres ascii simples
   /* Verifica se o nome possui apenas os caracteres
     a-z A-Z 0-9 _ */
   if((chr >= 'a' && chr <='z') ||
      (chr >= 'A' && chr <='Z') ||
      (chr >= '0' && chr <='9') ||
      chr == '_')
   {
    valido=true;
    i++;
   }
   else valido=false;

   /* Validação de caracteres utf8. Caracteres UTF8 possuem 2 bytes.
      Sendo que o primeiro byte deve estar entre 0xC2 e 0xDF e o segundo/terceiro
      deve estar entre 0x80 e 0xBF.
      Ref.: http://www.fileformat.info/info/unicode/utf8.htm
            http://en.wikipedia.org/wiki/UTF-8

      Trecho extraído da url:

      The value of each individual byte indicates its UTF-8 function, as follows:
      00 to 7F hex (0 to 127): first and only byte of a sequence.
      80 to BF hex (128 to 191): continuing byte in a multi-byte sequence.
      C2 to DF hex (194 to 223): first byte of a two-byte sequence.
      E0 to EF hex (224 to 239): first byte of a three-byte sequence.  */
   if(!valido && (i < tam-1))
   {
    chr1=nome_obj[i+1];

    if((i + 2) <= (tam-1))
     chr2=nome_obj[i+2];
    else
     chr2=0;

       //Caractere UTF-8 com 2 bytes
    if((chr  >= 0xC2 && chr <= 0xDF &&
        chr1 >= 0x80 && chr1 <= 0xBF) ||

       //Caractere UTF-8 com 3 bytes
       (chr  >= 0xE0 && chr <= 0xEF &&
        chr1 >= 0x80 && chr1 <= 0xBF &&
        chr2 >= 0x80 && chr2 <= 0xBF))
    valido=true;

    //Caso o caractere validado seja de 2 bytes
    if(chr >= 0xC2 && chr <= 0xDF)
     //Incrementa em 2 a posição na string
     i+=2;
    else
     /* Incrementa em 3 a posição na string pois o
        caractere validado é de 3 bytes */
     i+=3;
   }
  }

  return(valido);
 }
}

void BaseObject::setProtected(bool valor)
{
 protected_obj=valor;
}

void BaseObject::definirNome(const QString &nome)
{
 /* Caso se tente atribuir um nome vazio ao objeto
    é disparada uma exceção */
 if(nome.isEmpty())
  throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int qtd;
  QString nome_aux=nome;

  qtd=nome_aux.count(QChar('\0'));
  if(qtd >=1) nome_aux.chop(qtd);

  //Caso o nome não seja válido dispara uma exceção
  if(!isValidName(nome_aux))
   throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   nome_aux.remove('\"');
   this->obj_name=nome_aux;
  }
 }
}

void BaseObject::setComment(const QString &comentario)
{
 this->comment=comentario;
}

void BaseObject::setSchema(BaseObject *esquema)
{
 if(!esquema)
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_SCHEMA)
                .arg(QString::fromUtf8(this->obj_name)).arg(this->getTypeName()),
                ERR_ASG_NOT_ALOC_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(esquema && esquema->obterTipoObjeto()!=OBJ_SCHEMA)
  throw Exception(ERR_ASG_INV_SCHEMA_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE ||
     obj_type==OBJ_VIEW  || obj_type==OBJ_DOMAIN ||
     obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
     obj_type==OBJ_SEQUENCE || obj_type==OBJ_CONVERSION ||
     obj_type==OBJ_TYPE || obj_type==OBJ_OPCLASS ||
     obj_type==OBJ_OPFAMILY)
  {
   this->schema=esquema;
  }
  else
   throw Exception(ERR_ASG_INV_SCHEMA_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void BaseObject::setOwner(BaseObject *dono)
{
 if(dono && dono->obterTipoObjeto()!=OBJ_ROLE)
  throw Exception(ERR_ASG_INV_ROLE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE ||
     obj_type==OBJ_DOMAIN || obj_type==OBJ_SCHEMA ||
     obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
     obj_type==OBJ_CONVERSION ||
     obj_type==OBJ_LANGUAGE || obj_type==OBJ_TYPE ||
     obj_type==OBJ_TABLESPACE || obj_type==OBJ_DATABASE ||
     obj_type==OBJ_OPCLASS || obj_type==OBJ_OPFAMILY)
  {
   this->owner=dono;
  }
  else
   throw Exception(ERR_ASG_ROLE_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void BaseObject::setTablespace(BaseObject *espacotabela)
{
 if(espacotabela && espacotabela->obterTipoObjeto()!=OBJ_TABLESPACE)
  throw Exception(ERR_ASG_INV_TABLESPACE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(obj_type==OBJ_INDEX ||
     obj_type==OBJ_TABLE ||
     obj_type==OBJ_CONSTRAINT ||
     obj_type==OBJ_DATABASE)
  {
   this->tablespace=espacotabela;
  }
  else
   throw Exception(ERR_ASG_TABSPC_INV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

QString BaseObject::getName(bool formatar)
{
 if(formatar)
 {
  QString nome_aux;

  /* Formata o nome do objeto e marca o flag que indica se o objeto
     é um operador ou não */
  nome_aux=formatName(this->obj_name, (obj_type==OBJ_OPERATOR));

  if(this->schema)
   nome_aux=formatName(this->schema->getName()) + "." + nome_aux;

  return(nome_aux);
 }
 else return(this->obj_name);
}

QString BaseObject::getComment(void)
{
 return(comment);
}

BaseObject *BaseObject::getSchema(void)
{
 return(schema);
}

BaseObject *BaseObject::getOwner(void)
{
 return(owner);
}

BaseObject *BaseObject::getTablespace(void)
{
 return(tablespace);
}

ObjectType BaseObject::obterTipoObjeto(void)
{
 return(obj_type);
}

QString BaseObject::getTypeName(void)
{
 return(BaseObject::getTypeName(this->obj_type));
}

QString BaseObject::getSchemaName(void)
{
 return(BaseObject::getSchemaName(this->obj_type));
}

QString BaseObject::getSQLName(void)
{
 return(BaseObject::getSQLName(this->obj_type));
}

bool BaseObject::isProtected(void)
{
 return(protected_obj);
}

unsigned BaseObject::getObjectId(void)
{
 return(object_id);
}

bool BaseObject::operator == (const QString &nome)
{
 return(this->obj_name==nome);
}

bool BaseObject::operator != (const QString &nome)
{
 return(this->obj_name!=nome);
}

QString BaseObject::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(BaseObject::obterDefinicaoObjeto(tipo_def, false));
}

QString BaseObject::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 QString def;

 if((tipo_def==SchemaParser::SQL_DEFINITION &&
     obj_type!=BASE_OBJECT && obj_type!=BASE_RELATIONSHIP &&
     obj_type!=BASE_TABLE && obj_type!=OBJ_TEXTBOX) ||

    (tipo_def==SchemaParser::XML_DEFINITION &&
     obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE))
 {
  bool formatar;
  QString str_aux;

  //Formata o nome dos objetos caso uma definição SQL está sendo gerada
  formatar=(tipo_def==SchemaParser::SQL_DEFINITION ||
            (tipo_def==SchemaParser::XML_DEFINITION && forma_reduzida &&
             obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_RELATIONSHIP &&
             obj_type!=BASE_RELATIONSHIP));
  attributes[objs_schemas[obj_type]]="1";

  /* Marcando a flag que indica que o a forma de comentário a ser gerada
     para o objeto é específica para o mesmo, fugindo da regra padrão.
     (Ver arquivo de esquema SQL para comentários) */
  switch(obj_type)
  {
   case OBJ_COLUMN:
   case OBJ_AGGREGATE:
   case OBJ_FUNCTION:
   case OBJ_CAST:
   case OBJ_CONSTRAINT:
   case OBJ_RULE:
   case OBJ_TRIGGER:
   case OBJ_OPERATOR:
   case OBJ_OPCLASS:
   case OBJ_OPFAMILY:
    attributes[ParsersAttributes::DIF_SQL]="1";
   break;
   default:
    attributes[ParsersAttributes::DIF_SQL]="";
   break;
  }

  attributes[ParsersAttributes::NAME]=this->getName(formatar);
  attributes[ParsersAttributes::SQL_OBJECT]=objs_sql[this->obj_type];

  if(tipo_def==SchemaParser::XML_DEFINITION && schema)
  {
   attributes[ParsersAttributes::SCHEMA]=schema->obterDefinicaoObjeto(tipo_def, true);
  }

  if(tipo_def==SchemaParser::XML_DEFINITION)
    attributes[ParsersAttributes::PROTECTED]=(protected_obj ? "1" : "");

  if(comment!="")
  {
   attributes[ParsersAttributes::COMMENT]=comment;

   if((tipo_def==SchemaParser::SQL_DEFINITION &&
       obj_type!=OBJ_TABLESPACE &&
       obj_type!=OBJ_DATABASE) ||
      tipo_def==SchemaParser::XML_DEFINITION)
   {
    SchemaParser::setIgnoreUnkownAttributes(true);
    attributes[ParsersAttributes::COMMENT]=
    SchemaParser::getObjectDefinition(ParsersAttributes::COMMENT, attributes, tipo_def);
   }
  }

  if(tablespace)
  {
   if(tipo_def==SchemaParser::SQL_DEFINITION)
    attributes[ParsersAttributes::TABLESPACE]=tablespace->getName(formatar);
   else
    attributes[ParsersAttributes::TABLESPACE]=tablespace->obterDefinicaoObjeto(tipo_def, true);
  }

  if(owner)
  {
   if(tipo_def==SchemaParser::SQL_DEFINITION)
   {
    attributes[ParsersAttributes::OWNER]=owner->getName(formatar);

    /** Apenas espaços de tabelas e banco de dados não têm um comando ALTER SET OWNER
        pois por regra do PostgreSQL, espaços de tabelas e banco de dados devem ser criados
        com apenas por linha de comando isolada das demais **/
    if((tipo_def==SchemaParser::SQL_DEFINITION &&
        obj_type!=OBJ_TABLESPACE &&
        obj_type!=OBJ_DATABASE) ||
       tipo_def==SchemaParser::XML_DEFINITION)
    {
     SchemaParser::setIgnoreUnkownAttributes(true);
     attributes[ParsersAttributes::OWNER]=
     SchemaParser::getObjectDefinition(ParsersAttributes::OWNER, attributes, tipo_def);
    }
   }
   else
    attributes[ParsersAttributes::OWNER]=owner->obterDefinicaoObjeto(tipo_def, true);
  }

  if(forma_reduzida)
   attributes[ParsersAttributes::REDUCED_FORM]="1";
  else
   attributes[ParsersAttributes::REDUCED_FORM]="";

  try
  {
   /* Caso seja a definição XML a ser obtida, os atributos
      terão os caracteres <, >, ', " e & substituídos pelos
      equivalentes no XML, evitando assim a construção de
      código XMl com erros de sintaxe */
   def=SchemaParser::getObjectDefinition(objs_schemas[obj_type], attributes, tipo_def);
   if(tipo_def==SchemaParser::XML_DEFINITION)
   {
    QRegExp vet_regexp[]={
                           QRegExp("(=\")+"),
                           QRegExp("(\")(([\r\n\t])+|(\\ )+|(/>)+|(>)+)"),
                           QRegExp("(<)+([a-z])+(>)+"),
                           QRegExp("(</)+([a-z])+(>)+")
                         };

    int pos=0, pos1=0, pos_ant=0, qtd=0;
    QString str_aux;

    do
    {
     pos_ant=pos1;
     //Tenta extrair os valores dos atributos
     pos=vet_regexp[0].indexIn(def, pos);
     pos+=vet_regexp[0].matchedLength();
     pos1=vet_regexp[1].indexIn(def, pos);

     /* CAso não consigua extrair os valores dos atributos
        usa expressões regulares para retirada de conteúdo
        de tags vazias (sem atributos) ex.: <comment>,<condition>,<expression> */
     if(pos < 0)
     {
      pos=vet_regexp[2].indexIn(def, pos_ant);
      pos+=vet_regexp[2].matchedLength();
      pos1=vet_regexp[3].indexIn(def, pos);
     }

     //Calcula a quantidade de caracteres extraidos
     qtd=(pos > 0 ? (pos1-pos) : 0);
     if(pos >= 0)
     {
      //Obtém a substring extraída com as expressões regulares
      str_aux=def.mid(pos, qtd);
      //Substitui os caracteres especiais pelas entidades XML
      str_aux.replace('\"',XMLParser::CHAR_QUOT);
      str_aux.replace('<',XMLParser::CHAR_LT);
      str_aux.replace('>',XMLParser::CHAR_GT);
      //Substitui na definição XML original pela string modificada
      def.replace(pos,qtd,str_aux);
      pos+=qtd;
     }
    }
    /* Enquanto as posições das expressões encontradas sejam válidas
       Posições menores que 0 indicam que nenhuma das expressões regulares
       conseguiram encontrar valores */
    while(pos >=0 && pos1 >=0);
   }

   clearAttributes();
  }
  catch(Exception &e)
  {
   SchemaParser::restartParser();
   clearAttributes();
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }

 return(def);
}

void BaseObject::setAttribute(const QString &atrib, const QString &valor)
{
 attributes[atrib]=valor;
}

void BaseObject::clearAttributes(void)
{
 map<QString, QString>::iterator itr, itr_end;

 itr=attributes.begin();
 itr_end=attributes.end();

 while(itr!=itr_end)
 {
  itr->second="";
  itr++;
 }
}

void BaseObject::operator = (BaseObject &obj)
{
 this->owner=obj.owner;
 this->schema=obj.schema;
 this->tablespace=obj.tablespace;
 this->comment=obj.comment;
 this->obj_name=obj.obj_name;
 this->obj_type=obj.obj_type;
 this->protected_obj=obj.protected_obj;
}

