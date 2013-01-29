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

/**
\ingroup libpgmodeler_ui
\class CastWidget
\brief Implements the operations to create/edit type casts via form.
*/

#ifndef CONVERSAOTIPO_WIDGET_H
#define CONVERSAOTIPO_WIDGET_H

#include "baseobjectwidget.h"
#include "tipopgsqlwidget.h"
#include "ui_castwidget.h"

class CastWidget: public BaseObjectWidget, public Ui::CastWidget {
 private:
  Q_OBJECT

  /// @details Datatype configurations
  TipoPgSQLWidget *src_datatype, *trg_datatype;

  /// @details Conversion function selector
  SeletorObjetoWidget *conv_func_sel;

  void hideEvent(QHideEvent *event);

 public:
  CastWidget(QWidget * parent = 0);
  void setAttributes(DatabaseModel *model, OperationList *op_list, Cast *cast);

 public slots:
  void applyConfiguration(void);
};

#endif
