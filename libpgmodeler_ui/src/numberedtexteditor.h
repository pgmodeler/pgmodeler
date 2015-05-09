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
\class NumberedTextEditor
\brief Implements a simple text editor with a line number block attached to it
*/

#ifndef NUMBERED_TEXT_EDITOR_H
#define NUMBERED_TEXT_EDITOR_H

#include <QPlainTextEdit>
#include "linenumberswidget.h"

class NumberedTextEditor : public QPlainTextEdit {
  private:
    Q_OBJECT

    LineNumbersWidget *line_number_wgt;

    int getLineNumbersWidth(void);

  public:
    NumberedTextEditor(QWidget * parent = 0);
    ~NumberedTextEditor(void);

    void setFont(const QFont &font);

  private slots:
    void updateLineNumbersWidth(void);
    void updateLineNumbers(QRect rect, int dy);
};

#endif
