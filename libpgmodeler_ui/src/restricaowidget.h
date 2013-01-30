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
\class RestricaoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de restrições.
*/

#ifndef RESTRICAO_WIDGET_H
#define RESTRICAO_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_restricaowidget.h"
#include "tabelaobjetoswidget.h"

class RestricaoWidget: public BaseObjectWidget, public Ui::RestricaoWidget {
		Q_OBJECT

	private:
		QFrame *frame_info;

		//! @details Destaque de sintaxe do campo de expressão de checagem
		SyntaxHighlighter *dest_exp_checagem;

		//! @details Tabelas de objetos para controlar colunas referenciadas na restrição
		TabelaObjetosWidget *tab_colunas,
												*tab_colunas_ref;

		//! @details Seletor de tabela referenciada
		SeletorObjetoWidget *sel_tabela_ref;

		//! @details Atualiza o combo de colunas do tipo especificado (Origem ou Referenciadas)
		void atualizarComboColunas(unsigned tipo_cmb);

		/*! @details Adiciona uma coluna   tabela de tipo especificado (origem ou referenciada)
			na linha especificada */
		void adicionarColuna(Column *coluna, unsigned tipo_col, int idx_lin);

	public:
		RestricaoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, BaseObject *objeto_pai, OperationList *op_list, Constraint *restricao);

	private slots:
		void hideEvent(QHideEvent *);

		//! @details Exibe os campos pertinentes ao tipo da restição selecionado
		void selecionarTipoRestricao(void);

		/*! @details Atualiza o combo de colunas referenciadas conforme o usuário
		 manipule o seletor de tabela referenciada */
		void selecionarTabelaReferenciada(void);

		//! @details Métodos de manipulação das tabelas de colunas
		void adicionarColuna(int idx_lin);
		void removerColuna(int idx_lin);
		void removerColunas(void);

	public slots:
		void applyConfiguration(void);
};

#endif
