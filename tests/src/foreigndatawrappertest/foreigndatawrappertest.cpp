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

class ForeignDataWrapperTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void assignValidFunctionDoesntRaiseException(void);
		void assignInvalidFunctionRaisesException(void);
		void codeGeneratedIsWellFormed(void);
		void modelReturnsFDWasFunctionsReferences(void);
};

void ForeignDataWrapperTest::assignValidFunctionDoesntRaiseException(void)
{
	ForeignDataWrapper fdw;
	Function func_handler, func_validator;

	fdw.setName("fdw");

	try
	{
		func_handler.setName("func_handler");
		func_handler.setReturnType(PgSqlType("fdw_handler"));
		fdw.setHandlerFunction(&func_handler);
	}
	catch(Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}

	try
	{
		func_validator.setName("func_validator");
		func_validator.addParameter(Parameter("param1", PgSqlType("text", 1)));
		func_validator.addParameter(Parameter("param2", PgSqlType("oid")));
		fdw.setValidatorFunction(&func_validator);
	}
	catch(Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

void ForeignDataWrapperTest::assignInvalidFunctionRaisesException(void)
{
	ForeignDataWrapper fdw;
	Function func_handler, func_validator;

	fdw.setName("fdw");
	func_handler.setName("func_handler");
	func_validator.setName("func_validator");

	try
	{
		func_handler.setReturnType(PgSqlType("fdw_handler"));
		func_handler.addParameter(Parameter("param1", PgSqlType("integer")));
		fdw.setHandlerFunction(&func_handler);
		QFAIL("Expected exception(s) not raised!");
	}
	catch(Exception &e)
	{
		QVERIFY(e.getErrorCode() == ErrorCode::AsgFunctionInvalidParamCount);
	}

	try
	{
		func_handler.removeParameters();
		func_handler.setReturnType(PgSqlType("trigger"));
		fdw.setHandlerFunction(&func_handler);
		QFAIL("Expected exception(s) not raised!");
	}
	catch(Exception &e)
	{
		QVERIFY(e.getErrorCode() == ErrorCode::AsgFunctionInvalidReturnType);
	}

	try
	{
		func_validator.addParameter(Parameter("param1", PgSqlType("integer")));
		func_validator.addParameter(Parameter("param2", PgSqlType("varchar")));
		fdw.setValidatorFunction(&func_validator);
		QFAIL("Expected exception(s) not raised!");
	}
	catch(Exception &e)
	{
		QVERIFY(e.getErrorCode() == ErrorCode::AsgFunctionInvalidParameters);
	}

	try
	{
		func_validator.removeParameters();
		func_validator.addParameter(Parameter("param1", PgSqlType("text")));
		func_validator.addParameter(Parameter("param2", PgSqlType("oid")));
		func_validator.addParameter(Parameter("param3", PgSqlType("smallint")));
		fdw.setValidatorFunction(&func_validator);
		QFAIL("Expected exception(s) not raised!");
	}
	catch(Exception &e)
	{
		QVERIFY(e.getErrorCode() == ErrorCode::AsgFunctionInvalidParamCount);
	}
}

void ForeignDataWrapperTest::codeGeneratedIsWellFormed(void)
{
	ForeignDataWrapper fdw;
	Role owner;
	Schema public_sch;
	Function func_handler, func_validator;
	QString sql_code =QString(
"-- object: fdw | type: FOREIGN DATA WRAPPER -- \
-- DROP FOREIGN DATA WRAPPER IF EXISTS fdw CASCADE; \
CREATE FOREIGN DATA WRAPPER fdw \
HANDLER public.func_handler() \
VALIDATOR public.func_validator(text[],oid) \
OPTIONS (opt1 'value1',opt2 'value2'); \
-- ddl-end -- \
COMMENT ON FOREIGN DATA WRAPPER fdw IS 'This is a test comment on FDW'; \
-- ddl-end -- \
ALTER FOREIGN DATA WRAPPER fdw OWNER TO postgres; \
-- ddl-end -- ").simplified();

	QString xml_code =QString(
"<foreigndatawrapper name=\"fdw\" options=\"opt1*value1*opt2*value2\"> \
<role name=\"postgres\"/> \
<comment><![CDATA[This is a test comment on FDW]]></comment> \
<function ref-type=\"handler\" signature=\"public.func_handler()\"/> \
<function ref-type=\"validator\" signature=\"public.func_validator(text[],oid)\"/> \
</foreigndatawrapper>").replace("*", ForeignDataWrapper::OptionSeparator).simplified();

	try
	{
		public_sch.setName("public");
		owner.setName("postgres");

		fdw.setName("fdw");
		fdw.setOwner(&owner);
		func_handler.setName("func_handler");
		func_handler.setReturnType(PgSqlType("fdw_handler"));
		func_handler.setSchema(&public_sch);
		fdw.setHandlerFunction(&func_handler);

		func_validator.setName("func_validator");
		func_validator.addParameter(Parameter("param1", PgSqlType("text", 1)));
		func_validator.addParameter(Parameter("param2", PgSqlType("oid")));
		func_validator.setSchema(&public_sch);
		fdw.setValidatorFunction(&func_validator);

		fdw.setOption("opt1", "value1");
		fdw.setOption("opt2", "value2");
		fdw.setComment("This is a test comment on FDW");

		QString res_sql_code = fdw.getCodeDefinition(SchemaParser::SqlDefinition).simplified();
		QCOMPARE(sql_code, res_sql_code);

		QString res_xml_code = fdw.getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

void ForeignDataWrapperTest::modelReturnsFDWasFunctionsReferences(void)
{
	try
	{
		DatabaseModel model;
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

QTEST_MAIN(ForeignDataWrapperTest)
#include "foreigndatawrappertest.moc"
