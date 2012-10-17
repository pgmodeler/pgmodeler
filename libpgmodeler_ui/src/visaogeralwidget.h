/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: VisaoGeralWidget
# Descrição: Definição da classe que implementa o widget de visão geral do modelo.
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
#ifndef VISAO_GERAL_WIDGET_H
#define VISAO_GERAL_WIDGET_H

#include <QtGui>
#include "excecao.h"
#include "ui_visaogeralwidget.h"
//***********************************************************
class VisaoGeralWidget: public QWidget, public Ui::VisaoGeralWidget {
 Q_OBJECT
 private:
  //Armazena a cena a qual será desenhada na visão geral
  QGraphicsScene *cena;

  //Armazena o viewport o qual está exibindo a cena
  QGraphicsView *viewp;

  //Fator de zoom da cena
  float fator_zoom;

  //Fator de redimensionamento dos widgets (20% do tamanho da cena)
  const static float FATOR_REDIM=0.20f;

  void mouseDoubleClickEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *evento);
  void mouseReleaseEvent(QMouseEvent *evento);
  void mouseMoveEvent(QMouseEvent *evento);

 public:
  VisaoGeralWidget(QGraphicsScene *cena, QWidget *parent = 0);

 public slots:
  //Atualiza a exibição da visão geral com a última modificação na cena
  void atualizarVisaoGeral(void);

  //Redimensiona a geometria do frame de janela do mundo
  void redimensionarFrameJanela(void);

  //Redimensiona todo o widget de visão geral
  void redimensionarVisaoGeral(void);

  void atualizarFatorZoom(float zoom);

  //Sobrecarga dos metodos de manipulação dos eventos de abertura e fechamento da janela
  void closeEvent(QCloseEvent *evento);
  void showEvent(QShowEvent *evento);

  signals:
   //Sinal emitido quando a janela é aberta ou fechada (o parâmetro indica se a janela está ou não visível)
   void s_visaoGeralVisivel(bool);
};
//***********************************************************
#endif
