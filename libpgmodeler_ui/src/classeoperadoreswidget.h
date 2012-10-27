/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: FamiliaOperadoresWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição dos atributos de classe de operadores.
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
#ifndef CLASSEOPERADORES_WIDGET_H
#define CLASSEOPERADORES_WIDGET_H

#include "objetobasewidget.h"
#include "ui_classeoperadoreswidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

/* Declarando a classe ElemClasseOperadores como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com o formulário de edição
 de classes de operadores.*/
#include <QMetaType>
Q_DECLARE_METATYPE(ElemClasseOperadores)

class ClasseOperadoresWidget: public ObjetoBaseWidget, public Ui::ClasseOperadoresWidget {
 Q_OBJECT

 private:
   SeletorObjetoWidget *sel_familiaop,
                       *sel_funcao,
                       *sel_operador;

   TipoPgSQLWidget *tipo_dado,
                   *tipo_armazenamento;

   //Tabela de objetos que armazena os elementos de classe de operadores
   TabelaObjetosWidget *tab_elementos;

 public:
   ClasseOperadoresWidget(QWidget * parent = 0);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, ClasseOperadores *classe_op);

 private slots:
   void hideEvent(QHideEvent *);

   /* Métodos que muda a visualização do formulário de configuração de elementos
      conforme o tipo de elemento selecionado no combobox 'tipo_elem_cmb' */
   void selecionarTipoElemento(int tipo);

   //Exibe as informações de um dado elemento na tabela de elementos na linha especificada
   void exibirDadosElemento(ElemClasseOperadores elem, int idx_linha);

   /* Configura uma instância da classe ElemClasseOperadores com os dados fornecidos no formulário
      adicionalmente o elemento configurado é exibido na linha especificada da tabela de elementos */
   void manipularElemento(int idx_linha);

   /* Preenche o formulário de elemento com os dados do elemento selecionado na tabela para
      permitir uma posterior atualização dos atributos */
   void editarElemento(int idx_linha);

 public slots:
   void aplicarConfiguracao(void);
};

#endif
