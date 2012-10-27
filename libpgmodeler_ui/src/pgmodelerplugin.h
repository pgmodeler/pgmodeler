/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: PgModelerLibrary
# Descrição: Definição da classe que  implementa  a base para suporte de plugins de terceiros,
#            baseados em bibliotecas compartilhadas.
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
#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "modelowidget.h"

/* Os plugins no pgModeler deve estar dentro da pasta "plugins"
   em um diretório próprio (recomenda-se usar o mesmo nome do plugin)
   e deve possuir a seguinte estrutura básica:

   pgmodeler_root/plugins/
                         +- pluginA
                                  +---(lib)*(pluginA.)(so|dll) (biblioteca que represnta o plugin)
                                  +---pluginA.png              (ícone que representa o plugin, deve possuir o mesmo nome

  Obs.: Plugins podem fazer uso de subdiretórios mas qualquer referência a estes devem ser feitas
        manualmente pelo criado do plugin.
*/
class PgModelerPlugin {
 public:
  PgModelerPlugin(void) {}
  virtual ~PgModelerPlugin(void) {}

  //Método que executa o plugin
  virtual void executarPlugin(ModeloWidget *modelo)=0;

  //Retorna o texto que é exibido na ação que executa o plugin
  virtual QString obterRotuloPlugin(void)=0;
};

/* Declara a classe PgModelerPlugin como interface, ou seja, a base para
   implementação de plugins. Todo plugin deve herdar esta classe e usar a
   diretiva Q_INTERFACE em sua construção */
Q_DECLARE_INTERFACE(PgModelerPlugin,"pgmodelerplugin")

#endif
