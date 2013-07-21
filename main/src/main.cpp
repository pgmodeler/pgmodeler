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

/* Fusion is the default widget style for pgModeler. User can change this by calling
the executable using -style option. This same style is applied to crashhandler. */
QString app_style="Fusion";

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

		#ifdef Q_OS_MAC
				cmd="startapp pgmodeler-ch -style " + app_style;
		#else
				cmd="pgmodeler-ch -style " + app_style;
		#endif
	#else
		cmd="pgmodeler-ch.exe -style " + app_style;
	#endif

	//Creates the stacktrace file
	output.setFileName(GlobalAttributes::TEMPORARY_DIR +
										 GlobalAttributes::DIR_SEPARATOR +
										 GlobalAttributes::STACKTRACE_FILE);
	output.open(QFile::WriteOnly);

	if(output.isOpen())
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

		QStringList params;
		int idx=0, idx1;
		bool enable_stylesheet=true;

		for(int param=0; param < argc; param++)
			params.push_back(argv[param]);

		//Checking if the user want to disable stylesheets using param: -no-stylesheet
		idx1=params.indexOf(QRegExp("-no-stylesheet", Qt::CaseSensitive, QRegExp::FixedString));

		//Cheking if the user want to use a different theme from default Fusion
		idx=params.indexOf(QRegExp("-style", Qt::CaseSensitive, QRegExp::FixedString));

		//Getting the theme name
		if(idx>=0 && idx < params.size() - 1)
		{
			app_style=params[idx+1];
			params.erase(params.begin() + idx);
			params.erase(params.begin() + idx);
		}

		//Disabling the stylesheet
		if(idx1>=0)
		{
			params.erase(params.begin() + idx1);
			enable_stylesheet=false;
		}

		//Install a signal handler to start crashhandler when SIGSEGV or SIGABRT is emitted
		signal(SIGSEGV, startCrashHandler);
		signal(SIGABRT, startCrashHandler);

		argc=1;
		Application app(argc,argv);
		QTranslator translator;
		QFile ui_style(GlobalAttributes::CONFIGURATIONS_DIR +
									 GlobalAttributes::DIR_SEPARATOR +
									 GlobalAttributes::UI_STYLE_CONF +
									 GlobalAttributes::CONFIGURATION_EXT);
		QString stylesheet;
		QFileInfo fi(argv[0]);

		//Apply the style to application
		QApplication::setStyle(QStyleFactory::create(app_style));

		//Changing the current working dir to the executable's directory in
		QDir::setCurrent(fi.absolutePath());

		//Adding paths which executable will find plugins and it's dependecies
		app.addLibraryPath(fi.absolutePath());
		app.addLibraryPath(GlobalAttributes::PLUGINS_DIR);

		//Tries to load the ui translation according to the system's locale
		translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);

		//Installs the translator on the application
		app.installTranslator(&translator);

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

		splash.show();
		splash.repaint();

		//Creates the main form
		MainWindow fmain;

		//Indicating that the splash screen must be closed when the main window is shown
		splash.finish(&fmain);

		if(enable_stylesheet)
		{
			//Loading app style sheet
			ui_style.open(QFile::ReadOnly);

			//Raises an error if ui style is not found
			if(!ui_style.isOpen())
			{
				Messagebox msg;
				msg.show(Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(ui_style.fileName()),
													 ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__));
			}
			else
				stylesheet=ui_style.readAll();

			app.setStyleSheet(stylesheet);
		}

		fmain.showMaximized();

		//If the user specifies a list of files to be loaded
		params.pop_front();
		if(!params.isEmpty())
			fmain.loadModels(params);

		app.exec();
		return(0);
	}
	catch(Exception &e)
	{
		return(e.getErrorType());
	}
}
