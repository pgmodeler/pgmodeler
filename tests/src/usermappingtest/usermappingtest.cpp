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

	QCOMPARE(usr_mapping.getName(), QString("public@") );

	usr_mapping.setForeignServer(&server);
	QCOMPARE(usr_mapping.getName(), QString("public@server_test") );

	usr_mapping.setOwner(&role);
	QCOMPARE(usr_mapping.getName(), QString("postgres@server_test") );

	usr_mapping.setOwner(nullptr);
	usr_mapping.setForeignServer(nullptr);
	QCOMPARE(usr_mapping.getName(), QString("public@") );
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
"<usermapping name=\"postgres@server_test\" options=\"opt1#value1*opt2#value2\"> \
<role name=\"postgres\"/> \
<foreignserver name=\"server_test\"/> \
</usermapping>")
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
		usr_mapping.setOwner(&role);
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
	DatabaseModel model;
	ForeignDataWrapper fdw;
	ForeignServer server;
	Role role;
	UserMapping usr_mapping;

	try
	{
		role.setName("postgres");
		fdw.setName("fdw");

		server.setName("server_test");
		server.setOwner(&role);
		server.setForeignDataWrapper(&fdw);
		usr_mapping.setForeignServer(&server);
		usr_mapping.setOwner(&role);
		usr_mapping.setOption("opt1", "value1");
		usr_mapping.setOption("opt2", "value2");

		model.addRole(&role);
		model.addForeignServer(&server);
		model.addUserMapping(&usr_mapping);

		vector<BaseObject *> refs, deps;
		model.getObjectDependecies(&usr_mapping, deps);

		model.getObjectReferences(&server, refs);
		model.removeUserMapping(&usr_mapping);
		model.removeForeignServer(&server);
		model.removeForeignDataWrapper(&fdw);
		model.removeRole(&role);

		QVERIFY(deps.size() >= 2);
		QVERIFY(refs.size() == 1);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

void UserMappingTest::modelCreatesUserMappingfromXMLandResultingXMLisEqual(void)
{
	DatabaseModel model;
	ForeignDataWrapper fdw;
	ForeignServer server;
	Role role;
	UserMapping *usr_map = nullptr;
	QString xml_code, res_xml_code;

	try
	{
		role.setName("postgres");
		fdw.setName("fdw");

		server.setName("server_test");
		server.setOwner(&role);
		server.setForeignDataWrapper(&fdw);

		model.addRole(&role);
		model.addForeignServer(&server);
		model.addForeignDataWrapper(&fdw);

		xml_code=QString("<usermapping name=\"postgres@server_test\" options=\"opt1#value1*opt2#value2\"> \
<role name=\"postgres\"/> \
<foreignserver name=\"server_test\"/> \
</usermapping>").replace("#", ForeignObject::OptionValueSeparator)
								.replace("*", ForeignObject::OptionsSeparator);

		model.getXMLParser()->loadXMLBuffer(xml_code);
		usr_map = dynamic_cast<UserMapping *>(model.createObject(ObjectType::UserMapping));

		QVERIFY(usr_map != nullptr);

		res_xml_code = usr_map->getCodeDefinition(SchemaParser::XmlDefinition).simplified();
		xml_code = xml_code.simplified();

		model.removeUserMapping(usr_map);
		model.removeForeignServer(&server);
		model.removeForeignDataWrapper(&fdw);
		model.removeRole(&role);

		if(usr_map)
			delete(usr_map);

		QCOMPARE(xml_code, res_xml_code);
	}
	catch (Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

QTEST_MAIN(UserMappingTest)
#include "usermappingtest.moc"
