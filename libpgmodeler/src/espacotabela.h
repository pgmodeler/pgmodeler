/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)m
# Description: Definição da classe EspacoTabela que é usada para
#             gerar os códigos SQL pertinentes aos tablespaces
#             em banco de dados.
#
# Creation date:o: 05/06/2008
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
#ifndef ESPACOTABELA_H
#define ESPACOTABELA_H

#include "objetobase.h"

class EspacoTabela: public BaseObject{
 private:
  static unsigned tabspace_id;

  //Diretório em que se encontra o espaco de tabela
  QString diretorio;

 public:
  EspacoTabela(void);

  //Define o nome do espaço de tabela
  void setName(const QString &obj_name);

  //Define o diretório do tablespace
  void definirDiretorio(const QString &diretorio);

  //Retorna o diretório configurado para o espaço de tabela
  QString obterDiretorio(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
