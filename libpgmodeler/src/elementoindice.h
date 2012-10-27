/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definição da classe ElementoIndice que é usada para
#             armazenar e gerar os códigos SQL/XML pertinentes aos atributos
#             usados nas índices.
# Creation date:o: 26/05/2010
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
#ifndef ELEMENTO_INDICE_H
#define ELEMENTO_INDICE_H

#include "coluna.h"
#include "classeoperadores.h"

class ElementoIndice {
 private:
  /* Coluna referenciada pelo elemento do índice. Este atributo é
     mutuamente exclusivo com o atributo expressao, ou seja,
     quando um é setado o outro tem seu valor zerado */
  Coluna *coluna;

  /*Expressão referenciada pelo elemento do índice. Este atributo é
    mutuamente exclusivo com o atributo coluna, ou seja,
    quando um é setado o outro tem seu valor zerado */
  QString expressao;

  //Classe de operadores referenciada pelo elemento
  ClasseOperadores *classe_oper;

  //Atributos booleanos do elemento (ASC|DESC, NULLS FIRST|LAST)
  bool atrib_elemento[2];

 public:
   const static unsigned ORDEM_ASCENDENTE=0;
   const static unsigned NULOS_PRIMEIRO=1;

   ElementoIndice(void);
   ~ElementoIndice(void){};

   //Métodos de configuração do elemento
   void definirColuna(Coluna *coluna);
   void definirExpressao(const QString &expressao);
   void definirClasseOperadores(ClasseOperadores *classe_oper);

   //Define o estado de uma das 2 configurações booleanas do elemento
   void definirAtributo(unsigned id_atrib, bool valor);

   //Obtém uma das configurações do elemento
   bool obterAtributo(unsigned id_atrib);

   //Métodos de obtenção dos atributos do elemento
   Coluna *obterColuna(void);
   QString obterExpressao(void);
   ClasseOperadores *obterClasseOperadores(void);

   //Retorna a definição SQL ou XML do objeto
   QString obterDefinicaoObjeto(unsigned tipo_def);
};

#endif
