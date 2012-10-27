/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: PermissaoWidget
# Description:o: Definição da classe que implementa o formulário de
#            edição dos atributos de permissões de objetos.
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
#ifndef PERMISSAO_WIDGET_H
#define PERMISSAO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_permissaowidget.h"
#include "tabelaobjetoswidget.h"

class PermissaoWidget: public ObjetoBaseWidget, public Ui::PermissaoWidget {
 Q_OBJECT

 private:
   //Armazena a permissão selecionada atualmente para edição
   Permissao *permissao;
   //Conteineres de papéis e de permissões
   TabelaObjetosWidget *tab_papeis, *tab_permissoes;

 public:
   PermissaoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ObjetoBase *objeto_pai, ObjetoBase *objeto);

 private slots:
   //Exibe a janela de seleção de objetos do modelo
   void selecionarPapel(void);

   //Seleciona uma permissão cujo índice é especificado no parâmetro
   void selecionarPermissao(int idx_perm);

   //Adiciona ao modelo a permissão configurada no formulário
   void adicionarPermissao(void);

   /* Método de configuração de uma permissão. Este método
      faz a atribuição dos valores preenchidos no formulário
        permissão do parâmetro. Este método é compartilhado
      entre as ações de adição e atualização de permissão */
   void configurarPermissao(Permissao *permissao);

   /* Preenche o formulário com os dados da permissão selecionada
      na tabela de permissões para uma posterior edição */
   void editarPermissao(void);

   //Remove a permissão selecionada na tabela de permissões
   void removerPermissao(void);

   //Remove todas as permissões da tabela de permissão
   void removerPermissoes(void);

   //Atualiza os dados da permissão atualmente em edição
   void atualizarPermissao(void);

   //Lista todas as permissões relacionadas ao objeto atual
   void listarPermissoes(void);

   /* Cancela a operação de criação/edição de uma permissão
      limpando o formulário e bloqueando os botões relacionados */
   void cancelarOperacao(void);

   //Exibe os dados do papel selecionado na tabela de papéis
   void exibirDadosPapelSelecionado(void);

   /* Habilita ou desabilita os botões de edição com base
      nos dados preenchidos ou não no formulário */
   void habilitarBotoesEdicao(void);

   /* Selecionao checkbox relacionados a um privilégio na
      tabela de privilégios */
   void marcarPrivilegio(void);

 public slots:
   void hideEvent(QHideEvent *);
};

#endif
