/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: FamiliaOperadoresWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos de família de operadores.
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
#ifndef FAMILIAOPERADORES_WIDGET_H
#define FAMILIAOPERADORES_WIDGET_H

#include "objetobasewidget.h"
#include "ui_familiaoperadoreswidget.h"

class FamiliaOperadoresWidget: public ObjetoBaseWidget, public Ui::FamiliaOperadoresWidget {
 Q_OBJECT

 private:

 public:
   FamiliaOperadoresWidget(QWidget * parent = 0);
   void definirAtributos(DatabaseModel *modelo, OperationList *lista_op, OperatorFamily *familia_op);

 private slots:
   void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
