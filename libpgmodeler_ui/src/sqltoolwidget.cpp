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

#include "sqltoolwidget.h"

SQLToolWidget::SQLToolWidget(QWidget * parent) : QWidget(parent)
{
  setupUi(this);
  h_splitter->setSizes({0, 10000});
  connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
  connect(objects_btn, SIGNAL(toggled(bool)), database_gb, SLOT(setVisible(bool)));
  connect(results_btn, SIGNAL(toggled(bool)), result_trw, SLOT(setVisible(bool)));
}

void SQLToolWidget::hide(void)
{
  QWidget::hide();
  emit s_visibilityChanged(false);
}
