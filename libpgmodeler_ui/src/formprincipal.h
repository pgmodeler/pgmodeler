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
#include "modelowidget.h"
#include "formsobre.h"
#include "messagebox.h"
#include "formbasico.h"
#include "formrestauracaomodelo.h"
#include "listaoperacoeswidget.h"
#include "visaoobjetoswidget.h"
#include "pgmodelerplugin.h"
#include "visaogeralwidget.h"

using namespace std;

class FormPrincipal: public QMainWindow, public Ui::FormPrincipal {
		Q_OBJECT

	private:
		//! @details Timer de salvamento automático do modelo
		QTimer tm_salvamento,
		tm_salvamento_tmp;

		//! @details Widget de visão geral do modelo
		VisaoGeralWidget *visaogeral_wgt;

		//! @details Formulário de restauração de modelos
		FormRestauracaoModelo *frestmodelo;

		//! @details Dockwidget o qual exibe as operações executadas sobre os objetos
		ListaOperacoesWidget *lista_oper;

		//! @details Dockwidget o qual exibe todos os objetos do modelo em forma de árvore/lista
		VisaoObjetosWidget *visao_objs;

		//! @details Armazena a referência para o modelo atualmente aberto
		ModeloWidget *modelo_atual;

		map<ModeloWidget *, vector<BaseObject *> > confs_arv_objs;

		//! @details Armazena o nome da operação atualmente selecionada
		QLabel *nome_op;

		//! @details Armazena o nome ícone da operação atualmente selecionada
		QLabel *icone_op;

		//! @details Título da janela principal
		QString titulo_janela;

		//! @details Armazena o intevalo de salvamento automático do modelo
		int interv_salvar;

		//! @details Sobrecarga do closeEvent(): Salva as configurações antes do encerramento do aplicativo
		void closeEvent(QCloseEvent *);

		//! @details Sobrecarga do showEvent(): Insere as ações do plugin na barra de plugins (se houver)
		void showEvent(QShowEvent *);

		//! @details Carrega os plugins no diretório plugins/
		void carregarPlugins(void);

	public:
		FormPrincipal(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~FormPrincipal(void);

	private slots:
		//! @details Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void definirOpcoesGrade(void);

		/*! @details Adiciona um novo modelo, caso o parâmetro 'nome_arq' esteja preenchido, o pgModeler
		 tentará carregar o modelo especificado pelo arquivo */
		void adicionarNovoModelo(const QString &nome_arq="");

		/*! @details Atualiza o estado (ativo/inativo) dos botões das barras
		 de ferramentas de acordo com a ação de abertura ou
		 fechamento de modelo */
		void atualizarEstadoFerramentas(bool modelo_fechado=false);
		void __atualizarEstadoFerramentas(void);

		/*! @details Efetua a atualização dos dockwidgets de visão geral,
		 lista de operações e arvore de objetos */
		void atualizarDockWidgets(void);
		void __atualizarDockWidgets(void);

		//! @details Atualiza a referência ao modelo atual quando as abas de modelos são ativadas
		void definirModeloAtual(void);

		/*! @details Executa o fechamento do modelo atual, o parâmetro 'idx_modelo' pode ser usado
		 para fechar um modelo especificado por seu índice */
		void fecharModelo(int idx_modelo=-1);

		//! @details Carrega um modelo a partir do disco
		void carregarModelo(void);

		//! @details Salva um determinado ModeloWidget
		void salvarModelo(ModeloWidget *modelo=NULL);

		//! @details Salva todos os modelos abertos
		void salvarTodosModelos(void);

		//! @details Imprime o modelo atual
		void imprimirModelo(void);

		//! @details Executa a exportação no modelo atual
		void exportarModelo(void);

		//! @details Atualiza os modelos abertos com as novas configurações
		void atualizarModelos(void);

		//! @details Aplica o zoom sobre o modelo atual
		void aplicarZoom(void);

		//! @details Exibe o pgModeler em tela cheia
		void exibirTelaCheia(bool tela_cheixa);

		//! @details Executa o plugin representado pela ação que disparou o slot
		void executarPlugin(void);

		//! @details Salva o modelo atualmente editados na pasta temporária em caso de crash o mesmo pode ser restaurado
		void salvarModeloTemporario(void);

		//! @details Exibe a visão geral do modelo atualmente aberto
		void exibirVisaoGeral(bool exibir);

		//! @details Atualiza o nome da aba do modelo atual caso o nome do banco de dados seja alterado
		void atualizarNomeAba(void);
};

#endif
