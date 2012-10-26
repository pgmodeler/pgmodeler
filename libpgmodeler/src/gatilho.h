/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Gatilho que é usada para
#             configurar e gerar os códigos SQL referentes
#             a gatilhos (triggers) em tabelas.
# Data de Criação: 11/10/2006
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
#ifndef GATILHO_H
#define GATILHO_H

#include "objetotabela.h"
#include "funcao.h"

class Gatilho: public ObjetoTabela{
 protected:
  //Argumentos que são passados a função que o gatilho executa
  vector<QString> argumentos;

  /* Lista de colunas usadas como condição de disparo da trigger
     Este atributo foi adicionado no PostgreSQL 9.1 é usado somente
     quando o evento UPDATE está presente. */
  vector<Coluna *> colunas_upd;

  //Função que será executada com o disparo do gatilho
  Funcao *funcao;

  //Condição que garante ou não a execução do gatilhos
  QString condicao;

  //Momento de disparo da a trigger (BEFORE, AFTER, INSTEAD OF)
  TipoDisparo tipo_disparo;

  //Mapa de flags que indica em que evento o gatilho de ver disparado
  map<unsigned, bool> eventos;

  //Flag que indica que a função do gatilho deve ser executada por linha da tabela
  bool por_linha;

  //Tabela referenciada pelo gatilho (apenas para gatilho restrição)
  ObjetoBase *tabela_ref;

  //Indica se o gatílho é postergavel ou não (apenas para gatilho restrição)
  bool postergavel;

  //Tipo de postergação do gatilho (apenas para gatilho restrição)
  TipoPostergacao tipo_postergacao;

  void definirAtributosBasicosGatilho(unsigned tipo_def);

  /* Formata a QString de argumentos usada pelo parser de esquema
     na geração da definição SQL do gatilho */
  void definirAtributoArgumentos(unsigned tipo_def);


 public:
  static const unsigned MODO_BEFORE=0;
  static const unsigned MODO_AFTER=1;
  static const unsigned MODO_INSTEADOF=2;

  Gatilho(void);

  /* Adiciona uma coluna como condição de disparo (apenas para evento update)
     As colunas adicionadas por esse método devem obrigatoriamente pertencer
       coluna dona do gatilho e nã�  outras tabelas */
  void adicionarColuna(Coluna *coluna);

  //Adiciona um argumento ao gatilho
  void adicionarArgumento(const QString &arg);

  //Define em quais eventos (INSERT, DELETE, UPDATE, TRUNCATE) o gatilho pode ser executado
  void definirEvento(TipoEvento evento, bool valor);

  //Define a função que será executada quando o gatilho for chamado
  void definirFuncao(Funcao *funcao);

  //Define a condição de execução do gatilho
  void definirCondicao(const QString &cond);

  //Define a tabela referenciada
  void definirTabReferenciada(ObjetoBase *tabela_ref);

  //Define o tipo de deferimento
  void definirTipoPostergacao(TipoPostergacao tipo);

  //Define se o gatilho é postgergavel ou não
  void definirPostergavel(bool valor);

  /* Edita um argumento através de seu índice, passando também
     o novo valor que este receberá */
  void editarArgumento(unsigned idx_arg, const QString &novo_arg);

  //Define o momento de execução do gatilho
  void definirTipoDisparo(TipoDisparo tipo_disp);

  //Define se o gatlho deve ser executado por linha da tabela
  void executarPorLinha(bool valor);

  //Retorna se o gatlho é executado no evento informado
  bool executaNoEvento(TipoEvento evento);

  //Obtém uma coluna referenciada pelo gatilho através do índice
  Coluna *obterColuna(unsigned idx_col);

  //Obtém um argumento através de seu índice
  QString obterArgumento(unsigned idx_arg);

  //Obtém a condição definida para execução do gatilho
  QString obterCondicao(void);

  //Obtém a função executada pelo gatilho chamado
  Funcao *obterFuncao(void);

  //Obtém o número de argumentos
  unsigned obterNumArgs(void);

  //Obtém o número de colunas
  unsigned obterNumColunas(void);

  //Retorna se o gatilho executa antes de evento
  TipoDisparo obterTipoDisparo(void);

  //Remove um argumento pelo seu índice
  void removerArgumento(unsigned idx_arg);
  void removerArgumentos(void);
  void removerColunas(void);

  //Obtém a tabela referenciada
  ObjetoBase *obterTabReferenciada(void);

  //Obtém o tipo de deferimento da restrição
  TipoPostergacao obterTipoPostergacao(void);

  //Obtém se a restrição é postgergavel ou não
  bool gatilhoPostergavel(void);

  /* Retorna se o gatilho referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar gatilhos os quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool referenciaColunaIncRelacao(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};

#endif
