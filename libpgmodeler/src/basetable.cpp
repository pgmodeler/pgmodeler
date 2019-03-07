/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
	obj_type=ObjectType::BaseTable;
	attributes[Attributes::Tag]=QString();
	attributes[Attributes::MaxObjCount]=QString();
	attributes[Attributes::CollapseMode]=QString();
	attributes[Attributes::Pagination]=QString();
	attributes[Attributes::AttribsPage]=QString();
	attributes[Attributes::ExtAttribsPage]=QString();
	pagination_enabled = false;
	collapse_mode = CollapseMode::NotCollapsed;
	resetCurrentPages();
}

void BaseTable::resetCurrentPages(void)
{
	curr_page[AttribsSection] = 0;
	curr_page[ExtAttribsSection] = 0;
}

void BaseTable::setTag(Tag *tag)
{
	setCodeInvalidated(this->tag != tag);
	this->tag=tag;
}

Tag *BaseTable::getTag(void)
{
	return(tag);
}

QString BaseTable::getAlterDefinition(BaseObject *object)
{
	try
	{
		return(BaseObject::getAlterDefinition(object));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void BaseTable::operator = (BaseTable &tab)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tab);
	this->tag=tab.tag;
}

CollapseMode BaseTable::getCollapseMode(void)
{
	return(collapse_mode);
}

void BaseTable::setPaginationEnabled(bool value)
{
	setCodeInvalidated(pagination_enabled != value);
	pagination_enabled = value;

	if(!pagination_enabled)
		resetCurrentPages();
}

bool BaseTable::isPaginationEnabled(void)
{
	return(pagination_enabled);
}

void BaseTable::setCurrentPage(unsigned section_id, unsigned value)
{
	if(section_id > ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(curr_page[section_id] != value);
	curr_page[section_id] = value;
}

unsigned BaseTable::getCurrentPage(unsigned section_id)
{
	if(section_id > ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(curr_page[section_id]);
}

void BaseTable::setCollapseMode(CollapseMode coll_mode)
{
	setCodeInvalidated(collapse_mode != coll_mode);
	collapse_mode = coll_mode;
}
