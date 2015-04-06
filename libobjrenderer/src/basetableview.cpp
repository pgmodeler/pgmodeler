/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

bool BaseTableView::hide_ext_attribs=false;
bool BaseTableView::hide_tags=false;

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

  body=new RoundedRectItem;
  title=new TableTitleView;

  ext_attribs_body=new RoundedRectItem;
  ext_attribs_body->setRoundedCorners(RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER);

  ext_attribs=new QGraphicsItemGroup;
  ext_attribs->setZValue(1);

  columns=new QGraphicsItemGroup;
  columns->setZValue(1);

  tag_name=new QGraphicsSimpleTextItem;
  tag_name->setZValue(3);

  tag_body=new QGraphicsPolygonItem;
  tag_body->setZValue(2);

  obj_shadow=new RoundedRectItem;
  obj_shadow->setZValue(-1);

  obj_selection=new RoundedRectItem;
  obj_selection->setVisible(false);
  obj_selection->setZValue(4);

  this->addToGroup(obj_selection);
  this->addToGroup(obj_shadow);
  this->addToGroup(columns);
	this->addToGroup(body);
	this->addToGroup(title);
  this->addToGroup(tag_name);
  this->addToGroup(tag_body);
	this->addToGroup(ext_attribs);
	this->addToGroup(ext_attribs_body);

	this->setAcceptHoverEvents(true);
	sel_child_obj=nullptr;
	connected_rels=0;
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	this->removeFromGroup(ext_attribs_body);
	this->removeFromGroup(ext_attribs);
	this->removeFromGroup(columns);
  this->removeFromGroup(tag_name);
  this->removeFromGroup(tag_body);
	delete(ext_attribs_body);
	delete(ext_attribs);
	delete(body);
	delete(title);
	delete(columns);
  delete(tag_name);
  delete(tag_body);
}

void BaseTableView::setHideExtAttributes(bool value)
{
  hide_ext_attribs=value;
}

void BaseTableView::setHideTags(bool value)
{
  hide_tags=value;
}

bool BaseTableView::isExtAttributesHidden(void)
{
  return(hide_ext_attribs);
}

bool BaseTableView::isTagsHidden(void)
{
  return(hide_tags);
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

	sel_child_obj=nullptr;
}

void BaseTableView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	/* Case the table itself is not selected shows the child selector
		at mouse position */
	if(!this->isSelected())
	{
		QList<QGraphicsItem *> items;
		double cols_height, item_idx, ext_height=0;
		QRectF rect, rect1;

		items.append(columns->childItems());

		if(!hide_ext_attribs)
		{
			items.append(ext_attribs->childItems());
			ext_height=ext_attribs->boundingRect().height();
		}

		//Calculates the default item height
		cols_height=roundf((columns->boundingRect().height() + ext_height) / static_cast<double>(items.size()));

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
      //QPolygonF pol;
			BaseObjectView *item=dynamic_cast<TableObjectView *>(items[item_idx]);

			//Configures the selection with the item's dimension
			if(obj_selection->boundingRect().height()!=item->boundingRect().height())
			{
        dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
        dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0,0,
                                                                       title->boundingRect().width() - (2.5 * HORIZ_SPACING),
                                                                       item->boundingRect().height()));
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

void BaseTableView::updateConnectedRelsCount(int inc)
{
	connected_rels+=inc;
  if(connected_rels < 0) connected_rels=0;
}

void BaseTableView::configureTag(void)
{
  BaseTable *tab=dynamic_cast<BaseTable *>(this->getSourceObject());
  Tag *tag=tab->getTag();

  tag_body->setVisible(tag!=nullptr && !hide_tags);
  tag_name->setVisible(tag!=nullptr && !hide_tags);

  if(!hide_tags && tag)
  {
    QPolygonF pol;
    QPointF p1, p2;
    double bottom;
    QFont fnt=BaseObjectView::getFontStyle(ParsersAttributes::TAG).font();

    fnt.setPointSizeF(fnt.pointSizeF() * 0.80f);
    tag_name->setFont(fnt);
    tag_name->setText(tag->getName());
    tag_name->setBrush(BaseObjectView::getFontStyle(ParsersAttributes::TAG).foreground());

    p1=tag_name->boundingRect().topLeft(),
    p2=tag_name->boundingRect().bottomRight();
    bottom=this->boundingRect().bottom();

    pol.append(QPointF(p1.x()-BaseObjectView::HORIZ_SPACING, p1.y() - BaseObjectView::VERT_SPACING));
    pol.append(QPointF(p2.x(), p1.y() - BaseObjectView::VERT_SPACING));
    pol.append(QPointF(p2.x() + BaseObjectView::HORIZ_SPACING + 5, p2.y()/2));
    pol.append(QPointF(p2.x(), p2.y() + BaseObjectView::VERT_SPACING));
    pol.append(QPointF(p1.x(), p2.y() + BaseObjectView::VERT_SPACING));
    pol.append(QPointF(p1.x()-BaseObjectView::HORIZ_SPACING, p2.y() + BaseObjectView::VERT_SPACING));

    tag_body->setPolygon(pol);
    tag_body->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::TAG));
    tag_body->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::TAG));

    tag_name->setPos(-5, bottom - 1.5f);
    tag_body->setPos(-5, bottom - 1.5f);
  }
}

int BaseTableView::getConnectRelsCount(void)
{
	return(connected_rels);
}

