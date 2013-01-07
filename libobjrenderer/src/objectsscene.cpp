#include "objectsscene.h"

bool ObjectsScene::align_objs_grid=false;
bool ObjectsScene::show_grid=true;
bool ObjectsScene::show_page_delim=true;
unsigned ObjectsScene::grid_size=20;
QPrinter::PageSize ObjectsScene::paper_size=QPrinter::A4;
QPrinter::Orientation ObjectsScene::page_orientation=QPrinter::Landscape;
QRectF ObjectsScene::page_margins=QRectF(10,10,10,10);
QBrush ObjectsScene::grid;

ObjectsScene::ObjectsScene(void)
{
 moving_objs=false;
 this->setBackgroundBrush(grid);

 sel_ini_pnt.setX(NAN);
 sel_ini_pnt.setY(NAN);

 /* Configura o retângulo de seleção de objetos. Este retângulo fica
    acima de todos so objetos (zvalue=100) */
 selection_rect=new QGraphicsPolygonItem;
 selection_rect->setVisible(false);
 selection_rect->setZValue(100);

 rel_line=new QGraphicsLineItem;
 rel_line->setVisible(false);
 rel_line->setZValue(-1);
 rel_line->setPen(QColor(80,80,80));

 //Adiciona     cena o retângulo de seleção
 this->addItem(selection_rect);
 this->addItem(rel_line);
}

ObjectsScene::~ObjectsScene(void)
{
 QGraphicsItemGroup *item=NULL;
 QList<QGraphicsItem *> items;
 ObjectType obj_types[]={ OBJ_RELATIONSHIP, OBJ_TEXTBOX,
                          OBJ_VIEW, OBJ_TABLE };
 unsigned i;

 this->removeItem(selection_rect);
 this->removeItem(rel_line);

 //Remove os objetos em ordem conforme o vetor tipos[]
 for(i=0; i < 4; i++)
 {
  items=this->items();

  while(!items.isEmpty())
  {
   //Obtém o item e já tentando convertê-lo para QGraphicsItemGroup
   item=dynamic_cast<QGraphicsItemGroup *>(items.front());
      /* Caso o objeto seja um grupo de itens e possa ser convertido para uma das
      classes OGRelacionamento, OGTabela, OGCaixaTexto ou OGVisao, significa que
      o item pode ser removido */
   if(item && !item->parentItem() &&
      ((dynamic_cast<RelationshipView *>(item) && obj_types[i]==OBJ_RELATIONSHIP) ||
       (dynamic_cast<TextboxView *>(item) && obj_types[i]==OBJ_TEXTBOX) ||
       (dynamic_cast<GraphicalView *>(item) && obj_types[i]==OBJ_VIEW) ||
       (dynamic_cast<TableView *>(item) && obj_types[i]==OBJ_TABLE)))

   {
    this->removeItem(item);
   }

   items.pop_front();
  }
 }
}

QPointF ObjectsScene::alignPointToGrid(const QPointF &pnt)
{
 QPointF p(roundf(pnt.x()/grid_size) * grid_size,
           roundf(pnt.y()/grid_size) * grid_size);

 if(p.x() < 0) p.setX(0);
 if(p.y() < 0) p.setY(0);

 return(p);
}

void ObjectsScene::setSceneRect(const QRectF &rect)
{
 QGraphicsScene::setSceneRect(0, 0, rect.width(), rect.height());
}

void ObjectsScene::setGridSize(unsigned size)
{
 if(size >= 20 || grid.style()==Qt::NoBrush)
 {
  QImage grid_img;
  float width, height, x, y;
  QSizeF aux_size;
  QPrinter printer;
  QPainter painter;
  QPen pen;

  //Caso o tamanho do papel não seja personalizado
  if(paper_size!=QPrinter::Custom)
  {
   //Configura um dispositivo QPrinter para obter os tamanhos de página
   printer.setPageSize(paper_size);
   printer.setOrientation(page_orientation);
   printer.setPageMargins(page_margins.left(), page_margins.top(),
                          page_margins.right(), page_margins.bottom(), QPrinter::Millimeter);
   aux_size=printer.pageRect(QPrinter::DevicePixel).size();
  }
  //Caso o tipo de papel seja personalizado, usa as margens como tamanho do papel
  else
   aux_size=page_margins.size();


  width=fabs(roundf(aux_size.width()/static_cast<float>(size)) * size);
  height=fabs(roundf(aux_size.height()/static_cast<float>(size)) * size);

  //Cria uma instância de QImage para ser a textura do brush
  grid_size=size;
  grid_img=QImage(width, height, QImage::Format_ARGB32);

  //Aloca um QPaointer para executar os desenhos sobre a imagem
  painter.begin(&grid_img);

  //Limpa a imagem
  painter.fillRect(QRect(0,0,width,height), QColor(255,255,255));

  if(show_grid)
  {
   //Cria a grade
   pen.setColor(QColor(225, 225, 225));
   painter.setPen(pen);

   for(x=0; x < width; x+=size)
    for(y=0; y < height; y+=size)
     painter.drawRect(QRectF(QPointF(x,y),QPointF(x + size,y + size)));
  }

  //Cria as linhas que definem o limite do papel
  if(show_page_delim)
  {
   pen.setColor(QColor(75,115,195));
   pen.setStyle(Qt::DashLine);
   pen.setWidthF(1.85f);
   painter.setPen(pen);
   painter.drawLine(width-1, 0,width-1,height-1);
   painter.drawLine(0, height-1,width-1,height-1);
  }

  painter.end();
  grid.setTextureImage(grid_img);
 }
}

void ObjectsScene::showRelationshipLine(bool value, const QPointF &p)
{
 QList<QGraphicsItem *> items=this->items();
 QGraphicsItem::GraphicsItemFlags flags;
 BaseObjectView *object=NULL;
 BaseGraphicObject *base_obj=NULL;

 if(!isnan(p.x()) && !isnan(p.y()))
  rel_line->setLine(QLineF(p,p));

 rel_line->setVisible(value);

 //Configura as flags dos objetos na cena
 while(!items.isEmpty())
 {
  /* Caso a linha for exibida configura a flag dos objetos
     como sendo não movíveis */
  flags=QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges;

  object=dynamic_cast<BaseObjectView *>(items.front());

  if(object && object->getSourceObject())
  {
   base_obj=dynamic_cast<BaseGraphicObject *>(object->getSourceObject());

   /* Caso o objeto gráfico seja uma tabela, visão ou caixa texto, ativa
      a flag de movimento caso o mesmo não esteja protegido */
   if(!value && base_obj &&
      base_obj->getObjectType()!=OBJ_RELATIONSHIP &&
      base_obj->getObjectType()!=BASE_RELATIONSHIP &&
      !base_obj->isProtected())
    flags=QGraphicsItem::ItemIsMovable |
          QGraphicsItem::ItemIsSelectable |
          QGraphicsItem::ItemSendsGeometryChanges;
   else
    /* Caso a linha for exibida configura a flag dos objetos
       como sendo não movíveis */
    flags=QGraphicsItem::ItemIsSelectable |
          QGraphicsItem::ItemSendsGeometryChanges;
  }

  items.front()->setFlags(flags);
  items.pop_front();
 }
}

void ObjectsScene::setGridOptions(bool show_grd, bool align_objs_grd, bool show_pag_dlm)
{
 bool redef_grade=(ObjectsScene::show_grid!=show_grd ||
                   ObjectsScene::show_page_delim!=show_pag_dlm ||
                   grid.style()==Qt::NoBrush);

 ObjectsScene::show_grid=show_grd;
 ObjectsScene::show_page_delim=show_pag_dlm;
 ObjectsScene::align_objs_grid=align_objs_grd;

 //Redefine a grade se necessário
 if(redef_grade)
 {
  grid.setStyle(Qt::NoBrush);
  setGridSize(ObjectsScene::grid_size);
 }
}

void ObjectsScene::getGridOptions(bool &show_grd, bool &align_objs_grd, bool &show_pag_dlm)
{
 show_grd=ObjectsScene::show_grid;
 align_objs_grd=ObjectsScene::align_objs_grid;
 show_pag_dlm=ObjectsScene::show_page_delim;
}

void ObjectsScene::setPageConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins)
{
 ObjectsScene::paper_size=paper_sz;
 ObjectsScene::page_orientation=orient;
 ObjectsScene::page_margins=margins;
}

void ObjectsScene::getPageConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins)
{
 paper_sz=ObjectsScene::paper_size;
 orient=ObjectsScene::page_orientation;
 margins=ObjectsScene::page_margins;
}

void ObjectsScene::emitObjectModification(BaseGraphicObject *object)
{
 emit s_objectModified(object);
}

void ObjectsScene::emitChildObjectSelection(TableObject *child_obj)
{
 /* Trata o sinal de OGTabela::objetoFilhoSelecionado somente quando não
    houver outros objetos selecionados na cena */
 if(this->selectedItems().isEmpty())
 {
  vector<BaseObject *> vet;

  //Insere um vetor o objeto filho da tabela selecionado
  vet.push_back(child_obj);
  //Encaminha o objeto através do sinal
  emit s_popupMenuRequested(vet);
 }
}

void ObjectsScene::emitObjectSelection(BaseGraphicObject *object, bool selected)
{
 if(object)
  emit s_objectSelected(object, selected);
}

void ObjectsScene::addItem(QGraphicsItem *item)
{
 if(item)
 {
  RelationshipView *rel=dynamic_cast<RelationshipView *>(item);
  TableView *tab=dynamic_cast<TableView *>(item);
  BaseObjectView *obj=dynamic_cast<BaseObjectView *>(item);

  /* Caso particular para classes OGRelacionamento e OGTabela:
     conecta os sinais quando novos objetos dos tipos acima são
     inseridos no modelo */
  if(rel)
   connect(rel, SIGNAL(s_relationshipModified(BaseGraphicObject*)),
           this, SLOT(emitObjectModification(BaseGraphicObject*)));
  else if(tab)
   connect(tab, SIGNAL(s_childObjectSelected(TableObject*)),
           this, SLOT(emitChildObjectSelection(TableObject*)));

  if(obj)
   connect(obj, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)),
           this, SLOT(emitObjectSelection(BaseGraphicObject*,bool)));

  QGraphicsScene::addItem(item);
 }
}

void ObjectsScene::removeItem(QGraphicsItem *item)
{
 if(item)
 {
  BaseObjectView *object=dynamic_cast<BaseObjectView *>(item);
  RelationshipView *rel=dynamic_cast<RelationshipView *>(item);
  TableView *tab=dynamic_cast<TableView *>(item);

  /* Caso particular para classes OGRelacionamento e OGTabela:
     desconecta os sinais anteriormente conectados    cena e que
     são disparados por tabela ou relacionamento */
  if(rel)
  {
   disconnect(rel, NULL, this, NULL);
   rel->disconnectTables();
  }
  else if(tab)
   disconnect(tab, NULL, this, NULL);
  else if(object)
   disconnect(object, NULL, this, NULL);

  /* Como a classe QGraphicsScene não delete o item apenas o retira da cena,
     força a destruição do objeto */
  item->setVisible(false);
  item->setActive(false);

  //O item removido não é desalocado na chamada do método e sim quando a cena é destruída.
  QGraphicsScene::removeItem(item);
 }
}

void ObjectsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
 QGraphicsScene::mouseDoubleClickEvent(event);

 if(this->selectedItems().size()==1 && event->buttons()==Qt::LeftButton)
 {
  //Obtém o objeto gráfico selecionado
  BaseObjectView *obj=dynamic_cast<BaseObjectView *>(this->selectedItems().at(0));

  //Caso seja mesmo um objeto, emite o sinal com o objeto de origem
  if(obj)
   emit s_objectDoubleClicked(dynamic_cast<BaseGraphicObject *>(obj->getSourceObject()));
 }
}

void ObjectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
 if(event->buttons()==Qt::LeftButton ||
    (event->buttons()==Qt::RightButton && this->selectedItems().isEmpty()))
 {
  //Obtém o item sob a posição do mouse
  QGraphicsItem* item=this->itemAt(event->scenePos().x(), event->scenePos().y());

  /* Caso algum item foi obtido acima e a linha de relacionamento esteja visível
     permite a multiseleção sem pressionar o control, para isso, o objeto é
     marcado como selecionado. Isso evita que quando um usuário esteja criando um
     relacionamento entre tabelas precise pressionar control para escolher 2 tabelas */
  if(item && item->isEnabled() &&
     rel_line->isVisible())
   item->setSelected(!item->isSelected());

  QGraphicsScene::mousePressEvent(event);
 }

 /* Caso não hajam itens selecionados, exibe o retângulo de seleção de objetos.
    Este permanecerá visível até que o usuário solte o botão esquedo */
 if(this->selectedItems().isEmpty() && event->buttons()==Qt::LeftButton)
 {
  sel_ini_pnt=event->scenePos();
  selection_rect->setVisible(true);

  //Emite um sinal indicando que nenhum objeto está selecionado
  emit s_objectSelected(NULL,false);
 }
}

void ObjectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
 if(event->buttons()==Qt::LeftButton)
 {
  if(!rel_line->isVisible())
  {
   //Caso o usuário inicie o movimento de objetos
   if(!this->selectedItems().isEmpty() && !moving_objs && event->modifiers()==Qt::NoModifier)
   {
    //Dispara um sinal indicando o evento
    emit s_objectsMoved(false);
    //Marca o flag indicando que o usário está movimentando objetos
    moving_objs=true;
   }

   /*Caso o alinhamento esteja ativo e haja objetos selecionados efetua o alinhamento
     do ponto (posição do evento �   grade */
   if(align_objs_grid && !selection_rect->isVisible())
    event->setScenePos(this->alignPointToGrid(event->scenePos()));
   else if(selection_rect->isVisible())
   {
    //Atualiza a posição do retângulo de seleção
    QPolygonF pol;
    pol.append(sel_ini_pnt);
    pol.append(QPointF(event->scenePos().x(), sel_ini_pnt.y()));
    pol.append(QPointF(event->scenePos().x(), event->scenePos().y()));
    pol.append(QPointF(sel_ini_pnt.x(), event->scenePos().y()));
    selection_rect->setPolygon(pol);
    selection_rect->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));
    selection_rect->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
   }
  }
 }

 if(rel_line->isVisible())
  rel_line->setLine(QLineF(rel_line->line().p1(), event->scenePos()));

 QGraphicsScene::mouseMoveEvent(event);
}

void ObjectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
 QGraphicsScene::mouseReleaseEvent(event);

 /* Caso haja objetos selecionados e o botão esquerdo do mouse for liberado
    finaliza o movimento de objetos, alinhando-os    grade se necessário */
 if(!this->selectedItems().isEmpty() && moving_objs &&
    event->button()==Qt::LeftButton && event->modifiers()==Qt::NoModifier)
 {
  unsigned i, count;
  QList<QGraphicsItem *> items=this->selectedItems();
  float x1,y1,x2,y2;
  QRectF rect;
  RelationshipView *rel=NULL;

  /* Obtém os pontos extremos da cena para verificar se algum objeto
     ultrapassa estes limites. Caso isso aconteça, reconfigura o tamanho da
     cena para comportar a nova posição dos objetos */
  x1=this->sceneRect().left();
  y1=this->sceneRect().top();
  x2=this->sceneRect().right();
  y2=this->sceneRect().bottom();

  //Varre a lista de objetos selecionados
  count=items.size();
  for(i=0; i < count; i++)
  {
   /* A partir do objeto atual da lista tenta convertê-lo em relacionamento, pois
      este tipo de objeto deve ser tratato de forma diferente */
   rel=dynamic_cast<RelationshipView *>(items[i]);

   //Caso o objeto não seja um relacionamento
   if(!rel)
   {
    if(align_objs_grid)
     //Move o objeto para um ponto ajustado    grade
     items[i]->setPos(alignPointToGrid(items[i]->pos()));
    else
    {
     //Caso o alinhamento    grade não esteja disponível, apneas ajusta o ponto se o mesmo for negativo
     QPointF p=items[i]->pos();
     if(p.x() < 0) p.setX(0);
     if(p.y() < 0) p.setY(0);
     items[i]->setPos(p);
    }

    //Obtém o retângulo de dimensão do objeto para comparações com a dimensão da cena
    rect.setTopLeft(items[i]->pos());
    rect.setSize(items[i]->boundingRect().size());
   }
   else
   {
    //Obtém o retângulo de dimensão do relacionamento
    rect=rel->__boundingRect();
   }

   /* Efetua as comparações entre as extremidades da cena e do retângulo,
      é nesta comparação que se calcula a nova dimensão da cena */
   if(rect.left() < x1) x1=rect.left();
   if(rect.top() < y1) y1=rect.top();
   if(rect.right() > x2) x2=rect.right();
   if(rect.bottom() > y2) y2=rect.bottom();
  }

  //Configura o retângulo com as dimensões obtidas
  rect.setCoords(x1, y1, x2, y2);

  /* Caso este retângulo seja diferente do retângulo da cena a nova dimensão passará a ser
     o boundingRect dos itens parindo da origem e acrescido em 5% */
  if(rect!=this->sceneRect())
  {
   rect=this->itemsBoundingRect();
   rect.setTopLeft(QPointF(0,0));
   rect.setWidth(rect.width() * 1.05f);
   rect.setHeight(rect.height() * 1.05f);
   this->setSceneRect(rect);
  }

  //Emite um sinal indicando que os objetos finalizaram o movimento
  emit s_objectsMoved(true);
  moving_objs=false;
 }
 //Caso o retângulo de seleção esteja visível e o botão esquerdo foi liberado
 else if(selection_rect->isVisible() && event->button()==Qt::LeftButton)
 {
  QPolygonF pol;
  QPainterPath sel_area;

  /* Configura uma área de seleção de objetos e com base nesta área é que
     serão selecionados os objetos que colidem com a primeira */
  sel_area.addRect(selection_rect->polygon().boundingRect());
  this->setSelectionArea(sel_area, Qt::IntersectsItemShape);

  //Esconde o retângulo de seleção
  selection_rect->setVisible(false);
  selection_rect->setPolygon(pol);
  sel_ini_pnt.setX(NAN);
  sel_ini_pnt.setY(NAN);
 }
}

void ObjectsScene::alignObjectsToGrid(void)
{
 QList<QGraphicsItem *> items=this->items();
 RelationshipView *rel=NULL;
 BaseTableView *tab=NULL;
 TextboxView *lab=NULL;
 vector<QPointF> points;
 unsigned i, count, i1, count1;

 count=items.size();
 for(i=0; i < count; i++)
 {
  /* Obtém somente os objetos que são convertido �   classe QGraphicsItemGroup e
     que não tenham objeto pai. Isso é feito pois o método items() retorna TODOS
     os itens desconsiderando se eles pertencem ou não a grupos, e isso no contexto
     dos objetos do modelo é errado pois todos os objetos do grupo precisam ser alinhados
     conforme seu grupo */
  if(dynamic_cast<QGraphicsItemGroup *>(items[i]) && !items[i]->parentItem())
  {
   //Converte o item atual para tabela
   tab=dynamic_cast<BaseTableView *>(items[i]);
   //Converte o item atual para relacionamento
   rel=dynamic_cast<RelationshipView *>(items[i]);

   //Caso o item foi convertido para tabela
   if(tab)
    //Move o objeto usando o método setPos da classe OGTabelaBase com o ponto alinhado�   grade
    tab->setPos(this->alignPointToGrid(tab->pos()));
   /* Caso o item foi convertido para relacionamento, efetua um tratamento diferenciado,
      movendo pontos e rótulos individualmente */
   else if(rel)
   {
    //Obtém os pontos do relacionamento, alinha-os e os reatribui ao relacionamento
    points=rel->getSourceObject()->getPoints();
    count1=points.size();
    for(i1=0; i1 < count1; i1++)
     points[i1]=this->alignPointToGrid(points[i1]);

    if(count1 > 0)
    {
     rel->getSourceObject()->setPoints(points);
     //Reconfigura a linha após o alinhamento dos pontos
     rel->configureLine();
    }

    //Alinha os rótulos�   grade
    for(i1=BaseRelationship::LABEL_SRC_CARD;
        i1<=BaseRelationship::LABEL_REL_NAME; i1++)
    {
     lab=rel->getLabel(i1);
     if(lab)
      lab->setPos(this->alignPointToGrid(lab->pos()));
    }
   }
   //Para os demais objetos do modelo usa o método padrão setPos
   else
    items[i]->setPos(this->alignPointToGrid(items[i]->pos()));
  }
 }
}

void ObjectsScene::update(void)
{
 this->setBackgroundBrush(grid);
 QGraphicsScene::update(this->sceneRect());
}

