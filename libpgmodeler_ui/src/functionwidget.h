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
\class FuncaoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de funções.
*/

#ifndef FUNCAO_WIDGET_H
#define FUNCAO_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_functionwidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

class FuncaoWidget: public BaseObjectWidget, public Ui::FunctionWidget  {
	private:
		Q_OBJECT

		/*! \brief Destaque de código-fonte usado para destacar o código da função
			definido pelo usuário */
		SyntaxHighlighter *source_code_hl;

		//! \brief Widget de tipo Pgsql usado para configurar o tipo de retorno da função
		TipoPgSQLWidget *ret_type;

		//! \brief Tabela que representa a tabela de retorno da função
		TabelaObjetosWidget *return_tab,
												//! \brief Tabela de parâmetros da função
												*parameters_tab;

		//! \brief Converte os dados da linha da tabela informada em um parâmetro
		Parameter getParameter(TabelaObjetosWidget *tab, unsigned idx_lin);

		//! \brief Exibe os dados do parâmetro na tabela e linha selecionadas
		void showParameterData(Parameter param, TabelaObjetosWidget *tab, unsigned idx_lin);

		/*! \brief Valida a nova configuração da função em relação a demais objetos que a referenciam.
		 A exemplo disso temos objetos das classes ConversaoCodificacao, ConversaoTipo,
		 FuncaoAgregada, Gatilho, Linguagem, Operador, Tipo */
		void validateConfiguredFunction(void);

	public:
		FuncaoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Function *funcao);

	private slots:
		void alternateReturnTypes(void);

		/*! \brief Seleciona a linguagem de definição da função, e caso disponível,
			carrega o arquivo de destaque de sintaxe referênt  linguagem
			selecionada */
		void selectLanguage(void);

		//! \brief Exibe a janela de configuração de parâmetros
		void showParameterForm(void);

		/*! \brief Manipula um parâmetro que foi configurado pelo form. de parâmetro
			exibindo seus dados na tabela da parâmetros correta */
		void handleParameter(int);

		void hideEvent(QHideEvent *);

	public slots:
		void applyConfiguration(void);
};

#endif
