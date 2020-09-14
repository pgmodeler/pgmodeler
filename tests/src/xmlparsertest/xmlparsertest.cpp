/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "xmlparser.h"

class XmlParserTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void correctlyConvertJsonValsToXmlEntites();
};

void XmlParserTest::correctlyConvertJsonValsToXmlEntites()
{
	QString value = "value=\"'{\"attr\": { \"\" }}'::json\"		value-abc=\"true\"     value-cde=\"'{\"sign_aspect\": { \"message_no\": 0, \"message_multi\": \"\" }}'::json\"\n",
			expected = "value=\"'{&quot;attr&quot;: { &quot;&quot; }}'::json\"		value-abc=\"true\"     value-cde=\"'{&quot;sign_aspect&quot;: { &quot;message_no&quot;: 0, &quot;message_multi&quot;: &quot;&quot; }}'::json\"\n",
			result;

	try
	{
		result = XmlParser::convertCharsToXMLEntities(value);
		QCOMPARE(result, expected);
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

QTEST_MAIN(XmlParserTest)
#include "xmlparsertest.moc"
