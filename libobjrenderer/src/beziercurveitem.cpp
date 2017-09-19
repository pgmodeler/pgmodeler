/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "beziercurveitem.h"
#include "baserelationship.h"

BezierCurveItem::BezierCurveItem(QGraphicsItem *parent) :  QGraphicsPathItem(parent)
{

}

void BezierCurveItem::setPath(const QPainterPath &path)
{
	QPainterPathStroker ps;
	QGraphicsPathItem::setPath(path);
	stroke = ps.createStroke(path);
}

void BezierCurveItem::setLine(const QLineF &line, bool simple_curve, bool invert_brect, unsigned rel_type)
{
	QPainterPath path;
	QRectF brect;
	QPointF control_pnt;

	if(simple_curve)
	{
		if(rel_type == BaseRelationship::RELATIONSHIP_FK)
		{
			brect.setTopLeft(line.p1());
			brect.setBottomRight(line.p2());
			control_pnt = QPointF(brect.right(), brect.top());
		}
		else
		{
			if(line.angle() >= 0 && line.angle() <= 180)
			{
				brect.setTopLeft(line.p1());
				brect.setBottomRight(line.p2());
				control_pnt = QPointF(brect.left(), brect.bottom());
			}
			else
			{
				brect.setTopLeft(line.p2());
				brect.setBottomRight(line.p1());
				control_pnt = QPointF(brect.right(), brect.top());
			}
		}

		path = QPainterPath(brect.topLeft());
	}
	else
	{
		if(!invert_brect)
		{
			brect.setTopLeft(line.p1());
			brect.setBottomRight(line.p2());
			path = QPainterPath(brect.topLeft());
		}
		else
		{
			brect.setBottomLeft(line.p1());
			brect.setTopRight(line.p2());
			path = QPainterPath(brect.topRight());
		}
	}

	if(simple_curve)
		path.quadTo(control_pnt, brect.bottomRight());
	else
	{
		if(!invert_brect)
		{
			path.cubicTo(QPointF(brect.center().x(), brect.top()),
									 QPointF(brect.center().x(), brect.bottom()),
									 brect.bottomRight());
		}
		else
		{
			path.cubicTo(QPointF(brect.right(), brect.center().y()),
									 QPointF(brect.left(), brect.center().y()),
									 brect.bottomLeft());
		}
	}

	this->setPath(path);
}

bool BezierCurveItem::contains(const QPointF &pnt) const
{
	return(stroke.contains(pnt));
}

bool BezierCurveItem::collidesWithPath(const QPainterPath &path, Qt::ItemSelectionMode) const
{
	return(stroke.intersects(path));
}
