/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: RegraWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de regras.
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
#ifndef REGRA_WIDGET_H
#define REGRA_WIDGET_H

#include "objetobasewidget.h"
#include "ui_regrawidget.h"
#include "tabelaobjetoswidget.h"
//***********************************************************
class RegraWidget: public ObjetoBaseWidget, public Ui::RegraWidget {
 Q_OBJECT

 private:
  //Destacadores de sintaxe dos campos de expressão condicional e comando sql
  DestaqueSintaxe *dest_exp_condicional,
                  *dest_comando;

  //Tabela que armazena os comandos sql da regra
  TabelaObjetosWidget *tab_comandos;

 public:
   RegraWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, ListaOperacoes *lista_op, Regra *regra);

 private slots:
  void hideEvent(QHideEvent *);
  //Exibe o comando digitado no campo de comando sql na tabela, na linha idx_lin
  void manipularComando(int idx_lin);
  //Exibe o dado da linha da tabela (idx_lin) no campo de comando sql
  void editarComando(int idx_lin);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
