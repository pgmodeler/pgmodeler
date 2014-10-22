/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

  box=new RoundedRectItem;
	box->setZValue(0);

	this->addToGroup(box);
	this->addToGroup(sch_name);
  this->setZValue(-10);

	this->configureObject();
	all_selected=false;

	this->setFlag(ItemSendsGeometryChanges, true);
}

SchemaView::~SchemaView()
{
  this->removeFromGroup(box);
  this->removeFromGroup(sch_name);

  delete(box);
  delete(sch_name);

	disconnect(this, nullptr, dynamic_cast<BaseGraphicObject *>(this->getSourceObject()), nullptr);
}

void SchemaView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//If the user press SHIFT + left-click select all the schema children
	if(event->modifiers()==Qt::ShiftModifier &&
		 event->buttons()==Qt::LeftButton && !all_selected)
		this->selectChildren();
	else
		BaseObjectView::mousePressEvent(event);
}

void SchemaView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->modifiers()==Qt::ShiftModifier && all_selected)
		event->ignore();
	else
		BaseObjectView::mouseReleaseEvent(event);
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

	//Clears the current scene selection because only one schema and its children can be selected at once
	this->scene()->clearSelection();
	all_selected=true;

	while(itr!=children.end())
	{
		(*itr)->setSelected(true);
		itr++;
	}

	this->setSelected(true);
}

bool SchemaView::isChildrenSelected(void)
{
	QList<BaseObjectView *>::Iterator itr=children.begin();
	bool selected=true;

	while(itr!=children.end() && selected)
	{
		selected=(*itr)->isSelected();
		itr++;
	}

	return(selected);
}

QVariant SchemaView::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemPositionChange)
		last_pos=this->pos();
	else if(change==ItemPositionHasChanged && this->isSelected())
	{	
		float dx=pos().x() - last_pos.x(),
					dy=pos().y() - last_pos.y();

		for(auto child : children)
			child->moveBy(dx, dy);
	}

	return(BaseObjectView::itemChange(change, value));
}

unsigned SchemaView::getChildrenCount()
{
	return(children.size());
}

QList<BaseObjectView *> SchemaView::getChildren(void)
{
	return(children);
}

void SchemaView::configureObject(void)
{
	Schema *schema=dynamic_cast<Schema *>(this->getSourceObject());

	this->fetchChildren();

	/* Only configures the schema view if the rectangle is visible and there are
		children objects. Otherwise the schema view is hidden */
	if(schema->isRectVisible() && !children.isEmpty())
	{
		QColor color;
		QRectF rect;
		QFont font;
		float sp_h=0, sp_v=0, txt_h=0;
    float x1=1000000, y1=1000000, x2=-1000000, y2=-1000000, width=0;
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

    if(rect.width() > sch_name->boundingRect().width())
      width=(x2-x1)+1;
    else
      width=sch_name->boundingRect().width();

		//Configures the box with the points calculated above
		sp_h=(3 * HORIZ_SPACING);
		sp_v=(3 * VERT_SPACING) + txt_h;

    rect.setTopLeft(QPointF(-sp_h, 0));
    rect.setTopRight(QPointF(width + sp_h, 0));
    rect.setBottomRight(QPointF(width + sp_h, y2-y1 + sp_v));
    rect.setBottomLeft(QPointF(-sp_h, y2-y1 + sp_v));
    box->setRect(rect);

		//Sets the schema view position
		this->setFlag(ItemSendsGeometryChanges, false);
		this->moveBy(-this->pos().x(),-this->pos().y());
		this->setPos(QPointF(x1, y1 - txt_h));
		this->setFlag(ItemSendsGeometryChanges, true);

		color=schema->getFillColor();
    color.setAlpha(80);
		box->setBrush(color);

    color=QColor(color.red()/3,color.green()/3,color.blue()/3, 80);
		box->setPen(QPen(color, 1, Qt::DashLine));

		this->bounding_rect=rect;
		this->setVisible(true);

		this->setToolTip(Utf8String::create(schema->getName(true)) +  " (" + schema->getTypeName() + ")");
		sch_name->setToolTip(this->toolTip());

    this->protected_icon->setPos(QPointF( sch_name->boundingRect().width() + sp_h,
                                          sch_name->pos().y() + VERT_SPACING ));

    this->configureObjectSelection();
    this->configureProtectedIcon();
    this->configurePositionInfo(this->pos());
    this->configureSQLDisabledInfo();
	}
	else
		this->setVisible(false);
}
