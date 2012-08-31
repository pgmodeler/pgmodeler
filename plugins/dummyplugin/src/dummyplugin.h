/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: Viewport
# Descrição: Esta classe deriva a classe QGraphicsScene e implementa a visualização
#            e outras operações relacionados aos objetos gráficos do modelo.
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License

#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef DUMMY_PLUGIN_H
#define DUMMY_PLUGIN_H

#include "pgmodelerplugin.h"
//***********************************************************
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
//***********************************************************
#endif
