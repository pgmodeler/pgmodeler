/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: FamiliaOperadoresWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de família de operadores.
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
#ifndef FAMILIAOPERADORES_WIDGET_H
#define FAMILIAOPERADORES_WIDGET_H

#include "objetobasewidget.h"
#include "ui_familiaoperadoreswidget.h"
//***********************************************************
class FamiliaOperadoresWidget: public ObjetoBaseWidget, public Ui::FamiliaOperadoresWidget {
 Q_OBJECT

 private:

 public:
   FamiliaOperadoresWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, FamiliaOperadores *familia_op);

 private slots:
   void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
