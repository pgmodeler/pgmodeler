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

ObjectsListModel::ObjectsListModel(const std::vector<BaseObject *> &list, QObject *parent) : QAbstractTableModel(parent)
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

		for(auto &obj : list)
		{
			item_data.push_back(obj->getName());
			item_data.push_back(obj->getTypeName());
			item_data.push_back(QString::number(obj->getObjectId()));
			item_data.push_back("parent name");
			item_data.push_back("parent type");
			item_data.push_back("comment");
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
		if(role == Qt::DisplayRole)
			return (item_data.at(index.row() * col_count + index.column()));

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

		if(role == Qt::ToolTipRole)
			return tooltip_data.at(section);

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
