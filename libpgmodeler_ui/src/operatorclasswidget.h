/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: OperatorClassWidget
# Description: Implements the operations to create/edit operator class via form.
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
#ifndef OPERATOR_CLASS_WIDGET_H
#define OPERATOR_CLASS_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_operatorclasswidget.h"
#include "tipopgsqlwidget.h"
#include "tabelaobjetoswidget.h"

/* Declaring the OperatorClassElement class as a Qt metatype in order to permit
   that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(OperatorClassElement)

class OperatorClassWidget: public BaseObjectWidget, public Ui::OperatorClassWidget {
  private:
   Q_OBJECT

   //Family, funciton and operator selectors
   SeletorObjetoWidget *family_sel,
                       *function_sel,
                       *operator_sel;

   TipoPgSQLWidget *data_type,
                   *storage_type;

   //Stores the elements of operator class
   TabelaObjetosWidget *elements_tab;

   void hideEvent(QHideEvent *event);

 public:
   OperatorClassWidget(QWidget * parent = 0);
   void setAttributes(DatabaseModel *model, OperationList *op_list, OperatorClass *op_class);

 private slots:
   //Configures the element form according to the passed element type
   void selectElementType(int elem_type);

   //Shows the data from the passed element on the element table
   void showElementData(OperatorClassElement elem, int lin_idx);

   /* Configures an instance of OperatorClassElement using the data on the form, additionally
      the configured element  is shown on the specified line on table */
   void handleElement(int lin_idx);

   /* Preenche o formulário de elemento com os dados do elemento selecionado na tabela para
      permitir uma posterior atualização dos atributos */

   //Fill the form fields with the element data on the specified table line
   void editElement(int lin_idx);

 public slots:
   void applyConfiguration(void);
};

#endif
