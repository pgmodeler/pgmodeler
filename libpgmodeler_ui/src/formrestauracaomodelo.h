/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libpgsqldbm_ui
# Classe: FormRestrauracaoModelos
# Descrição: Formulário de restauração de modelos temporários
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
#ifndef FORM_RESTAURACAO_MODELOS_H
#define FORM_RESTAURACAO_MODELOS_H

#include <QtGui>
#include "atributosglobais.h"
#include "ui_formrestauracaomodelo.h"

class FormRestauracaoModelo: public QDialog, public Ui::FormRestauracaoModelos {
 private:
  Q_OBJECT

 public:
  FormRestauracaoModelo(QWidget * parent = 0, Qt::WindowFlags f = 0);

  //Obtém a lista de arquivos temporários
  QStringList obterModelosTemporarios(void);

  //Obtém a lista de arquivos temporários selecionados para restauração
  QStringList obterModelosSelecionados(void);

 public slots:
  void exec(void);
  //Exclui todos os arquivos temporários da pasta tmp
  void excluirModelosTemporarios(void);
  //Retorna se existe arquivos temporários na pasta
  bool existeModelosTemporarios(void);

 private slots:
  void habilitarRestauracao(void);
};

#endif
