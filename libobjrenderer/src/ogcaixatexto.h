/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: OGCaixaTexto
# Descrição: Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            da classe CaixaTexto. O sufixo OG foi adicionado ao nome da caixa para evitar conflitos
#            de nomes entra a classe CaixaTexto da biblioteca libpgsqldbm e a classe abaixo definida.
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef OG_CAIXA_TEXTO_H
#define OG_CAIXA_TEXTO_H

#include "caixatexto.h"
#include "objetografico.h"
//***********************************************************
class OGCaixaTexto: public ObjetoGrafico {
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
  OGCaixaTexto(CaixaTexto *caixa, const QBrush &brush=Qt::NoBrush, const QPen &pen=Qt::NoPen);
  ~OGCaixaTexto(void);

 protected slots:
  void configurarObjeto(void);
};
//***********************************************************
#endif
