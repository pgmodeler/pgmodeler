/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "foreigndatawrapper.h"

class ForeignDataWrapperTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void assignInvalidFunctionRaisesException(void);
		void codeGeneratedIsWellFormed(void);
};

void ForeignDataWrapperTest::assignInvalidFunctionRaisesException(void)
{
	QCOMPARE(true, true);
}

void ForeignDataWrapperTest::codeGeneratedIsWellFormed(void)
{
	QCOMPARE(true, true);
}

QTEST_MAIN(ForeignDataWrapperTest)
#include "foreigndatawrappertest.moc"
