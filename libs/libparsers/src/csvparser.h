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
\class CsvParser
\brief This class implements basic operations to handle CSV documents
*/

#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <QString>
#include <QList>

class CsvParser {
	private:
		QList<QStringList> values;

		QStringList columns;

	public:
		CsvParser();

		void parseFile(const QString &filename, const QChar &separator = ';', const QChar &text_delim = '"', bool cols_in_first_row = false);

		void parseBuffer(const QString &csv_buf, const QChar &separator = ';', const QChar &text_delim = '"', bool cols_in_first_row = false);

		const QStringList &getColumnNames();

		const QString &getValue(int col_idx);
};

#endif
