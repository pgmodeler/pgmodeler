/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: TipoPgSQLWidget
# Descrição: Definição da classe que implementa o formulário de
#            edição de tipos nativos do PostgreSQL. Este formulário
#            é compartilhado por vários outros por isso a necessidade
#            de se criar esta classe com esse fim específico evitando
#            a redundância de código e de objetos gráficos.
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
#ifndef TIPOPGSQL_WIDGET_H
#define TIPOPGSQL_WIDGET_H

#include <QtGui>
#include "ui_tipopgsqlwidget.h"
#include "tipospgsql.h"
#include "destaquesintaxe.h"
#include "modelobd.h"
//***********************************************************
class TipoPgSQLWidget: public QWidget, public Ui::TipoPgSQLWidget {
 Q_OBJECT

 private:
   //Tipo configurado pelo formulário
   TipoPgSQL tipo;

   //Destacador de sintaxe usado para destacar o formato do tipo
   DestaqueSintaxe *destaque_fmt;

 public:
   TipoPgSQLWidget(QWidget * parent = 0, const QString &rotulo="");

   /* Método utilitário disponibilizado para as demais classes o qual
      obtém a lista de tipos do modelo em questão dando a possiblidade
      de quais tipos obter */
   static void obterTiposPgSQL(QComboBox *combo, ModeloBD *modelo,
                               bool tipo_usr=true, bool dominio=true,
                               bool tipo_oid=true, bool pseudo=true);

 private slots:
   /* Atualiza o formato do tipo à medida que os campos
      do formulário são modificados */
   void atualizarFormatoTipo(void);

 public slots:
   void definirAtributos(TipoPgSQL tipo, ModeloBD *modelo,
                         bool tipo_usr=true, bool dominio=true,
                         bool tipo_oid=true, bool pseudo=true);

   //Obtém o tipo configurado no formulário
   TipoPgSQL obterTipoPgSQL(void);
};
//***********************************************************
#endif
