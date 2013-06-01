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

#include "basegraphicobject.h"

BaseGraphicObject::BaseGraphicObject(void)
{
	is_modified=true;
	attributes[ParsersAttributes::X_POS]="";
	attributes[ParsersAttributes::Y_POS]="";
	attributes[ParsersAttributes::POSITION]="";
	receiver_object=nullptr;
}

void BaseGraphicObject::setProtected(bool value)
{
	BaseObject::setProtected(value);

	if(!this->signalsBlocked())
		emit s_objectProtected(this->isProtected());
}

void BaseGraphicObject::setSystemObject(bool value)
{
	BaseObject::setSystemObject(value);

	 if(!this->signalsBlocked())
		 emit s_objectProtected(this->isProtected());
}

void BaseGraphicObject::setModified(bool value)
{
	is_modified=value;

	if(!this->signalsBlocked() && is_modified)
		emit s_objectModified();
}

bool BaseGraphicObject::isModified(void)
{
	return(is_modified);
}

void BaseGraphicObject::setPositionAttribute(void)
{
	attributes[ParsersAttributes::X_POS]=QString("%1").arg(position.x());
	attributes[ParsersAttributes::Y_POS]=QString("%1").arg(position.y());
	attributes[ParsersAttributes::POSITION]=SchemaParser::getCodeDefinition(ParsersAttributes::POSITION,
																																					attributes, SchemaParser::XML_DEFINITION);
}

void  BaseGraphicObject::setPosition(QPointF pos)
{
	position=pos;
}

QPointF BaseGraphicObject::getPosition(void)
{
	return(position);
}

void BaseGraphicObject::operator = (BaseGraphicObject &obj)
{
	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(obj);
	this->position=obj.position;
}

void BaseGraphicObject::setReceiverObject(QObject *obj)
{
	if(!obj && receiver_object)
		disconnect(this, nullptr, receiver_object, nullptr);

	receiver_object=obj;
}

QObject *BaseGraphicObject::getReceiverObject(void)
{
	return(receiver_object);
}
