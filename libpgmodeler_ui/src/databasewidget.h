/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: DatabaseWidget
# Description: Implements the operations to create/edit database attributes via form.
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
#ifndef DATABASE_WIDGET_H
#define DATABASE_WIDGET_H

#include "ui_databasewidget.h"
#include "baseobjectwidget.h"

class DatabaseWidget: public BaseObjectWidget, public Ui::DatabaseWidget {
 private:
  Q_OBJECT

 public:
   DatabaseWidget(QWidget * parent = 0);
   void setAttributes(DatabaseModel *model);

 public slots:
   void applyConfiguration(void);
};

#endif
