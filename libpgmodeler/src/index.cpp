#include "index.h"

Index::Index(void)
{
 index_attribs[UNIQUE]=false;
 index_attribs[CONCURRENT]=false;
 obj_type=OBJ_INDEX;
 fill_factor=90;
 attributes[ParsersAttributes::UNIQUE]="";
 attributes[ParsersAttributes::CONCURRENT]="";
 attributes[ParsersAttributes::TABLE]="";
 attributes[ParsersAttributes::INDEX_TYPE]="";
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::EXPRESSION]="";
 attributes[ParsersAttributes::FACTOR]="";
 attributes[ParsersAttributes::CONDITION]="";
 attributes[ParsersAttributes::OP_CLASS]="";
 attributes[ParsersAttributes::NULLS_FIRST]="";
 attributes[ParsersAttributes::ASC_ORDER]="";
 attributes[ParsersAttributes::DECL_IN_TABLE]="";
 attributes[ParsersAttributes::ELEMENTS]="";
 attributes[ParsersAttributes::FAST_UPDATE]="";
}

void Index::setElementsAttribute(unsigned def_type)
{
 QString str_elem;
 unsigned i, count;

 count=elements.size();
 for(i=0; i < count; i++)
 {
  str_elem+=elements[i].getCodeDefinition(def_type);
  if(i < (count-1) && def_type==SchemaParser::SQL_DEFINITION) str_elem+=",";
 }

 attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

int Index::isElementExists(Column *column)
{
 int count, idx;
 Column *col=NULL;
 bool found=false;

 idx=0;
 count=elements.size();

 while(idx < count && !found)
 {
  //Obtém a coluna
  col=elements[idx].getColumn();

  if(col && column)
   found=(col==column || col->getName()==column->getName());

  if(!found) idx++;
 }

 if(!found) idx=-1;
 return(idx);
}

int Index::isElementExists(const QString &expr)
{
 int count, idx;
 bool found=false;

 idx=0;
 count=elements.size();

 while(idx < count && !found)
 {
  found=(!expr.isEmpty() && elements[idx].getExpression()==expr);
  if(!found) idx++;
 }

 if(!found) idx=-1;
 return(idx);
}

void Index::addElement(const QString &expr, OperatorClass *op_class, bool asc_order, bool nulls_first)
{
 //Caso a expressão esteja vazia, dispara exceção.
 if(expr.isEmpty())
 {
  throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  IndexElement elem;

  //Caso a expressão a ser atribuída ao índice já exista, dispara-se uma exceção
  if(isElementExists(expr) >= 0)
   throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.setExpression(expr);
  elem.setOperatorClass(op_class);
  elem.setSortAttribute(IndexElement::NULLS_FIRST, nulls_first);
  elem.setSortAttribute(IndexElement::ASC_ORDER, asc_order);

  //Adiciona o elemento ao final da lista de elementos do índice
  elements.push_back(elem);
 }
}

void Index::addElement(Column *column, OperatorClass *op_class, bool asc_order, bool nulls_first)
{
 //Caso a coluna não esteja aloca, dispara exceção.
 if(!column)
 {
 throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
                        .arg(QString::fromUtf8(this->getName()))
                        .arg(BaseObject::getTypeName(OBJ_INDEX)),
               ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  IndexElement elem;

  //Caso a coluna a ser atribuída ao índice já exista, dispara-se uma exceção
  if(isElementExists(column) >= 0)
   throw Exception(ERR_INS_DUPLIC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.setColumn(column);
  elem.setOperatorClass(op_class);
  elem.setSortAttribute(IndexElement::NULLS_FIRST, nulls_first);
  elem.setSortAttribute(IndexElement::ASC_ORDER, asc_order);

  //Adiciona o elemento ao final da lista de elementos do índice
  elements.push_back(elem);
 }
}

void Index::removeElement(unsigned idx_elem)
{
 vector<IndexElement>::iterator itr;

 /* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(idx_elem < elements.size())
  throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  elements.erase(elements.begin() + idx_elem);
}

void Index::removeElements(void)
{
 elements.clear();
}

void Index::setIndexAttribute(unsigned attrib_id, bool value)
{
 if(attrib_id > FAST_UPDATE)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 index_attribs[attrib_id]=value;
}

void Index::setFillFactor(unsigned factor)
{
 fill_factor=factor;
}

void Index::setIndexingType(TipoIndexacao idx_type)
{
 this->indexing_type=idx_type;
}

void Index::setConditionalExpression(const QString &expr)
{
 conditional_expr=expr;
}

unsigned Index::getFillFactor(void)
{
 return(fill_factor);
}

IndexElement Index::getElement(unsigned elem_idx)
{
/* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(elem_idx >= elements.size())
  throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(elements[elem_idx]);
}

unsigned Index::getElementCount(void)
{
 return(elements.size());
}

bool Index::getIndexAttribute(unsigned attrib_id)
{
 if(attrib_id > FAST_UPDATE)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(index_attribs[attrib_id]);
}

TipoIndexacao Index::getIndexingType(void)
{
 return(indexing_type);
}

QString Index::getConditionalExpression(void)
{
 return(conditional_expr);
}

bool Index::isReferRelationshipColumn(void)
{
 vector<IndexElement>::iterator itr, itr_end;
 Column *col=NULL;
 bool enc=false;

 /* Primeira lista de elementos é que será varrida
    para isso as referências ao primeiro e ultimo elementos
    serão obtidas */
 itr=elements.begin();
 itr_end=elements.end();

 /* Efetua uma iteração verifica se as colunas dos elementos
    foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que a restrição referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !enc)
 {
  //Obtém a coluna
  col=(*itr).getColumn();

  //Obtém se a coluna foi incluída por relacionamento ou não
  enc=(col && col->isAddedByRelationship());
  //Passa para a próxima coluna
  itr++;
 }

 return(enc);
}

QString Index::getCodeDefinition(unsigned tipo_def)
{
 setElementsAttribute(tipo_def);
 attributes[ParsersAttributes::UNIQUE]=(index_attribs[UNIQUE] ? "1" : "");
 attributes[ParsersAttributes::CONCURRENT]=(index_attribs[CONCURRENT] ? "1" : "");
 attributes[ParsersAttributes::FAST_UPDATE]=(index_attribs[FAST_UPDATE] ? "1" : "");
 attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
 attributes[ParsersAttributes::CONDITION]=conditional_expr;

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fill_factor);

 /* Caso o índice não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração do índice está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!isReferRelationshipColumn())
  attributes[ParsersAttributes::DECL_IN_TABLE]="1";

 return(BaseObject::__getCodeDefinition(tipo_def));
}

