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
\class ModelWidget
\brief Definição da classe que implementa o modelo de banco de dados
	em sua forma de widget, ou seja, que permite a interação do usuário
	para criação de objetos gráficos e outras operações sobre os mesmos.
*/

#ifndef MODEL_WIDGET_H
#define MODEL_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "operationlist.h"
#include "messagebox.h"
#include "baseform.h"
#include "objectsscene.h"
#include "taskprogresswidget.h"

class ModelWidget: public QWidget {
	private:
		Q_OBJECT

		MessageBox msg_box;

		//! \brief Fator de zoom atual da cena de objetos
		float current_zoom;

		//! \brief Indica que o modelo foi modificado por alguma operação
		bool modified;

		bool isReservedObject(BaseObject *obj);

		void configureSubmenu(BaseObject *obj);

		/*! \brief Flag que indica se o modelo está em uma operação de recorte.
		 Essa flag modifica o funcionamento dos métodos colarObjetos, excluirObjeto e
		 copiarObjetos() */
		static bool cut_operation;

		//! \brief Armazena a nível de classe o modelo que deu origem a operação de copia/recorte
		static ModelWidget *src_model;

		//! \brief Objetos copiados no modelo de origem
		static vector<BaseObject *> copied_objects;

		//! \brief Frame que indica que o modelo está protegido
		QFrame *protected_model_frm;

		//! \brief Cena de objetos gráficos do modelo
		ObjectsScene *scene;

		//! \brief Viewport que gerencia a cena de objetos
		QGraphicsView *viewport;

		//! \brief Menu popup geral do modelo
		QMenu popup_menu,
					//! \brief Menu de inserção de novo objeto no modelo
					new_object_menu,
					quick_actions_menu,
					schemas_menu,
					owners_menu;

		//! \brief Ações do menu popup
		QAction *action_source_code,
						*action_edit,
						*action_protect,
						*action_unprotect,
						*action_remove,
						*action_select_all,
						*action_convert_relnn,
						*action_copy,
						*action_paste,
						*action_cut,
						*action_deps_refs,
						*action_new_object,
						*action_rename,
						*action_moveto_schema,
						*action_edit_perms,
						*action_change_owner,
						*action_quick_actions,
						*action_sel_sch_children;

		//! \brief Ações de inserção de novos objetos no modelo
		map<ObjectType, QAction *> actions_ins_objects;

		//! \brief Armazena os objetos do modelo selecionados na cena
		vector<BaseObject *> selected_objects;

		//! \brief Tipo do objeto a ser inserido no modelo
		ObjectType new_obj_type;

		//! \brief Lista de operações executadas sobre os objetos do modelo
		OperationList *op_list;

		//! \brief Modelo de objetos do widget
		DatabaseModel *db_model;

		//! \brief Armazena o nome do arquivo de modelo carregado no widget
		QString filename,
						tmp_filename;

	protected:
		//! \brief Constantes usadas nas operações de zoom
		static const float	MINIMUM_ZOOM=0.35f,
												MAXIMUM_ZOOM=4.0f,
												ZOOM_INCREMENT=0.05f;

		/*! \brief Configura a cena alinhando os objetos e a redimensionando quandos
		 os objetos extrapolam o tamanho máximo dela */
		void adjustSceneSize(void);

		void resizeEvent(QResizeEvent *);
		void mousePressEvent(QMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void focusInEvent(QFocusEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

		//! \brief Modifica o zoom do viewport quando se pressiona CONTROL e utiliza o wheel do mouse
		void wheelEvent(QWheelEvent * event);

		/*! \brief Filtro de eventos usados quando é necessário desviar eventos de objetos filhos.
		 Um desses desvios é o tratamento do WheelEvent das barras de rolagens do viewport
		 pelo modelo widget, isso faz com que o WheelEvent seja executado no Modelo e não
		 nas barras */
		bool eventFilter(QObject *object, QEvent *event);

		//! \brief Cancela a operação de adição de novo objeto (valido apenas para objetos gráficos)
		void cancelObjectAddition(void);

		//! \brief Desabilita as ações do modelo quando uma ação de adição de objeto gráfica está ativa
		void disableModelActions(void);

	public:
		ModelWidget(QWidget *parent = 0);
		~ModelWidget(void);

		void setModified(bool value);

		//! \brief Obtém o nome do arquivo que deu origem ao modelo
		QString getFilename(void);

		//! \brief Obtém o nome temporário do arquivo usado para salvar uma cópia de segurança na pasta tmp do pgModeler
		QString getTempFilename(void);

		//! \brief Exibe o formulário de edição do objeto conforme o tipo passado
		void showObjectForm(ObjectType obj_type, BaseObject *object=NULL, BaseObject *parent_obj=NULL, QPointF pos=QPointF(NAN, NAN));

		//! \brief Aplica um zoom ao modelo
		void applyZoom(float zoom);

		//! \brief Retorna o zoom atual em que se encontra o modelo
		float currentZoom(void);

		//! \brief Retorna se o modelo foi modificado ou não
		bool isModified(void);

		//! \brief Returns the database model object
		DatabaseModel *getDatabaseModel(void);

	private slots:
		/*! \brief Os slots manipular*() gerenciam os sinais enviados pela cena e modelo para execução
		 de operações adicionais como incluir objetos modificados na lista de operações, criar
		 objetos na cena e remover objetos da cena de forma automática */
		void handleObjectAddition(BaseObject *object);
		void handleObjectRemoval(BaseObject *object);
		void handleObjectsMovement(bool end_moviment);
		void handleObjectModification(BaseGraphicObject *object);
		void handleObjectDoubleClick(BaseGraphicObject *object);

		//! \brief Configura o menu popup conforme a lista de objetos passada
		void configurePopupMenu(vector<BaseObject *> objects=vector<BaseObject *>());

		//! \brief Exibe um menu popup específico para objetos de tabela
		void configureObjectMenu(BaseObject *object);

		void showObjectMenu(void);

		//! \brief Exibe as dependências e referências do objeto
		void showDependenciesReferences(void);

		//! \brief Exibe o formulário de edição do objeto selecionado
		void editObject(void);

		//! \brief Protege os objetos selecionados
		void protectObject(void);

		//! \brief Executa o widget de renomeio de objeto
		void renameObject(void);

		void moveToSchema(void);

		void changeOwner(void);

		void editPermissions(void);

		void selectSchemaChildren(void);

		//! \brief Exclui os objetos selecionados
		void removeObjects(void);

		//! \brief Seleciona todos os objetos no modelo
		void selectAllObjects(void);

		//! \brief Copia todos os objetos selecionados no modelo
		void copyObjects(void);

		//! \brief Cola todos os objetos no modelo
		void pasteObjects(void);

		/*! \brief Recorta os objetos selecionados no modelo. Este método executa
		 apenas a cópia de objetos, marcando a flag op_recortar e setando
		 o modelo de origem. O restante da operação de recorte que é exlcuir
		 os objetos selecionados é executado no método colarObjetos() */
		void cutObjects(void);

		//! \brief Faz a conversão de um relacionamento n-n
		void convertRelationshipNN(void);

		//! \brief Exibe o código fonte do objeto selecionado
		void showSourceCode(void);

		//! \brief Adiciona um novo objeto ao modelo ou tabela selecionada
		void addNewObject(void);

		/*! \brief Configura a lista de objetos selecionados toda vez que o
		 sinal selectionChanged() vindo da cena é disparado */
		void configureObjectSelection(void);

		/*! \brief Retorna um vetor com as dimensões das páginas que contém objetos
		 para serem impressos */
		vector<QRectF> getPagesForPrinting(const QSizeF &paper_size, unsigned &h_page_cnt, unsigned &v_page_cnt);

		//! \brief Exibe o widget de visão geral. Para fechá-la o usuário pode dar um duplo-clique ou pressionar esc.
		//! \brief void exibirVisaoGeral(bool exibir);

	public slots:
		void loadModel(const QString &filename);
		void saveModel(const QString &filename);
		void saveModel(void);

		/*! \brief Imprime o modelo no objeto QPrinter passado. Os dois parâmetros bool
			são usados para exibir ou não a grade e número de páginas na impressão */
		void printModel(QPrinter *printer, bool print_grid, bool print_page_nums);

	signals:
		/*! \brief Sinais personalizados usados para sinalizarem
		 a modificação do modelo. Este sinal é capturado pelo
		 form principal para atualizar as ferramentas */
		void s_objetoModificado(void);
		void s_objetosMovimentados(void);
		void s_objetoCriado(void);
		void s_objetoRemovido(void);
		void s_selecaoObjetoAtualizada(void);
		void s_zoomModificado(float);
		void s_modeloRedimensionado(void);

		friend class FormPrincipal;
		friend class ModelExportForm;
		friend class OperationListWidget;
		friend class ModelObjectsWidget;
		friend class ModelOverviewWidget;
};

#endif
