#include "tableobjectview.h"

const QString TableObjectView::TYPE_SEPARATOR(" ");
const QString TableObjectView::CONSTR_SEPARATOR(" ");
const QString TableObjectView::TXT_UNIQUE("uq");
const QString TableObjectView::TXT_PRIMARY_KEY("pk");
const QString TableObjectView::TXT_FOREIGN_KEY("fk");
const QString TableObjectView::TXT_NOT_NULL("nn");
const QString TableObjectView::DELIMITADOR_REST_INI("«");
const QString TableObjectView::DELIMITADOR_REST_FIM("»");

TableObjectView::TableObjectView(TableObject *object) : BaseObjectView(object)
{
 //O descritor é alocado no método de configuração conforme o tipo do objeto de origem
 descriptor=NULL;

 //Aloca os rótulos e os adiciona ao grupo
 for(unsigned i=0; i < 3; i++)
 {
  lables[i]=new QGraphicsSimpleTextItem;
  this->addToGroup(lables[i]);
 }
}

TableObjectView::~TableObjectView(void)
{
 //Desaloca e remove do grupo todos os objetos filhos
 this->removeFromGroup(descriptor);
 delete(descriptor);

 for(unsigned i=0; i < 3; i++)
 {
  this->removeFromGroup(lables[i]);
  delete(lables[i]);
 }
}

void TableObjectView::configureDescriptor(ConstraintType constr_type)
{
 ObjectType obj_type=BASE_OBJECT;
 Column *column=dynamic_cast<Column *>(this->getSourceObject());
 float factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;
 bool ellipse_desc=false;

 //Obtém o tipo do objeto de origem, é com base nele que o descritor será alocado
 if(this->getSourceObject())
  obj_type=this->getSourceObject()->getObjectType();

 /* Descritores em forma de elipse são alocados para colunas (com ou sem not-null)
    todos os demais tipos têm descritores poligonais */
 ellipse_desc=((column && constr_type==BaseType::null) ||
              (obj_type!=OBJ_INDEX && obj_type!=OBJ_RULE &&
               obj_type!=OBJ_TRIGGER && obj_type!=OBJ_COLUMN));

 //Destrói o descritor quando o tipo atual é diferente do novo
 if(descriptor && ((ellipse_desc && !dynamic_cast<QGraphicsEllipseItem *>(descriptor)) ||
                  (!ellipse_desc && dynamic_cast<QGraphicsEllipseItem *>(descriptor))))
 {
  this->removeFromGroup(descriptor);
  delete(descriptor);
  descriptor=NULL;
 }

 //Aloca o descritor e o adiciona ao grupo
 if(!descriptor)
 {
  //Caso seja em elipse, cria um QGraphicsEllipseItem
  if(ellipse_desc)
   descriptor=new QGraphicsEllipseItem;
  //Caso contrário que um QGraphicsPolygonItem
  else
   descriptor=new QGraphicsPolygonItem;

  this->addToGroup(descriptor);
 }

 //Caso o objeto de origem seja coluna
 if(column)
 {
  QString attrib;
  QPolygonF pol;

  /* Caso o tipo da restrição não esteja definido (a coluna não tem nenhuma restrição ligada a ela)
     cria um descritor elíptico */
  if(constr_type==BaseType::null)
  {
   QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

   //Cria um descritor elíptico de 10x10 (por padrão) porém aplica o fator entre a fonte padrão e fonte configurada
   desc->setRect(QRectF(QPointF(0,0),
                        QSizeF(9.0f * factor, 9.0f * factor)));

   if(column->isNotNull())
    attrib=ParsersAttributes::NN_COLUMN;
   else
    attrib=ParsersAttributes::COLUMN;

   //Configura o preenchimento de acordo com o atributo selecionado acima
   desc->setBrush(this->getFillStyle(attrib));
   desc->setPen(this->getBorderStyle(attrib));
  }
  //Configura o descritor poligonal
  else
  {
   QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);

   //Cria um polígono conforme o tipo de restrição da coluna
   if(constr_type==ConstraintType::primary_key)
   {
    attrib=ParsersAttributes::PK_COLUMN;
    pol.append(QPointF(2,0)); pol.append(QPointF(0,2)); pol.append(QPointF(0,7));
    pol.append(QPointF(2,9)); pol.append(QPointF(3,8)); pol.append(QPointF(3,6));
    pol.append(QPointF(4,6)); pol.append(QPointF(5,7)); pol.append(QPointF(6,6));
    pol.append(QPointF(7,5)); pol.append(QPointF(9,7)); pol.append(QPointF(9,3));
    pol.append(QPointF(3,3)); pol.append(QPointF(3,1));
   }
   else if(constr_type==ConstraintType::foreign_key)
   {
    attrib=ParsersAttributes::FK_COLUMN;
    pol.append(QPointF(0,3)); pol.append(QPointF(0,6)); pol.append(QPointF(4,6));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0)); pol.append(QPointF(4,0));
    pol.append(QPointF(4,3));
   }
   else if(constr_type==ConstraintType::unique)
   {
    attrib=ParsersAttributes::UQ_COLUMN;
    pol.append(QPointF(4,0)); pol.append(QPointF(0,4)); pol.append(QPointF(0,5));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0));
   }

   if(factor!=1.0f)
    this->resizePolygon(pol,
                        pol.boundingRect().width() * factor,
                        pol.boundingRect().height()  * factor);

   //Atribui o polígono configurado e configura o estilo de cores do descritor
   desc->setPolygon(pol);
   desc->setBrush(this->getFillStyle(attrib));
   desc->setPen(this->getBorderStyle(attrib));
  }
 }
 //Configura um descritor poligonal para indice, regra ou gatilho
 else if(obj_type==OBJ_INDEX ||
         obj_type==OBJ_RULE ||
         obj_type==OBJ_TRIGGER)
 {
  TableObject *tab_obj=dynamic_cast<TableObject *>(this->getSourceObject());
  QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);
  QPolygonF pol;

  pol.append(QPointF(5,0)); pol.append(QPointF(0,5)); pol.append(QPointF(4,9));
  pol.append(QPointF(9,9)); pol.append(QPointF(9,4));

  if(factor!=1.0f)
   this->resizePolygon(pol,
                       pol.boundingRect().width() * factor ,
                       pol.boundingRect().height() * factor);

  desc->setPolygon(pol);
  desc->setBrush(this->getFillStyle(tab_obj->getSchemaName()));
  desc->setPen(this->getBorderStyle(tab_obj->getSchemaName()));
 }
 //Configura um descritor elíptico padrão (usado para referências de visões)
 else
 {
  QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

  desc->setRect(QRectF(QPointF(0,0),
                       QSizeF(9.0f * factor, 9.0f * factor)));
  desc->setBrush(this->getFillStyle(ParsersAttributes::REFERENCE));
  desc->setPen(this->getBorderStyle(ParsersAttributes::REFERENCE));
 }
}

void TableObjectView::configureObject(void)
{
 //Caso haja um objeto de tabela atribuído ao subitem
 if(this->getSourceObject())
 {
  QTextCharFormat fmt;
  float px;
  QString str_constr;
  TableObject *tab_obj=dynamic_cast<TableObject *>(this->getSourceObject());
  Column *column=dynamic_cast<Column *>(tab_obj);
  ConstraintType constr_type=ConstraintType::null;

  //Caso seja uma coluna
  if(column)
  {
   //Obtém a string de restrições relacionads   coluna
   str_constr=this->getConstraintString(column);

   /* Determina o tipo primário de restrição conforme as checagens a seguir.
      É com base no tipo da restrição que o descritor será criado. Adicionalmente
      obtém a formatação de fonte para o tipo da restrição */
   if(str_constr.find(TXT_PRIMARY_KEY)>=0)
   {
    fmt=font_config[ParsersAttributes::PK_COLUMN];
    constr_type=ConstraintType::primary_key;
   }
   else if(str_constr.find(TXT_FOREIGN_KEY)>=0)
   {
    fmt=font_config[ParsersAttributes::FK_COLUMN];
    constr_type=ConstraintType::foreign_key;
   }
   else if(str_constr.find(TXT_UNIQUE)>=0)
   {
    fmt=font_config[ParsersAttributes::UQ_COLUMN];
    constr_type=ConstraintType::unique;
   }
   else if(str_constr.find(TXT_NOT_NULL)>=0)
    fmt=font_config[ParsersAttributes::NN_COLUMN];
   else
    fmt=font_config[ParsersAttributes::COLUMN];

   if(column->isAddedByRelationship())
    fmt=font_config[ParsersAttributes::INH_COLUMN];
   else if(column->isProtected())
    fmt=font_config[ParsersAttributes::PROT_COLUMN];
  }
  //Caso não seja uma coluna, obtém a formatação para o tipo do objeto de tabela
  else
   fmt=font_config[tab_obj->getSchemaName()];

  //Configura o descritor com o tipo da restrição
  configureDescriptor(constr_type);

  //Posiciona o descritor como o primeiro item
  descriptor->setPos(HORIZ_SPACING, 1);
  px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HORIZ_SPACING);

  /* Configurando os rótulos do subitem.
     Os rótulos do subitem têm o seguinte esquema: [nome do objeto] [tipo] [restrições] */
  lables[0]->setText(QString::fromUtf8(tab_obj->getName()));
  lables[0]->setFont(fmt.font());
  lables[0]->setBrush(fmt.foreground());
  lables[0]->setPos(px, 0);
  px+=lables[0]->boundingRect().width();

  //Configura o rótulo de tipo
  fmt=font_config[ParsersAttributes::OBJECT_TYPE];
  if(column)
   lables[1]->setText(QString::fromUtf8(TYPE_SEPARATOR + (*column->getType())));
  else
   lables[1]->setText(QString::fromUtf8(TYPE_SEPARATOR + tab_obj->getSchemaName()));

  lables[1]->setFont(fmt.font());
  lables[1]->setBrush(fmt.foreground());
  lables[1]->setPos(px, 0);
  px+=lables[1]->boundingRect().width() + (3 * HORIZ_SPACING);

  /* Configura o rótulo de restrições. Para objetos índice, regras e gatilho
     o rótulo de restrições armazena informações sobre modo de disparo,
     eventos, entre outros */
  fmt=font_config[ParsersAttributes::CONSTRAINTS];
  if(column)
   lables[2]->setText(QString::fromUtf8(str_constr));
  else
  {
   Rule *rule=dynamic_cast<Rule *>(tab_obj);
   Trigger *trigger=dynamic_cast<Trigger *>(tab_obj);
   Index *index=dynamic_cast<Index *>(tab_obj);

   //Configurando a string de restrições para regra
   if(rule)
   {
    str_constr+=(~rule->getEventType()).mid(3,1);
    str_constr+=CONSTR_SEPARATOR;
    str_constr+=(~rule->getExecutionType()).mid(0,1);
    str_constr=str_constr.lower();
   }
   //Configurando a string de restrições para gatilho
   else if(trigger)
   {
    str_constr+=(~trigger->getFiringType()).mid(0,1);
    str_constr+=CONSTR_SEPARATOR;

    for(unsigned i=EventType::on_insert; i < EventType::on_truncate; i++)
    {
     if(trigger->isExecuteOnEvent(EventType(i)))
      str_constr+=(~EventType(i)).mid(3,1);
    }
    str_constr=str_constr.lower();
   }
   //Configurando a string de restrições para índice
   else if(index)
   {
    if(index->getIndexAttribute(Index::UNIQUE))
     str_constr+="u";

    if(index->getIndexAttribute(Index::CONCURRENT))
     str_constr+="c";

    if(index->getIndexAttribute(Index::FAST_UPDATE))
     str_constr+="f";
   }

   if(!str_constr.isEmpty())
    lables[2]->setText(QString::fromUtf8(DELIMITADOR_REST_INI + " " +
                                          str_constr + " " +
                                          DELIMITADOR_REST_FIM));

  }

  lables[2]->setFont(fmt.font());
  lables[2]->setBrush(fmt.foreground());
  lables[2]->setPos(px, 0);

  //Calcula o retângulo de dimensão do subitem, que é composto pela junção de todas as dimensões dos objetos (descritor e rótulos)
  descriptor->setPos(HORIZ_SPACING, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
  bounding_rect.setTopLeft(QPointF(descriptor->boundingRect().left(), lables[0]->boundingRect().top()));

  //Caso particular: Caso o rótulo de restrições esteja vazio usa a dimensão do rótulo de tipo
  if(lables[2]->boundingRect().width()==0)
   bounding_rect.setBottomRight(QPointF(lables[1]->boundingRect().right(), lables[0]->boundingRect().bottom()));
  else
   bounding_rect.setBottomRight(QPointF(lables[2]->boundingRect().right(), lables[0]->boundingRect().bottom()));
 }
}

void TableObjectView::configureObject(Reference reference)
{
 QTextCharFormat fmt;
 float px;
 QString str_aux;

 //Configura e posiciona o descritor da referência
 configureDescriptor();
 descriptor->setPos(HORIZ_SPACING, 1);
 px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HORIZ_SPACING);

 //Caso o tipo da referência seja a uma coluna
 if(reference.getReferenceType()==Reference::REFER_COLUMN)
 {
  //Configura o rótulo de nome no formato: [tabela].[coluna]
  fmt=font_config[ParsersAttributes::REF_TABLE];
  lables[0]->setText(reference.getTable()->getName() + ".");
  lables[0]->setFont(fmt.font());
  lables[0]->setBrush(fmt.foreground());
  lables[0]->setPos(px, 0);
  px+=lables[0]->boundingRect().width();

  fmt=font_config[ParsersAttributes::REF_COLUMN];
  if(reference.getColumn())
   lables[1]->setText(reference.getColumn()->getName());
  else
   lables[1]->setText("*");

  lables[1]->setFont(fmt.font());
  lables[1]->setBrush(fmt.foreground());
  lables[1]->setPos(px, 0);
  px+=lables[1]->boundingRect().width();
 }
 //Caso a referência seja a uma expressão
 else
 {
  fmt=font_config[ParsersAttributes::REF_TABLE];

  //Trunca a expressão em 20 caracters caso a mesma ultrapasse este comprimento
  str_aux=reference.getExpression().mid(0,20);
  if(reference.getExpression().size() > 20) str_aux+="...";

  lables[0]->setText(str_aux);
  lables[0]->setFont(fmt.font());
  lables[0]->setBrush(fmt.foreground());
  lables[1]->setText("");
  lables[0]->setPos(px, 0);
  px+=lables[0]->boundingRect().width();
 }

 //Caso a referência possua um alias configura o rótulo para exibi-lo
 if((reference.getColumn() && reference.getColumnAlias()!="") ||
    (reference.getAlias()!="" && reference.getReferenceType()==Reference::REFER_EXPRESSION))
 {
  if(reference.getReferenceType()==Reference::REFER_EXPRESSION)
   str_aux=reference.getAlias();
  else
   str_aux=reference.getColumnAlias();

  str_aux=" (" + str_aux + ") ";
  fmt=font_config[ParsersAttributes::ALIAS];
  lables[2]->setText(str_aux);
  lables[2]->setFont(fmt.font());
  lables[2]->setBrush(fmt.foreground());
  lables[2]->setPos(px, 0);
 }

 //Configura o retângulo de dimensão do subitem
 descriptor->setPos(HORIZ_SPACING, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
 bounding_rect.setTopLeft(QPointF(descriptor->pos().x(), lables[0]->pos().y()));

 if(lables[2]->text().isEmpty())
  bounding_rect.setBottomRight(QPointF(lables[1]->boundingRect().right(), lables[0]->boundingRect().bottom()));
 else
  bounding_rect.setBottomRight(QPointF(lables[2]->boundingRect().right(), lables[0]->boundingRect().bottom()));
}

void TableObjectView::setChildObjectXPos(unsigned obj_idx, float px)
{
 if(obj_idx >= 4)
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(obj_idx==0)
  descriptor->setPos(px, descriptor->pos().y());
 else
  lables[obj_idx-1]->setPos(px, lables[obj_idx-1]->pos().y());
}

QGraphicsItem *TableObjectView::getChildObject(unsigned obj_idx)
{
 if(obj_idx >= 4)
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(obj_idx==0)
  return(descriptor);
 else
  return(lables[obj_idx-1]);
}

QString TableObjectView::getConstraintString(Column *column)
{
 if(column && column->getParentTable())
 {
  Table *table=dynamic_cast<Table *>(column->getParentTable());
  QString str_constr;
  Constraint *constr=NULL;
  vector<Constraint *>::iterator itr,itr_end;
  vector<Constraint *> constraints;
  ConstraintType constr_type;
  unsigned i, count;

  count=table->getConstraintCount();
  for(i=0; i < count; i++)
   constraints.push_back(table->getConstraint(i));

  //Obtém as referências ao primeiro e último elemento da lita de constraints
  itr=constraints.begin();
  itr_end=constraints.end();

  while(itr!=itr_end)
  {
   constr=(*itr); //Obtém uma constraint
   itr++; //Passa para a próxima da lista

   //Verfica se a coluna está sendo referenciada dentro da constraint
   if(constr->getColumn(column->getName(), Constraint::SOURCE_COLS))
   {
    //Obtém o tipo da constraint
    constr_type=constr->getConstraintType();

    //Para cada tipo de constraint concatena-se o texto referente
    //   a mesma e um separadar de textos de constraints
    if(constr_type==ConstraintType::primary_key)
     str_constr=TXT_PRIMARY_KEY + CONSTR_SEPARATOR;
    else if(constr_type==ConstraintType::foreign_key)
     str_constr+=TXT_FOREIGN_KEY + CONSTR_SEPARATOR;
    else if(constr_type==ConstraintType::unique)
     str_constr+=TXT_UNIQUE + CONSTR_SEPARATOR;
   }
  }

  //Caso a coluna seja não-nula adiciona a QString formatada o texto referente   constraint NOT NULL
  if(column->isNotNull()) str_constr+=TXT_NOT_NULL + CONSTR_SEPARATOR;

  //Caso a QString formatada tenha sido criada
  if(str_constr!="")
   //Termina sua formatação concatenando a QString formatada aos caracteres delimitadores de restricoes
   str_constr= DELIMITADOR_REST_INI +
              CONSTR_SEPARATOR + str_constr +
              DELIMITADOR_REST_FIM;

  return(str_constr);
 }
 else return("");
}

