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
#include "globalattributes.h"
#include "pgmodelerunittest.h"
#include "defaultlanguages.h"
#include "qtcompat/qtextstreamcompat.h"

class ProcedureTest: public QObject, public PgModelerUnitTest {
	private:
		Q_OBJECT

	public:
		ProcedureTest() : PgModelerUnitTest(SCHEMASDIR) {}

	private slots:
		void throwsErrorOnInvalidParameter();
		void generatesNameAndSignatureCorrectly();
		void generatesSQLCorrectly();
		void generatesXMLCorrectly();
		void modelReturnsProcedureDepsRefs();
		void modelCreatesProcedureFromXML();
};

void ProcedureTest::throwsErrorOnInvalidParameter()
{
	try
	{
		QFAIL("No error related to invalid type raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgInvalidTypeObject);
	}
}

void ProcedureTest::generatesNameAndSignatureCorrectly()
{
	QFAIL("Not implemented!");
}

void ProcedureTest::generatesSQLCorrectly()
{
	QFAIL("Not implemented!");
}

void ProcedureTest::generatesXMLCorrectly()
{
	QFAIL("Not implemented!");
}

void ProcedureTest::modelReturnsProcedureDepsRefs()
{
	QFAIL("Not implemented!");
}

void ProcedureTest::modelCreatesProcedureFromXML()
{
	QFAIL("Not implemented!");
}

QTEST_MAIN(ProcedureTest)
#include "proceduretest.moc"
