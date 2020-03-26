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

#include <QTranslator>
#include "pgmodelercli.h"

int main(int argc, char **argv)
{
	QTextStream out(stdout);

#ifdef DEMO_VERSION
	out << endl;
	out << QString("pgModeler ") << GlobalAttributes::PgModelerVersion << QT_TR_NOOP(" command line interface.") << endl;
	out << QT_TR_NOOP("PostgreSQL Database Modeler Project - pgmodeler.io") << endl;
	out << QT_TR_NOOP("Copyright 2006-2019 Raphael A. Silva <raphael@pgmodeler.io>") << endl;
	out << QT_TR_NOOP("\n** CLI disabled in demonstration version! **") << endl << endl;
#else
	try
	{
        PgModelerCliApp pgmodeler_cli(argc, argv);
        QTranslator translator(&pgmodeler_cli);

		//Tries to load the ui translation according to the system's locale
		translator.load(QLocale::system().name(), GlobalAttributes::getLanguagesDir());

		//Installs the translator on the application
		pgmodeler_cli.installTranslator(&translator);

		//Executes the cli
		return pgmodeler_cli.exec();
	}
	catch(Exception &e)
	{
		out << endl;
		out << e.getExceptionsText();
		out << QString("** pgmodeler-cli aborted due to critical error(s). **") << endl << endl;
		return (e.getErrorCode()==ErrorCode::Custom ? -1 : enum_cast(e.getErrorCode()));
	}
#endif
}
