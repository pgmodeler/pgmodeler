/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: ListaObjetosWidget
# Description:o: Definição da classe que implementa o formulário de
#            exibição das dependências e referências a um objeto
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#ifndef LISTA_OBJETOS_WIDGET_H
#define LISTA_OBJETOS_WIDGET_H

#include "ui_listaobjetoswidget.h"
#include "objetobasewidget.h"
#include "objetografico.h"

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

#endif
