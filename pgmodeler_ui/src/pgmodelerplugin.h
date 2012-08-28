/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: PgModelerLibrary
# Descrição: Definição da classe que  implementa  a base para suporte de plugins de terceiros,
#            baseados em bibliotecas compartilhadas.
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
#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "modelowidget.h"
//***********************************************************
class PgModelerPlugin: public QObject {
 private:
  Q_OBJECT

  //Instância da classe QLibrary que trata do carregamento bibliotecas em disco
  QLibrary biblioteca;

  //Armazena o nome do plugin (definido na configuração XML)
  QString nome_plugin;

  //Armazena o ícone do plugin (definido na configuração do XML)
  QPixmap icone_plugin;

 public:
  PgModelerPlugin(void);

  PgModelerPlugin(const QString &nome_conf);

  //Carrega o plugin através do arquivo XML de configuração
  void carregarPlugin(const QString &nome_conf);

  //Obtém a versão do plugin
  QString obterVersaoPlugin(void);

  //Obtém o ícone do plugin
  QPixmap obterIconePlugin(void);

  //Retorna a versão do pgModela na qual o plugin foi compilada
  QString obterVersaoPgModeler(void);

 public slots:
  /* Faz a chamada à função que executa o plugin.
     O plugin tentará localizar a função na biblioteca através do
     atributo 'nome_plugin' em maíusculo, caso este símbolo seja
     encontrado executa a função na biblioteca senão dispara um erro */
  void executarPlugin(ModeloWidget *modelo);
};
//***********************************************************
#endif
