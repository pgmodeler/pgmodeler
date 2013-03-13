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
#include <QTranslator>

#ifndef Q_OS_WIN
#include "execinfo.h"
#endif

void startCrashHandler(int signal)
{
	ofstream output;
	QString lin, cmd;

	/** At the moment the backtrace function does not exists on MingW (Windows) this way
		 the code that generates the stacktrace is available only on Linux/Unix systems */
	#ifndef Q_OS_WIN
		void *stack[20];
		size_t stack_size, i;
		char **symbols=NULL;

		stack_size = backtrace(stack, 20);
		symbols = backtrace_symbols(stack, stack_size);

		#ifdef Q_OS_MAC
			cmd="crashhandler.app";
		#else
			cmd="crashhandler";
		#endif
	#else
	cmd="crashhandler.exe";
	#endif

	//Creates the stacktrace file
	output.open((GlobalAttributes::TEMPORARY_DIR +
							 GlobalAttributes::DIR_SEPARATOR +
							 GlobalAttributes::STACKTRACE_FILE).toStdString().c_str());

	if(output.is_open())
	{
		lin=QString("** pgModeler [v%1] crashed after receive signal: %2 **\n\nDate/Time:%3\n")
				.arg(GlobalAttributes::PGMODELER_VERSION)
				.arg(signal)
				.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

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

	//Executes the crashhandler command (which must be on the same directory as the pgModeler executable)
	cmd=QApplication::applicationDirPath() + GlobalAttributes::DIR_SEPARATOR + cmd;

	//Mac OSX little fix: configure the correct path to call crashhandler.app
	#ifdef Q_OS_MAC
		cmd.replace("pgmodeler.app/Contents/MacOS/","");
		cmd=QString("open ") + cmd;
	#endif

	system(cmd.toStdString().c_str());
	exit(1);
}

int main(int argc, char **argv)
{
	try
	{
		//Install a signal handler to start crashhandler when SIGSEGV or SIGABRT is emitted
		signal(SIGSEGV, startCrashHandler);
		signal(SIGABRT, startCrashHandler);

		Application app(argc,argv);
		QTranslator translator;

		app.addLibraryPath(GlobalAttributes::PLUGINS_DIR);

		//Tries to load the ui translation according to the system's locale
		translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);

		//Installs the translator on the application
		app.installTranslator(&translator);

		//Loading the application splash screen
		QPixmap pixmap(":imagens/imagens/pgmodeler_logo.png");
		QPixmap alfa(":imagens/imagens/pgmodeler_logo_alfa.png");
		pixmap.setAlphaChannel(alfa);

		//Draws the current version code on the splash
		QFont fnt;
		QPainter p;
		fnt.setBold(true);

		QFontMetrics fm(fnt);
		QString str_ver=QString("v%1").arg(GlobalAttributes::PGMODELER_VERSION);
		QRect ret=fm.boundingRect(str_ver);

		p.begin(&pixmap);
		p.setFont(fnt);
		p.setPen(QColor(255,255,255));
		p.drawText(QPointF((pixmap.size().width()*0.55f)-(ret.width()/2),
											 pixmap.size().width()-17), str_ver);
		p.end();

		QSplashScreen splash(pixmap);
		splash.show();
		splash.repaint();

		//Creates the main form
		MainWindow fmain;

		//Indicating that the splash screen must be closed when the main window is shown
		splash.finish(&fmain);

		fmain.showMaximized();
		app.exec();

		return(0);
	}
	catch(Exception &e)
	{
		return(e.getErrorType());
	}
}
