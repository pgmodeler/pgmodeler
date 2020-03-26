/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "textpolygonitem.h"

TextPolygonItem::TextPolygonItem(QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
	text_item = new QGraphicsSimpleTextItem;
}

TextPolygonItem::~TextPolygonItem()
{
	delete text_item;
}

void TextPolygonItem::setText(const QString &text)
{
	text_item->setText(text);
}

void TextPolygonItem::setTextPen(const QPen &pen)
{
	text_item->setPen(pen);
}

void TextPolygonItem::setTextBrush(const QBrush &brush)
{
	text_item->setBrush(brush);
}

QRectF TextPolygonItem::getTextBoundingRect()
{
	return text_item->boundingRect();
}

void TextPolygonItem::setTextPos(const QPointF &pos)
{
	text_item->setPos(pos);
}

void TextPolygonItem::setTextPos(double x, double y)
{
	text_item->setPos(x, y);
}

void TextPolygonItem::setFont(const QFont &fnt)
{
	text_item->setFont(fnt);
}

QFont TextPolygonItem::getFont()
{
	return text_item->font();
}

void TextPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsPolygonItem::paint(painter, option, widget);

	painter->translate(text_item->pos());
	text_item->paint(painter, option, widget);
}

void TextPolygonItem::resizePolygon(QPolygonF &pol, double width, double height)
{
	QVector<QPointF>::iterator itr,itr_end;
	double coef_a, coef_b;

	itr=pol.begin();
	itr_end=pol.end();

	//Calculates the resize factor
	coef_a=width / pol.boundingRect().width();
	coef_b=height / pol.boundingRect().height();

	//Applies the resize factor to all the polygon points
	while(itr!=itr_end)
	{
		itr->setX(itr->x() * coef_a);
		itr->setY(itr->y() * coef_b);
		itr++;
	}
}
