/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ColunaWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de colunas.
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
#ifndef COLUNA_WIDGET_H
#define COLUNA_WIDGET_H

#include "objetobasewidget.h"
#include "ui_colunawidget.h"
#include "tipopgsqlwidget.h"
//***********************************************************
class ColunaWidget: public ObjetoBaseWidget, public Ui::ColunaWidget {
 Q_OBJECT

 private:
   //Destacador de sintaxe para o campo de valor padrão da coluna
   DestaqueSintaxe *dest_valor_padrao;

   //Seletor de tipo da coluna
   TipoPgSQLWidget *tipo_col;

 public:
   ColunaWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ObjetoBase *objeto_pai, ListaOperacoes *lista_op, Coluna *coluna);

 private slots:
  void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
