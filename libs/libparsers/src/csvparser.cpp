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

#include "csvparser.h"
#include "utilsns.h"
#include "exception.h"

CsvParser::CsvParser()
{

}

void CsvParser::parseFile(const QString &filename, const QChar &separator, const QChar &text_delim, bool cols_in_first_row)
{
	try
	{
		parseBuffer(UtilsNs::loadFile(filename), separator, text_delim, cols_in_first_row);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void CsvParser::parseBuffer(const QString &csv_buf, const QChar &separator, const QChar &text_delim, bool cols_in_first_row)
{
	columns.clear();
	values.clear();

	if(csv_buf.isEmpty())
		return;

}

const QStringList &CsvParser::getColumnNames()
{
	return columns;
}

const QString &CsvParser::getValue(int col_idx)
{

}
