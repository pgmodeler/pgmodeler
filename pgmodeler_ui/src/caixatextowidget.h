/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: CaixaTextoWidget
# Descrição: Definição da classe que implementa o formulário de criação e
#            edição de caixa de texto no modelo de banco de dados.
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
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
#ifndef CAIXATEXTO_WIDGET_H
#define CAIXATEXTO_WIDGET_H

#include "ui_caixatextowidget.h"
#include "objetobasewidget.h"
//***********************************************************
class CaixaTextoWidget: public ObjetoBaseWidget, public Ui::CaixaTextoWidget {
 Q_OBJECT

 private:

 public:
   CaixaTextoWidget(QWidget * parent = 0);
   void hideEvent(QHideEvent *evento);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, CaixaTexto *caixa_texto=NULL, float px_objeto=NAN, float py_objeto=NAN);

 private slots:
  void selecionarCorTexto(void);

 public slots:
  void aplicarConfiguracao(void);
};
//***********************************************************
#endif
