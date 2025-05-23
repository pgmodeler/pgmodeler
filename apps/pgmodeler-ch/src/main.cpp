/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "crashhandlerform.h"
#include "application.h"
#include <QTranslator>
#include "guiutilsns.h"

int main(int argc, char **argv)
{
	try
	{
		GlobalAttributes::init(argv[0], true);
		Application app(argc,argv);
		QStringList args = app.arguments();
		app.loadTranslations(QLocale::system().name(), false);

		CrashHandlerForm crashhandler(args.size() > 1 && args[1]==CrashHandlerForm::AnalysisMode);
		GuiUtilsNs::resizeDialog(&crashhandler);
		crashhandler.show();
		app.exec();

		return 0;
	}
	catch(Exception &e)
	{
		QTextStream out(stdout);
		out << e.getExceptionsText();
		return enum_t(e.getErrorCode());
	}
}
