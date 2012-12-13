#include "sequence.h"

const QString Sequence::MAX_POSITIVE_VALUE="+9223372036854775807";
const QString Sequence::MAX_NEGATIVE_VALUE="-9223372036854775808";

Sequence::Sequence(void)
{
 obj_type=OBJ_SEQUENCE;
 cycle=false;
 increment=start=cache="1";
 min_value="0";
 max_value=MAX_POSITIVE_VALUE;
 owner_col=NULL;

 attributes[ParsersAttributes::INCREMENT]="";
 attributes[ParsersAttributes::MIN_VALUE]="";
 attributes[ParsersAttributes::MAX_VALUE]="";
 attributes[ParsersAttributes::START]="";
 attributes[ParsersAttributes::CACHE]="";
 attributes[ParsersAttributes::CYCLE]="";
 attributes[ParsersAttributes::OWNER_COLUMN]="";
}

bool Sequence::isNullValue(const QString &value)
{
 unsigned i, count;
 bool is_null;

 i=0;
 is_null=true;
 count=value.size();
 while(i < count && is_null)
 {
  is_null=(value[i]=='0' || value[i]=='+' || value[i]=='-');
  i++;
 }
 return(is_null);
}

bool Sequence::isValidValue(const QString &value)
{
 /*
  Para que um valor seja válido o mesmo deve ou não iniciar com
  operador + ou - ser constituído apenas de números. E o seu
  tamanho não deve ultrapassar o tamanho da constante VALOR_MAX_POSITIVO
 */
 if(value.size() > MAX_POSITIVE_VALUE.size())
  return(false);
 else
 {
  unsigned i, count;
  bool is_oper=false, is_num=false, is_valid=true;

  count=value.size();
  for(i=0; i < count && is_valid; i++)
  {
   if((value[i]=='-' || value[i]=='+') && !is_num)
   {
    if(!is_oper) is_oper=true;
   }
   else if((value[i]>='0' && value[i]<='9'))
   {
    if(!is_num) is_num=true;
   }
   else is_valid=false;
  }

  if(!is_num) is_valid=false;
  return(is_valid);
 }
}

QString Sequence::formatValue(const QString &value)
{
 QString fmt_value;

 //Verifica se o valor é válido
 if(isValidValue(value))
 {
  unsigned i, count, neg_cnt;

  i=neg_cnt=0;
  count=value.size();
  /* Conta a quantidade de operadores negativo, pois
     dependendo da quantidade o mesmo pode interferir
     no sinal do número */
  while((value[i]=='+' || value[i]=='-') && i < count)
  {
   if(value[i]=='-') neg_cnt++;
   i++;
  }

  //Caso a quantidade de negativos seja ímpar o número será negativo
  if(neg_cnt % 2 != 0) fmt_value+="-";
  //valor_fmt+=valor.substr(i, qtd);
  fmt_value+=value.mid(i, count);
 }

 return(fmt_value);
}

int Sequence::compareValues(QString value1, QString value2)
{
 if(value1==value2)
  return(0);
 else
 {
  char ops[2];
  unsigned i, idx, count;
  QString *vet_values[2]={&value1, &value2}, aux_value;

  for(i=0; i < 2; i++)
  {
   //Obtém o sinal do número
   ops[i]=vet_values[i]->at(0).toAscii();
   //Caso não possua sinal, um + será adicionado
   if(ops[i]!='-' && ops[i]!='+') ops[i]='+';

   //Obtém o restante do número sem o sinal
   idx=1;
   count=vet_values[i]->size();
   while(idx < count)
   {
    if(vet_values[i]->at(idx)!='0')
    aux_value+=vet_values[i]->at(idx);
    idx++;
   }
   (*vet_values[i])=aux_value;
   aux_value="";
  }

  //Compara os sinais e os valores, caso sejam iguais retorna 0
  if(ops[0]==ops[1] && value1==value2)
   return(0);
  /* Caso os operadores sejam iguais e o valor1 for menor que o valor2 ou
     se os sinais sejam diferentes */
  else if((ops[0]=='-' && ops[1]=='-' && value1 > value2) ||
          (ops[0]=='+' && ops[1]=='+' && value1 < value2) ||
          (ops[0]=='-' && ops[1]=='+'))
   //Retorna -1 indicando que o valor 1 é menor que o valor 2
   return(-1);
  else
   //Retorna 1 indicando que o valor2 é maior que valor1
   return(1);
 }
}

void Sequence::setName(const QString &name)
{
 QString prev_name=this->getName(true);

 BaseObject::setName(name);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

void Sequence::setSchema(BaseObject *schema)
{
 Tabela *table=NULL;
 QString prev_name=this->getName(true);

 //Caso a coluna possuidora da sequencia exista
 if(owner_col)
 {
  //Obtém a tabela pai da coluna
  table=dynamic_cast<Tabela *>(owner_col->getParentTable());

  //Verifica se o esquema sendo atribuíd  seqüência é o mesmo da tabela possuidora
  if(table && table->getSchema()!=schema)
    throw Exception(ERR_ASG_SEQ_DIF_TABLE_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 //Atribui o esquema   sequencia
 BaseObject::setSchema(schema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

void Sequence::setCycle(bool value)
{
 cycle=value;
}

void Sequence::setValues(QString minv, QString maxv, QString inc, QString start, QString cache)
{
 minv=formatValue(minv);
 maxv=formatValue(maxv);
 inc=formatValue(inc);
 start=formatValue(start);
 cache=formatValue(cache);

 /* Caso algum atributo após a formatação esteja vazio quer dizer
    que seu valor é invalido, sendo assim uma exceção é disparada*/
 if(minv==""   || maxv=="" || inc=="" ||
    start=="" || cache=="")
  throw Exception(ERR_ASG_INV_VALUE_SEQ_ATTRIBS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(compareValues(minv,maxv) > 0)
  throw Exception(ERR_ASG_INV_SEQ_MIN_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(compareValues(start, minv) < 0 ||
         compareValues(start, maxv) > 0)
  throw Exception(ERR_ASG_INV_SEQ_START_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(isNullValue(inc))
  throw Exception(ERR_ASG_INV_SEQ_INCR_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(isNullValue(cache))
  throw Exception(ERR_ASG_INV_SEQ_CACHE_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->min_value=minv;
 this->max_value=maxv;
 this->increment=inc;
 this->cache=cache;
 this->start=start;
}

void Sequence::setOwnerColumn(Tabela *table, const QString &col_name)
{
 if(!table || col_name=="")
  this->owner_col=NULL;
 else if(table)
 {
  // Verifica se a tabela não pertence ao mesmo esquema da sequencia.
  //   Caso não pertença, dispara uma exceção.
  if(table->getSchema()!=this->schema)
   throw Exception(Exception::getErrorMessage(ERR_ASG_TAB_DIF_SEQ_SCHEMA)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_TAB_DIF_SEQ_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    /* Verifica se a tabela não pertence ao mesmo dono da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(table->getOwner()!=this->owner)
   throw Exception(Exception::getErrorMessage(ERR_ASG_SEQ_OWNER_DIF_TABLE)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_SEQ_OWNER_DIF_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Obtém a coluna da tabela com base no nome passado
  this->owner_col=table->obterColuna(col_name);

  if(this->owner_col && this->owner_col->isAddedByRelationship() &&
     this->owner_col->getObjectId() > this->object_id)
   this->object_id=BaseObject::getGlobalId();


  //Caso a coluna não exista
  if(!this->owner_col)
   throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}

void Sequence::setOwnerColumn(Column *column)
{
 Tabela *tabela=NULL;

 if(!column)
  this->owner_col=NULL;
 else
 {
  tabela=dynamic_cast<Tabela *>(column->getParentTable());

  //CAso a coluna possuidor não seja de uma tabela
  if(!tabela)
   throw Exception(Exception::getErrorMessage(ERR_ASG_INV_OWNER_COL_SEQ)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_INV_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verifica se a tabela não pertence ao mesmo esquema da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(tabela->getSchema()!=this->schema)
   throw Exception(Exception::getErrorMessage(ERR_ASG_TAB_DIF_SEQ_SCHEMA)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_TAB_DIF_SEQ_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verifica se a tabela não pertence ao mesmo dono da sequencia.
     Caso não pertença, dispara uma exceção. */
  if(tabela->getOwner()!=this->owner)
   throw Exception(Exception::getErrorMessage(ERR_ASG_SEQ_OWNER_DIF_TABLE)
                 .arg(QString::fromUtf8(this->getName(true))),
                 ERR_ASG_SEQ_OWNER_DIF_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  this->owner_col=column;

  if(column && column->isAddedByRelationship() &&
     column->getObjectId() > this->object_id)
   this->object_id=BaseObject::getGlobalId();
 }
}

bool Sequence::isReferRelationshipColumn(void)
{
 return(owner_col && owner_col->isAddedByRelationship());
}

bool Sequence::isCycle(void)
{
 return(cycle);
}

QString Sequence::getMaxValue(void)
{
 return(max_value);
}

QString Sequence::getMinValue(void)
{
 return(min_value);
}

QString Sequence::getCache(void)
{
 return(cache);
}

QString Sequence::getIncrement(void)
{
 return(increment);
}

QString Sequence::getStart(void)
{
 return(start);
}

Column *Sequence::getOwnerColumn(void)
{
 return(owner_col);
}

QString Sequence::getCodeDefinition(unsigned def_type)
{
 QString str_aux;
 Tabela *table=NULL;

 //Caso haja uma coluna possuidora
 if(owner_col)
 {
  table=dynamic_cast<Tabela *>(owner_col->getParentTable());
  /* Formata o atributo possuidora como sendo o nome da tabela
     e a coluna possuidora */
  str_aux=table->getName(true) + "." + owner_col->getName(true);
 }
 attributes[ParsersAttributes::OWNER_COLUMN]=str_aux;

 attributes[ParsersAttributes::INCREMENT]=increment;
 attributes[ParsersAttributes::MIN_VALUE]=min_value;
 attributes[ParsersAttributes::MAX_VALUE]=max_value;
 attributes[ParsersAttributes::START]=start;
 attributes[ParsersAttributes::CACHE]=cache;
 attributes[ParsersAttributes::CYCLE]=(cycle ? "1" : "");

 return(BaseObject::__getCodeDefinition(def_type));
}

void Sequence::operator = (Sequence &seq)
{
 QString prev_name=this->getName(true);

 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(seq);

 this->cycle=seq.cycle;
 this->max_value=seq.max_value;
 this->min_value=seq.min_value;
 this->start=seq.start;
 this->increment=seq.increment;
 this->cache=seq.cache;
 this->owner_col=seq.owner_col;

 TipoPgSQL::renomearTipoUsuario(prev_name, this, this->getName(true));
}

