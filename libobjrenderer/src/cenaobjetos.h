/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: Viewport
# Descrição: Esta classe deriva a classe QGraphicsScene e implementa a visualização
#            e outras operações relacionados aos objetos gráficos do modelo.
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef CENA_OBJETOS_H
#define CENA_OBJETOS_H

#include <QtGui>
#include "ogrelacionamento.h"
#include "ogvisao.h"
#include "ogtabela.h"
//***********************************************************
class CenaObjetos: public QGraphicsScene {
 private:
  Q_OBJECT

  //Opções de alinhamento de objetos, exibição da grade e limites das paginas
  static bool alin_objs_grade, exibir_grade, exibir_lim_pagina;

  //Tamanho da grade da cena
  static unsigned tam_grade;

  //Tamanho do papel (vide QPrinter::PaperSize)
  static QPrinter::PaperSize tam_papel;

  //Orientacao do papel (Landscape ou portrait, QPrinter::Orientation)
  static QPrinter::Orientation orientacao_pag;

  //Margens do papel
  static QRectF margens_pag;

  /* Flag que indica que está ocorrendo a movimentção de objetos.
     Isso indica quando o sinal s_inicioMovimentoObjetos() ou
     s_finalMovimentoObjetos() deve ser disparado */
  bool movendo_objs;

  //Ponto inicial do retângulo de seleção
  QPointF sel_ini;

  //Retângulo de seleção de objetos na cena
  QGraphicsPolygonItem *ret_selecao;

  QGraphicsLineItem *linha_rel;

  /* Para minimizar memory leaks e fragmentação da memória,
     os objetos removidos da cena por padrão não são deletados
     (vide QGraphicsScene::removeItem()). Por isso, os objetos
     removidos da cena são armazenandos na lista abaixo e estes
     por sua vez são deletados efetivamente quando a cena também
     é deletada. Esta abordagem pode aumentar um pouco o consumo de
     memória mas diminui o risco de segmentation fault devido a execução
     de tarefas complexas e interligadas como as da lista de operações e
     desenho de objetos na cena */
  vector<ObjetoGrafico *> objs_inativos;

  //Método utilitário para alinhar um ponto à grade
  static QPointF alinharPontoGrade(const QPointF &pnt);

 protected:
  /* Método sobrecarregados de eventos do mouse. Executa operações adicionais antes das
     operações de mouse padrão da classe superior QGraphicsScene */
  void mousePressEvent(QGraphicsSceneMouseEvent *evento);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *evento);
  void	mouseReleaseEvent(QGraphicsSceneMouseEvent *evento);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evento);

  //Exibe uma linha a partir do ponto 'p' que simula a criação de um relacionamento entre tabelas
  void exibirLinhaRelacionamento(bool valor, const QPointF &p=QPointF(NAN,NAN));

 public:

  //Brush que define o padrão da grade
  static QBrush grade;

  CenaObjetos(void);
  ~CenaObjetos(void);

  /* Métodos estáticos que definem o tamanho da grade e as opções de
     alinhamento de objetos à grade e exibição da própria grade. */
  static void definirGrade(unsigned tam);
  static void definirOpcoesGrade(bool exibir_grade, bool alin_objs_grade, bool exibir_lim_pagina);
  static void obterOpcoesGrade(bool &exibir_grade, bool &alin_objs_grade, bool &exibir_lim_pagina);

  /* Métodos de configuração das propriedades da página. Obs.: O usuário precisa chamar o método definirGrade
     para atualiza as configurações da página */
  static void definirConfiguracaoPagina(QPrinter::PaperSize tam_papel, QPrinter::Orientation orientacao, QRectF margens);
  static void obterConfiguracaoPagina(QPrinter::PaperSize &tam_papel, QPrinter::Orientation &orientacao, QRectF &margens);

  void addItem(QGraphicsItem *item);
  void removeItem(QGraphicsItem *item);
  void setSceneRect(const QRectF &ret);

 public slots:
  //Alinha todos os objetos do modelo à grade
  void alinharObjetosGrade(void);
  void update(void);

 private slots:
  /* Estes slots tratam sinais os quais são emitidos individualmente por objetos
     da cena (ex.: OGTabela::s_objetoFilhoSelecionado ou OGRelacionamento::s_relacionamentoModificado ou
     s_objetoSelecionado)
     e estes sinais por sua vez são de certa forma encaminhados para classe superiores
     através do sinais s_menupopRequisitado, s_objetoModificado ou s_objetoSelecionado. Ex.: A classe ModeloWidget precisa
     tratar o sinal s_objetoFilhoSelecionado exibindo um menupop, somente com o auxilio dos métodos
     abaixo isso é possível */
  void sinalizarModificacaoObjeto(ObjetoGraficoBase *objeto);
  void sinalizarObjetoFilhoSelecionado(ObjetoTabela *obj_filho);
  void sinalizarObjetoSelecionado(ObjetoGraficoBase *objeto, bool selecionado);

 signals:
  /* Este sinal é emitdo sempre quando se inicia ou finaliza o movimento de objetos.
     o parâmetro 'fim_movimento' indica se o sinal foi emitido ou não no término
     do movimento */
  void s_objetosMovimentados(bool fim_movimento);

  //Emitido quando um objeto é modificado
  void s_objetoModificado(ObjetoGraficoBase *objeto);

  //Emitido quando um usuário pressiona botão direito do mouse na cena
  void s_menupopupRequisitado(vector<ObjetoBase *>);

  //Emitido quando o usuário dá um duplo clique com esquerdo no objeto
  void s_objetoDuploClique(ObjetoGraficoBase *objeto);

  /* Emitido quando um objeto é selecionado no modelo, este sinal pode ser usado
     quando se deseja obter o objeto selecionado no momento do clique, pois o sinal
     CenaObjetos::selectionChanged() não dá esta possibilidade. */
  void s_objetoSelecionado(ObjetoGraficoBase *objeto, bool selecionado);

  friend class ModeloWidget;
};
//***********************************************************
#endif
