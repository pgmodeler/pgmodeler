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

	references=new QGraphicsItemGroup;
	references->setZValue(1);
	this->addToGroup(references);
	this->configureObject();
}

GraphicalView::~GraphicalView(void)
{
	this->removeFromGroup(references);
	delete(references);
}

void GraphicalView::configureObject(void)
{
	View *view=dynamic_cast<View *>(this->getSourceObject());
	QPolygonF pol;
	int i, count;
	Reference ref;
	float width;
	QPen pen;
	TableObjectView *graph_ref=NULL;
	QList<QGraphicsItem *> subitems;

	//Configures the view's title
	title->configureObject(view);

	//Gets the reference count on SELECT part of the SQL definition
	count=view->getReferenceCount(Reference::SQL_REFER_SELECT);

	//Moves the references group to the origin to be moved latter
	references->moveBy(-references->scenePos().x(),
										 -references->scenePos().y());

	subitems=references->children();

	for(i=0; i < count; i++)
	{
		ref=view->getReference(i, Reference::SQL_REFER_SELECT);

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

		references->removeFromGroup(graph_ref);
		graph_ref->configureObject(ref);
		graph_ref->moveBy(HORIZ_SPACING, (i * graph_ref->boundingRect().height()) + VERT_SPACING);
		references->addToGroup(graph_ref);
	}

	//Destroy the graphical references not used
	i=subitems.size()-1;
	while(i > count-1)
	{
		graph_ref=dynamic_cast<TableObjectView *>(subitems[i]);
		references->removeFromGroup(graph_ref);
		delete(graph_ref);
		i--;
	}

	if(references->boundingRect().width() > title->boundingRect().width())
		width=references->boundingRect().width() + (2 * HORIZ_SPACING);
	else
		width=title->boundingRect().width() + (2 * HORIZ_SPACING);

	title->resizeTitle(width, title->boundingRect().height());

	pol.append(QPointF(0.0f,0.0f));
	pol.append(QPointF(1.0f,0.0f));
	pol.append(QPointF(1.0f,1.0f));
	pol.append(QPointF(0.0f,1.0f));

	this->resizePolygon(pol, width, references->boundingRect().height() + (2 * VERT_SPACING));
	body->setPolygon(pol);
	body->setBrush(this->getFillStyle(ParsersAttributes::VIEW_BODY));

	pen=this->getBorderStyle(ParsersAttributes::VIEW_BODY);
	pen.setStyle(Qt::DashLine);

	body->setPen(pen);
	body->setPos(0, title->boundingRect().height()-1);
	references->setPos(body->pos());

	//Set the labels (alias) position to the right corner of each reference
	subitems=references->children();
	while(!subitems.isEmpty())
	{
		graph_ref=dynamic_cast<TableObjectView *>(subitems.front());
		subitems.pop_front();
		graph_ref->setChildObjectXPos(3, body->boundingRect().width() -
																	graph_ref->boundingRect().width() - (2 * HORIZ_SPACING));
	}


	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + title->boundingRect().width() * 0.925f,
												 2 * VERT_SPACING);

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(body->boundingRect().width());
	this->bounding_rect.setHeight(title->boundingRect().height() + body->boundingRect().height() - 1);

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();
}

