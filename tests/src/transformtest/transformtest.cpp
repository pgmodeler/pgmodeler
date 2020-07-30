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

class TransformTest: public QObject, public PgModelerUnitTest {
	private:
		Q_OBJECT

	public:
		TransformTest() : PgModelerUnitTest(SCHEMASDIR) {}

	private slots:
		void throwsErrorOnInvalidType();
		void throwsErrorOnInvalidLanguage();
		void throwsErrorOnInvalidFunctionParamCount();
		void throwsErrorOnInvalidFunctionParamType();
		void throwsErrorOnInvalidFunctionReturnType();
		void generatesNameAndSignatureCorrectly();
		//void generatesSQLCorrectly();
		//void generatesXMLCorrectly();
};

void TransformTest::throwsErrorOnInvalidType()
{
	try
	{
		Transform transf;
		transf.setType(PgSqlType(""));
		QFAIL("No error related to invalid type raised!");
	}
	catch(Exception &e)
	{
			QCOMPARE(e.getErrorCode(), ErrorCode::AsgInvalidTypeObject);
	}
}

void TransformTest::throwsErrorOnInvalidLanguage()
{
	try
	{
		Transform transf;
		transf.setLanguage(nullptr);
		QFAIL("No error related to invalid language raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgNotAllocatedLanguage);
	}
}

void TransformTest::throwsErrorOnInvalidFunctionParamCount()
{
	Transform transf;
	Function func;

	try
	{
		func.addParameter(Parameter("p1", PgSqlType("internal")));
		func.addParameter(Parameter("p2", PgSqlType("internal")));
		transf.setFunction(&func, Transform::ToSqlFunc);
		QFAIL("No error related to invalid function (param count) raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgFunctionInvalidParamCount);
	}
}

void TransformTest::throwsErrorOnInvalidFunctionParamType()
{
	Transform transf;
	Function func;

	try
	{
		func.addParameter(Parameter("p1", PgSqlType("varchar")));
		transf.setType(PgSqlType("varchar"));
		transf.setFunction(&func, Transform::ToSqlFunc);
		QFAIL("No error related to invalid function (param type) raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgFunctionInvalidParameters);
	}

	try
	{
		func.removeParameters();
		func.addParameter(Parameter("p1", PgSqlType("bigint")));
		transf.setFunction(&func, Transform::FromSqlFunc);
		QFAIL("No error related to invalid function (param type) raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgFunctionInvalidParameters);
	}
}

void TransformTest::throwsErrorOnInvalidFunctionReturnType()
{
	Transform transf;
	Function func;

	try
	{
		transf.setType(PgSqlType("varchar"));
		func.addParameter(Parameter("p1", PgSqlType("internal")));
		func.setReturnType(PgSqlType("bigint"));
		transf.setFunction(&func, Transform::ToSqlFunc);

		QFAIL("No error related to invalid function (return type) raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgFunctionInvalidReturnType);
	}

	try
	{
		func.removeParameters();
		func.addParameter(Parameter("p1", PgSqlType("internal")));
		func.setReturnType(PgSqlType("bigint"));
		transf.setType(PgSqlType("varchar"));
		transf.setFunction(&func, Transform::FromSqlFunc);

		QFAIL("No error related to invalid function (return type) raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::AsgFunctionInvalidReturnType);
	}
}

void TransformTest::generatesNameAndSignatureCorrectly()
{
	Transform transf;
	Function to_sql_func, from_sql_func;
	Language lang;

	try
	{
		QCOMPARE(transf.getName(), "smallint_undefined");
		QCOMPARE(transf.getSignature(), "FOR smallint LANGUAGE undefined");

		lang.setName(DefaultLanguages::PlPgsql);
		transf.setLanguage(&lang);

		transf.setType(PgSqlType("varchar"));
		to_sql_func.addParameter(Parameter("p1", PgSqlType("internal")));
		to_sql_func.setReturnType(PgSqlType("varchar"));
		transf.setFunction(&to_sql_func, Transform::ToSqlFunc);

		from_sql_func.addParameter(Parameter("p1", PgSqlType("internal")));
		from_sql_func.setReturnType(PgSqlType("internal"));
		transf.setFunction(&from_sql_func, Transform::FromSqlFunc);

		QCOMPARE(transf.getName(), "varchar_plpgsql");
		QCOMPARE(transf.getSignature(), "FOR varchar LANGUAGE plpgsql");

		transf.setFunction(nullptr, Transform::ToSqlFunc);
		to_sql_func.setReturnType(PgSqlType::parseString("timestamp with time zone"));
		transf.setType(PgSqlType::parseString("timestamp with time zone"));
		transf.setFunction(&to_sql_func, Transform::ToSqlFunc);

		QCOMPARE(transf.getName(), "timestamp_with_time_zone_plpgsql");
		QCOMPARE(transf.getSignature(), "FOR timestamp with time zone LANGUAGE plpgsql");
	}
	catch(Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

QTEST_MAIN(TransformTest)
#include "transformtest.moc"
