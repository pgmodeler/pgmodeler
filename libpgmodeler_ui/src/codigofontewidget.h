/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: CodigoFonteWidget
# Description:Definição da classe que implementa o formulário de visualização
#            do código SQL/XML de um dado objeto.
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
#ifndef CODIGOFONTE_WIDGET_H
#define CODIGOFONTE_WIDGET_H

#include "ui_codigofontewidget.h"
#include "objetobasewidget.h"

class CodigoFonteWidget: public ObjetoBaseWidget, public Ui::CodigoFonteWidget {
 Q_OBJECT
 private:
   DestaqueSintaxe *destaque_sql,
                   *destaque_xml;

   void aplicarConfiguracao(void){}

 public:
   CodigoFonteWidget(QWidget * parent = 0);
   void hideEvent(QHideEvent *evento);
   void definirAtributos(ModeloBD *modelo, BaseObject *objeto=NULL);

 private slots:
   void gerarCodigoFonteObjeto(int=0);
   void definirAbaCodigoFonte(int=0);
};

#endif

