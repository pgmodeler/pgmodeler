/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: FormPrincipal
# Descrição: Definição da classe que implementa a caixa de informações do software.
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
#ifndef FORM_SOBRE_H
#define FORM_SOBRE_H

#include <QWidget>
#include "ui_formsobre.h"
#include "atributosglobais.h"
//***********************************************************
class FormSobre: public QWidget, public Ui::FormSobre {
 Q_OBJECT

 private:

 public:
   FormSobre(QWidget *parent = 0);
  ~FormSobre(void){}

 private slots:
};
//***********************************************************
#endif
