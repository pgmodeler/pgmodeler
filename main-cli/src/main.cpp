/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

	try
	{
		PgModelerCLI pgmodeler_cli(argc, argv);
		QTranslator translator;

		//Tries to load the ui translation according to the system's locale
		translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);

		//Installs the translator on the application
		pgmodeler_cli.installTranslator(&translator);

		//Executes the cli
		return(pgmodeler_cli.exec());
	}
	catch(Exception &e)
	{
		out << endl;
		out << e.getExceptionsText();
		out << "** pgmodeler-cli aborted due to critical error(s). **" << endl << endl;
		return(e.getErrorType()==ERR_CUSTOM ? -1 : e.getErrorType());
	}
}
