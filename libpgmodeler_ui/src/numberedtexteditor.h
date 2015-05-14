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
\brief Implements a simple text editor with a line number block attached to it. This class is
based upon the code editor example provided by Qt
*/

#ifndef NUMBERED_TEXT_EDITOR_H
#define NUMBERED_TEXT_EDITOR_H

#include <QPlainTextEdit>
#include "linenumberswidget.h"

class NumberedTextEditor : public QPlainTextEdit {
  private:
    Q_OBJECT

    static bool line_nums_visible, highlight_lines;

    static QColor line_hl_color;

    static QFont default_font;

    //! brief Widget used to expose document line numbers
    LineNumbersWidget *line_number_wgt;

    //! brief Determines and returns the line numbers widget width
    int getLineNumbersWidth(void);

  protected:
    void resizeEvent(QResizeEvent *);

  public:
    NumberedTextEditor(QWidget * parent = 0);

    static void setDefaultFont(const QFont &font);
    static void setLineNumbersVisible(bool value);
    static void setHighlightLines(bool value);
    static void setLineHighlightColor(const QColor &color);

  public slots:
    void updateLineNumbers(void);
    void updateLineNumbersSize(void);
    void highlightCurrentLine(void);
};

#endif
