/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ConfGeralWidget
# Descrição: Implementa o widget de configurações gerais do modelo.
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
#ifndef CONF_GERAL_WIDGET_H
#define CONF_GERAL_WIDGET_H

#include "ui_confgeralwidget.h"
#include "confbasewidget.h"
#include "cenaobjetos.h"
#include "listaoperacoes.h"
//***********************************************************
class ConfGeralWidget: public QWidget, public Ui::ConfGeralWidget, public ConfBaseWidget {
 private:
 Q_OBJECT

 public:
  ConfGeralWidget(QWidget * parent=0);

  void salvarConfiguracao(void);
  void carregarConfiguracao(void);

 public slots:
  void aplicarConfiguracao(void);
  void restaurarPadroes(void);

 private slots:
   //Faz a conversão dos valores das margens para a unidade selecionada
   void converterUnidadeMargem(void);
};
//***********************************************************
#endif
