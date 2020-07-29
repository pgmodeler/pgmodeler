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

class TransformTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void throwsErrorOnInvalidType();
		void throwsErrorOnInvalidFunction();
		void throwsErrorOnInvalidLanguage();
		//void generatesNameCorrectly();
};

void TransformTest::throwsErrorOnInvalidType()
{
	try
	{
		Transform transf;

		transf.setType(PgSqlType::Null);
		QFAIL("No error related to invalid type raised!");
	}
	catch(Exception &e)
	{
			QCOMPARE(e.getErrorCode(), ErrorCode::AsgInvalidTypeObject);
	}
}

void TransformTest::throwsErrorOnInvalidFunction()
{
	try
	{
		QFAIL("No error related to invalid function raised!");
	}
	catch(Exception &e)
	{

	}
}

void TransformTest::throwsErrorOnInvalidLanguage()
{
	try
	{
		QFAIL("No error related to invalid language raised!");
	}
	catch(Exception &e)
	{

	}
}

QTEST_MAIN(TransformTest)
#include "transformtest.moc"
