/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "layerpathitem.h"

LayerPathItem::LayerPathItem(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{

}

LayerPathItem::~LayerPathItem()
{

}

void LayerPathItem::setPath(const QPainterPath &path)
{
	QGraphicsPathItem::setPath(path);

	path_brects.clear();

	for(auto &pol : path.toFillPolygons())
		path_brects.append(pol.boundingRect());
}

void LayerPathItem::setTextAlignment(Qt::Alignment align)
{
	text_align = align;
}

void LayerPathItem::setText(const QString &txt)
{
	text = txt;
}

void LayerPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QFont fnt = painter->font();
	QFontMetrics fm(fnt);
	QPointF pnt;

	painter->save();
	QGraphicsPathItem::paint(painter, option, widget);
	painter->restore();

	painter->save();
	painter->translate(0,0);
	painter->setPen(QColor(0,0,0));
	painter->setBrush(QColor(0,0,0));

	fnt.setPointSizeF(fnt.pointSizeF() * 0.50);
	painter->setFont(fnt);

	for(auto &brect : path_brects)
	{
		if(text_align == Qt::AlignLeft)
			pnt = brect.topLeft() + QPointF(LayerRectSpacing, LayerRectSpacing);
		else
			pnt = brect.topRight() - QPointF(LayerRectSpacing + (fm.horizontalAdvance(text)/2) , -LayerRectSpacing);

		painter->drawText(pnt, text);
	}

	painter->restore();
}
