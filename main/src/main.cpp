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

#include "pgmodelerapp.h"
#include "mainwindow.h"

#ifndef Q_OS_WIN
#include "execinfo.h"
#endif

void startCrashHandler(int signal)
{
	QFile output;
	QString lin, cmd;

	/** At the moment the backtrace function does not exists on MingW (Windows) this way
	 the code that generates the stacktrace is available only on Linux/Unix systems */
#ifndef Q_OS_WIN
	void *stack[30];
	size_t stack_size;
	char **symbols=nullptr;
	stack_size = backtrace(stack, 30);
	symbols = backtrace_symbols(stack, stack_size);
#endif

	cmd=QString("\"%1\"").arg(GlobalAttributes::getPgModelerCHandlerPath()) + QString(" -style ") + GlobalAttributes::DefaultQtStyle;

	//Creates the stacktrace file
	output.setFileName(GlobalAttributes::getTemporaryFilePath(GlobalAttributes::StacktraceFile));
	output.open(QFile::WriteOnly);

	if(output.isOpen())
	{
		lin=QString("** pgModeler crashed after receive signal: %1 **\n\nDate/Time: %2 \nVersion: %3 \nBuild: %4 \n")
			.arg(signal)
			.arg(QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss")))
			.arg(GlobalAttributes::PgModelerVersion)
			.arg(GlobalAttributes::PgModelerBuildNumber);

		lin+=QString("Compilation Qt version: %1\nRunning Qt version: %2\n\n")
			 .arg(QT_VERSION_STR)
			 .arg(qVersion());

		output.write(lin.toStdString().c_str(), lin.size());

#ifndef Q_OS_WIN
		for(size_t i=0; i < stack_size; i++)
		{
			lin=QString("[%1] ").arg(stack_size-1-i) + QString(symbols[i]) + QString("\n");
			output.write(lin.toStdString().c_str(), lin.size());
		}
		free(symbols);
#else
		lin=QString("** Stack trace unavailable on Windows system **");
		output.write(lin.toStdString().c_str(), lin.size());
#endif

		output.close();
	}

	/* Changing the working dir to the main executable in order to call the crash handler
  if the PGMODELER_CHANDLER_PATH isn't set */
	QDir dir;
	dir.cd(QApplication::applicationDirPath());

	exit(1 + system(cmd.toStdString().c_str()));
}

int main(int argc, char **argv)
{
	try
	{
		/* Registering the below classes as metatypes in order to make
		 * them liable to be sent through signal parameters. */
		qRegisterMetaType<ObjectType>("ObjectType");
		qRegisterMetaType<Exception>("Exception");
		qRegisterMetaType<ValidationInfo>("ValidationInfo");
		qRegisterMetaType<ObjectsDiffInfo>("ObjectsDiffInfo");

		//Install a signal handler to start crashhandler when SIGSEGV or SIGABRT is emitted
		signal(SIGSEGV, startCrashHandler);
		signal(SIGABRT, startCrashHandler);

		//Checking if the user specified another widget style using the -style param
		bool using_style=false;

		for(int i=0; i < argc && !using_style; i++)
			using_style=QString(argv[i]).contains("-style");

		PgModelerApp::setAttribute(Qt::AA_UseHighDpiPixmaps);

		//High DPI suport via application attributes is available only from Qt 5.6.0
		#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
			Application::setAttribute(Qt::AA_EnableHighDpiScaling);
		#endif

		PgModelerApp app(argc,argv);
		int res=0;

		//If no custom style is specified we force the usage of Fusion (the default for Qt and pgModeler)
		if(!using_style)
			app.setStyle(GlobalAttributes::DefaultQtStyle);

		//Loading the application splash screen
		QSplashScreen splash;
		QPixmap pix(QPixmap(QString(":imagens/imagens/pgmodeler_splash.png")));
		splash.setPixmap(pix);
		splash.setMask(pix.mask());
		splash.show();
		app.processEvents();

		//Creates the main form
		MainWindow fmain;

		fmain.show();
		splash.finish(&fmain);

		//Loading models via command line on MacOSX are disabled until the file association work correclty on that system
#ifndef Q_OS_MAC
		QStringList params=app.arguments();
		params.pop_front();

		//If the user specifies a list of files to be loaded
		if(!params.isEmpty())
			fmain.loadModels(params);
#endif

		res = app.exec();
		app.closeAllWindows();

		return res;
	}
	catch(Exception &e)
	{
		QTextStream ts(stdout);
		ts << e.getExceptionsText();
		return enum_cast(e.getErrorCode());
	}
}
