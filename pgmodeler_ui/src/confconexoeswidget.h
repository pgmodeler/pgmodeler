/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ConfConexoesWidget
# Descrição: Implementa o widget de configurações de conexões com o sgbd.
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef CONF_CONEXOES_WIDGET_H
#define CONF_CONEXOES_WIDGET_H

#include "ui_confconexoeswidget.h"
#include "confbasewidget.h"
#include "conexaobd.h"
//***********************************************************
class ConfConexoesWidget: public QWidget, public Ui::ConfConexoesWidget, public ConfBaseWidget {
 private:
  Q_OBJECT

  //Configura uma instância de conexão com os valores preenchidos no formulário
  void configurarConexao(ConexaoBD *conexao);

 public:
  ConfConexoesWidget(QWidget * parent=0);
  ~ConfConexoesWidget(void);

  void salvarConfiguracao(void);
  void carregarConfiguracao(void);

  //Preenche um mapa com as conexões carregadas no formulário
  void obterConexoes(map<QString, ConexaoBD *> &conexoes);

 public slots:
  void restaurarPadroes(void);

 private slots:
   //Limpa o formulário para criação de uma nova conexão
   void novaConexao(void);

   //Manipula (cria / atualiza) uma conexão com base nos dados do formulário
   void manipularConexao(void);

   //Edita uma conexão selecionada no combo de conexões
   void editarConexao(void);

   //Efetua o teste de conexão com base nos dados do formulário
   void testarConexao(void);

   //Remove a conexão selecionada no combo
   void removerConexao(void);

   //Habilita os campos relacionados com os certificados digitais
   void habilitarCertificados(void);

   //Habilita o botão de teste de conexão
   void habilitarTesteConexao(void);

   /* Este método não é usado nesta classe pois as modificações são aplicadas diretamente
      ao arquivo, operação esta tratada pelo método salvarConfiguracao() */
   void aplicarConfiguracao(void){}
};
//***********************************************************
#endif
