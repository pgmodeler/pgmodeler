/*
# PostgreSQL Database Modeler (pgModeler)
#
# (c) Copyright 2006-2026 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# DEVELOPMENT, MAINTENANCE AND COMMERCIAL DISTRIBUTION BY:
# Nullptr Labs Software e Tecnologia LTDA <contact@nullptrlabs.io>
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
#include <signal.h>
#include <QSplashScreen>

#ifdef PRIV_CODE_SYMBOLS
	#include "privcoreinit.h"
	#include "privcoreclasses.h"
#endif

#ifndef Q_OS_WIN
	#include "execinfo.h"
#else
	#include <windows.h>
	#include <stdio.h>
#endif

namespace {
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

		cmd=QString("\"%1\"").arg(GlobalAttributes::getPgModelerCHandlerPath()) + " -style " + GlobalAttributes::DefaultQtStyle;

		//Creates the stacktrace file
		output.setFileName(GlobalAttributes::getTemporaryFilePath(GlobalAttributes::StacktraceFile));
		output.open(QFile::WriteOnly);

		if(output.isOpen())
		{
			lin=QString("** pgModeler crashed after receive signal: %1 **\n\nDate/Time: %2 \nVersion: %3 \nBuild: %4 \n")
				.arg(signal)
				.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
				.arg(GlobalAttributes::PgModelerVersion)
				.arg(GlobalAttributes::PgModelerBuildNumber);

			lin+=QString("Compilation Qt version: %1\nRunning Qt version: %2\n\n")
				 .arg(QT_VERSION_STR)
				 .arg(qVersion());

			output.write(lin.toStdString().c_str(), lin.size());

	#ifndef Q_OS_WIN
			for(size_t i=0; i < stack_size; i++)
			{
				lin=QString("[%1] ").arg(stack_size-1-i) + QString(symbols[i]) + "\n";
				output.write(lin.toStdString().c_str(), lin.size());
			}
			free(symbols);
	#else
		void *win_stack[30];
		USHORT win_stack_size = CaptureStackBackTrace(1, 29, win_stack, nullptr);

		char exe_path[MAX_PATH];
		GetModuleFileNameA(nullptr, exe_path, MAX_PATH);
		DWORD64 exe_base = reinterpret_cast<DWORD64>(GetModuleHandle(nullptr));

		lin = QString("Executable: %1\n\n").arg(exe_path);
		output.write(lin.toStdString().c_str(), lin.size());

		// Compute per-frame module base and offset
		struct FrameInfo { DWORD64 base, offset; char path[MAX_PATH]; };
		FrameInfo frames[30];

		for(USHORT i = 0; i < win_stack_size; i++)
		{
			HMODULE mod = nullptr;
			GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
												 GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
												 reinterpret_cast<LPCSTR>(win_stack[i]), &mod);
			frames[i].base = reinterpret_cast<DWORD64>(mod);
			frames[i].offset = reinterpret_cast<DWORD64>(win_stack[i]) - frames[i].base;
			frames[i].path[0] = '\0';
			if(mod) GetModuleFileNameA(mod, frames[i].path, MAX_PATH);
		}

		// Locate addr2line.exe next to this executable (only available in debug/dev builds)
		QString exe_dir = QString(exe_path).replace('/', '\\').section('\\', 0, -2);
		QString addr2line_bin = exe_dir + "\\addr2line.exe";

		QStringList resolved;

		if(QFile::exists(addr2line_bin))
		{
			// Pass exe-relative offsets for exe frames; 0x0 placeholder for DLL frames
			// so addr2line output lines stay aligned with frame indices
			QString addr_args;
			for(USHORT i = 0; i < win_stack_size; i++)
				addr_args += (frames[i].base == exe_base)
					? QString(" 0x%1").arg(frames[i].offset, 0, 16)
					: QString(" 0x0");

			// addr2line -f (function) -C (demangle) -p (pretty-print combined line)
			// Windows-style path works fine with MinGW addr2line for both binary and -e arg
			QString addr2line_cmd = QString("\"%1\" -e \"%2\" -f -C -p%3 2>&1")
					.arg(addr2line_bin).arg(exe_path).arg(addr_args);

			FILE *pipe = _popen(addr2line_cmd.toLocal8Bit().constData(), "r");
			if(pipe)
			{
				char pipe_buf[512];
				while(fgets(pipe_buf, sizeof(pipe_buf), pipe))
					resolved.append(QString(pipe_buf).trimmed());
				_pclose(pipe);
			}
		}

		for(USHORT i = 0; i < win_stack_size; i++)
		{
			USHORT disp_idx = win_stack_size - 1 - i;
			QString mod_name = QString(frames[i].path).replace('\\', '/').section('/', -1);

			if(frames[i].base == exe_base && i < resolved.size() && !resolved[i].startsWith("??"))
				lin = QString("[%1] %2").arg(disp_idx).arg(resolved[i]);
			else
				lin = QString("[%1] %2+0x%3").arg(disp_idx).arg(mod_name).arg(frames[i].offset, 0, 16);

			lin += "\n";
			output.write(lin.toStdString().c_str(), lin.size());
		}
	#endif

			output.close();
		}

		/* Changing the working dir to the main executable in order to call the crash handler
		if the PGMODELER_CHANDLER_PATH isn't set */
		QDir dir;
		dir.cd(QApplication::applicationDirPath());

		exit(1 + system(cmd.toStdString().c_str()));
	}
}

int main(int argc, char **argv)
{
	try
	{		
		//Install a signal handler to start crashhandler when SIGSEGV or SIGABRT is emitted
		signal(SIGSEGV, startCrashHandler);
		signal(SIGABRT, startCrashHandler);

		GlobalAttributes::init(argv[0], true);
		PgModelerApp app(argc,argv);
		int res=0;

		// Loading the application splash screen
		QSplashScreen splash;
		QPixmap pix(":images/images/pgmodeler_splash.png");

		if(qApp->primaryScreen()->devicePixelRatio() > 1)
			pix.setDevicePixelRatio(qApp->primaryScreen()->devicePixelRatio());
		else
			pix = pix.scaledToWidth(320, Qt::SmoothTransformation);

		splash.setPixmap(pix);
		splash.show();
		splash.raise();
		app.processEvents();

		//Creates the main form
		MainWindow fmain;

		#ifdef PRIV_CODE_SYMBOLS
			__pgm_plus_gui_init
		#endif

		// Displaying the splash for one and a half second after displaying the main window
		QTimer::singleShot(1500, &splash, [&splash, &fmain]() {
			fmain.show();
			splash.finish(&fmain);
		 });

		//Loading models via command line on MacOSX are disabled until the file association work correclty on that system
		#ifndef Q_OS_MACOS
			QStringList params = app.arguments();
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
		return enum_t(e.getErrorCode());
	}
}
