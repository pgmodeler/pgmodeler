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

#include "graphicalview.h"

GraphicalView::GraphicalView(View *view) : BaseTableView(view)
{
	connect(view, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

	columns=new QGraphicsItemGroup;
	columns->setZValue(1);
	this->addToGroup(columns);
	this->configureObject();
}

GraphicalView::~GraphicalView(void)
{
	disconnect(this, SLOT(configureObject(void)));
}

void GraphicalView::configureObject(void)
{
	View *view=dynamic_cast<View *>(this->getSourceObject());
	QPolygonF pol;
	int i, count, count1=0;
	Reference ref;
	QPen pen;
	TableObjectView *graph_ref=nullptr;
	QList<QGraphicsItem *> subitems;
	vector<TableObject *> tab_objs;
	QGraphicsItemGroup *groups[]={ columns, ext_attribs };
	QGraphicsPolygonItem *bodies[]={ body, ext_attribs_body };
	QString attribs[]={ ParsersAttributes::VIEW_BODY, ParsersAttributes::VIEW_EXT_BODY };
	float width, type_width=0, px=0;
	TableObjectView *col_item=nullptr;
	QList<TableObjectView *> col_items;
	TableObject *tab_obj=nullptr;

	//Configures the view's title
	title->configureObject(view);

	//Gets the reference count on SELECT part of the SQL definition
	count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

	if(count==0)
		count=count1=view->getReferenceCount(Reference::SQL_VIEW_DEFINITION);

	//Moves the references group to the origin to be moved latter
	columns->moveBy(-columns->scenePos().x(),
										 -columns->scenePos().y());

	subitems=columns->childItems();

	for(i=0; i < count; i++)
	{
		if(count1==0)
			ref=view->getReference(i, Reference::SQL_REFER_SELECT);
		else
			ref=view->getReference(i, Reference::SQL_VIEW_DEFINITION);

		//Reuses the subitem if it was allocated before
		if(!subitems.isEmpty() && i < subitems.size())
		{
			graph_ref=dynamic_cast<TableObjectView *>(subitems[i]);

			//Moves the reference to the origin to be moved latter
			graph_ref->moveBy(-graph_ref->scenePos().x(),
												-graph_ref->scenePos().y());
		}
		else
			graph_ref=new TableObjectView;

		columns->removeFromGroup(graph_ref);
		graph_ref->configureObject(ref);
		graph_ref->moveBy(HORIZ_SPACING, (i * graph_ref->boundingRect().height()) + VERT_SPACING);
		columns->addToGroup(graph_ref);
	}

	//Destroy the graphical references not used
	i=subitems.size()-1;
	while(i > count-1)
	{
		graph_ref=dynamic_cast<TableObjectView *>(subitems[i]);
		columns->removeFromGroup(graph_ref);
		delete(graph_ref);
		i--;
	}

	tab_objs.assign(view->getObjectList(OBJ_RULE)->begin(),
									view->getObjectList(OBJ_RULE)->end());
	tab_objs.insert(tab_objs.end(),
									view->getObjectList(OBJ_TRIGGER)->begin(),
									view->getObjectList(OBJ_TRIGGER)->end());

	ext_attribs->setVisible(!tab_objs.empty() && !hide_ext_attribs);
	ext_attribs_body->setVisible(!tab_objs.empty() && !hide_ext_attribs);

	if(!tab_objs.empty())
	{
		count=tab_objs.size();

		//Gets the subitems of the current group
		subitems=ext_attribs->childItems();
		ext_attribs->moveBy(-ext_attribs->scenePos().x(),
												-ext_attribs->scenePos().y());
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
			ext_attribs->removeFromGroup(col_item);
			delete(col_item);
			i--;
		}

		//Set all items position
		while(!col_items.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(col_items.front());
			ext_attribs->removeFromGroup(col_item);
			col_items.pop_front();

			//Positioning the type label
			col_item->setChildObjectXPos(2, px);

			//Positioning the constraints label
			col_item->setChildObjectXPos(3, px + type_width);
			ext_attribs->addToGroup(col_item);
		}
	}

	/* Calculating the maximum width between the title, columns and extended attributes.
		This width is used to set the uniform width of table */
	if(!columns->childItems().isEmpty() &&
		 (columns->boundingRect().width() > title->boundingRect().width() &&
			((hide_ext_attribs || (columns->boundingRect().width() > ext_attribs->boundingRect().width())))))
		width=columns->boundingRect().width() + (2 * HORIZ_SPACING);
	else if(!ext_attribs->childItems().isEmpty() &&  !hide_ext_attribs &&
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
	for(int idx=0; idx < 2; idx++)
	{
		this->resizePolygon(pol, width, groups[idx]->boundingRect().height() + (2 * VERT_SPACING));
		bodies[idx]->setPolygon(pol);
		bodies[idx]->setBrush(this->getFillStyle(attribs[idx]));
		pen=this->getBorderStyle(attribs[idx]);
		pen.setStyle(Qt::DashLine);
		bodies[idx]->setPen(pen);

		if(idx==0)
			bodies[idx]->setPos(title->pos().x(), title->boundingRect().height()-1);
		else
			bodies[idx]->setPos(title->pos().x(),
													title->boundingRect().height() +
													bodies[0]->boundingRect().height() - 2);
		groups[idx]->setPos(bodies[idx]->pos());

		subitems=groups[idx]->childItems();
		while(!subitems.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(subitems.front());
			subitems.pop_front();
			col_item->setChildObjectXPos(3, width -
																	 col_item->boundingRect().width() - (2 * HORIZ_SPACING) - 1);
		}
	}

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(title->boundingRect().width());

	if(!ext_attribs->isVisible())
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() - 1);
	else
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() +
																	ext_attribs_body->boundingRect().height() -2);

	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + title->boundingRect().width() * 0.90f,
												 2 * VERT_SPACING);

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();

	this->table_tooltip=Utf8String::create(view->getName(true)) +
											" (" + view->getTypeName() + ")";

	this->setToolTip(this->table_tooltip);
}

