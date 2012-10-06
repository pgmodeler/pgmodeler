/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe TabelaBase que implementa operações
#             comuns para manipulação grafica de objetos do tipo tabela (visões)
# Data de Criação: 09/04/2008
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
#ifndef TABELABASE_H
#define TABELABASE_H

#include "objetograficobase.h"
//***********************************************************
class TabelaBase: public ObjetoGraficoBase {
 protected:
 public:
  TabelaBase(void);
  friend class RelacionamentoBase;
  friend class ModeloBD;
};
//***********************************************************
#endif
