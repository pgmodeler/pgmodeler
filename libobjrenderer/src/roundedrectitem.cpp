/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "roundedrectitem.h"

RoundedRectItem::RoundedRectItem(QGraphicsItem *parent) :  QGraphicsRectItem(parent)
{
	radius=5;
	corners=ALL_CORNERS;
}

void RoundedRectItem::setBorderRadius(double radius)
{
	this->radius=radius;
	createPolygon();
}

double RoundedRectItem::getBorderRadius(void)
{
	return(radius);
}

void RoundedRectItem::setRoundedCorners(unsigned corners)
{
	if(corners > ALL_CORNERS)
		corners=ALL_CORNERS;

	this->corners=corners;
	createPolygon();
}

unsigned RoundedRectItem::getRoundedCorners(void)
{
	return(corners);
}

bool RoundedRectItem::isCornerRounded(unsigned corner)
{
	return((corners & corner)==corner);
}

void RoundedRectItem::setRect(const QRectF &rect)
{
	QGraphicsRectItem::setRect(rect);
	createPolygon();
}

void RoundedRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(this->pen());
	painter->setBrush(this->brush());

	if(corners==NO_CORNERS)
		painter->drawRect(this->rect());
	else if(corners==ALL_CORNERS)
		painter->drawRoundedRect(this->rect(), radius, radius);
	else
		painter->drawPolygon(polygon);
}

void RoundedRectItem::createPolygon(void)
{
	polygon.clear();

	if(corners!=NO_CORNERS && corners!=ALL_CORNERS && this->rect().isValid())
	{
		QRectF rect=this->rect();

		if(isCornerRounded(TOPLEFT_CORNER))
			polygon << calculatePoints(rect.topLeft() + QPointF(radius, radius), 180, 90);
		else
			polygon.append(this->rect().topLeft());

		if(isCornerRounded(TOPRIGHT_CORNER))
			polygon << calculatePoints(this->rect().topRight() + QPointF(-radius, radius), 90, 0);
		else
			polygon.append(this->rect().topRight());

		if(isCornerRounded(BOTTOMRIGHT_CORNER))
			polygon << calculatePoints(this->rect().bottomRight() + QPointF(-radius, -radius), 360, 270);
		else
			polygon.append(this->rect().bottomRight());

		if(isCornerRounded(BOTTOMLEFT_CORNER))
			polygon << calculatePoints(this->rect().bottomLeft() + QPointF(radius, -radius), 270, 180);
		else
			polygon.append(this->rect().bottomLeft());
	}
}

QVector<QPointF> RoundedRectItem::calculatePoints(QPointF start_pnt, double start_angle, double end_angle)
{
	QVector<QPointF> points;
	QLineF lin;
	double inc=(start_angle > end_angle ? -10 : 10), ang=start_angle;
	bool end=false;

	while(!end)
	{
		lin.setP1(start_pnt);
		lin.setLength(radius);
		lin.setAngle(ang);
		points.append(lin.p2());
		ang+=inc;

		end=((inc > 0 && ang > end_angle) ||
			 (inc < 0 && ang < end_angle));
	}

	return(points);
}
