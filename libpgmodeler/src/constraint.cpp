#include "constraint.h"

Constraint::Constraint(void)
{
 ref_table=NULL;
 obj_type=OBJ_CONSTRAINT;
 deferrable=false;
 fill_factor=100;

 attributes[ParsersAttributes::PK_CONSTR]="";
 attributes[ParsersAttributes::FK_CONSTR]="";
 attributes[ParsersAttributes::CK_CONSTR]="";
 attributes[ParsersAttributes::UQ_CONSTR]="";
 attributes[ParsersAttributes::REF_TABLE]="";
 attributes[ParsersAttributes::SRC_COLUMNS]="";
 attributes[ParsersAttributes::DST_COLUMNS]="";
 attributes[ParsersAttributes::DEL_ACTION]="";
 attributes[ParsersAttributes::UPD_ACTION]="";
 attributes[ParsersAttributes::EXPRESSION]="";
 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::COMPARISON_TYPE]="";
 attributes[ParsersAttributes::DEFER_TYPE]="";
 attributes[ParsersAttributes::DEFERRABLE]="";
 attributes[ParsersAttributes::TABLE]="";
 attributes[ParsersAttributes::DECL_IN_TABLE]="";
 attributes[ParsersAttributes::FACTOR]="";
}

Constraint::~Constraint(void)
{
 removeColumns();
}

void Constraint::setConstraintType(TipoRestricao constr_type)
{
 this->constr_type=constr_type;
}

void Constraint::setActionType(TipoAcao action_type, bool upd)
{
 //Se upd==true o tipo de ação no update é que será definido
 if(upd)
  this->upd_action=action_type;
 else
  this->del_action=action_type;
}

void Constraint::setCheckExpression(const QString &expr)
{
 check_expr=expr;
}

bool Constraint::isColumnExists(Column *column, unsigned col_type)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col_aux=NULL;
 bool found=false;

 //Caso a coluna a ser buscada não esteja aloca, dispara uma exceção
 if(!column)
  throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(col_type==SOURCE_COL)
 {
  itr=columns.begin();
  itr_end=columns.end();
 }
 else
 {
  itr=ref_columns.begin();
  itr_end=ref_columns.end();
 }

 /* Varre a lista de colunas selecionada verificando se o nome da
    coluna é igual ao nome de uma das colunas da lista ou mesmo
    se os endereços das colunas envolvidas são iguais */
 while(itr!=itr_end && !found)
 {
  col_aux=(*itr);
  found=(col_aux==column || col_aux->getName()==column->getName());
  itr++;
 }

 return(found);
}

void Constraint::addColumn(Column *column, unsigned col_type)
{
 //Caso a coluna não esteja aloca, dispara exceção.
 if(!column)
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
                        .arg(QString::fromUtf8(this->getName()))
                        .arg(BaseObject::getTypeName(OBJ_CONSTRAINT)),
                 ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(constr_type!=TipoRestricao::check)
 {
  //Só adiciona a coluna em uma das lista caso a mesma não exista em uma delas
  if(!isColumnExists(column,col_type))
  {
   //Caso a coluna a ser atribuida seja da lista de colunas de destino
   if(col_type==REFERENCED_COL)
    //Insere a coluna na lista de destino
    ref_columns.push_back(column);
   else
    columns.push_back(column);
  }
 }
}

void Constraint::setTablespace(Tablespace *tabspc)
{
 try
 {
  if(tabspc &&
     constr_type!=TipoRestricao::primary_key &&
     constr_type!=TipoRestricao::unique)
   throw Exception(ERR_ASG_TABSPC_INV_CONSTR_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  BaseObject::setTablespace(tabspc);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}

void Constraint::setColumnsAttribute(unsigned col_type, unsigned def_type, bool inc_addedbyrel)
{
 vector<Column *> *col_vector=NULL;
 Column *col=NULL;
 QString str_cols, attrib;
 unsigned i, count;
 bool formatar=(def_type==SchemaParser::SQL_DEFINITION);

 /* Caso a coluna selecionada seja a de destino,
    obtém a lista de colunas de destino e marca
    que o atributo a ser configurado é do de
    colunas da tabela de destino */
 if(col_type==REFERENCED_COL)
 {
  col_vector=&ref_columns;
  attrib=ParsersAttributes::DST_COLUMNS;
 }
 else
 {
  col_vector=&columns;
  attrib=ParsersAttributes::SRC_COLUMNS;
 }

 /* Varre a lista de colunas contatenando os nomes
    dos elementos separando-os por vírgula */
 count=col_vector->size();
 for(i=0; i < count; i++)
 {
  col=col_vector->at(i);

  /* No caso de definição XML as colunas protegidas (adicionaa   restrição
     por relacionamento) não podem ser incluídas pois estas serão inseridas
     na restrição no momento da criação do relacionamento a partir do XML respectivo
     por isso o parâmetro 'inc_insporrelacao' pode ser usado para resolver esse caso. */
  if((def_type==SchemaParser::SQL_DEFINITION) ||
     ((def_type==SchemaParser::XML_DEFINITION) &&
      ((inc_addedbyrel && col->isAddedByRelationship()) ||
       (inc_addedbyrel && !col->isAddedByRelationship()) ||
       (!inc_addedbyrel && !col->isAddedByRelationship()))))
  {
   str_cols+=col->getName(formatar);
   str_cols+=",";
  }
 }

 str_cols.remove(str_cols.size()-1,1);
 attributes[attrib]=str_cols;
}

void Constraint::setReferencedTable(BaseObject *tab_ref)
{
 this->ref_table=tab_ref;
}

void Constraint::setDeferralType(TipoPostergacao deferral_type)
{
 this->deferral_type=deferral_type;
}

void Constraint::setDeferrable(bool value)
{
 deferrable=value;
}

void Constraint::setMatchType(TipoComparacao match_type)
{
 this->match_type=match_type;
}

void Constraint::setFillFactor(unsigned factor)
{
 if(factor < 10) factor=10;
 fill_factor=factor;
}

unsigned Constraint::getFillFactor(void)
{
 return(fill_factor);
}

TipoRestricao Constraint::getConstraintType(void)
{
 return(constr_type);
}

TipoAcao Constraint::getActionType(bool upd)
{
 //Se upd==true o tipo de ação no update é que será retornado
 if(upd)
  return(upd_action);
 else
  return(del_action);
}

QString Constraint::getCheckExpression(void)
{
 return(check_expr);
}

Column *Constraint::getColumn(unsigned col_idx, unsigned col_type)
{
 vector<Column *> *col_list=NULL;

 //Obtendo a lista de colunas de acorodo com o tipo de coluna selecionado
 col_list=(col_type==SOURCE_COL ? &columns : &ref_columns);

 /* Caso o índice de coluna a ser obtido seja inválido, um erro
    será retornado */
 if(col_idx>=col_list->size())
  throw Exception(ERR_REF_COLUMN_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna a coluna no índice especificado
 return(col_list->at(col_idx));
}

Column *Constraint::getColumn(const QString &name, unsigned col_type)
{
 bool found=false;
 vector<Column *> *col_list=NULL;
 vector<Column *>::iterator itr_col, itr_end_col;

 //Obtém a lista de colunas de acordo com o tipo
 col_list=(col_type==SOURCE_COL? &columns : &ref_columns);

 //Obtém as referencias para o primeiro e o ultimo elemento da lista de colunas
 itr_col=col_list->begin();
 itr_end_col=col_list->end();

 //Varre a lista de colunas verificando se existe alguma ocorrencia do nome passado
 while(itr_col!=itr_end_col)
 {
  found=((*itr_col)->getName()==name);
  //Caso o no não seja encontrado passa para o próximo elemento
  if(!found) itr_col++;
  else break; //Caso seja encontrado, encerra a execuação da varredura
 }

 //caso seja encontrada, retorna a coluna senão retorna nulo
 if(found) return(*itr_col);
 else return(NULL);
}

BaseObject *Constraint::getReferencedTable(void)
{
 return(ref_table);
}

unsigned Constraint::getColumnCount(unsigned col_type)
{
 if(col_type==REFERENCED_COL)
  return(ref_columns.size());
 else
  return(columns.size());
}

void Constraint::removeColumns(void)
{
 columns.clear();
 ref_columns.clear();
}

void Constraint::removeColumn(const QString &name, unsigned col_type)
{
 vector<Column *>::iterator itr, itr_end;
 vector<Column *> *cols=NULL;
 Column *col=NULL;

 //Se col_dest==true, a lista a ser pesquisada será a de destino
 if(col_type==REFERENCED_COL)
  //Selecionando a lista de destino para pesquisa
  cols=&ref_columns;
 else
  cols=&columns;

 //Obtém a referência ao primeiro elemento da lista selecionada
 itr=cols->begin();
 //Obtém a referência ao ultimo elemento da lista selecionada
 itr_end=cols->end();

 /* Efetua um iteração comparando o nome de cada coluna da lista
    com o nome que se deseja encontrar */
 while(itr!=itr_end)
 {
  col=(*itr); //Obtém a coluna

  //Caso o nome da coluna coincida com o nome a se localizar
  if(col->getName()==name)
  {
   //Remove o elemento da lista
   cols->erase(itr);
   break;
  }
  else itr++; //Passa para a próxima coluna
 }
}

TipoPostergacao Constraint::getDeferralType(void)
{
 return(deferral_type);
}

bool Constraint::isDeferrable(void)
{
 return(deferrable);
}

bool Constraint::isReferRelationshipColumn(void)
{
 vector<Column *>::iterator itr, itr_end;
 Column *col=NULL;
 bool found=false;

 /* Primeira lista de colunas da origem é que será varrida
    para isso as referências ao primeiro e ultimo elementos
    serão obtidas */
 itr=columns.begin();
 itr_end=columns.end();

 /* Efetua uma iteração verifica se as colunas da lista
    atual foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que a restrição referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !found)
 {
  //Obtém a coluna
  col=(*itr);
  //Obtém se a coluna foi incluída por relacionamento ou não
  found=col->isAddedByRelationship();
  //Passa para a próxima coluna
  itr++;

  /* Caso a lista de colunas de origem foi completamente varrida
     e nenhuma das colunas desta vieram de relacionamentos, a
     lista de colunas referenciadas é que será varrida com o
     mesmo intuito */
  if(itr==itr_end && itr_end!=ref_columns.end() && !found)
  {
   itr=ref_columns.begin();
   itr_end=ref_columns.end();
  }
 }

 return(found);
}

TipoComparacao Constraint::getMatchType(void)
{
 return(match_type);
}

QString Constraint::getCodeDefinition(unsigned def_type)
{
 return(getCodeDefinition(def_type, false));
}

QString Constraint::getCodeDefinition(unsigned def_type, bool inc_addedbyrel)
{
 QString attrib;

 attributes[ParsersAttributes::PK_CONSTR]="";
 attributes[ParsersAttributes::FK_CONSTR]="";
 attributes[ParsersAttributes::CK_CONSTR]="";
 attributes[ParsersAttributes::UQ_CONSTR]="";

 switch(!constr_type)
 {
  case TipoRestricao::check:
   attrib=ParsersAttributes::CK_CONSTR;
  break;
  case TipoRestricao::primary_key:
   attrib=ParsersAttributes::PK_CONSTR;
  break;
  case TipoRestricao::foreign_key:
   attrib=ParsersAttributes::FK_CONSTR;
  break;
  case TipoRestricao::unique:
   attrib=ParsersAttributes::UQ_CONSTR;
  break;
 }
 attributes[attrib]="1";

 attributes[ParsersAttributes::TYPE]=attrib;
 attributes[ParsersAttributes::UPD_ACTION]=(~upd_action);
 attributes[ParsersAttributes::DEL_ACTION]=(~del_action);
 attributes[ParsersAttributes::EXPRESSION]=check_expr;

 if(constr_type!=TipoRestricao::check)
 {
  setColumnsAttribute(SOURCE_COL, def_type, inc_addedbyrel);

  /* Só gera a definição das colunas referenciadas da chave estrangeira
     caso o número de colunas da origem e destino sejam iguais, isso significa
     que a chave está configurada corretamente, caso contrário não gera o atributo
     forçando o parser de esquemas a retornar um erro pois a chave estrangeira está
     mal configurada. */
  if(constr_type==TipoRestricao::foreign_key && columns.size() == ref_columns.size())
   setColumnsAttribute(REFERENCED_COL, def_type, inc_addedbyrel);
 }

 attributes[ParsersAttributes::REF_TABLE]=(ref_table ? ref_table->getName(true) : "");
 attributes[ParsersAttributes::DEFERRABLE]=(deferrable ? "1" : "");
 attributes[ParsersAttributes::COMPARISON_TYPE]=(~match_type);
 attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

 /* Caso a restrição não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração da restrição está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!isReferRelationshipColumn() || constr_type==TipoRestricao::primary_key)
  attributes[ParsersAttributes::DECL_IN_TABLE]="1";

 if(constr_type==TipoRestricao::primary_key || constr_type==TipoRestricao::unique)
  attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fill_factor);
 else
  attributes[ParsersAttributes::FACTOR]="";

 return(BaseObject::__getCodeDefinition(def_type));
}

