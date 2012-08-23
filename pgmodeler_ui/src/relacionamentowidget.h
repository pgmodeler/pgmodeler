/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: RestricaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de relacionamentos.
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
#ifndef RELACIONAMENTO_WIDGET_H
#define RELACIONAMENTO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_relacionamentowidget.h"
#include "tabelaobjetoswidget.h"
//***********************************************************
class RelacionamentoWidget: public ObjetoBaseWidget, public Ui::RelacionamentoWidget {
 private:
  Q_OBJECT
   /* Quantidade de elementos na lista de operações antes do relacionamento
      ser editado. Este atributo é usado para se saber, em caso de cancelamento
      da edição do relacionamento, a quantidade de operações relacionada ao
      objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
   unsigned qtd_operacoes;

   //Destacadores de sintaxe para os campos de nome da tabela de origem e destino
   DestaqueSintaxe *dest_tab_orig,
                   *dest_tab_dest;

   //Tabela as quais armazenam os atributos e restrições do relacionamento
   TabelaObjetosWidget *tab_atributos,
                       *tab_restricoes;

   /* Lista os objetos do relacionamento na tabela respectiva, de acordo
      com o tipo do objeto passado */
   void listarObjetos(TipoObjetoBase tipo_obj);

   //Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
   void exibirDadosObjeto(ObjetoTabela *objeto, int idx_lin);

 protected:
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tabela *tab_orig, Tabela *tab_dest, unsigned tipo_rel);

 public:
   RelacionamentoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, RelacionamentoBase *relacao);

 private slots:
  void hideEvent(QHideEvent *);

  //Adiciona um objeto à tabela a qual aciona o método
  void adicionarObjeto(void);
  //Edita um objeto selecionado  na tabela a qual aciona o método
  void editarObjeto(int idx_lin);
  //Remove um objeto selecionado  na tabela a qual aciona o método
  void removerObjeto(int idx_lin);
  //Remove todos os objetos da tabela a qual aciona o método
  void removerObjetos(void);

 public slots:
   void aplicarConfiguracao(void);
   void cancelarConfiguracao(void);

 friend class ModeloWidget;
};
//***********************************************************
#endif
