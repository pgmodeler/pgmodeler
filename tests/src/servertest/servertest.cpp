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

class ServerTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void codeGeneratedIsWellFormed(void);
		void modelReturnsDepsAndRefsForServer(void);
		void modelCreatesServerfromXMLandResultingXMLisEqual(void);
};

void ServerTest::codeGeneratedIsWellFormed(void)
{
	Server server;
	Role owner;
	QString sql_code =QString(
"-- object: server_test | type: SERVER -- \
-- DROP SERVER IF EXISTS fdw CASCADE; \
CREATE SERVER server_test \
TYPE 'dbtype' \
VERSION '1.0' \
FOREIGN DATA WRAPPER fdw \
OPTIONS (opt1 'value1',opt2 'value2'); \
-- ddl-end -- \
COMMENT ON SERVER server_test IS 'This is a test comment on server'; \
-- ddl-end -- \
ALTER SERVER server OWNER TO postgres; \
-- ddl-end -- ").simplified();

	QString xml_code =QString(
"<foreigndatawrapper name=\"fdw\" options=\"opt1#value1*opt2#value2\"> \
<role name=\"postgres\"/> \
<comment><![CDATA[This is a test comment on FDW]]></comment> \
<function ref-type=\"handler\" signature=\"public.func_handler()\"/> \
<function ref-type=\"validator\" signature=\"public.func_validator(text[],oid)\"/> \
</foreigndatawrapper>").replace("#", Server::OptionValueSeparator)
											 .replace("*", Server::OptionsSeparator).simplified();

	try
	{
		owner.setName("postgres");

		server.setName("server_test");
		server.setOwner(&owner);

		server.setOption("opt1", "value1");
		server.setOption("opt2", "value2");
		server.setComment("This is a test comment on server");

		QString res_sql_code = server.getCodeDefinition(SchemaParser::SqlDefinition).simplified();
		QCOMPARE(sql_code, res_sql_code);

		QString res_xml_code = server.getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

void ServerTest::modelReturnsDepsAndRefsForServer(void)
{
	/*DatabaseModel model;
	Role owner;
	Schema public_sch;
	Language lang;
	Function func_handler, func_validator;
	ForeignDataWrapper fdw;

	try
	{
		public_sch.setName("public");
		owner.setName("postgres");

		model.addSchema(&public_sch);
		model.addRole(&owner);

		fdw.setName("fdw");
		fdw.setOwner(&owner);
		func_handler.setName("func_handler");
		func_handler.setReturnType(PgSqlType("fdw_handler"));
		func_handler.setSchema(&public_sch);
		func_handler.setSourceCode("foo");
		func_handler.setOwner(&owner);
		func_handler.setLanguage(&lang);
		fdw.setHandlerFunction(&func_handler);

		func_validator.setName("func_validator");
		func_validator.addParameter(Parameter("param1", PgSqlType("text", 1)));
		func_validator.addParameter(Parameter("param2", PgSqlType("oid")));
		func_validator.setSchema(&public_sch);
		func_validator.setSourceCode("foo");
		func_validator.setOwner(&owner);
		func_validator.setLanguage(&lang);
		fdw.setValidatorFunction(&func_validator);

		fdw.setOption("opt1", "value1");
		fdw.setOption("opt2", "value2");
		fdw.setComment("This is a test comment on FDW");

		model.addFunction(&func_handler);
		model.addFunction(&func_validator);
		model.addForeignDataWrapper(&fdw);

		vector<BaseObject *> deps;
		model.getObjectDependecies(&fdw, deps);

		QVERIFY(deps.size() >= 3);

		deps.clear();
		model.getObjectReferences(&func_validator, deps);
		QVERIFY(deps.size() == 1);

		deps.clear();
		model.getObjectReferences(&func_handler, deps);
		QVERIFY(deps.size() == 1);

		model.removeForeignDataWrapper(&fdw);
		model.removeFunction(&func_handler);
		model.removeFunction(&func_validator);
		model.removeLanguage(&lang);
		model.removeSchema(&public_sch);
		model.removeRole(&owner);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	} */
}

void ServerTest::modelCreatesServerfromXMLandResultingXMLisEqual(void)
{
	/*DatabaseModel model;
	Role owner;
	Schema public_sch;
	Language lang;
	Function func_handler, func_validator;
	ForeignDataWrapper *fdw = nullptr;
	QString xml_code, res_xml_code;

	try
	{
		public_sch.setName("public");
		owner.setName("postgres");

		model.addSchema(&public_sch);
		model.addRole(&owner);

		func_handler.setName("func_handler");
		func_handler.setReturnType(PgSqlType("fdw_handler"));
		func_handler.setSchema(&public_sch);
		func_handler.setSourceCode("foo");
		func_handler.setOwner(&owner);
		func_handler.setLanguage(&lang);

		func_validator.setName("func_validator");
		func_validator.addParameter(Parameter("param1", PgSqlType("text", 1)));
		func_validator.addParameter(Parameter("param2", PgSqlType("oid")));
		func_validator.setSchema(&public_sch);
		func_validator.setSourceCode("foo");
		func_validator.setOwner(&owner);
		func_validator.setLanguage(&lang);

		model.addFunction(&func_handler);
		model.addFunction(&func_validator);

		xml_code=QString("<foreigndatawrapper name=\"fdw\" options=\"opt1#value1*opt2#value2\"> \
<role name=\"postgres\"/> \
<comment><![CDATA[This is a test comment on FDW]]></comment> \
<function ref-type=\"handler\" signature=\"public.func_handler()\"/> \
<function ref-type=\"validator\" signature=\"public.func_validator(text[],oid)\"/> \
</foreigndatawrapper>").replace("#", ForeignDataWrapper::OptionValueSeparator)
										.replace("*", ForeignDataWrapper::OptionsSeparator);

		model.getXMLParser()->loadXMLBuffer(xml_code);
		fdw = dynamic_cast<ForeignDataWrapper *>(model.createObject(ObjectType::ForeignDataWrapper));

		model.removeForeignDataWrapper(fdw);
		model.removeFunction(&func_handler);
		model.removeFunction(&func_validator);
		model.removeLanguage(&lang);
		model.removeSchema(&public_sch);
		model.removeRole(&owner);

		QVERIFY(fdw != nullptr);

		res_xml_code = fdw->getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		xml_code = xml_code.simplified();

		if(fdw)
			delete(fdw);

		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	} */
}

QTEST_MAIN(ServerTest)
#include "servertest.moc"
