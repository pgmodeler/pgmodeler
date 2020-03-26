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

#include "basegraphicobject.h"

BaseGraphicObject::BaseGraphicObject()
{
	is_modified=true;
	is_faded_out=false;
	attributes[Attributes::XPos]=QString();
	attributes[Attributes::YPos]=QString();
	attributes[Attributes::Position]=QString();
	attributes[Attributes::FadedOut]=QString();
	attributes[Attributes::Layer]=QString();
	attributes[Attributes::ZValue]=QString();
	receiver_object=nullptr;
	layer = 0;	
	z_value=0;
}

void BaseGraphicObject::setProtected(bool value)
{
	BaseObject::setProtected(value);
	emit s_objectProtected(this->isProtected());
}

void BaseGraphicObject::setSystemObject(bool value)
{
	BaseObject::setSystemObject(value);
}

void BaseGraphicObject::setModified(bool value)
{
	is_modified=value;

	if(is_modified)
		emit s_objectModified();
}

void BaseGraphicObject::setSQLDisabled(bool value)
{
	bool curr_val=sql_disabled;

	BaseObject::setSQLDisabled(value);

	if(value != curr_val)
		emit s_objectModified();
}

void BaseGraphicObject::setFadedOut(bool value)
{
	setCodeInvalidated(is_faded_out != value);
	is_faded_out = value;
}

bool BaseGraphicObject::isModified()
{
	return is_modified;
}

bool BaseGraphicObject::isFadedOut()
{
	return is_faded_out;
}

void BaseGraphicObject::setFadedOutAttribute()
{
	attributes[Attributes::FadedOut]=(is_faded_out ? Attributes::True : QString());
}

void BaseGraphicObject::setPositionAttribute()
{
	attributes[Attributes::XPos]=QString("%1").arg(position.x());
	attributes[Attributes::YPos]=QString("%1").arg(position.y());
	attributes[Attributes::Position]=schparser.getCodeDefinition(Attributes::Position, attributes, SchemaParser::XmlDefinition);
}

void  BaseGraphicObject::setPosition(QPointF pos)
{
	setCodeInvalidated(position != pos);
	position=pos;
}

QPointF BaseGraphicObject::getPosition()
{
	return position;
}

void BaseGraphicObject::operator = (BaseGraphicObject &obj)
{
	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(obj);
	this->position=obj.position;
	this->z_value=obj.z_value;
}

void BaseGraphicObject::setReceiverObject(QObject *obj)
{
	if(!obj && receiver_object)
		disconnect(this, nullptr, receiver_object, nullptr);

	receiver_object=obj;
}

QObject *BaseGraphicObject::getOverlyingObject()
{
	return receiver_object;
}

bool BaseGraphicObject::isGraphicObject(ObjectType type)
{
	return (type==ObjectType::Table || type==ObjectType::View || type==ObjectType::Relationship ||
				 type==ObjectType::BaseRelationship || type==ObjectType::Textbox || type==ObjectType::Schema ||
				 type==ObjectType::ForeignTable);
}

void BaseGraphicObject::setLayer(unsigned layer)
{
	setCodeInvalidated(this->layer != layer);
	this->layer = layer;
}

unsigned BaseGraphicObject::getLayer()
{
	return layer;
}

void BaseGraphicObject::setZValue(int z_value)
{
	if(z_value < MinZValue)
		z_value = MinZValue;
	else if(z_value > MaxZValue)
		z_value = MaxZValue;

	this->z_value = z_value;
}

int BaseGraphicObject::getZValue()
{
	return z_value;
}
