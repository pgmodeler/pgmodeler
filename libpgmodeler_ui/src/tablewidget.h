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
\class TableWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de tabela.
*/

#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tablewidget.h"
#include "objecttablewidget.h"
#include "tableview.h"

class TableWidget: public BaseObjectWidget, public Ui::TableWidget {
	private:
		Q_OBJECT

		/*! \brief Quantidade de elementos na lista de operações antes da tabela
			ser editada. Este atributo é usado para se saber, em caso de cancelamento
			da edição da tabela, a quantidade de operações relacionadas ao
			objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
		unsigned operation_count;

		//! \brief Armazena as tabelas de objetos filhos da tabela (colunas, restrições, índices, etc)
		map<ObjectType, ObjectTableWidget *> objects_tab_map;

		/*! \brief Lista os objetos do relacionamento na tabela respectiva, de acordo
			com o tipo do objeto passado */
		void listObjects(ObjectType obj_type);

		//! \brief Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
		void showObjectData(TableObject *object, int row);

		//! \brief Seleciona a tabela de objetos de acordo com o tipo passado
		ObjectTableWidget *selectObjectTable(ObjectType obj_type);

		//! \brief Seleciona o tipo de objeto de acordo com o objeto sender informado
		ObjectType selectObjectType(QObject *sender);

		//! \brief Exibe o formulário de edição de objetos de tabela conforme o tipo passado
		void showTableObjectForm(ObjectType obj_type);

		void hideEvent(QHideEvent *event);

	public:
		TableWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Table *table, float pos_x, float pos_y);

	private slots:
		//! \brief Adiciona ou edita um objeto da tabela a qual aciona o método
		void handleObject(void);

		//! \brief Remove um objeto selecionado  na tabela a qual aciona o método
		void removeObject(int row);

		//! \brief Remove todos os objetos da tabela a qual aciona o método
		void removeObjects(void);

		//! \brief Faz a movimentação entre dois objetos da tabela
		void moveObjects(int idx1, int idx2);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);
};

#endif
