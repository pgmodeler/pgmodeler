/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: FormBasico
# Descrição: Definição da classe que um container de widgets de configuração
#            de objetos no modelo de banco de dados.
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
#ifndef FROM_BASICO_H
#define FROM_BASICO_H

#include <QtGui>
#include "ui_formbasico.h"
#include "excecao.h"
#include "caixamensagem.h"
//***********************************************************
class FormBasico: public QDialog, public Ui::FormBasico {
 Q_OBJECT

 private:

 public:
   FormBasico(QWidget * parent = 0, Qt::WindowFlags f = 0);
  ~FormBasico(void){}
  void definirBotoes(unsigned conf_botoes=CaixaMensagem::BOTAO_OK_CANCELAR);

 public slots:
};
//***********************************************************
#endif
