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

#include "graphicalview.h"

GraphicalView::GraphicalView(View *view) : BaseTableView(view)
{
	connect(view, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

	columns=new QGraphicsItemGroup;
	columns->setZValue(1);
	this->addToGroup(columns);
	configurePlaceholder();
	this->configureObject();
}

void GraphicalView::configureObject(void)
{
	View *view=dynamic_cast<View *>(this->getSourceObject());
	int i, count, count1=0;
	Reference ref;
	QPen pen;
	TableObjectView *graph_ref=nullptr;
	QList<QGraphicsItem *> subitems;
	vector<TableObject *> tab_objs, ext_view_objs;
	QGraphicsItemGroup *groups[]={ columns, ext_attribs };
	RoundedRectItem *bodies[]={ body, ext_attribs_body };
	QString attribs[]={ Attributes::ViewBody, Attributes::ViewExtBody },
			tag_attribs[]={ Attributes::TableBody, Attributes::TableExtBody };
	double width, type_width=0, px=0;
	TableObjectView *col_item=nullptr;
	QList<TableObjectView *> col_items;
	TableObject *tab_obj=nullptr;
	Tag *tag=view->getTag();

	//Configures the view's title
	title->configureObject(view);

	for(auto &obj : view->getObjects())
		ext_view_objs.push_back(dynamic_cast<TableObject *>(obj));

	attribs_toggler->setHasExtAttributes(!hide_ext_attribs && !ext_view_objs.empty());

	//Gets the reference count on SELECT part of the SQL definition
	count=view->getReferenceCount(Reference::SqlReferSelect);

	if(count==0)
		count=count1=view->getReferenceCount(Reference::SqlViewDefinition);

	//Moves the references group to the origin to be moved latter
	columns->moveBy(-columns->scenePos().x(),	-columns->scenePos().y());
	columns->setVisible(view->getCollapseMode() != CollapseMode::AllAttribsCollapsed);
	ext_attribs->setVisible(!ext_view_objs.empty() && view->getCollapseMode() == CollapseMode::NotCollapsed);
	body->setVisible(columns->isVisible());
	ext_attribs_body->setVisible(ext_attribs->isVisible());

	if(!columns->isVisible())
	{
		for(auto &item : columns->childItems())
		{
			columns->removeFromGroup(item);
			delete(item);
		}
	}
	else
	{
		subitems=columns->childItems();
		for(i=0; i < count; i++)
		{
			if(count1==0)
				ref=view->getReference(i, Reference::SqlReferSelect);
			else
				ref=view->getReference(i, Reference::SqlViewDefinition);

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
			graph_ref->moveBy(HorizSpacing, i * graph_ref->boundingRect().height());
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
	}

	if(!hide_ext_attribs && view->getCollapseMode() == CollapseMode::NotCollapsed)
		tab_objs.assign(ext_view_objs.begin(), ext_view_objs.end());

	if(tab_objs.empty())
	{
		for(auto &item : ext_attribs->childItems())
		{
			ext_attribs->removeFromGroup(item);
			delete(item);
		}
	}
	else
	{
		count=tab_objs.size();

		//Gets the subitems of the current group
		subitems=ext_attribs->childItems();
		ext_attribs->moveBy(-ext_attribs->scenePos().x(), -ext_attribs->scenePos().y());
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
			col_item->moveBy(HorizSpacing, (i * col_item->boundingRect().height()) + VertSpacing);

			/* Calculates the width of the name + type of the object. This is used to align all
			the constraint labels on table */
			width=col_item->getChildObject(TableObjectView::ObjDescriptor)->boundingRect().width() +
						col_item->getChildObject(TableObjectView::NameLabel)->boundingRect().width() + (3 * HorizSpacing);
			if(px < width)  px=width;

			//Gets the maximum width of the column type label to align all at same horizontal position
			if(type_width < col_item->getChildObject(TableObjectView::TypeLabel)->boundingRect().width())
				type_width=col_item->getChildObject(TableObjectView::TypeLabel)->boundingRect().width() + (3 * HorizSpacing);

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
			col_item->setChildObjectXPos(TableObjectView::TypeLabel, px);

			//Positioning the constraints label
			col_item->setChildObjectXPos(TableObjectView::ConstrAliasLabel, px + type_width);
			ext_attribs->addToGroup(col_item);
		}
	}

	width = calculateWidth();

	//Resizes the title using the new width
	title->resizeTitle(width, title->boundingRect().height());

	//Resizes the columns/extended attributes using the new width
	for(int idx=0; idx < 2; idx++)
	{
		if(!groups[idx]->isVisible())
			continue;

		bodies[idx]->setRect(QRectF(0,0, width, groups[idx]->boundingRect().height() + (2 * VertSpacing)));
		pen=this->getBorderStyle(attribs[idx]);
		pen.setStyle(Qt::DashLine);

		if(!tag)
			bodies[idx]->setBrush(this->getFillStyle(attribs[idx]));
		else
		{
			bodies[idx]->setBrush(tag->getFillStyle(tag_attribs[idx]));
			pen.setColor(tag->getElementColor(tag_attribs[idx], Tag::BorderColor));
		}

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
			col_item->setChildObjectXPos(TableObjectView::ConstrAliasLabel,
																	 width - col_item->getChildObject(3)->boundingRect().width() - (2 * HorizSpacing));
		}
	}

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(title->boundingRect().width());

	BaseTableView::__configureObject(width);

	if(!view->getAlias().isEmpty())
		table_tooltip += QString("\nAlias: %1").arg(view->getAlias());

	if(!view->getComment().isEmpty())
		table_tooltip += QString("\n---\n%1").arg(view->getComment());

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();
	configureTag();
	configureSQLDisabledInfo();
	requestRelationshipsUpdate();
}

