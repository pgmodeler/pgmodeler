/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ElementoIndice que é usada para
#             armazenar e gerar os códigos SQL/XML pertinentes aos atributos
#             usados nas índices.
# Data de Criação: 26/05/2010
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
#ifndef ELEMENTO_INDICE_H
#define ELEMENTO_INDICE_H

#include "coluna.h"
#include "classeoperadores.h"
//***********************************************************
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
//***********************************************************
#endif
