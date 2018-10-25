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

#include "tabletitleview.h"

TableTitleView::TableTitleView(void) : BaseObjectView(nullptr)
{
	schema_name=new QGraphicsSimpleTextItem;
	schema_name->setZValue(1);

	obj_name=new QGraphicsSimpleTextItem;
	obj_name->setZValue(1);

	//box=new QGraphicsPolygonItem;
	box=new RoundedRectItem;
	box->setRoundedCorners(RoundedRectItem::TopLeftCorner | RoundedRectItem::TopRightCorner);
	box->setZValue(0);

	this->addToGroup(box);
	this->addToGroup(schema_name);
	this->addToGroup(obj_name);
}

TableTitleView::~TableTitleView(void)
{
	this->removeFromGroup(schema_name);
	this->removeFromGroup(obj_name);
	this->removeFromGroup(box);
	delete(schema_name);
	delete(obj_name);
	delete(box);
}

void TableTitleView::configureObject(BaseGraphicObject *object)
{
	QTextCharFormat fmt;
	QString name_attrib, schema_name_attrib, title_color_attrib;
	QPen pen;
	Schema *schema=nullptr;
	QFont font;
	Tag *tag=nullptr;
	Table *table = dynamic_cast<Table *>(object);

	//Raises an error if the object related to the title is not allocated
	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	//Raises an error if the object is invalid
	else if(object->getObjectType()!=ObjectType::Table  &&
			object->getObjectType()!=ObjectType::View)
		throw Exception(ErrorCode::OprObjectInvalidType, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	schema=dynamic_cast<Schema *>(object->getSchema());
	tag=dynamic_cast<BaseTable *>(object)->getTag();

	if(object->getObjectType()==ObjectType::View && !tag)
	{
		name_attrib=Attributes::VIEW_NAME;
		schema_name_attrib=Attributes::VIEW_SCHEMA_NAME;
		title_color_attrib=Attributes::VIEW_TITLE;
	}
	else
	{
		name_attrib=Attributes::TABLE_NAME;
		schema_name_attrib=Attributes::TABLE_SCHEMA_NAME;
		title_color_attrib=Attributes::TABLE_TITLE;
	}

	//Strike out the table name when its sql is disabled
	fmt=font_config[schema_name_attrib];
	font=fmt.font();
	schema_name->setFont(font);

	if(!tag)
		schema_name->setBrush(fmt.foreground());
	else
		schema_name->setBrush(tag->getElementColor(schema_name_attrib, Tag::FillColor1));

	if(schema->isRectVisible())
		schema_name->setText(QString(" "));
	else
	{
		if(compact_view && !schema->getAlias().isEmpty())
			schema_name->setText(schema->getAlias() + QString("."));
		else
			schema_name->setText(schema->getName() + QString("."));
	}

	fmt=font_config[name_attrib];
	font=fmt.font();

	obj_name->setFont(font);
	obj_name->setText(compact_view && !object->getAlias().isEmpty() ? object->getAlias() : object->getName());

	if(!tag)
	{
		obj_name->setBrush(fmt.foreground());
		box->setBrush(this->getFillStyle(title_color_attrib));
	}
	else
	{
		obj_name->setBrush(tag->getElementColor(name_attrib, Tag::FillColor1));
		box->setBrush(tag->getFillStyle(title_color_attrib));
	}

	pen=this->getBorderStyle(title_color_attrib);

	if(tag)
		pen.setColor(tag->getElementColor(title_color_attrib, Tag::BorderColor));

	if(object->getObjectType()==ObjectType::View ||
	   (table && table->isPartition()))
		pen.setStyle(Qt::DashLine);

	box->setPen(pen);

	if(schema->isRectVisible())
		this->resizeTitle(obj_name->boundingRect().width()  + (2 * HorizSpacing),
						  obj_name->boundingRect().height() + (2 * VertSpacing));
	else
		this->resizeTitle(obj_name->boundingRect().width() + schema_name->boundingRect().width() + (2 * HorizSpacing),
						  schema_name->boundingRect().height() + (2 * VertSpacing));
}

void TableTitleView::resizeTitle(double width, double height)
{
	box->setRect(QRectF(0,0, width, height));

	if(schema_name->text()==QString(" "))
		obj_name->setPos((box->boundingRect().width() - obj_name->boundingRect().width())/2.0f, VertSpacing);
	else
	{
		schema_name->setPos((box->boundingRect().width() - (schema_name->boundingRect().width() + obj_name->boundingRect().width()))/2.0f, VertSpacing);
		obj_name->setPos(schema_name->pos().x() + schema_name->boundingRect().width(), VertSpacing);
		obj_name->setPos(schema_name->pos().x() + schema_name->boundingRect().width(), VertSpacing);
	}

	this->bounding_rect.setTopLeft(this->pos());
	this->bounding_rect.setSize(QSizeF(box->boundingRect().width(), box->boundingRect().height()));
}

