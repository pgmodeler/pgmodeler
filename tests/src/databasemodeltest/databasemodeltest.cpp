/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

class DatabaseModelTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void saveObjectsPositioning(void);
		void loadObjectsPositioning(void);
};

void DatabaseModelTest::saveObjectsPositioning(void)
{
	DatabaseModel dbmodel;
	QTextStream out(stdout);
	QString output=QFileInfo(BINDIR).absolutePath() + GlobalAttributes::DIR_SEPARATOR + QString("demo.opf"),
			input=SAMPLESDIR + GlobalAttributes::DIR_SEPARATOR + QString("demo.dbm");

	try
	{
		QDir dir;
		dir.remove(output);
		dbmodel.createSystemObjects(false);
		dbmodel.loadModel(input);
		dbmodel.saveObjectsPositioning(output);
	}
	catch (Exception &e)
	{
		out << e.getExceptionsText() << endl;
	}

	QCOMPARE(QFileInfo(output).exists(), true);
}

void DatabaseModelTest::loadObjectsPositioning(void)
{
	DatabaseModel dbmodel;
	QTextStream out(stdout);
	QString input_opf=QFileInfo(BINDIR).absolutePath() + GlobalAttributes::DIR_SEPARATOR + QString("demo.opf"),
			input_dbm=SAMPLESDIR + GlobalAttributes::DIR_SEPARATOR + QString("demo.dbm");

	try
	{
		dbmodel.createSystemObjects(false);
		dbmodel.loadModel(input_dbm);
		dbmodel.loadObjectsPositioning(input_opf);
		QCOMPARE(true, true);
	}
	catch (Exception &e)
	{
		out << e.getExceptionsText() << endl;
		QCOMPARE(false, true);
	}
}

QTEST_MAIN(DatabaseModelTest)
#include "databasemodeltest.moc"
