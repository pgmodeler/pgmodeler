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

#include "crashhandler.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char **argv)
{
	try
	{
		QApplication app(argc,argv);
		QTranslator translator;
		QPrintDialog print_dlg;

		//Loads the ui translation for crashhandler
		translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);
		app.installTranslator(&translator);

		CrashHandler crashhandler;

		if(argc > 1)
			crashhandler.loadReport(argv[1]);

		crashhandler.show();
		app.exec();

		return(0);
	}
	catch(Exception &e)
	{
		return(e.getErrorType());
	}
}
