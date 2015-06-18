/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "bugreportform.h"

class CrashHandlerForm: public BugReportForm {
	private:
		Q_OBJECT

    //! brief Display the stacktrace
    QPlainTextEdit *stack_txt;

    //! brief Container for widget that handles input report file
    QWidget *input_wgt;

    QLabel *input_lbl;

    //! brief Display the path to input report file
    QLineEdit *input_edt;

    //! brief Triggers the model saving to filesystem
    QToolButton *save_tb,

    //! brief Triggers the report file loading
    *load_tb;

		//! \brief Load a report file showing its contents on the form
		void loadReport(const QString &filename);

    void setAnalysisMode(bool value);

    //! brief Generates a report buffer containing the issue details, model and stacktrace
    QByteArray generateReportBuffer(void);

	public:
    //! brief Analysis mode argument
    const static QString ANALYSIS_MODE;

    CrashHandlerForm(bool analysis_mode=false, QWidget * parent = 0, Qt::WindowFlags f = 0);

  private slots:
		void loadReport(void);
    void saveModel(void);
};

#endif
