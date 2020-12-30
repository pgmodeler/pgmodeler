/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

class ColorPickerWidget: public QWidget, public Ui::ColorPickerWidget {
	private:
		Q_OBJECT
		
		//! \brief Random number generator engine used to generate random colors for buttons
		default_random_engine rand_num_engine;
		
		//! \brief List of buttons used to configure each color
		QList<QToolButton *> buttons;
		
		//! \brief List of colors assinged for each button
		QList<QColor> colors;
		
		//! \brief Palette assinged to buttons when the color picker is disabled
		QColor disable_color;
		
	protected:
		bool eventFilter(QObject *object, QEvent *event);
		
	public:
		static constexpr int MaxColorButtons=20;
		
		explicit ColorPickerWidget(int color_count, QWidget * parent = nullptr);
		
		void setColor(int color_idx, const QColor &color);
		QColor getColor(int color_idx);
		unsigned getColorCount();
		bool isButtonVisible(unsigned idx);
		
		void setButtonToolTip(unsigned button_idx, const QString &tooltip);
		
	public slots:
		void setEnabled(bool value);
		void setButtonVisible(unsigned idx, bool value);
		void selectColor();
		void generateRandomColors();
		
	signals:
		void s_colorChanged(unsigned, QColor);
		void s_colorsChanged();
};

#endif
