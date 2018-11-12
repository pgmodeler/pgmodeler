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

	sel_rect = new QGraphicsRectItem;

	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
	{
		arrows[arr_id] = new QGraphicsPolygonItem;
		arrows_selected[arr_id] = false;
	}

	arrows[AttribsExpandArrow]->setToolTip(trUtf8("Expands the currently collapsed section of the object"));
	arrows[AttribsCollapseArrow]->setToolTip(trUtf8("Collapses the currently expanded section of the object"));
	arrows[NextAttribsArrow]->setToolTip(trUtf8("Displays the next attributes page"));
	arrows[PrevAttribsArrow]->setToolTip(trUtf8("Displays the previous attributes page"));
	arrows[TogglePaginationBtn]->setToolTip(trUtf8("Toggles the attributes pagination on the object"));
	has_ext_attribs = false;
	pagination_enabled = false;
	collapse_mode = CollapseMode::NotCollapsed;
	arrows_width = arrows_height = 0;
	curr_attribs_page = 0;
	max_attribs_pages = 5;
	configureButtonsState();
}

AttributesTogglerItem::~AttributesTogglerItem(void)
{
	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
		delete(arrows[arr_id]);

	delete(sel_rect);
}

void AttributesTogglerItem::setArrowsBrush(const QBrush &brush)
{
	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
		arrows[arr_id]->setBrush(brush);
}

void AttributesTogglerItem::setArrowsPen(const QPen &pen)
{
	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
		arrows[arr_id]->setPen(pen);
}

void AttributesTogglerItem::setRect(const QRectF &rect)
{
	configureButtons(rect);
}

void AttributesTogglerItem::setCollapseMode(CollapseMode coll_mode)
{
	//Avoiding setting up Extended attribs collapsed when the toggler is configured to not having extended attribs
	if(!has_ext_attribs && coll_mode == CollapseMode::ExtAttribsCollapsed)
		collapse_mode = CollapseMode::NotCollapsed;
	else
		collapse_mode = coll_mode;

	configureButtonsState();
}

void AttributesTogglerItem::setArrowSelected(const QPointF &pnt, bool clicked)
{
	QRectF rect;
	double h_spacing = 4 * BaseObjectView::HorizSpacing;
	unsigned coll_mode = static_cast<unsigned>(collapse_mode);

	this->setToolTip(QString());
	clearArrowSelection();

	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
	{
		rect.setSize(QSizeF(arrows[arr_id]->boundingRect().width() + h_spacing, this->boundingRect().height()));
		rect.moveTo(arrows[arr_id]->pos().x() - (h_spacing/2), 0);
		arrows_selected[arr_id] = rect.contains(pnt) && arrows[arr_id]->isVisible();

		if(arrows_selected[arr_id])
		{
			this->setToolTip(arrows[arr_id]->toolTip());

			if(clicked)
			{
				if(arr_id == AttribsExpandArrow || arr_id == AttribsCollapseArrow)
				{
					if(arr_id == AttribsExpandArrow)
						coll_mode++;
					else if(arr_id == AttribsCollapseArrow)
						coll_mode--;

					if(!has_ext_attribs && static_cast<CollapseMode>(coll_mode) == CollapseMode::ExtAttribsCollapsed)
						coll_mode += (arr_id == AttribsExpandArrow ? 1 : -1);

					if(coll_mode > enum_cast(CollapseMode::NotCollapsed))
						collapse_mode = (arr_id == AttribsExpandArrow ? CollapseMode::NotCollapsed : CollapseMode::AllAttribsCollapsed);
					else
						collapse_mode = static_cast<CollapseMode>(coll_mode);
				}
				else if(arr_id == TogglePaginationBtn)
				{
					pagination_enabled = !pagination_enabled;
				}
				else if(max_attribs_pages != 0)
				{
					if(arr_id == PrevAttribsArrow)
						curr_attribs_page--;
					else if(arr_id == NextAttribsArrow)
						curr_attribs_page++;

					if(curr_attribs_page >= max_attribs_pages)
						curr_attribs_page = (arr_id == PrevAttribsArrow ? 0 : max_attribs_pages - 1);
				}

				configureButtons(this->rect());
				clearArrowSelection();
				configureButtonsState();

				if(arr_id == TogglePaginationBtn)
					emit s_paginationToggled(pagination_enabled);
				else if(arr_id == AttribsExpandArrow || arr_id == AttribsCollapseArrow)
					emit s_collapseModeChanged(collapse_mode);
				else
					#warning "Just a test! The signal emitted here is for page change"
					emit s_collapseModeChanged(collapse_mode);
			}
			else
			{
				//Configuring the selection rectangle if the arrows isn't clicked
				QRectF rect;
				QSizeF size = QSizeF(arrows[AttribsExpandArrow]->boundingRect().size().width() + (2 * BaseObjectView::HorizSpacing),
														 arrows_height + BaseObjectView::VertSpacing);
				double px = 0, py = 0, arr_x = arrows[arr_id]->pos().x();

				rect.setSize(size);
				px = arr_x - (((arr_x + size.width()) - (arr_x + arrows[arr_id]->boundingRect().width()))/2);
				py = (this->boundingRect().size().height() - size.height())/2.5;

				sel_rect->setBrush(BaseObjectView::getFillStyle(Attributes::ObjSelection));
				sel_rect->setPen(BaseObjectView::getBorderStyle(Attributes::ObjSelection));
				sel_rect->setRect(rect);
				sel_rect->setPos(px, py);
			}

			break;
		}
	}
}

void AttributesTogglerItem::configureButtonsState(void)
{
	arrows[AttribsExpandArrow]->setOpacity(collapse_mode == CollapseMode::ExtAttribsCollapsed ||
																				 collapse_mode == CollapseMode::AllAttribsCollapsed? 1 : 0.40);

	arrows[AttribsCollapseArrow]->setOpacity(collapse_mode == CollapseMode::ExtAttribsCollapsed ||
																					 collapse_mode == CollapseMode::NotCollapsed ? 1 : 0.40);

	arrows[PrevAttribsArrow]->setOpacity(max_attribs_pages != 0 && curr_attribs_page > 0 ? 1 : 0.40);
	arrows[NextAttribsArrow]->setOpacity(max_attribs_pages != 0 && curr_attribs_page < max_attribs_pages - 1 ? 1 : 0.40);

	arrows[PrevAttribsArrow]->setVisible(pagination_enabled);
	arrows[NextAttribsArrow]->setVisible(pagination_enabled);
}

void AttributesTogglerItem::setHasExtAttributes(bool value)
{
	has_ext_attribs = value;
	configureButtonsState();
}

void AttributesTogglerItem::setPaginationEnabled(bool value)
{
	pagination_enabled = value;
	configureButtons(this->boundingRect());
	configureButtonsState();
}

void AttributesTogglerItem::clearArrowSelection(void)
{
	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
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
	arrows_height = pol.boundingRect().height();
	height += arrows_height;

	if(pagination_enabled)
	{
		arrows[PrevAttribsArrow]->setPolygon(pol);
		arr_width = pol.boundingRect().width() + h_spacing;
		pol.remove(0);
		pol.translate(-8 * factor, 0);
		pol.append(QPointF(8 * factor, 5 * factor));
		arrows[NextAttribsArrow]->setPolygon(pol);
		arr_width += pol.boundingRect().width() + h_spacing;
	}

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

	pol.clear();
	pol.append(QPointF(4 * factor, 0));
	pol.append(QPointF(8 * factor, 4 * factor));
	pol.append(QPointF(4 * factor, 8 * factor));
	pol.append(QPointF(0, 4 * factor));
	arrows[TogglePaginationBtn]->setPolygon(pol);
	arr_width += pol.boundingRect().width() + h_spacing;

	arrows_width = arr_width;

	new_rect.setHeight(height);
	RoundedRectItem::setRect(new_rect);

	px = (new_rect.width() - arr_width + h_spacing)/2;

	arrows[TogglePaginationBtn]->setPos(px, (new_rect.height() - arrows[TogglePaginationBtn]->boundingRect().height())/2);
	px += arrows[TogglePaginationBtn]->boundingRect().width() + h_spacing;

	if(pagination_enabled)
	{
		arrows[PrevAttribsArrow]->setPos(px, (new_rect.height() - arrows[PrevAttribsArrow]->boundingRect().height())/2);
		px += arrows[PrevAttribsArrow]->boundingRect().width() + h_spacing;

		arrows[NextAttribsArrow]->setPos(px, (new_rect.height() - arrows[NextAttribsArrow]->boundingRect().height())/2);
		px += arrows[NextAttribsArrow]->boundingRect().width() + h_spacing;
	}

	arrows[AttribsCollapseArrow]->setPos(px, (new_rect.height() - arrows[AttribsCollapseArrow]->boundingRect().height())/2);
	px += arrows[AttribsCollapseArrow]->boundingRect().width() + h_spacing * 0.80;

	arrows[AttribsExpandArrow]->setPos(px, (new_rect.height() - arrows[AttribsExpandArrow]->boundingRect().height())/2);
}

void AttributesTogglerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	RoundedRectItem::paint(painter, option, widget);

	for(unsigned arr_id = 0; arr_id < 5; arr_id++)
	{
		if(!arrows[arr_id]->isVisible())
			continue;

		painter->save();
		painter->translate(arrows[arr_id]->pos());
		painter->setOpacity(arrows[arr_id]->opacity());
		arrows[arr_id]->paint(painter, option, widget);
		painter->restore();

		if(arrows_selected[arr_id] && arrows[arr_id]->opacity() > 0.40)
		{
			painter->save();
			painter->translate(sel_rect->pos());
			sel_rect->paint(painter, option, widget);
			painter->restore();
		}
	}
}
