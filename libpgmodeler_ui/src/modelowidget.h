/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: ModeloWidget
# Description:Definição da classe que implementa o modelo de banco de dados
#            em sua forma de widget, ou seja, que permite a interação do usuário
#            para criação de objetos gráficos e outras operações sobre os mesmos.
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
#ifndef MODELO_WIDGET_H
#define MODELO_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "operationlist.h"
#include "caixamensagem.h"
#include "formbasico.h"
#include "objectsscene.h"
#include "progressotarefa.h"
//#include "visaogeralwidget.h"

class ModeloWidget: public QWidget {
 Q_OBJECT

 private:
  //Fator de zoom atual da cena de objetos
  float zoom_atual;

  //Indica que o modelo foi modificado por alguma operação
  bool modificado;

 protected:
  //Constantes usadas nas operações de zoom
  static const float ZOOM_MINIMO=0.35f,
                     ZOOM_MAXIMO=4.0f,
                     INC_ZOOM=0.05f;

  /* Flag que indica se o modelo está em uma operação de recorte.
     Essa flag modifica o funcionamento dos métodos colarObjetos, excluirObjeto e
     copiarObjetos() */
  static bool op_recortar;

  //Armazena a nível de classe o modelo que deu origem a operação de copia/recorte
  static ModeloWidget *modelo_orig;

  //Objetos copiados no modelo de origem
  static vector<BaseObject *> objs_copiados;

  //Frame que indica que o modelo está protegido
  QFrame *modelo_protegido_frm;

  //Cena de objetos gráficos do modelo
  ObjectsScene *cena;

  //Viewport que gerencia a cena de objetos
  QGraphicsView *viewport;

        //Menu popup geral do modelo
  QMenu menu_popup,
        //Menu de inserção de novo objeto no modelo
        menu_novo_obj,
        menu_acoes_rapidas;

  //Ações do menu popup
  QAction *action_codigo_fonte,
          *action_editar,
          *action_proteger,
          *action_desproteger,
          *action_excluir,
          *action_selecionar_todos,
          *action_converter_relnn,
          *action_copiar,
          *action_colar,
          *action_recortar,
          *action_deps_refs,
          *action_novo_obj,
          *action_rename,
          *action_acoes_rapidas;

  //Ações de inserção de novos objetos no modelo
  map<ObjectType, QAction *> acoes_ins_objs;

  //Armazena os objetos do modelo selecionados na cena
  vector<BaseObject *> objs_selecionados;

  //Tipo do objeto a ser inserido no modelo
  ObjectType tipo_novo_obj;

  //Lista de operações executadas sobre os objetos do modelo
  OperationList *lista_op;

  //Modelo de objetos do widget
  DatabaseModel *modelo;

  //Armazena o nome do arquivo de modelo carregado no widget
  QString nome_arquivo,
          nome_arquivo_tmp;

  /* Configura a cena alinhando os objetos e a redimensionando quandos
     os objetos extrapolam o tamanho máximo dela */
  void ajustarTamanhoCena(void);

  void resizeEvent(QResizeEvent *);
  void mousePressEvent(QMouseEvent *evento);
  void keyPressEvent(QKeyEvent *evento);
  void focusInEvent(QFocusEvent *evento);
  void keyReleaseEvent(QKeyEvent *evento);

  //Modifica o zoom do viewport quando se pressiona CONTROL e utiliza o wheel do mouse
  void wheelEvent(QWheelEvent * evento);

  /* Filtro de eventos usados quando é necessário desviar eventos de objetos filhos.
     Um desses desvios é o tratamento do WheelEvent das barras de rolagens do viewport
     pelo modelo widget, isso faz com que o WheelEvent seja executado no Modelo e não
     nas barras */
  bool eventFilter(QObject *objeto, QEvent *evento);

  //Cancela a operação de adição de novo objeto (valido apenas para objetos gráficos)
  void cancelarAdicaoObjeto(void);

  //Desabilita as ações do modelo quando uma ação de adição de objeto gráfica está ativa
  void desabilitarAcoesModelo(void);

 public:
   ModeloWidget(QWidget *parent = 0);
  ~ModeloWidget(void);

  void definirModificado(bool valor);

  //Obtém o nome do arquivo que deu origem ao modelo
  QString getNameArquivo(void);

  //Obtém o nome temporário do arquivo usado para salvar uma cópia de segurança na pasta tmp do pgModeler
  QString getNameArquivoTemp(void);

  //Exibe o formulário de edição do objeto conforme o tipo passado
  void exibirFormObjeto(ObjectType tipo_obj, BaseObject *objeto=NULL, BaseObject *objeto_pai=NULL, QPointF pos=QPointF(NAN, NAN));

  //Aplica um zoom ao modelo
  void aplicarZoom(float zoom);

  //Retorna o zoom atual em que se encontra o modelo
  float zoomAtual(void);

  //Retorna se o modelo foi modificado ou não
  bool modeloModificado(void);

 private slots:
  /* Os slots manipular*() gerenciam os sinais enviados pela cena e modelo para execução
     de operações adicionais como incluir objetos modificados na lista de operações, criar
     objetos na cena e remover objetos da cena de forma automática */
  void manipularAdicaoObjeto(BaseObject *objeto);
  void manipularRemocaoObjeto(BaseObject *objeto);
  void manipularMovimentoObjetos(bool fim_movimento);
  void manipularModificacaoObjeto(BaseGraphicObject *objeto);
  void manipularDuploCliqueObjeto(BaseGraphicObject *objeto);

  //Configura o menu popup conforme a lista de objetos passada
  void configurarMenuPopup(vector<BaseObject *> objs_selecionados=vector<BaseObject *>());

  //Exibe um menu popup específico para objetos de tabela
  void exibirMenuObjetoTabela(vector<BaseObject *> objs_selecionados);

  //Exibe as dependências e referências do objeto
  void exibirDepsRefs(void);

  //Exibe o formulário de edição do objeto selecionado
  void editarObjeto(void);

  //Protege os objetos selecionados
  void protegerObjeto(void);

  //Executa o widget de renomeio de objeto
  void renomearObjeto(void);

  //Exclui os objetos selecionados
  void excluirObjetos(void);

  //Seleciona todos os objetos no modelo
  void selecionarTodos(void);

  //Copia todos os objetos selecionados no modelo
  void copiarObjetos(void);

  //Cola todos os objetos no modelo
  void colarObjetos(void);

  /* Recorta os objetos selecionados no modelo. Este método executa
     apenas a cópia de objetos, marcando a flag op_recortar e setando
     o modelo de origem. O restante da operação de recorte que é exlcuir
     os objetos selecionados é executado no método colarObjetos() */
  void recortarObjetos(void);

  //Faz a conversão de um relacionamento n-n
  void converterRelacionamentoNN(void);

  //Exibe o código fonte do objeto selecionado
  void exibirCodigoFonte(void);

  //Adiciona um novo objeto ao modelo ou tabela selecionada
  void adicionarNovoObjeto(void);

  /* Configura a lista de objetos selecionados toda vez que o
     sinal selectionChanged() vindo da cena é disparado */
  void configurarSelecaoObjetos(void);

  /* Retorna um vetor com as dimensões das páginas que contém objetos
     para serem impressos */
  vector<QRectF> obterPaginasImpressao(const QSizeF &tam_papel, unsigned &qtd_pag_h, unsigned &qtd_pag_v);

  //Exibe o widget de visão geral. Para fechá-la o usuário pode dar um duplo-clique ou pressionar esc.
  //void exibirVisaoGeral(bool exibir);

 public slots:
   void carregarModelo(const QString &nome_arq);
   void salvarModelo(const QString &nome_arq);
   void salvarModelo(void);

   /* Imprime o modelo no objeto QPrinter passado. Os dois parâmetros bool
      são usados para exibir ou não a grade e número de páginas na impressão */
   void imprimirModelo(QPrinter *printer, bool exibir_grade_imp, bool imp_num_pag);

 signals:
  /* Sinais personalizados usados para sinalizarem
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
  friend class FormExportacao;
  friend class ListaOperacoesWidget;
  friend class VisaoObjetosWidget;
  friend class VisaoGeralWidget;
};

#endif
