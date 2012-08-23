/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe FamiliaOperadores que é usada para
#             gerar os códigos SQL pertinentes às famílias de operadores
#             referenciadas pelas classes de operadores
# Data de Criação: 08/07/2008
#
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
#ifndef FAMILIAOPERADORES_H
#define FAMILIAOPERADORES_H

#include "objetobase.h"
//***********************************************************
class FamiliaOperadores: public ObjetoBase {
 private:
  //Tipo de indexação usada na família de operadores
  TipoIndexacao tipo_index;

 public:
  FamiliaOperadores(void);

  //Define o tipo de indexacao usado na família
  void definirTipoIndexacao(TipoIndexacao tipo);

  //Retorna o tipo de indexação
  TipoIndexacao obterTipoIndexacao(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida);
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
