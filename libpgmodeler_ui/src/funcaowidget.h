/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: FuncaoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de funções.
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
#ifndef FUNCAO_WIDGET_H
#define FUNCAO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_funcaowidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

class FuncaoWidget: public ObjetoBaseWidget, public Ui::FuncaoWidget  {
 Q_OBJECT

 private:
   /* Destaque de código-fonte usado para destacar o código da função
      definido pelo usuário */
   DestaqueSintaxe *destaque_codigo;

   //Widget de tipo Pgsql usado para configurar o tipo de retorno da função
   TipoPgSQLWidget *tipo_ret;

                       //Tabela que representa a tabela de retorno da função
   TabelaObjetosWidget *tab_retorno,
                       //Tabela de parâmetros da função
                       *tab_parametros;

   //Converte os dados da linha da tabela informada em um parâmetro
   Parametro obterParametro(TabelaObjetosWidget *tab, unsigned idx_lin);

   //Exibe os dados do parâmetro na tabela e linha selecionadas
   void exibirDadosParametro(Parametro param, TabelaObjetosWidget *tab, unsigned idx_lin);

   /* Valida a nova configuração da função em relação a demais objetos que a referenciam.
     A exemplo disso temos objetos das classes ConversaoCodificacao, ConversaoTipo,
     FuncaoAgregada, Gatilho, Linguagem, Operador, Tipo */
   void validarFuncaoConfigurada(void);

 public:
   FuncaoWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Funcao *funcao);

 private slots:
   void alternarTiposRetorno(void);

   /* Seleciona a linguagem de definição da função, e caso disponível,
      carrega o arquivo de destaque de sintaxe referênt�  linguagem
      selecionada */
   void selecionarLinguagem(void);

   //Exibe a janela de configuração de parâmetros
   void exibirFormParametro(void);

   /* Manipula um parâmetro que foi configurado pelo form. de parâmetro
      exibindo seus dados na tabela da parâmetros correta */
   void manipularParametro(int);

   void hideEvent(QHideEvent *);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
