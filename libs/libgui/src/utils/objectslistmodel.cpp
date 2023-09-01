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

#include "objectslistmodel.h"
#include "guiutilsns.h"
#include <pgsqltypes/pgsqltype.h>
#include "objectstablewidget.h"
#include "tableobject.h"
#include "permission.h"
#include "baserelationship.h"

ObjectsListModel::ObjectsListModel(const std::vector<BaseObject *> &list, const QString &search_attr, QObject *parent) : QAbstractTableModel(parent)
{
	try
	{
		header_icons.clear();
		col_count = 6;
		row_count = list.size();

		insertColumns(0, col_count);
		insertRows(0, row_count);

		for(int col=0; col < col_count; col++)
		{
			header_data.push_back("Column" + QString::number(col));
			header_icons.push_back(QIcon());
		}

		/* for(auto &obj : list)
		{
			items_text.append(obj->getName());
			items_text.append(obj->getTypeName());
			items_text.append("object id");
			items_text.append("parent");
			items_text.append("parent type");
			items_text.append(obj->getComment());
		} */

		ItemData item_dt;
		BaseObject *parent_obj;
		attribs_map search_attribs;
		ObjectType obj_type;
		int sub_type = -1;

		for(auto &obj : list)
		{
			sub_type = -1;
			obj_type = obj->getObjectType();

			if(obj_type == ObjectType::BaseRelationship ||
				 obj_type == ObjectType::Relationship)
				sub_type = dynamic_cast<BaseRelationship *>(obj)->getRelationshipType();
			else if(obj_type == ObjectType::Constraint)
				sub_type = dynamic_cast<Constraint *>(obj)->getConstraintType().getTypeId();

			//First column: Object name
			item_dt.text = obj->getName();
			//item_dt.icon = QIcon(GuiUtilsNs::getIconPath(obj_type, sub_type));
			item_dt.icon = GuiUtilsNs::getIconPath(obj_type, sub_type);
			//item_dt.data = QVariant::fromValue<void *>(reinterpret_cast<void *>(obj));
			item_dt.data = obj; //QVariant::fromValue<void *>(reinterpret_cast<void *>(obj));

			if(obj->isProtected() || obj->isSystemObject())
			{
				//item_dt.font.setItalic(true);
				item_dt.italic = true;
				//item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor);
				item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor).name();
			}
			else if(dynamic_cast<TableObject *>(obj) &&
							dynamic_cast<TableObject *>(obj)->isAddedByRelationship())
			{
				//item_dt.font.setItalic(true);
				item_dt.italic = true;
				//item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::RelAddedItemAltFgColor);
				item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::RelAddedItemAltFgColor).name();
			}

			//item_dt.font.setStrikeOut(obj->isSQLDisabled() && !obj->isSystemObject());
			item_dt.strikeout = obj->isSQLDisabled() && !obj->isSystemObject();
			item_data.append(item_dt);

			//Second column: Object type
			item_dt.clear();
			item_dt.text = obj->getTypeName();
			//item_dt.font.setItalic(true);
			item_dt.italic = true;
			item_data.append(item_dt);

			//Third column: Object id
			item_dt.clear();
			item_dt.text = QString::number(obj->getObjectId());
			item_data.append(item_dt);

			//Fourth column: Parent object name
			item_dt.clear();

			if(dynamic_cast<TableObject *>(obj))
				parent_obj = dynamic_cast<TableObject *>(obj)->getParentTable();
			else if(obj->getSchema())
				parent_obj = obj->getSchema();
			else if(dynamic_cast<Permission *>(obj))
				parent_obj = dynamic_cast<Permission *>(obj)->getObject();
			else
				parent_obj = obj->getDatabase();

			item_dt.text = parent_obj ? parent_obj->getName() : "-";
			//item_dt.data = QVariant::fromValue<void *>(reinterpret_cast<void *>(parent_obj));
			item_dt.data = parent_obj;

			if(parent_obj)
			{
				//item_dt.icon = QIcon(GuiUtilsNs::getIconPath(parent_obj->getObjectType()));
				item_dt.icon = GuiUtilsNs::getIconPath(parent_obj->getObjectType());

				if(parent_obj->isProtected() || parent_obj->isSystemObject())
				{
					//item_dt.font.setItalic(true);
					item_dt.italic = true;
					//item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor);
					item_dt.fg_color = ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor).name();
				}
			}

			item_data.append(item_dt);

			//Fifth column: Parent object type
			item_dt.clear();
			item_dt.text = parent_obj ? parent_obj->getTypeName() : "-";
			//item_dt.font.setItalic(true);
			item_dt.italic = true;
			item_data.append(item_dt);

			//Sixth column: object comment or the specified search attribute
			item_dt.clear();
			search_attribs = obj->getSearchAttributes();

			if(search_attr != Attributes::Name &&
				 search_attr != Attributes::Schema &&
				 search_attr != Attributes::Comment)
			{
				item_dt.text = search_attribs[search_attr];
			}
			else
				item_dt.text = obj->getComment();

			item_data.append(item_dt);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectsListModel::~ObjectsListModel()
{
	QTextStream out(stdout);
	out << "ObjectsListModel::~ObjectsListModel()" << Qt::endl;
	item_data.clear();
	header_data.clear();
}

int ObjectsListModel::rowCount(const QModelIndex &) const
{
	return row_count;
}

int ObjectsListModel::columnCount(const QModelIndex &) const
{
	return col_count;
}

QModelIndex ObjectsListModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

QModelIndex ObjectsListModel::parent(const QModelIndex &) const
{
	return QModelIndex();
}

QVariant ObjectsListModel::data(const QModelIndex &index, int role) const
{
	if(index.row() < row_count && index.column() < col_count)
	{
		const ItemData &item_dt = item_data.at(index.row() * col_count + index.column());
		//int idx = index.row() * col_count + index.column();

		if(role == Qt::DisplayRole)
			return item_dt.text;
			//return items_text.at(index.row() * col_count + index.column());
			//return item_text.at(index.row() * col_count + index.column());

		//if(role == Qt::ForegroundRole && item_dt.fg_color != Qt::transparent)
		//	return QColor(item_dt.fg_color);

		//if(role == Qt::BackgroundRole && item_dt.bg_color != Qt::transparent)
		//	return item_dt.bg_color;

		//if(role == Qt::UserRole)
		//	return item_dt.data;

		//if(role == Qt::FontRole)
		//	return item_dt.font;

		if(role == Qt::DecorationRole)
			return QIcon(item_dt.icon);
			//return items_icon.at(idx);

		if(role == Qt::TextAlignmentRole)
			return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
	}

	return QVariant();
}

QVariant ObjectsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal)
	{
		if(section >= col_count)
			return QVariant();

		if(role == Qt::DisplayRole)
			return header_data.at(section);

		if(role == Qt::DecorationRole)
			return header_icons.at(section);

		if(role == Qt::TextAlignmentRole)
			return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags ObjectsListModel::flags(const QModelIndex &) const
{
	return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled );
}

bool ObjectsListModel::isEmpty()
{
	return (row_count <= 0);
}
