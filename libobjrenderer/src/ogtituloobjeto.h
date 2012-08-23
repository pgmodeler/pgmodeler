/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: OGTituloObjeto
# Descrição: Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            do título de visões e tabelas.
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
#ifndef OG_TITULO_OBJETO_H
#define OG_TITULO_OBJETO_H

#include "visao.h"
#include "tabela.h"
#include "objetografico.h"
//***********************************************************
class OGTituloObjeto: public ObjetoGrafico
{
 private:
  Q_OBJECT

  //Objeto gráfico poligonal que define a borda do título
  QGraphicsPolygonItem *caixa;

  //Objetos gráifcos textuais que definem o nome e esquema do objeto dono do título
  QGraphicsSimpleTextItem *nome,
                          *esquema;

 public:
  OGTituloObjeto(void);
  ~OGTituloObjeto(void);

  void configurarObjeto(ObjetoGraficoBase *objeto);
  void redimensionarTitulo(float larg, float alt);
};
//***********************************************************
#endif
