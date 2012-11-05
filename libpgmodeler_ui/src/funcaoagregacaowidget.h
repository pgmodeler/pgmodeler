/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: FuncaoAgregacaoWidget
# Description:o: Definição da classe que implementa o formulário de
#            edição dos atributos de funções de agregação.
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
#ifndef FUNCAO_AGREGADA_WIDGET_H
#define FUNCAO_AGREGADA_WIDGET_H

#include <QtGui>
#include "objetobasewidget.h"
#include "ui_funcaoagregacaowidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

class FuncaoAgregacaoWidget: public ObjetoBaseWidget, public Ui::FuncaoAgregacaoWidget {
 Q_OBJECT

 private:          //Widget de seleção de tipo de entrada da função
   TipoPgSQLWidget *tipo_entrada,
                   //Widget de tipo de estado da função
                   *tipo_estado;

                   /* Tabela de listagem de tipos de entrada da função.
                      Os elementos desta tabela são convertidos em TipoPgSQL e
                      atribuídos como tipos de entrada da função agregada */
  TabelaObjetosWidget *tab_tipos_entrada;

                   //Destacador de sintaxe usado no campo de expressão condicional
   DestaqueSintaxe *destaque_cond;

                  //Seletores de funções e operador relacionao   função de agregação
   SeletorObjetoWidget *sel_op_ordenacao,
                       *sel_func_final,
                       *sel_func_transicao;

 public:
   FuncaoAgregacaoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, FuncaoAgregacao *funcao_ag);

 private slots:
  void hideEvent(QHideEvent *);

  //Adiciona ou atualiza o tipo selecionado no widget 'tipo_entrada'   tabela de tipos de entrada
  void manipularTipoDado(int linha);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
