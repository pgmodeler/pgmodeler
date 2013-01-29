/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: crashhandler
# Classe: CrashHandler
# Description: Implements the pgModeler's crash handler enabling the generation an analysis
#              of crash report files.
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
#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <QDialog>
#include "exception.h"
#include "globalattributes.h"
#include "ui_crashhandler.h"
#include "syntaxhighlighter.h"
#include "messagebox.h"

#include <iostream>
#include <fstream>

class CrashHandler : public QDialog, Ui::CrashHandler
{
 private:
  Q_OBJECT

  /// @details Syntax highlight for model text widget
  SyntaxHighlighter *hl_model_txt;

  /// @details Delimiter character which separates the sections of the compressed file
  const static char CHR_DELIMITER;

 public:
  CrashHandler(QWidget * parent = 0, Qt::WindowFlags f = 0);

  /// @details Load a report file showing its contents on the form
  void loadReport(const QString &filename);

 public slots:
  void generateReport(void);
  void enableGeneration(void);
};

#endif
