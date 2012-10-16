/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libutil
# Descrição:  Definição do namespace AtributosGlobais, um espaço de nomes o qual
#             armazena uma série de strings estáticas constantes, usadas
#             para referênciar as variáveis de configuração globais do software.
# Data de Criação: 14/05/2010
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
#ifndef ATRIBUTOS_GLOBAIS_H
#define ATRIBUTOS_GLOBAIS_H

#include <cstdlib>
#include <QString>
#include <QDir>
//***********************************************************
namespace AtributosGlobais {

/* Variáveis usadas para referenciar os diretórios do
   pgModeler. Por padrão, ele busca os diretórios
   conf/, schemas/ e lang/ na pasta de execução atual.
   Porém estes valores podem ser sobrescritos usandos as
   variáveis de ambiente:
     PGMODELER_SCHEMAS_DIR
     PGMODELER_CONF_DIR
     PGMODELER_LANG_DIR
     PGMODELER_PLUGINS_DIR
     PGMODELER_TMP_DIR
*/

static const QString
  /** ticket#1 **/
  /* De acordo com a documentação da libXml, os caminhos usados pelo parser são
     em URI (ex.: file://a/b/c) logo, em Windows, os caminhos são em formato
     C:\a\b\c, isso provocava o erro no parser que não conseguia localizar
     as DTD's. A solução para esse problema é substituir as '\' do caminho por '/' */

  /** issue#7 **/
  /* Correção de referência incorreta às variáveis de ambiente */
  /* Caso a variável não esteja especificada, o pgModeler procura a pasta requisitada no diretório de atual "." */
  DIR_RAIZ_ESQUEMAS=(getenv("PGMODELER_SCHEMAS_DIR") ? QString(getenv("PGMODELER_SCHEMAS_DIR")).replace("\\","/") : QString("./schemas")),
  DIR_CONFIGURACOES=(getenv("PGMODELER_CONF_DIR") ? QString(getenv("PGMODELER_CONF_DIR")).replace("\\","/") : QString("./conf")),
  DIR_LINGUAS=(getenv("PGMODELER_LANG_DIR") ? QString(getenv("PGMODELER_LANG_DIR")).replace("\\","/") : QString("./lang")),
  DIR_PLUGINS=(getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace("\\","/") : QString("./plugins")),
  DIR_TEMPORARIO=(getenv("PGMODELER_TMP_DIR") ? QString(getenv("PGMODELER_TMP_DIR")).replace("\\","/") : QString("./tmp"));

static const QString
   VERSAO_PGMODELER="0.3.4-rc",
   ARQ_CRASH_HANDLER="pgmodeler%1.crash",
   ARQ_STACKTRACE=".stacktrace",

   SEP_DIRETORIO="/",
   DIR_CONF_PADRAO="defaults", //Nome do diretório o qual guarda as configurações padrão do pgModeler
   DIR_ESQUEMAS="schemas", //Nome padrão de diretórios de esquemas
   DIR_ESQUEMA_SQL="sql", //Nome padrão para diretório de esquemas sql
   DIR_ESQUEMA_XML="xml", //Nome padrão para diretório de esquemas xml
   DIR_ESQUEMA_COMUM="common", //Nome padrão para diretório de esquemas comuns entres as versões do pgsql
   EXT_ESQUEMA=".sch", //Extensão padrão de arquivos de esquema
   DIR_DTD_OBJETO="dtd", //Diretório padrão de arquivos dtd para os xml de objetos
   EXT_DTD_OBJETO=".dtd", //Extensão padrão de arquivos dtd
   DTD_RAIZ="dbmodel", //Nome do DTD raiz dos arquivos XML de modelo
   EXT_CONFIGURACAO=".conf", //Extensão padrão para arquivos de configuração
   SUF_ARQ_DESTAQUE="-highlight", //Sufixo de arquivos de destaque de sintaxe de linguagens

   CONF_DESTAQUE_CODIGO="source-code-highlight", //Nome padrão para o dtd de sintaxe de linguagem
   CONF_ESTILO_OBJETOS="objects-style", //Nome padrão para o arquivo de estilo de objetos
   CONF_GERAL="pgmodeler", //Nome padrão para o arquivo de configuração geral do pgModeler
   CONF_CONEXOES="connections", //Nome padrão para o arquivo de configuração de conexões a SGBDs

   CONF_DESTAQUE_SQL="sql-highlight", //Arquivo de configuração de destaque da linguagem SQL
   CONF_DESTAQUE_XML="xml-highlight", //Arquivo de configuração de destaque da linguagem XML

   MODELO_EXEMPLO="example.pgmodel"; //Nome padrão do arquivo de modelo de exemplo da configuração de aparência de objetos
};
//***********************************************************
#endif
