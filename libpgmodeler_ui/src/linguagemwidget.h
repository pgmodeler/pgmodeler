/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: LinguagemWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de linguagens.
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
#ifndef LINGUAGEM_WIDGET_H
#define LINGUAGEM_WIDGET_H

#include "objetobasewidget.h"
#include "ui_linguagemwidget.h"

class LinguagemWidget: public ObjetoBaseWidget, public Ui::LinguagemWidget {
 Q_OBJECT

 private:
   SeletorObjetoWidget *sel_func_handler,
                       *sel_func_validator,
                       *sel_func_inline;
 public:
   LinguagemWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Linguagem *linguagem);

 private slots:
   void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
