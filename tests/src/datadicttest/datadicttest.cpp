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
#include "databasemodel.h"

class DataDictTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void generateASimpleDataDictForATable(void);
};

void DataDictTest::generateASimpleDataDictForATable(void)
{	
	DatabaseModel dbmodel;
	Table table;
	Schema *schema = nullptr;
	Column *col = nullptr;
	Constraint *constr = nullptr;

	try
	{
		dbmodel.createSystemObjects(true);
		schema = dbmodel.getSchema("public");

		table.setName("table_test");
		table.setSchema(schema);

		col = new Column;
		col->setName("col_1");
		col->setType(PgSqlType("integer"));
		table.addColumn(col);

		col = new Column;
		col->setName("col_2");
		col->setType(PgSqlType("varchar", 1, 30));
		col->setNotNull(true);
		table.addColumn(col);

		col = new Column;
		col->setName("col_3");
		col->setType(PgSqlType("text"));
		col->setNotNull(true);
		table.addColumn(col);

		col = new Column;
		col->setName("col_4");
		col->setType(PgSqlType::parseString("timestamp without time zone"));
		table.addColumn(col);

		constr = new Constraint;
		constr->setName("table_test_pk");
		constr->setConstraintType(ConstraintType::PrimaryKey);
		constr->addColumn(table.getColumn("col_1"), Constraint::SourceCols);
		table.addConstraint(constr);

		constr = new Constraint;
		constr->setName("table_test_uq");
		constr->setConstraintType(ConstraintType::Unique);
		constr->addColumn(table.getColumn("col_2"), Constraint::SourceCols);
		constr->addColumn(table.getColumn("col_3"), Constraint::SourceCols);
		table.addConstraint(constr);

		dbmodel.addTable(&table);

		QTextStream out(stdout);
		out << dbmodel.getDataDictionary(false) << endl;
		dbmodel.removeTable(&table);
	}
	catch (Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

QTEST_MAIN(DataDictTest)
#include "datadicttest.moc"
