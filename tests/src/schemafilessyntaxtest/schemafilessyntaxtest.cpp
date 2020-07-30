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
#include "databasemodel.h"
#include "pgmodelerunittest.h"

class SchemaFilesSyntaxTest: public QObject, public PgModelerUnitTest {
	private:
		Q_OBJECT

	public:
		SchemaFilesSyntaxTest() : PgModelerUnitTest(SCHEMASDIR) {}

	private slots:
		void schemasDontGenerateSyntaxErrors();
};


void SchemaFilesSyntaxTest::schemasDontGenerateSyntaxErrors()
{
	SchemaParser schparser;
	QString schname;
	attribs_map attrs;

	for(auto type : BaseObject::getObjectTypes())
	{
		schname = BaseObject::getSchemaName(type);

		if(schname.isEmpty())
			continue;

		try
		{
			schparser.ignoreEmptyAttributes(true);
			schparser.ignoreUnkownAttributes(true);
			schparser.getCodeDefinition(schname, attrs, SchemaParser::SqlDefinition);

			schparser.ignoreEmptyAttributes(true);
			schparser.ignoreUnkownAttributes(true);
			schparser.getCodeDefinition(schname, attrs, SchemaParser::XmlDefinition);

			schparser.ignoreEmptyAttributes(true);
			schparser.ignoreUnkownAttributes(true);
			schparser.getCodeDefinition(GlobalAttributes::getSchemaFilePath("alter", schname), attrs);
		}
		catch(Exception &e)
		{
			if(e.getErrorCode() == ErrorCode::InvalidSyntax ||
				 e.getErrorCode() == ErrorCode::InvalidInstruction ||
				 e.getErrorCode() == ErrorCode::InvalidMetacharacter ||
				 e.getErrorCode() == ErrorCode::InvalidAttribute ||
				 e.getErrorCode() == ErrorCode::InvalidOperatorInExpression)
			{
				QFAIL(e.getErrorMessage().toStdString().c_str());
			}

			QWARN(e.getErrorMessage().toStdString().c_str());
		}
	}
}

QTEST_MAIN(SchemaFilesSyntaxTest)
#include "schemafilessyntaxtest.moc"
