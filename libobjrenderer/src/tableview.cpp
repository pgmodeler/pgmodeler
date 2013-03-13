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

#include "tableview.h"

TableView::TableView(Table *table) : BaseTableView(table)
{
	connect(table, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

	sel_child_obj=NULL;
	ext_attribs_body=new QGraphicsPolygonItem;
	columns=new QGraphicsItemGroup;
	ext_attribs=new QGraphicsItemGroup;

	columns->setZValue(1);
	ext_attribs->setZValue(1);

	this->addToGroup(columns);
	this->addToGroup(ext_attribs);
	this->addToGroup(ext_attribs_body);
	this->configureObject();

	this->setAcceptHoverEvents(true);
}

TableView::~TableView(void)
{
	disconnect(this, SLOT(configureObject(void)));

	this->removeFromGroup(ext_attribs_body);
	this->removeFromGroup(columns);
	this->removeFromGroup(ext_attribs);

	delete(ext_attribs_body);
	delete(columns);
	delete(ext_attribs);
}

void TableView::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

QVariant TableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemSelectedHasChanged)
	{
		this->setToolTip(this->table_tooltip);
		BaseObjectView::configureObjectSelection();
	}

	return(BaseTableView::itemChange(change, value));
}

void TableView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	if(!this->isSelected() && obj_selection->isVisible())
		obj_selection->setVisible(false);

	sel_child_obj=NULL;
}

void TableView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
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

void TableView::configureObject(void)
{
	Table *table=dynamic_cast<Table *>(this->getSourceObject());
	QPolygonF pol;
	int i, count, idx;
	float width, type_width=0, px=0;
	QPen pen;
	TableObjectView *col_item=NULL;
	QList<QGraphicsItem *> subitems;
	QList<TableObjectView *> col_items;
	TableObject *tab_obj=NULL;
	QGraphicsItemGroup *grupos[]={ columns, ext_attribs };
	QGraphicsPolygonItem *corpos[]={ body, ext_attribs_body };
	vector<TableObject *> tab_objs;
	QString atribs[]={ ParsersAttributes::TABLE_BODY, ParsersAttributes::TABLE_EXT_BODY };

	//Configures the table title
	title->configureObject(table);
	px=0;

	for(idx=0; idx < 2; idx++)
	{
		tab_objs.clear();

		if(idx==0)
		{
			tab_objs.assign(table->getObjectList(OBJ_COLUMN)->begin(),
											table->getObjectList(OBJ_COLUMN)->end());
		}
		else
		{
			tab_objs.assign(table->getObjectList(OBJ_RULE)->begin(),
											table->getObjectList(OBJ_RULE)->end());
			tab_objs.insert(tab_objs.end(),
											table->getObjectList(OBJ_TRIGGER)->begin(),
											table->getObjectList(OBJ_TRIGGER)->end());
			tab_objs.insert(tab_objs.end(),
											table->getObjectList(OBJ_INDEX)->begin(),
											table->getObjectList(OBJ_INDEX)->end());
		}

		//Gets the subitems of the current group
		subitems=grupos[idx]->children();
		grupos[idx]->moveBy(-grupos[idx]->scenePos().x(),
												-grupos[idx]->scenePos().y());
		count=tab_objs.size();

		//Special case: if there is no item on extended attributes, the extended body is hidden
		if(idx==1)
		{
			grupos[idx]->setVisible(count > 0);
			corpos[idx]->setVisible(count > 0);
		}

		for(i=0; i < count; i++)
		{
			tab_obj=tab_objs.at(i);

			//Reusing the subitem if it was allocated before
			if(!subitems.isEmpty() && i < subitems.size())
			{
				col_item=dynamic_cast<TableObjectView *>(subitems[i]);
				col_item->setSourceObject(tab_obj);
				col_item->configureObject();
				col_item->moveBy(-col_item->scenePos().x(),
												 -col_item->scenePos().y());
			}
			else
				col_item=new TableObjectView(tab_obj);

			//Configures the item and set its position
			col_item->configureObject();
			col_item->moveBy(HORIZ_SPACING, (i * col_item->boundingRect().height()) + VERT_SPACING);

			/* Calculates the width of the name + type of the object. This is used to align all
			the constraint labels on table */
			width=col_item->getChildObject(0)->boundingRect().width() +
						col_item->getChildObject(1)->boundingRect().width() + (3 * HORIZ_SPACING);
			if(px < width)  px=width;

			//Gets the maximum width of the column type label to align all at same horizontal position
			if(type_width < col_item->getChildObject(2)->boundingRect().width())
				type_width=col_item->getChildObject(2)->boundingRect().width() + (3 * HORIZ_SPACING);

			col_items.push_back(col_item);
		}

		//Destroy the unused items
		i=subitems.size()-1;
		while(i > count-1)
		{
			col_item=dynamic_cast<TableObjectView *>(subitems[i]);
			grupos[idx]->removeFromGroup(col_item);
			delete(col_item);
			i--;
		}

		//Set all items position
		while(!col_items.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(col_items.front());
			grupos[idx]->removeFromGroup(col_item);
			col_items.pop_front();

			//Positioning the type label
			col_item->setChildObjectXPos(2, px);

			//Positioning the constraints label
			col_item->setChildObjectXPos(3, px + type_width);
			grupos[idx]->addToGroup(col_item);
		}
	}

	/* Calculating the maximum width between the title, columns and extended attributes.
		This width is used to set the uniform width of table */
	if(!columns->children().isEmpty() &&
		 (columns->boundingRect().width() > title->boundingRect().width() &&
			columns->boundingRect().width() > ext_attribs->boundingRect().width()))
		width=columns->boundingRect().width() + (2 * HORIZ_SPACING);
	else if(!ext_attribs->children().isEmpty() &&
					(ext_attribs->boundingRect().width() > title->boundingRect().width() &&
					 ext_attribs->boundingRect().width() > columns->boundingRect().width()))
		width=ext_attribs->boundingRect().width() + (2 * HORIZ_SPACING);
	else
		width=title->boundingRect().width() + (2 * HORIZ_SPACING);

	//Resizes the title using the new width
	title->resizeTitle(width, title->boundingRect().height());

	pol.clear();
	pol.append(QPointF(0.0f,0.0f));
	pol.append(QPointF(1.0f,0.0f));
	pol.append(QPointF(1.0f,1.0f));
	pol.append(QPointF(0.0f,1.0f));

	//Resizes the columns/extended attributes using the new width
	for(idx=0; idx < 2; idx++)
	{
		this->resizePolygon(pol, width, grupos[idx]->boundingRect().height() + (2 * VERT_SPACING));
		corpos[idx]->setPolygon(pol);
		corpos[idx]->setBrush(this->getFillStyle(atribs[idx]));
		pen=this->getBorderStyle(atribs[idx]);
		corpos[idx]->setPen(pen);

		if(idx==0)
			corpos[idx]->setPos(title->pos().x(), title->boundingRect().height()-1);
		else
			corpos[idx]->setPos(title->pos().x(),
													title->boundingRect().height() +
													corpos[0]->boundingRect().height() - 2);
		grupos[idx]->setPos(corpos[idx]->pos());

		subitems=grupos[idx]->children();
		while(!subitems.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(subitems.front());
			subitems.pop_front();
			col_item->setChildObjectXPos(3, width -
																	 col_item->boundingRect().width() - (2 * HORIZ_SPACING) - 1);
		}
	}

	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + title->boundingRect().width() * 0.925f,
												 2 * VERT_SPACING);

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(title->boundingRect().width());

	if(!ext_attribs->isVisible())
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() - 1);
	else
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() +
																	ext_attribs_body->boundingRect().height() -2);

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();

	this->table_tooltip=Utf8String::create(table->getName(true)) +
											" (" + table->getTypeName() + ")";

	this->setToolTip(this->table_tooltip);
}
