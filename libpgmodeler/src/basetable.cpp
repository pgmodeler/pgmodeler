/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "basetable.h"

BaseTable::BaseTable(void)
{
 tag=nullptr;
 obj_type=BASE_TABLE;
 attributes[ParsersAttributes::TAG]="";
}

void BaseTable::setTag(Tag *tag)
{
  this->tag=tag;
}

Tag *BaseTable::getTag(void)
{
  return(tag);
}

void BaseTable::operator = (BaseTable &tab)
{
  (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tab);
  this->tag=tab.tag;
}
