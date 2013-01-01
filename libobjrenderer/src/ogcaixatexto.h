/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libobjrenderer
# Classe: OGCaixaTexto
# Description:Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            da classe CaixaTexto. O sufixo OG foi adicionado ao nome da caixa para evitar conflitos
#            de nomes entra a classe CaixaTexto da biblioteca libpgmodeler e a classe abaixo definida.
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
#ifndef OG_CAIXA_TEXTO_H
#define OG_CAIXA_TEXTO_H

#include "textbox.h"
#include "baseobjectview.h"

class OGCaixaTexto: public BaseObjectView {
 private:
  Q_OBJECT

  //Parâmetros de cores da caixa
  QPen pen; //Borda
  QBrush brush; //Preenchimento

  //Objeto gráfico poligonal que define a caixa de texto
  QGraphicsPolygonItem *caixa;

  //Objeto gráfico de texto que define o conteúdo textual da caixa
  QGraphicsSimpleTextItem *texto;

 public:
  OGCaixaTexto(Textbox *caixa, const QBrush &brush=Qt::NoBrush, const QPen &pen=Qt::NoPen);
  ~OGCaixaTexto(void);

 protected slots:
  void configureObject(void);
};

#endif
