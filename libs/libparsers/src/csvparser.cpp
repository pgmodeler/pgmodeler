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
#include <QTextStream>

CsvParser::CsvParser()
{
	setOptions(';', '"', QChar::LineFeed, false);
	curr_pos = curr_row = 0;
}

void CsvParser::setOptions(const QChar &sep, const QChar &txt_delim, const QChar &ln_break, bool cols_fst_row)
{
	//if(sep == txt_delim)
	// throw error?

	separator = sep;
	text_delim = txt_delim;
	line_break = ln_break;
	cols_in_first_row = cols_fst_row;
}

CsvDocument CsvParser::parseFile(const QString &filename)
{
	try
	{
		return parseBuffer(UtilsNs::loadFile(filename));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

CsvDocument CsvParser::parseBuffer(const QString &csv_buf)
{
	if(csv_buf.isEmpty())
		return CsvDocument();

	buffer = csv_buf;
	curr_pos = 0;

	CsvDocument csv_doc;
	QStringList values;
	QTextStream out(stdout);

	while(curr_pos < buffer.length())
		csv_doc.addValues(extractValues());

	return csv_doc;
}

QString CsvParser::extractValue()
{
	bool delim_open = false,
			delim_closed = false;
	QString value, dbl_delim;
	QChar chr;

	while(curr_pos < buffer.length())
	{
		chr = buffer.at(curr_pos);

		if(chr == text_delim && !delim_open)
		{
			delim_open = true;
			curr_pos++;
		}
		else if(chr == text_delim && delim_open)
		{
			dbl_delim.append(text_delim);
			curr_pos++;
		}
		else
		{
			if(delim_open && !dbl_delim.isEmpty())
			{
				if(dbl_delim.length() == 1)
				{
					dbl_delim.clear();
					delim_closed = true;
				}
				else
				{
					value.append(text_delim);
					dbl_delim.clear();
				}
			}
			else if((!delim_open ||
							 (delim_open && delim_closed)) &&
							(chr == separator || chr == line_break))
			{
				curr_pos++;

				if(chr == line_break)
					curr_row++;

				return value;
			}
			else
			{
				value.append(chr);
				curr_pos++;
			}
		}
	}

	return value;
}

QStringList CsvParser::extractValues()
{
	QStringList values;
	int last_row = curr_row;

	while(curr_pos < buffer.length() && last_row == curr_row)
		values.append(extractValue());

	return values;
}
