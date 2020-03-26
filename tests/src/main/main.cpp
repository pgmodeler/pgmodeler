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

#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

int main(int, char **argv)
{
  QFileInfo fi;
  QString test_dir=QString("%1/%2").arg(BINDIR).arg("tests");
  QDir dir(test_dir);
  QStringList tests=dir.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::Executable);
  int result=0;

  //Removing the runtests from the list of available tests
  fi.setFile(QString(argv[0]));
  tests.removeOne(fi.fileName());  

  //Iterates over the list of tests retrieved from BINDIR/tests and run each of them
  for(auto &test : tests)
  {
    result=QProcess::execute(test_dir + QString("/") + test);

    if(result == -2)
      qDebug() << "** Could not start test executable:" << test;
    else if(result == -1)
      qDebug() << "** The test " << test << " crashed when running.";

    if(result!=0) break;
  }

	return result;
}
