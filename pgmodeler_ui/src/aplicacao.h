/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: Aplicacao
# Descrição: Esta classe é uma derivação da classe QApplication.
#            A mesa apenas sobrecarrega o método notify() de sua
#            classe pai para tratar as execeções disparadas cujas
#            instâncias são da classe Excecao.
# Data de Criação: 30/08/2007
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
#ifndef APLICACAO_H
#define APLICACAO_H

#include <QApplication>
#include <QMessageBox>
#include "caixamensagem.h"
//***********************************************************
extern CaixaMensagem *caixa_msg;
//-----------------------------------------------------------
class Aplicacao: public QApplication {
 private:
 public:

  //O construtor da classe executa o construtor da classe pai
  Aplicacao(int & argc, char ** argv) : QApplication(argc,argv)
  {

  }

  bool notify(QObject * receiver, QEvent * event)
  {
   try
   {
    //Executa normalmente o notify da classe pai
    return(QApplication::notify(receiver,event));
   }
   catch(Excecao &e)
   {
    //Exibe uma caixa de mensagem de erro crítico com a mensagem da exceção
    caixa_msg->show(e);
    return(false);
   }
   catch(...) //Capturando as demais execeções
   {
    //Exibe uma caixa de mensagem de erro crítico com a mensagem da exceção
    caixa_msg->show(trUtf8("Erro"),trUtf8("Exceção desconhecida capturada!"), CaixaMensagem::ICONE_ERRO);
    return(false);
   }
  }
};
//***********************************************************
#endif
