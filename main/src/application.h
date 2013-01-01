/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: main
# Classe: Application
# Description: This class inherits from QApplication and has the notify() method modified
#              to treat the exceptions raised by pgModeler components.
# Creation date: 30/08/2007
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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMessageBox>
#include "caixamensagem.h"

extern CaixaMensagem *caixa_msg;

class Application: public QApplication {
 private:
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
    caixa_msg->show(e);
    return(false);
   }
   catch(...)
   {
    caixa_msg->show(trUtf8("Error"),trUtf8("Unknown exception caught!"), CaixaMensagem::ICONE_ERRO);
    return(false);
   }
  }
};

#endif
