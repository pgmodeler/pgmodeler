#include "ogvisao.h"

OGVisao::OGVisao(View *visao) : OGTabelaBase(visao)
{
 connect(visao, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

 referencias=new QGraphicsItemGroup;
 referencias->setZValue(1);
 this->addToGroup(referencias);
 this->configureObject();
}

OGVisao::~OGVisao(void)
{
 this->removeFromGroup(referencias);
 delete(referencias);
}

void OGVisao::configureObject(void)
{
 View *visao=dynamic_cast<View *>(this->getSourceObject());
 QPolygonF pol;
 int i, qtd;
 Reference ref_visao;
 float larg;
 QPen pen;
 OGSubItemObjeto *referencia=NULL;
 QList<QGraphicsItem *> subitens;

 //Configura o título da visão
 titulo->configureObject(visao);

 //Obtém a quantidade de referências na parte SELECT
 qtd=visao->getReferenceCount(Reference::SQL_REFER_SELECT);

 //Move o container de referências para origem para ser reposicionado posteriormente
 referencias->moveBy(-referencias->scenePos().x(),
                     -referencias->scenePos().y());
 subitens=referencias->children();

 for(i=0; i < qtd; i++)
 {
  //Obtém uma referência
  ref_visao=visao->getReference(i, Reference::SQL_REFER_SELECT);

  /* Caso o índice 'i' esteja dentro dos limites de subitens de referência
     reaproveita o item e o reconfigura */
  if(!subitens.isEmpty() && i < subitens.size())
  {
   //Obtém a referência ao subitem na posição 'i'
   referencia=dynamic_cast<OGSubItemObjeto *>(subitens[i]);
   //Move a referência para a origem para reposicionar posteriormente
   referencia->moveBy(-referencia->scenePos().x(),
                      -referencia->scenePos().y());
  }
  //Caso o índice esteja fora da capacidade de subitens, aloca um novo
  else
   referencia=new OGSubItemObjeto;

  //Configura o subitem (referência)
  referencias->removeFromGroup(referencia);
  referencia->configureObject(ref_visao);
  referencia->moveBy(HORIZ_SPACING, (i * referencia->boundingRect().height()) + VERT_SPACING);
  //Adiciona a referência configurada ao grupo
  referencias->addToGroup(referencia);
 }

 //Remove as referências não utilizadas
 i=subitens.size()-1;
 while(i > qtd-1)
 {
  referencia=dynamic_cast<OGSubItemObjeto *>(subitens[i]);
  referencias->removeFromGroup(referencia);
  delete(referencia);
  i--;
 }

 /* Calcula a maior largura dentre o título e corpo de referências. Esta largura
    é usada como largura uniforme de toda a visão */
 if(referencias->boundingRect().width() > titulo->boundingRect().width())
  larg=referencias->boundingRect().width() + (2 * HORIZ_SPACING);
 else
  larg=titulo->boundingRect().width() + (2 * HORIZ_SPACING);

 titulo->redimensionarTitulo(larg, titulo->boundingRect().height());

 pol.append(QPointF(0.0f,0.0f));
 pol.append(QPointF(1.0f,0.0f));
 pol.append(QPointF(1.0f,1.0f));
 pol.append(QPointF(0.0f,1.0f));

 //Redimensiona o polígono do corpo de referências
 this->resizePolygon(pol, larg, referencias->boundingRect().height() + (2 * VERT_SPACING));

 corpo->setPolygon(pol);
 corpo->setBrush(this->getFillStyle(ParsersAttributes::VIEW_BODY));

 //Define que a borda de visão é traceja
 pen=this->getBorderStyle(ParsersAttributes::VIEW_BODY);
 pen.setStyle(Qt::DashLine);

 corpo->setPen(pen);
 corpo->setPos(0, titulo->boundingRect().height()-1);
 referencias->setPos(corpo->pos());

 //Reposiciona os rótulos de aliases para o canto direito da visão
 subitens=referencias->children();
 while(!subitens.isEmpty())
 {
  referencia=dynamic_cast<OGSubItemObjeto *>(subitens.front());
  subitens.pop_front();
  referencia->definirPosXObjetoFilho(3, corpo->boundingRect().width() -
                                     referencia->boundingRect().width() - (2 * HORIZ_SPACING));
 }


 //Reposiciona o ícone de proteção da visão na extrema direita do título
 protected_icon->setPos(titulo->pos().x() + titulo->boundingRect().width() * 0.925f,
                         2 * VERT_SPACING);

 /* Define o retângulo de dimensão da visão como sendo o ponto inicial o topLeft() do título
    e a largura sendo a do corpo de referências, já a altura da visão é a soma das alturas
    do elementos que a constitui */
 this->bounding_rect.setTopLeft(titulo->boundingRect().topLeft());
 this->bounding_rect.setWidth(corpo->boundingRect().width());
 this->bounding_rect.setHeight(titulo->boundingRect().height() + corpo->boundingRect().height() - 1);

 BaseObjectView::__configureObject();
 BaseObjectView::configureObjectShadow();
 BaseObjectView::configureObjectSelection();
}

