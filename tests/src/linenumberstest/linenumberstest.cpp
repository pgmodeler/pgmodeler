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
#include "numberedtexteditor.h"
#include <QHBoxLayout>
#include <QDialog>

class LineNumbersTest: public QObject {
  private:
    Q_OBJECT

  private slots:
		void handleLineSelectionUsingMouse();
};

void LineNumbersTest::handleLineSelectionUsingMouse()
{
  QDialog *dlg=new QDialog;
	NumberedTextEditor *edt=new NumberedTextEditor(dlg);
  QHBoxLayout *layout=new QHBoxLayout(dlg);

	LineNumbersWidget::setColors(Qt::darkGray, QColor(230,230,230));
  layout->addWidget(edt);
	dlg->setMinimumSize(640,220);

	edt->setPlainText("--\n\
-- PostgreSQL database dump\n\
--\n\
-- Dumped from database version 9.4.4\n\
-- Dumped by pg_dump version 9.4.4\n\
-- Started on 2015-07-20 19:24:52 WEST\n\
\n\
SET statement_timeout = 0;\n\
SET lock_timeout = 0;\n\
SET client_encoding = 'UTF8';\n\
SET standard_conforming_strings = on;\n\
SET check_function_bodies = false;\n\
SET client_min_messages = warning;\n\
--\n\
-- PostgreSQL database dump\n\
--\n\
-- Dumped from database version 9.4.4\n\
-- Dumped by pg_dump version 9.4.4\n\
-- Started on 2015-07-20 19:24:52 WEST\n\
SET statement_timeout = 0;\n\
SET lock_timeout = 0;\n\
SET client_encoding = 'UTF8';\n\
SET standard_conforming_strings = on;\n\
SET check_function_bodies = false;\n\
SET client_min_messages = warning;\n\
-- Dumped from database version 9.4.4\n\
-- Dumped by pg_dump version 9.4.4\n\
-- Started on 2015-07-20 19:24:52 WEST\n\
SET statement_timeout = 0;\n\
SET lock_timeout = 0;\n\
SET client_encoding = 'UTF8';\n\
SET standard_conforming_strings = on;\n\
SET check_function_bodies = false;\n\
SET client_min_messages = warning;\n\
-- Dumped from database version 9.4.4\n\
-- Dumped by pg_dump version 9.4.4\n\
-- Started on 2015-07-20 19:24:52 WEST\n\
SET statement_timeout = 0;\n\
SET lock_timeout = 0;\n\
SET client_encoding = 'UTF8';\n\
SET standard_conforming_strings = on;\n\
SET check_function_bodies = false;\n\
SET client_min_messages = warning;");

  dlg->exec();
}

QTEST_MAIN(LineNumbersTest)
#include "linenumberstest.moc"
