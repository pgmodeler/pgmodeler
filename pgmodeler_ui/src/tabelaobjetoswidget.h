/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: TabelaObjetosWidget
# Descrição: Definição da classe que implementa uma tabela para manipulação
#            vários objetos em uma grade. Incluíndo funções de inclusão e exclusão
#            de objeto e movimentação dos mesmos nas linhas da tabela. Esta classe
#            é usada como auxiliar em formulários os quais trabalham com vários objetos
#            filhos de um objeto pai único (ex.: Tabelas, Indices, Restrições).
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
#ifndef TABELA_OBJETOS_WIDGET_H
#define TABELA_OBJETOS_WIDGET_H

#include "ui_tabelaobjetoswidget.h"
#include "objetobasewidget.h"
//***********************************************************
class TabelaObjetosWidget: public QWidget, public Ui::TabelaObjetosWidget {
 Q_OBJECT

 private:
   /* Indica se caixas de confirmação serão exibidas a cada exclusão de
      itens da tabela, por padrão as exclusões são feitas sem confirmação */
   bool conf_exclusoes;

 public:
   //Constantes usadas ativa/desativa um conjunto de botões da tabela
   const static unsigned BTN_INSERIR_ITEM=1,
                         BTN_REMOVER_ITEM=2,
                         BTN_ATUALIZAR_ITEM=4,
                         BTN_MOVER_ITENS=8,
                         BTN_EDITAR_ITEM=16,
                         BTN_LIMPAR_ITENS=32,
                         TODOS_BOTOES=63,
                         SEM_BOTOES=0;

   TabelaObjetosWidget(unsigned conf_botoes=TODOS_BOTOES,/*BTN_INSERIR_ITEM | BTN_REMOVER_ITEM | BTN_MOVER_ITENS |
                                            BTN_EDITAR_ITEM  | BTN_LIMPAR_ITENS*/
                       bool conf_exclusoes=false, QWidget * parent = 0);

   //Define o número de colunas da tabela
   void definirNumColunas(unsigned num_colunas);

   //Define o rótulo do cabeçalho de uma coluna
   void definirRotuloCabecalho(const QString &rotulo, unsigned idx_col);

   //Define o ícone do rótulo de uma coluna
   void definirIconeCabecalho(const QIcon &icone, unsigned idx_col);

   //Define o ícone de uma dada célula
   void definirIconeCelula(const QIcon &icone, unsigned idx_lin, unsigned idx_col);

   //Define o texto de uma dada célula
   void definirTextoCelula(const QString &texto, unsigned idx_lin, unsigned idx_col);

   //Define o dado que uma linha armazena
   void definirDadoLinha(const QVariant &dado, unsigned idx_lin);

   void definirFonteLinha(int idx_lin, const QFont &fonte, const QColor &cor_texto, const QColor &cor_fundo);

   //Retorna o número de colunas definidas na tabela
   unsigned obterNumColunas(void);

   //Retorna o número de linhas atuais na tabela
   unsigned obterNumLinhas(void);

   //Retorna o rótulo de um cabeçalho de colun
   QString obterRotuloCabecalho(unsigned idx_col);

   //Retorna o texto de uma célula
   QString obterTextoCelula(unsigned idx_lin, unsigned idx_col);

   //Retorna o dado armazenado numa linha
   QVariant obterDadoLinha(unsigned idx_lin);

   //Remove uma coluna através de seu índice
   void removerColuna(unsigned idx_col);

   //Adiciona uma coluna no índice especificado
   void adicionarColuna(unsigned idx_col);

   //Retorna o índice da linha selecionada na tabela
   int obterLinhaSelecionada(void);

   /* Retorna o índice da linha buscando-a através do dado
      que ela armazena. Caso este não seja encontrada
      o método retorna -1 */
   int obterIndiceLinha(const QVariant &dado);

   //Define os botões disponíveis para controle da tabela
   void definirConfiguracaoBotoes(unsigned botoes);

 private slots:
  /* Move a linha selecionada para cima ou para baixo de acordo com o
     botão de movimentação acionado pelo usuário */
  void moverLinhas(void);

  //Remove a linha selecionada atualmente
  void removerLinha(void);

  /* Dispara um sinal indicando que uma linha está preparada para edição.
     A edição da linha deve ser manipulada/implementada no objeto o qual
     fizer uso da tabela de objetos pois por ser tratar de um procedimento
     especifico não foi implementado nesta classe, por isso o método apenas
     dispara o sinal de edição da linha. */
  void editarLinha(void);

  /* Dispara um sinal indicando que uma linha está preparada para atualização.
     A atualização da linha deve ser manipulada/implementada no objeto o qual
     fizer uso da tabela de objetos pois por ser tratar de um procedimento
     especifico não foi implementado nesta classe, por isso o método apenas
     dispara o sinal de atualização da linha. */
  void atualizarLinha(void);

  //Habilita os botões de acordo com  a linha selecionada na tabela
  void habilitarBotoes(void);

 public slots:
  //Adiciona uma linha ao final da tabela
  void adicionarLinha(void);

  //Remove todas as lista da tabela
  void removerLinhas(void);

  //Remove uma linha na posição selecionada
  void removerLinha(unsigned idx_lin);

  //Limpa a seleção da tabela desmarcando cada linha selecionada
  void limparSelecao(void);

  //Seleciona a linha cujo indice está especificado
  void selecionarLinha(int idx_lin);

  //Define o estado de habilitação dos botãos especificados
  void habilitarBotoes(unsigned conf_botoes, bool valor);

 signals:
   /* Sinal disparando quando uma linha é adicionada.
      O índice da linha adicionada é enviado com o sinal. */
   void s_linhaAdicionada(int);

   /* Sinal disparado quando duas linhas tem suas posições trocadas.
      O índice das linhas trocadas são enviado com o sinal. */
   void s_linhasMovidas(int,int);

   //Sinal disparado quano todas as linhas da tabela são excluídas
   void s_linhasRemovidas(void);

   /* Sinal disparado quando uma dada linha é removida. O índice da linha
      removida é enviado com o sinal. */
   void s_linhaRemovida(int);

   /* Sinal disparado quando uma dada linha é selecionada. O índice da linha
      selecionada é enviado com o sinal. */
   void s_linhaSelecionada(int);

   /* Sinal disparado quando uma dada linha é editada.
      O índice da linha editada é enviado com o sinal. */
   void s_linhaEditada(int);

   /* Sinal disparado quando o botão de atualizar a linha e acionado.
      O índice da linha atualizada é enviado com o sinal. */
   void s_linhaAtualizada(int);

   /* Sinal disparado quando uma dada coluna é removida.
      O índice da coluna removida é enviado com o sinal. */
   void s_colunaRemovida(int);

   /* Sinal disparado quando uma dada coluna é adicionada.
      O índice da coluna adicionada é enviado com o sinal. */
   void s_colunaAdicionada(int);
};
//***********************************************************
#endif
