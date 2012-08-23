/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Sequencia que é usada para
#             gerar os códigos SQL pertinentes às sequências
#             ou auto-enumerações
# Data de Criação: 26/04/2008
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
#ifndef SEQUENCIA_H
#define SEQUENCIA_H

#include "objetobase.h"
#include "tabela.h"
//***********************************************************
class Sequencia: public ObjetoBase {
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

  //Define o esquema ao qual a sequência pertence
  void definirEsquema(ObjetoBase *esquema);

  //Métodos de obtenção dos atributos da seqüência
  bool sequenciaCiclica(void);
  QString obterValorMax(void);
  QString obterValorMin(void);
  QString obterIncremento(void);
  QString obterInicio(void);
  QString obterCache(void);
  Coluna *obterPossuidora(void);
  bool referenciaColunaIncRelacao(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif