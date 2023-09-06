/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include <QCryptographicHash>

BaseTable::BaseTable()
{
	tag=nullptr;
	obj_type=ObjectType::BaseTable;
	attributes[Attributes::Tag]="";
	attributes[Attributes::MaxObjCount]="";
	attributes[Attributes::CollapseMode]="";
	attributes[Attributes::Pagination]="";
	attributes[Attributes::AttribsPage]="";
	attributes[Attributes::ExtAttribsPage]="";
	attributes[Attributes::ZValue]="";
	pagination_enabled = false;
	collapse_mode = CollapseMode::NotCollapsed;
	resetCurrentPages();
}

void BaseTable::setCodeInvalidated(bool value)
{
	BaseGraphicObject::setCodeInvalidated(value);
	hash_code = "";
}

void BaseTable::resetCurrentPages()
{
	curr_page[AttribsSection] = 0;
	curr_page[ExtAttribsSection] = 0;
}

void BaseTable::setTag(Tag *tag)
{
	setCodeInvalidated(this->tag != tag);
	this->tag=tag;
}

Tag *BaseTable::getTag()
{
	return tag;
}

bool BaseTable::isBaseTable(ObjectType obj_tp)
{
	return (obj_tp == ObjectType::Table ||
				 obj_tp == ObjectType::ForeignTable ||
				 obj_tp == ObjectType::View);
}

QString BaseTable::getAlterCode(BaseObject *object)
{
	try
	{
		return BaseObject::getAlterCode(object);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void BaseTable::operator = (BaseTable &tab)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tab);
	this->tag=tab.tag;
}

BaseTable::CollapseMode BaseTable::getCollapseMode()
{
	return collapse_mode;
}

void BaseTable::setPaginationEnabled(bool value)
{
	setCodeInvalidated(pagination_enabled != value);
	pagination_enabled = value;

	if(!pagination_enabled)
		resetCurrentPages();
}

bool BaseTable::isPaginationEnabled()
{
	return pagination_enabled;
}

void BaseTable::setCurrentPage(TableSection section_id, unsigned value)
{
	if(section_id > ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(curr_page[section_id] != value);
	curr_page[section_id] = value;
}

unsigned BaseTable::getCurrentPage(TableSection section_id)
{
	if(section_id > ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return curr_page[section_id];
}

void BaseTable::generateHashCode()
{
	if(!isModified())
		return;

	QString buf;
	QStringList tab_obj_names;
	QCryptographicHash hash_gen(QCryptographicHash::Md5);

	buf.append(obj_name);

	for(auto &obj : getObjects())
		buf.append(obj->getName());

	buf.append(tag ? tag->getName() : "");
	buf.append(schema ? schema->getName() : "");
	buf.append(QString::number(collapse_mode));
	buf.append(QString::number(static_cast<int>(pagination_enabled)));
	buf.append(QString::number(curr_page[0]));
	buf.append(QString::number(curr_page[1]));
	buf.append(QString::number(getPosition().x()));
	buf.append(QString::number(getPosition().y()));

	hash_gen.addData(buf.toUtf8());
	hash_code = hash_gen.result().toHex();
}

void BaseTable::setCollapseMode(CollapseMode coll_mode)
{
	setCodeInvalidated(collapse_mode != coll_mode);
	collapse_mode = coll_mode;
}

void BaseTable::setZValue(int z_value)
{
	setCodeInvalidated(this->z_value != z_value);
	BaseGraphicObject::setZValue(z_value);
}

QString BaseTable::getHashCode()
{
	return hash_code;
}

void BaseTable::setModified(bool value)
{
	BaseGraphicObject::setModified(value);
	generateHashCode();
}

void BaseTable::setPosition(const QPointF &pos)
{
	BaseGraphicObject::setPosition(pos);
	hash_code = "";
}
