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

#include "objectsdiffinfo.h"

const unsigned ObjectsDiffInfo::CREATE_OBJECT=0;
const unsigned ObjectsDiffInfo::DROP_OBJECT=1;
const unsigned ObjectsDiffInfo::ALTER_OBJECT=2;
const unsigned ObjectsDiffInfo::NO_DIFFERENCE=3;

ObjectsDiffInfo::ObjectsDiffInfo(void)
{
  object=old_object=nullptr;
	diff_type=NO_DIFFERENCE;
}

ObjectsDiffInfo::ObjectsDiffInfo(unsigned diff_type, BaseObject *object, BaseObject *new_object)
{
	this->diff_type=diff_type;
	this->object=object;
  this->old_object=new_object;
}

unsigned ObjectsDiffInfo::getDiffType(void)
{
	return(diff_type);
}

QString ObjectsDiffInfo::getInfoMessage(void)
{
  QString msg=QT_TR_NOOP("%1 `%2' `(%3)' [id: %4]"), obj_name;
  //TableObject *tab_obj=dynamic_cast<TableObject *>(object);
  ObjectType obj_type=object->getObjectType();

  /*if(tab_obj)
	 obj_name=tab_obj->getParentTable()->getName(true) + "." + tab_obj->getName(true);
	else
   obj_name=object->getName(true);*/

  /* Forcing the usage of BaseObject::getSignature for the following object,
     since the custom getSignature for those types return some undesired
     SQL keywords for this context */
  if(obj_type==OBJ_CONSTRAINT || obj_type==OBJ_TRIGGER || obj_type==OBJ_RULE)
    obj_name=dynamic_cast<TableObject *>(object)->TableObject::getSignature();
  else if(obj_type==OBJ_OPCLASS || obj_type==OBJ_OPFAMILY)
    obj_name=object->BaseObject::getSignature();
  else
    obj_name=object->getSignature();

	if(diff_type==DROP_OBJECT)
	{
    return(msg.arg("<font color=\"#e00000\"><strong>DROP</strong></font>")
              .arg(obj_name)
              .arg(object->getTypeName())
              .arg(object->getObjectId()));
	}
	else if(diff_type==CREATE_OBJECT)
	{
    return(msg.arg("<font color=\"#008000\"><strong>CREATE</strong></font>")
              .arg(obj_name)
              .arg(object->getTypeName())
              .arg(object->getObjectId()));
	}
	else if(diff_type==ALTER_OBJECT)
	{
    return(msg.arg("<font color=\"#ff8000\"><strong>ALTER</strong></font>")
              .arg(obj_name)
              .arg(object->getTypeName())
              .arg(object->getObjectId()));

	}

	return("");
}

BaseObject *ObjectsDiffInfo::getObject(void)
{
  return(object);
}

BaseObject *ObjectsDiffInfo::getOldObject(void)
{
  return(old_object);
}
