/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Referencia que é usada para
#             para montar a declaração SQL das visões
# Data de Criação: 05/09/2008
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
#ifndef REFERENCIA_H
#define REFERENCIA_H

#include "tabela.h"
#include "esquema.h"
//***********************************************************
class Referencia {
 private:
  //Guarda a referência para uma tabela
  Tabela *tabela;

  //Guarda a referência para uma coluna da tabela
  Coluna *coluna;

  //Armazena uma expressão que forma a declaração de uma visão
  QString expressao,
         //Armazena um alias para expressão ou tabela
         alias,
         //Armazena um alias para coluna
         alias_coluna;

 public:
  //Constantes usada para identificar o tipo de referência
  static const unsigned REFER_COLUNA=0,
                        REFER_EXPRESSAO=1;

  //Constantes usadas na geração do SQL da referência
  static const unsigned SQL_REFER_WHERE=10,
                        SQL_REFER_SELECT=20,
                        SQL_REFER_FROM=30;

  Referencia(void);

  //Construtor o qual cria uma referência a uma coluna/tabela
  Referencia(Tabela *tabela, Coluna *coluna, const QString &alias_tab, const QString &alias_col);

  //Construtor o qual cria uma referência a uma expressão
  Referencia(const QString &expressao, const QString &alias_exp);

  //Obtém a tabela referenciada
  Tabela *obterTabela(void);

  //Obtém a coluna referenciada
  Coluna *obterColuna(void);

  //Retorna o alias da coluna referenciada
  QString obterAliasColuna(void);

  //Retorna o alias da tabela ou expressão
  QString obterAlias(void);

  //Retorna a expressão
  QString obterExpressao(void);

  /* Retorna o tipo da referência
    (ref. a um objeto ou ref. a uma expressão) */
  unsigned obterTipoReferencia(void);

  //Obtém a definição SQL da referência
  QString obterDefinicaoSQL(unsigned tipo_sql);
  QString obterDefinicaoXML(void);

  //Compara os atributos de duas referências
  bool operator == (Referencia &refer);
};
//***********************************************************
#endif
