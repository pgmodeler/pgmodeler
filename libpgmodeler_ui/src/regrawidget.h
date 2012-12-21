/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: RegraWidget
# Description:Definição da classe que implementa o formulário de
#            edição dos atributos de regras.
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
#ifndef REGRA_WIDGET_H
#define REGRA_WIDGET_H

#include "objetobasewidget.h"
#include "ui_regrawidget.h"
#include "tabelaobjetoswidget.h"

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
   void definirAtributos(DatabaseModel *modelo, Table *tabela_pai, OperationList *lista_op, Rule *regra);

 private slots:
  void hideEvent(QHideEvent *);
  //Exibe o comando digitado no campo de comando sql na tabela, na linha idx_lin
  void manipularComando(int idx_lin);
  //Exibe o dado da linha da tabela (idx_lin) no campo de comando sql
  void editarComando(int idx_lin);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
