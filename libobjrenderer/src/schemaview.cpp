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

#include "schemaview.h"

SchemaView::SchemaView(Schema *schema) : BaseObjectView(schema)
{
	connect(schema, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

	sch_name=new QGraphicsSimpleTextItem;
	sch_name->setZValue(1);

	box=new QGraphicsPolygonItem;
	box->setZValue(0);

	this->addToGroup(box);
	this->addToGroup(sch_name);
	this->setZValue(-10);

	//Shadow objects are not used in this type of object
	delete(this->obj_shadow);
	this->obj_shadow=nullptr;

	this->configureObject();
}

void SchemaView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//If the user press SHIFT + left-click select all the schema children
	if(event->modifiers()==Qt::ShiftModifier &&
		 event->buttons()==Qt::LeftButton)
	{
		this->setFlag(QGraphicsItem::ItemIsMovable, false);
		this->setFlag(QGraphicsItem::ItemIsSelectable, false);
		this->scene()->clearSelection();
		this->selectChildren();
	}
	//Selects the schema
	else
	{
		this->setFlag(QGraphicsItem::ItemIsMovable);
		this->setFlag(QGraphicsItem::ItemIsSelectable);
		BaseObjectView::mousePressEvent(event);
		this->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
}

void SchemaView::fetchChildren(void)
{
	Schema *schema=dynamic_cast<Schema *>(this->getSourceObject());
	DatabaseModel *model=dynamic_cast<DatabaseModel *>(schema->getDatabase());
	vector<BaseObject *> objs, objs1;

	//Gets all tables and views that belongs to the schema
	objs=model->getObjects(OBJ_TABLE, schema);
	objs1=model->getObjects(OBJ_VIEW, schema);
	objs.insert(objs.end(), objs1.begin(), objs1.end());

	children.clear();
	while(!objs.empty())
	{
		children.push_front(dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(objs.back())->getReceiverObject()));
		objs.pop_back();
	}
}

void SchemaView::selectChildren(void)
{
	QList<BaseObjectView *>::Iterator itr=children.begin();

	while(itr!=children.end())
	{
		(*itr)->setSelected(true);
		itr++;
	}
}

void SchemaView::configureObject(void)
{
	Schema *schema=dynamic_cast<Schema *>(this->getSourceObject());

	this->fetchChildren();

	/* Only configures the schema view if the rectangle is visible and there are
		children objects. Otherwise the schema view is hidden */
	if(schema->isRectVisible() && !children.isEmpty())
	{
		QPolygonF pol;
		QColor color;
		QRectF rect;
		QFont font;
		float sp_h, sp_v, txt_h;
		float x1=1000000, y1=1000000, x2=-1000000, y2=-1000000;
		QList<BaseObjectView *>::Iterator itr=children.begin();

		//Configures the bounding rect based upon the children dimension
		while(itr!=children.end())
		{
			rect.setTopLeft((*itr)->pos());
			rect.setSize((*itr)->boundingRect().size());

			if(rect.left() < x1)
				x1 = rect.left();
			if(rect.right() > x2)
				x2 = rect.right();

			if(rect.top() < y1)
				y1 = rect.top();
			if(rect.bottom() > y2)
				y2 = rect.bottom();

			itr++;
		}

		//Configures the schema name at the top
		sch_name->setText(Utf8String::create(schema->getName()));
		font=BaseObjectView::getFontStyle(ParsersAttributes::GLOBAL).font();
		font.setItalic(true);
		font.setBold(true);
		font.setPointSizeF(font.pointSizeF() * 1.3f);
		sch_name->setFont(font);
		sch_name->setPos(HORIZ_SPACING, VERT_SPACING);
		txt_h=sch_name->boundingRect().height() + (2 * VERT_SPACING);

		//Configures the box with the points calculated above
		sp_h=(3 * HORIZ_SPACING);
		sp_v=(3 * VERT_SPACING) + txt_h;
		pol.append(QPointF(-sp_h, 0));
		pol.append(QPointF(x2-x1 + sp_h, 0));
		pol.append(QPointF(x2-x1 + sp_h, y2-y1 + sp_v));
		pol.append(QPointF(-sp_h, y2-y1 + sp_v));
		box->setPolygon(pol);

		//Sets the schema view position
		this->moveBy(-this->pos().x(),-this->pos().y());
		this->setPos(QPointF(x1, y1 - txt_h));

		color=schema->getFillColor();
		color.setAlpha(80);
		box->setBrush(color);

		color=QColor(color.red()/3,color.green()/3,color.blue()/3, 80);
		box->setPen(QPen(color, 1, Qt::DashLine));

		rect.setTopLeft(pol.at(0));
		rect.setTopRight(pol.at(1));
		rect.setBottomRight(pol.at(2));
		rect.setBottomLeft(pol.at(3));

		this->bounding_rect=rect;
		this->setVisible(true);

		this->configureObjectSelection();
		this->configureProtectedIcon();
		this->configurePositionInfo(this->pos());

		this->setToolTip(Utf8String::create(schema->getName(true)) +  " (" + schema->getTypeName() + ")");
		sch_name->setToolTip(this->toolTip());

		this->protected_icon->setPos(QPointF( sch_name->boundingRect().width() + sp_h,
																					sch_name->pos().y() + VERT_SPACING ));
	}
	else
		this->setVisible(false);
}
