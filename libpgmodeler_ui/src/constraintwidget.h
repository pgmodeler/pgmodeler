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

#ifndef CONSTRAINT_WIDGET_H
#define CONSTRAINT_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_constraintwidget.h"
#include "tabelaobjetoswidget.h"

class ConstraintWidget: public BaseObjectWidget, public Ui::ConstraintWidget {
	private:
		Q_OBJECT

		QFrame *info_frm;

		//! \brief Destaque de sintaxe do campo de expressão de checagem
		SyntaxHighlighter *check_expr_hl;

		//! \brief Tabelas de objetos para controlar colunas referenciadas na restrição
		TabelaObjetosWidget *columns_tab,
												*ref_columns_tab;

		//! \brief Seletor de tabela referenciada
		SeletorObjetoWidget *ref_table_sel;

		//! \brief Atualiza o combo de colunas do tipo especificado (Origem ou Referenciadas)
		void updateColumnsCombo(unsigned col_id);

		/*! \brief Adiciona uma coluna   tabela de tipo especificado (origem ou referenciada)
			na linha especificada */
		void addColumn(Column *column, unsigned col_id, int row);

		void hideEvent(QHideEvent *event);

	public:
		ConstraintWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Constraint *constr);

	private slots:
		//! \brief Exibe os campos pertinentes ao tipo da restição selecionado
		void selectConstraintType(void);

		/*! \brief Atualiza o combo de colunas referenciadas conforme o usuário
		 manipule o seletor de tabela referenciada */
		void selectReferencedTable(void);

		//! \brief Métodos de manipulação das tabelas de colunas
		void addColumn(int row);
		void removeColumn(int idx_lin);
		void removeColumns(void);

	public slots:
		void applyConfiguration(void);
};

#endif
