/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: FormExportacao
# Descrição: Formulário de exportação do modelo
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef FORM_EXPORTACAO_H
#define FORM_EXPORTACAO_H

#include "ui_formexportacao.h"
#include "parseresquema.h"
#include "modelobd.h"
//***********************************************************
class FormExportacao: public QDialog, public Ui::FormExportacao {
 private:
  Q_OBJECT
  ModeloBD *modelo;

 public:
  FormExportacao(QWidget * parent = 0, Qt::WindowFlags f = 0);

 public slots:
  void show(ModeloBD *modelo);
  void hideEvent(QHideEvent *);

 private slots:
  void habilitarTipoExportacao(void);
  void exportarModelo(void);
  void selecionarArquivoDestino(void);
  void ocultarProgressoExportacao(void);
};
//***********************************************************
#endif
