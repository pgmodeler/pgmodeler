/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "attributestoggleritem.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include "baseobjectview.h"

AttributesTogglerItem::AttributesTogglerItem(QGraphicsItem *parent) : RoundedRectItem(parent)
{
	this->setRoundedCorners(RoundedRectItem::BottomLeftCorner | RoundedRectItem::BottomRightCorner);

	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
	{
		arrows[arr_id] = new QGraphicsPolygonItem;
		arrows_selected[arr_id] = false;
	}

	arrows[AttribsToggler]->setToolTip(trUtf8("Toggles the display of columns and extended attributes"));
	arrows[NextAttribsArrow]->setToolTip(trUtf8("Display the next columns page"));
	arrows[PrevAttribsArrow]->setToolTip(trUtf8("Display the previous columns page"));
	toggler_inverted = false;
}

void AttributesTogglerItem::setArrowsBrush(const QBrush &brush)
{
	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
		arrows[arr_id]->setBrush(brush);
}

void AttributesTogglerItem::setArrowsPen(const QPen &pen)
{
	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
		arrows[arr_id]->setPen(pen);
}

void AttributesTogglerItem::setRect(const QRectF &rect)
{
	configureButtons(rect);
}

void AttributesTogglerItem::setCollapseMode(CollapseMode coll_mode)
{
	collapse_mode = coll_mode;
	configureButtons(this->rect());
}

void AttributesTogglerItem::setArrowSelected(const QPointF &pnt, bool clicked)
{
	QRectF rect;
	double h_spacing = 4 * BaseObjectView::HorizSpacing;

	this->setToolTip(QString());

	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
	{
		rect.setSize(QSizeF(arrows[arr_id]->boundingRect().width() + h_spacing, this->boundingRect().height()));
		rect.moveTo(arrows[arr_id]->pos().x() - (h_spacing/2), 0);
		arrows_selected[arr_id] = rect.contains(pnt);

		if(arrows_selected[arr_id])
		{
			this->setToolTip(arrows[arr_id]->toolTip());

			if(clicked)
			{
				if(collapse_mode == CollapseMode::NotCollapsed)
					collapse_mode = CollapseMode::ExtAttribsCollapsed;
				else if(collapse_mode == CollapseMode::ExtAttribsCollapsed)
					collapse_mode = CollapseMode::AllAttribsCollapsed;
				else
					collapse_mode = CollapseMode::NotCollapsed;

				configureButtons(this->rect());
				emit s_collapseModeChanged(collapse_mode);
			}
		}
	}

	this->update();
}

void AttributesTogglerItem::clearArrowSelection(void)
{
	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
		arrows_selected[arr_id] = false;

	this->update();
}

void AttributesTogglerItem::configureButtons(const QRectF &rect)
{
	double fnt_factor = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->logicalDotsPerInch() / 96.0f,
			pixel_ratio = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->devicePixelRatio(),
			factor = fnt_factor * pixel_ratio,
			arr_width = 0, px = 0,
			h_spacing = 8 * BaseObjectView::HorizSpacing,
			height =  4 * BaseObjectView::VertSpacing;
	QPolygonF pol;
	QRectF new_rect = rect;

	pol.append(QPointF(0, 5 * factor));
	pol.append(QPointF(8 * factor, 0));
	pol.append(QPointF(8 * factor, 10 * factor));
	height += pol.boundingRect().height();
	arr_width = pol.boundingRect().width() + h_spacing;
	arrows[PrevAttribsArrow]->setPolygon(pol);

	pol.remove(0);
	pol.translate(-8 * factor, 0);
	pol.append(QPointF(8 * factor, 5 * factor));
	arrows[NextAttribsArrow]->setPolygon(pol);
	arr_width += pol.boundingRect().width() + h_spacing;

	pol.clear();
	if(collapse_mode == CollapseMode::NotCollapsed)
	{
		pol.append(QPointF(5 * factor, 0));
		pol.append(QPointF(0, 8 * factor));
		pol.append(QPointF(10 * factor, 8 * factor));
	}
	else
	{
		pol.append(QPointF(0, 0));
		pol.append(QPointF(10 * factor, 0));
		pol.append(QPointF(5 * factor, 8 * factor));
	}

	arrows[AttribsToggler]->setPolygon(pol);
	arr_width += pol.boundingRect().width();

	new_rect.setHeight(height);
	RoundedRectItem::setRect(new_rect);

	px = (new_rect.width() - arr_width)/2;

	arrows[PrevAttribsArrow]->setPos(px, (new_rect.height() - arrows[PrevAttribsArrow]->boundingRect().height())/2);
	px += arrows[PrevAttribsArrow]->boundingRect().width() + h_spacing;

	arrows[NextAttribsArrow]->setPos(px, (new_rect.height() - arrows[NextAttribsArrow]->boundingRect().height())/2);
	px += arrows[NextAttribsArrow]->boundingRect().width() + h_spacing * 0.80;

	arrows[AttribsToggler]->setPos(px, (new_rect.height() - arrows[AttribsToggler]->boundingRect().height())/2);
}

void AttributesTogglerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	RoundedRectItem::paint(painter, option, widget);

	for(unsigned arr_id = 0; arr_id < 3; arr_id++)
	{
		painter->save();
		painter->translate(arrows[arr_id]->pos());
		arrows[arr_id]->paint(painter, option, widget);
		painter->restore();

		if(arrows_selected[arr_id])
		{
			QRectF sel_rect;
			QSizeF size = QSizeF(arrows[AttribsToggler]->boundingRect().size().width() + (2 * BaseObjectView::HorizSpacing),
													 arrows[PrevAttribsArrow]->boundingRect().size().height() + BaseObjectView::HorizSpacing);
			double tx = 0, ty = 0, arr_x = arrows[arr_id]->pos().x();

			sel_rect.setSize(size);
			tx = arr_x - (((arr_x + size.width()) - (arr_x + arrows[arr_id]->boundingRect().width()))/2);
			ty = (this->boundingRect().size().height() - size.height())/2.5;

			painter->save();
			painter->translate(tx, ty);
			painter->setBrush(BaseObjectView::getFillStyle(Attributes::ObjSelection));
			painter->setPen(BaseObjectView::getBorderStyle(Attributes::ObjSelection));
			painter->drawRect(sel_rect);
			painter->restore();
		}
	}
}
