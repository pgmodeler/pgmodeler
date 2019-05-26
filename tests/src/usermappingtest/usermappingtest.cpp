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

class UserMappingTest: public QObject {
	private:
		Q_OBJECT

	private slots:
		void generatesNameCorrectly(void);
		void codeGeneratedIsWellFormed(void);
		void modelReturnsDepsAndRefsForUserMapping(void);
		void modelCreatesUserMappingfromXMLandResultingXMLisEqual(void);
};

void UserMappingTest::generatesNameCorrectly(void)
{
	ForeignDataWrapper fdw;
	ForeignServer server;
	Role role;
	UserMapping usr_mapping;

	role.setName("postgres");
	fdw.setName("fdw");

	server.setName("server_test");
	server.setOwner(&role);
	server.setForeignDataWrapper(&fdw);

	QCOMPARE(usr_mapping.getName(), "public@");

	usr_mapping.setForeignServer(&server);
	QCOMPARE(usr_mapping.getName(), "public@server_test");

	usr_mapping.setRole(&role);
	QCOMPARE(usr_mapping.getName(), "postgres@server_test");

	usr_mapping.setRole(nullptr);
	usr_mapping.setForeignServer(nullptr);
	QCOMPARE(usr_mapping.getName(), "public@");
}

void UserMappingTest::codeGeneratedIsWellFormed(void)
{
	ForeignDataWrapper fdw;
	ForeignServer server;
	Role role;
	UserMapping usr_mapping;
	QString sql_code =QString(
"-- object: postgres@server_test | type: USER MAPPING -- \
-- DROP USER MAPPING IF EXISTS FOR postgres SERVER server_test; \
CREATE USER MAPPING FOR postgres \
SERVER server_test \
OPTIONS (opt1 'value1',opt2 'value2'); \
-- ddl-end -- ").simplified();

QString xml_code =QString(
"<usermapping name=\"postgres@server_test\" role=\"postgres\" server=\"server_test\" options=\"opt1#value1*opt2#value2\"> </usermapping>")
									.replace("#", ForeignServer::OptionValueSeparator)
									.replace("*", ForeignServer::OptionsSeparator).simplified();

	try
	{
		role.setName("postgres");
		fdw.setName("fdw");

		server.setName("server_test");
		server.setOwner(&role);
		server.setForeignDataWrapper(&fdw);
		usr_mapping.setForeignServer(&server);
		usr_mapping.setRole(&role);
		usr_mapping.setOption("opt1", "value1");
		usr_mapping.setOption("opt2", "value2");

		QString res_sql_code = usr_mapping.getCodeDefinition(SchemaParser::SqlDefinition).simplified();
		QCOMPARE(sql_code, res_sql_code);

		QString res_xml_code = usr_mapping.getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void UserMappingTest::modelReturnsDepsAndRefsForUserMapping(void)
{
	/*DatabaseModel model;
	Role owner;
	Schema public_sch;
	ForeignDataWrapper fdw;
	ForeignServer server;

	try
	{
		public_sch.setName("public");
		owner.setName("postgres");

		model.addSchema(&public_sch);
		model.addRole(&owner);

		fdw.setName("fdw");
		model.addForeignDataWrapper(&fdw);

		server.setName("server_test");
		server.setForeignDataWrapper(&fdw);
		model.addServer(&server);

		vector<BaseObject *> refs, deps;
		model.getObjectDependecies(&server, deps);

		model.getObjectReferences(&fdw, refs);
		model.removeServer(&server);
		model.removeForeignDataWrapper(&fdw);
		model.removeSchema(&public_sch);
		model.removeRole(&owner);

		QVERIFY(deps.size() >= 2);
		QVERIFY(refs.size() == 1);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}*/
}

void UserMappingTest::modelCreatesUserMappingfromXMLandResultingXMLisEqual(void)
{
	/*DatabaseModel model;
	Role owner;
	Schema public_sch;
	ForeignDataWrapper fdw;
	ForeignServer *server = nullptr;
	QString xml_code, res_xml_code;

	try
	{
		public_sch.setName("public");
		owner.setName("postgres");

		model.addSchema(&public_sch);
		model.addRole(&owner);

		fdw.setName("fdw");
		model.addForeignDataWrapper(&fdw);

		xml_code=QString("<server name=\"server_test\" options=\"opt1#value1*opt2#value2\"> \
<role name=\"postgres\"/> \
<comment><![CDATA[This is a test comment on server]]></comment> \
<foreigndatawrapper name=\"fdw\"/> \
</server>").replace("#", ForeignDataWrapper::OptionValueSeparator)
											 .replace("*", ForeignDataWrapper::OptionsSeparator);

		model.getXMLParser()->loadXMLBuffer(xml_code);
		server = dynamic_cast<ForeignServer *>(model.createObject(ObjectType::ForeignServer));

		QVERIFY(server != nullptr);

		res_xml_code = server->getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		xml_code = xml_code.simplified();

		model.removeServer(server);
		model.removeForeignDataWrapper(&fdw);
		model.removeSchema(&public_sch);
		model.removeRole(&owner);

		if(server)
			delete(server);

		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}*/
}

QTEST_MAIN(UserMappingTest)
#include "usermappingtest.moc"
