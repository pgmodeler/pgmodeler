/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição: Definição da classe Coluna que é utilizada pela classe Tabela.
# Data de Criação: 12/09/2006
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
#ifndef COLUNA_H
#define COLUNA_H

#include "objetotabela.h"
#include "dominio.h"
//***********************************************************
class Coluna: public ObjetoTabela{
 protected:
  /* Armazena o nome anterior da coluna antes da mudança de nome da mesma.
     Este atributo auxilia no processo de referência de colunas adicionadas
     por relacionamentos. */
  QString nome_antigo;

  //Indica se a coluna será não nula (com obrigatoriedade de preenchimento)
  bool nao_nulo;

  //Tipo da coluna da tabela
  TipoPgSQL tipo;

  /* Valor padrão da coluna.
     Obs: O usuário deve formatar o valor padrão de
          acordo com o exigido por cada tipo do banco de dados.
          Ex.: para um varchar(10) o valor padrão deve ser 'abcdef'(incluindo o ') ou
               para uma data '2006-09-12' e assim por diante. */
  QString valor_padrao;

 public:
  Coluna(void);

  //Define que a coluna é de preenchimento obrigatório (NOT NULL)
  void definirNaoNulo(bool valor);

  //Define o tipo da coluna
  void definirTipo(TipoPgSQL tipo);

  /* Define o valor padrão da coluna. Devem ser informados junto ao valor
     as particularidades de cada tipo, como aspas, hifens e etc. */
  void definirValorPadrao(const QString &valor);

    /* Define o nome da coluna. Este método mantém o último nome da coluna
     armazenado para auxiliar os métodos de relacionamento de colunas
     adicionadas por relacionamentos com restrições/indices e sequencias */
  void definirNome(const QString &nome);

  //Retorna se o campo é de preenchimento obrigatório
  bool naoNulo(void);

  //Retorna o tipo da coluna
  TipoPgSQL obterTipo(void);

  //Retorna o valor padrão da coluna
  QString obterValorPadrao(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);

  //Retorna o nome anterior da coluna
  QString obterNomeAntigo(bool formatar=false);

  //Obtém a referência ao tipo da coluna na forma [esquema].tabela.nome_coluna%TYPE
  QString obterReferenciaTipo(void);

  void operator = (Coluna &coluna);

  friend class Tabela;
};
//***********************************************************
#endif
