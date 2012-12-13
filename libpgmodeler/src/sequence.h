/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Sequencia que é usada para
#             gerar os códigos SQL pertinentes as sequências
#             ou auto-enumerações
# Creation date: 26/04/2008
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
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "baseobject.h"
#include "tabela.h"

class Sequence: public BaseObject {
 private:
  //Define se a sequencia é cíclica
  bool cycle;

          //Valor mínimo
  QString  min_value,
          //Valor máximo
          max_value,
          //Valor atual da sequencia
          start,
          //Incemento da sequencia
          increment,
          //Valor de cache da sequencia
          cache;

  //Objeto possuidor da sequencia
  Column *owner_col;

  /* Retorna se o valor passado é constituido de números
     e inicia com + ou - */
  bool isValidValue(const QString &value);

  /* Retorna se o valor passado é nulo (0) */
  bool isNullValue(const QString &value);

  //Retorna um valor formatado excluindo operadores adicionais
  QString formatValue(const QString &value);

  /* Compara dois valores QString em formato numérico e retorna
     -1 se o valor1 é menor que o valor2, 0 se os dois são iguais
     e 1 se valor1 é maior que valor 2 */
  int compareValues(QString value1, QString value2);

 public:
  /* Constantes as quais indicam os valores mínimo e
     máximo aceito por uma sequência */
  static const QString MAX_POSITIVE_VALUE;
  static const QString MAX_NEGATIVE_VALUE;

  Sequence(void);

  //Define se a seqüência é cíclica
  void setCycle(bool value);

  /* Define todos os valores numéricos da sequência de uma só vez.
     Isso é feito pois existe uma dependência entre eles e validações
     necessárias antes de atribuí-los a  sequência */
  void setValues(QString minv, QString maxv,
                      QString inc, QString start,
                      QString cache);

  //Define a tabela/coluna possuidora da sequência
  void setOwnerColumn(Tabela *tabela, const QString &col_name);
  void setOwnerColumn(Column *column);

  void setName(const QString &name);

  //Define o esquema ao qual a sequência pertence
  void setSchema(BaseObject *schema);

  //Métodos de obtenção dos atributos da seqüência
  bool isCycle(void);
  QString getMaxValue(void);
  QString getMinValue(void);
  QString getIncrement(void);
  QString getStart(void);
  QString getCache(void);
  Column *getOwnerColumn(void);

  bool isReferRelationshipColumn(void);

  void operator = (Sequence &seq);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type);
};

#endif
