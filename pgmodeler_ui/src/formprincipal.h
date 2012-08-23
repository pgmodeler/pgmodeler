/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: FormPrincipal
# Descrição: Definição da classe que implementa o formulário principal do software,
#            Reune e gerencia todas as classes das bibliotecas implementadas.
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
#ifndef FORM_PRINCIPAL_H
#define FORM_PRINCIPAL_H

#include <QtGui>
#include "ui_formprincipal.h"
#include "modelowidget.h"
#include "formsobre.h"
#include "caixamensagem.h"
#include "formbasico.h"
#include "listaoperacoeswidget.h"
#include "visaoobjetoswidget.h"

using namespace std;
//***********************************************************
class FormPrincipal: public QMainWindow, public Ui::FormPrincipal {
 Q_OBJECT

 private:
  static const QString VERSAO_PGMODELER;

  QTimer tm_salvamento;
  ListaOperacoesWidget *lista_oper;
  VisaoObjetosWidget *visao_objs;
  ModeloWidget *modelo_atual;
  QLabel *nome_op;
  QLabel *icone_op;
  QString titulo_janela;
  int interv_salvar;

  void closeEvent(QCloseEvent *);

 public:
   FormPrincipal(QWidget *parent = 0, Qt::WindowFlags flags = 0);
  ~FormPrincipal(void);

 private slots:
  void definirOpcoesGrade(void);
  void adicionarNovoModelo(const QString &nome_arq="");

  /* Atualiza o estado (ativo/inativo) dos botões das barras
     de ferramentas de acordo com a ação de abertura ou
     fechamento de modelo */
  void atualizarEstadoFerramentas(bool modelo_fechado=false);
  void __atualizarEstadoFerramentas(void);

  /* Efetua a atualização dos dockwidgets de visão geral,
     lista de operações e arvore de objetos */
  void atualizarDockWidgets(void);
  void __atualizarDockWidgets(void);

  void definirModeloAtual(void);
  void fecharModelo(int idx_modelo=-1);

  void carregarModelo(void);
  void salvarModelo(ModeloWidget *modelo=NULL);
  void salvarTodosModelos(void);
  void imprimirModelo(void);
  void exportarModelo(void);

  //Atualiza os modelos abertos com as novas configurações
  void atualizarModelos(void);

  void aplicarZoom(void);
  void exibirTelaCheia(bool tela_cheixa);
};
//***********************************************************
#endif
