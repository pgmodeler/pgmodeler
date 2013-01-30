#include "basegraphicobject.h"

BaseGraphicObject::BaseGraphicObject(void)
{
	is_modified=true;
	attributes[ParsersAttributes::X_POS]="";
	attributes[ParsersAttributes::Y_POS]="";
	attributes[ParsersAttributes::POSITION]="";
	receiver_object=NULL;
}

void BaseGraphicObject::setProtected(bool value)
{
	is_protected=value;

	if(!this->signalsBlocked())
		emit s_objectProtected(value);
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
		disconnect(this, NULL, receiver_object, NULL);

	receiver_object=obj;
}

QObject *BaseGraphicObject::getReceiverObject(void)
{
	return(receiver_object);
}
