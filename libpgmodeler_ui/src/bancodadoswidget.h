/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Classe: BancoDadosWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos do banco de dados.
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
#ifndef BANCODADOS_WIDGET_H
#define BANCODADOS_WIDGET_H

#include "ui_bancodadoswidget.h"
#include "baseobjectwidget.h"

class BancoDadosWidget: public BaseObjectWidget, public Ui::BancoDadosWidget {
 Q_OBJECT

 private:

 public:
   BancoDadosWidget(QWidget * parent = 0);
   void setAttributes(DatabaseModel *model);

 private slots:

 public slots:
   void applyConfiguration(void);
};

#endif
