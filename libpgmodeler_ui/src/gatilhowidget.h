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
\class GatilhoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de gatilhos / gatilhos restrições.
*/

#ifndef GATILHO_WIDGET_H
#define GATILHO_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_gatilhowidget.h"
#include "tabelaobjetoswidget.h"

class GatilhoWidget: public BaseObjectWidget, public Ui::GatilhoWidget {
		Q_OBJECT

	private:
		//! \brief Destaque de sintaxe do campo de expressão condicional
		SyntaxHighlighter *dest_exp_condicional;

		//! \brief Tabelas de objetos para controlar colunas e argumentos
		TabelaObjetosWidget *tab_colunas,
												*tab_argumentos;

		//! \brief Seletor de tabela referenciada e função executada pelo gatilho
		SeletorObjetoWidget *sel_tabela_ref,
												*sel_funcao;

		//! \brief Adiciona uma coluna   tabelade colunas referenciadas pelo gatilho
		void adicionarColuna(Column *coluna, int idx_lin);

	public:
		GatilhoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, Table *tabela_pai, OperationList *op_list, Trigger *restricao);

	private slots:
		void hideEvent(QHideEvent *);
		void adicionarColuna(int idx_lin);
		void manipularArgumento(int idx_lin);
		void editarArgumento(int idx_lin);
		void atualizarComboColunas(void);
		void definirGratilhoRestricao(bool valor);

	public slots:
		void applyConfiguration(void);
};

#endif
