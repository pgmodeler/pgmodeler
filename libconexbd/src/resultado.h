/*
# Projeto: Squid-Autoconf
# Sub-projeto: Biblioteca libconexbd
# Classe: Resultado
# Descrição: Esta classe encapsula algumas funções de manipulação de resultados
#            de comandos SQL do PostgreSQL implementadas pela biblioteca libpq
# Data de Criação: 14/07/2009
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
#ifndef RESULTADO_H
#define RESULTADO_H

#include "excecao.h"
#include <libpq-fe.h>
#include <cstdlib>
//***********************************************************
class Resultado {
 private:
  /* Indica se o resultado foi copiado, este flag é utilizado
     para evitar falhas de segmentação na chamada do destrutor
     da classe, pois como o ponteiro resultado_sql é copiado
     para outro elementos, caso este seja destruído pode causar
     falha na referência. Sendo assim, tal ponteiro só é excluído
     quando este flag está marcado como falso */
  bool res_copiado;
  void destruirResultado(void);

 protected:
  //Armazena o índice atual da tupla, apenas para navegação
  int tupla_atual;

  /* Indica que o resultado foi gerado a partir de um comando
     o qual não gera tuplas. Exemplo: INSERT, DELETE, CREATE .. */
  bool res_sem_tuplas;

  //Armazena o objeto de resultado do comando SQL
  PGresult *resultado_sql;

  /* Esta classe só pode ser construída a partir de um
     resultado de comando SQL gerado na classe ConexaoBD */
  Resultado(PGresult *resultado_sql);

 public:
   //Constantes usadas na navegação nas tuplas de resultado
   static const unsigned TUPLA_INICIAL=0,
                         TUPLA_FINAL=1,
                         TUPLA_ANTERIOR=2,
                         TUPLA_POSTERIOR=3;

   Resultado(void);
  ~Resultado(void);

  //Retorna o valor de uma determinada coluna
  char *obterValorColuna(const QString &nome_coluna);
  char *obterValorColuna(int idx_coluna);

  //Retorna o tamanho do dado alocado em uma determinada coluna
  int obterTamanhoColuna(const QString &nome_coluna);
  int obterTamanhoColuna(int idx_coluna);

  /* Retorna o número de linhas afetadas pelo comando que gerou
     o resultado caso este seja um INSERT, DELETE, UPDATE ou o 
     número de tuplas vindas de um comando SELECT */
  int obterNumTuplas(void);

  //Retorna o número de colunas presentes em uma tupla
  int obterNumColunas(void);

  //Retorna o nome da coluna associada ao índice passado
  QString obterNomeColuna(int idx_coluna);

  //Retorna o índice da coluna associada ao nome passado
  int obterIndiceColuna(const QString &nome_coluna);

  //Retorna a tupla atual em que a navegação está
  int obterTuplaAtual(void);

  //Retorna se uma determinada coluna está em formato binário
  bool colunaFormatoBinario(const QString &nome_coluna);
  bool colunaFormatoBinario(int idx_coluna);

  //Método de navegação pelas tuplas do resultado
  bool acessarTupla(unsigned tipo_tupla);

  //Método de cópia entre resultados
  void operator = (Resultado &res);

  friend class ConexaoBD;
};
//***********************************************************
#endif
