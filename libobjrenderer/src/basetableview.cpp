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

#include "basetableview.h"

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	body=new QGraphicsPolygonItem;
	title=new TableTitleView;
	this->addToGroup(body);
	this->addToGroup(title);
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	delete(body);
	delete(title);
}

QVariant BaseTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	BaseObjectView::itemChange(change, value);

	if(change==ItemPositionHasChanged)
		emit s_objectMoved();

	return(value);
}
