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
	int i, count;
	QPen pen;
	TableObjectView *graph_ref=nullptr;
	QList<QGraphicsItem *> subitems;
	vector<TableObject *> tab_objs, ext_tab_objs;
	QGraphicsItemGroup *groups[]={ columns, ext_attribs };
	RoundedRectItem *bodies[]={ body, ext_attribs_body };
	QString attribs[]={ Attributes::ViewBody, Attributes::ViewExtBody },
			tag_attribs[]={ Attributes::TableBody, Attributes::TableExtBody };
	double width, type_width=0, px=0;
	TableObjectView *col_item=nullptr;
	QList<TableObjectView *> col_items;
	TableObject *tab_obj=nullptr;
	Tag *tag=view->getTag();
	QStringList col_names, col_types;
	unsigned col_cnt = 0, ext_attr_cnt = 0, total_objs_cnt = 0,
			start_col = 0, end_col = 0, start_ext = 0, end_ext = 0;
	CollapseMode collapse_mode = view->getCollapseMode();

	//Configures the view's title
	title->configureObject(view);

	for(auto &obj : view->getObjects())
		ext_tab_objs.push_back(dynamic_cast<TableObject *>(obj));

	attribs_toggler->setHasExtAttributes(!hide_ext_attribs && !ext_tab_objs.empty());

	col_names = (!compact_view ? view->getColumnNames() : view->getColumnAliases());
	col_types = view->getColumnTypes();

	/* Calculating the amount of objects visible on the table.
	 * If the extended attributes are hidden somehow the amount of these objects is discarded and
	 * only columns will be visible on the pagination */
	if(collapse_mode != CollapseMode::AllAttribsCollapsed)
		total_objs_cnt = col_names.size() + (collapse_mode != CollapseMode::ExtAttribsCollapsed ? ext_tab_objs.size() : 0);

	attribs_toggler->setPaginationEnabled(view->isPaginationEnabled());

	/* If the pagination is enabled for the table and the amount of objects is greater than the
	 * number of objects per page we configure the pagination parameter */
	if(view->isPaginationEnabled() && total_objs_cnt > attribs_per_page)
	{
		// Calculating the proportions of columns and extended attributes displayed per page
		double col_factor = col_names.size() / static_cast<double>(total_objs_cnt),
					ext_factor = 1 - col_factor;
		unsigned max_pages = 0, res = 0, curr_page = view->getCurrentPage();

		/* Determining the amount of columns and extended attribs to be displayed based upon
		 * the proportions of columns/ext. attribs and the total of these objects */
		col_cnt = floor(col_factor * attribs_per_page);
		ext_attr_cnt = hide_ext_attribs ? 0 : floor(ext_factor * attribs_per_page);

		/* If the extended attributes factor is too small so the counter related to it
		 * is zero we force it to have at least one visible extended attribute */
		if(ext_attr_cnt == 0 && !hide_ext_attribs && !ext_tab_objs.empty())
			ext_attr_cnt = 1;

		/* In certain situations the calculation aren't exact so we include to the column amount
		 * the remaining number of elements (generally this ins't greater than 1) */
		res = attribs_per_page - (col_cnt + ext_attr_cnt);
		if(res > 0) col_cnt += res;

		// Determining the maximum amount of pages
		max_pages = ceil(total_objs_cnt / static_cast<double>(attribs_per_page));

		// Validating the current page related to the maximum determined
		if(curr_page >= max_pages)
			curr_page = max_pages - 1;

		// Calculating the start and end columns/ext. attributes for the current page
		start_col = curr_page * col_cnt;
		end_col = start_col + col_cnt;
		start_ext = curr_page * ext_attr_cnt;
		end_ext = start_ext + ext_attr_cnt;

		if(start_col > static_cast<unsigned>(col_names.size()))
			start_col = col_names.size();

		if(start_ext > ext_tab_objs.size())
			start_ext = ext_tab_objs.size();

		if(end_col >  static_cast<unsigned>(col_names.size()) || curr_page == max_pages - 1)
			end_col = col_names.size();

		if(end_ext > ext_tab_objs.size() || curr_page == max_pages - 1)
			end_ext = ext_tab_objs.size();

		// Configure the attributes toggler item withe the calculated pagination parameters
		attribs_toggler->setPaginationValues(curr_page, max_pages);
	}
	else
	{
		total_objs_cnt = 0;
		col_cnt = col_names.size();
		ext_attr_cnt = ext_tab_objs.size();
		attribs_toggler->setPaginationValues(0, 0);
	}

	//Moves the references group to the origin to be moved latter
	columns->moveBy(-columns->scenePos().x(),	-columns->scenePos().y());
	columns->setVisible(view->getCollapseMode() != CollapseMode::AllAttribsCollapsed && start_col < static_cast<unsigned>(col_names.size()));
	body->setVisible(columns->isVisible());

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
		QStringList aux_col_names, aux_col_types;

		aux_col_names = col_names.mid(start_col, col_cnt);
		aux_col_types = col_types.mid(start_col, col_cnt);
		count = aux_col_names.size();

		subitems=columns->childItems();
		for(i=0; i < count; i++)
		{
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

			graph_ref->configureObject(aux_col_names[i], aux_col_types[i], QString());
			graph_ref->moveBy(HorizSpacing, (i * graph_ref->boundingRect().height()) + VertSpacing);

			/* Calculates the width of the name + type of the object. This is used to align all
			the constraint labels on table */
			width=graph_ref->getChildObject(TableObjectView::ObjDescriptor)->boundingRect().width() +
						graph_ref->getChildObject(TableObjectView::NameLabel)->boundingRect().width() + (8 * HorizSpacing);
			if(px < width)  px=width;

			//Gets the maximum width of the column type label to align all at same horizontal position
			if(type_width < graph_ref->getChildObject(TableObjectView::TypeLabel)->boundingRect().width())
				type_width=graph_ref->getChildObject(TableObjectView::TypeLabel)->boundingRect().width() + (3 * HorizSpacing);

			col_items.push_back(graph_ref);
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

		//Set all items position
		while(!col_items.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(col_items.front());
			columns->removeFromGroup(col_item);
			col_items.pop_front();

			//Positioning the type label
			col_item->setChildObjectXPos(TableObjectView::TypeLabel, px);

			//Positioning the constraints label			
			col_item->setChildObjectXPos(TableObjectView::ConstrAliasLabel, px + type_width);
			columns->addToGroup(col_item);
		}
	}

	if(!hide_ext_attribs && view->getCollapseMode() == CollapseMode::NotCollapsed)
	{
		if(view->isPaginationEnabled() && total_objs_cnt != 0)
			tab_objs.assign(ext_tab_objs.begin() + start_ext, ext_tab_objs.begin() + end_ext);
		else
			tab_objs.assign(ext_tab_objs.begin(), ext_tab_objs.end());
	}

	ext_attribs->setVisible(!tab_objs.empty() && view->getCollapseMode() == CollapseMode::NotCollapsed);
	ext_attribs_body->setVisible(ext_attribs->isVisible());

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
		/* Configuring the brush and pen of the bodies even if they aren't visible
		 * the attributes toggler at the bottom of the view uses the color of the attributes body
		 * this will avoid the creation of a transparent toggler */
		if(!tag)
		{
			bodies[idx]->setBrush(this->getFillStyle(attribs[idx]));
			pen=this->getBorderStyle(attribs[idx]);
		}
		else
		{
			bodies[idx]->setBrush(tag->getFillStyle(tag_attribs[idx]));
			pen.setColor(tag->getElementColor(tag_attribs[idx], Tag::BorderColor));
		}

		pen.setStyle(Qt::DashLine);
		bodies[idx]->setPen(pen);

		// We avoid the construction of the rect related to the current body item if the related group isn't visible
		if(!groups[idx]->isVisible())
			continue;

		bodies[idx]->setRect(QRectF(0,0, width, groups[idx]->boundingRect().height() + (2 * VertSpacing)));

		if(idx==0)
			bodies[idx]->setPos(title->pos().x(), title->boundingRect().height() - 1);
		else
		{
			if(bodies[0]->isVisible())
				bodies[idx]->setPos(title->pos().x(),
														title->boundingRect().height() +
														bodies[0]->boundingRect().height() - 2);
			else
				bodies[idx]->setPos(title->pos().x(), title->boundingRect().height()-1);
		}

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

