/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe FamiliaOperadores que é usada para
#             gerar os códigos SQL pertinente� s famílias de operadores
#             referenciadas pelas classes de operadores
# Creation date:o: 08/07/2008
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
#ifndef FAMILIAOPERADORES_H
#define FAMILIAOPERADORES_H

#include "objetobase.h"

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

#endif
