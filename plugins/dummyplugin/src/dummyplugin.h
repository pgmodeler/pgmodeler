/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: DummyPlugin
# Descrição: Esta classe implementa métodos básicos para criação de plugins do pgModeler.
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
#ifndef DUMMY_PLUGIN_H
#define DUMMY_PLUGIN_H

#include "pgmodelerplugin.h"

class DummyPlugin: public QObject, public PgModelerPlugin {
 private:
  Q_OBJECT
  //Declara a interface na qual é usada para implementação de plugin
  Q_INTERFACES(PgModelerPlugin)

 public:
  DummyPlugin(void);

  /* Método de obtenção do rótulo do plugin. Este rótulo é usado
     como texto da ação que dispara o plugin na janela principal */
  QString obterRotuloPlugin(void);

  //Método de execução do plugin sobrecarregado da classe PgModelerPlugin
  void executarPlugin(ModeloWidget *modelo);
};

#endif
