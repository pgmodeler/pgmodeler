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
\ingroup libpgmodeler_ui
\class BugReportForm
\brief Implements operations to permit user generate bug reports without use the crash handler application.
*/

#ifndef BUG_REPORT_FORM_H
#define BUG_REPORT_FORM_H

#include <QDialog>
#include "ui_bugreportform.h"
#include "syntaxhighlighter.h"

class LIBPGMODELER_UI_EXPORT BugReportForm : public QDialog, public Ui::BugReportForm {
	private:
		Q_OBJECT

		//! \brief Syntax highlight for model text widget
		SyntaxHighlighter *hl_model_txt;

  protected:
		//! \brief Delimiter character which separates the sections of the compressed file
		const static char CHR_DELIMITER;

    //! brief Generates an uncompressed buffer based upon the data in fields
    virtual QByteArray generateReportBuffer(void);

    //! brief Generates the bug report file from uncompressed buffer
    void generateReport(const QByteArray &buf);

  public:
    BugReportForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

  public slots:
		void generateReport(void);

  private slots:
    void enableGeneration(void);
    void attachModel(void);
    void selectOutput(void);
};

#endif
