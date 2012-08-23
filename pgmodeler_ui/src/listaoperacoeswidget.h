/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ListaOperacoesWidget
# Descrição: Definição da classe que implementa a arvore de operações executadas
#            no modelo de banco de dados.
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
#ifndef LISTA_OPERACOES_WIDGET_H
#define LISTA_OPERACOES_WIDGET_H

#include <QtGui>
#include "ui_listaoperacoeswidget.h"
#include "modelowidget.h"
#include "caixamensagem.h"
//***********************************************************
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
//***********************************************************
#endif
