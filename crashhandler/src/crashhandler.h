/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: App crashhandler
# Classe: CrashHandler
# Descrição: Definição da classe que implementa o formulário de
#            tratamentos de crashing do pgModeler.
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
#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <QDialog>
#include "excecao.h"
#include "atributosglobais.h"
#include "ui_crashhandler.h"
#include "destaquesintaxe.h"
#include "caixamensagem.h"

#include <iostream>
#include <fstream>

class CrashHandler : public QDialog, Ui::CrashHandler
{
 private:
  Q_OBJECT

  //Destacador de sintaxe para o código modelo
  DestaqueSintaxe *dest_modelo_txt;

  //Caractere delimitador que separa as seções no arquivo compactado
  const static char CHR_DELIMITADOR;

 public:
  CrashHandler(QWidget * parent = 0, Qt::WindowFlags f = 0);

  //Carrega um arquivo .crash para ser analisado
  void carregarRelatorio(const QString &arquivo);

 public slots:
  //Gera um arquivo .crash
  void gerarRelatorio(void);
  void habilitarGeracao(void);
};

#endif
