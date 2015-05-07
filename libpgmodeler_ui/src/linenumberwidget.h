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
\class LineNumberWidget
\brief Implements a simple line number widget that expose the current highlighted line in a text editor
*/

#ifndef LINE_NUMBER_WIDGET_H
#define LINE_NUMBER_WIDGET_H

#include <QWidget>

class LineNumberWidget : public QWidget {
  private:
    Q_OBJECT

  public:
    LineNumberWidget(QWidget * parent = 0);
    ~LineNumberWidget(void);
};

#endif
