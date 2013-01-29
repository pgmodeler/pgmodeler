/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

/**
\ingroup libpgmodeler_ui
\class PapelWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de esquemas.
*/

#ifndef PAPEL_WIDGET_H
#define PAPEL_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_papelwidget.h"
#include "tabelaobjetoswidget.h"

class PapelWidget: public BaseObjectWidget, public Ui::PapelWidget {
 Q_OBJECT

 private:
   /** @details Armazena as referência s tabelas de membros do papel para que sejam
      referenciados nos demais métodos de forma mais eficaz */
   TabelaObjetosWidget *tab_membros[3];

   /// @details Preenche as tabelas de membros do papel
   void preencherTabelaMembros(void);

   /// @details Exibe o dado do papel na tabela especificada
   void exibirDadosPapel(Role *papel, unsigned idx_tabela, unsigned lin);

 public:
   PapelWidget(QWidget * parent = 0);

   void hideEvent(QHideEvent *);
   void setAttributes(DatabaseModel *model, OperationList *op_list, Role *papel);

 private slots:
   /** @details Preenche com informações relacionadas ao papel selecionado
      na linha atual da tabela em foco */
   void exibirDadosPapelSelecionado(void);

   /** @details Seleciona um papel membro para ser inserido na tabela de membro
      atualmente em foco. Este slot é usando tanto para adicionar
      um membro na tabela quanto para editar um membro selecionado */
   void selecionarPapelMembro(void);

   /** @details Faz a conexão de sinais-slots para cada tabela de objetos
        medida que a tabela em foco mude */
   void configurarSelecaoPapeis(void);

 public slots:
   void applyConfiguration(void);
};

#endif
