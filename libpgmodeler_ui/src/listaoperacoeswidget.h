/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: ListaOperacoesWidget
# Descrição: Definição da classe que implementa a arvore de operações executadas
#            no modelo de banco de dados.
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
#ifndef LISTA_OPERACOES_WIDGET_H
#define LISTA_OPERACOES_WIDGET_H

#include <QtGui>
#include "ui_listaoperacoeswidget.h"
#include "modelowidget.h"
#include "caixamensagem.h"

class ListaOperacoesWidget: public QDockWidget, public Ui::ListaOperacoesWidget {
 Q_OBJECT

 private:
  //Armazena o modelo atual o qual será exibido na visão geral
  ModeloWidget *modelo_wgt;

  void atualizarModeloObjetos(void);

 public:
   ListaOperacoesWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

 public slots:
   void atualizarListaOperacoes(void);
   void definirModelo(ModeloWidget *modelo);
   void desfazerOperacao(void);
   void refazerOperacao(void);
   void excluirOperacoes(void);

 private slots:
   void selecionarItem(QTreeWidgetItem *item, int coluna);

 signals:
   /* Sinal emitido pelo widget quando uma operação sobre
      a lista é executada */
   void s_operacaoExecutada(void);
   void s_listaOperacoesAtualizada(void);
};

#endif
