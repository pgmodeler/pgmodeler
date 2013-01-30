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
\class VisaoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de visões.
*/

#ifndef VISAO_WIDGET_H
#define VISAO_WIDGET_H

#include <QtGui>
#include "baseobjectwidget.h"
#include "ui_visaowidget.h"
#include "tabelaobjetoswidget.h"

/*! @details Declarando a classe Referencia como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar instancias de classes em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe Referencia.*/
#include <QMetaType>
Q_DECLARE_METATYPE(Reference)

class VisaoWidget: public BaseObjectWidget, public Ui::VisaoWidget {
		Q_OBJECT

	private:
		//! @details Frame usado como dica para criação de uma referência a todas as colunas de uma tabela
		QFrame *frame_info;

		/*! @details Esta instância de visão é usada para receber todos os atributos configurados no formulário.
		 Uma vez atribuídos os dados e validados a mesma tem seus atributos copiados para a visão
		 que realemente precisa ser modificada/criada (this->objeto) */
		View visao_aux;

		//! @details Tabela de listagem das referências cadastradas para a visão
		TabelaObjetosWidget *tab_referencias;

		//! @details Destacador de sintaxe usado no campo de expressão e de código fonte da visão
		SyntaxHighlighter *destaque_expr,
											*destaque_codigo;

		//! @details Seletores de tabela e coluna relacionados   visao
		SeletorObjetoWidget *sel_tabela,
												*sel_coluna;

		//! @details Exibe os dados de uma dada referência na tabela de referências
		void exibirDadosReferencia(Reference refer, bool selec_from, bool from_where,
															 bool apos_where, unsigned idx_ref);

		//! @details Método auxiliar que limpa os campos do formulário de referências
		void limparFormReferencia(void);

	public:
		VisaoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, View *visao, float px, float py);

	private slots:
		void hideEvent(QHideEvent *);

		/*! @details Exibe os campos necessários no formulário conforme o
		 tipo de referencia selecionado */
		void selecionarTipoReferencia(void);

		/*! @details Cria uma referência com base no que está no formulário
		 e a exibe na tabela no índice informado no parâmetro */
		void manipularReferencia(int);

		/*! @details Obtém a referência na linha especificada no parâmetro
		 e exibe seus dados no formulário para edição */
		void editarReferencia(int);

		/*! @details Controla a forma como é exibida os nomes da tabela e coluna
		 selecionada na referência */
		void exibirNomeObjeto(void);

		/*! @details Atualiza o campo de código fonte da visão. Este campo
		 é preenchido com o código gerado pelo atributo 'visao_aux' */
		void atualizarPrevisaoCodigo(void);

	public slots:
		void applyConfiguration(void);
};

#endif
