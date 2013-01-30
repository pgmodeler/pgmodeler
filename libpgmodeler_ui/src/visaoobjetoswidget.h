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
\class VisaoObjetosWidget
\brief Definição da classe que implementa a arvore e lista de objetos no modelo de banco de dados.
*/

#ifndef VISAO_OBJETOS_WIDGET_H
#define VISAO_OBJETOS_WIDGET_H

#include <QtGui>
#include "ui_visaoobjetoswidget.h"
#include "modelowidget.h"
#include "messagebox.h"

class VisaoObjetosWidget: public QDockWidget, public Ui::VisaoObjetosWidget {
		Q_OBJECT

	private:
		/*! @details Indica que a visão de objetos é usada de forma simplificada como
			seletora de objetos auxiliar de outros formulários. Interações
			como excluir, destacar, editar e exibir código dos objetos
			são desabilitadas */
		bool	visao_simplificada,

					salvar_arvore;

		/*! @details Armazena o endereço do objeto relacionado ao item marcado na árvore
		 ou na lista de objetos */
		BaseObject *objeto_selecao;

		/*! @details Menu popup o qual contém as ações: destacar objeto no modelo,
		 excluir do modelo, edição do objeto */
		QMenu *menu_popup;

		/*! @details Armazena a configuração de tamanho inicial do splitter para
		 para uso em conjunto com o botão de exibição das configurações
		 da visão de objetos.
		 Armazena a configuração de posição da slider da barra de rolagem
		 vertical da árvore de objetos para que a mesma seja restaurada
		 sempre que a mesma sofrer atualizações */
		QSettings config_widgets;

		//! @details Widget de Modelo de objetos o qual é acessado
		ModeloWidget *modelo_wgt;

		//! @details Modelo o qual é acessado quando um modelo widget não é especificado
		DatabaseModel *modelo_bd;

		//! @details Armazena quais os tipos de objetos são visíveis na visão
		map<ObjectType, bool> map_objs_visiveis;

		//! @details Atualiza a árvore inteira do banco de dados
		void atualizarSubBancoDados(void);

		//! @details Atualiza somente a árvore de esquema e seus subitens
		void atualizarSubArvoreEsquema(QTreeWidgetItem *raiz);

		//! @details Atualiza somente a árvore de tabelas em um determinado esquema
		void atualizarSubArvoreTabela(QTreeWidgetItem *raiz, BaseObject *esquema);

		//! @details Atualiza a arvore de objetos
		void atualizarArvoreObjetos(void);

		//! @details Atualiza a lista de objetos
		void atualizarListaObjetos(void);

		//! @details Retorna um item da árvore relacionado ao objeto passado
		QTreeWidgetItem *obterItemArvore(BaseObject *objeto);

		/*! @details Gera um valor em um objeto QVariant para armazenamento dos
		 endereços dos objetos do modelo para armazenamento em
		 itens de QTreeWidgets e QListWidgetItem para permitir
		 a interação entre direta como os objetos sem selecioná-los
		 no modelo */
		QVariant gerarValorItem(BaseObject *objeto);

		//! @details Implementa a movimentação da janela quando esta é exibida de forma simplificada
		void mouseMoveEvent(QMouseEvent *);
		void closeEvent(QCloseEvent *);
		void showEvent(QShowEvent *);

	public:
		VisaoObjetosWidget(bool visao_simplificada=false, QWidget * parent = 0, Qt::WindowFlags f = 0);
		BaseObject *obterObjetoSelecao(void);

	protected:
		//! @details Salva os itens atualmente expandidos na árvore no vetor passado
		void salvarEstadoArvore(vector<BaseObject *> &itens_arv);

		//! @details Restaura a árvore ao estado anterior expandindo os itens do vetor passado
		void restaurarEstadoArvore(vector<BaseObject *> &itens_arv);

		//! @details Indica ao widget que o estado da árvore de objetos deve ser salvo/restaurado automaticamente
		void salvarEstadoArvore(bool valor);

	public slots:
		void definirModelo(ModeloWidget *modelo_wgt);
		void definirModelo(DatabaseModel *modelo_bd);
		void mudarVisaoObjetos(void);
		void atualizarVisaoObjetos(void);
		void definirObjetoVisivel(ObjectType tipo_obj, bool visivel);
		void close(void);

	private slots:
		void definirObjetoVisivel(QListWidgetItem *item);
		void definirTodosObjetosVisiveis(bool);
		void selecionarObjeto(void);
		void exibirMenuObjeto(void);
		void editarObjeto(void);

	signals:
		/*! @details Sinais personalizados usados para sinalizarem
		 a modificação da visão de objetos. Este sinal é capturado pelo
		 form principal para atualizar as ferramentas */
		void s_visaoObjetosModificada(void);
		void s_visibilityChanged(BaseObject *,bool);

		friend class FormPrincipal;
};

#endif
