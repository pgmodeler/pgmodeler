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
\class SourceCodeEditor
\brief Implements a simple source code widget with a line number block attached to it
*/

#ifndef SOURCE_CODE_EDITOR_H
#define SOURCE_CODE_EDITOR_H

#include <QTextEdit>
#include "linenumberwidget.h"

class SourceCodeEditor : public QTextEdit {
  private:
    Q_OBJECT
    LineNumberWidget *line_number_wgt;

    int getLineNumbersWidth(void);

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    SourceCodeEditor(QWidget * parent = 0);
    ~SourceCodeEditor(void);

  private slots:
    void updateLineNumbersWidth(void);
    void updateLineNumbers(void);
};

#endif
