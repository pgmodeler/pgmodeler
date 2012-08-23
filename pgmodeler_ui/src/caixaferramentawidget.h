/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: CaixaFerramentaWidget
# Descrição: Definição da classe que implementa a caixa de ferramentas de controle
#            das operações executadas sobre o o modelo widget e seus objetos.
#
# Copyright (C) 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# Q Public License version 1.0 (QPL 1.0).
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
#ifndef CAIXA_FERRAMENTA_WIDGET_H
#define CAIXA_FERRAMENTA_WIDGET_H

#include "ui_caixaferramentawidget.h"
#include "modelowidget.h"
//***********************************************************
class CaixaFerramentaWidget: public QWidget, public Ui::CaixaFerramentaWidget {
 Q_OBJECT

 private:
  //Modelo Widget o qual é controlado pela caixa de ferramentas
  ModeloWidget *modelo_wgt;

 public:
  CaixaFerramentaWidget(QWidget *parent = 0);
  ~CaixaFerramentaWidget(void){}

  //Define o modelo a ser controlado pela caixa de ferramentas
  void definirModeloWidget(ModeloWidget *modelo_wgt);

 public slots:
  //Atualiza o estado das ferramentas de acordo com a situação atual do modelo
  void atualizarEstadoFerramentas(void);

  //Define a operação sobre o modelo de acordo com a ação clicada
  void definirOperacao(void);

 signals:
};
//***********************************************************
#endif
