/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: CaixaTextoWidget
# Description:Definição da classe que implementa o formulário de criação e
#            edição de caixa de texto no modelo de banco de dados.
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
#ifndef CAIXATEXTO_WIDGET_H
#define CAIXATEXTO_WIDGET_H

#include "ui_caixatextowidget.h"
#include "objetobasewidget.h"

class CaixaTextoWidget: public ObjetoBaseWidget, public Ui::CaixaTextoWidget {
 Q_OBJECT

 private:

 public:
   CaixaTextoWidget(QWidget * parent = 0);
   void hideEvent(QHideEvent *evento);
   void definirAtributos(ModeloBD *modelo, OperationList *lista_op, Textbox *caixa_texto=NULL, float px_objeto=NAN, float py_objeto=NAN);

 private slots:
  void selecionarCorTexto(void);

 public slots:
  void aplicarConfiguracao(void);
};

#endif
