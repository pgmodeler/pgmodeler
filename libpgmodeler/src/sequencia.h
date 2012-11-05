/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definição da classe Sequencia que é usada para
#             gerar os códigos SQL pertinentes as sequências
#             ou auto-enumerações
# Creation date:o: 26/04/2008
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
#ifndef SEQUENCIA_H
#define SEQUENCIA_H

#include "objetobase.h"
#include "tabela.h"

class Sequencia: public BaseObject {
 private:
  //Define se a sequencia é cíclica
  bool ciclica;

          //Valor mínimo
  QString  valor_min,
          //Valor máximo
          valor_max,
          //Valor atual da sequencia
          inicio,
          //Incemento da sequencia
          incremento,
          //Valor de cache da sequencia
          cache;

  //Objeto possuidor da sequencia
  Coluna *coluna;

  /* Retorna se o valor passado é constituido de números
     e inicia com + ou - */
  bool valorValido(const QString &valor);

  /* Retorna se o valor passado é nulo (0) */
  bool valorNulo(const QString &valor);

  //Retorna um valor formatado excluindo operadores adicionais
  QString formatarValor(const QString &valor);

  /* Compara dois valores QString em formato numérico e retorna
     -1 se o valor1 é menor que o valor2, 0 se os dois são iguais
     e 1 se valor1 é maior que valor 2 */
  int compararValores(QString valor1, QString valor2);

 public:
  /* Constantes as quais indicam os valores mínimo e
     máximo aceito por uma sequência */
  static const QString VALOR_MAX_POSITIVO;
  static const QString VALOR_MAX_NEGATIVO;

  Sequencia(void);

  //Define se a seqüência é cíclica
  void definirCiclica(bool valor);

  /* Define todos os valores numéricos da sequência de uma só vez.
     Isso é feito pois existe uma dependência entre eles e validações
     necessárias antes de atribuí-los a  sequência */
  void definirValores(QString vmin, QString vmax,
                      QString inc, QString inicio,
                      QString cache);

  //Define a tabela/coluna possuidora da sequência
  void definirPossuidora(Tabela *tabela, const QString &nome_coluna);
  void definirPossuidora(Coluna *coluna);

  void setName(const QString &obj_name);

  //Define o esquema ao qual a sequência pertence
  void setSchema(BaseObject *schema);

  //Métodos de obtenção dos atributos da seqüência
  bool sequenciaCiclica(void);
  QString obterValorMax(void);
  QString obterValorMin(void);
  QString obterIncremento(void);
  QString obterInicio(void);
  QString obterCache(void);
  Coluna *obterPossuidora(void);
  bool referenciaColunaIncRelacao(void);

  void operator = (Sequencia &seq);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
};

#endif
