/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright (C) 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# Q Public License version 1.0 (QPL 1.0).
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

/**
\ingroup libpgmodeler_ui
\class ModelOverviewWidget
\brief Definição da classe que implementa o widget de visão geral do modelo.
*/

#ifndef MODEL_OVERVIEW_WIDGET_H
#define MODEL_OVERVIEW_WIDGET_H

#include <QtGui>
#include "exception.h"
#include "ui_modeloverviewwidget.h"
#include "modelowidget.h"

class ModelOverviewWidget: public QWidget, public Ui::ModelOverviewWidget {
	private:
		Q_OBJECT

		//! \brief Modelo usado para exibição na visão geral
		ModeloWidget *model;

		//! \brief Fator de zoom da cena
		float zoom_factor;

		//! \brief Fator de redimensionamento dos widgets (20% do tamanho da cena)
		const static float RESIZE_FACTOR=0.20f;

		void mouseDoubleClickEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void closeEvent(QCloseEvent *event);
		void showEvent(QShowEvent *event);

		/*! \brief Atualiza a exibição da visão geral com a última modificação na cena, o parâmetro
		 'forcar_atual' força a atualização mesmo que o widget não esteja visível */
		void updateOverview(bool force_update);

	public:
		ModelOverviewWidget(QWidget *parent = 0);

	public slots:
		//! \brief Atualiza a visão geral (caso o widiget esteja visível)
		void updateOverview(void);

		//! \brief Redimensiona a geometria do frame de janela do mundo
		void resizeWindowFrame(void);

		//! \brief Redimensiona todo o widget de visão geral
		void resizeOverview(void);

		//! \brief Atualiza o fator de zoom da cena
		void updateZoomFactor(float zoom);

		//! \brief Define o modelo a ser exibido na visão geral
		void show(ModeloWidget *model);

	signals:
		//! \brief Sinal emitido quando a janela é aberta ou fechada (o parâmetro indica se a janela está ou não visível)
		void s_overviewVisible(bool);
};

#endif
