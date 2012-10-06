/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca libpgsqldbm_ui
# Classe: TipoWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de tipos definidos pelo usuário.
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
#ifndef TIPO_WIDGET_H
#define TIPO_WIDGET_H

#include "objetobasewidget.h"
#include "ui_tipowidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

/* Declarando a classe Parametro como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar instancias de classes em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe Parametro.*/
#include <QMetaType>
Q_DECLARE_METATYPE(Parametro)
//***********************************************************
class TipoWidget: public ObjetoBaseWidget, public Ui::TipoWidget {
 Q_OBJECT

 private:
   //Widgets seletores de funções do tipo
   SeletorObjetoWidget *sel_funcoes[7];

   //Widgets de configuração de tipo cópia e tipo de elemento
   TipoPgSQLWidget *tipo_copia,
                   *tipo_elemento;

   //Tabelas para armazenamento das enumerações e atributos de tipos compostos
   TabelaObjetosWidget *tab_enumeracoes,
                       *tab_atributos;

 public:
   TipoWidget(QWidget * parent = 0);
   //~TipoWidget(void);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tipo *tipo);

 private slots:
   void hideEvent(QHideEvent *);

   /* Manipula o formulário exibindo os campos de acordo com a configuração
      de tipo selecionada */
   void selecionarConfiguracaoTipo(void);

   //Manipula as enumerações do tipo
   void manipularEnumeracao(int idx_linha);

   //Exibe o form de parâmetros usado como editor de atributos de tipos compostos
   void exibirFormAtributo(void);
   //Exibe os atributos do tipo composto na tabela de atributos
   void manipularAtributo(int res);

 public slots:
   void aplicarConfiguracao(void);
};
//***********************************************************
#endif
