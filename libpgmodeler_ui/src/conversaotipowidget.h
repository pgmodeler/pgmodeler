/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: ConversaoTipoWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos de conversão de tipos.
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
#ifndef CONVERSAOTIPO_WIDGET_H
#define CONVERSAOTIPO_WIDGET_H

#include "objetobasewidget.h"
#include "tipopgsqlwidget.h"
#include "ui_conversaotipowidget.h"

class ConversaoTipoWidget: public ObjetoBaseWidget, public Ui::ConversaoTipoWidget {
 Q_OBJECT

 private:
   /* Widgets de configuração dos tipos de dados (origem / destino)
      da conversão */
   TipoPgSQLWidget *tipo_dado_orig, *tipo_dado_dest;

   //Widget seletor da função de conversão
   SeletorObjetoWidget *sel_funcao_conv;

 public:
   ConversaoTipoWidget(QWidget * parent = 0);
   void definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Cast *conv_tipo);

 private slots:
   void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
