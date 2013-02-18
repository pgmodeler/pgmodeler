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
\brief Definição da classe que implementa o formulário de edição dos atributos de relacionamentos.
*/

#ifndef RELATIONSHIP_WIDGET_H
#define RELATIONSHIP_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_relationshipwidget.h"
#include "tabelaobjetoswidget.h"

class RelationshipWidget: public BaseObjectWidget, public Ui::RelationshipWidget {
	private:
		Q_OBJECT
		/*! \brief Quantidade de elementos na lista de operações antes do relacionamento
			ser editado. Este atributo é usado para se saber, em caso de cancelamento
			da edição do relacionamento, a quantidade de operações relacionada ao
			objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
		unsigned operation_count;

		//! \brief Destacadores de sintaxe para os campos de nome da tabela de origem e destino
		SyntaxHighlighter *table1_hl,
											*table2_hl;

		//! \brief Tabela as quais armazenam os atributos e restrições do relacionamento
		TabelaObjetosWidget *attributes_tab,
												*constraints_tab,
												*advanced_objs_tab;

		/*! \brief Lista os objetos do relacionamento na tabela respectiva, de acordo
			com o tipo do objeto passado */
		void listObjects(ObjectType obj_type);

		//! \brief Lista os objetos avançados
		void listAdvancedObjects(void);

		//! \brief Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
		void showObjectData(TableObject *object, int row);

	protected:
		void setAttributes(DatabaseModel *model, OperationList *op_list, Table *src_tab, Table *dst_tab, unsigned rel_type);

	public:
		RelationshipWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseRelationship *base_rel);

	private slots:
		void hideEvent(QHideEvent *event);

		//! \brief Adiciona um objeto   tabela a qual aciona o método
		void addObject(void);

		//! \brief Edita um objeto selecionado  na tabela a qual aciona o método
		void editObject(int row);

		//! \brief Remove um objeto selecionado  na tabela a qual aciona o método
		void removeObject(int row);

		//! \brief Remove todos os objetos da tabela a qual aciona o método
		void removeObjects(void);

		//! \brief Exibe o formulário referente ao objeto criado ou que representa o relationamento
		void showAdvancedObject(int idx);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);

		friend class ModeloWidget;
};

#endif
