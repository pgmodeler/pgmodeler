/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: BancoDadosWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos do banco de dados.
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
#ifndef BANCODADOS_WIDGET_H
#define BANCODADOS_WIDGET_H

#include "ui_bancodadoswidget.h"
#include "objetobasewidget.h"
//***********************************************************
class BancoDadosWidget: public ObjetoBaseWidget, public Ui::BancoDadosWidget {
 Q_OBJECT

 private:

 public:
   BancoDadosWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo);

 private slots:

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
