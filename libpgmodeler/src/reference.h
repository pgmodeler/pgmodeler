/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Referencia que é usada para
#             para montar a declaração SQL das visões
# Creation date: 05/09/2008
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
#ifndef REFERENCE_H
#define REFERENCE_H

#include "tabela.h"
#include "schema.h"

class Reference {
 private:
  //Guarda a referência para uma tabela
  Tabela *table;

  //Guarda a referência para uma coluna da tabela
  Column *column;

  //Armazena uma expressão que forma a declaração de uma visão
  QString expression,
         //Armazena um alias para expressão ou tabela
         alias,
         //Armazena um alias para coluna
         column_alias;

 public:
  //Constantes usada para identificar o tipo de referência
  static const unsigned REFER_COLUMN=0,
                        REFER_EXPRESSION=1;

  //Constantes usadas na geração do SQL da referência
  static const unsigned SQL_REFER_WHERE=10,
                        SQL_REFER_SELECT=20,
                        SQL_REFER_FROM=30;

  Reference(void);

  //Construtor o qual cria uma referência a uma coluna/tabela
  Reference(Tabela *table, Column *column, const QString &alias_tab, const QString &alias_col);

  //Construtor o qual cria uma referência a uma expressão
  Reference(const QString &expression, const QString &alias_exp);

  //Obtém a tabela referenciada
  Tabela *getTable(void);

  //Obtém a coluna referenciada
  Column *getColumn(void);

  //Retorna o alias da coluna referenciada
  QString getColumnAlias(void);

  //Retorna o alias da tabela ou expressão
  QString getAlias(void);

  //Retorna a expressão
  QString getExpression(void);

  /* Retorna o tipo da referência
    (ref. a um objeto ou ref. a uma expressão) */
  unsigned getReferenceType(void);

  //Obtém a definição SQL da referência
  QString getSQLDefinition(unsigned tipo_sql);
  QString getXMLDefinition(void);

  //Compara os atributos de duas referências
  bool operator == (Reference &refer);
};

#endif
