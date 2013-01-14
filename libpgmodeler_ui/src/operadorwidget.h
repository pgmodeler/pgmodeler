/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: OperadorWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos de operadores.
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
#ifndef OPERADOR_WIDGET_H
#define OPERADOR_WIDGET_H

#include "baseobjectwidget.h"
#include "tipopgsqlwidget.h"
#include "ui_operadorwidget.h"

class OperadorWidget: public BaseObjectWidget, public Ui::OperadorWidget {
 Q_OBJECT

 private:
   TipoPgSQLWidget *tipo_args[2];
   SeletorObjetoWidget *sel_funcoes[3],
                       *sel_operadores[6];

 public:
   OperadorWidget(QWidget * parent = 0);
   void setAttributes(DatabaseModel *model, OperationList *op_list, Operator *operador);

 private slots:
  void hideEvent(QHideEvent *);

 public slots:
   void applyConfiguration(void);
};

#endif
