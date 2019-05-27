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

/**
\ingroup libpgmodeler_ui
\class ResultSetModel
\brief Implements a model representation of ResultSet class which can be used to show large amount of data in instances of QTableView.
*/

#ifndef RESULT_SET_MODEL_H
#define RESULT_SET_MODEL_H

#include <QAbstractTableModel>
#include "resultset.h"
#include "catalog.h"

class ResultSetModel: public QAbstractTableModel {
	private:
		Q_OBJECT

		int col_count, row_count;
		QStringList item_data, header_data, tooltip_data;

		void insertColumn(int, const QModelIndex &){}
		void insertRow(int, const QModelIndex &){}

	public:
		ResultSetModel(ResultSet &res, Catalog &catalog, QObject *parent = 0);
		virtual int rowCount(const QModelIndex & = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &) const;
		virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
		virtual QModelIndex parent(const QModelIndex &) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		virtual Qt::ItemFlags flags(const QModelIndex &) const;
		void append(ResultSet &res);
		bool isEmpty(void);
};

#endif
