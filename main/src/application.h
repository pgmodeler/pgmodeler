/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

/*
# The original implementation of createUserConfiguration() and copyFilesRecursively() was
# originally written by Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com> and is
# available at: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
#
# The current code was reviewed and minimally changed by Raphael Araújo e Silva <raphael@pgmodeler.com.br>
*/

/**
\ingroup main
\class Application
\brief This class inherits from QApplication and has the notify() method modified
 to treat the exceptions raised by pgModeler components.
\note <strong>Creation date:</strong> 30/08/2007
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QTextStream>
#include <QTranslator>
#include <QFile>

class Application: public QApplication {
	private:
		/*! \brief Creates the pgModeler's configuration dir on user's home folder.
		The output path is platform dependant and is ruled by GlobalAttributes::CONFIGURATIONS_DIR */
		void createUserConfiguration(void);

		//! \brief Copy files from a path to another recursively
		void copyFilesRecursively(const QString &src_path, const QString &dst_path);

	public:
		Application(int & argc, char ** argv);
		bool notify(QObject * receiver, QEvent * event);
};

#endif
