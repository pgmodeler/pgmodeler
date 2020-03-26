/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "objectsscene.h"

SchemaView::SchemaView(Schema *schema) : BaseObjectView(schema)
{
	connect(schema, SIGNAL(s_objectModified()), this, SLOT(configureObject()));

	sch_name=new QGraphicsSimpleTextItem;
	sch_name->setZValue(1);

	box=new RoundedRectItem;
	box->setZValue(0);

	obj_selection=new RoundedRectItem;
	obj_selection->setVisible(false);
	obj_selection->setZValue(4);
	this->addToGroup(obj_selection);

	this->addToGroup(box);
	this->addToGroup(sch_name);
	this->setZValue(-100);

	this->configureObject();
	all_selected=false;

	this->setFlag(ItemSendsGeometryChanges, true);
}

SchemaView::~SchemaView()
{
	this->removeFromGroup(box);
	this->removeFromGroup(sch_name);

	delete box;
	delete sch_name;
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

void SchemaView::fetchChildren()
{
	Schema *schema=dynamic_cast<Schema *>(this->getUnderlyingObject());
	DatabaseModel *model=dynamic_cast<DatabaseModel *>(schema->getDatabase());
	vector<BaseObject *> objs, list;
	vector<ObjectType> types = { ObjectType::Table, ObjectType::ForeignTable, ObjectType::View };

	//Gets all tables and views that belongs to the schema
	for(auto &type : types)
	{
		list = model->getObjects(type, schema);
		objs.insert(objs.end(), list.begin(), list.end());
	}

	children.clear();
	while(!objs.empty())
	{
		children.push_front(dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(objs.back())->getOverlyingObject()));
		objs.pop_back();
	}
}

void SchemaView::selectChildren()
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

bool SchemaView::isChildrenSelected()
{
	QList<BaseObjectView *>::Iterator itr=children.begin();
	bool selected=true;

	while(itr!=children.end() && selected)
	{
		selected=(*itr)->isSelected();
		itr++;
	}

	return selected;
}

QVariant SchemaView::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemPositionChange)
		last_pos=this->pos();
	else if(change==ItemPositionHasChanged && this->isSelected())
	{
		double dx=pos().x() - last_pos.x(),
				dy=pos().y() - last_pos.y();

		for(auto &child : children)
			child->moveBy(dx, dy);
	}

	return BaseObjectView::itemChange(change, value);
}

unsigned SchemaView::getChildrenCount()
{
	return children.size();
}

QList<BaseObjectView *> SchemaView::getChildren()
{
	return children;
}

void SchemaView::togglePlaceholder(bool visible)
{
	for(auto &obj : getChildren())
		obj->togglePlaceholder(visible);
}

void SchemaView::moveTo(QPointF new_pos)
{
	double dx=new_pos.x() - pos().x(),
			dy=new_pos.y() - pos().y();

	this->setPos(new_pos);

	for(auto &child : children)
		child->moveBy(dx, dy);
}

void SchemaView::configureObject()
{
	Schema *schema=dynamic_cast<Schema *>(this->getUnderlyingObject());
	this->fetchChildren();

	/* Only configures the schema view if the rectangle is visible and there are
		children objects. Otherwise the schema view is hidden */
	if(schema->isRectVisible() && !children.isEmpty())
	{
		QColor color;
		QRectF rect;
		QFont font;
		double sp_h=0, sp_v=0, txt_h=0;
		double x1=1000000, y1=1000000, x2=-1000000, y2=-1000000, width=0;
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
		sch_name->setText(compact_view && !schema->getAlias().isEmpty() ? schema->getAlias() : schema->getName());
		font=BaseObjectView::getFontStyle(Attributes::Global).font();
		font.setItalic(true);
		font.setBold(true);
		font.setPointSizeF(font.pointSizeF() * 1.3);

		sch_name->setFont(font);
		sch_name->setPos(HorizSpacing, VertSpacing);
		txt_h=sch_name->boundingRect().height() + (2 * VertSpacing);

		//Configures the box with the points calculated above
		sp_h=(3 * HorizSpacing);
		sp_v=(3 * VertSpacing) + txt_h;

		width=(x2-x1) + 1;

		if(width < sch_name->boundingRect().width())
			width=sch_name->boundingRect().width();

		rect.setTopLeft(QPointF(-sp_h, 0));
		rect.setTopRight(QPointF(width + sp_h, 0));
		rect.setBottomRight(QPointF(width + sp_h, y2-y1 + sp_v));
		rect.setBottomLeft(QPointF(-sp_h, y2-y1 + sp_v));
		box->setRect(rect);

		//Sets the schema view position
		this->setFlag(ItemSendsGeometryChanges, false);
		this->moveBy(-this->pos().x(),-this->pos().y());
		this->setPos(QPointF(x1, y1 - txt_h));
		schema->setPosition(this->mapToScene(rect.topLeft()));
		this->setFlag(ItemSendsGeometryChanges, true);

		color=schema->getFillColor();
		color.setAlpha(ObjectAlphaChannel * 0.80);
		box->setBrush(color);

		color=QColor(color.red()/3,color.green()/3,color.blue()/3, 80);
		box->setPen(QPen(color, 1, Qt::SolidLine));

		this->bounding_rect=rect;

		ObjectsScene *scene = dynamic_cast<ObjectsScene *>(this->scene());
		this->setVisible(scene && scene->isLayerActive(schema->getLayer()));

		this->setToolTip(schema->getName(true) +
										 QString(" (") + schema->getTypeName() + QString(")") +
										 QString("\nId: %1").arg(schema->getObjectId()));
		sch_name->setToolTip(this->toolTip());

		this->protected_icon->setPos(QPointF( sch_name->boundingRect().width() + sp_h,
											  sch_name->pos().y() + VertSpacing ));

		this->configureObjectSelection();
		this->configureProtectedIcon();
		this->configurePositionInfo(this->pos());
		this->configureSQLDisabledInfo();
	}
	else
		this->setVisible(false);
}
