#include "styledtextboxview.h"

StyledTextboxView::StyledTextboxView(Textbox *txtbox, bool override_style) : TextboxView(txtbox, override_style)
{
	QPolygonF pol;

	pol.append(QPointF(0,0));
	pol.append(QPointF(20,0));
	pol.append(QPointF(0,20));

	fold=new QGraphicsPolygonItem;
	fold->setPolygon(pol);

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

	this->__configureObject();
	fold->setBrush(text_item->brush());
	fold->setPen(text_item->pen());

	rect = text_item->boundingRect();
	pol = text_item->polygon();

	if(rect.height() < fold->boundingRect().height())
		rect.setHeight(fold->boundingRect().height() + (2 * VertSpacing));

	TextPolygonItem::resizePolygon(pol, rect.width() + fold->boundingRect().width(), rect.height());

	pnt=pol.at(2);
	pol.remove(2);
	pol.insert(2, QPointF(pnt.x(), round(pnt.y() - fold->boundingRect().height())));
	pol.insert(3, QPointF(round(pnt.x() - fold->boundingRect().width()), pnt.y()));

	text_item->setPolygon(pol);
	rect = text_item->boundingRect();
	fold->setPos(rect.width() - fold->boundingRect().width(),
							 rect.height() - fold->boundingRect().height());

	bounding_rect = text_item->boundingRect();

	this->configureObjectShadow();
	this->configureObjectSelection();
}
