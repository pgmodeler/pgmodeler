/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: VisaoObjetosWidget
# Descrição: Definição da classe que implementa a arvore e lista de objetos
#            no modelo de banco de dados.
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
#ifndef VISAO_OBJETOS_WIDGET_H
#define VISAO_OBJETOS_WIDGET_H

#include <QtGui>
#include "ui_visaoobjetoswidget.h"
#include "modelowidget.h"
#include "caixamensagem.h"
//***********************************************************
class VisaoObjetosWidget: public QDockWidget, public Ui::VisaoObjetosWidget {
 Q_OBJECT

 private:
   /* Indica que a visão de objetos é usada de forma simplificada como
      seletora de objetos auxiliar de outros formulários. Interações
      como excluir, destacar, editar e exibir código dos objetos
      são desabilitadas */
   bool visao_simplificada;

   /* Armazena o endereço do objeto relacionado ao item marcado na árvore
     ou na lista de objetos */
   ObjetoBase *objeto_selecao;

  /* Menu popup o qual contém as ações: destacar objeto no modelo,
     excluir do modelo, edição do objeto */
  QMenu *menu_popup;

  /* Armazena a configuração de tamanho inicial do splitter para
     para uso em conjunto com o botão de exibição das configurações
     da visão de objetos.
     Armazena a configuração de posição da slider da barra de rolagem
     vertical da árvore de objetos para que a mesma seja restaurada
     sempre que a mesma sofrer atualizações */
  QSettings config_widgets;

  //Widget de Modelo de objetos o qual é acessado
  ModeloWidget *modelo_wgt;

  //Modelo o qual é acessado quando um modelo widget não é especificado
  ModeloBD *modelo_bd;

  //Armazena quais os tipos de objetos são visíveis na visão
  map<TipoObjetoBase, bool> map_objs_visiveis;

  //Atualiza a árvore inteira do banco de dados
  void atualizarSubBancoDados(void);

  //Atualiza somente a árvore de esquema e seus subitens
  void atualizarSubArvoreEsquema(QTreeWidgetItem *raiz);

  //Atualiza somente a árvore de tabelas em um determinado esquema
  void atualizarSubArvoreTabela(QTreeWidgetItem *raiz, ObjetoBase *esquema);

  //Atualiza a arvore de objetos
  void atualizarArvoreObjetos(void);

  //Atualiza a lista de objetos
  void atualizarListaObjetos(void);

  /* Gera um valor em um objeto QVariant para armazenamento dos
     endereços dos objetos do modelo para armazenamento em
     itens de QTreeWidgets e QListWidgetItem para permitir
     a interação entre direta como os objetos sem selecioná-los
     no modelo */
  QVariant gerarValorItem(ObjetoBase *objeto);

  /* Expande os itens da árvore até que o item refente ao objeto informado
     no parâmetro esteja visível */
  void expandirItemArvore(ObjetoBase *objeto);

 public:
   VisaoObjetosWidget(bool visao_simplificada=false, QWidget * parent = 0, Qt::WindowFlags f = 0);
  void closeEvent(QCloseEvent *);
  ObjetoBase *obterObjetoSelecao(void);

 private:
   //Implementa a movimentação da janela quando esta é exibida de forma simplificada
   void mouseMoveEvent(QMouseEvent *);

 public slots:
   void definirModelo(ModeloWidget *modelo_wgt);
   void definirModelo(ModeloBD *modelo_bd);
   void mudarVisaoObjetos(void);
   void atualizarVisaoObjetos(void);
   void definirObjetoVisivel(TipoObjetoBase tipo_obj, bool visivel);
   void close(void);

 private slots:
   void definirObjetoVisivel(QListWidgetItem *item);
   void definirTodosObjetosVisiveis(bool);
   void selecionarObjeto(void);
   void exibirMenuObjeto(void);
   void editarObjeto(void);

  signals:
  /* Sinais personalizados usados para sinalizarem
     a modificação da visão de objetos. Este sinal é capturado pelo
     form principal para atualizar as ferramentas */
  void s_visaoObjetosModificada(void);
  void s_visibilityChanged(ObjetoBase *,bool);
};
//***********************************************************
#endif
