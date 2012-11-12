#include "index.h"

Index::Index(void)
{
 atrib_indice[UNIQUE]=false;
 atrib_indice[CONCORRENTE]=false;
 obj_type=OBJ_INDEX;
 fator_preenc=90;
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

void Index::definirAtributoElementos(unsigned tipo_def)
{
 QString str_elem;
 unsigned i, qtd;

 qtd=elementos.size();
 for(i=0; i < qtd; i++)
 {
  str_elem+=elementos[i].getCodeDefinition(tipo_def);
  if(i < (qtd-1) && tipo_def==SchemaParser::SQL_DEFINITION) str_elem+=",";
 }

 attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

int Index::elementoExiste(Column *coluna)
{
 int qtd, idx;
 Column *col=NULL;
 bool enc=false;

 idx=0;
 qtd=elementos.size();

 while(idx < qtd && !enc)
 {
  //Obtém a coluna
  col=elementos[idx].getColumn();

  if(col && coluna)
   enc=(col==coluna || col->getName()==coluna->getName());

  if(!enc) idx++;
 }

 if(!enc) idx=-1;
 return(idx);
}

int Index::elementoExiste(const QString &expressao)
{
 int qtd, idx;
 bool enc=false;

 idx=0;
 qtd=elementos.size();

 while(idx < qtd && !enc)
 {
  enc=(!expressao.isEmpty() && elementos[idx].getExpression()==expressao);
  if(!enc) idx++;
 }

 if(!enc) idx=-1;
 return(idx);
}

void Index::adicionarElemento(const QString &expressao, OperatorClass *classe_oper, bool ordem_asc, bool nulos_primeiro)
{
 //Caso a expressão esteja vazia, dispara exceção.
 if(expressao.isEmpty())
 {
  throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  IndexElement elem;

  //Caso a expressão a ser atribuída ao índice já exista, dispara-se uma exceção
  if(elementoExiste(expressao) >= 0)
   throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.setExpression(expressao);
  elem.setOperatorClass(classe_oper);
  elem.setSortAttribute(IndexElement::NULLS_FIRST, nulos_primeiro);
  elem.setSortAttribute(IndexElement::ASC_ORDER, ordem_asc);

  //Adiciona o elemento ao final da lista de elementos do índice
  elementos.push_back(elem);
 }
}

void Index::adicionarElemento(Column *coluna, OperatorClass *classe_oper, bool ordem_asc, bool nulos_primeiro)
{
 //Caso a coluna não esteja aloca, dispara exceção.
 if(!coluna)
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
  if(elementoExiste(coluna) >= 0)
   throw Exception(ERR_INS_DUPLIC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  elem.setColumn(coluna);
  elem.setOperatorClass(classe_oper);
  elem.setSortAttribute(IndexElement::NULLS_FIRST, nulos_primeiro);
  elem.setSortAttribute(IndexElement::ASC_ORDER, ordem_asc);

  //Adiciona o elemento ao final da lista de elementos do índice
  elementos.push_back(elem);
 }
}

void Index::removerElemento(unsigned idx_elem)
{
 vector<IndexElement>::iterator itr;

 /* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(idx_elem < elementos.size())
  throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  elementos.erase(elementos.begin() + idx_elem);
}

void Index::removerElementos(void)
{
 elementos.clear();
}

void Index::definirAtributo(unsigned id_atrib, bool valor)
{
 if(id_atrib > ATUAL_RAPIDA)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 atrib_indice[id_atrib]=valor;
}

void Index::definirFatorPreenchimento(unsigned fator)
{
 fator_preenc=fator;
}

void Index::definirTipoIndexacao(TipoIndexacao tipo_indexacao)
{
 this->tipo_indexacao=tipo_indexacao;
}

void Index::definirExpCondicional(const QString &exp)
{
 exp_condicional=exp;
}

unsigned Index::obterFatorPreenchimento(void)
{
 return(fator_preenc);
}

IndexElement Index::obterElemento(unsigned idx_elem)
{
/* Verifica se o índice condiz com o tamanho das listas de elementos,
   caso não conincida, dispara exceção */
 if(idx_elem >= elementos.size())
  throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(elementos[idx_elem]);
}

unsigned Index::obterNumElementos(void)
{
 return(elementos.size());
}

bool Index::obterAtributo(unsigned id_atrib)
{
 if(id_atrib > ATUAL_RAPIDA)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atrib_indice[id_atrib]);
}

TipoIndexacao Index::obterTipoIndexacao(void)
{
 return(tipo_indexacao);
}

QString Index::obterExpCondicional(void)
{
 return(exp_condicional);
}

bool Index::referenciaColunaIncRelacao(void)
{
 vector<IndexElement>::iterator itr, itr_end;
 Column *col=NULL;
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
 definirAtributoElementos(tipo_def);
 attributes[ParsersAttributes::UNIQUE]=(atrib_indice[UNIQUE] ? "1" : "");
 attributes[ParsersAttributes::CONCURRENT]=(atrib_indice[CONCORRENTE] ? "1" : "");
 attributes[ParsersAttributes::FAST_UPDATE]=(atrib_indice[ATUAL_RAPIDA] ? "1" : "");
 attributes[ParsersAttributes::INDEX_TYPE]=(~tipo_indexacao);
 attributes[ParsersAttributes::CONDITION]=exp_condicional;

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fator_preenc);

 /* Caso o índice não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração do índice está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!referenciaColunaIncRelacao())
  attributes[ParsersAttributes::DECL_IN_TABLE]="1";

 return(BaseObject::__getCodeDefinition(tipo_def));
}

