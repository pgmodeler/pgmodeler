/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: RestricaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de tabela.
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
#ifndef TABELA_WIDGET_H
#define TABELA_WIDGET_H

#include "objetobasewidget.h"
#include "ui_tabelawidget.h"
#include "tabelaobjetoswidget.h"
#include "ogtabela.h"
//***********************************************************
class TabelaWidget: public ObjetoBaseWidget, public Ui::TabelaWidget {
 Q_OBJECT

 private:
   /* Quantidade de elementos na lista de operações antes da tabela
      ser editada. Este atributo é usado para se saber, em caso de cancelamento
      da edição da tabela, a quantidade de operações relacionadas ao
      objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
   unsigned qtd_operacoes;

   //Armazena as tabelas de objetos filhos da tabela (colunas, restrições, índices, etc)
   map<TipoObjetoBase, TabelaObjetosWidget *> mapa_tab_objetos;

   /* Lista os objetos do relacionamento na tabela respectiva, de acordo
      com o tipo do objeto passado */
   void listarObjetos(TipoObjetoBase tipo_obj);

   //Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
   void exibirDadosObjeto(ObjetoTabela *objeto, int idx_lin);

   //Seleciona a tabela de objetos de acordo com o tipo passado
   TabelaObjetosWidget *selecionarTabelaObjetos(TipoObjetoBase tipo_obj);

   //Seleciona o tipo de objeto de acordo com o objeto sender informado
   TipoObjetoBase selecionarTipoObjeto(QObject *tab_sender);

   //Exibe o formulário de edição de objetos de tabela conforme o tipo passado
   void exibirFormObjetoTabela(TipoObjetoBase tipo_obj);

 public:
   TabelaWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tabela *tabela, float pos_x, float pos_y);

 private slots:
  void hideEvent(QHideEvent *);

  //Adiciona ou edita um objeto da tabela a qual aciona o método
  void manipularObjeto(void);
  //Remove um objeto selecionado  na tabela a qual aciona o método
  void removerObjeto(int idx_lin);
  //Remove todos os objetos da tabela a qual aciona o método
  void removerObjetos(void);
  //Faz a movimentação entre dois objetos da tabela
  void moverObjetos(int idx1, int idx2);

 public slots:
   void aplicarConfiguracao(void);
   void cancelarConfiguracao(void);
};
//***********************************************************
#endif
