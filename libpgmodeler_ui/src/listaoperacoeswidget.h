/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

/**
\ingroup libpgmodeler_ui
\class ListaOperacoesWidget
\brief Definição da classe que implementa a arvore de operações executadas no modelo de banco de dados.
*/

#ifndef LISTA_OPERACOES_WIDGET_H
#define LISTA_OPERACOES_WIDGET_H

#include <QtGui>
#include "ui_listaoperacoeswidget.h"
#include "modelowidget.h"
#include "messagebox.h"

class ListaOperacoesWidget: public QDockWidget, public Ui::ListaOperacoesWidget {
		Q_OBJECT

	private:
		//! \brief Armazena o modelo atual o qual será exibido na visão geral
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
		/*! \brief Sinal emitido pelo widget quando uma operação sobre
			a lista é executada */
		void s_operacaoExecutada(void);
		void s_listaOperacoesAtualizada(void);
};

#endif
