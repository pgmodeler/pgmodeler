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
\class FormRestrauracaoModelos
\brief Formulário de restauração de modelos temporários
*/

#ifndef FORM_RESTAURACAO_MODELOS_H
#define FORM_RESTAURACAO_MODELOS_H

#include <QtGui>
#include "globalattributes.h"
#include "ui_formrestauracaomodelo.h"

class FormRestauracaoModelo: public QDialog, public Ui::FormRestauracaoModelos {
 private:
  Q_OBJECT

 public:
  FormRestauracaoModelo(QWidget * parent = 0, Qt::WindowFlags f = 0);

  /// @details Obtém a lista de arquivos temporários
  QStringList obterModelosTemporarios(void);

  /// @details Obtém a lista de arquivos temporários selecionados para restauração
  QStringList obterModelosSelecionados(void);

 public slots:
  void exec(void);
  /// @details Exclui todos os arquivos temporários da pasta tmp
  void excluirModelosTemporarios(void);
  /// @details Retorna se existe arquivos temporários na pasta
  bool existeModelosTemporarios(void);

 private slots:
  void habilitarRestauracao(void);
};

#endif
