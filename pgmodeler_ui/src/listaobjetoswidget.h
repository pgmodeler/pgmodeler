/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ListaObjetosWidget
# Descrição: Definição da classe que implementa o formulário de
#            exibição das dependências e referências a um objeto
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
#ifndef LISTA_OBJETOS_WIDGET_H
#define LISTA_OBJETOS_WIDGET_H

#include "ui_listaobjetoswidget.h"
#include "objetobasewidget.h"
#include "objetografico.h"
//***********************************************************
class ListaObjetosWidget: public ObjetoBaseWidget, public Ui::ListaObjetosWidget {
 Q_OBJECT

 private:
  void aplicarConfiguracao(void){}

  //Carrega os widgets que contém as referências e dependências do objeto
  void atualizarListaObjetos(vector<ObjetoBase *> &objetos, QTableWidget *tabela_wgt);

 protected slots:
  void hideEvent(QHideEvent *);

 public:
  ListaObjetosWidget(QWidget * parent = 0);
  void definirAtributos(ModeloBD *modelo, ObjetoBase *objeto, ObjetoBase *objeto_pai=NULL);
};
//***********************************************************
#endif
