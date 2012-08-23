/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Linguagem que é usado para
#             e gerar os códigos SQL pertinentes às linguagens
#             procedurais.
# Data de Criação: 19/05/2008
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
#ifndef LINGUAGEM_H
#define LINGUAGEM_H

#include "objetobase.h"
#include "funcao.h"
#include "papel.h"
//***********************************************************
class Linguagem: public ObjetoBase{
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
  void definirNome(const QString &nome);

  //Define se a linguagem é confiável ou não
  void definirConfiavel(bool valor);

  void definirFuncao(Funcao *funcao, unsigned tipo_func);
  bool linguagemConfiavel(void);
  Funcao *obterFuncao(unsigned tipo_func);

  QString obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida);
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
