/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: PapelWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de esquemas.
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
#ifndef PAPEL_WIDGET_H
#define PAPEL_WIDGET_H

#include "objetobasewidget.h"
#include "ui_papelwidget.h"
#include "tabelaobjetoswidget.h"
//***********************************************************
class PapelWidget: public ObjetoBaseWidget, public Ui::PapelWidget {
 Q_OBJECT

 private:
   /* Armazena as referências às tabelas de membros do papel para que sejam
      referenciados nos demais métodos de forma mais eficaz */
   TabelaObjetosWidget *tab_membros[3];

   //Preenche as tabelas de membros do papel
   void preencherTabelaMembros(void);

   //Exibe o dado do papel na tabela especificada
   void exibirDadosPapel(Papel *papel, unsigned idx_tabela, unsigned lin);

 public:
   PapelWidget(QWidget * parent = 0);

   void hideEvent(QHideEvent *);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Papel *papel);

 private slots:
   /* Preenche com informações relacionadas ao papel selecionado
      na linha atual da tabela em foco */
   void exibirDadosPapelSelecionado(void);

   /* Seleciona um papel membro para ser inserido na tabela de membro
      atualmente em foco. Este slot é usando tanto para adicionar
      um membro na tabela quanto para editar um membro selecionado */
   void selecionarPapelMembro(void);

   /* Faz a conexão de sinais-slots para cada tabela de objetos
      à medida que a tabela em foco mude */
   void configurarSelecaoPapeis(void);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
