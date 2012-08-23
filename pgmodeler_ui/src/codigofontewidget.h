/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: CodigoFonteWidget
# Descrição: Definição da classe que implementa o formulário de visualização
#            do código SQL/XML de um dado objeto.
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
#ifndef CODIGOFONTE_WIDGET_H
#define CODIGOFONTE_WIDGET_H

#include "ui_codigofontewidget.h"
#include "objetobasewidget.h"
//***********************************************************
class CodigoFonteWidget: public ObjetoBaseWidget, public Ui::CodigoFonteWidget {
 Q_OBJECT
 private:
   DestaqueSintaxe *destaque_sql,
                   *destaque_xml;

   void aplicarConfiguracao(void){}

 public:
   CodigoFonteWidget(QWidget * parent = 0);
   void hideEvent(QHideEvent *evento);
   void definirAtributos(ModeloBD *modelo, ObjetoBase *objeto=NULL);

 private slots:
   void gerarCodigoFonteObjeto(int=0);
   void definirAbaCodigoFonte(int=0);
};
//***********************************************************
#endif

