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

#include <QtTest/QtTest>
#include "csvparser.h"

class CsvParserTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void testOneRowBufferWithQuotesInValues();
		void testOneRowBufferWithoutLineBreak();
		void testTwoBufferWithSimpleAndQuotedValues();
		void testOneRowBufferWithSimpleAndQuotedValues();
};

void CsvParserTest::testOneRowBufferWithSimpleAndQuotedValues()
{
	try
	{
		CsvParser csvparser;
		CsvDocument csvdoc;
		QString buffer;

		buffer = "value 1;value 2;\"value 3\"\n";
		csvdoc = csvparser.parseBuffer(buffer);

		QCOMPARE(csvdoc.getColumnCount(), 3);
		QCOMPARE(csvdoc.getRowCount(), 1);

		QCOMPARE(csvdoc.getValue(0, 0), "value 1");
		QCOMPARE(csvdoc.getValue(0, 1), "value 2");
		QCOMPARE(csvdoc.getValue(0, 2), "value 3");
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void CsvParserTest::testTwoBufferWithSimpleAndQuotedValues()
{
	try
	{
		CsvParser csvparser;
		CsvDocument csvdoc;
		QString buffer;

		buffer = "value 1;value 2;\"value 3\"\n";
		buffer += "\"value 4\";\"value 5\";\"value 6\"\n";
		csvdoc = csvparser.parseBuffer(buffer);

		QCOMPARE(csvdoc.getColumnCount(), 3);
		QCOMPARE(csvdoc.getRowCount(), 2);

		QCOMPARE(csvdoc.getValue(0, 0), "value 1");
		QCOMPARE(csvdoc.getValue(0, 1), "value 2");
		QCOMPARE(csvdoc.getValue(0, 2), "value 3");

		QCOMPARE(csvdoc.getValue(1, 0), "value 4");
		QCOMPARE(csvdoc.getValue(1, 1), "value 5");
		QCOMPARE(csvdoc.getValue(1, 2), "value 6");
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void CsvParserTest::testOneRowBufferWithoutLineBreak()
{
	try
	{
		CsvParser csvparser;
		CsvDocument csvdoc;
		QString buffer;

		buffer = "value 1;value 2;\"value 3\"";
		csvdoc = csvparser.parseBuffer(buffer);

		QCOMPARE(csvdoc.getColumnCount(), 3);
		QCOMPARE(csvdoc.getRowCount(), 1);

		QCOMPARE(csvdoc.getValue(0, 0), "value 1");
		QCOMPARE(csvdoc.getValue(0, 1), "value 2");
		QCOMPARE(csvdoc.getValue(0, 2), "value 3");
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void CsvParserTest::testOneRowBufferWithQuotesInValues()
{
	try
	{
		CsvParser csvparser;
		CsvDocument csvdoc;
		QString buffer;

		buffer = "\"value \"\"1\"\";\"value\"\"2\";\"value 3\"\n";
		csvdoc = csvparser.parseBuffer(buffer);

		QCOMPARE(csvdoc.getColumnCount(), 3);
		QCOMPARE(csvdoc.getRowCount(), 1);

		QCOMPARE(csvdoc.getValue(0, 0), "value \"1\"");
		QCOMPARE(csvdoc.getValue(0, 1), "value\"2");
		QCOMPARE(csvdoc.getValue(0, 2), "value 3");
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

QTEST_MAIN(CsvParserTest)
#include "csvparsertest.moc"
