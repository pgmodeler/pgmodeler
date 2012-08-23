/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: VisaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de visões.
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
#ifndef VISAO_WIDGET_H
#define VISAO_WIDGET_H

#include <QtGui>
#include "objetobasewidget.h"
#include "ui_visaowidget.h"
#include "tabelaobjetoswidget.h"

/* Declarando a classe Referencia como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar instancias de classes em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe Referencia.*/
#include <QMetaType>
Q_DECLARE_METATYPE(Referencia)
//***********************************************************
class VisaoWidget: public ObjetoBaseWidget, public Ui::VisaoWidget {
 Q_OBJECT

 private:
   //Frame usado como dica para criação de uma referência a todas as colunas de uma tabela
   QFrame *frame_info;

  /* Esta instância de visão é usada para receber todos os atributos configurados no formulário.
     Uma vez atribuídos os dados e validados a mesma tem seus atributos copiados para a visão
     que realemente precisa ser modificada/criada (this->objeto) */
  Visao visao_aux;

                   //Tabela de listagem das referências cadastradas para a visão
  TabelaObjetosWidget *tab_referencias;

                   //Destacador de sintaxe usado no campo de expressão e de código fonte da visão
   DestaqueSintaxe *destaque_expr,
                   *destaque_codigo;

                  //Seletores de tabela e coluna relacionados à visao
   SeletorObjetoWidget *sel_tabela,
                       *sel_coluna;

   //Exibe os dados de uma dada referência na tabela de referências
   void exibirDadosReferencia(Referencia refer, bool selec_from, bool from_where,
                              bool apos_where, unsigned idx_ref);

   //Método auxiliar que limpa os campos do formulário de referências
   void limparFormReferencia(void);

 public:
   VisaoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Visao *visao, float px, float py);

 private slots:
  void hideEvent(QHideEvent *);

  /* Exibe os campos necessários no formulário conforme o
     tipo de referencia selecionado */
  void selecionarTipoReferencia(void);

  /* Cria uma referência com base no que está no formulário
     e a exibe na tabela no índice informado no parâmetro */
  void manipularReferencia(int);

  /* Obtém a referência na linha especificada no parâmetro
     e exibe seus dados no formulário para edição */
  void editarReferencia(int);

  /* Controla a forma como é exibida os nomes da tabela e coluna
     selecionada na referência */
  void exibirNomeObjeto(void);

  /* Atualiza o campo de código fonte da visão. Este campo
     é preenchido com o código gerado pelo atributo 'visao_aux' */
  void atualizarPrevisaoCodigo(void);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
