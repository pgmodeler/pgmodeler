/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definição da classe Linguagem que é usado para
#             e gerar os códigos SQL pertinente s linguagens
#             procedurais.
# Creation date:o: 19/05/2008
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
#ifndef LINGUAGEM_H
#define LINGUAGEM_H

#include "objetobase.h"
#include "funcao.h"
#include "papel.h"

class Linguagem: public BaseObject{
 private:
  /* Indica se a linguaguem é confiável ou não.
     Ser confiável indica que a linguaguem pode ser usada por um usuário
     sem privilégios sem comprometar a segurança do SGBD */
  bool confiavel;

  //Função que definem o funcionamento da linguagem
  Funcao *funcoes[3];

 public:                //Função de validação sintática da linguagem
  const static unsigned FUNC_VALIDATOR=0,
                        //Função usada para executar as funções escritas nesta linguagem
                        FUNC_HANDLER=1,
                        //Função usada para executar instruções inlines (DO's) nesta linguagem (apenas para pgsql 9.x)
                        FUNC_INLINE=2;

  Linguagem(void);

  //Define o nome da linguagem
  void setName(const QString &obj_name);

  //Define se a linguagem é confiável ou não
  void definirConfiavel(bool valor);

  void definirFuncao(Funcao *funcao, unsigned tipo_func);
  bool linguagemConfiavel(void);
  Funcao *obterFuncao(unsigned tipo_func);

  QString getCodeDefinition(unsigned tipo_def, bool forma_reduzida);
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
