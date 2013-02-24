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
\class ModelObjectsWidget
\brief Definição da classe que implementa a arvore e lista de objetos no modelo de banco de dados.
*/

#ifndef MODEL_OBJECTS_WIDGET_H
#define MODEL_OBJECTS_WIDGET_H

#include <QtGui>
#include "ui_modelobjectswidget.h"
#include "modelowidget.h"
#include "messagebox.h"

class ModelObjectsWidget: public QDockWidget, public Ui::ModelObjectsWidget {
	private:
		Q_OBJECT

		/*! \brief Indica que a visão de objetos é usada de forma simplificada como
			seletora de objetos auxiliar de outros formulários. Interações
			como excluir, destacar, editar e exibir código dos objetos
			são desabilitadas */
		bool	simplified_view,

					save_tree_state;

		/*! \brief Armazena o endereço do objeto relacionado ao item marcado na árvore
		 ou na lista de objetos */
		BaseObject *selected_object;

		/*! \brief Armazena a configuração de tamanho inicial do splitter para
		 para uso em conjunto com o botão de exibição das configurações
		 da visão de objetos.
		 Armazena a configuração de posição da slider da barra de rolagem
		 vertical da árvore de objetos para que a mesma seja restaurada
		 sempre que a mesma sofrer atualizações */
		QSettings widgets_conf;

		//! \brief Widget de Modelo de objetos o qual é acessado
		ModeloWidget *model_wgt;

		//! \brief Modelo o qual é acessado quando um modelo widget não é especificado
		DatabaseModel *db_model;

		//! \brief Armazena quais os tipos de objetos são visíveis na visão
		map<ObjectType, bool> visible_objs_map;

		//! \brief Atualiza a árvore inteira do banco de dados
		void updateDatabaseTree(void);

		//! \brief Atualiza somente a árvore de esquema e seus subitens
		void updatedSchemaTree(QTreeWidgetItem *root);

		//! \brief Atualiza somente a árvore de tabelas em um determinado esquema
		void updateTableTree(QTreeWidgetItem *root, BaseObject *schema);

		//! \brief Atualiza a arvore de objetos
		void updateObjectsTree(void);

		//! \brief Atualiza a lista de objetos
		void updateObjectsList(void);

		//! \brief Retorna um item da árvore relacionado ao objeto passado
		QTreeWidgetItem *getTreeItem(BaseObject *object);

		/*! \brief Gera um valor em um objeto QVariant para armazenamento dos
		 endereços dos objetos do modelo para armazenamento em
		 itens de QTreeWidgets e QListWidgetItem para permitir
		 a interação entre direta como os objetos sem selecioná-los
		 no modelo */
		QVariant generateItemValue(BaseObject *object);

		//! \brief Implementa a movimentação da janela quando esta é exibida de forma simplificada
		void mouseMoveEvent(QMouseEvent *);
		void closeEvent(QCloseEvent *);
		void showEvent(QShowEvent *);

	public:
		ModelObjectsWidget(bool simplified_view=false, QWidget * parent = 0, Qt::WindowFlags f = 0);

		BaseObject *getSelectedObject(void);

	protected:
		//! \brief Salva os itens atualmente expandidos na árvore no vetor passado
		void saveTreeState(vector<BaseObject *> &tree_items);

		//! \brief Restaura a árvore ao estado anterior expandindo os itens do vetor passado
		void restoreTreeState(vector<BaseObject *> &tree_items);

		//! \brief Indica ao widget que o estado da árvore de objetos deve ser salvo/restaurado automaticamente
		void saveTreeState(bool value);

	public slots:
		void setModel(ModeloWidget *model_wgt);
		void setModel(DatabaseModel *db_model);
		void changeObjectsView(void);
		void updateObjectsView(void);
		void setObjectVisible(ObjectType obj_type, bool visible);
		void close(void);

	private slots:
		void setObjectVisible(QListWidgetItem *item);
		void setAllObjectsVisible(bool);
		void selectObject(void);
		void showObjectMenu(void);
		void editObject(void);

	signals:
		void s_visibilityChanged(BaseObject *,bool);

		friend class FormPrincipal;
};

#endif
