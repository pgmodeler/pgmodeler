/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ModeloWidget
# Descrição: Definição da classe que implementa o modelo de banco de dados
#            em sua forma de widget, ou seja, que permite a interação do usuário
#            para criação de objetos gráficos e outras operações sobre os mesmos.
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License

#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef MODELO_WIDGET_H
#define MODELO_WIDGET_H

#include <QtGui>
#include "modelobd.h"
#include "listaoperacoes.h"
#include "caixamensagem.h"
#include "formbasico.h"
#include "cenaobjetos.h"
#include "progressotarefa.h"
//***********************************************************
class ModeloWidget: public QWidget {
 Q_OBJECT

 private:
  //Fator de zoom atual da cena de objetos
  float zoom_atual;

  bool modificado;

 protected:
  //Constantes usadas nas operações de zoom
  static const float ZOOM_MINIMO=0.35f,
                     ZOOM_MAXIMO=6.0f,
                     INC_ZOOM=0.05f;

  /* Flag que indica se o modelo está em uma operação de recorte.
     Essa flag modifica o funcionamento dos métodos colarObjetos, excluirObjeto e
     copiarObjetos() */
  static bool op_recortar;

  //Armazena a nível de classe o modelo que deu origem a operação de copia/recorte
  static ModeloWidget *modelo_orig;

  //Objetos copiados no modelo de origem
  static vector<ObjetoBase *> objs_copiados;

  //Frame que indica que o modelo está protegido
  QFrame *modelo_protegido_frm;

  //Cena de objetos gráficos do modelo
  CenaObjetos *cena;

  //Viewport que gerencia a cena de objetos
  QGraphicsView *viewport;

        //Menu popup geral do modelo
  QMenu menu_popup,
        //Menu de inserção de novo objeto no modelo
        menu_novo_obj;

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
          *action_novo_obj;

  //Ações de inserção de novos objetos no modelo
  map<TipoObjetoBase, QAction *> acoes_ins_objs;

  //Armazena os objetos do modelo selecionados na cena
  vector<ObjetoBase *> objs_selecionados;

  //Tipo do objeto a ser inserido no modelo
  TipoObjetoBase tipo_novo_obj;

  //Lista de operações executadas sobre os objetos do modelo
  ListaOperacoes *lista_op;

  //Modelo de objetos do widget
  ModeloBD *modelo;

  //Armazena o nome do arquivo de modelo carregado no widget
  QString nome_arquivo;

  /* Configura a cena alinhando os objetos e a redimensionando quandos
     os objetos extrapolam o tamanho máximo dela */
  void ajustarTamanhoCena(void);

  void resizeEvent(QResizeEvent *);
  void mousePressEvent(QMouseEvent *evento);
  void keyPressEvent(QKeyEvent *evento);
  void focusInEvent(QFocusEvent *evento);

  //Cancela a operação de adição de novo objeto (valido apenas para objetos gráficos)
  void cancelarAdicaoObjeto(void);

  //Desabilita as ações do modelo quando uma ação de adição de objeto gráfica está ativa
  void desabilitarAcoesModelo(void);

 public:
   ModeloWidget(QWidget *parent = 0);
  ~ModeloWidget(void);

  //Obtém o nome do arquivo que deu origem ao modelo
  QString obterNomeArquivo(void);

  //Exibe o formulário de edição do objeto conforme o tipo passado
  void exibirFormObjeto(TipoObjetoBase tipo_obj, ObjetoBase *objeto=NULL, ObjetoBase *objeto_pai=NULL, QPointF pos=QPointF(NAN, NAN));

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
  void manipularAdicaoObjeto(ObjetoBase *objeto);
  void manipularRemocaoObjeto(ObjetoBase *objeto);
  void manipularMovimentoObjetos(bool fim_movimento);
  void manipularModificacaoObjeto(ObjetoGraficoBase *objeto);
  void manipularDuploCliqueObjeto(ObjetoGraficoBase *objeto);

  //Configura o menu popup conforme a lista de objetos passada
  void configurarMenuPopup(vector<ObjetoBase *> objs_selecionados=vector<ObjetoBase *>());

  //Exibe um menu popup específico para objetos de tabela
  void exibirMenuObjetoTabela(vector<ObjetoBase *> objs_selecionados);

  //Exibe as dependências e referências do objeto
  void exibirDepsRefs(void);

  //Exibe o formulário de edição do objeto selecionado
  void editarObjeto(void);

  //Protege os objetos selecionados
  void protegerObjeto(void);

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

  friend class FormPrincipal;
  friend class ListaOperacoesWidget;
  friend class VisaoObjetosWidget;
};
//***********************************************************
#endif
