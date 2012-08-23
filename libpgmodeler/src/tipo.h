/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Tipo que é usada para
#             gerar os códigos SQL pertinentes aos tipos
#             definidos pelo usuário
#
# Data de Criação: 05/06/2008
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
#ifndef TIPO_H
#define TIPO_H

#include "objetobase.h"
#include "esquema.h"
#include "funcao.h"
#include "papel.h"
//***********************************************************
class Tipo: public ObjetoBase {
 private:
  //Configuração do tipo
  unsigned config;

  //Atributos para a configuração TIPO_COMPOSTO
  vector<Parametro> atributos;

  //Atributos para a configuração TIPO_ENUMERACAO
  vector<QString> enumeracoes;

  //Atributos para a configuração TIPO_BASE
  //Funções usadas pelo tipo
  Funcao *funcoes[7];

  //Comprimento interno ( > 0 - TAMANHO FIXO, 0 - TAMANHO VARIÁVEL)
  unsigned comp_interno; //INTERNALLENGTH

  //Indica se o tipo é passado por valor
  bool por_valor; //PASSEDBYVALUE

  //Alinhamento de armazenamento (char, smallint (int2), integer (int4) ou double precision)
  TipoPgSQL alinhamento, //ALIGNMENT
            elemento; //ELEMENT

  //Tipo de armazenamento (plain, main, extended, external)
  TipoArmazenamento armazenamento; //STORAGE

  //Valor padrão do tipo
  QString valor_padrao; //DEFAULT

  //** Atributos Adicionados devido à alteraçõpes na versão 8.4 **/
  //Categoria do tipo (apenas para tipo base) - PADRÃO 'U'
  TipoCategoria categoria; //CATEGORY

  //Usado em conjunto ao tipo de categoria (apenas para tipo base) - Padrão FALSE
  bool preferido; //PREFERRED

  /* Tipo o qual terá alguns de seus atributos copiados para o tipo atual
     (apenas para tipo base). Caso o tipo_copia seja 'any' significa que o tipo
     atual não copia atributos de tipo algum */
  TipoPgSQL tipo_copia; //LIKE

  //Caractere delimitador de valores para arrays do tipo criado
  char delimitador; //DELIMITER

  //Verifica se um atibuto com o nome no parâmetro já existe
  bool atributoExiste(const QString &nome_atrib);

  //Verifica se uma enumeração com o nome no parâmetro já existe
  bool enumeracaoExiste(const QString &nome_enum);

  /* Formata a QString de elementos usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void definirAtributoElementos(unsigned tipo_def);

  /* Formata a QString de enumerações usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void definirAtributoEnumeracoes(unsigned tipo_def);

  /* Efetua a conversão dos parâmetros e tipos de retornos, de funções.
     Caso o parametro 'conv_inversa' estiver marcado, o método
     faz a conversão de 'nome_do_tipo' para 'any' caso contrário
     faz a conversão de 'any' para 'nome_do_tipo'. Este método é usado
     quando se gera os codigos SQL do tipo definido pelo usuário */
  void converterParametrosFuncoes(bool conv_inversa=false);

 public:
  static const unsigned TIPO_BASE=10,
                        TIPO_ENUMERACAO=20,
                        TIPO_COMPOSTO=30;

  static const unsigned FUNCAO_INPUT=0,
                        FUNCAO_OUTPUT=1,
                        FUNCAO_RECV=2,
                        FUNCAO_SEND=3,
                        FUNCAO_TPMOD_IN=4,
                        FUNCAO_TPMOD_OUT=5,
                        FUNCAO_ANALYZE=6;

  Tipo(void);
  ~Tipo(void);

  //Atribui um nome ao tipo
  void definirNome(const QString &nome);

  //Atribui um esquema ao tipo
  void definirEsquema(ObjetoBase *esquema);

  //Define a configuração do tipo
  void definirConfiguracao(unsigned conf);

  //Métodos de configuração para TIPO_COMPOSTO
  void adicionarAtributo(Parametro atrib);
  void removerAtributo(unsigned idx_atrib);
  void removerAtributos(void);

  //Métodos de configuração para TIPO_ENUMERACAO
  void adicionarEnumeracao(const QString &enumer);
  void removerEnumeracao(unsigned idx_enum);
  void removerEnumeracoes(void);

  //Métodos de configuração para TIPO_BASE
  void definirFuncao(unsigned conf_func, Funcao *funcao);
  void definirCompInterno(unsigned comp);
  void definirPorValor(bool valor);
  void definirAlinhamento(TipoPgSQL tipo);
  void definirArmazenamento(TipoArmazenamento armaz);
  void definirValorPadrao(const QString &valor_padrao);
  void definirElemento(TipoPgSQL elemento);
  void definirDelimitador(char delim);
  void definirCategoria(TipoCategoria categoria);
  void definirPreferido(bool preferido);
  void definirTipoCopia(TipoPgSQL tipo_copia);

  Parametro obterAtributo(unsigned idx_atrib);
  unsigned obterNumAtributos(void);
  QString obterEnumeracao(unsigned idx_enum);
  unsigned obterNumEnumeracoes(void);
  unsigned obterConfiguracao(void);
  TipoCategoria obterCategoria(void);
  bool tipoPreferido(void);
  TipoPgSQL obterTipoCopia(void);



  //Métodos de obtenção para TIPO_BASE
  Funcao *obterFuncao(unsigned conf_func);
  unsigned obterCompInterno(void);
  bool passadoPorValor(void);
  TipoPgSQL obterAlinhamento(void);
  TipoArmazenamento obterArmazenamento(void);
  QString obterValorPadrao(void);
  TipoPgSQL obterElemento(void);
  char obterDelimitador(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida);
  QString obterDefinicaoObjeto(unsigned tipo_def);

  void operator = (Tipo &tipo);

  friend class ModeloBD;
};
//***********************************************************
#endif
