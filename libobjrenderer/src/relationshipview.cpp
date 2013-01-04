#include "relationshipview.h"

RelationshipView::RelationshipView(BaseRelationship *rel) : BaseObjectView(rel)
{
 //Dispara uma exceção caso tente criar um relacionamento a partir de um objeto não alocado
 if(!rel)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 for(unsigned i=BaseRelationship::LABEL_SRC_CARD;
      i <= BaseRelationship::LABEL_REL_NAME; i++)
 {
  //Aloca os rótulos
  if(rel->getLabel(i))
  {
   rel->getLabel(i)->setTextColor(BaseObjectView::getFontStyle(ParsersAttributes::LABEL).foreground());
   labels[i]=new TextboxView(rel->getLabel(i),
                               BaseObjectView::getFillStyle(ParsersAttributes::LABEL),
                               BaseObjectView::getBorderStyle(ParsersAttributes::LABEL));
   labels[i]->setZValue(1);
   this->addToGroup(labels[i]);
  }
  else
   labels[i]=NULL;
 }

 sel_object=NULL;
 sel_object_idx=-1;
 configuring_line=false;

 descriptor=new QGraphicsPolygonItem;
 descriptor->setZValue(0);
 this->addToGroup(descriptor);

 tables[0]=tables[1]=NULL;

 /* Relacionamento tem o menor Z para ficar abaixo de todos os
    demais objetos do modelo */
 this->setZValue(-1);
 this->configureObject();
}

RelationshipView::~RelationshipView(void)
{
 QGraphicsItem *item=NULL;

 //Exclui todos os objetos criados e agregados ao relacionamento
 for(int i=0; i < 3; i++)
 {
  if(labels[i])
  {
   this->removeFromGroup(labels[i]);
   delete(labels[i]);
  }
 }

 while(!lines.empty())
 {
  item=lines.back();
  this->removeFromGroup(item);
  lines.pop_back();
  delete(item);
 }

 while(!attributes.empty())
 {
  item=attributes.back();
  this->removeFromGroup(item);
  attributes.pop_back();
  delete(item);
 }

 this->removeFromGroup(descriptor);
 delete(descriptor);
}

BaseRelationship *RelationshipView::getSourceObject(void)
{
 return(dynamic_cast<BaseRelationship *>(this->BaseObjectView::getSourceObject()));
}

TextboxView *RelationshipView::getLabel(unsigned lab_idx)
{
 if(lab_idx > BaseRelationship::LABEL_REL_NAME)
  return(NULL);
 else
  return(labels[lab_idx]);
}

QVariant RelationshipView::itemChange(GraphicsItemChange change, const QVariant &value)
{
 //Caso seja uma mudança na posição
 if(change==ItemPositionChange)
 {
  //Força o relacionamento a não ser movimentável
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
 }
 //Caso a mundança no objeto foi a de seleção ou deseleção
 else if(change==ItemSelectedHasChanged)
 {
  unsigned i, count;
  QPen pen;
  QColor color;

  if(value.toBool())
   this->sel_order=++BaseObjectView::global_sel_order;

  emit s_objectSelected(dynamic_cast<BaseGraphicObject *>(this->getSourceObject()), value.toBool());

  pos_info_pol->setVisible(value.toBool());
  pos_info_txt->setVisible(value.toBool());
  obj_selection->setVisible(value.toBool());
  this->configurePositionInfo();

  //Seleciona ou não os rótulos conforme o parâmetro 'value'
  for(i=0; i < 3; i++)
  {
   if(labels[i])
    labels[i]->itemChange(change, value);
  }

  //Deixa visível os pontos do relacionamento caso 'value' seja true
  count=graph_points.size();
  for(i=0; i < count; i++)
   graph_points[i]->setVisible(value.toBool());

  /* Caso o relacionamento está selecionado (value = true)
     as linhas do relacionamento tem sua cor alterada para denotar
     que as mesmas também estão selecionadas */
  if(value.toBool())
  {
   QColor cor1=BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION).color(),
          cor2=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP).color();

   color.setRedF((cor1.redF() + cor2.greenF())/2.0f);
   color.setGreenF((cor1.greenF() + cor2.greenF())/2.0f);
   color.setBlueF((cor1.blueF() + cor2.blueF())/2.0f);
  }
  else
   color=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP).color();

  //Aplica a cor nas linhas do relacionamento
  count=lines.size();
  for(i=0; i < count; i++)
  {
   pen=lines[i]->pen();
   pen.setColor(color);
   lines[i]->setPen(pen);
  }

  //Exibe/esconde a seleção dos atributos
  count=attributes.size();
  for(i=0; i < count; i++)
   attributes[i]->children().at(3)->setVisible(value.toBool());
 }

 return(value);
}

void RelationshipView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
 /* O relacionamento em sim não pode ser movimentado pelo usuário, porém pode ser selecionado.
    Mas para que isso aconteça é necessário marcar a flag QGraphicsItem::ItemIsMovable,
    selecionar o relacionamento e depois desmarcar a flag. */
 this->setFlag(QGraphicsItem::ItemIsMovable);

 //Executa o mousePressEvent da classe base para selecionar o relacionamento
 BaseObjectView::mousePressEvent(event);

 //Desmarca a flag QGraphicsItem::ItemIsMovable para que o relacionamento não seja movido pelo usuário
 this->setFlag(QGraphicsItem::ItemIsMovable, false);

 //Caso o relacionamento não esteja protegido
 if(!this->getSourceObject()->isProtected())
 {
  BaseRelationship *base_rel=this->getSourceObject();

  //Reseta a posição dos rótulos caso o botão do meio esteja pressionado
  if(event->buttons()==Qt::MidButton)
  {
   for(unsigned i=0; i < 3; i++)
    base_rel->setLabelDistance(i, QPointF(NAN,NAN));
   this->configureLabels();
  }
  //Caso o usuário esteja com o SHIFT pressionado
  else if(event->modifiers()==Qt::ShiftModifier)
  {
   QLineF lin;
   QPointF p;
   QRectF rect;
   unsigned i, count;
   bool pnt_rem=false;
   vector<QPointF> pontos=base_rel->getPoints();

   if(!base_rel->isSelfRelationship())
   {
    /* Clicando com botão esquerdo sobre um ponto na linha de um relacionamento
       o mesmo será removido da linha */
    if(event->buttons()==Qt::LeftButton)
    {
     emit s_relationshipModified(base_rel);

     count=graph_points.size();
     for(i=0; i < count; i++)
     {
      //Obtém a dimensão do ponto gráfico
      rect.setTopLeft(graph_points[i]->pos());
      rect.setSize(graph_points[i]->boundingRect().size());

      //Caso a posição do mouse esteja dentro do ponto efetua a remoção
      if(rect.contains(event->pos()))
      {
       pontos.erase(pontos.begin()+i);
       base_rel->setPoints(pontos);
       this->configureLine();
       pnt_rem=true;
       break;
      }
     }

     /* Clicando com botão esquerdo sobre a linha de um relacionamento
        um novo ponto será criado na linha */
     count=lines.size();
     for(i=0; i < count && !pnt_rem; i++)
     {
      /* Cria uma linha com base na posição do cursor. Esta
         linha será usada para calcular o ponto exato (intersecção)
         onde o novo ponto deve ser inserido na linha */
      if(lines[i]->line().angle()>=179 || lines[i]->line().angle()>=359)
      {
       lin.setP1(QPointF(event->pos().x(), event->pos().y()-50));
       lin.setP2(QPointF(event->pos().x(), event->pos().y()+50));
      }
      else
      {
       lin.setP1(QPointF(event->pos().x()-50, event->pos().y()));
       lin.setP2(QPointF(event->pos().x()+50, event->pos().y()));
      }

      //Caso a linha criada acima intercepta uma linha do relacionamento
      if(lines[i]->line().intersect(lin,&p)==QLineF::BoundedIntersection)
      {
       //Insere o ponto na linha
       if(i >= pontos.size())
        pontos.push_back( event->pos());
       else
        pontos.insert(pontos.begin() + i, event->pos());
       base_rel->setPoints(pontos);

       //Reconfigura a linha
       this->configureLine();
       break;
      }
     }
    }
   }
  }
  /* Caso o usuário clique com o esquerdo sem nenhum modificado do teclado pressionado.
     Será verificado se algum objeto filho pode ser selecionado na posição do cursor */
  else if(event->button()==Qt::LeftButton)
  {
   QRectF rect;
   unsigned count, i;

   //Verifica se algum ponto está selecionado sob o cursor
   count=graph_points.size();
   for(i=0; i < count && !sel_object; i++)
   {
    rect.setTopLeft(graph_points[i]->pos());
    rect.setSize(graph_points[i]->boundingRect().size());

    /* Caso o retangulo do objeto contenha o ponto,
       seleciona o mesmo */
    if(rect.contains(event->pos()))
    {
     sel_object=graph_points[i];
     sel_object_idx=i;
    }
   }

   //Verifica se algum rótulo está selecionado sob o cursor
   for(i=0; i < 3 && !sel_object; i++)
   {
    if(labels[i])
    {
     rect.setTopLeft(labels[i]->pos());
     rect.setSize(labels[i]->boundingRect().size());

     /* Caso o retangulo do objeto contenha o ponto,
        seleciona o mesmo */
     if(rect.contains(event->pos()))
     {
      sel_object=labels[i];
      sel_object_idx=i;
     }
    }
   }
  }
 }
}

void RelationshipView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
 //Caso o relacionamento esteja selecionado e não esteja protegido
 if(this->isSelected() && event->buttons()==Qt::LeftButton &&
    !this->getSourceObject()->isProtected())
 {
  //Move o objeto filho selecionado (ponto ou rótulo)
  if(dynamic_cast<QGraphicsPolygonItem *>(sel_object))
  {
   BaseRelationship *rel_base=this->getSourceObject();
   vector<QPointF> pontos=rel_base->getPoints();

   pontos[sel_object_idx]=event->pos();
   rel_base->setPoints(pontos);
   this->configureLine();
  }
  else if(dynamic_cast<TextboxView *>(sel_object))
   sel_object->setPos(event->pos());
 }

 BaseObjectView::mouseMoveEvent(event);
}

void RelationshipView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
 BaseRelationship *base_rel=this->getSourceObject();

 if(event->button()==Qt::LeftButton)
 {
  /* Caso o usuário libere o botão esquerdo e objeto anteriormente
     selecionado seja um rótulo */
  if(dynamic_cast<TextboxView *>(sel_object))
  {
   //Calcula o deslocamento do rótulo de sua posição inicial até a posição atual
   base_rel->setLabelDistance(sel_object_idx,
                                    QPointF(sel_object->pos() - labels_ini_pos[sel_object_idx]));
  }

  sel_object_idx=-1;
  sel_object=NULL;
 }

 BaseObjectView::mouseReleaseEvent(event);
}

void RelationshipView::disconnectTables(void)
{
 for(unsigned i=0; i < 2; i++)
  disconnect(tables[i], NULL, this, NULL);
}

void RelationshipView::configureObject(void)
{
 BaseRelationship *rel_base=this->getSourceObject();

 //Armazena as tabelas envolvidas no relacionamento
 tables[0]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::SRC_TABLE)->getReceiverObject());
 tables[1]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::DST_TABLE)->getReceiverObject());

 //Executa a configuração inicial do relacionamento
 this->configureLine();

 /* Conecta os slots do relacionamento com o sinal de movimento das tabelas para
    que a linha / rotulos da relacionamento sejam reposicionados sempre quando
    uma das tabelas for movida */
 for(unsigned i=0; i < 2; i++)
  connect(tables[i], SIGNAL(s_objectMoved(void)), this, SLOT(configureLine(void)));

 connect(rel_base, SIGNAL(s_objectModified()), this, SLOT(configureLine(void)));
}

void RelationshipView::configurePositionInfo(void)
{
 if(this->isSelected())
 {
  BaseObjectView::configurePositionInfo(descriptor->pos());
  pos_info_txt->setPos(descriptor->pos().x(),
                       descriptor->pos().y() - pos_info_txt->boundingRect().height());
  pos_info_pol->setPos(descriptor->pos().x(),
                       descriptor->pos().y() - pos_info_pol->boundingRect().height());
 }
}

void RelationshipView::configureLine(void)
{
 if(!configuring_line)
 {
  BaseRelationship *base_rel=this->getSourceObject();
  Relationship *rel=dynamic_cast<Relationship *>(base_rel);
  vector<QPointF> points;
  QLineF lin_aux[2];
  QGraphicsLineItem *lin=NULL;
  QPointF pos, p_int, p_central[2];
  QRectF rect;
  QGraphicsItem *item=NULL;
  QPen pen;
  QGraphicsPolygonItem *pol=NULL;
  QPolygonF pol_aux;
  int i, i1, count, idx_lin_desc=0;

  configuring_line=true;

  //Caso seja um auto relacionamento
  if(base_rel->isSelfRelationship())
  {
   float fator=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

   /* Cria uma linha fixa no seguinte formato:

            +----<>
            |     |
     ----------   |
     | Tabela |---+
     ----------

     Esta linha não pode ser movida nem ter pontos adicionados */
   pos=tables[0]->pos();
   rect=tables[0]->boundingRect();

   p_central[0].setX(pos.x() + rect.width());
   p_central[0].setY(pos.y() + (rect.height()/2.5f));

   p_central[1].setX(pos.x() + (rect.width()/1.5f));
   p_central[1].setY(pos.y());

   points.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[0].y()));
   points.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[1].y() - (10 * fator)));
   points.push_back(QPointF(p_central[1].x(),  p_central[1].y() - (10 * fator)));
   base_rel->setPoints(points);
  }
  //Caso não seja um autorelacionamento
  else
  {
   Relationship *rel=dynamic_cast<Relationship *>(base_rel);

   /* Caso especial: Relacionamento 1-1 identificadores.

      Para este tipo de relacionamento, em especial, é necessário reatribuir
      as tabelas nos índices corretamente para que a linha que designar relacionamento
      identificador seja desenhado no lado correto da entidade fraca (tabela receptora)
   */
   if(rel &&
      rel->getRelationshipType()==Relationship::RELATIONSHIP_11 &&
      rel->isIdentifier())
   {
    tables[0]=dynamic_cast<BaseTableView *>(rel->getReferenceTable()->getReceiverObject());
    tables[1]=dynamic_cast<BaseTableView *>(rel->getReceiverTable()->getReceiverObject());
   }

   /* Cria linhas horizontais e verticais denotando as bordas das tabela.
      É com base nestas bordas que serão calculados os pontos de interseção
      da linha do relacionamento com a borda da tabela. Este ponto de interseção
      é onde será conectada a linha do relacionamento na tabela */
   for(i=0; i < 2; i++)
   {
    rect=tables[i]->boundingRect();
    pos=tables[i]->pos();

    //Calcula o ponto central da tabela
    p_central[i].setX(pos.x() + (rect.width()/2.0f));
    p_central[i].setY(pos.y() + (rect.height()/2.0f));
   }

   //Obtém os pontos do relacionamento
   points=base_rel->getPoints();

   /* Caso não haja pontos no relacionamento a linha auxiliar será entre
      os pontos centrais das tabelas */
   if(points.empty())
   {
    lin_aux[0].setPoints(p_central[0], p_central[1]);
    lin_aux[1]=lin_aux[0];
   }
   /* Caso haja pontos no relacionamento as linha auxiliares serão:
     1) entre o ponto central da tabela de origem com o primeiro ponto do relacionamento
     2) entre o ponto central da tabela de destino com o último ponto do relacionamento */
   else
   {
    lin_aux[0].setPoints(p_central[0], points[0]);
    lin_aux[1].setPoints(p_central[1], points[points.size()-1]);
   }

   //Criando uma representação gráfica para os pontos do relacionamento
   count=points.size();
   pol_aux.append(QPointF(0,0)); pol_aux.append(QPointF(5,0));
   pol_aux.append(QPointF(5,5)); pol_aux.append(QPointF(0,5));

   for(i=0; i < count; i++)
   {
    if(i >= static_cast<int>(graph_points.size()))
    {
     pol=new QGraphicsPolygonItem;
     graph_points.push_back(pol);
     pol->setZValue(0);
     pol->setPolygon(pol_aux);
     pol->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));
     pol->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
     this->addToGroup(pol);
    }
    else
     pol=graph_points[i];

    pol->setPos(QPointF(points[i].x()-GRAPHIC_PNT_RADIUS,
                        points[i].y()-GRAPHIC_PNT_RADIUS));
    pol->setVisible(this->isSelected());
   }

   //Remove os pontos gráficos não usados
   i=graph_points.size()-1;
   while(i > count-1)
   {
    item=graph_points.back();
    graph_points.pop_back();
    this->removeFromGroup(item);
    delete(item);
    i--;
   }
  }

  //Caso o relacionamento seja de dependência e seja entre uma tabela-visão a linha será tracejada
  pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);
  if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_DEP)
   pen.setStyle(Qt::DashLine);

  //Armazena os pontos de interseção das bordas da tabela no vetor de pontos do relacionamento
  points.insert(points.begin(),p_central[0]);
  points.push_back(p_central[1]);

  /* Caso o relacionamento seja identificador, é criado um ponto adicional
     na mediana da linha que sustenta o descritor. Desta forma os seguimentos de linhas
     entre o descritor e a entidade fraca terão sua espessura modificada para denotar
     o relacionamento identificador */
  if(rel && rel->isIdentifier())
  {
   //Calcula o índice do ponto inicial da linha que sustenta o descritor
   idx_lin_desc=(points.size()/2);
   p_central[0]=points[idx_lin_desc];

   //Obtém o segundo ponto da linha
   if(idx_lin_desc + 1 > static_cast<int>(points.size()))
    p_central[1]=points[idx_lin_desc+1];
   else
    p_central[1]=points[idx_lin_desc-1];

   //Cria o ponto mediano da linha
   p_int.setX((p_central[0].x() + p_central[1].x())/2.0f);
   p_int.setY((p_central[0].y() + p_central[1].y())/2.0f);

   //Insere o mesmo no meio da lista de pontos
   points.insert(points.begin() + idx_lin_desc, p_int);
  }

  //Criando as linhas do relacionamento
  count=points.size();
  for(i=0; i < count-1; i++)
  {
   if(i >= static_cast<int>(lines.size()))
   {
    lin=new QGraphicsLineItem;
    lin->setZValue(-1);
    lines.push_back(lin);
    this->addToGroup(lin);
   }
   else
    lin=lines[i];

   /* Caso seja identificador a linha atual tem sua espessura modificada
      caso seu indice seja igual ou superior ao índice da linha que sustenta
      o descritor */
   if(rel && rel->isIdentifier() && i >= idx_lin_desc)
    pen.setWidthF(1.75f);
   else
    pen.setWidthF(1.0f);

   lin->setLine(QLineF(points[i], points[i+1]));
   lin->setPen(pen);
  }

  //Remove a linhas não utilizadas (caso não for auto relacionamento)
  if(!base_rel->isSelfRelationship())
  {
   i=points.size()-1;
   i1=lines.size();
   while(i1 > i)
   {
    item=lines.back();
    lines.pop_back();
    this->removeFromGroup(item);
    delete(item);
    i1--;
   }
  }

  //Configura os rótulos e o descritor
  this->configureDescriptor();
  this->configureLabels();
  this->configureProtectedIcon();
  configuring_line=false;

  //O tool tip do objeto grafico será o nome formatado do objeto de origem
  this->setToolTip(QString::fromUtf8(base_rel->getName(true)));

  for(i=0; i < 3; i++)
  {
   if(labels[i])
    labels[i]->setToolTip(QString::fromUtf8(base_rel->getName(true)));
  }

  //O tool tip do objeto grafico será o nome formatado do objeto de origem
  descriptor->setToolTip(QString::fromUtf8(base_rel->getName(true)));
 }
}

void RelationshipView::configureDescriptor(void)
{
 QLineF lin;
 QPolygonF pol;
 BaseRelationship *base_rel=this->getSourceObject();
 Relationship *rel=dynamic_cast<Relationship *>(base_rel);
 unsigned rel_type=base_rel->getRelationshipType();
 float x, y, factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;
 QPen pen;
 QPointF pnt;
 vector<QPointF> points=base_rel->getPoints();

 //Configura o estilo da borda do descritor
 pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);

 if(rel_type==BaseRelationship::RELATIONSHIP_DEP)
  pen.setStyle(Qt::DashLine);

 //Configura a borda e preenchimento do descritor
 descriptor->setPen(pen);
 descriptor->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::RELATIONSHIP));

 /* Cria um polígono triangular (para relacionamentos de dep. ou gen.)
    ou losangular (para relacionamentos 1n ou nn) */
 if(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
    rel_type==BaseRelationship::RELATIONSHIP_GEN)
 {
  pol.append(QPointF(0,0)); pol.append(QPointF(21,13));
  pol.append(QPointF(0,26)); pol.append(QPointF(0,13));
 }
 else
 {
  pol.append(QPointF(13,0)); pol.append(QPointF(26,13));
  pol.append(QPointF(13,26)); pol.append(QPointF(0,13));
 }

 //Redimensiona o polígono conforme o fator de tamanho da fonte
 if(factor!=1.0f)
  this->resizePolygon(pol,
                              pol.boundingRect().width() * factor ,
                              pol.boundingRect().height() * factor);

 /* Caso seja um auto relacionamento, o posição do descritor será
    baseada no ponto do meio da lista de pontos */
 if(base_rel->isSelfRelationship())
  pnt=points.at(points.size()/2);
 else
 {
  //Obtém a linha do meio
  lin=lines.at(lines.size()/2)->line();

  /* Caso o relacionamento seja identificador usa como base de posição
     o ponto inicial da linha obtida */
  if(rel && rel->isIdentifier())
   pnt=lin.p1();
  //Caso contrário, calcula o ponto médio da linha obtida
  else
  {
   pnt.setX((lin.p1().x() + lin.p2().x()) / 2.0f);
   pnt.setY((lin.p1().y() + lin.p2().y()) / 2.0f);
  }

  /* Configura o angulo de rotação do descritor, sua sombra e da seleção
     com base na inclinação da linha de sustentação */
  descriptor->setRotation(-lin.angle());
  obj_selection->setRotation(-lin.angle());
  obj_shadow->setRotation(-lin.angle());
 }

 //Configura a posição do descritor
 x=pnt.x() - (pol.boundingRect().width()/2.0f);
 y=pnt.y() - (pol.boundingRect().height()/2.0f);

 //Posiciona o ícone de proteção ao centro do descritor
 protected_icon->setPos(x + ((pol.boundingRect().width()/2.0f) * 0.60f),
                         y + ((pol.boundingRect().height()/2.0f) * 0.55f));

 descriptor->setPolygon(pol);
 descriptor->setTransformOriginPoint(descriptor->boundingRect().center());
 descriptor->setPos(x, y);

 obj_selection->setPolygon(pol);
 obj_selection->setTransformOriginPoint(obj_selection->boundingRect().center());
 obj_selection->setPos(x,y);

 //Posiciona a sombra deslocada em x = 1px e y = 2px em relação ao descritor
 obj_shadow->setPolygon(pol);
 obj_shadow->setTransformOriginPoint(obj_shadow->boundingRect().center());
 obj_shadow->setPos(x + 1.5f, y + 2.5f);
 obj_shadow->setPen(QColor(0,0,0,100));
 obj_shadow->setBrush(QColor(0,0,0,100));

 //Configura os atributos do relacionamento
 this->configureAttributes();
 this->configurePositionInfo();
}

void RelationshipView::configureAttributes(void)
{
 Relationship *rel=dynamic_cast<Relationship *>(this->getSourceObject());

 /* Atributos somente são configurados caso o objeto de origem seja um relacionamento
    entre tabelas. Relacionamentos entre tabela e visão são ignorados */
 if(rel)
 {
  int i, count;
  Column *col=NULL;
  QGraphicsItemGroup *attrib=NULL;
  QGraphicsLineItem *lin=NULL;
  QGraphicsEllipseItem *desc=NULL;
  QGraphicsPolygonItem *sel_attrib=NULL;
  QGraphicsSimpleTextItem *text=NULL;
  QGraphicsItemGroup *item=NULL;
  QPointF p_aux;
  QTextCharFormat fmt;
  QFont font;
  QRectF rect;
  QPolygonF pol;
  float py, px,
        factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

  //Obtém configuração de fonte dos atributos
  fmt=font_config[ParsersAttributes::ATTRIBUTE];
  font=fmt.font();
  font.setPointSizeF(font.pointSizeF() * 0.80f);

  //Configura o retângulo base para criação do descritor do atributo
  rect.setTopLeft(QPointF(0,0));
  rect.setSize(QSizeF(8 * factor, 8 * factor));

  //Calcula a posição do primeiro atributo com base na quantidade de atributos e tamanho do descritor
  count=rel->getAttributeCount();
  px=descriptor->pos().x() + descriptor->boundingRect().width() + ((3 * HORIZ_SPACING) * factor);
  py=descriptor->pos().y() - (count * rect.height()/(4.0f * factor));

  for(i=0; i < count; i++)
  {
   //Obtém um atributo do relacionamento
   col=rel->getAttribute(i);

   //Aloca um atributo
   if(i >= static_cast<int>(attributes.size()))
   {
    attrib=new QGraphicsItemGroup;
    attrib->setZValue(-1);

    //Cria a linha que conecta o atributo ao centro do descritor
    lin=new QGraphicsLineItem;
    lin->setZValue(-1);
    attrib->addToGroup(lin);

    //Cria o descritor do atributo
    desc=new QGraphicsEllipseItem;
    desc->setZValue(0);
    attrib->addToGroup(desc);

    //Cria o texto do atributo
    text=new QGraphicsSimpleTextItem;
    text->setZValue(0);
    attrib->addToGroup(text);

    sel_attrib=new QGraphicsPolygonItem;
    sel_attrib->setZValue(1);
    sel_attrib->setVisible(false);
    attrib->addToGroup(sel_attrib);

    //Adiciona o atributo recém criado ao relacionamento
    this->addToGroup(attrib);
    attributes.push_back(attrib);
   }
   else
   {
    attrib=attributes[i];
    lin=dynamic_cast<QGraphicsLineItem *>(attrib->children().at(0));
    desc=dynamic_cast<QGraphicsEllipseItem *>(attrib->children().at(1));
    text=dynamic_cast<QGraphicsSimpleTextItem *>(attrib->children().at(2));
    sel_attrib=dynamic_cast<QGraphicsPolygonItem *>(attrib->children().at(3));
   }

   desc->setRect(rect);
   desc->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::ATTRIBUTE));
   desc->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::ATTRIBUTE));
   lin->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP));
   text->setBrush(fmt.foreground());
   text->setFont(font);
   sel_attrib->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
   sel_attrib->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));

   //Move o atributo para a posição calculada
   attrib->setPos(px, py);

   //Define o texto e a posição do mesmo no atributo
   text->setText(QString::fromUtf8(col->getName()));
   text->setPos(QPointF(desc->pos().x() + desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
   desc->setPos(0, VERT_SPACING * factor);

   pol.clear();
   pol.append(text->boundingRect().topLeft());
   pol.append(text->boundingRect().topRight()  + QPointF(desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
   pol.append(text->boundingRect().bottomRight() + QPointF(desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
   pol.append(text->boundingRect().bottomLeft());
   sel_attrib->setPolygon(pol);

   //Calcula o ponto central do descritor para a configuração da linha entre o descritor e o atributo
   p_aux=this->mapToItem(attrib, descriptor->pos().x() + (descriptor->boundingRect().width()/2.0f),
                                   descriptor->pos().y() + (descriptor->boundingRect().height()/2.0f));
   lin->setLine(QLineF(p_aux, desc->boundingRect().center()));

   py+=desc->boundingRect().height() + (2 * VERT_SPACING);
  }

  //Remove os atributos não utilizados
  i=attributes.size()-1;
  while(i > count-1)
  {
   item=attributes.back();
   attributes.pop_back();
   this->removeFromGroup(item);
   delete(item);
   i--;
  }
 }
}

void RelationshipView::configureLabels(void)
{
 float x=0,y=0;
 QPointF pnt;
 BaseRelationship *base_rel=this->getSourceObject();
 unsigned rel_type=base_rel->getRelationshipType();
 QPointF label_dist;

 //Obtém os deslocamentos do rótulo de nome
 label_dist=base_rel->getLabelDistance(BaseRelationship::LABEL_REL_NAME);

 //Define a posição do rótulo como sendo abaixo do descritor
 pnt=descriptor->pos();
 x=pnt.x() -
   ((labels[BaseRelationship::LABEL_REL_NAME]->boundingRect().width() -
     descriptor->boundingRect().width())/2.0f);

 if(base_rel->isSelfRelationship())
  y=pnt.y() -
    labels[BaseRelationship::LABEL_REL_NAME]->boundingRect().height() - (2 * VERT_SPACING);
 else
  y=pnt.y() + descriptor->boundingRect().height() + VERT_SPACING;

 labels_ini_pos[BaseRelationship::LABEL_REL_NAME]=QPointF(x,y);

 //Caso haja um deslocamento anterior do rótulo, incrementa sua posição com o deslocamento
 if(!isnan(label_dist.x()))
 {
  x+=label_dist.x();
  y+=label_dist.y();
 }

 //Move o rótulo para o ponto calculado
 labels[BaseRelationship::LABEL_REL_NAME]->setPos(x,y);
 dynamic_cast<Textbox *>(labels[BaseRelationship::LABEL_REL_NAME]->getSourceObject())->setModified(true);

 /* Caso o relacionamento não seja de generalização ou dependência,
    a posição dos rótulos de cardinalidade será atualizada */
 if(rel_type!=BaseRelationship::RELATIONSHIP_GEN &&
    rel_type!=BaseRelationship::RELATIONSHIP_DEP)
 {
  QPointF pi, pf, p_int, pos;
  unsigned idx, i1;
  float dl, da;
  QLineF lins[2], borders[2][4];
  QRectF tab_rect, rect;
  unsigned label_ids[2]={ BaseRelationship::LABEL_SRC_CARD,
                          BaseRelationship::LABEL_DST_CARD };

  lins[0]=lines[0]->line();
  lins[1]=lines[lines.size()-1]->line();

  /* Cria linhas horizontais e verticais denotando as bordas das tabela.
     É com base nestas bordas que serão calculados os pontos de interseção
     da linha do relacionamento com a borda da tabela. Este ponto de interseção
     é onde será posicionado inicialmente o rótulo de cardinalidade de cada tabela */
  for(idx=0; idx < 2; idx++)
  {
   rect=tables[idx]->boundingRect();
   pos=tables[idx]->pos();

   //Cria as linhas de borda da tabela
   borders[idx][0].setPoints(pos, QPointF(pos.x(), pos.y() + rect.height()));
   borders[idx][1].setPoints(QPointF(pos.x(), pos.y() + rect.height()),
                             QPointF(pos.x() + rect.width(), pos.y() + rect.height()));
   borders[idx][2].setPoints(QPointF(pos.x() + rect.width(), pos.y()),
                             QPointF(pos.x() + rect.width(), pos.y() + rect.height()));
   borders[idx][3].setPoints(pos, QPointF(pos.x() + rect.width(), pos.y()));
  }

  /* Calcula os pontos de interseção entre as linhas auxiliares e as bordas das tabela,
     com isso se obtém os pontos onde o relacionamento deve ser conectado */
  for(idx=0; idx < 2; idx++)
  {
   for(i1=0; i1 < 4; i1++)
   {
    if(lins[idx].intersect(borders[idx][i1], &p_int)==QLineF::BoundedIntersection)
    {
     //O ponto central passa a ser o ponto de interseção na borda da tabela
     if(idx==0)
      lins[idx].setP1(p_int);
     else
      lins[idx].setP2(p_int);
     break;
    }
   }
  }

  for(idx=0; idx < 2; idx++)
  {
   /* Caso não haja distância configurada para o rótulo em questão,
      ele será posicionado automaticamente em relaço    linha fixa
      respectiva. Os rótulos de cardinalidade por padrão são posicionados
      de forma a ficarem rente   s linhas fixas. O exemplos mostra os casos
      possíveis:

      1) Linha horizontal:
         ----------                              ----------
         | Tabela |-[rotulo]-----<>-----[rotulo]-| Tabela |
         ----------                              ----------
         >> Os rótulos de cadinalidade são posicionado    frente (ou atrás)
            da tabela e centralizados verticalmente em relaçã   s linhas fixas.

      2) Linha vertical:
         ----------
         | Tabela | >> Os rótulos são posicionados abaixo ou acima das tabelas
         ----------    e centralizados horizontalmente em relaçã   s linhas fixas.
              |
           [rotulo]
              |
           [rotulo]
              |
          ----------
          | Tabela |
          ----------
     */
    if(idx==0)
    {
     pi=lins[idx].p1();
     pf=lins[idx].p2();
    }
    else
    {
     pi=lins[idx].p2();
     pf=lins[idx].p1();
    }

    dl=labels[label_ids[idx]]->boundingRect().width()/2.0f;
    da=labels[label_ids[idx]]->boundingRect().height()/2.0f;

    x=pi.x() - dl;
    y=pi.y() - da;

    tab_rect.setTopLeft(tables[idx]->pos());
    tab_rect.setSize(tables[idx]->boundingRect().size());

    rect.setTopLeft(QPointF(x,y));
    rect.setSize(labels[idx]->boundingRect().size());

    if(rect.contains(tab_rect.bottomRight()))
    { x+=dl + HORIZ_SPACING; y+=da + VERT_SPACING; }
    else if(rect.contains(tab_rect.bottomLeft()))
    { x-=dl + HORIZ_SPACING; y+=da + VERT_SPACING; }
    else if(rect.contains(tab_rect.topLeft()))
    { x-=dl + HORIZ_SPACING; y-=da + VERT_SPACING; }
    else if(rect.contains(tab_rect.topRight()))
    { x+=dl + HORIZ_SPACING;  y-=da + VERT_SPACING; }
    else
    {
     if(tab_rect.contains(rect.bottomLeft()) && tab_rect.contains(rect.bottomRight()))
      y-=da + VERT_SPACING;
     else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.topRight()))
      y+=da + VERT_SPACING;

     if(tab_rect.contains(rect.topRight()) && tab_rect.contains(rect.bottomRight()))
      x-=dl + HORIZ_SPACING;
     else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.bottomLeft()))
      x+=dl + HORIZ_SPACING;
    }

    labels_ini_pos[label_ids[idx]]=QPointF(x,y);
    label_dist=base_rel->getLabelDistance(label_ids[idx]);
    if(!isnan(label_dist.x()))
    {
     x+=label_dist.x();
     y+=label_dist.y();
    }

   //Move o rótulo para a posição calculada
   labels[label_ids[idx]]->setPos(x,y);
   dynamic_cast<Textbox *>(labels[label_ids[idx]]->getSourceObject())->setModified(true);
  }
 }
}

QRectF RelationshipView::__boundingRect(void)
{
 float x1=0, y1=0, x2=0, y2=0;
 unsigned i, count;
 QPointF p;
 QRectF rect;
 vector<QPointF> points=dynamic_cast<BaseRelationship *>(this->getSourceObject())->getPoints();

 //O tamanho de referência será o do descritor
 x1=descriptor->pos().x();
 y1=descriptor->pos().y();
 x2=descriptor->pos().x() + descriptor->boundingRect().width();
 y2=descriptor->pos().y() + descriptor->boundingRect().height();

 //Verifica se algum dos pontos extrapola a dimensão de referência
 count=points.size();
 for(i=0; i < count; i++)
 {
  p=points[i];
  if(x1 > p.x()) x1=p.x() - GRAPHIC_PNT_RADIUS;
  if(y1 > p.y()) y1=p.y() - GRAPHIC_PNT_RADIUS;
  if(x2 < p.x()) x2=p.x() + GRAPHIC_PNT_RADIUS;
  if(y2 < p.y()) y2=p.y() + GRAPHIC_PNT_RADIUS;
 }

 //Verifica se a dimensão de um dos rótulos extrapola a dimensão de referência
 for(i=0; i < 3; i++)
 {
  if(labels[i])
  {
   rect.setTopLeft(labels[i]->scenePos());
   rect.setSize(labels[i]->boundingRect().size());
   if(x1 > rect.left()) x1=rect.left();
   if(y1 > rect.top()) y1=rect.top();
   if(x2 < rect.right()) x2=rect.right();
   if(y2 < rect.bottom()) y2=rect.bottom();
  }
 }

 return(QRectF(x1, y1, x2, y2));
}

