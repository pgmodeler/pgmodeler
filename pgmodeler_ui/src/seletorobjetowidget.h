/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: SeletorObjetoWidget
# Descrição: Definição da classe que implementa um seletor de objetos simples
#            o qual integra o formulário de visão de objetos onde o usuário
#            pode selecionar o objeto a ser exibido e alguns botões de controle
#            do próprio seletor, usados para limpar o campo e para abrir a janela
#            de seleção.
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
#ifndef SELETOR_OBJETO_WIDGET_H
#define SELETOR_OBJETO_WIDGET_H

#include <QtGui>
#include "modelobd.h"
#include "destaquesintaxe.h"
#include "ui_seletorobjetowidget.h"
//***********************************************************
class SeletorObjetoWidget: public QWidget, public Ui::SeletorObjetoWidget {
 Q_OBJECT

 private:
   /* Armazena o seletor que está focalizado atualmente. Este atributo é usado
      para que outro seletor em um mesmo formulário não recebe o
      valor selecionado para o seletor que realmente deve mostrar o nome
      do objeto */
   static QObject *seletor_atual;

   /* Armazena o destacador de sintaxe que pode ser usado
      para colorir o texto do seletor de acordo com a sintaxe SQL */
   DestaqueSintaxe *destaque_txt;

   //Armazena o objeto selecionado pelo usuário
   ObjetoBase *objeto;

   //Armazena o tipo de objeto que deve ser selecionado pelo seletor
   TipoObjetoBase tipo_obj_seletor;

   /* Armazena o modelo usado como referencia para seleção dos objetos
      se este objeto não estive definido o seletor de objetos não
      será exibido ficando inativo */
   ModeloBD *modelo;

 public:
   /* Um seletor deve sempre ser instanciado com um rótulo e o tipo de objeto
      que o seletor aceita */
   SeletorObjetoWidget(TipoObjetoBase tipo_obj_seletor, bool inst_destaque_txt, QWidget * parent = 0);

  //Retorna o objeto selecionado pelo usuário
  ObjetoBase *obterObjeto(void);

  //Define o objeto inicial a ser exibido no seletor
  void definirObjeto(ObjetoBase *objeto);

  //Define o modelo o qual o seletor de objeto buscará os objetos
  void definirModelo(ModeloBD *modelo);

 private slots:
   //Preenche um dos campos os quais referenciam um esquema, espaço tabela ou dono.
   void exibirObjetoSelecionado(ObjetoBase *obj_sel, bool=false);

   //Exibe o formulário de seleção de objetos
   void exibirSelecaoObjetos(void);

 public slots:
   //Remove o objeto selecionado, limpando o campo do nome e anulando a referência ao objeto
   void removerObjetoSelecionado(void);

 signals:
   /* Sinal disparando quando um objeto é selecionado
      e tem seu nome exibido na caixa de texto. */
   void s_objetoSelecionado(void);

   //Sinal disparando quando um objeto é deselecionado (removido)
   void s_objetoRemovido(void);
};
//***********************************************************
#endif
