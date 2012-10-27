/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Descrição: Definição da classe FuncaoAgregacao que é usada para
#            gerar os códigos SQL pertinentes a esta� s funções
#            de agregação (aggregates)
#
# Creation date:o: 16/04/2008
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
#ifndef FUNCAO_AGREGACAO_H
#define FUNCAO_AGREGACAO_H

#include "objetobase.h"
#include "funcao.h"
#include "operador.h"

class FuncaoAgregacao: public ObjetoBase {
 private:
  /* Lista de tipos com os quais a função opera.
     Caso a mesma esteja vazia será considerado
     todos os tipos possíveis '*' */

 /* Para manter a compatibilidade com a sintaxe antiga,
    onde a função agregada aceita apenas um tipo de dado,
    a lista de tipos 'tipo_dados' deve possuir apenas 1 elemento.
    Para se declarar uma função de agregação a qual trabalha com vários
    tipos na sintaxe antiga, o único elemento da lista deve ser do tipo
    'any' */
  vector<TipoPgSQL> tipo_dados;

  //Função de transição
  Funcao *funcoes[2];

  //Tipo de dado para o valor de estado da função
  TipoPgSQL tipo_estado;

  //Condição inicial da função
  QString cond_inicial;

  //Operador de ordenação
  Operador *op_ordenacao;

  /* Formata a QString de tipos usada pelo parser de esquema
     na geração da definição SQL da função de agregação */
  void definirAtributoTipos(unsigned tipo_def);

  /* Valida as funções atribuia �  função de agregação de acordo
     com as regras da documentação */
  bool funcaoValida(unsigned idx_func, Funcao *func);

 public:
  //Constantes usadas para referenciar as funções usadas pela função de agregação
  static const unsigned FUNCAO_FINAL=0,
                        FUNCAO_TRANSICAO=1;

  FuncaoAgregacao(void);

  //Define uma função executada pela função de agregação
  void definirFuncao(unsigned idx_func, Funcao *func);

  //Define o tipo de dados do estado da função
  void definirTipoEstado(TipoPgSQL tipo_estado);

  //Define a condição inicial da função de agregação
  void definirCondicaoInicial(const QString &cond);

  //Define o operador de ordenação da função de agregação
  void definirOperadorOrdenacao(Operador *op_ordenacao);

  //Adiciona um tipo de dado aceito pela função de agregação
  void adicionarTipoDado(TipoPgSQL tipo);

  //Remove um tipo de dado aceito pela função de agregação
  void removerTipoDado(unsigned idx_tipo);

  //Remove todos os tipo de dados aceitos pela função de agregação
  void removerTiposDado(void);

  //Verifica se um tipo de dado já foi inserido
  bool tipoDadoExiste(TipoPgSQL tipo);

  //Métodos de obtenção de atributos da função de agregação
  Funcao *obterFuncao(unsigned idx_func);
  TipoPgSQL obterTipoEstado(void);
  QString obterCondicaoInicial(void);
  Operador *obterOperadorOrdenacao(void);
  TipoPgSQL obterTipoDado(unsigned idx_tipo);
  unsigned obterNumTipoDados(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};

#endif
