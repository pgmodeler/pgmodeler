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

/**
\ingroup libgui
\class ObjectsListModel
\brief Implements a model representation for quickly fill a QTableView object with database model objects infow.
*/

#ifndef OBJECTS_LIST_MODEL_H
#define OBJECTS_LIST_MODEL_H

#include "guiglobal.h"
#include <QAbstractTableModel>
#include <QIcon>
#include <QFont>
#include "baseobject.h"

class __libgui ObjectsListModel: public QAbstractTableModel {
 private:
	Q_OBJECT

	struct ItemData {
		QString text, fg_color, bg_color, icon;
		bool italic, strikeout;
		BaseObject *data;
		QSize sz_hint;

		ItemData() {
			clear();
		}

		inline void clear() {
			text = icon = "";
			fg_color = bg_color = "";
			italic = strikeout = false;
			data = nullptr;
			sz_hint.setHeight(0);
			sz_hint.setWidth(0);
		}
	};

	int col_count, row_count;

	QList<ItemData> item_data;

	QList<ItemData> header_data;

	void insertColumn(int, const QModelIndex &){}
	void insertRow(int, const QModelIndex &){}

	inline QVariant getItemData(const ItemData &item_dt, int role) const;
	void fillModel(const std::vector<BaseObject *> &list, const QString &search_attr =  "");

 public:
	enum ColumnId: int {
		ObjName,
		ObjType,
		ObjId,
		ParentName,
		ParentType,
		SearchAttr
	};

	ObjectsListModel(const std::vector<BaseObject *> &list, const QString &search_attr =  "", QObject *parent = nullptr);
	virtual int rowCount(const QModelIndex & = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual QModelIndex parent(const QModelIndex &) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &) const;
	bool isEmpty();
};

#endif
