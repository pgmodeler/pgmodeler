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
#include <QMessageBox>
#include "messagebox.h"

class Application: public QApplication {
	private:
		Messagebox msg_box;

	public:

		Application(int & argc, char ** argv) : QApplication(argc,argv)
		{}

		bool notify(QObject * receiver, QEvent * event)
		{
			try
			{
				return(QApplication::notify(receiver,event));
			}
			catch(Exception &e)
			{
				msg_box.show(e);
				return(false);
			}
			catch(...)
			{
				msg_box.show(trUtf8("Error"),trUtf8("Unknown exception caught!"), Messagebox::ERROR_ICON);
				return(false);
			}
		}
};

#endif
