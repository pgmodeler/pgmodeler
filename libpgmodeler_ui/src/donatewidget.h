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
\class DonateWidget
\brief Implements a basic dialog to ask for donations shown as a popup widget on main window.
*/

#ifndef DONATE_WIDGET_H
#define DONATE_WIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include "ui_donatewidget.h"

class DonateWidget: public QWidget, public Ui::DonateWidget {
  private:
    Q_OBJECT

  public:
    DonateWidget(QWidget *parent = 0);

  signals:
    void s_visibilityChanged(bool value);
};

#endif
