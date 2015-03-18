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

#include "tableview.h"

TableView::TableView(Table *table) : BaseTableView(table)
{
	connect(table, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));
	this->configureObject();
}

void TableView::configureObject(void)
{
	Table *table=dynamic_cast<Table *>(this->getSourceObject());
	int i, count, obj_idx;
	float width=0, type_width=0, px=0, cy=0, old_width=0, old_height=0;
	QPen pen;
	TableObjectView *col_item=nullptr;
	QList<QGraphicsItem *> subitems;
	QList<TableObjectView *> col_items;
	TableObject *tab_obj=nullptr;
	QGraphicsItemGroup *groups[]={ columns, ext_attribs };
  RoundedRectItem *bodies[]={ body, ext_attribs_body };
	vector<TableObject *> tab_objs;
	QString atribs[]={ ParsersAttributes::TABLE_BODY, ParsersAttributes::TABLE_EXT_BODY };
  Tag *tag=table->getTag();

	//Configures the table title
	title->configureObject(table);
	px=0;

	old_width=this->bounding_rect.width();
	old_height=this->bounding_rect.height();

	for(obj_idx=0; obj_idx < 2; obj_idx++)
	{
		tab_objs.clear();

		if(obj_idx==0)
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
		subitems=groups[obj_idx]->childItems();
		groups[obj_idx]->moveBy(-groups[obj_idx]->scenePos().x(),
												-groups[obj_idx]->scenePos().y());
		count=tab_objs.size();

		//Special case: if there is no item on extended attributes, the extended body is hidden
		if(obj_idx==1)
		{
			groups[obj_idx]->setVisible(count > 0 && !hide_ext_attribs);
			bodies[obj_idx]->setVisible(count > 0 && !hide_ext_attribs);
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
			groups[obj_idx]->removeFromGroup(col_item);
			delete(col_item);
			i--;
		}

		//Set all items position
		while(!col_items.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(col_items.front());
			groups[obj_idx]->removeFromGroup(col_item);
			col_items.pop_front();

			//Positioning the type label
			col_item->setChildObjectXPos(2, px);

			//Positioning the constraints label
			col_item->setChildObjectXPos(3, px + type_width);
			groups[obj_idx]->addToGroup(col_item);
		}
	}

	/* Calculating the maximum width between the title, columns and extended attributes.
		This width is used to set the uniform width of table */
	if(!columns->childItems().isEmpty() &&
		 (columns->boundingRect().width() > title->boundingRect().width() &&
			(hide_ext_attribs || (columns->boundingRect().width() > ext_attribs->boundingRect().width()))))
		width=columns->boundingRect().width() + (2 * HORIZ_SPACING);
	else if(!ext_attribs->childItems().isEmpty() && !hide_ext_attribs &&
					(ext_attribs->boundingRect().width() > title->boundingRect().width() &&
					 ext_attribs->boundingRect().width() > columns->boundingRect().width()))
		width=ext_attribs->boundingRect().width() + (2 * HORIZ_SPACING);
	else
		width=title->boundingRect().width() + (2 * HORIZ_SPACING);

	//Resizes the title using the new width
	title->resizeTitle(width, title->boundingRect().height());

	//Resizes the columns/extended attributes using the new width
	for(obj_idx=0; obj_idx < 2; obj_idx++)
	{
    bodies[obj_idx]->setRect(QRectF(0,0, width, groups[obj_idx]->boundingRect().height() + (2 * VERT_SPACING)));
		pen=this->getBorderStyle(atribs[obj_idx]);

    if(!tag)
			bodies[obj_idx]->setBrush(this->getFillStyle(atribs[obj_idx]));
    else
    {
			pen.setColor(tag->getElementColor(atribs[obj_idx], Tag::BORDER_COLOR));
			bodies[obj_idx]->setBrush(tag->getFillStyle(atribs[obj_idx]));
    }

		bodies[obj_idx]->setPen(pen);

		if(obj_idx==0)
			bodies[obj_idx]->setPos(title->pos().x(), title->boundingRect().height()-1);
		else
			bodies[obj_idx]->setPos(title->pos().x(),
													title->boundingRect().height() +
													bodies[0]->boundingRect().height() - 2);
		groups[obj_idx]->setPos(bodies[obj_idx]->pos());

		subitems=groups[obj_idx]->childItems();
		while(!subitems.isEmpty())
		{
			col_item=dynamic_cast<TableObjectView *>(subitems.front());
			subitems.pop_front();
			col_item->setChildObjectXPos(3, width -
																	 col_item->boundingRect().width() - (2 * HORIZ_SPACING) - 1);


			//Generating the connection points of the columns
			if(obj_idx==0)
			{
				tab_obj=dynamic_cast<TableObject *>(col_item->getSourceObject());
				cy=title->boundingRect().height() + col_item->pos().y() + (col_item->boundingRect().height()/2);
				conn_points[tab_obj].resize(2);
				conn_points[tab_obj][LEFT_CONN_POINT]=QPointF(col_item->pos().x(), cy);
				conn_points[tab_obj][RIGHT_CONN_POINT]=QPointF(col_item->pos().x() + width - 4.5f , cy);
			}
		}
	}

	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + title->boundingRect().width() * 0.90f,
												 2 * VERT_SPACING);

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(title->boundingRect().width());

	if(!ext_attribs->isVisible())
  {
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() - 1);
    body->setRoundedCorners(RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER);
  }
	else
  {
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() +
																	ext_attribs_body->boundingRect().height() -2);
    body->setRoundedCorners(RoundedRectItem::NONE_CORNERS);
  }

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();

  this->table_tooltip=table->getName(true) +
                      QString(" (") + table->getTypeName() + QString(") \n") +
                      QString("Id: %1\n").arg(table->getObjectId()) +
											TableObjectView::CONSTR_DELIM_START +
											trUtf8("Connected rels: %1").arg(this->getConnectRelsCount()) +
											TableObjectView::CONSTR_DELIM_END;

	this->setToolTip(this->table_tooltip);

  configureTag();
  configureSQLDisabledInfo();

	if((old_width!=0 && this->bounding_rect.width()!=old_width) ||
		 (old_height!=0 && this->bounding_rect.height()!=old_height))
		emit s_objectDimensionChanged();
}

QPointF TableView::getConnectionPoints(TableObject *tab_obj, unsigned pnt_type)
{
	if(!tab_obj)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(pnt_type > RIGHT_CONN_POINT)
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(conn_points.count(tab_obj)==0)
		//Returns the center point in case of the connection point of the table object wasn't calculated already
		return(this->getCenter());

	return(conn_points[tab_obj][pnt_type]);
}
