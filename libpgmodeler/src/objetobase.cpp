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

QString BaseObject::getTypeName(ObjectType obj_type)
{
 if(obj_type!=BASE_OBJECT)
  /* Devido a classe ObjetoBase não ser derivada de QObject a função tr() é ineficiente para traduzir os nomes
     dos tipos de objetos sendo assim é chamado o metódo de tradução diretamente da aplicação especificando o
     contexto (ObjetoBase) no arquivo .ts e o texto a ser traduzido */
  return(QApplication::translate("BaseObject",obj_type_names[obj_type],"",QApplication::UnicodeUTF8));
 else
  return("");
}

QString BaseObject::getSchemaName(ObjectType obj_type )
{
 return(objs_schemas[obj_type ]);
}

QString BaseObject::getSQLName(ObjectType obj_type)
{
 return(objs_sql[obj_type]);
}

QString BaseObject::formatName(const QString &name, bool is_operator)
{
 int i;
 bool is_formated=false;
 QString frmt_name;
 QChar chr, chr1, chr2;
 QRegExp regexp_vect[]={
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
 for(i=0; i < 5 && !is_formated; i++)
  is_formated=regexp_vect[i].exactMatch(name);

 /* Caso o nome não esteja formatado ou
    o mesmo simboliza o nome de um operador (o qual possui caracteres
    inválidos de acordo com a regra e é a única exceção a qual seu
    nome é formatado mesmo sendo inválido) ou se nome é valido de acordo
    com as regras do SGBD para os demais tipos de objetos */
 if(!is_formated && (is_operator || isValidName(name)))
 {
  bool is_upper=false;
  unsigned i, qtd;

  /* Verifica se o nome passado possui alguma
     letra maíuscula, caso possui, o nome será
     colocado entre aspas */
  qtd=name.size();
  //for(i=0; i < qtd && !maiusc; i++)
  i=0;
  while(i < qtd && !is_upper)
  {
   chr=name[i];

   if(((i + 1) < (qtd-1)) &&
       ((chr >= 0xC2 && chr <= 0xDF) ||
        (chr >= 0xE0 && chr <= 0xEF)))
    chr1=name[i+1];
   else
    chr1=0;

   if((i + 2) < (qtd-1) &&
      chr >= 0xE0 && chr <= 0xEF)
    chr2=name[i+2];
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
    is_upper=true;
   }

  }

  //Caso o nome possua letras maiúsculas
  if(is_upper)
   frmt_name="\"" + name + "\"";
  else
   frmt_name=name;
 }
 else if(is_formated)
  frmt_name=name;

 return(frmt_name);
}

bool BaseObject::isValidName(const QString &name)
{
 int len;

 /* No cálculo do tamanho de uma QString o '\0' é considerado,
    sendo assim, para evitar comparações desnecessárias com
    o caracterece \0, o tamanho original é decrementado em 1 */
 len=name.size();

 /* Caso o nome seja maior do que o tamanho máximo
    aceito pelo SGBD (atualmente 63 bytes) */
 if(name.isEmpty() || len > OBJECT_NAME_MAX_LENGTH)
  return(false);
 else
 {
  int i=0;
  bool valid=true;
  QChar chr='\0', chr1='\0', chr2='\0';

  chr=name[0];
  if(len > 1)
   chr1=name[len-1];

  /* Verifica se o nome do objeto está entre aspas ou não.
     Situações do tipo "nome ou nome" não são aceitas */
  if(chr=='\"' && chr1=='\"')
  {
   /* Valida o nome mas a contagem de caracteres se iniciará
      após a primeira aspa e finaliza antes de última */
   valid=true; i++; len--;
  }

  while(valid && i < len)
  {
   chr=name[i];

   //Validação de caracteres ascii simples
   /* Verifica se o nome possui apenas os caracteres
     a-z A-Z 0-9 _ */
   if((chr >= 'a' && chr <='z') ||
      (chr >= 'A' && chr <='Z') ||
      (chr >= '0' && chr <='9') ||
      chr == '_')
   {
    valid=true;
    i++;
   }
   else valid=false;

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
   if(!valid && (i < len-1))
   {
    chr1=name[i+1];

    if((i + 2) <= (len-1))
     chr2=name[i+2];
    else
     chr2=0;

       //Caractere UTF-8 com 2 bytes
    if((chr  >= 0xC2 && chr <= 0xDF &&
        chr1 >= 0x80 && chr1 <= 0xBF) ||

       //Caractere UTF-8 com 3 bytes
       (chr  >= 0xE0 && chr <= 0xEF &&
        chr1 >= 0x80 && chr1 <= 0xBF &&
        chr2 >= 0x80 && chr2 <= 0xBF))
    valid=true;

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

  return(valid);
 }
}

void BaseObject::setProtected(bool value)
{
 protected_obj=value;
}

void BaseObject::setName(const QString &name)
{
 /* Caso se tente atribuir um nome vazio ao objeto
    é disparada uma exceção */
 if(name.isEmpty())
  throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int count;
  QString aux_name=name;

  count=aux_name.count(QChar('\0'));
  if(count >=1) aux_name.chop(count);

  //Caso o nome não seja válido dispara uma exceção
  if(!isValidName(aux_name))
   throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   aux_name.remove('\"');
   this->obj_name=aux_name;
  }
 }
}

void BaseObject::setComment(const QString &comment)
{
 this->comment=comment;
}

void BaseObject::setSchema(BaseObject *schema)
{
 if(!schema)
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_SCHEMA)
                .arg(QString::fromUtf8(this->obj_name)).arg(this->getTypeName()),
                ERR_ASG_NOT_ALOC_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(schema && schema->getType()!=OBJ_SCHEMA)
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
   this->schema=schema;
  }
  else
   throw Exception(ERR_ASG_INV_SCHEMA_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void BaseObject::setOwner(BaseObject *owner)
{
 if(owner && owner->getType()!=OBJ_ROLE)
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
   this->owner=owner;
  }
  else
   throw Exception(ERR_ASG_ROLE_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void BaseObject::setTablespace(BaseObject *tablespace)
{
 if(tablespace && tablespace->getType()!=OBJ_TABLESPACE)
  throw Exception(ERR_ASG_INV_TABLESPACE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(obj_type==OBJ_INDEX ||
     obj_type==OBJ_TABLE ||
     obj_type==OBJ_CONSTRAINT ||
     obj_type==OBJ_DATABASE)
  {
   this->tablespace=tablespace;
  }
  else
   throw Exception(ERR_ASG_TABSPC_INV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

QString BaseObject::getName(bool format)
{
 if(format)
 {
  QString aux_name;

  /* Formata o nome do objeto e marca o flag que indica se o objeto
     é um operador ou não */
  aux_name=formatName(this->obj_name, (obj_type==OBJ_OPERATOR));

  if(this->schema)
   aux_name=formatName(this->schema->getName()) + "." + aux_name;

  return(aux_name);
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

ObjectType BaseObject::getType(void)
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

bool BaseObject::operator == (const QString &name)
{
 return(this->obj_name==name);
}

bool BaseObject::operator != (const QString &name)
{
 return(this->obj_name!=name);
}

QString BaseObject::getCodeDefinition(unsigned def_type)
{
 return(BaseObject::getCodeDefinition(def_type, false));
}

QString BaseObject::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 QString code_def;

 if((def_type==SchemaParser::SQL_DEFINITION &&
     obj_type!=BASE_OBJECT && obj_type!=BASE_RELATIONSHIP &&
     obj_type!=BASE_TABLE && obj_type!=OBJ_TEXTBOX) ||

    (def_type==SchemaParser::XML_DEFINITION &&
     obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE))
 {
  bool format;

  //Formata o nome dos objetos caso uma definição SQL está sendo gerada
  format=(def_type==SchemaParser::SQL_DEFINITION ||
            (def_type==SchemaParser::XML_DEFINITION && reduced_form &&
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

  attributes[ParsersAttributes::NAME]=this->getName(format);
  attributes[ParsersAttributes::SQL_OBJECT]=objs_sql[this->obj_type];

  if(def_type==SchemaParser::XML_DEFINITION && schema)
  {
   attributes[ParsersAttributes::SCHEMA]=schema->getCodeDefinition(def_type, true);
  }

  if(def_type==SchemaParser::XML_DEFINITION)
    attributes[ParsersAttributes::PROTECTED]=(protected_obj ? "1" : "");

  if(comment!="")
  {
   attributes[ParsersAttributes::COMMENT]=comment;

   if((def_type==SchemaParser::SQL_DEFINITION &&
       obj_type!=OBJ_TABLESPACE &&
       obj_type!=OBJ_DATABASE) ||
      def_type==SchemaParser::XML_DEFINITION)
   {
    SchemaParser::setIgnoreUnkownAttributes(true);
    attributes[ParsersAttributes::COMMENT]=
    SchemaParser::getObjectDefinition(ParsersAttributes::COMMENT, attributes, def_type);
   }
  }

  if(tablespace)
  {
   if(def_type==SchemaParser::SQL_DEFINITION)
    attributes[ParsersAttributes::TABLESPACE]=tablespace->getName(format);
   else
    attributes[ParsersAttributes::TABLESPACE]=tablespace->getCodeDefinition(def_type, true);
  }

  if(owner)
  {
   if(def_type==SchemaParser::SQL_DEFINITION)
   {
    attributes[ParsersAttributes::OWNER]=owner->getName(format);

    /** Apenas espaços de tabelas e banco de dados não têm um comando ALTER SET OWNER
        pois por regra do PostgreSQL, espaços de tabelas e banco de dados devem ser criados
        com apenas por linha de comando isolada das demais **/
    if((def_type==SchemaParser::SQL_DEFINITION &&
        obj_type!=OBJ_TABLESPACE &&
        obj_type!=OBJ_DATABASE) ||
       def_type==SchemaParser::XML_DEFINITION)
    {
     SchemaParser::setIgnoreUnkownAttributes(true);
     attributes[ParsersAttributes::OWNER]=
     SchemaParser::getObjectDefinition(ParsersAttributes::OWNER, attributes, def_type);
    }
   }
   else
    attributes[ParsersAttributes::OWNER]=owner->getCodeDefinition(def_type, true);
  }

  if(reduced_form)
   attributes[ParsersAttributes::REDUCED_FORM]="1";
  else
   attributes[ParsersAttributes::REDUCED_FORM]="";

  try
  {
   /* Caso seja a definição XML a ser obtida, os atributos
      terão os caracteres <, >, ', " e & substituídos pelos
      equivalentes no XML, evitando assim a construção de
      código XMl com erros de sintaxe */
   code_def=SchemaParser::getObjectDefinition(objs_schemas[obj_type], attributes, def_type);
   if(def_type==SchemaParser::XML_DEFINITION)
   {
    QRegExp regexp_vect[]={
                           QRegExp("(=\")+"),
                           QRegExp("(\")(([\r\n\t])+|(\\ )+|(/>)+|(>)+)"),
                           QRegExp("(<)+([a-z])+(>)+"),
                           QRegExp("(</)+([a-z])+(>)+")
                         };

    int pos=0, pos1=0, prev_pos=0, count=0;
    QString str_aux;

    do
    {
     prev_pos=pos1;
     //Tenta extrair os valores dos atributos
     pos=regexp_vect[0].indexIn(code_def, pos);
     pos+=regexp_vect[0].matchedLength();
     pos1=regexp_vect[1].indexIn(code_def, pos);

     /* CAso não consigua extrair os valores dos atributos
        usa expressões regulares para retirada de conteúdo
        de tags vazias (sem atributos) ex.: <comment>,<condition>,<expression> */
     if(pos < 0)
     {
      pos=regexp_vect[2].indexIn(code_def, prev_pos);
      pos+=regexp_vect[2].matchedLength();
      pos1=regexp_vect[3].indexIn(code_def, pos);
     }

     //Calcula a quantidade de caracteres extraidos
     count=(pos > 0 ? (pos1-pos) : 0);
     if(pos >= 0)
     {
      //Obtém a substring extraída com as expressões regulares
      str_aux=code_def.mid(pos, count);
      //Substitui os caracteres especiais pelas entidades XML
      str_aux.replace('\"',XMLParser::CHAR_QUOT);
      str_aux.replace('<',XMLParser::CHAR_LT);
      str_aux.replace('>',XMLParser::CHAR_GT);
      //Substitui na definição XML original pela string modificada
      code_def.replace(pos,count,str_aux);
      pos+=count;
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

 return(code_def);
}

void BaseObject::setAttribute(const QString &attrib, const QString &value)
{
 attributes[attrib]=value;
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

