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

CsvParser::CsvParser()
{
	setSpecialChars(CsvDocument::Separator,
						 CsvDocument::TextDelimiter,
						 CsvDocument::LineBreak);
	cols_in_first_row = false;
	curr_pos = curr_row = 0;
}

void CsvParser::setSpecialChars(const QChar &sep, const QChar &txt_delim, const QChar &ln_break)
{
	separator = sep;
	text_delim = txt_delim;
	line_break = ln_break;
}

void CsvParser::setColumnInFirstRow(bool value)
{
	cols_in_first_row = value;
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

	try
	{
		buffer = csv_buf;
		curr_pos = curr_row = 0;

		CsvDocument csv_doc(separator, text_delim, line_break);

		// Extracting the column names if the option to do so is set
		if(curr_row == 0 && cols_in_first_row)
			csv_doc.setColumns(extractRow());

		// Extracting all the document rows while there's chars to be read
		while(curr_pos < buffer.length())
			csv_doc.addRow(extractRow());

		return csv_doc;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString CsvParser::extractValue()
{
	bool delim_open = false,
			delim_closed = false;
	QString value;
	QChar chr;
	int delim_cnt = 0;

	while(curr_pos < buffer.length())
	{
		chr = buffer.at(curr_pos);

		/* If we find a starting text delimiter and we still didn't
		 * open a text delimiter */
		if(chr == text_delim && !delim_open)
		{
			/* Indicates that we are inside a quoted value: "value"
			 * From this point,.the parser will interpret separators and line breaks
			 * as part of the value being extracted until the closing delimiter is found */
			delim_open = true;
			curr_pos++;
		}
		// If we find the a text delimiter char and we are extracting a quoted value
		else if(chr == text_delim && delim_open)
		{
			// We increment the amount of contigous delimiters found until now
			delim_cnt++;
			curr_pos++;
		}
		else
		{
			if(delim_open && delim_cnt > 0)
			{
				/* If the amount of contigous delimiters are not even we consider that
				 * the last text delimiter character was closing the quoted value,
				 * so we flag the closing of the quoted value */
				if(delim_cnt % 2 != 0)
					delim_closed = true;

				/* If the number of contigous delimiters is even we need to insert delimiters
				 * in the value being extract by half number of contigous delimiters, since
				 * according to CSV RFC, to represent a text delimiter inside a quoted values
				 * we must write two double quotes (e.g. "foo""bar" is translated to foo"bar) */
				value = value.leftJustified(value.size() + (delim_cnt / 2), text_delim);
				delim_cnt = 0;
			}

			/* If we find a separator or line break character and
			 * we are not inside a quoted value or we finished extracting
			 * a quoted value we need to return the whole extracted value */
			if((chr == separator || chr == line_break) &&
				 (!delim_open || (delim_open && delim_closed)))
			{
				curr_pos++;

				/* If the character is line break we need to indicate to the parser
				 * the the next line will be parsed */
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

QStringList CsvParser::extractRow()
{
	QStringList values;
	int last_row = curr_row;

	while(curr_pos < buffer.length() && last_row == curr_row)
		values.append(extractValue());

	return values;
}
