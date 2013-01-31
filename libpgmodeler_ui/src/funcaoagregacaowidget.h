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
\class FuncaoAgregacaoWidget
\brief Definição da classe que implementa o formulário de
 edição dos atributos de funções de agregação.
*/

#ifndef FUNCAO_AGREGADA_WIDGET_H
#define FUNCAO_AGREGADA_WIDGET_H

#include <QtGui>
#include "baseobjectwidget.h"
#include "ui_funcaoagregacaowidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

class FuncaoAgregacaoWidget: public BaseObjectWidget, public Ui::FuncaoAgregacaoWidget {
		Q_OBJECT

	private:
		//! \brief Widget de seleção de tipo de entrada da função
		TipoPgSQLWidget *tipo_entrada,

										//! \brief Widget de tipo de estado da função
										*tipo_estado;

		/*! \brief Tabela de listagem de tipos de entrada da função.
		Os elementos desta tabela são convertidos em TipoPgSQL e
		atribuídos como tipos de entrada da função agregada */
		TabelaObjetosWidget *tab_tipos_entrada;

		//! \brief Destacador de sintaxe usado no campo de expressão condicional
		SyntaxHighlighter *destaque_cond;

		//! \brief Seletores de funções e operador relacionao   função de agregação
		SeletorObjetoWidget *sel_op_ordenacao,
												*sel_func_final,
												*sel_func_transicao;

	public:
		FuncaoAgregacaoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Aggregate *funcao_ag);

	private slots:
		void hideEvent(QHideEvent *);

		//! \brief Adiciona ou atualiza o tipo selecionado no widget 'tipo_entrada'   tabela de tipos de entrada
		void manipularTipoDado(int linha);

	public slots:
		void applyConfiguration(void);
};

#endif
