/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "basetableview.h"

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	body=new QGraphicsPolygonItem;
	title=new TableTitleView;
	ext_attribs_body=new QGraphicsPolygonItem;
	ext_attribs=new QGraphicsItemGroup;
	columns=new QGraphicsItemGroup;
	columns->setZValue(1);
	this->addToGroup(columns);
	this->addToGroup(body);
	this->addToGroup(title);
	this->addToGroup(ext_attribs);
	this->addToGroup(ext_attribs_body);
	ext_attribs->setZValue(1);
	this->setAcceptHoverEvents(true);
	sel_child_obj=NULL;
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	this->removeFromGroup(ext_attribs_body);
	this->removeFromGroup(ext_attribs);
	this->removeFromGroup(columns);
	delete(ext_attribs_body);
	delete(ext_attribs);
	delete(body);
	delete(title);
	delete(columns);
}

QVariant BaseTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemSelectedHasChanged)
	{
		this->setToolTip(this->table_tooltip);
		BaseObjectView::configureObjectSelection();
	}

	if(change==ItemPositionHasChanged)
		emit s_objectMoved();

	BaseObjectView::itemChange(change, value);

	return(value);
}

void BaseTableView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Emit a signal containing the select child object if the user right-click the focused item
	if(!this->isSelected() && event->buttons()==Qt::RightButton && sel_child_obj)
	{
		if(this->scene())
		 this->scene()->clearSelection();

		/* Deactivate the table in order not to hide the child object selection.
			 The table object is reativated when the context menu is hidden */
		this->setEnabled(false);

		emit s_childObjectSelected(sel_child_obj);
	}
	else
		BaseObjectView::mousePressEvent(event);
}

void BaseTableView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	if(!this->isSelected() && obj_selection->isVisible())
		obj_selection->setVisible(false);

	sel_child_obj=NULL;
}

void BaseTableView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	/* Case the table itself is not selected shows the child selector
		at mouse position */
	if(!this->isSelected())
	{
		QList<QGraphicsItem *> items;
		float cols_height;
		float item_idx;
		QRectF rect, rect1;

		items.append(columns->children());
		items.append(ext_attribs->children());

		//Calculates the default item height
		cols_height=roundf((columns->boundingRect().height() +
												ext_attribs->boundingRect().height()) / static_cast<float>(items.size()));

		//Calculates the item index based upon the mouse position
		rect=this->mapRectToItem(title, title->boundingRect());
		item_idx=(event->pos().y() - rect.bottom()) / cols_height;

		//If the index is invalid clears the selection
		if(item_idx < 0 || item_idx >= items.size())
		{
			this->hoverLeaveEvent(event);
			this->setToolTip(this->table_tooltip);
		}
		else if(!items.isEmpty())
		{
			QPolygonF pol;
			BaseObjectView *item=dynamic_cast<TableObjectView *>(items[item_idx]);

			//Configures the selection with the item's dimension
			if(obj_selection->boundingRect().height()!=item->boundingRect().height())
			{
				pol.append(QPointF(0.0f,0.0f));
				pol.append(QPointF(1.0f,0.0f));
				pol.append(QPointF(1.0f,1.0f));
				pol.append(QPointF(0.0f,1.0f));
				this->resizePolygon(pol, title->boundingRect().width() - (2.5 * HORIZ_SPACING),
														item->boundingRect().height());
				obj_selection->setPolygon(pol);
			}

			//Sets the selection position as same as item's position
			rect1=this->mapRectToItem(item, item->boundingRect());
			obj_selection->setVisible(true);
			obj_selection->setPos(QPointF(title->pos().x() + HORIZ_SPACING,-rect1.top()));

			//Stores the selected child object
			sel_child_obj=dynamic_cast<TableObject *>(item->getSourceObject());
			this->setToolTip(item->toolTip());
		}
	}
}
