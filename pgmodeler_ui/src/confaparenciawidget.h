/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ConfAparenciaWidget
# Descrição: Implementa o widget de configurações de aparência dos objetos do modelo.
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
#ifndef CONF_APARENCIA_WIDGET_H
#define CONF_APARENCIA_WIDGET_H

#include "ui_confaparenciawidget.h"
#include "confbasewidget.h"
#include "cenaobjetos.h"
#include "modelobd.h"
#include <algorithm>
//***********************************************************
class ConfAparenciaWidget: public QWidget, public Ui::ConfAparenciaWidget, public ConfBaseWidget {
 private:
  Q_OBJECT

  //Classe auxiliar que armazena os dados da formatação de cada elemento
  class ItemConfAparencia {
   public:
    QString id_conf;
    QTextCharFormat fmt_fonte;
    QColor cores[3];
    bool conf_obj;
  };

  //Diálogo de seleção de cor
  QColorDialog cor_dlg;

  //Viewport que mostra o modelo de exemplo
  QGraphicsView *viewp;

  //Cena que armazena os objetos do modelo de exemplo
  CenaObjetos *cena;

  //Modelo de objetos exemplo
  ModeloBD *modelo;

  //Vetor o qual armazena cada elemento de configuração de aparência do modelo
  vector<ItemConfAparencia> itens_conf;

  //Carrega o modelo de exemplo
  void criarObjetosExemplo(void);

 public:
  ConfAparenciaWidget(QWidget * parent=0);
  ~ConfAparenciaWidget(void);

  void salvarConfiguracao(void);
  void carregarConfiguracao(void);

 private slots:
  //Habilita os itens do formulário conforme o elemento selecionado no combobox 'elemento_cmb'
  void habilitarElemConfiguracao(void);

  //Aplica o estilo de fonte ao elemento atual
  void aplicarEstiloFonte(void);

  //Aplica o estilo de cor ao elemento atual
  void aplicarCorElemento(void);

  /* Este método não é usado nesta classe pois as modificações são aplicadas diretamente
     ao arquivo, operação esta tratada pelo método salvarConfiguracao() */
  void aplicarConfiguracao(void){}

 public slots:
  void restaurarPadroes(void);
};
//***********************************************************
#endif
