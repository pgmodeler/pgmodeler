/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: DominioWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos de esquemas.
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef DOMINIO_WIDGET_H
#define DOMINIO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_dominiowidget.h"
#include "tipopgsqlwidget.h"

class DominioWidget: public ObjetoBaseWidget, public Ui::DominioWidget {
 Q_OBJECT

 private:
   DestaqueSintaxe *destaque_expr;
   TipoPgSQLWidget *tipo_dominio;

 public:
   DominioWidget(QWidget * parent = 0);
   void definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Domain *dominio);

 private slots:
  void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
