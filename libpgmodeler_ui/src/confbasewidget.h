/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: ConfBaseWidget
# Description:o: Implementa operações básicas para gravação de arquios  e cesso � s configuraçoẽs
#            nos widgets de configurações.
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
#ifndef CONF_BASE_WIDGET_H
#define CONF_BASE_WIDGET_H

#include "exception.h"
#include "xmlparser.h"
#include "parsersattributes.h"
#include <algorithm>

class ConfBaseWidget {
 protected:
  //Mapa que armazena os parâmetros de configuração carregados do arquivos
  map<QString, map<QString, QString> > params_config;

  /* Salva as configurações em arquivo. O parâmetro 'id_conf' indica qual o
     tipo da configuração a ser salva. (vide AtributosGlobais::CONF_???) */
  void salvarConfiguracao(const QString &id_conf);

  /* Carrega as configurações do arquivo. O vetor 'atribs_chaves' é usado para especificar o nome de elementos
     considerados chave do mapa de configuração */
  void carregarConfiguracao(const QString &id_conf, const vector<QString> &atribs_chave=vector<QString>());

  //Obtém um parâmetro do arquivo de configuração carregado pelo ParserXML
  void obterParamsConfiguracao(const vector<QString> &atribs_chave);

  //Restaura as configurações padrão carregando-as do arquivo original (na pasta conf/defaults)
  void restaurarPadroes(const QString &id_conf);

 public:
  ConfBaseWidget(void){}

  //Adiciona um parâmetro de configuração ao mapa de configurações. Caso o mesmo já existe seus valores são substituídos
  void adicionarParamConfiguracao(const QString &param, const map<QString, QString> &atributos);

  //Obtém os parâmetros carregados do arquivo
  map<QString, map<QString, QString> > obterParamsConfiguracao(void);

  //Exclui os valores de um dado parâmetro de configuração
  void excluirParamConfiguracao(const QString &param);

  //Exclui todos os parâmetros de configuração
  void excluirParamsConfiguracao(void);

  /** Método puramente virtual **/
  //Aplica as configurações padrão
  virtual void aplicarConfiguracao(void)=0;
};

#endif
