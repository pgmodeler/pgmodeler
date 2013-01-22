/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Classe: ConfAparenciaWidget
# Description:Implementa o widget de configurações de aparência dos objetos do modelo.
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
#ifndef CONF_APARENCIA_WIDGET_H
#define CONF_APARENCIA_WIDGET_H

#include "ui_appearanceconfigwidget.h"
#include "baseconfigwidget.h"
#include "objectsscene.h"
#include "databasemodel.h"
#include <algorithm>

class AppearanceConfigWidget: public QWidget, public Ui::AppearanceConfigWidget, public BaseConfigWidget {
 private:
  Q_OBJECT

  //Classe auxiliar que armazena os dados da formatação de cada elemento
  class AppearanceConfigItem {
   public:
    QString conf_id;
    QTextCharFormat font_fmt;
    QColor colors[3];
    bool obj_conf;
  };

  //Diálogo de seleção de cor
  QColorDialog color_dlg;

  //Viewport que mostra o modelo de exemplo
  QGraphicsView *viewp;

  //Cena que armazena os objetos do modelo de exemplo
  ObjectsScene *scene;

  //Modelo de objetos exemplo
  DatabaseModel *model;

  //Vetor o qual armazena cada elemento de configuração de aparência do modelo
  vector<AppearanceConfigItem> conf_items;

  //Carrega o modelo de exemplo
  void loadExampleObjects(void);

 public:
  AppearanceConfigWidget(QWidget * parent=0);
  ~AppearanceConfigWidget(void);

  void saveConfiguration(void);
  void loadConfiguration(void);

 private slots:
  //Habilita os itens do formulário conforme o elemento selecionado no combobox 'elemento_cmb'
  void enableConfigElement(void);

  //Aplica o estilo de fonte ao elemento atual
  void applyFontStyle(void);

  //Aplica o estilo de cor ao elemento atual
  void applyElementColor(void);

  /* Este método não é usado nesta classe pois as modificações são aplicadas diretamente
     ao arquivo, operação esta tratada pelo método salvarConfiguracao() */
  void aplicarConfiguracao(void){}

 public slots:
  void restoreDefaults(void);
};

#endif
