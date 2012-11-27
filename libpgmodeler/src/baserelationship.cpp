#include "baserelationship.h"
#include <QApplication>

BaseRelationship::BaseRelationship(BaseRelationship *rel)
{
 if(!rel)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Inicializa com NULL os rótulos pois os mesmos são copiados no operador =
 for(unsigned i=0; i < 3; i++)
  lables[i]=NULL;

 (*(this))=(*rel);
}

BaseRelationship::BaseRelationship(unsigned rel_type, BaseTable *src_tab, BaseTable *dst_tab,
                                   bool src_mandatory, bool dst_mandatory)

{
 try
 {
  QString str_aux;

  this->connected=false;
  this->dst_mandatory=dst_mandatory;
  this->src_mandatory=src_mandatory;
  this->src_table=src_tab;
  this->dst_table=dst_tab;
  this->rel_type=rel_type;

  for(unsigned i=0; i < 3; i++)
  {
   lables[i]=NULL;
   lables_dist[i]=QPointF(NAN, NAN);
  }

  configureRelationship();

  str_aux=QApplication::translate("RelacionamentoBase","rel_%1_%2","",QApplication::UnicodeUTF8);
  setName(str_aux.arg(src_tab->getName()).arg(dst_tab->getName()));
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void BaseRelationship::configureRelationship(void)
{
 obj_type=BASE_RELATIONSHIP;

 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::SRC_REQUIRED]="";
 attributes[ParsersAttributes::DST_REQUIRED]="";
 attributes[ParsersAttributes::SRC_TABLE]="";
 attributes[ParsersAttributes::DST_TABLE]="";
 attributes[ParsersAttributes::POINTS]="";
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::CONSTRAINTS]="";
 attributes[ParsersAttributes::ELEMENTS]="";
 attributes[ParsersAttributes::SRC_SUFFIX]="";
 attributes[ParsersAttributes::DST_SUFFIX]="";
 attributes[ParsersAttributes::AUTO_SUFFIX]="";
 attributes[ParsersAttributes::IDENTIFIER]="";
 attributes[ParsersAttributes::REDUCED_FORM]="";
 attributes[ParsersAttributes::DEFERRABLE]="";
 attributes[ParsersAttributes::DEFER_TYPE]="";
 attributes[ParsersAttributes::TABLE_NAME]="";
 attributes[ParsersAttributes::SPECIAL_PK_COLS]="";


 //Verifica se o tipo de relacionamento é valido
 if(rel_type <= RELATIONSHIP_DEP)
 {
  try
  {
   /* Verifica se uma das tabelas envolvidas no relacionamentos
      não estão alocadas, caso isso ocorra, dispara uma exceção */
   if(!src_table || !dst_table)
    throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_TABLE)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                  ERR_ASG_NOT_ALOC_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   /* Caso o tipo de relacionamento seja de generalização ou de dependência
      e as tabelas de origem e destino forem iguais, será disparada uma exceção
      pois uma tabela não pode herdar/copiar atributos dela mesma */
   if((rel_type==RELATIONSHIP_GEN ||
       rel_type==RELATIONSHIP_DEP) && src_table==dst_table)
    throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   /* Aloca o rótulo de nome do relacionamento, todos os tipos de relacionamento
      possui esse rótulo */
   lables[LABEL_REL_NAME]=new Textbox;
   lables[LABEL_REL_NAME]->setTextAttribute(Textbox::ITALIC_TXT, true);

   /* Alocando os rótulos de cardinalidade, exceto para os relacionamentos
      de generalização e dependência, pois esses não trabalham com cardinalidade */
   if(rel_type!=RELATIONSHIP_GEN &&
      rel_type!=RELATIONSHIP_DEP)
   {
    //Aloca os rótulos
    lables[LABEL_SRC_CARD]=new Textbox;
    lables[LABEL_DST_CARD]=new Textbox;
    lables[LABEL_SRC_CARD]->setTextAttribute(Textbox::ITALIC_TXT, true);
    lables[LABEL_DST_CARD]->setTextAttribute(Textbox::ITALIC_TXT, true);

    /* Define a obrigatoriedade das tabelas e já cria o texto
       dos rótulos de cardinalidade */
    setMandatoryTable(SRC_TABLE,dst_mandatory);
    setMandatoryTable(DST_TABLE,src_mandatory);
   }
  }
  catch(bad_alloc &e)
  {
   throw Exception(ERR_GLOBAL_OBJBADALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }
 else
  throw Exception(ERR_ALOC_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

BaseRelationship::~BaseRelationship(void)
{
 //Desconecta o relacionamento
 disconnectRelationship();

 //Exclui os rótulos
 for(unsigned i=0; i<3; i++)
  if(lables[i]) delete(lables[i]);
}

void BaseRelationship::setName(const QString &name)
{
 try
 {
  BaseObject::setName(name);

  if(lables[LABEL_REL_NAME])
   lables[LABEL_REL_NAME]->setComment(name);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void BaseRelationship::setMandatoryTable(unsigned table_id, bool value)
{
 QString cmin, aux;
 unsigned label_id;

 /* Retona um erro ao usuário indicando que a combinação de cardinalidade
    e tipo de relacionamento não é implementado. O unico caso onde isso
    acontece é no relacionamento do tipo (1,1)-<>-(1,1) */
 if(rel_type==RELATIONSHIP_11 &&
    ((table_id==SRC_TABLE && value && src_mandatory) ||
     (table_id==DST_TABLE && value && dst_mandatory)))
  throw Exception(ERR_NOT_IMPL_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso a tabela de origem seja obrigatório
 if(table_id==SRC_TABLE)
 {
  dst_mandatory=value;
  /* Indica que o rótulo de cardinalidade
     de origem será configurado */
  label_id=LABEL_SRC_CARD;
 }
 else
 {
  if(rel_type!=RELATIONSHIP_1N)
   src_mandatory=value;
  else
   /* Relacionamento do tipo 1-n,  a entidade do lado
      n sempre será no formato (0,n) */
   src_mandatory=false;

  /* Indica que o rótulo de cardinalidade
     de destino será configurado */
  label_id=LABEL_DST_CARD;
 }

 /* Caso o valor seja false, indica que a participação
    da tabela em questão é opcional */
 if(!value) cmin="0";
 else cmin="1";

 if(lables[label_id])
 {
  //Configurando o rótulo para o relacionamento 1-1
  if(rel_type==RELATIONSHIP_11)
   lables[label_id]->setComment("(" + cmin + ",1)");
  //Configurando o rótulo para o relacionamento 1-n
  else if(rel_type==RELATIONSHIP_1N)
  {
   aux=(table_id==SRC_TABLE ? "1" : "n");
   lables[label_id]->setComment("(" + cmin + "," + aux + ")");
  }
  else if(rel_type==RELATIONSHIP_NN)
  {
   /* A cardinalidade de relacionamentos n-n sempre será 0-n pois
      será gerada uma tabela própria, portanto a cardinalidade mínima
      é ignorada */
   //rotulos[tipo_rot]->definirComentario("(" + cmin + ",n)");
   lables[label_id]->setComment("(n)");
  }
  lables[label_id]->setModified(true);
 }
}

BaseTable *BaseRelationship::getTable(unsigned table_id)
{
 //Retorna a tabela de acordo o índice passado
 if(table_id==SRC_TABLE)
  return(src_table);
 else if(table_id==DST_TABLE)
  return(dst_table);
 else
  return(NULL);
}

bool BaseRelationship::isTableMandatory(unsigned table_id)
{
 /* Retorna o flag de obrigatoriedade de acordo o índice
    da tabela passado */
 if(table_id==SRC_TABLE)
  return(dst_mandatory);
 else
  return(src_mandatory);
}

void BaseRelationship::disconnectRelationship(void)
{
 if(connected)
 {
  connected=false;
  src_table->setModified(true);
  dst_table->setModified(true);
 }
}

void BaseRelationship::connectRelationship(void)
{
 //Caso o relacionamento não esteja conectado
 if(!connected)
 {
  //Indica que o relacionamento está conectado
  connected=true;

  /* Marca que as tabelas do relacionamentos estão modificadas para forçar
     o redimensionamento das mesmas */
  src_table->setModified(true);
  dst_table->setModified(true);
 }
}

Textbox *BaseRelationship::getLabel(unsigned label_id)
{
 // Retorna o rótulo na posição especificada, caso o índice
 //   do mesmo seja válido
 if(label_id<=LABEL_REL_NAME)
  return(lables[label_id]);
 else
  //Dispara uma exceção caso o índice usado seja inválido
  throw Exception(ERR_REF_LABEL_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
 
unsigned BaseRelationship::getRelationshipType(void)
{
 return(rel_type);
}

bool BaseRelationship::isRelationshipConnected(void)
{
 return(connected);
}

bool BaseRelationship::isSelfRelationship(void)
{
 return(dst_table==src_table);
}

void BaseRelationship::setRelationshipAttributes(void)
{
 unsigned count, i;
 QString str_aux,
         label_attribs[3]={ ParsersAttributes::SRC_LABEL,
                            ParsersAttributes::DST_LABEL,
                            ParsersAttributes::NAME_LABEL};

 //Definindo o atributo de tipo de relacionamento
 switch(rel_type)
 {
  case RELATIONSHIP_11: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_11; break;
  case RELATIONSHIP_1N: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_1N; break;
  case RELATIONSHIP_NN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_NN; break;
  case RELATIONSHIP_GEN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_GEN; break;
  default:
    if(src_table->getObjectType()==OBJ_VIEW)
     attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATION_TAB_VIEW;
    else
     attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_DEP;
  break;
 }

 attributes[ParsersAttributes::SRC_REQUIRED]=(dst_mandatory ? "1" : "");
 attributes[ParsersAttributes::DST_REQUIRED]=(src_mandatory ? "1" : "");

 if(src_table)
  attributes[ParsersAttributes::SRC_TABLE]=src_table->getName(true);

 if(dst_table)
  attributes[ParsersAttributes::DST_TABLE]=dst_table->getName(true);


 //Criando a definição XML da linha do relacionamento
 count=points.size();
 for(i=0; i < count; i++)
 {
  attributes[ParsersAttributes::X_POS]=QString("%1").arg(points[i].x());
  attributes[ParsersAttributes::Y_POS]=QString("%1").arg(points[i].y());
  str_aux+=SchemaParser::getObjectDefinition(ParsersAttributes::POSITION,
                                                attributes, SchemaParser::XML_DEFINITION);
 }
 attributes[ParsersAttributes::POINTS]=str_aux;

 //Obtendo a posição dos rótulos
 str_aux="";
 for(i=0; i < 3; i++)
 {
  if(!isnan(lables_dist[i].x()))
  {
   attributes[ParsersAttributes::X_POS]=QString("%1").arg(lables_dist[i].x());
   attributes[ParsersAttributes::Y_POS]=QString("%1").arg(lables_dist[i].y());
   attributes[ParsersAttributes::POSITION]=SchemaParser::getObjectDefinition(ParsersAttributes::POSITION,
                                                 attributes, SchemaParser::XML_DEFINITION);
   attributes[ParsersAttributes::REF_TYPE]=label_attribs[i];
   str_aux+=SchemaParser::getObjectDefinition(ParsersAttributes::LABEL,
                                                attributes, SchemaParser::XML_DEFINITION);
  }
 }
 attributes[ParsersAttributes::LABELS_POS]=str_aux;
}

QString BaseRelationship::getCodeDefinition(void)
{
 bool reduced_form;

 setRelationshipAttributes();
 reduced_form=attributes[ParsersAttributes::POINTS].isEmpty();
 return(BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION,reduced_form));
}

void BaseRelationship::setPoints(const vector<QPointF> &points)
{
 this->points=points;
}

void BaseRelationship::setLabelDistance(unsigned label_id, QPointF label_dist)
{
 if(label_id > LABEL_REL_NAME)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->lables_dist[label_id]=label_dist;
}

QPointF BaseRelationship::getLabelDistance(unsigned label_id)
{
 if(label_id > LABEL_REL_NAME)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(this->lables_dist[label_id]);
}

vector<QPointF> BaseRelationship::getPoints(void)
{
 return(points);
}

void BaseRelationship::operator = (BaseRelationship &rel)
{
 (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(rel);
 //this->conectado=rel.conectado;
 this->connected=false;
 this->src_table=rel.src_table;
 this->dst_table=rel.dst_table;
 this->rel_type=rel.rel_type;
 this->points=rel.points;

 /* Inicializa as distâncias dos rótulos com NAN.
    Quando este valor está presente em uma das coordenadas
    de uma distância de rótulo, o mesmo sempre será
    ajustado automaticamente */
 for(int i=0; i < 3; i++)
 {
  if(rel.lables[i])
  {
   if(!this->lables[i])
    this->lables[i]=new Textbox;

   (*this->lables[i])=(*rel.lables[i]);
  }
  this->lables_dist[i]=rel.lables_dist[i];
 }


 /* Define ambas as tabelas como não obrigatórias temporariamente a fim de evitar
    um disparo de erro prematuro, vide definirTabelaObrigatoria() */
 this->setMandatoryTable(SRC_TABLE, false);
 this->setMandatoryTable(DST_TABLE, false);

 this->setMandatoryTable(SRC_TABLE, rel.dst_mandatory);
 this->setMandatoryTable(DST_TABLE, rel.src_mandatory);
}

