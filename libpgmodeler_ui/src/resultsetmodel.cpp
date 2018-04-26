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

#include "resultsetmodel.h"

ResultSetModel::ResultSetModel(ResultSet &res, Catalog &catalog, QObject *parent) : QAbstractTableModel(parent)
{
	try
	{
		Catalog aux_cat = catalog;
		vector<unsigned> type_ids;
		vector<unsigned>::iterator end;
		vector<attribs_map> types;
		map<int, QString> type_names;
		int col = 0;

		col_count = res.getColumnCount();
		row_count = res.getTupleCount();
		insertColumns(0, col_count);
		insertRows(0, row_count);

		for(col=0; col < col_count; col++)
		{
			header_data.push_back(res.getColumnName(col));
			type_ids.push_back(res.getColumnTypeId(col));
		}

		if(res.accessTuple(ResultSet::FIRST_TUPLE))
		{
			do
			{
				//Fills the current row with the values of current tuple
				for(int col=0; col < col_count; col++)
				{
					if(res.isColumnBinaryFormat(col))
						item_data.push_back(trUtf8("[binary data]"));
					else
						item_data.push_back(res.getColumnValue(col));
				}
			}
			while(res.accessTuple(ResultSet::NEXT_TUPLE));
		}

		aux_cat.setFilter(Catalog::LIST_ALL_OBJS);
		std::sort(type_ids.begin(), type_ids.end());
		end=std::unique(type_ids.begin(), type_ids.end());
		type_ids.erase(end, type_ids.end());

		types = aux_cat.getObjectsAttributes(OBJ_TYPE, QString(), QString(), type_ids);
		col = 0;

		for(auto &tp : types)
			type_names[tp[ParsersAttributes::OID].toInt()]=tp[ParsersAttributes::NAME];

		for(col=0; col < col_count; col++)
			tooltip_data.push_back(type_names[res.getColumnTypeId(col)]);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

int ResultSetModel::rowCount(const QModelIndex &) const
{
	return(row_count);
}

int ResultSetModel::columnCount(const QModelIndex &) const
{
	return(col_count);
}

QModelIndex ResultSetModel::index(int row, int column, const QModelIndex &parent) const
{
	return(QAbstractTableModel::index(row, column, parent));
}

QModelIndex ResultSetModel::parent(const QModelIndex &) const
{
	return(QModelIndex());
}

QVariant ResultSetModel::data(const QModelIndex &index, int role) const
{
	if(index.row() < row_count && index.column() < col_count)
	{
		if(role == Qt::DisplayRole)
			return(item_data.at(index.row() * col_count + index.column()));

		if(role == Qt::TextAlignmentRole)
			return(QVariant(Qt::AlignLeft | Qt::AlignVCenter));
	}

	return(QVariant(QVariant::Invalid));
}

QVariant ResultSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal)
	{
		if(section >= col_count)
			return(QVariant(QVariant::Invalid));

		if(role == Qt::DisplayRole)
			return(header_data.at(section));

		if(role == Qt::ToolTipRole)
			return(tooltip_data.at(section));

		if(role == Qt::TextAlignmentRole)
			return(QVariant(Qt::AlignLeft | Qt::AlignVCenter));
	}

	return(QAbstractTableModel::headerData(section, orientation, role));
}

Qt::ItemFlags ResultSetModel::flags(const QModelIndex &) const
{
	return(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled );
}

