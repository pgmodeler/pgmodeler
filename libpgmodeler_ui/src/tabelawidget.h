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
\brief Definição da classe que implementa o formulário de edição dos atributos de tabela.
*/

#ifndef TABELA_WIDGET_H
#define TABELA_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tabelawidget.h"
#include "tabelaobjetoswidget.h"
#include "tableview.h"

class TabelaWidget: public BaseObjectWidget, public Ui::TabelaWidget {
		Q_OBJECT

	private:
		/*! @details Quantidade de elementos na lista de operações antes da tabela
			ser editada. Este atributo é usado para se saber, em caso de cancelamento
			da edição da tabela, a quantidade de operações relacionadas ao
			objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
		unsigned qtd_operacoes;

		//! @details Armazena as tabelas de objetos filhos da tabela (colunas, restrições, índices, etc)
		map<ObjectType, TabelaObjetosWidget *> mapa_tab_objetos;

		/*! @details Lista os objetos do relacionamento na tabela respectiva, de acordo
			com o tipo do objeto passado */
		void listarObjetos(ObjectType tipo_obj);

		//! @details Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
		void exibirDadosObjeto(TableObject *object, int idx_lin);

		//! @details Seleciona a tabela de objetos de acordo com o tipo passado
		TabelaObjetosWidget *selecionarTabelaObjetos(ObjectType tipo_obj);

		//! @details Seleciona o tipo de objeto de acordo com o objeto sender informado
		ObjectType selecionarTipoObjeto(QObject *tab_sender);

		//! @details Exibe o formulário de edição de objetos de tabela conforme o tipo passado
		void exibirFormObjetoTabela(ObjectType tipo_obj);

	public:
		TabelaWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Table *table, float pos_x, float pos_y);

	private slots:
		void hideEvent(QHideEvent *);

		//! @details Adiciona ou edita um objeto da tabela a qual aciona o método
		void manipularObjeto(void);
		//! @details Remove um objeto selecionado  na tabela a qual aciona o método
		void removerObjeto(int idx_lin);
		//! @details Remove todos os objetos da tabela a qual aciona o método
		void removerObjetos(void);
		//! @details Faz a movimentação entre dois objetos da tabela
		void moverObjetos(int idx1, int idx2);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);
};

#endif
