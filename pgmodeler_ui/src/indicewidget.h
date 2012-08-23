/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: RestricaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de índices.
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
#ifndef INDICE_WIDGET_H
#define INDICE_WIDGET_H

#include "objetobasewidget.h"
#include "ui_indicewidget.h"
#include "tabelaobjetoswidget.h"

/* Declarando a classe ElementoIndice como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar instancias de classes em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe ElementoIndice.*/
#include <QMetaType>
Q_DECLARE_METATYPE(ElementoIndice);
//***********************************************************
class IndiceWidget: public ObjetoBaseWidget, public Ui::IndiceWidget {
 Q_OBJECT

 private:
   //Destaque de sintaxe do campo de expressão condicional
   DestaqueSintaxe *dest_exp_condicional,
                   *dest_exp_elemento;

   //Tabelas de objetos para controlar elementos do índice
   TabelaObjetosWidget *tab_elementos;

   //Seletor de classe de operadores dos elementos
   SeletorObjetoWidget *sel_classe_op;

   //Atualiza o combo de colunas com as existentes na tabela
   void atualizarComboColunas(void);

   //Exibe o dado de um elemento na linha especificada da tabela
   void exibirDadosElemento(ElementoIndice elem, int idx_elem);

 public:
   IndiceWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, ListaOperacoes *lista_op, Indice *indice);

 private slots:
  void hideEvent(QHideEvent *);
  void selecionarObjetoElemento(void);

  //Métodos de manipulação dos elementos do índice
  void manipularElemento(int idx_elem);
  void editarElemento(int idx_elem);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
