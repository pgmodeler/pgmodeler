/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: FuncaoAgregacaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de funções de agregação.
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
#ifndef FUNCAO_AGREGADA_WIDGET_H
#define FUNCAO_AGREGADA_WIDGET_H

#include <QtGui>
#include "objetobasewidget.h"
#include "ui_funcaoagregacaowidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"
//***********************************************************
class FuncaoAgregacaoWidget: public ObjetoBaseWidget, public Ui::FuncaoAgregacaoWidget {
 Q_OBJECT

 private:          //Widget de seleção de tipo de entrada da função
   TipoPgSQLWidget *tipo_entrada,
                   //Widget de tipo de estado da função
                   *tipo_estado;

                   /* Tabela de listagem de tipos de entrada da função.
                      Os elementos desta tabela são convertidos em TipoPgSQL e
                      atribuídos como tipos de entrada da função agregada */
  TabelaObjetosWidget *tab_tipos_entrada;

                   //Destacador de sintaxe usado no campo de expressão condicional
   DestaqueSintaxe *destaque_cond;

                  //Seletores de funções e operador relacionados à função de agregação
   SeletorObjetoWidget *sel_op_ordenacao,
                       *sel_func_final,
                       *sel_func_transicao;

 public:
   FuncaoAgregacaoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, FuncaoAgregacao *funcao_ag);

 private slots:
  void hideEvent(QHideEvent *);

  //Adiciona ou atualiza o tipo selecionado no widget 'tipo_entrada' à tabela de tipos de entrada
  void manipularTipoDado(int linha);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
