/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class ColorPìckerWidget
\brief Implements a basic widget to handle color selection.
*/

#ifndef COLOR_PICKER_WIDGET_H
#define COLOR_PICKER_WIDGET_H

#include <QWidget>
#include <QColorDialog>
#include <QHBoxLayout>
#include "ui_colorpickerwidget.h"
#include "exception.h"
#include <QTime>
#include <random>

using namespace std;

class ColorPickerWidget: public QWidget, public Ui::ColorPickerWidget {
	private:
		Q_OBJECT

		//! brief Random number generator engine used to generate random colors for buttons
		default_random_engine rand_num_engine;

		unsigned color_count;

		QList<QToolButton *> buttons;

	public:
		static const unsigned MAX_COLOR_BUTTONS=20;

		explicit ColorPickerWidget(unsigned color_count, QWidget * parent = 0);

		void setColor(unsigned color_idx, const QColor &color);
		QColor getColor(unsigned color_idx);

	private slots:
		void selectColor(void);
		void generateRandomColors(void);
};

#endif
