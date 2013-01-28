/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler User Interface Library (libpgmodeler_ui)
# Classe: QuickRenameWidget
# Description: Widget used to quickly rename objects without open the editing form
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
#ifndef QUICK_RENAME_WIDGET_H
#define QUICK_RENAME_WIDGET_H

#include <QtGui>
#include "ui_quickrenamewidget.h"
#include "databasemodel.h"
#include "operationlist.h"

class QuickRenameWidget: public QDialog, public Ui::QuickRenameWidget {
 private:
  Q_OBJECT

  /// Object to be renamed
  BaseObject *object;

  /// Operation list used to register the modification over object
  OperationList *op_list;

  /// Model used to made duplicity name validations
  DatabaseModel *model;

  void hideEvent(QHideEvent *);

 public:
  QuickRenameWidget(QWidget *parent);

  void setAttributes(BaseObject *object, DatabaseModel *model, OperationList *op_list);

 public slots:
  void exec(void);

 private slots:
  void applyRenaming(void);
};

#endif
