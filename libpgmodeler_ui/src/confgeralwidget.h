/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Classe: ConfGeralWidget
# Description:Implementa o widget de configurações gerais do modelo.
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
#ifndef CONF_GERAL_WIDGET_H
#define CONF_GERAL_WIDGET_H

#include "ui_confgeralwidget.h"
#include "baseconfigwidget.h"
#include "objectsscene.h"
#include "operationlist.h"

class ConfGeralWidget: public QWidget, public Ui::ConfGeralWidget, public BaseConfigWidget {
 private:
 Q_OBJECT

 public:
  ConfGeralWidget(QWidget * parent=0);

  void saveConfiguration(void);
  void loadConfiguration(void);

 public slots:
  void applyConfiguration(void);
  void restoreDefaults(void);

 private slots:
   //Faz a conversão dos valores das margens para a unidade selecionada
   void converterUnidadeMargem(void);
};

#endif
