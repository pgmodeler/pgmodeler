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

  sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, true, false);
  sql_cmd_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
                                GlobalAttributes::DIR_SEPARATOR +
                                GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                GlobalAttributes::CONFIGURATION_EXT);

  h_splitter->setSizes({0, 10000});
  connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
  connect(results_tb, SIGNAL(toggled(bool)), result_trw, SLOT(setVisible(bool)));
}

void SQLToolWidget::updateConnections(map<QString, Connection *> &conns)
{
  map<QString, Connection *>::iterator itr=conns.begin();
  connections_cmb->clear();

  //Add the connections to the combo
  while(itr!=conns.end())
  {
    connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
    itr++;
  }
}

void SQLToolWidget::hide(void)
{
  QWidget::hide();
  emit s_visibilityChanged(false);
}
