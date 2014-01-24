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

#include "mainwindow.h"
#include "application.h"

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
		void *stack[20];
		size_t stack_size, i;
		char **symbols=nullptr;
		stack_size = backtrace(stack, 20);
		symbols = backtrace_symbols(stack, stack_size);
	#endif

	#ifdef Q_OS_MAC
		cmd=QApplication::applicationDirPath() + GlobalAttributes::DIR_SEPARATOR +
				GlobalAttributes::CRASH_HANDLER_PATH + " -style " + GlobalAttributes::DEFAULT_QT_STYLE;
	#else
		cmd=GlobalAttributes::CRASH_HANDLER_PATH + " -style " + GlobalAttributes::DEFAULT_QT_STYLE;
	#endif

	//Creates the stacktrace file
	output.setFileName(GlobalAttributes::TEMPORARY_DIR +
										 GlobalAttributes::DIR_SEPARATOR +
										 GlobalAttributes::STACKTRACE_FILE);
	output.open(QFile::WriteOnly);

	if(output.isOpen())
	{
    lin=QString("** pgModeler crashed after receive signal: %1 **\n\nDate/Time: %2 \nVersion: %3 \nBuild: %4 \n")
        .arg(signal)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(GlobalAttributes::PGMODELER_VERSION)
        .arg(GlobalAttributes::PGMODELER_BUILD_NUMBER);

		lin+=QString("Compilation Qt version: %1\nRunning Qt version: %2\n\n")
				 .arg(QT_VERSION_STR)
				 .arg(qVersion());

		output.write(lin.toStdString().c_str(), lin.size());

		#ifndef Q_OS_WIN
			for(i=0; i < stack_size; i++)
			{
				lin=QString(symbols[i]) + QString("\n");
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
		them liable to be sent through signal parameters. */
		qRegisterMetaType<ObjectType>("ObjectType");
		qRegisterMetaType<Exception>("Exception");
		qRegisterMetaType<ValidationInfo>("ValidationInfo");

		//Install a signal handler to start crashhandler when SIGSEGV or SIGABRT is emitted
		signal(SIGSEGV, startCrashHandler);
		signal(SIGABRT, startCrashHandler);

		Application app(argc,argv);

		//Loading the application splash screen
		QSplashScreen splash;
		QPixmap pix(QPixmap(":imagens/imagens/pgmodeler_splash.png"));
		splash.setPixmap(pix);
		splash.setMask(pix.mask());

		#ifndef Q_OS_MAC
			splash.setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint);
		#else
			splash.setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
		#endif

    #ifdef Q_OS_WIN
      splash.show();
    #else
      splash.showMaximized();
    #endif

		app.processEvents();

		//Creates the main form
		MainWindow fmain;

		//Loading models via command line on MacOSX are disabled until the file association work correclty on that system
		#ifndef Q_OS_MAC
     QStringList params=app.arguments();
     params.pop_front();

		 //If the user specifies a list of files to be loaded
     if(!params.isEmpty())
      fmain.loadModels(params);
		#endif

		splash.finish(&fmain);
		fmain.showMaximized();

		return(app.exec());
	}
	catch(Exception &e)
	{
		QTextStream ts(stdout);
		ts << e.getExceptionsText();
		return(e.getErrorType());
	}
}
