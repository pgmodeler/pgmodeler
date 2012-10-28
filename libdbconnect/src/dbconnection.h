/*
# Projeto: Squid-Autoconf
# Sub-project: Biblioteca libconexbd
# Class: DBConnection
# Description: This class encapsulates some connection manipulation functions
#              implemented by the PostgreSQL libpq
#
# Creation date: 13/07/2009
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
#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "resultset.h"
#include <map>

class ConexaoBD {
 private:
  //Descritor de conexão com o banco
  PGconn *conexao;

  //Parâmetros usados para gerar a string de conexão
  map<QString, QString> params_conexao;

  //String de conexão formatada
  QString str_conexao;

  //Gera a string de conexão com base no mapa de parâmetros de conexão
  void gerarStringConexao(void);

 public:
  static const QString PARAM_FQDN_SERVIDOR,
                       PARAM_IP_SERVIDOR,
                       PARAM_PORTA,
                       PARAM_NOME_BD,
                       PARAM_USUARIO,
                       PARAM_SENHA,
                       PARAM_TEMPO_CONEXAO,
                       PARAM_OPCOES,
                       PARAM_MODO_SSL,
                       PARAM_CERT_SSL,
                       PARAM_CHAVE_SSL,
                       PARAM_CERT_RAIZ_SSL,
                       PARAM_CRL_SSL,
                       PARAM_SERVIDOR_KERBEROS,
                       PARAM_LIB_GSSAPI,
                       SSL_DESATIVADO,
                       SSL_PERMITIR,
                       SSL_PREFERIR,
                       SSL_REQUERER,
                       SSL_VERIF_AUT_CERT,
                       SSL_VERIF_COMPLETA;
  ConexaoBD(void);
  ConexaoBD(const QString &servidor, const QString &porta, const QString &usuario, const QString &senha, const QString &nome_bd);
  ~ConexaoBD(void);

  /* Define um parâmetro de conexão. Este só deve ser chamado antes de se
     conectar ao banco */
  void definirParamConexao(const QString &parametro, const QString &valor);

  //Cria uma conexão simples com o banco
  void conectar(void);

  //Reinicia a conexão com o banco de dados
  void reiniciar(void);

  //Fecha a conexão aberta
  void fechar(void);

  //Retorna o valor definido atualmente para um dado parâmetro da conexão
  QString obterParamConexao(const QString &parametro);

  //Retorna o mapa com os parâmetros de configuração da conexão
  map<QString, QString> obterParamsConexao(void);

  //Retorna a string de conexão usada para se conectar ao banco
  QString obterStringConexao(void);

  //Retorna a versão do SGBD no formato XX.YY.ZZ
  QString obterVersaoSGBD(void);

  //Retorna se a conexão está estabelecida
  bool conexaoEstabelecida(void);

  /* Executa um comando DML no servidor usando a conexão aberta
     retornando um objeto de resutlado da consulta */
  void executarComandoDML(const QString &sql, ResultSet &resultado);

  /* Executa um comando DDL no servidor usando a conexão aberta
     sem retorno de objetos de resultado */
  void executarComandoDDL(const QString &sql);

  //Atribui uma conexão a outra
  void operator = (ConexaoBD &conex);
};

#endif
