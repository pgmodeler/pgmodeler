/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libobjrenderer
# Classe: OGVisao
# Description:o: Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            da classe Visao. O sufixo OG foi adicionado ao nome da classe para evitar conflitos
#            de nomes entra a classe Visao da biblioteca libpgsqldbm e a classe abaixo definida.
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
#ifndef OG_VISAO_H
#define OG_VISAO_H

#include "visao.h"
#include "ogtabelabase.h"

class OGVisao: public OGTabelaBase {
 private:
  Q_OBJECT

 //Objeto gráfico agrupador que armanzena os subitens que denotam as referências da visão
  QGraphicsItemGroup *referencias;

 public:
  OGVisao(Visao *visao);
  ~OGVisao(void);

 private slots:
  void configurarObjeto(void);
};

#endif
