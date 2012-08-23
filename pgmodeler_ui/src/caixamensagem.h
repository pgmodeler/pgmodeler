/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: CaixaMensagem
# Descrição: Definição da classe usada para exibição de caixas
#            de mensagens personalizadas, como exibição de pilha
#            de exceções geradas no software, icones indicativos
#            de erros, alertas, informações , etc.
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
#ifndef CAIXA_MENSAGEM_H
#define CAIXA_MENSAGEM_H

#include "ui_caixamensagem.h"
#include "excecao.h"
//***********************************************************
class CaixaMensagem: public QDialog, public Ui::CaixaMensagem {
 private:
 Q_OBJECT
  bool cancelada;

 public:
   //Constantes usadas para definir o ícone da mensagem
   static const unsigned SEM_ICONE=10,
                         ICONE_ERRO=11,
                         ICONE_INFO=12,
                         ICONE_ALERTA=13,
                         ICONE_CONFIRM=14;

   //Constantes usadas para configurar quais botões são visíveis
   static const unsigned BOTAO_SIM_NAO=0,
                         BOTAO_OK_CANCELAR=1,
                         BOTAO_OK=2,
                         BOTAO_SIM_NAO_CANCELAR=3;

   CaixaMensagem(QWidget * parent = 0, Qt::WindowFlags f = 0);
  ~CaixaMensagem(void){}

   //Método padrão de exibição da mensagem, necessário configurar icone, texto, titulo e botões
   void show(const QString &titulo, const QString &texto, unsigned icone=SEM_ICONE,
             unsigned botoes=BOTAO_OK);

   /* Exibe a caixa de mensagem configurando-a com as mensagens de uma Exceção, por padrão
      é exibida uma caixa com icone de erro e um botão ok além disso a árvore de
      exceções é exibida. Os parâmetros 'msg' e 'tipo_ico' são usados para personalizar a exibição
      da mensagem de erro da exceção. Se usado o parametro 'msg' a mensagem da exceção é sobrepostas
      estando disponível apenas na pilha de erros. */
   void show(Excecao e, const QString &msg="", unsigned tipo_ico=ICONE_ERRO);

   //Retorna se o usuário marcou a opção de cancela na caixa
   bool caixaCanceleda(void);

private slots:
   void clickSimOK(void);
   void clickNaoCancelar(void);
   void exibirListaExecoes(void);
};
//***********************************************************
#endif
