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
\class FormPrincipal
\brief Definição da classe que implementa o formulário principal do software,
 Reune e gerencia todas as classes das bibliotecas implementadas.
*/

#ifndef FORM_PRINCIPAL_H
#define FORM_PRINCIPAL_H

#include <QtGui>
#include "ui_formprincipal.h"
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

class FormPrincipal: public QMainWindow, public Ui::FormPrincipal {
		Q_OBJECT

	private:
		//! \brief Timer de salvamento automático do modelo
		QTimer tm_salvamento,
		tm_salvamento_tmp;

		MessageBox msg_box;

		//! \brief Widget de visão geral do modelo
		ModelOverviewWidget *visaogeral_wgt;

		//! \brief Formulário de restauração de modelos
		ModelRestorationForm *frestmodelo;

		//! \brief Dockwidget o qual exibe as operações executadas sobre os objetos
		OperationListWidget *lista_oper;

		//! \brief Dockwidget o qual exibe todos os objetos do modelo em forma de árvore/lista
		ModelObjectsWidget *visao_objs;

		//! \brief Armazena a referência para o modelo atualmente aberto
		ModelWidget *modelo_atual;

		map<ModelWidget *, vector<BaseObject *> > confs_arv_objs;

		//! \brief Armazena o nome da operação atualmente selecionada
		QLabel *nome_op;

		//! \brief Armazena o nome ícone da operação atualmente selecionada
		QLabel *icone_op;

		//! \brief Título da janela principal
		QString titulo_janela;

		//! \brief Armazena o intevalo de salvamento automático do modelo
		int interv_salvar;

		//! \brief Sobrecarga do closeEvent(): Salva as configurações antes do encerramento do aplicativo
		void closeEvent(QCloseEvent *);

		//! \brief Sobrecarga do showEvent(): Insere as ações do plugin na barra de plugins (se houver)
		void showEvent(QShowEvent *);

		//! \brief Carrega os plugins no diretório plugins/
		void carregarPlugins(void);

	public:
		FormPrincipal(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~FormPrincipal(void);

	private slots:
		//! \brief Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void definirOpcoesGrade(void);

		/*! \brief Adiciona um novo modelo, caso o parâmetro 'nome_arq' esteja preenchido, o pgModeler
		 tentará carregar o modelo especificado pelo arquivo */
		void adicionarNovoModelo(const QString &nome_arq="");

		/*! \brief Atualiza o estado (ativo/inativo) dos botões das barras
		 de ferramentas de acordo com a ação de abertura ou
		 fechamento de modelo */
		void atualizarEstadoFerramentas(bool modelo_fechado=false);
		void __atualizarEstadoFerramentas(void);

		/*! \brief Efetua a atualização dos dockwidgets de visão geral,
		 lista de operações e arvore de objetos */
		void atualizarDockWidgets(void);
		void __atualizarDockWidgets(void);

		//! \brief Atualiza a referência ao modelo atual quando as abas de modelos são ativadas
		void definirModeloAtual(void);

		/*! \brief Executa o fechamento do modelo atual, o parâmetro 'idx_modelo' pode ser usado
		 para fechar um modelo especificado por seu índice */
		void fecharModelo(int idx_modelo=-1);

		//! \brief Carrega um modelo a partir do disco
		void carregarModelo(void);

		//! \brief Salva um determinado ModeloWidget
		void salvarModelo(ModelWidget *modelo=NULL);

		//! \brief Salva todos os modelos abertos
		void salvarTodosModelos(void);

		//! \brief Imprime o modelo atual
		void imprimirModelo(void);

		//! \brief Executa a exportação no modelo atual
		void exportarModelo(void);

		//! \brief Atualiza os modelos abertos com as novas configurações
		void atualizarModelos(void);

		//! \brief Aplica o zoom sobre o modelo atual
		void aplicarZoom(void);

		//! \brief Exibe o pgModeler em tela cheia
		void exibirTelaCheia(bool tela_cheixa);

		//! \brief Executa o plugin representado pela ação que disparou o slot
		void executarPlugin(void);

		//! \brief Salva o modelo atualmente editados na pasta temporária em caso de crash o mesmo pode ser restaurado
		void salvarModeloTemporario(void);

		//! \brief Exibe a visão geral do modelo atualmente aberto
		void exibirVisaoGeral(bool exibir);

		//! \brief Atualiza o nome da aba do modelo atual caso o nome do banco de dados seja alterado
		void atualizarNomeAba(void);
};

#endif
