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

#include "tabletitleview.h"

TableTitleView::TableTitleView(void) : BaseObjectView(NULL)
{
	schema_name=new QGraphicsSimpleTextItem;
	schema_name->setZValue(1);

	obj_name=new QGraphicsSimpleTextItem;
	obj_name->setZValue(1);

	box=new QGraphicsPolygonItem;
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
	Schema *schema=dynamic_cast<Schema *>(object->getSchema());

	//Raises an error if the object related to the title is not allocated
	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	//Raises an error if the object is invalid
	else if(object->getObjectType()!=OBJ_TABLE  &&
					object->getObjectType()!=OBJ_VIEW)
		throw Exception(ERR_OPR_OBJ_INV_TYPE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(object->getObjectType()==OBJ_VIEW)
	{
		name_attrib=ParsersAttributes::VIEW_NAME;
		schema_name_attrib=ParsersAttributes::VIEW_SCHEMA_NAME;
		title_color_attrib=ParsersAttributes::VIEW_TITLE;
	}
	else
	{
		name_attrib=ParsersAttributes::TABLE_NAME;
		schema_name_attrib=ParsersAttributes::TABLE_SCHEMA_NAME;
		title_color_attrib=ParsersAttributes::TABLE_TITLE;
	}

	fmt=font_config[schema_name_attrib];
	schema_name->setFont(fmt.font());
	schema_name->setBrush(fmt.foreground());

	if(schema->isRectVisible())
		schema_name->setText(" ");
	else
		schema_name->setText(Utf8String::create(schema->getName() + "."));

	fmt=font_config[name_attrib];
	obj_name->setFont(fmt.font());
	obj_name->setBrush(fmt.foreground());
	obj_name->setText(Utf8String::create(object->getName()));

	box->setBrush(this->getFillStyle(title_color_attrib));
	pen=this->getBorderStyle(title_color_attrib);

	if(object->getObjectType()==OBJ_VIEW)
		pen.setStyle(Qt::DashLine);

	box->setPen(pen);

	if(schema->isRectVisible())
		this->resizeTitle(obj_name->boundingRect().width()  + (2 * HORIZ_SPACING),
											obj_name->boundingRect().height() + (2 * VERT_SPACING));
	else
		this->resizeTitle(obj_name->boundingRect().width() + schema_name->boundingRect().width() + (2 * HORIZ_SPACING),
											schema_name->boundingRect().height() + (2 * VERT_SPACING));
}

void TableTitleView::resizeTitle(float width, float height)
{
	QPolygonF pol;
	pol=box->polygon();

	if(pol.isEmpty())
	{
		pol.append(QPointF(0.0f,0.0f));
		pol.append(QPointF(1.0f,0.0f));
		pol.append(QPointF(1.0f,1.0f));
		pol.append(QPointF(0.0f,1.0f));
	}

	this->resizePolygon(pol, width, height);
	box->setPolygon(pol);

	if(schema_name->text()==" ")
		obj_name->setPos((box->boundingRect().width() - obj_name->boundingRect().width())/2.0f, VERT_SPACING);
	else
	{
		schema_name->setPos((box->boundingRect().width() - (schema_name->boundingRect().width() + obj_name->boundingRect().width()))/2.0f, VERT_SPACING);
		obj_name->setPos(schema_name->pos().x() + schema_name->boundingRect().width(), VERT_SPACING);
		obj_name->setPos(schema_name->pos().x() + schema_name->boundingRect().width(), VERT_SPACING);
	}

	this->bounding_rect.setTopLeft(this->pos());
	this->bounding_rect.setSize(QSizeF(box->boundingRect().width(), box->boundingRect().height()));
}

