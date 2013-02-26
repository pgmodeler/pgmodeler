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
\class MainWindow
\brief Definição da classe que implementa o formulário principal do software,
 Reune e gerencia todas as classes das bibliotecas implementadas.
*/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "modelwidget.h"
#include "aboutform.h"
#include "messagebox.h"
#include "baseform.h"
#include "modelrestorationform.h"
#include "operationlistwidget.h"
#include "modelobjectswidget.h"
#include "pgmodelerplugin.h"
#include "modeloverviewwidget.h"

using namespace std;

class MainWindow: public QMainWindow, public Ui::MainWindow {
	private:
		Q_OBJECT

		//! \brief Timer de salvamento automático do modelo
		QTimer model_save_timer,	tmpmodel_save_timer;

		MessageBox msg_box;

		//! \brief Widget de visão geral do modelo
		ModelOverviewWidget *overview_wgt;

		//! \brief Formulário de restauração de modelos
		ModelRestorationForm *restoration_form;

		//! \brief Dockwidget o qual exibe as operações executadas sobre os objetos
		OperationListWidget *oper_list_wgt;

		//! \brief Dockwidget o qual exibe todos os objetos do modelo em forma de árvore/lista
		ModelObjectsWidget *model_objs_wgt;

		//! \brief Armazena a referência para o modelo atualmente aberto
		ModelWidget *current_model;

		map<ModelWidget *, vector<BaseObject *> > model_tree_states;

		//! \brief Título da janela principal
		QString window_title;

		//! \brief Armazena o intevalo de salvamento automático do modelo
		int save_interval;

		//! \brief Sobrecarga do closeEvent(): Salva as configurações antes do encerramento do aplicativo
		void closeEvent(QCloseEvent *);

		//! \brief Sobrecarga do showEvent(): Insere as ações do plugin na barra de plugins (se houver)
		void showEvent(QShowEvent *);

		//! \brief Carrega os plugins no diretório plugins/
		void carregarPlugins(void);

	public:
		MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow(void);

	public slots:
		/*! \brief Adiciona um novo modelo, caso o parâmetro 'nome_arq' esteja preenchido, o pgModeler
		 tentará carregar o modelo especificado pelo arquivo */
		void addModel(const QString &filename="");

		/*! \brief Executa o fechamento do modelo atual, o parâmetro 'idx_modelo' pode ser usado
		 para fechar um modelo especificado por seu índice */
		void closeModel(int model_id=-1);

		int getModelCount(void);

		ModelWidget *getModel(int idx);

	private slots:
		//! \brief Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void setGridOptions(void);

		/*! \brief Atualiza o estado (ativo/inativo) dos botões das barras
		 de ferramentas de acordo com a ação de abertura ou
		 fechamento de modelo */
		void updateToolsState(bool model_closed=false);
		void __updateToolsState(void);

		/*! \brief Efetua a atualização dos dockwidgets de visão geral,
		 lista de operações e arvore de objetos */
		void updateDockWidgets(void);
		void __updateDockWidgets(void);

		//! \brief Atualiza a referência ao modelo atual quando as abas de modelos são ativadas
		void setCurrentModel(void);

		//! \brief Carrega um modelo a partir do disco
		void loadModel(void);

		//! \brief Salva um determinado ModeloWidget
		void saveModel(ModelWidget *model=NULL);

		//! \brief Salva todos os modelos abertos
		void saveAllModels(void);

		//! \brief Imprime o modelo atual
		void printModel(void);

		//! \brief Executa a exportação no modelo atual
		void exportModel(void);

		//! \brief Atualiza os modelos abertos com as novas configurações
		void updateModels(void);

		//! \brief Aplica o zoom sobre o modelo atual
		void applyZoom(void);

		//! \brief Exibe o pgModeler em tela cheia
		void showFullScreen(bool fullscreen);

		//! \brief Executa o plugin representado pela ação que disparou o slot
		void executePlugin(void);

		//! \brief Salva o modelo atualmente editados na pasta temporária em caso de crash o mesmo pode ser restaurado
		void saveTemporaryModel(void);

		//! \brief Exibe a visão geral do modelo atualmente aberto
		void showOverview(bool show);

		//! \brief Atualiza o nome da aba do modelo atual caso o nome do banco de dados seja alterado
		void updateModelTabName(void);
};

#endif
