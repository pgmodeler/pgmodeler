/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libparsers
\class CsvDocument
\brief This class implements basic operations to store a parsed CSV document in memory.
*/

#ifndef CSV_DOCUMENT_H
#define CSV_DOCUMENT_H

#include "exception.h"
#include <QStringList>

class CsvDocument {
	private:
		QStringList columns;

		QList<QStringList> values;

		void addValues(const QStringList &values);

		void setValue(int row, int col, const QString &value);

	public:
		CsvDocument();

		int getRowCount();

		int getColumnCount();

		bool isEmpty();

		QStringList getColumnNames();

		QString getValue(int row, const QString &col_name);

		QString getValue(int col, int row);

		friend class CsvParser;
};

#endif
