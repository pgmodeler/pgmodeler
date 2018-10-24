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

#include "basegraphicobject.h"

BaseGraphicObject::BaseGraphicObject(void)
{
	is_modified=true;
	is_faded_out=false;
	attributes[ParsersAttributes::X_POS]=QString();
	attributes[ParsersAttributes::Y_POS]=QString();
	attributes[ParsersAttributes::POSITION]=QString();
	attributes[ParsersAttributes::FADED_OUT]=QString();
	receiver_object=nullptr;
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

bool BaseGraphicObject::isModified(void)
{
	return(is_modified);
}

bool BaseGraphicObject::isFadedOut(void)
{
	return(is_faded_out);
}

void BaseGraphicObject::setFadedOutAttribute(void)
{
	attributes[ParsersAttributes::FADED_OUT]=(is_faded_out ? ParsersAttributes::_TRUE_ : QString());
}

void BaseGraphicObject::setPositionAttribute(void)
{
	attributes[ParsersAttributes::X_POS]=QString("%1").arg(position.x());
	attributes[ParsersAttributes::Y_POS]=QString("%1").arg(position.y());
	attributes[ParsersAttributes::POSITION]=schparser.getCodeDefinition(ParsersAttributes::POSITION, attributes, SchemaParser::XmlDefinition);
}

void  BaseGraphicObject::setPosition(QPointF pos)
{
	setCodeInvalidated(position != pos);
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

bool BaseGraphicObject::isGraphicObject(ObjectType type)
{
	return(type==ObjectType::ObjTable || type==ObjectType::ObjView || type==ObjectType::ObjRelationship ||
				 type==ObjectType::ObjBaseRelationship || type==ObjectType::ObjTextbox || type==ObjectType::ObjSchema);
}
