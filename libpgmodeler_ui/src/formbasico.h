/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: FormBasico
# Description:Definição da classe que um container de widgets de configuração
#            de objetos no modelo de banco de dados.
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
#ifndef FROM_BASICO_H
#define FROM_BASICO_H

#include <QtGui>
#include "ui_formbasico.h"
#include "exception.h"
#include "caixamensagem.h"

class FormBasico: public QDialog, public Ui::FormBasico {
 Q_OBJECT

 private:

 public:
   FormBasico(QWidget * parent = 0, Qt::WindowFlags f = 0);
  ~FormBasico(void){}
  void definirBotoes(unsigned conf_botoes=CaixaMensagem::BOTAO_OK_CANCELAR);

 public slots:
};

#endif
