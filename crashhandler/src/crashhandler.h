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
\ingroup crashhandler
\class CrashHandler
\brief Implements the pgModeler's crash handler enabling the generation an analysis of crash report files.
*/

#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <QDialog>
#include "exception.h"
#include "globalattributes.h"
#include "ui_crashhandler.h"
#include "syntaxhighlighter.h"
#include "messagebox.h"

class CrashHandler : public QDialog, Ui::CrashHandler
{
	private:
		Q_OBJECT

		//! \brief Syntax highlight for model text widget
		SyntaxHighlighter *hl_model_txt;

		//! \brief Delimiter character which separates the sections of the compressed file
		const static char CHR_DELIMITER;

	public:
		CrashHandler(QWidget * parent = 0, Qt::WindowFlags f = 0);

		//! \brief Load a report file showing its contents on the form
		void loadReport(const QString &filename);

	public slots:
		void generateReport(void);
		void enableGeneration(void);
};

#endif
