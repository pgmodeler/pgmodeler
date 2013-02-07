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
\class PapelWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de esquemas.
*/

#ifndef ROLE_WIDGET_H
#define ROLE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_rolewidget.h"
#include "tabelaobjetoswidget.h"

class RoleWidget: public BaseObjectWidget, public Ui::RoleWidget {
	private:
		Q_OBJECT

		/*! \brief Armazena as referência s tabelas de membros do papel para que sejam
			referenciados nos demais métodos de forma mais eficaz */
		TabelaObjetosWidget *members_tab[3];

		//! \brief Preenche as tabelas de membros do papel
		void fillMembersTable(void);

		//! \brief Exibe o dado do papel na tabela especificada
		void showRoleData(Role *papel, unsigned idx_tabela, unsigned lin);

		void hideEvent(QHideEvent *event);

	public:
		RoleWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Role *role);

	private slots:
		/*! \brief Preenche com informações relacionadas ao papel selecionado
			na linha atual da tabela em foco */
		void showSelectedRoleData(void);

		/*! \brief Seleciona um papel membro para ser inserido na tabela de membro
			atualmente em foco. Este slot é usando tanto para adicionar
			um membro na tabela quanto para editar um membro selecionado */
		void selectMemberRole(void);

		/*! \brief Faz a conexão de sinais-slots para cada tabela de objetos
				medida que a tabela em foco mude */
		void configureRoleSelection(void);

	public slots:
		void applyConfiguration(void);
};

#endif
