/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Regra que é usado para configurar
#              e gerar os códigos SQL referentes a regras para tabelas.
# Data de Criação: 26/09/2006
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
#ifndef REGRA_H
#define REGRA_H

#include "objetotabela.h"
#include "coluna.h"
//***********************************************************
class Regra: public ObjetoTabela{
 private:
  //Comandos que a regra executará ao ser chamada
  vector<QString> comandos;

  //Expressão condicional da regra
  QString exp_condicional;

  //Tipo de execução dos comandos da regra (ALSO, INSTEAD)
  TipoExecucao tipo_exec;

  //Tipo de evento que dispara a regra (ON SELECT, NO UPDATE, ON INSERT, ON DELETE)
  TipoEvento tipo_evento;

  /* Formata a QString de comandos usada pelo parser de esquema
     na geração da definição SQL da regra */
  void definirAtributoComandos(void);

 public:
  Regra(void);

  //Adiciona um comando SQL que será executado pela regra
  void adicionarComando(const QString &comando);

  //Define a expressão condicional da regra
  void definirExpCondicional(const QString &exp);

  //Define o tipo de execução da regra
  void definirTipoExecucao(TipoExecucao tipo);

  //Define o tipo de evento no qual a regra é executada
  void definirTipoEvento(TipoEvento tipo);

  //Obtém um comando executado pela regra através de seu índice
  QString obterComando(unsigned idx_cmd);

  //Retorna a quantidade de comandos existentes
  unsigned obterNumComandos(void);

  //Obtém a expressão condicional da regra
  QString obterExpCondicional(void);

  //Retorna o tipo de evento de execução da regra
  TipoEvento obterTipoEvento(void);

  //Retorna o tipo de execução da regra
  TipoExecucao obterTipoExecucao(void);

  //Remove um comando da regra através do índice
  void removerComando(unsigned idx_cmd);

  //Remove todos os comandos da regra
  void removerComandos(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
