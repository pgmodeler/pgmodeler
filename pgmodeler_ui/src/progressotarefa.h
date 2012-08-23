/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ProgressaoTarefa
# Descrição: Definição da classe que implementa um widget que sinaliza a progressão
#           de execução de tarefas
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
#ifndef PROGRESSO_TAREFA_H
#define PROGRESSO_TAREFA_H

#include <QtGui>
#include "ui_progressotarefa.h"
#include <map>
using namespace std;
//***********************************************************
class ProgressoTarefa: public QDialog, public Ui::ProgressoTarefa
{
 private:
  Q_OBJECT

  /* Armazena os icones que são exibidos conforme os tokes dos icones
     'id_icone' no slot executarProgresso() são enviados */
  map<unsigned, QIcon> icones;

 public:
  ProgressoTarefa(QWidget *parent=0, Qt::WindowFlags f=0);

  //Adiciona um ícone identificado pelo código 'id'
  void adicionarIcone(unsigned id, const QIcon &ico);

 public slots:
  void close(void);
  void executarProgesso(int progresso, const QString &texto, unsigned id_ico);
};
//***********************************************************
#endif
