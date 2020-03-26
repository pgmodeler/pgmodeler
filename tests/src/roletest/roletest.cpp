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
#include "pgmodelerns.h"
#include "role.h"

class RoleTest: public QObject {
  private:
    Q_OBJECT

  private slots:
    void alterCommandEndsWithSemiColon();
};

void RoleTest::alterCommandEndsWithSemiColon()
{
  Role role1, role2;
  QString alter_cmd;

  role1.setName("role1");
  role1.setOption(Role::OpSuperuser, true);

  role2.setName("role2");
  role2.setOption(Role::OpCreateDb, false);
  role2.setOption(Role::OpCreateRole, false);
  role2.setOption(Role::OpLogin, false);

  alter_cmd=role1.getAlterDefinition(&role2);
  alter_cmd.remove(QString("\n%1\n").arg(Attributes::DdlEndToken));
  QCOMPARE(alter_cmd.endsWith(";"), true);
}

QTEST_MAIN(RoleTest)
#include "roletest.moc"
