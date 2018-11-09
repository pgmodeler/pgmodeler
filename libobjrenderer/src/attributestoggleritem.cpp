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

	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
	{
		arrows[arr_id] = new QGraphicsPolygonItem;
		arrows_selected[arr_id] = false;
	}

	arrows[AttribsExpandArrow]->setToolTip(trUtf8("Expands the currently collapsed section of the object"));
	arrows[AttribsCollapseArrow]->setToolTip(trUtf8("Collapses the currently expanded section of the object"));
	arrows[NextAttribsArrow]->setToolTip(trUtf8("Display the next columns page"));
	arrows[PrevAttribsArrow]->setToolTip(trUtf8("Display the previous columns page"));
	has_ext_attribs = false;
	collapse_mode = CollapseMode::NotCollapsed;
	arrows_width = 0;
}

void AttributesTogglerItem::setArrowsBrush(const QBrush &brush)
{
	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
		arrows[arr_id]->setBrush(brush);
}

void AttributesTogglerItem::setArrowsPen(const QPen &pen)
{
	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
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
	unsigned coll_mode = static_cast<unsigned>(collapse_mode);

	this->setToolTip(QString());
	clearArrowSelection();

	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
	{
		rect.setSize(QSizeF(arrows[arr_id]->boundingRect().width() + h_spacing, this->boundingRect().height()));
		rect.moveTo(arrows[arr_id]->pos().x() - (h_spacing/2), 0);
		arrows_selected[arr_id] = rect.contains(pnt);

		if(arrows_selected[arr_id])
		{
			this->setToolTip(arrows[arr_id]->toolTip());

			if(clicked)
			{
				if(arr_id == AttribsExpandArrow)
					coll_mode++;
				else if(arr_id == AttribsCollapseArrow)
					coll_mode--;

				if(coll_mode > enum_cast(CollapseMode::NotCollapsed))
					collapse_mode = (arr_id == AttribsExpandArrow ? CollapseMode::NotCollapsed : CollapseMode::AllAttribsCollapsed);
				else
					collapse_mode = static_cast<CollapseMode>(coll_mode);

				configureButtons(this->rect());
				emit s_collapseModeChanged(collapse_mode);
			}

			break;
		}
	}

	this->update();
}

void AttributesTogglerItem::setHasExtAttributes(bool value)
{
	has_ext_attribs = value;
}

void AttributesTogglerItem::clearArrowSelection(void)
{
	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
		arrows_selected[arr_id] = false;

	this->update();
}

double AttributesTogglerItem::getArrowsWidth(void)
{
	return(arrows_width);
}

void AttributesTogglerItem::configureButtons(const QRectF &rect)
{
	double fnt_factor = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->logicalDotsPerInch() / 96.0f,
			pixel_ratio = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->devicePixelRatio(),
			factor = fnt_factor * pixel_ratio,
			arr_width = 0, px = 0,
			h_spacing = 6 * BaseObjectView::HorizSpacing,
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
	pol.append(QPointF(5 * factor, 0));
	pol.append(QPointF(0, 8 * factor));
	pol.append(QPointF(10 * factor, 8 * factor));
	arrows[AttribsCollapseArrow]->setPolygon(pol);
	arr_width += pol.boundingRect().width() + h_spacing;

	pol.clear();
	pol.append(QPointF(0, 0));
	pol.append(QPointF(10 * factor, 0));
	pol.append(QPointF(5 * factor, 8 * factor));
	arrows[AttribsExpandArrow]->setPolygon(pol);
	arr_width += pol.boundingRect().width() + h_spacing;

	arrows_width = arr_width;

	new_rect.setHeight(height);
	RoundedRectItem::setRect(new_rect);

	px = (new_rect.width() - arr_width + h_spacing)/2;

	arrows[PrevAttribsArrow]->setPos(px, (new_rect.height() - arrows[PrevAttribsArrow]->boundingRect().height())/2);
	px += arrows[PrevAttribsArrow]->boundingRect().width() + h_spacing;

	arrows[NextAttribsArrow]->setPos(px, (new_rect.height() - arrows[NextAttribsArrow]->boundingRect().height())/2);
	px += arrows[NextAttribsArrow]->boundingRect().width() + h_spacing;

	arrows[AttribsCollapseArrow]->setPos(px, (new_rect.height() - arrows[AttribsCollapseArrow]->boundingRect().height())/2);
	px += arrows[AttribsCollapseArrow]->boundingRect().width() + h_spacing * 0.80;

	arrows[AttribsExpandArrow]->setPos(px, (new_rect.height() - arrows[AttribsExpandArrow]->boundingRect().height())/2);
}

void AttributesTogglerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	double p_opacity = 1;
	RoundedRectItem::paint(painter, option, widget);

	for(unsigned arr_id = 0; arr_id < 4; arr_id++)
	{
		painter->save();
		painter->translate(arrows[arr_id]->pos());

		if((collapse_mode == CollapseMode::NotCollapsed && arr_id == AttribsExpandArrow) ||
			 (collapse_mode == CollapseMode::AllAttribsCollapsed && arr_id == AttribsCollapseArrow))
			p_opacity = 0.40;
		else
			p_opacity = 1;

		painter->setOpacity(p_opacity);
		arrows[arr_id]->paint(painter, option, widget);
		painter->restore();

		if(arrows_selected[arr_id] && p_opacity > 0.40)
		{
			QRectF sel_rect;
			QSizeF size = QSizeF(arrows[AttribsExpandArrow]->boundingRect().size().width() + (2 * BaseObjectView::HorizSpacing),
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
