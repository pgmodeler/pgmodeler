/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: GatilhoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de gatilhos / gatilhos restrições.
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
#ifndef GATILHO_WIDGET_H
#define GATILHO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_gatilhowidget.h"
#include "tabelaobjetoswidget.h"
//***********************************************************
class GatilhoWidget: public ObjetoBaseWidget, public Ui::GatilhoWidget {
 Q_OBJECT

 private:
   //Destaque de sintaxe do campo de expressão condicional
   DestaqueSintaxe *dest_exp_condicional;

   //Tabelas de objetos para controlar colunas e argumentos
   TabelaObjetosWidget *tab_colunas,
                       *tab_argumentos;

   //Seletor de tabela referenciada e função executada pelo gatilho
   SeletorObjetoWidget *sel_tabela_ref,
                       *sel_funcao;

   //Adiciona uma coluna à tabelade colunas referenciadas pelo gatilho
   void adicionarColuna(Coluna *coluna, int idx_lin);

 public:
   GatilhoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, ListaOperacoes *lista_op, Gatilho *restricao);

 private slots:
  void hideEvent(QHideEvent *);
  void adicionarColuna(int idx_lin);
  void manipularArgumento(int idx_lin);
  void editarArgumento(int idx_lin);
  void atualizarComboColunas(void);
  void definirGratilhoRestricao(bool valor);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
