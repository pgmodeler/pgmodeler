#include "styledtextboxview.h"

StyledTextboxView::StyledTextboxView(Textbox *txtbox, bool override_style) : TextboxView(txtbox, override_style)
{
	fold=new QGraphicsPolygonItem;
	this->addToGroup(fold);
	this->configureObject();
}

StyledTextboxView::~StyledTextboxView()
{
	this->removeFromGroup(fold);
	delete fold;
}

void StyledTextboxView::configureObject()
{
	QRectF rect;
	QPolygonF pol;
	QPointF pnt;
	RoundedRectItem rect_item;

	this->__configureObject();

	pol.append(QPointF(0,0));
	pol.append(QPointF(25,0));
	pol.append(QPointF(0,25));

	fold->setBrush(text_item->brush());
	fold->setPen(text_item->pen());
	fold->setPolygon(pol);

	rect = text_item->boundingRect();
	rect.setLeft(rect.left() - HorizSpacing);

	if(rect.height() < fold->boundingRect().height())
		rect.setHeight(fold->boundingRect().height() + (2 * VertSpacing));

	rect_item.setRoundedCorners(RoundedRectItem::TopLeftCorner |
															RoundedRectItem::TopRightCorner |
															RoundedRectItem::BottomLeftCorner);
	rect_item.setRect(rect);
	pol = rect_item.getPolygon();
	resizePolygon(pol, rect.width() + fold->boundingRect().width(), rect.height());

	pnt = pol.at(20);
	pol[20] = QPointF(pnt.x(), round(pnt.y() - fold->boundingRect().height()));
	pol[21] = QPointF(round(pnt.x() - fold->boundingRect().width()), pnt.y());

	text_item->setPolygon(pol);
	rect = text_item->boundingRect();
	fold->setPos(rect.width() - fold->boundingRect().width(),
							 rect.height() - fold->boundingRect().height());

	bounding_rect = text_item->boundingRect();

	this->configureObjectShadow();
	this->configureObjectSelection();
}
